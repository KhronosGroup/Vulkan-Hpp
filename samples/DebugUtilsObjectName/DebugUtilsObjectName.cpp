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
#include "vulkan/vulkan.hpp"

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
    vk::UniqueInstance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger( instance );
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert( !physicalDevices.empty() );

    uint32_t graphicsQueueFamilyIndex =
      vk::su::findGraphicsQueueFamilyIndex( physicalDevices[0].getQueueFamilyProperties() );
    vk::UniqueDevice device = vk::su::createDevice( physicalDevices[0], graphicsQueueFamilyIndex );

    // create an image
    vk::ImageCreateInfo imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         vk::Format::eB8G8R8A8Unorm,
                                         vk::Extent3D( 640, 640, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         vk::ImageTiling::eLinear,
                                         vk::ImageUsageFlagBits::eTransferSrc );
    vk::UniqueImage     image = device->createImageUnique( imageCreateInfo );

    /* VULKAN_KEY_START */

    vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
      vk::ObjectType::eImage, NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST( VkImage, *image ), "Image name" );
    device->setDebugUtilsObjectNameEXT( debugUtilsObjectNameInfo );

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
