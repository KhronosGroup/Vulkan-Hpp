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

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_STRUCT_DEFAULT_INIT

#include <vulkan/vulkan.hpp>

class MyVulkanTest
{
public:
  MyVulkanTest();

private:
  vk::ApplicationInfo applicationInfo;
};

#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic error "-Wmissing-field-initializers"
#endif

MyVulkanTest::MyVulkanTest()
  : applicationInfo{ .pApplicationName   = "My Application",
                     .applicationVersion = VK_MAKE_VERSION( 0, 0, 1 ),
                     .pEngineName        = "My Engine",
                     .engineVersion      = VK_MAKE_VERSION( 0, 0, 1 ),
                     .apiVersion         = VK_API_VERSION_1_0 }
{
  this->applicationInfo = vk::ApplicationInfo{ .pApplicationName   = "My Application",
                                               .applicationVersion = VK_MAKE_VERSION( 0, 0, 1 ),
                                               .pEngineName        = "My Engine",
                                               .engineVersion      = VK_MAKE_VERSION( 0, 0, 1 ),
                                               .apiVersion         = VK_API_VERSION_1_0 };
}

int main( int /*argc*/, char ** /*argv*/ )
{
  char const * appName       = "DesignatedInitializers";
  uint32_t     appVersion    = 1;
  char const * engineName    = "Vulkan.hpp";
  uint32_t     engineVersion = 1;
  uint32_t     apiVersion    = 1;

  // default initialization is available in any case
  vk::ApplicationInfo ai0;

  // aggregate initialization: need to explicitly specify sType and pNext, as well as all the other members
  vk::ApplicationInfo ai1{ vk::StructureType::eApplicationInfo, nullptr, appName, appVersion, engineName, engineVersion, VK_API_VERSION_1_2 };

#if ( 20 <= VULKAN_HPP_CPP_VERSION )
  // no default initialization: all members except sType have to be specified; the order has to be respected
  vk::ApplicationInfo ai2 = { .pApplicationName = appName,
                              .applicationVersion = appVersion,
                              .pEngineName = engineName,
                              .engineVersion = engineVersion,
                              .apiVersion = apiVersion };

  vk::ApplicationInfo ai3{ .pApplicationName = appName,
                           .applicationVersion = appVersion,
                           .pEngineName = engineName,
                           .engineVersion = engineVersion,
                           .apiVersion = apiVersion };
#endif /*VULKAN_HPP_CPP_VERSION*/

  return 0;
}

#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
