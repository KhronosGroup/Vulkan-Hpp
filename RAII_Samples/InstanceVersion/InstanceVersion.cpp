// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : InstanceVersion
//                     Get the version of instance-level functionality supported by the implementation.

#include "../utils/utils.hpp"

#include <iostream>
#include <sstream>

std::string decodeAPIVersion( uint32_t apiVersion )
{
  return std::to_string( VK_VERSION_MAJOR( apiVersion ) ) + "." + std::to_string( VK_VERSION_MINOR( apiVersion ) ) + "." +
         std::to_string( VK_VERSION_PATCH( apiVersion ) );
}

int main()
{
  try
  {
    vk::raii::Context context;

    /* VULKAN_KEY_START */

    uint32_t apiVersion = context.enumerateInstanceVersion();
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
