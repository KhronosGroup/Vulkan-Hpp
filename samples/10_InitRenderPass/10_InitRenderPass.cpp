// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 10_InitRenderPass
//                     Initialize a render pass

#include <iostream>
#include "vulkan/vulkan.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

static char const* AppName = "10_InitRenderPass";
static char const* EngineName = "Vulkan.hpp";

static std::vector<char const*> getInstanceExtensions()
{
  std::vector<char const*> extensions;

  extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if  defined(VK_USE_PLATFORM_ANDROID_KHR)
  extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
  extensions.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_VI_NN)
  extensions.push_back(VK_NN_VI_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
  extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
  extensions.push_back(VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME);
#endif

  return extensions;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CLOSE:
    PostQuitMessage(0);
    break;
  default:
    break;
  }
  return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

HWND initializeWindow(std::string const& className, std::string const& windowName, LONG width, LONG height)
{
  WNDCLASSEX windowClass;
  memset(&windowClass, 0, sizeof(WNDCLASSEX));

  HINSTANCE instance = GetModuleHandle(nullptr);
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProc;
  windowClass.hInstance = instance;
  windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  windowClass.lpszClassName = className.c_str();
  windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

  if (!RegisterClassEx(&windowClass))
  {
    throw std::runtime_error("Failed to register WNDCLASSEX -> terminating");
  }

  RECT windowRect = { 0, 0, width, height };
  AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

  HWND window = CreateWindowEx(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU, 100, 100, windowRect.right - windowRect.left,
    windowRect.bottom - windowRect.top, nullptr, nullptr, instance, nullptr);
  if (!window)
  {
    throw std::runtime_error("Failed to create window -> terminating");
  }

  return window;
}
#else
#pragma error "unhandled platform"
#endif

int main(int argc, char *argv[])
{
  try
  {
    vk::ApplicationInfo appInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_1);
    std::vector<char const*> instanceExtensions = getInstanceExtensions();
    vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo, 0, nullptr, static_cast<uint32_t>(instanceExtensions.size()), instanceExtensions.data());
    vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    uint32_t width = 50;
    uint32_t height = 50;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND window = initializeWindow(AppName, "Sample", width, height);
    vk::UniqueSurfaceKHR surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR({}, GetModuleHandle(nullptr), window));
#else
#pragma error "unhandled platform"
#endif

    // determine a queueFamilyIndex that supports graphics
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();
    size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(),
      std::find_if(queueFamilyProperties.begin(),
        queueFamilyProperties.end(),
        [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));

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
          graphicsQueueFamilyIndex = i;
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
    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo({}, static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority);
    vk::UniqueDevice device = physicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo({}, 1, &deviceQueueCreateInfo, 0, nullptr));

    // get the supported VkFormats
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevices[0].getSurfaceCapabilitiesKHR(surface.get());
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevices[0].getSurfaceFormatsKHR(surface.get());
    assert(!formats.empty());
    vk::Format format = (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

    VkExtent2D swapchainExtent;
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
      // If the surface size is undefined, the size is set to the size of the images requested.
      swapchainExtent.width = glm::clamp(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
      swapchainExtent.height = glm::clamp(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }
    else
    {
      // If the surface size is defined, the swap chain size must match
      swapchainExtent = surfaceCapabilities.currentExtent;
    }

    vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
      (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    vk::SwapchainCreateInfoKHR swapChainCreateInfo({}, surface.get(), surfaceCapabilities.minImageCount, format, vk::ColorSpaceKHR::eSrgbNonlinear, swapchainExtent, 1,
      vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, 0, nullptr, preTransform, compositeAlpha, vk::PresentModeKHR::eFifo, true,
      nullptr);

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

    const vk::Format depthFormat = vk::Format::eD16Unorm;

    /* VULKAN_HPP_KEY_START */

    vk::AttachmentDescription attachments[2];
    attachments[0] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), format, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
    attachments[1] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), depthFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare,
      vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
    vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorReference, nullptr, &depthReference);

    vk::UniqueRenderPass renderPass = device->createRenderPassUnique(vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), 2, attachments, 1, &subpass));

    // Note: No need to explicitly destroy the RenderPass or the Semaphore, as the corresponding destroy
    // functions are called by the destructor of the UniqueRenderPass and the UniqueSemaphore on leaving this scope.

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
