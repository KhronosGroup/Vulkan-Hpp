
template <typename HandleType>
class ObjectDestroyShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using destroy_pfn_t = typename std::conditional<has_parent<HandleType>,

                                                  void ( ParentType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const,

                                                  void ( HandleType::* )( const AllocationCallbacks *, const Dispatcher & ) const>::type;
  using selector_t    = typename std::conditional<has_parent<HandleType>, ParentType, HandleType>::type;

  template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectDestroyShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &selector_t::destroy ) ) )
    , m_dispatch( &dispatch )
    , m_allocationCallbacks( allocationCallbacks )
  {
  }

public:
  template <typename T = HandleType>
  typename std::enable_if<has_parent<T>, void>::type destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
  }

  template <typename T = HandleType>
  typename std::enable_if<!has_parent<T>, void>::type destroy( HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( handle.*m_destroy )( m_allocationCallbacks, *m_dispatch );
  }

private:
  destroy_pfn_t<DispatchLoaderBase>   m_destroy             = nullptr;
  const DispatchLoaderBase *          m_dispatch            = nullptr;
  Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
};

template <typename HandleType>
class ObjectFreeShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using destroy_pfn_t = void ( ParentType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectFreeShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                    const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::free ) ) )
    , m_dispatch( &dispatch )
    , m_allocationCallbacks( allocationCallbacks )
  {
  }

public:
  void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
  }

private:
  destroy_pfn_t<DispatchLoaderBase>   m_destroy             = nullptr;
  const DispatchLoaderBase *          m_dispatch            = nullptr;
  Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
};

template <typename HandleType>
class ObjectReleaseShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using destroy_pfn_t = void ( ParentType::* )( HandleType, const Dispatcher & ) const;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectReleaseShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::release ) ) ), m_dispatch( &dispatch )
  {
  }

public:
  void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( handle, *m_dispatch );
  }

private:
  destroy_pfn_t<DispatchLoaderBase> m_destroy  = nullptr;
  const DispatchLoaderBase *        m_dispatch = nullptr;
};

template <typename HandleType, typename PoolType>
class PoolFreeShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using ret_t = decltype( std::declval<ParentType>().free( PoolType(), 0u, nullptr, Dispatcher() ) );

  template <class Dispatcher>
  using destroy_pfn_t = ret_t<Dispatcher> ( ParentType::* )( PoolType, uint32_t, const HandleType *, const Dispatcher & ) const;

  PoolFreeShared() = default;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  PoolFreeShared( SharedHandle<PoolType> pool, const Dispatcher & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::free ) ) )
    , m_pool( std::move( pool ) )
    , m_dispatch( &dispatch )
  {
  }

public:
  void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
    ( parent.*m_destroy )( m_pool.get(), 1u, &handle, *m_dispatch );
  }

private:
  destroy_pfn_t<DispatchLoaderBase> m_destroy  = nullptr;
  const DispatchLoaderBase *        m_dispatch = nullptr;
  SharedHandle<PoolType>            m_pool{};
};