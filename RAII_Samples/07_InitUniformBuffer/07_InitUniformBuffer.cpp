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
// VulkanHpp Samples : 07_InitUniformBufferRAII
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
    std::unique_ptr<vk::raii::Context>  context = vk::raii::su::make_unique<vk::raii::Context>();
    std::unique_ptr<vk::raii::Instance> instance =
      vk::raii::su::makeUniqueInstance( *context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::makeUniqueDebugUtilsMessengerEXT( *instance );
#endif
    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::makeUniquePhysicalDevice( *instance );
    uint32_t                                  graphicsQueueFamilyIndex =
      vk::su::findGraphicsQueueFamilyIndex( physicalDevice->getQueueFamilyProperties() );
    std::unique_ptr<vk::raii::Device> device =
      vk::raii::su::makeUniqueDevice( *physicalDevice, graphicsQueueFamilyIndex );

    /* VULKAN_HPP_KEY_START */

    glm::mat4x4 model = glm::mat4x4( 1.0f );
    glm::mat4x4 view =
      glm::lookAt( glm::vec3( -5.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
    glm::mat4x4 projection = glm::perspective( glm::radians( 45.0f ), 1.0f, 0.1f, 100.0f );
    // clang-format off
    glm::mat4x4 clip = glm::mat4x4( 1.0f,  0.0f, 0.0f, 0.0f,
                                    0.0f, -1.0f, 0.0f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 0.0f,
                                    0.0f,  0.0f, 0.5f, 1.0f );  // vulkan clip space has inverted y and half z !
    // clang-format on
    glm::mat4x4 mvpc = clip * projection * view * model;

    vk::BufferCreateInfo              bufferCreateInfo( {}, sizeof( mvpc ), vk::BufferUsageFlagBits::eUniformBuffer );
    std::unique_ptr<vk::raii::Buffer> uniformDataBuffer =
      vk::raii::su::make_unique<vk::raii::Buffer>( *device, bufferCreateInfo );
    vk::MemoryRequirements memoryRequirements = uniformDataBuffer->getMemoryRequirements();

    uint32_t typeIndex =
      vk::su::findMemoryType( physicalDevice->getMemoryProperties(),
                              memoryRequirements.memoryTypeBits,
                              vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    vk::MemoryAllocateInfo                  memoryAllocateInfo( memoryRequirements.size, typeIndex );
    std::unique_ptr<vk::raii::DeviceMemory> uniformDataMemory =
      vk::raii::su::make_unique<vk::raii::DeviceMemory>( *device, memoryAllocateInfo );

    uint8_t * pData = static_cast<uint8_t *>( uniformDataMemory->mapMemory( 0, memoryRequirements.size ) );
    memcpy( pData, &mvpc, sizeof( mvpc ) );
    uniformDataMemory->unmapMemory();

    uniformDataBuffer->bindMemory( **uniformDataMemory, 0 );

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
