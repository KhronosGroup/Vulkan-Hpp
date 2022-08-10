// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// VulkanHpp Samples : DeviceExtensionProperties
//                     Get extension properties per physical device.

#include "../utils/utils.hpp"

#include <vector>

static char const * AppName    = "DeviceExtensionProperties";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
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
