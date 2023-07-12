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
  using handle_type = HandleType;
  using parent      = parent_of_t<handle_type>;

  template <class Dispatcher>
  using destroy_pfn_t = void ( parent::* )( handle_type kty, const Dispatcher & d ) const VULKAN_HPP_NOEXCEPT > ;

  ObjectReleaseShared() = default;

  template <class Dispatcher>
  ObjectReleaseShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                       const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : m_destroy( reinterpret_cast<decltype( destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &parent::release ) ) ), m_loader( &disp )
  {
  }

protected:
  void Destroy( parent parent, handle_type handle ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_destroy && m_loader );
    ( parent.*m_destroy )( handle, *m_loader );
  }

private:
  destroy_pfn_t<DispatchLoaderBase> m_destroy = nullptr;
  const DispatchLoaderBase *        m_loader  = nullptr;
};