
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

template <typename HandleType>
class SharedHandleTraits;

class NoParent;

template <class HandleType>
using parent_of_t = typename HandleType::ParentType;

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

template <typename HandleType, typename HeaderType>
class SharedHandleBase
{
public:
  using ParentType = parent_of_t<HandleType>;

public:
  SharedHandleBase() = default;

  template <typename... Args>
  SharedHandleBase( HandleType handle, Args &&... control_args ) VULKAN_HPP_NOEXCEPT
    : m_handle( handle )
    , m_control( std::make_shared<HeaderType>( std::forward<Args>( control_args )... ) )
  {
  }

  SharedHandleBase( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
    : m_handle( o.m_handle )
    , m_control( o.m_control )
  {
  }

  SharedHandleBase( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
    : m_handle( o.m_handle )
    , m_control( std::move( o.m_control ) )
  {
    o.m_handle = nullptr;
  }

  SharedHandleBase & operator=( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
  {
    reset();
    m_handle   = o.m_handle;
    m_control  = std::move( o.m_control );
    o.m_handle = nullptr;
    return *this;
  }

  SharedHandleBase & operator=( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
  {
    reset();
    m_handle  = o.m_handle;
    m_control = o.m_control;
    return *this;
  }

  ~SharedHandleBase()
  {
    reset();
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
    if ( !m_handle )
      return;

    auto control = std::exchange( m_control, nullptr );
    auto handle  = std::exchange( m_handle, nullptr );

    // only this function owns the last reference to the control block
    if ( control.use_count() == 1 )
      SharedHandle<HandleType>::internalDestroy( *control, handle );
  }

  template <typename T = HandleType>
  typename std::enable_if<has_parent<T>, const SharedHandle<ParentType> &>::type getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent;
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

protected:
  std::shared_ptr<HeaderType> m_control;
  HandleType                  m_handle{};
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
  explicit SharedHandle( HandleType handle, SharedHandle<typename BaseType::ParentType> parent, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
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
#endif