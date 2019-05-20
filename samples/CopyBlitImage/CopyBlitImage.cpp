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
#include "vulkan/vulkan.hpp"

static char const* AppName = "CopyBlitImage";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(640, 640));

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevices[0].getSurfaceCapabilitiesKHR(surfaceData.surface.get());
    if (!(surfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferDst))
    {
      std::cout << "Surface cannot be destination of blit - abort \n";
      exit(-1);
    }

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::su::SwapChainData swapChainData(physicalDevices[0], device, surfaceData.surface, surfaceData.extent, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst
      , graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);

    /* VULKAN_KEY_START */

    vk::FormatProperties formatProperties = physicalDevices[0].getFormatProperties(swapChainData.colorFormat);
    assert((formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc) && "Format cannot be used as transfer source");

    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());

    // Get the index of the next available swapchain image:
    vk::ResultValue<uint32_t> nextImage = device->acquireNextImageKHR(swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
    assert(nextImage.result == vk::Result::eSuccess);
    assert(nextImage.value < swapChainData.images.size());
    uint32_t currentBuffer = nextImage.value;

    commandBuffers[0]->begin(vk::CommandBufferBeginInfo());
    vk::su::setImageLayout(commandBuffers[0], swapChainData.images[currentBuffer], swapChainData.colorFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

    // Create an image, map it, and write some values to the image
    vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, swapChainData.colorFormat, vk::Extent3D(surfaceData.extent, 1), 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eLinear, vk::ImageUsageFlagBits::eTransferSrc);
    vk::UniqueImage blitSourceImage = device->createImageUnique(imageCreateInfo);

    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevices[0].getMemoryProperties();
    vk::MemoryRequirements memoryRequirements = device->getImageMemoryRequirements(blitSourceImage.get());
    uint32_t memoryTypeIndex = vk::su::findMemoryType(memoryProperties, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible);

    vk::UniqueDeviceMemory deviceMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex));
    device->bindImageMemory(blitSourceImage.get(), deviceMemory.get(), 0);

    vk::su::setImageLayout(commandBuffers[0], blitSourceImage.get(), swapChainData.colorFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral);

    commandBuffers[0]->end();

    /* Queue the command buffer for execution */
    vk::UniqueFence commandFence = device->createFenceUnique({});
    vk::PipelineStageFlags pipeStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    graphicsQueue.submit(vk::SubmitInfo(1, &imageAcquiredSemaphore.get(), &pipeStageFlags, 1, &commandBuffers[0].get()), commandFence.get());

    /* Make sure command buffer is finished before mapping */
    while (device->waitForFences(commandFence.get(), true, vk::su::FenceTimeout) == vk::Result::eTimeout)
      ;

    unsigned char* pImageMemory = static_cast<unsigned char*>(device->mapMemory(deviceMemory.get(), 0, memoryRequirements.size));

    // Checkerboard of 8x8 pixel squares
    for (uint32_t row = 0; row < surfaceData.extent.height; row++)
    {
      for (uint32_t col = 0; col < surfaceData.extent.width; col++)
      {
        unsigned char rgb = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
        pImageMemory[0] = rgb;
        pImageMemory[1] = rgb;
        pImageMemory[2] = rgb;
        pImageMemory[3] = 255;
        pImageMemory += 4;
      }
    }

    // Flush the mapped memory and then unmap it. Assume it isn't coherent since we didn't really confirm
    device->flushMappedMemoryRanges(vk::MappedMemoryRange(deviceMemory.get(), 0, memoryRequirements.size));
    device->unmapMemory(deviceMemory.get());

    commandBuffers[0]->reset({});
    commandBuffers[0]->begin(vk::CommandBufferBeginInfo());

    // Intend to blit from this image, set the layout accordingly
    vk::su::setImageLayout(commandBuffers[0], blitSourceImage.get(), swapChainData.colorFormat, vk::ImageLayout::eGeneral, vk::ImageLayout::eTransferSrcOptimal);

    vk::Image blitDestinationImage = swapChainData.images[currentBuffer];

    // Do a 32x32 blit to all of the dst image - should get big squares
    vk::ImageSubresourceLayers imageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
    vk::ImageBlit imageBlit(imageSubresourceLayers, { { vk::Offset3D(0, 0, 0), vk::Offset3D(32, 32, 1) } }, imageSubresourceLayers, { { vk::Offset3D(0, 0, 0), vk::Offset3D(surfaceData.extent.width, surfaceData.extent.height, 1) } });
    commandBuffers[0]->blitImage(blitSourceImage.get(), vk::ImageLayout::eTransferSrcOptimal, blitDestinationImage, vk::ImageLayout::eTransferDstOptimal, imageBlit, vk::Filter::eLinear);

    // Use a barrier to make sure the blit is finished before the copy starts
    vk::ImageMemoryBarrier memoryBarrier(vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eMemoryRead, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferDstOptimal,
      VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, blitDestinationImage, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
    commandBuffers[0]->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags(), nullptr, nullptr, memoryBarrier);

    // Do a image copy to part of the dst image - checks should stay small
    vk::ImageCopy imageCopy(imageSubresourceLayers, vk::Offset3D(), imageSubresourceLayers, vk::Offset3D(256, 256, 0), vk::Extent3D(128, 128, 1));
    commandBuffers[0]->copyImage(blitSourceImage.get(), vk::ImageLayout::eTransferSrcOptimal, blitDestinationImage, vk::ImageLayout::eTransferDstOptimal, imageCopy);

    vk::ImageMemoryBarrier prePresentBarrier(vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eMemoryRead, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR,
      VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, swapChainData.images[currentBuffer], vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
    commandBuffers[0]->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTopOfPipe, vk::DependencyFlags(), nullptr, nullptr, prePresentBarrier);
    commandBuffers[0]->end();

    vk::UniqueFence drawFence = device->createFenceUnique({});
    graphicsQueue.submit(vk::SubmitInfo(0, nullptr, nullptr, 1, &commandBuffers[0].get()), drawFence.get());
    graphicsQueue.waitIdle();

    /* Make sure command buffer is finished before presenting */
    while (device->waitForFences(drawFence.get(), true, vk::su::FenceTimeout) == vk::Result::eTimeout)
      ;

    /* Now present the image in the window */
    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChainData.swapChain.get(), &currentBuffer, nullptr));
    Sleep(1000);

    /* VULKAN_KEY_END */

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    DestroyWindow(surfaceData.window);
#else
#pragma error "unhandled platform"
#endif
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error err)
  {
    std::cout << "std::runtime_error: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
