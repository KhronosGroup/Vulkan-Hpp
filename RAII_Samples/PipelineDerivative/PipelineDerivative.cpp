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

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"
#include "vulkan/vulkan.hpp"

#include <thread>

static char const * AppName    = "PipelineDerivative";
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

    vk::raii::su::TextureData textureData( *physicalDevice, *device );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( *commandBuffer, vk::su::CheckerboardImageGenerator() );

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
      vk::raii::su::makeUniqueRenderPass( *device, colorFormat, depthBufferData.format );

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

    std::unique_ptr<vk::raii::DescriptorPool> descriptorPool = vk::raii::su::makeUniqueDescriptorPool(
      *device, { { vk::DescriptorType::eUniformBuffer, 1 }, { vk::DescriptorType::eCombinedImageSampler, 1 } } );
    std::unique_ptr<vk::raii::DescriptorSet> descriptorSet =
      vk::raii::su::makeUniqueDescriptorSet( *device, *descriptorPool, *descriptorSetLayout );

    vk::raii::su::updateDescriptorSets( *device,
                                        *descriptorSet,
                                        { { vk::DescriptorType::eUniformBuffer, *uniformBufferData.buffer, nullptr } },
                                        { textureData } );

    std::unique_ptr<vk::raii::PipelineCache> pipelineCache =
      vk::raii::su::make_unique<vk::raii::PipelineCache>( *device, vk::PipelineCacheCreateInfo() );

    /* VULKAN_KEY_START */

    // Create two pipelines.
    //
    // First pipeline has VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT set.
    // Second pipeline has a modified fragment shader and sets the VK_PIPELINE_CREATE_DERIVATIVE_BIT flag.

    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
      vk::PipelineShaderStageCreateInfo( {}, vk::ShaderStageFlagBits::eVertex, **vertexShaderModule, "main" ),
      vk::PipelineShaderStageCreateInfo( {}, vk::ShaderStageFlagBits::eFragment, **fragmentShaderModule, "main" )
    };

    vk::VertexInputBindingDescription vertexInputBindingDescription( 0, sizeof( texturedCubeData[0] ) );
    std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
      vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 ),
      vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32A32Sfloat, 16 )
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
      {}, vertexInputBindingDescription, vertexInputAttributeDescriptions );

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
      {}, vk::PrimitiveTopology::eTriangleList );

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo( {}, 1, nullptr, 1, nullptr );

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo( {},
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

    vk::StencilOpState stencilOpState(
      vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
      {}, true, true, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState );

    vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                 vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA );
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendOp::eAdd,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendFactor::eZero,
                                                                             vk::BlendOp::eAdd,
                                                                             colorComponentFlags );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
      {}, false, vk::LogicOp::eNoOp, pipelineColorBlendAttachmentState, { { 1.0f, 1.0f, 1.0f, 1.0f } } );

    std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( {}, dynamicStates );

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
                                                               **pipelineLayout,
                                                               **renderPass );

    std::unique_ptr<vk::raii::Pipeline> basePipeline =
      vk::raii::su::make_unique<vk::raii::Pipeline>( *device, *pipelineCache, graphicsPipelineCreateInfo );
    switch ( basePipeline->getConstructorSuccessCode() )
    {
      case vk::Result::eSuccess: break;
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
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule2 =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C_2 );
    glslang::FinalizeProcess();

    // Modify pipeline info to reflect derivation
    pipelineShaderStageCreateInfos[1] =
      vk::PipelineShaderStageCreateInfo( {}, vk::ShaderStageFlagBits::eFragment, **fragmentShaderModule2, "main" );
    graphicsPipelineCreateInfo.flags              = vk::PipelineCreateFlagBits::eDerivative;
    graphicsPipelineCreateInfo.basePipelineHandle = **basePipeline;
    graphicsPipelineCreateInfo.basePipelineIndex  = -1;

    // And create the derived pipeline
    std::unique_ptr<vk::raii::Pipeline> derivedPipeline =
      vk::raii::su::make_unique<vk::raii::Pipeline>( *device, *pipelineCache, graphicsPipelineCreateInfo );
    switch ( derivedPipeline->getConstructorSuccessCode() )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::ePipelineCompileRequiredEXT:
        // something meaningfull here
        break;
      default: assert( false );  // should never happen
    }

    /* VULKAN_KEY_END */

    std::unique_ptr<vk::raii::Semaphore> imageAcquiredSemaphore =
      vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );

    // Get the index of the next available swapchain image
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) =
      swapChainData.swapChain->acquireNextImage( vk::su::FenceTimeout, **imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    commandBuffer->beginRenderPass(
      vk::RenderPassBeginInfo(
        **renderPass, **framebuffers[imageIndex], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );
    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, **derivedPipeline );
    commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics, **pipelineLayout, 0, { **descriptorSet }, {} );

    commandBuffer->bindVertexBuffers( 0, { **vertexBufferData.buffer }, { 0 } );
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

    std::unique_ptr<vk::raii::Fence> drawFence = vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( **imageAcquiredSemaphore, waitDestinationStageMask, **commandBuffer );
    graphicsQueue->submit( submitInfo, **drawFence );

    while ( vk::Result::eTimeout == device->waitForFences( { **drawFence }, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::PresentInfoKHR presentInfoKHR( nullptr, **swapChainData.swapChain, imageIndex );
    result = presentQueue->presentKHR( presentInfoKHR );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR:
        std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
        break;
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
