// SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : ArrayProxy
//                   Compile test on using vk::ArrayProxy

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <vulkan/vulkan.h>
import vulkan;
#else
#  include <iostream>
#  include <vulkan/vulkan_raii.hpp>
   VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  try
  {
    // test operator==() with vk-handle and nullptr
    vk::Instance instance;
    release_assert( instance == nullptr );
    release_assert( nullptr == instance );

    instance = vk::createInstance( {} );
    release_assert( instance != nullptr );
    release_assert( nullptr != instance );

    vk::Instance instance2;
    release_assert( instance != instance2 );
    release_assert( !( instance == instance2 ) );

    release_assert( instance2 < instance );

    instance2 = vk::createInstance( {} );
    release_assert( instance != instance2 );

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
    release_assert( physicalDevices.size() == 2 );
    if ( physicalDevices[0] == physicalDevices[1] )
    {
      std::cout << "fufu";
    }
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    std::exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    std::exit( -1 );
  }

  return 0;
}
