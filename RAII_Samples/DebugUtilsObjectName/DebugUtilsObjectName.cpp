// Copyright(c) 2020, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : DebugUtilsObjectName
//                     Demonstrate usage of DebugUtilsObjectName

#include "../utils/utils.hpp"

static char const * AppName    = "DebugUtilsObjectName";
static char const * EngineName = "Vulkan.hpp";

#if defined( _MSC_VER ) && !defined( _WIN64 )
#  define NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( type, x ) static_cast<type>( x )
#else
#  define NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( type, x ) reinterpret_cast<uint64_t>( static_cast<type>( x ) )
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::raii::PhysicalDevices physicalDevices( instance );
    assert( !physicalDevices.empty() );

    uint32_t         graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevices[0].getQueueFamilyProperties() );
    vk::raii::Device device                   = vk::raii::su::makeDevice( physicalDevices[0], graphicsQueueFamilyIndex );

    // create an image
    vk::raii::Image image = vk::raii::su::makeImage( device );

    /* VULKAN_KEY_START */

    vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
      vk::ObjectType::eImage, NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( VkImage, static_cast<vk ::Image>( image ) ), "Image name" );
    device.setDebugUtilsObjectNameEXT( debugUtilsObjectNameInfo );

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
