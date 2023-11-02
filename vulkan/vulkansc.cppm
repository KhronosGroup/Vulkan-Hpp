// Copyright 2015-2023 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

// Note: This module is still in an experimental state.
// Any feedback is welcome on https://github.com/KhronosGroup/Vulkan-Hpp/issues.

module;

#include <vulkan/vulkansc.hpp>
#include <vulkan/vulkansc_extension_inspection.hpp>
#include <vulkan/vulkansc_format_traits.hpp>
#include <vulkan/vulkansc_hash.hpp>
#include <vulkan/vulkansc_raii.hpp>
#include <vulkan/vulkansc_shared.hpp>

export module vulkansc_hpp;

export namespace VULKAN_HPP_NAMESPACE
{
  //=====================================
  //=== HARDCODED TYPEs AND FUNCTIONs ===
  //=====================================
  using VULKAN_HPP_NAMESPACE::ArrayWrapper1D;
  using VULKAN_HPP_NAMESPACE::ArrayWrapper2D;
  using VULKAN_HPP_NAMESPACE::DispatchLoaderBase;
  using VULKAN_HPP_NAMESPACE::DispatchLoaderDynamic;
  using VULKAN_HPP_NAMESPACE::Flags;
  using VULKAN_HPP_NAMESPACE::FlagTraits;

#if !defined( VK_NO_PROTOTYPES )
  using VULKAN_HPP_NAMESPACE::DispatchLoaderStatic;
#endif /*VK_NO_PROTOTYPES*/

  using VULKAN_HPP_NAMESPACE::operator&;
  using VULKAN_HPP_NAMESPACE::operator|;
  using VULKAN_HPP_NAMESPACE::operator^;
  using VULKAN_HPP_NAMESPACE::operator~;
  using VULKAN_HPP_DEFAULT_DISPATCHER_TYPE;

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  using VULKAN_HPP_NAMESPACE::ArrayProxy;
  using VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries;
  using VULKAN_HPP_NAMESPACE::Optional;
  using VULKAN_HPP_NAMESPACE::StridedArrayProxy;
  using VULKAN_HPP_NAMESPACE::StructureChain;
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  using VULKAN_HPP_NAMESPACE::ObjectDestroy;
  using VULKAN_HPP_NAMESPACE::ObjectDestroyShared;
  using VULKAN_HPP_NAMESPACE::ObjectFree;
  using VULKAN_HPP_NAMESPACE::ObjectFreeShared;
  using VULKAN_HPP_NAMESPACE::ObjectRelease;
  using VULKAN_HPP_NAMESPACE::ObjectReleaseShared;
  using VULKAN_HPP_NAMESPACE::PoolFree;
  using VULKAN_HPP_NAMESPACE::PoolFreeShared;
  using VULKAN_HPP_NAMESPACE::SharedHandle;
  using VULKAN_HPP_NAMESPACE::UniqueHandle;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  //==================
  //=== BASE TYPEs ===
  //==================
  using VULKAN_HPP_NAMESPACE::Bool32;
  using VULKAN_HPP_NAMESPACE::DeviceAddress;
  using VULKAN_HPP_NAMESPACE::DeviceSize;
  using VULKAN_HPP_NAMESPACE::RemoteAddressNV;
  using VULKAN_HPP_NAMESPACE::SampleMask;

  //=============
  //=== ENUMs ===
  //=============
  using VULKAN_HPP_NAMESPACE::CppType;

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::AccessFlagBits;
  using VULKAN_HPP_NAMESPACE::AccessFlags;
  using VULKAN_HPP_NAMESPACE::AttachmentDescriptionFlagBits;
  using VULKAN_HPP_NAMESPACE::AttachmentDescriptionFlags;
  using VULKAN_HPP_NAMESPACE::AttachmentLoadOp;
  using VULKAN_HPP_NAMESPACE::AttachmentStoreOp;
  using VULKAN_HPP_NAMESPACE::BlendFactor;
  using VULKAN_HPP_NAMESPACE::BlendOp;
  using VULKAN_HPP_NAMESPACE::BorderColor;
  using VULKAN_HPP_NAMESPACE::BufferCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::BufferCreateFlags;
  using VULKAN_HPP_NAMESPACE::BufferUsageFlagBits;
  using VULKAN_HPP_NAMESPACE::BufferUsageFlags;
  using VULKAN_HPP_NAMESPACE::BufferViewCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::BufferViewCreateFlags;
  using VULKAN_HPP_NAMESPACE::ColorComponentFlagBits;
  using VULKAN_HPP_NAMESPACE::ColorComponentFlags;
  using VULKAN_HPP_NAMESPACE::CommandBufferLevel;
  using VULKAN_HPP_NAMESPACE::CommandBufferResetFlagBits;
  using VULKAN_HPP_NAMESPACE::CommandBufferResetFlags;
  using VULKAN_HPP_NAMESPACE::CommandBufferUsageFlagBits;
  using VULKAN_HPP_NAMESPACE::CommandBufferUsageFlags;
  using VULKAN_HPP_NAMESPACE::CommandPoolCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::CommandPoolCreateFlags;
  using VULKAN_HPP_NAMESPACE::CommandPoolResetFlagBits;
  using VULKAN_HPP_NAMESPACE::CommandPoolResetFlags;
  using VULKAN_HPP_NAMESPACE::CompareOp;
  using VULKAN_HPP_NAMESPACE::ComponentSwizzle;
  using VULKAN_HPP_NAMESPACE::CullModeFlagBits;
  using VULKAN_HPP_NAMESPACE::CullModeFlags;
  using VULKAN_HPP_NAMESPACE::DependencyFlagBits;
  using VULKAN_HPP_NAMESPACE::DependencyFlags;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolCreateFlags;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlagBits;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlags;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateFlags;
  using VULKAN_HPP_NAMESPACE::DescriptorType;
  using VULKAN_HPP_NAMESPACE::DeviceCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::DeviceCreateFlags;
  using VULKAN_HPP_NAMESPACE::DeviceQueueCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::DeviceQueueCreateFlags;
  using VULKAN_HPP_NAMESPACE::DynamicState;
  using VULKAN_HPP_NAMESPACE::EventCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::EventCreateFlags;
  using VULKAN_HPP_NAMESPACE::FenceCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::FenceCreateFlags;
  using VULKAN_HPP_NAMESPACE::Filter;
  using VULKAN_HPP_NAMESPACE::Format;
  using VULKAN_HPP_NAMESPACE::FormatFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::FormatFeatureFlags;
  using VULKAN_HPP_NAMESPACE::FramebufferCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::FramebufferCreateFlags;
  using VULKAN_HPP_NAMESPACE::FrontFace;
  using VULKAN_HPP_NAMESPACE::ImageAspectFlagBits;
  using VULKAN_HPP_NAMESPACE::ImageAspectFlags;
  using VULKAN_HPP_NAMESPACE::ImageCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::ImageCreateFlags;
  using VULKAN_HPP_NAMESPACE::ImageLayout;
  using VULKAN_HPP_NAMESPACE::ImageTiling;
  using VULKAN_HPP_NAMESPACE::ImageType;
  using VULKAN_HPP_NAMESPACE::ImageUsageFlagBits;
  using VULKAN_HPP_NAMESPACE::ImageUsageFlags;
  using VULKAN_HPP_NAMESPACE::ImageViewCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::ImageViewCreateFlags;
  using VULKAN_HPP_NAMESPACE::ImageViewType;
  using VULKAN_HPP_NAMESPACE::IndexType;
  using VULKAN_HPP_NAMESPACE::InstanceCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::InstanceCreateFlags;
  using VULKAN_HPP_NAMESPACE::InternalAllocationType;
  using VULKAN_HPP_NAMESPACE::LogicOp;
  using VULKAN_HPP_NAMESPACE::MemoryHeapFlagBits;
  using VULKAN_HPP_NAMESPACE::MemoryHeapFlags;
  using VULKAN_HPP_NAMESPACE::MemoryMapFlagBits;
  using VULKAN_HPP_NAMESPACE::MemoryMapFlags;
  using VULKAN_HPP_NAMESPACE::MemoryPropertyFlagBits;
  using VULKAN_HPP_NAMESPACE::MemoryPropertyFlags;
  using VULKAN_HPP_NAMESPACE::ObjectType;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceType;
  using VULKAN_HPP_NAMESPACE::PipelineBindPoint;
  using VULKAN_HPP_NAMESPACE::PipelineCacheCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineCacheCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersion;
  using VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineLayoutCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineLayoutCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineStageFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineStageFlags;
  using VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateFlags;
  using VULKAN_HPP_NAMESPACE::PolygonMode;
  using VULKAN_HPP_NAMESPACE::PrimitiveTopology;
  using VULKAN_HPP_NAMESPACE::QueryControlFlagBits;
  using VULKAN_HPP_NAMESPACE::QueryControlFlags;
  using VULKAN_HPP_NAMESPACE::QueryPipelineStatisticFlagBits;
  using VULKAN_HPP_NAMESPACE::QueryPipelineStatisticFlags;
  using VULKAN_HPP_NAMESPACE::QueryPoolCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::QueryPoolCreateFlags;
  using VULKAN_HPP_NAMESPACE::QueryResultFlagBits;
  using VULKAN_HPP_NAMESPACE::QueryResultFlags;
  using VULKAN_HPP_NAMESPACE::QueryType;
  using VULKAN_HPP_NAMESPACE::QueueFlagBits;
  using VULKAN_HPP_NAMESPACE::QueueFlags;
  using VULKAN_HPP_NAMESPACE::RenderPassCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::RenderPassCreateFlags;
  using VULKAN_HPP_NAMESPACE::Result;
  using VULKAN_HPP_NAMESPACE::SampleCountFlagBits;
  using VULKAN_HPP_NAMESPACE::SampleCountFlags;
  using VULKAN_HPP_NAMESPACE::SamplerAddressMode;
  using VULKAN_HPP_NAMESPACE::SamplerCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::SamplerCreateFlags;
  using VULKAN_HPP_NAMESPACE::SamplerMipmapMode;
  using VULKAN_HPP_NAMESPACE::SemaphoreCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::SemaphoreCreateFlags;
  using VULKAN_HPP_NAMESPACE::ShaderStageFlagBits;
  using VULKAN_HPP_NAMESPACE::ShaderStageFlags;
  using VULKAN_HPP_NAMESPACE::SharingMode;
  using VULKAN_HPP_NAMESPACE::StencilFaceFlagBits;
  using VULKAN_HPP_NAMESPACE::StencilFaceFlags;
  using VULKAN_HPP_NAMESPACE::StencilOp;
  using VULKAN_HPP_NAMESPACE::StructureType;
  using VULKAN_HPP_NAMESPACE::SubpassContents;
  using VULKAN_HPP_NAMESPACE::SubpassDescriptionFlagBits;
  using VULKAN_HPP_NAMESPACE::SubpassDescriptionFlags;
  using VULKAN_HPP_NAMESPACE::SystemAllocationScope;
  using VULKAN_HPP_NAMESPACE::VendorId;
  using VULKAN_HPP_NAMESPACE::VertexInputRate;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::ChromaLocation;
  using VULKAN_HPP_NAMESPACE::ChromaLocationKHR;
  using VULKAN_HPP_NAMESPACE::ExternalFenceFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalFenceFeatureFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalFenceFeatureFlags;
  using VULKAN_HPP_NAMESPACE::ExternalFenceHandleTypeFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalFenceHandleTypeFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalFenceHandleTypeFlags;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryFeatureFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryFeatureFlags;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlags;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreFeatureFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreFeatureFlags;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreHandleTypeFlagBits;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreHandleTypeFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreHandleTypeFlags;
  using VULKAN_HPP_NAMESPACE::FenceImportFlagBits;
  using VULKAN_HPP_NAMESPACE::FenceImportFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::FenceImportFlags;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateFlagBits;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateFlags;
  using VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags;
  using VULKAN_HPP_NAMESPACE::PointClippingBehavior;
  using VULKAN_HPP_NAMESPACE::PointClippingBehaviorKHR;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrModelConversion;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrModelConversionKHR;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrRange;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrRangeKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreImportFlagBits;
  using VULKAN_HPP_NAMESPACE::SemaphoreImportFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreImportFlags;
  using VULKAN_HPP_NAMESPACE::SubgroupFeatureFlagBits;
  using VULKAN_HPP_NAMESPACE::SubgroupFeatureFlags;
  using VULKAN_HPP_NAMESPACE::TessellationDomainOrigin;
  using VULKAN_HPP_NAMESPACE::TessellationDomainOriginKHR;

