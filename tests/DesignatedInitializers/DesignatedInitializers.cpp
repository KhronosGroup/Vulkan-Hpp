// Copyright(c) 2020, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Tests : DesignatedInitializers
//                   Compile test on using designated initializers


#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <cstdint>
import vulkan;
#else
#  include <cstdint>
#  include <iostream>
#  include <vector>
#  include <vulkan/vulkan.hpp>
#endif

#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif

class MyVulkanTest
{
public:
  MyVulkanTest();

private:
  vk::ApplicationInfo applicationInfo;
};

MyVulkanTest::MyVulkanTest()
  : applicationInfo{ .pApplicationName   = "My Application",
                     .applicationVersion = vk::makeApiVersion( 0, 0, 1, 0 ),
                     .pEngineName        = "My Engine",
                     .engineVersion      = vk::makeApiVersion( 0, 0, 1, 0 ),
                     .apiVersion         = vk::ApiVersion10 }
{
  this->applicationInfo = vk::ApplicationInfo{ .pApplicationName   = "My Application",
                                               .applicationVersion = vk::makeApiVersion( 0, 0, 1, 0 ),
                                               .pEngineName        = "My Engine",
                                               .engineVersion      = vk::makeApiVersion( 0, 0, 1, 0 ),
                                               .apiVersion         = vk::ApiVersion10 };
}

int main( int /*argc*/, char ** /*argv*/ )
{
  char const * appName       = "DesignatedInitializers";
  uint32_t     appVersion    = 1;
  char const * engineName    = "Vulkan.hpp";
  uint32_t     engineVersion = 1;

  // default initialization is available in any case
  vk::ApplicationInfo ai0;

  // aggregate initialization: need to explicitly specify sType and pNext, as well as all the other members !
  vk::ApplicationInfo ai1{ vk::StructureType::eApplicationInfo, nullptr, appName, appVersion, engineName, engineVersion, vk::ApiVersion12 };

  // it's allowed, but not recommended to explicitly specify sType
  vk::ApplicationInfo ai2 = { .sType = vk::StructureType::eApplicationInfo };

  // any number of the members can be specified; the order has to be respected
  vk::ApplicationInfo ai3 = { .pApplicationName = appName };
  vk::ApplicationInfo ai4 = { .applicationVersion = 1, .engineVersion = 2 };

  vk::ApplicationInfo ai5{ .pEngineName = engineName };
  vk::ApplicationInfo ai6{ .pApplicationName = appName, .apiVersion = vk::ApiVersion12 };

  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
  std::vector<char const *>              extensions;
  vk::DeviceCreateInfo                   info_device{
                      .queueCreateInfoCount    = (uint32_t)queueCreateInfos.size(),
                      .pQueueCreateInfos       = queueCreateInfos.data(),
                      .enabledExtensionCount   = (uint32_t)extensions.size(),
                      .ppEnabledExtensionNames = extensions.data(),
  };

  return 0;
}
