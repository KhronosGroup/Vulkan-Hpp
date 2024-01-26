// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_HASH_HPP
#define VULKAN_HASH_HPP

#include <vulkan/vulkansc.hpp>

namespace std
{
  //=======================================
  //=== HASH structures for Flags types ===
  //=======================================

  template <typename BitType>
  struct hash<VULKAN_HPP_NAMESPACE::Flags<BitType>>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Flags<BitType> const & flags ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<typename std::underlying_type<BitType>::type>{}( static_cast<typename std::underlying_type<BitType>::type>( flags ) );
    }
  };

  //===================================
  //=== HASH structures for handles ===
  //===================================

  //=== VK_VERSION_1_0 ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Instance>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Instance const & instance ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkInstance>{}( static_cast<VkInstance>( instance ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevice>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevice const & physicalDevice ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkPhysicalDevice>{}( static_cast<VkPhysicalDevice>( physicalDevice ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Device>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Device const & device ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDevice>{}( static_cast<VkDevice>( device ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Queue>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Queue const & queue ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkQueue>{}( static_cast<VkQueue>( queue ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceMemory>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceMemory const & deviceMemory ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDeviceMemory>{}( static_cast<VkDeviceMemory>( deviceMemory ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Fence>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Fence const & fence ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkFence>{}( static_cast<VkFence>( fence ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Semaphore>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Semaphore const & semaphore ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSemaphore>{}( static_cast<VkSemaphore>( semaphore ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Event>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Event const & event ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkEvent>{}( static_cast<VkEvent>( event ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueryPool>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueryPool const & queryPool ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkQueryPool>{}( static_cast<VkQueryPool>( queryPool ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Buffer>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Buffer const & buffer ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkBuffer>{}( static_cast<VkBuffer>( buffer ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferView>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferView const & bufferView ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkBufferView>{}( static_cast<VkBufferView>( bufferView ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Image>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Image const & image ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkImage>{}( static_cast<VkImage>( image ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageView>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageView const & imageView ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkImageView>{}( static_cast<VkImageView>( imageView ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ShaderModule>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ShaderModule const & shaderModule ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkShaderModule>{}( static_cast<VkShaderModule>( shaderModule ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCache>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCache const & pipelineCache ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkPipelineCache>{}( static_cast<VkPipelineCache>( pipelineCache ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Pipeline>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Pipeline const & pipeline ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkPipeline>{}( static_cast<VkPipeline>( pipeline ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineLayout>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineLayout const & pipelineLayout ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkPipelineLayout>{}( static_cast<VkPipelineLayout>( pipelineLayout ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Sampler>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Sampler const & sampler ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSampler>{}( static_cast<VkSampler>( sampler ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorPool>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorPool const & descriptorPool ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDescriptorPool>{}( static_cast<VkDescriptorPool>( descriptorPool ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSet>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSet const & descriptorSet ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDescriptorSet>{}( static_cast<VkDescriptorSet>( descriptorSet ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetLayout>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetLayout const & descriptorSetLayout ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDescriptorSetLayout>{}( static_cast<VkDescriptorSetLayout>( descriptorSetLayout ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Framebuffer>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Framebuffer const & framebuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkFramebuffer>{}( static_cast<VkFramebuffer>( framebuffer ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPass>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPass const & renderPass ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkRenderPass>{}( static_cast<VkRenderPass>( renderPass ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandPool>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandPool const & commandPool ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkCommandPool>{}( static_cast<VkCommandPool>( commandPool ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBuffer>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandBuffer const & commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkCommandBuffer>{}( static_cast<VkCommandBuffer>( commandBuffer ) );
    }
  };

  //=== VK_VERSION_1_1 ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion const & samplerYcbcrConversion ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSamplerYcbcrConversion>{}( static_cast<VkSamplerYcbcrConversion>( samplerYcbcrConversion ) );
    }
  };

  //=== VK_VERSION_1_3 ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PrivateDataSlot>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PrivateDataSlot const & privateDataSlot ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkPrivateDataSlot>{}( static_cast<VkPrivateDataSlot>( privateDataSlot ) );
    }
  };

  //=== VK_KHR_surface ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceKHR const & surfaceKHR ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSurfaceKHR>{}( static_cast<VkSurfaceKHR>( surfaceKHR ) );
    }
  };

  //=== VK_KHR_swapchain ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SwapchainKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SwapchainKHR const & swapchainKHR ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSwapchainKHR>{}( static_cast<VkSwapchainKHR>( swapchainKHR ) );
    }
  };

  //=== VK_KHR_display ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayKHR const & displayKHR ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDisplayKHR>{}( static_cast<VkDisplayKHR>( displayKHR ) );
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayModeKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayModeKHR const & displayModeKHR ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDisplayModeKHR>{}( static_cast<VkDisplayModeKHR>( displayModeKHR ) );
    }
  };

  //=== VK_EXT_debug_utils ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT const & debugUtilsMessengerEXT ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkDebugUtilsMessengerEXT>{}( static_cast<VkDebugUtilsMessengerEXT>( debugUtilsMessengerEXT ) );
    }
  };

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV const & semaphoreSciSyncPoolNV ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<VkSemaphoreSciSyncPoolNV>{}( static_cast<VkSemaphoreSciSyncPoolNV>( semaphoreSciSyncPoolNV ) );
    }
  };
#endif /*VK_USE_PLATFORM_SCI*/

#if 14 <= VULKAN_HPP_CPP_VERSION
  //======================================
  //=== HASH structures for structures ===
  //======================================

#  if !defined( VULKAN_HPP_HASH_COMBINE )
#    define VULKAN_HPP_HASH_COMBINE( seed, value ) \
      seed ^= std::hash<std::decay<decltype( value )>::type>{}( value ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 )
