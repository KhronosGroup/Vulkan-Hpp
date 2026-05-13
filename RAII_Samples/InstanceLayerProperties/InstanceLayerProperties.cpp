// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : InstanceLayerProperties
//                     Get global layer properties to know what layers are available to enable at CreateInstance time.

#include "../utils/utils.hpp"

#include <iostream>
#include <sstream>
#include <vector>

int main()
{
  try
  {
    vk::raii::Context context;

    /* VULKAN_KEY_START */

    std::vector<vk::LayerProperties> layerProperties = context.enumerateInstanceLayerProperties();

    std::cout << "Instance Layers:" << std::endl;
    if ( layerProperties.empty() )
    {
      std::cout << "Set the environment variable VK_LAYER_PATH to point to the location of your layers" << std::endl;
    }
    for ( auto const & lp : layerProperties )
    {
      std::cout << lp.layerName << ":" << std::endl;
      std::cout << "\tVersion: " << lp.implementationVersion << std::endl;
      std::cout << "\tAPI Version: (" << ( lp.specVersion >> 22 ) << "." << ( ( lp.specVersion >> 12 ) & 0x03FF ) << "." << ( lp.specVersion & 0xFFF ) << ")"
                << std::endl;
      std::cout << "\tDescription: " << lp.description << std::endl;
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
