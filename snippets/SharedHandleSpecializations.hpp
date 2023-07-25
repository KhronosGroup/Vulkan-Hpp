
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
enum class SwapchainOwns
{
  no,
  yes,
};

struct ImageHeader : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>
{
  ImageHeader( SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::Image>>            parent,
               typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter deleter = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter(),
               SwapchainOwns                                                     swapchainOwned = SwapchainOwns::no )
    : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>( std::move( parent ),
                                                                                                                                 std::move( deleter ) )
    , swapchainOwned( swapchainOwned )
  {
  }

  SwapchainOwns swapchainOwned = SwapchainOwns::no;
};

template <>
class SharedHandle<VULKAN_HPP_NAMESPACE::Image> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageHeader>
{
  using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageHeader>;
  using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter;
  friend BaseType;

public:
  using element_type = VULKAN_HPP_NAMESPACE::Image;

public:
  SharedHandle() = default;

  explicit SharedHandle( VULKAN_HPP_NAMESPACE::Image                 handle,
                         SharedHandle<typename BaseType::ParentType> parent,
                         SwapchainOwns                               swapchain_owned = SwapchainOwns::no,
                         DeleterType                                 deleter         = DeleterType() )
    : BaseType( handle, std::move( parent ), std::move( deleter ), swapchain_owned )
  {
  }

  VULKAN_HPP_NAMESPACE::Image & put( SharedHandle<typename BaseType::ParentType> parent,
                                     SwapchainOwns                               swapchain_owned = SwapchainOwns::no,
                                     DeleterType                                 deleter         = DeleterType() ) VULKAN_HPP_NOEXCEPT
  {
    return BaseType::put( std::move( parent ), std::move( deleter ), swapchain_owned );
  }

protected:
  void internalDestroy() VULKAN_HPP_NOEXCEPT
  {
    if ( m_control->swapchainOwned == SwapchainOwns::no )
    {
      m_control->deleter.destroy( getParent(), m_handle );
    }
  }
};

struct SwapchainHeader
{
  SwapchainHeader( SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface,
                   SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent,
                   typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter =
                     typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter() )
    : surface( std::move( surface ) ), parent( std::move( parent ) ), deleter( std::move( deleter ) )
  {
  }

  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface{};
  SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent{};
  typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter{};
};

template <>
class SharedHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainHeader>
{
  using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainHeader>;
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
    : BaseType( handle, std::move( surface ), std::move( parent ), std::move( deleter ) )
  {
  }

public:
  VULKAN_HPP_NAMESPACE::SurfaceKHR getSurface() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->surface.get();
  }

  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> getSurfaceHandle() const VULKAN_HPP_NOEXCEPT
  {
    return m_control->surface;
  }

  VULKAN_HPP_NAMESPACE::SwapchainKHR & put( SharedHandle<typename BaseType::ParentType>    parent,
                                            SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> surface,
                                            DeleterType                                    deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
  {
    return BaseType::put( std::move( surface ), std::move( parent ), std::move( deleter ) );
  }

protected:
  using BaseType::internalDestroy;
};
#endif