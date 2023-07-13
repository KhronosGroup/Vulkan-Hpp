#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

template <typename HandleType>
class SharedHandleTraits;

template <typename T, typename...>
struct is_pool_free : std::false_type
{
};

template <typename... Args>
struct is_pool_free<PoolFreeShared<Args...>> : std::true_type
{
};

template <typename T, typename... Args>
VULKAN_HPP_CONSTEXPR VULKAN_HPP_INLINE bool is_pool_free_v = is_pool_free<T, Args...>::value;

//=====================================================================================================================

template <typename Header>
class ControlBlockBase
{
public:
  using header_type = Header;

public:
  ControlBlockBase() = default;

  ControlBlockBase( const ControlBlockBase & o ) : m_control( o.m_control )
  {
    add_ref();
  }

  ControlBlockBase( ControlBlockBase && o ) VULKAN_HPP_NOEXCEPT : m_control( o.m_control )
  {
    o.m_control = nullptr;
  }

  ControlBlockBase & operator=( ControlBlockBase && o ) VULKAN_HPP_NOEXCEPT
  {
    m_control   = o.m_control;
    o.m_control = nullptr;
    return *this;
  }

  ControlBlockBase & operator=( const ControlBlockBase & o ) VULKAN_HPP_NOEXCEPT
  {
    m_control = o.m_control;
    add_ref();
    return *this;
  }

public:
  size_t ref_count() const VULKAN_HPP_NOEXCEPT
  {
    if ( !m_control )
      return 0;
    return m_control->ref_cnt;
  }

  size_t add_ref() VULKAN_HPP_NOEXCEPT
  {
    if ( !m_control )
      return 0;
    return ++m_control->ref_cnt;
  }

  size_t release() VULKAN_HPP_NOEXCEPT
  {
    if ( !m_control )
      return 0;
    auto r = --m_control->ref_cnt;
    if ( !r )
    {
      delete m_control;
      m_control = nullptr;
    }
    return r;
  }

  void allocate()
  {
    m_control = new header_type;
  }

protected:
  header_type * m_control = nullptr;
};

//=====================================================================================================================

class NoParent;

template <typename HandleType>
class SharedHandle;

template <typename ParentType, typename Deleter>
struct SharedHeader
{
  using deleter_type = Deleter;
  SharedHandle<ParentType> parent{};
  std::atomic_size_t       ref_cnt{ 1 };
  deleter_type             deleter{};
};

template <typename Deleter>
struct SharedHeader<NoParent, Deleter>
{
  using deleter_type = Deleter;
  std::atomic_size_t ref_cnt{ 1 };
  deleter_type       deleter{};
};

//=====================================================================================================================

template <typename HandleType>
class BasicControlBlock : public ControlBlockBase<SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>
{
public:
  using parent_type = parent_of_t<HandleType>;
  using base_type   = ControlBlockBase<SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>;
  using base_type::m_control;
  using deleter_type = typename base_type::deleter_type;

public:
  BasicControlBlock() = default;

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  BasicControlBlock( SharedHandle<parent_type> xparent, deleter_type deleter )
  {
    base::allocate();
    m_control->parent  = std::move( xparent );
    m_control->deleter = std::move( deleter );
  }

  template <typename T = HandleType, typename = typename std::enable_if<!has_parent<T>>::type>
  BasicControlBlock( deleter_type deleter )
  {
    base::allocate();
    m_control->deleter = std::move( deleter );
  }

public:
  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  parent_type getParent() const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_control && m_control->parent );
    return m_control->parent.get();
  }

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  SharedHandle<parent_type> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_control && m_control->parent );
    return m_control->parent;
  }
};

template <class HandleType, class ControlBlock = BasicControlBlock<HandleType>>
class SharedHandleBase
{
public:
  using control_block = ControlBlock;
  using parent_type   = parent_of_t<HandleType>;
  using handle_type   = HandleType;

public:
  SharedHandleBase() = default;

  explicit SharedHandleBase( handle_type handle, control_block xcontrol ) VULKAN_HPP_NOEXCEPT
    : m_handle( handle )
    , m_control( std::move( xcontrol ) )
  {
  }

