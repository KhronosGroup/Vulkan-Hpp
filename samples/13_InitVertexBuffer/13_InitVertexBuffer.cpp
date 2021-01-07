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
// VulkanHpp Samples : 13_InitVertexBuffer
//                     Initialize vertex buffer

#include "../utils/geometries.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <iostream>

static char const * AppName    = "13_InitVertexBuffer";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger( instance );
#endif

    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 64, 64 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, *surfaceData.surface );
    vk::UniqueDevice device =
      vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::UniqueCommandPool   commandPool = vk::su::createCommandPool( device, graphicsAndPresentQueueFamilyIndex.first );
    vk::UniqueCommandBuffer commandBuffer = std::move( device
                                                         ->allocateCommandBuffersUnique( vk::CommandBufferAllocateInfo(
                                                           commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) )
                                                         .front() );

    vk::Queue graphicsQueue = device->getQueue( graphicsAndPresentQueueFamilyIndex.first, 0 );

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device,
                                         *surfaceData.surface,
                                         surfaceData.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment |
                                           vk::ImageUsageFlagBits::eTransferSrc,
                                         vk::UniqueSwapchainKHR(),
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::UniqueRenderPass renderPass =
      vk::su::createRenderPass( device, swapChainData.colorFormat, depthBufferData.format );

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(
      device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    /* VULKAN_KEY_START */

    // create a vertex buffer for some vertex and color data
    vk::UniqueBuffer vertexBuffer = device->createBufferUnique( vk::BufferCreateInfo(
      vk::BufferCreateFlags(), sizeof( coloredCubeData ), vk::BufferUsageFlagBits::eVertexBuffer ) );

    // allocate device memory for that buffer
    vk::MemoryRequirements memoryRequirements = device->getBufferMemoryRequirements( vertexBuffer.get() );
    uint32_t               memoryTypeIndex =
      vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                              memoryRequirements.memoryTypeBits,
                              vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::UniqueDeviceMemory deviceMemory =
      device->allocateMemoryUnique( vk::MemoryAllocateInfo( memoryRequirements.size, memoryTypeIndex ) );

    // copy the vertex and color data into that device memory
    uint8_t * pData = static_cast<uint8_t *>( device->mapMemory( deviceMemory.get(), 0, memoryRequirements.size ) );
    memcpy( pData, coloredCubeData, sizeof( coloredCubeData ) );
    device->unmapMemory( deviceMemory.get() );

    // and bind the device memory to the vertex buffer
    device->bindBufferMemory( vertexBuffer.get(), deviceMemory.get(), 0 );

    vk::UniqueSemaphore imageAcquiredSemaphore =
      device->createSemaphoreUnique( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) );
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(
      swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    commandBuffer->begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );

    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass.get(),
                                                 framebuffers[currentBuffer.value].get(),
                                                 vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ),
                                                 clearValues );
    commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );

    commandBuffer->bindVertexBuffers( 0, *vertexBuffer, { 0 } );

    commandBuffer->endRenderPass();
    commandBuffer->end();
    vk::su::submitAndWait( device, graphicsQueue, commandBuffer );

    // Note: No need to explicitly destroy the vertexBuffer, deviceMemory, or semaphore, as the destroy functions are
    // called by the destructor of the UniqueBuffer, UniqueDeviceMemory, and UniqueSemaphore, respectively, on leaving
    // this scope.

    /* VULKAN_KEY_END */
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
