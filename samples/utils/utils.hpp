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
      BufferData(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::DeviceSize size, vk::BufferUsageFlags usage,
                 vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

      vk::UniqueBuffer        buffer;
      vk::UniqueDeviceMemory  deviceMemory;
    };

    struct ImageData
    {
      ImageData(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::Format format, vk::Extent2D const& extent, vk::ImageTiling tiling, vk::ImageUsageFlags usage
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

    class CheckerboardTextureCreator
    {
    public:
      void operator()(void* data, vk::Extent2D &extent) const;
    };

    class MonochromeTextureGenerator
    {
    public:
      MonochromeTextureGenerator(std::array<unsigned char, 3> const& rgb_);

      void operator()(void* data, vk::Extent2D &extent) const;

    private:
      std::array<unsigned char, 3> const& rgb;
    };


    struct TextureData
    {
      TextureData(vk::PhysicalDevice &physicalDevice, vk::UniqueDevice &device, vk::Extent2D const& extent_ = {256, 256}, vk::ImageUsageFlags usageFlags = {},
                  vk::FormatFeatureFlags formatFeatureFlags = {});

      template <typename TextureCreator>
      void setTexture(vk::UniqueDevice &device, vk::UniqueCommandBuffer &commandBuffer, TextureCreator const& textureCreator)
      {
        void* data = needsStaging
          ? device->mapMemory(stagingBufferData->deviceMemory.get(), 0, device->getBufferMemoryRequirements(stagingBufferData->buffer.get()).size)
          : device->mapMemory(imageData->deviceMemory.get(), 0, device->getImageMemoryRequirements(imageData->image.get()).size);
        textureCreator(data, extent);
        device->unmapMemory(needsStaging ? stagingBufferData->deviceMemory.get() : imageData->deviceMemory.get());

        if (needsStaging)
        {
          // Since we're going to blit to the texture image, set its layout to eTransferDstOptimal
          vk::su::setImageLayout(commandBuffer, imageData->image.get(), imageData->format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
          vk::BufferImageCopy copyRegion(0, extent.width, extent.height, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1), vk::Offset3D(0, 0, 0), vk::Extent3D(extent, 1));
          commandBuffer->copyBufferToImage(stagingBufferData->buffer.get(), imageData->image.get(), vk::ImageLayout::eTransferDstOptimal, copyRegion);
          // Set the layout for the texture image from eTransferDstOptimal to SHADER_READ_ONLY
          vk::su::setImageLayout(commandBuffer, imageData->image.get(), imageData->format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
        }
        else
        {
          // If we can use the linear tiled image as a texture, just do it
          vk::su::setImageLayout(commandBuffer, imageData->image.get(), imageData->format, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eShaderReadOnlyOptimal);
        }
      }

      vk::Format                  format;
      vk::Extent2D                extent;
      bool                        needsStaging;
      std::unique_ptr<BufferData> stagingBufferData;
      std::unique_ptr<ImageData>  imageData;
      vk::UniqueSampler           textureSampler;
    };

    struct UUID
    {
    public:
      UUID(uint8_t data[VK_UUID_SIZE]);

      uint8_t m_data[VK_UUID_SIZE];
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
    void copyToDevice(vk::UniqueDevice &device, vk::UniqueDeviceMemory &memory, T const* pData, size_t count, size_t stride = sizeof(T))
    {
      assert(sizeof(T) <= stride);
      uint8_t* deviceData = static_cast<uint8_t*>(device->mapMemory(memory.get(), 0, count * stride));
      if (stride == sizeof(T))
      {
        memcpy(deviceData, pData, count * sizeof(T));
      }
      else
      {
        for (size_t i = 0; i < count; i++)
        {
          memcpy(deviceData, &pData[i], sizeof(T));
          deviceData += stride;
        }
      }
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

    vk::UniqueDeviceMemory allocateMemory(vk::UniqueDevice const& device, vk::PhysicalDeviceMemoryProperties const& memoryProperties, vk::MemoryRequirements const& memoryRequirements,
                                          vk::MemoryPropertyFlags memoryPropertyFlags);
    vk::UniqueCommandPool createCommandPool(vk::UniqueDevice &device, uint32_t queueFamilyIndex);
    vk::UniqueDebugReportCallbackEXT createDebugReportCallback(vk::UniqueInstance &instance);
    vk::UniqueDescriptorPool createDescriptorPool(vk::UniqueDevice &device, std::vector<vk::DescriptorPoolSize> const& poolSizes);
    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::UniqueDevice &device, std::vector<std::pair<vk::DescriptorType, vk::ShaderStageFlags>> const& bindingData, vk::DescriptorSetLayoutCreateFlags flags = {});
    vk::UniqueDevice createDevice(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<std::string> const& extensions = {}, vk::PhysicalDeviceFeatures const* physicalDeviceFeatures = nullptr, void const* pNext = nullptr);
    std::vector<vk::UniqueFramebuffer> createFramebuffers(vk::UniqueDevice &device, vk::UniqueRenderPass &renderPass, std::vector<vk::UniqueImageView> const& imageViews, vk::UniqueImageView const& depthImageView, vk::Extent2D const& extent);
    vk::UniquePipeline createGraphicsPipeline(vk::UniqueDevice &device, vk::UniquePipelineCache &pipelineCache, vk::UniqueShaderModule &vertexShaderModule,
      vk::UniqueShaderModule &fragmentShaderModule, uint32_t vertexStride, bool depthBuffered, bool textured, vk::UniquePipelineLayout &pipelineLayout, vk::UniqueRenderPass &renderPass);
    vk::UniqueInstance createInstance(std::string const& appName, std::string const& engineName, std::vector<std::string> const& extensions = {}, uint32_t apiVersion = VK_API_VERSION_1_0);
    vk::UniqueRenderPass createRenderPass(vk::UniqueDevice &device, vk::Format colorFormat, vk::Format depthFormat, vk::AttachmentLoadOp loadOp = vk::AttachmentLoadOp::eClear, vk::ImageLayout colorFinalLayout = vk::ImageLayout::ePresentSrcKHR);
    VkBool32 debugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
    uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& surface);
    uint32_t findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits, vk::MemoryPropertyFlags requirementsMask);
    std::vector<std::string> getDeviceExtensions();
    std::vector<std::string> getInstanceExtensions();
    vk::PresentModeKHR pickPresentMode(std::vector<vk::PresentModeKHR> const& presentModes);
    vk::SurfaceFormatKHR pickSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& formats);
    void setImageLayout(vk::UniqueCommandBuffer &commandBuffer, vk::Image image, vk::Format format, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout);
    void submitAndWait(vk::UniqueDevice &device, vk::Queue queue, vk::UniqueCommandBuffer &commandBuffer);
    void updateDescriptorSets(vk::UniqueDevice &device, vk::UniqueDescriptorSet &descriptorSet, vk::DescriptorType descriptorType, vk::DescriptorBufferInfo const* descriptorBufferInfo, vk::DescriptorImageInfo const* descriptorImageInfo = nullptr);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND initializeWindow(std::string const& className, std::string const& windowName, LONG width, LONG height);
#else
#pragma error "unhandled platform"
#endif
  }
}

std::ostream& operator<<(std::ostream& os, vk::su::UUID const& uuid);
