// Copyright(c) 2024, NVIDIA CORPORATION. All rights reserved.
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

#ifdef VULKAN_HPP_USE_CXX_MODULE
  import vulkan;
#else
#  include <iostream>
#  include <vulkan/vulkan.hpp>
#endif

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
namespace vk {
  namespace detail {
    DispatchLoaderDynamic defaultDispatchLoaderDynamic;
  }
}
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance;
    instance = vk::createInstance( {} );
    void( instance != nullptr );

    vk::Instance anotherInstance = std::move( instance );
    void( instance == nullptr );
    void( anotherInstance != nullptr );

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
