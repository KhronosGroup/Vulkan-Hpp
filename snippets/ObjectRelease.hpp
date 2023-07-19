template <typename OwnerType, typename Dispatch>
class ObjectRelease
{
public:
  ObjectRelease() = default;

  ObjectRelease( OwnerType owner, Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
    : m_owner( owner )
    , m_dispatch( &dispatch )
  {
  }

  OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
  {
    return m_owner;
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
    m_owner.release( t, *m_dispatch );
  }

private:
  OwnerType        m_owner    = {};
  Dispatch const * m_dispatch = nullptr;
};

//================================================================================================

template <typename HandleType>
class ObjectReleaseShared
{
public:
  using ParentType = parent_of_t<HandleType>;

  template <class Dispatcher>
  using destroy_pfn_t = void ( ParentType::* )( HandleType kty, const Dispatcher & d ) const;

  template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ObjectReleaseShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::release ) ) ), m_loader( &disp )
  {
  }

public:
  void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_loader );
    ( parent.*m_destroy )( handle, *m_loader );
  }

private:
  destroy_pfn_t<DispatchLoaderBase> m_destroy = nullptr;
  const DispatchLoaderBase *        m_loader  = nullptr;
};