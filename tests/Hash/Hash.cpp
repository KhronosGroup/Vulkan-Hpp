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

#include "vulkan/vulkan.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

static char const * AppName    = "Hash";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    vk::UniqueInstance  instance = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) );

    auto h1 = std::hash<vk::Instance>{}( *instance );
    auto h2 = std::hash<VkInstance>{}( static_cast<VkInstance>( *instance ) );
    assert( h1 == h2 );

    std::unordered_set<vk::Instance> uset;
    uset.insert( *instance );

    std::unordered_map<vk::Instance, size_t> umap;
    umap[*instance] = 1;

    vk::AabbPositionsKHR aabb0, aabb1;
    auto                 h3 = std::hash<vk::AabbPositionsKHR>{}( aabb0 );
    auto                 h4 = std::hash<vk::AabbPositionsKHR>{}( aabb1 );
    assert( h3 == h4 );

    aabb0.minX = 1.0f;
    auto h5    = std::hash<vk::AabbPositionsKHR>{}( aabb0 );
    assert( h3 != h5 );

    std::unordered_set<vk::AabbPositionsKHR> aabbSet;
    aabbSet.insert( aabb0 );

    std::unordered_map<vk::AabbPositionsKHR, size_t> aabbMap;
    aabbMap[aabb0] = 1;

    vk::AccelerationStructureInstanceKHR asi;
    asi.instanceCustomIndex                    = 1;
    asi.mask                                   = 2;
    asi.instanceShaderBindingTableRecordOffset = 3;
    asi.flags                                  = 4;
    auto h6                                    = std::hash<vk::AccelerationStructureInstanceKHR>{}( asi );
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