  //=== VK_VERSION_1_2 ===
  using VULKAN_HPP_NAMESPACE::DescriptorBindingFlagBits;
  using VULKAN_HPP_NAMESPACE::DescriptorBindingFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::DescriptorBindingFlags;
  using VULKAN_HPP_NAMESPACE::DriverId;
  using VULKAN_HPP_NAMESPACE::DriverIdKHR;
  using VULKAN_HPP_NAMESPACE::ResolveModeFlagBits;
  using VULKAN_HPP_NAMESPACE::ResolveModeFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::ResolveModeFlags;
  using VULKAN_HPP_NAMESPACE::SamplerReductionMode;
  using VULKAN_HPP_NAMESPACE::SamplerReductionModeEXT;
  using VULKAN_HPP_NAMESPACE::SemaphoreType;
  using VULKAN_HPP_NAMESPACE::SemaphoreTypeKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreWaitFlagBits;
  using VULKAN_HPP_NAMESPACE::SemaphoreWaitFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreWaitFlags;
  using VULKAN_HPP_NAMESPACE::ShaderFloatControlsIndependence;
  using VULKAN_HPP_NAMESPACE::ShaderFloatControlsIndependenceKHR;

  //=== VK_VERSION_1_3 ===
  using VULKAN_HPP_NAMESPACE::AccessFlagBits2;
  using VULKAN_HPP_NAMESPACE::AccessFlagBits2KHR;
  using VULKAN_HPP_NAMESPACE::AccessFlags2;
  using VULKAN_HPP_NAMESPACE::FormatFeatureFlagBits2;
  using VULKAN_HPP_NAMESPACE::FormatFeatureFlagBits2KHR;
  using VULKAN_HPP_NAMESPACE::FormatFeatureFlags2;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackFlagBits;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackFlags;
  using VULKAN_HPP_NAMESPACE::PipelineStageFlagBits2;
  using VULKAN_HPP_NAMESPACE::PipelineStageFlagBits2KHR;
  using VULKAN_HPP_NAMESPACE::PipelineStageFlags2;
  using VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateFlagBits;
  using VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateFlags;
  using VULKAN_HPP_NAMESPACE::RenderingFlagBits;
  using VULKAN_HPP_NAMESPACE::RenderingFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::RenderingFlags;
  using VULKAN_HPP_NAMESPACE::SubmitFlagBits;
  using VULKAN_HPP_NAMESPACE::SubmitFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::SubmitFlags;
  using VULKAN_HPP_NAMESPACE::ToolPurposeFlagBits;
  using VULKAN_HPP_NAMESPACE::ToolPurposeFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::ToolPurposeFlags;

  //=== VKSC_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::FaultLevel;
  using VULKAN_HPP_NAMESPACE::FaultQueryBehavior;
  using VULKAN_HPP_NAMESPACE::FaultType;
  using VULKAN_HPP_NAMESPACE::PipelineCacheValidationVersion;
  using VULKAN_HPP_NAMESPACE::PipelineMatchControl;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::ColorSpaceKHR;
  using VULKAN_HPP_NAMESPACE::CompositeAlphaFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::CompositeAlphaFlagsKHR;
  using VULKAN_HPP_NAMESPACE::PresentModeKHR;
  using VULKAN_HPP_NAMESPACE::SurfaceTransformFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::SurfaceTransformFlagsKHR;

  //=== VK_KHR_swapchain ===
  using VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR;
  using VULKAN_HPP_NAMESPACE::SwapchainCreateFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::SwapchainCreateFlagsKHR;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::DisplayModeCreateFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::DisplayModeCreateFlagsKHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneAlphaFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneAlphaFlagsKHR;
  using VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateFlagsKHR;

  //=== VK_EXT_display_surface_counter ===
  using VULKAN_HPP_NAMESPACE::SurfaceCounterFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::SurfaceCounterFlagsEXT;

  //=== VK_EXT_display_control ===
  using VULKAN_HPP_NAMESPACE::DeviceEventTypeEXT;
  using VULKAN_HPP_NAMESPACE::DisplayEventTypeEXT;
  using VULKAN_HPP_NAMESPACE::DisplayPowerStateEXT;

  //=== VK_EXT_discard_rectangles ===
  using VULKAN_HPP_NAMESPACE::DiscardRectangleModeEXT;
  using VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateFlagsEXT;

  //=== VK_EXT_conservative_rasterization ===
  using VULKAN_HPP_NAMESPACE::ConservativeRasterizationModeEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateFlagsEXT;

  //=== VK_EXT_depth_clip_enable ===
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateFlagsEXT;

  //=== VK_KHR_performance_query ===
  using VULKAN_HPP_NAMESPACE::AcquireProfilingLockFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::AcquireProfilingLockFlagsKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionFlagsKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterScopeKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterStorageKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterUnitKHR;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataFlagsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagsEXT;

  //=== VK_EXT_blend_operation_advanced ===
  using VULKAN_HPP_NAMESPACE::BlendOverlapEXT;

