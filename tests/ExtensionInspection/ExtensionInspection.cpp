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

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include <vulkan/vulkan_extension_inspection.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
#if ( 201907 <= __cpp_constexpr ) && ( !defined( __GNUC__ ) || ( 110400 < GCC_VERSION ) )
  static_assert( vk::isInstanceExtension( VK_KHR_SURFACE_EXTENSION_NAME ), "static_assert test failed" );
  static_assert( vk::isDeviceExtension( VK_KHR_SWAPCHAIN_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::isDeprecatedExtension( VK_EXT_DEBUG_REPORT_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::getExtensionDeprecatedBy( VK_EXT_DEBUG_REPORT_EXTENSION_NAME ) == VK_EXT_DEBUG_UTILS_EXTENSION_NAME, "static assert test failed" );
  static_assert( vk::isPromotedExtension( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::getExtensionPromotedTo( VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME ) == "VK_VERSION_1_2", "static assert test failed" );
  static_assert( vk::isObsoletedExtension( VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME ), "static assert test failed" );
  static_assert( vk::getExtensionObsoletedBy( VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME ) == VK_KHR_MAINTENANCE_1_EXTENSION_NAME,
                 "static assert test failed" );
#endif

  std::set<std::string> const & instanceExtensions = vk::getInstanceExtensions();
  assert( instanceExtensions.find( VK_KHR_SURFACE_EXTENSION_NAME ) != instanceExtensions.end() );

  std::set<std::string> const & deviceExtensions = vk::getDeviceExtensions();
  assert( deviceExtensions.find( VK_KHR_SWAPCHAIN_EXTENSION_NAME ) != deviceExtensions.end() );

  std::map<std::string, std::vector<std::vector<std::string>>> depends = vk::getExtensionDepends( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
  assert( ( depends.size() == 1 ) && ( depends.begin()->first == "VK_VERSION_1_0" ) && ( depends.begin()->second.size() == 1 ) &&
          ( depends.begin()->second[0].size() == 1 ) && ( depends.begin()->second[0][0] == VK_KHR_SURFACE_EXTENSION_NAME ) );

  auto [available0, deps0] = vk::getExtensionDepends( "VK_VERSION_1_0", VK_KHR_SURFACE_EXTENSION_NAME );
  assert( available0 && deps0.empty() );

  auto [available1, deps1] = vk::getExtensionDepends( "VK_VERSION_1_0", VK_KHR_SWAPCHAIN_EXTENSION_NAME );
  assert( available1 && ( deps1.size() == 1 ) && ( deps1[0].size() == 1 ) && ( deps1[0][0] == VK_KHR_SURFACE_EXTENSION_NAME ) );

  auto [available2, deps2] = vk::getExtensionDepends( "VK_VERSION_1_1", VK_KHR_SWAPCHAIN_EXTENSION_NAME );
  assert( available2 && ( deps2.size() == 1 ) && ( deps2[0].size() == 1 ) && ( deps2[0][0] == VK_KHR_SURFACE_EXTENSION_NAME ) );

  auto [available3, deps3] = vk::getExtensionDepends( "VK_VERSION_1_1", VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME );
  assert( !available3 );

  auto [available4, deps4] = vk::getExtensionDepends( "VK_VERSION_1_3", VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME );
  assert( available4 && deps4.empty() );

  std::map<std::string, std::string> const & deprecatedExtensions = vk::getDeprecatedExtensions();
  auto                                       deprecatedIt         = deprecatedExtensions.find( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
  assert( ( deprecatedIt != deprecatedExtensions.end() ) && ( deprecatedIt->second == VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) );

  std::map<std::string, std::string> const & obsoletedExtensions = vk::getObsoletedExtensions();
  auto                                       obsoletedIt         = obsoletedExtensions.find( VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME );
  assert( ( obsoletedIt != obsoletedExtensions.end() ) && ( obsoletedIt->second == VK_KHR_MAINTENANCE_1_EXTENSION_NAME ) );

  std::map<std::string, std::string> const & promotedExtensions = vk::getPromotedExtensions();
  auto                                       promotedIt         = promotedExtensions.find( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
  assert( ( promotedIt != promotedExtensions.end() ) && ( promotedIt->second == VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) );

  return 0;
}