  SharedHandleBase( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
    : m_handle( o.m_handle )
    , m_control( o.m_control )
  {
  }

  SharedHandleBase( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
    : m_handle( o.m_handle )
    , m_control( std::move( o.m_control ) )
  {
    o.m_handle = nullptr;
  }

  SharedHandleBase & operator=( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
  {
    release();
    m_handle   = o.m_handle;
    m_control  = std::move( o.m_control );
    o.m_handle = nullptr;
    return *this;
  }

  SharedHandleBase & operator=( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
  {
    release();
    m_handle  = o.m_handle;
    m_control = o.m_control;
    return *this;
  }

  ~SharedHandleBase()
  {
    release();
  }

public:
  auto get() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }

  explicit operator bool() const VULKAN_HPP_NOEXCEPT
  {
    return bool( m_handle );
  }

  const auto * operator->() const VULKAN_HPP_NOEXCEPT
  {
    return &m_handle;
  }

  auto * operator->() VULKAN_HPP_NOEXCEPT
  {
    return &m_handle;
  }

  size_t add_ref() VULKAN_HPP_NOEXCEPT
  {
    if ( !m_handle )
      return 0;
    return m_control.add_ref();
  }

  size_t release() VULKAN_HPP_NOEXCEPT
  {
    if ( !m_handle )
      return 0;

    auto r = m_control.ref_count();
    if ( r == 1 )
      static_cast<SharedHandle<HandleType> *>( this )->internal_destroy();
    return m_control.release();
  }

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  parent_type getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getParent();
  }

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  SharedHandle<parent_type> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getParentHandle();
  }

protected:
  control_block m_control{};
  handle_type   m_handle{};
};

template <typename HandleType>
class SharedHandle : public SharedHandleBase<HandleType>
{
private:
  using base    = SharedHandleBase<HandleType>;
  using deleter = typename SharedHandleTraits<HandleType>::deleter;
  using base::m_control;
  using base::m_handle;
  friend base;

public:
  using element_type = HandleType;

  SharedHandle() = default;

  template <typename Dispatcher, typename T = HandleType, typename = typename std::enable_if<has_parent<T> && !is_pool_free_v<base::deleter>>::type>
  explicit SharedHandle( HandleType                                              handle,
                         SharedHandle<typename base::parent>                     xparent,
                         Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                         const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : base( handle, { std::move( xparent ), { allocationCallbacks, disp } } )
  {
  }

  // pool
  template <typename Dispatcher,
            typename PoolType,
            typename T = HandleType,
            typename   = typename std::enable_if<has_parent<T> && is_pool_free_v<base::deleter>>::type>
  explicit SharedHandle( HandleType                          handle,
                         SharedHandle<typename base::parent> xparent,
                         PoolType                            pool,
                         const Dispatcher & disp             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : base( handle, { std::move( xparent ), { pool, disp } } )
  {
  }

  template <typename Dispatcher, typename T = HandleType, typename = typename std::enable_if<!has_parent<T>>::type>
  explicit shared_handle( HandleType                                              handle,
                          Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                          const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : base( handle, { { allocationCallbacks, disp } } )
  {
  }

protected:
  template <typename T = HandleType, typename = typename std::enable_if<!has_parent<T>>::type>
  void internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    m_control->deleter.Destroy( m_handle );
    m_handle = nullptr;
  }

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  void internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    m_control->deleter.Destroy( m_control.getParent(), m_handle );
    m_handle = nullptr;
  }
};

struct ImageHeader : SharedHeader<parent_of_t<Image>, typename SharedHandleTraits<Image>::deleter>
{
  bool swapchain_owned = false;
};

struct ImageControlBlock : ControlBlockBase<ImageHeader>
{
  using parent = parent_of_t<Image>;
  using base   = ControlBlockBase<ImageHeader>;
  using base::control;

public:
  ImageControlBlock() = default;

  template <typename Deleter>
  ImageControlBlock( SharedHandle<parent> xparent, Deleter deleter, bool swapchain_owned = false )
  {
    allocate();
    m_control->parent          = std::move( xparent );
    m_control->swapchain_owned = swapchain_owned;
  }

  parent getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent.get();
  }

  SharedHandle<parent> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent;
  }

  bool swapchainOwned() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->swapchain_owned;
  }
};

template <>
class SharedHandle<Image> : public SharedHandleBase<Image, ImageControlBlock>
{
  using base = SharedHandleBase<Image, ImageControlBlock>;
  friend base;

public:
  SharedHandle() = default;

  template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  explicit SharedHandle( vk::Image                                               handle,
                         SharedHandle<Device>                                    xparent,
                         bool                                                    swapchain_owned = false,
                         Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                         const Dispatcher & disp                                 VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
    : base( handle, { std::move( xparent ), { allocationCallbacks, disp }, swapchain_owned } )
  {
  }

protected:
  void internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    if ( !control.swapchainOwned() )
    {
      m_control->deleter.Destroy( m_control.getParent(), m_handle );
    }
    m_handle = nullptr;
  }
};

template <typename SharedType>
VULKAN_HPP_INLINE std::vector<typename SharedType::element_type> sharedToRaw( std::vector<SharedType> const & handles )
{
  std::vector<typename SharedType::element_type> newBuffer( handles.size() );
  std::transform( handles.begin(), handles.end(), newBuffer.begin(), []( SharedType const & handle ) { return handle.get(); } );
  return newBuffer;
}
#endif