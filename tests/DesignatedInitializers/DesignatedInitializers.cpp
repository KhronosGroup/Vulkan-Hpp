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

#include <iostream>
#include <vulkan/vulkan.hpp>

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
  // to prevent a warning on unreferenced function vk::getDispatchLoaderStatic, use just one arbitrary vk-function
  (void)vk::enumerateInstanceVersion();

  char const * appName       = "DesignatedInitializers";
  uint32_t     appVersion    = 1;
  char const * engineName    = "Vulkan.hpp";
  uint32_t     engineVersion = 1;

  // default initialization is available in any case
  vk::ApplicationInfo ai0;

#if !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )
  // due to default initializers, any number of members can be initialized
  // most IDEs will probably help with the order of the members !
  vk::ApplicationInfo ai1( appName );
  vk::ApplicationInfo ai2( appName, appVersion );
  vk::ApplicationInfo ai3( appName, appVersion, engineName );
  vk::ApplicationInfo ai4( appName, appVersion, engineName, engineVersion );
  vk::ApplicationInfo ai5( appName, appVersion, engineName, engineVersion, VK_API_VERSION_1_2 );

  // a structure in namespace vk:: can be copied from the corresponding vulkan C-struct
  VkApplicationInfo   vai;
  vk::ApplicationInfo ai6( vai );

#else

  // aggregate initialization: need to explicitly specify sType and pNext, as well as all the other members !
  vk::ApplicationInfo ai1{ vk::StructureType::eApplicationInfo, nullptr, appName, appVersion, engineName, engineVersion, VK_API_VERSION_1_2 };

#  if ( 20 <= VULKAN_HPP_CPP_VERSION )
  // designated initializers are available with C++20

  // it's allowed, but not recommended to explicitly specify sType
  vk::ApplicationInfo ai2 = { .sType = vk::StructureType::eApplicationInfo };

  // any number of the members can be specified; the order has to be respected
  vk::ApplicationInfo ai3 = { .pApplicationName = appName };
  vk::ApplicationInfo ai4 = { .applicationVersion = 1, .engineVersion = 2 };

  vk::ApplicationInfo ai5{ .pEngineName = engineName };
  vk::ApplicationInfo ai6{ .pApplicationName = appName, .apiVersion = VK_API_VERSION_1_2 };
#  endif
#endif

  return 0;
}
