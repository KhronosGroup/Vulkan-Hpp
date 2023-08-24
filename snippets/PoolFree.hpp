  template <typename OwnerType, typename PoolType, typename Dispatch>
  class PoolFree
  {
    public:
      PoolFree() = default;

    PoolFree( OwnerType                 owner,
              PoolType                  pool,
              Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
        : m_owner( owner )
        , m_pool( pool )
        , m_dispatch( &dispatch )
      {}

      OwnerType getOwner() const VULKAN_HPP_NOEXCEPT { return m_owner; }
      PoolType getPool() const VULKAN_HPP_NOEXCEPT { return m_pool; }
      Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT { return *m_dispatch; }

    protected:
      template <typename T>
      void destroy(T t) VULKAN_HPP_NOEXCEPT
      {
        ( m_owner.free )( m_pool, t, *m_dispatch );
      }

    private:
      OwnerType        m_owner    = OwnerType();
      PoolType         m_pool     = PoolType();
      Dispatch const * m_dispatch = nullptr;
  };
