// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : ArrayProxy
//                     Compile test on using vk::ArrayProxy

#undef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0

#include <iostream>
#include <vulkan/vulkan_raii.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // test operator==() with vk-handle and nullptr
    vk::Instance instance;
    assert( instance == nullptr );
    assert( nullptr == instance );

    instance = vk::createInstance( {} );
    assert( instance != nullptr );
    assert( nullptr != instance );

    vk::Instance instance2;
    assert( instance != instance2 );
    assert( !( instance == instance2 ) );

    instance2 = vk::createInstance( {} );
    assert( instance != instance2 );

    if ( instance == instance2 )
    {
      std::cout << "hoho";
    }
    if (instance != instance2)
    {
      std::cout << "haha";
    }
    if (instance < instance2)
    {
      std::cout << "hihi";
    }

    VkInstance vki = 0;
    if (vki == instance)
    {
      std::cout << "hoho";
    }

    vk::PhysicalDevice physicalDevice;
    if (physicalDevice == instance )
    {
      std::cout << "hoho";
    }

    instance.destroy();
    instance2.destroy();

    // test operator==() with vk::raii-handle and nullptr
    vk::raii::Context context;

    vk::raii::Instance i0 = context.createInstance( {} );
    vk::raii::Instance i1 = context.createInstance( {} );

    if ( i0 == i1 )
    {
      std::cout << "hahah";
    }
    if ( i0 != i1 )
    {
      std::cout << "hoho";
    }
    if ( i0 < i1 )
    {
      std::cout << "hihi";
    }

    if (i0 == nullptr)
    {
      std::cout << "kiki";
    }
    if (nullptr == i0)
    {
      std::cout << "kaka";
    }
    if (i0 != nullptr)
    {
      std::cout << "keke";
    }
    if (nullptr != i0)
    {
      std::cout << "kuku";
    }

    vk::raii::PhysicalDevices physicalDevices( i0 );
    assert( physicalDevices.size() == 2 );
    if ( physicalDevices[0] == physicalDevices[1] )
    {
      std::cout << "fufu";
    }
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
