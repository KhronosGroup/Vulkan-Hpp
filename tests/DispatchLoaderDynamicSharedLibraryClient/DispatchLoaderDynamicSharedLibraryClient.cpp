// SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : DispatchLoaderDynamicSharedLibraryClient
//                   Compile test on DispatchLoaderDynamic functions

#if !defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include "../test_macros.hpp"
#include <iostream>
#include <map>
#include <vulkan/vulkan.hpp>

int main()
{
  try
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    vk::Instance instance = vk::createInstance( {}, nullptr );

    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );

    // create a dispatcher, based on additional vkDevice/vkGetDeviceProcAddr
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    release_assert( !physicalDevices.empty() );

    vk::Device device = physicalDevices[0].createDevice( {}, nullptr );

    // function pointer specialization for device
    VULKAN_HPP_DEFAULT_DISPATCHER.init( device );
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  return 0;
}
