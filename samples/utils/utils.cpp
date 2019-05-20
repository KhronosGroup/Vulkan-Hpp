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
#include <iomanip>
#include <numeric>

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
    vk::UniqueDeviceMemory allocateMemory(vk::UniqueDevice const& device, vk::PhysicalDeviceMemoryProperties const& memoryProperties, vk::MemoryRequirements const& memoryRequirements,
                                          vk::MemoryPropertyFlags memoryPropertyFlags)
    {
      uint32_t memoryTypeIndex = findMemoryType(memoryProperties, memoryRequirements.memoryTypeBits, memoryPropertyFlags);

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

    vk::UniqueDescriptorPool createDescriptorPool(vk::UniqueDevice &device, std::vector<vk::DescriptorPoolSize> const& poolSizes)
    {
      assert(!poolSizes.empty());
      uint32_t maxSets = std::accumulate(poolSizes.begin(), poolSizes.end(), 0, [](uint32_t sum, vk::DescriptorPoolSize const& dps) { return sum + dps.descriptorCount; });
      assert(0 < maxSets);

      vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, checked_cast<uint32_t>(poolSizes.size()), poolSizes.data());
      return device->createDescriptorPoolUnique(descriptorPoolCreateInfo);
    }

    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::UniqueDevice &device, std::vector<std::pair<vk::DescriptorType, vk::ShaderStageFlags>> const& bindingData, vk::DescriptorSetLayoutCreateFlags flags)
    {
      std::vector<vk::DescriptorSetLayoutBinding> bindings(bindingData.size());
      for (size_t i = 0; i < bindingData.size(); i++)
      {
        bindings[i] = vk::DescriptorSetLayoutBinding(checked_cast<uint32_t>(i), bindingData[i].first, 1, bindingData[i].second);
      }
      return device->createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo(flags, checked_cast<uint32_t>(bindings.size()), bindings.data()));
    }

    vk::UniqueDevice createDevice(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<std::string> const& extensions, vk::PhysicalDeviceFeatures const* physicalDeviceFeatures,
                                  void const* pNext)
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
      vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo, 0, nullptr, checked_cast<uint32_t>(enabledExtensions.size()), enabledExtensions.data(), physicalDeviceFeatures);
      deviceCreateInfo.pNext = pNext;
      return physicalDevice.createDeviceUnique(deviceCreateInfo);
    }

    std::vector<vk::UniqueFramebuffer> createFramebuffers(vk::UniqueDevice &device, vk::UniqueRenderPass &renderPass, std::vector<vk::UniqueImageView> const& imageViews, vk::UniqueImageView const& depthImageView, vk::Extent2D const& extent)
    {
      vk::ImageView attachments[2];
      attachments[1] = depthImageView.get();

      std::vector<vk::UniqueFramebuffer> framebuffers;
      framebuffers.reserve(imageViews.size());
      for (auto const& view : imageViews)
      {
        attachments[0] = view.get();
        framebuffers.push_back(device->createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(), renderPass.get(), depthImageView ? 2 : 1, attachments, extent.width, extent.height, 1)));
      }

      return framebuffers;
    }

    vk::UniquePipeline createGraphicsPipeline(vk::UniqueDevice &device, vk::UniquePipelineCache &pipelineCache, vk::UniqueShaderModule &vertexShaderModule,
      vk::UniqueShaderModule &fragmentShaderModule, uint32_t vertexStride, bool depthBuffered, bool textured, vk::UniquePipelineLayout &pipelineLayout, vk::UniqueRenderPass &renderPass)
    {
      vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] =
      {
        vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"),
        vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main")
      };

      vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
      if (0 < vertexStride)
      {
        vk::VertexInputBindingDescription vertexInputBindingDescription(0, vertexStride);
        vk::VertexInputAttributeDescription vertexInputAttributeDescriptions[2] =
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
          vk::VertexInputAttributeDescription(1, 0, textured ? vk::Format::eR32G32Sfloat : vk::Format::eR32G32B32A32Sfloat, 16)
        };
        pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
        pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
        pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 2;
        pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;
      }

      vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

      vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

      vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(), false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, false, 0.0f, 0.0f, 0.0f, 1.0f);

      vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

      vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
      vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(vk::PipelineDepthStencilStateCreateFlags(), depthBuffered, depthBuffered, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState);

      vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
      vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(false, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, colorComponentFlags);
      vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, 1, &pipelineColorBlendAttachmentState, { { (1.0f, 1.0f, 1.0f, 1.0f) } });

      vk::DynamicState dynamicStates[2] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
      vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), 2, dynamicStates);

      vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlags(), 2, pipelineShaderStageCreateInfos, &pipelineVertexInputStateCreateInfo, &pipelineInputAssemblyStateCreateInfo, nullptr, &pipelineViewportStateCreateInfo, &pipelineRasterizationStateCreateInfo, &pipelineMultisampleStateCreateInfo, &pipelineDepthStencilStateCreateInfo, &pipelineColorBlendStateCreateInfo, &pipelineDynamicStateCreateInfo, pipelineLayout.get(), renderPass.get());

      return device->createGraphicsPipelineUnique(pipelineCache.get(), graphicsPipelineCreateInfo);
    }

    vk::UniqueInstance createInstance(std::string const& appName, std::string const& engineName, std::vector<std::string> const& extensions, uint32_t apiVersion)
    {
      std::vector<char const*> enabledLayers;
#if !defined(NDEBUG)
      // Enable standard validation layer to find as much errors as possible!
      enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
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
      vk::ApplicationInfo applicationInfo(appName.c_str(), 1, engineName.c_str(), 1, apiVersion);
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

    vk::UniqueRenderPass createRenderPass(vk::UniqueDevice &device, vk::Format colorFormat, vk::Format depthFormat, vk::AttachmentLoadOp loadOp, vk::ImageLayout colorFinalLayout)
    {
      std::vector<vk::AttachmentDescription> attachmentDescriptions;
      assert(colorFormat != vk::Format::eUndefined);
      attachmentDescriptions.push_back(vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), colorFormat, vk::SampleCountFlagBits::e1, loadOp, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, colorFinalLayout));
      if (depthFormat != vk::Format::eUndefined)
      {
        attachmentDescriptions.push_back(vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), depthFormat, vk::SampleCountFlagBits::e1, loadOp, vk::AttachmentStoreOp::eStore,
          vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal));
      }
      vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);
      vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
      vk::SubpassDescription subpassDescription(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachment, nullptr, (depthFormat != vk::Format::eUndefined) ? &depthAttachment : nullptr);
      return device->createRenderPassUnique(vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), static_cast<uint32_t>(attachmentDescriptions.size()), attachmentDescriptions.data(), 1, &subpassDescription));
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

    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& surface)
    {
      std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
      assert(queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max());

      uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(queueFamilyProperties);
      if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, surface))
      {
        return std::make_pair(graphicsQueueFamilyIndex, graphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
      }

      // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
      for (size_t i = 0; i < queueFamilyProperties.size(); i++)
      {
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
        {
          return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        }
      }

      // there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
      for (size_t i = 0; i < queueFamilyProperties.size(); i++)
      {
        if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
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

    vk::PresentModeKHR pickPresentMode(std::vector<vk::PresentModeKHR> const& presentModes)
    {
      vk::PresentModeKHR pickedMode = vk::PresentModeKHR::eFifo;;
      for(const auto& presentMode : presentModes)
      {
        if(presentMode == vk::PresentModeKHR::eMailbox)
        {
          pickedMode = presentMode;
          break;
        }

        if(presentMode == vk::PresentModeKHR::eImmediate)
        {
          pickedMode = presentMode;
        }
      }
      return pickedMode;
    }

    vk::SurfaceFormatKHR pickSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& formats)
    {
      assert(!formats.empty());
      vk::SurfaceFormatKHR pickedFormat = formats[0];
      if (formats.size() == 1)
      {
        if (formats[0].format == vk::Format::eUndefined)
        {
          pickedFormat.format     = vk::Format::eB8G8R8A8Unorm;
          pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        }
      }
      else
      {
        // request several formats, the first found will be used
        vk::Format        requestedFormats[]  = { vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm };
        vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        for (size_t i = 0; i < sizeof(requestedFormats) / sizeof(requestedFormats[0]); i++)
        {
          vk::Format requestedFormat = requestedFormats[i];
          auto it = std::find_if(formats.begin(), formats.end(), [requestedFormat, requestedColorSpace](auto const& f) { return (f.format == requestedFormat) && (f.colorSpace == requestedColorSpace); });
          if (it != formats.end())
          {
            pickedFormat = *it;
            break;
          }
        }
      }
      assert(pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);
      return pickedFormat;
    }

    void setImageLayout(vk::UniqueCommandBuffer &commandBuffer, vk::Image image, vk::Format format, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout)
    {
      vk::AccessFlags sourceAccessMask;
      switch (oldImageLayout)
      {
        case vk::ImageLayout::eTransferDstOptimal:
          sourceAccessMask = vk::AccessFlagBits::eTransferWrite;
          break;
        case vk::ImageLayout::ePreinitialized:
          sourceAccessMask = vk::AccessFlagBits::eHostWrite;
          break;
        case vk::ImageLayout::eGeneral:     // sourceAccessMask is empty
        case vk::ImageLayout::eUndefined:
          break;
        default:
          assert(false);
          break;
      }

      vk::PipelineStageFlags sourceStage;
      switch (oldImageLayout)
      {
        case vk::ImageLayout::eGeneral:
        case vk::ImageLayout::ePreinitialized:
          sourceStage = vk::PipelineStageFlagBits::eHost;
          break;
        case vk::ImageLayout::eTransferDstOptimal:
          sourceStage = vk::PipelineStageFlagBits::eTransfer;
          break;
        case vk::ImageLayout::eUndefined:
          sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
          break;
        default:
          assert(false);
          break;
      }

      vk::AccessFlags destinationAccessMask;
      switch (newImageLayout)
      {
        case vk::ImageLayout::eColorAttachmentOptimal:
          destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
          break;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
          destinationAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
          break;
        case vk::ImageLayout::eGeneral:   // empty destinationAccessMask
          break;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
          destinationAccessMask = vk::AccessFlagBits::eShaderRead;
          break;
        case vk::ImageLayout::eTransferSrcOptimal:
          destinationAccessMask = vk::AccessFlagBits::eTransferRead;
          break;
        case vk::ImageLayout::eTransferDstOptimal:
          destinationAccessMask = vk::AccessFlagBits::eTransferWrite;
          break;
        default:
          assert(false);
          break;
      }

      vk::PipelineStageFlags destinationStage;
      switch (newImageLayout)
      {
        case vk::ImageLayout::eColorAttachmentOptimal:
          destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
          break;
        case vk::ImageLayout::eGeneral:
          destinationStage = vk::PipelineStageFlagBits::eHost;
          break;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
          destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
          break;
        case vk::ImageLayout::eTransferDstOptimal:
        case vk::ImageLayout::eTransferSrcOptimal:
          destinationStage = vk::PipelineStageFlagBits::eTransfer;
          break;
        default:
          assert(false);
          break;
      }

      vk::ImageAspectFlags aspectMask;
      if (newImageLayout == vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal)
      {
        aspectMask = vk::ImageAspectFlagBits::eDepth;
        if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
        {
          aspectMask |= vk::ImageAspectFlagBits::eStencil;
        }
      }
      else
      {
        aspectMask = vk::ImageAspectFlagBits::eColor;
      }

      vk::ImageSubresourceRange imageSubresourceRange(aspectMask, 0, 1, 0, 1);
      vk::ImageMemoryBarrier imageMemoryBarrier(sourceAccessMask, destinationAccessMask, oldImageLayout, newImageLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image, imageSubresourceRange);
      return commandBuffer->pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier);
    }

    void submitAndWait(vk::UniqueDevice &device, vk::Queue queue, vk::UniqueCommandBuffer &commandBuffer)
    {
      vk::UniqueFence fence = device->createFenceUnique(vk::FenceCreateInfo());
      vk::PipelineStageFlags pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
      queue.submit(vk::SubmitInfo(0, nullptr, &pipelineStageFlags, 1, &commandBuffer.get()), fence.get());
      while (vk::Result::eTimeout == device->waitForFences(fence.get(), VK_TRUE, vk::su::FenceTimeout))
        ;
    }

    void updateDescriptorSets(vk::UniqueDevice &device, vk::UniqueDescriptorSet &descriptorSet, vk::DescriptorType descriptorType, vk::DescriptorBufferInfo const* descriptorBufferInfo, vk::DescriptorImageInfo const* descriptorImageInfo)
    {
      std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
      writeDescriptorSets.push_back(vk::WriteDescriptorSet(descriptorSet.get(), 0, 0, 1, descriptorType, nullptr, descriptorBufferInfo, nullptr));
      if (descriptorImageInfo)
      {
        writeDescriptorSets.push_back(vk::WriteDescriptorSet(descriptorSet.get(), 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, descriptorImageInfo, nullptr, nullptr));
      }
      device->updateDescriptorSets(writeDescriptorSets, nullptr);
    }

    BufferData::BufferData(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags propertyFlags)
    {
      buffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), size, usage));
      deviceMemory = vk::su::allocateMemory(device, physicalDevice.getMemoryProperties(), device->getBufferMemoryRequirements(buffer.get()), propertyFlags);
      device->bindBufferMemory(buffer.get(), deviceMemory.get(), 0);
    }

    DepthBufferData::DepthBufferData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice & device, vk::Format format, vk::Extent2D const& extent)
      : ImageData(physicalDevice, device, format, extent, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::ImageLayout::eUndefined, vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageAspectFlagBits::eDepth)
    {}

    ImageData::ImageData(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::Format format_, vk::Extent2D const& extent, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::ImageLayout initialLayout, vk::MemoryPropertyFlags memoryProperties, vk::ImageAspectFlags aspectMask)
      : format(format_)
    {
      vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, format, vk::Extent3D(extent, 1), 1, 1, vk::SampleCountFlagBits::e1, tiling, usage | vk::ImageUsageFlagBits::eSampled, vk::SharingMode::eExclusive, 0, nullptr, initialLayout);
      image = device->createImageUnique(imageCreateInfo);

      deviceMemory = vk::su::allocateMemory(device, physicalDevice.getMemoryProperties(), device->getImageMemoryRequirements(image.get()), memoryProperties);

      device->bindImageMemory(image.get(), deviceMemory.get(), 0);

      vk::ComponentMapping componentMapping(ComponentSwizzle::eR, ComponentSwizzle::eG, ComponentSwizzle::eB, ComponentSwizzle::eA);
      vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D, format, componentMapping, vk::ImageSubresourceRange(aspectMask, 0, 1, 0, 1));
      imageView = device->createImageViewUnique(imageViewCreateInfo);
    }

    SurfaceData::SurfaceData(vk::UniqueInstance &instance, std::string const& className, std::string const& windowName, vk::Extent2D const& extent_)
      : extent(extent_)
    {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
      window = vk::su::initializeWindow(className.c_str(), windowName.c_str(), extent.width, extent.height);
      surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR(vk::Win32SurfaceCreateFlagsKHR(), GetModuleHandle(nullptr), window));
