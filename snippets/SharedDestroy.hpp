
template <typename HandleType>
class SharedHandleTraits;

// Silence the function cast warnings.
#if defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __INTEL_COMPILER )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

template <typename HandleType>
class ObjectDestroyShared
{
public:
  using DestructorType = typename SharedHandleTraits<HandleType>::DestructorType;

  template <class Dispatcher>
  using DestroyFunctionPointerType = typename std::conditional<HasDestructor<HandleType>::value,
                                                               void ( DestructorType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const,
                                                               void ( HandleType::* )( const AllocationCallbacks *, const Dispatcher & ) const>::type;

  using SelectorType = typename std::conditional<HasDestructor<HandleType>::value, DestructorType, HandleType>::type;

  template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectDestroyShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<DestroyFunctionPointerType<Dispatcher>>( &SelectorType::destroy ) ) )
    , m_dispatch( &dispatch )
    , m_allocationCallbacks( allocationCallbacks )
  {
  }

public:
  template <typename T = HandleType>
  typename std::enable_if<HasDestructor<T>::value, void>::type destroy( DestructorType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
  }

  template <typename T = HandleType>
  typename std::enable_if<!HasDestructor<T>::value, void>::type destroy( HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( handle.*m_destroy )( m_allocationCallbacks, *m_dispatch );
  }

private:
  DestroyFunctionPointerType<DispatchLoaderBase> m_destroy             = nullptr;
  const DispatchLoaderBase *                     m_dispatch            = nullptr;
  Optional<const AllocationCallbacks>            m_allocationCallbacks = nullptr;
};

template <typename HandleType>
class ObjectFreeShared
{
public:
  using DestructorType = typename SharedHandleTraits<HandleType>::DestructorType;

  template <class Dispatcher>
  using DestroyFunctionPointerType = void ( DestructorType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectFreeShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                    const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<DestroyFunctionPointerType<Dispatcher>>( &DestructorType::free ) ) )
    , m_dispatch( &dispatch )
    , m_allocationCallbacks( allocationCallbacks )
  {
  }

public:
  void destroy( DestructorType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
  }

private:
  DestroyFunctionPointerType<DispatchLoaderBase> m_destroy             = nullptr;
  const DispatchLoaderBase *                     m_dispatch            = nullptr;
  Optional<const AllocationCallbacks>            m_allocationCallbacks = nullptr;
};

template <typename HandleType>
class ObjectReleaseShared
{
public:
  using DestructorType = typename SharedHandleTraits<HandleType>::DestructorType;

  template <class Dispatcher>
  using DestroyFunctionPointerType = void ( DestructorType::* )( HandleType, const Dispatcher & ) const;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectReleaseShared( const Dispatcher & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<DestroyFunctionPointerType<Dispatcher>>( &DestructorType::release ) ) )
    , m_dispatch( &dispatch )
  {
  }

public:
  void destroy( DestructorType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, *m_dispatch );
  }

private:
  DestroyFunctionPointerType<DispatchLoaderBase> m_destroy  = nullptr;
  const DispatchLoaderBase *                     m_dispatch = nullptr;
};

template <typename HandleType, typename PoolType>
class PoolFreeShared
{
public:
  using DestructorType = typename SharedHandleTraits<HandleType>::DestructorType;

  template <class Dispatcher>
  using ReturnType = decltype( std::declval<DestructorType>().free( PoolType(), 0u, nullptr, Dispatcher() ) );

  template <class Dispatcher>
  using DestroyFunctionPointerType = ReturnType<Dispatcher> ( DestructorType::* )( PoolType, uint32_t, const HandleType *, const Dispatcher & ) const;

  PoolFreeShared() = default;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  PoolFreeShared( SharedHandle<PoolType> pool, const Dispatcher & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<DestroyFunctionPointerType<Dispatcher>>( &DestructorType::free ) ) )
    , m_dispatch( &dispatch )
    , m_pool( std::move( pool ) )
  {
  }

public:
  void destroy( DestructorType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( m_pool.get(), 1u, &handle, *m_dispatch );
  }

private:
  DestroyFunctionPointerType<DispatchLoaderBase> m_destroy  = nullptr;
  const DispatchLoaderBase *                     m_dispatch = nullptr;
  SharedHandle<PoolType>                         m_pool{};
};

#if defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __INTEL_COMPILER )
#  pragma GCC diagnostic pop
#endif
