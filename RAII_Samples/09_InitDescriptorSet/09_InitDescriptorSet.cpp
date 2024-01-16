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
// VulkanHpp Samples : 09_InitDescriptorSet
//                     Initialize a descriptor set

#if defined( _MSC_VER )
#  pragma warning( disable : 4201 )  // disable warning C4201: nonstandard extension used: nameless struct/union; needed
                                     // to get glm/detail/type_vec?.hpp without warnings
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../../samples/utils/math.hpp"
#include "../utils/utils.hpp"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

static char const * AppName    = "09_InitDescriptorSet";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    uint32_t         graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::raii::Device device                   = vk::raii::su::makeDevice( physicalDevice, graphicsQueueFamilyIndex );

    vk::raii::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4              mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( vk::Extent2D( 0, 0 ) );
    vk::raii::su::copyToDevice( uniformBufferData.deviceMemory, mvpcMatrix );

    vk::raii::DescriptorSetLayout descriptorSetLayout =
      vk::raii::su::makeDescriptorSetLayout( device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );

    /* VULKAN_HPP_KEY_START */

    // create a descriptor pool
    vk::DescriptorPoolSize       poolSize( vk::DescriptorType::eUniformBuffer, 1 );
    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSize );
    vk::raii::DescriptorPool     descriptorPool( device, descriptorPoolCreateInfo );

    // allocate a descriptor set
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, *descriptorSetLayout );
    vk::raii::DescriptorSet       descriptorSet = std::move( vk::raii::DescriptorSets( device, descriptorSetAllocateInfo ).front() );

    vk::DescriptorBufferInfo descriptorBufferInfo( uniformBufferData.buffer, 0, sizeof( glm::mat4x4 ) );
    vk::WriteDescriptorSet   writeDescriptorSet( descriptorSet, 0, 0, vk::DescriptorType::eUniformBuffer, {}, descriptorBufferInfo );
    device.updateDescriptorSets( writeDescriptorSet, nullptr );

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
