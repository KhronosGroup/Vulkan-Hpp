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
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::raii::PhysicalDevices physicalDevices( instance );
    assert( !physicalDevices.empty() );

    uint32_t         graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevices[0].getQueueFamilyProperties() );
    vk::raii::Device device                   = vk::raii::su::makeDevice( physicalDevices[0], graphicsQueueFamilyIndex );

    // create an image
    vk::raii::Image image = vk::raii::su::makeImage( device );

    /* VULKAN_KEY_START */

    device.setDebugUtilsObjectNameEXT( *image, "Image name" );

    /* VULKAN_KEY_END */
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
