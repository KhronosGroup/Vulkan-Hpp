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

    glslang::InitializeProcess();

    std::vector<unsigned int> vertexShaderSPV;
    bool ok = vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C, vertexShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo              vertexShaderModuleCreateInfo( {}, vertexShaderSPV );
    std::unique_ptr<vk::raii::ShaderModule> vertexShaderModule =
      vk::raii::su::make_unique<vk::raii::ShaderModule>( *device, vertexShaderModuleCreateInfo );

    std::vector<unsigned int> fragmentShaderSPV;
    ok = vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C, fragmentShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo              fragmentShaderModuleCreateInfo( {}, fragmentShaderSPV );
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule =
      vk::raii::su::make_unique<vk::raii::ShaderModule>( *device, fragmentShaderModuleCreateInfo );

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
