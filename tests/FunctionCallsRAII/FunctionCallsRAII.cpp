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
import vulkan;
#else
#  include <vulkan/vulkan_raii.hpp>
#endif

int main()
{
  //======================================
  // Vulkan base API interface definitions
  //======================================

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
    vk::raii::Fence fence = nullptr;
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
    vk::raii::Buffer       buffer       = nullptr;
    vk::raii::DeviceMemory deviceMemory = nullptr;
    vk::DeviceSize         memoryOffset = 0;
    buffer.bindMemory( deviceMemory, memoryOffset );
  }

  {
    vk::raii::Image        image        = nullptr;
    vk::raii::DeviceMemory deviceMemory = nullptr;
    vk::DeviceSize         memoryOffset = 0;
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
    vk::raii::Device       device = nullptr;
    std::vector<vk::Fence> fences;
    device.resetFences( fences );
  }

  {
    vk::raii::Fence fence  = nullptr;
    vk::Result      result = fence.getStatus();
  }

  {
    vk::raii::Device       device = nullptr;
    std::vector<vk::Fence> fences;
    vk::Bool32             waitAll = vk::True;
    uint64_t               timeout = 1000000000;
    vk::Result             result  = device.waitForFences( fences, waitAll, timeout );
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
    std::size_t                            dataSize    = sizeof( uint32_t );
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

  // Buffer commands
  {
    vk::raii::Device     device = nullptr;
    vk::BufferCreateInfo bufferCreateInfo;
    vk::raii::Buffer     buffer = device.createBuffer( bufferCreateInfo );
  }
  {
    vk::raii::Device     device = nullptr;
    vk::BufferCreateInfo bufferCreateInfo;
    vk::raii::Buffer     buffer( device, bufferCreateInfo );
  }

  // Image commands
  {
    vk::raii::Device    device = nullptr;
    vk::ImageCreateInfo imageCreateInfo;
    vk::raii::Image     image = device.createImage( imageCreateInfo );
  }
  {
    vk::raii::Device    device = nullptr;
    vk::ImageCreateInfo imageCreateInfo;
    vk::raii::Image     image( device, imageCreateInfo );
  }

  {
    vk::raii::Image       image = nullptr;
    vk::ImageSubresource  imageSubresource;
    vk::SubresourceLayout subresourceLayout = image.getSubresourceLayout( imageSubresource );
  }

  // Image view commands
  {
    vk::raii::Device        device = nullptr;
    vk::ImageViewCreateInfo imageViewCreateInfo;
    vk::raii::ImageView     imageView = device.createImageView( imageViewCreateInfo );
  }
  {
    vk::raii::Device        device = nullptr;
    vk::ImageViewCreateInfo imageViewCreateInfo;
    vk::raii::ImageView     imageView( device, imageViewCreateInfo );
  }

  // Command pool commands
  {
    vk::raii::Device          device = nullptr;
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    vk::raii::CommandPool     commandPool = device.createCommandPool( commandPoolCreateInfo );
  }
  {
    vk::raii::Device          device = nullptr;
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    vk::raii::CommandPool     commandPool( device, commandPoolCreateInfo );
  }

  {
    vk::raii::CommandPool     commandPool = nullptr;
    vk::CommandPoolResetFlags flags       = {};
    commandPool.reset( flags );
  }

  // Command buffer commands
  {
    vk::raii::Device                     device = nullptr;
    vk::CommandBufferAllocateInfo        commandBufferAllocateInfo;
    std::vector<vk::raii::CommandBuffer> commandBuffers = device.allocateCommandBuffers( commandBufferAllocateInfo );
  }
  {
    vk::raii::Device              device = nullptr;
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
    vk::raii::CommandBuffers      commandBuffers( device, commandBufferAllocateInfo );
  }

  {
    vk::raii::CommandBuffer    commandBuffer = nullptr;
    vk::CommandBufferBeginInfo beginInfo;
    commandBuffer.begin( beginInfo );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    commandBuffer.end();
  }

  {
    vk::raii::CommandBuffer     commandBuffer = nullptr;
    vk::CommandBufferResetFlags flags         = {};
    commandBuffer.reset( flags );
  }

  // Command buffer building commands
  {
    vk::raii::CommandBuffer     commandBuffer = nullptr;
    vk::raii::Buffer            srcBuffer     = nullptr;
    vk::raii::Buffer            dstBuffer     = nullptr;
    std::vector<vk::BufferCopy> copyRegions;
    commandBuffer.copyBuffer( srcBuffer, dstBuffer, copyRegions );
  }

  {
    vk::raii::CommandBuffer    commandBuffer  = nullptr;
    vk::raii::Image            srcImage       = nullptr;
    vk::ImageLayout            srcImageLayout = {};
    vk::raii::Image            dstImage       = nullptr;
    vk::ImageLayout            dstImageLayout = {};
    std::vector<vk::ImageCopy> copyRegions;
    commandBuffer.copyImage( srcImage, srcImageLayout, dstImage, dstImageLayout, copyRegions );
  }

  {
    vk::raii::CommandBuffer          commandBuffer = nullptr;
    vk::raii::Buffer                 buffer        = nullptr;
    vk::raii::Image                  image         = nullptr;
    vk::ImageLayout                  imageLayout   = {};
    std::vector<vk::BufferImageCopy> regions;
    commandBuffer.copyBufferToImage( buffer, image, imageLayout, regions );
  }

  {
    vk::raii::CommandBuffer          commandBuffer = nullptr;
    vk::raii::Image                  image         = nullptr;
    vk::ImageLayout                  imageLayout   = {};
    vk::raii::Buffer                 buffer        = nullptr;
    std::vector<vk::BufferImageCopy> regions;
    commandBuffer.copyImageToBuffer( image, imageLayout, buffer, regions );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::Buffer        dstBuffer     = nullptr;
    vk::DeviceSize          dstOffset     = 0;
    std::vector<uint8_t>    data( 1024 );
    commandBuffer.updateBuffer<uint8_t>( dstBuffer, dstOffset, data );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::Buffer        buffer        = nullptr;
    vk::DeviceSize          offset        = 0;
    vk::DeviceSize          size          = vk::WholeSize;
    commandBuffer.fillBuffer( buffer, offset, size, 0 );
  }

  {
    vk::raii::CommandBuffer              commandBuffer   = nullptr;
    vk::PipelineStageFlagBits            srcStageMask    = {};
    vk::PipelineStageFlagBits            dstStageMask    = {};
    vk::DependencyFlags                  dependencyFlags = {};
    std::vector<vk::MemoryBarrier>       memoryBarriers;
    std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers;
    std::vector<vk::ImageMemoryBarrier>  imageMemoryBarriers;
    commandBuffer.pipelineBarrier( srcStageMask, dstStageMask, dependencyFlags, memoryBarriers, bufferMemoryBarriers, imageMemoryBarriers );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::QueryPool     queryPool     = nullptr;
    uint32_t                query         = 0;
    vk::QueryControlFlags   flags         = {};
    commandBuffer.beginQuery( queryPool, query, flags );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::QueryPool     queryPool     = nullptr;
    uint32_t                query         = 0;
    commandBuffer.endQuery( queryPool, query );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::QueryPool     queryPool     = nullptr;
    uint32_t                firstQuery    = 0;
    uint32_t                queryCount    = 1;
    commandBuffer.resetQueryPool( queryPool, firstQuery, queryCount );
  }

  {
    vk::raii::CommandBuffer   commandBuffer = nullptr;
    vk::PipelineStageFlagBits stage         = {};
    vk::raii::QueryPool       queryPool     = nullptr;
    uint32_t                  query         = 0;
    commandBuffer.writeTimestamp( stage, queryPool, query );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::QueryPool     queryPool     = nullptr;
    uint32_t                firstQuery    = 0;
    uint32_t                queryCount    = 1;
    vk::raii::Buffer        dstBuffer     = nullptr;
    vk::DeviceSize          dstOffset     = 0;
    vk::DeviceSize          stride        = 0;
    vk::QueryResultFlags    flags         = {};
    commandBuffer.copyQueryPoolResults( queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags );
  }

  {
    vk::raii::CommandBuffer        commandBuffer = nullptr;
    std::vector<vk::CommandBuffer> secondaryCommandBuffers;
    commandBuffer.executeCommands( secondaryCommandBuffers );
  }

  //=========================================
  // Vulkan compute API interface definitions
  //=========================================

  // Event commands
  {
    vk::raii::Device    device = nullptr;
    vk::EventCreateInfo eventCreateInfo;
    vk::raii::Event     event = device.createEvent( eventCreateInfo );
  }
  {
    vk::raii::Device    device = nullptr;
    vk::EventCreateInfo eventCreateInfo;
    vk::raii::Event     event( device, eventCreateInfo );
  }

  {
    vk::raii::Event event  = nullptr;
    vk::Result      result = event.getStatus();
  }

  {
    vk::raii::Event event = nullptr;
    event.set();
  }

  {
    vk::raii::Event event = nullptr;
    event.reset();
  }

  // Buffer view commands
  {
    vk::raii::Device         device = nullptr;
    vk::BufferViewCreateInfo bufferViewCreateInfo;
    vk::raii::BufferView     bufferView = device.createBufferView( bufferViewCreateInfo );
  }
  {
    vk::raii::Device         device = nullptr;
    vk::BufferViewCreateInfo bufferViewCreateInfo;
    vk::raii::BufferView     bufferView( device, bufferViewCreateInfo );
  }

  // Shader commands
  {
    vk::raii::Device           device = nullptr;
    vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
    vk::raii::ShaderModule     shaderModule = device.createShaderModule( shaderModuleCreateInfo );
  }
  {
    vk::raii::Device           device = nullptr;
    vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
    vk::raii::ShaderModule     shaderModule( device, shaderModuleCreateInfo );
  }

  // Pipeline Cache commands
  {
    vk::raii::Device            device = nullptr;
    vk::PipelineCacheCreateInfo pipelineCacheCreateInfo;
    vk::raii::PipelineCache     pipelineCache = device.createPipelineCache( pipelineCacheCreateInfo );
  }
  {
    vk::raii::Device            device = nullptr;
    vk::PipelineCacheCreateInfo pipelineCacheCreateInfo;
    vk::raii::PipelineCache     pipelineCache( device, pipelineCacheCreateInfo );
  }

  {
    vk::raii::PipelineCache pipelineCache = nullptr;
    std::vector<uint8_t>    data          = pipelineCache.getData();
  }

  {
    vk::raii::PipelineCache        dstCache = nullptr;
    std::vector<vk::PipelineCache> srcCaches;
    dstCache.merge( srcCaches );
  }

  // Compute pipeline commands
  {
    vk::raii::Device                           device        = nullptr;
    vk::raii::PipelineCache                    pipelineCache = nullptr;
    std::vector<vk::ComputePipelineCreateInfo> computePipelineCreateInfos;
    std::vector<vk::raii::Pipeline>            pipelines = device.createComputePipelines( pipelineCache, computePipelineCreateInfos );
  }
  {
    vk::raii::Device                           device        = nullptr;
    vk::raii::PipelineCache                    pipelineCache = nullptr;
    std::vector<vk::ComputePipelineCreateInfo> computePipelineCreateInfos;
    vk::raii::Pipelines                        pipelines( device, pipelineCache, computePipelineCreateInfos );
  }
  {
    vk::raii::Device              device        = nullptr;
    vk::raii::PipelineCache       pipelineCache = nullptr;
    vk::ComputePipelineCreateInfo computePipelineCreateInfo;
    vk::raii::Pipeline            pipeline = device.createComputePipeline( pipelineCache, computePipelineCreateInfo );
  }
  {
    vk::raii::Device              device        = nullptr;
    vk::raii::PipelineCache       pipelineCache = nullptr;
    vk::ComputePipelineCreateInfo computePipelineCreateInfo;
    vk::raii::Pipeline            pipeline( device, pipelineCache, computePipelineCreateInfo );
  }

  // Pipeline layout commands
  {
    vk::raii::Device             device = nullptr;
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    vk::raii::PipelineLayout     pipelineLayout = device.createPipelineLayout( pipelineLayoutCreateInfo );
  }
  {
    vk::raii::Device             device = nullptr;
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    vk::raii::PipelineLayout     pipelineLayout( device, pipelineLayoutCreateInfo );
  }

  // Sampler commands
  {
    vk::raii::Device      device = nullptr;
    vk::SamplerCreateInfo samplerCreateInfo;
    vk::raii::Sampler     sampler = device.createSampler( samplerCreateInfo );
  }
  {
    vk::raii::Device      device = nullptr;
    vk::SamplerCreateInfo samplerCreateInfo;
    vk::raii::Sampler     sampler( device, samplerCreateInfo );
  }

  // Descriptor set commands
  {
    vk::raii::Device                  device = nullptr;
    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    vk::raii::DescriptorSetLayout     descriptorSetLayout = device.createDescriptorSetLayout( descriptorSetLayoutCreateInfo );
  }
  {
    vk::raii::Device                  device = nullptr;
    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    vk::raii::DescriptorSetLayout     descriptorSetLayout( device, descriptorSetLayoutCreateInfo );
  }

  {
    vk::raii::Device             device = nullptr;
    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    vk::raii::DescriptorPool     descriptorPool = device.createDescriptorPool( descriptorPoolCreateInfo );
  }
  {
    vk::raii::Device             device = nullptr;
    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    vk::raii::DescriptorPool     descriptorPool( device, descriptorPoolCreateInfo );
  }

  {
    vk::raii::DescriptorPool     descriptorPool = nullptr;
    vk::DescriptorPoolResetFlags flags          = {};
    descriptorPool.reset( flags );
  }

  {
    vk::raii::Device                     device = nullptr;
    vk::DescriptorSetAllocateInfo        descriptorSetAllocateInfo;
    std::vector<vk::raii::DescriptorSet> descriptorSets = device.allocateDescriptorSets( descriptorSetAllocateInfo );
  }

  {
    vk::raii::Device                    device = nullptr;
    std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
    std::vector<vk::CopyDescriptorSet>  copyDescriptorSets;
    device.updateDescriptorSets( writeDescriptorSets, copyDescriptorSets );
  }

  // Command buffer building commands
  {
    vk::raii::CommandBuffer commandBuffer     = nullptr;
    vk::PipelineBindPoint   pipelineBindPoint = {};
    vk::raii::Pipeline      pipeline          = nullptr;
    commandBuffer.bindPipeline( pipelineBindPoint, pipeline );
  }

  {
    vk::raii::CommandBuffer        commandBuffer     = nullptr;
    vk::PipelineBindPoint          pipelineBindPoint = {};
    vk::raii::PipelineLayout       pipelineLayout    = nullptr;
    uint32_t                       firstSet          = 0;
    std::vector<vk::DescriptorSet> descriptorSets;
    std::vector<uint32_t>          dynamicOffsets;
    commandBuffer.bindDescriptorSets( pipelineBindPoint, pipelineLayout, firstSet, descriptorSets, dynamicOffsets );
  }

  {
    vk::raii::CommandBuffer                commandBuffer = nullptr;
    vk::raii::Image                        image         = nullptr;
    vk::ImageLayout                        imageLayout   = {};
    vk::ClearColorValue                    clearColor    = {};
    std::vector<vk::ImageSubresourceRange> ranges;
    commandBuffer.clearColorImage( image, imageLayout, clearColor, ranges );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    uint32_t                groupCountX   = 1;
    uint32_t                groupCountY   = 1;
    uint32_t                groupCountZ   = 1;
    commandBuffer.dispatch( groupCountX, groupCountY, groupCountZ );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::Buffer        buffer        = nullptr;
    vk::DeviceSize          offset        = 0;
    commandBuffer.dispatchIndirect( buffer, offset );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::Event         event         = nullptr;
    vk::PipelineStageFlags  stageMask     = {};
    commandBuffer.setEvent( event, stageMask );
  }

  {
    vk::raii::CommandBuffer commandBuffer = nullptr;
    vk::raii::Event         event         = nullptr;
    vk::PipelineStageFlags  stageMask     = {};
    commandBuffer.resetEvent( event, stageMask );
  }

  {
    vk::raii::CommandBuffer              commandBuffer = nullptr;
    std::vector<vk::Event>               events;
    vk::PipelineStageFlags               srcStageMask = {};
    vk::PipelineStageFlags               dstStageMask = {};
    std::vector<vk::MemoryBarrier>       memoryBarriers;
    std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers;
    std::vector<vk::ImageMemoryBarrier>  imageMemoryBarriers;
    commandBuffer.waitEvents( events, srcStageMask, dstStageMask, memoryBarriers, bufferMemoryBarriers, imageMemoryBarriers );
  }

  {
    vk::raii::CommandBuffer  commandBuffer  = nullptr;
    vk::raii::PipelineLayout pipelineLayout = nullptr;
    vk::ShaderStageFlags     stageFlags     = {};
    uint32_t                 offset         = 0;
    std::vector<uint32_t>    values;
    commandBuffer.pushConstants( pipelineLayout, stageFlags, offset, static_cast<uint32_t>( values.size() * sizeof( uint32_t ) ), values.data() );
  }
  {
    vk::raii::CommandBuffer  commandBuffer  = nullptr;
    vk::raii::PipelineLayout pipelineLayout = nullptr;
    vk::ShaderStageFlags     stageFlags     = {};
    uint32_t                 offset         = 0;
    std::vector<uint32_t>    values;
    commandBuffer.pushConstants<uint32_t>( pipelineLayout, stageFlags, offset, values );
  }

  //==========================================
  // Vulkan graphics API interface definitions
  //==========================================

  // Graphics Pipeline commands
  {
    vk::raii::Device                            device        = nullptr;
    vk::raii::PipelineCache                     pipelineCache = nullptr;
    std::vector<vk::GraphicsPipelineCreateInfo> graphicsPipelineCreateInfos;
    std::vector<vk::raii::Pipeline>             pipelines = device.createGraphicsPipelines( pipelineCache, graphicsPipelineCreateInfos );
  }
  {
    vk::raii::Device               device        = nullptr;
    vk::raii::PipelineCache        pipelineCache = nullptr;
    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
    vk::raii::Pipeline             pipelines = device.createGraphicsPipeline( pipelineCache, graphicsPipelineCreateInfo );
  }
  {
    vk::raii::Device                            device        = nullptr;
    vk::raii::PipelineCache                     pipelineCache = nullptr;
    std::vector<vk::GraphicsPipelineCreateInfo> graphicsPipelineCreateInfos;
    vk::raii::Pipelines                         pipelines( device, pipelineCache, graphicsPipelineCreateInfos );
  }
  {
    vk::raii::Device               device        = nullptr;
    vk::raii::PipelineCache        pipelineCache = nullptr;
    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
    vk::raii::Pipeline             pipeline( device, pipelineCache, graphicsPipelineCreateInfo );
  }

  // Pass commands
  {
    vk::raii::Device          device = nullptr;
    vk::FramebufferCreateInfo framebufferCreateInfo;
    vk::raii::Framebuffer     framebuffer = device.createFramebuffer( framebufferCreateInfo );
  }
  {
    vk::raii::Device          device = nullptr;
    vk::FramebufferCreateInfo framebufferCreateInfo;
    vk::raii::Framebuffer     framebuffer( device, framebufferCreateInfo );
  }

  {
    vk::raii::Device         device = nullptr;
    vk::RenderPassCreateInfo renderPassCreateInfo;
    vk::raii::RenderPass     renderPass = device.createRenderPass( renderPassCreateInfo );
  }
  {
    vk::raii::Device         device = nullptr;
    vk::RenderPassCreateInfo renderPassCreateInfo;
    vk::raii::RenderPass     renderPass( device, renderPassCreateInfo );
  }

  {
    vk::raii::RenderPass renderPass  = nullptr;
    vk::Extent2D         granularity = renderPass.getRenderAreaGranularity();
  }

  return 0;
}
