// Copyright(c) 2023, Ilya Doroshenko. All rights reserved.
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
// VulkanHpp Samples : SharedHandles
//                     Draw a textured cube using shared handles for resource management and correct order of destruction

#define VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>
#include <thread>
#include <vulkan/vulkan_shared.hpp>

static char const * AppName    = "SharedHandles";
static char const * EngineName = "Vulkan.hpp";

std::vector<vk::SharedFramebuffer> makeSharedFramebuffers( const vk::SharedDevice &           device,
                                                           const vk::SharedRenderPass &       renderPass,
                                                           const std::vector<vk::ImageView> & imageViews,
                                                           const vk::SharedImageView &        depthImageView,
                                                           const vk::Extent2D &               extent )
{
  // show the simplified usage with VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST defined
#if defined( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
  auto                               renderPassHandle = renderPass.get();  // lvalue reference is required for the capture below
  std::vector<vk::SharedFramebuffer> sharedFramebuffers;
  std::vector<vk::Framebuffer>       framebuffers = vk::su::createFramebuffers( device, renderPassHandle, imageViews, depthImageView, extent );
#else
  auto                               renderPassHandle = renderPass.get();  // lvalue reference is required for the capture below
  std::vector<vk::SharedFramebuffer> sharedFramebuffers;
  std::vector<vk::Framebuffer>       framebuffers = vk::su::createFramebuffers( device.get(), renderPassHandle, imageViews, depthImageView.get(), extent );
#endif
  sharedFramebuffers.reserve( framebuffers.size() );
  for ( auto & framebuffer : framebuffers )
  {
    sharedFramebuffers.emplace_back( framebuffer, device );
  }

  return sharedFramebuffers;
}

class Window
{
public:
  Window( const char * windowName, vk::Extent2D extent ) : window( vk::su::createWindow( windowName, extent ) ) {}

public:
  vk::su::WindowData window;
};

class Engine
{
public:
  Engine( const vk::su::WindowData & window )
  {
    instance = vk::SharedInstance{ vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() ) };
#if !defined( NDEBUG )
    debugUtilsMessenger =
      vk::SharedDebugUtilsMessengerEXT{ instance->createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() ), instance };
