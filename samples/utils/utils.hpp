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

#include "vulkan/vulkan.hpp"
#include <iostream>

namespace vk
{
  namespace su
  {
    const uint64_t FenceTimeout = 100000000;

    struct BufferData
    {
      BufferData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device, vk::DeviceSize size, vk::BufferUsageFlags usage);

      vk::UniqueBuffer        buffer;
      vk::UniqueDeviceMemory  deviceMemory;
    };

    struct ImageData
    {
      ImageData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice & device, vk::Format format, vk::Extent2D const& extent, vk::ImageTiling tiling, vk::ImageUsageFlags usage
        , vk::ImageLayout initialLayout, vk::MemoryPropertyFlags memoryProperties, vk::ImageAspectFlags aspectMask);

      vk::Format              format;
      vk::UniqueImage         image;
      vk::UniqueDeviceMemory  deviceMemory;
      vk::UniqueImageView     imageView;
    };

    struct DepthBufferData : public ImageData
    {
      DepthBufferData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice & device, vk::Format format, vk::Extent2D const& extent);
    };

    struct SurfaceData
    {
      SurfaceData(vk::UniqueInstance &instance, std::string const& className, std::string const& windowName, vk::Extent2D const& extent);

      vk::Extent2D          extent;
      HWND                  window;
      vk::UniqueSurfaceKHR  surface;
    };

    struct SwapChainData
    {
      SwapChainData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device, vk::UniqueSurfaceKHR &surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage, uint32_t graphicsFamilyIndex, uint32_t presentFamilyIndex);

      vk::Format                        colorFormat;
      vk::UniqueSwapchainKHR            swapChain;
      std::vector<vk::Image>            images;
      std::vector<vk::UniqueImageView>  imageViews;
    };

    struct TextureData
    {
      TextureData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device);

      void setCheckerboardTexture(vk::UniqueDevice &device, vk::UniqueCommandBuffer &commandBuffer);

      vk::Format                  format;
      vk::Extent2D                extent;
      bool                        needsStaging;
      std::unique_ptr<BufferData> bufferData;
      std::unique_ptr<ImageData>  imageData;
      vk::UniqueSampler           textureSampler;
    };


    template <typename TargetType, typename SourceType>
    VULKAN_HPP_INLINE TargetType checked_cast(SourceType value)
    {
      static_assert(sizeof(TargetType) <= sizeof(SourceType), "No need to cast from smaller to larger type!");
      static_assert(!std::numeric_limits<TargetType>::is_signed, "Only unsigned types supported!");
      static_assert(!std::numeric_limits<SourceType>::is_signed, "Only unsigned types supported!");
      assert(value <= std::numeric_limits<TargetType>::max());
      return static_cast<TargetType>(value);
    }

    template <class T>
    void copyToDevice(vk::UniqueDevice &device, vk::UniqueDeviceMemory &memory, T const* pData, size_t count)
    {
      uint8_t* deviceData = static_cast<uint8_t*>(device->mapMemory(memory.get(), 0, count * sizeof(T)));
      memcpy(deviceData, pData, count * sizeof(T));
      device->unmapMemory(memory.get());
    }

    template <class T>
    void copyToDevice(vk::UniqueDevice &device, vk::UniqueDeviceMemory &memory, T const& data)
    {
      copyToDevice<T>(device, memory, &data, 1);
    }

    template<class T>
    VULKAN_HPP_INLINE constexpr const T& clamp(const T& v, const T& lo, const T& hi)
    {
      return v < lo ? lo : hi < v ? hi : v;
    }

    vk::UniqueDeviceMemory allocateMemory(vk::UniqueDevice &device, vk::PhysicalDeviceMemoryProperties const& memoryProperties, vk::MemoryRequirements const& memoryRequirements, vk::MemoryPropertyFlags memoryPropertyFlags);
    vk::UniqueCommandPool createCommandPool(vk::UniqueDevice &device, uint32_t queueFamilyIndex);
    vk::UniqueDebugReportCallbackEXT createDebugReportCallback(vk::UniqueInstance &instance);
    vk::UniqueDescriptorPool createDescriptorPool(vk::UniqueDevice &device, bool textured = false);
    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::UniqueDevice &device, bool textured = false);
    vk::UniqueDevice createDevice(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<std::string> const& extensions = {});
    std::vector<vk::UniqueFramebuffer> createFramebuffers(vk::UniqueDevice &device, vk::UniqueRenderPass &renderPass, std::vector<vk::UniqueImageView> const& imageViews, vk::UniqueImageView &depthImageView, vk::Extent2D const& extent);
    vk::UniquePipeline createGraphicsPipeline(vk::UniqueDevice &device, vk::UniquePipelineCache &pipelineCache, vk::UniqueShaderModule &vertexShaderModule, vk::UniqueShaderModule &fragmentShaderModule, uint32_t vertexStride, vk::UniquePipelineLayout &pipelineLayout, vk::UniqueRenderPass &renderPass);
    vk::UniqueInstance createInstance(std::string const& appName, std::string const& engineName, std::vector<std::string> const& extensions = {}, uint32_t apiVersion = VK_API_VERSION_1_0);
    vk::UniqueRenderPass createRenderPass(vk::UniqueDevice &device, vk::Format colorFormat, vk::Format depthFormat);
    VkBool32 debugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
    uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::UniqueSurfaceKHR & surface);
    uint32_t findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits, vk::MemoryPropertyFlags requirementsMask);
    std::vector<std::string> getDeviceExtensions();
    std::vector<std::string> getInstanceExtensions();
    vk::Format pickColorFormat(std::vector<vk::SurfaceFormatKHR> const& formats);
    void setImageLayout(vk::UniqueCommandBuffer &commandBuffer, vk::Image image, vk::ImageAspectFlags aspectFlags, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout, vk::PipelineStageFlags sourceStageMask, vk::PipelineStageFlags destinationStageMask);
    void submitAndWait(vk::UniqueDevice &device, vk::Queue queue, vk::UniqueCommandBuffer &commandBuffer);
    void updateDescriptorSets(vk::UniqueDevice &device, vk::UniqueDescriptorSet &descriptorSet, vk::DescriptorBufferInfo const* descriptorBufferInfo, vk::DescriptorImageInfo const* descriptorImageInfo = nullptr);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND initializeWindow(std::string const& className, std::string const& windowName, LONG width, LONG height);
#else
#pragma error "unhandled platform"
#endif
  }
}
