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
// VulkanHpp Samples : InitTexture
//                     Initialize texture

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "InitTexture";
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

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(50, 50));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    /* VULKAN_KEY_START */

    vk::Format format = vk::Format::eR8G8B8A8Unorm;
    vk::FormatProperties formatProperties = physicalDevices[0].getFormatProperties(format);

    // See if we can use a linear tiled image for a texture, if not, we will need a staging buffer for the texture data
    bool needsStaging = !(formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);

    vk::UniqueImage image = device->createImageUnique(vk::ImageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, format, vk::Extent3D(surfaceData.extent, 1), 1, 1,
      vk::SampleCountFlagBits::e1, needsStaging ? vk::ImageTiling::eOptimal : vk::ImageTiling::eLinear,
      vk::ImageUsageFlagBits::eSampled | (needsStaging ? vk::ImageUsageFlagBits::eTransferDst : vk::ImageUsageFlagBits()),
      vk::SharingMode::eExclusive, 0, nullptr, needsStaging ? vk::ImageLayout::eUndefined : vk::ImageLayout::ePreinitialized));

    vk::MemoryRequirements memoryRequirements = device->getImageMemoryRequirements(image.get());
    uint32_t memoryTypeIndex = vk::su::findMemoryType(physicalDevices[0].getMemoryProperties(), memoryRequirements.memoryTypeBits,
      needsStaging ? vk::MemoryPropertyFlags() : (vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

    // allocate memory
    vk::UniqueDeviceMemory imageMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex));

    // bind memory
    device->bindImageMemory(image.get(), imageMemory.get(), 0);

    vk::UniqueBuffer textureBuffer;
    vk::UniqueDeviceMemory textureBufferMemory;
    if (needsStaging)
    {
      // Need a staging buffer to map and copy texture into
      textureBuffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), surfaceData.extent.width * surfaceData.extent.height * 4, vk::BufferUsageFlagBits::eTransferSrc));

      memoryRequirements = device->getBufferMemoryRequirements(textureBuffer.get());
      memoryTypeIndex = vk::su::findMemoryType(physicalDevices[0].getMemoryProperties(), memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

      // allocate memory
      textureBufferMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex));

      // bind memory
      device->bindBufferMemory(textureBuffer.get(), textureBufferMemory.get(), 0);
    }
    else
    {
      vk::SubresourceLayout subresourceLayout = device->getImageSubresourceLayout(image.get(), vk::ImageSubresource(vk::ImageAspectFlagBits::eColor));
    }

    void* data = device->mapMemory(needsStaging ? textureBufferMemory.get() : imageMemory.get(), 0, memoryRequirements.size, vk::MemoryMapFlags());

    // Checkerboard of 16x16 pixel squares
    unsigned char *pImageMemory = static_cast<unsigned char*>(data);
    for (uint32_t row = 0; row < surfaceData.extent.height; row++)
    {
      for (uint32_t col = 0; col < surfaceData.extent.width; col++)
      {
        unsigned char rgb = (((row & 0x10) == 0) ^ ((col & 0x10) == 0)) * 255;
        pImageMemory[0] = rgb;
        pImageMemory[1] = rgb;
        pImageMemory[2] = rgb;
        pImageMemory[3] = 255;
        pImageMemory += 4;
      }
    }

    device->unmapMemory(needsStaging ? textureBufferMemory.get() : imageMemory.get());

    commandBuffers[0]->begin(vk::CommandBufferBeginInfo());
    if (needsStaging)
    {
      // Since we're going to blit to the texture image, set its layout to eTransferDstOptimal
      vk::su::setImageLayout(commandBuffers[0], image.get(), format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
      vk::BufferImageCopy copyRegion(0, surfaceData.extent.width, surfaceData.extent.height, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1), vk::Offset3D(0, 0, 0), vk::Extent3D(surfaceData.extent, 1));
      commandBuffers[0]->copyBufferToImage(textureBuffer.get(), image.get(), vk::ImageLayout::eTransferDstOptimal, copyRegion);
      // Set the layout for the texture image from eTransferDstOptimal to SHADER_READ_ONLY
      vk::su::setImageLayout(commandBuffers[0], image.get(), format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
    }
    else
    {
      // If we can use the linear tiled image as a texture, just do it
      vk::su::setImageLayout(commandBuffers[0], image.get(), format, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eShaderReadOnlyOptimal);
    }

    commandBuffers[0]->end();
    vk::su::submitAndWait(device, graphicsQueue, commandBuffers[0]);

    vk::UniqueSampler sampler = device->createSamplerUnique(vk::SamplerCreateInfo(vk::SamplerCreateFlags(), vk::Filter::eNearest, vk::Filter::eNearest, vk::SamplerMipmapMode::eNearest,
      vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, 0.0f, false, 1.0f, false, vk::CompareOp::eNever, 0.0f, 0.0f,
      vk::BorderColor::eFloatOpaqueWhite));

    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D, format, componentMapping, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
    vk::UniqueImageView imageView = device->createImageViewUnique(imageViewCreateInfo);

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
