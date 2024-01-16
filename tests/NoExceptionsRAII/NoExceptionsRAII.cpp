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
// VulkanHpp Samples : NoExceptionsRAII
//                     Compile test with VULKAN_HPP_NO_EXCEPTIONS set and using raii-classes
//                     Note: this is _no_ functional test!! Don't ever code this way!!

#define VULKAN_HPP_NO_EXCEPTIONS

// for this test, we need to include vulkan_hpp_macros.hpp first to determine if VULKAN_HPP_RAII_NO_EXCEPTIONS is defined in this context
#include <vulkan/vulkan_hpp_macros.hpp>

// only if VULKAN_HPP_RAII_NO_EXCEPTIONS really is defined, this test is meaningfull and needs to compile and run.
#if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
#  include <vulkan/vulkan_raii.hpp>

static char const * AppName    = "NoExceptions";
static char const * EngineName = "Vulkan.hpp";
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
#if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
  vk::raii::Context context;

  vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
  auto                instance = context.createInstance( { {}, &appInfo } );
  assert( instance.has_value() );

  auto physicalDevices = instance->enumeratePhysicalDevices();
  assert( physicalDevices.has_value() );
  auto physicalDevice = std::move( physicalDevices->front() );

  // get the QueueFamilyProperties of the first PhysicalDevice
  std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

  // get the first index into queueFamiliyProperties which supports graphics
  size_t graphicsQueueFamilyIndex =
    std::distance( queueFamilyProperties.begin(),
                   std::find_if( queueFamilyProperties.begin(),
                                 queueFamilyProperties.end(),
                                 []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } ) );
  assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

  // create a Device
  float                     queuePriority = 0.0f;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
  auto                      device = physicalDevice.createDevice( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) );
  assert( device.has_value() );

  // create a CommandPool to allocate a CommandBuffer from
  auto commandPool = device->createCommandPool( vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), deviceQueueCreateInfo.queueFamilyIndex ) );
  assert( commandPool.has_value() );

  // allocate a CommandBuffer from the CommandPool
  auto commandBuffers = device->allocateCommandBuffers( vk::CommandBufferAllocateInfo( *commandPool, vk::CommandBufferLevel::ePrimary, 1 ) );
  assert( commandBuffers.has_value() );

  auto commandBuffer = std::move( commandBuffers->front() );
#endif

  return 0;
}
