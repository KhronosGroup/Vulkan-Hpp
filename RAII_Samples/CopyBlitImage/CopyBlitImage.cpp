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
// VulkanHpp Samples : CopyBlitImage
//                     Draw a cube

#include "../utils/utils.hpp"

#include <thread>

static char const * AppName    = "CopyBlitImage";
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

    vk::raii::su::SurfaceData surfaceData( *instance, AppName, vk::Extent2D( 640, 640 ) );

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilitiesKHR( **surfaceData.surface );
    if ( !( surfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferDst ) )
    {
      std::cout << "Surface cannot be destination of blit - abort \n";
      exit( -1 );
    }

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
                                                 vk::ImageUsageFlagBits::eTransferSrc |
                                                 vk::ImageUsageFlagBits::eTransferDst,
                                               {},
                                               graphicsAndPresentQueueFamilyIndex.first,
                                               graphicsAndPresentQueueFamilyIndex.second );

    /* VULKAN_KEY_START */

    vk::FormatProperties formatProperties = physicalDevice->getFormatProperties( swapChainData.colorFormat );
    assert( ( formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc ) &&
            "Format cannot be used as transfer source" );

    std::unique_ptr<vk::raii::Semaphore> imageAcquiredSemaphore =
      vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );

    // Get the index of the next available swapchain image:
    vk::Result result;
    uint32_t   imageIndex;
    std::tie( result, imageIndex ) =
      swapChainData.swapChain->acquireNextImage( vk::su::FenceTimeout, **imageAcquiredSemaphore );
    assert( result == vk::Result::eSuccess );
    assert( imageIndex < swapChainData.images.size() );

    commandBuffer->begin( vk::CommandBufferBeginInfo() );
    vk::raii::su::setImageLayout( *commandBuffer,
                                  static_cast<vk::Image>( swapChainData.images[imageIndex] ),
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eUndefined,
                                  vk::ImageLayout::eTransferDstOptimal );

    // Create an image, map it, and write some values to the image
    vk::ImageCreateInfo              imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         swapChainData.colorFormat,
                                         vk::Extent3D( surfaceData.extent, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         vk::ImageTiling::eLinear,
                                         vk::ImageUsageFlagBits::eTransferSrc );
    std::unique_ptr<vk::raii::Image> blitSourceImage = vk::raii::su::make_unique<vk::raii::Image>( *device, imageCreateInfo );

    vk::PhysicalDeviceMemoryProperties memoryProperties   = physicalDevice->getMemoryProperties();
    vk::MemoryRequirements             memoryRequirements = blitSourceImage->getMemoryRequirements();
    uint32_t                           memoryTypeIndex    = vk::su::findMemoryType(
      memoryProperties, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible );

    vk::MemoryAllocateInfo                  memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    std::unique_ptr<vk::raii::DeviceMemory> deviceMemory =
      vk::raii::su::make_unique<vk::raii::DeviceMemory>( *device, memoryAllocateInfo );
    blitSourceImage->bindMemory( **deviceMemory, 0 );

    vk::raii::su::setImageLayout( *commandBuffer,
                                  **blitSourceImage,
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eUndefined,
                                  vk::ImageLayout::eGeneral );

    commandBuffer->end();

    /* Queue the command buffer for execution */
    std::unique_ptr<vk::raii::Fence> commandFence = vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo() );
    vk::PipelineStageFlags           waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo                   submitInfo( **imageAcquiredSemaphore, waitDestinationStageMask, **commandBuffer );
    graphicsQueue->submit( submitInfo, **commandFence );

    /* Make sure command buffer is finished before mapping */
    while ( device->waitForFences( { **commandFence }, true, vk::su::FenceTimeout ) == vk::Result::eTimeout )
      ;

    unsigned char * pImageMemory =
      static_cast<unsigned char *>( deviceMemory->mapMemory( 0, memoryRequirements.size ) );

    // Checkerboard of 8x8 pixel squares
    for ( uint32_t row = 0; row < surfaceData.extent.height; row++ )
    {
      for ( uint32_t col = 0; col < surfaceData.extent.width; col++ )
      {
        unsigned char rgb = ( ( ( row & 0x8 ) == 0 ) ^ ( ( col & 0x8 ) == 0 ) ) * 255;
        pImageMemory[0]   = rgb;
        pImageMemory[1]   = rgb;
        pImageMemory[2]   = rgb;
        pImageMemory[3]   = 255;
        pImageMemory += 4;
      }
    }

    // Flush the mapped memory and then unmap it. Assume it isn't coherent since we didn't really confirm
    vk::MappedMemoryRange mappedMemoryRange( **deviceMemory, 0, memoryRequirements.size );
    device->flushMappedMemoryRanges( mappedMemoryRange );
    deviceMemory->unmapMemory();

    commandBuffer->reset( {} );
    commandBuffer->begin( vk::CommandBufferBeginInfo() );

    // Intend to blit from this image, set the layout accordingly
    vk::raii::su::setImageLayout( *commandBuffer,
                                  **blitSourceImage,
                                  swapChainData.colorFormat,
                                  vk::ImageLayout::eGeneral,
                                  vk::ImageLayout::eTransferSrcOptimal );

    vk::Image blitDestinationImage = static_cast<vk::Image>( swapChainData.images[imageIndex] );

    // Do a 32x32 blit to all of the dst image - should get big squares
    vk::ImageSubresourceLayers imageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 );
    vk::ImageBlit              imageBlit(
      imageSubresourceLayers,
      { { vk::Offset3D( 0, 0, 0 ), vk::Offset3D( 32, 32, 1 ) } },
      imageSubresourceLayers,
      { { vk::Offset3D( 0, 0, 0 ), vk::Offset3D( surfaceData.extent.width, surfaceData.extent.height, 1 ) } } );
    commandBuffer->blitImage( **blitSourceImage,
                              vk::ImageLayout::eTransferSrcOptimal,
                              blitDestinationImage,
                              vk::ImageLayout::eTransferDstOptimal,
                              imageBlit,
                              vk::Filter::eLinear );

    // Use a barrier to make sure the blit is finished before the copy starts
    vk::ImageMemoryBarrier memoryBarrier( vk::AccessFlagBits::eTransferWrite,
                                          vk::AccessFlagBits::eMemoryRead,
                                          vk::ImageLayout::eTransferDstOptimal,
                                          vk::ImageLayout::eTransferDstOptimal,
                                          VK_QUEUE_FAMILY_IGNORED,
                                          VK_QUEUE_FAMILY_IGNORED,
                                          blitDestinationImage,
                                          vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );
    commandBuffer->pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, {}, nullptr, nullptr, memoryBarrier );

    // Do a image copy to part of the dst image - checks should stay small
    vk::ImageCopy imageCopy( imageSubresourceLayers,
                             vk::Offset3D(),
                             imageSubresourceLayers,
                             vk::Offset3D( 256, 256, 0 ),
                             vk::Extent3D( 128, 128, 1 ) );
    commandBuffer->copyImage( **blitSourceImage,
                              vk::ImageLayout::eTransferSrcOptimal,
                              blitDestinationImage,
                              vk::ImageLayout::eTransferDstOptimal,
                              imageCopy );

    vk::ImageMemoryBarrier prePresentBarrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eMemoryRead,
      vk::ImageLayout::eTransferDstOptimal,
      vk::ImageLayout::ePresentSrcKHR,
      VK_QUEUE_FAMILY_IGNORED,
      VK_QUEUE_FAMILY_IGNORED,
      blitDestinationImage,
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );
    commandBuffer->pipelineBarrier( vk::PipelineStageFlagBits::eTransfer,
                                    vk::PipelineStageFlagBits::eTopOfPipe,
                                    {},
                                    nullptr,
                                    nullptr,
                                    prePresentBarrier );
    commandBuffer->end();

    std::unique_ptr<vk::raii::Fence> drawFence = vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo() );
    submitInfo                                 = vk::SubmitInfo( {}, {}, **commandBuffer );
    graphicsQueue->submit( submitInfo, **drawFence );
    graphicsQueue->waitIdle();

    /* Make sure command buffer is finished before presenting */
    while ( device->waitForFences( { **drawFence }, true, vk::su::FenceTimeout ) == vk::Result::eTimeout )
      ;

    /* Now present the image in the window */
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