  //=== VK_EXT_calibrated_timestamps ===
  using VULKAN_HPP_NAMESPACE::TimeDomainEXT;

  //=== VK_KHR_fragment_shading_rate ===
  using VULKAN_HPP_NAMESPACE::FragmentShadingRateCombinerOpKHR;

  //=== VK_EXT_validation_features ===
  using VULKAN_HPP_NAMESPACE::ValidationFeatureDisableEXT;
  using VULKAN_HPP_NAMESPACE::ValidationFeatureEnableEXT;

  //=== VK_EXT_headless_surface ===
  using VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateFlagBitsEXT;
  using VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateFlagsEXT;

  //=== VK_EXT_line_rasterization ===
  using VULKAN_HPP_NAMESPACE::LineRasterizationModeEXT;

  //=== VK_KHR_object_refresh ===
  using VULKAN_HPP_NAMESPACE::RefreshObjectFlagBitsKHR;
  using VULKAN_HPP_NAMESPACE::RefreshObjectFlagsKHR;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===
  using VULKAN_HPP_NAMESPACE::SciSyncClientTypeNV;
  using VULKAN_HPP_NAMESPACE::SciSyncPrimitiveTypeNV;
#endif /*VK_USE_PLATFORM_SCI*/

  //=========================
  //=== Index Type Traits ===
  //=========================
  using VULKAN_HPP_NAMESPACE::IndexTypeValue;

  //======================
  //=== ENUM to_string ===
  //======================
#if !defined( VULKAN_HPP_NO_TO_STRING )
  using VULKAN_HPP_NAMESPACE::to_string;
  using VULKAN_HPP_NAMESPACE::toHexString;
#endif /*VULKAN_HPP_NO_TO_STRING*/

  //=============================
  //=== EXCEPTIONs AND ERRORs ===
  //=============================
#if !defined( VULKAN_HPP_NO_EXCEPTIONS )
  using VULKAN_HPP_NAMESPACE::DeviceLostError;
  using VULKAN_HPP_NAMESPACE::Error;
  using VULKAN_HPP_NAMESPACE::errorCategory;
  using VULKAN_HPP_NAMESPACE::ErrorCategoryImpl;
  using VULKAN_HPP_NAMESPACE::ExtensionNotPresentError;
  using VULKAN_HPP_NAMESPACE::FeatureNotPresentError;
  using VULKAN_HPP_NAMESPACE::FormatNotSupportedError;
  using VULKAN_HPP_NAMESPACE::FragmentationError;
  using VULKAN_HPP_NAMESPACE::FragmentedPoolError;
  using VULKAN_HPP_NAMESPACE::IncompatibleDisplayKHRError;
  using VULKAN_HPP_NAMESPACE::IncompatibleDriverError;
  using VULKAN_HPP_NAMESPACE::InitializationFailedError;
  using VULKAN_HPP_NAMESPACE::InvalidDrmFormatModifierPlaneLayoutEXTError;
  using VULKAN_HPP_NAMESPACE::InvalidExternalHandleError;
  using VULKAN_HPP_NAMESPACE::InvalidOpaqueCaptureAddressError;
  using VULKAN_HPP_NAMESPACE::InvalidPipelineCacheDataError;
  using VULKAN_HPP_NAMESPACE::LayerNotPresentError;
  using VULKAN_HPP_NAMESPACE::LogicError;
  using VULKAN_HPP_NAMESPACE::make_error_code;
  using VULKAN_HPP_NAMESPACE::make_error_condition;
  using VULKAN_HPP_NAMESPACE::MemoryMapFailedError;
  using VULKAN_HPP_NAMESPACE::NativeWindowInUseKHRError;
  using VULKAN_HPP_NAMESPACE::NoPipelineMatchError;
  using VULKAN_HPP_NAMESPACE::OutOfDateKHRError;
  using VULKAN_HPP_NAMESPACE::OutOfDeviceMemoryError;
  using VULKAN_HPP_NAMESPACE::OutOfHostMemoryError;
  using VULKAN_HPP_NAMESPACE::OutOfPoolMemoryError;
  using VULKAN_HPP_NAMESPACE::SurfaceLostKHRError;
  using VULKAN_HPP_NAMESPACE::SystemError;
  using VULKAN_HPP_NAMESPACE::TooManyObjectsError;
  using VULKAN_HPP_NAMESPACE::UnknownError;
  using VULKAN_HPP_NAMESPACE::ValidationFailedError;
#endif /*VULKAN_HPP_NO_EXCEPTIONS*/

  using VULKAN_HPP_NAMESPACE::createResultValueType;
  using VULKAN_HPP_NAMESPACE::ignore;
  using VULKAN_HPP_NAMESPACE::resultCheck;
  using VULKAN_HPP_NAMESPACE::ResultValue;
  using VULKAN_HPP_NAMESPACE::ResultValueType;

  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::AttachmentUnused;
  using VULKAN_HPP_NAMESPACE::False;
  using VULKAN_HPP_NAMESPACE::LodClampNone;
  using VULKAN_HPP_NAMESPACE::MaxDescriptionSize;
  using VULKAN_HPP_NAMESPACE::MaxExtensionNameSize;
  using VULKAN_HPP_NAMESPACE::MaxMemoryHeaps;
  using VULKAN_HPP_NAMESPACE::MaxMemoryTypes;
  using VULKAN_HPP_NAMESPACE::MaxPhysicalDeviceNameSize;
  using VULKAN_HPP_NAMESPACE::QueueFamilyIgnored;
  using VULKAN_HPP_NAMESPACE::RemainingArrayLayers;
  using VULKAN_HPP_NAMESPACE::RemainingMipLevels;
  using VULKAN_HPP_NAMESPACE::SubpassExternal;
  using VULKAN_HPP_NAMESPACE::True;
  using VULKAN_HPP_NAMESPACE::UuidSize;
  using VULKAN_HPP_NAMESPACE::WholeSize;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::LuidSize;
  using VULKAN_HPP_NAMESPACE::MaxDeviceGroupSize;
  using VULKAN_HPP_NAMESPACE::QueueFamilyExternal;

  //=== VK_VERSION_1_2 ===
  using VULKAN_HPP_NAMESPACE::MaxDriverInfoSize;
  using VULKAN_HPP_NAMESPACE::MaxDriverNameSize;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::KHRSurfaceExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRSurfaceSpecVersion;

  //=== VK_KHR_swapchain ===
  using VULKAN_HPP_NAMESPACE::KHRSwapchainExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRSwapchainSpecVersion;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::KHRDisplayExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRDisplaySpecVersion;

  //=== VK_KHR_display_swapchain ===
  using VULKAN_HPP_NAMESPACE::KHRDisplaySwapchainExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRDisplaySwapchainSpecVersion;

  //=== VK_EXT_depth_range_unrestricted ===
  using VULKAN_HPP_NAMESPACE::EXTDepthRangeUnrestrictedExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDepthRangeUnrestrictedSpecVersion;

  //=== VK_NV_private_vendor_info ===
  using VULKAN_HPP_NAMESPACE::NVPrivateVendorInfoExtensionName;
  using VULKAN_HPP_NAMESPACE::NVPrivateVendorInfoSpecVersion;

  //=== VK_EXT_texture_compression_astc_hdr ===
  using VULKAN_HPP_NAMESPACE::EXTTextureCompressionAstcHdrExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTTextureCompressionAstcHdrSpecVersion;

  //=== VK_EXT_astc_decode_mode ===
  using VULKAN_HPP_NAMESPACE::EXTAstcDecodeModeExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTAstcDecodeModeSpecVersion;

  //=== VK_KHR_external_memory_fd ===
  using VULKAN_HPP_NAMESPACE::KHRExternalMemoryFdExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRExternalMemoryFdSpecVersion;

  //=== VK_KHR_external_semaphore_fd ===
  using VULKAN_HPP_NAMESPACE::KHRExternalSemaphoreFdExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRExternalSemaphoreFdSpecVersion;

  //=== VK_KHR_incremental_present ===
  using VULKAN_HPP_NAMESPACE::KHRIncrementalPresentExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRIncrementalPresentSpecVersion;

  //=== VK_EXT_direct_mode_display ===
  using VULKAN_HPP_NAMESPACE::EXTDirectModeDisplayExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDirectModeDisplaySpecVersion;

  //=== VK_EXT_display_surface_counter ===
  using VULKAN_HPP_NAMESPACE::EXTDisplaySurfaceCounterExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDisplaySurfaceCounterSpecVersion;

  //=== VK_EXT_display_control ===
  using VULKAN_HPP_NAMESPACE::EXTDisplayControlExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDisplayControlSpecVersion;

  //=== VK_EXT_discard_rectangles ===
  using VULKAN_HPP_NAMESPACE::EXTDiscardRectanglesExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDiscardRectanglesSpecVersion;

  //=== VK_EXT_conservative_rasterization ===
  using VULKAN_HPP_NAMESPACE::EXTConservativeRasterizationExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTConservativeRasterizationSpecVersion;

  //=== VK_EXT_depth_clip_enable ===
  using VULKAN_HPP_NAMESPACE::EXTDepthClipEnableExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDepthClipEnableSpecVersion;

  //=== VK_EXT_swapchain_colorspace ===
  using VULKAN_HPP_NAMESPACE::EXTSwapchainColorSpaceExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTSwapchainColorSpaceSpecVersion;

  //=== VK_EXT_hdr_metadata ===
  using VULKAN_HPP_NAMESPACE::EXTHdrMetadataExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTHdrMetadataSpecVersion;

  //=== VK_KHR_shared_presentable_image ===
  using VULKAN_HPP_NAMESPACE::KHRSharedPresentableImageExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRSharedPresentableImageSpecVersion;

  //=== VK_KHR_external_fence_fd ===
  using VULKAN_HPP_NAMESPACE::KHRExternalFenceFdExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRExternalFenceFdSpecVersion;

  //=== VK_KHR_performance_query ===
  using VULKAN_HPP_NAMESPACE::KHRPerformanceQueryExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRPerformanceQuerySpecVersion;

  //=== VK_KHR_get_surface_capabilities2 ===
  using VULKAN_HPP_NAMESPACE::KHRGetSurfaceCapabilities2ExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRGetSurfaceCapabilities2SpecVersion;

  //=== VK_KHR_get_display_properties2 ===
  using VULKAN_HPP_NAMESPACE::KHRGetDisplayProperties2ExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRGetDisplayProperties2SpecVersion;

  //=== VK_EXT_external_memory_dma_buf ===
  using VULKAN_HPP_NAMESPACE::EXTExternalMemoryDmaBufExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTExternalMemoryDmaBufSpecVersion;

  //=== VK_EXT_queue_family_foreign ===
  using VULKAN_HPP_NAMESPACE::EXTQueueFamilyForeignExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTQueueFamilyForeignSpecVersion;
  using VULKAN_HPP_NAMESPACE::QueueFamilyForeignEXT;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::EXTDebugUtilsExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTDebugUtilsSpecVersion;

  //=== VK_EXT_shader_stencil_export ===
  using VULKAN_HPP_NAMESPACE::EXTShaderStencilExportExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTShaderStencilExportSpecVersion;

  //=== VK_EXT_sample_locations ===
  using VULKAN_HPP_NAMESPACE::EXTSampleLocationsExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTSampleLocationsSpecVersion;

  //=== VK_EXT_blend_operation_advanced ===
  using VULKAN_HPP_NAMESPACE::EXTBlendOperationAdvancedExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTBlendOperationAdvancedSpecVersion;

  //=== VK_EXT_post_depth_coverage ===
  using VULKAN_HPP_NAMESPACE::EXTPostDepthCoverageExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTPostDepthCoverageSpecVersion;

  //=== VK_EXT_image_drm_format_modifier ===
  using VULKAN_HPP_NAMESPACE::EXTImageDrmFormatModifierExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTImageDrmFormatModifierSpecVersion;

  //=== VK_EXT_filter_cubic ===
  using VULKAN_HPP_NAMESPACE::EXTFilterCubicExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTFilterCubicSpecVersion;

  //=== VK_EXT_global_priority ===
  using VULKAN_HPP_NAMESPACE::EXTGlobalPriorityExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTGlobalPrioritySpecVersion;

  //=== VK_EXT_external_memory_host ===
  using VULKAN_HPP_NAMESPACE::EXTExternalMemoryHostExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTExternalMemoryHostSpecVersion;

  //=== VK_KHR_shader_clock ===
  using VULKAN_HPP_NAMESPACE::KHRShaderClockExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRShaderClockSpecVersion;

  //=== VK_EXT_calibrated_timestamps ===
  using VULKAN_HPP_NAMESPACE::EXTCalibratedTimestampsExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTCalibratedTimestampsSpecVersion;

  //=== VK_EXT_vertex_attribute_divisor ===
  using VULKAN_HPP_NAMESPACE::EXTVertexAttributeDivisorExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTVertexAttributeDivisorSpecVersion;

  //=== VK_KHR_swapchain_mutable_format ===
  using VULKAN_HPP_NAMESPACE::KHRSwapchainMutableFormatExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRSwapchainMutableFormatSpecVersion;

  //=== VK_EXT_pci_bus_info ===
  using VULKAN_HPP_NAMESPACE::EXTPciBusInfoExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTPciBusInfoSpecVersion;

  //=== VK_KHR_shader_terminate_invocation ===
  using VULKAN_HPP_NAMESPACE::KHRShaderTerminateInvocationExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRShaderTerminateInvocationSpecVersion;

  //=== VK_EXT_subgroup_size_control ===
  using VULKAN_HPP_NAMESPACE::EXTSubgroupSizeControlExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTSubgroupSizeControlSpecVersion;

  //=== VK_KHR_fragment_shading_rate ===
  using VULKAN_HPP_NAMESPACE::KHRFragmentShadingRateExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRFragmentShadingRateSpecVersion;

  //=== VK_EXT_shader_image_atomic_int64 ===
  using VULKAN_HPP_NAMESPACE::EXTShaderImageAtomicInt64ExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTShaderImageAtomicInt64SpecVersion;

  //=== VK_EXT_memory_budget ===
  using VULKAN_HPP_NAMESPACE::EXTMemoryBudgetExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTMemoryBudgetSpecVersion;

  //=== VK_EXT_validation_features ===
  using VULKAN_HPP_NAMESPACE::EXTValidationFeaturesExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTValidationFeaturesSpecVersion;

  //=== VK_EXT_fragment_shader_interlock ===
  using VULKAN_HPP_NAMESPACE::EXTFragmentShaderInterlockExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTFragmentShaderInterlockSpecVersion;

  //=== VK_EXT_ycbcr_image_arrays ===
  using VULKAN_HPP_NAMESPACE::EXTYcbcrImageArraysExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTYcbcrImageArraysSpecVersion;

  //=== VK_EXT_headless_surface ===
  using VULKAN_HPP_NAMESPACE::EXTHeadlessSurfaceExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTHeadlessSurfaceSpecVersion;

  //=== VK_EXT_line_rasterization ===
  using VULKAN_HPP_NAMESPACE::EXTLineRasterizationExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTLineRasterizationSpecVersion;

  //=== VK_EXT_shader_atomic_float ===
  using VULKAN_HPP_NAMESPACE::EXTShaderAtomicFloatExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTShaderAtomicFloatSpecVersion;

  //=== VK_EXT_index_type_uint8 ===
  using VULKAN_HPP_NAMESPACE::EXTIndexTypeUint8ExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTIndexTypeUint8SpecVersion;

  //=== VK_EXT_extended_dynamic_state ===
  using VULKAN_HPP_NAMESPACE::EXTExtendedDynamicStateExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTExtendedDynamicStateSpecVersion;

  //=== VK_EXT_shader_demote_to_helper_invocation ===
  using VULKAN_HPP_NAMESPACE::EXTShaderDemoteToHelperInvocationExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTShaderDemoteToHelperInvocationSpecVersion;

  //=== VK_EXT_texel_buffer_alignment ===
  using VULKAN_HPP_NAMESPACE::EXTTexelBufferAlignmentExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTTexelBufferAlignmentSpecVersion;

  //=== VK_EXT_robustness2 ===
  using VULKAN_HPP_NAMESPACE::EXTRobustness2ExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTRobustness2SpecVersion;

  //=== VK_EXT_custom_border_color ===
  using VULKAN_HPP_NAMESPACE::EXTCustomBorderColorExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTCustomBorderColorSpecVersion;

  //=== VK_KHR_object_refresh ===
  using VULKAN_HPP_NAMESPACE::KHRObjectRefreshExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRObjectRefreshSpecVersion;

  //=== VK_KHR_synchronization2 ===
  using VULKAN_HPP_NAMESPACE::KHRSynchronization2ExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRSynchronization2SpecVersion;

  //=== VK_EXT_ycbcr_2plane_444_formats ===
  using VULKAN_HPP_NAMESPACE::EXTYcbcr2Plane444FormatsExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTYcbcr2Plane444FormatsSpecVersion;

  //=== VK_EXT_image_robustness ===
  using VULKAN_HPP_NAMESPACE::EXTImageRobustnessExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTImageRobustnessSpecVersion;

  //=== VK_KHR_copy_commands2 ===
  using VULKAN_HPP_NAMESPACE::KHRCopyCommands2ExtensionName;
  using VULKAN_HPP_NAMESPACE::KHRCopyCommands2SpecVersion;

