// SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : DebugUtilsObjectName
//                     Demonstrate usage of DebugUtilsObjectName

#include "../utils/utils.hpp"

static char const * AppName    = "DebugUtilsObjectName";
static char const * EngineName = "Vulkan.hpp";

#if defined( _MSC_VER ) && !defined( _WIN64 )
#  define NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( type, x ) static_cast<type>( x )
#else
#  define NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( type, x ) reinterpret_cast<uint64_t>( static_cast<type>( x ) )
#endif

int main()
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    assert( !physicalDevices.empty() );

    uint32_t   graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevices[0].getQueueFamilyProperties() );
    vk::Device device                   = vk::su::createDevice( physicalDevices[0], graphicsQueueFamilyIndex );

    // create an image
    vk::ImageCreateInfo imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         vk::Format::eB8G8R8A8Unorm,
                                         vk::Extent3D( 640, 640, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         vk::ImageTiling::eLinear,
                                         vk::ImageUsageFlagBits::eTransferSrc );
    vk::Image           image = device.createImage( imageCreateInfo );

    /* VULKAN_KEY_START */

    device.setDebugUtilsObjectNameEXT( image, "Image name" );

    /* VULKAN_KEY_END */

    device.destroyImage( image );
    device.destroy();
#if !defined( NDEBUG )
    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
    instance.destroy();
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }
  return 0;
}
