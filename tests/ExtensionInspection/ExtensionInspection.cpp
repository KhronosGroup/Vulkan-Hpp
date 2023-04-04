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
// VulkanHpp Tests : ExtensionInspection
//                   Compile test on using extension inspection functions

#include <vulkan/vulkan_extension_inspection.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
#if ( 201907 <= __cpp_constexpr ) && ( !defined( __GNUC__ ) || ( 110300 < GCC_VERSION ) )
  static_assert( vk::isInstanceExtension( VK_KHR_SURFACE_EXTENSION_NAME ), "static_assert test failed" );
  static_assert( vk::isDeviceExtension( VK_KHR_SWAPCHAIN_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::isExtensionDeprecated( VK_EXT_DEBUG_REPORT_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::getExtensionDeprecatedBy( VK_EXT_DEBUG_REPORT_EXTENSION_NAME ) == VK_EXT_DEBUG_UTILS_EXTENSION_NAME, "static assert test failed" );
  static_assert( vk::isExtensionPromoted( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::getExtensionPromotedTo( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME ) == "VK_VERSION_1_2", "static assert test failed" );
#endif

  bool ok = vk::isInstanceExtension( VK_KHR_SURFACE_EXTENSION_NAME ) && vk::isDeviceExtension( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
  (void)ok;  // keep the compiler silent
  if ( vk::isExtensionDeprecated( VK_EXT_DEBUG_REPORT_EXTENSION_NAME ) )
  {
    std::string ext = vk::getExtensionDeprecatedBy( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
  }
  if ( vk::isExtensionPromoted( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME ) )
  {
    std::string ext = vk::getExtensionPromotedTo( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME );
  }

  return 0;
}
