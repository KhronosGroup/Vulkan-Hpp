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
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "11_InitShaders";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], vk::su::findGraphicsQueueFamilyIndex(physicalDevices[0].getQueueFamilyProperties()));

    /* VULKAN_HPP_KEY_START */

    glslang::InitializeProcess();

    std::vector<unsigned int> vertexShaderSPV;
    bool ok = vk::su::GLSLtoSPV(vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C, vertexShaderSPV);
    assert(ok);

    vk::ShaderModuleCreateInfo vertexShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), vertexShaderSPV.size() * sizeof(unsigned int), vertexShaderSPV.data());
    vk::UniqueShaderModule vertexShaderModule = device->createShaderModuleUnique(vertexShaderModuleCreateInfo);

    std::vector<unsigned int> fragmentShaderSPV;
    ok = vk::su::GLSLtoSPV(vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C, fragmentShaderSPV);
    assert(ok);

    vk::ShaderModuleCreateInfo fragmentShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), fragmentShaderSPV.size() * sizeof(unsigned int), fragmentShaderSPV.data());
    vk::UniqueShaderModule fragmentShaderModule = device->createShaderModuleUnique(fragmentShaderModuleCreateInfo);

    glslang::FinalizeProcess();

    // Note: No need to explicitly destroy the ShaderModules, as the corresponding destroy
    // functions are called by the destructor of the UniqueShaderModule on leaving this scope.

    /* VULKAN_HPP_KEY_END */
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error err)
  {
    std::cout << "std::runtime_error: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
