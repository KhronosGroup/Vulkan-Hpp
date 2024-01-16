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

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
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
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( vk::Format::eR8G8B8A8Unorm );
    if ( !( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment ) )
    {
      std::cout << "vk::Format::eR8G8B8A8Unorm format unsupported for input attachment\n";
      exit( -1 );
    }

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::raii::Device device = vk::raii::su::makeDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::raii::CommandPool   commandPool   = vk::raii::CommandPool( device, { {}, graphicsAndPresentQueueFamilyIndex.first } );
    vk::raii::CommandBuffer commandBuffer = vk::raii::su::makeCommandBuffer( device, commandPool );

    vk::raii::Queue graphicsQueue( device, graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::raii::Queue presentQueue( device, graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::raii::su::SwapChainData swapChainData( physicalDevice,
                                               device,
                                               surfaceData.surface,
                                               surfaceData.extent,
                                               vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                               {},
                                               graphicsAndPresentQueueFamilyIndex.first,
                                               graphicsAndPresentQueueFamilyIndex.second );

    vk::raii::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::raii::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4              mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::raii::su::copyToDevice( uniformBufferData.deviceMemory, mvpcMatrix );

    vk::raii::DescriptorSetLayout descriptorSetLayout =
      vk::raii::su::makeDescriptorSetLayout( device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::raii::PipelineLayout pipelineLayout( device, { {}, *descriptorSetLayout } );

    vk::Format           colorFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format;
    vk::raii::RenderPass renderPass  = vk::raii::su::makeRenderPass( device, colorFormat, depthBufferData.format );

    glslang::InitializeProcess();
    vk::raii::ShaderModule vertexShaderModule   = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C );
    vk::raii::ShaderModule fragmentShaderModule = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    std::vector<vk::raii::Framebuffer> framebuffers =
      vk::raii::su::makeFramebuffers( device, renderPass, swapChainData.imageViews, &depthBufferData.imageView, surfaceData.extent );

    vk::raii::su::BufferData vertexBufferData( physicalDevice, device, sizeof( coloredCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::su::copyToDevice( vertexBufferData.deviceMemory, coloredCubeData, sizeof( coloredCubeData ) / sizeof( coloredCubeData[0] ) );

    vk::raii::DescriptorPool descriptorPool = vk::raii::su::makeDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 1 } } );
    vk::raii::DescriptorSet  descriptorSet  = std::move( vk::raii::DescriptorSets( device, { descriptorPool, *descriptorSetLayout } ).front() );

    vk::raii::su::updateDescriptorSets(
      device, descriptorSet, { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, nullptr } }, {} );

    vk::raii::PipelineCache pipelineCache( device, vk::PipelineCacheCreateInfo() );
    vk::raii::Pipeline      graphicsPipeline = vk::raii::su::makeGraphicsPipeline( device,
                                                                              pipelineCache,
                                                                              vertexShaderModule,
                                                                              nullptr,
                                                                              fragmentShaderModule,
                                                                              nullptr,
                                                                              sizeof( coloredCubeData[0] ),
                                                                                   { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                              vk::FrontFace::eClockwise,
                                                                              true,
                                                                              pipelineLayout,
                                                                              renderPass );

    /* VULKAN_KEY_START */

    vk::raii::Semaphore imageAcquiredSemaphore( device, vk::SemaphoreCreateInfo() );

    // Get the index of the next available swapchain image:
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) = swapChainData.swapChain.acquireNextImage( vk::su::FenceTimeout, imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    // in order to get a clean desctruction sequence, instantiate the DeviceMemory for the result buffer first
    vk::raii::DeviceMemory queryResultMemory( nullptr );

    /* Allocate a uniform buffer that will take query results. */
    vk::BufferCreateInfo bufferCreateInfo( {}, 4 * sizeof( uint64_t ), vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst );
    vk::raii::Buffer     queryResultBuffer( device, bufferCreateInfo );

    vk::MemoryRequirements memoryRequirements = queryResultBuffer.getMemoryRequirements();
    uint32_t               memoryTypeIndex    = vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                                                       memoryRequirements.memoryTypeBits,
                                                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    queryResultMemory = vk::raii::DeviceMemory( device, memoryAllocateInfo );

    queryResultBuffer.bindMemory( queryResultMemory, 0 );

    vk::QueryPoolCreateInfo queryPoolCreateInfo( {}, vk::QueryType::eOcclusion, 2, {} );
    vk::raii::QueryPool     queryPool( device, queryPoolCreateInfo );

    commandBuffer.begin( {} );
    commandBuffer.resetQueryPool( queryPool, 0, 2 );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    commandBuffer.beginRenderPass(
      vk::RenderPassBeginInfo( renderPass, framebuffers[imageIndex], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );

    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSet }, {} );

    commandBuffer.bindVertexBuffers( 0, { vertexBufferData.buffer }, { 0 } );
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

    vk::raii::Fence drawFence( device, vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( *imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer );
    graphicsQueue.submit( submitInfo, *drawFence );

    graphicsQueue.waitIdle();

    std::vector<uint64_t> poolResults;
    std::tie( result, poolResults ) =
      queryPool.getResults<uint64_t>( 0, 2, 2 * sizeof( uint64_t ), sizeof( uint64_t ), vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eNotReady: std::cout << "vk::Device::getQueryPoolResults returned vk::Result::eNotReady !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }

    std::cout << "vkGetQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << poolResults[0] << "\n";
    std::cout << "samples_passed[1] = " << poolResults[1] << "\n";

    /* Read back query result from buffer */
    uint64_t * samplesPassedPtr = static_cast<uint64_t *>( queryResultMemory.mapMemory( 0, memoryRequirements.size, vk::MemoryMapFlags() ) );

    std::cout << "vkCmdCopyQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << samplesPassedPtr[0] << "\n";
    std::cout << "samples_passed[1] = " << samplesPassedPtr[1] << "\n";

    queryResultMemory.unmapMemory();

    while ( vk::Result::eTimeout == device.waitForFences( { drawFence }, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::PresentInfoKHR presentInfoKHR( nullptr, *swapChainData.swapChain, imageIndex );
    result = presentQueue.presentKHR( presentInfoKHR );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    /* VULKAN_KEY_END */
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
