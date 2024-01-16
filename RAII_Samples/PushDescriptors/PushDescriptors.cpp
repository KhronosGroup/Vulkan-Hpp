// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : PushDescriptors
//                     Use Push Descriptors to Draw Textured Cube

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>

static char const * AppName    = "PushDescriptors";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context context;

    /* VULKAN_KEY_START */

    // To use PUSH_DESCRIPTOR, you must also specify GET_PHYSICAL_DEVICE_PROPERTIES_2
    std::vector<vk::ExtensionProperties> extensionProperties = context.enumerateInstanceExtensionProperties();
    auto                                 propertyIterator =
      std::find_if( extensionProperties.begin(),
                    extensionProperties.end(),
                    []( vk::ExtensionProperties ep ) { return ( strcmp( ep.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME ) == 0 ); } );
    if ( propertyIterator == extensionProperties.end() )
    {
      std::cout << "No GET_PHYSICAL_DEVICE_PROPERTIES_2 extension" << std::endl;
      return 0;
    }

    std::vector<std::string> instanceExtensions = vk::su::getInstanceExtensions();
    instanceExtensions.push_back( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME );

    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, instanceExtensions );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    // Once instance is created, need to make sure the extension is available
    extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();
    propertyIterator    = std::find_if( extensionProperties.begin(),
                                     extensionProperties.end(),
                                     []( vk::ExtensionProperties ep ) { return ( strcmp( ep.extensionName, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME ) == 0 ); } );
    if ( propertyIterator == extensionProperties.end() )
    {
      std::cout << "No extension for push descriptors" << std::endl;
      return 0;
    }

    std::vector<std::string> deviceExtensions = vk::su::getDeviceExtensions();
    deviceExtensions.push_back( VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME );

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::raii::Device device = vk::raii::su::makeDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, deviceExtensions );

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
    textureData.setImage( commandBuffer, vk::su::CheckerboardImageGenerator() );

    vk::raii::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4              mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::raii::su::copyToDevice( uniformBufferData.deviceMemory, mvpcMatrix );

    // Need to specify that descriptor set layout will be for push descriptors
    vk::raii::DescriptorSetLayout descriptorSetLayout =
      vk::raii::su::makeDescriptorSetLayout( device,
                                             { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                               { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } },
                                             vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR );
    vk::raii::PipelineLayout pipelineLayout( device, { {}, *descriptorSetLayout } );

    vk::Format           colorFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format;
    vk::raii::RenderPass renderPass  = vk::raii::su::makeRenderPass( device, colorFormat, depthBufferData.format );

    glslang::InitializeProcess();
    vk::raii::ShaderModule vertexShaderModule   = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::raii::ShaderModule fragmentShaderModule = vk::raii::su::makeShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::raii::Framebuffer> framebuffers =
      vk::raii::su::makeFramebuffers( device, renderPass, swapChainData.imageViews, &depthBufferData.imageView, surfaceData.extent );

    vk::raii::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::su::copyToDevice( vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

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

    vk::DescriptorBufferInfo bufferInfo( uniformBufferData.buffer, 0, sizeof( glm::mat4x4 ) );
    vk::DescriptorImageInfo  imageInfo( textureData.sampler, textureData.imageData.imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
    vk::WriteDescriptorSet   writeDescriptorSets[2] = { vk::WriteDescriptorSet( {}, 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo ),
                                                        vk::WriteDescriptorSet( {}, 1, 0, vk::DescriptorType::eCombinedImageSampler, imageInfo ) };

    // this call is from an extension and needs the dynamic dispatcher !!
    commandBuffer.pushDescriptorSetKHR( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { 2, writeDescriptorSets } );

    commandBuffer.bindVertexBuffers( 0, { vertexBufferData.buffer }, { 0 } );
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
