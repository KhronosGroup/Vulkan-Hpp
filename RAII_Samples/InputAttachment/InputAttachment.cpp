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
// VulkanHpp Samples : InputAttachment
//                     Use an input attachment to draw a yellow triangle

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
// no need to ignore any warnings with GCC
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"
#include "vulkan/vulkan_raii.hpp"

#include <iostream>
#include <thread>

static char const * AppName    = "InputAttachment";
static char const * EngineName = "Vulkan.hpp";

static std::string vertexShaderText = R"(
#version 450

vec2 vertices[3];

void main()
{
  vertices[0] = vec2(-1.0f, -1.0f);
  vertices[1] = vec2( 1.0f, -1.0f);
  vertices[2] = vec2( 0.0f,  1.0f);

  gl_Position = vec4(vertices[gl_VertexIndex % 3], 0.0f, 1.0f);
}
)";

// Use subpassLoad to read from input attachment
static const char * fragmentShaderText = R"(
#version 450

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inputAttachment;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = subpassLoad(inputAttachment);
}
)";
int                 main( int /*argc*/, char ** /*argv*/ )
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

    vk::FormatProperties formatProperties = physicalDevice->getFormatProperties( vk::Format::eR8G8B8A8Unorm );
    if ( !( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment ) )
    {
      std::cout << "vk::Format::eR8G8B8A8Unorm format unsupported for input attachment\n";
      exit( -1 );
    }

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

    /* VULKAN_KEY_START */

    // Create a framebuffer with 2 attachments, one the color attachment the shaders render into, and the other an input
    // attachment which will be cleared to yellow, and then used by the shaders to color the drawn triangle. Final
    // result should be a yellow triangle

    // Create the image that will be used as the input attachment
    // The image for the color attachment is the presentable image already created as part of the SwapChainData
    vk::ImageCreateInfo              imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         swapChainData.colorFormat,
                                         vk::Extent3D( surfaceData.extent, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         vk::ImageTiling::eOptimal,
                                         vk::ImageUsageFlagBits::eInputAttachment |
                                           vk::ImageUsageFlagBits::eTransferDst );
    std::unique_ptr<vk::raii::Image> inputImage = vk::raii::su::make_unique<vk::raii::Image>( *device, imageCreateInfo );

    vk::MemoryRequirements memoryRequirements = inputImage->getMemoryRequirements();
    uint32_t               memoryTypeIndex =
      vk::su::findMemoryType( physicalDevice->getMemoryProperties(), memoryRequirements.memoryTypeBits, {} );
    vk::MemoryAllocateInfo                  memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    std::unique_ptr<vk::raii::DeviceMemory> inputMemory =
      vk::raii::su::make_unique<vk::raii::DeviceMemory>( *device, memoryAllocateInfo );
    inputImage->bindMemory( **inputMemory, 0 );

    // Set the image layout to TRANSFER_DST_OPTIMAL to be ready for clear
    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    vk::raii::su::setImageLayout( *commandBuffer,
                                  **inputImage,
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eUndefined,
                                  vk::ImageLayout::eTransferDstOptimal );

    vk::ClearColorValue       clearColorValue( std::array<float, 4>( { { 1.0f, 1.0f, 0.0f, 0.0f } } ) );
    vk::ImageSubresourceRange imageSubresourceRange(
      vk::ImageAspectFlagBits::eColor, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS );
    commandBuffer->clearColorImage(
      **inputImage, vk::ImageLayout::eTransferDstOptimal, clearColorValue, imageSubresourceRange );

    // Set the image layout to SHADER_READONLY_OPTIMAL for use by the shaders
    vk::raii::su::setImageLayout( *commandBuffer,
                                  **inputImage,
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eTransferDstOptimal,
                                  vk::ImageLayout::eShaderReadOnlyOptimal );

    vk::ComponentMapping componentMapping(
      vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
    imageSubresourceRange = vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
    vk::ImageViewCreateInfo imageViewCreateInfo(
      {}, **inputImage, vk::ImageViewType::e2D, swapChainData.colorFormat, componentMapping, imageSubresourceRange );
    std::unique_ptr<vk::raii::ImageView> inputAttachmentView =
      vk::raii::su::make_unique<vk::raii::ImageView>( *device, imageViewCreateInfo );

    vk::DescriptorSetLayoutBinding layoutBinding(
      0, vk::DescriptorType::eInputAttachment, 1, vk::ShaderStageFlagBits::eFragment );
    vk::DescriptorSetLayoutCreateInfo              descriptorSetLayoutCreateInfo( {}, layoutBinding );
    std::unique_ptr<vk::raii::DescriptorSetLayout> descriptorSetLayout =
      vk::raii::su::make_unique<vk::raii::DescriptorSetLayout>( *device, descriptorSetLayoutCreateInfo );

    vk::PipelineLayoutCreateInfo              pipelineLayoutCreateInfo( {}, **descriptorSetLayout );
    std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout =
      vk::raii::su::make_unique<vk::raii::PipelineLayout>( *device, pipelineLayoutCreateInfo );

    std::array<vk::AttachmentDescription, 2> attachments = {
      // First attachment is the color attachment - clear at the beginning of the renderpass and transition layout to
      // PRESENT_SRC_KHR at the end of renderpass
      vk::AttachmentDescription( {},
                                 swapChainData.colorFormat,
                                 vk::SampleCountFlagBits::e1,
                                 vk::AttachmentLoadOp::eClear,
                                 vk::AttachmentStoreOp::eStore,
                                 vk::AttachmentLoadOp::eDontCare,
                                 vk::AttachmentStoreOp::eDontCare,
                                 vk::ImageLayout::eUndefined,
                                 vk::ImageLayout::ePresentSrcKHR ),
      // Second attachment is input attachment.  Once cleared it should have width*height yellow pixels.
      // Doing a subpassLoad in the fragment shader should give the shader the color at the fragments x,y location from
      // the input attachment
      vk::AttachmentDescription( {},
                                 swapChainData.colorFormat,
                                 vk::SampleCountFlagBits::e1,
                                 vk::AttachmentLoadOp::eLoad,
                                 vk::AttachmentStoreOp::eDontCare,
                                 vk::AttachmentLoadOp::eDontCare,
                                 vk::AttachmentStoreOp::eDontCare,
                                 vk::ImageLayout::eShaderReadOnlyOptimal,
                                 vk::ImageLayout::eShaderReadOnlyOptimal )
    };
    vk::AttachmentReference  colorReference( 0, vk::ImageLayout::eColorAttachmentOptimal );
    vk::AttachmentReference  inputReference( 1, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::SubpassDescription   subPass( {}, vk::PipelineBindPoint::eGraphics, inputReference, colorReference );
    vk::RenderPassCreateInfo renderPassCreateInfo( {}, attachments, subPass );
    std::unique_ptr<vk::raii::RenderPass> renderPass =
      vk::raii::su::make_unique<vk::raii::RenderPass>( *device, renderPassCreateInfo );

    glslang::InitializeProcess();
    std::unique_ptr<vk::raii::ShaderModule> vertexShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText );
    glslang::FinalizeProcess();

    std::vector<std::unique_ptr<vk::raii::Framebuffer>> framebuffers = vk::raii::su::makeUniqueFramebuffers(
      *device, *renderPass, swapChainData.imageViews, inputAttachmentView, surfaceData.extent );

    vk::DescriptorPoolSize       poolSize( vk::DescriptorType::eInputAttachment, 1 );
    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSize );
    std::unique_ptr<vk::raii::DescriptorPool> descriptorPool =
      vk::raii::su::make_unique<vk::raii::DescriptorPool>( *device, descriptorPoolCreateInfo );

    vk::DescriptorSetAllocateInfo            descriptorSetAllocateInfo( **descriptorPool, **descriptorSetLayout );
    std::unique_ptr<vk::raii::DescriptorSet> descriptorSet = vk::raii::su::make_unique<vk::raii::DescriptorSet>(
      std::move( vk::raii::DescriptorSets( *device, descriptorSetAllocateInfo ).front() ) );

    vk::DescriptorImageInfo inputImageInfo( nullptr, **inputAttachmentView, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::WriteDescriptorSet  writeDescriptorSet(
      **descriptorSet, 0, 0, vk::DescriptorType::eInputAttachment, inputImageInfo );
    device->updateDescriptorSets( writeDescriptorSet, nullptr );

    std::unique_ptr<vk::raii::PipelineCache> pipelineCache =
      vk::raii::su::make_unique<vk::raii::PipelineCache>( *device, vk::PipelineCacheCreateInfo() );
    std::unique_ptr<vk::raii::Pipeline> graphicsPipeline =
      vk::raii::su::makeUniqueGraphicsPipeline( *device,
                                                *pipelineCache,
                                                *vertexShaderModule,
                                                nullptr,
                                                *fragmentShaderModule,
                                                nullptr,
                                                0,
                                                {},
                                                vk::FrontFace::eClockwise,
                                                false,
                                                *pipelineLayout,
                                                *renderPass );

    std::unique_ptr<vk::raii::Semaphore> imageAcquiredSemaphore =
      vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) =
      swapChainData.swapChain->acquireNextImage( vk::su::FenceTimeout, **imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    vk::ClearValue clearValue;
    clearValue.color = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    vk::RenderPassBeginInfo renderPassBeginInfo(
      **renderPass, **framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValue );
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, **graphicsPipeline );
    commandBuffer->bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics, **pipelineLayout, 0, { **descriptorSet }, nullptr );

    commandBuffer->setViewport( 0,
                                vk::Viewport( 0.0f,
                                              0.0f,
                                              static_cast<float>( surfaceData.extent.width ),
                                              static_cast<float>( surfaceData.extent.height ),
                                              0.0f,
                                              1.0f ) );
    commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer->draw( 3, 1, 0, 0 );
    commandBuffer->endRenderPass();
    commandBuffer->end();

    /* VULKAN_KEY_END */

    vk::raii::su::submitAndWait( *device, *graphicsQueue, *commandBuffer );

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
