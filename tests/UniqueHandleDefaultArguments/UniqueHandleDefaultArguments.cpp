// Copyright(c) 2020, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Test: Compile only test for issue 467.

// Should be used on 64 bit only, as on 32 bit the test is ambiguous.

#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <vulkan/vulkan.h>
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  VkSurfaceKHR surface       = 0;
  auto         uniqueSurface = vk::UniqueSurfaceKHR( static_cast<vk::SurfaceKHR>( surface ), vk::Instance() );

  vk::PhysicalDevice phys              = {};
  auto               uniqueDisplayMode = phys.createDisplayModeKHRUnique( {}, {} );

  vk::Device device           = {};
  auto       uniquePerfConfig = device.acquirePerformanceConfigurationINTELUnique( {} );

  return 0;
}
