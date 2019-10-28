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
#include "vulkan/vulkan.hpp"
#include <iomanip>
#include <sstream>
#include <vector>

static char const* AppName = "PhysicalDeviceQueueFamilyProperties";
static char const* EngineName = "Vulkan.hpp";

#define USE_WORKAROUND  1

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName);
#if !defined(NDEBUG)
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger(instance);
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for (size_t i=0 ; i<physicalDevices.size() ; i++)
    {
      // some features are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";

#if USE_WORKAROUND
      uint32_t queueFamilyPropertyCount;
      VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevices[i], &queueFamilyPropertyCount, nullptr);
      std::vector<vk::QueueFamilyProperties2> queueFamilyProperties2(queueFamilyPropertyCount);
      std::vector<vk::QueueFamilyCheckpointPropertiesNV> queueFamilyCheckpointProperties(queueFamilyPropertyCount);
      for (uint32_t j=0 ; j<queueFamilyPropertyCount ; j++)
      {
        queueFamilyProperties2[j].pNext = &queueFamilyCheckpointProperties[j];
      }
      VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevices[i], &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2*>(queueFamilyProperties2.data()));
#else
      // need to explicitly specify all the template arguments for getQueueFamilyProperties2 to make the compiler happy
      using Chain = vk::StructureChain<vk::QueueFamilyProperties2, vk::QueueFamilyCheckpointPropertiesNV>;
      auto queueFamilyProperties2 = physicalDevices[i].getQueueFamilyProperties2<Chain, std::allocator<Chain>, vk::DispatchLoaderDynamic>();
#endif
      for (size_t j = 0; j < queueFamilyProperties2.size(); j++)
      {
        std::cout << "\t" << "QueueFamily " << j << "\n";
#if USE_WORKAROUND
        vk::QueueFamilyProperties const& properties = queueFamilyProperties2[j].queueFamilyProperties;
#else
        vk::QueueFamilyProperties const& properties = queueFamilyProperties2[j].get<vk::QueueFamilyProperties2>().queueFamilyProperties;
#endif
        std::cout << "\t\t" << "QueueFamilyProperties:\n";
        std::cout << "\t\t\t" << "queueFlags                  = " << vk::to_string(properties.queueFlags) << "\n";
        std::cout << "\t\t\t" << "queueCount                  = " << properties.queueCount << "\n";
        std::cout << "\t\t\t" << "timestampValidBits          = " << properties.timestampValidBits << "\n";
        std::cout << "\t\t\t" << "minImageTransferGranularity = " << properties.minImageTransferGranularity.width << " x " << properties.minImageTransferGranularity.height << " x " << properties.minImageTransferGranularity.depth << "\n";
        std::cout << "\n";

        if (vk::su::contains(extensionProperties, "VK_NV_device_diagnostic_checkpoints"))
        {
#if USE_WORKAROUND
          vk::QueueFamilyCheckpointPropertiesNV const* checkpointProperties = static_cast<vk::QueueFamilyCheckpointPropertiesNV const*>(queueFamilyProperties2[j].pNext);
#else
          vk::QueueFamilyCheckpointPropertiesNV const& checkpointProperties = queueFamilyProperties2[j].get<vk::QueueFamilyCheckpointPropertiesNV>();
#endif
          std::cout << "\t\t" << "CheckPointPropertiesNV:\n";
#if USE_WORKAROUND
          std::cout << "\t\t\t" << "checkpointExecutionStageMask  = " << vk::to_string(checkpointProperties->checkpointExecutionStageMask) << "\n";
#else
          std::cout << "\t\t\t" << "checkpointExecutionStageMask  = " << vk::to_string(checkpointProperties.checkpointExecutionStageMask) << "\n";
#endif
          std::cout << "\n";
        }
      }
    }

    /* VULKAN_KEY_END */
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error err)
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
