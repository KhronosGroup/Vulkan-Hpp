// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : SurfaceCapabilities
//                     Get surface capabilities.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

static char const * AppName    = "SurfaceCapabilities";
static char const * EngineName = "Vulkan.hpp";

#if defined( VULKAN_HPP_NO_TO_STRING )
namespace local
{
  std::string to_string( vk::CompositeAlphaFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::CompositeAlphaFlagBitsKHR::eOpaque )
      result += "Opaque | ";
    if ( value & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied )
      result += "PreMultiplied | ";
    if ( value & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied )
      result += "PostMultiplied | ";
    if ( value & vk::CompositeAlphaFlagBitsKHR::eInherit )
      result += "Inherit | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  std::string to_string( vk::ImageUsageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::ImageUsageFlagBits::eTransferSrc )
      result += "TransferSrc | ";
    if ( value & vk::ImageUsageFlagBits::eTransferDst )
      result += "TransferDst | ";
    if ( value & vk::ImageUsageFlagBits::eSampled )
      result += "Sampled | ";
    if ( value & vk::ImageUsageFlagBits::eStorage )
      result += "Storage | ";
    if ( value & vk::ImageUsageFlagBits::eColorAttachment )
      result += "ColorAttachment | ";
    if ( value & vk::ImageUsageFlagBits::eDepthStencilAttachment )
      result += "DepthStencilAttachment | ";
    if ( value & vk::ImageUsageFlagBits::eTransientAttachment )
      result += "TransientAttachment | ";
    if ( value & vk::ImageUsageFlagBits::eInputAttachment )
      result += "InputAttachment | ";
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
    if ( value & vk::ImageUsageFlagBits::eVideoDecodeDstKHR )
      result += "VideoDecodeDstKHR | ";
    if ( value & vk::ImageUsageFlagBits::eVideoDecodeSrcKHR )
      result += "VideoDecodeSrcKHR | ";
    if ( value & vk::ImageUsageFlagBits::eVideoDecodeDpbKHR )
      result += "VideoDecodeDpbKHR | ";
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/
    if ( value & vk::ImageUsageFlagBits::eFragmentDensityMapEXT )
      result += "FragmentDensityMapEXT | ";
    if ( value & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
    if ( value & vk::ImageUsageFlagBits::eVideoEncodeDstKHR )
      result += "VideoEncodeDstKHR | ";
    if ( value & vk::ImageUsageFlagBits::eVideoEncodeSrcKHR )
      result += "VideoEncodeSrcKHR | ";
    if ( value & vk::ImageUsageFlagBits::eVideoEncodeDpbKHR )
      result += "VideoEncodeDpbKHR | ";
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/
    if ( value & vk::ImageUsageFlagBits::eInvocationMaskHUAWEI )
      result += "InvocationMaskHUAWEI | ";
    if ( value & vk::ImageUsageFlagBits::eSampleWeightQCOM )
      result += "SampleWeightQCOM | ";
    if ( value & vk::ImageUsageFlagBits::eSampleBlockMatchQCOM )
      result += "SampleBlockMatchQCOM | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  std::string to_string( vk::SurfaceTransformFlagBitsKHR value )
  {
    switch ( value )
    {
      case vk::SurfaceTransformFlagBitsKHR::eIdentity: return "Identity";
      case vk::SurfaceTransformFlagBitsKHR::eRotate90: return "Rotate90";
      case vk::SurfaceTransformFlagBitsKHR::eRotate180: return "Rotate180";
      case vk::SurfaceTransformFlagBitsKHR::eRotate270: return "Rotate270";
      case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror: return "HorizontalMirror";
      case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90: return "HorizontalMirrorRotate90";
      case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180: return "HorizontalMirrorRotate180";
      case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270: return "HorizontalMirrorRotate270";
      case vk::SurfaceTransformFlagBitsKHR::eInherit: return "Inherit";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  std::string to_string( vk::SurfaceTransformFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::SurfaceTransformFlagBitsKHR::eIdentity )
      result += "Identity | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eRotate90 )
      result += "Rotate90 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eRotate180 )
      result += "Rotate180 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eRotate270 )
      result += "Rotate270 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror )
      result += "HorizontalMirror | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90 )
      result += "HorizontalMirrorRotate90 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180 )
      result += "HorizontalMirrorRotate180 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270 )
      result += "HorizontalMirrorRotate270 | ";
    if ( value & vk::SurfaceTransformFlagBitsKHR::eInherit )
      result += "Inherit | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }
}  // namespace local
using local::to_string;
#else
using vk::to_string;
#endif

void cout( vk::SurfaceCapabilitiesKHR const & surfaceCapabilities )
{
  std::cout << std::string( "\t" ) << "Capabilities:\n";
  std::cout << std::string( "\t\t" ) << "currentExtent           = " << surfaceCapabilities.currentExtent.width << " x "
            << surfaceCapabilities.currentExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "currentTransform        = " << to_string( surfaceCapabilities.currentTransform ) << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageArrayLayers     = " << surfaceCapabilities.maxImageArrayLayers << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageCount           = " << surfaceCapabilities.maxImageCount << "\n";
  std::cout << std::string( "\t\t" ) << "maxImageExtent          = " << surfaceCapabilities.maxImageExtent.width << " x "
            << surfaceCapabilities.maxImageExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "minImageCount           = " << surfaceCapabilities.minImageCount << "\n";
  std::cout << std::string( "\t\t" ) << "minImageExtent          = " << surfaceCapabilities.minImageExtent.width << " x "
            << surfaceCapabilities.minImageExtent.height << "\n";
  std::cout << std::string( "\t\t" ) << "supportedCompositeAlpha = " << to_string( surfaceCapabilities.supportedCompositeAlpha ) << "\n";
  std::cout << std::string( "\t\t" ) << "supportedTransforms     = " << to_string( surfaceCapabilities.supportedTransforms ) << "\n";
  std::cout << std::string( "\t\t" ) << "supportedUsageFlags     = " << to_string( surfaceCapabilities.supportedUsageFlags ) << "\n";
  std::cout << "\n";
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // need to initialize the dynamic dispatcher before the very first vulkan call
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    static vk::DynamicLoader  dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
#endif

    std::vector<vk::ExtensionProperties> instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();
    auto                                 propertyIterator =
      std::find_if( instanceExtensionProperties.begin(),
                    instanceExtensionProperties.end(),
                    []( vk::ExtensionProperties const & ep ) { return strcmp( ep.extensionName, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME ) == 0; } );
    bool supportsGetSurfaceCapabilities2 = ( propertyIterator != instanceExtensionProperties.end() );

    std::vector<std::string> extensions = vk::su::getInstanceExtensions();
    if ( supportsGetSurfaceCapabilities2 )
    {
      extensions.push_back( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME );
    }

    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, extensions );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      // some properties are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";
      if ( supportsGetSurfaceCapabilities2 )
      {
        auto surfaceCapabilities2 = physicalDevices[i]
                                      .getSurfaceCapabilities2KHR<vk::SurfaceCapabilities2KHR,
                                                                  vk::DisplayNativeHdrSurfaceCapabilitiesAMD,
                                                                  vk::SharedPresentSurfaceCapabilitiesKHR,
                                                                  vk::SurfaceCapabilitiesFullScreenExclusiveEXT,
                                                                  vk::SurfaceProtectedCapabilitiesKHR>( surfaceData.surface );

        vk::SurfaceCapabilitiesKHR const & surfaceCapabilities = surfaceCapabilities2.get<vk::SurfaceCapabilities2KHR>().surfaceCapabilities;
        cout( surfaceCapabilities );

        if ( vk::su::contains( extensionProperties, "VK_AMD_display_native_hdr" ) )
        {
          vk::DisplayNativeHdrSurfaceCapabilitiesAMD displayNativeHdrSurfaceCapabilities =
            surfaceCapabilities2.get<vk::DisplayNativeHdrSurfaceCapabilitiesAMD>();
          std::cout << std::string( "\t" ) << "DisplayNativeHdrSurfaceCapabilitiesAMD:\n";
          std::cout << std::string( "\t\t" ) << "localDimmingSupport = " << !!displayNativeHdrSurfaceCapabilities.localDimmingSupport << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_KHR_shared_presentable_image" ) )
        {
          vk::SharedPresentSurfaceCapabilitiesKHR sharedPresentSurfaceCapabilities = surfaceCapabilities2.get<vk::SharedPresentSurfaceCapabilitiesKHR>();
          std::cout << std::string( "\t" ) << "SharedPresentSurfaceCapabilitiesKHR:\n";
          std::cout << std::string( "\t\t" )
                    << "sharedPresentSupportedUsageFlags  = " << to_string( sharedPresentSurfaceCapabilities.sharedPresentSupportedUsageFlags ) << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_EXT_full_screen_exclusive" ) )
        {
          vk::SurfaceCapabilitiesFullScreenExclusiveEXT surfaceCapabilitiesFullScreenExclusive =
            surfaceCapabilities2.get<vk::SurfaceCapabilitiesFullScreenExclusiveEXT>();
          std::cout << std::string( "\t" ) << "SurfaceCapabilitiesFullScreenExclusiveEXT:\n";
          std::cout << std::string( "\t\t" ) << "fullScreenExclusiveSupported  = " << !!surfaceCapabilitiesFullScreenExclusive.fullScreenExclusiveSupported
                    << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_KHR_surface_protected_capabilities" ) )
        {
          vk::SurfaceProtectedCapabilitiesKHR surfaceProtectedCapabilities = surfaceCapabilities2.get<vk::SurfaceProtectedCapabilitiesKHR>();
          std::cout << std::string( "\t" ) << "SurfaceProtectedCapabilitiesKHR:\n";
          std::cout << std::string( "\t\t" ) << "supportsProtected  = " << !!surfaceProtectedCapabilities.supportsProtected << "\n";
          std::cout << "\n";
        }
      }
      else
      {
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevices[i].getSurfaceCapabilitiesKHR( surfaceData.surface );
        cout( surfaceCapabilities );
      }
    }

    /* VULKAN_KEY_END */

    instance.destroySurfaceKHR( surfaceData.surface );
#if !defined( NDEBUG )
    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
    instance.destroy();
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }
  return 0;
}
