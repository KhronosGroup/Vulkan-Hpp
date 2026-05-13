// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 02_EnumerateDevices
//                     Enumerate physical devices

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/utils.hpp"

#include <iostream>

static std::string AppName    = "02_EnumerateDevices";
static std::string EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    /* VULKAN_HPP_KEY_START */

    // enumerate the physicalDevices
    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    /* VULKAN_HPP_KEY_END */

#if !defined( NDEBUG )
    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
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
  return 0;
}
