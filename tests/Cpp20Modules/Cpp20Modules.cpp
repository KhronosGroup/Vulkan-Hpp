// Copyright(c) 2024, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Test : Cpp20Module
//                  Compile test on using c++20 modules

import vulkan_hpp;

#include <vulkan/vulkan_hpp_macros.hpp>

static std::string AppName    = "Cpp20Modules";
static std::string EngineName = "Vulkan.cppm";

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  std::cout << "test Cpp20Module" << std::endl;

  /* VULKAN_HPP_KEY_START */
  try
  {
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize minimal set of function pointers
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif

    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, vk::makeApiVersion( 1, 0, 0, 0 ) );

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );

    // create an Instance
    vk::Instance instance = vk::createInstance( instanceCreateInfo );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );
#endif

    // destroy it again
    instance.destroy();
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  /* VULKAN_HPP_KEY_END */

  return 0;
}
