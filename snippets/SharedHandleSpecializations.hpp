
enum class SwapchainOwns
{
  no,
  yes,
};

struct ImageHeader : SharedHeader<DestructorTypeOf<Image>, typename SharedHandleTraits<Image>::deleter>
{
  ImageHeader( SharedHandle<DestructorTypeOf<Image>>       parent,
               typename SharedHandleTraits<Image>::deleter deleter        = typename SharedHandleTraits<Image>::deleter(),
               SwapchainOwns                               swapchainOwned = SwapchainOwns::no ) VULKAN_HPP_NOEXCEPT
    : SharedHeader<DestructorTypeOf<Image>, typename SharedHandleTraits<Image>::deleter>( std::move( parent ),
                                                                                                                                      std::move( deleter ) )
    , swapchainOwned( swapchainOwned )
  {
  }

  SwapchainOwns swapchainOwned = SwapchainOwns::no;
};

template <>
class SharedHandle<Image> : public SharedHandleBase<Image, ImageHeader>
{
  using BaseType    = SharedHandleBase<Image, ImageHeader>;
  using DeleterType = typename SharedHandleTraits<Image>::deleter;
  friend BaseType;

public:
  SharedHandle() = default;

  explicit SharedHandle( Image                                 handle,
                         SharedHandle<DestructorTypeOf<Image>> parent,
                         SwapchainOwns                         swapchain_owned = SwapchainOwns::no,
                         DeleterType                           deleter         = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( parent ), std::move( deleter ), swapchain_owned )
  {
  }

protected:
  static void internalDestroy( const ImageHeader & control, Image handle ) VULKAN_HPP_NOEXCEPT
  {
    if ( control.swapchainOwned == SwapchainOwns::no )
    {
      control.deleter.destroy( control.parent.get(), handle );
    }
  }
};

struct SwapchainHeader
{
  SwapchainHeader( SharedHandle<SurfaceKHR>                           surface,
                   SharedHandle<DestructorTypeOf<SwapchainKHR>>       parent,
                   typename SharedHandleTraits<SwapchainKHR>::deleter deleter =
                     typename SharedHandleTraits<SwapchainKHR>::deleter() ) VULKAN_HPP_NOEXCEPT
    : surface( std::move( surface ) )
    , parent( std::move( parent ) )
    , deleter( std::move( deleter ) )
  {
  }

  SharedHandle<SurfaceKHR>                           surface{};
  SharedHandle<DestructorTypeOf<SwapchainKHR>>       parent{};
  typename SharedHandleTraits<SwapchainKHR>::deleter deleter{};
};

template <>
class SharedHandle<SwapchainKHR> : public SharedHandleBase<SwapchainKHR, SwapchainHeader>
{
  using BaseType    = SharedHandleBase<SwapchainKHR, SwapchainHeader>;
  using DeleterType = typename SharedHandleTraits<SwapchainKHR>::deleter;
  friend BaseType;

public:
  SharedHandle() = default;

  explicit SharedHandle( SwapchainKHR                                 handle,
                         SharedHandle<DestructorTypeOf<SwapchainKHR>> parent,
                         SharedHandle<SurfaceKHR>                     surface,
                         DeleterType                                  deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
    : BaseType( handle, std::move( surface ), std::move( parent ), std::move( deleter ) )
  {
  }

public:
  const SharedHandle<SurfaceKHR> & getSurface() const VULKAN_HPP_NOEXCEPT
  {
    return getHeader().surface;
  }

protected:
  using BaseType::internalDestroy;
};

template <typename HandleType, typename DestructorType>
class SharedHandleBaseNoDestroy : public SharedHandleBase<HandleType, DestructorType>
{
public:
  using SharedHandleBase<HandleType, DestructorType>::SharedHandleBase;

  const DestructorType & getDestructorType() const VULKAN_HPP_NOEXCEPT
  {
    return SharedHandleBase<HandleType, DestructorType>::getHeader();
  }

protected:
  static void internalDestroy( const DestructorType &, HandleType ) VULKAN_HPP_NOEXCEPT {}
};