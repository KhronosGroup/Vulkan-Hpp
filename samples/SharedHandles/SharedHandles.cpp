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
// VulkanHpp Samples : DrawTexturedCube
//                     Draw a textured cube

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>

static char const * AppName    = "DrawTexturedCube";
static char const * EngineName = "Vulkan.hpp";

struct Demo
{
  Demo()
  {
    instance = vk::SharedInstance{ vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() ) };
#if !defined( NDEBUG )
    debugUtilsMessenger =
      vk::SharedDebugUtilsMessengerEXT{ instance->createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() ), instance };
#endif

    physicalDevice = instance->enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance.get(), AppName, vk::Extent2D( 500, 500 ) );
    extent  = surfaceData.extent;
    surface = vk::SharedSurfaceKHR{ surfaceData.surface, instance };
    windowData = std::make_unique<vk::su::WindowData>( std::move( surfaceData.window ) );

    graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    device = vk::SharedDevice{ vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() ) };
  }

  std::pair<uint32_t, uint32_t>       graphicsAndPresentQueueFamilyIndex; // the objects are out of order to verify correct handling of shared handles
  vk::SharedDebugUtilsMessengerEXT    debugUtilsMessenger;
  vk::SharedDevice                    device;
  vk::PhysicalDevice                  physicalDevice;  // physical device does not have a shared handle since it is not destroyed
  vk::SharedInstance                  instance;
  std::unique_ptr<vk::su::WindowData> windowData;
  vk::SharedSurfaceKHR                surface;
  vk::Extent2D                        extent;
};

