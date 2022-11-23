// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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

#include <iostream>
#include <map>
#include <set>
#include <vulkan/vulkan_format_traits.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
#if VULKAN_HPP_CPP_VERSION < 14
  assert( vk::blockSize( vk::Format::eR4G4UnormPack8 ) == 1 );
#else
  static_assert( vk::blockSize( vk::Format::eR4G4UnormPack8 ) == 1, "Wrong blocksize !" );
#endif

  vk::Format format = vk::Format::eR4G4UnormPack8;
  if ( vk::texelsPerBlock( format ) == 1 )
  {
    format = vk::Format::eR4G4B4A4UnormPack16;
  }

  assert( vk::blockExtent( vk::Format::eBc1RgbUnormBlock )[2] == 1 );

  format = vk::Format::eBc1RgbUnormBlock;
  if ( std::string( vk::compressionScheme( format ) ) == "BC" )
  {
    format = vk::Format::eR4G4UnormPack8;
  }

#if VULKAN_HPP_CPP_VERSION < 14
  assert( vk::isCompressed( vk::Format::eBc1RgbSrgbBlock ) );
#else
  static_assert( vk::isCompressed( vk::Format::eBc1RgbSrgbBlock ), "IsCompressed ?" );
#endif

  assert( vk::packed( format ) == 8 );

  if ( vk::componentsAreCompressed( format ) )
  {
    format = vk::Format::eBc1RgbUnormBlock;
  }

  constexpr vk::Format constFormat = vk::Format::eR4G4UnormPack8;
#if VULKAN_HPP_CPP_VERSION < 14
  assert( vk::componentBits( constFormat, 1 ) == 4 );
#else
  static_assert( vk::componentBits( constFormat, 1 ) == 4, "Wrong component bits !" );
#endif
  assert( vk::componentCount( constFormat ) == 2 );

  if ( strcmp( vk::componentName( constFormat, 0 ), "R" ) == 0 )
  {
    format = constFormat;
  }

  assert( std::string( vk::componentNumericFormat( constFormat, 1 ) ) == "UNORM" );

#if VULKAN_HPP_CPP_VERSION < 14
  assert( vk::componentPlaneIndex( constFormat, 8 ) == 0 );
#else
  static_assert( vk::componentPlaneIndex( constFormat, 8 ) == 0, "Hoo" );
