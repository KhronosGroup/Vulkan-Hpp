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

#include "utils.hpp"
#include "vulkan/vulkan.hpp"

PFN_vkCreateDebugReportCallbackEXT  pfnVkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
  return pfnVkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
  pfnVkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

namespace vk
{
  namespace su
  {
    vk::UniqueDeviceMemory allocateMemory(vk::UniqueDevice &device, vk::PhysicalDeviceMemoryProperties const& memoryProperties, vk::MemoryRequirements const& memoryRequirements, vk::MemoryPropertyFlags memoryPropertyFlags)
    {
      uint32_t memoryTypeBits = memoryRequirements.memoryTypeBits;
      uint32_t memoryTypeIndex = static_cast<uint32_t>(~0);
      for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
      {
        if ((memoryTypeBits & 1) == 1)
        {
          if ((memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
          {
            memoryTypeIndex = i;
            break;
          }
        }
        memoryTypeBits >>= 1;
      }
      assert(memoryTypeIndex != ~0);

      return device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex));
    }

    vk::UniqueCommandPool createCommandPool(vk::UniqueDevice &device, uint32_t queueFamilyIndex)
    {
      vk::CommandPoolCreateInfo commandPoolCreateInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndex);
      return device->createCommandPoolUnique(commandPoolCreateInfo);
    }

