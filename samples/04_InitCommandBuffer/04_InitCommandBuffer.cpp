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

#include <iostream>

static char const * AppName    = "04_InitCommandBuffer";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    uint32_t   graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::Device device                   = vk::su::createDevice( physicalDevice, graphicsQueueFamilyIndex );

    /* VULKAN_HPP_KEY_START */

    // create a CommandPool to allocate a CommandBuffer from
    vk::CommandPool commandPool = device.createCommandPool( vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), graphicsQueueFamilyIndex ) );

    // allocate a CommandBuffer from the CommandPool
    vk::CommandBuffer commandBuffer =
      device.allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 ) ).front();

    // freeing the commandBuffer is optional, as it will automatically freed when the corresponding CommandPool is
    // destroyed.
    device.freeCommandBuffers( commandPool, commandBuffer );

    // destroy the commandPool
    device.destroyCommandPool( commandPool );

    /* VULKAN_HPP_KEY_END */

    device.destroy();
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