#endif
    physicalDevice = instance->enumeratePhysicalDevices().front();

    createDeviceAndSwapChain( window );
    initialize();
  }

  void createDeviceAndSwapChain( const vk::su::WindowData & window )
  {
    VkSurfaceKHR surface;
    VkResult     err = glfwCreateWindowSurface( static_cast<VkInstance>( instance.get() ), window.handle, nullptr, &surface );
    if ( err != VK_SUCCESS )
      throw std::runtime_error( "Failed to create window!" );
    vk::SharedSurfaceKHR sharedSurface{ surface, instance };

    auto graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, sharedSurface.get() );
    device = vk::SharedDevice{ vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() ) };

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device.get(),
                                         sharedSurface.get(),
                                         window.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                         {},
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );
    swapChain = vk::SharedSwapchainKHR{ swapChainData.swapChain, device, sharedSurface };

    imageViews.reserve( swapChainData.images.size() );
    images.reserve( swapChainData.images.size() );

    // we don't want to destroy the images, since they are owned by the swapchain,
    // but for the consistent representation we might want shared textures
    std::transform( swapChainData.images.begin(),
                    swapChainData.images.end(),
                    std::back_inserter( images ),
                    [this]( vk::Image image ) {
                      return vk::SharedImage{ image, device, vk::SwapchainOwns::yes };
                    } );

    std::transform( swapChainData.imageViews.begin(),
                    swapChainData.imageViews.end(),
                    std::back_inserter( imageViews ),
                    [this]( vk::ImageView imageView ) {
                      return vk::SharedImageView{ imageView, device };
                    } );
    commandPool =
      vk::SharedCommandPool{ device->createCommandPool( { vk::CommandPoolCreateFlagBits::eResetCommandBuffer, graphicsAndPresentQueueFamilyIndex.first } ),
                             device };
    graphicsQueue = vk::SharedQueue{ device->getQueue( graphicsAndPresentQueueFamilyIndex.first, 0 ), device };

    presentQueue = vk::SharedQueue{ device->getQueue( graphicsAndPresentQueueFamilyIndex.second, 0 ), device };

    depthFormat = vk::Format::eD16Unorm;
    vk::su::DepthBufferData depthBufferData( physicalDevice, device.get(), depthFormat, window.extent );
    depthImage     = vk::SharedImage{ depthBufferData.image, device };
    depthImageView = vk::SharedImageView{ depthBufferData.imageView, device };
    depthMemory    = vk::SharedDeviceMemory{ depthBufferData.deviceMemory, device };

    renderPass =
      vk::SharedRenderPass{ vk::su::createRenderPass( device.get(),
                                                      vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( swapChain.getSurface().get() ) ).format,
                                                      depthFormat ),
                            device };

    framebuffers           = makeSharedFramebuffers( device, renderPass, swapChainData.imageViews, depthImageView, window.extent );
    imageAcquiredSemaphore = vk::SharedSemaphore{ device->createSemaphore( vk::SemaphoreCreateInfo() ), device };
    drawFence              = vk::SharedFence{ device->createFence( vk::FenceCreateInfo() ), device };
    // We don't need to explicitly keep sharedSurface anymore, it is owned by swapChain now.
  }

  void initialize()
  {
    commandBuffer = vk::SharedCommandBuffer{
      device->allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) ).front(), device, commandPool
    };

    auto device_handle  = device.get();
    descriptorSetLayout = vk::SharedDescriptorSetLayout{ vk::su::createDescriptorSetLayout(
                                                           device_handle,
                                                           { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                                             { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } ),
                                                         device };

    auto dsl = descriptorSetLayout.get();

    pipelineLayout = vk::SharedPipelineLayout{ device->createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), dsl ) ), device };

    glslang::InitializeProcess();
    vertexShaderModule = vk::SharedShaderModule{ vk::su::createShaderModule( device_handle, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T ), device };
    fragmentShaderModule =
      vk::SharedShaderModule{ vk::su::createShaderModule( device_handle, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C ), device };
    glslang::FinalizeProcess();

    descriptorPool = vk::SharedDescriptorPool{
      vk::su::createDescriptorPool( device_handle, { { vk::DescriptorType::eUniformBuffer, 1 }, { vk::DescriptorType::eCombinedImageSampler, 1 } } ), device
    };

    descriptorSetAllocateInfo = vk::DescriptorSetAllocateInfo( descriptorPool.get(), dsl );
    descriptorSet             = vk::SharedDescriptorSet{ device->allocateDescriptorSets( descriptorSetAllocateInfo ).front(), device, descriptorPool };

    pipelineCache    = vk::SharedPipelineCache{ device->createPipelineCache( vk::PipelineCacheCreateInfo() ), device };
    graphicsPipeline = vk::SharedPipeline{ vk::su::createGraphicsPipeline( device_handle,
                                                                           pipelineCache.get(),
                                                                           std::make_pair( vertexShaderModule.get(), nullptr ),
                                                                           std::make_pair( fragmentShaderModule.get(), nullptr ),
                                                                           sizeof( texturedCubeData[0] ),
                                                                           { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                           vk::FrontFace::eClockwise,
                                                                           true,
                                                                           pipelineLayout.get(),
                                                                           renderPass.get() ),
                                           device };

    // Get the index of the next available swapchain image:
    vk::ResultValue<uint32_t> currentBufferR = device->acquireNextImageKHR( swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
    assert( currentBufferR.result == vk::Result::eSuccess );
    assert( currentBufferR.value < framebuffers.size() );
    currentBuffer = currentBufferR.value;
  }

  void beginFrame( vk::Extent2D extent )
  {
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );
    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass.get(), framebuffers[currentBuffer].get(), vk::Rect2D( vk::Offset2D( 0, 0 ), extent ), clearValues );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
    commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline.get() );
    commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSet.get(), nullptr );

    commandBuffer->setViewport( 0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( extent.width ), static_cast<float>( extent.height ), 0.0f, 1.0f ) );
    commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), extent ) );
  }

  void endFrame()
  {
    commandBuffer->endRenderPass();
    commandBuffer->end();

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );

    auto ias    = imageAcquiredSemaphore.get();
    auto comBuf = commandBuffer.get();

    vk::SubmitInfo submitInfo( ias, waitDestinationStageMask, comBuf );
    graphicsQueue->submit( submitInfo, drawFence.get() );

    while ( vk::Result::eTimeout == device->waitForFences( drawFence.get(), VK_TRUE, vk::su::FenceTimeout ) )
      ;

    auto       swap   = swapChain.get();
    vk::Result result = presentQueue->presentKHR( vk::PresentInfoKHR( {}, swap, currentBuffer ) );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    /* VULKAN_KEY_END */
    device->waitIdle();
  }