  //=== VK_EXT_4444_formats ===
  using VULKAN_HPP_NAMESPACE::EXT4444FormatsExtensionName;
  using VULKAN_HPP_NAMESPACE::EXT4444FormatsSpecVersion;

  //=== VK_EXT_vertex_input_dynamic_state ===
  using VULKAN_HPP_NAMESPACE::EXTVertexInputDynamicStateExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTVertexInputDynamicStateSpecVersion;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===
  using VULKAN_HPP_NAMESPACE::NVExternalSciSyncExtensionName;
  using VULKAN_HPP_NAMESPACE::NVExternalSciSyncSpecVersion;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_memory_sci_buf ===
  using VULKAN_HPP_NAMESPACE::NVExternalMemorySciBufExtensionName;
  using VULKAN_HPP_NAMESPACE::NVExternalMemorySciBufSpecVersion;
#endif /*VK_USE_PLATFORM_SCI*/

  //=== VK_EXT_extended_dynamic_state2 ===
  using VULKAN_HPP_NAMESPACE::EXTExtendedDynamicState2ExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTExtendedDynamicState2SpecVersion;

  //=== VK_EXT_color_write_enable ===
  using VULKAN_HPP_NAMESPACE::EXTColorWriteEnableExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTColorWriteEnableSpecVersion;

  //=== VK_EXT_application_parameters ===
  using VULKAN_HPP_NAMESPACE::EXTApplicationParametersExtensionName;
  using VULKAN_HPP_NAMESPACE::EXTApplicationParametersSpecVersion;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  using VULKAN_HPP_NAMESPACE::NVExternalSciSync2ExtensionName;
  using VULKAN_HPP_NAMESPACE::NVExternalSciSync2SpecVersion;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===
  using VULKAN_HPP_NAMESPACE::QNXExternalMemoryScreenBufferExtensionName;
  using VULKAN_HPP_NAMESPACE::QNXExternalMemoryScreenBufferSpecVersion;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  //========================
  //=== CONSTEXPR VALUEs ===
  //========================
  using VULKAN_HPP_NAMESPACE::HeaderVersion;

  //=========================
  //=== CONSTEXPR CALLEEs ===
  //=========================
  using VULKAN_HPP_NAMESPACE::apiVersionMajor;
  using VULKAN_HPP_NAMESPACE::apiVersionMinor;
  using VULKAN_HPP_NAMESPACE::apiVersionPatch;
  using VULKAN_HPP_NAMESPACE::apiVersionVariant;
  using VULKAN_HPP_NAMESPACE::makeApiVersion;
  using VULKAN_HPP_NAMESPACE::makeVersion;
  using VULKAN_HPP_NAMESPACE::versionMajor;
  using VULKAN_HPP_NAMESPACE::versionMinor;
  using VULKAN_HPP_NAMESPACE::versionPatch;

  //==========================
  //=== CONSTEXPR CALLERs ===
  //==========================
  using VULKAN_HPP_NAMESPACE::ApiVersion;
  using VULKAN_HPP_NAMESPACE::ApiVersion10;
  using VULKAN_HPP_NAMESPACE::ApiVersion11;
  using VULKAN_HPP_NAMESPACE::ApiVersion12;
  using VULKAN_HPP_NAMESPACE::ApiVersion13;
  using VULKAN_HPP_NAMESPACE::HeaderVersionComplete;

  //===============
  //=== STRUCTs ===
  //===============

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::AllocationCallbacks;
  using VULKAN_HPP_NAMESPACE::ApplicationInfo;
  using VULKAN_HPP_NAMESPACE::AttachmentDescription;
  using VULKAN_HPP_NAMESPACE::AttachmentReference;
  using VULKAN_HPP_NAMESPACE::BaseInStructure;
  using VULKAN_HPP_NAMESPACE::BaseOutStructure;
  using VULKAN_HPP_NAMESPACE::BufferCopy;
  using VULKAN_HPP_NAMESPACE::BufferCreateInfo;
  using VULKAN_HPP_NAMESPACE::BufferImageCopy;
  using VULKAN_HPP_NAMESPACE::BufferMemoryBarrier;
  using VULKAN_HPP_NAMESPACE::BufferViewCreateInfo;
  using VULKAN_HPP_NAMESPACE::ClearAttachment;
  using VULKAN_HPP_NAMESPACE::ClearColorValue;
  using VULKAN_HPP_NAMESPACE::ClearDepthStencilValue;
  using VULKAN_HPP_NAMESPACE::ClearRect;
  using VULKAN_HPP_NAMESPACE::ClearValue;
  using VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo;
  using VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo;
  using VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo;
  using VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo;
  using VULKAN_HPP_NAMESPACE::ComponentMapping;
  using VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo;
  using VULKAN_HPP_NAMESPACE::CopyDescriptorSet;
  using VULKAN_HPP_NAMESPACE::DescriptorBufferInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorImageInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolSize;
  using VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo;
  using VULKAN_HPP_NAMESPACE::DeviceCreateInfo;
  using VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo;
  using VULKAN_HPP_NAMESPACE::DispatchIndirectCommand;
  using VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand;
  using VULKAN_HPP_NAMESPACE::DrawIndirectCommand;
  using VULKAN_HPP_NAMESPACE::EventCreateInfo;
  using VULKAN_HPP_NAMESPACE::ExtensionProperties;
  using VULKAN_HPP_NAMESPACE::Extent2D;
  using VULKAN_HPP_NAMESPACE::Extent3D;
  using VULKAN_HPP_NAMESPACE::FenceCreateInfo;
  using VULKAN_HPP_NAMESPACE::FormatProperties;
  using VULKAN_HPP_NAMESPACE::FramebufferCreateInfo;
  using VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo;
  using VULKAN_HPP_NAMESPACE::ImageBlit;
  using VULKAN_HPP_NAMESPACE::ImageCopy;
  using VULKAN_HPP_NAMESPACE::ImageCreateInfo;
  using VULKAN_HPP_NAMESPACE::ImageFormatProperties;
  using VULKAN_HPP_NAMESPACE::ImageMemoryBarrier;
  using VULKAN_HPP_NAMESPACE::ImageResolve;
  using VULKAN_HPP_NAMESPACE::ImageSubresource;
  using VULKAN_HPP_NAMESPACE::ImageSubresourceLayers;
  using VULKAN_HPP_NAMESPACE::ImageSubresourceRange;
  using VULKAN_HPP_NAMESPACE::ImageViewCreateInfo;
  using VULKAN_HPP_NAMESPACE::InstanceCreateInfo;
  using VULKAN_HPP_NAMESPACE::LayerProperties;
  using VULKAN_HPP_NAMESPACE::MappedMemoryRange;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateInfo;
  using VULKAN_HPP_NAMESPACE::MemoryBarrier;
  using VULKAN_HPP_NAMESPACE::MemoryHeap;
  using VULKAN_HPP_NAMESPACE::MemoryRequirements;
  using VULKAN_HPP_NAMESPACE::MemoryType;
  using VULKAN_HPP_NAMESPACE::Offset2D;
  using VULKAN_HPP_NAMESPACE::Offset3D;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties;
  using VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne;
  using VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState;
  using VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PushConstantRange;
  using VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo;
  using VULKAN_HPP_NAMESPACE::QueueFamilyProperties;
  using VULKAN_HPP_NAMESPACE::Rect2D;
  using VULKAN_HPP_NAMESPACE::RenderPassBeginInfo;
  using VULKAN_HPP_NAMESPACE::RenderPassCreateInfo;
  using VULKAN_HPP_NAMESPACE::SamplerCreateInfo;
  using VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo;
  using VULKAN_HPP_NAMESPACE::SpecializationInfo;
  using VULKAN_HPP_NAMESPACE::SpecializationMapEntry;
  using VULKAN_HPP_NAMESPACE::StencilOpState;
  using VULKAN_HPP_NAMESPACE::SubmitInfo;
  using VULKAN_HPP_NAMESPACE::SubpassDependency;
  using VULKAN_HPP_NAMESPACE::SubpassDescription;
  using VULKAN_HPP_NAMESPACE::SubresourceLayout;
  using VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription;
  using VULKAN_HPP_NAMESPACE::VertexInputBindingDescription;
  using VULKAN_HPP_NAMESPACE::Viewport;
  using VULKAN_HPP_NAMESPACE::WriteDescriptorSet;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo;
  using VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfoKHR;
  using VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo;
  using VULKAN_HPP_NAMESPACE::BindBufferMemoryInfoKHR;
  using VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo;
  using VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfoKHR;
  using VULKAN_HPP_NAMESPACE::BindImageMemoryInfo;
  using VULKAN_HPP_NAMESPACE::BindImageMemoryInfoKHR;
  using VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo;
  using VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfoKHR;
  using VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2;
  using VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2KHR;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupportKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo;
  using VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo;
  using VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo;
  using VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo;
  using VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceQueueInfo2;
  using VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo;
  using VULKAN_HPP_NAMESPACE::ExportFenceCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo;
  using VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo;
  using VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ExternalBufferProperties;
  using VULKAN_HPP_NAMESPACE::ExternalBufferPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::ExternalFenceProperties;
  using VULKAN_HPP_NAMESPACE::ExternalFencePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties;
  using VULKAN_HPP_NAMESPACE::ExternalImageFormatPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryProperties;
  using VULKAN_HPP_NAMESPACE::ExternalMemoryPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties;
  using VULKAN_HPP_NAMESPACE::ExternalSemaphorePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::FormatProperties2;
  using VULKAN_HPP_NAMESPACE::FormatProperties2KHR;
  using VULKAN_HPP_NAMESPACE::ImageFormatProperties2;
  using VULKAN_HPP_NAMESPACE::ImageFormatProperties2KHR;
  using VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2;
  using VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2KHR;
  using VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo;
  using VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfoKHR;
  using VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo;
  using VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference;
  using VULKAN_HPP_NAMESPACE::InputAttachmentAspectReferenceKHR;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo;
  using VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfoKHR;
  using VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo;
  using VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfoKHR;
  using VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements;
  using VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirementsKHR;
  using VULKAN_HPP_NAMESPACE::MemoryRequirements2;
  using VULKAN_HPP_NAMESPACE::MemoryRequirements2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceIDPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3PropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParameterFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointerFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointerFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo;
  using VULKAN_HPP_NAMESPACE::QueueFamilyProperties2;
  using VULKAN_HPP_NAMESPACE::QueueFamilyProperties2KHR;
  using VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo;
  using VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo;
  using VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo;
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfoKHR;

