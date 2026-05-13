// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : NoExceptions
//                   Compile test with VULKAN_HPP_NO_EXCEPTIONS set
//                   Note: this is _no_ functional test!! Don't ever code this way!!

#if !defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
#include <cstdint>
import vulkan;
#else
#  include <vector>
#  include <cstdint>
#  include <iostream>
#  include <algorithm>
#  include <vulkan/vulkan.hpp>
   VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif


static char const * AppName    = "NoExceptions";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  vk::detail::defaultDispatchLoaderDynamic.init();

  vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, vk::ApiVersion11 );
  vk::UniqueInstance  instance = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) ).value;
  vk::detail::defaultDispatchLoaderDynamic.init( *instance );

  std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices().value;
  release_assert( !physicalDevices.empty() );

  // get the QueueFamilyProperties of the first PhysicalDevice
  std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();

  // get the first index into queueFamiliyProperties which supports graphics
  std::size_t graphicsQueueFamilyIndex =
    std::distance( queueFamilyProperties.begin(),
                   std::find_if( queueFamilyProperties.begin(),
                                 queueFamilyProperties.end(),
                                 []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } ) );
  release_assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

  // create a UniqueDevice
  float                     queuePriority = 0.0f;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
  vk::UniqueDevice          device = physicalDevices[0].createDeviceUnique( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) ).value;
  vk::detail::defaultDispatchLoaderDynamic.init( *device );

  // create a UniqueCommandPool to allocate a CommandBuffer from
  vk::UniqueCommandPool commandPool =
    device->createCommandPoolUnique( vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), deviceQueueCreateInfo.queueFamilyIndex ) ).value;

  // allocate a CommandBuffer from the CommandPool
  vk::UniqueCommandBuffer commandBuffer =
    std::move( device->allocateCommandBuffersUnique( vk::CommandBufferAllocateInfo( commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) ).value.front() );

  return 0;
}