std::vector<vk::SharedFramebuffer> makeSharedFramebuffers( vk::SharedDevice                   device,
                                                           vk::RenderPass                     renderPass,
                                                           std::vector<vk::ImageView> const & imageViews,
                                                           vk::ImageView const &              depthImageView,
                                                           vk::Extent2D const &               extent )
{
  std::vector<vk::SharedFramebuffer> framebuffers;
  std::vector<vk::Framebuffer>       framebuffers_unsafe = vk::su::createFramebuffers( device.get(), renderPass, imageViews, depthImageView, extent );
  framebuffers.reserve( framebuffers_unsafe.size() );
  for ( auto & framebuffer : framebuffers_unsafe )
  {
    framebuffers.emplace_back( framebuffer, device );
  }

  return framebuffers;
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::unique_ptr<Demo> demo{ new Demo };

    auto       device_handle = demo->device;
    vk::Device device        = device_handle.get();

    vk::SharedCommandPool   commandPool{ vk::su::createCommandPool( demo->device.get(), demo->graphicsAndPresentQueueFamilyIndex.first ), demo->device };
    vk::SharedCommandBuffer commandBuffer{
      demo->device->allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) ).front(),
      demo->device,
      commandPool.get()
    };

    vk::Queue graphicsQueue = device.getQueue( demo->graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::Queue presentQueue  = device.getQueue( demo->graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::su::SwapChainData swapChainData( demo->physicalDevice,
                                         device,
                                         demo->surface.get(),
                                         demo->extent,
                                         vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                         {},
                                         demo->graphicsAndPresentQueueFamilyIndex.first,
                                         demo->graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( demo->physicalDevice, device, vk::Format::eD16Unorm, demo->extent );

    vk::su::TextureData textureData( demo->physicalDevice, device );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( device, commandBuffer.get(), vk::su::CheckerboardImageGenerator() );

    vk::su::BufferData uniformBufferData( demo->physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4        mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( demo->extent );
    vk::su::copyToDevice( device, uniformBufferData.deviceMemory, mvpcMatrix );

    vk::SharedDescriptorSetLayout descriptorSetLayout{ vk::su::createDescriptorSetLayout(
                                                         device,
                                                         { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                                           { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } ),
                                                       device_handle };

    auto dsl = descriptorSetLayout.get();

    vk::SharedPipelineLayout pipelineLayout{ device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), dsl ) ),
                                             device_handle };

    vk::SharedRenderPass renderPass{
      vk::su::createRenderPass(
        device, vk::su::pickSurfaceFormat( demo->physicalDevice.getSurfaceFormatsKHR( demo->surface.get() ) ).format, depthBufferData.format ),
      device_handle
    };

    glslang::InitializeProcess();
    vk::SharedShaderModule vertexShaderModule{ vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T ), device_handle };
    vk::SharedShaderModule fragmentShaderModule{ vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C ),
                                                 device_handle };
    glslang::FinalizeProcess();

    auto framebuffers = makeSharedFramebuffers( device_handle, renderPass.get(), swapChainData.imageViews, depthBufferData.imageView, demo->extent );

    vk::su::BufferData vertexBufferData( demo->physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vk::SharedDescriptorPool descriptorPool{
      vk::su::createDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 1 }, { vk::DescriptorType::eCombinedImageSampler, 1 } } ), device_handle
    };

    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool.get(), dsl );
    vk::SharedDescriptorSet       descriptorSet{ device.allocateDescriptorSets( descriptorSetAllocateInfo ).front(), device_handle, descriptorPool.get() };

    vk::su::updateDescriptorSets(
      device, descriptorSet.get(), { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, textureData );

    vk::SharedPipelineCache pipelineCache{ device.createPipelineCache( vk::PipelineCacheCreateInfo() ), device_handle };
    vk::SharedPipeline      graphicsPipeline{ vk::su::createGraphicsPipeline( device,
                                                                         pipelineCache.get(),
                                                                         std::make_pair( vertexShaderModule.get(), nullptr ),
                                                                         std::make_pair( fragmentShaderModule.get(), nullptr ),
                                                                         sizeof( texturedCubeData[0] ),
                                                                              { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                         vk::FrontFace::eClockwise,
                                                                         true,
                                                                         pipelineLayout.get(),
                                                                         renderPass.get() ),
                                         device_handle };

    // Get the index of the next available swapchain image:
    vk::SharedSemaphore       imageAcquiredSemaphore{ device.createSemaphore( vk::SemaphoreCreateInfo() ), device_handle };
    vk::ResultValue<uint32_t> currentBuffer =
      device.acquireNextImageKHR( swapChainData.swapChain, vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    vk::RenderPassBeginInfo renderPassBeginInfo(
      renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D( vk::Offset2D( 0, 0 ), demo->extent ), clearValues );
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline.get() );
    commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSet.get(), nullptr );

    commandBuffer->bindVertexBuffers( 0, vertexBufferData.buffer, { 0 } );
    commandBuffer->setViewport( 0,
                                vk::Viewport( 0.0f, 0.0f, static_cast<float>( demo->extent.width ), static_cast<float>( demo->extent.height ), 0.0f, 1.0f ) );
    commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), demo->extent ) );

    commandBuffer->draw( 12 * 3, 1, 0, 0 );
    commandBuffer->endRenderPass();
    commandBuffer->end();

    vk::SharedFence drawFence{ device.createFence( vk::FenceCreateInfo() ), device_handle };

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );

    auto ias    = imageAcquiredSemaphore.get();
    auto comBuf = commandBuffer.get();

    vk::SubmitInfo submitInfo( ias, waitDestinationStageMask, comBuf );
    graphicsQueue.submit( submitInfo, drawFence.get() );

    while ( vk::Result::eTimeout == device.waitForFences( drawFence.get(), VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::Result result = presentQueue.presentKHR( vk::PresentInfoKHR( {}, swapChainData.swapChain, currentBuffer.value ) );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    /* VULKAN_KEY_END */
    device.waitIdle();

    vertexBufferData.clear( device );
    uniformBufferData.clear( device );
    textureData.clear( device );
    depthBufferData.clear( device );
    swapChainData.clear( device );
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