  //=== VK_VERSION_1_2 ===
  using VULKAN_HPP_NAMESPACE::AttachmentDescription2;
  using VULKAN_HPP_NAMESPACE::AttachmentDescription2KHR;
  using VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout;
  using VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayoutKHR;
  using VULKAN_HPP_NAMESPACE::AttachmentReference2;
  using VULKAN_HPP_NAMESPACE::AttachmentReference2KHR;
  using VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout;
  using VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayoutKHR;
  using VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo;
  using VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfoEXT;
  using VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfoKHR;
  using VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo;
  using VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ConformanceVersion;
  using VULKAN_HPP_NAMESPACE::ConformanceVersionKHR;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfoEXT;
  using VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport;
  using VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupportEXT;
  using VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo;
  using VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfoKHR;
  using VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo;
  using VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfoKHR;
  using VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo;
  using VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo;
  using VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo;
  using VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo;
  using VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolvePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFloat16Int8FeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64FeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8FeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphorePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo;
  using VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfoKHR;
  using VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2;
  using VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2KHR;
  using VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo;
  using VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo;
  using VULKAN_HPP_NAMESPACE::SemaphoreSignalInfoKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo;
  using VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo;
  using VULKAN_HPP_NAMESPACE::SemaphoreWaitInfoKHR;
  using VULKAN_HPP_NAMESPACE::SubpassBeginInfo;
  using VULKAN_HPP_NAMESPACE::SubpassBeginInfoKHR;
  using VULKAN_HPP_NAMESPACE::SubpassDependency2;
  using VULKAN_HPP_NAMESPACE::SubpassDependency2KHR;
  using VULKAN_HPP_NAMESPACE::SubpassDescription2;
  using VULKAN_HPP_NAMESPACE::SubpassDescription2KHR;
  using VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve;
  using VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolveKHR;
  using VULKAN_HPP_NAMESPACE::SubpassEndInfo;
  using VULKAN_HPP_NAMESPACE::SubpassEndInfoKHR;
  using VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo;
  using VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfoKHR;

  //=== VK_VERSION_1_3 ===
  using VULKAN_HPP_NAMESPACE::BlitImageInfo2;
  using VULKAN_HPP_NAMESPACE::BlitImageInfo2KHR;
  using VULKAN_HPP_NAMESPACE::BufferCopy2;
  using VULKAN_HPP_NAMESPACE::BufferCopy2KHR;
  using VULKAN_HPP_NAMESPACE::BufferImageCopy2;
  using VULKAN_HPP_NAMESPACE::BufferImageCopy2KHR;
  using VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2;
  using VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2KHR;
  using VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo;
  using VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfoKHR;
  using VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo;
  using VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfoKHR;
  using VULKAN_HPP_NAMESPACE::CopyBufferInfo2;
  using VULKAN_HPP_NAMESPACE::CopyBufferInfo2KHR;
  using VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2;
  using VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2KHR;
  using VULKAN_HPP_NAMESPACE::CopyImageInfo2;
  using VULKAN_HPP_NAMESPACE::CopyImageInfo2KHR;
  using VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2;
  using VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2KHR;
  using VULKAN_HPP_NAMESPACE::DependencyInfo;
  using VULKAN_HPP_NAMESPACE::DependencyInfoKHR;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo;
  using VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements;
  using VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirementsKHR;
  using VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements;
  using VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirementsKHR;
  using VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo;
  using VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::FormatProperties3;
  using VULKAN_HPP_NAMESPACE::FormatProperties3KHR;
  using VULKAN_HPP_NAMESPACE::ImageBlit2;
  using VULKAN_HPP_NAMESPACE::ImageBlit2KHR;
  using VULKAN_HPP_NAMESPACE::ImageCopy2;
  using VULKAN_HPP_NAMESPACE::ImageCopy2KHR;
  using VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2;
  using VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2KHR;
  using VULKAN_HPP_NAMESPACE::ImageResolve2;
  using VULKAN_HPP_NAMESPACE::ImageResolve2KHR;
  using VULKAN_HPP_NAMESPACE::MemoryBarrier2;
  using VULKAN_HPP_NAMESPACE::MemoryBarrier2KHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4FeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4PropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2FeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceToolPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedback;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo;
  using VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::RenderingAttachmentInfo;
  using VULKAN_HPP_NAMESPACE::RenderingAttachmentInfoKHR;
  using VULKAN_HPP_NAMESPACE::RenderingInfo;
  using VULKAN_HPP_NAMESPACE::RenderingInfoKHR;
  using VULKAN_HPP_NAMESPACE::ResolveImageInfo2;
  using VULKAN_HPP_NAMESPACE::ResolveImageInfo2KHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo;
  using VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfoKHR;
  using VULKAN_HPP_NAMESPACE::ShaderRequiredSubgroupSizeCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::SubmitInfo2;
  using VULKAN_HPP_NAMESPACE::SubmitInfo2KHR;
  using VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock;
  using VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlockEXT;

  //=== VKSC_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption;
  using VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo;
  using VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo;
  using VULKAN_HPP_NAMESPACE::FaultCallbackInfo;
  using VULKAN_HPP_NAMESPACE::FaultData;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties;
  using VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne;
  using VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry;
  using VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry;
  using VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo;
  using VULKAN_HPP_NAMESPACE::PipelinePoolSize;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR;
  using VULKAN_HPP_NAMESPACE::SurfaceFormatKHR;

  //=== VK_KHR_swapchain ===
  using VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR;
  using VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR;
  using VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::PresentInfoKHR;
  using VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR;
  using VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR;
  using VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR;

  //=== VK_KHR_display_swapchain ===
  using VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR;

  //=== VK_EXT_astc_decode_mode ===
  using VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT;

  //=== VK_KHR_external_memory_fd ===
  using VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR;
  using VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR;

  //=== VK_KHR_external_semaphore_fd ===
  using VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR;
  using VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR;

  //=== VK_KHR_incremental_present ===
  using VULKAN_HPP_NAMESPACE::PresentRegionKHR;
  using VULKAN_HPP_NAMESPACE::PresentRegionsKHR;
  using VULKAN_HPP_NAMESPACE::RectLayerKHR;

  //=== VK_EXT_display_surface_counter ===
  using VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT;

  //=== VK_EXT_display_control ===
  using VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT;
  using VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT;
  using VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT;
  using VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT;

  //=== VK_EXT_discard_rectangles ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT;

  //=== VK_EXT_conservative_rasterization ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT;

  //=== VK_EXT_depth_clip_enable ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT;

  //=== VK_EXT_hdr_metadata ===
  using VULKAN_HPP_NAMESPACE::HdrMetadataEXT;
  using VULKAN_HPP_NAMESPACE::XYColorEXT;

  //=== VK_KHR_shared_presentable_image ===
  using VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR;

  //=== VK_KHR_external_fence_fd ===
  using VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR;
  using VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR;

  //=== VK_KHR_performance_query ===
  using VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceCounterResultKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR;
  using VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR;
  using VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR;

