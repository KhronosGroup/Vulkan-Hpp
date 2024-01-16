#pragma once

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

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
// no need to ignore any warnings with GCC
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#include "../../samples/utils/utils.hpp"

#include <numeric>
#include <vulkan/vulkan_raii.hpp>

namespace vk
{
  namespace raii
  {
    namespace su
    {
      vk::raii::DeviceMemory allocateDeviceMemory( vk::raii::Device const &                   device,
                                                   vk::PhysicalDeviceMemoryProperties const & memoryProperties,
                                                   vk::MemoryRequirements const &             memoryRequirements,
                                                   vk::MemoryPropertyFlags                    memoryPropertyFlags )
      {
        uint32_t               memoryTypeIndex = vk::su::findMemoryType( memoryProperties, memoryRequirements.memoryTypeBits, memoryPropertyFlags );
        vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
        return vk::raii::DeviceMemory( device, memoryAllocateInfo );
      }

      template <typename T>
      void copyToDevice( vk::raii::DeviceMemory const & deviceMemory, T const * pData, size_t count, vk::DeviceSize stride = sizeof( T ) )
      {
        assert( sizeof( T ) <= stride );
        uint8_t * deviceData = static_cast<uint8_t *>( deviceMemory.mapMemory( 0, count * stride ) );
        if ( stride == sizeof( T ) )
        {
          memcpy( deviceData, pData, count * sizeof( T ) );
        }
        else
        {
          for ( size_t i = 0; i < count; i++ )
          {
            memcpy( deviceData, &pData[i], sizeof( T ) );
            deviceData += stride;
          }
        }
        deviceMemory.unmapMemory();
      }

      template <typename T>
      void copyToDevice( vk::raii::DeviceMemory const & deviceMemory, T const & data )
      {
        copyToDevice<T>( deviceMemory, &data, 1 );
      }

      template <typename T, class... Args>
      std::unique_ptr<T> make_unique( Args &&... args )
      {
#if ( 14 <= VULKAN_HPP_CPP_VERSION )
        return std::make_unique<T>( std::forward<Args>( args )... );
#else
        return std::unique_ptr<T>( new T( std::forward<Args>( args )... ) );
#endif
      }

      template <typename Func>
      void oneTimeSubmit( vk::raii::Device const & device, vk::raii::CommandPool const & commandPool, vk::raii::Queue const & queue, Func const & func )
      {
        vk::raii::CommandBuffer commandBuffer = std::move( vk::raii::CommandBuffers( device, { *commandPool, vk::CommandBufferLevel::ePrimary, 1 } ).front() );
        commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit ) );
        func( commandBuffer );
        commandBuffer.end();
        vk::SubmitInfo submitInfo( nullptr, nullptr, *commandBuffer );
        queue.submit( submitInfo, nullptr );
        queue.waitIdle();
      }

      void setImageLayout(
        vk::raii::CommandBuffer const & commandBuffer, vk::Image image, vk::Format format, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout )
      {
        vk::AccessFlags sourceAccessMask;
        switch ( oldImageLayout )
        {
          case vk::ImageLayout::eTransferDstOptimal: sourceAccessMask = vk::AccessFlagBits::eTransferWrite; break;
          case vk::ImageLayout::ePreinitialized: sourceAccessMask = vk::AccessFlagBits::eHostWrite; break;
          case vk::ImageLayout::eGeneral:  // sourceAccessMask is empty
          case vk::ImageLayout::eUndefined: break;
          default: assert( false ); break;
        }

        vk::PipelineStageFlags sourceStage;
        switch ( oldImageLayout )
        {
          case vk::ImageLayout::eGeneral:
          case vk::ImageLayout::ePreinitialized: sourceStage = vk::PipelineStageFlagBits::eHost; break;
          case vk::ImageLayout::eTransferDstOptimal: sourceStage = vk::PipelineStageFlagBits::eTransfer; break;
          case vk::ImageLayout::eUndefined: sourceStage = vk::PipelineStageFlagBits::eTopOfPipe; break;
          default: assert( false ); break;
        }

        vk::AccessFlags destinationAccessMask;
        switch ( newImageLayout )
        {
          case vk::ImageLayout::eColorAttachmentOptimal: destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite; break;
          case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            destinationAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            break;
          case vk::ImageLayout::eGeneral:  // empty destinationAccessMask
          case vk::ImageLayout::ePresentSrcKHR: break;
          case vk::ImageLayout::eShaderReadOnlyOptimal: destinationAccessMask = vk::AccessFlagBits::eShaderRead; break;
          case vk::ImageLayout::eTransferSrcOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferRead; break;
          case vk::ImageLayout::eTransferDstOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferWrite; break;
          default: assert( false ); break;
        }

        vk::PipelineStageFlags destinationStage;
        switch ( newImageLayout )
        {
          case vk::ImageLayout::eColorAttachmentOptimal: destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput; break;
          case vk::ImageLayout::eDepthStencilAttachmentOptimal: destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests; break;
          case vk::ImageLayout::eGeneral: destinationStage = vk::PipelineStageFlagBits::eHost; break;
          case vk::ImageLayout::ePresentSrcKHR: destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe; break;
          case vk::ImageLayout::eShaderReadOnlyOptimal: destinationStage = vk::PipelineStageFlagBits::eFragmentShader; break;
          case vk::ImageLayout::eTransferDstOptimal:
          case vk::ImageLayout::eTransferSrcOptimal: destinationStage = vk::PipelineStageFlagBits::eTransfer; break;
          default: assert( false ); break;
        }

        vk::ImageAspectFlags aspectMask;
        if ( newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal )
        {
          aspectMask = vk::ImageAspectFlagBits::eDepth;
          if ( format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint )
          {
            aspectMask |= vk::ImageAspectFlagBits::eStencil;
          }
        }
        else
        {
          aspectMask = vk::ImageAspectFlagBits::eColor;
        }

        vk::ImageSubresourceRange imageSubresourceRange( aspectMask, 0, 1, 0, 1 );
        vk::ImageMemoryBarrier    imageMemoryBarrier( sourceAccessMask,
                                                   destinationAccessMask,
                                                   oldImageLayout,
                                                   newImageLayout,
                                                   VK_QUEUE_FAMILY_IGNORED,
                                                   VK_QUEUE_FAMILY_IGNORED,
                                                   image,
                                                   imageSubresourceRange );
        return commandBuffer.pipelineBarrier( sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier );
      }

