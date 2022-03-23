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
// VulkanHpp Samples : 08_InitPipelineLayoutRAII
//                     Initialize a descriptor and pipeline layout

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "08_InitPipelineLayout";
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

    /* VULKAN_HPP_KEY_START */

    // create a DescriptorSetLayout
    vk::DescriptorSetLayoutBinding    descriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex );
    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo( {}, descriptorSetLayoutBinding );
    vk::raii::DescriptorSetLayout     descriptorSetLayout( device, descriptorSetLayoutCreateInfo );

    // create a PipelineLayout using that DescriptorSetLayout
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo( {}, *descriptorSetLayout );
    vk::raii::PipelineLayout     pipelineLayout( device, pipelineLayoutCreateInfo );

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
