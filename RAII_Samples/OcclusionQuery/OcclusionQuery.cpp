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
#include "vulkan/vulkan_raii.hpp"

#include <iostream>
#include <thread>

static char const * AppName    = "OcclusionQuery";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::unique_ptr<vk::raii::Context>  context = vk::raii::su::make_unique<vk::raii::Context>();
    std::unique_ptr<vk::raii::Instance> instance =
      vk::raii::su::makeUniqueInstance( *context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::makeUniqueDebugUtilsMessengerEXT( *instance );
#endif
    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::makeUniquePhysicalDevice( *instance );

    vk::raii::su::SurfaceData surfaceData( *instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( *physicalDevice, *surfaceData.surface );
    std::unique_ptr<vk::raii::Device> device = vk::raii::su::makeUniqueDevice(
      *physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    std::unique_ptr<vk::raii::CommandPool> commandPool =
      vk::raii::su::makeUniqueCommandPool( *device, graphicsAndPresentQueueFamilyIndex.first );
    std::unique_ptr<vk::raii::CommandBuffer> commandBuffer =
      vk::raii::su::makeUniqueCommandBuffer( *device, *commandPool );

    std::unique_ptr<vk::raii::Queue> graphicsQueue =
      vk::raii::su::make_unique<vk::raii::Queue>( *device, graphicsAndPresentQueueFamilyIndex.first, 0 );
    std::unique_ptr<vk::raii::Queue> presentQueue =
      vk::raii::su::make_unique<vk::raii::Queue>( *device, graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::raii::su::SwapChainData swapChainData( *physicalDevice,
                                               *device,
                                               *surfaceData.surface,
                                               surfaceData.extent,
                                               vk::ImageUsageFlagBits::eColorAttachment |
                                                 vk::ImageUsageFlagBits::eTransferSrc,
                                               {},
                                               graphicsAndPresentQueueFamilyIndex.first,
                                               graphicsAndPresentQueueFamilyIndex.second );

    vk::raii::su::DepthBufferData depthBufferData(
      *physicalDevice, *device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::raii::su::BufferData uniformBufferData(
      *physicalDevice, *device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4 mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::raii::su::copyToDevice( *uniformBufferData.deviceMemory, mvpcMatrix );

    std::unique_ptr<vk::raii::DescriptorSetLayout> descriptorSetLayout = vk::raii::su::makeUniqueDescriptorSetLayout(
      *device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout =
      vk::raii::su::makeUniquePipelineLayout( *device, *descriptorSetLayout );

    vk::Format colorFormat =
      vk::su::pickSurfaceFormat( physicalDevice->getSurfaceFormatsKHR( **surfaceData.surface ) ).format;
    std::unique_ptr<vk::raii::RenderPass> renderPass =
      vk::raii::su::makeUniqueRenderPass( *device, colorFormat, depthBufferData.format );

    glslang::InitializeProcess();
    std::unique_ptr<vk::raii::ShaderModule> vertexShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C );
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    std::vector<std::unique_ptr<vk::raii::Framebuffer>> framebuffers = vk::raii::su::makeUniqueFramebuffers(
      *device, *renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::raii::su::BufferData vertexBufferData(
      *physicalDevice, *device, sizeof( coloredCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::su::copyToDevice(
      *vertexBufferData.deviceMemory, coloredCubeData, sizeof( coloredCubeData ) / sizeof( coloredCubeData[0] ) );

    std::unique_ptr<vk::raii::DescriptorPool> descriptorPool =
      vk::raii::su::makeUniqueDescriptorPool( *device, { { vk::DescriptorType::eUniformBuffer, 1 } } );
    std::unique_ptr<vk::raii::DescriptorSet> descriptorSet =
      vk::raii::su::makeUniqueDescriptorSet( *device, *descriptorPool, *descriptorSetLayout );

    vk::raii::su::updateDescriptorSets(
      *device, *descriptorSet, { { vk::DescriptorType::eUniformBuffer, *uniformBufferData.buffer, nullptr } }, {} );

    std::unique_ptr<vk::raii::PipelineCache> pipelineCache =
      vk::raii::su::make_unique<vk::raii::PipelineCache>( *device, vk::PipelineCacheCreateInfo() );
    std::unique_ptr<vk::raii::Pipeline> graphicsPipeline = vk::raii::su::makeUniqueGraphicsPipeline(
      *device,
      *pipelineCache,
      *vertexShaderModule,
      nullptr,
      *fragmentShaderModule,
      nullptr,
      sizeof( coloredCubeData[0] ),
      { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
      vk::FrontFace::eClockwise,
      true,
      *pipelineLayout,
      *renderPass );

    /* VULKAN_KEY_START */

    std::unique_ptr<vk::raii::Semaphore> imageAcquiredSemaphore =
      vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );

    // Get the index of the next available swapchain image:
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) =
      swapChainData.swapChain->acquireNextImage( vk::su::FenceTimeout, **imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    /* Allocate a uniform buffer that will take query results. */
    vk::BufferCreateInfo bufferCreateInfo(
      {}, 4 * sizeof( uint64_t ), vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst );
    std::unique_ptr<vk::raii::Buffer> queryResultBuffer =
      vk::raii::su::make_unique<vk::raii::Buffer>( *device, bufferCreateInfo );

    vk::MemoryRequirements memoryRequirements = queryResultBuffer->getMemoryRequirements();
    uint32_t               memoryTypeIndex =
      vk::su::findMemoryType( physicalDevice->getMemoryProperties(),
                              memoryRequirements.memoryTypeBits,
                              vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::MemoryAllocateInfo                  memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    std::unique_ptr<vk::raii::DeviceMemory> queryResultMemory =
      vk::raii::su::make_unique<vk::raii::DeviceMemory>( *device, memoryAllocateInfo );

    queryResultBuffer->bindMemory( **queryResultMemory, 0 );

    vk::QueryPoolCreateInfo              queryPoolCreateInfo( {}, vk::QueryType::eOcclusion, 2, {} );
    std::unique_ptr<vk::raii::QueryPool> queryPool =
      vk::raii::su::make_unique<vk::raii::QueryPool>( *device, queryPoolCreateInfo );

    commandBuffer->begin( {} );
    commandBuffer->resetQueryPool( **queryPool, 0, 2 );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    commandBuffer->beginRenderPass(
      vk::RenderPassBeginInfo(
        **renderPass, **framebuffers[imageIndex], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );

    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, **graphicsPipeline );
    commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics, **pipelineLayout, 0, { **descriptorSet }, {} );

    commandBuffer->bindVertexBuffers( 0, { **vertexBufferData.buffer }, { 0 } );
    commandBuffer->setViewport( 0,
                                vk::Viewport( 0.0f,
                                              0.0f,
                                              static_cast<float>( surfaceData.extent.width ),
                                              static_cast<float>( surfaceData.extent.height ),
                                              0.0f,
                                              1.0f ) );
    commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer->beginQuery( **queryPool, 0, vk::QueryControlFlags() );
    commandBuffer->endQuery( **queryPool, 0 );

    commandBuffer->beginQuery( **queryPool, 1, vk::QueryControlFlags() );
    commandBuffer->draw( 12 * 3, 1, 0, 0 );
    commandBuffer->endRenderPass();
    commandBuffer->endQuery( **queryPool, 1 );

    commandBuffer->copyQueryPoolResults( **queryPool,
                                         0,
                                         2,
                                         **queryResultBuffer,
                                         0,
                                         sizeof( uint64_t ),
                                         vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait );
    commandBuffer->end();

    std::unique_ptr<vk::raii::Fence> drawFence = vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( **imageAcquiredSemaphore, waitDestinationStageMask, **commandBuffer );
    graphicsQueue->submit( submitInfo, **drawFence );

    graphicsQueue->waitIdle();

    std::vector<uint64_t> poolResults;
    std::tie( result, poolResults ) = queryPool->getResults<uint64_t>(
      0, 2, 2 * sizeof( uint64_t ), sizeof( uint64_t ), vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eNotReady:
        std::cout << "vk::Device::getQueryPoolResults returned vk::Result::eNotReady !\n";
        break;
      default: assert( false );  // an unexpected result is returned !
    }

    std::cout << "vkGetQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << poolResults[0] << "\n";
    std::cout << "samples_passed[1] = " << poolResults[1] << "\n";

    /* Read back query result from buffer */
    uint64_t * samplesPassedPtr =
      static_cast<uint64_t *>( queryResultMemory->mapMemory( 0, memoryRequirements.size, vk::MemoryMapFlags() ) );

    std::cout << "vkCmdCopyQueryPoolResults data\n";
    std::cout << "samples_passed[0] = " << samplesPassedPtr[0] << "\n";
    std::cout << "samples_passed[1] = " << samplesPassedPtr[1] << "\n";

    queryResultMemory->unmapMemory();

    while ( vk::Result::eTimeout == device->waitForFences( { **drawFence }, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::PresentInfoKHR presentInfoKHR( nullptr, **swapChainData.swapChain, imageIndex );
    result = presentQueue->presentKHR( presentInfoKHR );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR:
        std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
        break;
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
