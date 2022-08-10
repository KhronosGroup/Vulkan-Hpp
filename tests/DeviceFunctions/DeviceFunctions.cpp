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
// VulkanHpp Samples : DeviceFunctions
//                     Compile test on device functions

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#undef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0

#include <iostream>
#include <vulkan/vulkan.hpp>

static char const * AppName    = "DeviceFunctions";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    vk::UniqueInstance  instance       = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) );
    vk::PhysicalDevice  physicalDevice = instance->enumeratePhysicalDevices().front();

    uint32_t propertyCount;
    physicalDevice.getQueueFamilyProperties( &propertyCount, nullptr );

    // get the QueueFamilyProperties of the first PhysicalDevice
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

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
    vk::UniqueDevice          device = physicalDevice.createDeviceUnique( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) );

    uint64_t handle = device->getAccelerationStructureHandleNV<uint8_t>( {}, vk::DispatchLoaderDynamic() );

    std::vector<vk::UniqueCommandBuffer>::allocator_type vectorAllocator;
    vk::UniqueCommandBuffer commandBuffer = std::move( device->allocateCommandBuffersUnique( {}, vectorAllocator, vk::DispatchLoaderStatic() ).front() );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );

    std::vector<vk::UniqueHandle<vk::CommandBuffer, vk::DispatchLoaderDynamic>>::allocator_type dynamicVectorAllocator;
    vk::UniqueHandle<vk::CommandBuffer, vk::DispatchLoaderDynamic>                              dynamicCommandBuffer =
      std::move( device->allocateCommandBuffersUnique( {}, dynamicVectorAllocator, vk::DispatchLoaderDynamic() ).front() );

    vk::Buffer       buffer       = device->createBuffer( {} );
    vk::UniqueBuffer uniqueBuffer = vk::UniqueBuffer( buffer, *device );

    vk::DeviceMemory       deviceMemory       = device->allocateMemory( {} );
    vk::UniqueDeviceMemory uniqueDeviceMemory = vk::UniqueDeviceMemory( deviceMemory, *device );

    vk::ResultValue<std::vector<vk::UniquePipeline>> pipelines = device->createGraphicsPipelinesUnique( nullptr, {} );
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  return 0;
}
