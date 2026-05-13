// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 01_InitInstanceRAII
//                     Create and destroy a vk::raii::Instance

#include "../utils/utils.hpp"

#include <iostream>

static std::string AppName    = "01_InitInstanceRAII";
static std::string EngineName = "Vulkan.hpp";

int main()
{
  /* VULKAN_HPP_KEY_START */

  try
  {
    // the very beginning: instantiate a context
    vk::raii::Context context;

    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, VK_API_VERSION_1_1 );

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );

    // create an Instance
    vk::raii::Instance instance( context, instanceCreateInfo );
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