      struct BufferData
      {
        BufferData( vk::raii::PhysicalDevice const & physicalDevice,
                    vk::raii::Device const &         device,
                    vk::DeviceSize                   size,
                    vk::BufferUsageFlags             usage,
                    vk::MemoryPropertyFlags          propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent )
          : buffer( device, vk::BufferCreateInfo( {}, size, usage ) )
#if !defined( NDEBUG )
          , m_size( size )
          , m_usage( usage )
          , m_propertyFlags( propertyFlags )
#endif
        {
          deviceMemory = vk::raii::su::allocateDeviceMemory( device, physicalDevice.getMemoryProperties(), buffer.getMemoryRequirements(), propertyFlags );
          buffer.bindMemory( deviceMemory, 0 );
        }

        BufferData( std::nullptr_t ) {}

        template <typename DataType>
        void upload( DataType const & data ) const
        {
          assert( ( m_propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent ) && ( m_propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible ) );
          assert( sizeof( DataType ) <= m_size );

          void * dataPtr = deviceMemory.mapMemory( 0, sizeof( DataType ) );
          memcpy( dataPtr, &data, sizeof( DataType ) );
          deviceMemory.unmapMemory();
        }

        template <typename DataType>
        void upload( std::vector<DataType> const & data, size_t stride = 0 ) const
        {
          assert( m_propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible );

          size_t elementSize = stride ? stride : sizeof( DataType );
          assert( sizeof( DataType ) <= elementSize );

          copyToDevice( deviceMemory, data.data(), data.size(), elementSize );
        }

        template <typename DataType>
        void upload( vk::raii::PhysicalDevice const & physicalDevice,
                     vk::raii::Device const &         device,
                     vk::raii::CommandPool const &    commandPool,
                     vk::raii::Queue const &          queue,
                     std::vector<DataType> const &    data,
                     size_t                           stride ) const
        {
          assert( m_usage & vk::BufferUsageFlagBits::eTransferDst );
          assert( m_propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal );

          size_t elementSize = stride ? stride : sizeof( DataType );
          assert( sizeof( DataType ) <= elementSize );

          size_t dataSize = data.size() * elementSize;
          assert( dataSize <= m_size );

          vk::raii::su::BufferData stagingBuffer( physicalDevice, device, dataSize, vk::BufferUsageFlagBits::eTransferSrc );
          copyToDevice( stagingBuffer.deviceMemory, data.data(), data.size(), elementSize );

          vk::raii::su::oneTimeSubmit( device,
                                       commandPool,
                                       queue,
                                       [&]( vk::raii::CommandBuffer const & commandBuffer )
                                       { commandBuffer.copyBuffer( *stagingBuffer.buffer, *this->buffer, vk::BufferCopy( 0, 0, dataSize ) ); } );
        }

