// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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

#include <iostream>
#include "vulkan/vulkan.hpp"

#define GLM_FORCE_RADIANS
#pragma warning(disable:4201)   // disable warning C4201: nonstandard extension used: nameless struct/union; needed to get glm/detail/type_vec?.hpp without warnings
#include <glm/gtc/matrix_transform.hpp>

static char const* AppName = "07_InitUniformBuffer";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char * /*argv[]*/)
{
  try
  {
    vk::ApplicationInfo appInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_1);
    vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo);
    vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    // determine a queueFamilyIndex that supports graphics
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();
    size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(),
      std::find_if(queueFamilyProperties.begin(),
        queueFamilyProperties.end(),
        [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));

    // create a device
    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo({}, static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority);
    vk::UniqueDevice device = physicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo({}, 1, &deviceQueueCreateInfo, 0, nullptr));

    /* VULKAN_HPP_KEY_START */

    glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4x4 view = glm::lookAt(glm::vec3(-5.0f, 3.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4x4 model = glm::mat4x4(1.0f);
    glm::mat4x4 clip = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 0.5f, 1.0f);   // vulkan clip space has inverted y and half z !
    glm::mat4x4 mvpc = clip * projection * view * model;

    vk::UniqueBuffer uniformDataBuffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), sizeof(mvpc), vk::BufferUsageFlagBits::eUniformBuffer));

    vk::MemoryRequirements memoryRequirements = device->getBufferMemoryRequirements(uniformDataBuffer.get());
    uint32_t typeBits = memoryRequirements.memoryTypeBits;

    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevices[0].getMemoryProperties();
    vk::MemoryPropertyFlags requirementsMask = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    uint32_t typeIndex = uint32_t(~0);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
      if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask))
      {
        typeIndex = i;
        break;
      }
      typeBits >>= 1;
    }
    assert(typeIndex != ~0);
    vk::UniqueDeviceMemory uniformDataMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, typeIndex));

    uint8_t* pData = static_cast<uint8_t*>(device->mapMemory(uniformDataMemory.get(), 0, memoryRequirements.size));
    memcpy(pData, &mvpc, sizeof(mvpc));
    device->unmapMemory(uniformDataMemory.get());

    device->bindBufferMemory(uniformDataBuffer.get(), uniformDataMemory.get(), 0);

    // Note: No need to explicitly destroy the memory or the buffer, as the corresponding destroy function is
    // called by the destructor of the UniqueMemory or UniqueBuffer, respectively, on leaving this scope.

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
