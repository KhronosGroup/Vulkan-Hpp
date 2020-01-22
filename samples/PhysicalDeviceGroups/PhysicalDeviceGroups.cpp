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

static char const* AppName = "PhysicalDeviceGroups";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName);
#if !defined(NDEBUG)
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger(instance);
#endif

    /* VULKAN_KEY_START */

    std::vector<vk::PhysicalDeviceGroupProperties> groupProperties = instance->enumeratePhysicalDeviceGroups();

    std::cout << std::boolalpha;
    for (size_t i=0 ; i< groupProperties.size() ; i++)
    {
      std::cout << "Group Properties " << i << "\n";
      std::cout << "\t" << "physicalDeviceCount = " << groupProperties[i].physicalDeviceCount << "\n";
      std::cout << "\t" << "physicalDevices:\n";
      for (size_t j = 0; j < groupProperties[i].physicalDeviceCount; j++)
      {
        std::cout << "\t\t" << j << " : " << groupProperties[i].physicalDevices[j] << "\n";
      }
      std::cout << "\t" << "subsetAllocation    = " << static_cast<bool>(groupProperties[i].subsetAllocation) << "\n";
      std::cout << "\n";

      if (1 < groupProperties[i].physicalDeviceCount)
      {
        vk::PhysicalDevice physicalDevice = groupProperties[i].physicalDevices[0];

        // get the QueueFamilyProperties of the first PhysicalDevice
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

        // get the first index into queueFamiliyProperties which supports graphics
        size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(),
                                                        std::find_if(queueFamilyProperties.begin(),
                                                                     queueFamilyProperties.end(),
                                                                     [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
        assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());

        // create a UniqueDevice
        float queuePriority = 0.0f;
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority);
        vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo);

        vk::DeviceGroupDeviceCreateInfo deviceGroupDeviceCreateInfo(groupProperties[i].physicalDeviceCount, groupProperties[i].physicalDevices);
        deviceCreateInfo.pNext = &deviceGroupDeviceCreateInfo;

        vk::UniqueDevice device = physicalDevice.createDeviceUnique(deviceCreateInfo);
      }
    }

    /* VULKAN_KEY_END */
  }
  catch (vk::SystemError& err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error& err)
  {
    std::cout << "std::runtime_error: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
