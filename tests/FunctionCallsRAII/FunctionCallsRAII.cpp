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
// VulkanHpp Samples : FunctionCallsRAII
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
#  include <cstdint>
import vulkan_hpp;
#else
#  include <vulkan/vulkan_raii.hpp>
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  //=== VK_VERSION_1_0 ===
  // Device initialization
  {
    vk::raii::Context      context;
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::raii::Instance     instance = context.createInstance( instanceCreateInfo );
  }
  {
    vk::raii::Context      context;
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::raii::Instance     instance( context, instanceCreateInfo );
  }

  {
    vk::raii::Instance                    instance        = nullptr;
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
  }
  {
    vk::raii::Instance        instance = nullptr;
    vk::raii::PhysicalDevices physicalDevices( instance );
  }

  {
    vk::raii::PhysicalDevice   physicalDevice         = nullptr;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
  }

  {
    vk::raii::PhysicalDevice physicalDevice   = nullptr;
    vk::Format               format           = {};
    vk::FormatProperties     formatProperties = physicalDevice.getFormatProperties( format );
  }

  {
    vk::raii::PhysicalDevice  physicalDevice = nullptr;
    vk::Format                format         = {};
    vk::ImageType             imageType      = {};
    vk::ImageTiling           imageTiling    = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties =
      physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags );
  }

  {
    vk::raii::PhysicalDevice     physicalDevice           = nullptr;
    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
  }

  {
    vk::raii::PhysicalDevice               physicalDevice        = nullptr;
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  }

  {
    vk::raii::PhysicalDevice           physicalDevice   = nullptr;
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
  }

  {
    vk::raii::Instance   instance     = nullptr;
    vk::PFN_VoidFunction voidFunction = instance.getProcAddr( "vkCreateInstance" );
  }

  {
    vk::raii::Device     device       = nullptr;
    vk::PFN_VoidFunction voidFunction = device.getProcAddr( "vkCreateInstance" );
  }

  // Device commands
  {
    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::DeviceCreateInfo     deviceCreateInfo;
    vk::raii::Device         device = physicalDevice.createDevice( deviceCreateInfo );
  }
  {
    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::DeviceCreateInfo     deviceCreateInfo;
    vk::raii::Device         device( physicalDevice, deviceCreateInfo );
  }

  // Extension discovery commands
  {
    vk::raii::Context                    context;
    std::vector<vk::ExtensionProperties> properties = context.enumerateInstanceExtensionProperties( nullptr );
  }

  {
    vk::raii::PhysicalDevice             physicalDevice = nullptr;
    std::vector<vk::ExtensionProperties> properties     = physicalDevice.enumerateDeviceExtensionProperties( nullptr );
  }

  // Layer discovery commands
  {
    vk::raii::Context                context;
    std::vector<vk::LayerProperties> properties = context.enumerateInstanceLayerProperties();
  }

  {
    vk::raii::PhysicalDevice         physicalDevice = nullptr;
    std::vector<vk::LayerProperties> properties     = physicalDevice.enumerateDeviceLayerProperties();
  }

  // Queue commands
  {
    vk::raii::Device device           = nullptr;
    uint32_t         queueFamilyIndex = 0;
    uint32_t         queueIndex       = 0;
    vk::raii::Queue  queue            = device.getQueue( queueFamilyIndex, queueIndex );
  }
  {
    vk::raii::Device device           = nullptr;
    uint32_t         queueFamilyIndex = 0;
    uint32_t         queueIndex       = 0;
    vk::raii::Queue  queue( device, queueFamilyIndex, queueIndex );
  }

  {
    vk::raii::Queue queue = nullptr;
    vk::SubmitInfo  submitInfo;
    vk::Fence       fence;
    queue.submit( submitInfo, fence );
  }

  {
    vk::raii::Queue queue = nullptr;
    queue.waitIdle();
  }

  {
    vk::raii::Device device = nullptr;
    device.waitIdle();
  }

  // Memory commands
  {
    vk::raii::Device       device = nullptr;
    vk::MemoryAllocateInfo memoryAllocateInfo;
    vk::raii::DeviceMemory deviceMemory = device.allocateMemory( memoryAllocateInfo );
  }
  {
    vk::raii::Device       device = nullptr;
    vk::MemoryAllocateInfo memoryAllocateInfo;
    vk::raii::DeviceMemory deviceMemory( device, memoryAllocateInfo );
  }

  {
    vk::raii::DeviceMemory deviceMemory   = nullptr;
    vk::DeviceSize         offset         = 0;
    vk::DeviceSize         size           = vk::WholeSize;
    vk::MemoryMapFlags     memoryMapFlags = {};
    void *                 pData          = deviceMemory.mapMemory( offset, size, memoryMapFlags );
  }

  {
    vk::raii::DeviceMemory deviceMemory = nullptr;
    deviceMemory.unmapMemory();
  }

  {
    vk::raii::Device                   device = nullptr;
    std::vector<vk::MappedMemoryRange> mappedMemoryRanges;
    device.flushMappedMemoryRanges( mappedMemoryRanges );
  }

  {
    vk::raii::Device                   device = nullptr;
    std::vector<vk::MappedMemoryRange> mappedMemoryRanges;
    device.invalidateMappedMemoryRanges( mappedMemoryRanges );
  }

  {
    vk::raii::DeviceMemory deviceMemory = nullptr;
    vk::DeviceSize         size         = deviceMemory.getCommitment();
  }

  // Memory management API commands
  {
    vk::raii::Buffer buffer = nullptr;
    vk::DeviceMemory deviceMemory;
    vk::DeviceSize   memoryOffset = 0;
    buffer.bindMemory( deviceMemory, memoryOffset );
  }

  {
    vk::raii::Image  image = nullptr;
    vk::DeviceMemory deviceMemory;
    vk::DeviceSize   memoryOffset = 0;
    image.bindMemory( deviceMemory, memoryOffset );
  }

  {
    vk::raii::Buffer       buffer             = nullptr;
    vk::MemoryRequirements memoryRequirements = buffer.getMemoryRequirements();
  }

  {
    vk::raii::Image        image              = nullptr;
    vk::MemoryRequirements memoryRequirements = image.getMemoryRequirements();
  }

  // Sparse resource memory management API commands (optional)
  {
    vk::raii::Image                                image                         = nullptr;
    std::vector<vk::SparseImageMemoryRequirements> sparseImageMemoryRequirements = image.getSparseMemoryRequirements();
  }

  {
    vk::raii::PhysicalDevice                     physicalDevice = nullptr;
    vk::Format                                   format         = {};
    vk::ImageType                                type           = {};
    vk::SampleCountFlagBits                      samples        = {};
    vk::ImageUsageFlags                          usage          = {};
    vk::ImageTiling                              tiling         = {};
    std::vector<vk::SparseImageFormatProperties> sparseImageFormatProperties =
      physicalDevice.getSparseImageFormatProperties( format, type, samples, usage, tiling );
  }

  {
    vk::raii::Queue    queue = nullptr;
    vk::BindSparseInfo bindSparseInfo;
    vk::Fence          fence;
    queue.bindSparse( bindSparseInfo, fence );
  }

  // Fence commands
  {
    vk::raii::Device    device = nullptr;
    vk::FenceCreateInfo fenceCreateInfo;
    vk::raii::Fence     fence = device.createFence( fenceCreateInfo );
  }
  {
    vk::raii::Device    device = nullptr;
    vk::FenceCreateInfo fenceCreateInfo;
    vk::raii::Fence     fence( device, fenceCreateInfo );
  }

  {
    vk::raii::Device device = nullptr;
    vk::Fence        fence;
    device.resetFences( fence );
  }

  {
    vk::raii::Fence fence  = nullptr;
    vk::Result      result = fence.getStatus();
  }

  {
    vk::raii::Device device = nullptr;
    vk::Fence        fence;
    vk::Bool32       waitAll = vk::True;
    uint64_t         timeout = 1000000000;
    vk::Result       result  = device.waitForFences( fence, waitAll, timeout );
  }

  // Queue semaphore commands
  {
    vk::raii::Device        device = nullptr;
    vk::SemaphoreCreateInfo semaphoreCreateInfo;
    vk::raii::Semaphore     semaphore = device.createSemaphore( semaphoreCreateInfo );
  }
  {
    vk::raii::Device        device = nullptr;
    vk::SemaphoreCreateInfo semaphoreCreateInfo;
    vk::raii::Semaphore     semaphore( device, semaphoreCreateInfo );
  }

  // Query commands
  {
    vk::raii::Device        device = nullptr;
    vk::QueryPoolCreateInfo queryPoolCreateInfo;
    vk::raii::QueryPool     queryPool = device.createQueryPool( queryPoolCreateInfo );
  }
  {
    vk::raii::Device        device = nullptr;
    vk::QueryPoolCreateInfo queryPoolCreateInfo;
    vk::raii::QueryPool     queryPool( device, queryPoolCreateInfo );
  }

  {
    vk::raii::QueryPool                    queryPool   = nullptr;
    uint32_t                               firstQuery  = 0;
    uint32_t                               queryCount  = 1;
    size_t                                 dataSize    = sizeof( uint32_t );
    vk::DeviceSize                         stride      = sizeof( uint32_t );
    vk::QueryResultFlagBits                flags       = {};
    vk::ResultValue<std::vector<uint32_t>> resultValue = queryPool.getResults<uint32_t>( firstQuery, queryCount, dataSize, stride, flags );
  }
  {
    vk::raii::QueryPool       queryPool   = nullptr;
    uint32_t                  firstQuery  = 0;
    uint32_t                  queryCount  = 1;
    vk::DeviceSize            stride      = sizeof( uint32_t );
    vk::QueryResultFlagBits   flags       = {};
    vk::ResultValue<uint32_t> resultValue = queryPool.getResult<uint32_t>( firstQuery, queryCount, stride, flags );
  }

  return 0;
}
