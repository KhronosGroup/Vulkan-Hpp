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
// VulkanHpp Samples : InitTexture
//                     Initialize texture

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../samples/utils/geometries.hpp"
#include "../samples/utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>

static char const * AppName    = "InitTexture";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::raii::Context  context;
    vk::raii::Instance instance = vk::raii::su::makeInstance( context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif
    vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices( instance ).front();

    vk::raii::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 50, 50 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::raii::Device device = vk::raii::su::makeDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::raii::CommandPool   commandPool   = vk::raii::CommandPool( device, { {}, graphicsAndPresentQueueFamilyIndex.first } );
    vk::raii::CommandBuffer commandBuffer = vk::raii::su::makeCommandBuffer( device, commandPool );

    vk::raii::Queue graphicsQueue( device, graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::raii::Queue presentQueue( device, graphicsAndPresentQueueFamilyIndex.second, 0 );

    /* VULKAN_KEY_START */

    vk::Format           format           = vk::Format::eR8G8B8A8Unorm;
    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( format );

    // See if we can use a linear tiled image for a texture, if not, we will need a staging buffer for the texture data
    bool needsStaging = !( formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage );

    // in order to get a clean desctruction sequence, instantiate the DeviceMemory for the image first
    vk::raii::DeviceMemory imageMemory( nullptr );

    vk::ImageCreateInfo imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         format,
                                         vk::Extent3D( surfaceData.extent, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         needsStaging ? vk::ImageTiling::eOptimal : vk::ImageTiling::eLinear,
                                         vk::ImageUsageFlagBits::eSampled | ( needsStaging ? vk::ImageUsageFlagBits::eTransferDst : vk::ImageUsageFlagBits() ),
                                         vk::SharingMode::eExclusive,
                                         {},
                                         needsStaging ? vk::ImageLayout::eUndefined : vk::ImageLayout::ePreinitialized );
    vk::raii::Image     image( device, imageCreateInfo );

    vk::MemoryRequirements memoryRequirements = image.getMemoryRequirements();
    uint32_t               memoryTypeIndex    = vk::su::findMemoryType(
      physicalDevice.getMemoryProperties(),
      memoryRequirements.memoryTypeBits,
      needsStaging ? vk::MemoryPropertyFlags() : ( vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent ) );

    // allocate memory
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    imageMemory = vk::raii::DeviceMemory( device, memoryAllocateInfo );

    // bind memory
    image.bindMemory( imageMemory, 0 );

    vk::raii::Buffer       textureBuffer       = nullptr;
    vk::raii::DeviceMemory textureBufferMemory = nullptr;
    if ( needsStaging )
    {
      // Need a staging buffer to map and copy texture into
      vk::BufferCreateInfo bufferCreateInfo( {}, surfaceData.extent.width * surfaceData.extent.height * 4, vk::BufferUsageFlagBits::eTransferSrc );
      textureBuffer = vk::raii::Buffer( device, bufferCreateInfo );

      memoryRequirements = textureBuffer.getMemoryRequirements();
      memoryTypeIndex    = vk::su::findMemoryType( physicalDevice.getMemoryProperties(),
                                                memoryRequirements.memoryTypeBits,
                                                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

      // allocate memory
      memoryAllocateInfo  = vk::MemoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
      textureBufferMemory = vk::raii::DeviceMemory( device, memoryAllocateInfo );

      // bind memory
      textureBuffer.bindMemory( textureBufferMemory, 0 );
    }
    else
    {
      vk::SubresourceLayout subresourceLayout = image.getSubresourceLayout( vk::ImageSubresource( vk::ImageAspectFlagBits::eColor ) );
    }

    void * data = needsStaging ? textureBufferMemory.mapMemory( 0, memoryRequirements.size, vk::MemoryMapFlags() )
                               : imageMemory.mapMemory( 0, memoryRequirements.size, vk::MemoryMapFlags() );

    // Checkerboard of 16x16 pixel squares
    unsigned char * pImageMemory = static_cast<unsigned char *>( data );
    for ( uint32_t row = 0; row < surfaceData.extent.height; row++ )
    {
      for ( uint32_t col = 0; col < surfaceData.extent.width; col++ )
      {
        unsigned char rgb = ( ( ( row & 0x10 ) == 0 ) ^ ( ( col & 0x10 ) == 0 ) ) * 255;
        pImageMemory[0]   = rgb;
        pImageMemory[1]   = rgb;
        pImageMemory[2]   = rgb;
        pImageMemory[3]   = 255;
        pImageMemory += 4;
      }
    }

    needsStaging ? textureBufferMemory.unmapMemory() : imageMemory.unmapMemory();

    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    if ( needsStaging )
    {
      // Since we're going to blit to the texture image, set its layout to eTransferDstOptimal
      vk::raii::su::setImageLayout( commandBuffer, image, format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
      vk::BufferImageCopy copyRegion( 0,
                                      surfaceData.extent.width,
                                      surfaceData.extent.height,
                                      vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
                                      vk::Offset3D( 0, 0, 0 ),
                                      vk::Extent3D( surfaceData.extent, 1 ) );
      commandBuffer.copyBufferToImage( textureBuffer, image, vk::ImageLayout::eTransferDstOptimal, copyRegion );
      // Set the layout for the texture image from eTransferDstOptimal to SHADER_READ_ONLY
      vk::raii::su::setImageLayout( commandBuffer, image, format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
    }
    else
    {
      // If we can use the linear tiled image as a texture, just do it
      vk::raii::su::setImageLayout( commandBuffer, image, format, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eShaderReadOnlyOptimal );
    }

    commandBuffer.end();
    vk::raii::su::submitAndWait( device, graphicsQueue, commandBuffer );

    vk::SamplerCreateInfo samplerCreateInfo( {},
                                             vk::Filter::eNearest,
                                             vk::Filter::eNearest,
                                             vk::SamplerMipmapMode::eNearest,
                                             vk::SamplerAddressMode::eClampToEdge,
                                             vk::SamplerAddressMode::eClampToEdge,
                                             vk::SamplerAddressMode::eClampToEdge,
                                             0.0f,
                                             false,
                                             1.0f,
                                             false,
                                             vk::CompareOp::eNever,
                                             0.0f,
                                             0.0f,
                                             vk::BorderColor::eFloatOpaqueWhite );
    vk::raii::Sampler     sampler( device, samplerCreateInfo );

    vk::ImageViewCreateInfo imageViewCreateInfo( {}, image, vk::ImageViewType::e2D, format, {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } );
    vk::raii::ImageView     imageView( device, imageViewCreateInfo );

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