#endif

  if ( vk::planeCount( format ) != 1 )
  {
    format = constFormat;
  }

  assert( vk::planeCompatibleFormat( vk::Format::eG8B8R83Plane420Unorm, 2 ) == vk::Format::eR8Unorm );

  if ( vk::planeHeightDivisor( format, 0 ) == vk::planeWidthDivisor( format, 0 ) )
  {
    format = vk::Format::eG8B8R83Plane420Unorm;
  }

  std::vector<vk::Format> allFormats = { vk::Format::eR4G4UnormPack8,
                                         vk::Format::eR4G4B4A4UnormPack16,
                                         vk::Format::eB4G4R4A4UnormPack16,
                                         vk::Format::eR5G6B5UnormPack16,
                                         vk::Format::eB5G6R5UnormPack16,
                                         vk::Format::eR5G5B5A1UnormPack16,
                                         vk::Format::eB5G5R5A1UnormPack16,
                                         vk::Format::eA1R5G5B5UnormPack16,
                                         vk::Format::eR8Unorm,
                                         vk::Format::eR8Snorm,
                                         vk::Format::eR8Uscaled,
                                         vk::Format::eR8Sscaled,
                                         vk::Format::eR8Uint,
                                         vk::Format::eR8Sint,
                                         vk::Format::eR8Srgb,
                                         vk::Format::eR8G8Unorm,
                                         vk::Format::eR8G8Snorm,
                                         vk::Format::eR8G8Uscaled,
                                         vk::Format::eR8G8Sscaled,
                                         vk::Format::eR8G8Uint,
                                         vk::Format::eR8G8Sint,
                                         vk::Format::eR8G8Srgb,
                                         vk::Format::eR8G8B8Unorm,
                                         vk::Format::eR8G8B8Snorm,
                                         vk::Format::eR8G8B8Uscaled,
                                         vk::Format::eR8G8B8Sscaled,
                                         vk::Format::eR8G8B8Uint,
                                         vk::Format::eR8G8B8Sint,
                                         vk::Format::eR8G8B8Srgb,
                                         vk::Format::eB8G8R8Unorm,
                                         vk::Format::eB8G8R8Snorm,
                                         vk::Format::eB8G8R8Uscaled,
                                         vk::Format::eB8G8R8Sscaled,
                                         vk::Format::eB8G8R8Uint,
                                         vk::Format::eB8G8R8Sint,
                                         vk::Format::eB8G8R8Srgb,
                                         vk::Format::eR8G8B8A8Unorm,
                                         vk::Format::eR8G8B8A8Snorm,
                                         vk::Format::eR8G8B8A8Uscaled,
                                         vk::Format::eR8G8B8A8Sscaled,
                                         vk::Format::eR8G8B8A8Uint,
                                         vk::Format::eR8G8B8A8Sint,
                                         vk::Format::eR8G8B8A8Srgb,
                                         vk::Format::eB8G8R8A8Unorm,
                                         vk::Format::eB8G8R8A8Snorm,
                                         vk::Format::eB8G8R8A8Uscaled,
                                         vk::Format::eB8G8R8A8Sscaled,
                                         vk::Format::eB8G8R8A8Uint,
                                         vk::Format::eB8G8R8A8Sint,
                                         vk::Format::eB8G8R8A8Srgb,
                                         vk::Format::eA8B8G8R8UnormPack32,
                                         vk::Format::eA8B8G8R8SnormPack32,
                                         vk::Format::eA8B8G8R8UscaledPack32,
                                         vk::Format::eA8B8G8R8SscaledPack32,
                                         vk::Format::eA8B8G8R8UintPack32,
                                         vk::Format::eA8B8G8R8SintPack32,
                                         vk::Format::eA8B8G8R8SrgbPack32,
                                         vk::Format::eA2R10G10B10UnormPack32,
                                         vk::Format::eA2R10G10B10SnormPack32,
                                         vk::Format::eA2R10G10B10UscaledPack32,
                                         vk::Format::eA2R10G10B10SscaledPack32,
                                         vk::Format::eA2R10G10B10UintPack32,
                                         vk::Format::eA2R10G10B10SintPack32,
                                         vk::Format::eA2B10G10R10UnormPack32,
                                         vk::Format::eA2B10G10R10SnormPack32,
                                         vk::Format::eA2B10G10R10UscaledPack32,
                                         vk::Format::eA2B10G10R10SscaledPack32,
                                         vk::Format::eA2B10G10R10UintPack32,
                                         vk::Format::eA2B10G10R10SintPack32,
                                         vk::Format::eR16Unorm,
                                         vk::Format::eR16Snorm,
                                         vk::Format::eR16Uscaled,
                                         vk::Format::eR16Sscaled,
                                         vk::Format::eR16Uint,
                                         vk::Format::eR16Sint,
                                         vk::Format::eR16Sfloat,
                                         vk::Format::eR16G16Unorm,
                                         vk::Format::eR16G16Snorm,
                                         vk::Format::eR16G16Uscaled,
                                         vk::Format::eR16G16Sscaled,
                                         vk::Format::eR16G16Uint,
                                         vk::Format::eR16G16Sint,
                                         vk::Format::eR16G16Sfloat,
                                         vk::Format::eR16G16B16Unorm,
                                         vk::Format::eR16G16B16Snorm,
                                         vk::Format::eR16G16B16Uscaled,
                                         vk::Format::eR16G16B16Sscaled,
                                         vk::Format::eR16G16B16Uint,
                                         vk::Format::eR16G16B16Sint,
                                         vk::Format::eR16G16B16Sfloat,
                                         vk::Format::eR16G16B16A16Unorm,
                                         vk::Format::eR16G16B16A16Snorm,
                                         vk::Format::eR16G16B16A16Uscaled,
                                         vk::Format::eR16G16B16A16Sscaled,
                                         vk::Format::eR16G16B16A16Uint,
                                         vk::Format::eR16G16B16A16Sint,
                                         vk::Format::eR16G16B16A16Sfloat,
                                         vk::Format::eR32Uint,
                                         vk::Format::eR32Sint,
                                         vk::Format::eR32Sfloat,
                                         vk::Format::eR32G32Uint,
                                         vk::Format::eR32G32Sint,
                                         vk::Format::eR32G32Sfloat,
                                         vk::Format::eR32G32B32Uint,
                                         vk::Format::eR32G32B32Sint,
                                         vk::Format::eR32G32B32Sfloat,
                                         vk::Format::eR32G32B32A32Uint,
                                         vk::Format::eR32G32B32A32Sint,
                                         vk::Format::eR32G32B32A32Sfloat,
                                         vk::Format::eR64Uint,
                                         vk::Format::eR64Sint,
                                         vk::Format::eR64Sfloat,
                                         vk::Format::eR64G64Uint,
                                         vk::Format::eR64G64Sint,
                                         vk::Format::eR64G64Sfloat,
                                         vk::Format::eR64G64B64Uint,
                                         vk::Format::eR64G64B64Sint,
                                         vk::Format::eR64G64B64Sfloat,
                                         vk::Format::eR64G64B64A64Uint,
                                         vk::Format::eR64G64B64A64Sint,
                                         vk::Format::eR64G64B64A64Sfloat,
                                         vk::Format::eB10G11R11UfloatPack32,
                                         vk::Format::eE5B9G9R9UfloatPack32,
                                         vk::Format::eD16Unorm,
                                         vk::Format::eX8D24UnormPack32,
                                         vk::Format::eD32Sfloat,
                                         vk::Format::eS8Uint,
                                         vk::Format::eD16UnormS8Uint,
                                         vk::Format::eD24UnormS8Uint,
                                         vk::Format::eD32SfloatS8Uint,
                                         vk::Format::eBc1RgbUnormBlock,
                                         vk::Format::eBc1RgbSrgbBlock,
                                         vk::Format::eBc1RgbaUnormBlock,
                                         vk::Format::eBc1RgbaSrgbBlock,
                                         vk::Format::eBc2UnormBlock,
                                         vk::Format::eBc2SrgbBlock,
                                         vk::Format::eBc3UnormBlock,
                                         vk::Format::eBc3SrgbBlock,
                                         vk::Format::eBc4UnormBlock,
                                         vk::Format::eBc4SnormBlock,
                                         vk::Format::eBc5UnormBlock,
                                         vk::Format::eBc5SnormBlock,
                                         vk::Format::eBc6HUfloatBlock,
                                         vk::Format::eBc6HSfloatBlock,
                                         vk::Format::eBc7UnormBlock,
                                         vk::Format::eBc7SrgbBlock,
                                         vk::Format::eEtc2R8G8B8UnormBlock,
                                         vk::Format::eEtc2R8G8B8SrgbBlock,
                                         vk::Format::eEtc2R8G8B8A1UnormBlock,
                                         vk::Format::eEtc2R8G8B8A1SrgbBlock,
                                         vk::Format::eEtc2R8G8B8A8UnormBlock,
                                         vk::Format::eEtc2R8G8B8A8SrgbBlock,
                                         vk::Format::eEacR11UnormBlock,
                                         vk::Format::eEacR11SnormBlock,
                                         vk::Format::eEacR11G11UnormBlock,
                                         vk::Format::eEacR11G11SnormBlock,
                                         vk::Format::eAstc4x4UnormBlock,
                                         vk::Format::eAstc4x4SrgbBlock,
                                         vk::Format::eAstc5x4UnormBlock,
                                         vk::Format::eAstc5x4SrgbBlock,
                                         vk::Format::eAstc5x5UnormBlock,
                                         vk::Format::eAstc5x5SrgbBlock,
                                         vk::Format::eAstc6x5UnormBlock,
                                         vk::Format::eAstc6x5SrgbBlock,
                                         vk::Format::eAstc6x6UnormBlock,
                                         vk::Format::eAstc6x6SrgbBlock,
                                         vk::Format::eAstc8x5UnormBlock,
                                         vk::Format::eAstc8x5SrgbBlock,
                                         vk::Format::eAstc8x6UnormBlock,
                                         vk::Format::eAstc8x6SrgbBlock,
                                         vk::Format::eAstc8x8UnormBlock,
                                         vk::Format::eAstc8x8SrgbBlock,
                                         vk::Format::eAstc10x5UnormBlock,
                                         vk::Format::eAstc10x5SrgbBlock,
                                         vk::Format::eAstc10x6UnormBlock,
                                         vk::Format::eAstc10x6SrgbBlock,
                                         vk::Format::eAstc10x8UnormBlock,
                                         vk::Format::eAstc10x8SrgbBlock,
                                         vk::Format::eAstc10x10UnormBlock,
                                         vk::Format::eAstc10x10SrgbBlock,
                                         vk::Format::eAstc12x10UnormBlock,
                                         vk::Format::eAstc12x10SrgbBlock,
                                         vk::Format::eAstc12x12UnormBlock,
                                         vk::Format::eAstc12x12SrgbBlock,
                                         vk::Format::eG8B8G8R8422Unorm,
                                         vk::Format::eB8G8R8G8422Unorm,
                                         vk::Format::eG8B8R83Plane420Unorm,
                                         vk::Format::eG8B8R82Plane420Unorm,
                                         vk::Format::eG8B8R83Plane422Unorm,
                                         vk::Format::eG8B8R82Plane422Unorm,
                                         vk::Format::eG8B8R83Plane444Unorm,
                                         vk::Format::eR10X6UnormPack16,
                                         vk::Format::eR10X6G10X6Unorm2Pack16,
                                         vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16,
                                         vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16,
                                         vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16,
                                         vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16,
                                         vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16,
                                         vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16,
                                         vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16,
                                         vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16,
                                         vk::Format::eR12X4UnormPack16,
                                         vk::Format::eR12X4G12X4Unorm2Pack16,
                                         vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16,
                                         vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16,
                                         vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16,
                                         vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16,
                                         vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16,
                                         vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16,
                                         vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16,
                                         vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16,
                                         vk::Format::eG16B16G16R16422Unorm,
                                         vk::Format::eB16G16R16G16422Unorm,
                                         vk::Format::eG16B16R163Plane420Unorm,
                                         vk::Format::eG16B16R162Plane420Unorm,
                                         vk::Format::eG16B16R163Plane422Unorm,
                                         vk::Format::eG16B16R162Plane422Unorm,
                                         vk::Format::eG16B16R163Plane444Unorm,
                                         vk::Format::eG8B8R82Plane444Unorm,
                                         vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16,
                                         vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16,
                                         vk::Format::eG16B16R162Plane444Unorm,
                                         vk::Format::eA4R4G4B4UnormPack16,
                                         vk::Format::eA4B4G4R4UnormPack16,
                                         vk::Format::eAstc4x4SfloatBlock,
                                         vk::Format::eAstc5x4SfloatBlock,
                                         vk::Format::eAstc5x5SfloatBlock,
                                         vk::Format::eAstc6x5SfloatBlock,
                                         vk::Format::eAstc6x6SfloatBlock,
                                         vk::Format::eAstc8x5SfloatBlock,
                                         vk::Format::eAstc8x6SfloatBlock,
                                         vk::Format::eAstc8x8SfloatBlock,
                                         vk::Format::eAstc10x5SfloatBlock,
                                         vk::Format::eAstc10x6SfloatBlock,
                                         vk::Format::eAstc10x8SfloatBlock,
                                         vk::Format::eAstc10x10SfloatBlock,
                                         vk::Format::eAstc12x10SfloatBlock,
                                         vk::Format::eAstc12x12SfloatBlock,
                                         vk::Format::ePvrtc12BppUnormBlockIMG,
                                         vk::Format::ePvrtc14BppUnormBlockIMG,
                                         vk::Format::ePvrtc22BppUnormBlockIMG,
                                         vk::Format::ePvrtc24BppUnormBlockIMG,
                                         vk::Format::ePvrtc12BppSrgbBlockIMG,
                                         vk::Format::ePvrtc14BppSrgbBlockIMG,
                                         vk::Format::ePvrtc22BppSrgbBlockIMG,
                                         vk::Format::ePvrtc24BppSrgbBlockIMG,
                                         vk::Format::eR16G16S105NV,
                                         vk::Format::eA4B4G4R4UnormPack16EXT,
                                         vk::Format::eA4R4G4B4UnormPack16EXT,
                                         vk::Format::eAstc10x10SfloatBlockEXT,
                                         vk::Format::eAstc10x5SfloatBlockEXT,
                                         vk::Format::eAstc10x6SfloatBlockEXT,
                                         vk::Format::eAstc10x8SfloatBlockEXT,
                                         vk::Format::eAstc12x10SfloatBlockEXT,
                                         vk::Format::eAstc12x12SfloatBlockEXT,
                                         vk::Format::eAstc4x4SfloatBlockEXT,
                                         vk::Format::eAstc5x4SfloatBlockEXT,
                                         vk::Format::eAstc5x5SfloatBlockEXT,
                                         vk::Format::eAstc6x5SfloatBlockEXT,
                                         vk::Format::eAstc6x6SfloatBlockEXT,
                                         vk::Format::eAstc8x5SfloatBlockEXT,
                                         vk::Format::eAstc8x6SfloatBlockEXT,
                                         vk::Format::eAstc8x8SfloatBlockEXT,
                                         vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR,
                                         vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR,
                                         vk::Format::eB16G16R16G16422UnormKHR,
                                         vk::Format::eB8G8R8G8422UnormKHR,
                                         vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR,
                                         vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR,
                                         vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR,
                                         vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16EXT,
                                         vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR,
                                         vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR,
                                         vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR,
                                         vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR,
                                         vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR,
                                         vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR,
                                         vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16EXT,
                                         vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR,
                                         vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR,
                                         vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR,
                                         vk::Format::eG16B16G16R16422UnormKHR,
                                         vk::Format::eG16B16R162Plane420UnormKHR,
                                         vk::Format::eG16B16R162Plane422UnormKHR,
                                         vk::Format::eG16B16R162Plane444UnormEXT,
                                         vk::Format::eG16B16R163Plane420UnormKHR,
                                         vk::Format::eG16B16R163Plane422UnormKHR,
                                         vk::Format::eG16B16R163Plane444UnormKHR,
                                         vk::Format::eG8B8G8R8422UnormKHR,
                                         vk::Format::eG8B8R82Plane420UnormKHR,
                                         vk::Format::eG8B8R82Plane422UnormKHR,
                                         vk::Format::eG8B8R82Plane444UnormEXT,
                                         vk::Format::eG8B8R83Plane420UnormKHR,
                                         vk::Format::eG8B8R83Plane422UnormKHR,
                                         vk::Format::eG8B8R83Plane444UnormKHR,
                                         vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR,
                                         vk::Format::eR10X6G10X6Unorm2Pack16KHR,
                                         vk::Format::eR10X6UnormPack16KHR,
                                         vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR,
                                         vk::Format::eR12X4G12X4Unorm2Pack16KHR,
                                         vk::Format::eR12X4UnormPack16KHR };

  std::map<std::string, std::set<vk::Format>> compatibleFormats;
  for ( auto const & f : allFormats )
  {
    compatibleFormats[vk::compatibilityClass( f )].insert( f );
  }
  for ( auto const & cf : compatibleFormats )
  {
    std::cout << "class = " << cf.first << "\n";
    for ( auto const & lf : cf.second )
    {
#if !defined(NDEBUG)
      for ( auto const & rf : cf.second )
      {
        assert( vk::blockSize( lf ) == vk::blockSize( rf ) );
        assert( vk::blockExtent( lf ) == vk::blockExtent( rf ) );
        assert( vk::texelsPerBlock( lf ) == vk::texelsPerBlock( rf ) );
      }
#endif
      std::cout << "\t" << vk::to_string( lf ) << "\n";
    }
    std::cout << std::endl;
  }

  return 0;
}
