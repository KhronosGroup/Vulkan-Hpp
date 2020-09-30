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

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
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
    vk::UniqueInstance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger( instance );
#endif

    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, *surfaceData.surface );
    vk::UniqueDevice device =
      vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::UniqueCommandPool   commandPool = vk::su::createCommandPool( device, graphicsAndPresentQueueFamilyIndex.first );
    vk::UniqueCommandBuffer commandBuffer = std::move( device
                                                         ->allocateCommandBuffersUnique( vk::CommandBufferAllocateInfo(
                                                           commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) )
                                                         .front() );

    vk::Queue graphicsQueue = device->getQueue( graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::Queue presentQueue  = device->getQueue( graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device,
                                         *surfaceData.surface,
                                         surfaceData.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment |
                                           vk::ImageUsageFlagBits::eTransferSrc,
                                         vk::UniqueSwapchainKHR(),
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::su::BufferData uniformBufferData(
      physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    vk::su::copyToDevice(
      device, uniformBufferData.deviceMemory, vk::su::createModelViewProjectionClipMatrix( surfaceData.extent ) );

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(
      device,
      { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
        { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } );
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(
      vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), *descriptorSetLayout ) );

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(
      device,
      vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface.get() ) ).format,
      depthBufferData.format,
      vk::AttachmentLoadOp::eClear,
      vk::ImageLayout::eColorAttachmentOptimal );

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule =
      vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::UniqueShaderModule fragmentShaderModule =
      vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(
      device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData(
      physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device,
                          vertexBufferData.deviceMemory,
                          texturedCubeData,
                          sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique( vk::PipelineCacheCreateInfo() );

    vk::UniquePipeline graphicsPipeline =
      vk::su::createGraphicsPipeline( device,
                                      pipelineCache,
                                      std::make_pair( *vertexShaderModule, nullptr ),
                                      std::make_pair( *fragmentShaderModule, nullptr ),
                                      sizeof( texturedCubeData[0] ),
                                      { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                      vk::FrontFace::eClockwise,
                                      true,
                                      pipelineLayout,
                                      renderPass );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );

    vk::su::TextureData greenTextureData( physicalDevice, device );
    greenTextureData.setImage( device, commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    vk::su::TextureData checkeredTextureData( physicalDevice, device );
    checkeredTextureData.setImage( device, commandBuffer, vk::su::CheckerboardImageGenerator() );

    // create two identical descriptor sets, each with a different texture but identical UBOs
    vk::UniqueDescriptorPool descriptorPool = vk::su::createDescriptorPool(
      device, { { vk::DescriptorType::eUniformBuffer, 2 }, { vk::DescriptorType::eCombinedImageSampler, 2 } } );

    std::array<vk::DescriptorSetLayout, 2> layouts = { descriptorSetLayout.get(), descriptorSetLayout.get() };
    std::vector<vk::UniqueDescriptorSet>   descriptorSets =
      device->allocateDescriptorSetsUnique( vk::DescriptorSetAllocateInfo( descriptorPool.get(), layouts ) );
    assert( descriptorSets.size() == 2 );

    vk::su::updateDescriptorSets( device,
                                  descriptorSets[0],
                                  { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, {} } },
                                  greenTextureData );
    vk::su::updateDescriptorSets( device,
                                  descriptorSets[1],
                                  { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, {} } },
                                  checkeredTextureData );

    /* VULKAN_KEY_START */

    // create four secondary command buffers, for each quadrant of the screen
    std::vector<vk::UniqueCommandBuffer> secondaryCommandBuffers = device->allocateCommandBuffersUnique(
      vk::CommandBufferAllocateInfo( commandPool.get(), vk::CommandBufferLevel::eSecondary, 4 ) );

    // Get the index of the next available swapchain image:
    vk::UniqueSemaphore       imageAcquiredSemaphore = device->createSemaphoreUnique( vk::SemaphoreCreateInfo() );
    vk::ResultValue<uint32_t> currentBuffer          = device->acquireNextImageKHR(
      swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    vk::su::setImageLayout( commandBuffer,
                            swapChainData.images[currentBuffer.value],
                            swapChainData.colorFormat,
                            vk::ImageLayout::eUndefined,
                            vk::ImageLayout::eColorAttachmentOptimal );

    const vk::DeviceSize offset = 0;
    vk::Viewport         viewport( 0.0f, 0.0f, 200.0f, 200.0f, 0.0f, 1.0f );
    vk::Rect2D           scissor( vk::Offset2D( 0, 0 ), vk::Extent2D( surfaceData.extent ) );

    // now we record four separate command buffers, one for each quadrant of the screen
    vk::CommandBufferInheritanceInfo commandBufferInheritanceInfo(
      renderPass.get(), 0, framebuffers[currentBuffer.value].get() );
    vk::CommandBufferBeginInfo secondaryBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit |
                                                     vk::CommandBufferUsageFlagBits::eRenderPassContinue,
                                                   &commandBufferInheritanceInfo );

    for ( int i = 0; i < 4; i++ )
    {
      viewport.x = 25.0f + 250.0f * ( i % 2 );
      viewport.y = 25.0f + 250.0f * ( i / 2 );

      secondaryCommandBuffers[i]->begin( secondaryBeginInfo );
      secondaryCommandBuffers[i]->bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline.get() );
      secondaryCommandBuffers[i]->bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[i == 0 || i == 3].get(), nullptr );
      secondaryCommandBuffers[i]->bindVertexBuffers( 0, vertexBufferData.buffer.get(), offset );
      secondaryCommandBuffers[i]->setViewport( 0, viewport );
      secondaryCommandBuffers[i]->setScissor( 0, scissor );
      secondaryCommandBuffers[i]->draw( 12 * 3, 1, 0, 0 );
      secondaryCommandBuffers[i]->end();
    }

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass.get(),
                                                 framebuffers[currentBuffer.value].get(),
                                                 vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ),
                                                 clearValues );
    // specifying VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS means this render pass may ONLY call
    // vkCmdExecuteCommands
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eSecondaryCommandBuffers );
    commandBuffer->executeCommands( vk::uniqueToRaw( secondaryCommandBuffers ) );
    commandBuffer->endRenderPass();

    vk::ImageMemoryBarrier prePresentBarrier(
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eMemoryRead,
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageLayout::ePresentSrcKHR,
      VK_QUEUE_FAMILY_IGNORED,
      VK_QUEUE_FAMILY_IGNORED,
      swapChainData.images[currentBuffer.value],
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );
    commandBuffer->pipelineBarrier( vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                    vk::PipelineStageFlagBits::eBottomOfPipe,
                                    vk::DependencyFlags(),
                                    nullptr,
                                    nullptr,
                                    prePresentBarrier );
    commandBuffer->end();

    vk::UniqueFence drawFence = device->createFenceUnique( vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( *imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer );
    graphicsQueue.submit( submitInfo, drawFence.get() );

    while ( vk::Result::eTimeout == device->waitForFences( drawFence.get(), VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::Result result =
      presentQueue.presentKHR( vk::PresentInfoKHR( {}, *swapChainData.swapChain, currentBuffer.value ) );
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
