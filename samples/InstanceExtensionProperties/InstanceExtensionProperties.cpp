// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : InstanceExtensionProperties
//                     Get global extension properties to know what extension are available to enable at CreateInstance
//                     time.

#include <iostream>
#include <sstream>
#include <vulkan/vulkan.hpp>

int main()
{
  try
  {
    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();

    // sort the extensions alphabetically

    std::sort( extensionProperties.begin(),
               extensionProperties.end(),
               []( vk::ExtensionProperties const & a, vk::ExtensionProperties const & b ) { return strcmp( a.extensionName, b.extensionName ) < 0; } );

    std::cout << "Instance Extensions:" << std::endl;
    for ( auto const & ep : extensionProperties )
    {
      std::cout << ep.extensionName << ":" << std::endl;
      std::cout << "\tVersion: " << ep.specVersion << std::endl;
      std::cout << std::endl;
    }

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
