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
// VulkanHpp Samples : PipelineDerivative
//                     This sample creates pipeline derivative and draws with it.

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

#include <thread>

static char const * AppName    = "PipelineDerivative";
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

    vk::su::TextureData textureData( physicalDevice, device );
    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( device, commandBuffer, vk::su::CheckerboardImageGenerator() );

    vk::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4        mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::su::copyToDevice( device, uniformBufferData.deviceMemory, mvpcMatrix );

    vk::DescriptorSetLayout descriptorSetLayout =
      vk::su::createDescriptorSetLayout( device,
                                         { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                           { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } );
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    vk::RenderPass renderPass = vk::su::createRenderPass(
      device, vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format, depthBufferData.format );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vk::DescriptorPool descriptorPool =
      vk::su::createDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 1 }, { vk::DescriptorType::eCombinedImageSampler, 1 } } );
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayout );
    vk::DescriptorSet             descriptorSet = device.allocateDescriptorSets( descriptorSetAllocateInfo ).front();

    vk::su::updateDescriptorSets( device, descriptorSet, { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, textureData );

    vk::PipelineCache pipelineCache = device.createPipelineCache( vk::PipelineCacheCreateInfo() );

    /* VULKAN_KEY_START */

    // Create two pipelines.
    //
    // First pipeline has VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT set.
    // Second pipeline has a modified fragment shader and sets the VK_PIPELINE_CREATE_DERIVATIVE_BIT flag.

    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main" ),
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main" )
    };

    vk::VertexInputBindingDescription                  vertexInputBindingDescription( 0, sizeof( texturedCubeData[0] ) );
    std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
      vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 ),
      vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32A32Sfloat, 16 )
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
      vk::PipelineVertexInputStateCreateFlags(), vertexInputBindingDescription, vertexInputAttributeDescriptions );

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo( vk::PipelineInputAssemblyStateCreateFlags(),
                                                                                   vk::PrimitiveTopology::eTriangleList );

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo( vk::PipelineRasterizationStateCreateFlags(),
                                                                                   false,
                                                                                   false,
                                                                                   vk::PolygonMode::eFill,
                                                                                   vk::CullModeFlagBits::eBack,
                                                                                   vk::FrontFace::eClockwise,
                                                                                   false,
                                                                                   0.0f,
                                                                                   0.0f,
                                                                                   0.0f,
                                                                                   1.0f );

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo( {}, vk::SampleCountFlagBits::e1 );

    vk::StencilOpState                      stencilOpState( vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
      vk::PipelineDepthStencilStateCreateFlags(), true, true, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState );

    vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                                                 vk::ColorComponentFlagBits::eA );
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendOp::eAdd,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendOp::eAdd,
                                                                             colorComponentFlags );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
      vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, pipelineColorBlendAttachmentState, { { 1.0f, 1.0f, 1.0f, 1.0f } } );

    std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(), dynamicStates );

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo( vk::PipelineCreateFlagBits::eAllowDerivatives,
                                                               pipelineShaderStageCreateInfos,
                                                               &pipelineVertexInputStateCreateInfo,
                                                               &pipelineInputAssemblyStateCreateInfo,
                                                               nullptr,
                                                               &pipelineViewportStateCreateInfo,
                                                               &pipelineRasterizationStateCreateInfo,
                                                               &pipelineMultisampleStateCreateInfo,
                                                               &pipelineDepthStencilStateCreateInfo,
                                                               &pipelineColorBlendStateCreateInfo,
                                                               &pipelineDynamicStateCreateInfo,
                                                               pipelineLayout,
                                                               renderPass );

    vk::Pipeline                  basePipeline;
    vk::ResultValue<vk::Pipeline> rvPipeline = device.createGraphicsPipeline( pipelineCache, graphicsPipelineCreateInfo );
    switch ( rvPipeline.result )
    {
      case vk::Result::eSuccess: basePipeline = std::move( rvPipeline.value ); break;
      case vk::Result::ePipelineCompileRequiredEXT:
        // something meaningfull here
        break;
      default: assert( false );  // should never happen
    }

    // Now create the derivative pipeline, using a different fragment shader
    // This shader will shade the cube faces with interpolated colors
    const std::string fragmentShaderText_T_C_2 = R"(
#version 450

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(inTexCoord.x, inTexCoord.y, 1.0f - inTexCoord.x - inTexCoord.y, 1.0f);
}
)";

    // Convert GLSL to SPIR-V
    glslang::InitializeProcess();
    vk::ShaderModule fragmentShaderModule2 = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C_2 );
    glslang::FinalizeProcess();

    // Modify pipeline info to reflect derivation
    pipelineShaderStageCreateInfos[1] =
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule2, "main" );
    graphicsPipelineCreateInfo.flags              = vk::PipelineCreateFlagBits::eDerivative;
    graphicsPipelineCreateInfo.basePipelineHandle = basePipeline;
    graphicsPipelineCreateInfo.basePipelineIndex  = -1;

    // And create the derived pipeline
    vk::Pipeline derivedPipeline;
    rvPipeline = device.createGraphicsPipeline( pipelineCache, graphicsPipelineCreateInfo );
    switch ( rvPipeline.result )
    {
      case vk::Result::eSuccess: derivedPipeline = std::move( rvPipeline.value ); break;
      case vk::Result::ePipelineCompileRequiredEXT:
        // something meaningfull here
        break;
      default: assert( false );  // should never happen
    }

    /* VULKAN_KEY_END */

    vk::Semaphore imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) );

    // Get the index of the next available swapchain image
    vk::ResultValue<uint32_t> currentBuffer = device.acquireNextImageKHR( swapChainData.swapChain, UINT64_MAX, imageAcquiredSemaphore, nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    commandBuffer.beginRenderPass(
      vk::RenderPassBeginInfo( renderPass, framebuffers[currentBuffer.value], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, derivedPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, {} );

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

    device.destroyFence( drawFence );
    device.destroySemaphore( imageAcquiredSemaphore );
    device.destroyShaderModule( fragmentShaderModule2 );
    device.destroyPipeline( derivedPipeline );
    device.destroyPipeline( basePipeline );
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
