// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 11_InitShaders
//                     Initialize vertex and fragment shaders

#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "glslang/Public/ShaderLang.h"

#include <iostream>

static char const * AppName    = "11_InitShaders";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    uint32_t   graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::Device device                   = vk::su::createDevice( physicalDevice, graphicsQueueFamilyIndex );

    /* VULKAN_HPP_KEY_START */

    glslang::InitializeProcess();

    std::vector<unsigned int> vertexShaderSPV;
#if !defined( NDEBUG )
    bool ok =
#endif
      vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C, vertexShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo vertexShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), vertexShaderSPV );
    vk::ShaderModule           vertexShaderModule = device.createShaderModule( vertexShaderModuleCreateInfo );

    std::vector<unsigned int> fragmentShaderSPV;
#if !defined( NDEBUG )
    ok =
#endif
      vk::su::GLSLtoSPV( vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C, fragmentShaderSPV );
    assert( ok );

    vk::ShaderModuleCreateInfo fragmentShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), fragmentShaderSPV );
    vk::ShaderModule           fragmentShaderModule = device.createShaderModule( fragmentShaderModuleCreateInfo );

    glslang::FinalizeProcess();

    device.destroyShaderModule( fragmentShaderModule );
    device.destroyShaderModule( vertexShaderModule );

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
