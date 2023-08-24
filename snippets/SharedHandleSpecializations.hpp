
enum class SwapchainOwns
{
  no,
  yes,
};

struct ImageHeader : SharedHeader<DeleteParentOf<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>
{
  ImageHeader( SharedHandle<DeleteParentOf<VULKAN_HPP_NAMESPACE::Image>>            parent,
               typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter deleter = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter(),
               SwapchainOwns                                                     swapchainOwned = SwapchainOwns::no ) VULKAN_HPP_NOEXCEPT
    : SharedHeader<DeleteParentOf<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>( std::move( parent ),
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

  explicit SharedHandle( VULKAN_HPP_NAMESPACE::Image                            handle,
                         SharedHandle<DeleteParentOf<VULKAN_HPP_NAMESPACE::Image>> parent,
                         SwapchainOwns                                          swapchain_owned = SwapchainOwns::no,
                         DeleterType                                            deleter         = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( parent ), std::move( deleter ), swapchain_owned )
  {
  }

protected:
  static void internalDestroy( const ImageHeader & control, VULKAN_HPP_NAMESPACE::Image handle ) VULKAN_HPP_NOEXCEPT
  {
    if ( control.swapchainOwned == SwapchainOwns::no )
    {
      control.deleter.destroy( control.parent.get(), handle );
    }
  }
};

struct SwapchainHeader
{
  SwapchainHeader( SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface,
                   SharedHandle<DeleteParentOf<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent,
                   typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter =
                     typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter() ) VULKAN_HPP_NOEXCEPT
    : surface( std::move( surface ) )
    , parent( std::move( parent ) )
    , deleter( std::move( deleter ) )
  {
  }

  SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface{};
  SharedHandle<DeleteParentOf<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent{};
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

  explicit SharedHandle( VULKAN_HPP_NAMESPACE::SwapchainKHR                            handle,
                         SharedHandle<DeleteParentOf<VULKAN_HPP_NAMESPACE::SwapchainKHR>> parent,
                         SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                surface,
                         DeleterType                                                   deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( surface ), std::move( parent ), std::move( deleter ) )
  {
  }

public:
  const SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> & getSurface() const VULKAN_HPP_NOEXCEPT
  {
    return getHeader().surface;
  }

protected:
  using BaseType::internalDestroy;
};

template <typename HandleType, typename ParentType>
class SharedHandleBaseNoDestroy : public SharedHandleBase<HandleType, ParentType>
{
  using BaseType = SharedHandleBase<HandleType, ParentType>;

public:
  using BaseType::SharedHandleBase;

  const ParentType & getParent() const VULKAN_HPP_NOEXCEPT
  {
    return BaseType::getHeader();
  }

protected:
  static void internalDestroy( const ParentType &, HandleType ) VULKAN_HPP_NOEXCEPT {}
};