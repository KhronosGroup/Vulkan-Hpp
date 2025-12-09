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
#  pragma warning( disable : 4996 )
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif


#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <cassert>
import vulkan;
#else
#  include <set>
#  include <map>
#  include <vector>
#  include <string>
#  include <vulkan/vulkan_extension_inspection.hpp>
#endif


int main( int /*argc*/, char ** /*argv*/ )
{
#if ( 201907 <= __cpp_constexpr ) && ( !defined( __GNUC__ ) || ( 110400 < GCC_VERSION ) )
  static_assert( vk::isInstanceExtension( vk::KHRSurfaceExtensionName ), "static_assert test failed" );
  static_assert( vk::isDeviceExtension( vk::KHRSwapchainExtensionName ), "static assert test failed" );
  static_assert( vk::isDeprecatedExtension( vk::EXTDebugReportExtensionName ), "static assert test failed" );
  static_assert( vk::getExtensionDeprecatedBy( vk::EXTDebugReportExtensionName ) == vk::EXTDebugUtilsExtensionName, "static assert test failed" );
  static_assert( vk::isPromotedExtension( vk::KHRSamplerMirrorClampToEdgeExtensionName ), "static assert test failed" );
  static_assert( vk::getExtensionPromotedTo( vk::KHRSamplerMirrorClampToEdgeExtensionName ) == "VK_VERSION_1_2", "static assert test failed" );
  static_assert( vk::isObsoletedExtension( vk::AMDNegativeViewportHeightExtensionName ), "static assert test failed" );
  static_assert( vk::getExtensionObsoletedBy( vk::AMDNegativeViewportHeightExtensionName ) == vk::KHRMaintenance1ExtensionName,
                 "static assert test failed" );
#endif

  std::set<std::string> const & instanceExtensions = vk::getInstanceExtensions();
  assert( instanceExtensions.find( vk::KHRSurfaceExtensionName ) != instanceExtensions.end() );

  std::set<std::string> const & deviceExtensions = vk::getDeviceExtensions();
  assert( deviceExtensions.find( vk::KHRSwapchainExtensionName ) != deviceExtensions.end() );

  std::map<std::string, std::vector<std::vector<std::string>>> depends = vk::getExtensionDepends( vk::KHRSwapchainExtensionName );
  assert( ( depends.size() == 1 ) && ( depends.begin()->first == "VK_VERSION_1_0" ) && ( depends.begin()->second.size() == 1 ) &&
          ( depends.begin()->second[0].size() == 1 ) && ( depends.begin()->second[0][0] == vk::KHRSurfaceExtensionName ) );

  auto [available0, deps0] = vk::getExtensionDepends( "VK_VERSION_1_0", vk::KHRSurfaceExtensionName );
  assert( available0 && deps0.empty() );

  auto [available1, deps1] = vk::getExtensionDepends( "VK_VERSION_1_0", vk::KHRSwapchainExtensionName );
  assert( available1 && ( deps1.size() == 1 ) && ( deps1[0].size() == 1 ) && ( deps1[0][0] == vk::KHRSurfaceExtensionName ) );

  auto [available2, deps2] = vk::getExtensionDepends( "VK_VERSION_1_1", vk::KHRSwapchainExtensionName );
  assert( available2 && ( deps2.size() == 1 ) && ( deps2[0].size() == 1 ) && ( deps2[0][0] == vk::KHRSurfaceExtensionName ) );

  auto [available3, deps3] = vk::getExtensionDepends( "VK_VERSION_1_1", vk::EXTShaderTileImageExtensionName );
  assert( !available3 );

  auto [available4, deps4] = vk::getExtensionDepends( "VK_VERSION_1_3", vk::EXTShaderTileImageExtensionName );
  assert( available4 && ( deps4.size() == 1 ) && deps4[0].empty() );

  std::map<std::string, std::string> const & deprecatedExtensions = vk::getDeprecatedExtensions();
  auto                                       deprecatedIt         = deprecatedExtensions.find( vk::EXTDebugReportExtensionName );
  assert( ( deprecatedIt != deprecatedExtensions.end() ) && ( deprecatedIt->second == vk::EXTDebugUtilsExtensionName ) );

  std::map<std::string, std::string> const & obsoletedExtensions = vk::getObsoletedExtensions();
  auto                                       obsoletedIt         = obsoletedExtensions.find( vk::AMDNegativeViewportHeightExtensionName );
  assert( ( obsoletedIt != obsoletedExtensions.end() ) && ( obsoletedIt->second == vk::KHRMaintenance1ExtensionName ) );

  std::map<std::string, std::string> const & promotedExtensions = vk::getPromotedExtensions();
  auto                                       promotedIt         = promotedExtensions.find( vk::EXTDebugMarkerExtensionName );
  assert( ( promotedIt != promotedExtensions.end() ) && ( promotedIt->second == vk::EXTDebugUtilsExtensionName ) );

  return 0;
}
