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
// VulkanHpp Samples : FunctionCalls
//                     Compile test on using function calls to catch API changes

#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )  // local variable is initialized but not referenced
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <vulkan/vulkan.h>
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
#endif

namespace vk
{
  namespace detail
  {
    DispatchLoaderDynamic defaultDispatchLoaderDynamic;
  }  // namespace detail
}  // namespace vk

int main( int /*argc*/, char ** /*argv*/ )
{
  //=== VK_VERSION_1_0 ===
  // Device initialization
  {
    vk::InstanceCreateInfo  instanceCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Instance            instance;
    vk::Result              result = vk::createInstance( &instanceCreateInfo, &allocationCallbacks, &instance );
  }
  {
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::Instance           instance = vk::createInstance( instanceCreateInfo );
  }

  {
    vk::Instance instance;
    instance.destroy();
  }

  {
    vk::Instance instance;
    uint32_t     physicalDeviceCount;
    vk::Result   result = instance.enumeratePhysicalDevices( &physicalDeviceCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::PhysicalDevice> physicalDevices( physicalDeviceCount );
      result = instance.enumeratePhysicalDevices( &physicalDeviceCount, physicalDevices.data() );
    }
  }
  {
    vk::Instance                    instance;
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
  }
  {
    vk::Instance                       instance;
    std::allocator<vk::PhysicalDevice> allocator;
    std::vector<vk::PhysicalDevice>    physicalDevices = instance.enumeratePhysicalDevices( allocator );
  }

  {
    vk::PhysicalDevice         physicalDevice;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures;
    physicalDevice.getFeatures( &physicalDeviceFeatures );
  }
  {
    vk::PhysicalDevice         physicalDevice;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
  }

  {
    vk::PhysicalDevice   physicalDevice;
    vk::Format           format = {};
    vk::FormatProperties formatProperties;
    physicalDevice.getFormatProperties( format, &formatProperties );
  }
  {
    vk::PhysicalDevice   physicalDevice;
    vk::Format           format           = {};
    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( format );
  }

  {
    vk::PhysicalDevice        physicalDevice;
    vk::Format                format      = {};
    vk::ImageType             imageType   = {};
    vk::ImageTiling           imageTiling = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties;
    vk::Result result = physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags, &imageFormatProperties );
  }
  {
    vk::PhysicalDevice        physicalDevice;
    vk::Format                format      = {};
    vk::ImageType             imageType   = {};
    vk::ImageTiling           imageTiling = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties =
      physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags );
  }

  {
    vk::PhysicalDevice           physicalDevice;
    vk::PhysicalDeviceProperties physicalDeviceProperties;
    physicalDevice.getProperties( &physicalDeviceProperties );
  }
  {
    vk::PhysicalDevice           physicalDevice;
    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
  }

  {
    vk::PhysicalDevice physicalDevice;
    uint32_t           queueFamilyPropertyCount;
    physicalDevice.getQueueFamilyProperties( &queueFamilyPropertyCount, nullptr );
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties( queueFamilyPropertyCount );
    physicalDevice.getQueueFamilyProperties( &queueFamilyPropertyCount, queueFamilyProperties.data() );
  }
  {
    vk::PhysicalDevice                     physicalDevice;
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  }
  {
    vk::PhysicalDevice                        physicalDevice;
    std::allocator<vk::QueueFamilyProperties> allocator;
    std::vector<vk::QueueFamilyProperties>    queueFamilyProperties = physicalDevice.getQueueFamilyProperties( allocator );
  }

  {
    vk::PhysicalDevice                 physicalDevice;
    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties( &memoryProperties );
  }
  {
    vk::PhysicalDevice                 physicalDevice;
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
  }

  {
    vk::Instance       instance;
    PFN_vkVoidFunction voidFunction = instance.getProcAddr( "vkCreateInstance" );
  }
  {
    vk::Instance       instance;
    std::string        name         = "vkCreateInstance";
    PFN_vkVoidFunction voidFunction = instance.getProcAddr( name );
  }

  {
    vk::Device         device;
    PFN_vkVoidFunction voidFunction = device.getProcAddr( "vkCreateInstance" );
  }
  {
    vk::Device         device;
    std::string        name         = "vkCreateInstance";
    PFN_vkVoidFunction voidFunction = device.getProcAddr( name );
  }

  // Device commands
  {
    vk::PhysicalDevice      physicalDevice;
    vk::DeviceCreateInfo    deviceCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Device              device;
    vk::Result              result = physicalDevice.createDevice( &deviceCreateInfo, &allocationCallbacks, &device );
  }
  {
    vk::PhysicalDevice   physicalDevice;
    vk::DeviceCreateInfo deviceCreateInfo;
    vk::Device           device = physicalDevice.createDevice( deviceCreateInfo );
  }

  {
    vk::Device device;
    device.destroy();
  }

  // Extension discovery commands
  {
    uint32_t   propertyCount;
    vk::Result result = vk::enumerateInstanceExtensionProperties( nullptr, &propertyCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::ExtensionProperties> properties( propertyCount );
      result = vk::enumerateInstanceExtensionProperties( nullptr, &propertyCount, properties.data() );
    }
  }
  {
    std::vector<vk::ExtensionProperties> properties = vk::enumerateInstanceExtensionProperties( nullptr );
  }
  {
    using Allocator = std::allocator<vk::ExtensionProperties>;
    Allocator                                       allocator;
    std::vector<vk::ExtensionProperties, Allocator> properties = vk::enumerateInstanceExtensionProperties( nullptr, allocator );
  }

  {
    vk::PhysicalDevice physicalDevice;
    uint32_t           propertyCount;
    vk::Result         result = physicalDevice.enumerateDeviceExtensionProperties( nullptr, &propertyCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::ExtensionProperties> properties( propertyCount );
      result = physicalDevice.enumerateDeviceExtensionProperties( nullptr, &propertyCount, properties.data() );
    }
  }
  {
    vk::PhysicalDevice                   physicalDevice;
    std::vector<vk::ExtensionProperties> properties = physicalDevice.enumerateDeviceExtensionProperties( nullptr );
  }
  {
    vk::PhysicalDevice physicalDevice;
    using Allocator = std::allocator<vk::ExtensionProperties>;
    Allocator                                       allocator;
    std::vector<vk::ExtensionProperties, Allocator> properties = physicalDevice.enumerateDeviceExtensionProperties( nullptr, allocator );
  }

  // Layer discovery commands
  {
    uint32_t   propertyCount;
    vk::Result result = vk::enumerateInstanceLayerProperties( &propertyCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::LayerProperties> properties( propertyCount );
      result = vk::enumerateInstanceLayerProperties( &propertyCount, properties.data() );
    }
  }
  {
    std::vector<vk::LayerProperties> properties = vk::enumerateInstanceLayerProperties();
  }
  {
    using Allocator = std::allocator<vk::LayerProperties>;
    Allocator                                   allocator;
    std::vector<vk::LayerProperties, Allocator> properties = vk::enumerateInstanceLayerProperties( allocator );
  }

  {
    vk::PhysicalDevice physicalDevice;
    uint32_t           propertyCount;
    vk::Result         result = physicalDevice.enumerateDeviceLayerProperties( &propertyCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::LayerProperties> properties( propertyCount );
      result = physicalDevice.enumerateDeviceLayerProperties( &propertyCount, properties.data() );
    }
  }
  {
    vk::PhysicalDevice               physicalDevice;
    std::vector<vk::LayerProperties> properties = physicalDevice.enumerateDeviceLayerProperties();
  }
  {
    vk::PhysicalDevice physicalDevice;
    using Allocator = std::allocator<vk::LayerProperties>;
    Allocator                                   allocator;
    std::vector<vk::LayerProperties, Allocator> properties = physicalDevice.enumerateDeviceLayerProperties( allocator );
  }

  // Queue commands
  {
    vk::Device device;
    uint32_t   queueFamilyIndex = 0;
    uint32_t   queueIndex       = 0;
    vk::Queue  queue;
    device.getQueue( queueFamilyIndex, queueIndex, &queue );
  }
  {
    vk::Device device;
    uint32_t   queueFamilyIndex = 0;
    uint32_t   queueIndex       = 0;
    vk::Queue  queue            = device.getQueue( queueFamilyIndex, queueIndex );
  }

  {
    vk::Queue      queue;
    uint32_t       submitCount = 1;
    vk::SubmitInfo submitInfo;
    vk::Fence      fence;
    vk::Result     result = queue.submit( submitCount, &submitInfo, fence );
  }
  {
    vk::Queue      queue;
    vk::SubmitInfo submitInfo;
    vk::Fence      fence;
    queue.submit( submitInfo, fence );
  }

  {
    vk::Queue queue;
    queue.waitIdle();
  }

  {
    vk::Device device;
    device.waitIdle();
  }

  // Memory commands
  {
    vk::Device              device;
    vk::MemoryAllocateInfo  memoryAllocateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::DeviceMemory        memory;
    vk::Result              result = device.allocateMemory( &memoryAllocateInfo, &allocationCallbacks, &memory );
  }
  {
    vk::Device             device;
    vk::MemoryAllocateInfo memoryAllocateInfo;
    vk::DeviceMemory       memory = device.allocateMemory( memoryAllocateInfo );
  }

  {
    vk::Device       device;
    vk::DeviceMemory memory;
    device.freeMemory( memory );
  }

  {
    vk::Device         device;
    vk::DeviceMemory   memory;
    vk::DeviceSize     offset         = 0;
    vk::DeviceSize     size           = vk::WholeSize;
    vk::MemoryMapFlags memoryMapFlags = {};
    void *             pData;
    vk::Result         result = device.mapMemory( memory, offset, size, memoryMapFlags, &pData );
  }
  {
    vk::Device         device;
    vk::DeviceMemory   memory;
    vk::DeviceSize     offset         = 0;
    vk::DeviceSize     size           = vk::WholeSize;
    vk::MemoryMapFlags memoryMapFlags = {};
    void *             pData          = device.mapMemory( memory, offset, size, memoryMapFlags );
  }

  {
    vk::Device       device;
    vk::DeviceMemory memory;
    device.unmapMemory( memory );
  }

  {
    vk::Device            device;
    vk::DeviceMemory      memory;
    vk::MappedMemoryRange mappedMemoryRange;
    vk::Result            result = device.flushMappedMemoryRanges( 1, &mappedMemoryRange );
  }
  {
    vk::Device                         device;
    std::vector<vk::MappedMemoryRange> mappedMemoryRanges;
    device.flushMappedMemoryRanges( mappedMemoryRanges );
  }

  {
    vk::Device            device;
    vk::MappedMemoryRange mappedMemoryRange;
    vk::Result            result = device.invalidateMappedMemoryRanges( 1, &mappedMemoryRange );
  }
  {
    vk::Device                         device;
    std::vector<vk::MappedMemoryRange> mappedMemoryRanges;
    device.invalidateMappedMemoryRanges( mappedMemoryRanges );
  }

  {
    vk::Device       device;
    vk::DeviceMemory memory;
    vk::DeviceSize   commitment;
    device.getMemoryCommitment( memory, &commitment );
  }
  {
    vk::Device       device;
    vk::DeviceMemory memory;
    vk::DeviceSize   commitment = device.getMemoryCommitment( memory );
  }

  // Memory management API commands
  {
    vk::Device       device;
    vk::Buffer       buffer;
    vk::DeviceMemory deviceMemory;
    vk::DeviceSize   memoryOffset = 0;
    device.bindBufferMemory( buffer, deviceMemory, memoryOffset );
  }

  {
    vk::Device       device;
    vk::Image        image;
    vk::DeviceMemory deviceMemory;
    vk::DeviceSize   memoryOffset = 0;
    device.bindImageMemory( image, deviceMemory, memoryOffset );
  }

  {
    vk::Device             device;
    vk::Buffer             buffer;
    vk::MemoryRequirements memoryRequirements;
    device.getBufferMemoryRequirements( buffer, &memoryRequirements );
  }
  {
    vk::Device             device;
    vk::Buffer             buffer;
    vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements( buffer );
  }

  {
    vk::Device             device;
    vk::Image              image;
    vk::MemoryRequirements memoryRequirements;
    device.getImageMemoryRequirements( image, &memoryRequirements );
  }
  {
    vk::Device             device;
    vk::Image              image;
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements( image );
  }

  // Sparse resource memory management API commands (optional)
  {
    vk::Device device;
    vk::Image  image;
    uint32_t   sparseMemoryRequirementCount;
    device.getImageSparseMemoryRequirements( image, &sparseMemoryRequirementCount, nullptr );
    if ( sparseMemoryRequirementCount )
    {
      std::vector<vk::SparseImageMemoryRequirements> sparseImageMemoryRequirements( sparseMemoryRequirementCount );
      device.getImageSparseMemoryRequirements( image, &sparseMemoryRequirementCount, sparseImageMemoryRequirements.data() );
    }
  }
  {
    vk::Device                                     device;
    vk::Image                                      image;
    std::vector<vk::SparseImageMemoryRequirements> sparseImageMemoryRequirementss = device.getImageSparseMemoryRequirements( image );
  }
  {
    vk::Device device;
    vk::Image  image;
    using Allocator = std::allocator<vk::SparseImageMemoryRequirements>;
    Allocator                                                 allocator;
    std::vector<vk::SparseImageMemoryRequirements, Allocator> sparseImageMemoryRequirementss = device.getImageSparseMemoryRequirements( image, allocator );
  }

  {
    vk::PhysicalDevice      physicalDevice;
    vk::Format              format        = {};
    vk::ImageType           type          = {};
    vk::SampleCountFlagBits samples       = {};
    vk::ImageUsageFlags     usage         = {};
    vk::ImageTiling         tiling        = {};
    uint32_t                propertyCount = 0;
    physicalDevice.getSparseImageFormatProperties( format, type, samples, usage, tiling, &propertyCount, nullptr );
    if ( propertyCount )
    {
      std::vector<vk::SparseImageFormatProperties> sparseImageFormatProperties( propertyCount );
      physicalDevice.getSparseImageFormatProperties( format, type, samples, usage, tiling, &propertyCount, sparseImageFormatProperties.data() );
    }
  }
  {
    vk::PhysicalDevice                           physicalDevice;
    vk::Format                                   format  = {};
    vk::ImageType                                type    = {};
    vk::SampleCountFlagBits                      samples = {};
    vk::ImageUsageFlags                          usage   = {};
    vk::ImageTiling                              tiling  = {};
    std::vector<vk::SparseImageFormatProperties> sparseImageFormatProperties =
      physicalDevice.getSparseImageFormatProperties( format, type, samples, usage, tiling );
  }
  {
    vk::PhysicalDevice      physicalDevice;
    vk::Format              format  = {};
    vk::ImageType           type    = {};
    vk::SampleCountFlagBits samples = {};
    vk::ImageUsageFlags     usage   = {};
    vk::ImageTiling         tiling  = {};
    using Allocator                 = std::allocator<vk::SparseImageFormatProperties>;
    Allocator                                               allocator;
    std::vector<vk::SparseImageFormatProperties, Allocator> sparseImageFormatProperties =
      physicalDevice.getSparseImageFormatProperties( format, type, samples, usage, tiling, allocator );
  }

  {
    vk::Queue          queue;
    uint32_t           bindInfoCount = 1;
    vk::BindSparseInfo bindSparseInfo;
    vk::Fence          fence;
    vk::Result         result = queue.bindSparse( bindInfoCount, &bindSparseInfo, fence );
  }

  {
    vk::Queue          queue;
    vk::BindSparseInfo bindSparseInfo;
    vk::Fence          fence;
    queue.bindSparse( bindSparseInfo, fence );
  }

  // Fence commands
  {
    vk::Device              device;
    vk::FenceCreateInfo     fenceCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Fence               fence;
    vk::Result              result = device.createFence( &fenceCreateInfo, &allocationCallbacks, &fence );
  }
  {
    vk::Device          device;
    vk::FenceCreateInfo fenceCreateInfo;
    vk::Fence           fence = device.createFence( fenceCreateInfo );
  }

  {
    vk::Device              device;
    vk::Fence               fence;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyFence( fence, &allocationCallbacks );
  }
  {
    vk::Device device;
    vk::Fence  fence;
    device.destroyFence( fence );
  }
  {
    vk::Device device;
    vk::Fence  fence;
    device.destroy( fence );
  }

  {
    vk::Device device;
    uint32_t   fenceCount = 1;
    vk::Fence  fence;
    vk::Result result = device.resetFences( fenceCount, &fence );
  }
  {
    vk::Device device;
    vk::Fence  fence;
    device.resetFences( fence );
  }

  {
    vk::Device device;
    vk::Fence  fence;
    vk::Result result = device.getFenceStatus( fence );
  }

  {
    vk::Device device;
    uint32_t   fenceCount = 1;
    vk::Fence  fence;
    vk::Bool32 waitAll = vk::True;
    uint64_t   timeout = 1000000000;
    vk::Result result  = device.waitForFences( fenceCount, &fence, waitAll, timeout );
  }
  {
    vk::Device device;
    vk::Fence  fence;
    vk::Bool32 waitAll = vk::True;
    uint64_t   timeout = 1000000000;
    vk::Result result  = device.waitForFences( fence, waitAll, timeout );
  }

  // Queue semaphore commands
  {
    vk::Device              device;
    vk::SemaphoreCreateInfo semaphoreCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Semaphore           semaphore;
    vk::Result              result = device.createSemaphore( &semaphoreCreateInfo, &allocationCallbacks, &semaphore );
  }
  {
    vk::Device              device;
    vk::SemaphoreCreateInfo semaphoreCreateInfo;
    vk::Semaphore           semaphore = device.createSemaphore( semaphoreCreateInfo );
  }

  {
    vk::Device              device;
    vk::Semaphore           semaphore;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroySemaphore( semaphore, &allocationCallbacks );
  }
  {
    vk::Device    device;
    vk::Semaphore semaphore;
    device.destroySemaphore( semaphore );
  }
  {
    vk::Device    device;
    vk::Semaphore semaphore;
    device.destroy( semaphore );
  }

  // Query commands
  {
    vk::Device              device;
    vk::QueryPoolCreateInfo queryPoolCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::QueryPool           queryPool;
    vk::Result              result = device.createQueryPool( &queryPoolCreateInfo, &allocationCallbacks, &queryPool );
  }
  {
    vk::Device              device;
    vk::QueryPoolCreateInfo queryPoolCreateInfo;
    vk::QueryPool           queryPool = device.createQueryPool( queryPoolCreateInfo );
  }

  {
    vk::Device              device;
    vk::QueryPool           queryPool;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyQueryPool( queryPool, &allocationCallbacks );
  }
  {
    vk::Device    device;
    vk::QueryPool queryPool;
    device.destroyQueryPool( queryPool );
  }
  {
    vk::Device    device;
    vk::QueryPool queryPool;
    device.destroy( queryPool );
  }

  {
    vk::Device           device;
    vk::QueryPool        queryPool;
    uint32_t             firstQuery = 0;
    uint32_t             queryCount = 1;
    size_t               dataSize   = sizeof( uint32_t );
    uint32_t             data;
    vk::DeviceSize       stride = 0;
    vk::QueryResultFlags flags  = {};
    vk::Result           result = device.getQueryPoolResults( queryPool, firstQuery, queryCount, dataSize, &data, stride, flags );
  }
  {
    vk::Device                             device;
    vk::QueryPool                          queryPool;
    uint32_t                               firstQuery  = 0;
    uint32_t                               queryCount  = 1;
    size_t                                 dataSize    = sizeof( uint32_t );
    vk::DeviceSize                         stride      = 0;
    vk::QueryResultFlags                   flags       = {};
    vk::ResultValue<std::vector<uint32_t>> resultValue = device.getQueryPoolResults<uint32_t>( queryPool, firstQuery, queryCount, dataSize, stride, flags );
  }
  {
    vk::Device                device;
    vk::QueryPool             queryPool;
    uint32_t                  firstQuery  = 0;
    uint32_t                  queryCount  = 1;
    vk::DeviceSize            stride      = 0;
    vk::QueryResultFlags      flags       = {};
    vk::ResultValue<uint32_t> resultValue = device.getQueryPoolResult<uint32_t>( queryPool, firstQuery, queryCount, stride, flags );
  }

  // Buffer commands
  {
    vk::Device              device;
    vk::BufferCreateInfo    bufferCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Buffer              buffer;
    vk::Result              result = device.createBuffer( &bufferCreateInfo, &allocationCallbacks, &buffer );
  }
  {
    vk::Device           device;
    vk::BufferCreateInfo bufferCreateInfo;
    vk::Buffer           buffer = device.createBuffer( bufferCreateInfo );
  }

  {
    vk::Device              device;
    vk::Buffer              buffer;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyBuffer( buffer, &allocationCallbacks );
  }
  {
    vk::Device device;
    vk::Buffer buffer;
    device.destroyBuffer( buffer );
  }
  {
    vk::Device device;
    vk::Buffer buffer;
    device.destroy( buffer );
  }

  // Image commands
  {
    vk::Device              device;
    vk::ImageCreateInfo     imageCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::Image               image;
    vk::Result              result = device.createImage( &imageCreateInfo, &allocationCallbacks, &image );
  }
  {
    vk::Device          device;
    vk::ImageCreateInfo imageCreateInfo;
    vk::Image           image = device.createImage( imageCreateInfo );
  }

  {
    vk::Device              device;
    vk::Image               image;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyImage( image, &allocationCallbacks );
  }
  {
    vk::Device device;
    vk::Image  image;
    device.destroyImage( image );
  }
  {
    vk::Device device;
    vk::Image  image;
    device.destroy( image );
  }

  {
    vk::Device            device;
    vk::Image             image;
    vk::ImageSubresource  subresource;
    vk::SubresourceLayout subresourceLayout;
    device.getImageSubresourceLayout( image, &subresource, &subresourceLayout );
  }
  {
    vk::Device            device;
    vk::Image             image;
    vk::ImageSubresource  subresource;
    vk::SubresourceLayout subresourceLayout = device.getImageSubresourceLayout( image, subresource );
  }

  // Image view commands
  {
    vk::Device              device;
    vk::ImageViewCreateInfo imageViewCreateInfo;
    vk::AllocationCallbacks allocationCallbacks;
    vk::ImageView           imageView;
    vk::Result              result = device.createImageView( &imageViewCreateInfo, &allocationCallbacks, &imageView );
  }
  {
    vk::Device              device;
    vk::ImageViewCreateInfo imageViewCreateInfo;
    vk::ImageView           imageView = device.createImageView( imageViewCreateInfo );
  }

  {
    vk::Device              device;
    vk::ImageView           imageView;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyImageView( imageView, &allocationCallbacks );
  }
  {
    vk::Device    device;
    vk::ImageView imageView;
    device.destroyImageView( imageView );
  }
  {
    vk::Device    device;
    vk::ImageView imageView;
    device.destroy( imageView );
  }

  // Command pool commands
  {
    vk::Device                device;
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    vk::AllocationCallbacks   allocationCallbacks;
    vk::CommandPool           commandPool;
    vk::Result                result = device.createCommandPool( &commandPoolCreateInfo, &allocationCallbacks, &commandPool );
  }
  {
    vk::Device                device;
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    vk::CommandPool           commandPool = device.createCommandPool( commandPoolCreateInfo );
  }

  {
    vk::Device              device;
    vk::CommandPool         commandPool;
    vk::AllocationCallbacks allocationCallbacks;
    device.destroyCommandPool( commandPool, &allocationCallbacks );
  }
  {
    vk::Device      device;
    vk::CommandPool commandPool;
    device.destroyCommandPool( commandPool );
  }
  {
    vk::Device      device;
    vk::CommandPool commandPool;
    device.destroy( commandPool );
  }

  {
    vk::Device                device;
    vk::CommandPool           commandPool;
    vk::CommandPoolResetFlags flags = {};
    device.resetCommandPool( commandPool, flags );
  }

  // Command buffer commands
  {
    vk::Device                     device;
    vk::CommandBufferAllocateInfo  commandBufferAllocateInfo;
    std::vector<vk::CommandBuffer> commandBuffers;
    vk::Result                     result = device.allocateCommandBuffers( &commandBufferAllocateInfo, commandBuffers.data() );
  }
  {
    vk::Device                     device;
    vk::CommandBufferAllocateInfo  commandBufferAllocateInfo;
    std::vector<vk::CommandBuffer> commandBuffers = device.allocateCommandBuffers( commandBufferAllocateInfo );
  }
  {
    vk::Device                    device;
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
    using Allocator = std::allocator<vk::CommandBuffer>;
    Allocator                                 allocator;
    std::vector<vk::CommandBuffer, Allocator> commandBuffers = device.allocateCommandBuffers( commandBufferAllocateInfo, allocator );
  }

  {
    vk::Device                     device;
    vk::CommandPool                commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;
    device.freeCommandBuffers( commandPool, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }
  {
    vk::Device                     device;
    vk::CommandPool                commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;
    device.freeCommandBuffers( commandPool, commandBuffers );
  }
  {
    vk::Device                     device;
    vk::CommandPool                commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;
    device.free( commandPool, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }
  {
    vk::Device                     device;
    vk::CommandPool                commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;
    device.free( commandPool, commandBuffers );
  }

  {
    vk::CommandBuffer          commandBuffer;
    vk::CommandBufferBeginInfo beginInfo;
    vk::Result                 result = commandBuffer.begin( &beginInfo );
  }
  {
    vk::CommandBuffer          commandBuffer;
    vk::CommandBufferBeginInfo beginInfo;
    commandBuffer.begin( beginInfo );
  }

  {
    vk::CommandBuffer commandBuffer;
    commandBuffer.end();
  }

  {
    vk::CommandBuffer           commandBuffer;
    vk::CommandBufferResetFlags flags = {};
    commandBuffer.reset( flags );
  }

#if 0
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    vk::Bool32         supported;
    uint32_t           queueFamilyIndex;
    vk::Result         result = physicalDevice.getSurfaceSupportKHR( queueFamilyIndex, surface, &supported );
  }
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    uint32_t           queueFamilyIndex;
    vk::Bool32         supported = physicalDevice.getSurfaceSupportKHR( queueFamilyIndex, surface );
  }
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    uint32_t           formatCount;
    vk::Result         result = physicalDevice.getSurfaceFormatsKHR( surface, &formatCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::SurfaceFormatKHR> formats( formatCount );
      result = physicalDevice.getSurfaceFormatsKHR( surface, &formatCount, formats.data() );
    }
  }
  {
    vk::PhysicalDevice                physicalDevice;
    vk::SurfaceKHR                    surface;
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR( surface );
  }
  {
    vk::PhysicalDevice                   physicalDevice;
    vk::SurfaceKHR                       surface;
    std::allocator<vk::SurfaceFormatKHR> allocator;
    std::vector<vk::SurfaceFormatKHR>    formats = physicalDevice.getSurfaceFormatsKHR( surface, allocator );
  }
#endif

  return 0;
}