  //=== VK_KHR_get_surface_capabilities2 ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR;
  using VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR;
  using VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR;

  //=== VK_KHR_get_display_properties2 ===
  using VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR;
  using VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR;
  using VULKAN_HPP_NAMESPACE::DisplayProperties2KHR;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT;
  using VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT;

  //=== VK_EXT_sample_locations ===
  using VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT;
  using VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT;
  using VULKAN_HPP_NAMESPACE::SampleLocationEXT;
  using VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT;
  using VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT;

  //=== VK_EXT_blend_operation_advanced ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT;

  //=== VK_EXT_image_drm_format_modifier ===
  using VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT;
  using VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT;
  using VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT;
  using VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT;

  //=== VK_EXT_filter_cubic ===
  using VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT;

  //=== VK_EXT_external_memory_host ===
  using VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT;
  using VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT;

  //=== VK_KHR_shader_clock ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR;

  //=== VK_EXT_calibrated_timestamps ===
  using VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoEXT;

  //=== VK_EXT_vertex_attribute_divisor ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineVertexInputDivisorStateCreateInfoEXT;
  using VULKAN_HPP_NAMESPACE::VertexInputBindingDivisorDescriptionEXT;

  //=== VK_EXT_pci_bus_info ===
  using VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT;

  //=== VK_KHR_fragment_shading_rate ===
  using VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR;
  using VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR;

  //=== VK_EXT_shader_image_atomic_int64 ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT;

  //=== VK_EXT_memory_budget ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT;

  //=== VK_EXT_validation_features ===
  using VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT;

  //=== VK_EXT_fragment_shader_interlock ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT;

  //=== VK_EXT_ycbcr_image_arrays ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT;

  //=== VK_EXT_headless_surface ===
  using VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT;

  //=== VK_EXT_line_rasterization ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceLineRasterizationFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceLineRasterizationPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineRasterizationLineStateCreateInfoEXT;

  //=== VK_EXT_shader_atomic_float ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT;

  //=== VK_EXT_index_type_uint8 ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceIndexTypeUint8FeaturesEXT;

  //=== VK_EXT_extended_dynamic_state ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT;

  //=== VK_EXT_texel_buffer_alignment ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT;

  //=== VK_EXT_robustness2 ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT;

  //=== VK_EXT_custom_border_color ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT;
  using VULKAN_HPP_NAMESPACE::SamplerCustomBorderColorCreateInfoEXT;

  //=== VK_KHR_object_refresh ===
  using VULKAN_HPP_NAMESPACE::RefreshObjectKHR;
  using VULKAN_HPP_NAMESPACE::RefreshObjectListKHR;

  //=== VK_KHR_synchronization2 ===
  using VULKAN_HPP_NAMESPACE::CheckpointData2NV;
  using VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV;

  //=== VK_EXT_ycbcr_2plane_444_formats ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT;

  //=== VK_EXT_4444_formats ===
  using VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT;

  //=== VK_EXT_vertex_input_dynamic_state ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT;
  using VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===
  using VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV;
  using VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV;
  using VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV;
  using VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV;
  using VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV;
  using VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV;
  using VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_memory_sci_buf ===
  using VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV;
  using VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV;
  using VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV;
  using VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciBufFeaturesNV;
#endif /*VK_USE_PLATFORM_SCI*/

  //=== VK_EXT_extended_dynamic_state2 ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT;

  //=== VK_EXT_color_write_enable ===
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT;
  using VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT;

  //=== VK_EXT_application_parameters ===
  using VULKAN_HPP_NAMESPACE::ApplicationParametersEXT;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  using VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV;
  using VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV;
  using VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===
  using VULKAN_HPP_NAMESPACE::ExternalFormatQNX;
  using VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX;
  using VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX;
  using VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX;
  using VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  //===============
  //=== HANDLEs ===
  //===============

  using VULKAN_HPP_NAMESPACE::isVulkanHandleType;

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::Buffer;
  using VULKAN_HPP_NAMESPACE::BufferView;
  using VULKAN_HPP_NAMESPACE::CommandBuffer;
  using VULKAN_HPP_NAMESPACE::CommandPool;
  using VULKAN_HPP_NAMESPACE::DescriptorPool;
  using VULKAN_HPP_NAMESPACE::DescriptorSet;
  using VULKAN_HPP_NAMESPACE::DescriptorSetLayout;
  using VULKAN_HPP_NAMESPACE::Device;
  using VULKAN_HPP_NAMESPACE::DeviceMemory;
  using VULKAN_HPP_NAMESPACE::Event;
  using VULKAN_HPP_NAMESPACE::Fence;
  using VULKAN_HPP_NAMESPACE::Framebuffer;
  using VULKAN_HPP_NAMESPACE::Image;
  using VULKAN_HPP_NAMESPACE::ImageView;
  using VULKAN_HPP_NAMESPACE::Instance;
  using VULKAN_HPP_NAMESPACE::PhysicalDevice;
  using VULKAN_HPP_NAMESPACE::Pipeline;
  using VULKAN_HPP_NAMESPACE::PipelineCache;
  using VULKAN_HPP_NAMESPACE::PipelineLayout;
  using VULKAN_HPP_NAMESPACE::QueryPool;
  using VULKAN_HPP_NAMESPACE::Queue;
  using VULKAN_HPP_NAMESPACE::RenderPass;
  using VULKAN_HPP_NAMESPACE::Sampler;
  using VULKAN_HPP_NAMESPACE::Semaphore;
  using VULKAN_HPP_NAMESPACE::ShaderModule;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion;

  //=== VK_VERSION_1_3 ===
  using VULKAN_HPP_NAMESPACE::PrivateDataSlot;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::SurfaceKHR;

  //=== VK_KHR_swapchain ===
  using VULKAN_HPP_NAMESPACE::SwapchainKHR;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::DisplayKHR;
  using VULKAN_HPP_NAMESPACE::DisplayModeKHR;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  using VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV;
#endif /*VK_USE_PLATFORM_SCI*/

  //======================
  //=== UNIQUE HANDLEs ===
  //======================

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::UniqueBuffer;
  using VULKAN_HPP_NAMESPACE::UniqueBufferView;
  using VULKAN_HPP_NAMESPACE::UniqueCommandBuffer;
  using VULKAN_HPP_NAMESPACE::UniqueDescriptorSet;
  using VULKAN_HPP_NAMESPACE::UniqueDescriptorSetLayout;
  using VULKAN_HPP_NAMESPACE::UniqueDevice;
  using VULKAN_HPP_NAMESPACE::UniqueEvent;
  using VULKAN_HPP_NAMESPACE::UniqueFence;
  using VULKAN_HPP_NAMESPACE::UniqueFramebuffer;
  using VULKAN_HPP_NAMESPACE::UniqueImage;
  using VULKAN_HPP_NAMESPACE::UniqueImageView;
  using VULKAN_HPP_NAMESPACE::UniqueInstance;
  using VULKAN_HPP_NAMESPACE::UniquePipeline;
  using VULKAN_HPP_NAMESPACE::UniquePipelineCache;
  using VULKAN_HPP_NAMESPACE::UniquePipelineLayout;
  using VULKAN_HPP_NAMESPACE::UniqueRenderPass;
  using VULKAN_HPP_NAMESPACE::UniqueSampler;
  using VULKAN_HPP_NAMESPACE::UniqueSemaphore;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::UniqueSamplerYcbcrConversion;

  //=== VK_VERSION_1_3 ===
  using VULKAN_HPP_NAMESPACE::UniquePrivateDataSlot;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::UniqueSurfaceKHR;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::UniqueDisplayKHR;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::UniqueDebugUtilsMessengerEXT;
  using VULKAN_HPP_NAMESPACE::UniqueHandleTraits;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  //======================
  //=== SHARED HANDLEs ===
  //======================

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

  //=== VK_VERSION_1_0 ===
  using VULKAN_HPP_NAMESPACE::SharedBuffer;
  using VULKAN_HPP_NAMESPACE::SharedBufferView;
  using VULKAN_HPP_NAMESPACE::SharedCommandBuffer;
  using VULKAN_HPP_NAMESPACE::SharedCommandPool;
  using VULKAN_HPP_NAMESPACE::SharedDescriptorPool;
  using VULKAN_HPP_NAMESPACE::SharedDescriptorSet;
  using VULKAN_HPP_NAMESPACE::SharedDescriptorSetLayout;
  using VULKAN_HPP_NAMESPACE::SharedDevice;
  using VULKAN_HPP_NAMESPACE::SharedDeviceMemory;
  using VULKAN_HPP_NAMESPACE::SharedEvent;
  using VULKAN_HPP_NAMESPACE::SharedFence;
  using VULKAN_HPP_NAMESPACE::SharedFramebuffer;
  using VULKAN_HPP_NAMESPACE::SharedImage;
  using VULKAN_HPP_NAMESPACE::SharedImageView;
  using VULKAN_HPP_NAMESPACE::SharedInstance;
  using VULKAN_HPP_NAMESPACE::SharedPhysicalDevice;
  using VULKAN_HPP_NAMESPACE::SharedPipeline;
  using VULKAN_HPP_NAMESPACE::SharedPipelineCache;
  using VULKAN_HPP_NAMESPACE::SharedPipelineLayout;
  using VULKAN_HPP_NAMESPACE::SharedQueryPool;
  using VULKAN_HPP_NAMESPACE::SharedQueue;
  using VULKAN_HPP_NAMESPACE::SharedRenderPass;
  using VULKAN_HPP_NAMESPACE::SharedSampler;
  using VULKAN_HPP_NAMESPACE::SharedSemaphore;
  using VULKAN_HPP_NAMESPACE::SharedShaderModule;

