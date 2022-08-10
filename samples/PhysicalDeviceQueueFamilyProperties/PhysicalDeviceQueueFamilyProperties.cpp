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
// VulkanHpp Samples : PhysicalDeviceQueueFamilyProperties
//                     Get queue family properties per physical device.

#include "../utils/utils.hpp"

#include <iomanip>
#include <sstream>
#include <vector>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "PhysicalDeviceQueueFamilyProperties";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, {}, VK_API_VERSION_1_1 );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      // some features are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";

      // need to explicitly specify all the template arguments for getQueueFamilyProperties2 to make the compiler happy
      using Chain                 = vk::StructureChain<vk::QueueFamilyProperties2, vk::QueueFamilyCheckpointPropertiesNV>;
      auto queueFamilyProperties2 = physicalDevices[i].getQueueFamilyProperties2<Chain, std::allocator<Chain>, vk::DispatchLoaderDynamic>();
      for ( size_t j = 0; j < queueFamilyProperties2.size(); j++ )
      {
        std::cout << std::string( "\t" ) << "QueueFamily " << j << "\n";
        vk::QueueFamilyProperties const & properties = queueFamilyProperties2[j].get<vk::QueueFamilyProperties2>().queueFamilyProperties;
        std::cout << std::string( "\t\t" ) << "QueueFamilyProperties:\n";
        std::cout << std::string( "\t\t\t" ) << "queueFlags                  = " << vk::to_string( properties.queueFlags ) << "\n";
        std::cout << std::string( "\t\t\t" ) << "queueCount                  = " << properties.queueCount << "\n";
        std::cout << std::string( "\t\t\t" ) << "timestampValidBits          = " << properties.timestampValidBits << "\n";
        std::cout << std::string( "\t\t\t" ) << "minImageTransferGranularity = " << properties.minImageTransferGranularity.width << " x "
                  << properties.minImageTransferGranularity.height << " x " << properties.minImageTransferGranularity.depth << "\n";
        std::cout << "\n";

        if ( vk::su::contains( extensionProperties, "VK_NV_device_diagnostic_checkpoints" ) )
        {
          vk::QueueFamilyCheckpointPropertiesNV const & checkpointProperties = queueFamilyProperties2[j].get<vk::QueueFamilyCheckpointPropertiesNV>();
          std::cout << std::string( "\t\t" ) << "CheckPointPropertiesNV:\n";
          std::cout << std::string( "\t\t\t" ) << "checkpointExecutionStageMask  = " << vk::to_string( checkpointProperties.checkpointExecutionStageMask )
                    << "\n";
          std::cout << "\n";
        }
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
