// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : SurfaceFormats
//                     Get surface formats.

#include "../utils/utils.hpp"

#include <iomanip>
#include <sstream>
#include <vector>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "SurfaceFormats";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
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
      std::cout << "PhysicalDevice " << i << "\n";
      std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevices[i].getSurfaceFormatsKHR( surfaceData.surface );
      for ( size_t j = 0; j < surfaceFormats.size(); j++ )
      {
        std::cout << std::string( "\t" ) << "Format " << j << "\n";
        std::cout << std::string( "\t\t" ) << "colorSpace  = " << vk::to_string( surfaceFormats[j].colorSpace ) << "\n";
        std::cout << std::string( "\t\t" ) << "format      = " << vk::to_string( surfaceFormats[j].format ) << "\n";
        std::cout << "\n";
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
