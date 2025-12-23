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


#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <cassert>
import vulkan;
#else
#  include <map>
#  include <vector>
#  include <iostream>
#  include <cassert>
#  include <vulkan/vulkan.hpp>
#endif


namespace vk {
  namespace detail {
    DispatchLoaderDynamic defaultDispatchLoaderDynamic;
  }
}

int main( int /*argc*/, char ** /*argv*/ )
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
    assert( !physicalDevices.empty() );

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
