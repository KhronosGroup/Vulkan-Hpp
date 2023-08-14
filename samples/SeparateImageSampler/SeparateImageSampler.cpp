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
// VulkanHpp Samples : SeparateImageSampler
//                     Use separate image and sampler in descriptor set and shader to draw a textured cube.

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
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

static char const * AppName    = "SeparateImageSampler";
static char const * EngineName = "Vulkan.hpp";

const std::string fragmentShaderTextTS_T_C = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 1) uniform texture2D tex;
layout (set = 0, binding = 2) uniform sampler samp;

layout (location = 0) in vec2 inTexCoords;

layout (location = 0) out vec4 outColor;

void main()
{
  // Combine the selected texture with sampler as a parameter
  vec4 resColor = texture(sampler2D(tex, samp), inTexCoords);

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

    vk::RenderPass renderPass = vk::su::createRenderPass( device,
                                                          vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format,
                                                          depthBufferData.format,
                                                          vk::AttachmentLoadOp::eClear );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderTextTS_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    /* VULKAN_KEY_START */

    commandBuffer.begin( vk::CommandBufferBeginInfo() );

    // Create the separate image
    vk::su::TextureData textureData( physicalDevice, device );
    textureData.setImage( device, commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    // Create the separate sampler
    vk::Sampler sampler = device.createSampler( vk::SamplerCreateInfo( vk::SamplerCreateFlags(),
                                                                       vk::Filter::eNearest,
                                                                       vk::Filter::eNearest,
                                                                       vk::SamplerMipmapMode::eNearest,
                                                                       vk::SamplerAddressMode::eClampToEdge,
                                                                       vk::SamplerAddressMode::eClampToEdge,
                                                                       vk::SamplerAddressMode::eClampToEdge,
                                                                       0.0f,
                                                                       false,
                                                                       1.0f,
                                                                       false,
                                                                       vk::CompareOp::eNever,
                                                                       0.0f,
                                                                       0.0f,
                                                                       vk::BorderColor::eFloatOpaqueWhite ) );

    // Create binding and layout for the following, matching contents of shader
    //   binding 0 = uniform buffer (MVP)
    //   binding 1 = texture2D
    //   binding 2 = sampler
    std::array<vk::DescriptorSetLayoutBinding, 3> resourceBindings = {
      { vk::DescriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex ),
        vk::DescriptorSetLayoutBinding( 1, vk::DescriptorType::eSampledImage, 1, vk::ShaderStageFlagBits::eFragment ),
        vk::DescriptorSetLayoutBinding( 2, vk::DescriptorType::eSampler, 1, vk::ShaderStageFlagBits::eFragment ) }
    };
    vk::DescriptorSetLayout descriptorSetLayout =
      device.createDescriptorSetLayout( vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), resourceBindings ) );

    // Create pipeline layout
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    // Create a single pool to contain data for the descriptor set
    std::array<vk::DescriptorPoolSize, 3> poolSizes = { { vk::DescriptorPoolSize( vk::DescriptorType::eUniformBuffer, 1 ),
                                                          vk::DescriptorPoolSize( vk::DescriptorType::eSampledImage, 1 ),
                                                          vk::DescriptorPoolSize( vk::DescriptorType::eSampler, 1 ) } };
    vk::DescriptorPool                    descriptorPool =
      device.createDescriptorPool( vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSizes ) );

    // Populate descriptor sets
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayout );
    vk::DescriptorSet             descriptorSet = device.allocateDescriptorSets( descriptorSetAllocateInfo ).front();

    vk::DescriptorBufferInfo              bufferInfo( uniformBufferData.buffer, 0, sizeof( glm::mat4x4 ) );
    vk::DescriptorImageInfo               imageInfo( textureData.sampler, textureData.imageData->imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::DescriptorImageInfo               samplerInfo( sampler, {}, {} );
    std::array<vk::WriteDescriptorSet, 3> descriptorWrites = { { vk::WriteDescriptorSet(
                                                                   descriptorSet, 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo ),
                                                                 vk::WriteDescriptorSet( descriptorSet, 1, 0, vk::DescriptorType::eSampledImage, imageInfo ),
                                                                 vk::WriteDescriptorSet( descriptorSet, 2, 0, vk::DescriptorType::eSampler, samplerInfo ) } };
    device.updateDescriptorSets( descriptorWrites, nullptr );

    /* VULKAN_KEY_END */

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

    // Get the index of the next available swapchain image:
    vk::Semaphore             imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo() );
    vk::ResultValue<uint32_t> currentBuffer = device.acquireNextImageKHR( swapChainData.swapChain, vk::su::FenceTimeout, imageAcquiredSemaphore, nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    vk::RenderPassBeginInfo renderPassBeginInfo(
      renderPass, framebuffers[currentBuffer.value], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );

    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, nullptr );

    commandBuffer.bindVertexBuffers( 0, vertexBufferData.buffer, { 0 } );
    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer.draw( 12 * 3, 1, 0, 0 );
    commandBuffer.endRenderPass();
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

    device.waitIdle();

    device.destroyFence( drawFence );
    device.destroySemaphore( imageAcquiredSemaphore );
    device.destroyPipeline( graphicsPipeline );
    device.destroyPipelineCache( pipelineCache );
    device.freeDescriptorSets( descriptorPool, descriptorSet );
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
    textureData.clear( device );
    device.destroySampler( sampler );
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