        // the DeviceMemory should be destroyed before the Buffer it is bound to; to get that order with the standard destructor
        // of the BufferData, the order of DeviceMemory and Buffer here matters
        vk::raii::DeviceMemory deviceMemory = nullptr;
        vk::raii::Buffer       buffer       = nullptr;
#if !defined( NDEBUG )
      private:
        vk::DeviceSize          m_size;
        vk::BufferUsageFlags    m_usage;
        vk::MemoryPropertyFlags m_propertyFlags;
#endif
      };

      struct ImageData
      {
        ImageData( vk::raii::PhysicalDevice const & physicalDevice,
                   vk::raii::Device const &         device,
                   vk::Format                       format_,
                   vk::Extent2D const &             extent,
                   vk::ImageTiling                  tiling,
                   vk::ImageUsageFlags              usage,
                   vk::ImageLayout                  initialLayout,
                   vk::MemoryPropertyFlags          memoryProperties,
                   vk::ImageAspectFlags             aspectMask )
          : format( format_ )
          , image( device,
                   { vk::ImageCreateFlags(),
                     vk::ImageType::e2D,
                     format,
                     vk::Extent3D( extent, 1 ),
                     1,
                     1,
                     vk::SampleCountFlagBits::e1,
                     tiling,
                     usage | vk::ImageUsageFlagBits::eSampled,
                     vk::SharingMode::eExclusive,
                     {},
                     initialLayout } )
        {
          deviceMemory = vk::raii::su::allocateDeviceMemory( device, physicalDevice.getMemoryProperties(), image.getMemoryRequirements(), memoryProperties );
          image.bindMemory( deviceMemory, 0 );
          imageView = vk::raii::ImageView( device, vk::ImageViewCreateInfo( {}, image, vk::ImageViewType::e2D, format, {}, { aspectMask, 0, 1, 0, 1 } ) );
        }

        ImageData( std::nullptr_t ) {}

        // the DeviceMemory should be destroyed before the Image it is bound to; to get that order with the standard destructor
        // of the ImageData, the order of DeviceMemory and Image here matters
        vk::Format             format;
        vk::raii::DeviceMemory deviceMemory = nullptr;
        vk::raii::Image        image        = nullptr;
        vk::raii::ImageView    imageView    = nullptr;
      };

      struct DepthBufferData : public ImageData
      {
        DepthBufferData( vk::raii::PhysicalDevice const & physicalDevice, vk::raii::Device const & device, vk::Format format, vk::Extent2D const & extent )
          : ImageData( physicalDevice,
                       device,
                       format,
                       extent,
                       vk::ImageTiling::eOptimal,
                       vk::ImageUsageFlagBits::eDepthStencilAttachment,
                       vk::ImageLayout::eUndefined,
                       vk::MemoryPropertyFlagBits::eDeviceLocal,
                       vk::ImageAspectFlagBits::eDepth )
        {
        }
      };

      struct SurfaceData
      {
        SurfaceData( vk::raii::Instance const & instance, std::string const & windowName, vk::Extent2D const & extent_ )
          : extent( extent_ ), window( vk::su::createWindow( windowName, extent ) )
        {
          VkSurfaceKHR _surface;
          VkResult     err = glfwCreateWindowSurface( static_cast<VkInstance>( *instance ), window.handle, nullptr, &_surface );
          if ( err != VK_SUCCESS )
            throw std::runtime_error( "Failed to create window!" );
          surface = vk::raii::SurfaceKHR( instance, _surface );
        }

        vk::Extent2D         extent;
        vk::su::WindowData   window;
        vk::raii::SurfaceKHR surface = nullptr;
      };

      struct SwapChainData
      {
        SwapChainData( vk::raii::PhysicalDevice const & physicalDevice,
                       vk::raii::Device const &         device,
                       vk::raii::SurfaceKHR const &     surface,
                       vk::Extent2D const &             extent,
                       vk::ImageUsageFlags              usage,
                       vk::raii::SwapchainKHR const *   pOldSwapchain,
                       uint32_t                         graphicsQueueFamilyIndex,
                       uint32_t                         presentQueueFamilyIndex )
        {
          vk::SurfaceFormatKHR surfaceFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surface ) );
          colorFormat                        = surfaceFormat.format;

          vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
          vk::Extent2D               swapchainExtent;
          if ( surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() )
          {
            // If the surface size is undefined, the size is set to the size of the images requested.
            swapchainExtent.width  = vk::su::clamp( extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width );
            swapchainExtent.height = vk::su::clamp( extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height );
          }
          else
          {
            // If the surface size is defined, the swap chain size must match
            swapchainExtent = surfaceCapabilities.currentExtent;
          }
          vk::SurfaceTransformFlagBitsKHR preTransform = ( surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity )
                                                         ? vk::SurfaceTransformFlagBitsKHR::eIdentity
                                                         : surfaceCapabilities.currentTransform;
          vk::CompositeAlphaFlagBitsKHR   compositeAlpha =
            ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
              : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
              : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit ) ? vk::CompositeAlphaFlagBitsKHR::eInherit
                                                                                                          : vk::CompositeAlphaFlagBitsKHR::eOpaque;
          vk::PresentModeKHR         presentMode = vk::su::pickPresentMode( physicalDevice.getSurfacePresentModesKHR( surface ) );
          vk::SwapchainCreateInfoKHR swapChainCreateInfo( {},
                                                          surface,
                                                          vk::su::clamp( 3u, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount ),
                                                          colorFormat,
                                                          surfaceFormat.colorSpace,
                                                          swapchainExtent,
                                                          1,
                                                          usage,
                                                          vk::SharingMode::eExclusive,
                                                          {},
                                                          preTransform,
                                                          compositeAlpha,
                                                          presentMode,
                                                          true,
                                                          pOldSwapchain ? **pOldSwapchain : nullptr );
          if ( graphicsQueueFamilyIndex != presentQueueFamilyIndex )
          {
            uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
            // If the graphics and present queues are from different queue families, we either have to explicitly
            // transfer ownership of images between the queues, or we have to create the swapchain with imageSharingMode
            // as vk::SharingMode::eConcurrent
            swapChainCreateInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
          }
          swapChain = vk::raii::SwapchainKHR( device, swapChainCreateInfo );

          images = swapChain.getImages();

          imageViews.reserve( images.size() );
          vk::ImageViewCreateInfo imageViewCreateInfo( {}, {}, vk::ImageViewType::e2D, colorFormat, {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } );
          for ( auto image : images )
          {
            imageViewCreateInfo.image = image;
            imageViews.emplace_back( device, imageViewCreateInfo );
          }
        }

        vk::Format                       colorFormat;
        vk::raii::SwapchainKHR           swapChain = nullptr;
        std::vector<vk::Image>           images;
        std::vector<vk::raii::ImageView> imageViews;
      };

      struct TextureData
      {
        TextureData( vk::raii::PhysicalDevice const & physicalDevice,
                     vk::raii::Device const &         device,
                     vk::Extent2D const &             extent_            = { 256, 256 },
                     vk::ImageUsageFlags              usageFlags         = {},
                     vk::FormatFeatureFlags           formatFeatureFlags = {},
                     bool                             anisotropyEnable   = false,
                     bool                             forceStaging       = false )
          : format( vk::Format::eR8G8B8A8Unorm )
          , extent( extent_ )
          , sampler( device,
                     { {},
                       vk::Filter::eLinear,
                       vk::Filter::eLinear,
                       vk::SamplerMipmapMode::eLinear,
                       vk::SamplerAddressMode::eRepeat,
                       vk::SamplerAddressMode::eRepeat,
                       vk::SamplerAddressMode::eRepeat,
                       0.0f,
                       anisotropyEnable,
                       16.0f,
                       false,
                       vk::CompareOp::eNever,
                       0.0f,
                       0.0f,
                       vk::BorderColor::eFloatOpaqueBlack } )
        {
          vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( format );

          formatFeatureFlags |= vk::FormatFeatureFlagBits::eSampledImage;
          needsStaging = forceStaging || ( ( formatProperties.linearTilingFeatures & formatFeatureFlags ) != formatFeatureFlags );
          vk::ImageTiling         imageTiling;
          vk::ImageLayout         initialLayout;
          vk::MemoryPropertyFlags requirements;
          if ( needsStaging )
          {
            assert( ( formatProperties.optimalTilingFeatures & formatFeatureFlags ) == formatFeatureFlags );
            stagingBufferData = BufferData( physicalDevice, device, extent.width * extent.height * 4, vk::BufferUsageFlagBits::eTransferSrc );
            imageTiling       = vk::ImageTiling::eOptimal;
            usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
            initialLayout = vk::ImageLayout::eUndefined;
          }
          else
          {
            imageTiling   = vk::ImageTiling::eLinear;
            initialLayout = vk::ImageLayout::ePreinitialized;
            requirements  = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
          }
          imageData = ImageData( physicalDevice,
                                 device,
                                 format,
                                 extent,
                                 imageTiling,
                                 usageFlags | vk::ImageUsageFlagBits::eSampled,
                                 initialLayout,
                                 requirements,
                                 vk::ImageAspectFlagBits::eColor );
        }

        template <typename ImageGenerator>
        void setImage( vk::raii::CommandBuffer const & commandBuffer, ImageGenerator const & imageGenerator )
        {
          void * data = needsStaging ? stagingBufferData.deviceMemory.mapMemory( 0, stagingBufferData.buffer.getMemoryRequirements().size )
                                     : imageData.deviceMemory.mapMemory( 0, imageData.image.getMemoryRequirements().size );
          imageGenerator( data, extent );
          needsStaging ? stagingBufferData.deviceMemory.unmapMemory() : imageData.deviceMemory.unmapMemory();

          if ( needsStaging )
          {
            // Since we're going to blit to the texture image, set its layout to eTransferDstOptimal
            vk::raii::su::setImageLayout( commandBuffer, imageData.image, imageData.format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
            vk::BufferImageCopy copyRegion( 0,
                                            extent.width,
                                            extent.height,
                                            vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
                                            vk::Offset3D( 0, 0, 0 ),
                                            vk::Extent3D( extent, 1 ) );
            commandBuffer.copyBufferToImage( stagingBufferData.buffer, imageData.image, vk::ImageLayout::eTransferDstOptimal, copyRegion );
            // Set the layout for the texture image from eTransferDstOptimal to eShaderReadOnlyOptimal
            vk::raii::su::setImageLayout(
              commandBuffer, imageData.image, imageData.format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
          }
          else
          {
            // If we can use the linear tiled image as a texture, just do it
            vk::raii::su::setImageLayout(
              commandBuffer, imageData.image, imageData.format, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eShaderReadOnlyOptimal );
          }
        }

        vk::Format        format;
        vk::Extent2D      extent;
        bool              needsStaging;
        BufferData        stagingBufferData = nullptr;
        ImageData         imageData         = nullptr;
        vk::raii::Sampler sampler;
      };

      std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex( vk::raii::PhysicalDevice const & physicalDevice,
                                                                            vk::raii::SurfaceKHR const &     surface )
      {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
        assert( queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max() );

        uint32_t graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( queueFamilyProperties );
        if ( physicalDevice.getSurfaceSupportKHR( graphicsQueueFamilyIndex, surface ) )
        {
          return std::make_pair( graphicsQueueFamilyIndex,
                                 graphicsQueueFamilyIndex );  // the first graphicsQueueFamilyIndex does also support presents
        }

        // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
        // graphics and present
        for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
        {
          if ( ( queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics ) &&
               physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
          {
            return std::make_pair( static_cast<uint32_t>( i ), static_cast<uint32_t>( i ) );
          }
        }

        // there's nothing like a single family index that supports both graphics and present -> look for an other
        // family index that supports present
        for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
        {
          if ( physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
          {
            return std::make_pair( graphicsQueueFamilyIndex, static_cast<uint32_t>( i ) );
          }
        }

        throw std::runtime_error( "Could not find queues for both graphics or present -> terminating" );
      }

      vk::raii::CommandBuffer makeCommandBuffer( vk::raii::Device const & device, vk::raii::CommandPool const & commandPool )
      {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 );
        return std::move( vk::raii::CommandBuffers( device, commandBufferAllocateInfo ).front() );
      }

      void fullPipelineBarrier( vk::raii::CommandBuffer const & commandBuffer )
      {
        vk::MemoryBarrier memoryBarrier( vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
                                         vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite );
        commandBuffer.pipelineBarrier( vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands, {}, memoryBarrier, nullptr, nullptr );
      }

      vk::raii::DescriptorPool makeDescriptorPool( vk::raii::Device const & device, std::vector<vk::DescriptorPoolSize> const & poolSizes )
      {
        assert( !poolSizes.empty() );
        uint32_t maxSets = std::accumulate(
          poolSizes.begin(), poolSizes.end(), 0, []( uint32_t sum, vk::DescriptorPoolSize const & dps ) { return sum + dps.descriptorCount; } );
        assert( 0 < maxSets );

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, poolSizes );
        return vk::raii::DescriptorPool( device, descriptorPoolCreateInfo );
      }

      vk::raii::DescriptorSetLayout makeDescriptorSetLayout( vk::raii::Device const &                                                            device,
                                                             std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>> const & bindingData,
                                                             vk::DescriptorSetLayoutCreateFlags                                                  flags = {} )
      {
        std::vector<vk::DescriptorSetLayoutBinding> bindings( bindingData.size() );
        for ( size_t i = 0; i < bindingData.size(); i++ )
        {
          bindings[i] = vk::DescriptorSetLayoutBinding(
            vk::su::checked_cast<uint32_t>( i ), std::get<0>( bindingData[i] ), std::get<1>( bindingData[i] ), std::get<2>( bindingData[i] ) );
        }
        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo( flags, bindings );
        return vk::raii::DescriptorSetLayout( device, descriptorSetLayoutCreateInfo );
      }

      vk::raii::Device makeDevice( vk::raii::PhysicalDevice const &   physicalDevice,
                                   uint32_t                           queueFamilyIndex,
                                   std::vector<std::string> const &   extensions             = {},
                                   vk::PhysicalDeviceFeatures const * physicalDeviceFeatures = nullptr,
                                   void const *                       pNext                  = nullptr )
      {
        std::vector<char const *> enabledExtensions;
        enabledExtensions.reserve( extensions.size() );
        for ( auto const & ext : extensions )
        {
          enabledExtensions.push_back( ext.data() );
        }

        float                     queuePriority = 0.0f;
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority );
        vk::DeviceCreateInfo      deviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo, {}, enabledExtensions, physicalDeviceFeatures, pNext );
        return vk::raii::Device( physicalDevice, deviceCreateInfo );
      }

      std::vector<vk::raii::Framebuffer> makeFramebuffers( vk::raii::Device const &                 device,
                                                           vk::raii::RenderPass &                   renderPass,
                                                           std::vector<vk::raii::ImageView> const & imageViews,
                                                           vk::raii::ImageView const *              pDepthImageView,
                                                           vk::Extent2D const &                     extent )
      {
        vk::ImageView attachments[2];
        attachments[1] = pDepthImageView ? *pDepthImageView : vk::ImageView();

        vk::FramebufferCreateInfo framebufferCreateInfo(
          vk::FramebufferCreateFlags(), renderPass, pDepthImageView ? 2 : 1, attachments, extent.width, extent.height, 1 );
        std::vector<vk::raii::Framebuffer> framebuffers;
        framebuffers.reserve( imageViews.size() );
        for ( auto const & imageView : imageViews )
        {
          attachments[0] = imageView;
          framebuffers.push_back( vk::raii::Framebuffer( device, framebufferCreateInfo ) );
        }

        return framebuffers;
      }

      vk::raii::Pipeline makeGraphicsPipeline( vk::raii::Device const &                             device,
                                               vk::raii::PipelineCache const &                      pipelineCache,
                                               vk::raii::ShaderModule const &                       vertexShaderModule,
                                               vk::SpecializationInfo const *                       vertexShaderSpecializationInfo,
                                               vk::raii::ShaderModule const &                       fragmentShaderModule,
                                               vk::SpecializationInfo const *                       fragmentShaderSpecializationInfo,
                                               uint32_t                                             vertexStride,
                                               std::vector<std::pair<vk::Format, uint32_t>> const & vertexInputAttributeFormatOffset,
                                               vk::FrontFace                                        frontFace,
                                               bool                                                 depthBuffered,
                                               vk::raii::PipelineLayout const &                     pipelineLayout,
                                               vk::raii::RenderPass const &                         renderPass )
      {
        std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
          vk::PipelineShaderStageCreateInfo( {}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main", vertexShaderSpecializationInfo ),
          vk::PipelineShaderStageCreateInfo( {}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main", fragmentShaderSpecializationInfo )
        };

        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;
        vk::PipelineVertexInputStateCreateInfo           pipelineVertexInputStateCreateInfo;
        vk::VertexInputBindingDescription                vertexInputBindingDescription( 0, vertexStride );

        if ( 0 < vertexStride )
        {
          vertexInputAttributeDescriptions.reserve( vertexInputAttributeFormatOffset.size() );
          for ( uint32_t i = 0; i < vertexInputAttributeFormatOffset.size(); i++ )
          {
            vertexInputAttributeDescriptions.emplace_back( i, 0, vertexInputAttributeFormatOffset[i].first, vertexInputAttributeFormatOffset[i].second );
          }
          pipelineVertexInputStateCreateInfo.setVertexBindingDescriptions( vertexInputBindingDescription );
          pipelineVertexInputStateCreateInfo.setVertexAttributeDescriptions( vertexInputAttributeDescriptions );
        }

        vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo( vk::PipelineInputAssemblyStateCreateFlags(),
                                                                                       vk::PrimitiveTopology::eTriangleList );

        vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

        vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo( vk::PipelineRasterizationStateCreateFlags(),
                                                                                       false,
                                                                                       false,
                                                                                       vk::PolygonMode::eFill,
                                                                                       vk::CullModeFlagBits::eBack,
                                                                                       frontFace,
                                                                                       false,
                                                                                       0.0f,
                                                                                       0.0f,
                                                                                       0.0f,
                                                                                       1.0f );

        vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo( {}, vk::SampleCountFlagBits::e1 );

        vk::StencilOpState                      stencilOpState( vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
        vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
          vk::PipelineDepthStencilStateCreateFlags(), depthBuffered, depthBuffered, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState );

        vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                                                     vk::ColorComponentFlagBits::eA );
        vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,
                                                                                 vk::BlendFactor::eZero,
                                                                                 vk::BlendFactor::eZero,
                                                                                 vk::BlendOp::eAdd,
                                                                                 vk::BlendFactor::eZero,
                                                                                 vk::BlendFactor::eZero,
                                                                                 vk::BlendOp::eAdd,
                                                                                 colorComponentFlags );
        vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
          vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, pipelineColorBlendAttachmentState, { { 1.0f, 1.0f, 1.0f, 1.0f } } );

        std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
        vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(), dynamicStates );

        vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo( vk::PipelineCreateFlags(),
                                                                   pipelineShaderStageCreateInfos,
                                                                   &pipelineVertexInputStateCreateInfo,
                                                                   &pipelineInputAssemblyStateCreateInfo,
                                                                   nullptr,
                                                                   &pipelineViewportStateCreateInfo,
                                                                   &pipelineRasterizationStateCreateInfo,
                                                                   &pipelineMultisampleStateCreateInfo,
                                                                   &pipelineDepthStencilStateCreateInfo,
                                                                   &pipelineColorBlendStateCreateInfo,
                                                                   &pipelineDynamicStateCreateInfo,
                                                                   pipelineLayout,
                                                                   renderPass );

        return vk::raii::Pipeline( device, pipelineCache, graphicsPipelineCreateInfo );
      }

      vk::raii::Image makeImage( vk::raii::Device const & device )
      {
        vk::ImageCreateInfo imageCreateInfo( {},
                                             vk::ImageType::e2D,
                                             vk::Format::eB8G8R8A8Unorm,
                                             vk::Extent3D( 640, 640, 1 ),
                                             1,
                                             1,
                                             vk::SampleCountFlagBits::e1,
                                             vk::ImageTiling::eLinear,
                                             vk::ImageUsageFlagBits::eTransferSrc );
        return vk::raii::Image( device, imageCreateInfo );
      }

      vk::raii::Instance makeInstance( vk::raii::Context const &        context,
                                       std::string const &              appName,
                                       std::string const &              engineName,
                                       std::vector<std::string> const & layers     = {},
                                       std::vector<std::string> const & extensions = {},
                                       uint32_t                         apiVersion = VK_API_VERSION_1_0 )
      {
        vk::ApplicationInfo       applicationInfo( appName.c_str(), 1, engineName.c_str(), 1, apiVersion );
        std::vector<char const *> enabledLayers = vk::su::gatherLayers( layers
#if !defined( NDEBUG )
                                                                        ,
                                                                        context.enumerateInstanceLayerProperties()
#endif
        );
        std::vector<char const *> enabledExtensions = vk::su::gatherExtensions( extensions
#if !defined( NDEBUG )
                                                                                ,
                                                                                context.enumerateInstanceExtensionProperties()
#endif
        );
#if defined( NDEBUG )
        vk::StructureChain<vk::InstanceCreateInfo>
#else
        vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
          instanceCreateInfoChain = vk::su::makeInstanceCreateInfoChain( applicationInfo, enabledLayers, enabledExtensions );

        return vk::raii::Instance( context, instanceCreateInfoChain.get<vk::InstanceCreateInfo>() );
      }

      vk::raii::RenderPass makeRenderPass( vk::raii::Device const & device,
                                           vk::Format               colorFormat,
                                           vk::Format               depthFormat,
                                           vk::AttachmentLoadOp     loadOp           = vk::AttachmentLoadOp::eClear,
                                           vk::ImageLayout          colorFinalLayout = vk::ImageLayout::ePresentSrcKHR )
      {
        std::vector<vk::AttachmentDescription> attachmentDescriptions;
        assert( colorFormat != vk::Format::eUndefined );
        attachmentDescriptions.emplace_back( vk::AttachmentDescriptionFlags(),
                                             colorFormat,
                                             vk::SampleCountFlagBits::e1,
                                             loadOp,
                                             vk::AttachmentStoreOp::eStore,
                                             vk::AttachmentLoadOp::eDontCare,
                                             vk::AttachmentStoreOp::eDontCare,
                                             vk::ImageLayout::eUndefined,
                                             colorFinalLayout );
        if ( depthFormat != vk::Format::eUndefined )
        {
          attachmentDescriptions.emplace_back( vk::AttachmentDescriptionFlags(),
                                               depthFormat,
                                               vk::SampleCountFlagBits::e1,
                                               loadOp,
                                               vk::AttachmentStoreOp::eDontCare,
                                               vk::AttachmentLoadOp::eDontCare,
                                               vk::AttachmentStoreOp::eDontCare,
                                               vk::ImageLayout::eUndefined,
                                               vk::ImageLayout::eDepthStencilAttachmentOptimal );
        }
        vk::AttachmentReference  colorAttachment( 0, vk::ImageLayout::eColorAttachmentOptimal );
        vk::AttachmentReference  depthAttachment( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );
        vk::SubpassDescription   subpassDescription( vk::SubpassDescriptionFlags(),
                                                   vk::PipelineBindPoint::eGraphics,
                                                     {},
                                                   colorAttachment,
                                                     {},
                                                   ( depthFormat != vk::Format::eUndefined ) ? &depthAttachment : nullptr );
        vk::RenderPassCreateInfo renderPassCreateInfo( vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription );
        return vk::raii::RenderPass( device, renderPassCreateInfo );
      }

      vk::Format pickDepthFormat( vk::raii::PhysicalDevice const & physicalDevice )
      {
        std::vector<vk::Format> candidates = { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
        for ( vk::Format format : candidates )
        {
          vk::FormatProperties props = physicalDevice.getFormatProperties( format );

          if ( props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
          {
            return format;
          }
        }
        throw std::runtime_error( "failed to find supported format!" );
      }

      void submitAndWait( vk::raii::Device const & device, vk::raii::Queue const & queue, vk::raii::CommandBuffer const & commandBuffer )
      {
        vk::raii::Fence fence( device, vk::FenceCreateInfo() );
        queue.submit( vk::SubmitInfo( nullptr, nullptr, *commandBuffer ), fence );
        while ( vk::Result::eTimeout == device.waitForFences( { fence }, VK_TRUE, vk::su::FenceTimeout ) )
          ;
      }

      void updateDescriptorSets(
        vk::raii::Device const &                                                                                                    device,
        vk::raii::DescriptorSet const &                                                                                             descriptorSet,
        std::vector<std::tuple<vk::DescriptorType, vk::raii::Buffer const &, vk::DeviceSize, vk::raii::BufferView const *>> const & bufferData,
        vk::raii::su::TextureData const &                                                                                           textureData,
        uint32_t                                                                                                                    bindingOffset = 0 )
      {
        std::vector<vk::DescriptorBufferInfo> bufferInfos;
        bufferInfos.reserve( bufferData.size() );

        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
        writeDescriptorSets.reserve( bufferData.size() + 1 );
        uint32_t dstBinding = bindingOffset;
        for ( auto const & bd : bufferData )
        {
          bufferInfos.emplace_back( std::get<1>( bd ), 0, std::get<2>( bd ) );
          vk::BufferView bufferView;
          if ( std::get<3>( bd ) )
          {
            bufferView = *std::get<3>( bd );
          }
          writeDescriptorSets.emplace_back(
            descriptorSet, dstBinding++, 0, 1, std::get<0>( bd ), nullptr, &bufferInfos.back(), std::get<3>( bd ) ? &bufferView : nullptr );
        }

        vk::DescriptorImageInfo imageInfo( textureData.sampler, textureData.imageData.imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
        writeDescriptorSets.emplace_back( descriptorSet, dstBinding, 0, vk::DescriptorType::eCombinedImageSampler, imageInfo, nullptr, nullptr );

        device.updateDescriptorSets( writeDescriptorSets, nullptr );
      }

      void updateDescriptorSets(
        vk::raii::Device const &                                                                                                    device,
        vk::raii::DescriptorSet const &                                                                                             descriptorSet,
        std::vector<std::tuple<vk::DescriptorType, vk::raii::Buffer const &, vk::DeviceSize, vk::raii::BufferView const *>> const & bufferData,
        std::vector<vk::raii::su::TextureData> const &                                                                              textureData,
        uint32_t                                                                                                                    bindingOffset = 0 )
      {
        std::vector<vk::DescriptorBufferInfo> bufferInfos;
        bufferInfos.reserve( bufferData.size() );

        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
        writeDescriptorSets.reserve( bufferData.size() + ( textureData.empty() ? 0 : 1 ) );
        uint32_t dstBinding = bindingOffset;
        for ( auto const & bd : bufferData )
        {
          bufferInfos.emplace_back( std::get<1>( bd ), 0, std::get<2>( bd ) );
          vk::BufferView bufferView;
          if ( std::get<3>( bd ) )
          {
            bufferView = *std::get<3>( bd );
          }
          writeDescriptorSets.emplace_back(
            descriptorSet, dstBinding++, 0, 1, std::get<0>( bd ), nullptr, &bufferInfos.back(), std::get<3>( bd ) ? &bufferView : nullptr );
        }

        std::vector<vk::DescriptorImageInfo> imageInfos;
        if ( !textureData.empty() )
        {
          imageInfos.reserve( textureData.size() );
          for ( auto const & thd : textureData )
          {
            imageInfos.emplace_back( thd.sampler, thd.imageData.imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
          }
          writeDescriptorSets.emplace_back( descriptorSet,
                                            dstBinding,
                                            0,
                                            vk::su::checked_cast<uint32_t>( imageInfos.size() ),
                                            vk::DescriptorType::eCombinedImageSampler,
                                            imageInfos.data(),
                                            nullptr,
                                            nullptr );
        }

        device.updateDescriptorSets( writeDescriptorSets, nullptr );
      }

    }  // namespace su
  }    // namespace raii
}  // namespace vk
