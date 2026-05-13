// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : EnumerateDevicesAdvanced
//                     Enumerate physical devices

#include "../utils/utils.hpp"

#include <iomanip>
#include <iostream>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "EnumerateDevicesAdvanced";
static char const * EngineName = "Vulkan.hpp";

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
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    for ( auto const & physicalDevice : physicalDevices )
    {
      vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

      std::cout << "apiVersion: ";
      std::cout << ( ( properties.apiVersion >> 22 ) & 0xfff ) << '.';  // Major.
      std::cout << ( ( properties.apiVersion >> 12 ) & 0x3ff ) << '.';  // Minor.
      std::cout << ( properties.apiVersion & 0xfff );                   // Patch.
      std::cout << '\n';

      std::cout << "driverVersion: " << properties.driverVersion << '\n';

      std::cout << std::showbase << std::internal << std::setfill( '0' ) << std::hex;
      std::cout << "vendorId: " << std::setw( 6 ) << properties.vendorID << '\n';
      std::cout << "deviceId: " << std::setw( 6 ) << properties.deviceID << '\n';
      std::cout << std::noshowbase << std::right << std::setfill( ' ' ) << std::dec;

      std::cout << "deviceType: " << vk::to_string( properties.deviceType ) << "\n";

      std::cout << "deviceName: " << properties.deviceName << '\n';

      std::cout << "pipelineCacheUUID: " << vk::su::UUID( properties.pipelineCacheUUID ) << "\n\n";
    }

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
