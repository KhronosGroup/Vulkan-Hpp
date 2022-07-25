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
// VulkanHpp Samples : SurfaceFormats
//                     Get surface formats.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

static char const * AppName    = "SurfaceFormats";
static char const * EngineName = "Vulkan.hpp";

#if defined( VULKAN_HPP_NO_TO_STRING )
namespace local
{
  std::string to_string( vk::ColorSpaceKHR value )
  {
    switch ( value )
    {
      case vk::ColorSpaceKHR::eSrgbNonlinear: return "SrgbNonlinear";
      case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT: return "DisplayP3NonlinearEXT";
      case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT: return "ExtendedSrgbLinearEXT";
      case vk::ColorSpaceKHR::eDisplayP3LinearEXT: return "DisplayP3LinearEXT";
      case vk::ColorSpaceKHR::eDciP3NonlinearEXT: return "DciP3NonlinearEXT";
      case vk::ColorSpaceKHR::eBt709LinearEXT: return "Bt709LinearEXT";
      case vk::ColorSpaceKHR::eBt709NonlinearEXT: return "Bt709NonlinearEXT";
      case vk::ColorSpaceKHR::eBt2020LinearEXT: return "Bt2020LinearEXT";
      case vk::ColorSpaceKHR::eHdr10St2084EXT: return "Hdr10St2084EXT";
      case vk::ColorSpaceKHR::eDolbyvisionEXT: return "DolbyvisionEXT";
      case vk::ColorSpaceKHR::eHdr10HlgEXT: return "Hdr10HlgEXT";
      case vk::ColorSpaceKHR::eAdobergbLinearEXT: return "AdobergbLinearEXT";
      case vk::ColorSpaceKHR::eAdobergbNonlinearEXT: return "AdobergbNonlinearEXT";
      case vk::ColorSpaceKHR::ePassThroughEXT: return "PassThroughEXT";
      case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: return "ExtendedSrgbNonlinearEXT";
      case vk::ColorSpaceKHR::eDisplayNativeAMD: return "DisplayNativeAMD";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  std::string to_string( vk::Format value )
  {
    switch ( value )
    {
      case vk::Format::eUndefined: return "Undefined";
      case vk::Format::eR4G4UnormPack8: return "R4G4UnormPack8";
      case vk::Format::eR4G4B4A4UnormPack16: return "R4G4B4A4UnormPack16";
      case vk::Format::eB4G4R4A4UnormPack16: return "B4G4R4A4UnormPack16";
      case vk::Format::eR5G6B5UnormPack16: return "R5G6B5UnormPack16";
      case vk::Format::eB5G6R5UnormPack16: return "B5G6R5UnormPack16";
      case vk::Format::eR5G5B5A1UnormPack16: return "R5G5B5A1UnormPack16";
      case vk::Format::eB5G5R5A1UnormPack16: return "B5G5R5A1UnormPack16";
      case vk::Format::eA1R5G5B5UnormPack16: return "A1R5G5B5UnormPack16";
      case vk::Format::eR8Unorm: return "R8Unorm";
      case vk::Format::eR8Snorm: return "R8Snorm";
      case vk::Format::eR8Uscaled: return "R8Uscaled";
      case vk::Format::eR8Sscaled: return "R8Sscaled";
      case vk::Format::eR8Uint: return "R8Uint";
      case vk::Format::eR8Sint: return "R8Sint";
      case vk::Format::eR8Srgb: return "R8Srgb";
      case vk::Format::eR8G8Unorm: return "R8G8Unorm";
      case vk::Format::eR8G8Snorm: return "R8G8Snorm";
      case vk::Format::eR8G8Uscaled: return "R8G8Uscaled";
      case vk::Format::eR8G8Sscaled: return "R8G8Sscaled";
      case vk::Format::eR8G8Uint: return "R8G8Uint";
      case vk::Format::eR8G8Sint: return "R8G8Sint";
      case vk::Format::eR8G8Srgb: return "R8G8Srgb";
      case vk::Format::eR8G8B8Unorm: return "R8G8B8Unorm";
      case vk::Format::eR8G8B8Snorm: return "R8G8B8Snorm";
      case vk::Format::eR8G8B8Uscaled: return "R8G8B8Uscaled";
      case vk::Format::eR8G8B8Sscaled: return "R8G8B8Sscaled";
      case vk::Format::eR8G8B8Uint: return "R8G8B8Uint";
      case vk::Format::eR8G8B8Sint: return "R8G8B8Sint";
      case vk::Format::eR8G8B8Srgb: return "R8G8B8Srgb";
      case vk::Format::eB8G8R8Unorm: return "B8G8R8Unorm";
      case vk::Format::eB8G8R8Snorm: return "B8G8R8Snorm";
      case vk::Format::eB8G8R8Uscaled: return "B8G8R8Uscaled";
      case vk::Format::eB8G8R8Sscaled: return "B8G8R8Sscaled";
      case vk::Format::eB8G8R8Uint: return "B8G8R8Uint";
      case vk::Format::eB8G8R8Sint: return "B8G8R8Sint";
      case vk::Format::eB8G8R8Srgb: return "B8G8R8Srgb";
      case vk::Format::eR8G8B8A8Unorm: return "R8G8B8A8Unorm";
      case vk::Format::eR8G8B8A8Snorm: return "R8G8B8A8Snorm";
      case vk::Format::eR8G8B8A8Uscaled: return "R8G8B8A8Uscaled";
      case vk::Format::eR8G8B8A8Sscaled: return "R8G8B8A8Sscaled";
      case vk::Format::eR8G8B8A8Uint: return "R8G8B8A8Uint";
      case vk::Format::eR8G8B8A8Sint: return "R8G8B8A8Sint";
      case vk::Format::eR8G8B8A8Srgb: return "R8G8B8A8Srgb";
      case vk::Format::eB8G8R8A8Unorm: return "B8G8R8A8Unorm";
      case vk::Format::eB8G8R8A8Snorm: return "B8G8R8A8Snorm";
      case vk::Format::eB8G8R8A8Uscaled: return "B8G8R8A8Uscaled";
      case vk::Format::eB8G8R8A8Sscaled: return "B8G8R8A8Sscaled";
      case vk::Format::eB8G8R8A8Uint: return "B8G8R8A8Uint";
      case vk::Format::eB8G8R8A8Sint: return "B8G8R8A8Sint";
      case vk::Format::eB8G8R8A8Srgb: return "B8G8R8A8Srgb";
      case vk::Format::eA8B8G8R8UnormPack32: return "A8B8G8R8UnormPack32";
      case vk::Format::eA8B8G8R8SnormPack32: return "A8B8G8R8SnormPack32";
      case vk::Format::eA8B8G8R8UscaledPack32: return "A8B8G8R8UscaledPack32";
      case vk::Format::eA8B8G8R8SscaledPack32: return "A8B8G8R8SscaledPack32";
      case vk::Format::eA8B8G8R8UintPack32: return "A8B8G8R8UintPack32";
      case vk::Format::eA8B8G8R8SintPack32: return "A8B8G8R8SintPack32";
      case vk::Format::eA8B8G8R8SrgbPack32: return "A8B8G8R8SrgbPack32";
      case vk::Format::eA2R10G10B10UnormPack32: return "A2R10G10B10UnormPack32";
      case vk::Format::eA2R10G10B10SnormPack32: return "A2R10G10B10SnormPack32";
      case vk::Format::eA2R10G10B10UscaledPack32: return "A2R10G10B10UscaledPack32";
      case vk::Format::eA2R10G10B10SscaledPack32: return "A2R10G10B10SscaledPack32";
      case vk::Format::eA2R10G10B10UintPack32: return "A2R10G10B10UintPack32";
      case vk::Format::eA2R10G10B10SintPack32: return "A2R10G10B10SintPack32";
      case vk::Format::eA2B10G10R10UnormPack32: return "A2B10G10R10UnormPack32";
      case vk::Format::eA2B10G10R10SnormPack32: return "A2B10G10R10SnormPack32";
      case vk::Format::eA2B10G10R10UscaledPack32: return "A2B10G10R10UscaledPack32";
      case vk::Format::eA2B10G10R10SscaledPack32: return "A2B10G10R10SscaledPack32";
      case vk::Format::eA2B10G10R10UintPack32: return "A2B10G10R10UintPack32";
      case vk::Format::eA2B10G10R10SintPack32: return "A2B10G10R10SintPack32";
      case vk::Format::eR16Unorm: return "R16Unorm";
      case vk::Format::eR16Snorm: return "R16Snorm";
      case vk::Format::eR16Uscaled: return "R16Uscaled";
      case vk::Format::eR16Sscaled: return "R16Sscaled";
      case vk::Format::eR16Uint: return "R16Uint";
      case vk::Format::eR16Sint: return "R16Sint";
      case vk::Format::eR16Sfloat: return "R16Sfloat";
      case vk::Format::eR16G16Unorm: return "R16G16Unorm";
      case vk::Format::eR16G16Snorm: return "R16G16Snorm";
      case vk::Format::eR16G16Uscaled: return "R16G16Uscaled";
      case vk::Format::eR16G16Sscaled: return "R16G16Sscaled";
      case vk::Format::eR16G16Uint: return "R16G16Uint";
      case vk::Format::eR16G16Sint: return "R16G16Sint";
      case vk::Format::eR16G16Sfloat: return "R16G16Sfloat";
      case vk::Format::eR16G16B16Unorm: return "R16G16B16Unorm";
      case vk::Format::eR16G16B16Snorm: return "R16G16B16Snorm";
      case vk::Format::eR16G16B16Uscaled: return "R16G16B16Uscaled";
      case vk::Format::eR16G16B16Sscaled: return "R16G16B16Sscaled";
      case vk::Format::eR16G16B16Uint: return "R16G16B16Uint";
      case vk::Format::eR16G16B16Sint: return "R16G16B16Sint";
      case vk::Format::eR16G16B16Sfloat: return "R16G16B16Sfloat";
      case vk::Format::eR16G16B16A16Unorm: return "R16G16B16A16Unorm";
      case vk::Format::eR16G16B16A16Snorm: return "R16G16B16A16Snorm";
      case vk::Format::eR16G16B16A16Uscaled: return "R16G16B16A16Uscaled";
      case vk::Format::eR16G16B16A16Sscaled: return "R16G16B16A16Sscaled";
      case vk::Format::eR16G16B16A16Uint: return "R16G16B16A16Uint";
      case vk::Format::eR16G16B16A16Sint: return "R16G16B16A16Sint";
      case vk::Format::eR16G16B16A16Sfloat: return "R16G16B16A16Sfloat";
      case vk::Format::eR32Uint: return "R32Uint";
      case vk::Format::eR32Sint: return "R32Sint";
      case vk::Format::eR32Sfloat: return "R32Sfloat";
      case vk::Format::eR32G32Uint: return "R32G32Uint";
      case vk::Format::eR32G32Sint: return "R32G32Sint";
      case vk::Format::eR32G32Sfloat: return "R32G32Sfloat";
      case vk::Format::eR32G32B32Uint: return "R32G32B32Uint";
      case vk::Format::eR32G32B32Sint: return "R32G32B32Sint";
      case vk::Format::eR32G32B32Sfloat: return "R32G32B32Sfloat";
      case vk::Format::eR32G32B32A32Uint: return "R32G32B32A32Uint";
      case vk::Format::eR32G32B32A32Sint: return "R32G32B32A32Sint";
      case vk::Format::eR32G32B32A32Sfloat: return "R32G32B32A32Sfloat";
      case vk::Format::eR64Uint: return "R64Uint";
      case vk::Format::eR64Sint: return "R64Sint";
      case vk::Format::eR64Sfloat: return "R64Sfloat";
      case vk::Format::eR64G64Uint: return "R64G64Uint";
      case vk::Format::eR64G64Sint: return "R64G64Sint";
      case vk::Format::eR64G64Sfloat: return "R64G64Sfloat";
      case vk::Format::eR64G64B64Uint: return "R64G64B64Uint";
      case vk::Format::eR64G64B64Sint: return "R64G64B64Sint";
      case vk::Format::eR64G64B64Sfloat: return "R64G64B64Sfloat";
      case vk::Format::eR64G64B64A64Uint: return "R64G64B64A64Uint";
      case vk::Format::eR64G64B64A64Sint: return "R64G64B64A64Sint";
      case vk::Format::eR64G64B64A64Sfloat: return "R64G64B64A64Sfloat";
      case vk::Format::eB10G11R11UfloatPack32: return "B10G11R11UfloatPack32";
      case vk::Format::eE5B9G9R9UfloatPack32: return "E5B9G9R9UfloatPack32";
      case vk::Format::eD16Unorm: return "D16Unorm";
      case vk::Format::eX8D24UnormPack32: return "X8D24UnormPack32";
      case vk::Format::eD32Sfloat: return "D32Sfloat";
      case vk::Format::eS8Uint: return "S8Uint";
      case vk::Format::eD16UnormS8Uint: return "D16UnormS8Uint";
      case vk::Format::eD24UnormS8Uint: return "D24UnormS8Uint";
      case vk::Format::eD32SfloatS8Uint: return "D32SfloatS8Uint";
      case vk::Format::eBc1RgbUnormBlock: return "Bc1RgbUnormBlock";
      case vk::Format::eBc1RgbSrgbBlock: return "Bc1RgbSrgbBlock";
      case vk::Format::eBc1RgbaUnormBlock: return "Bc1RgbaUnormBlock";
      case vk::Format::eBc1RgbaSrgbBlock: return "Bc1RgbaSrgbBlock";
      case vk::Format::eBc2UnormBlock: return "Bc2UnormBlock";
      case vk::Format::eBc2SrgbBlock: return "Bc2SrgbBlock";
      case vk::Format::eBc3UnormBlock: return "Bc3UnormBlock";
      case vk::Format::eBc3SrgbBlock: return "Bc3SrgbBlock";
      case vk::Format::eBc4UnormBlock: return "Bc4UnormBlock";
      case vk::Format::eBc4SnormBlock: return "Bc4SnormBlock";
      case vk::Format::eBc5UnormBlock: return "Bc5UnormBlock";
      case vk::Format::eBc5SnormBlock: return "Bc5SnormBlock";
      case vk::Format::eBc6HUfloatBlock: return "Bc6HUfloatBlock";
      case vk::Format::eBc6HSfloatBlock: return "Bc6HSfloatBlock";
      case vk::Format::eBc7UnormBlock: return "Bc7UnormBlock";
      case vk::Format::eBc7SrgbBlock: return "Bc7SrgbBlock";
      case vk::Format::eEtc2R8G8B8UnormBlock: return "Etc2R8G8B8UnormBlock";
      case vk::Format::eEtc2R8G8B8SrgbBlock: return "Etc2R8G8B8SrgbBlock";
      case vk::Format::eEtc2R8G8B8A1UnormBlock: return "Etc2R8G8B8A1UnormBlock";
      case vk::Format::eEtc2R8G8B8A1SrgbBlock: return "Etc2R8G8B8A1SrgbBlock";
      case vk::Format::eEtc2R8G8B8A8UnormBlock: return "Etc2R8G8B8A8UnormBlock";
      case vk::Format::eEtc2R8G8B8A8SrgbBlock: return "Etc2R8G8B8A8SrgbBlock";
      case vk::Format::eEacR11UnormBlock: return "EacR11UnormBlock";
      case vk::Format::eEacR11SnormBlock: return "EacR11SnormBlock";
      case vk::Format::eEacR11G11UnormBlock: return "EacR11G11UnormBlock";
      case vk::Format::eEacR11G11SnormBlock: return "EacR11G11SnormBlock";
      case vk::Format::eAstc4x4UnormBlock: return "Astc4x4UnormBlock";
      case vk::Format::eAstc4x4SrgbBlock: return "Astc4x4SrgbBlock";
      case vk::Format::eAstc5x4UnormBlock: return "Astc5x4UnormBlock";
      case vk::Format::eAstc5x4SrgbBlock: return "Astc5x4SrgbBlock";
      case vk::Format::eAstc5x5UnormBlock: return "Astc5x5UnormBlock";
      case vk::Format::eAstc5x5SrgbBlock: return "Astc5x5SrgbBlock";
      case vk::Format::eAstc6x5UnormBlock: return "Astc6x5UnormBlock";
      case vk::Format::eAstc6x5SrgbBlock: return "Astc6x5SrgbBlock";
      case vk::Format::eAstc6x6UnormBlock: return "Astc6x6UnormBlock";
      case vk::Format::eAstc6x6SrgbBlock: return "Astc6x6SrgbBlock";
      case vk::Format::eAstc8x5UnormBlock: return "Astc8x5UnormBlock";
      case vk::Format::eAstc8x5SrgbBlock: return "Astc8x5SrgbBlock";
      case vk::Format::eAstc8x6UnormBlock: return "Astc8x6UnormBlock";
      case vk::Format::eAstc8x6SrgbBlock: return "Astc8x6SrgbBlock";
      case vk::Format::eAstc8x8UnormBlock: return "Astc8x8UnormBlock";
      case vk::Format::eAstc8x8SrgbBlock: return "Astc8x8SrgbBlock";
      case vk::Format::eAstc10x5UnormBlock: return "Astc10x5UnormBlock";
      case vk::Format::eAstc10x5SrgbBlock: return "Astc10x5SrgbBlock";
      case vk::Format::eAstc10x6UnormBlock: return "Astc10x6UnormBlock";
      case vk::Format::eAstc10x6SrgbBlock: return "Astc10x6SrgbBlock";
      case vk::Format::eAstc10x8UnormBlock: return "Astc10x8UnormBlock";
      case vk::Format::eAstc10x8SrgbBlock: return "Astc10x8SrgbBlock";
      case vk::Format::eAstc10x10UnormBlock: return "Astc10x10UnormBlock";
      case vk::Format::eAstc10x10SrgbBlock: return "Astc10x10SrgbBlock";
      case vk::Format::eAstc12x10UnormBlock: return "Astc12x10UnormBlock";
      case vk::Format::eAstc12x10SrgbBlock: return "Astc12x10SrgbBlock";
      case vk::Format::eAstc12x12UnormBlock: return "Astc12x12UnormBlock";
      case vk::Format::eAstc12x12SrgbBlock: return "Astc12x12SrgbBlock";
      case vk::Format::eG8B8G8R8422Unorm: return "G8B8G8R8422Unorm";
      case vk::Format::eB8G8R8G8422Unorm: return "B8G8R8G8422Unorm";
      case vk::Format::eG8B8R83Plane420Unorm: return "G8B8R83Plane420Unorm";
      case vk::Format::eG8B8R82Plane420Unorm: return "G8B8R82Plane420Unorm";
      case vk::Format::eG8B8R83Plane422Unorm: return "G8B8R83Plane422Unorm";
      case vk::Format::eG8B8R82Plane422Unorm: return "G8B8R82Plane422Unorm";
      case vk::Format::eG8B8R83Plane444Unorm: return "G8B8R83Plane444Unorm";
      case vk::Format::eR10X6UnormPack16: return "R10X6UnormPack16";
      case vk::Format::eR10X6G10X6Unorm2Pack16: return "R10X6G10X6Unorm2Pack16";
      case vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16: return "R10X6G10X6B10X6A10X6Unorm4Pack16";
      case vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16: return "G10X6B10X6G10X6R10X6422Unorm4Pack16";
      case vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16: return "B10X6G10X6R10X6G10X6422Unorm4Pack16";
      case vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16: return "G10X6B10X6R10X63Plane420Unorm3Pack16";
      case vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16: return "G10X6B10X6R10X62Plane420Unorm3Pack16";
      case vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16: return "G10X6B10X6R10X63Plane422Unorm3Pack16";
      case vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16: return "G10X6B10X6R10X62Plane422Unorm3Pack16";
      case vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16: return "G10X6B10X6R10X63Plane444Unorm3Pack16";
      case vk::Format::eR12X4UnormPack16: return "R12X4UnormPack16";
      case vk::Format::eR12X4G12X4Unorm2Pack16: return "R12X4G12X4Unorm2Pack16";
      case vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16: return "R12X4G12X4B12X4A12X4Unorm4Pack16";
      case vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16: return "G12X4B12X4G12X4R12X4422Unorm4Pack16";
      case vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16: return "B12X4G12X4R12X4G12X4422Unorm4Pack16";
      case vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16: return "G12X4B12X4R12X43Plane420Unorm3Pack16";
      case vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16: return "G12X4B12X4R12X42Plane420Unorm3Pack16";
      case vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16: return "G12X4B12X4R12X43Plane422Unorm3Pack16";
      case vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16: return "G12X4B12X4R12X42Plane422Unorm3Pack16";
      case vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16: return "G12X4B12X4R12X43Plane444Unorm3Pack16";
      case vk::Format::eG16B16G16R16422Unorm: return "G16B16G16R16422Unorm";
      case vk::Format::eB16G16R16G16422Unorm: return "B16G16R16G16422Unorm";
      case vk::Format::eG16B16R163Plane420Unorm: return "G16B16R163Plane420Unorm";
      case vk::Format::eG16B16R162Plane420Unorm: return "G16B16R162Plane420Unorm";
      case vk::Format::eG16B16R163Plane422Unorm: return "G16B16R163Plane422Unorm";
      case vk::Format::eG16B16R162Plane422Unorm: return "G16B16R162Plane422Unorm";
      case vk::Format::eG16B16R163Plane444Unorm: return "G16B16R163Plane444Unorm";
      case vk::Format::eG8B8R82Plane444Unorm: return "G8B8R82Plane444Unorm";
      case vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16: return "G10X6B10X6R10X62Plane444Unorm3Pack16";
      case vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16: return "G12X4B12X4R12X42Plane444Unorm3Pack16";
      case vk::Format::eG16B16R162Plane444Unorm: return "G16B16R162Plane444Unorm";
      case vk::Format::eA4R4G4B4UnormPack16: return "A4R4G4B4UnormPack16";
      case vk::Format::eA4B4G4R4UnormPack16: return "A4B4G4R4UnormPack16";
      case vk::Format::eAstc4x4SfloatBlock: return "Astc4x4SfloatBlock";
      case vk::Format::eAstc5x4SfloatBlock: return "Astc5x4SfloatBlock";
      case vk::Format::eAstc5x5SfloatBlock: return "Astc5x5SfloatBlock";
      case vk::Format::eAstc6x5SfloatBlock: return "Astc6x5SfloatBlock";
      case vk::Format::eAstc6x6SfloatBlock: return "Astc6x6SfloatBlock";
      case vk::Format::eAstc8x5SfloatBlock: return "Astc8x5SfloatBlock";
      case vk::Format::eAstc8x6SfloatBlock: return "Astc8x6SfloatBlock";
      case vk::Format::eAstc8x8SfloatBlock: return "Astc8x8SfloatBlock";
      case vk::Format::eAstc10x5SfloatBlock: return "Astc10x5SfloatBlock";
      case vk::Format::eAstc10x6SfloatBlock: return "Astc10x6SfloatBlock";
      case vk::Format::eAstc10x8SfloatBlock: return "Astc10x8SfloatBlock";
      case vk::Format::eAstc10x10SfloatBlock: return "Astc10x10SfloatBlock";
      case vk::Format::eAstc12x10SfloatBlock: return "Astc12x10SfloatBlock";
      case vk::Format::eAstc12x12SfloatBlock: return "Astc12x12SfloatBlock";
      case vk::Format::ePvrtc12BppUnormBlockIMG: return "Pvrtc12BppUnormBlockIMG";
      case vk::Format::ePvrtc14BppUnormBlockIMG: return "Pvrtc14BppUnormBlockIMG";
      case vk::Format::ePvrtc22BppUnormBlockIMG: return "Pvrtc22BppUnormBlockIMG";
      case vk::Format::ePvrtc24BppUnormBlockIMG: return "Pvrtc24BppUnormBlockIMG";
      case vk::Format::ePvrtc12BppSrgbBlockIMG: return "Pvrtc12BppSrgbBlockIMG";
      case vk::Format::ePvrtc14BppSrgbBlockIMG: return "Pvrtc14BppSrgbBlockIMG";
      case vk::Format::ePvrtc22BppSrgbBlockIMG: return "Pvrtc22BppSrgbBlockIMG";
      case vk::Format::ePvrtc24BppSrgbBlockIMG: return "Pvrtc24BppSrgbBlockIMG";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }
}  // namespace local
using local::to_string;
#else
using vk::to_string;
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    vk::raii::PhysicalDevices physicalDevices( instance );

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      std::cout << "PhysicalDevice " << i << "\n";
      std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevices[i].getSurfaceFormatsKHR( *surfaceData.surface );
      for ( size_t j = 0; j < surfaceFormats.size(); j++ )
      {
        std::cout << std::string( "\t" ) << "Format " << j << "\n";
        std::cout << std::string( "\t\t" ) << "colorSpace  = " << to_string( surfaceFormats[j].colorSpace ) << "\n";
        std::cout << std::string( "\t\t" ) << "format      = " << to_string( surfaceFormats[j].format ) << "\n";
        std::cout << "\n";
      }
    }

    /* VULKAN_KEY_END */
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
