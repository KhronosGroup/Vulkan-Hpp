// Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : 06_InitDepthBufferRAII
//                     Initialize a depth buffer

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "06_InitDepthBuffer";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::unique_ptr<vk::raii::Context>  context = vk::raii::su::make_unique<vk::raii::Context>();
    std::unique_ptr<vk::raii::Instance> instance =
      vk::raii::su::makeUniqueInstance( *context, AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::makeUniqueDebugUtilsMessengerEXT( *instance );
#endif
    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::makeUniquePhysicalDevice( *instance );

    vk::raii::su::SurfaceData surfaceData( *instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( *physicalDevice, *surfaceData.surface );
    std::unique_ptr<vk::raii::Device> device = vk::raii::su::makeUniqueDevice(
      *physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    /* VULKAN_HPP_KEY_START */

    const vk::Format     depthFormat      = vk::Format::eD16Unorm;
    vk::FormatProperties formatProperties = physicalDevice->getFormatProperties( depthFormat );

    vk::ImageTiling tiling;
    if ( formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
    {
      tiling = vk::ImageTiling::eLinear;
    }
    else if ( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
    {
      tiling = vk::ImageTiling::eOptimal;
    }
    else
    {
      throw std::runtime_error( "DepthStencilAttachment is not supported for D16Unorm depth format." );
    }
    vk::ImageCreateInfo              imageCreateInfo( {},
                                         vk::ImageType::e2D,
                                         depthFormat,
                                         vk::Extent3D( surfaceData.extent, 1 ),
                                         1,
                                         1,
                                         vk::SampleCountFlagBits::e1,
                                         tiling,
                                         vk::ImageUsageFlagBits::eDepthStencilAttachment );
    std::unique_ptr<vk::raii::Image> depthImage = vk::raii::su::make_unique<vk::raii::Image>( *device, imageCreateInfo );

    vk::PhysicalDeviceMemoryProperties memoryProperties   = physicalDevice->getMemoryProperties();
    vk::MemoryRequirements             memoryRequirements = depthImage->getMemoryRequirements();

    uint32_t typeBits  = memoryRequirements.memoryTypeBits;
    uint32_t typeIndex = uint32_t( ~0 );
    for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++ )
    {
      if ( ( typeBits & 1 ) &&
           ( ( memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal ) ==
             vk::MemoryPropertyFlagBits::eDeviceLocal ) )
      {
        typeIndex = i;
        break;
      }
      typeBits >>= 1;
    }
    assert( typeIndex != uint32_t( ~0 ) );

    vk::MemoryAllocateInfo                  memoryAllocateInfo( memoryRequirements.size, typeIndex );
    std::unique_ptr<vk::raii::DeviceMemory> depthMemory =
      vk::raii::su::make_unique<vk::raii::DeviceMemory>( *device, memoryAllocateInfo );
    depthImage->bindMemory( **depthMemory, 0 );

    vk::ComponentMapping componentMapping(
      vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
    vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 );
    vk::ImageViewCreateInfo   imageViewCreateInfo(
      {}, **depthImage, vk::ImageViewType::e2D, depthFormat, componentMapping, subResourceRange );
    std::unique_ptr<vk::raii::ImageView> depthView =
      vk::raii::su::make_unique<vk::raii::ImageView>( *device, imageViewCreateInfo );

    /* VULKAN_HPP_KEY_END */
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
