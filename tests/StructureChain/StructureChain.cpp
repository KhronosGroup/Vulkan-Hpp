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
// VulkanHpp Tests : StructureChain
//                   Compile-test for StructureChains

#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "StructureChain";
static char const* EngineName = "Vulkan.hpp";

#if defined(_MSC_VER)
#pragma warning( disable : 4189 )
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::ApplicationInfo appInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_1);
    vk::UniqueInstance instance = vk::createInstanceUnique(vk::InstanceCreateInfo({}, &appInfo));
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    // some valid StructureChains
    vk::StructureChain<vk::PhysicalDeviceProperties2> sc0;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties> sc1;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance3Properties> sc2;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePushDescriptorPropertiesKHR> sc3;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties> sc4;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDevicePushDescriptorPropertiesKHR> sc6;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties, vk::PhysicalDevicePushDescriptorPropertiesKHR> sc7;

    // some not valid StructureChains
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties> x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties, vk::PhysicalDevicePushDescriptorPropertiesKHR> x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDevicePushDescriptorPropertiesKHR, vk::PhysicalDeviceMaintenance3Properties> x;
    //vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceIDProperties> x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceProperties2> x;

    vk::PhysicalDevice & pd = physicalDevices[0];

    // simple call, passing structures in
    vk::PhysicalDeviceFeatures2 pdf;
    pd.getFeatures2(&pdf);

    // simple calls, getting structure back
    vk::PhysicalDeviceFeatures2 a = pd.getFeatures2();
    vk::PhysicalDeviceFeatures2 b = pd.getFeatures2(vk::DispatchLoaderStatic());
    
    // complex calls, getting StructureChain back
    auto c = pd.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    vk::PhysicalDeviceFeatures2 & c0 = c.get<vk::PhysicalDeviceFeatures2>();
    vk::PhysicalDeviceVariablePointerFeatures & c1 = c.get<vk::PhysicalDeviceVariablePointerFeatures>();

    auto t0 = c.get<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    
    auto d = pd.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>(vk::DispatchLoaderStatic());
    vk::PhysicalDeviceFeatures2 & d0 = d.get<vk::PhysicalDeviceFeatures2>();
    vk::PhysicalDeviceVariablePointerFeatures & d1 = d.get<vk::PhysicalDeviceVariablePointerFeatures>();

    auto t1 = d.get<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();

    using StructureChain = vk::StructureChain<vk::QueueFamilyProperties2, vk::QueueFamilyCheckpointPropertiesNV>;
    using AllocatorType = std::vector<StructureChain>::allocator_type;
    auto qfd = pd.getQueueFamilyProperties2<StructureChain, AllocatorType>(vk::DispatchLoaderStatic());
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
