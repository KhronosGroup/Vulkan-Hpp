// Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 04_InitCommandBuffer
//                     Create command buffer

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "04_InitCommandBuffer";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName);
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    uint32_t graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex(physicalDevices[0].getQueueFamilyProperties());
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsQueueFamilyIndex);

    /* VULKAN_HPP_KEY_START */

    // create a UniqueCommandPool to allocate a CommandBuffer from
    vk::UniqueCommandPool commandPool = device->createCommandPoolUnique(vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), graphicsQueueFamilyIndex));

    // allocate a CommandBuffer from the CommandPool
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    // Note: No need to explicitly free the CommandBuffer or destroy the CommandPool, as the corresponding free and destroy
    // functions are called by the destructor of the UniqueCommandBuffer and the UniqueCommandPool on leaving this scope.

    /* VULKAN_HPP_KEY_END */
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
