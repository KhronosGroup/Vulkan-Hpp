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
// VulkanHpp Samples : TexelBuffer
//                     Use a texel buffer to draw a green triangle.

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>

static char const * AppName    = "TexelBuffer";
static char const * EngineName = "Vulkan.hpp";

static const std::string vertexShaderText = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform samplerBuffer texels;
layout (location = 0) out vec4 outColor;

vec2 vertices[3];

void main()
{
  float r = texelFetch(texels, 0).r;
  float g = texelFetch(texels, 1).r;
  float b = texelFetch(texels, 2).r;
  outColor = vec4(r, g, b, 1.0);

  vertices[0] = vec2(-1.0, -1.0);
  vertices[1] = vec2( 1.0, -1.0);
  vertices[2] = vec2( 0.0,  1.0);
  gl_Position = vec4(vertices[gl_VertexIndex % 3], 0.0, 1.0);
}
)";

int main( int /*argc*/, char ** /*argv*/ )
{
  const float texels[] = { 118.0f / 255.0f, 185.0f / 255.0f, 0.0f };

  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
    if ( physicalDeviceProperties.limits.maxTexelBufferElements < 4 )
    {
      std::cout << "maxTexelBufferElements too small\n";
      exit( -1 );
    }

    vk::Format           texelFormat      = vk::Format::eR32Sfloat;
    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( texelFormat );
    if ( !( formatProperties.bufferFeatures & vk::FormatFeatureFlagBits::eUniformTexelBuffer ) )
    {
      std::cout << "R32_SFLOAT format unsupported for texel buffer\n";
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

    vk::raii::su::BufferData texelBufferData( physicalDevice, device, sizeof( texels ), vk::BufferUsageFlagBits::eUniformTexelBuffer );
    texelBufferData.upload( texels );

    vk::BufferViewCreateInfo bufferViewCreateInfo( {}, texelBufferData.buffer, texelFormat, 0, sizeof( texels ) );
    vk::raii::BufferView     texelBufferView( device, bufferViewCreateInfo );

    vk::raii::DescriptorSetLayout descriptorSetLayout =
      vk::raii::su::makeDescriptorSetLayout( device, { { vk::DescriptorType::eUniformTexelBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::raii::PipelineLayout pipelineLayout( device, { {}, *descriptorSetLayout } );

    vk::Format           colorFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format;
    vk::raii::RenderPass renderPass  = vk::raii::su::makeRenderPass( device, colorFormat, vk::Format::eUndefined );

    glslang::InitializeProcess();
    vk::raii::ShaderModule vertexShaderModule   = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
    vk::raii::ShaderModule fragmentShaderModule = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    std::vector<vk::raii::Framebuffer> framebuffers =
      vk::raii::su::makeFramebuffers( device, renderPass, swapChainData.imageViews, nullptr, surfaceData.extent );

    vk::raii::DescriptorPool descriptorPool = vk::raii::su::makeDescriptorPool( device, { { vk::DescriptorType::eUniformTexelBuffer, 1 } } );
    vk::raii::DescriptorSet  descriptorSet  = std::move( vk::raii::DescriptorSets( device, { descriptorPool, *descriptorSetLayout } ).front() );

    vk::raii::su::updateDescriptorSets(
      device, descriptorSet, { { vk::DescriptorType::eUniformTexelBuffer, texelBufferData.buffer, VK_WHOLE_SIZE, &texelBufferView } }, {} );

    vk::raii::PipelineCache pipelineCache( device, vk::PipelineCacheCreateInfo() );
    vk::raii::Pipeline      graphicsPipeline = vk::raii::su::makeGraphicsPipeline(
      device, pipelineCache, vertexShaderModule, nullptr, fragmentShaderModule, nullptr, 0, {}, vk::FrontFace::eClockwise, true, pipelineLayout, renderPass );

    /* VULKAN_KEY_START */

    // Get the index of the next available swapchain image:
    vk::raii::Semaphore imageAcquiredSemaphore( device, vk::SemaphoreCreateInfo() );
    vk::Result          result;
    uint32_t            imageIndex;
    std::tie( result, imageIndex ) = swapChainData.swapChain.acquireNextImage( vk::su::FenceTimeout, imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    commandBuffer.begin( vk::CommandBufferBeginInfo() );

    vk::ClearValue clearValue;
    clearValue.color = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass, framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValue );

    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSet }, nullptr );

    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer.draw( 3, 1, 0, 0 );
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
