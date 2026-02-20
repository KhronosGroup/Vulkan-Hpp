class Instance;
class Device;
using PFN_dummy = void ( * )();

#if VK_NO_PROTOTYPES
# define VULKAN_HPP_DEFAULT_ADDR( x ) 0
#else
# define VULKAN_HPP_DEFAULT_ADDR( x ) ::x
#endif

class DispatchLoader
{
public:
  DispatchLoader() VULKAN_HPP_NOEXCEPT = default;
  DispatchLoader( DispatchLoader const & rhs ) VULKAN_HPP_NOEXCEPT = default;

  DispatchLoader(PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
  {
    init(getInstanceProcAddr);
  }

  template <typename DynamicLoader
  #if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
    = VULKAN_HPP_NAMESPACE::detail::DynamicLoader
  #endif
  >
  void init()
  {
    static DynamicLoader dl;
    init( dl );
  }

  template <typename DynamicLoader>
  void init( DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
  {
    PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    init( getInstanceProcAddr );
  }

  void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT(getInstanceProcAddr);

    vkGetInstanceProcAddr = getInstanceProcAddr;

    ${initialCommandAssignments}
  }

  void init( VkInstance instance ) VULKAN_HPP_NOEXCEPT
  {
    ${instanceCommandAssignments}
  }

  void init( VkDevice device ) VULKAN_HPP_NOEXCEPT
  {
    ${deviceCommandAssignments}
  }

  void init( Instance instanceCpp ) VULKAN_HPP_NOEXCEPT;
  void init( Device deviceCpp ) VULKAN_HPP_NOEXCEPT;

public:
  ${commandMembers}
};

#undef VULKAN_HPP_DEFAULT_ADDR