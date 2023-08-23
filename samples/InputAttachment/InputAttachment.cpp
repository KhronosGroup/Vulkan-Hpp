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

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

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

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( vk::Format::eR8G8B8A8Unorm );
    if ( !( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment ) )
    {
      std::cout << "vk::Format::eR8G8B8A8Unorm format unsupported for input attachment\n";
      exit( -1 );
    }

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

    /* VULKAN_KEY_START */

    // Create a framebuffer with 2 attachments, one the color attachment the shaders render into, and the other an input
    // attachment which will be cleared to yellow, and then used by the shaders to color the drawn triangle. Final
    // result should be a yellow triangle

    // Create the image that will be used as the input attachment
    // The image for the color attachment is the presentable image already created as part of the SwapChainData
    vk::ImageCreateInfo imageCreateInfo( vk::ImageCreateFlags(),
                                         vk::ImageType::e2D,
                                         swapChainData.colorFormat,
                                         vk::Extent3D( surfaceData.extent, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         vk::ImageTiling::eOptimal,
                                         vk::ImageUsageFlagBits::eInputAttachment | vk::ImageUsageFlagBits::eTransferDst );
    vk::Image           inputImage = device.createImage( imageCreateInfo );

    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements( inputImage );
    uint32_t memoryTypeIndex     = vk::su::findMemoryType( physicalDevice.getMemoryProperties(), memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlags() );
    vk::DeviceMemory inputMemory = device.allocateMemory( vk::MemoryAllocateInfo( memoryRequirements.size, memoryTypeIndex ) );
    device.bindImageMemory( inputImage, inputMemory, 0 );

    // Set the image layout to TRANSFER_DST_OPTIMAL to be ready for clear
    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    vk::su::setImageLayout( commandBuffer, inputImage, swapChainData.colorFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );

    commandBuffer.clearColorImage( inputImage,
                                   vk::ImageLayout::eTransferDstOptimal,
                                   vk::ClearColorValue( 1.0f, 1.0f, 0.0f, 0.0f ),
                                   vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS ) );

    // Transitioning the layout of the inputImage from TransferDstOptimal to ShaderReadOnlyOptimal is implicitly done by a subpassDependency in the
    // RenderPassCreateInfo below

    vk::ImageViewCreateInfo imageViewCreateInfo(
      {}, inputImage, vk::ImageViewType::e2D, swapChainData.colorFormat, {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } );
    vk::ImageView inputAttachmentView = device.createImageView( imageViewCreateInfo );

    vk::DescriptorSetLayoutBinding layoutBinding( 0, vk::DescriptorType::eInputAttachment, 1, vk::ShaderStageFlagBits::eFragment );
    vk::DescriptorSetLayout        descriptorSetLayout =
      device.createDescriptorSetLayout( vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), layoutBinding ) );

    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    std::array<vk::AttachmentDescription, 2> attachments = {
      // First attachment is the color attachment - clear at the beginning of the renderpass and transition layout to
      // PRESENT_SRC_KHR at the end of renderpass
      vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
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
      vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                 swapChainData.colorFormat,
                                 vk::SampleCountFlagBits::e1,
                                 vk::AttachmentLoadOp::eLoad,
                                 vk::AttachmentStoreOp::eDontCare,
                                 vk::AttachmentLoadOp::eDontCare,
                                 vk::AttachmentStoreOp::eDontCare,
                                 vk::ImageLayout::eTransferDstOptimal,  // transition layout from TransferDstOptimal to ShaderReadOnlyOptimal
                                 vk::ImageLayout::eShaderReadOnlyOptimal )
    };
    vk::AttachmentReference  colorReference( 0, vk::ImageLayout::eColorAttachmentOptimal );
    vk::AttachmentReference  inputReference( 1, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::SubpassDescription   subpassDescription( {}, vk::PipelineBindPoint::eGraphics, inputReference, colorReference );
    vk::SubpassDependency    subpassDependency( VK_SUBPASS_EXTERNAL,
                                             0,
                                             vk::PipelineStageFlagBits::eTransfer,
                                             vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eFragmentShader,
                                             vk::AccessFlagBits::eTransferWrite,
                                             vk::AccessFlagBits::eColorAttachmentWrite  // needed for first attachment
                                               | vk::AccessFlagBits::eInputAttachmentRead | vk::AccessFlagBits::eShaderRead |
                                               vk::AccessFlagBits::eColorAttachmentRead  // needed for second attachment
    );
    vk::RenderPassCreateInfo renderPassCreateInfo( {}, attachments, subpassDescription, subpassDependency );
    vk::RenderPass           renderPass = device.createRenderPass( renderPassCreateInfo );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, inputAttachmentView, surfaceData.extent );

    vk::DescriptorPoolSize poolSize( vk::DescriptorType::eInputAttachment, 1 );
    vk::DescriptorPool     descriptorPool =
      device.createDescriptorPool( vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSize ) );

    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayout );
    vk::DescriptorSet             descriptorSet = device.allocateDescriptorSets( descriptorSetAllocateInfo ).front();

    vk::DescriptorImageInfo inputImageInfo( nullptr, inputAttachmentView, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::WriteDescriptorSet  writeDescriptorSet( descriptorSet, 0, 0, vk::DescriptorType::eInputAttachment, inputImageInfo );
    device.updateDescriptorSets( writeDescriptorSet, nullptr );

    vk::PipelineCache pipelineCache    = device.createPipelineCache( vk::PipelineCacheCreateInfo() );
    vk::Pipeline      graphicsPipeline = vk::su::createGraphicsPipeline( device,
                                                                    pipelineCache,
                                                                    std::make_pair( vertexShaderModule, nullptr ),
                                                                    std::make_pair( fragmentShaderModule, nullptr ),
                                                                    0,
                                                                         {},
                                                                    vk::FrontFace::eClockwise,
                                                                    false,
                                                                    pipelineLayout,
                                                                    renderPass );

    vk::Semaphore imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo() );

    vk::ResultValue<uint32_t> nexImage =
      device.acquireNextImage2KHR( vk::AcquireNextImageInfoKHR( swapChainData.swapChain, UINT64_MAX, imageAcquiredSemaphore, {}, 1 ) );
    assert( nexImage.result == vk::Result::eSuccess );
    uint32_t currentBuffer = nexImage.value;

    vk::ClearValue clearValue;
    clearValue.color = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    commandBuffer.beginRenderPass(
      vk::RenderPassBeginInfo( renderPass, framebuffers[currentBuffer], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValue ),
      vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, nullptr );

    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer.draw( 3, 1, 0, 0 );
    commandBuffer.endRenderPass();
    commandBuffer.end();

    /* VULKAN_KEY_END */

    vk::su::submitAndWait( device, graphicsQueue, commandBuffer );

    vk::Result result = presentQueue.presentKHR( vk::PresentInfoKHR( {}, swapChainData.swapChain, currentBuffer ) );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    device.destroySemaphore( imageAcquiredSemaphore );
    device.destroyPipeline( graphicsPipeline );
    device.destroyPipelineCache( pipelineCache );
    device.freeDescriptorSets( descriptorPool, descriptorSet );
    device.destroyDescriptorPool( descriptorPool );
    for ( auto framebuffer : framebuffers )
    {
      device.destroyFramebuffer( framebuffer );
    }
    device.destroyShaderModule( fragmentShaderModule );
    device.destroyShaderModule( vertexShaderModule );
    device.destroyRenderPass( renderPass );
    device.destroyPipelineLayout( pipelineLayout );
    device.destroyDescriptorSetLayout( descriptorSetLayout );
    device.destroyImageView( inputAttachmentView );
    device.destroyImage( inputImage );  // destroy the inputImage before freeing the bound inputMemory !
    device.freeMemory( inputMemory );
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
