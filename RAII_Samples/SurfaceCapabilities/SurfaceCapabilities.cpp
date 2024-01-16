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
// VulkanHpp Samples : SurfaceCapabilities
//                     Get surface capabilities.

#include "../utils/utils.hpp"

#include <iomanip>
#include <sstream>
#include <vector>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "SurfaceCapabilities";
static char const * EngineName = "Vulkan.hpp";

void cout( vk::SurfaceCapabilitiesKHR const & surfaceCapabilities )
{
  std::cout << std::string( "\t" ) << "Capabilities:\n";
  std::cout << std::string( "\t\t" ) << "currentExtent           = " << surfaceCapabilities.currentExtent.width << " x "
            << surfaceCapabilities.currentExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "currentTransform        = " << vk::to_string( surfaceCapabilities.currentTransform ) << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageArrayLayers     = " << surfaceCapabilities.maxImageArrayLayers << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageCount           = " << surfaceCapabilities.maxImageCount << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageExtent          = " << surfaceCapabilities.maxImageExtent.width << " x "
            << surfaceCapabilities.maxImageExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "minImageCount           = " << surfaceCapabilities.minImageCount << "\n";
  std::cout << std::string( "\t\t" ) << "minImageExtent          = " << surfaceCapabilities.minImageExtent.width << " x "
            << surfaceCapabilities.minImageExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "supportedCompositeAlpha = " << vk::to_string( surfaceCapabilities.supportedCompositeAlpha ) << "\n";
  std::cout << std::string( "\t\t" ) << "supportedTransforms     = " << vk::to_string( surfaceCapabilities.supportedTransforms ) << "\n";
  std::cout << std::string( "\t\t" ) << "supportedUsageFlags     = " << vk::to_string( surfaceCapabilities.supportedUsageFlags ) << "\n";
  std::cout << "\n";
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context context;

    std::vector<vk::ExtensionProperties> instanceExtensionProperties = context.enumerateInstanceExtensionProperties();

    bool supportsGetSurfaceCapabilities2 =
      std::any_of( instanceExtensionProperties.begin(),
                   instanceExtensionProperties.end(),
                   []( vk::ExtensionProperties const & ep ) { return strcmp( ep.extensionName, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME ) == 0; } );

    std::vector<std::string> extensions = vk::su::getInstanceExtensions();
    if ( supportsGetSurfaceCapabilities2 )
    {
      extensions.push_back( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME );
    }

    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, extensions );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    vk::raii::PhysicalDevices physicalDevices( instance );

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      // some properties are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";
      if ( supportsGetSurfaceCapabilities2 )
      {
        auto surfaceCapabilities2 =
          physicalDevices[i]
            .getSurfaceCapabilities2KHR<vk::SurfaceCapabilities2KHR,
                                        vk::DisplayNativeHdrSurfaceCapabilitiesAMD,
                                        vk::SharedPresentSurfaceCapabilitiesKHR,
                                        vk::SurfaceProtectedCapabilitiesKHR>( { static_cast<vk::SurfaceKHR>( surfaceData.surface ) } );

        vk::SurfaceCapabilitiesKHR const & surfaceCapabilities = surfaceCapabilities2.get<vk::SurfaceCapabilities2KHR>().surfaceCapabilities;
        cout( surfaceCapabilities );

        if ( vk::su::contains( extensionProperties, "VK_AMD_display_native_hdr" ) )
        {
          vk::DisplayNativeHdrSurfaceCapabilitiesAMD displayNativeHdrSurfaceCapabilities =
            surfaceCapabilities2.get<vk::DisplayNativeHdrSurfaceCapabilitiesAMD>();
          std::cout << std::string( "\t" ) << "DisplayNativeHdrSurfaceCapabilitiesAMD:\n";
          std::cout << std::string( "\t\t" ) << "localDimmingSupport = " << !!displayNativeHdrSurfaceCapabilities.localDimmingSupport << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_KHR_shared_presentable_image" ) )
        {
          vk::SharedPresentSurfaceCapabilitiesKHR sharedPresentSurfaceCapabilities = surfaceCapabilities2.get<vk::SharedPresentSurfaceCapabilitiesKHR>();
          std::cout << std::string( "\t" ) << "SharedPresentSurfaceCapabilitiesKHR:\n";
          std::cout << std::string( "\t\t" )
                    << "sharedPresentSupportedUsageFlags  = " << vk::to_string( sharedPresentSurfaceCapabilities.sharedPresentSupportedUsageFlags ) << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_KHR_surface_protected_capabilities" ) )
        {
          vk::SurfaceProtectedCapabilitiesKHR surfaceProtectedCapabilities = surfaceCapabilities2.get<vk::SurfaceProtectedCapabilitiesKHR>();
          std::cout << std::string( "\t" ) << "SurfaceProtectedCapabilitiesKHR:\n";
          std::cout << std::string( "\t\t" ) << "supportsProtected  = " << !!surfaceProtectedCapabilities.supportsProtected << "\n";
          std::cout << "\n";
        }
      }
      else
      {
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevices[i].getSurfaceCapabilitiesKHR( surfaceData.surface );
        cout( surfaceCapabilities );
      }
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
