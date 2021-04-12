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
// VulkanHpp Samples : PhysicalDeviceGroups
//                     Get the PhysicalDeviceGroups.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <vector>

static char const * AppName    = "PhysicalDeviceGroups";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::unique_ptr<vk::raii::Context>  context = vk::raii::su::make_unique<vk::raii::Context>();
    std::unique_ptr<vk::raii::Instance> instance =
      vk::raii::su::makeUniqueInstance( *context, AppName, EngineName, {}, {}, VK_API_VERSION_1_1 );
#if !defined( NDEBUG )
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::makeUniqueDebugUtilsMessengerEXT( *instance );
#endif

    /* VULKAN_KEY_START */

    std::vector<vk::PhysicalDeviceGroupProperties> groupProperties = instance->enumeratePhysicalDeviceGroups();

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < groupProperties.size(); i++ )
    {
      std::cout << "Group Properties " << i << " :\n";
      std::cout << "\t"
                << "physicalDeviceCount = " << groupProperties[i].physicalDeviceCount << "\n";
      std::cout << "\t"
                << "physicalDevices:\n";
      for ( size_t j = 0; j < groupProperties[i].physicalDeviceCount; j++ )
      {
        std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::make_unique<vk::raii::PhysicalDevice>(
          static_cast<VkPhysicalDevice>( groupProperties[i].physicalDevices[j] ), instance->getDispatcher() );
        std::cout << "\t\t" << j << " : " << physicalDevice->getProperties().deviceName << "\n";
      }
      std::cout << "\t"
                << "subsetAllocation    = " << !!groupProperties[i].subsetAllocation << "\n";
      std::cout << "\n";

      if ( 1 < groupProperties[i].physicalDeviceCount )
      {
        std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::make_unique<vk::raii::PhysicalDevice>(
          static_cast<VkPhysicalDevice>( groupProperties[i].physicalDevices[0] ), instance->getDispatcher() );

        // get the QueueFamilyProperties of the first PhysicalDevice
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice->getQueueFamilyProperties();

        // get the first index into queueFamiliyProperties which supports graphics
        auto propertyIterator = std::find_if(
          queueFamilyProperties.begin(), queueFamilyProperties.end(), []( vk::QueueFamilyProperties const & qfp ) {
            return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
          } );
        size_t graphicsQueueFamilyIndex = std::distance( queueFamilyProperties.begin(), propertyIterator );
        assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

        // create a Device
        float                     queuePriority = 0.0f;
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo(
          {}, static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
        vk::StructureChain<vk::DeviceCreateInfo, vk::DeviceGroupDeviceCreateInfo> deviceCreateInfoChain(
          { {}, deviceQueueCreateInfo },
          { groupProperties[i].physicalDeviceCount, groupProperties[i].physicalDevices } );

        std::unique_ptr<vk::raii::Device> device =
          vk::raii::su::make_unique<vk::raii::Device>( *physicalDevice, deviceCreateInfoChain.get<vk::DeviceCreateInfo>() );
      }
    }

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
