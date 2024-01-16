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

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

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

    vk::raii::su::TextureData textureData( physicalDevice, device );

    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    vk::raii::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4              mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::raii::su::copyToDevice( uniformBufferData.deviceMemory, mvpcMatrix );

    vk::Format           colorFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format;
    vk::raii::RenderPass renderPass  = vk::raii::su::makeRenderPass( device, colorFormat, depthBufferData.format );

    glslang::InitializeProcess();
    vk::raii::ShaderModule vertexShaderModule   = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
    vk::raii::ShaderModule fragmentShaderModule = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText );
    glslang::FinalizeProcess();

    std::vector<vk::raii::Framebuffer> framebuffers =
      vk::raii::su::makeFramebuffers( device, renderPass, swapChainData.imageViews, &depthBufferData.imageView, surfaceData.extent );

    vk::raii::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::su::copyToDevice( vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    /* VULKAN_KEY_START */

    // Create first layout to contain uniform buffer data
    vk::DescriptorSetLayoutBinding    uniformBinding( 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex );
    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo( {}, uniformBinding );
    vk::raii::DescriptorSetLayout     uniformLayout( device, descriptorSetLayoutCreateInfo );

    // Create second layout containing combined sampler/image data
    vk::DescriptorSetLayoutBinding sampler2DBinding( 0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eVertex );
    descriptorSetLayoutCreateInfo.pBindings = &sampler2DBinding;
    vk::raii::DescriptorSetLayout samplerLayout( device, descriptorSetLayoutCreateInfo );

    // Create pipeline layout with multiple descriptor sets
    std::array<vk::DescriptorSetLayout, 2> descriptorSetLayouts = { uniformLayout, samplerLayout };
    vk::PipelineLayoutCreateInfo           pipelineLayoutCreateInfo( {}, descriptorSetLayouts );
    vk::raii::PipelineLayout               pipelineLayout( device, pipelineLayoutCreateInfo );

    // Create a single pool to contain data for our two descriptor sets
    std::array<vk::DescriptorPoolSize, 2> poolSizes = { vk::DescriptorPoolSize( vk::DescriptorType::eUniformBuffer, 1 ),
                                                        vk::DescriptorPoolSize( vk::DescriptorType::eCombinedImageSampler, 1 ) };
    vk::DescriptorPoolCreateInfo          descriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 2, poolSizes );
    vk::raii::DescriptorPool              descriptorPool( device, descriptorPoolCreateInfo );

    // Populate descriptor sets
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayouts );
    vk::raii::DescriptorSets      descriptorSets( device, descriptorSetAllocateInfo );

    // Populate with info about our uniform buffer
    vk::DescriptorBufferInfo              uniformBufferInfo( uniformBufferData.buffer, 0, sizeof( glm::mat4x4 ) );
    vk::DescriptorImageInfo               textureImageInfo( textureData.sampler, textureData.imageData.imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
    std::array<vk::WriteDescriptorSet, 2> writeDescriptorSets = {
      { vk::WriteDescriptorSet( descriptorSets[0], 0, 0, vk::DescriptorType::eUniformBuffer, {}, uniformBufferInfo ),
        vk::WriteDescriptorSet( descriptorSets[1], 0, 0, vk::DescriptorType::eCombinedImageSampler, textureImageInfo ) }
    };
    device.updateDescriptorSets( writeDescriptorSets, nullptr );

    /* VULKAN_KEY_END */

    vk::raii::PipelineCache pipelineCache( device, vk::PipelineCacheCreateInfo() );
    vk::raii::Pipeline      graphicsPipeline = vk::raii::su::makeGraphicsPipeline( device,
                                                                              pipelineCache,
                                                                              vertexShaderModule,
                                                                              nullptr,
                                                                              fragmentShaderModule,
                                                                              nullptr,
                                                                              sizeof( texturedCubeData[0] ),
                                                                                   { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
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

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass, framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSets[0], descriptorSets[1] }, nullptr );

    vk::Buffer buffer = vertexBufferData.buffer;
    commandBuffer.bindVertexBuffers( 0, buffer, { 0 } );
    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

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

    device.waitIdle();
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
