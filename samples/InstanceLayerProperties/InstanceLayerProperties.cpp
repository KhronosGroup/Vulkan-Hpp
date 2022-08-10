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
// VulkanHpp Samples : InstanceLayerProperties
//                     Get global layer properties to know what layers are available to enable at CreateInstance time.

#include <iostream>
#include <sstream>
#include <vector>
#include <vulkan/vulkan.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    /* VULKAN_KEY_START */

    std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();

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
    std::cout << "std::runtexceptionime_error: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }
  return 0;
}