  //=== VK_VERSION_1_1 ===
  using VULKAN_HPP_NAMESPACE::SharedSamplerYcbcrConversion;

  //=== VK_VERSION_1_3 ===
  using VULKAN_HPP_NAMESPACE::SharedPrivateDataSlot;

  //=== VK_KHR_surface ===
  using VULKAN_HPP_NAMESPACE::SharedSurfaceKHR;

  //=== VK_KHR_swapchain ===
  using VULKAN_HPP_NAMESPACE::SharedSwapchainKHR;

  //=== VK_KHR_display ===
  using VULKAN_HPP_NAMESPACE::SharedDisplayKHR;
  using VULKAN_HPP_NAMESPACE::SharedDisplayModeKHR;

  //=== VK_EXT_debug_utils ===
  using VULKAN_HPP_NAMESPACE::SharedDebugUtilsMessengerEXT;

#  if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  using VULKAN_HPP_NAMESPACE::SharedSemaphoreSciSyncPoolNV;
#  endif /*VK_USE_PLATFORM_SCI*/
  using VULKAN_HPP_NAMESPACE::SharedHandleTraits;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  //===========================
  //=== COMMAND Definitions ===
  //===========================
  using VULKAN_HPP_NAMESPACE::createInstance;
  using VULKAN_HPP_NAMESPACE::enumerateInstanceExtensionProperties;
  using VULKAN_HPP_NAMESPACE::enumerateInstanceLayerProperties;
  using VULKAN_HPP_NAMESPACE::enumerateInstanceVersion;

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  using VULKAN_HPP_NAMESPACE::createInstanceUnique;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  using VULKAN_HPP_NAMESPACE::StructExtends;
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if defined( VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL )
  using VULKAN_HPP_NAMESPACE::DynamicLoader;
#endif /*VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL*/

  //=====================
  //=== Format Traits ===
  //=====================
  using VULKAN_HPP_NAMESPACE::blockExtent;
  using VULKAN_HPP_NAMESPACE::blockSize;
  using VULKAN_HPP_NAMESPACE::compatibilityClass;
  using VULKAN_HPP_NAMESPACE::componentBits;
  using VULKAN_HPP_NAMESPACE::componentCount;
  using VULKAN_HPP_NAMESPACE::componentName;
  using VULKAN_HPP_NAMESPACE::componentNumericFormat;
  using VULKAN_HPP_NAMESPACE::componentPlaneIndex;
  using VULKAN_HPP_NAMESPACE::componentsAreCompressed;
  using VULKAN_HPP_NAMESPACE::compressionScheme;
  using VULKAN_HPP_NAMESPACE::isCompressed;
  using VULKAN_HPP_NAMESPACE::packed;
  using VULKAN_HPP_NAMESPACE::planeCompatibleFormat;
  using VULKAN_HPP_NAMESPACE::planeCount;
  using VULKAN_HPP_NAMESPACE::planeHeightDivisor;
  using VULKAN_HPP_NAMESPACE::planeWidthDivisor;
  using VULKAN_HPP_NAMESPACE::texelsPerBlock;

  //======================================
  //=== Extension inspection functions ===
  //======================================
  using VULKAN_HPP_NAMESPACE::getDeprecatedExtensions;
  using VULKAN_HPP_NAMESPACE::getDeviceExtensions;
  using VULKAN_HPP_NAMESPACE::getExtensionDepends;
  using VULKAN_HPP_NAMESPACE::getExtensionDeprecatedBy;
  using VULKAN_HPP_NAMESPACE::getExtensionObsoletedBy;
  using VULKAN_HPP_NAMESPACE::getExtensionPromotedTo;
  using VULKAN_HPP_NAMESPACE::getInstanceExtensions;
  using VULKAN_HPP_NAMESPACE::getObsoletedExtensions;
  using VULKAN_HPP_NAMESPACE::getPromotedExtensions;
  using VULKAN_HPP_NAMESPACE::isDeprecatedExtension;
  using VULKAN_HPP_NAMESPACE::isDeviceExtension;
  using VULKAN_HPP_NAMESPACE::isInstanceExtension;
  using VULKAN_HPP_NAMESPACE::isObsoletedExtension;
  using VULKAN_HPP_NAMESPACE::isPromotedExtension;

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    //======================
    //=== RAII HARDCODED ===
    //======================

    using VULKAN_HPP_RAII_NAMESPACE::Context;
    using VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher;
    using VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher;
    using VULKAN_HPP_RAII_NAMESPACE::exchange;
    using VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher;

    //====================
    //=== RAII HANDLEs ===
    //====================

    //=== VK_VERSION_1_0 ===
    using VULKAN_HPP_RAII_NAMESPACE::Buffer;
    using VULKAN_HPP_RAII_NAMESPACE::BufferView;
    using VULKAN_HPP_RAII_NAMESPACE::CommandBuffer;
    using VULKAN_HPP_RAII_NAMESPACE::CommandBuffers;
    using VULKAN_HPP_RAII_NAMESPACE::CommandPool;
    using VULKAN_HPP_RAII_NAMESPACE::DescriptorPool;
    using VULKAN_HPP_RAII_NAMESPACE::DescriptorSet;
    using VULKAN_HPP_RAII_NAMESPACE::DescriptorSetLayout;
    using VULKAN_HPP_RAII_NAMESPACE::DescriptorSets;
    using VULKAN_HPP_RAII_NAMESPACE::Device;
    using VULKAN_HPP_RAII_NAMESPACE::DeviceMemory;
    using VULKAN_HPP_RAII_NAMESPACE::Event;
    using VULKAN_HPP_RAII_NAMESPACE::Fence;
    using VULKAN_HPP_RAII_NAMESPACE::Framebuffer;
    using VULKAN_HPP_RAII_NAMESPACE::Image;
    using VULKAN_HPP_RAII_NAMESPACE::ImageView;
    using VULKAN_HPP_RAII_NAMESPACE::Instance;
    using VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice;
    using VULKAN_HPP_RAII_NAMESPACE::PhysicalDevices;
    using VULKAN_HPP_RAII_NAMESPACE::Pipeline;
    using VULKAN_HPP_RAII_NAMESPACE::PipelineCache;
    using VULKAN_HPP_RAII_NAMESPACE::PipelineLayout;
    using VULKAN_HPP_RAII_NAMESPACE::Pipelines;
    using VULKAN_HPP_RAII_NAMESPACE::QueryPool;
    using VULKAN_HPP_RAII_NAMESPACE::Queue;
    using VULKAN_HPP_RAII_NAMESPACE::RenderPass;
    using VULKAN_HPP_RAII_NAMESPACE::Sampler;
    using VULKAN_HPP_RAII_NAMESPACE::Semaphore;
    using VULKAN_HPP_RAII_NAMESPACE::ShaderModule;

    //=== VK_VERSION_1_1 ===
    using VULKAN_HPP_RAII_NAMESPACE::SamplerYcbcrConversion;

    //=== VK_VERSION_1_3 ===
    using VULKAN_HPP_RAII_NAMESPACE::PrivateDataSlot;

    //=== VK_KHR_surface ===
    using VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR;

    //=== VK_KHR_swapchain ===
    using VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR;
    using VULKAN_HPP_RAII_NAMESPACE::SwapchainKHRs;

    //=== VK_KHR_display ===
    using VULKAN_HPP_RAII_NAMESPACE::DisplayKHR;
    using VULKAN_HPP_RAII_NAMESPACE::DisplayKHRs;
    using VULKAN_HPP_RAII_NAMESPACE::DisplayModeKHR;

    //=== VK_EXT_debug_utils ===
    using VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT;

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync2 ===
    using VULKAN_HPP_RAII_NAMESPACE::SemaphoreSciSyncPoolNV;
#  endif /*VK_USE_PLATFORM_SCI*/

  }  // namespace VULKAN_HPP_RAII_NAMESPACE
#endif
}  // namespace VULKAN_HPP_NAMESPACE
