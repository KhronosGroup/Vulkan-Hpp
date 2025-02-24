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
// VulkanHpp Samples : Hash
//                     Compile test on using std::hash on handles

#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )  // local variable is initialized but not referenced
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "vulkan/vulkan_hash.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

static char const * AppName    = "Hash";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    {
      vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
      vk::UniqueInstance  instance = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) );

      auto h1 = std::hash<vk::Instance>{}( *instance );
      auto h2 = std::hash<VkInstance>{}( *instance );
      assert( h1 == h2 );

      std::unordered_set<vk::Instance> uset;
      uset.insert( *instance );

      std::unordered_map<vk::Instance, size_t> umap;
      umap[*instance] = 1;

      vk::FormatFeatureFlags fff;
      auto                   hf = std::hash<vk::FormatFeatureFlags>{}( fff );
    }

#if 14 <= VULKAN_HPP_CPP_VERSION
    {
      vk::AabbPositionsKHR aabb0, aabb1;
      auto                 h1 = std::hash<vk::AabbPositionsKHR>{}( aabb0 );
      auto                 h2 = std::hash<vk::AabbPositionsKHR>{}( aabb1 );
      assert( h1 == h2 );

      aabb0.minX = 1.0f;
      auto h3    = std::hash<vk::AabbPositionsKHR>{}( aabb0 );
      assert( h1 != h3 );

      std::unordered_set<vk::AabbPositionsKHR> aabbSet;
      aabbSet.insert( aabb0 );

      std::unordered_map<vk::AabbPositionsKHR, size_t> aabbMap;
      aabbMap[aabb0] = 1;

      vk::AccelerationStructureBuildSizesInfoKHR asbsi;
      auto                                       h4 = std::hash<vk::AccelerationStructureBuildSizesInfoKHR>{}( asbsi );
    }

    {
      std::string         name1 = AppName;
      std::string         name2 = AppName;
      vk::ApplicationInfo appInfo1( name1.c_str(), 1, EngineName, 1, VK_API_VERSION_1_1 );
      vk::ApplicationInfo appInfo2( name2.c_str(), 1, EngineName, 1, VK_API_VERSION_1_1 );
      auto                h1 = std::hash<vk::ApplicationInfo>{}( appInfo1 );
      auto                h2 = std::hash<vk::ApplicationInfo>{}( appInfo2 );
      assert( h1 == h2 );
      assert( appInfo1 == appInfo2 );
#  if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
      assert( appInfo1 <= appInfo2 );
#  endif
    }

    {
      std::vector<const char *> enabledLayers1 = { "Layer1", "Layer2", "Layer3" };
      auto                      enabledLayers2 = enabledLayers1;

      vk::ApplicationInfo    appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
      vk::InstanceCreateInfo info1( {}, &appInfo, static_cast<uint32_t>( enabledLayers1.size() ), enabledLayers1.data() );
      vk::InstanceCreateInfo info2( {}, &appInfo, static_cast<uint32_t>( enabledLayers2.size() ), enabledLayers2.data() );
      auto                   h1 = std::hash<vk::InstanceCreateInfo>{}( info1 );
      auto                   h2 = std::hash<vk::InstanceCreateInfo>{}( info2 );
      assert( h1 == h2 );
      assert( info1 == info2 );
#  if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
      assert( info1 <= info2 );
#  endif
    }
#endif
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  return 0;
}