#else
#pragma error "unhandled platform"
#endif
    }

    SwapChainData::SwapChainData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device, vk::UniqueSurfaceKHR &surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage, uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex)
    {
      colorFormat = vk::su::pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface.get())).format;

      vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface.get());
      VkExtent2D swapchainExtent;
      if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
      {
        // If the surface size is undefined, the size is set to the size of the images requested.
        swapchainExtent.width = clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        swapchainExtent.height = clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
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
      vk::PresentModeKHR presentMode = vk::su::pickPresentMode(physicalDevice.getSurfacePresentModesKHR(*surface));
      vk::SwapchainCreateInfoKHR swapChainCreateInfo({}, surface.get(), surfaceCapabilities.minImageCount, colorFormat, vk::ColorSpaceKHR::eSrgbNonlinear, swapchainExtent, 1, usage,
        vk::SharingMode::eExclusive, 0, nullptr, preTransform, compositeAlpha, presentMode, true, nullptr);
      uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
      if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
      {
        // If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode as vk::SharingMode::eConcurrent
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
      }
      swapChain = device->createSwapchainKHRUnique(swapChainCreateInfo);

      images = device->getSwapchainImagesKHR(swapChain.get());

      imageViews.reserve(images.size());
      vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
      vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
      for (auto image : images)
      {
        vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, colorFormat, componentMapping, subResourceRange);
        imageViews.push_back(device->createImageViewUnique(imageViewCreateInfo));
      }
    }

    void CheckerboardTextureCreator::operator()(void* data, vk::Extent2D &extent) const
    {
      // Checkerboard of 16x16 pixel squares
      unsigned char *pImageMemory = static_cast<unsigned char*>(data);
      for (uint32_t row = 0; row < extent.height; row++)
      {
        for (uint32_t col = 0; col < extent.width; col++)
        {
          unsigned char rgb = (((row & 0x10) == 0) ^ ((col & 0x10) == 0)) * 255;
          pImageMemory[0] = rgb;
          pImageMemory[1] = rgb;
          pImageMemory[2] = rgb;
          pImageMemory[3] = 255;
          pImageMemory += 4;
        }
      }
    }

    MonochromeTextureGenerator::MonochromeTextureGenerator(std::array<unsigned char, 3> const& rgb_)
      : rgb(rgb_)
    {}

    void MonochromeTextureGenerator::operator()(void* data, vk::Extent2D &extent) const
    {
      // fill in with the monochrome color
      unsigned char *pImageMemory = static_cast<unsigned char*>(data);
      for (uint32_t row = 0; row < extent.height; row++)
      {
        for (uint32_t col = 0; col < extent.width; col++)
        {
          pImageMemory[0] = rgb[0];
          pImageMemory[1] = rgb[1];
          pImageMemory[2] = rgb[2];
          pImageMemory[3] = 255;
          pImageMemory += 4;
        }
      }
    }

    TextureData::TextureData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device, vk::Extent2D const& extent_, vk::ImageUsageFlags usageFlags, vk::FormatFeatureFlags formatFeatureFlags)
      : format(vk::Format::eR8G8B8A8Unorm)
      , extent(extent_)
    {
      vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
      vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(format);

      formatFeatureFlags |= vk::FormatFeatureFlagBits::eSampledImage;
      needsStaging = (formatProperties.linearTilingFeatures & formatFeatureFlags) != formatFeatureFlags;
      vk::ImageTiling imageTiling;
      vk::ImageLayout initialLayout;
      vk::MemoryPropertyFlags requirements;
      if (needsStaging)
      {
        assert((formatProperties.optimalTilingFeatures & formatFeatureFlags) == formatFeatureFlags);
        stagingBufferData = std::make_unique<BufferData>(physicalDevice, device, extent.width * extent.height * 4, vk::BufferUsageFlagBits::eTransferSrc);
        imageTiling = vk::ImageTiling::eOptimal;
        usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
        initialLayout = vk::ImageLayout::eUndefined;
      }
      else
      {
        imageTiling = vk::ImageTiling::eLinear;
        initialLayout = vk::ImageLayout::ePreinitialized;
        requirements = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
      }
      imageData = std::make_unique<ImageData>(physicalDevice, device, format, extent, imageTiling, usageFlags | vk::ImageUsageFlagBits::eSampled, initialLayout, requirements, vk::ImageAspectFlagBits::eColor);

      textureSampler = device->createSamplerUnique(vk::SamplerCreateInfo(vk::SamplerCreateFlags(), vk::Filter::eNearest, vk::Filter::eNearest, vk::SamplerMipmapMode::eNearest,
        vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, 0.0f, false, 1.0f, false, vk::CompareOp::eNever, 0.0f, 0.0f
        , vk::BorderColor::eFloatOpaqueWhite));
    }

    UUID::UUID(uint8_t data[VK_UUID_SIZE])
    {
      memcpy(m_data, data, VK_UUID_SIZE * sizeof(uint8_t));
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

std::ostream& operator<<(std::ostream& os, vk::su::UUID const& uuid)
{
  os << std::setfill('0');
  for (int j = 0; j < VK_UUID_SIZE; ++j)
  {
    os << std::hex << std::setw(2) << static_cast<uint32_t>(uuid.m_data[j]);
    if (j == 3 || j == 5 || j == 7 || j == 9)
    {
      std::cout << '-';
    }
  }
  os << std::setfill(' ');
  return os;
}
