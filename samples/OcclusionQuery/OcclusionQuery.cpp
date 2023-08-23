// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : OcclusionQuery
//                     Use occlusion query to determine if drawing renders any samples.

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>

static char const * AppName    = "OcclusionQuery";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::Device                    device = vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::CommandPool   commandPool = device.createCommandPool( { {}, graphicsAndPresentQueueFamilyIndex.first } );
    vk::CommandBuffer commandBuffer =
      device.allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 ) ).front();

    vk::Queue graphicsQueue = device.getQueue( graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::Queue presentQueue  = device.getQueue( graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device,
                                         surfaceData.surface,
                                         surfaceData.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                         {},
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4        mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::su::copyToDevice( device, uniformBufferData.deviceMemory, mvpcMatrix );

    vk::DescriptorSetLayout descriptorSetLayout =
      vk::su::createDescriptorSetLayout( device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    vk::RenderPass renderPass = vk::su::createRenderPass(
      device, vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format, depthBufferData.format );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData( physicalDevice, device, sizeof( coloredCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, coloredCubeData, sizeof( coloredCubeData ) / sizeof( coloredCubeData[0] ) );

    vk::DescriptorPool            descriptorPool = vk::su::createDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 1 } } );
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayout );
    vk::DescriptorSet             descriptorSet = device.allocateDescriptorSets( descriptorSetAllocateInfo ).front();

    vk::su::updateDescriptorSets( device, descriptorSet, { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, {} );

    vk::PipelineCache pipelineCache    = device.createPipelineCache( vk::PipelineCacheCreateInfo() );
    vk::Pipeline      graphicsPipeline = vk::su::createGraphicsPipeline( device,
                                                                    pipelineCache,
                                                                    std::make_pair( vertexShaderModule, nullptr ),
                                                                    std::make_pair( fragmentShaderModule, nullptr ),
                                                                    sizeof( coloredCubeData[0] ),
                                                                         { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32B32A32Sfloat, 16 } },
                                                                    vk::FrontFace::eClockwise,
                                                                    true,
                                                                    pipelineLayout,
                                                                    renderPass );

    /* VULKAN_KEY_START */

    vk::Semaphore imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) );

    // Get the index of the next available swapchain image:
    vk::ResultValue<uint32_t> currentBuffer = device.acquireNextImageKHR( swapChainData.swapChain, UINT64_MAX, imageAcquiredSemaphore, nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    /* Allocate a uniform buffer that will take query results. */
    vk::Buffer queryResultBuffer = device.createBuffer( vk::BufferCreateInfo(
      vk::BufferCreateFlags(), 4 * sizeof( uint64_t ), vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst ) );

    vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements( queryResultBuffer );
    uint32_t               memoryTypeIndex    = vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                                                       memoryRequirements.memoryTypeBits,
                                                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::DeviceMemory       queryResultMemory  = device.allocateMemory( vk::MemoryAllocateInfo( memoryRequirements.size, memoryTypeIndex ) );

    device.bindBufferMemory( queryResultBuffer, queryResultMemory, 0 );

    vk::QueryPool queryPool =
      device.createQueryPool( vk::QueryPoolCreateInfo( vk::QueryPoolCreateFlags(), vk::QueryType::eOcclusion, 2, vk::QueryPipelineStatisticFlags() ) );

    commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    commandBuffer.resetQueryPool( queryPool, 0, 2 );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    commandBuffer.beginRenderPass(
      vk::RenderPassBeginInfo( renderPass, framebuffers[currentBuffer.value], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );

    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, {} );

    commandBuffer.bindVertexBuffers( 0, vertexBufferData.buffer, { 0 } );
    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer.beginQuery( queryPool, 0, vk::QueryControlFlags() );
    commandBuffer.endQuery( queryPool, 0 );

    commandBuffer.beginQuery( queryPool, 1, vk::QueryControlFlags() );
    commandBuffer.draw( 12 * 3, 1, 0, 0 );
    commandBuffer.endRenderPass();
    commandBuffer.endQuery( queryPool, 1 );

    commandBuffer.copyQueryPoolResults(
      queryPool, 0, 2, queryResultBuffer, 0, sizeof( uint64_t ), vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait );
    commandBuffer.end();

    vk::Fence drawFence = device.createFence( vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( imageAcquiredSemaphore, waitDestinationStageMask, commandBuffer );
    graphicsQueue.submit( submitInfo, drawFence );

    graphicsQueue.waitIdle();

    vk::ResultValue<std::vector<uint64_t>> rv = device.getQueryPoolResults<uint64_t>(
      queryPool, 0, 2, 2 * sizeof( uint64_t ), sizeof( uint64_t ), vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait );
    switch ( rv.result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eNotReady: std::cout << "vk::Device::getQueryPoolResults returned vk::Result::eNotReady !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }

    std::cout << "vkGetQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << rv.value[0] << "\n";
    std::cout << "samples_passed[1] = " << rv.value[1] << "\n";

    /* Read back query result from buffer */
    uint64_t * samplesPassedPtr = static_cast<uint64_t *>( device.mapMemory( queryResultMemory, 0, memoryRequirements.size, vk::MemoryMapFlags() ) );

    std::cout << "vkCmdCopyQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << samplesPassedPtr[0] << "\n";
    std::cout << "samples_passed[1] = " << samplesPassedPtr[1] << "\n";

    device.unmapMemory( queryResultMemory );

    while ( vk::Result::eTimeout == device.waitForFences( drawFence, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::Result result = presentQueue.presentKHR( vk::PresentInfoKHR( {}, swapChainData.swapChain, currentBuffer.value ) );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    device.destroyFence( drawFence );
    device.destroyQueryPool( queryPool );
    device.destroyBuffer( queryResultBuffer );  // destroy the queryResultBuffer before freeing the bound queryResultMemory !
    device.freeMemory( queryResultMemory );
    device.destroySemaphore( imageAcquiredSemaphore );

    /* VULKAN_KEY_END */

    device.destroyPipeline( graphicsPipeline );
    device.destroyPipelineCache( pipelineCache );
    device.freeDescriptorSets( descriptorPool, descriptorSet );
    device.destroyDescriptorPool( descriptorPool );
    vertexBufferData.clear( device );
    for ( auto framebuffer : framebuffers )
    {
      device.destroyFramebuffer( framebuffer );
    }
    device.destroyShaderModule( fragmentShaderModule );
    device.destroyShaderModule( vertexShaderModule );
    device.destroyRenderPass( renderPass );
    device.destroyPipelineLayout( pipelineLayout );
    device.destroyDescriptorSetLayout( descriptorSetLayout );
    uniformBufferData.clear( device );
    depthBufferData.clear( device );
    swapChainData.clear( device );
    device.freeCommandBuffers( commandPool, commandBuffer );
    device.destroyCommandPool( commandPool );
    device.destroy();
    instance.destroySurfaceKHR( surfaceData.surface );
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