    vk::UniqueDebugReportCallbackEXT createDebugReportCallback(vk::UniqueInstance &instance)
    {
      vk::DebugReportFlagsEXT flags(vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eError);
      return instance->createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(flags, &vk::su::debugReportCallback));
    }

    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::UniqueDevice &device)
    {
      vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex);
      return device->createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo({}, 1, &descriptorSetLayoutBinding));
    }

    vk::UniqueDevice createDevice(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<std::string> const& extensions)
    {
      std::vector<char const*> enabledExtensions;
      enabledExtensions.reserve(extensions.size());
      for (auto const& ext : extensions)
      {
        enabledExtensions.push_back(ext.data());
      }

      // create a UniqueDevice
      float queuePriority = 0.0f;
      vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority);
      vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo, 0, nullptr, checked_cast<uint32_t>(enabledExtensions.size()), enabledExtensions.data());
      return physicalDevice.createDeviceUnique(deviceCreateInfo);
    }

    std::vector<vk::UniqueFramebuffer> createFramebuffers(vk::UniqueDevice &device, vk::UniqueRenderPass &renderPass, std::vector<vk::UniqueImageView> const& imageViews, vk::UniqueImageView &depthImageView, int width, int height)
    {
      vk::ImageView attachments[2];
      attachments[1] = depthImageView.get();

      std::vector<vk::UniqueFramebuffer> framebuffers;
      framebuffers.reserve(imageViews.size());
      for (auto const& view : imageViews)
      {
        attachments[0] = view.get();
        framebuffers.push_back(device->createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(), renderPass.get(), 2, attachments, width, height, 1)));
      }

      return framebuffers;
    }

    vk::UniquePipeline createGraphicsPipeline(vk::UniqueDevice &device, vk::UniquePipelineCache &pipelineCache, vk::UniqueShaderModule &vertexShaderModule, vk::UniqueShaderModule &fragmentShaderModule, uint32_t vertexStride, vk::UniquePipelineLayout &pipelineLayout, vk::UniqueRenderPass &renderPass)
    {
      vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] =
      {
        vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"),
        vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main")
      };

      vk::VertexInputBindingDescription vertexInputBindingDescription(0, vertexStride);
      vk::VertexInputAttributeDescription vertexInputAttributeDescriptions[2] =
      {
        vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
        vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, 16)
      };
      vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(vk::PipelineVertexInputStateCreateFlags(), 1, &vertexInputBindingDescription, 2, vertexInputAttributeDescriptions);

      vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

      vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

      vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(), false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, false, 0.0f, 0.0f, 0.0f, 1.0f);

      vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

      vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
      vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(vk::PipelineDepthStencilStateCreateFlags(), true, true, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState);

      vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
      vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(false, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, colorComponentFlags);
      vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, 1, &pipelineColorBlendAttachmentState, { { (1.0f, 1.0f, 1.0f, 1.0f) } });

      vk::DynamicState dynamicStates[2] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
      vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), 2, dynamicStates);

      vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlags(), 2, pipelineShaderStageCreateInfos, &pipelineVertexInputStateCreateInfo, &pipelineInputAssemblyStateCreateInfo, nullptr, &pipelineViewportStateCreateInfo, &pipelineRasterizationStateCreateInfo, &pipelineMultisampleStateCreateInfo, &pipelineDepthStencilStateCreateInfo, &pipelineColorBlendStateCreateInfo, &pipelineDynamicStateCreateInfo, pipelineLayout.get(), renderPass.get());

      return device->createGraphicsPipelineUnique(pipelineCache.get(), graphicsPipelineCreateInfo);
    }

    vk::UniqueImage createImage(vk::UniqueDevice &device, vk::Format format, uint32_t width, uint32_t height, vk::ImageTiling tiling)
    {
      vk::Extent3D extend3D(width, height, 1);
      vk::ImageCreateInfo imageCreateInfo({}, vk::ImageType::e2D, format, extend3D, 1, 1, vk::SampleCountFlagBits::e1, tiling, vk::ImageUsageFlagBits::eDepthStencilAttachment);
      return device->createImageUnique(imageCreateInfo);
    }

    vk::UniqueImageView createImageView(vk::UniqueDevice &device, vk::UniqueImage &image, vk::Format format)
    {
      vk::ComponentMapping componentMapping(ComponentSwizzle::eR, ComponentSwizzle::eG, ComponentSwizzle::eB, ComponentSwizzle::eA);
      vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
      vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D, format, componentMapping, imageSubresourceRange);
      return device->createImageViewUnique(imageViewCreateInfo);
    }

    vk::UniqueInstance createInstance(std::string const& appName, std::string const& engineName, std::vector<std::string> const& extensions)
    {
      std::vector<char const*> enabledLayers;
#if !defined(NDEBUG)
      // Enable standard validation layer to find as much errors as possible!
      enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

      std::vector<char const*> enabledExtensions;
      enabledExtensions.reserve(extensions.size());
      for (auto const& ext : extensions)
      {
        enabledExtensions.push_back(ext.data());
      }
#if !defined(NDEBUG)
      if (std::find(extensions.begin(), extensions.end(), VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == extensions.end())
      {
        enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
      }
#endif

      // create a UniqueInstance
      vk::ApplicationInfo applicationInfo(appName.c_str(), 1, engineName.c_str(), 1, VK_API_VERSION_1_1);
      vk::UniqueInstance instance = vk::createInstanceUnique(vk::InstanceCreateInfo({}, &applicationInfo, checked_cast<uint32_t>(enabledLayers.size()), enabledLayers.data(),
        checked_cast<uint32_t>(enabledExtensions.size()), enabledExtensions.data()));

#if !defined(NDEBUG)
      static bool initialized = false;
      if (!initialized)
      {
        pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance->getProcAddr("vkCreateDebugReportCallbackEXT"));
        pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance->getProcAddr("vkDestroyDebugReportCallbackEXT"));
        assert(pfnVkCreateDebugReportCallbackEXT && pfnVkDestroyDebugReportCallbackEXT);
        initialized = true;
      }
