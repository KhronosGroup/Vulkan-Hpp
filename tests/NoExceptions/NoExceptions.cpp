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
// VulkanHpp Samples : NoExceptions
//                     Compile test with VULKAN_HPP_NO_EXCEPTIONS set
//                     Note: this is _no_ functional test!! Don't ever code this way!!

#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <iostream>
#include <vulkan/vulkan.hpp>

static char const * AppName    = "NoExceptions";
static char const * EngineName = "Vulkan.hpp";

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main( int /*argc*/, char ** /*argv*/ )
{
  VULKAN_HPP_DEFAULT_DISPATCHER.init();

  vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
  vk::UniqueInstance  instance = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) ).value;
  VULKAN_HPP_DEFAULT_DISPATCHER.init( *instance );

  std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices().value;
  assert( !physicalDevices.empty() );

  // get the QueueFamilyProperties of the first PhysicalDevice
  std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();

  // get the first index into queueFamiliyProperties which supports graphics
  size_t graphicsQueueFamilyIndex =
    std::distance( queueFamilyProperties.begin(),
                   std::find_if( queueFamilyProperties.begin(),
                                 queueFamilyProperties.end(),
                                 []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } ) );
  assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

  // create a UniqueDevice
  float                     queuePriority = 0.0f;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
  vk::UniqueDevice          device = physicalDevices[0].createDeviceUnique( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) ).value;
  VULKAN_HPP_DEFAULT_DISPATCHER.init( *device );

  // create a UniqueCommandPool to allocate a CommandBuffer from
  vk::UniqueCommandPool commandPool =
    device->createCommandPoolUnique( vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), deviceQueueCreateInfo.queueFamilyIndex ) ).value;

  // allocate a CommandBuffer from the CommandPool
  vk::UniqueCommandBuffer commandBuffer =
    std::move( device->allocateCommandBuffersUnique( vk::CommandBufferAllocateInfo( commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) ).value.front() );

  return 0;
}
