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

#include "../utils/math.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>

#define GLM_FORCE_RADIANS
#pragma warning(disable:4201)   // disable warning C4201: nonstandard extension used: nameless struct/union; needed to get glm/detail/type_vec?.hpp without warnings
#include <glm/gtc/matrix_transform.hpp>

static char const* AppName = "09_InitDescriptorSet";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName);
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], vk::su::findGraphicsQueueFamilyIndex(physicalDevices[0].getQueueFamilyProperties()));

    vk::su::BufferData uniformBufferData(physicalDevices[0], device, sizeof(glm::mat4x4), vk::BufferUsageFlagBits::eUniformBuffer);
    vk::su::copyToDevice(device, uniformBufferData.deviceMemory, vk::su::createModelViewProjectionClipMatrix(vk::Extent2D(0, 0)));

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device, { {vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex} });

    /* VULKAN_HPP_KEY_START */

    // create a descriptor pool
    vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, 1);
    vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique(vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, 1, &poolSize));

    // allocate a descriptor set
    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(descriptorPool.get(), 1, &descriptorSetLayout.get()));

    vk::DescriptorBufferInfo descriptorBufferInfo(uniformBufferData.buffer.get(), 0, sizeof(glm::mat4x4));
    device->updateDescriptorSets(vk::WriteDescriptorSet(descriptorSets[0].get(), 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &descriptorBufferInfo), {});

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
