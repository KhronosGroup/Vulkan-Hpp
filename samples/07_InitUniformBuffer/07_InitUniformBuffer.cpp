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
// VulkanHpp Samples : 07_InitUniformBuffer
//                     Initialize a uniform buffer

#if defined( _MSC_VER )
#  pragma warning( disable : 4127 )  // disable warning 4127: conditional expression is constant
#  pragma warning( disable : 4201 )  // disable warning C4201: nonstandard extension used: nameless struct/union; needed
                                     // to get glm/detail/type_vec?.hpp without warnings
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/utils.hpp"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

static char const * AppName    = "07_InitUniformBuffer";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    uint32_t   graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::Device device                   = vk::su::createDevice( physicalDevice, graphicsQueueFamilyIndex );

    /* VULKAN_HPP_KEY_START */

    glm::mat4x4 model      = glm::mat4x4( 1.0f );
    glm::mat4x4 view       = glm::lookAt( glm::vec3( -5.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
    glm::mat4x4 projection = glm::perspective( glm::radians( 45.0f ), 1.0f, 0.1f, 100.0f );
    // clang-format off
    glm::mat4x4 clip = glm::mat4x4( 1.0f,  0.0f, 0.0f, 0.0f,
                                    0.0f, -1.0f, 0.0f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 1.0f );  // vulkan clip space has inverted y and half z !
    // clang-format on
    glm::mat4x4 mvpc = clip * projection * view * model;

    vk::Buffer uniformDataBuffer =
      device.createBuffer( vk::BufferCreateInfo( vk::BufferCreateFlags(), sizeof( mvpc ), vk::BufferUsageFlagBits::eUniformBuffer ) );

    vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements( uniformDataBuffer );
    uint32_t               typeIndex          = vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                                                 memoryRequirements.memoryTypeBits,
                                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::DeviceMemory       uniformDataMemory  = device.allocateMemory( vk::MemoryAllocateInfo( memoryRequirements.size, typeIndex ) );

    uint8_t * pData = static_cast<uint8_t *>( device.mapMemory( uniformDataMemory, 0, memoryRequirements.size ) );
    memcpy( pData, &mvpc, sizeof( mvpc ) );
    device.unmapMemory( uniformDataMemory );

    device.bindBufferMemory( uniformDataBuffer, uniformDataMemory, 0 );

    // free device memory and destroy buffer
    device.destroyBuffer( uniformDataBuffer );  // the Buffer should be destroyed before the bound DeviceMemory is freed
    device.freeMemory( uniformDataMemory );

    /* VULKAN_HPP_KEY_END */

    device.destroy();
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
