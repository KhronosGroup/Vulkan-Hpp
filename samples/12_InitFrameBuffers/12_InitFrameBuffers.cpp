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
// VulkanHpp Samples : 12_InitFrameBuffers
//                     Initialize framebuffers

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "12_InitFrameBuffers";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char * /*argv[]*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    uint32_t width = 64;
    uint32_t height = 64;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND window = vk::su::initializeWindow(AppName, AppName, width, height);
    vk::UniqueSurfaceKHR surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR({}, GetModuleHandle(nullptr), window));
#else
#pragma error "unhandled platform"
#endif

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::Format colorFormat = vk::su::pickColorFormat(physicalDevices[0].getSurfaceFormatsKHR(surface.get()));
    vk::UniqueSwapchainKHR swapChain = vk::su::createSwapChain(physicalDevices[0], surface, device, width, height, colorFormat, graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);
    std::vector<vk::UniqueImageView> swapChainImageViews = vk::su::createSwapChainImageViews(device, swapChain, colorFormat);

    vk::Format depthFormat = vk::Format::eD16Unorm;
    vk::UniqueImage depthImage = vk::su::createImage(device, depthFormat, width, height);
    vk::UniqueDeviceMemory depthMemory = vk::su::allocateMemory(device, physicalDevices[0].getMemoryProperties(), device->getImageMemoryRequirements(depthImage.get()), vk::MemoryPropertyFlagBits::eDeviceLocal);
    device->bindImageMemory(depthImage.get(), depthMemory.get(), 0);
    vk::UniqueImageView depthViewImage = vk::su::createImageView(device, depthImage, depthFormat);

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, colorFormat, depthFormat);

    /* VULKAN_KEY_START */

    vk::ImageView attachments[2];
    attachments[1] = depthViewImage.get();

    std::vector<vk::UniqueFramebuffer> framebuffers;
    framebuffers.reserve(swapChainImageViews.size());
    for (auto const& view : swapChainImageViews)
    {
      attachments[0] = view.get();
      framebuffers.push_back(device->createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(), renderPass.get(), 2, attachments, width, height, 1)));
    }

    // Note: No need to explicitly destroy the Framebuffers, as the destroy functions are called by the destructor of the UniqueFramebuffer on leaving this scope.

    /* VULKAN_KEY_END */

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    DestroyWindow(window);
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
