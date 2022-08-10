// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : InstanceVersion
//                     Get the version of instance-level functionality supported by the implementation.

#include <iostream>
#include <sstream>
#include <vulkan/vulkan.hpp>

std::string decodeAPIVersion( uint32_t apiVersion )
{
  return std::to_string( VK_VERSION_MAJOR( apiVersion ) ) + "." + std::to_string( VK_VERSION_MINOR( apiVersion ) ) + "." +
         std::to_string( VK_VERSION_PATCH( apiVersion ) );
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    /* VULKAN_KEY_START */

    uint32_t apiVersion = vk::enumerateInstanceVersion();
    std::cout << "APIVersion = " << decodeAPIVersion( apiVersion ) << std::endl;

    /* VULKAN_KEY_END */
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
  return 0;
}
