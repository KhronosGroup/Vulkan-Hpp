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
// VulkanHpp Samples : 16_Vulkan_1_1
//                     Determine if the current system can use Vulkan 1.1 API features

#include "../utils/utils.hpp"

static char const * AppName    = "16_Vulkan_1_1";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    /* VULKAN_KEY_START */

    // Keep track of the major/minor version we can actually use
    uint16_t    usingMajorVersion  = 1;
    uint16_t    usingMinorVersion  = 0;
    std::string usingVersionString = "";

    // Set the desired version we want
    uint16_t    desiredMajorVersion  = 1;
    uint16_t    desiredMinorVersion  = 1;
    uint32_t    desiredVersion       = VK_MAKE_VERSION( desiredMajorVersion, desiredMinorVersion, 0 );
    std::string desiredVersionString = "";
    desiredVersionString += std::to_string( desiredMajorVersion );
    desiredVersionString += ".";
    desiredVersionString += std::to_string( desiredMinorVersion );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize the DipatchLoaderDynamic to use
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif

    // Determine what API version is available
    uint32_t apiVersion = vk::enumerateInstanceVersion();

    // Translate the version into major/minor for easier comparison
    uint32_t loader_major_version = VK_VERSION_MAJOR( apiVersion );
    uint32_t loader_minor_version = VK_VERSION_MINOR( apiVersion );
    std::cout << "Loader/Runtime support detected for Vulkan " << loader_major_version << "." << loader_minor_version << "\n";

    // Check current version against what we want to run
    if ( loader_major_version > desiredMajorVersion || ( loader_major_version == desiredMajorVersion && loader_minor_version >= desiredMinorVersion ) )
    {
      // Create the instance
      vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions(), desiredVersion );
#if !defined( NDEBUG )
      vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

      // Get the list of physical devices
      std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

      // Go through the list of physical devices and select only those that are capable of running the API version we
      // want.
      std::vector<vk::PhysicalDevice> desiredPhysicalDevices;
      std::copy_if( physicalDevices.begin(),
                    physicalDevices.end(),
                    std::back_inserter( desiredPhysicalDevices ),
                    [desiredVersion]( vk::PhysicalDevice const & pd ) { return pd.getProperties().apiVersion >= desiredVersion; } );

      // If we have something in the desired version physical device list, we're good
      if ( desiredPhysicalDevices.size() > 0 )
      {
        usingMajorVersion = desiredMajorVersion;
        usingMinorVersion = desiredMinorVersion;
      }

#if !defined( NDEBUG )
      instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
      instance.destroy();
    }

    usingVersionString += std::to_string( usingMajorVersion );
    usingVersionString += ".";
    usingVersionString += std::to_string( usingMinorVersion );

    if ( usingMinorVersion < desiredMinorVersion )
    {
      std::cout << "Determined that this system can only use Vulkan API version " << usingVersionString << " instead of desired version "
                << desiredVersionString << std::endl;
    }
    else
    {
      std::cout << "Determined that this system can run desired Vulkan API version " << desiredVersionString << std::endl;
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
