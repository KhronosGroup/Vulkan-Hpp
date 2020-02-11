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

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
  return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const * pAllocator)
{
  return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}


VkBool32 debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
  VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData, void * /*pUserData*/)
{
  std::string message;

  message += vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) + ": " + vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) + ":\n";
  message += std::string("\t") + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
  message += std::string("\t") + "messageIdNumber = " + std::to_string(pCallbackData->messageIdNumber) + "\n";
  message += std::string("\t") + "message         = <" + pCallbackData->pMessage + ">\n";
  if (0 < pCallbackData->queueLabelCount)
  {
    message += std::string("\t") + "Queue Labels:\n";
    for (uint8_t i = 0; i < pCallbackData->queueLabelCount; i++)
    {
      message += std::string("\t\t") + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
    }
  }
  if (0 < pCallbackData->cmdBufLabelCount)
  {
    message += std::string("\t") + "CommandBuffer Labels:\n";
    for (uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
    {
      message += std::string("\t\t") + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
    }
  }
  if (0 < pCallbackData->objectCount)
  {
    for (uint8_t i = 0; i < pCallbackData->objectCount; i++)
    {
      message += std::string("\t") + "Object " + std::to_string(i) + "\n";
      message += std::string("\t\t") + "objectType   = " + vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) + "\n";
      message += std::string("\t\t") + "objectHandle = " + std::to_string(pCallbackData->pObjects[i].objectHandle) + "\n";
      if (pCallbackData->pObjects[i].pObjectName)
      {
        message += std::string("\t\t") + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
      }
  }
}

#ifdef _WIN32
  MessageBox(NULL, message.c_str(), "Alert", MB_OK);
#else
  std::cout << message << std::endl;
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
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    // initialize the DipatchLoaderDynamic to use
    static vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

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
    instanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    vk::ApplicationInfo applicationInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_1);
    vk::InstanceCreateInfo instanceCreateInfo( vk::InstanceCreateFlags(), &applicationInfo, vk::su::checked_cast<uint32_t>(instanceLayerNames.size()), instanceLayerNames.data(),
      vk::su::checked_cast<uint32_t>(instanceExtensionNames.size()) , instanceExtensionNames.data() );
    vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance);
#endif

    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr("vkCreateDebugUtilsMessengerEXT"));
    if (!pfnVkCreateDebugUtilsMessengerEXT)
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
      exit(1);
    }

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    if (!pfnVkDestroyDebugUtilsMessengerEXT)
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
      exit(1);
    }

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = instance->createDebugUtilsMessengerEXTUnique(vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &debugMessageFunc));

    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    assert(!queueFamilyProperties.empty());

    auto qfpIt = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(), [](vk::QueueFamilyProperties const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });
    assert(qfpIt != queueFamilyProperties.end());
    uint32_t queueFamilyIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), qfpIt));

    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority);
    vk::UniqueDevice device = physicalDevice.createDeviceUnique(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo));

    // Create a command pool (not a UniqueCommandPool, for testing purposes!
    vk::CommandPool commandPool = device->createCommandPool(vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), queueFamilyIndex));

    // The commandPool is not destroyed automatically (as it's not a UniqueCommandPool.
    // That is, the device is destroyed before the commmand pool and will trigger a validation error.
    std::cout << "*** INTENTIONALLY calling vkDestroyDevice before destroying command pool ***\n";
    std::cout << "*** The following error message is EXPECTED ***\n";

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
