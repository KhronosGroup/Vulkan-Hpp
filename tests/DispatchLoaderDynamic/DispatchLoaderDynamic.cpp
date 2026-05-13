// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : DispatchLoaderDynamic
//                   Compile test on DispatchLoaderDynamic functions

#if !defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <map>
#  include <vector>
#  include <iostream>
#  include <vulkan/vulkan.hpp>
   VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  try
  {
    // three equivalent minimal initializations of the default dispatcher... you just need to use one of them

    // initialize minimal set of function pointers
    vk::detail::defaultDispatchLoaderDynamic.init();

    // the same initialization, now with explicitly providing a DynamicLoader
    vk::detail::DynamicLoader dl;
    vk::detail::defaultDispatchLoaderDynamic.init( dl );

    // the same initialization, now with explicitly providing the initial function pointer
    PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    vk::detail::defaultDispatchLoaderDynamic.init( getInstanceProcAddr );

    vk::Instance instance = vk::createInstance( {}, nullptr );

    // initialize function pointers for instance
    vk::detail::defaultDispatchLoaderDynamic.init( instance );

    // create a dispatcher, based on additional vkDevice/vkGetDeviceProcAddr
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    release_assert( !physicalDevices.empty() );

    vk::Device device = physicalDevices[0].createDevice( {}, nullptr );

    // optional function pointer specialization for device
    vk::detail::defaultDispatchLoaderDynamic.init( device );
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
