
template <typename HandleType>
class SharedHandleTraits;

class NoDestructor
{
};

template <typename HandleType, typename = void>
struct HasDestructorType : std::false_type
{
};

template <typename HandleType>
struct HasDestructorType<HandleType, decltype( (void)typename SharedHandleTraits<HandleType>::DestructorType() )> : std::true_type
{
};

template <typename HandleType, typename Enable = void>
struct GetDestructorType
{
  using type = NoDestructor;
};

template <typename HandleType>
struct GetDestructorType<HandleType, typename std::enable_if<HasDestructorType<HandleType>::value>::type>
{
  using type = typename SharedHandleTraits<HandleType>::DestructorType;
};

template <class HandleType>
using DestructorTypeOf = typename GetDestructorType<HandleType>::type;

template <class HandleType>
struct HasDestructor : std::integral_constant<bool, !std::is_same<DestructorTypeOf<HandleType>, NoDestructor>::value>
{
};

//=====================================================================================================================

template <typename HandleType>
class SharedHandle;

template <typename DestructorType, typename Deleter>
struct SharedHeader
{
  SharedHeader( SharedHandle<DestructorType> parent, Deleter deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
    : parent( std::move( parent ) )
    , deleter( std::move( deleter ) )
  {
  }

  SharedHandle<DestructorType> parent;
  Deleter                      deleter;
};

template <typename Deleter>
struct SharedHeader<NoDestructor, Deleter>
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
  ReferenceCounter( Args &&... control_args ) : m_header( std::forward<Args>( control_args )... )
  {
  }

  ReferenceCounter( const ReferenceCounter & )             = delete;
  ReferenceCounter & operator=( const ReferenceCounter & ) = delete;

public:
  size_t addRef() VULKAN_HPP_NOEXCEPT
  {
    // Relaxed memory order is sufficient since this does not impose any ordering on other operations
    return m_ref_cnt.fetch_add( 1, std::memory_order_relaxed );
  }

  size_t release() VULKAN_HPP_NOEXCEPT
  {
    // A release memory order to ensure that all releases are ordered
    return m_ref_cnt.fetch_sub( 1, std::memory_order_release );
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
    : m_control( o.m_control )
    , m_handle( o.m_handle )
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
      // noop in x86, but does thread synchronization in ARM
      // it is required to ensure that last thread is getting to destroy the control block
      // by ordering all atomic operations before this fence
      std::atomic_thread_fence( std::memory_order_acquire );
      ForwardType::internalDestroy( getHeader(), m_handle );
      delete m_control;
    }
  }

public:
  HandleType get() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }

  HandleType operator*() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }

  explicit operator bool() const VULKAN_HPP_NOEXCEPT
  {
    return bool( m_handle );
  }

#if defined( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
  operator HandleType() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }
#endif

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
  typename std::enable_if<HasDestructor<T>::value, const SharedHandle<DestructorTypeOf<HandleType>> &>::type getDestructorType() const VULKAN_HPP_NOEXCEPT
  {
    return getHeader().parent;
  }

protected:
  template <typename T = HandleType>
  static typename std::enable_if<!HasDestructor<T>::value, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
  {
    control.deleter.destroy( handle );
  }

  template <typename T = HandleType>
  static typename std::enable_if<HasDestructor<T>::value, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
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
class SharedHandle : public SharedHandleBase<HandleType, SharedHeader<DestructorTypeOf<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>
{
private:
  using BaseType    = SharedHandleBase<HandleType, SharedHeader<DestructorTypeOf<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>;
  using DeleterType = typename SharedHandleTraits<HandleType>::deleter;
  friend BaseType;

public:
  SharedHandle() = default;

  template <typename T = HandleType, typename = typename std::enable_if<HasDestructor<T>::value>::type>
  explicit SharedHandle( HandleType handle, SharedHandle<DestructorTypeOf<HandleType>> parent, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( parent ), std::move( deleter ) )
  {
  }

  template <typename T = HandleType, typename = typename std::enable_if<!HasDestructor<T>::value>::type>
  explicit SharedHandle( HandleType handle, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT : BaseType( handle, std::move( deleter ) )
  {
  }

protected:
  using BaseType::internalDestroy;
};
