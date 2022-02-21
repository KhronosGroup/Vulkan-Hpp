// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : FormatTraits
//                     Compile test on using format traits functions

#include <vulkan/vulkan_format_traits.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  static_assert( vk::blockSize( vk::Format::eR4G4UnormPack8 ) == 1, "Wrong blocksize !" );

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

  static_assert( vk::isCompressed( vk::Format::eBc1RgbSrgbBlock ), "IsCompressed ?" );

  assert( vk::packed( format ) == 8 );

  if ( vk::componentsAreCompressed( format ) )
  {
    format = vk::Format::eBc1RgbUnormBlock;
  }

  constexpr vk::Format constFormat = vk::Format::eR4G4UnormPack8;
  static_assert( vk::componentBits( constFormat, 1 ) == 4, "Wrong component bits !" );
  assert( vk::componentCount( constFormat ) == 2 );

  if ( strcmp( vk::componentName( constFormat, 0 ), "R" ) == 0 )
  {
    format = constFormat;
  }

  assert( std::string( vk::componentNumericFormat( constFormat, 1 ) ) == "UNORM" );

  static_assert( vk::componentPlaneIndex( constFormat, 8 ) == 0, "Hoo" );

  if ( vk::planeCount( format ) != 1 )
  {
    format = constFormat;
  }

  assert( vk::planeCompatibleFormat( vk::Format::eG8B8R83Plane420Unorm, 2 ) == vk::Format::eR8Unorm );

  if ( vk::planeHeightDivisor( format, 0 ) == vk::planeWidthDivisor( format, 0 ) )
  {
    format = vk::Format::eG8B8R83Plane420Unorm;
  }

  return 0;
}
