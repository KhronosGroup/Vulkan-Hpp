// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 11_InitShaders
//                     Initialize vertex and fragment shaders

#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>

static char const * AppName    = "11_InitShaders";
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

    glslang::InitializeProcess();

    std::vector<unsigned int> vertexShaderSPV;
#if !defined( NDEBUG )
    bool ok =
#endif
      vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C, vertexShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo vertexShaderModuleCreateInfo( {}, vertexShaderSPV );
    vk::raii::ShaderModule     vertexShaderModule( device, vertexShaderModuleCreateInfo );

    std::vector<unsigned int> fragmentShaderSPV;
#if !defined( NDEBUG )
    ok =
#endif
      vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C, fragmentShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo fragmentShaderModuleCreateInfo( {}, fragmentShaderSPV );
    vk::raii::ShaderModule     fragmentShaderModule( device, fragmentShaderModuleCreateInfo );

    glslang::FinalizeProcess();

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
