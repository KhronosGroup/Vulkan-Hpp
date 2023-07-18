
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

template <typename HandleType>
class SharedHandleTraits;

class NoParent;
class Image;

template <class HandleType>
using parent_of_t = typename SharedHandleTraits<HandleType>::parent;

template <class HandleType>
VULKAN_HPP_CONSTEXPR_INLINE bool has_parent = !std::is_same<parent_of_t<HandleType>, NoParent>::value;

//=====================================================================================================================

template <typename Header>
class ControlBlockBase
{
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
    m_control = new Header;
  }

protected:
  Header * m_control = nullptr;
};

//=====================================================================================================================

class NoParent;

template <typename HandleType>
class SharedHandle;

template <typename ParentType, typename Deleter>
struct SharedHeader
{
  SharedHandle<ParentType> parent{};
  std::atomic_size_t       ref_cnt{ 1 };
  Deleter                  deleter{};
};

template <typename Deleter>
struct SharedHeader<NoParent, Deleter>
{
  std::atomic_size_t ref_cnt{ 1 };
  Deleter            deleter{};
};

//=====================================================================================================================

template <typename HandleType>
class BasicControlBlock : public ControlBlockBase<SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>
{
public:
  using ParentType  = parent_of_t<HandleType>;
  using DeleterType = typename SharedHandleTraits<HandleType>::deleter;
  using BaseType    = ControlBlockBase<SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>;
  using BaseType::m_control;
  using BaseType::allocate;

public:
  BasicControlBlock() = default;

  template <typename std::enable_if<has_parent<HandleType>>::type * = nullptr>
  BasicControlBlock( SharedHandle<ParentType> parent, DeleterType deleter )
  {
    allocate();
    m_control->parent  = std::move( parent );
    m_control->deleter = std::move( deleter );
  }

  template <typename std::enable_if<!has_parent<HandleType>>::type * = nullptr>
  BasicControlBlock( DeleterType deleter )
  {
    allocate();
    m_control->deleter = std::move( deleter );
  }

public:
  typename std::enable_if<has_parent<HandleType>, ParentType>::type getParent() const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_control && m_control->parent );
    return m_control->parent.get();
  }
  
  typename std::enable_if<has_parent<HandleType>, SharedHandle<ParentType>>::type getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( m_control && m_control->parent );
    return m_control->parent;
  }
};

template <class HandleType, class ControlBlock = BasicControlBlock<HandleType>>
class SharedHandleBase
{
public:
  using ParentType = parent_of_t<HandleType>;

public:
  SharedHandleBase() = default;

  explicit SharedHandleBase( HandleType handle, ControlBlock control ) VULKAN_HPP_NOEXCEPT
    : m_handle( handle )
    , m_control( std::move( control ) )
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
  HandleType get() const VULKAN_HPP_NOEXCEPT
  {
    return m_handle;
  }

  explicit operator bool() const VULKAN_HPP_NOEXCEPT
  {
    return bool( m_handle );
  }

  const HandleType * operator->() const VULKAN_HPP_NOEXCEPT
  {
    return &m_handle;
  }

  HandleType * operator->() VULKAN_HPP_NOEXCEPT
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
  ParentType getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getParent();
  }

  template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
  SharedHandle<ParentType> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getParentHandle();
  }

protected:
  ControlBlock m_control{};
  HandleType   m_handle{};
};

template <typename HandleType>
class SharedHandle : public SharedHandleBase<HandleType>
{
private:
  using BaseType    = SharedHandleBase<HandleType>;
  using DeleterType = typename SharedHandleTraits<HandleType>::deleter;
  using BaseType::m_control;
  using BaseType::m_handle;
  friend BaseType;

public:
  using element_type = HandleType;

  SharedHandle() = default;

  template <typename Dispatcher, typename std::enable_if<has_parent<HandleType>>::type * = nullptr>
  explicit SharedHandle( HandleType handle, SharedHandle<typename BaseType::ParentType> parent, DeleterType deleter = DeleterType() )
    : BaseType( handle, { std::move( parent ), std::move( deleter ) } )
  {
  }

  template <typename Dispatcher, typename std::enable_if<!has_parent<HandleType>>::type * = nullptr>
  explicit SharedHandle( HandleType handle, DeleterType deleter = DeleterType() ) : BaseType( handle, std::move( deleter ) )
  {
  }

protected:
  typename std::enable_if<!has_parent<HandleType>, void>::type internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    m_control.m_control->deleter.destroy( m_handle );
    m_handle = nullptr;
  }

  typename std::enable_if<has_parent<HandleType>, void>::type internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    m_control.m_control->deleter.destroy( m_control.getParent(), m_handle );
    m_handle = nullptr;
  }
};

// struct ImageHeader : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>
//{
//   bool swapchainOwned = false;
// };
//
// struct ImageControlBlock : ControlBlockBase<ImageHeader>
//{
//   using ParentType = parent_of_t<VULKAN_HPP_NAMESPACE::Image>;
//   using ControlBlockBase<ImageHeader>::m_control;
//
// public:
//   ImageControlBlock() = default;
//
//   template <typename Deleter>
//   ImageControlBlock( SharedHandle<ParentType> parent, Deleter deleter, bool swapchain_owned = false )
//   {
//     allocate();
//     m_control->parent         = std::move( parent );
//     m_control->swapchainOwned = swapchain_owned;
//   }
//
//   ParentType getParent() const VULKAN_HPP_NOEXCEPT
//   {
//     return m_control->parent.get();
//   }
//
//   SharedHandle<ParentType> getParentHandle() const VULKAN_HPP_NOEXCEPT
//   {
//     return m_control->parent;
//   }
//
//   bool swapchainOwned() const VULKAN_HPP_NOEXCEPT
//   {
//     return m_control->swapchainOwned;
//   }
// };
//
// template <>
// class SharedHandle<VULKAN_HPP_NAMESPACE::Image> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageControlBlock>
//{
//   using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageControlBlock>;
//   using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter;
//   friend BaseType;
//
// public:
//   using element_type = VULKAN_HPP_NAMESPACE::Image;
//
// public:
//   SharedHandle() = default;
//
//   template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
//   explicit SharedHandle( VULKAN_HPP_NAMESPACE::Image handle, SharedHandle<Device> parent, bool swapchain_owned = false, DeleterType deleter = DeleterType() )
//     : BaseType( handle, { std::move( parent ), std::move( deleter ), swapchain_owned } )
//   {
//   }
//
// protected:
//   void internal_destroy() VULKAN_HPP_NOEXCEPT
//   {
//     if ( !control.swapchainOwned() )
//     {
//       m_control.m_control->deleter.destroy( m_control.getParent(), m_handle );
//     }
//     m_handle = nullptr;
//   }
// };

template <typename SharedType>
VULKAN_HPP_INLINE std::vector<typename SharedType::element_type> sharedToRaw( std::vector<SharedType> const & handles )
{
  std::vector<typename SharedType::element_type> newBuffer( handles.size() );
  std::transform( handles.begin(), handles.end(), newBuffer.begin(), []( SharedType const & handle ) { return handle.get(); } );
  return newBuffer;
}
#endif