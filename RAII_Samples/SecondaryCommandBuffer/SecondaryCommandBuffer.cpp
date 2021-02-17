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
// VulkanHpp Samples : SecondaryCommandBuffer
//                     Draw several cubes using primary and secondary command buffers

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
// no need to ignore any warnings with GCC
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"
#include "vulkan/vulkan.hpp"

#include <iostream>
#include <thread>

static char const * AppName    = "SecondaryCommandBuffer";
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
      *device,
      { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
        { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } );
    std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout =
      vk::raii::su::makeUniquePipelineLayout( *device, *descriptorSetLayout );

    vk::Format colorFormat =
      vk::su::pickSurfaceFormat( physicalDevice->getSurfaceFormatsKHR( **surfaceData.surface ) ).format;
    std::unique_ptr<vk::raii::RenderPass> renderPass =
      vk::raii::su::makeUniqueRenderPass( *device,
                                          colorFormat,
                                          depthBufferData.format,
                                          vk::AttachmentLoadOp::eClear,
                                          vk::ImageLayout::eColorAttachmentOptimal );

    glslang::InitializeProcess();
    std::unique_ptr<vk::raii::ShaderModule> vertexShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<std::unique_ptr<vk::raii::Framebuffer>> framebuffers = vk::raii::su::makeUniqueFramebuffers(
      *device, *renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::raii::su::BufferData vertexBufferData(
      *physicalDevice, *device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::su::copyToDevice(
      *vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    std::unique_ptr<vk::raii::PipelineCache> pipelineCache =
      vk::raii::su::make_unique<vk::raii::PipelineCache>( *device, vk::PipelineCacheCreateInfo() );
    std::unique_ptr<vk::raii::Pipeline> graphicsPipeline = vk::raii::su::makeUniqueGraphicsPipeline(
      *device,
      *pipelineCache,
      *vertexShaderModule,
      nullptr,
      *fragmentShaderModule,
      nullptr,
      sizeof( texturedCubeData[0] ),
      { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
      vk::FrontFace::eClockwise,
      true,
      *pipelineLayout,
      *renderPass );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );

    vk::raii::su::TextureData greenTextureData( *physicalDevice, *device );
    greenTextureData.setImage( *commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    vk::raii::su::TextureData checkeredTextureData( *physicalDevice, *device );
    checkeredTextureData.setImage( *commandBuffer, vk::su::CheckerboardImageGenerator() );

    // create two identical descriptor sets, each with a different texture but identical UBOs
    std::unique_ptr<vk::raii::DescriptorPool> descriptorPool = vk::raii::su::makeUniqueDescriptorPool(
      *device, { { vk::DescriptorType::eUniformBuffer, 2 }, { vk::DescriptorType::eCombinedImageSampler, 2 } } );

    std::array<vk::DescriptorSetLayout, 2> layouts = { **descriptorSetLayout, **descriptorSetLayout };
    vk::DescriptorSetAllocateInfo          descriptorSetAllocateInfo( **descriptorPool, layouts );
    vk::raii::DescriptorSets               descriptorSets( *device, descriptorSetAllocateInfo );
    assert( descriptorSets.size() == 2 );

    vk::raii::su::updateDescriptorSets( *device,
                                        descriptorSets[0],
                                        { { vk::DescriptorType::eUniformBuffer, *uniformBufferData.buffer, {} } },
                                        greenTextureData );
    vk::raii::su::updateDescriptorSets( *device,
                                        descriptorSets[1],
                                        { { vk::DescriptorType::eUniformBuffer, *uniformBufferData.buffer, {} } },
                                        checkeredTextureData );

    /* VULKAN_KEY_START */

    // create four secondary command buffers, for each quadrant of the screen
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo( **commandPool, vk::CommandBufferLevel::eSecondary, 4 );
    vk::raii::CommandBuffers      secondaryCommandBuffers( *device, commandBufferAllocateInfo );

    // Get the index of the next available swapchain image:
    std::unique_ptr<vk::raii::Semaphore> imageAcquiredSemaphore =
      vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) =
      swapChainData.swapChain->acquireNextImage( vk::su::FenceTimeout, **imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    vk::raii::su::setImageLayout( *commandBuffer,
                                  static_cast<vk::Image>( swapChainData.images[imageIndex] ),
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eUndefined,
                                  vk::ImageLayout::eColorAttachmentOptimal );

    const vk::DeviceSize offset = 0;
    vk::Viewport         viewport( 0.0f, 0.0f, 200.0f, 200.0f, 0.0f, 1.0f );
    vk::Rect2D           scissor( vk::Offset2D( 0, 0 ), vk::Extent2D( surfaceData.extent ) );

    // now we record four separate command buffers, one for each quadrant of the screen
    vk::CommandBufferInheritanceInfo commandBufferInheritanceInfo( **renderPass, 0, **framebuffers[imageIndex] );
    vk::CommandBufferBeginInfo       secondaryBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit |
                                                     vk::CommandBufferUsageFlagBits::eRenderPassContinue,
                                                   &commandBufferInheritanceInfo );

    std::array<vk::CommandBuffer, 4> executeCommandBuffers;
    for ( int i = 0; i < 4; i++ )
    {
      viewport.x = 25.0f + 250.0f * ( i % 2 );
      viewport.y = 25.0f + 250.0f * ( i / 2 );

      secondaryCommandBuffers[i].begin( secondaryBeginInfo );
      secondaryCommandBuffers[i].bindPipeline( vk::PipelineBindPoint::eGraphics, **graphicsPipeline );
      secondaryCommandBuffers[i].bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics, **pipelineLayout, 0, { *descriptorSets[i == 0 || i == 3] }, nullptr );
      secondaryCommandBuffers[i].bindVertexBuffers( 0, { **vertexBufferData.buffer }, offset );
      secondaryCommandBuffers[i].setViewport( 0, viewport );
      secondaryCommandBuffers[i].setScissor( 0, scissor );
      secondaryCommandBuffers[i].draw( 12 * 3, 1, 0, 0 );
      secondaryCommandBuffers[i].end();
      executeCommandBuffers[i] = *secondaryCommandBuffers[i];
    }

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    vk::RenderPassBeginInfo renderPassBeginInfo(
      **renderPass, **framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );

    // specifying VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS means this render pass may ONLY call
    // vkCmdExecuteCommands
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eSecondaryCommandBuffers );
    commandBuffer->executeCommands( executeCommandBuffers );
    commandBuffer->endRenderPass();

    vk::ImageSubresourceRange imageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
    vk::ImageMemoryBarrier    prePresentBarrier( vk::AccessFlagBits::eColorAttachmentWrite,
                                              vk::AccessFlagBits::eMemoryRead,
                                              vk::ImageLayout::eColorAttachmentOptimal,
                                              vk::ImageLayout::ePresentSrcKHR,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              static_cast<vk::Image>( swapChainData.images[imageIndex] ),
                                              imageSubresourceRange );
    commandBuffer->pipelineBarrier( vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                    vk::PipelineStageFlagBits::eBottomOfPipe,
                                    vk::DependencyFlags(),
                                    nullptr,
                                    nullptr,
                                    prePresentBarrier );
    commandBuffer->end();

    std::unique_ptr<vk::raii::Fence> drawFence = vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( **imageAcquiredSemaphore, waitDestinationStageMask, **commandBuffer );
    graphicsQueue->submit( submitInfo, **drawFence );

    while ( vk::Result::eTimeout == device->waitForFences( { **drawFence }, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    result = presentQueue->presentKHR( vk::PresentInfoKHR( {}, **swapChainData.swapChain, imageIndex, {} ) );
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

    device->waitIdle();
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
