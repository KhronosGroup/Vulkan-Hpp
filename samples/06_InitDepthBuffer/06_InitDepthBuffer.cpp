// Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 06_InitDepthBuffer
//                     Initialize a depth buffer

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "06_InitDepthBuffer";
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

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    /* VULKAN_HPP_KEY_START */

    const vk::Format depthFormat = vk::Format::eD16Unorm;
    vk::FormatProperties formatProperties = physicalDevices[0].getFormatProperties(depthFormat);

    vk::ImageTiling tiling;
    if (formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
      tiling = vk::ImageTiling::eLinear;
    }
    else if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
      tiling = vk::ImageTiling::eOptimal;
    }
    else
    {
      throw std::runtime_error("DepthStencilAttachment is not supported for D16Unorm depth format.");
    }
    vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, depthFormat, vk::Extent3D(surfaceData.extent, 1), 1, 1, vk::SampleCountFlagBits::e1, tiling, vk::ImageUsageFlagBits::eDepthStencilAttachment);
    vk::UniqueImage depthImage = device->createImageUnique(imageCreateInfo);

    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevices[0].getMemoryProperties();
    vk::MemoryRequirements memoryRequirements = device->getImageMemoryRequirements(depthImage.get());
    uint32_t typeBits = memoryRequirements.memoryTypeBits;
    uint32_t typeIndex = uint32_t(~0);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
      if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) == vk::MemoryPropertyFlagBits::eDeviceLocal))
      {
        typeIndex = i;
        break;
      }
      typeBits >>= 1;
    }
    assert(typeIndex != ~0);
    vk::UniqueDeviceMemory depthMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, typeIndex));

    device->bindImageMemory(depthImage.get(), depthMemory.get(), 0);

    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
    vk::UniqueImageView depthView = device->createImageViewUnique(vk::ImageViewCreateInfo(vk::ImageViewCreateFlags(), depthImage.get(), vk::ImageViewType::e2D, depthFormat, componentMapping, subResourceRange));

    /* VULKAN_HPP_KEY_END */

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
