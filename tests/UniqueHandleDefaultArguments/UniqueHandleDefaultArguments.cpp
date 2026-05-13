// SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Test: Compile only test for issue 467.

// Should be used on 64 bit only, as on 32 bit the test is ambiguous.

#if !defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <vulkan/vulkan.h>
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  {
    VkSurfaceKHR surface       = 0;
    auto         uniqueSurface = vk::UniqueSurfaceKHR( static_cast<vk::SurfaceKHR>( surface ), vk::Instance() );
  }

  {
    vk::PhysicalDevice phys              = {};
    auto               uniqueDisplayMode = phys.createDisplayModeKHRUnique( {}, {} );
  }

  {
    vk::Device device           = {};
    auto       uniquePerfConfig = device.acquirePerformanceConfigurationINTELUnique( {} );
  }

  {
    vk::PhysicalDevice phys          = {};
    auto               uniqueDisplay = phys.getDrmDisplayEXTUnique( 0, 0 );
  }
  return 0;
}
