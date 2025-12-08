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
// VulkanHpp Samples : DispatchLoaderStatic
//                     Compile test on DispatchLoaderStatic functions

#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan_hpp;
#else
#  include <iostream>
#  include <map>
#  include <vulkan/vulkan.hpp>
#endif

template<typename T> void release_assert( const T &condition )
{
  if ( !condition ) throw std::runtime_error( "failed assert" );
}

int main( int /*argc*/, char ** /*argv*/ )
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
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  return 0;
}
