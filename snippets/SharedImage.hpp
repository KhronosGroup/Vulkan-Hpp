
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
struct ImageHeader : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>
{
  bool swapchainOwned = false;
};

struct ImageControlBlock : ControlBlockBase<ImageHeader>
{
  using ParentType = parent_of_t<VULKAN_HPP_NAMESPACE::Image>;
  using ControlBlockBase<ImageHeader>::m_control;
  using ControlBlockBase<ImageHeader>::allocate;

public:
  ImageControlBlock() = default;

  template <typename Deleter>
  ImageControlBlock( SharedHandle<ParentType> parent, Deleter deleter, bool swapchain_owned = false )
  {
    allocate();
    m_control->parent         = std::move( parent );
    m_control->deleter        = std::move( deleter );
    m_control->swapchainOwned = swapchain_owned;
  }

  ParentType getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent.get();
  }

  SharedHandle<ParentType> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent;
  }

  bool swapchainOwned() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->swapchainOwned;
  }
};

template <>
class SharedHandle<VULKAN_HPP_NAMESPACE::Image> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageControlBlock>
{
  using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageControlBlock>;
  using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter;
  friend BaseType;

public:
  using element_type = VULKAN_HPP_NAMESPACE::Image;

public:
  SharedHandle() = default;

  explicit SharedHandle( VULKAN_HPP_NAMESPACE::Image                 handle,
                         SharedHandle<typename BaseType::ParentType> parent,
                         bool                                        swapchain_owned = false,
                         DeleterType                                 deleter         = DeleterType() )
    : BaseType( handle, { std::move( parent ), std::move( deleter ), swapchain_owned } )
  {
  }

protected:
  void internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    if ( !m_control.swapchainOwned() )
    {
      m_control.m_control->deleter.destroy( m_control.getParent(), m_handle );
    }
    m_handle = nullptr;
  }
};

struct SwapchainHeader
{
  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface;
  SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent{};
  std::atomic_size_t                                                       ref_cnt{ 1 };
  typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter{};
};

struct SwapchainControlBlock : ControlBlockBase<SwapchainHeader>
{
  using ParentType = parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>;
  using ControlBlockBase<SwapchainHeader>::m_control;
  using ControlBlockBase<SwapchainHeader>::allocate;

public:
  SwapchainControlBlock() = default;

  template <typename Deleter>
  SwapchainControlBlock( SharedHandle<ParentType> parent, SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> surface, Deleter deleter )
  {
    allocate();
    m_control->parent  = std::move( parent );
    m_control->surface = std::move( surface );
    m_control->deleter = std::move( deleter );
  }

  ParentType getParent() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent.get();
  }

  SharedHandle<ParentType> getParentHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->parent;
  }

  VULKAN_HPP_NAMESPACE::SurfaceKHR getSurface() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->surface.get();
  }

  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> getSurfaceHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->surface;
  }
};

template <>
class SharedHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainControlBlock>
{
  using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainControlBlock>;
  using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter;
  friend BaseType;

public:
  using element_type = VULKAN_HPP_NAMESPACE::SwapchainKHR;

public:
  SharedHandle() = default;

  explicit SharedHandle( VULKAN_HPP_NAMESPACE::SwapchainKHR             handle,
                         SharedHandle<typename BaseType::ParentType>    parent,
                         SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> surface,
                         DeleterType                                    deleter = DeleterType() )
    : BaseType( handle, { std::move( parent ), std::move( surface ), std::move( deleter ) } )
  {
  }

public:
  VULKAN_HPP_NAMESPACE::SurfaceKHR getSurface() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getSurface();
  }

  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> getSurfaceHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control.getSurfaceHandle();
  }

protected:
  void internal_destroy() VULKAN_HPP_NOEXCEPT
  {
    m_control.m_control->deleter.destroy( m_control.getParent(), m_handle );
    m_handle = nullptr;
  }
};
#endif