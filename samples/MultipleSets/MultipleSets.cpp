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
// VulkanHpp Samples : MultipleSets
//                     Use multiple descriptor sets to draw a textured cube.

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
#  if ( 9 <= __GNUC__ )
#    pragma GCC diagnostic ignored "-Winit-list-lifetime"
#  endif
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

static char const * AppName    = "MultipleSets";
static char const * EngineName = "Vulkan.hpp";

const std::string vertexShaderText = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform buffer
{
  mat4 mvp;
} uniformBuffer;

layout (set = 1, binding = 0) uniform sampler2D surface;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 inTexCoord;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outTexCoord;

void main()
{
  outColor = texture(surface, vec2(0.0f));
  outTexCoord = inTexCoord;
  gl_Position = uniformBuffer.mvp * pos;
}
)";

const std::string fragmentShaderText = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inTexCoord;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = inColor;

  // create a border to see the cube more easily
  if ((inTexCoord.x < 0.01f) || (0.99f < inTexCoord.x) || (inTexCoord.y < 0.01f) || (0.99f < inTexCoord.y))
  {
    outColor *= vec4(0.1f, 0.1f, 0.1f, 1.0f);
  }
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

    vk::su::TextureData textureData( physicalDevice, device );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( device, commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

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
      vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
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

    /* VULKAN_KEY_START */

    // Create first layout to contain uniform buffer data
    vk::DescriptorSetLayoutBinding uniformBinding(
      0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex );
    vk::UniqueDescriptorSetLayout uniformLayout = device->createDescriptorSetLayoutUnique(
      vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), uniformBinding ) );

    // Create second layout containing combined sampler/image data
    vk::DescriptorSetLayoutBinding sampler2DBinding(
      0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eVertex );
    vk::UniqueDescriptorSetLayout samplerLayout = device->createDescriptorSetLayoutUnique(
      vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), sampler2DBinding ) );

    // Create pipeline layout with multiple descriptor sets
    std::array<vk::DescriptorSetLayout, 2> descriptorSetLayouts = { { uniformLayout.get(), samplerLayout.get() } };
    vk::UniquePipelineLayout               pipelineLayout       = device->createPipelineLayoutUnique(
      vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayouts ) );

    // Create a single pool to contain data for our two descriptor sets
    std::array<vk::DescriptorPoolSize, 2> poolSizes = { vk::DescriptorPoolSize( vk::DescriptorType::eUniformBuffer, 1 ),
                                                        vk::DescriptorPoolSize(
                                                          vk::DescriptorType::eCombinedImageSampler, 1 ) };
    vk::UniqueDescriptorPool              descriptorPool = device->createDescriptorPoolUnique(
      vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 2, poolSizes ) );

    // Populate descriptor sets
    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique(
      vk::DescriptorSetAllocateInfo( descriptorPool.get(), descriptorSetLayouts ) );

    // Populate with info about our uniform buffer
    vk::DescriptorBufferInfo              uniformBufferInfo( uniformBufferData.buffer.get(), 0, sizeof( glm::mat4x4 ) );
    vk::DescriptorImageInfo               textureImageInfo( textureData.textureSampler.get(),
                                              textureData.imageData->imageView.get(),
                                              vk::ImageLayout::eShaderReadOnlyOptimal );
    std::array<vk::WriteDescriptorSet, 2> writeDescriptorSets = {
      { vk::WriteDescriptorSet(
          descriptorSets[0].get(), 0, 0, vk::DescriptorType::eUniformBuffer, {}, uniformBufferInfo ),
        vk::WriteDescriptorSet(
          descriptorSets[1].get(), 0, 0, vk::DescriptorType::eCombinedImageSampler, textureImageInfo ) }
    };
    device->updateDescriptorSets( writeDescriptorSets, nullptr );

    /* VULKAN_KEY_END */

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique( vk::PipelineCacheCreateInfo() );
    vk::UniquePipeline      graphicsPipeline =
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

    // Get the index of the next available swapchain image:
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
    commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
                                       pipelineLayout.get(),
                                       0,
                                       { descriptorSets[0].get(), descriptorSets[1].get() },
                                       nullptr );

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
      case vk::Result::eSuboptimalKHR:
        std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
        break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

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
