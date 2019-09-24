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
// VulkanHpp Samples : DispatchLoaderDynamic
//                     Compile test on DispatchLoaderDynamic functions

#include "vulkan/vulkan.hpp"
#include <iostream>
#include <map>

static char const* AppName = "DispatchLoaderDynamic";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::DispatchLoaderDynamic dld0;
    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

  // empty DispatchLoaderDynamic, used for init calls later on
    vk::DispatchLoaderDynamic dld_boot(vkGetInstanceProcAddr);
    vk::Instance instance = vk::createInstance({}, nullptr, dld_boot);

    vk::DispatchLoaderDynamic dld1(instance, vkGetInstanceProcAddr);

    // compare to the empty dispatcher, and init the empty dispatcher the same way
    assert(memcmp(&dld0, &dld1, sizeof(vk::DispatchLoaderDynamic)) != 0);
    dld0.init(instance, vkGetInstanceProcAddr);
    assert(memcmp(&dld0, &dld1, sizeof(vk::DispatchLoaderDynamic)) == 0);

    // create a dispatcher, based on additional vkDevice/vkGetDeviceProcAddr
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices(dld1);

    assert(!physicalDevices.empty());

    vk::Device device = physicalDevices[0].createDevice({}, nullptr, dld1);

    // device specialization functions
    vk::DispatchLoaderDynamic dld2(instance, vkGetInstanceProcAddr, device);


    // compare to "simpler" dispatcher and make them equal
    assert(memcmp(&dld0, &dld2, sizeof(vk::DispatchLoaderDynamic)) != 0);
    dld0.init(instance, vkGetInstanceProcAddr, device);
    assert(memcmp(&dld0, &dld2, sizeof(vk::DispatchLoaderDynamic)) == 0);
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
