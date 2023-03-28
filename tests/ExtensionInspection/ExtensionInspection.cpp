// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file vk::Format::except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, vk::Format::either vk::Format::express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// VulkanHpp Samples : FormatTraits
//                     Compile test on using format traits functions

#include <vulkan/vulkan_extension_inspection.hpp>

#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )  // local variable is initialized but not referenced
#elif defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  bool ok = vk::isInstanceExtension( VK_KHR_SURFACE_EXTENSION_NAME ) && vk::isDeviceExtension( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

  return 0;
}
