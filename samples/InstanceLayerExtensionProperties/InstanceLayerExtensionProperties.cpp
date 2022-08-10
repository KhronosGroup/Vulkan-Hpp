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
// VulkanHpp Samples : InstanceLayerExtensionProperties
//                     Get list of global layers and their associated extensions, if any.

#include <iostream>
#include <sstream>
#include <vector>
#include <vulkan/vulkan.hpp>

struct PropertyData
{
  PropertyData( vk::LayerProperties const & layerProperties_, std::vector<vk::ExtensionProperties> const & extensionProperties_ )
    : layerProperties( layerProperties_ ), extensionProperties( extensionProperties_ )
  {
  }

  vk::LayerProperties                  layerProperties;
  std::vector<vk::ExtensionProperties> extensionProperties;
};

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();

    /* VULKAN_KEY_START */

    std::vector<PropertyData> propertyData;
    propertyData.reserve( layerProperties.size() );

    for ( auto const & layerProperty : layerProperties )
    {
      std::vector<vk::ExtensionProperties> extensionProperties =
        vk::enumerateInstanceExtensionProperties( vk::Optional<const std::string>( layerProperty.layerName ) );
      propertyData.emplace_back( layerProperty, extensionProperties );
    }

    /* VULKAN_KEY_END */

    std::cout << "Instance Layers:" << std::endl;
    if ( propertyData.empty() )
    {
      std::cout << "Set the environment variable VK_LAYER_PATH to point to the location of your layers" << std::endl;
    }
    else
    {
      for ( auto const & pd : propertyData )
      {
        std::cout << pd.layerProperties.layerName << std::endl;
        std::cout << "Layer Extensions: ";
        if ( pd.extensionProperties.empty() )
        {
          std::cout << "None";
        }
        else
        {
          for ( auto it = pd.extensionProperties.begin(); it != pd.extensionProperties.end(); ++it )
          {
            if ( it != pd.extensionProperties.begin() )
            {
              std::cout << ", ";
            }
            std::cout << it->extensionName << " Version " << it->specVersion;
          }
        }
        std::cout << std::endl << std::endl;
      }
    }
    std::cout << std::endl;
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
