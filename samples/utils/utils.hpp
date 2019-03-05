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

    template <typename TargetType, typename SourceType>
    VULKAN_HPP_INLINE TargetType checked_cast(SourceType value)
    {
      static_assert(sizeof(TargetType) <= sizeof(SourceType), "No need to cast from smaller to larger type!");
      static_assert(!std::numeric_limits<TargetType>::is_signed, "Only unsigned types supported!");
      static_assert(!std::numeric_limits<SourceType>::is_signed, "Only unsigned types supported!");
      assert(value <= std::numeric_limits<TargetType>::max());
      return static_cast<TargetType>(value);
    }

    template<class T>
    VULKAN_HPP_INLINE constexpr const T& clamp(const T& v, const T& lo, const T& hi)
    {
      return v < lo ? lo : hi < v ? hi : v;
    }

    vk::UniqueDeviceMemory allocateMemory(vk::UniqueDevice &device, vk::PhysicalDeviceMemoryProperties const& memoryProperties, vk::MemoryRequirements const& memoryRequirements, vk::MemoryPropertyFlags memoryPropertyFlags);
    vk::UniqueDebugReportCallbackEXT createDebugReportCallback(vk::UniqueInstance &instance);
    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(vk::UniqueDevice &device);
    vk::UniqueDevice createDevice(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<std::string> const& extensions = {});
    vk::UniqueInstance createInstance(std::string const& appName, std::string const& engineName, std::vector<std::string> const& extensions = {});
    VkBool32 debugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
    uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::UniqueSurfaceKHR & surface);
    std::vector<std::string> getDeviceExtensions();
    std::vector<std::string> getInstanceExtensions();
    vk::Format pickColorFormat(std::vector<vk::SurfaceFormatKHR> const& formats);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND initializeWindow(std::string const& className, std::string const& windowName, LONG width, LONG height);
#else
#pragma error "unhandled platform"
#endif
  }
}
