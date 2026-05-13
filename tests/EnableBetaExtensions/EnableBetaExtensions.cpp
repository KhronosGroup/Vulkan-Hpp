// SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : EnableBetaExtensions
//                   Compile test with VK_ENABLE_BETA_EXTENSIONS defined

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif


#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
# include <vulkan/vulkan.hpp>
  VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  vk::PhysicalDevice physicalDevice;

  auto features = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDevicePortabilitySubsetFeaturesKHR>();
  auto properties = physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePortabilitySubsetPropertiesKHR>();
  return 0;
}
