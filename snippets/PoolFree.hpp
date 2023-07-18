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
  using destroy_pfn_t = void ( ParentType::* )( PoolType pool, HandleType kty, const Dispatcher & d ) const;

  PoolFreeShared() = default;

  template <class Dispatcher>
  PoolFreeShared( PoolType pool, const Dispatcher & disp VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::free ) ) ), m_pool( pool ), m_loader( &disp )
  {
  }

public:
  void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_loader );
    ( parent.*m_destroy )( m_pool, handle, *m_loader );
  }

private:
  destroy_pfn_t<DispatchLoaderBase> m_destroy = nullptr;
  PoolType                          m_pool    = PoolType();
  const DispatchLoaderBase *        m_loader  = nullptr;
};