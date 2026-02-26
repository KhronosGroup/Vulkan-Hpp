template <typename OwnerType, typename Dispatch>
class ObjectDestroy
{
public:
  ObjectDestroy() = default;

  ObjectDestroy( OwnerType                           owner,
                 Optional<AllocationCallbacks const> allocationCallbacks VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr ),
                 Dispatch const & dispatch           VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_owner( owner )
    , m_allocationCallbacks( allocationCallbacks )
    , m_dispatch( &dispatch )
  {
  }

  OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
  {
    return m_owner;
  }

  Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
  {
    return m_allocationCallbacks;
  }

  Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
  {
    return *m_dispatch;
  }

protected:
  template <typename T>
  void destroy( T t ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_owner && m_dispatch );
    m_owner.destroy( t, m_allocationCallbacks, *m_dispatch );
  }

private:
  OwnerType                           m_owner               = {};
  Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
  Dispatch const *                    m_dispatch            = nullptr;
};

class NoParent;

template <typename Dispatch>
class ObjectDestroy<NoParent, Dispatch>
{
public:
  ObjectDestroy() = default;

  ObjectDestroy( Optional<AllocationCallbacks const> allocationCallbacks,
                 Dispatch const & dispatch           VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_allocationCallbacks( allocationCallbacks )
    , m_dispatch( &dispatch )
  {
  }

  Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
  {
    return m_allocationCallbacks;
  }

  Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
  {
    return *m_dispatch;
  }

protected:
  template <typename T>
  void destroy( T t ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_dispatch );
    t.destroy( m_allocationCallbacks, *m_dispatch );
  }

private:
  Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
  Dispatch const *                    m_dispatch            = nullptr;
};

template <typename OwnerType, typename Dispatch>
class DummyDestroy
{
public:
  DummyDestroy() = default;

  DummyDestroy( OwnerType                           owner,
                Optional<AllocationCallbacks const> allocationCallbacks VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr ),
                Dispatch const & dispatch           VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_owner( owner )
    , m_allocationCallbacks( allocationCallbacks )
    , m_dispatch( &dispatch )
  {
  }

  OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
  {
    return m_owner;
  }

  Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
  {
    return m_allocationCallbacks;
  }

  Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
  {
    return *m_dispatch;
  }

protected:
  template <typename T>
  void destroy( T /*t*/ ) VULKAN_HPP_NOEXCEPT
  {
  }

private:
  OwnerType                           m_owner               = {};
  Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
  Dispatch const *                    m_dispatch            = nullptr;
};
