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
// VulkanHpp Samples : DynamicUniform
//                     Draw 2 Cubes using dynamic uniform buffer

#if defined( _MSC_VER )
#  pragma warning( disable : 4127 )  // conditional expression is constant
#endif

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>

static char const * AppName    = "DynamicUniform";
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

    /* VULKAN_KEY_START */

    vk::PhysicalDeviceLimits limits = physicalDevice.getProperties().limits;
    if ( limits.maxDescriptorSetUniformBuffersDynamic < 1 )
    {
      std::cout << "No dynamic uniform buffers supported\n";
      exit( -1 );
    }

    /* Set up uniform buffer with 2 transform matrices in it */
    glm::mat4x4 mvpcs[2];
    glm::mat4x4 model      = glm::mat4x4( 1.0f );
    glm::mat4x4 view       = glm::lookAt( glm::vec3( 0.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
    glm::mat4x4 projection = glm::perspective( glm::radians( 45.0f ), 1.0f, 0.1f, 100.0f );
    // clang-format off
    glm::mat4x4 clip = glm::mat4x4( 1.0f,  0.0f, 0.0f, 0.0f,
                                    0.0f, -1.0f, 0.0f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 1.0f );  // vulkan clip space has inverted y and half z !
    // clang-format on
    mvpcs[0] = clip * projection * view * model;

    model    = glm::translate( model, glm::vec3( -1.5f, 1.5f, -1.5f ) );
    mvpcs[1] = clip * projection * view * model;

    vk::DeviceSize bufferSize = sizeof( glm::mat4x4 );
    if ( limits.minUniformBufferOffsetAlignment )
    {
      bufferSize = ( bufferSize + limits.minUniformBufferOffsetAlignment - 1 ) & ~( limits.minUniformBufferOffsetAlignment - 1 );
    }

    vk::raii::su::BufferData uniformBufferData( physicalDevice, device, 2 * bufferSize, vk::BufferUsageFlagBits::eUniformBuffer );
    vk::raii::su::copyToDevice( uniformBufferData.deviceMemory, mvpcs, 2, bufferSize );

    // create a DescriptorSetLayout with vk::DescriptorType::eUniformBufferDynamic
    vk::raii::DescriptorSetLayout descriptorSetLayout =
      vk::raii::su::makeDescriptorSetLayout( device, { { vk::DescriptorType::eUniformBufferDynamic, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::raii::PipelineLayout pipelineLayout( device, { {}, *descriptorSetLayout } );

    // create a DescriptorPool with vk::DescriptorType::eUniformBufferDynamic
    vk::raii::DescriptorPool descriptorPool = vk::raii::su::makeDescriptorPool( device, { { vk::DescriptorType::eUniformBufferDynamic, 1 } } );
    vk::raii::DescriptorSet  descriptorSet  = std::move( vk::raii::DescriptorSets( device, { descriptorPool, *descriptorSetLayout } ).front() );

    vk::raii::su::updateDescriptorSets(
      device, descriptorSet, { { vk::DescriptorType::eUniformBufferDynamic, uniformBufferData.buffer, bufferSize, nullptr } }, {} );

    vk::raii::PipelineCache pipelineCache( device, vk::PipelineCacheCreateInfo() );
    vk::raii::Pipeline      graphicsPipeline =
      vk::raii::su::makeGraphicsPipeline( device,
                                          pipelineCache,
                                          vertexShaderModule,
                                          nullptr,
                                          fragmentShaderModule,
                                          nullptr,
                                          sizeof( coloredCubeData[0] ),
                                          { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32B32A32Sfloat, 16 } },
                                          vk::FrontFace::eClockwise,
                                          true,
                                          pipelineLayout,
                                          renderPass );

    // Get the index of the next available swapchain image:
    vk::raii::Semaphore imageAcquiredSemaphore( device, vk::SemaphoreCreateInfo() );
    vk::Result          result;
    uint32_t            imageIndex;
    std::tie( result, imageIndex ) = swapChainData.swapChain.acquireNextImage( vk::su::FenceTimeout, imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    commandBuffer.begin( {} );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass, framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );

    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    /* The first draw should use the first matrix in the buffer */
    uint32_t dynamicOffset = 0;
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSet }, dynamicOffset );

    commandBuffer.bindVertexBuffers( 0, { vertexBufferData.buffer }, { 0 } );
    commandBuffer.draw( 12 * 3, 1, 0, 0 );

    // the second draw should use the second matrix in the buffer;
    dynamicOffset = (uint32_t)bufferSize;
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSet }, dynamicOffset );
    commandBuffer.draw( 12 * 3, 1, 0, 0 );

    commandBuffer.endRenderPass();
    commandBuffer.end();

    vk::raii::Fence drawFence( device, vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( *imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer );
    graphicsQueue.submit( submitInfo, *drawFence );

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
