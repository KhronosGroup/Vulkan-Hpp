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
// VulkanHpp Samples : CreateDebugReportCallback
//                     Draw a cube

#include "vulkan/vulkan.hpp"
#include <iostream>
#include <sstream>

static char const* AppName = "CreateDebugReportCallback";
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


int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> props = vk::enumerateInstanceExtensionProperties();

    auto propsIterator = std::find_if(props.begin(), props.end(), [](vk::ExtensionProperties const& ep) { return strcmp(ep.extensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0; });
    if (propsIterator == props.end())
    {
      std::cout << "Something went very wrong, cannot find " << VK_EXT_DEBUG_REPORT_EXTENSION_NAME << " extension" << std::endl;
      exit(1);
    }

    vk::ApplicationInfo applicationInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_0);
    const char *extensionName = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
    vk::UniqueInstance instance = vk::createInstanceUnique(vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &applicationInfo, 0, nullptr, 1, &extensionName));

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
