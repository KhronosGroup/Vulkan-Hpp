// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : DispatchLoaderStatic
//                   Compile test on DispatchLoaderStatic functions

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
#include <cassert>
import vulkan;
#else
#  include <iostream>
#  include <map>
#  include <vulkan/vulkan.hpp>
#endif


int main()
{
  try
  {
    vk::Instance instance = vk::createInstance( {} );

    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    release_assert( !physicalDevices.empty() );

    vk::Device device = physicalDevices[0].createDevice( {} );

    device.destroy();
    instance.destroy();
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    std::exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    std::exit( -1 );
  }

  return 0;
}
