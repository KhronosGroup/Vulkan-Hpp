// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : NoDefaultDispatcher
//                   Compile test with VULKAN_HPP_NO_DEFAULT_DISPATCHER defined


#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
#  include <vulkan/vulkan_raii.hpp>
#  include <vulkan/vulkan_shared.hpp>
#endif

int main()
{
  return 0;
}
