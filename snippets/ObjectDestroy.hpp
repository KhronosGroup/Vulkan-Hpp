struct AllocationCallbacks;

template <typename OwnerType, typename Dispatch>
class ObjectDestroy
{
public:
  ObjectDestroy() = default;

  ObjectDestroy( OwnerType                                               owner,
                 Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                 Dispatch const & dispatch                               VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_owner( owner )
    , m_allocationCallbacks( allocationCallbacks )
    , m_dispatch( &dispatch )
  {
  }

  OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
  {
    return m_owner;
  }

  Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
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
  {
  }

  Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
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
  Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
  Dispatch const *                    m_dispatch            = nullptr;
};

//================================================================================================

template <typename HandleType>
class ObjectDestroyShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using destroy_pfn_t = typename std::conditional<has_parent<HandleType>,

                                                  void ( ParentType::* )( HandleType kty, const AllocationCallbacks * pAllocator, const Dispatcher & d ) const,

                                                  void ( HandleType::* )( const AllocationCallbacks * pAllocator, const Dispatcher & d ) const>::type;
  using selector_t = typename std::conditional<has_parent<HandleType>, ParentType, HandleType>::type;

  template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectDestroyShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &selector_t::destroy ) ) )
    , m_loader( &disp )
    , m_allocationCallbacks( allocationCallbacks )
  {
  }

public:
  template <typename T = HandleType>
  typename std::enable_if<has_parent<T>, void>::type destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_loader );
    ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_loader );
  }

  template <typename T = HandleType>
  typename std::enable_if<!has_parent<T>, void>::type destroy( HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_loader );
    ( handle.*m_destroy )( m_allocationCallbacks, *m_loader );
  }

private:
  destroy_pfn_t<DispatchLoaderBase>   m_destroy             = nullptr;
  const DispatchLoaderBase *          m_loader              = nullptr;
  Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
};