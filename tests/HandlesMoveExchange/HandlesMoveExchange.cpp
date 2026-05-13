// SPDX-FileCopyrightText: 2024-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0


#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <iostream>
#  include <vulkan/vulkan.hpp>
   VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main()
{
  try
  {
    vk::Instance instance;
    instance = vk::createInstance( {} );
    release_assert( instance != nullptr );

    vk::Instance anotherInstance = std::move( instance );
    release_assert( instance == nullptr );
    release_assert( anotherInstance != nullptr );

    anotherInstance.destroy();
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
