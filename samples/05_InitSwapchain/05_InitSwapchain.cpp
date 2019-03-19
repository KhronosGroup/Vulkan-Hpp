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
// VulkanHpp Samples : 05_InitSwapchain
//                     Initialize a swapchain

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "05_InitSwapchain";
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

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();
    uint32_t graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex(queueFamilyProperties);

    /* VULKAN_HPP_KEY_START */

    uint32_t width = 64;
    uint32_t height = 64;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND window = vk::su::initializeWindow(AppName, AppName, width, height);

    vk::UniqueSurfaceKHR surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR(vk::Win32SurfaceCreateFlagsKHR(), GetModuleHandle(nullptr), window));
#else
#pragma error "unhandled platform"
#endif

    // determine a queueFamilyIndex that suports present
    // first check if the graphicsQueueFamiliyIndex is good enough
    size_t presentQueueFamilyIndex = physicalDevices[0].getSurfaceSupportKHR(static_cast<uint32_t>(graphicsQueueFamilyIndex), surface.get()) ? graphicsQueueFamilyIndex : queueFamilyProperties.size();
    if (presentQueueFamilyIndex == queueFamilyProperties.size())
    {
      // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
      for (size_t i = 0; i < queueFamilyProperties.size(); i++)
      {
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevices[0].getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.get()))
        {
          graphicsQueueFamilyIndex = vk::su::checked_cast<uint32_t>(i);
          presentQueueFamilyIndex = i;
          break;
        }
      }
      if (presentQueueFamilyIndex == queueFamilyProperties.size())
      {
        // there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
        for (size_t i = 0; i < queueFamilyProperties.size(); i++)
        {
          if (physicalDevices[0].getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.get()))
          {
            presentQueueFamilyIndex = i;
            break;
          }
        }
      }
    }
    if ((graphicsQueueFamilyIndex == queueFamilyProperties.size()) || (presentQueueFamilyIndex == queueFamilyProperties.size()))
    {
      throw std::runtime_error("Could not find a queue for graphics or present -> terminating");
    }

    // create a device
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsQueueFamilyIndex, vk::su::getDeviceExtensions());

    // get the supported VkFormats
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevices[0].getSurfaceFormatsKHR(surface.get());
    assert(!formats.empty());
    vk::Format format = (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevices[0].getSurfaceCapabilitiesKHR(surface.get());
    VkExtent2D swapchainExtent;
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
      // If the surface size is undefined, the size is set to the size of the images requested.
      swapchainExtent.width = vk::su::clamp(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
      swapchainExtent.height = vk::su::clamp(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }
    else
    {
      // If the surface size is defined, the swap chain size must match
      swapchainExtent = surfaceCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

    vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(), surface.get(), surfaceCapabilities.minImageCount, format, vk::ColorSpaceKHR::eSrgbNonlinear,
      swapchainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, 0, nullptr, preTransform, compositeAlpha, swapchainPresentMode, true, nullptr);

    uint32_t queueFamilyIndices[2] = { static_cast<uint32_t>(graphicsQueueFamilyIndex), static_cast<uint32_t>(presentQueueFamilyIndex) };
    if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
    {
      // If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
      // the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
      swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
      swapChainCreateInfo.queueFamilyIndexCount = 2;
      swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    vk::UniqueSwapchainKHR swapChain = device->createSwapchainKHRUnique(swapChainCreateInfo);

    std::vector<vk::Image> swapChainImages = device->getSwapchainImagesKHR(swapChain.get());

    std::vector<vk::UniqueImageView> imageViews;
    imageViews.reserve(swapChainImages.size());
    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    for (auto image : swapChainImages)
    {
      vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format, componentMapping, subResourceRange);
      imageViews.push_back(device->createImageViewUnique(imageViewCreateInfo));
    }

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    DestroyWindow(window);
#else
#pragma error "unhandled platform"
#endif

    // Note: No need to explicitly destroy the ImageViews or the swapChain, as the corresponding destroy
    // functions are called by the destructor of the UniqueImageView and the UniqueSwapChainKHR on leaving this scope.

    /* VULKAN_HPP_KEY_END */
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
