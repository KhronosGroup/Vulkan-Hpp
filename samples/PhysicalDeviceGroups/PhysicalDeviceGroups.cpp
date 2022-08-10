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

#include <vector>

static char const * AppName    = "PhysicalDeviceGroups";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, {}, VK_API_VERSION_1_1 );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    /* VULKAN_KEY_START */

    std::vector<vk::PhysicalDeviceGroupProperties> groupProperties = instance.enumeratePhysicalDeviceGroups();

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < groupProperties.size(); i++ )
    {
      std::cout << "Group Properties " << i << "\n";
      std::cout << "\t"
                << "physicalDeviceCount = " << groupProperties[i].physicalDeviceCount << "\n";
      std::cout << "\t"
                << "physicalDevices:\n";
      for ( size_t j = 0; j < groupProperties[i].physicalDeviceCount; j++ )
      {
        std::cout << "\t\t" << j << " : " << groupProperties[i].physicalDevices[j].getProperties().deviceName << "\n";
      }
      std::cout << "\t"
                << "subsetAllocation    = " << !!groupProperties[i].subsetAllocation << "\n";
      std::cout << "\n";

      if ( 0 < groupProperties[i].physicalDeviceCount )
      {
        vk::PhysicalDevice physicalDevice = groupProperties[i].physicalDevices[0];

        // get the QueueFamilyProperties of the first PhysicalDevice
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

        // get the first index into queueFamiliyProperties which supports graphics
        auto   propertyIterator         = std::find_if( queueFamilyProperties.begin(),
                                              queueFamilyProperties.end(),
                                              []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } );
        size_t graphicsQueueFamilyIndex = std::distance( queueFamilyProperties.begin(), propertyIterator );
        assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

        // create a Device
        float                     queuePriority = 0.0f;
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
        vk::StructureChain<vk::DeviceCreateInfo, vk::DeviceGroupDeviceCreateInfo> deviceCreateInfoChain(
          { {}, deviceQueueCreateInfo }, { groupProperties[i].physicalDeviceCount, groupProperties[i].physicalDevices } );

        vk::Device device = physicalDevice.createDevice( deviceCreateInfoChain.get<vk::DeviceCreateInfo>() );

        // ... and destroy it again
        device.destroy();
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
