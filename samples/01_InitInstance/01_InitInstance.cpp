// Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 01_InitInstance
//                     Create and destroy a vk::Instance

#include <iostream>
#include <vulkan/vulkan.hpp>

static std::string AppName    = "01_InitInstance";
static std::string EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  /* VULKAN_HPP_KEY_START */

  try
  {
    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, VK_API_VERSION_1_1 );

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );

    // create an Instance
    vk::Instance instance = vk::createInstance( instanceCreateInfo );

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
