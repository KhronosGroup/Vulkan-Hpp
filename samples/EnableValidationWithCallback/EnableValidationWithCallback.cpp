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
// VulkanHpp Samples : EnableValidationWithCallback
//                     Show how to enable validation layers and provide callback

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

static char const* AppName = "EnableValidationWithCallback";
static char const* EngineName = "Vulkan.hpp";

PFN_vkCreateDebugReportCallbackEXT  pfnVkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
  return pfnVkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
  pfnVkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}


VKAPI_ATTR VkBool32 VKAPI_CALL dbgFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objType*/, uint64_t /*srcObject*/, size_t /*location*/, int32_t msgCode, const char *pLayerPrefix, const char *pMsg, void * /*pUserData*/)
{
  std::ostringstream message;

  switch (flags)
  {
    case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
      message << "INFORMATION: ";
      break;
    case VK_DEBUG_REPORT_WARNING_BIT_EXT:
      message << "WARNING: ";
      break;
    case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
      message << "PERFORMANCE WARNING: ";
      break;
    case VK_DEBUG_REPORT_ERROR_BIT_EXT:
      message << "ERROR: ";
      break;
    case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
      message << "DEBUG: ";
      break;
    default:
      message << "unknown flag (" << flags << "): ";
      break;
  }
  message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

#ifdef _WIN32
  MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
#else
  std::cout << message.str() << std::endl;
#endif

  return false;
}

bool checkLayers(std::vector<char const*> const& layers, std::vector<vk::LayerProperties> const& properties)
{
  // return true if all layers are listed in the properties
  return std::all_of(layers.begin(), layers.end(), [&properties](char const* name)
  {
    return std::find_if(properties.begin(), properties.end(), [&name](vk::LayerProperties const& property) { return strcmp(property.layerName, name) == 0; }) != properties.end();
  });
}

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    std::vector<vk::LayerProperties> instanceLayerProperties = vk::enumerateInstanceLayerProperties();

    /* VULKAN_KEY_START */

    // Use standard_validation meta layer that enables all recommended validation layers
    std::vector<char const*> instanceLayerNames;
    instanceLayerNames.push_back("VK_LAYER_KHRONOS_validation");
    if (!checkLayers(instanceLayerNames, instanceLayerProperties))
    {
      std::cout << "Set the environment variable VK_LAYER_PATH to point to the location of your layers" << std::endl;
      exit(1);
    }

    /* Enable debug callback extension */
    std::vector<char const*> instanceExtensionNames;
    instanceExtensionNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    vk::ApplicationInfo applicationInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_0);
    vk::InstanceCreateInfo instanceCreateInfo( vk::InstanceCreateFlags(), &applicationInfo, vk::su::checked_cast<uint32_t>(instanceLayerNames.size()), instanceLayerNames.data(),
      vk::su::checked_cast<uint32_t>(instanceExtensionNames.size()) , instanceExtensionNames.data() );
    vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();
    assert(!queueFamilyProperties.empty());

    auto qfpIt = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(), [](vk::QueueFamilyProperties const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });
    assert(qfpIt != queueFamilyProperties.end());
    uint32_t queueFamilyIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), qfpIt));

    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority);
    vk::UniqueDevice device = physicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo));

    pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance->getProcAddr("vkCreateDebugReportCallbackEXT"));
    if (!pfnVkCreateDebugReportCallbackEXT)
    {
      std::cout << "GetInstanceProcAddr: Unable to find vkCreateDebugReportCallbackEXT function." << std::endl;
      exit(1);
    }
    pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance->getProcAddr("vkDestroyDebugReportCallbackEXT"));
    if (!pfnVkDestroyDebugReportCallbackEXT)
    {
      std::cout << "GetInstanceProcAddr: Unable to find vkDestroyDebugReportCallbackEXT function." << std::endl;
      exit(1);
    }

    vk::UniqueDebugReportCallbackEXT debugReportCallback = instance->createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning, dbgFunc));

    // Create a command pool (not a UniqueCommandPool, for testing purposes!
    vk::CommandPool commandPool = device->createCommandPool(vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), queueFamilyIndex));

    // The commandPool is not destroyed automatically (as it's not a UniqueCommandPool.
    // That is, the device is destroyed before the commmand pool and will trigger a validation error.
    std::cout << "*** INTENTIONALLY calling vkDestroyDevice before destroying command pool ***\n";
    std::cout << "*** The following error message is EXPECTED ***\n";

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
