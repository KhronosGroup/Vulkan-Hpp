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
// VulkanHpp Samples : PushConstants
//                     Use push constants in a simple shader, validate the correct value was read.

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"
#include "vulkan/vulkan.hpp"

#include <iostream>
#include <thread>

static char const * AppName    = "PushConstants";
static char const * EngineName = "Vulkan.hpp";

const std::string fragmentShaderText = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform pushBlock
{
  int   iFoo;
  float fBar;
} pushConstantsBlock;

layout (location = 0) in vec2 inTexCoords;

layout (location = 0) out vec4 outColor;

void main()
{
  vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
  vec4 red   = vec4(1.0f, 0.0f, 0.0f, 1.0f);

  // Start with passing color
  vec4 resColor = green;

  // See if we've read in the correct push constants
  if ((pushConstantsBlock.iFoo != 2) || (pushConstantsBlock.fBar != 1.0f))
  {
    resColor = red;
  }

  // Create a border to see the cube more easily
  if ((inTexCoords.x < 0.01f) || (0.99f < inTexCoords.x)
   || (inTexCoords.y < 0.01f) || (0.99f < inTexCoords.y))
  {
    resColor *= vec4(0.1f, 0.1f, 0.1f, 1.0f);
  }

  outColor = resColor;
}
)";

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

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(
      device,
      vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface.get() ) ).format,
      depthBufferData.format );

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule =
      vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::UniqueShaderModule fragmentShaderModule =
      vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText );
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(
      device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData(
      physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device,
                          vertexBufferData.deviceMemory,
                          texturedCubeData,
                          sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    // Create binding and layout for the following, matching contents of shader
    //   binding 0 = uniform buffer (MVP)
    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(
      device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );

    /* VULKAN_KEY_START */

    // Set up our push constant range, which mirrors the declaration of
    vk::PushConstantRange    pushConstantRanges( vk::ShaderStageFlagBits::eFragment, 0, 8 );
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(
      vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), *descriptorSetLayout, pushConstantRanges ) );

    // Create a single pool to contain data for our descriptor set
    std::array<vk::DescriptorPoolSize, 2> poolSizes = { vk::DescriptorPoolSize( vk::DescriptorType::eUniformBuffer, 1 ),
                                                        vk::DescriptorPoolSize(
                                                          vk::DescriptorType::eCombinedImageSampler, 1 ) };
    vk::UniqueDescriptorPool              descriptorPool = device->createDescriptorPoolUnique(
      vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSizes ) );

    // Populate descriptor sets
    vk::UniqueDescriptorSet descriptorSet = std::move(
      device->allocateDescriptorSetsUnique( vk::DescriptorSetAllocateInfo( *descriptorPool, *descriptorSetLayout ) )
        .front() );

    // Populate with info about our uniform buffer for MVP
    vk::DescriptorBufferInfo bufferInfo( uniformBufferData.buffer.get(), 0, sizeof( glm::mat4x4 ) );
    device->updateDescriptorSets(
      vk::WriteDescriptorSet( *descriptorSet, 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo ), {} );

    // Create our push constant data, which matches shader expectations
    std::array<unsigned, 2> pushConstants = { { (unsigned)2, (unsigned)0x3F800000 } };

    // Ensure we have enough room for push constant data
    assert( ( sizeof( pushConstants ) <= physicalDevice.getProperties().limits.maxPushConstantsSize ) &&
            "Too many push constants" );
    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    commandBuffer->pushConstants<unsigned>(
      pipelineLayout.get(), vk::ShaderStageFlagBits::eFragment, 0, pushConstants );

    /* VULKAN_KEY_END */

    vk::UniquePipelineCache pipelineCache    = device->createPipelineCacheUnique( vk::PipelineCacheCreateInfo() );
    vk::UniquePipeline      graphicsPipeline = vk::su::createGraphicsPipeline(
      device,
      pipelineCache,
      std::make_pair( *vertexShaderModule, nullptr ),
      std::make_pair( *fragmentShaderModule, nullptr ),
      sizeof( texturedCubeData[0] ),
      { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32B32A32Sfloat, 16 } },
      vk::FrontFace::eClockwise,
      true,
      pipelineLayout,
      renderPass );

    vk::UniqueSemaphore       imageAcquiredSemaphore = device->createSemaphoreUnique( vk::SemaphoreCreateInfo() );
    vk::ResultValue<uint32_t> currentBuffer          = device->acquireNextImageKHR(
      swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass.get(),
                                                 framebuffers[currentBuffer.value].get(),
                                                 vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ),
                                                 clearValues );
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline.get() );
    commandBuffer->bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSet.get(), nullptr );

    commandBuffer->bindVertexBuffers( 0, *vertexBufferData.buffer, { 0 } );
    commandBuffer->setViewport( 0,
                                vk::Viewport( 0.0f,
                                              0.0f,
                                              static_cast<float>( surfaceData.extent.width ),
                                              static_cast<float>( surfaceData.extent.height ),
                                              0.0f,
                                              1.0f ) );
    commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer->draw( 12 * 3, 1, 0, 0 );
    commandBuffer->endRenderPass();
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
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
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
