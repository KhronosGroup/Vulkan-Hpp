// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : DispatchLoaderDynamic
//                     Compile test on DispatchLoaderDynamic functions

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <iostream>
#include <map>
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // three equivalent minimal initializations of the default dispatcher... you just need to use one of them

    // initialize minimal set of function pointers
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    // the same initialization, now with explicitly providing a DynamicLoader
    vk::DynamicLoader dl;
    VULKAN_HPP_DEFAULT_DISPATCHER.init( dl );

    // the same initialization, now with explicitly providing the initial function pointer
    PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( getInstanceProcAddr );

    vk::Instance instance = vk::createInstance( {}, nullptr );

    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );

    // create a dispatcher, based on additional vkDevice/vkGetDeviceProcAddr
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    assert( !physicalDevices.empty() );

    vk::Device device = physicalDevices[0].createDevice( {}, nullptr );

    // optional function pointer specialization for device
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
