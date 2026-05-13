// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : Reflection
//                   Compile-test for VULKAN_HPP_USE_REFLECT

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#define VULKAN_HPP_USE_REFLECT

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <iostream>
#  include <vulkan/vulkan.hpp>
#endif

static char const * AppName    = "StructureChain";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  vk::AabbPositionsKHR a;
  auto                 ra = a.reflect();

#if ( 14 <= VULKAN_HPP_CPP_VERSION )
  vk::StructureChain<vk::InstanceCreateInfo, vk::DebugReportCallbackCreateInfoEXT> chain;

  vk::InstanceCreateInfo ici;
  release_assert( chain.elementEquals<vk::InstanceCreateInfo>( ici ) );

  vk::ApplicationInfo applicationInfo( AppName, 1, EngineName, 1, vk::ApiVersion11 );
  ici.pApplicationInfo = &applicationInfo;
  release_assert( !chain.elementEquals<vk::InstanceCreateInfo>( ici ) );
#endif

  return 0;
}
