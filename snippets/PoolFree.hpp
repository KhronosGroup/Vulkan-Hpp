template <typename OwnerType, typename PoolType, typename Dispatch>
class PoolFree
{
public:
  PoolFree() = default;

  PoolFree( OwnerType owner, PoolType pool, Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_owner( owner )
    , m_pool( pool )
    , m_dispatch( &dispatch )
  {
  }

  OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
  {
    return m_owner;
  }

  PoolType getPool() const VULKAN_HPP_NOEXCEPT
  {
    return m_pool;
  }

  Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
  {
    return *m_dispatch;
  }

protected:
  template <typename T>
  void destroy( T t ) VULKAN_HPP_NOEXCEPT
  {
    ( m_owner.free )( m_pool, t, *m_dispatch );
  }

private:
  OwnerType        m_owner    = OwnerType();
  PoolType         m_pool     = PoolType();
  Dispatch const * m_dispatch = nullptr;
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