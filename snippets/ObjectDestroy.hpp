  struct AllocationCallbacks;

  template <typename OwnerType, typename Dispatch>
  class ObjectDestroy
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( OwnerType owner,
                   Optional<const AllocationCallbacks> allocationCallbacks
                                             VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                   Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {}

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT { return m_owner; }
    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT { return m_allocationCallbacks; }
    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT { return *m_dispatch; }

  protected:
    template <typename T>
    void destroy(T t) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.destroy( t, m_allocationCallbacks, *m_dispatch );
    }

  private:
    OwnerType                           m_owner               = {};
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };

  class NoParent;

  template <typename Dispatch>
  class ObjectDestroy<NoParent, Dispatch>
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( Optional<const AllocationCallbacks> allocationCallbacks,
                   Dispatch const & dispatch           VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {}

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT { return m_allocationCallbacks; }
    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT { return *m_dispatch; }

  protected:
    template <typename T>
    void destroy(T t) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_dispatch );
      t.destroy( m_allocationCallbacks, *m_dispatch );
    }

  private:
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };
