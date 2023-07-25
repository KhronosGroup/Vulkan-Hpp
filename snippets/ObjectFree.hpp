template <typename OwnerType, typename Dispatch>
class ObjectFree
{
public:
  ObjectFree() = default;

  ObjectFree( OwnerType                                               owner,
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
    ( m_owner.free )( t, m_allocationCallbacks, *m_dispatch );
  }

private:
  OwnerType                           m_owner               = {};
  Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
  Dispatch const *                    m_dispatch            = nullptr;
};

//================================================================================================

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