#endif

      return instance;
    }

    vk::UniqueRenderPass createRenderPass(vk::UniqueDevice &device, vk::Format colorFormat, vk::Format depthFormat)
    {
      vk::AttachmentDescription attachmentDescriptions[2] =
      {
        vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), colorFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR),
        vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), depthFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal)
      };
      vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);
      vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
      vk::SubpassDescription subpassDescription(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachment, nullptr, &depthAttachment);
      return device->createRenderPassUnique(vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), 2, attachmentDescriptions, 1, &subpassDescription));
    }

    vk::UniqueSwapchainKHR createSwapChain(vk::PhysicalDevice physicalDevice, vk::UniqueSurfaceKHR &surface, vk::UniqueDevice &device, uint32_t width, uint32_t height, vk::Format format, uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex)
    {
      vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface.get());
      VkExtent2D swapchainExtent;
      if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
      {
        // If the surface size is undefined, the size is set to the size of the images requested.
        swapchainExtent.width = clamp(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        swapchainExtent.height = clamp(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
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

      uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
      if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
      {
        // If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode as vk::SharingMode::eConcurrent
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
      }
      return device->createSwapchainKHRUnique(swapChainCreateInfo);
    }

    std::vector<vk::UniqueImageView> createSwapChainImageViews(vk::UniqueDevice &device, vk::UniqueSwapchainKHR &swapChain, vk::Format format)
    {
      std::vector<vk::Image> images = device->getSwapchainImagesKHR(swapChain.get());
      std::vector<vk::UniqueImageView> imageViews;
      imageViews.reserve(images.size());

      vk::ComponentMapping componentMapping(ComponentSwizzle::eR, ComponentSwizzle::eG, ComponentSwizzle::eB, ComponentSwizzle::eA);
      vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

      for (auto const& image : images)
      {
        imageViews.push_back(device->createImageViewUnique(vk::ImageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format, componentMapping, imageSubresourceRange)));
      }
      return imageViews;
    }

    VkBool32 debugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t /*messageCode*/, const char* /*pLayerPrefix*/, const char* pMessage, void* /*pUserData*/)
    {
      switch (flags)
      {
        case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
          std::cerr << "INFORMATION: ";
          break;
        case VK_DEBUG_REPORT_WARNING_BIT_EXT:
          std::cerr << "WARNING: ";
          break;
        case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
          std::cerr << "PERFORMANCE WARNING: ";
          break;
        case VK_DEBUG_REPORT_ERROR_BIT_EXT:
          std::cerr << "ERROR: ";
          break;
        case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
          std::cerr << "DEBUG: ";
          break;
        default:
          std::cerr << "unknown flag (" << flags << "): ";
          break;
      }
      std::cerr << pMessage << std::endl;
      return VK_TRUE;
    }

    uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
    {
      // get the first index into queueFamiliyProperties which supports graphics
      size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
        [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
      assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());

      return checked_cast<uint32_t>(graphicsQueueFamilyIndex);
    }

    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::UniqueSurfaceKHR & surface)
    {
      std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
      assert(queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max());

      uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(queueFamilyProperties);
      if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, surface.get()))
      {
        return std::make_pair(graphicsQueueFamilyIndex, graphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
      }

      // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
      for (size_t i = 0; i < queueFamilyProperties.size(); i++)
      {
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.get()))
        {
          return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        }
      }

      // there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
      for (size_t i = 0; i < queueFamilyProperties.size(); i++)
      {
        if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.get()))
        {
          return std::make_pair(graphicsQueueFamilyIndex, static_cast<uint32_t>(i));
        }
      }

      throw std::runtime_error("Could not find queues for both graphics or present -> terminating");
    }

    uint32_t findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits, vk::MemoryPropertyFlags requirementsMask)
    {
      uint32_t typeIndex = uint32_t(~0);
      for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
      {
        if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask))
        {
          typeIndex = i;
          break;
        }
        typeBits >>= 1;
      }
      assert(typeIndex != ~0);
      return typeIndex;
    }

    std::vector<std::string> getDeviceExtensions()
    {
      return{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    }

    vk::ImageTiling getImageTiling(vk::FormatProperties const& formatProperties)
    {
      if (formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
      {
        return vk::ImageTiling::eLinear;
      }
      else if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
      {
        return vk::ImageTiling::eOptimal;
      }
      else
      {
        throw std::runtime_error("DepthStencilAttachment is not supported for D16Unorm depth format.");
      }
    }

    std::vector<std::string> getInstanceExtensions()
    {
      std::vector<std::string> extensions;
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

    vk::Format pickColorFormat(std::vector<vk::SurfaceFormatKHR> const& formats)
    {
      assert(!formats.empty());
      return (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;
    }

    void submitAndWait(vk::UniqueDevice &device, vk::Queue queue, vk::UniqueCommandBuffer &commandBuffer)
    {
      vk::UniqueFence fence = device->createFenceUnique(vk::FenceCreateInfo());
      vk::PipelineStageFlags pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
      queue.submit(vk::SubmitInfo(0, nullptr, &pipelineStageFlags, 1, &commandBuffer.get()), fence.get());
      while (vk::Result::eTimeout == device->waitForFences(fence.get(), VK_TRUE, vk::su::FenceTimeout))
        ;
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
  }
}