#  endif

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR const & acquireNextImageInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.swapchain );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.timeout );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.fence );
      VULKAN_HPP_HASH_COMBINE( seed, acquireNextImageInfoKHR.deviceMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR const & acquireProfilingLockInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, acquireProfilingLockInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, acquireProfilingLockInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, acquireProfilingLockInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, acquireProfilingLockInfoKHR.timeout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AllocationCallbacks>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AllocationCallbacks const & allocationCallbacks ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pUserData );
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pfnAllocation );
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pfnReallocation );
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pfnFree );
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pfnInternalAllocation );
      VULKAN_HPP_HASH_COMBINE( seed, allocationCallbacks.pfnInternalFree );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ApplicationInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ApplicationInfo const & applicationInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, applicationInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, applicationInfo.pNext );
      for ( const char * p = applicationInfo.pApplicationName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, applicationInfo.applicationVersion );
      for ( const char * p = applicationInfo.pEngineName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, applicationInfo.engineVersion );
      VULKAN_HPP_HASH_COMBINE( seed, applicationInfo.apiVersion );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ApplicationParametersEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ApplicationParametersEXT const & applicationParametersEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.vendorID );
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.deviceID );
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.key );
      VULKAN_HPP_HASH_COMBINE( seed, applicationParametersEXT.value );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentDescription>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentDescription const & attachmentDescription ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.flags );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.format );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.samples );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.loadOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.storeOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.stencilLoadOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.stencilStoreOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.initialLayout );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription.finalLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentDescription2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentDescription2 const & attachmentDescription2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.flags );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.format );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.samples );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.loadOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.storeOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.stencilLoadOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.stencilStoreOp );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.initialLayout );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescription2.finalLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout const & attachmentDescriptionStencilLayout ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescriptionStencilLayout.sType );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescriptionStencilLayout.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescriptionStencilLayout.stencilInitialLayout );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentDescriptionStencilLayout.stencilFinalLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentReference>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentReference const & attachmentReference ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference.attachment );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference.layout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentReference2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentReference2 const & attachmentReference2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference2.attachment );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference2.layout );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReference2.aspectMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout const & attachmentReferenceStencilLayout ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReferenceStencilLayout.sType );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReferenceStencilLayout.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentReferenceStencilLayout.stencilLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Extent2D>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Extent2D const & extent2D ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, extent2D.width );
      VULKAN_HPP_HASH_COMBINE( seed, extent2D.height );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SampleLocationEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SampleLocationEXT const & sampleLocationEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationEXT.x );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationEXT.y );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT const & sampleLocationsInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.sampleLocationsPerPixel );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.sampleLocationGridSize );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.sampleLocationsCount );
      VULKAN_HPP_HASH_COMBINE( seed, sampleLocationsInfoEXT.pSampleLocations );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT const & attachmentSampleLocationsEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, attachmentSampleLocationsEXT.attachmentIndex );
      VULKAN_HPP_HASH_COMBINE( seed, attachmentSampleLocationsEXT.sampleLocationsInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BaseInStructure>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BaseInStructure const & baseInStructure ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, baseInStructure.sType );
      VULKAN_HPP_HASH_COMBINE( seed, baseInStructure.pNext );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BaseOutStructure>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BaseOutStructure const & baseOutStructure ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, baseOutStructure.sType );
      VULKAN_HPP_HASH_COMBINE( seed, baseOutStructure.pNext );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo const & bindBufferMemoryDeviceGroupInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryDeviceGroupInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryDeviceGroupInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryDeviceGroupInfo.deviceIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryDeviceGroupInfo.pDeviceIndices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo const & bindBufferMemoryInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryInfo.buffer );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryInfo.memory );
      VULKAN_HPP_HASH_COMBINE( seed, bindBufferMemoryInfo.memoryOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Offset2D>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Offset2D const & offset2D ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, offset2D.x );
      VULKAN_HPP_HASH_COMBINE( seed, offset2D.y );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Rect2D>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Rect2D const & rect2D ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, rect2D.offset );
      VULKAN_HPP_HASH_COMBINE( seed, rect2D.extent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo const & bindImageMemoryDeviceGroupInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.deviceIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.pDeviceIndices );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.splitInstanceBindRegionCount );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryDeviceGroupInfo.pSplitInstanceBindRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindImageMemoryInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindImageMemoryInfo const & bindImageMemoryInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryInfo.image );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryInfo.memory );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemoryInfo.memoryOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR const & bindImageMemorySwapchainInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemorySwapchainInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemorySwapchainInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemorySwapchainInfoKHR.swapchain );
      VULKAN_HPP_HASH_COMBINE( seed, bindImageMemorySwapchainInfoKHR.imageIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo const & bindImagePlaneMemoryInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bindImagePlaneMemoryInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bindImagePlaneMemoryInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bindImagePlaneMemoryInfo.planeAspect );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageSubresourceLayers>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageSubresourceLayers const & imageSubresourceLayers ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceLayers.aspectMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceLayers.mipLevel );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceLayers.baseArrayLayer );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceLayers.layerCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Offset3D>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Offset3D const & offset3D ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, offset3D.x );
      VULKAN_HPP_HASH_COMBINE( seed, offset3D.y );
      VULKAN_HPP_HASH_COMBINE( seed, offset3D.z );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageBlit2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageBlit2 const & imageBlit2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.srcSubresource );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.srcOffsets[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.dstSubresource );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, imageBlit2.dstOffsets[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BlitImageInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BlitImageInfo2 const & blitImageInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.srcImage );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.srcImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.dstImage );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.dstImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.pRegions );
      VULKAN_HPP_HASH_COMBINE( seed, blitImageInfo2.filter );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferCopy>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferCopy const & bufferCopy ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferCopy2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferCopy2 const & bufferCopy2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy2.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy2.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCopy2.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferCreateInfo const & bufferCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.size );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.usage );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.sharingMode );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.queueFamilyIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, bufferCreateInfo.pQueueFamilyIndices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo const & bufferDeviceAddressInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferDeviceAddressInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferDeviceAddressInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferDeviceAddressInfo.buffer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Extent3D>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Extent3D const & extent3D ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, extent3D.width );
      VULKAN_HPP_HASH_COMBINE( seed, extent3D.height );
      VULKAN_HPP_HASH_COMBINE( seed, extent3D.depth );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferImageCopy>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferImageCopy const & bufferImageCopy ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.bufferOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.bufferRowLength );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.bufferImageHeight );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.imageSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.imageOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy.imageExtent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferImageCopy2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferImageCopy2 const & bufferImageCopy2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.bufferOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.bufferRowLength );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.bufferImageHeight );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.imageSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.imageOffset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferImageCopy2.imageExtent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferMemoryBarrier const & bufferMemoryBarrier ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.srcQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.dstQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.buffer );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.offset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2 const & bufferMemoryBarrier2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.srcStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.dstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.srcQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.dstQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.buffer );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.offset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryBarrier2.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 const & bufferMemoryRequirementsInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryRequirementsInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryRequirementsInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferMemoryRequirementsInfo2.buffer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo const & bufferOpaqueCaptureAddressCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferOpaqueCaptureAddressCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferOpaqueCaptureAddressCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferOpaqueCaptureAddressCreateInfo.opaqueCaptureAddress );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::BufferViewCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::BufferViewCreateInfo const & bufferViewCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.buffer );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.format );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.offset );
      VULKAN_HPP_HASH_COMBINE( seed, bufferViewCreateInfo.range );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CheckpointData2NV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CheckpointData2NV const & checkpointData2NV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, checkpointData2NV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, checkpointData2NV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, checkpointData2NV.stage );
      VULKAN_HPP_HASH_COMBINE( seed, checkpointData2NV.pCheckpointMarker );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ClearDepthStencilValue>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ClearDepthStencilValue const & clearDepthStencilValue ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, clearDepthStencilValue.depth );
      VULKAN_HPP_HASH_COMBINE( seed, clearDepthStencilValue.stencil );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ClearRect>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ClearRect const & clearRect ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, clearRect.rect );
      VULKAN_HPP_HASH_COMBINE( seed, clearRect.baseArrayLayer );
      VULKAN_HPP_HASH_COMBINE( seed, clearRect.layerCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo const & commandBufferAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferAllocateInfo.commandPool );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferAllocateInfo.level );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferAllocateInfo.commandBufferCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo const & commandBufferInheritanceInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.renderPass );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.subpass );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.framebuffer );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.occlusionQueryEnable );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.queryFlags );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceInfo.pipelineStatistics );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo const & commandBufferBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferBeginInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferBeginInfo.pInheritanceInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo const & commandBufferInheritanceRenderingInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.viewMask );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.colorAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.pColorAttachmentFormats );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.depthAttachmentFormat );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.stencilAttachmentFormat );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferInheritanceRenderingInfo.rasterizationSamples );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo const & commandBufferSubmitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferSubmitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferSubmitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferSubmitInfo.commandBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, commandBufferSubmitInfo.deviceMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo const & commandPoolCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolCreateInfo.queueFamilyIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption const & commandPoolMemoryConsumption ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryConsumption.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryConsumption.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryConsumption.commandPoolAllocated );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryConsumption.commandPoolReservedSize );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryConsumption.commandBufferAllocated );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo const & commandPoolMemoryReservationCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryReservationCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryReservationCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryReservationCreateInfo.commandPoolReservedSize );
      VULKAN_HPP_HASH_COMBINE( seed, commandPoolMemoryReservationCreateInfo.commandPoolMaxCommandBuffers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ComponentMapping>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ComponentMapping const & componentMapping ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, componentMapping.r );
      VULKAN_HPP_HASH_COMBINE( seed, componentMapping.g );
      VULKAN_HPP_HASH_COMBINE( seed, componentMapping.b );
      VULKAN_HPP_HASH_COMBINE( seed, componentMapping.a );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SpecializationMapEntry>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SpecializationMapEntry const & specializationMapEntry ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, specializationMapEntry.constantID );
      VULKAN_HPP_HASH_COMBINE( seed, specializationMapEntry.offset );
      VULKAN_HPP_HASH_COMBINE( seed, specializationMapEntry.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SpecializationInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SpecializationInfo const & specializationInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, specializationInfo.mapEntryCount );
      VULKAN_HPP_HASH_COMBINE( seed, specializationInfo.pMapEntries );
      VULKAN_HPP_HASH_COMBINE( seed, specializationInfo.dataSize );
      VULKAN_HPP_HASH_COMBINE( seed, specializationInfo.pData );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo const & pipelineShaderStageCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.stage );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.module );
      for ( const char * p = pipelineShaderStageCreateInfo.pName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageCreateInfo.pSpecializationInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo const & computePipelineCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.stage );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.layout );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.basePipelineHandle );
      VULKAN_HPP_HASH_COMBINE( seed, computePipelineCreateInfo.basePipelineIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ConformanceVersion>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ConformanceVersion const & conformanceVersion ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, conformanceVersion.major );
      VULKAN_HPP_HASH_COMBINE( seed, conformanceVersion.minor );
      VULKAN_HPP_HASH_COMBINE( seed, conformanceVersion.subminor );
      VULKAN_HPP_HASH_COMBINE( seed, conformanceVersion.patch );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CopyBufferInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CopyBufferInfo2 const & copyBufferInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.srcBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.dstBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferInfo2.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 const & copyBufferToImageInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.srcBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.dstImage );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.dstImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, copyBufferToImageInfo2.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CopyDescriptorSet>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CopyDescriptorSet const & copyDescriptorSet ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.sType );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.srcSet );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.srcBinding );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.srcArrayElement );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.dstSet );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.dstBinding );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.dstArrayElement );
      VULKAN_HPP_HASH_COMBINE( seed, copyDescriptorSet.descriptorCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageCopy2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageCopy2 const & imageCopy2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.srcSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.dstSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy2.extent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CopyImageInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CopyImageInfo2 const & copyImageInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.srcImage );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.srcImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.dstImage );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.dstImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageInfo2.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 const & copyImageToBufferInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.srcImage );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.srcImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.dstBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, copyImageToBufferInfo2.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT const & debugUtilsLabelEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsLabelEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsLabelEXT.pNext );
      for ( const char * p = debugUtilsLabelEXT.pLabelName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      for ( size_t i = 0; i < 4; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, debugUtilsLabelEXT.color[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT const & debugUtilsObjectNameInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectNameInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectNameInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectNameInfoEXT.objectType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectNameInfoEXT.objectHandle );
      for ( const char * p = debugUtilsObjectNameInfoEXT.pObjectName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT const & debugUtilsMessengerCallbackDataEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.flags );
      for ( const char * p = debugUtilsMessengerCallbackDataEXT.pMessageIdName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.messageIdNumber );
      for ( const char * p = debugUtilsMessengerCallbackDataEXT.pMessage; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.queueLabelCount );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.pQueueLabels );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.cmdBufLabelCount );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.pCmdBufLabels );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.objectCount );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCallbackDataEXT.pObjects );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT const & debugUtilsMessengerCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.flags );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.messageSeverity );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.messageType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.pfnUserCallback );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsMessengerCreateInfoEXT.pUserData );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT const & debugUtilsObjectTagInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.objectType );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.objectHandle );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.tagName );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.tagSize );
      VULKAN_HPP_HASH_COMBINE( seed, debugUtilsObjectTagInfoEXT.pTag );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryBarrier2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryBarrier2 const & memoryBarrier2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.srcStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.dstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier2.dstAccessMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageSubresourceRange>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageSubresourceRange const & imageSubresourceRange ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceRange.aspectMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceRange.baseMipLevel );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceRange.levelCount );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceRange.baseArrayLayer );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresourceRange.layerCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2 const & imageMemoryBarrier2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.srcStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.dstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.oldLayout );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.newLayout );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.srcQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.dstQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.image );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier2.subresourceRange );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DependencyInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DependencyInfo const & dependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.dependencyFlags );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.memoryBarrierCount );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.pMemoryBarriers );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.bufferMemoryBarrierCount );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.pBufferMemoryBarriers );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.imageMemoryBarrierCount );
      VULKAN_HPP_HASH_COMBINE( seed, dependencyInfo.pImageMemoryBarriers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorBufferInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorBufferInfo const & descriptorBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorBufferInfo.buffer );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorBufferInfo.offset );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorBufferInfo.range );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorImageInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorImageInfo const & descriptorImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorImageInfo.sampler );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorImageInfo.imageView );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorImageInfo.imageLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorPoolSize>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorPoolSize const & descriptorPoolSize ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolSize.type );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolSize.descriptorCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo const & descriptorPoolCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.maxSets );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.poolSizeCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolCreateInfo.pPoolSizes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo const & descriptorPoolInlineUniformBlockCreateInfo ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolInlineUniformBlockCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolInlineUniformBlockCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorPoolInlineUniformBlockCreateInfo.maxInlineUniformBlockBindings );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo const & descriptorSetAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetAllocateInfo.descriptorPool );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetAllocateInfo.descriptorSetCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetAllocateInfo.pSetLayouts );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding const & descriptorSetLayoutBinding ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBinding.binding );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBinding.descriptorType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBinding.descriptorCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBinding.stageFlags );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBinding.pImmutableSamplers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo const & descriptorSetLayoutBindingFlagsCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBindingFlagsCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBindingFlagsCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBindingFlagsCreateInfo.bindingCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutBindingFlagsCreateInfo.pBindingFlags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo const & descriptorSetLayoutCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutCreateInfo.bindingCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutCreateInfo.pBindings );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport const & descriptorSetLayoutSupport ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutSupport.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutSupport.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetLayoutSupport.supported );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo const & descriptorSetVariableDescriptorCountAllocateInfo )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountAllocateInfo.descriptorSetCount );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountAllocateInfo.pDescriptorCounts );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport const & descriptorSetVariableDescriptorCountLayoutSupport )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountLayoutSupport.sType );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountLayoutSupport.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, descriptorSetVariableDescriptorCountLayoutSupport.maxVariableDescriptorCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements const & deviceBufferMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceBufferMemoryRequirements.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceBufferMemoryRequirements.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceBufferMemoryRequirements.pCreateInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo const & deviceQueueCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.queueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.queueCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueCreateInfo.pQueuePriorities );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures const & physicalDeviceFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.robustBufferAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.fullDrawIndexUint32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.imageCubeArray );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.independentBlend );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.geometryShader );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.tessellationShader );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sampleRateShading );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.dualSrcBlend );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.logicOp );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.multiDrawIndirect );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.drawIndirectFirstInstance );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.depthClamp );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.depthBiasClamp );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.fillModeNonSolid );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.depthBounds );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.wideLines );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.largePoints );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.alphaToOne );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.multiViewport );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.samplerAnisotropy );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.textureCompressionETC2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.textureCompressionASTC_LDR );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.textureCompressionBC );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.occlusionQueryPrecise );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.pipelineStatisticsQuery );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.vertexPipelineStoresAndAtomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.fragmentStoresAndAtomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderTessellationAndGeometryPointSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderImageGatherExtended );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageImageExtendedFormats );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageImageMultisample );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageImageReadWithoutFormat );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderClipDistance );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderCullDistance );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderInt64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderInt16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderResourceResidency );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.shaderResourceMinLod );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseBinding );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidencyBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidencyImage2D );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidencyImage3D );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidency2Samples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidency4Samples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidency8Samples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidency16Samples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.sparseResidencyAliased );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.variableMultisampleRate );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures.inheritedQueries );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceCreateInfo const & deviceCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.queueCreateInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.pQueueCreateInfos );
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.enabledLayerCount );
      for ( size_t i = 0; i < deviceCreateInfo.enabledLayerCount; ++i )
      {
        for ( const char * p = deviceCreateInfo.ppEnabledLayerNames[i]; *p != '\0'; ++p )
        {
          VULKAN_HPP_HASH_COMBINE( seed, *p );
        }
      }
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.enabledExtensionCount );
      for ( size_t i = 0; i < deviceCreateInfo.enabledExtensionCount; ++i )
      {
        for ( const char * p = deviceCreateInfo.ppEnabledExtensionNames[i]; *p != '\0'; ++p )
        {
          VULKAN_HPP_HASH_COMBINE( seed, *p );
        }
      }
      VULKAN_HPP_HASH_COMBINE( seed, deviceCreateInfo.pEnabledFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT const & deviceEventInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceEventInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceEventInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceEventInfoEXT.deviceEvent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo const & deviceGroupCommandBufferBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupCommandBufferBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupCommandBufferBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupCommandBufferBeginInfo.deviceMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo const & deviceGroupDeviceCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupDeviceCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupDeviceCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupDeviceCreateInfo.physicalDeviceCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupDeviceCreateInfo.pPhysicalDevices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR const & deviceGroupPresentCapabilitiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentCapabilitiesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentCapabilitiesKHR.pNext );
      for ( size_t i = 0; i < VK_MAX_DEVICE_GROUP_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentCapabilitiesKHR.presentMask[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentCapabilitiesKHR.modes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR const & deviceGroupPresentInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentInfoKHR.swapchainCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentInfoKHR.pDeviceMasks );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupPresentInfoKHR.mode );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo const & deviceGroupRenderPassBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupRenderPassBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupRenderPassBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupRenderPassBeginInfo.deviceMask );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupRenderPassBeginInfo.deviceRenderAreaCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupRenderPassBeginInfo.pDeviceRenderAreas );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo const & deviceGroupSubmitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.waitSemaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.pWaitSemaphoreDeviceIndices );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.commandBufferCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.pCommandBufferDeviceMasks );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.signalSemaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSubmitInfo.pSignalSemaphoreDeviceIndices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR const & deviceGroupSwapchainCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSwapchainCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSwapchainCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceGroupSwapchainCreateInfoKHR.modes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageCreateInfo const & imageCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.imageType );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.format );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.extent );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.mipLevels );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.arrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.samples );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.tiling );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.usage );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.sharingMode );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.queueFamilyIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.pQueueFamilyIndices );
      VULKAN_HPP_HASH_COMBINE( seed, imageCreateInfo.initialLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements const & deviceImageMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceImageMemoryRequirements.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceImageMemoryRequirements.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceImageMemoryRequirements.pCreateInfo );
      VULKAN_HPP_HASH_COMBINE( seed, deviceImageMemoryRequirements.planeAspect );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo const & deviceMemoryOpaqueCaptureAddressInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceMemoryOpaqueCaptureAddressInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceMemoryOpaqueCaptureAddressInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceMemoryOpaqueCaptureAddressInfo.memory );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo const & pipelineCacheCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheCreateInfo.initialDataSize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheCreateInfo.pInitialData );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelinePoolSize>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelinePoolSize const & pipelinePoolSize ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelinePoolSize.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelinePoolSize.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelinePoolSize.poolEntrySize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelinePoolSize.poolEntryCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo const & deviceObjectReservationCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pipelineCacheCreateInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pPipelineCacheCreateInfos );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pipelinePoolSizeCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pPipelinePoolSizes );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.semaphoreRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.commandBufferRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.fenceRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.deviceMemoryRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.bufferRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.imageRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.eventRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.queryPoolRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.bufferViewRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.imageViewRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.layeredImageViewRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pipelineCacheRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.pipelineLayoutRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.renderPassRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.graphicsPipelineRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.computePipelineRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.descriptorSetLayoutRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.samplerRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.descriptorPoolRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.descriptorSetRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.framebufferRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.commandPoolRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.samplerYcbcrConversionRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.surfaceRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.swapchainRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.displayModeRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.subpassDescriptionRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.attachmentDescriptionRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.descriptorSetLayoutBindingRequestCount );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.descriptorSetLayoutBindingLimit );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxImageViewMipLevels );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxImageViewArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxLayeredImageViewMipLevels );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxOcclusionQueriesPerPool );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxPipelineStatisticsQueriesPerPool );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxTimestampQueriesPerPool );
      VULKAN_HPP_HASH_COMBINE( seed, deviceObjectReservationCreateInfo.maxImmutableSamplersPerDescriptorSetLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo const & devicePrivateDataCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, devicePrivateDataCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, devicePrivateDataCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, devicePrivateDataCreateInfo.privateDataSlotRequestCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceQueueInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 const & deviceQueueInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueInfo2.flags );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueInfo2.queueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, deviceQueueInfo2.queueIndex );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV const & deviceSemaphoreSciSyncPoolReservationCreateInfoNV )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, deviceSemaphoreSciSyncPoolReservationCreateInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, deviceSemaphoreSciSyncPoolReservationCreateInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, deviceSemaphoreSciSyncPoolReservationCreateInfoNV.semaphoreSciSyncPoolRequestCount );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DispatchIndirectCommand>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DispatchIndirectCommand const & dispatchIndirectCommand ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, dispatchIndirectCommand.x );
      VULKAN_HPP_HASH_COMBINE( seed, dispatchIndirectCommand.y );
      VULKAN_HPP_HASH_COMBINE( seed, dispatchIndirectCommand.z );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT const & displayEventInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayEventInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayEventInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayEventInfoEXT.displayEvent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR const & displayModeParametersKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayModeParametersKHR.visibleRegion );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeParametersKHR.refreshRate );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR const & displayModeCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayModeCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeCreateInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeCreateInfoKHR.parameters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR const & displayModePropertiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayModePropertiesKHR.displayMode );
      VULKAN_HPP_HASH_COMBINE( seed, displayModePropertiesKHR.parameters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR const & displayModeProperties2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayModeProperties2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeProperties2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayModeProperties2KHR.displayModeProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR const & displayPlaneCapabilitiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.supportedAlpha );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.minSrcPosition );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.maxSrcPosition );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.minSrcExtent );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.maxSrcExtent );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.minDstPosition );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.maxDstPosition );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.minDstExtent );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilitiesKHR.maxDstExtent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR const & displayPlaneCapabilities2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilities2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilities2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneCapabilities2KHR.capabilities );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR const & displayPlaneInfo2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneInfo2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneInfo2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneInfo2KHR.mode );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneInfo2KHR.planeIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR const & displayPlanePropertiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPlanePropertiesKHR.currentDisplay );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlanePropertiesKHR.currentStackIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR const & displayPlaneProperties2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneProperties2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneProperties2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayPlaneProperties2KHR.displayPlaneProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT const & displayPowerInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPowerInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayPowerInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayPowerInfoEXT.powerState );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR const & displayPresentInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPresentInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayPresentInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayPresentInfoKHR.srcRect );
      VULKAN_HPP_HASH_COMBINE( seed, displayPresentInfoKHR.dstRect );
      VULKAN_HPP_HASH_COMBINE( seed, displayPresentInfoKHR.persistent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR const & displayPropertiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.display );
      for ( const char * p = displayPropertiesKHR.displayName; *p != '\0'; ++p )
      {
        VULKAN_HPP_HASH_COMBINE( seed, *p );
      }
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.physicalDimensions );
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.physicalResolution );
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.supportedTransforms );
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.planeReorderPossible );
      VULKAN_HPP_HASH_COMBINE( seed, displayPropertiesKHR.persistentContent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplayProperties2KHR const & displayProperties2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displayProperties2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displayProperties2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displayProperties2KHR.displayProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR const & displaySurfaceCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.displayMode );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.planeIndex );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.planeStackIndex );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.transform );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.globalAlpha );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.alphaMode );
      VULKAN_HPP_HASH_COMBINE( seed, displaySurfaceCreateInfoKHR.imageExtent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand const & drawIndexedIndirectCommand ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drawIndexedIndirectCommand.indexCount );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndexedIndirectCommand.instanceCount );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndexedIndirectCommand.firstIndex );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndexedIndirectCommand.vertexOffset );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndexedIndirectCommand.firstInstance );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrawIndirectCommand>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrawIndirectCommand const & drawIndirectCommand ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drawIndirectCommand.vertexCount );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndirectCommand.instanceCount );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndirectCommand.firstVertex );
      VULKAN_HPP_HASH_COMBINE( seed, drawIndirectCommand.firstInstance );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT const & drmFormatModifierProperties2EXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierProperties2EXT.drmFormatModifier );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierProperties2EXT.drmFormatModifierPlaneCount );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierProperties2EXT.drmFormatModifierTilingFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT const & drmFormatModifierPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesEXT.drmFormatModifier );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesEXT.drmFormatModifierPlaneCount );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesEXT.drmFormatModifierTilingFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT const & drmFormatModifierPropertiesList2EXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesList2EXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesList2EXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesList2EXT.drmFormatModifierCount );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesList2EXT.pDrmFormatModifierProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT const & drmFormatModifierPropertiesListEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesListEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesListEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesListEXT.drmFormatModifierCount );
      VULKAN_HPP_HASH_COMBINE( seed, drmFormatModifierPropertiesListEXT.pDrmFormatModifierProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::EventCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::EventCreateInfo const & eventCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, eventCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, eventCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, eventCreateInfo.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo const & exportFenceCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceCreateInfo.handleTypes );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV const & exportFenceSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportFenceSciSyncInfoNV.pAttributes );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo const & exportMemoryAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportMemoryAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportMemoryAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportMemoryAllocateInfo.handleTypes );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV const & exportMemorySciBufInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportMemorySciBufInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportMemorySciBufInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportMemorySciBufInfoNV.pAttributes );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo const & exportSemaphoreCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreCreateInfo.handleTypes );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV const & exportSemaphoreSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, exportSemaphoreSciSyncInfoNV.pAttributes );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExtensionProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExtensionProperties const & extensionProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      for ( size_t i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, extensionProperties.extensionName[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, extensionProperties.specVersion );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalMemoryProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalMemoryProperties const & externalMemoryProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryProperties.externalMemoryFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryProperties.exportFromImportedHandleTypes );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryProperties.compatibleHandleTypes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalBufferProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalBufferProperties const & externalBufferProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalBufferProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalBufferProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalBufferProperties.externalMemoryProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalFenceProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalFenceProperties const & externalFenceProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalFenceProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalFenceProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalFenceProperties.exportFromImportedHandleTypes );
      VULKAN_HPP_HASH_COMBINE( seed, externalFenceProperties.compatibleHandleTypes );
      VULKAN_HPP_HASH_COMBINE( seed, externalFenceProperties.externalFenceFeatures );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalFormatQNX>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalFormatQNX const & externalFormatQNX ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalFormatQNX.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalFormatQNX.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalFormatQNX.externalFormat );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties const & externalImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalImageFormatProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalImageFormatProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalImageFormatProperties.externalMemoryProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo const & externalMemoryBufferCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryBufferCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryBufferCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryBufferCreateInfo.handleTypes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo const & externalMemoryImageCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryImageCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryImageCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalMemoryImageCreateInfo.handleTypes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties const & externalSemaphoreProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, externalSemaphoreProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, externalSemaphoreProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, externalSemaphoreProperties.exportFromImportedHandleTypes );
      VULKAN_HPP_HASH_COMBINE( seed, externalSemaphoreProperties.compatibleHandleTypes );
      VULKAN_HPP_HASH_COMBINE( seed, externalSemaphoreProperties.externalSemaphoreFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FaultData>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FaultData const & faultData ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, faultData.sType );
      VULKAN_HPP_HASH_COMBINE( seed, faultData.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, faultData.faultLevel );
      VULKAN_HPP_HASH_COMBINE( seed, faultData.faultType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FaultCallbackInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FaultCallbackInfo const & faultCallbackInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, faultCallbackInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, faultCallbackInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, faultCallbackInfo.faultCount );
      VULKAN_HPP_HASH_COMBINE( seed, faultCallbackInfo.pFaults );
      VULKAN_HPP_HASH_COMBINE( seed, faultCallbackInfo.pfnFaultCallback );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FenceCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FenceCreateInfo const & fenceCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, fenceCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, fenceCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, fenceCreateInfo.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR const & fenceGetFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetFdInfoKHR.fence );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetFdInfoKHR.handleType );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV const & fenceGetSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetSciSyncInfoNV.fence );
      VULKAN_HPP_HASH_COMBINE( seed, fenceGetSciSyncInfoNV.handleType );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT const & filterCubicImageViewImageFormatPropertiesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, filterCubicImageViewImageFormatPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, filterCubicImageViewImageFormatPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, filterCubicImageViewImageFormatPropertiesEXT.filterCubic );
      VULKAN_HPP_HASH_COMBINE( seed, filterCubicImageViewImageFormatPropertiesEXT.filterCubicMinmax );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FormatProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FormatProperties const & formatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties.linearTilingFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties.optimalTilingFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties.bufferFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FormatProperties2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FormatProperties2 const & formatProperties2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties2.formatProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FormatProperties3>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FormatProperties3 const & formatProperties3 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties3.sType );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties3.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties3.linearTilingFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties3.optimalTilingFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, formatProperties3.bufferFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR const & fragmentShadingRateAttachmentInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, fragmentShadingRateAttachmentInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, fragmentShadingRateAttachmentInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, fragmentShadingRateAttachmentInfoKHR.pFragmentShadingRateAttachment );
      VULKAN_HPP_HASH_COMBINE( seed, fragmentShadingRateAttachmentInfoKHR.shadingRateAttachmentTexelSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo const & framebufferAttachmentImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.usage );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.width );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.height );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.layerCount );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.viewFormatCount );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentImageInfo.pViewFormats );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo const & framebufferAttachmentsCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentsCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentsCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentsCreateInfo.attachmentImageInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferAttachmentsCreateInfo.pAttachmentImageInfos );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::FramebufferCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::FramebufferCreateInfo const & framebufferCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.renderPass );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.pAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.width );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.height );
      VULKAN_HPP_HASH_COMBINE( seed, framebufferCreateInfo.layers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::VertexInputBindingDescription const & vertexInputBindingDescription ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription.binding );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription.stride );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription.inputRate );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription const & vertexInputAttributeDescription ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription.location );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription.binding );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription.format );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription.offset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo const & pipelineVertexInputStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo const & pipelineInputAssemblyStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineInputAssemblyStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineInputAssemblyStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineInputAssemblyStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineInputAssemblyStateCreateInfo.topology );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo const & pipelineTessellationStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationStateCreateInfo.patchControlPoints );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::Viewport>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Viewport const & viewport ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, viewport.x );
      VULKAN_HPP_HASH_COMBINE( seed, viewport.y );
      VULKAN_HPP_HASH_COMBINE( seed, viewport.width );
      VULKAN_HPP_HASH_COMBINE( seed, viewport.height );
      VULKAN_HPP_HASH_COMBINE( seed, viewport.minDepth );
      VULKAN_HPP_HASH_COMBINE( seed, viewport.maxDepth );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo const & pipelineViewportStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.viewportCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.pViewports );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.scissorCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineViewportStateCreateInfo.pScissors );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo const & pipelineRasterizationStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.depthClampEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.polygonMode );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.cullMode );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.frontFace );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.depthBiasEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.depthBiasConstantFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.depthBiasClamp );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationStateCreateInfo.lineWidth );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo const & pipelineMultisampleStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.rasterizationSamples );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.sampleShadingEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.minSampleShading );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.pSampleMask );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.alphaToCoverageEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineMultisampleStateCreateInfo.alphaToOneEnable );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::StencilOpState>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::StencilOpState const & stencilOpState ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.failOp );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.passOp );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.depthFailOp );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.compareOp );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.compareMask );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.writeMask );
      VULKAN_HPP_HASH_COMBINE( seed, stencilOpState.reference );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo const & pipelineDepthStencilStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.depthTestEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.depthWriteEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.depthCompareOp );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.stencilTestEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.front );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.back );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.minDepthBounds );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDepthStencilStateCreateInfo.maxDepthBounds );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState const & pipelineColorBlendAttachmentState ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.blendEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.srcColorBlendFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.dstColorBlendFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.colorBlendOp );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.srcAlphaBlendFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.dstAlphaBlendFactor );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.alphaBlendOp );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAttachmentState.colorWriteMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo const & pipelineColorBlendStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.logicOpEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.logicOp );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.pAttachments );
      for ( size_t i = 0; i < 4; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendStateCreateInfo.blendConstants[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo const & pipelineDynamicStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDynamicStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDynamicStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDynamicStateCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDynamicStateCreateInfo.dynamicStateCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDynamicStateCreateInfo.pDynamicStates );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo const & graphicsPipelineCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.stageCount );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pStages );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pVertexInputState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pInputAssemblyState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pTessellationState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pViewportState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pRasterizationState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pMultisampleState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pDepthStencilState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pColorBlendState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.pDynamicState );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.layout );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.renderPass );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.subpass );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.basePipelineHandle );
      VULKAN_HPP_HASH_COMBINE( seed, graphicsPipelineCreateInfo.basePipelineIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::XYColorEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::XYColorEXT const & xYColorEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, xYColorEXT.x );
      VULKAN_HPP_HASH_COMBINE( seed, xYColorEXT.y );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::HdrMetadataEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::HdrMetadataEXT const & hdrMetadataEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.displayPrimaryRed );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.displayPrimaryGreen );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.displayPrimaryBlue );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.whitePoint );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.maxLuminance );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.minLuminance );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.maxContentLightLevel );
      VULKAN_HPP_HASH_COMBINE( seed, hdrMetadataEXT.maxFrameAverageLightLevel );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT const & headlessSurfaceCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, headlessSurfaceCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, headlessSurfaceCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, headlessSurfaceCreateInfoEXT.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageBlit>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageBlit const & imageBlit ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit.srcSubresource );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, imageBlit.srcOffsets[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, imageBlit.dstSubresource );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, imageBlit.dstOffsets[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageCopy>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageCopy const & imageCopy ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy.srcSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy.dstSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageCopy.extent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubresourceLayout>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubresourceLayout const & subresourceLayout ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subresourceLayout.offset );
      VULKAN_HPP_HASH_COMBINE( seed, subresourceLayout.size );
      VULKAN_HPP_HASH_COMBINE( seed, subresourceLayout.rowPitch );
      VULKAN_HPP_HASH_COMBINE( seed, subresourceLayout.arrayPitch );
      VULKAN_HPP_HASH_COMBINE( seed, subresourceLayout.depthPitch );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT const & imageDrmFormatModifierExplicitCreateInfoEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierExplicitCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierExplicitCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierExplicitCreateInfoEXT.drmFormatModifier );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierExplicitCreateInfoEXT.drmFormatModifierPlaneCount );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierExplicitCreateInfoEXT.pPlaneLayouts );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT const & imageDrmFormatModifierListCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierListCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierListCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierListCreateInfoEXT.drmFormatModifierCount );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierListCreateInfoEXT.pDrmFormatModifiers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT const & imageDrmFormatModifierPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageDrmFormatModifierPropertiesEXT.drmFormatModifier );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo const & imageFormatListCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatListCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatListCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatListCreateInfo.viewFormatCount );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatListCreateInfo.pViewFormats );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageFormatProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageFormatProperties const & imageFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties.maxExtent );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties.maxMipLevels );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties.maxArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties.sampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties.maxResourceSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageFormatProperties2 const & imageFormatProperties2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageFormatProperties2.imageFormatProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageMemoryBarrier const & imageMemoryBarrier ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.oldLayout );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.newLayout );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.srcQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.dstQueueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.image );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryBarrier.subresourceRange );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 const & imageMemoryRequirementsInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryRequirementsInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryRequirementsInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageMemoryRequirementsInfo2.image );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo const & imagePlaneMemoryRequirementsInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imagePlaneMemoryRequirementsInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imagePlaneMemoryRequirementsInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imagePlaneMemoryRequirementsInfo.planeAspect );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageResolve>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageResolve const & imageResolve ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve.srcSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve.dstSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve.extent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageResolve2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageResolve2 const & imageResolve2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.srcSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.srcOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.dstSubresource );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.dstOffset );
      VULKAN_HPP_HASH_COMBINE( seed, imageResolve2.extent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo const & imageStencilUsageCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageStencilUsageCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageStencilUsageCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageStencilUsageCreateInfo.stencilUsage );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageSubresource>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageSubresource const & imageSubresource ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresource.aspectMask );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresource.mipLevel );
      VULKAN_HPP_HASH_COMBINE( seed, imageSubresource.arrayLayer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR const & imageSwapchainCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageSwapchainCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageSwapchainCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageSwapchainCreateInfoKHR.swapchain );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT const & imageViewASTCDecodeModeEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageViewASTCDecodeModeEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewASTCDecodeModeEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewASTCDecodeModeEXT.decodeMode );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageViewCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageViewCreateInfo const & imageViewCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.image );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.viewType );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.format );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.components );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewCreateInfo.subresourceRange );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo const & imageViewUsageCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, imageViewUsageCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewUsageCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, imageViewUsageCreateInfo.usage );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR const & importFenceFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.fence );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceFdInfoKHR.fd );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV const & importFenceSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importFenceSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceSciSyncInfoNV.fence );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceSciSyncInfoNV.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importFenceSciSyncInfoNV.handle );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR const & importMemoryFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryFdInfoKHR.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryFdInfoKHR.fd );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT const & importMemoryHostPointerInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryHostPointerInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryHostPointerInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryHostPointerInfoEXT.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemoryHostPointerInfoEXT.pHostPointer );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV const & importMemorySciBufInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importMemorySciBufInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemorySciBufInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importMemorySciBufInfoNV.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importMemorySciBufInfoNV.handle );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX const & importScreenBufferInfoQNX ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importScreenBufferInfoQNX.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importScreenBufferInfoQNX.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importScreenBufferInfoQNX.buffer );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR const & importSemaphoreFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreFdInfoKHR.fd );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV const & importSemaphoreSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreSciSyncInfoNV.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreSciSyncInfoNV.handleType );
      VULKAN_HPP_HASH_COMBINE( seed, importSemaphoreSciSyncInfoNV.handle );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference const & inputAttachmentAspectReference ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, inputAttachmentAspectReference.subpass );
      VULKAN_HPP_HASH_COMBINE( seed, inputAttachmentAspectReference.inputAttachmentIndex );
      VULKAN_HPP_HASH_COMBINE( seed, inputAttachmentAspectReference.aspectMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::InstanceCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::InstanceCreateInfo const & instanceCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.pApplicationInfo );
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.enabledLayerCount );
      for ( size_t i = 0; i < instanceCreateInfo.enabledLayerCount; ++i )
      {
        for ( const char * p = instanceCreateInfo.ppEnabledLayerNames[i]; *p != '\0'; ++p )
        {
          VULKAN_HPP_HASH_COMBINE( seed, *p );
        }
      }
      VULKAN_HPP_HASH_COMBINE( seed, instanceCreateInfo.enabledExtensionCount );
      for ( size_t i = 0; i < instanceCreateInfo.enabledExtensionCount; ++i )
      {
        for ( const char * p = instanceCreateInfo.ppEnabledExtensionNames[i]; *p != '\0'; ++p )
        {
          VULKAN_HPP_HASH_COMBINE( seed, *p );
        }
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::LayerProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::LayerProperties const & layerProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      for ( size_t i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, layerProperties.layerName[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, layerProperties.specVersion );
      VULKAN_HPP_HASH_COMBINE( seed, layerProperties.implementationVersion );
      for ( size_t i = 0; i < VK_MAX_DESCRIPTION_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, layerProperties.description[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MappedMemoryRange>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MappedMemoryRange const & mappedMemoryRange ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, mappedMemoryRange.sType );
      VULKAN_HPP_HASH_COMBINE( seed, mappedMemoryRange.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, mappedMemoryRange.memory );
      VULKAN_HPP_HASH_COMBINE( seed, mappedMemoryRange.offset );
      VULKAN_HPP_HASH_COMBINE( seed, mappedMemoryRange.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo const & memoryAllocateFlagsInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateFlagsInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateFlagsInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateFlagsInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateFlagsInfo.deviceMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryAllocateInfo const & memoryAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateInfo.allocationSize );
      VULKAN_HPP_HASH_COMBINE( seed, memoryAllocateInfo.memoryTypeIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryBarrier>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryBarrier const & memoryBarrier ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, memoryBarrier.dstAccessMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo const & memoryDedicatedAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedAllocateInfo.image );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedAllocateInfo.buffer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements const & memoryDedicatedRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedRequirements.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedRequirements.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedRequirements.prefersDedicatedAllocation );
      VULKAN_HPP_HASH_COMBINE( seed, memoryDedicatedRequirements.requiresDedicatedAllocation );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR const & memoryFdPropertiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryFdPropertiesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryFdPropertiesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryFdPropertiesKHR.memoryTypeBits );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR const & memoryGetFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetFdInfoKHR.memory );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetFdInfoKHR.handleType );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV const & memoryGetSciBufInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetSciBufInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetSciBufInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetSciBufInfoNV.memory );
      VULKAN_HPP_HASH_COMBINE( seed, memoryGetSciBufInfoNV.handleType );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryHeap>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryHeap const & memoryHeap ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryHeap.size );
      VULKAN_HPP_HASH_COMBINE( seed, memoryHeap.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT const & memoryHostPointerPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryHostPointerPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryHostPointerPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryHostPointerPropertiesEXT.memoryTypeBits );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo const & memoryOpaqueCaptureAddressAllocateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryOpaqueCaptureAddressAllocateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryOpaqueCaptureAddressAllocateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryOpaqueCaptureAddressAllocateInfo.opaqueCaptureAddress );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryRequirements>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryRequirements const & memoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements.size );
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements.alignment );
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements.memoryTypeBits );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryRequirements2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryRequirements2 const & memoryRequirements2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memoryRequirements2.memoryRequirements );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV const & memorySciBufPropertiesNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memorySciBufPropertiesNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, memorySciBufPropertiesNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, memorySciBufPropertiesNV.memoryTypeBits );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MemoryType>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MemoryType const & memoryType ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, memoryType.propertyFlags );
      VULKAN_HPP_HASH_COMBINE( seed, memoryType.heapIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT const & multisamplePropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, multisamplePropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, multisamplePropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, multisamplePropertiesEXT.maxSampleLocationGridSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR const & performanceCounterDescriptionKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.flags );
      for ( size_t i = 0; i < VK_MAX_DESCRIPTION_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.name[i] );
      }
      for ( size_t i = 0; i < VK_MAX_DESCRIPTION_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.category[i] );
      }
      for ( size_t i = 0; i < VK_MAX_DESCRIPTION_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, performanceCounterDescriptionKHR.description[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PerformanceCounterKHR const & performanceCounterKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.unit );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.scope );
      VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.storage );
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, performanceCounterKHR.uuid[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR const & performanceQueryReservationInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, performanceQueryReservationInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, performanceQueryReservationInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, performanceQueryReservationInfoKHR.maxPerformanceQueriesPerPool );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR const & performanceQuerySubmitInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, performanceQuerySubmitInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, performanceQuerySubmitInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, performanceQuerySubmitInfoKHR.counterPassIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures const & physicalDevice16BitStorageFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.storageBuffer16BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.uniformAndStorageBuffer16BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.storagePushConstant16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice16BitStorageFeatures.storageInputOutput16 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT const & physicalDevice4444FormatsFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice4444FormatsFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice4444FormatsFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice4444FormatsFeaturesEXT.formatA4R4G4B4 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice4444FormatsFeaturesEXT.formatA4B4G4R4 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures const & physicalDevice8BitStorageFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice8BitStorageFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice8BitStorageFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice8BitStorageFeatures.storageBuffer8BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice8BitStorageFeatures.uniformAndStorageBuffer8BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevice8BitStorageFeatures.storagePushConstant8 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT const & physicalDeviceASTCDecodeFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceASTCDecodeFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceASTCDecodeFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceASTCDecodeFeaturesEXT.decodeModeSharedExponent );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT const & physicalDeviceBlendOperationAdvancedFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedFeaturesEXT.advancedBlendCoherentOperations );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT const & physicalDeviceBlendOperationAdvancedPropertiesEXT )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendMaxColorAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendIndependentBlend );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendNonPremultipliedSrcColor );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendNonPremultipliedDstColor );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendCorrelatedOverlap );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBlendOperationAdvancedPropertiesEXT.advancedBlendAllOperations );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures const & physicalDeviceBufferDeviceAddressFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBufferDeviceAddressFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBufferDeviceAddressFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBufferDeviceAddressFeatures.bufferDeviceAddress );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceBufferDeviceAddressFeatures.bufferDeviceAddressMultiDevice );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT const & physicalDeviceColorWriteEnableFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceColorWriteEnableFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceColorWriteEnableFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceColorWriteEnableFeaturesEXT.colorWriteEnable );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT const &
                              physicalDeviceConservativeRasterizationPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.primitiveOverestimationSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.maxExtraPrimitiveOverestimationSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.extraPrimitiveOverestimationSizeGranularity );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.primitiveUnderestimation );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.conservativePointAndLineRasterization );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.degenerateTrianglesRasterized );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.degenerateLinesRasterized );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.fullyCoveredFragmentShaderInputVariable );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceConservativeRasterizationPropertiesEXT.conservativeRasterizationPostDepthCoverage );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT const & physicalDeviceCustomBorderColorFeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorFeaturesEXT.customBorderColors );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorFeaturesEXT.customBorderColorWithoutFormat );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT const & physicalDeviceCustomBorderColorPropertiesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceCustomBorderColorPropertiesEXT.maxCustomBorderColorSamplers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT const & physicalDeviceDepthClipEnableFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthClipEnableFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthClipEnableFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthClipEnableFeaturesEXT.depthClipEnable );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties const & physicalDeviceDepthStencilResolveProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.supportedDepthResolveModes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.supportedStencilResolveModes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.independentResolveNone );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDepthStencilResolveProperties.independentResolve );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures const & physicalDeviceDescriptorIndexingFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderInputAttachmentArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderUniformTexelBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderStorageTexelBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderStorageImageArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderInputAttachmentArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderUniformTexelBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.shaderStorageTexelBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingPartiallyBound );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.descriptorBindingVariableDescriptorCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingFeatures.runtimeDescriptorArray );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties const & physicalDeviceDescriptorIndexingProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxUpdateAfterBindDescriptorsInAllPools );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.shaderUniformBufferArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.shaderSampledImageArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.shaderStorageBufferArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.shaderStorageImageArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.shaderInputAttachmentArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.robustBufferAccessUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.quadDivergentImplicitLod );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxPerStageUpdateAfterBindResources );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDescriptorIndexingProperties.maxDescriptorSetUpdateAfterBindInputAttachments );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT const & physicalDeviceDiscardRectanglePropertiesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDiscardRectanglePropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDiscardRectanglePropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDiscardRectanglePropertiesEXT.maxDiscardRectangles );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties const & physicalDeviceDriverProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.driverID );
      for ( size_t i = 0; i < VK_MAX_DRIVER_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.driverName[i] );
      }
      for ( size_t i = 0; i < VK_MAX_DRIVER_INFO_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.driverInfo[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDriverProperties.conformanceVersion );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures const & physicalDeviceDynamicRenderingFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDynamicRenderingFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDynamicRenderingFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceDynamicRenderingFeatures.dynamicRendering );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT const & physicalDeviceExtendedDynamicState2FeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicState2FeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicState2FeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicState2FeaturesEXT.extendedDynamicState2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicState2FeaturesEXT.extendedDynamicState2LogicOp );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicState2FeaturesEXT.extendedDynamicState2PatchControlPoints );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT const & physicalDeviceExtendedDynamicStateFeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicStateFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicStateFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExtendedDynamicStateFeaturesEXT.extendedDynamicState );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo const & physicalDeviceExternalBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalBufferInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalBufferInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalBufferInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalBufferInfo.usage );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalBufferInfo.handleType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo const & physicalDeviceExternalFenceInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalFenceInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalFenceInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalFenceInfo.handleType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo const & physicalDeviceExternalImageFormatInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalImageFormatInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalImageFormatInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalImageFormatInfo.handleType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT const & physicalDeviceExternalMemoryHostPropertiesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryHostPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryHostPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryHostPropertiesEXT.minImportedHostPointerAlignment );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV const & physicalDeviceExternalMemorySciBufFeaturesNV ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemorySciBufFeaturesNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemorySciBufFeaturesNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemorySciBufFeaturesNV.sciBufImport );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemorySciBufFeaturesNV.sciBufExport );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX const & physicalDeviceExternalMemoryScreenBufferFeaturesQNX ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryScreenBufferFeaturesQNX.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryScreenBufferFeaturesQNX.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalMemoryScreenBufferFeaturesQNX.screenBufferImport );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV const & physicalDeviceExternalSciSync2FeaturesNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.sciSyncFence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.sciSyncSemaphore2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.sciSyncImport );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSync2FeaturesNV.sciSyncExport );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV const & physicalDeviceExternalSciSyncFeaturesNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.sciSyncFence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.sciSyncSemaphore );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.sciSyncImport );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSciSyncFeaturesNV.sciSyncExport );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo const & physicalDeviceExternalSemaphoreInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSemaphoreInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSemaphoreInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceExternalSemaphoreInfo.handleType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 const & physicalDeviceFeatures2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFeatures2.features );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties const & physicalDeviceFloatControlsProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.denormBehaviorIndependence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.roundingModeIndependence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderSignedZeroInfNanPreserveFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderSignedZeroInfNanPreserveFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderSignedZeroInfNanPreserveFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormPreserveFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormPreserveFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormPreserveFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormFlushToZeroFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormFlushToZeroFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderDenormFlushToZeroFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTEFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTEFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTEFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTZFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTZFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFloatControlsProperties.shaderRoundingModeRTZFloat64 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT const & physicalDeviceFragmentShaderInterlockFeaturesEXT )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShaderInterlockFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShaderInterlockFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShaderInterlockFeaturesEXT.fragmentShaderSampleInterlock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShaderInterlockFeaturesEXT.fragmentShaderPixelInterlock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShaderInterlockFeaturesEXT.fragmentShaderShadingRateInterlock );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR const & physicalDeviceFragmentShadingRateFeaturesKHR ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateFeaturesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateFeaturesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateFeaturesKHR.pipelineFragmentShadingRate );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateFeaturesKHR.primitiveFragmentShadingRate );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateFeaturesKHR.attachmentFragmentShadingRate );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR const & physicalDeviceFragmentShadingRateKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateKHR.sampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRateKHR.fragmentSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR const & physicalDeviceFragmentShadingRatePropertiesKHR ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.minFragmentShadingRateAttachmentTexelSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentShadingRateAttachmentTexelSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentShadingRateAttachmentTexelSizeAspectRatio );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.primitiveFragmentShadingRateWithMultipleViewports );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.layeredShadingRateAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateNonTrivialCombinerOps );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentSizeAspectRatio );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentShadingRateCoverageSamples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.maxFragmentShadingRateRasterizationSamples );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithShaderDepthStencilWrites );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithSampleMask );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithShaderSampleMask );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithConservativeRasterization );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithFragmentShaderInterlock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateWithCustomSampleLocations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceFragmentShadingRatePropertiesKHR.fragmentShadingRateStrictMultiplyCombiner );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties const & physicalDeviceGroupProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceGroupProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceGroupProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceGroupProperties.physicalDeviceCount );
      for ( size_t i = 0; i < VK_MAX_DEVICE_GROUP_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceGroupProperties.physicalDevices[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceGroupProperties.subsetAllocation );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures const & physicalDeviceHostQueryResetFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceHostQueryResetFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceHostQueryResetFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceHostQueryResetFeatures.hostQueryReset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties const & physicalDeviceIDProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.pNext );
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.deviceUUID[i] );
      }
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.driverUUID[i] );
      }
      for ( size_t i = 0; i < VK_LUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.deviceLUID[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.deviceNodeMask );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceIDProperties.deviceLUIDValid );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT const & physicalDeviceImageDrmFormatModifierInfoEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.drmFormatModifier );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.sharingMode );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.queueFamilyIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageDrmFormatModifierInfoEXT.pQueueFamilyIndices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 const & physicalDeviceImageFormatInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.format );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.type );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.tiling );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.usage );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageFormatInfo2.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures const & physicalDeviceImageRobustnessFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageRobustnessFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageRobustnessFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageRobustnessFeatures.robustImageAccess );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT const & physicalDeviceImageViewImageFormatInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageViewImageFormatInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageViewImageFormatInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImageViewImageFormatInfoEXT.imageViewType );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures const & physicalDeviceImagelessFramebufferFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImagelessFramebufferFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImagelessFramebufferFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceImagelessFramebufferFeatures.imagelessFramebuffer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures const & physicalDeviceInlineUniformBlockFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockFeatures.inlineUniformBlock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockFeatures.descriptorBindingInlineUniformBlockUpdateAfterBind );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties const & physicalDeviceInlineUniformBlockProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.maxInlineUniformBlockSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.maxPerStageDescriptorInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.maxDescriptorSetInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceInlineUniformBlockProperties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits const & physicalDeviceLimits ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxImageDimension1D );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxImageDimension2D );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxImageDimension3D );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxImageDimensionCube );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxImageArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTexelBufferElements );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxUniformBufferRange );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxStorageBufferRange );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPushConstantsSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxMemoryAllocationCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxSamplerAllocationCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.bufferImageGranularity );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.sparseAddressSpaceSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxBoundDescriptorSets );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageDescriptorInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxPerStageResources );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetUniformBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetStorageBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDescriptorSetInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxVertexInputAttributes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxVertexInputBindings );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxVertexInputAttributeOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxVertexInputBindingStride );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxVertexOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationGenerationLevel );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationPatchSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationControlPerVertexInputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationControlPerVertexOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationControlPerPatchOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationControlTotalOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationEvaluationInputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTessellationEvaluationOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxGeometryShaderInvocations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxGeometryInputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxGeometryOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxGeometryOutputVertices );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxGeometryTotalOutputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFragmentInputComponents );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFragmentOutputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFragmentDualSrcAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFragmentCombinedOutputResources );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxComputeSharedMemorySize );
      for ( size_t i = 0; i < 3; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxComputeWorkGroupCount[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxComputeWorkGroupInvocations );
      for ( size_t i = 0; i < 3; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxComputeWorkGroupSize[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.subPixelPrecisionBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.subTexelPrecisionBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.mipmapPrecisionBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDrawIndexedIndexValue );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxDrawIndirectCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxSamplerLodBias );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxSamplerAnisotropy );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxViewports );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxViewportDimensions[i] );
      }
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.viewportBoundsRange[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.viewportSubPixelBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minMemoryMapAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minTexelBufferOffsetAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minUniformBufferOffsetAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minStorageBufferOffsetAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minTexelOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTexelOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minTexelGatherOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxTexelGatherOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.minInterpolationOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxInterpolationOffset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.subPixelInterpolationOffsetBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFramebufferWidth );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFramebufferHeight );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxFramebufferLayers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.framebufferColorSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.framebufferDepthSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.framebufferStencilSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.framebufferNoAttachmentsSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxColorAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.sampledImageColorSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.sampledImageIntegerSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.sampledImageDepthSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.sampledImageStencilSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.storageImageSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxSampleMaskWords );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.timestampComputeAndGraphics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.timestampPeriod );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxClipDistances );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxCullDistances );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.maxCombinedClipAndCullDistances );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.discreteQueuePriorities );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.pointSizeRange[i] );
      }
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.lineWidthRange[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.pointSizeGranularity );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.lineWidthGranularity );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.strictLines );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.standardSampleLocations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.optimalBufferCopyOffsetAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.optimalBufferCopyRowPitchAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceLimits.nonCoherentAtomSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties const & physicalDeviceMaintenance3Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance3Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance3Properties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance3Properties.maxPerSetDescriptors );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance3Properties.maxMemoryAllocationSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features const & physicalDeviceMaintenance4Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Features.maintenance4 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties const & physicalDeviceMaintenance4Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Properties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMaintenance4Properties.maxBufferSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT const & physicalDeviceMemoryBudgetPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryBudgetPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryBudgetPropertiesEXT.pNext );
      for ( size_t i = 0; i < VK_MAX_MEMORY_HEAPS; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryBudgetPropertiesEXT.heapBudget[i] );
      }
      for ( size_t i = 0; i < VK_MAX_MEMORY_HEAPS; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryBudgetPropertiesEXT.heapUsage[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties const & physicalDeviceMemoryProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties.memoryTypeCount );
      for ( size_t i = 0; i < VK_MAX_MEMORY_TYPES; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties.memoryTypes[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties.memoryHeapCount );
      for ( size_t i = 0; i < VK_MAX_MEMORY_HEAPS; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties.memoryHeaps[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 const & physicalDeviceMemoryProperties2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMemoryProperties2.memoryProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures const & physicalDeviceMultiviewFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewFeatures.multiview );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewFeatures.multiviewGeometryShader );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewFeatures.multiviewTessellationShader );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties const & physicalDeviceMultiviewProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewProperties.maxMultiviewViewCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceMultiviewProperties.maxMultiviewInstanceIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT const & physicalDevicePCIBusInfoPropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.pciDomain );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.pciBus );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.pciDevice );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePCIBusInfoPropertiesEXT.pciFunction );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR const & physicalDevicePerformanceQueryFeaturesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryFeaturesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryFeaturesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryFeaturesKHR.performanceCounterQueryPools );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryFeaturesKHR.performanceCounterMultipleQueryPools );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR const & physicalDevicePerformanceQueryPropertiesKHR ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryPropertiesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryPropertiesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePerformanceQueryPropertiesKHR.allowCommandBufferQueryCopies );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures const & physicalDevicePipelineCreationCacheControlFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePipelineCreationCacheControlFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePipelineCreationCacheControlFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePipelineCreationCacheControlFeatures.pipelineCreationCacheControl );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties const & physicalDevicePointClippingProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePointClippingProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePointClippingProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePointClippingProperties.pointClippingBehavior );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures const & physicalDevicePrivateDataFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePrivateDataFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePrivateDataFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDevicePrivateDataFeatures.privateData );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties const & physicalDeviceSparseProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSparseProperties.residencyStandard2DBlockShape );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSparseProperties.residencyStandard2DMultisampleBlockShape );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSparseProperties.residencyStandard3DBlockShape );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSparseProperties.residencyAlignedMipSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSparseProperties.residencyNonResidentStrict );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties const & physicalDeviceProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.apiVersion );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.driverVersion );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.vendorID );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.deviceID );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.deviceType );
      for ( size_t i = 0; i < VK_MAX_PHYSICAL_DEVICE_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.deviceName[i] );
      }
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.pipelineCacheUUID[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.limits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties.sparseProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 const & physicalDeviceProperties2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProperties2.properties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures const & physicalDeviceProtectedMemoryFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryFeatures.protectedMemory );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties const & physicalDeviceProtectedMemoryProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceProtectedMemoryProperties.protectedNoFault );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT const & physicalDeviceRobustness2FeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2FeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2FeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2FeaturesEXT.robustBufferAccess2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2FeaturesEXT.robustImageAccess2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2FeaturesEXT.nullDescriptor );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT const & physicalDeviceRobustness2PropertiesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2PropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2PropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2PropertiesEXT.robustStorageBufferAccessSizeAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceRobustness2PropertiesEXT.robustUniformBufferAccessSizeAlignment );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT const & physicalDeviceSampleLocationsPropertiesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.sampleLocationSampleCounts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.maxSampleLocationGridSize );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.sampleLocationCoordinateRange[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.sampleLocationSubPixelBits );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSampleLocationsPropertiesEXT.variableSampleLocations );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties const & physicalDeviceSamplerFilterMinmaxProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerFilterMinmaxProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerFilterMinmaxProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerFilterMinmaxProperties.filterMinmaxSingleComponentFormats );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerFilterMinmaxProperties.filterMinmaxImageComponentMapping );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures const & physicalDeviceSamplerYcbcrConversionFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerYcbcrConversionFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerYcbcrConversionFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures const & physicalDeviceScalarBlockLayoutFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceScalarBlockLayoutFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceScalarBlockLayoutFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceScalarBlockLayoutFeatures.scalarBlockLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures const & physicalDeviceSeparateDepthStencilLayoutsFeatures )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSeparateDepthStencilLayoutsFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSeparateDepthStencilLayoutsFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSeparateDepthStencilLayoutsFeatures.separateDepthStencilLayouts );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT const & physicalDeviceShaderAtomicFloatFeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderBufferFloat32Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderBufferFloat32AtomicAdd );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderBufferFloat64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderBufferFloat64AtomicAdd );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderSharedFloat32Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderSharedFloat32AtomicAdd );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderSharedFloat64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderSharedFloat64AtomicAdd );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderImageFloat32Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.shaderImageFloat32AtomicAdd );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.sparseImageFloat32Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicFloatFeaturesEXT.sparseImageFloat32AtomicAdd );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features const & physicalDeviceShaderAtomicInt64Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicInt64Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicInt64Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicInt64Features.shaderBufferInt64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderAtomicInt64Features.shaderSharedInt64Atomics );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR const & physicalDeviceShaderClockFeaturesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderClockFeaturesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderClockFeaturesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderClockFeaturesKHR.shaderSubgroupClock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderClockFeaturesKHR.shaderDeviceClock );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures const &
                              physicalDeviceShaderDemoteToHelperInvocationFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDemoteToHelperInvocationFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDemoteToHelperInvocationFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDemoteToHelperInvocationFeatures.shaderDemoteToHelperInvocation );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures const & physicalDeviceShaderDrawParametersFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDrawParametersFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDrawParametersFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderDrawParametersFeatures.shaderDrawParameters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features const & physicalDeviceShaderFloat16Int8Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderFloat16Int8Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderFloat16Int8Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderFloat16Int8Features.shaderFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderFloat16Int8Features.shaderInt8 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT const & physicalDeviceShaderImageAtomicInt64FeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderImageAtomicInt64FeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderImageAtomicInt64FeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderImageAtomicInt64FeaturesEXT.shaderImageInt64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderImageAtomicInt64FeaturesEXT.sparseImageInt64Atomics );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures const & physicalDeviceShaderIntegerDotProductFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductFeatures.shaderIntegerDotProduct );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties const & physicalDeviceShaderIntegerDotProductProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct8BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct8BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct8BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct4x8BitPackedUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct4x8BitPackedSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct4x8BitPackedMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct16BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct16BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct16BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct32BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct32BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct32BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct64BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct64BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProduct64BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed,
                               physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures const & physicalDeviceShaderSubgroupExtendedTypesFeatures )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderSubgroupExtendedTypesFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderSubgroupExtendedTypesFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderSubgroupExtendedTypesFeatures.shaderSubgroupExtendedTypes );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures const & physicalDeviceShaderTerminateInvocationFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderTerminateInvocationFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderTerminateInvocationFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceShaderTerminateInvocationFeatures.shaderTerminateInvocation );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties const & physicalDeviceSubgroupProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.subgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.supportedStages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.supportedOperations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupProperties.quadOperationsInAllStages );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures const & physicalDeviceSubgroupSizeControlFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlFeatures.subgroupSizeControl );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlFeatures.computeFullSubgroups );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties const & physicalDeviceSubgroupSizeControlProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.minSubgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.maxSubgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.maxComputeWorkgroupSubgroups );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSubgroupSizeControlProperties.requiredSubgroupSizeStages );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR const & physicalDeviceSurfaceInfo2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSurfaceInfo2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSurfaceInfo2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSurfaceInfo2KHR.surface );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features const & physicalDeviceSynchronization2Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSynchronization2Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSynchronization2Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceSynchronization2Features.synchronization2 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT const & physicalDeviceTexelBufferAlignmentFeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentFeaturesEXT.texelBufferAlignment );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties const & physicalDeviceTexelBufferAlignmentProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.storageTexelBufferOffsetAlignmentBytes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.storageTexelBufferOffsetSingleTexelAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.uniformTexelBufferOffsetAlignmentBytes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTexelBufferAlignmentProperties.uniformTexelBufferOffsetSingleTexelAlignment );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures const & physicalDeviceTextureCompressionASTCHDRFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTextureCompressionASTCHDRFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTextureCompressionASTCHDRFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTextureCompressionASTCHDRFeatures.textureCompressionASTC_HDR );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures const & physicalDeviceTimelineSemaphoreFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreFeatures.timelineSemaphore );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties const & physicalDeviceTimelineSemaphoreProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceTimelineSemaphoreProperties.maxTimelineSemaphoreValueDifference );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties const & physicalDeviceToolProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.pNext );
      for ( size_t i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.name[i] );
      }
      for ( size_t i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.version[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.purposes );
      for ( size_t i = 0; i < VK_MAX_DESCRIPTION_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.description[i] );
      }
      for ( size_t i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceToolProperties.layer[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures const & physicalDeviceUniformBufferStandardLayoutFeatures )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceUniformBufferStandardLayoutFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceUniformBufferStandardLayoutFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceUniformBufferStandardLayoutFeatures.uniformBufferStandardLayout );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures const & physicalDeviceVariablePointersFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVariablePointersFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVariablePointersFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVariablePointersFeatures.variablePointersStorageBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVariablePointersFeatures.variablePointers );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT const & physicalDeviceVertexAttributeDivisorPropertiesEXT )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexAttributeDivisorPropertiesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexAttributeDivisorPropertiesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexAttributeDivisorPropertiesEXT.maxVertexAttribDivisor );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT const & physicalDeviceVertexInputDynamicStateFeaturesEXT )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexInputDynamicStateFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexInputDynamicStateFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVertexInputDynamicStateFeaturesEXT.vertexInputDynamicState );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features const & physicalDeviceVulkan11Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.storageBuffer16BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.uniformAndStorageBuffer16BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.storagePushConstant16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.storageInputOutput16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.multiview );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.multiviewGeometryShader );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.multiviewTessellationShader );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.variablePointersStorageBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.variablePointers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.protectedMemory );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.samplerYcbcrConversion );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Features.shaderDrawParameters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties const & physicalDeviceVulkan11Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.pNext );
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.deviceUUID[i] );
      }
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.driverUUID[i] );
      }
      for ( size_t i = 0; i < VK_LUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.deviceLUID[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.deviceNodeMask );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.deviceLUIDValid );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.subgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.subgroupSupportedStages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.subgroupSupportedOperations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.subgroupQuadOperationsInAllStages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.pointClippingBehavior );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.maxMultiviewViewCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.maxMultiviewInstanceIndex );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.protectedNoFault );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.maxPerSetDescriptors );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan11Properties.maxMemoryAllocationSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features const & physicalDeviceVulkan12Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.samplerMirrorClampToEdge );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.drawIndirectCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.storageBuffer8BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.uniformAndStorageBuffer8BitAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.storagePushConstant8 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderBufferInt64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderSharedInt64Atomics );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderInt8 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderInputAttachmentArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderUniformTexelBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderStorageTexelBufferArrayDynamicIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderUniformBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderSampledImageArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderStorageBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderStorageImageArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderInputAttachmentArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderUniformTexelBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderStorageTexelBufferArrayNonUniformIndexing );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingUniformBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingSampledImageUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingStorageImageUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingStorageBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingUniformTexelBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingStorageTexelBufferUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingUpdateUnusedWhilePending );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingPartiallyBound );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.descriptorBindingVariableDescriptorCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.runtimeDescriptorArray );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.samplerFilterMinmax );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.scalarBlockLayout );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.imagelessFramebuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.uniformBufferStandardLayout );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderSubgroupExtendedTypes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.separateDepthStencilLayouts );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.hostQueryReset );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.timelineSemaphore );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.bufferDeviceAddress );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.bufferDeviceAddressCaptureReplay );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.bufferDeviceAddressMultiDevice );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.vulkanMemoryModel );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.vulkanMemoryModelDeviceScope );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.vulkanMemoryModelAvailabilityVisibilityChains );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderOutputViewportIndex );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.shaderOutputLayer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Features.subgroupBroadcastDynamicId );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties const & physicalDeviceVulkan12Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.driverID );
      for ( size_t i = 0; i < VK_MAX_DRIVER_NAME_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.driverName[i] );
      }
      for ( size_t i = 0; i < VK_MAX_DRIVER_INFO_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.driverInfo[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.conformanceVersion );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.denormBehaviorIndependence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.roundingModeIndependence );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderSignedZeroInfNanPreserveFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderSignedZeroInfNanPreserveFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderSignedZeroInfNanPreserveFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormPreserveFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormPreserveFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormPreserveFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormFlushToZeroFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormFlushToZeroFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderDenormFlushToZeroFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTEFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTEFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTEFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTZFloat16 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTZFloat32 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderRoundingModeRTZFloat64 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxUpdateAfterBindDescriptorsInAllPools );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderUniformBufferArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderSampledImageArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderStorageBufferArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderStorageImageArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.shaderInputAttachmentArrayNonUniformIndexingNative );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.robustBufferAccessUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.quadDivergentImplicitLod );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageDescriptorUpdateAfterBindInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxPerStageUpdateAfterBindResources );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindSamplers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindUniformBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindStorageBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindSampledImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindStorageImages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxDescriptorSetUpdateAfterBindInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.supportedDepthResolveModes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.supportedStencilResolveModes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.independentResolveNone );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.independentResolve );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.filterMinmaxSingleComponentFormats );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.filterMinmaxImageComponentMapping );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.maxTimelineSemaphoreValueDifference );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan12Properties.framebufferIntegerColorSampleCounts );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features const & physicalDeviceVulkan13Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.robustImageAccess );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.inlineUniformBlock );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.descriptorBindingInlineUniformBlockUpdateAfterBind );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.pipelineCreationCacheControl );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.privateData );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.shaderDemoteToHelperInvocation );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.shaderTerminateInvocation );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.subgroupSizeControl );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.computeFullSubgroups );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.synchronization2 );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.textureCompressionASTC_HDR );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.shaderZeroInitializeWorkgroupMemory );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.dynamicRendering );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.shaderIntegerDotProduct );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Features.maintenance4 );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties const & physicalDeviceVulkan13Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.minSubgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxSubgroupSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxComputeWorkgroupSubgroups );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.requiredSubgroupSizeStages );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxInlineUniformBlockSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxPerStageDescriptorInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxDescriptorSetInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxInlineUniformTotalSize );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct8BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct8BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct8BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct4x8BitPackedUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct4x8BitPackedSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct4x8BitPackedMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct16BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct16BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct16BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct32BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct32BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct32BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct64BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct64BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProduct64BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating8BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating16BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating32BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating64BitSignedAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.storageTexelBufferOffsetAlignmentBytes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.storageTexelBufferOffsetSingleTexelAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.uniformTexelBufferOffsetAlignmentBytes );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.uniformTexelBufferOffsetSingleTexelAlignment );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkan13Properties.maxBufferSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures const & physicalDeviceVulkanMemoryModelFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanMemoryModelFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanMemoryModelFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanMemoryModelFeatures.vulkanMemoryModel );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanMemoryModelFeatures.vulkanMemoryModelDeviceScope );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanMemoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features const & physicalDeviceVulkanSC10Features ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Features.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Features.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Features.shaderAtomicInstructions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties const & physicalDeviceVulkanSC10Properties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.deviceNoDynamicHostAllocations );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.deviceDestroyFreesMemory );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.commandPoolMultipleCommandBuffersRecording );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.commandPoolResetCommandBuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.commandBufferSimultaneousUse );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.secondaryCommandBufferNullOrImagelessFramebuffer );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.recycleDescriptorSetMemory );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.recyclePipelineMemory );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxRenderPassSubpasses );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxRenderPassDependencies );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxSubpassInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxSubpassPreserveAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxFramebufferAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxDescriptorSetLayoutBindings );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxQueryFaultCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxCallbackFaultCount );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxCommandPoolCommandBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceVulkanSC10Properties.maxCommandBufferSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT const & physicalDeviceYcbcr2Plane444FormatsFeaturesEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcr2Plane444FormatsFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcr2Plane444FormatsFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcr2Plane444FormatsFeaturesEXT.ycbcr2plane444Formats );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT const & physicalDeviceYcbcrImageArraysFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcrImageArraysFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcrImageArraysFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceYcbcrImageArraysFeaturesEXT.ycbcrImageArrays );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures const & physicalDeviceZeroInitializeWorkgroupMemoryFeatures ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceZeroInitializeWorkgroupMemoryFeatures.sType );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceZeroInitializeWorkgroupMemoryFeatures.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, physicalDeviceZeroInitializeWorkgroupMemoryFeatures.shaderZeroInitializeWorkgroupMemory );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne const & pipelineCacheHeaderVersionOne ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionOne.headerSize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionOne.headerVersion );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionOne.vendorID );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionOne.deviceID );
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionOne.pipelineCacheUUID[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne const & pipelineCacheHeaderVersionSafetyCriticalOne ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.headerVersionOne );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.validationVersion );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.implementationData );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.pipelineIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.pipelineIndexStride );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheHeaderVersionSafetyCriticalOne.pipelineIndexOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry const & pipelineCacheSafetyCriticalIndexEntry ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.pipelineIdentifier[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.pipelineMemorySize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.jsonSize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.jsonOffset );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.stageIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.stageIndexStride );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheSafetyCriticalIndexEntry.stageIndexOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry const & pipelineCacheStageValidationIndexEntry ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheStageValidationIndexEntry.codeSize );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCacheStageValidationIndexEntry.codeOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT const & pipelineColorBlendAdvancedStateCreateInfoEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAdvancedStateCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAdvancedStateCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAdvancedStateCreateInfoEXT.srcPremultiplied );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAdvancedStateCreateInfoEXT.dstPremultiplied );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorBlendAdvancedStateCreateInfoEXT.blendOverlap );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT const & pipelineColorWriteCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorWriteCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorWriteCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorWriteCreateInfoEXT.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineColorWriteCreateInfoEXT.pColorWriteEnables );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCreationFeedback>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCreationFeedback const & pipelineCreationFeedback ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedback.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedback.duration );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo const & pipelineCreationFeedbackCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedbackCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedbackCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedbackCreateInfo.pPipelineCreationFeedback );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedbackCreateInfo.pipelineStageCreationFeedbackCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineCreationFeedbackCreateInfo.pPipelineStageCreationFeedbacks );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT const & pipelineDiscardRectangleStateCreateInfoEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.discardRectangleMode );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.discardRectangleCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineDiscardRectangleStateCreateInfoEXT.pDiscardRectangles );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR const & pipelineFragmentShadingRateStateCreateInfoKHR ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineFragmentShadingRateStateCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineFragmentShadingRateStateCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineFragmentShadingRateStateCreateInfoKHR.fragmentSize );
      for ( size_t i = 0; i < 2; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, pipelineFragmentShadingRateStateCreateInfoKHR.combinerOps[i] );
      }
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PushConstantRange>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PushConstantRange const & pushConstantRange ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pushConstantRange.stageFlags );
      VULKAN_HPP_HASH_COMBINE( seed, pushConstantRange.offset );
      VULKAN_HPP_HASH_COMBINE( seed, pushConstantRange.size );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo const & pipelineLayoutCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.setLayoutCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.pSetLayouts );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.pushConstantRangeCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineLayoutCreateInfo.pPushConstantRanges );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo const & pipelineOfflineCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineOfflineCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineOfflineCreateInfo.pNext );
      for ( size_t i = 0; i < VK_UUID_SIZE; ++i )
      {
        VULKAN_HPP_HASH_COMBINE( seed, pipelineOfflineCreateInfo.pipelineIdentifier[i] );
      }
      VULKAN_HPP_HASH_COMBINE( seed, pipelineOfflineCreateInfo.matchControl );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineOfflineCreateInfo.poolEntrySize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT const & pipelineRasterizationConservativeStateCreateInfoEXT ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationConservativeStateCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationConservativeStateCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationConservativeStateCreateInfoEXT.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationConservativeStateCreateInfoEXT.conservativeRasterizationMode );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationConservativeStateCreateInfoEXT.extraPrimitiveOverestimationSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT const & pipelineRasterizationDepthClipStateCreateInfoEXT )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationDepthClipStateCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationDepthClipStateCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationDepthClipStateCreateInfoEXT.flags );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRasterizationDepthClipStateCreateInfoEXT.depthClipEnable );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo const & pipelineRenderingCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.viewMask );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.colorAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.pColorAttachmentFormats );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.depthAttachmentFormat );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineRenderingCreateInfo.stencilAttachmentFormat );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT const & pipelineSampleLocationsStateCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineSampleLocationsStateCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineSampleLocationsStateCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineSampleLocationsStateCreateInfoEXT.sampleLocationsEnable );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineSampleLocationsStateCreateInfoEXT.sampleLocationsInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo const & pipelineShaderStageRequiredSubgroupSizeCreateInfo )
      const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageRequiredSubgroupSizeCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageRequiredSubgroupSizeCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineShaderStageRequiredSubgroupSizeCreateInfo.requiredSubgroupSize );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo>
  {
    std::size_t operator()(
      VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo const & pipelineTessellationDomainOriginStateCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationDomainOriginStateCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationDomainOriginStateCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, pipelineTessellationDomainOriginStateCreateInfo.domainOrigin );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PresentInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PresentInfoKHR const & presentInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.waitSemaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.pWaitSemaphores );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.swapchainCount );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.pSwapchains );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.pImageIndices );
      VULKAN_HPP_HASH_COMBINE( seed, presentInfoKHR.pResults );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RectLayerKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RectLayerKHR const & rectLayerKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, rectLayerKHR.offset );
      VULKAN_HPP_HASH_COMBINE( seed, rectLayerKHR.extent );
      VULKAN_HPP_HASH_COMBINE( seed, rectLayerKHR.layer );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PresentRegionKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PresentRegionKHR const & presentRegionKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionKHR.rectangleCount );
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionKHR.pRectangles );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PresentRegionsKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PresentRegionsKHR const & presentRegionsKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionsKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionsKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionsKHR.swapchainCount );
      VULKAN_HPP_HASH_COMBINE( seed, presentRegionsKHR.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo const & privateDataSlotCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, privateDataSlotCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, privateDataSlotCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, privateDataSlotCreateInfo.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo const & protectedSubmitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, protectedSubmitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, protectedSubmitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, protectedSubmitInfo.protectedSubmit );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo const & queryPoolCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.queryType );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.queryCount );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolCreateInfo.pipelineStatistics );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR const & queryPoolPerformanceCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolPerformanceCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolPerformanceCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolPerformanceCreateInfoKHR.queueFamilyIndex );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolPerformanceCreateInfoKHR.counterIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, queryPoolPerformanceCreateInfoKHR.pCounterIndices );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV const & queueFamilyCheckpointProperties2NV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyCheckpointProperties2NV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyCheckpointProperties2NV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyCheckpointProperties2NV.checkpointExecutionStageMask );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueueFamilyProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueueFamilyProperties const & queueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties.queueFlags );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties.queueCount );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties.timestampValidBits );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties.minImageTransferGranularity );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::QueueFamilyProperties2 const & queueFamilyProperties2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, queueFamilyProperties2.queueFamilyProperties );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RefreshObjectKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RefreshObjectKHR const & refreshObjectKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectKHR.objectType );
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectKHR.objectHandle );
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectKHR.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RefreshObjectListKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RefreshObjectListKHR const & refreshObjectListKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectListKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectListKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectListKHR.objectCount );
      VULKAN_HPP_HASH_COMBINE( seed, refreshObjectListKHR.pObjects );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo const & renderPassAttachmentBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassAttachmentBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassAttachmentBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassAttachmentBeginInfo.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassAttachmentBeginInfo.pAttachments );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPassBeginInfo const & renderPassBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.renderPass );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.framebuffer );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.renderArea );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.clearValueCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassBeginInfo.pClearValues );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassDescription>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassDescription const & subpassDescription ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.flags );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pipelineBindPoint );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.inputAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.colorAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pColorAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pResolveAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pDepthStencilAttachment );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.preserveAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription.pPreserveAttachments );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassDependency>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassDependency const & subpassDependency ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.srcSubpass );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.dstSubpass );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.srcStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.dstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency.dependencyFlags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo const & renderPassCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.pAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.subpassCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.pSubpasses );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.dependencyCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo.pDependencies );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassDescription2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassDescription2 const & subpassDescription2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.flags );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pipelineBindPoint );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.viewMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.inputAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pInputAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.colorAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pColorAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pResolveAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pDepthStencilAttachment );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.preserveAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescription2.pPreserveAttachments );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassDependency2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassDependency2 const & subpassDependency2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.srcSubpass );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.dstSubpass );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.srcStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.dstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.srcAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.dstAccessMask );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.dependencyFlags );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDependency2.viewOffset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 const & renderPassCreateInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.flags );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.attachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.pAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.subpassCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.pSubpasses );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.dependencyCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.pDependencies );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.correlatedViewMaskCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassCreateInfo2.pCorrelatedViewMasks );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo const & renderPassInputAttachmentAspectCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassInputAttachmentAspectCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassInputAttachmentAspectCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassInputAttachmentAspectCreateInfo.aspectReferenceCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassInputAttachmentAspectCreateInfo.pAspectReferences );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo const & renderPassMultiviewCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.subpassCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.pViewMasks );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.dependencyCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.pViewOffsets );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.correlationMaskCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassMultiviewCreateInfo.pCorrelationMasks );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT const & subpassSampleLocationsEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassSampleLocationsEXT.subpassIndex );
      VULKAN_HPP_HASH_COMBINE( seed, subpassSampleLocationsEXT.sampleLocationsInfo );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT const & renderPassSampleLocationsBeginInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.attachmentInitialSampleLocationsCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.pAttachmentInitialSampleLocations );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.postSubpassSampleLocationsCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderPassSampleLocationsBeginInfoEXT.pPostSubpassSampleLocations );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::RenderingInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::RenderingInfo const & renderingInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.renderArea );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.layerCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.viewMask );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.colorAttachmentCount );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.pColorAttachments );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.pDepthAttachment );
      VULKAN_HPP_HASH_COMBINE( seed, renderingInfo.pStencilAttachment );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ResolveImageInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ResolveImageInfo2 const & resolveImageInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.srcImage );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.srcImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.dstImage );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.dstImageLayout );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.regionCount );
      VULKAN_HPP_HASH_COMBINE( seed, resolveImageInfo2.pRegions );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerCreateInfo const & samplerCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.magFilter );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.minFilter );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.mipmapMode );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.addressModeU );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.addressModeV );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.addressModeW );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.mipLodBias );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.anisotropyEnable );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.maxAnisotropy );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.compareEnable );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.compareOp );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.minLod );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.maxLod );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.borderColor );
      VULKAN_HPP_HASH_COMBINE( seed, samplerCreateInfo.unnormalizedCoordinates );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo const & samplerReductionModeCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, samplerReductionModeCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, samplerReductionModeCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, samplerReductionModeCreateInfo.reductionMode );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo const & samplerYcbcrConversionCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.format );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.ycbcrModel );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.ycbcrRange );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.components );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.xChromaOffset );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.yChromaOffset );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.chromaFilter );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionCreateInfo.forceExplicitReconstruction );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties const & samplerYcbcrConversionImageFormatProperties ) const
      VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionImageFormatProperties.sType );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionImageFormatProperties.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionImageFormatProperties.combinedImageSamplerDescriptorCount );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo const & samplerYcbcrConversionInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, samplerYcbcrConversionInfo.conversion );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV const & sciSyncAttributesInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, sciSyncAttributesInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, sciSyncAttributesInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, sciSyncAttributesInfoNV.clientType );
      VULKAN_HPP_HASH_COMBINE( seed, sciSyncAttributesInfoNV.primitiveType );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX const & screenBufferFormatPropertiesQNX ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.sType );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.format );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.externalFormat );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.screenUsage );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.formatFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.samplerYcbcrConversionComponents );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.suggestedYcbcrModel );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.suggestedYcbcrRange );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.suggestedXChromaOffset );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferFormatPropertiesQNX.suggestedYChromaOffset );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX const & screenBufferPropertiesQNX ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferPropertiesQNX.sType );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferPropertiesQNX.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferPropertiesQNX.allocationSize );
      VULKAN_HPP_HASH_COMBINE( seed, screenBufferPropertiesQNX.memoryTypeBits );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo const & semaphoreCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreCreateInfo.flags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR const & semaphoreGetFdInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetFdInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetFdInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetFdInfoKHR.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetFdInfoKHR.handleType );
      return seed;
    }
  };

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV const & semaphoreGetSciSyncInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetSciSyncInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetSciSyncInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetSciSyncInfoNV.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreGetSciSyncInfoNV.handleType );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV const & semaphoreSciSyncCreateInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncCreateInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncCreateInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncCreateInfoNV.semaphorePool );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncCreateInfoNV.pFence );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV const & semaphoreSciSyncPoolCreateInfoNV ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncPoolCreateInfoNV.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncPoolCreateInfoNV.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSciSyncPoolCreateInfoNV.handle );
      return seed;
    }
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo const & semaphoreSignalInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSignalInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSignalInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSignalInfo.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSignalInfo.value );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo const & semaphoreSubmitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.semaphore );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.value );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.stageMask );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreSubmitInfo.deviceIndex );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo const & semaphoreTypeCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreTypeCreateInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreTypeCreateInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreTypeCreateInfo.semaphoreType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreTypeCreateInfo.initialValue );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo const & semaphoreWaitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.flags );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.semaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.pSemaphores );
      VULKAN_HPP_HASH_COMBINE( seed, semaphoreWaitInfo.pValues );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR const & sharedPresentSurfaceCapabilitiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, sharedPresentSurfaceCapabilitiesKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, sharedPresentSurfaceCapabilitiesKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, sharedPresentSurfaceCapabilitiesKHR.sharedPresentSupportedUsageFlags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubmitInfo const & submitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.waitSemaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.pWaitSemaphores );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.pWaitDstStageMask );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.commandBufferCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.pCommandBuffers );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.signalSemaphoreCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo.pSignalSemaphores );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubmitInfo2>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubmitInfo2 const & submitInfo2 ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.sType );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.flags );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.waitSemaphoreInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.pWaitSemaphoreInfos );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.commandBufferInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.pCommandBufferInfos );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.signalSemaphoreInfoCount );
      VULKAN_HPP_HASH_COMBINE( seed, submitInfo2.pSignalSemaphoreInfos );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassBeginInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassBeginInfo const & subpassBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassBeginInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, subpassBeginInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, subpassBeginInfo.contents );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve>
  {
    std::size_t
      operator()( VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve const & subpassDescriptionDepthStencilResolve ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescriptionDepthStencilResolve.sType );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescriptionDepthStencilResolve.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescriptionDepthStencilResolve.depthResolveMode );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescriptionDepthStencilResolve.stencilResolveMode );
      VULKAN_HPP_HASH_COMBINE( seed, subpassDescriptionDepthStencilResolve.pDepthStencilResolveAttachment );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SubpassEndInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SubpassEndInfo const & subpassEndInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, subpassEndInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, subpassEndInfo.pNext );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT const & surfaceCapabilities2EXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.minImageCount );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.maxImageCount );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.currentExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.minImageExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.maxImageExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.maxImageArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.supportedTransforms );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.currentTransform );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.supportedCompositeAlpha );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.supportedUsageFlags );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2EXT.supportedSurfaceCounters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR const & surfaceCapabilitiesKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.minImageCount );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.maxImageCount );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.currentExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.minImageExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.maxImageExtent );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.maxImageArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.supportedTransforms );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.currentTransform );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.supportedCompositeAlpha );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilitiesKHR.supportedUsageFlags );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR const & surfaceCapabilities2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceCapabilities2KHR.surfaceCapabilities );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceFormatKHR const & surfaceFormatKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, surfaceFormatKHR.format );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceFormatKHR.colorSpace );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR const & surfaceFormat2KHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, surfaceFormat2KHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceFormat2KHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, surfaceFormat2KHR.surfaceFormat );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT const & swapchainCounterCreateInfoEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCounterCreateInfoEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCounterCreateInfoEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCounterCreateInfoEXT.surfaceCounters );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const & swapchainCreateInfoKHR ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.sType );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.flags );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.surface );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.minImageCount );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageFormat );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageColorSpace );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageExtent );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageArrayLayers );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageUsage );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.imageSharingMode );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.queueFamilyIndexCount );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.pQueueFamilyIndices );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.preTransform );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.compositeAlpha );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.presentMode );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.clipped );
      VULKAN_HPP_HASH_COMBINE( seed, swapchainCreateInfoKHR.oldSwapchain );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo const & timelineSemaphoreSubmitInfo ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.sType );
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.waitSemaphoreValueCount );
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.pWaitSemaphoreValues );
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.signalSemaphoreValueCount );
      VULKAN_HPP_HASH_COMBINE( seed, timelineSemaphoreSubmitInfo.pSignalSemaphoreValues );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT const & validationFeaturesEXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.enabledValidationFeatureCount );
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.pEnabledValidationFeatures );
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.disabledValidationFeatureCount );
      VULKAN_HPP_HASH_COMBINE( seed, validationFeaturesEXT.pDisabledValidationFeatures );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT const & vertexInputAttributeDescription2EXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.location );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.binding );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.format );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputAttributeDescription2EXT.offset );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT const & vertexInputBindingDescription2EXT ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.sType );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.binding );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.stride );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.inputRate );
      VULKAN_HPP_HASH_COMBINE( seed, vertexInputBindingDescription2EXT.divisor );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::WriteDescriptorSet>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::WriteDescriptorSet const & writeDescriptorSet ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.sType );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.dstSet );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.dstBinding );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.dstArrayElement );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.descriptorCount );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.descriptorType );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.pImageInfo );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.pBufferInfo );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSet.pTexelBufferView );
      return seed;
    }
  };

  template <>
  struct hash<VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock const & writeDescriptorSetInlineUniformBlock ) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSetInlineUniformBlock.sType );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSetInlineUniformBlock.pNext );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSetInlineUniformBlock.dataSize );
      VULKAN_HPP_HASH_COMBINE( seed, writeDescriptorSetInlineUniformBlock.pData );
      return seed;
    }
  };

#endif  // 14 <= VULKAN_HPP_CPP_VERSION

}  // namespace std
#endif
