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

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __GNUC__ )
#  if ( 9 <= __GNUC__ )
#    pragma GCC diagnostic ignored "-Winit-list-lifetime"
#  endif
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../../samples/utils/geometries.hpp"
#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "13_InitVertexBuffer";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 64, 64 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::raii::Device device = vk::raii::su::makeDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::raii::CommandPool   commandPool   = vk::raii::CommandPool( device, { {}, graphicsAndPresentQueueFamilyIndex.first } );
    vk::raii::CommandBuffer commandBuffer = vk::raii::su::makeCommandBuffer( device, commandPool );

    vk::raii::Queue graphicsQueue( device, graphicsAndPresentQueueFamilyIndex.first, 0 );

    vk::raii::su::SwapChainData swapChainData( physicalDevice,
                                               device,
                                               surfaceData.surface,
                                               surfaceData.extent,
                                               vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                               {},
                                               graphicsAndPresentQueueFamilyIndex.first,
                                               graphicsAndPresentQueueFamilyIndex.second );

    vk::raii::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::raii::RenderPass renderPass = vk::raii::su::makeRenderPass( device, swapChainData.colorFormat, depthBufferData.format );

    std::vector<vk::raii::Framebuffer> framebuffers =
      vk::raii::su::makeFramebuffers( device, renderPass, swapChainData.imageViews, &depthBufferData.imageView, surfaceData.extent );

    /* VULKAN_KEY_START */

    // in order to get a clean desctruction sequence, instantiate the DeviceMemory for the vertex buffer first
    vk::raii::DeviceMemory deviceMemory( nullptr );

    // create a vertex buffer for some vertex and color data
    vk::BufferCreateInfo bufferCreateInfo( {}, sizeof( coloredCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::Buffer     vertexBuffer( device, bufferCreateInfo );

    // allocate device memory for that buffer
    vk::MemoryRequirements memoryRequirements = vertexBuffer.getMemoryRequirements();
    uint32_t               memoryTypeIndex    = vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                                                       memoryRequirements.memoryTypeBits,
                                                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    deviceMemory = vk::raii::DeviceMemory( device, memoryAllocateInfo );

    // copy the vertex and color data into that device memory
    uint8_t * pData = static_cast<uint8_t *>( deviceMemory.mapMemory( 0, memoryRequirements.size ) );
    memcpy( pData, coloredCubeData, sizeof( coloredCubeData ) );
    deviceMemory.unmapMemory();

    // and bind the device memory to the vertex buffer
    vertexBuffer.bindMemory( deviceMemory, 0 );

    vk::raii::Semaphore imageAcquiredSemaphore( device, vk::SemaphoreCreateInfo() );

    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) = swapChainData.swapChain.acquireNextImage( vk::su::FenceTimeout, imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    commandBuffer.begin( {} );

    vk::RenderPassBeginInfo renderPassBeginInfo( renderPass, framebuffers[imageIndex], vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ), clearValues );
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );

    commandBuffer.bindVertexBuffers( 0, { vertexBuffer }, { 0 } );

    commandBuffer.endRenderPass();
    commandBuffer.end();
    vk::raii::su::submitAndWait( device, graphicsQueue, commandBuffer );

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
