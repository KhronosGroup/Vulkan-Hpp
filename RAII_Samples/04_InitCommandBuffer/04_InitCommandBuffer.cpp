// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 04_InitCommandBufferRAII
//                     Create command buffer

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "04_InitCommandBufferRAII";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    uint32_t         graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::raii::Device device                   = vk::raii::su::makeDevice( physicalDevice, graphicsQueueFamilyIndex );

    /* VULKAN_HPP_KEY_START */

    // create a CommandPool to allocate a CommandBuffer from
    vk::CommandPoolCreateInfo commandPoolCreateInfo( {}, graphicsQueueFamilyIndex );
    vk::raii::CommandPool     commandPool( device, commandPoolCreateInfo );

    // allocate a CommandBuffer from the CommandPool
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 );
    vk::raii::CommandBuffer       commandBuffer = std::move( vk::raii::CommandBuffers( device, commandBufferAllocateInfo ).front() );

    /* VULKAN_HPP_KEY_END */
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
