// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : DispatchLoaderDynamic
//                     Compile test on DispatchLoaderDynamic functions

#include "vulkan/vulkan.hpp"
#include <iostream>
#include <map>

static char const* AppName = "DispatchLoaderDynamic";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    // empty DispatchLoaderDynamic, used for init calls later on
    vk::DispatchLoaderDynamic dld0;

    HMODULE vulkanDll = LoadLibrary("C:/Windows/System32/vulkan-1.dll");
    if (vulkanDll)
    {
      // create a dispatcher, based on vkInstance/vkGetInstanceProcAddr only
      PFN_vkCreateInstance vkCreateInstance = PFN_vkCreateInstance(GetProcAddress(vulkanDll, "vkCreateInstance"));
      assert(vkCreateInstance);

      VkInstanceCreateInfo vkInstanceCreateInfo = {};
      vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      VkInstance vkInstance;
      VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &vkInstance);
      assert(vkResult == VK_SUCCESS);

      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = PFN_vkGetInstanceProcAddr(GetProcAddress(vulkanDll, "vkGetInstanceProcAddr"));
      assert(vkGetInstanceProcAddr);

      vk::DispatchLoaderDynamic dld1(vkInstance, vkGetInstanceProcAddr);

      // compare to the empty dispatcher, and init the empty dispatcher the same way
      assert(memcmp(&dld0, &dld1, sizeof(vk::DispatchLoaderDynamic)) != 0);
      dld0.init(vkInstance, vkGetInstanceProcAddr);
      assert(memcmp(&dld0, &dld1, sizeof(vk::DispatchLoaderDynamic)) == 0);


      // create a dispatcher, based on additional vkDevice/vkGetDeviceProcAddr
      PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = PFN_vkEnumeratePhysicalDevices(GetProcAddress(vulkanDll, "vkEnumeratePhysicalDevices"));
      assert(vkEnumeratePhysicalDevices);

      std::vector<VkPhysicalDevice> physicalDevices;
      uint32_t physicalDeviceCount;
      do
      {
        vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, nullptr);
        if ((vkResult == VK_SUCCESS) && physicalDeviceCount)
        {
          physicalDevices.resize(physicalDeviceCount);
          vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, physicalDevices.data());
        }
      } while (vkResult != VK_SUCCESS);
      assert(vkResult == VK_SUCCESS);
      assert(physicalDeviceCount <= physicalDevices.size());
      if (physicalDeviceCount < physicalDevices.size())
      {
        physicalDevices.resize(physicalDeviceCount);
      }
      assert(!physicalDevices.empty());

      PFN_vkCreateDevice vkCreateDevice = PFN_vkCreateDevice(GetProcAddress(vulkanDll, "vkCreateDevice"));
      assert(vkCreateDevice);

      VkDeviceCreateInfo vkDeviceCreateInfo = {};
      vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      VkDevice vkDevice;
      vkResult = vkCreateDevice(physicalDevices[0], &vkDeviceCreateInfo, nullptr, &vkDevice);
      assert(vkResult == VK_SUCCESS);

      PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = PFN_vkGetDeviceProcAddr(GetProcAddress(vulkanDll, "vkGetDeviceProcAddr"));
      vk::DispatchLoaderDynamic dld2(vkInstance, vkGetInstanceProcAddr, vkDevice, vkGetDeviceProcAddr);

      // compare to "simpler" dispatcher and make them equal
      assert(memcmp(&dld0, &dld2, sizeof(vk::DispatchLoaderDynamic)) != 0);
      dld0.init(vkInstance, vkGetInstanceProcAddr, vkDevice, vkGetDeviceProcAddr);
      assert(memcmp(&dld0, &dld2, sizeof(vk::DispatchLoaderDynamic)) == 0);

      FreeLibrary(vulkanDll);
    }
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }

  return 0;
}
