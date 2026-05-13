// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : DeviceExtensionProperties
//                     Get extension properties per physical device.

#include "../utils/utils.hpp"

#include <vector>

static char const * AppName    = "DeviceExtensionProperties";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();
      std::cout << "PhysicalDevice " << i << " : " << extensionProperties.size() << " extensions:\n";

      // sort the extensions alphabetically
      std::sort( extensionProperties.begin(),
                 extensionProperties.end(),
                 []( vk::ExtensionProperties const & a, vk::ExtensionProperties const & b ) { return strcmp( a.extensionName, b.extensionName ) < 0; } );
      for ( auto const & ep : extensionProperties )
      {
        std::cout << "\t" << ep.extensionName << ":" << std::endl;
        std::cout << "\t\tVersion: " << ep.specVersion << std::endl;
        std::cout << std::endl;
      }
    }

    /* VULKAN_KEY_END */

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
