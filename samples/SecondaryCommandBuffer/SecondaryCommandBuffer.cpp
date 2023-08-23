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

#include <iostream>
#include <thread>

static char const * AppName    = "SecondaryCommandBuffer";
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
      vk::su::createDescriptorSetLayout( device,
                                         { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                           { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } );
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    vk::RenderPass renderPass = vk::su::createRenderPass( device,
                                                          vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format,
                                                          depthBufferData.format,
                                                          vk::AttachmentLoadOp::eClear,
                                                          vk::ImageLayout::eColorAttachmentOptimal );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vk::PipelineCache pipelineCache = device.createPipelineCache( vk::PipelineCacheCreateInfo() );

    vk::Pipeline graphicsPipeline = vk::su::createGraphicsPipeline( device,
                                                                    pipelineCache,
                                                                    std::make_pair( vertexShaderModule, nullptr ),
                                                                    std::make_pair( fragmentShaderModule, nullptr ),
                                                                    sizeof( texturedCubeData[0] ),
                                                                    { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                    vk::FrontFace::eClockwise,
                                                                    true,
                                                                    pipelineLayout,
                                                                    renderPass );

    commandBuffer.begin( vk::CommandBufferBeginInfo() );

    vk::su::TextureData greenTextureData( physicalDevice, device );
    greenTextureData.setImage( device, commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    vk::su::TextureData checkeredTextureData( physicalDevice, device );
    checkeredTextureData.setImage( device, commandBuffer, vk::su::CheckerboardImageGenerator() );

    // create two identical descriptor sets, each with a different texture but identical UBOs
    vk::DescriptorPool descriptorPool =
      vk::su::createDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 2 }, { vk::DescriptorType::eCombinedImageSampler, 2 } } );

    std::array<vk::DescriptorSetLayout, 2> layouts = { descriptorSetLayout, descriptorSetLayout };
    vk::DescriptorSetAllocateInfo          descriptorSetAllocateInfo( descriptorPool, layouts );
    std::vector<vk::DescriptorSet>         descriptorSets = device.allocateDescriptorSets( descriptorSetAllocateInfo );
    assert( descriptorSets.size() == 2 );

    vk::su::updateDescriptorSets(
      device, descriptorSets[0], { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, greenTextureData );
    vk::su::updateDescriptorSets(
      device, descriptorSets[1], { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, checkeredTextureData );

    /* VULKAN_KEY_START */

    // create four secondary command buffers, for each quadrant of the screen
    std::vector<vk::CommandBuffer> secondaryCommandBuffers =
      device.allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::eSecondary, 4 ) );

    // Get the index of the next available swapchain image:
    vk::Semaphore             imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo() );
    vk::ResultValue<uint32_t> currentBuffer = device.acquireNextImageKHR( swapChainData.swapChain, vk::su::FenceTimeout, imageAcquiredSemaphore, nullptr );
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
    vk::CommandBufferInheritanceInfo commandBufferInheritanceInfo( renderPass, 0, framebuffers[currentBuffer.value] );
    vk::CommandBufferBeginInfo       secondaryBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit | vk::CommandBufferUsageFlagBits::eRenderPassContinue,
                                                   &commandBufferInheritanceInfo );

    for ( int i = 0; i < 4; i++ )
    {
      viewport.x = 25.0f + 250.0f * ( i % 2 );
      viewport.y = 25.0f + 250.0f * ( i / 2 );

      secondaryCommandBuffers[i].begin( secondaryBeginInfo );
      secondaryCommandBuffers[i].bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
      secondaryCommandBuffers[i].bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSets[i == 0 || i == 3], nullptr );
      secondaryCommandBuffers[i].bindVertexBuffers( 0, vertexBufferData.buffer, offset );
      secondaryCommandBuffers[i].setViewport( 0, viewport );
      secondaryCommandBuffers[i].setScissor( 0, scissor );
      secondaryCommandBuffers[i].draw( 12 * 3, 1, 0, 0 );
      secondaryCommandBuffers[i].end();
    }

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    vk::RenderPassBeginInfo renderPassBeginInfo(
      renderPass, framebuffers[currentBuffer.value], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );
    // specifying VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS means this render pass may ONLY call
    // vkCmdExecuteCommands
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eSecondaryCommandBuffers );
    commandBuffer.executeCommands( secondaryCommandBuffers );
    commandBuffer.endRenderPass();

    vk::ImageMemoryBarrier prePresentBarrier( vk::AccessFlagBits::eColorAttachmentWrite,
                                              {},
                                              vk::ImageLayout::eColorAttachmentOptimal,
                                              vk::ImageLayout::ePresentSrcKHR,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              swapChainData.images[currentBuffer.value],
                                              vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );
    commandBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlags(), nullptr, nullptr, prePresentBarrier );
    commandBuffer.end();

    vk::Fence drawFence = device.createFence( vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( imageAcquiredSemaphore, waitDestinationStageMask, commandBuffer );
    graphicsQueue.submit( submitInfo, drawFence );

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

    /* VULKAN_KEY_END */

    device.waitIdle();

    device.destroyFence( drawFence );
    device.destroySemaphore( imageAcquiredSemaphore );
    device.destroyPipeline( graphicsPipeline );
    device.destroyPipelineCache( pipelineCache );
    device.freeDescriptorSets( descriptorPool, descriptorSets );
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
    checkeredTextureData.clear( device );
    greenTextureData.clear( device );
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
