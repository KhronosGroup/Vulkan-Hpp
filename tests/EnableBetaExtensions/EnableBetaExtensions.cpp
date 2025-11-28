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
// VulkanHpp Tests : EnableBetaExtensions
//                   Compile test with VK_ENABLE_BETA_EXTENSIONS defined

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
# include <vulkan/vulkan.hpp>
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
  vk::PhysicalDevice physicalDevice;

  auto features = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDevicePortabilitySubsetFeaturesKHR>();
  auto properties = physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePortabilitySubsetPropertiesKHR>();
  return 0;
}
