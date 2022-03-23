// Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 03_InitDeviceRAII
//                     Create and destroy a device

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "03_InitDeviceRAII";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    /* VULKAN_HPP_KEY_START */

    // find the index of the first queue family that supports graphics
    uint32_t graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );

    // create a Device
    float                     queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo( {}, graphicsQueueFamilyIndex, 1, &queuePriority );
    vk::DeviceCreateInfo      deviceCreateInfo( {}, deviceQueueCreateInfo );

    vk::raii::Device device( physicalDevice, deviceCreateInfo );

    /* VULKAN_HPP_KEY_END */
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
