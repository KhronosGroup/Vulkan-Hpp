// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : FormatTraits
//                   Compile test on using format traits functions

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <cstring>
#  include <iostream>
#  include <map>
#  include <set>
#  include <vulkan/vulkan_format_traits.hpp>
#endif


int main()
{
#if VULKAN_HPP_CPP_VERSION < 14 || defined( VULKAN_HPP_USE_CXX_MODULE )
  release_assert( vk::blockSize( vk::Format::eR4G4UnormPack8 ) == 1 );
#else
  static_assert( vk::blockSize( vk::Format::eR4G4UnormPack8 ) == 1, "Wrong blocksize !" );
#endif

  vk::Format format = vk::Format::eR4G4UnormPack8;
  if ( vk::texelsPerBlock( format ) == 1 )
  {
    format = vk::Format::eR4G4B4A4UnormPack16;
  }

  release_assert( vk::blockExtent( vk::Format::eBc1RgbUnormBlock )[2] == 1 );

  format = vk::Format::eBc1RgbUnormBlock;
  if ( std::string( vk::compressionScheme( format ) ) == "BC" )
  {
    format = vk::Format::eR4G4UnormPack8;
  }

#if VULKAN_HPP_CPP_VERSION < 14 || defined( VULKAN_HPP_USE_CXX_MODULE )
  release_assert( vk::isCompressed( vk::Format::eBc1RgbSrgbBlock ) );
#else
  static_assert( vk::isCompressed( vk::Format::eBc1RgbSrgbBlock ), "IsCompressed ?" );
#endif

  release_assert( vk::packed( format ) == 8 );

  if ( vk::componentsAreCompressed( format ) )
  {
    format = vk::Format::eBc1RgbUnormBlock;
  }

  constexpr vk::Format constFormat = vk::Format::eR4G4UnormPack8;
#if VULKAN_HPP_CPP_VERSION < 14 || defined( VULKAN_HPP_USE_CXX_MODULE )
  release_assert( vk::componentBits( constFormat, 1 ) == 4 );
#else
  static_assert( vk::componentBits( constFormat, 1 ) == 4, "Wrong component bits !" );
#endif
  release_assert( vk::componentCount( constFormat ) == 2 );

  if ( std::strcmp( vk::componentName( constFormat, 0 ), "R" ) == 0 )
  {
    format = constFormat;
  }

  release_assert( std::string( vk::componentNumericFormat( constFormat, 1 ) ) == "UNORM" );

#if VULKAN_HPP_CPP_VERSION < 14 || defined( VULKAN_HPP_USE_CXX_MODULE )
  release_assert( vk::componentPlaneIndex( constFormat, 8 ) == 0 );
#else
  static_assert( vk::componentPlaneIndex( constFormat, 8 ) == 0, "Hoo" );
#endif

  if ( vk::planeCount( format ) != 1 )
  {
    format = constFormat;
  }

  release_assert( vk::planeCompatibleFormat( vk::Format::eG8B8R83Plane420Unorm, 2 ) == vk::Format::eR8Unorm );

  if ( vk::planeHeightDivisor( format, 0 ) == vk::planeWidthDivisor( format, 0 ) )
  {
    format = vk::Format::eG8B8R83Plane420Unorm;
  }

  auto const & allFormats = vk::getAllFormats();

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
#if !defined( NDEBUG )
      for ( auto const & rf : cf.second )
      {
        release_assert( vk::blockSize( lf ) == vk::blockSize( rf ) );
        release_assert( vk::blockExtent( lf ) == vk::blockExtent( rf ) );
        release_assert( vk::texelsPerBlock( lf ) == vk::texelsPerBlock( rf ) );
      }
#endif
      std::cout << "\t" << vk::to_string( lf ) << "\n";
    }
    std::cout << std::endl;
  }

#if VULKAN_HPP_CPP_VERSION < 14 || defined( VULKAN_HPP_USE_CXX_MODULE )
  release_assert( vk::hasRedComponent( vk::Format::eR4G4B4A4UnormPack16 ) );
  release_assert( vk::hasGreenComponent( vk::Format::eR4G4B4A4UnormPack16 ) );
  release_assert( vk::hasBlueComponent( vk::Format::eR4G4B4A4UnormPack16 ) );
  release_assert( vk::hasAlphaComponent( vk::Format::eR4G4B4A4UnormPack16 ) );
  release_assert( vk::isColor( vk::Format::eR4G4B4A4UnormPack16 ) );
#else
  static_assert( vk::hasRedComponent( vk::Format::eR4G4B4A4UnormPack16 ), "Hoo" );
  static_assert( vk::hasGreenComponent( vk::Format::eR4G4B4A4UnormPack16 ), "Hoo" );
  static_assert( vk::hasBlueComponent( vk::Format::eR4G4B4A4UnormPack16 ), "Hoo" );
  static_assert( vk::hasAlphaComponent( vk::Format::eR4G4B4A4UnormPack16 ), "Hoo" );
  static_assert( vk::isColor( vk::Format::eR4G4B4A4UnormPack16 ), "Hoo" );
#endif

  return 0;
}
