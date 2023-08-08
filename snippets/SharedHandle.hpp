
template <typename HandleType>
class SharedHandleTraits;

class NoParent;

template <typename HandleType, typename = void>
struct HasParentType : std::false_type
{
};

template <typename HandleType>
struct HasParentType<HandleType, decltype( (void)typename HandleType::ParentType() )> : std::true_type
{
};

template <typename HandleType, typename Enable = void>
struct GetParentType
{
  using type = NoParent;
};

template <typename HandleType>
struct GetParentType<HandleType, typename std::enable_if<HasParentType<HandleType>::value>::type>
{
  using type = typename HandleType::ParentType;
};

template <class HandleType>
using parent_of_t = typename GetParentType<HandleType>::type;

template <class HandleType>
VULKAN_HPP_CONSTEXPR_INLINE bool has_parent = !std::is_same<parent_of_t<HandleType>, NoParent>::value;

//=====================================================================================================================

template <typename HandleType>
class SharedHandle;

template <typename ParentType, typename Deleter>
struct SharedHeader
{
  SharedHeader( SharedHandle<ParentType> parent, Deleter deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
    : parent( std::move( parent ) )
    , deleter( std::move( deleter ) )
  {
  }

  SharedHandle<ParentType> parent{};
  Deleter                  deleter;
};

template <typename Deleter>
struct SharedHeader<NoParent, Deleter>
{
  SharedHeader( Deleter deleter = Deleter() ) VULKAN_HPP_NOEXCEPT : deleter( std::move( deleter ) ) {}

  Deleter deleter;
};

//=====================================================================================================================

template <typename HeaderType>
class ReferenceCounter
{
public:
  template <typename... Args>
  ReferenceCounter( Args &&... control_args ) : m_header( std::forward<Args>( control_args )... ){};
  ReferenceCounter( const ReferenceCounter & )             = delete;
  ReferenceCounter & operator=( const ReferenceCounter & ) = delete;

public:
  size_t addRef() VULKAN_HPP_NOEXCEPT
  {
    return m_ref_cnt.fetch_add( 1, std::memory_order_relaxed );
  }

  size_t release() VULKAN_HPP_NOEXCEPT
  {
    return m_ref_cnt.fetch_sub( 1, std::memory_order_acq_rel );
  }

public:
  std::atomic_size_t m_ref_cnt{ 1 };
  HeaderType         m_header{};
};

//=====================================================================================================================

template <typename HandleType, typename HeaderType, typename ForwardType = SharedHandle<HandleType>>
class SharedHandleBase
{
public:
  SharedHandleBase() = default;

  template <typename... Args>
  SharedHandleBase( HandleType handle, Args &&... control_args )
    : m_control( new ReferenceCounter<HeaderType>( std::forward<Args>( control_args )... ) ), m_handle( handle )
  {
  }

  SharedHandleBase( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
  {
    o.addRef();
    m_handle  = o.m_handle;
    m_control = o.m_control;
  }

  SharedHandleBase( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
    : m_handle( o.m_handle )
    , m_control( o.m_control )
  {
    o.m_handle  = nullptr;
    o.m_control = nullptr;
  }

  SharedHandleBase & operator=( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
  {
    SharedHandleBase( o ).swap( *this );
    return *this;
  }

  SharedHandleBase & operator=( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
  {
    SharedHandleBase( std::move( o ) ).swap( *this );
    return *this;
  }

  ~SharedHandleBase()
  {
    // only this function owns the last reference to the control block
    // the same principle is used in the default deleter of std::shared_ptr
    if ( m_control && ( m_control->release() == 1 ) )
    {
      ForwardType::internalDestroy( getHeader(), m_handle );
      delete m_control;
    }
  }

public:
  HandleType get() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }

  explicit operator bool() const VULKAN_HPP_NOEXCEPT
  {
    return bool( m_handle );
  }

  const HandleType * operator->() const VULKAN_HPP_NOEXCEPT
  {
    return &m_handle;
  }

  HandleType * operator->() VULKAN_HPP_NOEXCEPT
  {
    return &m_handle;
  }

  void reset() VULKAN_HPP_NOEXCEPT
  {
    SharedHandleBase().swap( *this );
  }

  void swap( SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
  {
    std::swap( m_handle, o.m_handle );
    std::swap( m_control, o.m_control );
  }

  template <typename T = HandleType>
  typename std::enable_if<has_parent<T>, const SharedHandle<parent_of_t<HandleType>> &>::type getParent() const VULKAN_HPP_NOEXCEPT
  {
    return getHeader().parent;
  }

protected:
  template <typename T = HandleType>
  static typename std::enable_if<!has_parent<T>, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
  {
    control.deleter.destroy( handle );
  }

  template <typename T = HandleType>
  static typename std::enable_if<has_parent<T>, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
  {
    control.deleter.destroy( control.parent.get(), handle );
  }

  const HeaderType & getHeader() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->m_header;
  }

private:
  void addRef() const VULKAN_HPP_NOEXCEPT
  {
    if ( m_control )
      m_control->addRef();
  }

protected:
  ReferenceCounter<HeaderType> * m_control = nullptr;
  HandleType                     m_handle{};
};

template <typename HandleType>
class SharedHandle : public SharedHandleBase<HandleType, SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>
{
private:
  using BaseType    = SharedHandleBase<HandleType, SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>;
  using DeleterType = typename SharedHandleTraits<HandleType>::deleter;
  friend BaseType;

public:
  using element_type = HandleType;

  SharedHandle() = default;

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  explicit SharedHandle( HandleType handle, SharedHandle<parent_of_t<HandleType>> parent, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( parent ), std::move( deleter ) )
  {
  }

  template <typename T = HandleType, typename = typename std::enable_if<!has_parent<T>>::type>
  explicit SharedHandle( HandleType handle, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT : BaseType( handle, std::move( deleter ) )
  {
  }

protected:
  using BaseType::internalDestroy;
};

template <typename SharedType>
VULKAN_HPP_INLINE std::vector<typename SharedType::element_type> sharedToRaw( std::vector<SharedType> const & handles )
{
  std::vector<typename SharedType::element_type> newBuffer( handles.size() );
  std::transform( handles.begin(), handles.end(), newBuffer.begin(), []( SharedType const & handle ) { return handle.get(); } );
  return newBuffer;
}