public:
  vk::SharedSwapchainKHR           swapChain;       // swapchain owns surface, that owns Instance, which should be destroyed last
  vk::PhysicalDevice               physicalDevice;  // physical device does not have a shared handle since it is not destroyed
  vk::SharedDevice                 device;
  vk::SharedInstance               instance;  // we don't need to keep the instance, this is just for convenience
  vk::SharedDebugUtilsMessengerEXT debugUtilsMessenger;

  std::vector<vk::SharedImageView> imageViews;
  std::vector<vk::SharedImage>     images;

  uint32_t            currentBuffer = 0;
  vk::SharedSemaphore imageAcquiredSemaphore;

  // memory still needs to be before the resources that use it in order to get a proper destruction sequence.
  vk::SharedDeviceMemory depthMemory;
  vk::SharedImage        depthImage;
  vk::SharedImageView    depthImageView;
  vk::Format             depthFormat;

  vk::SharedCommandPool   commandPool;
  vk::SharedCommandBuffer commandBuffer;

  vk::SharedQueue graphicsQueue;  // queue is not destroyed, shared handle is purely for consistency
  vk::SharedQueue presentQueue;

  vk::SharedPipelineCache       pipelineCache;
  vk::SharedPipelineLayout      pipelineLayout;
  vk::SharedRenderPass          renderPass;
  vk::SharedPipeline            graphicsPipeline;
  vk::SharedDescriptorPool      descriptorPool;
  vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo;
  vk::SharedDescriptorSet       descriptorSet;
  vk::SharedDescriptorSetLayout descriptorSetLayout;

  vk::SharedShaderModule vertexShaderModule;
  vk::SharedShaderModule fragmentShaderModule;

  std::vector<vk::SharedFramebuffer> framebuffers;

  vk::SharedFence drawFence;
};

class Asset
{
public:
  Asset( const Engine & engine, vk::Extent2D extent )
  {
    auto               device_handle = engine.device.get();
    vk::su::BufferData vertexBufferData( engine.physicalDevice, device_handle, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device_handle, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vertexBuffer       = vk::SharedBuffer{ vertexBufferData.buffer, engine.device };
    vertexBufferMemory = vk::SharedDeviceMemory{ vertexBufferData.deviceMemory, engine.device };

    engine.commandBuffer->begin( vk::CommandBufferBeginInfo() );

    vk::su::BufferData uniformBufferData( engine.physicalDevice, device_handle, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4        mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( extent );
    vk::su::copyToDevice( device_handle, uniformBufferData.deviceMemory, mvpcMatrix );

    uniformBufferMemory = vk::SharedDeviceMemory{ uniformBufferData.deviceMemory, engine.device };
    uniformBuffer       = vk::SharedBuffer{ uniformBufferData.buffer, engine.device };

    vk::su::TextureData textureData( engine.physicalDevice, device_handle );
    textureData.setImage( device_handle, engine.commandBuffer.get(), vk::su::CheckerboardImageGenerator() );

    textureImage       = vk::SharedImage{ textureData.imageData->image, engine.device };
    textureImageMemory = vk::SharedDeviceMemory{ textureData.imageData->deviceMemory, engine.device };
    textureImageView   = vk::SharedImageView{ textureData.imageData->imageView, engine.device };
    textureSampler     = vk::SharedSampler{ textureData.sampler, engine.device };

    vk::su::updateDescriptorSets(
      device_handle, engine.descriptorSet.get(), { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, textureData );
    engine.commandBuffer->end();

    vk::su::submitAndWait( device_handle, engine.graphicsQueue.get(), engine.commandBuffer.get() );
  }

  void draw( vk::CommandBuffer commandBuffer )
  {
    commandBuffer.bindVertexBuffers( 0, vertexBuffer.get(), { 0 } );
    commandBuffer.draw( 12 * 3, 1, 0, 0 );
  }

  vk::SharedDeviceMemory vertexBufferMemory;
  vk::SharedBuffer       vertexBuffer;

  vk::SharedDeviceMemory uniformBufferMemory;
  vk::SharedBuffer       uniformBuffer;

  vk::SharedDeviceMemory textureImageMemory;
  vk::SharedImage        textureImage;
  vk::SharedImageView    textureImageView;
  vk::SharedSampler      textureSampler;
};

class Application
{
public:
  Application() : window( AppName, vk::Extent2D( 500, 500 ) ), engine( window.window ), asset( engine, vk::Extent2D( 500, 500 ) ) {}

  void renderFrame()
  {
    engine.beginFrame( vk::Extent2D( 500, 500 ) );
    asset.draw( engine.commandBuffer.get() );
    engine.endFrame();
  }

  int start()
  {
    renderFrame();
    return 0;
  }

private:
  // order of window, engine and asset variables is important !
  Window window;
  Engine engine;
  Asset  asset;
};

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    return Application{}.start();
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
}
