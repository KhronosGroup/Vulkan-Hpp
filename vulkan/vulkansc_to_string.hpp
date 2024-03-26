// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_TO_STRING_HPP
#define VULKAN_TO_STRING_HPP

#include <vulkan/vulkansc_enums.hpp>

#if __cpp_lib_format
#  include <format>  // std::format
#else
#  include <sstream>  // std::stringstream
#endif

namespace VULKAN_HPP_NAMESPACE
{

  //==========================
  //=== BITMASKs to_string ===
  //==========================

  //=== VK_VERSION_1_0 ===

  VULKAN_HPP_INLINE std::string to_string( FormatFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & FormatFeatureFlagBits::eSampledImage )
      result += "SampledImage | ";
    if ( value & FormatFeatureFlagBits::eStorageImage )
      result += "StorageImage | ";
    if ( value & FormatFeatureFlagBits::eStorageImageAtomic )
      result += "StorageImageAtomic | ";
    if ( value & FormatFeatureFlagBits::eUniformTexelBuffer )
      result += "UniformTexelBuffer | ";
    if ( value & FormatFeatureFlagBits::eStorageTexelBuffer )
      result += "StorageTexelBuffer | ";
    if ( value & FormatFeatureFlagBits::eStorageTexelBufferAtomic )
      result += "StorageTexelBufferAtomic | ";
    if ( value & FormatFeatureFlagBits::eVertexBuffer )
      result += "VertexBuffer | ";
    if ( value & FormatFeatureFlagBits::eColorAttachment )
      result += "ColorAttachment | ";
    if ( value & FormatFeatureFlagBits::eColorAttachmentBlend )
      result += "ColorAttachmentBlend | ";
    if ( value & FormatFeatureFlagBits::eDepthStencilAttachment )
      result += "DepthStencilAttachment | ";
    if ( value & FormatFeatureFlagBits::eBlitSrc )
      result += "BlitSrc | ";
    if ( value & FormatFeatureFlagBits::eBlitDst )
      result += "BlitDst | ";
    if ( value & FormatFeatureFlagBits::eSampledImageFilterLinear )
      result += "SampledImageFilterLinear | ";
    if ( value & FormatFeatureFlagBits::eTransferSrc )
      result += "TransferSrc | ";
    if ( value & FormatFeatureFlagBits::eTransferDst )
      result += "TransferDst | ";
    if ( value & FormatFeatureFlagBits::eMidpointChromaSamples )
      result += "MidpointChromaSamples | ";
    if ( value & FormatFeatureFlagBits::eSampledImageYcbcrConversionLinearFilter )
      result += "SampledImageYcbcrConversionLinearFilter | ";
    if ( value & FormatFeatureFlagBits::eSampledImageYcbcrConversionSeparateReconstructionFilter )
      result += "SampledImageYcbcrConversionSeparateReconstructionFilter | ";
    if ( value & FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicit )
      result += "SampledImageYcbcrConversionChromaReconstructionExplicit | ";
    if ( value & FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable )
      result += "SampledImageYcbcrConversionChromaReconstructionExplicitForceable | ";
    if ( value & FormatFeatureFlagBits::eDisjoint )
      result += "Disjoint | ";
    if ( value & FormatFeatureFlagBits::eCositedChromaSamples )
      result += "CositedChromaSamples | ";
    if ( value & FormatFeatureFlagBits::eSampledImageFilterMinmax )
      result += "SampledImageFilterMinmax | ";
    if ( value & FormatFeatureFlagBits::eSampledImageFilterCubicEXT )
      result += "SampledImageFilterCubicEXT | ";
    if ( value & FormatFeatureFlagBits::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ImageCreateFlagBits::eSparseBinding )
      result += "SparseBinding | ";
    if ( value & ImageCreateFlagBits::eSparseResidency )
      result += "SparseResidency | ";
    if ( value & ImageCreateFlagBits::eSparseAliased )
      result += "SparseAliased | ";
    if ( value & ImageCreateFlagBits::eMutableFormat )
      result += "MutableFormat | ";
    if ( value & ImageCreateFlagBits::eCubeCompatible )
      result += "CubeCompatible | ";
    if ( value & ImageCreateFlagBits::eAlias )
      result += "Alias | ";
    if ( value & ImageCreateFlagBits::eSplitInstanceBindRegions )
      result += "SplitInstanceBindRegions | ";
    if ( value & ImageCreateFlagBits::e2DArrayCompatible )
      result += "2DArrayCompatible | ";
    if ( value & ImageCreateFlagBits::eBlockTexelViewCompatible )
      result += "BlockTexelViewCompatible | ";
    if ( value & ImageCreateFlagBits::eExtendedUsage )
      result += "ExtendedUsage | ";
    if ( value & ImageCreateFlagBits::eProtected )
      result += "Protected | ";
    if ( value & ImageCreateFlagBits::eDisjoint )
      result += "Disjoint | ";
    if ( value & ImageCreateFlagBits::eSampleLocationsCompatibleDepthEXT )
      result += "SampleLocationsCompatibleDepthEXT | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageUsageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ImageUsageFlagBits::eTransferSrc )
      result += "TransferSrc | ";
    if ( value & ImageUsageFlagBits::eTransferDst )
      result += "TransferDst | ";
    if ( value & ImageUsageFlagBits::eSampled )
      result += "Sampled | ";
    if ( value & ImageUsageFlagBits::eStorage )
      result += "Storage | ";
    if ( value & ImageUsageFlagBits::eColorAttachment )
      result += "ColorAttachment | ";
    if ( value & ImageUsageFlagBits::eDepthStencilAttachment )
      result += "DepthStencilAttachment | ";
    if ( value & ImageUsageFlagBits::eTransientAttachment )
      result += "TransientAttachment | ";
    if ( value & ImageUsageFlagBits::eInputAttachment )
      result += "InputAttachment | ";
    if ( value & ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( InstanceCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryHeapFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & MemoryHeapFlagBits::eDeviceLocal )
      result += "DeviceLocal | ";
    if ( value & MemoryHeapFlagBits::eMultiInstance )
      result += "MultiInstance | ";
    if ( value & MemoryHeapFlagBits::eSeuSafe )
      result += "SeuSafe | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryPropertyFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & MemoryPropertyFlagBits::eDeviceLocal )
      result += "DeviceLocal | ";
    if ( value & MemoryPropertyFlagBits::eHostVisible )
      result += "HostVisible | ";
    if ( value & MemoryPropertyFlagBits::eHostCoherent )
      result += "HostCoherent | ";
    if ( value & MemoryPropertyFlagBits::eHostCached )
      result += "HostCached | ";
    if ( value & MemoryPropertyFlagBits::eLazilyAllocated )
      result += "LazilyAllocated | ";
    if ( value & MemoryPropertyFlagBits::eProtected )
      result += "Protected | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( QueueFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & QueueFlagBits::eGraphics )
      result += "Graphics | ";
    if ( value & QueueFlagBits::eCompute )
      result += "Compute | ";
    if ( value & QueueFlagBits::eTransfer )
      result += "Transfer | ";
    if ( value & QueueFlagBits::eProtected )
      result += "Protected | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SampleCountFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SampleCountFlagBits::e1 )
      result += "1 | ";
    if ( value & SampleCountFlagBits::e2 )
      result += "2 | ";
    if ( value & SampleCountFlagBits::e4 )
      result += "4 | ";
    if ( value & SampleCountFlagBits::e8 )
      result += "8 | ";
    if ( value & SampleCountFlagBits::e16 )
      result += "16 | ";
    if ( value & SampleCountFlagBits::e32 )
      result += "32 | ";
    if ( value & SampleCountFlagBits::e64 )
      result += "64 | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceQueueCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DeviceQueueCreateFlagBits::eProtected )
      result += "Protected | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineStageFlags value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & PipelineStageFlagBits::eTopOfPipe )
      result += "TopOfPipe | ";
    if ( value & PipelineStageFlagBits::eDrawIndirect )
      result += "DrawIndirect | ";
    if ( value & PipelineStageFlagBits::eVertexInput )
      result += "VertexInput | ";
    if ( value & PipelineStageFlagBits::eVertexShader )
      result += "VertexShader | ";
    if ( value & PipelineStageFlagBits::eTessellationControlShader )
      result += "TessellationControlShader | ";
    if ( value & PipelineStageFlagBits::eTessellationEvaluationShader )
      result += "TessellationEvaluationShader | ";
    if ( value & PipelineStageFlagBits::eGeometryShader )
      result += "GeometryShader | ";
    if ( value & PipelineStageFlagBits::eFragmentShader )
      result += "FragmentShader | ";
    if ( value & PipelineStageFlagBits::eEarlyFragmentTests )
      result += "EarlyFragmentTests | ";
    if ( value & PipelineStageFlagBits::eLateFragmentTests )
      result += "LateFragmentTests | ";
    if ( value & PipelineStageFlagBits::eColorAttachmentOutput )
      result += "ColorAttachmentOutput | ";
    if ( value & PipelineStageFlagBits::eComputeShader )
      result += "ComputeShader | ";
    if ( value & PipelineStageFlagBits::eTransfer )
      result += "Transfer | ";
    if ( value & PipelineStageFlagBits::eBottomOfPipe )
      result += "BottomOfPipe | ";
    if ( value & PipelineStageFlagBits::eHost )
      result += "Host | ";
    if ( value & PipelineStageFlagBits::eAllGraphics )
      result += "AllGraphics | ";
    if ( value & PipelineStageFlagBits::eAllCommands )
      result += "AllCommands | ";
    if ( value & PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryMapFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageAspectFlags value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & ImageAspectFlagBits::eColor )
      result += "Color | ";
    if ( value & ImageAspectFlagBits::eDepth )
      result += "Depth | ";
    if ( value & ImageAspectFlagBits::eStencil )
      result += "Stencil | ";
    if ( value & ImageAspectFlagBits::eMetadata )
      result += "Metadata | ";
    if ( value & ImageAspectFlagBits::ePlane0 )
      result += "Plane0 | ";
    if ( value & ImageAspectFlagBits::ePlane1 )
      result += "Plane1 | ";
    if ( value & ImageAspectFlagBits::ePlane2 )
      result += "Plane2 | ";
    if ( value & ImageAspectFlagBits::eMemoryPlane0EXT )
      result += "MemoryPlane0EXT | ";
    if ( value & ImageAspectFlagBits::eMemoryPlane1EXT )
      result += "MemoryPlane1EXT | ";
    if ( value & ImageAspectFlagBits::eMemoryPlane2EXT )
      result += "MemoryPlane2EXT | ";
    if ( value & ImageAspectFlagBits::eMemoryPlane3EXT )
      result += "MemoryPlane3EXT | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( FenceCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & FenceCreateFlagBits::eSignaled )
      result += "Signaled | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( EventCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & EventCreateFlagBits::eDeviceOnly )
      result += "DeviceOnly | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( QueryPipelineStatisticFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & QueryPipelineStatisticFlagBits::eInputAssemblyVertices )
      result += "InputAssemblyVertices | ";
    if ( value & QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives )
      result += "InputAssemblyPrimitives | ";
    if ( value & QueryPipelineStatisticFlagBits::eVertexShaderInvocations )
      result += "VertexShaderInvocations | ";
    if ( value & QueryPipelineStatisticFlagBits::eGeometryShaderInvocations )
      result += "GeometryShaderInvocations | ";
    if ( value & QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives )
      result += "GeometryShaderPrimitives | ";
    if ( value & QueryPipelineStatisticFlagBits::eClippingInvocations )
      result += "ClippingInvocations | ";
    if ( value & QueryPipelineStatisticFlagBits::eClippingPrimitives )
      result += "ClippingPrimitives | ";
    if ( value & QueryPipelineStatisticFlagBits::eFragmentShaderInvocations )
      result += "FragmentShaderInvocations | ";
    if ( value & QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches )
      result += "TessellationControlShaderPatches | ";
    if ( value & QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations )
      result += "TessellationEvaluationShaderInvocations | ";
    if ( value & QueryPipelineStatisticFlagBits::eComputeShaderInvocations )
      result += "ComputeShaderInvocations | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( QueryPoolCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( QueryResultFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & QueryResultFlagBits::e64 )
      result += "64 | ";
    if ( value & QueryResultFlagBits::eWait )
      result += "Wait | ";
    if ( value & QueryResultFlagBits::eWithAvailability )
      result += "WithAvailability | ";
    if ( value & QueryResultFlagBits::ePartial )
      result += "Partial | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( BufferCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & BufferCreateFlagBits::eSparseBinding )
      result += "SparseBinding | ";
    if ( value & BufferCreateFlagBits::eSparseResidency )
      result += "SparseResidency | ";
    if ( value & BufferCreateFlagBits::eSparseAliased )
      result += "SparseAliased | ";
    if ( value & BufferCreateFlagBits::eProtected )
      result += "Protected | ";
    if ( value & BufferCreateFlagBits::eDeviceAddressCaptureReplay )
      result += "DeviceAddressCaptureReplay | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( BufferUsageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & BufferUsageFlagBits::eTransferSrc )
      result += "TransferSrc | ";
    if ( value & BufferUsageFlagBits::eTransferDst )
      result += "TransferDst | ";
    if ( value & BufferUsageFlagBits::eUniformTexelBuffer )
      result += "UniformTexelBuffer | ";
    if ( value & BufferUsageFlagBits::eStorageTexelBuffer )
      result += "StorageTexelBuffer | ";
    if ( value & BufferUsageFlagBits::eUniformBuffer )
      result += "UniformBuffer | ";
    if ( value & BufferUsageFlagBits::eStorageBuffer )
      result += "StorageBuffer | ";
    if ( value & BufferUsageFlagBits::eIndexBuffer )
      result += "IndexBuffer | ";
    if ( value & BufferUsageFlagBits::eVertexBuffer )
      result += "VertexBuffer | ";
    if ( value & BufferUsageFlagBits::eIndirectBuffer )
      result += "IndirectBuffer | ";
    if ( value & BufferUsageFlagBits::eShaderDeviceAddress )
      result += "ShaderDeviceAddress | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( BufferViewCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageViewCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCacheCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PipelineCacheCreateFlagBits::eExternallySynchronized )
      result += "ExternallySynchronized | ";
    if ( value & PipelineCacheCreateFlagBits::eReadOnly )
      result += "ReadOnly | ";
    if ( value & PipelineCacheCreateFlagBits::eUseApplicationStorage )
      result += "UseApplicationStorage | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ColorComponentFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ColorComponentFlagBits::eR )
      result += "R | ";
    if ( value & ColorComponentFlagBits::eG )
      result += "G | ";
    if ( value & ColorComponentFlagBits::eB )
      result += "B | ";
    if ( value & ColorComponentFlagBits::eA )
      result += "A | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( CullModeFlags value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & CullModeFlagBits::eFront )
      result += "Front | ";
    if ( value & CullModeFlagBits::eBack )
      result += "Back | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineColorBlendStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PipelineCreateFlagBits::eDisableOptimization )
      result += "DisableOptimization | ";
    if ( value & PipelineCreateFlagBits::eViewIndexFromDeviceIndex )
      result += "ViewIndexFromDeviceIndex | ";
    if ( value & PipelineCreateFlagBits::eDispatchBase )
      result += "DispatchBase | ";
    if ( value & PipelineCreateFlagBits::eFailOnPipelineCompileRequired )
      result += "FailOnPipelineCompileRequired | ";
    if ( value & PipelineCreateFlagBits::eEarlyReturnOnFailure )
      result += "EarlyReturnOnFailure | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineDepthStencilStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineDynamicStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineInputAssemblyStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineLayoutCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineMultisampleStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineShaderStageCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PipelineShaderStageCreateFlagBits::eAllowVaryingSubgroupSize )
      result += "AllowVaryingSubgroupSize | ";
    if ( value & PipelineShaderStageCreateFlagBits::eRequireFullSubgroups )
      result += "RequireFullSubgroups | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineTessellationStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineVertexInputStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineViewportStateCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( ShaderStageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ShaderStageFlagBits::eVertex )
      result += "Vertex | ";
    if ( value & ShaderStageFlagBits::eTessellationControl )
      result += "TessellationControl | ";
    if ( value & ShaderStageFlagBits::eTessellationEvaluation )
      result += "TessellationEvaluation | ";
    if ( value & ShaderStageFlagBits::eGeometry )
      result += "Geometry | ";
    if ( value & ShaderStageFlagBits::eFragment )
      result += "Fragment | ";
    if ( value & ShaderStageFlagBits::eCompute )
      result += "Compute | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorPoolCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
      result += "FreeDescriptorSet | ";
    if ( value & DescriptorPoolCreateFlagBits::eUpdateAfterBind )
      result += "UpdateAfterBind | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorPoolResetFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorSetLayoutCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool )
      result += "UpdateAfterBindPool | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( AccessFlags value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & AccessFlagBits::eIndirectCommandRead )
      result += "IndirectCommandRead | ";
    if ( value & AccessFlagBits::eIndexRead )
      result += "IndexRead | ";
    if ( value & AccessFlagBits::eVertexAttributeRead )
      result += "VertexAttributeRead | ";
    if ( value & AccessFlagBits::eUniformRead )
      result += "UniformRead | ";
    if ( value & AccessFlagBits::eInputAttachmentRead )
      result += "InputAttachmentRead | ";
    if ( value & AccessFlagBits::eShaderRead )
      result += "ShaderRead | ";
    if ( value & AccessFlagBits::eShaderWrite )
      result += "ShaderWrite | ";
    if ( value & AccessFlagBits::eColorAttachmentRead )
      result += "ColorAttachmentRead | ";
    if ( value & AccessFlagBits::eColorAttachmentWrite )
      result += "ColorAttachmentWrite | ";
    if ( value & AccessFlagBits::eDepthStencilAttachmentRead )
      result += "DepthStencilAttachmentRead | ";
    if ( value & AccessFlagBits::eDepthStencilAttachmentWrite )
      result += "DepthStencilAttachmentWrite | ";
    if ( value & AccessFlagBits::eTransferRead )
      result += "TransferRead | ";
    if ( value & AccessFlagBits::eTransferWrite )
      result += "TransferWrite | ";
    if ( value & AccessFlagBits::eHostRead )
      result += "HostRead | ";
    if ( value & AccessFlagBits::eHostWrite )
      result += "HostWrite | ";
    if ( value & AccessFlagBits::eMemoryRead )
      result += "MemoryRead | ";
    if ( value & AccessFlagBits::eMemoryWrite )
      result += "MemoryWrite | ";
    if ( value & AccessFlagBits::eColorAttachmentReadNoncoherentEXT )
      result += "ColorAttachmentReadNoncoherentEXT | ";
    if ( value & AccessFlagBits::eFragmentShadingRateAttachmentReadKHR )
      result += "FragmentShadingRateAttachmentReadKHR | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( AttachmentDescriptionFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & AttachmentDescriptionFlagBits::eMayAlias )
      result += "MayAlias | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DependencyFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DependencyFlagBits::eByRegion )
      result += "ByRegion | ";
    if ( value & DependencyFlagBits::eDeviceGroup )
      result += "DeviceGroup | ";
    if ( value & DependencyFlagBits::eViewLocal )
      result += "ViewLocal | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( FramebufferCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & FramebufferCreateFlagBits::eImageless )
      result += "Imageless | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( RenderPassCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( SubpassDescriptionFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandPoolCreateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & CommandPoolCreateFlagBits::eTransient )
      result += "Transient | ";
    if ( value & CommandPoolCreateFlagBits::eResetCommandBuffer )
      result += "ResetCommandBuffer | ";
    if ( value & CommandPoolCreateFlagBits::eProtected )
      result += "Protected | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandPoolResetFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandBufferResetFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & CommandBufferResetFlagBits::eReleaseResources )
      result += "ReleaseResources | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandBufferUsageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & CommandBufferUsageFlagBits::eOneTimeSubmit )
      result += "OneTimeSubmit | ";
    if ( value & CommandBufferUsageFlagBits::eRenderPassContinue )
      result += "RenderPassContinue | ";
    if ( value & CommandBufferUsageFlagBits::eSimultaneousUse )
      result += "SimultaneousUse | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( QueryControlFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & QueryControlFlagBits::ePrecise )
      result += "Precise | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( StencilFaceFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & StencilFaceFlagBits::eFront )
      result += "Front | ";
    if ( value & StencilFaceFlagBits::eBack )
      result += "Back | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_VERSION_1_1 ===

  VULKAN_HPP_INLINE std::string to_string( SubgroupFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SubgroupFeatureFlagBits::eBasic )
      result += "Basic | ";
    if ( value & SubgroupFeatureFlagBits::eVote )
      result += "Vote | ";
    if ( value & SubgroupFeatureFlagBits::eArithmetic )
      result += "Arithmetic | ";
    if ( value & SubgroupFeatureFlagBits::eBallot )
      result += "Ballot | ";
    if ( value & SubgroupFeatureFlagBits::eShuffle )
      result += "Shuffle | ";
    if ( value & SubgroupFeatureFlagBits::eShuffleRelative )
      result += "ShuffleRelative | ";
    if ( value & SubgroupFeatureFlagBits::eClustered )
      result += "Clustered | ";
    if ( value & SubgroupFeatureFlagBits::eQuad )
      result += "Quad | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PeerMemoryFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PeerMemoryFeatureFlagBits::eCopySrc )
      result += "CopySrc | ";
    if ( value & PeerMemoryFeatureFlagBits::eCopyDst )
      result += "CopyDst | ";
    if ( value & PeerMemoryFeatureFlagBits::eGenericSrc )
      result += "GenericSrc | ";
    if ( value & PeerMemoryFeatureFlagBits::eGenericDst )
      result += "GenericDst | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryAllocateFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & MemoryAllocateFlagBits::eDeviceMask )
      result += "DeviceMask | ";
    if ( value & MemoryAllocateFlagBits::eDeviceAddress )
      result += "DeviceAddress | ";
    if ( value & MemoryAllocateFlagBits::eDeviceAddressCaptureReplay )
      result += "DeviceAddressCaptureReplay | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalMemoryHandleTypeFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalMemoryHandleTypeFlagBits::eOpaqueFd )
      result += "OpaqueFd | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eOpaqueWin32 )
      result += "OpaqueWin32 | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eOpaqueWin32Kmt )
      result += "OpaqueWin32Kmt | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eD3D11Texture )
      result += "D3D11Texture | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eD3D11TextureKmt )
      result += "D3D11TextureKmt | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eD3D12Heap )
      result += "D3D12Heap | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eD3D12Resource )
      result += "D3D12Resource | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eDmaBufEXT )
      result += "DmaBufEXT | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eHostAllocationEXT )
      result += "HostAllocationEXT | ";
    if ( value & ExternalMemoryHandleTypeFlagBits::eHostMappedForeignMemoryEXT )
      result += "HostMappedForeignMemoryEXT | ";
#if defined( VK_USE_PLATFORM_SCI )
    if ( value & ExternalMemoryHandleTypeFlagBits::eSciBufNV )
      result += "SciBufNV | ";
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    if ( value & ExternalMemoryHandleTypeFlagBits::eScreenBufferQNX )
      result += "ScreenBufferQNX | ";
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalMemoryFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalMemoryFeatureFlagBits::eDedicatedOnly )
      result += "DedicatedOnly | ";
    if ( value & ExternalMemoryFeatureFlagBits::eExportable )
      result += "Exportable | ";
    if ( value & ExternalMemoryFeatureFlagBits::eImportable )
      result += "Importable | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalFenceHandleTypeFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalFenceHandleTypeFlagBits::eOpaqueFd )
      result += "OpaqueFd | ";
    if ( value & ExternalFenceHandleTypeFlagBits::eOpaqueWin32 )
      result += "OpaqueWin32 | ";
    if ( value & ExternalFenceHandleTypeFlagBits::eOpaqueWin32Kmt )
      result += "OpaqueWin32Kmt | ";
    if ( value & ExternalFenceHandleTypeFlagBits::eSyncFd )
      result += "SyncFd | ";
#if defined( VK_USE_PLATFORM_SCI )
    if ( value & ExternalFenceHandleTypeFlagBits::eSciSyncObjNV )
      result += "SciSyncObjNV | ";
    if ( value & ExternalFenceHandleTypeFlagBits::eSciSyncFenceNV )
      result += "SciSyncFenceNV | ";
#endif /*VK_USE_PLATFORM_SCI*/

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalFenceFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalFenceFeatureFlagBits::eExportable )
      result += "Exportable | ";
    if ( value & ExternalFenceFeatureFlagBits::eImportable )
      result += "Importable | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( FenceImportFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & FenceImportFlagBits::eTemporary )
      result += "Temporary | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreImportFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SemaphoreImportFlagBits::eTemporary )
      result += "Temporary | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalSemaphoreHandleTypeFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eOpaqueFd )
      result += "OpaqueFd | ";
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32 )
      result += "OpaqueWin32 | ";
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32Kmt )
      result += "OpaqueWin32Kmt | ";
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eD3D12Fence )
      result += "D3D12Fence | ";
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eSyncFd )
      result += "SyncFd | ";
#if defined( VK_USE_PLATFORM_SCI )
    if ( value & ExternalSemaphoreHandleTypeFlagBits::eSciSyncObjNV )
      result += "SciSyncObjNV | ";
#endif /*VK_USE_PLATFORM_SCI*/

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalSemaphoreFeatureFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ExternalSemaphoreFeatureFlagBits::eExportable )
      result += "Exportable | ";
    if ( value & ExternalSemaphoreFeatureFlagBits::eImportable )
      result += "Importable | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_VERSION_1_2 ===

  VULKAN_HPP_INLINE std::string to_string( DescriptorBindingFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DescriptorBindingFlagBits::eUpdateAfterBind )
      result += "UpdateAfterBind | ";
    if ( value & DescriptorBindingFlagBits::eUpdateUnusedWhilePending )
      result += "UpdateUnusedWhilePending | ";
    if ( value & DescriptorBindingFlagBits::ePartiallyBound )
      result += "PartiallyBound | ";
    if ( value & DescriptorBindingFlagBits::eVariableDescriptorCount )
      result += "VariableDescriptorCount | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ResolveModeFlags value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & ResolveModeFlagBits::eSampleZero )
      result += "SampleZero | ";
    if ( value & ResolveModeFlagBits::eAverage )
      result += "Average | ";
    if ( value & ResolveModeFlagBits::eMin )
      result += "Min | ";
    if ( value & ResolveModeFlagBits::eMax )
      result += "Max | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreWaitFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SemaphoreWaitFlagBits::eAny )
      result += "Any | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_VERSION_1_3 ===

  VULKAN_HPP_INLINE std::string to_string( PipelineCreationFeedbackFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PipelineCreationFeedbackFlagBits::eValid )
      result += "Valid | ";
    if ( value & PipelineCreationFeedbackFlagBits::eApplicationPipelineCacheHit )
      result += "ApplicationPipelineCacheHit | ";
    if ( value & PipelineCreationFeedbackFlagBits::eBasePipelineAcceleration )
      result += "BasePipelineAcceleration | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( ToolPurposeFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & ToolPurposeFlagBits::eValidation )
      result += "Validation | ";
    if ( value & ToolPurposeFlagBits::eProfiling )
      result += "Profiling | ";
    if ( value & ToolPurposeFlagBits::eTracing )
      result += "Tracing | ";
    if ( value & ToolPurposeFlagBits::eAdditionalFeatures )
      result += "AdditionalFeatures | ";
    if ( value & ToolPurposeFlagBits::eModifyingFeatures )
      result += "ModifyingFeatures | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( PrivateDataSlotCreateFlags )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineStageFlags2 value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & PipelineStageFlagBits2::eTopOfPipe )
      result += "TopOfPipe | ";
    if ( value & PipelineStageFlagBits2::eDrawIndirect )
      result += "DrawIndirect | ";
    if ( value & PipelineStageFlagBits2::eVertexInput )
      result += "VertexInput | ";
    if ( value & PipelineStageFlagBits2::eVertexShader )
      result += "VertexShader | ";
    if ( value & PipelineStageFlagBits2::eTessellationControlShader )
      result += "TessellationControlShader | ";
    if ( value & PipelineStageFlagBits2::eTessellationEvaluationShader )
      result += "TessellationEvaluationShader | ";
    if ( value & PipelineStageFlagBits2::eGeometryShader )
      result += "GeometryShader | ";
    if ( value & PipelineStageFlagBits2::eFragmentShader )
      result += "FragmentShader | ";
    if ( value & PipelineStageFlagBits2::eEarlyFragmentTests )
      result += "EarlyFragmentTests | ";
    if ( value & PipelineStageFlagBits2::eLateFragmentTests )
      result += "LateFragmentTests | ";
    if ( value & PipelineStageFlagBits2::eColorAttachmentOutput )
      result += "ColorAttachmentOutput | ";
    if ( value & PipelineStageFlagBits2::eComputeShader )
      result += "ComputeShader | ";
    if ( value & PipelineStageFlagBits2::eAllTransfer )
      result += "AllTransfer | ";
    if ( value & PipelineStageFlagBits2::eBottomOfPipe )
      result += "BottomOfPipe | ";
    if ( value & PipelineStageFlagBits2::eHost )
      result += "Host | ";
    if ( value & PipelineStageFlagBits2::eAllGraphics )
      result += "AllGraphics | ";
    if ( value & PipelineStageFlagBits2::eAllCommands )
      result += "AllCommands | ";
    if ( value & PipelineStageFlagBits2::eCopy )
      result += "Copy | ";
    if ( value & PipelineStageFlagBits2::eResolve )
      result += "Resolve | ";
    if ( value & PipelineStageFlagBits2::eBlit )
      result += "Blit | ";
    if ( value & PipelineStageFlagBits2::eClear )
      result += "Clear | ";
    if ( value & PipelineStageFlagBits2::eIndexInput )
      result += "IndexInput | ";
    if ( value & PipelineStageFlagBits2::eVertexAttributeInput )
      result += "VertexAttributeInput | ";
    if ( value & PipelineStageFlagBits2::ePreRasterizationShaders )
      result += "PreRasterizationShaders | ";
    if ( value & PipelineStageFlagBits2::eTransformFeedbackEXT )
      result += "TransformFeedbackEXT | ";
    if ( value & PipelineStageFlagBits2::eConditionalRenderingEXT )
      result += "ConditionalRenderingEXT | ";
    if ( value & PipelineStageFlagBits2::eCommandPreprocessNV )
      result += "CommandPreprocessNV | ";
    if ( value & PipelineStageFlagBits2::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";
    if ( value & PipelineStageFlagBits2::eAccelerationStructureBuildKHR )
      result += "AccelerationStructureBuildKHR | ";
    if ( value & PipelineStageFlagBits2::eRayTracingShaderKHR )
      result += "RayTracingShaderKHR | ";
    if ( value & PipelineStageFlagBits2::eFragmentDensityProcessEXT )
      result += "FragmentDensityProcessEXT | ";
    if ( value & PipelineStageFlagBits2::eTaskShaderEXT )
      result += "TaskShaderEXT | ";
    if ( value & PipelineStageFlagBits2::eMeshShaderEXT )
      result += "MeshShaderEXT | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( AccessFlags2 value )
  {
    if ( !value )
      return "None";

    std::string result;
    if ( value & AccessFlagBits2::eIndirectCommandRead )
      result += "IndirectCommandRead | ";
    if ( value & AccessFlagBits2::eIndexRead )
      result += "IndexRead | ";
    if ( value & AccessFlagBits2::eVertexAttributeRead )
      result += "VertexAttributeRead | ";
    if ( value & AccessFlagBits2::eUniformRead )
      result += "UniformRead | ";
    if ( value & AccessFlagBits2::eInputAttachmentRead )
      result += "InputAttachmentRead | ";
    if ( value & AccessFlagBits2::eShaderRead )
      result += "ShaderRead | ";
    if ( value & AccessFlagBits2::eShaderWrite )
      result += "ShaderWrite | ";
    if ( value & AccessFlagBits2::eColorAttachmentRead )
      result += "ColorAttachmentRead | ";
    if ( value & AccessFlagBits2::eColorAttachmentWrite )
      result += "ColorAttachmentWrite | ";
    if ( value & AccessFlagBits2::eDepthStencilAttachmentRead )
      result += "DepthStencilAttachmentRead | ";
    if ( value & AccessFlagBits2::eDepthStencilAttachmentWrite )
      result += "DepthStencilAttachmentWrite | ";
    if ( value & AccessFlagBits2::eTransferRead )
      result += "TransferRead | ";
    if ( value & AccessFlagBits2::eTransferWrite )
      result += "TransferWrite | ";
    if ( value & AccessFlagBits2::eHostRead )
      result += "HostRead | ";
    if ( value & AccessFlagBits2::eHostWrite )
      result += "HostWrite | ";
    if ( value & AccessFlagBits2::eMemoryRead )
      result += "MemoryRead | ";
    if ( value & AccessFlagBits2::eMemoryWrite )
      result += "MemoryWrite | ";
    if ( value & AccessFlagBits2::eShaderSampledRead )
      result += "ShaderSampledRead | ";
    if ( value & AccessFlagBits2::eShaderStorageRead )
      result += "ShaderStorageRead | ";
    if ( value & AccessFlagBits2::eShaderStorageWrite )
      result += "ShaderStorageWrite | ";
    if ( value & AccessFlagBits2::eTransformFeedbackWriteEXT )
      result += "TransformFeedbackWriteEXT | ";
    if ( value & AccessFlagBits2::eTransformFeedbackCounterReadEXT )
      result += "TransformFeedbackCounterReadEXT | ";
    if ( value & AccessFlagBits2::eTransformFeedbackCounterWriteEXT )
      result += "TransformFeedbackCounterWriteEXT | ";
    if ( value & AccessFlagBits2::eConditionalRenderingReadEXT )
      result += "ConditionalRenderingReadEXT | ";
    if ( value & AccessFlagBits2::eCommandPreprocessReadNV )
      result += "CommandPreprocessReadNV | ";
    if ( value & AccessFlagBits2::eCommandPreprocessWriteNV )
      result += "CommandPreprocessWriteNV | ";
    if ( value & AccessFlagBits2::eFragmentShadingRateAttachmentReadKHR )
      result += "FragmentShadingRateAttachmentReadKHR | ";
    if ( value & AccessFlagBits2::eAccelerationStructureReadKHR )
      result += "AccelerationStructureReadKHR | ";
    if ( value & AccessFlagBits2::eAccelerationStructureWriteKHR )
      result += "AccelerationStructureWriteKHR | ";
    if ( value & AccessFlagBits2::eFragmentDensityMapReadEXT )
      result += "FragmentDensityMapReadEXT | ";
    if ( value & AccessFlagBits2::eColorAttachmentReadNoncoherentEXT )
      result += "ColorAttachmentReadNoncoherentEXT | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( SubmitFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SubmitFlagBits::eProtected )
      result += "Protected | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( RenderingFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & RenderingFlagBits::eContentsSecondaryCommandBuffers )
      result += "ContentsSecondaryCommandBuffers | ";
    if ( value & RenderingFlagBits::eSuspending )
      result += "Suspending | ";
    if ( value & RenderingFlagBits::eResuming )
      result += "Resuming | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( FormatFeatureFlags2 value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & FormatFeatureFlagBits2::eSampledImage )
      result += "SampledImage | ";
    if ( value & FormatFeatureFlagBits2::eStorageImage )
      result += "StorageImage | ";
    if ( value & FormatFeatureFlagBits2::eStorageImageAtomic )
      result += "StorageImageAtomic | ";
    if ( value & FormatFeatureFlagBits2::eUniformTexelBuffer )
      result += "UniformTexelBuffer | ";
    if ( value & FormatFeatureFlagBits2::eStorageTexelBuffer )
      result += "StorageTexelBuffer | ";
    if ( value & FormatFeatureFlagBits2::eStorageTexelBufferAtomic )
      result += "StorageTexelBufferAtomic | ";
    if ( value & FormatFeatureFlagBits2::eVertexBuffer )
      result += "VertexBuffer | ";
    if ( value & FormatFeatureFlagBits2::eColorAttachment )
      result += "ColorAttachment | ";
    if ( value & FormatFeatureFlagBits2::eColorAttachmentBlend )
      result += "ColorAttachmentBlend | ";
    if ( value & FormatFeatureFlagBits2::eDepthStencilAttachment )
      result += "DepthStencilAttachment | ";
    if ( value & FormatFeatureFlagBits2::eBlitSrc )
      result += "BlitSrc | ";
    if ( value & FormatFeatureFlagBits2::eBlitDst )
      result += "BlitDst | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageFilterLinear )
      result += "SampledImageFilterLinear | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageFilterCubic )
      result += "SampledImageFilterCubic | ";
    if ( value & FormatFeatureFlagBits2::eTransferSrc )
      result += "TransferSrc | ";
    if ( value & FormatFeatureFlagBits2::eTransferDst )
      result += "TransferDst | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageFilterMinmax )
      result += "SampledImageFilterMinmax | ";
    if ( value & FormatFeatureFlagBits2::eMidpointChromaSamples )
      result += "MidpointChromaSamples | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageYcbcrConversionLinearFilter )
      result += "SampledImageYcbcrConversionLinearFilter | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageYcbcrConversionSeparateReconstructionFilter )
      result += "SampledImageYcbcrConversionSeparateReconstructionFilter | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicit )
      result += "SampledImageYcbcrConversionChromaReconstructionExplicit | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable )
      result += "SampledImageYcbcrConversionChromaReconstructionExplicitForceable | ";
    if ( value & FormatFeatureFlagBits2::eDisjoint )
      result += "Disjoint | ";
    if ( value & FormatFeatureFlagBits2::eCositedChromaSamples )
      result += "CositedChromaSamples | ";
    if ( value & FormatFeatureFlagBits2::eStorageReadWithoutFormat )
      result += "StorageReadWithoutFormat | ";
    if ( value & FormatFeatureFlagBits2::eStorageWriteWithoutFormat )
      result += "StorageWriteWithoutFormat | ";
    if ( value & FormatFeatureFlagBits2::eSampledImageDepthComparison )
      result += "SampledImageDepthComparison | ";
    if ( value & FormatFeatureFlagBits2::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_KHR_surface ===

  VULKAN_HPP_INLINE std::string to_string( CompositeAlphaFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & CompositeAlphaFlagBitsKHR::eOpaque )
      result += "Opaque | ";
    if ( value & CompositeAlphaFlagBitsKHR::ePreMultiplied )
      result += "PreMultiplied | ";
    if ( value & CompositeAlphaFlagBitsKHR::ePostMultiplied )
      result += "PostMultiplied | ";
    if ( value & CompositeAlphaFlagBitsKHR::eInherit )
      result += "Inherit | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_KHR_swapchain ===

  VULKAN_HPP_INLINE std::string to_string( SwapchainCreateFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SwapchainCreateFlagBitsKHR::eSplitInstanceBindRegions )
      result += "SplitInstanceBindRegions | ";
    if ( value & SwapchainCreateFlagBitsKHR::eProtected )
      result += "Protected | ";
    if ( value & SwapchainCreateFlagBitsKHR::eMutableFormat )
      result += "MutableFormat | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceGroupPresentModeFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DeviceGroupPresentModeFlagBitsKHR::eLocal )
      result += "Local | ";
    if ( value & DeviceGroupPresentModeFlagBitsKHR::eRemote )
      result += "Remote | ";
    if ( value & DeviceGroupPresentModeFlagBitsKHR::eSum )
      result += "Sum | ";
    if ( value & DeviceGroupPresentModeFlagBitsKHR::eLocalMultiDevice )
      result += "LocalMultiDevice | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_KHR_display ===

  VULKAN_HPP_INLINE std::string to_string( DisplayModeCreateFlagsKHR )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( DisplayPlaneAlphaFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DisplayPlaneAlphaFlagBitsKHR::eOpaque )
      result += "Opaque | ";
    if ( value & DisplayPlaneAlphaFlagBitsKHR::eGlobal )
      result += "Global | ";
    if ( value & DisplayPlaneAlphaFlagBitsKHR::ePerPixel )
      result += "PerPixel | ";
    if ( value & DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied )
      result += "PerPixelPremultiplied | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DisplaySurfaceCreateFlagsKHR )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( SurfaceTransformFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SurfaceTransformFlagBitsKHR::eIdentity )
      result += "Identity | ";
    if ( value & SurfaceTransformFlagBitsKHR::eRotate90 )
      result += "Rotate90 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eRotate180 )
      result += "Rotate180 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eRotate270 )
      result += "Rotate270 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eHorizontalMirror )
      result += "HorizontalMirror | ";
    if ( value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90 )
      result += "HorizontalMirrorRotate90 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180 )
      result += "HorizontalMirrorRotate180 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270 )
      result += "HorizontalMirrorRotate270 | ";
    if ( value & SurfaceTransformFlagBitsKHR::eInherit )
      result += "Inherit | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_EXT_display_surface_counter ===

  VULKAN_HPP_INLINE std::string to_string( SurfaceCounterFlagsEXT value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & SurfaceCounterFlagBitsEXT::eVblank )
      result += "Vblank | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  //=== VK_EXT_discard_rectangles ===

  VULKAN_HPP_INLINE std::string to_string( PipelineDiscardRectangleStateCreateFlagsEXT )
  {
    return "{}";
  }

  //=== VK_EXT_conservative_rasterization ===

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationConservativeStateCreateFlagsEXT )
  {
    return "{}";
  }

  //=== VK_EXT_depth_clip_enable ===

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationDepthClipStateCreateFlagsEXT )
  {
    return "{}";
  }

  //=== VK_KHR_performance_query ===

  VULKAN_HPP_INLINE std::string to_string( PerformanceCounterDescriptionFlagsKHR value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & PerformanceCounterDescriptionFlagBitsKHR::ePerformanceImpacting )
      result += "PerformanceImpacting | ";
    if ( value & PerformanceCounterDescriptionFlagBitsKHR::eConcurrentlyImpacted )
      result += "ConcurrentlyImpacted | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( AcquireProfilingLockFlagsKHR )
  {
    return "{}";
  }

  //=== VK_EXT_debug_utils ===

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessageSeverityFlagsEXT value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DebugUtilsMessageSeverityFlagBitsEXT::eVerbose )
      result += "Verbose | ";
    if ( value & DebugUtilsMessageSeverityFlagBitsEXT::eInfo )
      result += "Info | ";
    if ( value & DebugUtilsMessageSeverityFlagBitsEXT::eWarning )
      result += "Warning | ";
    if ( value & DebugUtilsMessageSeverityFlagBitsEXT::eError )
      result += "Error | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessageTypeFlagsEXT value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & DebugUtilsMessageTypeFlagBitsEXT::eGeneral )
      result += "General | ";
    if ( value & DebugUtilsMessageTypeFlagBitsEXT::eValidation )
      result += "Validation | ";
    if ( value & DebugUtilsMessageTypeFlagBitsEXT::ePerformance )
      result += "Performance | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessengerCallbackDataFlagsEXT )
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessengerCreateFlagsEXT )
  {
    return "{}";
  }

  //=== VK_EXT_headless_surface ===

  VULKAN_HPP_INLINE std::string to_string( HeadlessSurfaceCreateFlagsEXT )
  {
    return "{}";
  }

  //=== VK_KHR_object_refresh ===

  VULKAN_HPP_INLINE std::string to_string( RefreshObjectFlagsKHR )
  {
    return "{}";
  }

  //=======================
  //=== ENUMs to_string ===
  //=======================

  VULKAN_HPP_INLINE std::string toHexString( uint32_t value )
  {
#if __cpp_lib_format
    return std::format( "{:x}", value );
#else
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
#endif
  }

  //=== VK_VERSION_1_0 ===

  VULKAN_HPP_INLINE std::string to_string( Result value )
  {
    switch ( value )
    {
      case Result::eSuccess: return "Success";
      case Result::eNotReady: return "NotReady";
      case Result::eTimeout: return "Timeout";
      case Result::eEventSet: return "EventSet";
      case Result::eEventReset: return "EventReset";
      case Result::eIncomplete: return "Incomplete";
      case Result::eErrorOutOfHostMemory: return "ErrorOutOfHostMemory";
      case Result::eErrorOutOfDeviceMemory: return "ErrorOutOfDeviceMemory";
      case Result::eErrorInitializationFailed: return "ErrorInitializationFailed";
      case Result::eErrorDeviceLost: return "ErrorDeviceLost";
      case Result::eErrorMemoryMapFailed: return "ErrorMemoryMapFailed";
      case Result::eErrorLayerNotPresent: return "ErrorLayerNotPresent";
      case Result::eErrorExtensionNotPresent: return "ErrorExtensionNotPresent";
      case Result::eErrorFeatureNotPresent: return "ErrorFeatureNotPresent";
      case Result::eErrorIncompatibleDriver: return "ErrorIncompatibleDriver";
      case Result::eErrorTooManyObjects: return "ErrorTooManyObjects";
      case Result::eErrorFormatNotSupported: return "ErrorFormatNotSupported";
      case Result::eErrorFragmentedPool: return "ErrorFragmentedPool";
      case Result::eErrorUnknown: return "ErrorUnknown";
      case Result::eErrorOutOfPoolMemory: return "ErrorOutOfPoolMemory";
      case Result::eErrorInvalidExternalHandle: return "ErrorInvalidExternalHandle";
      case Result::eErrorFragmentation: return "ErrorFragmentation";
      case Result::eErrorInvalidOpaqueCaptureAddress: return "ErrorInvalidOpaqueCaptureAddress";
      case Result::ePipelineCompileRequired: return "PipelineCompileRequired";
      case Result::eErrorValidationFailed: return "ErrorValidationFailed";
      case Result::eErrorInvalidPipelineCacheData: return "ErrorInvalidPipelineCacheData";
      case Result::eErrorNoPipelineMatch: return "ErrorNoPipelineMatch";
      case Result::eErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
      case Result::eErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
      case Result::eSuboptimalKHR: return "SuboptimalKHR";
      case Result::eErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
      case Result::eErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
      case Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT: return "ErrorInvalidDrmFormatModifierPlaneLayoutEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( StructureType value )
  {
    switch ( value )
    {
      case StructureType::eApplicationInfo: return "ApplicationInfo";
      case StructureType::eInstanceCreateInfo: return "InstanceCreateInfo";
      case StructureType::eDeviceQueueCreateInfo: return "DeviceQueueCreateInfo";
      case StructureType::eDeviceCreateInfo: return "DeviceCreateInfo";
      case StructureType::eSubmitInfo: return "SubmitInfo";
      case StructureType::eMemoryAllocateInfo: return "MemoryAllocateInfo";
      case StructureType::eMappedMemoryRange: return "MappedMemoryRange";
      case StructureType::eFenceCreateInfo: return "FenceCreateInfo";
      case StructureType::eSemaphoreCreateInfo: return "SemaphoreCreateInfo";
      case StructureType::eEventCreateInfo: return "EventCreateInfo";
      case StructureType::eQueryPoolCreateInfo: return "QueryPoolCreateInfo";
      case StructureType::eBufferCreateInfo: return "BufferCreateInfo";
      case StructureType::eBufferViewCreateInfo: return "BufferViewCreateInfo";
      case StructureType::eImageCreateInfo: return "ImageCreateInfo";
      case StructureType::eImageViewCreateInfo: return "ImageViewCreateInfo";
      case StructureType::ePipelineCacheCreateInfo: return "PipelineCacheCreateInfo";
      case StructureType::ePipelineShaderStageCreateInfo: return "PipelineShaderStageCreateInfo";
      case StructureType::ePipelineVertexInputStateCreateInfo: return "PipelineVertexInputStateCreateInfo";
      case StructureType::ePipelineInputAssemblyStateCreateInfo: return "PipelineInputAssemblyStateCreateInfo";
      case StructureType::ePipelineTessellationStateCreateInfo: return "PipelineTessellationStateCreateInfo";
      case StructureType::ePipelineViewportStateCreateInfo: return "PipelineViewportStateCreateInfo";
      case StructureType::ePipelineRasterizationStateCreateInfo: return "PipelineRasterizationStateCreateInfo";
      case StructureType::ePipelineMultisampleStateCreateInfo: return "PipelineMultisampleStateCreateInfo";
      case StructureType::ePipelineDepthStencilStateCreateInfo: return "PipelineDepthStencilStateCreateInfo";
      case StructureType::ePipelineColorBlendStateCreateInfo: return "PipelineColorBlendStateCreateInfo";
      case StructureType::ePipelineDynamicStateCreateInfo: return "PipelineDynamicStateCreateInfo";
      case StructureType::eGraphicsPipelineCreateInfo: return "GraphicsPipelineCreateInfo";
      case StructureType::eComputePipelineCreateInfo: return "ComputePipelineCreateInfo";
      case StructureType::ePipelineLayoutCreateInfo: return "PipelineLayoutCreateInfo";
      case StructureType::eSamplerCreateInfo: return "SamplerCreateInfo";
      case StructureType::eDescriptorSetLayoutCreateInfo: return "DescriptorSetLayoutCreateInfo";
      case StructureType::eDescriptorPoolCreateInfo: return "DescriptorPoolCreateInfo";
      case StructureType::eDescriptorSetAllocateInfo: return "DescriptorSetAllocateInfo";
      case StructureType::eWriteDescriptorSet: return "WriteDescriptorSet";
      case StructureType::eCopyDescriptorSet: return "CopyDescriptorSet";
      case StructureType::eFramebufferCreateInfo: return "FramebufferCreateInfo";
      case StructureType::eRenderPassCreateInfo: return "RenderPassCreateInfo";
      case StructureType::eCommandPoolCreateInfo: return "CommandPoolCreateInfo";
      case StructureType::eCommandBufferAllocateInfo: return "CommandBufferAllocateInfo";
      case StructureType::eCommandBufferInheritanceInfo: return "CommandBufferInheritanceInfo";
      case StructureType::eCommandBufferBeginInfo: return "CommandBufferBeginInfo";
      case StructureType::eRenderPassBeginInfo: return "RenderPassBeginInfo";
      case StructureType::eBufferMemoryBarrier: return "BufferMemoryBarrier";
      case StructureType::eImageMemoryBarrier: return "ImageMemoryBarrier";
      case StructureType::eMemoryBarrier: return "MemoryBarrier";
      case StructureType::eLoaderInstanceCreateInfo: return "LoaderInstanceCreateInfo";
      case StructureType::eLoaderDeviceCreateInfo: return "LoaderDeviceCreateInfo";
      case StructureType::ePhysicalDeviceSubgroupProperties: return "PhysicalDeviceSubgroupProperties";
      case StructureType::eBindBufferMemoryInfo: return "BindBufferMemoryInfo";
      case StructureType::eBindImageMemoryInfo: return "BindImageMemoryInfo";
      case StructureType::ePhysicalDevice16BitStorageFeatures: return "PhysicalDevice16BitStorageFeatures";
      case StructureType::eMemoryDedicatedRequirements: return "MemoryDedicatedRequirements";
      case StructureType::eMemoryDedicatedAllocateInfo: return "MemoryDedicatedAllocateInfo";
      case StructureType::eMemoryAllocateFlagsInfo: return "MemoryAllocateFlagsInfo";
      case StructureType::eDeviceGroupRenderPassBeginInfo: return "DeviceGroupRenderPassBeginInfo";
      case StructureType::eDeviceGroupCommandBufferBeginInfo: return "DeviceGroupCommandBufferBeginInfo";
      case StructureType::eDeviceGroupSubmitInfo: return "DeviceGroupSubmitInfo";
      case StructureType::eBindBufferMemoryDeviceGroupInfo: return "BindBufferMemoryDeviceGroupInfo";
      case StructureType::eBindImageMemoryDeviceGroupInfo: return "BindImageMemoryDeviceGroupInfo";
      case StructureType::ePhysicalDeviceGroupProperties: return "PhysicalDeviceGroupProperties";
      case StructureType::eDeviceGroupDeviceCreateInfo: return "DeviceGroupDeviceCreateInfo";
      case StructureType::eBufferMemoryRequirementsInfo2: return "BufferMemoryRequirementsInfo2";
      case StructureType::eImageMemoryRequirementsInfo2: return "ImageMemoryRequirementsInfo2";
      case StructureType::eMemoryRequirements2: return "MemoryRequirements2";
      case StructureType::ePhysicalDeviceFeatures2: return "PhysicalDeviceFeatures2";
      case StructureType::ePhysicalDeviceProperties2: return "PhysicalDeviceProperties2";
      case StructureType::eFormatProperties2: return "FormatProperties2";
      case StructureType::eImageFormatProperties2: return "ImageFormatProperties2";
      case StructureType::ePhysicalDeviceImageFormatInfo2: return "PhysicalDeviceImageFormatInfo2";
      case StructureType::eQueueFamilyProperties2: return "QueueFamilyProperties2";
      case StructureType::ePhysicalDeviceMemoryProperties2: return "PhysicalDeviceMemoryProperties2";
      case StructureType::ePhysicalDevicePointClippingProperties: return "PhysicalDevicePointClippingProperties";
      case StructureType::eRenderPassInputAttachmentAspectCreateInfo: return "RenderPassInputAttachmentAspectCreateInfo";
      case StructureType::eImageViewUsageCreateInfo: return "ImageViewUsageCreateInfo";
      case StructureType::ePipelineTessellationDomainOriginStateCreateInfo: return "PipelineTessellationDomainOriginStateCreateInfo";
      case StructureType::eRenderPassMultiviewCreateInfo: return "RenderPassMultiviewCreateInfo";
      case StructureType::ePhysicalDeviceMultiviewFeatures: return "PhysicalDeviceMultiviewFeatures";
      case StructureType::ePhysicalDeviceMultiviewProperties: return "PhysicalDeviceMultiviewProperties";
      case StructureType::ePhysicalDeviceVariablePointersFeatures: return "PhysicalDeviceVariablePointersFeatures";
      case StructureType::eProtectedSubmitInfo: return "ProtectedSubmitInfo";
      case StructureType::ePhysicalDeviceProtectedMemoryFeatures: return "PhysicalDeviceProtectedMemoryFeatures";
      case StructureType::ePhysicalDeviceProtectedMemoryProperties: return "PhysicalDeviceProtectedMemoryProperties";
      case StructureType::eDeviceQueueInfo2: return "DeviceQueueInfo2";
      case StructureType::eSamplerYcbcrConversionCreateInfo: return "SamplerYcbcrConversionCreateInfo";
      case StructureType::eSamplerYcbcrConversionInfo: return "SamplerYcbcrConversionInfo";
      case StructureType::eBindImagePlaneMemoryInfo: return "BindImagePlaneMemoryInfo";
      case StructureType::eImagePlaneMemoryRequirementsInfo: return "ImagePlaneMemoryRequirementsInfo";
      case StructureType::ePhysicalDeviceSamplerYcbcrConversionFeatures: return "PhysicalDeviceSamplerYcbcrConversionFeatures";
      case StructureType::eSamplerYcbcrConversionImageFormatProperties: return "SamplerYcbcrConversionImageFormatProperties";
      case StructureType::ePhysicalDeviceExternalImageFormatInfo: return "PhysicalDeviceExternalImageFormatInfo";
      case StructureType::eExternalImageFormatProperties: return "ExternalImageFormatProperties";
      case StructureType::ePhysicalDeviceExternalBufferInfo: return "PhysicalDeviceExternalBufferInfo";
      case StructureType::eExternalBufferProperties: return "ExternalBufferProperties";
      case StructureType::ePhysicalDeviceIdProperties: return "PhysicalDeviceIdProperties";
      case StructureType::eExternalMemoryBufferCreateInfo: return "ExternalMemoryBufferCreateInfo";
      case StructureType::eExternalMemoryImageCreateInfo: return "ExternalMemoryImageCreateInfo";
      case StructureType::eExportMemoryAllocateInfo: return "ExportMemoryAllocateInfo";
      case StructureType::ePhysicalDeviceExternalFenceInfo: return "PhysicalDeviceExternalFenceInfo";
      case StructureType::eExternalFenceProperties: return "ExternalFenceProperties";
      case StructureType::eExportFenceCreateInfo: return "ExportFenceCreateInfo";
      case StructureType::eExportSemaphoreCreateInfo: return "ExportSemaphoreCreateInfo";
      case StructureType::ePhysicalDeviceExternalSemaphoreInfo: return "PhysicalDeviceExternalSemaphoreInfo";
      case StructureType::eExternalSemaphoreProperties: return "ExternalSemaphoreProperties";
      case StructureType::ePhysicalDeviceMaintenance3Properties: return "PhysicalDeviceMaintenance3Properties";
      case StructureType::eDescriptorSetLayoutSupport: return "DescriptorSetLayoutSupport";
      case StructureType::ePhysicalDeviceShaderDrawParametersFeatures: return "PhysicalDeviceShaderDrawParametersFeatures";
      case StructureType::ePhysicalDeviceVulkan11Features: return "PhysicalDeviceVulkan11Features";
      case StructureType::ePhysicalDeviceVulkan11Properties: return "PhysicalDeviceVulkan11Properties";
      case StructureType::ePhysicalDeviceVulkan12Features: return "PhysicalDeviceVulkan12Features";
      case StructureType::ePhysicalDeviceVulkan12Properties: return "PhysicalDeviceVulkan12Properties";
      case StructureType::eImageFormatListCreateInfo: return "ImageFormatListCreateInfo";
      case StructureType::eAttachmentDescription2: return "AttachmentDescription2";
      case StructureType::eAttachmentReference2: return "AttachmentReference2";
      case StructureType::eSubpassDescription2: return "SubpassDescription2";
      case StructureType::eSubpassDependency2: return "SubpassDependency2";
      case StructureType::eRenderPassCreateInfo2: return "RenderPassCreateInfo2";
      case StructureType::eSubpassBeginInfo: return "SubpassBeginInfo";
      case StructureType::eSubpassEndInfo: return "SubpassEndInfo";
      case StructureType::ePhysicalDevice8BitStorageFeatures: return "PhysicalDevice8BitStorageFeatures";
      case StructureType::ePhysicalDeviceDriverProperties: return "PhysicalDeviceDriverProperties";
      case StructureType::ePhysicalDeviceShaderAtomicInt64Features: return "PhysicalDeviceShaderAtomicInt64Features";
      case StructureType::ePhysicalDeviceShaderFloat16Int8Features: return "PhysicalDeviceShaderFloat16Int8Features";
      case StructureType::ePhysicalDeviceFloatControlsProperties: return "PhysicalDeviceFloatControlsProperties";
      case StructureType::eDescriptorSetLayoutBindingFlagsCreateInfo: return "DescriptorSetLayoutBindingFlagsCreateInfo";
      case StructureType::ePhysicalDeviceDescriptorIndexingFeatures: return "PhysicalDeviceDescriptorIndexingFeatures";
      case StructureType::ePhysicalDeviceDescriptorIndexingProperties: return "PhysicalDeviceDescriptorIndexingProperties";
      case StructureType::eDescriptorSetVariableDescriptorCountAllocateInfo: return "DescriptorSetVariableDescriptorCountAllocateInfo";
      case StructureType::eDescriptorSetVariableDescriptorCountLayoutSupport: return "DescriptorSetVariableDescriptorCountLayoutSupport";
      case StructureType::ePhysicalDeviceDepthStencilResolveProperties: return "PhysicalDeviceDepthStencilResolveProperties";
      case StructureType::eSubpassDescriptionDepthStencilResolve: return "SubpassDescriptionDepthStencilResolve";
      case StructureType::ePhysicalDeviceScalarBlockLayoutFeatures: return "PhysicalDeviceScalarBlockLayoutFeatures";
      case StructureType::eImageStencilUsageCreateInfo: return "ImageStencilUsageCreateInfo";
      case StructureType::ePhysicalDeviceSamplerFilterMinmaxProperties: return "PhysicalDeviceSamplerFilterMinmaxProperties";
      case StructureType::eSamplerReductionModeCreateInfo: return "SamplerReductionModeCreateInfo";
      case StructureType::ePhysicalDeviceVulkanMemoryModelFeatures: return "PhysicalDeviceVulkanMemoryModelFeatures";
      case StructureType::ePhysicalDeviceImagelessFramebufferFeatures: return "PhysicalDeviceImagelessFramebufferFeatures";
      case StructureType::eFramebufferAttachmentsCreateInfo: return "FramebufferAttachmentsCreateInfo";
      case StructureType::eFramebufferAttachmentImageInfo: return "FramebufferAttachmentImageInfo";
      case StructureType::eRenderPassAttachmentBeginInfo: return "RenderPassAttachmentBeginInfo";
      case StructureType::ePhysicalDeviceUniformBufferStandardLayoutFeatures: return "PhysicalDeviceUniformBufferStandardLayoutFeatures";
      case StructureType::ePhysicalDeviceShaderSubgroupExtendedTypesFeatures: return "PhysicalDeviceShaderSubgroupExtendedTypesFeatures";
      case StructureType::ePhysicalDeviceSeparateDepthStencilLayoutsFeatures: return "PhysicalDeviceSeparateDepthStencilLayoutsFeatures";
      case StructureType::eAttachmentReferenceStencilLayout: return "AttachmentReferenceStencilLayout";
      case StructureType::eAttachmentDescriptionStencilLayout: return "AttachmentDescriptionStencilLayout";
      case StructureType::ePhysicalDeviceHostQueryResetFeatures: return "PhysicalDeviceHostQueryResetFeatures";
      case StructureType::ePhysicalDeviceTimelineSemaphoreFeatures: return "PhysicalDeviceTimelineSemaphoreFeatures";
      case StructureType::ePhysicalDeviceTimelineSemaphoreProperties: return "PhysicalDeviceTimelineSemaphoreProperties";
      case StructureType::eSemaphoreTypeCreateInfo: return "SemaphoreTypeCreateInfo";
      case StructureType::eTimelineSemaphoreSubmitInfo: return "TimelineSemaphoreSubmitInfo";
      case StructureType::eSemaphoreWaitInfo: return "SemaphoreWaitInfo";
      case StructureType::eSemaphoreSignalInfo: return "SemaphoreSignalInfo";
      case StructureType::ePhysicalDeviceBufferDeviceAddressFeatures: return "PhysicalDeviceBufferDeviceAddressFeatures";
      case StructureType::eBufferDeviceAddressInfo: return "BufferDeviceAddressInfo";
      case StructureType::eBufferOpaqueCaptureAddressCreateInfo: return "BufferOpaqueCaptureAddressCreateInfo";
      case StructureType::eMemoryOpaqueCaptureAddressAllocateInfo: return "MemoryOpaqueCaptureAddressAllocateInfo";
      case StructureType::eDeviceMemoryOpaqueCaptureAddressInfo: return "DeviceMemoryOpaqueCaptureAddressInfo";
      case StructureType::ePhysicalDeviceVulkan13Features: return "PhysicalDeviceVulkan13Features";
      case StructureType::ePhysicalDeviceVulkan13Properties: return "PhysicalDeviceVulkan13Properties";
      case StructureType::ePipelineCreationFeedbackCreateInfo: return "PipelineCreationFeedbackCreateInfo";
      case StructureType::ePhysicalDeviceShaderTerminateInvocationFeatures: return "PhysicalDeviceShaderTerminateInvocationFeatures";
      case StructureType::ePhysicalDeviceToolProperties: return "PhysicalDeviceToolProperties";
      case StructureType::ePhysicalDeviceShaderDemoteToHelperInvocationFeatures: return "PhysicalDeviceShaderDemoteToHelperInvocationFeatures";
      case StructureType::ePhysicalDevicePrivateDataFeatures: return "PhysicalDevicePrivateDataFeatures";
      case StructureType::eDevicePrivateDataCreateInfo: return "DevicePrivateDataCreateInfo";
      case StructureType::ePrivateDataSlotCreateInfo: return "PrivateDataSlotCreateInfo";
      case StructureType::ePhysicalDevicePipelineCreationCacheControlFeatures: return "PhysicalDevicePipelineCreationCacheControlFeatures";
      case StructureType::eMemoryBarrier2: return "MemoryBarrier2";
      case StructureType::eBufferMemoryBarrier2: return "BufferMemoryBarrier2";
      case StructureType::eImageMemoryBarrier2: return "ImageMemoryBarrier2";
      case StructureType::eDependencyInfo: return "DependencyInfo";
      case StructureType::eSubmitInfo2: return "SubmitInfo2";
      case StructureType::eSemaphoreSubmitInfo: return "SemaphoreSubmitInfo";
      case StructureType::eCommandBufferSubmitInfo: return "CommandBufferSubmitInfo";
      case StructureType::ePhysicalDeviceSynchronization2Features: return "PhysicalDeviceSynchronization2Features";
      case StructureType::ePhysicalDeviceZeroInitializeWorkgroupMemoryFeatures: return "PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures";
      case StructureType::ePhysicalDeviceImageRobustnessFeatures: return "PhysicalDeviceImageRobustnessFeatures";
      case StructureType::eCopyBufferInfo2: return "CopyBufferInfo2";
      case StructureType::eCopyImageInfo2: return "CopyImageInfo2";
      case StructureType::eCopyBufferToImageInfo2: return "CopyBufferToImageInfo2";
      case StructureType::eCopyImageToBufferInfo2: return "CopyImageToBufferInfo2";
      case StructureType::eBlitImageInfo2: return "BlitImageInfo2";
      case StructureType::eResolveImageInfo2: return "ResolveImageInfo2";
      case StructureType::eBufferCopy2: return "BufferCopy2";
      case StructureType::eImageCopy2: return "ImageCopy2";
      case StructureType::eImageBlit2: return "ImageBlit2";
      case StructureType::eBufferImageCopy2: return "BufferImageCopy2";
      case StructureType::eImageResolve2: return "ImageResolve2";
      case StructureType::ePhysicalDeviceSubgroupSizeControlProperties: return "PhysicalDeviceSubgroupSizeControlProperties";
      case StructureType::ePipelineShaderStageRequiredSubgroupSizeCreateInfo: return "PipelineShaderStageRequiredSubgroupSizeCreateInfo";
      case StructureType::ePhysicalDeviceSubgroupSizeControlFeatures: return "PhysicalDeviceSubgroupSizeControlFeatures";
      case StructureType::ePhysicalDeviceInlineUniformBlockFeatures: return "PhysicalDeviceInlineUniformBlockFeatures";
      case StructureType::ePhysicalDeviceInlineUniformBlockProperties: return "PhysicalDeviceInlineUniformBlockProperties";
      case StructureType::eWriteDescriptorSetInlineUniformBlock: return "WriteDescriptorSetInlineUniformBlock";
      case StructureType::eDescriptorPoolInlineUniformBlockCreateInfo: return "DescriptorPoolInlineUniformBlockCreateInfo";
      case StructureType::ePhysicalDeviceTextureCompressionAstcHdrFeatures: return "PhysicalDeviceTextureCompressionAstcHdrFeatures";
      case StructureType::eRenderingInfo: return "RenderingInfo";
      case StructureType::eRenderingAttachmentInfo: return "RenderingAttachmentInfo";
      case StructureType::ePipelineRenderingCreateInfo: return "PipelineRenderingCreateInfo";
      case StructureType::ePhysicalDeviceDynamicRenderingFeatures: return "PhysicalDeviceDynamicRenderingFeatures";
      case StructureType::eCommandBufferInheritanceRenderingInfo: return "CommandBufferInheritanceRenderingInfo";
      case StructureType::ePhysicalDeviceShaderIntegerDotProductFeatures: return "PhysicalDeviceShaderIntegerDotProductFeatures";
      case StructureType::ePhysicalDeviceShaderIntegerDotProductProperties: return "PhysicalDeviceShaderIntegerDotProductProperties";
      case StructureType::ePhysicalDeviceTexelBufferAlignmentProperties: return "PhysicalDeviceTexelBufferAlignmentProperties";
      case StructureType::eFormatProperties3: return "FormatProperties3";
      case StructureType::ePhysicalDeviceMaintenance4Features: return "PhysicalDeviceMaintenance4Features";
      case StructureType::ePhysicalDeviceMaintenance4Properties: return "PhysicalDeviceMaintenance4Properties";
      case StructureType::eDeviceBufferMemoryRequirements: return "DeviceBufferMemoryRequirements";
      case StructureType::eDeviceImageMemoryRequirements: return "DeviceImageMemoryRequirements";
      case StructureType::ePhysicalDeviceVulkanSc10Features: return "PhysicalDeviceVulkanSc10Features";
      case StructureType::ePhysicalDeviceVulkanSc10Properties: return "PhysicalDeviceVulkanSc10Properties";
      case StructureType::eDeviceObjectReservationCreateInfo: return "DeviceObjectReservationCreateInfo";
      case StructureType::eCommandPoolMemoryReservationCreateInfo: return "CommandPoolMemoryReservationCreateInfo";
      case StructureType::eCommandPoolMemoryConsumption: return "CommandPoolMemoryConsumption";
      case StructureType::ePipelinePoolSize: return "PipelinePoolSize";
      case StructureType::eFaultData: return "FaultData";
      case StructureType::eFaultCallbackInfo: return "FaultCallbackInfo";
      case StructureType::ePipelineOfflineCreateInfo: return "PipelineOfflineCreateInfo";
      case StructureType::eSwapchainCreateInfoKHR: return "SwapchainCreateInfoKHR";
      case StructureType::ePresentInfoKHR: return "PresentInfoKHR";
      case StructureType::eDeviceGroupPresentCapabilitiesKHR: return "DeviceGroupPresentCapabilitiesKHR";
      case StructureType::eImageSwapchainCreateInfoKHR: return "ImageSwapchainCreateInfoKHR";
      case StructureType::eBindImageMemorySwapchainInfoKHR: return "BindImageMemorySwapchainInfoKHR";
      case StructureType::eAcquireNextImageInfoKHR: return "AcquireNextImageInfoKHR";
      case StructureType::eDeviceGroupPresentInfoKHR: return "DeviceGroupPresentInfoKHR";
      case StructureType::eDeviceGroupSwapchainCreateInfoKHR: return "DeviceGroupSwapchainCreateInfoKHR";
      case StructureType::eDisplayModeCreateInfoKHR: return "DisplayModeCreateInfoKHR";
      case StructureType::eDisplaySurfaceCreateInfoKHR: return "DisplaySurfaceCreateInfoKHR";
      case StructureType::eDisplayPresentInfoKHR: return "DisplayPresentInfoKHR";
      case StructureType::ePrivateVendorInfoPlaceholderOffset0NV: return "PrivateVendorInfoPlaceholderOffset0NV";
      case StructureType::eImageViewAstcDecodeModeEXT: return "ImageViewAstcDecodeModeEXT";
      case StructureType::ePhysicalDeviceAstcDecodeFeaturesEXT: return "PhysicalDeviceAstcDecodeFeaturesEXT";
      case StructureType::eImportMemoryFdInfoKHR: return "ImportMemoryFdInfoKHR";
      case StructureType::eMemoryFdPropertiesKHR: return "MemoryFdPropertiesKHR";
      case StructureType::eMemoryGetFdInfoKHR: return "MemoryGetFdInfoKHR";
      case StructureType::eImportSemaphoreFdInfoKHR: return "ImportSemaphoreFdInfoKHR";
      case StructureType::eSemaphoreGetFdInfoKHR: return "SemaphoreGetFdInfoKHR";
      case StructureType::ePresentRegionsKHR: return "PresentRegionsKHR";
      case StructureType::eSurfaceCapabilities2EXT: return "SurfaceCapabilities2EXT";
      case StructureType::eDisplayPowerInfoEXT: return "DisplayPowerInfoEXT";
      case StructureType::eDeviceEventInfoEXT: return "DeviceEventInfoEXT";
      case StructureType::eDisplayEventInfoEXT: return "DisplayEventInfoEXT";
      case StructureType::eSwapchainCounterCreateInfoEXT: return "SwapchainCounterCreateInfoEXT";
      case StructureType::ePhysicalDeviceDiscardRectanglePropertiesEXT: return "PhysicalDeviceDiscardRectanglePropertiesEXT";
      case StructureType::ePipelineDiscardRectangleStateCreateInfoEXT: return "PipelineDiscardRectangleStateCreateInfoEXT";
      case StructureType::ePhysicalDeviceConservativeRasterizationPropertiesEXT: return "PhysicalDeviceConservativeRasterizationPropertiesEXT";
      case StructureType::ePipelineRasterizationConservativeStateCreateInfoEXT: return "PipelineRasterizationConservativeStateCreateInfoEXT";
      case StructureType::ePhysicalDeviceDepthClipEnableFeaturesEXT: return "PhysicalDeviceDepthClipEnableFeaturesEXT";
      case StructureType::ePipelineRasterizationDepthClipStateCreateInfoEXT: return "PipelineRasterizationDepthClipStateCreateInfoEXT";
      case StructureType::eHdrMetadataEXT: return "HdrMetadataEXT";
      case StructureType::eSharedPresentSurfaceCapabilitiesKHR: return "SharedPresentSurfaceCapabilitiesKHR";
      case StructureType::eImportFenceFdInfoKHR: return "ImportFenceFdInfoKHR";
      case StructureType::eFenceGetFdInfoKHR: return "FenceGetFdInfoKHR";
      case StructureType::ePhysicalDevicePerformanceQueryFeaturesKHR: return "PhysicalDevicePerformanceQueryFeaturesKHR";
      case StructureType::ePhysicalDevicePerformanceQueryPropertiesKHR: return "PhysicalDevicePerformanceQueryPropertiesKHR";
      case StructureType::eQueryPoolPerformanceCreateInfoKHR: return "QueryPoolPerformanceCreateInfoKHR";
      case StructureType::ePerformanceQuerySubmitInfoKHR: return "PerformanceQuerySubmitInfoKHR";
      case StructureType::eAcquireProfilingLockInfoKHR: return "AcquireProfilingLockInfoKHR";
      case StructureType::ePerformanceCounterKHR: return "PerformanceCounterKHR";
      case StructureType::ePerformanceCounterDescriptionKHR: return "PerformanceCounterDescriptionKHR";
      case StructureType::ePerformanceQueryReservationInfoKHR: return "PerformanceQueryReservationInfoKHR";
      case StructureType::ePhysicalDeviceSurfaceInfo2KHR: return "PhysicalDeviceSurfaceInfo2KHR";
      case StructureType::eSurfaceCapabilities2KHR: return "SurfaceCapabilities2KHR";
      case StructureType::eSurfaceFormat2KHR: return "SurfaceFormat2KHR";
      case StructureType::eDisplayProperties2KHR: return "DisplayProperties2KHR";
      case StructureType::eDisplayPlaneProperties2KHR: return "DisplayPlaneProperties2KHR";
      case StructureType::eDisplayModeProperties2KHR: return "DisplayModeProperties2KHR";
      case StructureType::eDisplayPlaneInfo2KHR: return "DisplayPlaneInfo2KHR";
      case StructureType::eDisplayPlaneCapabilities2KHR: return "DisplayPlaneCapabilities2KHR";
      case StructureType::eDebugUtilsObjectNameInfoEXT: return "DebugUtilsObjectNameInfoEXT";
      case StructureType::eDebugUtilsObjectTagInfoEXT: return "DebugUtilsObjectTagInfoEXT";
      case StructureType::eDebugUtilsLabelEXT: return "DebugUtilsLabelEXT";
      case StructureType::eDebugUtilsMessengerCallbackDataEXT: return "DebugUtilsMessengerCallbackDataEXT";
      case StructureType::eDebugUtilsMessengerCreateInfoEXT: return "DebugUtilsMessengerCreateInfoEXT";
      case StructureType::eSampleLocationsInfoEXT: return "SampleLocationsInfoEXT";
      case StructureType::eRenderPassSampleLocationsBeginInfoEXT: return "RenderPassSampleLocationsBeginInfoEXT";
      case StructureType::ePipelineSampleLocationsStateCreateInfoEXT: return "PipelineSampleLocationsStateCreateInfoEXT";
      case StructureType::ePhysicalDeviceSampleLocationsPropertiesEXT: return "PhysicalDeviceSampleLocationsPropertiesEXT";
      case StructureType::eMultisamplePropertiesEXT: return "MultisamplePropertiesEXT";
      case StructureType::ePhysicalDeviceBlendOperationAdvancedFeaturesEXT: return "PhysicalDeviceBlendOperationAdvancedFeaturesEXT";
      case StructureType::ePhysicalDeviceBlendOperationAdvancedPropertiesEXT: return "PhysicalDeviceBlendOperationAdvancedPropertiesEXT";
      case StructureType::ePipelineColorBlendAdvancedStateCreateInfoEXT: return "PipelineColorBlendAdvancedStateCreateInfoEXT";
      case StructureType::eDrmFormatModifierPropertiesListEXT: return "DrmFormatModifierPropertiesListEXT";
      case StructureType::ePhysicalDeviceImageDrmFormatModifierInfoEXT: return "PhysicalDeviceImageDrmFormatModifierInfoEXT";
      case StructureType::eImageDrmFormatModifierListCreateInfoEXT: return "ImageDrmFormatModifierListCreateInfoEXT";
      case StructureType::eImageDrmFormatModifierExplicitCreateInfoEXT: return "ImageDrmFormatModifierExplicitCreateInfoEXT";
      case StructureType::eImageDrmFormatModifierPropertiesEXT: return "ImageDrmFormatModifierPropertiesEXT";
      case StructureType::eDrmFormatModifierPropertiesList2EXT: return "DrmFormatModifierPropertiesList2EXT";
      case StructureType::ePhysicalDeviceImageViewImageFormatInfoEXT: return "PhysicalDeviceImageViewImageFormatInfoEXT";
      case StructureType::eFilterCubicImageViewImageFormatPropertiesEXT: return "FilterCubicImageViewImageFormatPropertiesEXT";
      case StructureType::eImportMemoryHostPointerInfoEXT: return "ImportMemoryHostPointerInfoEXT";
      case StructureType::eMemoryHostPointerPropertiesEXT: return "MemoryHostPointerPropertiesEXT";
      case StructureType::ePhysicalDeviceExternalMemoryHostPropertiesEXT: return "PhysicalDeviceExternalMemoryHostPropertiesEXT";
      case StructureType::ePhysicalDeviceShaderClockFeaturesKHR: return "PhysicalDeviceShaderClockFeaturesKHR";
      case StructureType::ePhysicalDeviceVertexAttributeDivisorPropertiesEXT: return "PhysicalDeviceVertexAttributeDivisorPropertiesEXT";
      case StructureType::ePhysicalDevicePciBusInfoPropertiesEXT: return "PhysicalDevicePciBusInfoPropertiesEXT";
      case StructureType::eFragmentShadingRateAttachmentInfoKHR: return "FragmentShadingRateAttachmentInfoKHR";
      case StructureType::ePipelineFragmentShadingRateStateCreateInfoKHR: return "PipelineFragmentShadingRateStateCreateInfoKHR";
      case StructureType::ePhysicalDeviceFragmentShadingRatePropertiesKHR: return "PhysicalDeviceFragmentShadingRatePropertiesKHR";
      case StructureType::ePhysicalDeviceFragmentShadingRateFeaturesKHR: return "PhysicalDeviceFragmentShadingRateFeaturesKHR";
      case StructureType::ePhysicalDeviceFragmentShadingRateKHR: return "PhysicalDeviceFragmentShadingRateKHR";
      case StructureType::ePhysicalDeviceShaderImageAtomicInt64FeaturesEXT: return "PhysicalDeviceShaderImageAtomicInt64FeaturesEXT";
      case StructureType::ePhysicalDeviceMemoryBudgetPropertiesEXT: return "PhysicalDeviceMemoryBudgetPropertiesEXT";
      case StructureType::eValidationFeaturesEXT: return "ValidationFeaturesEXT";
      case StructureType::ePhysicalDeviceFragmentShaderInterlockFeaturesEXT: return "PhysicalDeviceFragmentShaderInterlockFeaturesEXT";
      case StructureType::ePhysicalDeviceYcbcrImageArraysFeaturesEXT: return "PhysicalDeviceYcbcrImageArraysFeaturesEXT";
      case StructureType::eHeadlessSurfaceCreateInfoEXT: return "HeadlessSurfaceCreateInfoEXT";
      case StructureType::ePhysicalDeviceShaderAtomicFloatFeaturesEXT: return "PhysicalDeviceShaderAtomicFloatFeaturesEXT";
      case StructureType::ePhysicalDeviceExtendedDynamicStateFeaturesEXT: return "PhysicalDeviceExtendedDynamicStateFeaturesEXT";
      case StructureType::ePhysicalDeviceTexelBufferAlignmentFeaturesEXT: return "PhysicalDeviceTexelBufferAlignmentFeaturesEXT";
      case StructureType::ePhysicalDeviceRobustness2FeaturesEXT: return "PhysicalDeviceRobustness2FeaturesEXT";
      case StructureType::ePhysicalDeviceRobustness2PropertiesEXT: return "PhysicalDeviceRobustness2PropertiesEXT";
      case StructureType::eSamplerCustomBorderColorCreateInfoEXT: return "SamplerCustomBorderColorCreateInfoEXT";
      case StructureType::ePhysicalDeviceCustomBorderColorPropertiesEXT: return "PhysicalDeviceCustomBorderColorPropertiesEXT";
      case StructureType::ePhysicalDeviceCustomBorderColorFeaturesEXT: return "PhysicalDeviceCustomBorderColorFeaturesEXT";
      case StructureType::eRefreshObjectListKHR: return "RefreshObjectListKHR";
      case StructureType::eQueueFamilyCheckpointProperties2NV: return "QueueFamilyCheckpointProperties2NV";
      case StructureType::eCheckpointData2NV: return "CheckpointData2NV";
      case StructureType::ePhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT: return "PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT";
      case StructureType::ePhysicalDevice4444FormatsFeaturesEXT: return "PhysicalDevice4444FormatsFeaturesEXT";
      case StructureType::ePhysicalDeviceVertexInputDynamicStateFeaturesEXT: return "PhysicalDeviceVertexInputDynamicStateFeaturesEXT";
      case StructureType::eVertexInputBindingDescription2EXT: return "VertexInputBindingDescription2EXT";
      case StructureType::eVertexInputAttributeDescription2EXT: return "VertexInputAttributeDescription2EXT";
#if defined( VK_USE_PLATFORM_SCI )
      case StructureType::eImportFenceSciSyncInfoNV: return "ImportFenceSciSyncInfoNV";
      case StructureType::eExportFenceSciSyncInfoNV: return "ExportFenceSciSyncInfoNV";
      case StructureType::eFenceGetSciSyncInfoNV: return "FenceGetSciSyncInfoNV";
      case StructureType::eSciSyncAttributesInfoNV: return "SciSyncAttributesInfoNV";
      case StructureType::eImportSemaphoreSciSyncInfoNV: return "ImportSemaphoreSciSyncInfoNV";
      case StructureType::eExportSemaphoreSciSyncInfoNV: return "ExportSemaphoreSciSyncInfoNV";
      case StructureType::eSemaphoreGetSciSyncInfoNV: return "SemaphoreGetSciSyncInfoNV";
      case StructureType::ePhysicalDeviceExternalSciSyncFeaturesNV: return "PhysicalDeviceExternalSciSyncFeaturesNV";
      case StructureType::eImportMemorySciBufInfoNV: return "ImportMemorySciBufInfoNV";
      case StructureType::eExportMemorySciBufInfoNV: return "ExportMemorySciBufInfoNV";
      case StructureType::eMemoryGetSciBufInfoNV: return "MemoryGetSciBufInfoNV";
      case StructureType::eMemorySciBufPropertiesNV: return "MemorySciBufPropertiesNV";
      case StructureType::ePhysicalDeviceExternalMemorySciBufFeaturesNV: return "PhysicalDeviceExternalMemorySciBufFeaturesNV";
#endif /*VK_USE_PLATFORM_SCI*/
      case StructureType::ePhysicalDeviceExtendedDynamicState2FeaturesEXT: return "PhysicalDeviceExtendedDynamicState2FeaturesEXT";
      case StructureType::ePhysicalDeviceColorWriteEnableFeaturesEXT: return "PhysicalDeviceColorWriteEnableFeaturesEXT";
      case StructureType::ePipelineColorWriteCreateInfoEXT: return "PipelineColorWriteCreateInfoEXT";
      case StructureType::eApplicationParametersEXT: return "ApplicationParametersEXT";
#if defined( VK_USE_PLATFORM_SCI )
      case StructureType::eSemaphoreSciSyncPoolCreateInfoNV: return "SemaphoreSciSyncPoolCreateInfoNV";
      case StructureType::eSemaphoreSciSyncCreateInfoNV: return "SemaphoreSciSyncCreateInfoNV";
      case StructureType::ePhysicalDeviceExternalSciSync2FeaturesNV: return "PhysicalDeviceExternalSciSync2FeaturesNV";
      case StructureType::eDeviceSemaphoreSciSyncPoolReservationCreateInfoNV: return "DeviceSemaphoreSciSyncPoolReservationCreateInfoNV";
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      case StructureType::eScreenBufferPropertiesQNX: return "ScreenBufferPropertiesQNX";
      case StructureType::eScreenBufferFormatPropertiesQNX: return "ScreenBufferFormatPropertiesQNX";
      case StructureType::eImportScreenBufferInfoQNX: return "ImportScreenBufferInfoQNX";
      case StructureType::eExternalFormatQNX: return "ExternalFormatQNX";
      case StructureType::ePhysicalDeviceExternalMemoryScreenBufferFeaturesQNX: return "PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX";
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCacheHeaderVersion value )
  {
    switch ( value )
    {
      case PipelineCacheHeaderVersion::eOne: return "One";
      case PipelineCacheHeaderVersion::eSafetyCriticalOne: return "SafetyCriticalOne";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ObjectType value )
  {
    switch ( value )
    {
      case ObjectType::eUnknown: return "Unknown";
      case ObjectType::eInstance: return "Instance";
      case ObjectType::ePhysicalDevice: return "PhysicalDevice";
      case ObjectType::eDevice: return "Device";
      case ObjectType::eQueue: return "Queue";
      case ObjectType::eSemaphore: return "Semaphore";
      case ObjectType::eCommandBuffer: return "CommandBuffer";
      case ObjectType::eFence: return "Fence";
      case ObjectType::eDeviceMemory: return "DeviceMemory";
      case ObjectType::eBuffer: return "Buffer";
      case ObjectType::eImage: return "Image";
      case ObjectType::eEvent: return "Event";
      case ObjectType::eQueryPool: return "QueryPool";
      case ObjectType::eBufferView: return "BufferView";
      case ObjectType::eImageView: return "ImageView";
      case ObjectType::eShaderModule: return "ShaderModule";
      case ObjectType::ePipelineCache: return "PipelineCache";
      case ObjectType::ePipelineLayout: return "PipelineLayout";
      case ObjectType::eRenderPass: return "RenderPass";
      case ObjectType::ePipeline: return "Pipeline";
      case ObjectType::eDescriptorSetLayout: return "DescriptorSetLayout";
      case ObjectType::eSampler: return "Sampler";
      case ObjectType::eDescriptorPool: return "DescriptorPool";
      case ObjectType::eDescriptorSet: return "DescriptorSet";
      case ObjectType::eFramebuffer: return "Framebuffer";
      case ObjectType::eCommandPool: return "CommandPool";
      case ObjectType::eSamplerYcbcrConversion: return "SamplerYcbcrConversion";
      case ObjectType::ePrivateDataSlot: return "PrivateDataSlot";
      case ObjectType::eSurfaceKHR: return "SurfaceKHR";
      case ObjectType::eSwapchainKHR: return "SwapchainKHR";
      case ObjectType::eDisplayKHR: return "DisplayKHR";
      case ObjectType::eDisplayModeKHR: return "DisplayModeKHR";
      case ObjectType::eDebugUtilsMessengerEXT: return "DebugUtilsMessengerEXT";
#if defined( VK_USE_PLATFORM_SCI )
      case ObjectType::eSemaphoreSciSyncPoolNV: return "SemaphoreSciSyncPoolNV";
#endif /*VK_USE_PLATFORM_SCI*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( VendorId value )
  {
    switch ( value )
    {
      case VendorId::eVIV: return "VIV";
      case VendorId::eVSI: return "VSI";
      case VendorId::eKazan: return "Kazan";
      case VendorId::eCodeplay: return "Codeplay";
      case VendorId::eMESA: return "MESA";
      case VendorId::ePocl: return "Pocl";
      case VendorId::eMobileye: return "Mobileye";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( Format value )
  {
    switch ( value )
    {
      case Format::eUndefined: return "Undefined";
      case Format::eR4G4UnormPack8: return "R4G4UnormPack8";
      case Format::eR4G4B4A4UnormPack16: return "R4G4B4A4UnormPack16";
      case Format::eB4G4R4A4UnormPack16: return "B4G4R4A4UnormPack16";
      case Format::eR5G6B5UnormPack16: return "R5G6B5UnormPack16";
      case Format::eB5G6R5UnormPack16: return "B5G6R5UnormPack16";
      case Format::eR5G5B5A1UnormPack16: return "R5G5B5A1UnormPack16";
      case Format::eB5G5R5A1UnormPack16: return "B5G5R5A1UnormPack16";
      case Format::eA1R5G5B5UnormPack16: return "A1R5G5B5UnormPack16";
      case Format::eR8Unorm: return "R8Unorm";
      case Format::eR8Snorm: return "R8Snorm";
      case Format::eR8Uscaled: return "R8Uscaled";
      case Format::eR8Sscaled: return "R8Sscaled";
      case Format::eR8Uint: return "R8Uint";
      case Format::eR8Sint: return "R8Sint";
      case Format::eR8Srgb: return "R8Srgb";
      case Format::eR8G8Unorm: return "R8G8Unorm";
      case Format::eR8G8Snorm: return "R8G8Snorm";
      case Format::eR8G8Uscaled: return "R8G8Uscaled";
      case Format::eR8G8Sscaled: return "R8G8Sscaled";
      case Format::eR8G8Uint: return "R8G8Uint";
      case Format::eR8G8Sint: return "R8G8Sint";
      case Format::eR8G8Srgb: return "R8G8Srgb";
      case Format::eR8G8B8Unorm: return "R8G8B8Unorm";
      case Format::eR8G8B8Snorm: return "R8G8B8Snorm";
      case Format::eR8G8B8Uscaled: return "R8G8B8Uscaled";
      case Format::eR8G8B8Sscaled: return "R8G8B8Sscaled";
      case Format::eR8G8B8Uint: return "R8G8B8Uint";
      case Format::eR8G8B8Sint: return "R8G8B8Sint";
      case Format::eR8G8B8Srgb: return "R8G8B8Srgb";
      case Format::eB8G8R8Unorm: return "B8G8R8Unorm";
      case Format::eB8G8R8Snorm: return "B8G8R8Snorm";
      case Format::eB8G8R8Uscaled: return "B8G8R8Uscaled";
      case Format::eB8G8R8Sscaled: return "B8G8R8Sscaled";
      case Format::eB8G8R8Uint: return "B8G8R8Uint";
      case Format::eB8G8R8Sint: return "B8G8R8Sint";
      case Format::eB8G8R8Srgb: return "B8G8R8Srgb";
      case Format::eR8G8B8A8Unorm: return "R8G8B8A8Unorm";
      case Format::eR8G8B8A8Snorm: return "R8G8B8A8Snorm";
      case Format::eR8G8B8A8Uscaled: return "R8G8B8A8Uscaled";
      case Format::eR8G8B8A8Sscaled: return "R8G8B8A8Sscaled";
      case Format::eR8G8B8A8Uint: return "R8G8B8A8Uint";
      case Format::eR8G8B8A8Sint: return "R8G8B8A8Sint";
      case Format::eR8G8B8A8Srgb: return "R8G8B8A8Srgb";
      case Format::eB8G8R8A8Unorm: return "B8G8R8A8Unorm";
      case Format::eB8G8R8A8Snorm: return "B8G8R8A8Snorm";
      case Format::eB8G8R8A8Uscaled: return "B8G8R8A8Uscaled";
      case Format::eB8G8R8A8Sscaled: return "B8G8R8A8Sscaled";
      case Format::eB8G8R8A8Uint: return "B8G8R8A8Uint";
      case Format::eB8G8R8A8Sint: return "B8G8R8A8Sint";
      case Format::eB8G8R8A8Srgb: return "B8G8R8A8Srgb";
      case Format::eA8B8G8R8UnormPack32: return "A8B8G8R8UnormPack32";
      case Format::eA8B8G8R8SnormPack32: return "A8B8G8R8SnormPack32";
      case Format::eA8B8G8R8UscaledPack32: return "A8B8G8R8UscaledPack32";
      case Format::eA8B8G8R8SscaledPack32: return "A8B8G8R8SscaledPack32";
      case Format::eA8B8G8R8UintPack32: return "A8B8G8R8UintPack32";
      case Format::eA8B8G8R8SintPack32: return "A8B8G8R8SintPack32";
      case Format::eA8B8G8R8SrgbPack32: return "A8B8G8R8SrgbPack32";
      case Format::eA2R10G10B10UnormPack32: return "A2R10G10B10UnormPack32";
      case Format::eA2R10G10B10SnormPack32: return "A2R10G10B10SnormPack32";
      case Format::eA2R10G10B10UscaledPack32: return "A2R10G10B10UscaledPack32";
      case Format::eA2R10G10B10SscaledPack32: return "A2R10G10B10SscaledPack32";
      case Format::eA2R10G10B10UintPack32: return "A2R10G10B10UintPack32";
      case Format::eA2R10G10B10SintPack32: return "A2R10G10B10SintPack32";
      case Format::eA2B10G10R10UnormPack32: return "A2B10G10R10UnormPack32";
      case Format::eA2B10G10R10SnormPack32: return "A2B10G10R10SnormPack32";
      case Format::eA2B10G10R10UscaledPack32: return "A2B10G10R10UscaledPack32";
      case Format::eA2B10G10R10SscaledPack32: return "A2B10G10R10SscaledPack32";
      case Format::eA2B10G10R10UintPack32: return "A2B10G10R10UintPack32";
      case Format::eA2B10G10R10SintPack32: return "A2B10G10R10SintPack32";
      case Format::eR16Unorm: return "R16Unorm";
      case Format::eR16Snorm: return "R16Snorm";
      case Format::eR16Uscaled: return "R16Uscaled";
      case Format::eR16Sscaled: return "R16Sscaled";
      case Format::eR16Uint: return "R16Uint";
      case Format::eR16Sint: return "R16Sint";
      case Format::eR16Sfloat: return "R16Sfloat";
      case Format::eR16G16Unorm: return "R16G16Unorm";
      case Format::eR16G16Snorm: return "R16G16Snorm";
      case Format::eR16G16Uscaled: return "R16G16Uscaled";
      case Format::eR16G16Sscaled: return "R16G16Sscaled";
      case Format::eR16G16Uint: return "R16G16Uint";
      case Format::eR16G16Sint: return "R16G16Sint";
      case Format::eR16G16Sfloat: return "R16G16Sfloat";
      case Format::eR16G16B16Unorm: return "R16G16B16Unorm";
      case Format::eR16G16B16Snorm: return "R16G16B16Snorm";
      case Format::eR16G16B16Uscaled: return "R16G16B16Uscaled";
      case Format::eR16G16B16Sscaled: return "R16G16B16Sscaled";
      case Format::eR16G16B16Uint: return "R16G16B16Uint";
      case Format::eR16G16B16Sint: return "R16G16B16Sint";
      case Format::eR16G16B16Sfloat: return "R16G16B16Sfloat";
      case Format::eR16G16B16A16Unorm: return "R16G16B16A16Unorm";
      case Format::eR16G16B16A16Snorm: return "R16G16B16A16Snorm";
      case Format::eR16G16B16A16Uscaled: return "R16G16B16A16Uscaled";
      case Format::eR16G16B16A16Sscaled: return "R16G16B16A16Sscaled";
      case Format::eR16G16B16A16Uint: return "R16G16B16A16Uint";
      case Format::eR16G16B16A16Sint: return "R16G16B16A16Sint";
      case Format::eR16G16B16A16Sfloat: return "R16G16B16A16Sfloat";
      case Format::eR32Uint: return "R32Uint";
      case Format::eR32Sint: return "R32Sint";
      case Format::eR32Sfloat: return "R32Sfloat";
      case Format::eR32G32Uint: return "R32G32Uint";
      case Format::eR32G32Sint: return "R32G32Sint";
      case Format::eR32G32Sfloat: return "R32G32Sfloat";
      case Format::eR32G32B32Uint: return "R32G32B32Uint";
      case Format::eR32G32B32Sint: return "R32G32B32Sint";
      case Format::eR32G32B32Sfloat: return "R32G32B32Sfloat";
      case Format::eR32G32B32A32Uint: return "R32G32B32A32Uint";
      case Format::eR32G32B32A32Sint: return "R32G32B32A32Sint";
      case Format::eR32G32B32A32Sfloat: return "R32G32B32A32Sfloat";
      case Format::eR64Uint: return "R64Uint";
      case Format::eR64Sint: return "R64Sint";
      case Format::eR64Sfloat: return "R64Sfloat";
      case Format::eR64G64Uint: return "R64G64Uint";
      case Format::eR64G64Sint: return "R64G64Sint";
      case Format::eR64G64Sfloat: return "R64G64Sfloat";
      case Format::eR64G64B64Uint: return "R64G64B64Uint";
      case Format::eR64G64B64Sint: return "R64G64B64Sint";
      case Format::eR64G64B64Sfloat: return "R64G64B64Sfloat";
      case Format::eR64G64B64A64Uint: return "R64G64B64A64Uint";
      case Format::eR64G64B64A64Sint: return "R64G64B64A64Sint";
      case Format::eR64G64B64A64Sfloat: return "R64G64B64A64Sfloat";
      case Format::eB10G11R11UfloatPack32: return "B10G11R11UfloatPack32";
      case Format::eE5B9G9R9UfloatPack32: return "E5B9G9R9UfloatPack32";
      case Format::eD16Unorm: return "D16Unorm";
      case Format::eX8D24UnormPack32: return "X8D24UnormPack32";
      case Format::eD32Sfloat: return "D32Sfloat";
      case Format::eS8Uint: return "S8Uint";
      case Format::eD16UnormS8Uint: return "D16UnormS8Uint";
      case Format::eD24UnormS8Uint: return "D24UnormS8Uint";
      case Format::eD32SfloatS8Uint: return "D32SfloatS8Uint";
      case Format::eBc1RgbUnormBlock: return "Bc1RgbUnormBlock";
      case Format::eBc1RgbSrgbBlock: return "Bc1RgbSrgbBlock";
      case Format::eBc1RgbaUnormBlock: return "Bc1RgbaUnormBlock";
      case Format::eBc1RgbaSrgbBlock: return "Bc1RgbaSrgbBlock";
      case Format::eBc2UnormBlock: return "Bc2UnormBlock";
      case Format::eBc2SrgbBlock: return "Bc2SrgbBlock";
      case Format::eBc3UnormBlock: return "Bc3UnormBlock";
      case Format::eBc3SrgbBlock: return "Bc3SrgbBlock";
      case Format::eBc4UnormBlock: return "Bc4UnormBlock";
      case Format::eBc4SnormBlock: return "Bc4SnormBlock";
      case Format::eBc5UnormBlock: return "Bc5UnormBlock";
      case Format::eBc5SnormBlock: return "Bc5SnormBlock";
      case Format::eBc6HUfloatBlock: return "Bc6HUfloatBlock";
      case Format::eBc6HSfloatBlock: return "Bc6HSfloatBlock";
      case Format::eBc7UnormBlock: return "Bc7UnormBlock";
      case Format::eBc7SrgbBlock: return "Bc7SrgbBlock";
      case Format::eEtc2R8G8B8UnormBlock: return "Etc2R8G8B8UnormBlock";
      case Format::eEtc2R8G8B8SrgbBlock: return "Etc2R8G8B8SrgbBlock";
      case Format::eEtc2R8G8B8A1UnormBlock: return "Etc2R8G8B8A1UnormBlock";
      case Format::eEtc2R8G8B8A1SrgbBlock: return "Etc2R8G8B8A1SrgbBlock";
      case Format::eEtc2R8G8B8A8UnormBlock: return "Etc2R8G8B8A8UnormBlock";
      case Format::eEtc2R8G8B8A8SrgbBlock: return "Etc2R8G8B8A8SrgbBlock";
      case Format::eEacR11UnormBlock: return "EacR11UnormBlock";
      case Format::eEacR11SnormBlock: return "EacR11SnormBlock";
      case Format::eEacR11G11UnormBlock: return "EacR11G11UnormBlock";
      case Format::eEacR11G11SnormBlock: return "EacR11G11SnormBlock";
      case Format::eAstc4x4UnormBlock: return "Astc4x4UnormBlock";
      case Format::eAstc4x4SrgbBlock: return "Astc4x4SrgbBlock";
      case Format::eAstc5x4UnormBlock: return "Astc5x4UnormBlock";
      case Format::eAstc5x4SrgbBlock: return "Astc5x4SrgbBlock";
      case Format::eAstc5x5UnormBlock: return "Astc5x5UnormBlock";
      case Format::eAstc5x5SrgbBlock: return "Astc5x5SrgbBlock";
      case Format::eAstc6x5UnormBlock: return "Astc6x5UnormBlock";
      case Format::eAstc6x5SrgbBlock: return "Astc6x5SrgbBlock";
      case Format::eAstc6x6UnormBlock: return "Astc6x6UnormBlock";
      case Format::eAstc6x6SrgbBlock: return "Astc6x6SrgbBlock";
      case Format::eAstc8x5UnormBlock: return "Astc8x5UnormBlock";
      case Format::eAstc8x5SrgbBlock: return "Astc8x5SrgbBlock";
      case Format::eAstc8x6UnormBlock: return "Astc8x6UnormBlock";
      case Format::eAstc8x6SrgbBlock: return "Astc8x6SrgbBlock";
      case Format::eAstc8x8UnormBlock: return "Astc8x8UnormBlock";
      case Format::eAstc8x8SrgbBlock: return "Astc8x8SrgbBlock";
      case Format::eAstc10x5UnormBlock: return "Astc10x5UnormBlock";
      case Format::eAstc10x5SrgbBlock: return "Astc10x5SrgbBlock";
      case Format::eAstc10x6UnormBlock: return "Astc10x6UnormBlock";
      case Format::eAstc10x6SrgbBlock: return "Astc10x6SrgbBlock";
      case Format::eAstc10x8UnormBlock: return "Astc10x8UnormBlock";
      case Format::eAstc10x8SrgbBlock: return "Astc10x8SrgbBlock";
      case Format::eAstc10x10UnormBlock: return "Astc10x10UnormBlock";
      case Format::eAstc10x10SrgbBlock: return "Astc10x10SrgbBlock";
      case Format::eAstc12x10UnormBlock: return "Astc12x10UnormBlock";
      case Format::eAstc12x10SrgbBlock: return "Astc12x10SrgbBlock";
      case Format::eAstc12x12UnormBlock: return "Astc12x12UnormBlock";
      case Format::eAstc12x12SrgbBlock: return "Astc12x12SrgbBlock";
      case Format::eG8B8G8R8422Unorm: return "G8B8G8R8422Unorm";
      case Format::eB8G8R8G8422Unorm: return "B8G8R8G8422Unorm";
      case Format::eG8B8R83Plane420Unorm: return "G8B8R83Plane420Unorm";
      case Format::eG8B8R82Plane420Unorm: return "G8B8R82Plane420Unorm";
      case Format::eG8B8R83Plane422Unorm: return "G8B8R83Plane422Unorm";
      case Format::eG8B8R82Plane422Unorm: return "G8B8R82Plane422Unorm";
      case Format::eG8B8R83Plane444Unorm: return "G8B8R83Plane444Unorm";
      case Format::eR10X6UnormPack16: return "R10X6UnormPack16";
      case Format::eR10X6G10X6Unorm2Pack16: return "R10X6G10X6Unorm2Pack16";
      case Format::eR10X6G10X6B10X6A10X6Unorm4Pack16: return "R10X6G10X6B10X6A10X6Unorm4Pack16";
      case Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16: return "G10X6B10X6G10X6R10X6422Unorm4Pack16";
      case Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16: return "B10X6G10X6R10X6G10X6422Unorm4Pack16";
      case Format::eG10X6B10X6R10X63Plane420Unorm3Pack16: return "G10X6B10X6R10X63Plane420Unorm3Pack16";
      case Format::eG10X6B10X6R10X62Plane420Unorm3Pack16: return "G10X6B10X6R10X62Plane420Unorm3Pack16";
      case Format::eG10X6B10X6R10X63Plane422Unorm3Pack16: return "G10X6B10X6R10X63Plane422Unorm3Pack16";
      case Format::eG10X6B10X6R10X62Plane422Unorm3Pack16: return "G10X6B10X6R10X62Plane422Unorm3Pack16";
      case Format::eG10X6B10X6R10X63Plane444Unorm3Pack16: return "G10X6B10X6R10X63Plane444Unorm3Pack16";
      case Format::eR12X4UnormPack16: return "R12X4UnormPack16";
      case Format::eR12X4G12X4Unorm2Pack16: return "R12X4G12X4Unorm2Pack16";
      case Format::eR12X4G12X4B12X4A12X4Unorm4Pack16: return "R12X4G12X4B12X4A12X4Unorm4Pack16";
      case Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16: return "G12X4B12X4G12X4R12X4422Unorm4Pack16";
      case Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16: return "B12X4G12X4R12X4G12X4422Unorm4Pack16";
      case Format::eG12X4B12X4R12X43Plane420Unorm3Pack16: return "G12X4B12X4R12X43Plane420Unorm3Pack16";
      case Format::eG12X4B12X4R12X42Plane420Unorm3Pack16: return "G12X4B12X4R12X42Plane420Unorm3Pack16";
      case Format::eG12X4B12X4R12X43Plane422Unorm3Pack16: return "G12X4B12X4R12X43Plane422Unorm3Pack16";
      case Format::eG12X4B12X4R12X42Plane422Unorm3Pack16: return "G12X4B12X4R12X42Plane422Unorm3Pack16";
      case Format::eG12X4B12X4R12X43Plane444Unorm3Pack16: return "G12X4B12X4R12X43Plane444Unorm3Pack16";
      case Format::eG16B16G16R16422Unorm: return "G16B16G16R16422Unorm";
      case Format::eB16G16R16G16422Unorm: return "B16G16R16G16422Unorm";
      case Format::eG16B16R163Plane420Unorm: return "G16B16R163Plane420Unorm";
      case Format::eG16B16R162Plane420Unorm: return "G16B16R162Plane420Unorm";
      case Format::eG16B16R163Plane422Unorm: return "G16B16R163Plane422Unorm";
      case Format::eG16B16R162Plane422Unorm: return "G16B16R162Plane422Unorm";
      case Format::eG16B16R163Plane444Unorm: return "G16B16R163Plane444Unorm";
      case Format::eG8B8R82Plane444Unorm: return "G8B8R82Plane444Unorm";
      case Format::eG10X6B10X6R10X62Plane444Unorm3Pack16: return "G10X6B10X6R10X62Plane444Unorm3Pack16";
      case Format::eG12X4B12X4R12X42Plane444Unorm3Pack16: return "G12X4B12X4R12X42Plane444Unorm3Pack16";
      case Format::eG16B16R162Plane444Unorm: return "G16B16R162Plane444Unorm";
      case Format::eA4R4G4B4UnormPack16: return "A4R4G4B4UnormPack16";
      case Format::eA4B4G4R4UnormPack16: return "A4B4G4R4UnormPack16";
      case Format::eAstc4x4SfloatBlock: return "Astc4x4SfloatBlock";
      case Format::eAstc5x4SfloatBlock: return "Astc5x4SfloatBlock";
      case Format::eAstc5x5SfloatBlock: return "Astc5x5SfloatBlock";
      case Format::eAstc6x5SfloatBlock: return "Astc6x5SfloatBlock";
      case Format::eAstc6x6SfloatBlock: return "Astc6x6SfloatBlock";
      case Format::eAstc8x5SfloatBlock: return "Astc8x5SfloatBlock";
      case Format::eAstc8x6SfloatBlock: return "Astc8x6SfloatBlock";
      case Format::eAstc8x8SfloatBlock: return "Astc8x8SfloatBlock";
      case Format::eAstc10x5SfloatBlock: return "Astc10x5SfloatBlock";
      case Format::eAstc10x6SfloatBlock: return "Astc10x6SfloatBlock";
      case Format::eAstc10x8SfloatBlock: return "Astc10x8SfloatBlock";
      case Format::eAstc10x10SfloatBlock: return "Astc10x10SfloatBlock";
      case Format::eAstc12x10SfloatBlock: return "Astc12x10SfloatBlock";
      case Format::eAstc12x12SfloatBlock: return "Astc12x12SfloatBlock";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FormatFeatureFlagBits value )
  {
    switch ( value )
    {
      case FormatFeatureFlagBits::eSampledImage: return "SampledImage";
      case FormatFeatureFlagBits::eStorageImage: return "StorageImage";
      case FormatFeatureFlagBits::eStorageImageAtomic: return "StorageImageAtomic";
      case FormatFeatureFlagBits::eUniformTexelBuffer: return "UniformTexelBuffer";
      case FormatFeatureFlagBits::eStorageTexelBuffer: return "StorageTexelBuffer";
      case FormatFeatureFlagBits::eStorageTexelBufferAtomic: return "StorageTexelBufferAtomic";
      case FormatFeatureFlagBits::eVertexBuffer: return "VertexBuffer";
      case FormatFeatureFlagBits::eColorAttachment: return "ColorAttachment";
      case FormatFeatureFlagBits::eColorAttachmentBlend: return "ColorAttachmentBlend";
      case FormatFeatureFlagBits::eDepthStencilAttachment: return "DepthStencilAttachment";
      case FormatFeatureFlagBits::eBlitSrc: return "BlitSrc";
      case FormatFeatureFlagBits::eBlitDst: return "BlitDst";
      case FormatFeatureFlagBits::eSampledImageFilterLinear: return "SampledImageFilterLinear";
      case FormatFeatureFlagBits::eTransferSrc: return "TransferSrc";
      case FormatFeatureFlagBits::eTransferDst: return "TransferDst";
      case FormatFeatureFlagBits::eMidpointChromaSamples: return "MidpointChromaSamples";
      case FormatFeatureFlagBits::eSampledImageYcbcrConversionLinearFilter: return "SampledImageYcbcrConversionLinearFilter";
      case FormatFeatureFlagBits::eSampledImageYcbcrConversionSeparateReconstructionFilter: return "SampledImageYcbcrConversionSeparateReconstructionFilter";
      case FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicit: return "SampledImageYcbcrConversionChromaReconstructionExplicit";
      case FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable:
        return "SampledImageYcbcrConversionChromaReconstructionExplicitForceable";
      case FormatFeatureFlagBits::eDisjoint: return "Disjoint";
      case FormatFeatureFlagBits::eCositedChromaSamples: return "CositedChromaSamples";
      case FormatFeatureFlagBits::eSampledImageFilterMinmax: return "SampledImageFilterMinmax";
      case FormatFeatureFlagBits::eSampledImageFilterCubicEXT: return "SampledImageFilterCubicEXT";
      case FormatFeatureFlagBits::eFragmentShadingRateAttachmentKHR: return "FragmentShadingRateAttachmentKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ImageCreateFlagBits value )
  {
    switch ( value )
    {
      case ImageCreateFlagBits::eSparseBinding: return "SparseBinding";
      case ImageCreateFlagBits::eSparseResidency: return "SparseResidency";
      case ImageCreateFlagBits::eSparseAliased: return "SparseAliased";
      case ImageCreateFlagBits::eMutableFormat: return "MutableFormat";
      case ImageCreateFlagBits::eCubeCompatible: return "CubeCompatible";
      case ImageCreateFlagBits::eAlias: return "Alias";
      case ImageCreateFlagBits::eSplitInstanceBindRegions: return "SplitInstanceBindRegions";
      case ImageCreateFlagBits::e2DArrayCompatible: return "2DArrayCompatible";
      case ImageCreateFlagBits::eBlockTexelViewCompatible: return "BlockTexelViewCompatible";
      case ImageCreateFlagBits::eExtendedUsage: return "ExtendedUsage";
      case ImageCreateFlagBits::eProtected: return "Protected";
      case ImageCreateFlagBits::eDisjoint: return "Disjoint";
      case ImageCreateFlagBits::eSampleLocationsCompatibleDepthEXT: return "SampleLocationsCompatibleDepthEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ImageTiling value )
  {
    switch ( value )
    {
      case ImageTiling::eOptimal: return "Optimal";
      case ImageTiling::eLinear: return "Linear";
      case ImageTiling::eDrmFormatModifierEXT: return "DrmFormatModifierEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ImageType value )
  {
    switch ( value )
    {
      case ImageType::e1D: return "1D";
      case ImageType::e2D: return "2D";
      case ImageType::e3D: return "3D";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ImageUsageFlagBits value )
  {
    switch ( value )
    {
      case ImageUsageFlagBits::eTransferSrc: return "TransferSrc";
      case ImageUsageFlagBits::eTransferDst: return "TransferDst";
      case ImageUsageFlagBits::eSampled: return "Sampled";
      case ImageUsageFlagBits::eStorage: return "Storage";
      case ImageUsageFlagBits::eColorAttachment: return "ColorAttachment";
      case ImageUsageFlagBits::eDepthStencilAttachment: return "DepthStencilAttachment";
      case ImageUsageFlagBits::eTransientAttachment: return "TransientAttachment";
      case ImageUsageFlagBits::eInputAttachment: return "InputAttachment";
      case ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR: return "FragmentShadingRateAttachmentKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( InstanceCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( InternalAllocationType value )
  {
    switch ( value )
    {
      case InternalAllocationType::eExecutable: return "Executable";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryHeapFlagBits value )
  {
    switch ( value )
    {
      case MemoryHeapFlagBits::eDeviceLocal: return "DeviceLocal";
      case MemoryHeapFlagBits::eMultiInstance: return "MultiInstance";
      case MemoryHeapFlagBits::eSeuSafe: return "SeuSafe";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryPropertyFlagBits value )
  {
    switch ( value )
    {
      case MemoryPropertyFlagBits::eDeviceLocal: return "DeviceLocal";
      case MemoryPropertyFlagBits::eHostVisible: return "HostVisible";
      case MemoryPropertyFlagBits::eHostCoherent: return "HostCoherent";
      case MemoryPropertyFlagBits::eHostCached: return "HostCached";
      case MemoryPropertyFlagBits::eLazilyAllocated: return "LazilyAllocated";
      case MemoryPropertyFlagBits::eProtected: return "Protected";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PhysicalDeviceType value )
  {
    switch ( value )
    {
      case PhysicalDeviceType::eOther: return "Other";
      case PhysicalDeviceType::eIntegratedGpu: return "IntegratedGpu";
      case PhysicalDeviceType::eDiscreteGpu: return "DiscreteGpu";
      case PhysicalDeviceType::eVirtualGpu: return "VirtualGpu";
      case PhysicalDeviceType::eCpu: return "Cpu";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueueFlagBits value )
  {
    switch ( value )
    {
      case QueueFlagBits::eGraphics: return "Graphics";
      case QueueFlagBits::eCompute: return "Compute";
      case QueueFlagBits::eTransfer: return "Transfer";
      case QueueFlagBits::eProtected: return "Protected";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SampleCountFlagBits value )
  {
    switch ( value )
    {
      case SampleCountFlagBits::e1: return "1";
      case SampleCountFlagBits::e2: return "2";
      case SampleCountFlagBits::e4: return "4";
      case SampleCountFlagBits::e8: return "8";
      case SampleCountFlagBits::e16: return "16";
      case SampleCountFlagBits::e32: return "32";
      case SampleCountFlagBits::e64: return "64";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SystemAllocationScope value )
  {
    switch ( value )
    {
      case SystemAllocationScope::eCommand: return "Command";
      case SystemAllocationScope::eObject: return "Object";
      case SystemAllocationScope::eCache: return "Cache";
      case SystemAllocationScope::eDevice: return "Device";
      case SystemAllocationScope::eInstance: return "Instance";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceQueueCreateFlagBits value )
  {
    switch ( value )
    {
      case DeviceQueueCreateFlagBits::eProtected: return "Protected";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineStageFlagBits value )
  {
    switch ( value )
    {
      case PipelineStageFlagBits::eTopOfPipe: return "TopOfPipe";
      case PipelineStageFlagBits::eDrawIndirect: return "DrawIndirect";
      case PipelineStageFlagBits::eVertexInput: return "VertexInput";
      case PipelineStageFlagBits::eVertexShader: return "VertexShader";
      case PipelineStageFlagBits::eTessellationControlShader: return "TessellationControlShader";
      case PipelineStageFlagBits::eTessellationEvaluationShader: return "TessellationEvaluationShader";
      case PipelineStageFlagBits::eGeometryShader: return "GeometryShader";
      case PipelineStageFlagBits::eFragmentShader: return "FragmentShader";
      case PipelineStageFlagBits::eEarlyFragmentTests: return "EarlyFragmentTests";
      case PipelineStageFlagBits::eLateFragmentTests: return "LateFragmentTests";
      case PipelineStageFlagBits::eColorAttachmentOutput: return "ColorAttachmentOutput";
      case PipelineStageFlagBits::eComputeShader: return "ComputeShader";
      case PipelineStageFlagBits::eTransfer: return "Transfer";
      case PipelineStageFlagBits::eBottomOfPipe: return "BottomOfPipe";
      case PipelineStageFlagBits::eHost: return "Host";
      case PipelineStageFlagBits::eAllGraphics: return "AllGraphics";
      case PipelineStageFlagBits::eAllCommands: return "AllCommands";
      case PipelineStageFlagBits::eNone: return "None";
      case PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR: return "FragmentShadingRateAttachmentKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryMapFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageAspectFlagBits value )
  {
    switch ( value )
    {
      case ImageAspectFlagBits::eColor: return "Color";
      case ImageAspectFlagBits::eDepth: return "Depth";
      case ImageAspectFlagBits::eStencil: return "Stencil";
      case ImageAspectFlagBits::eMetadata: return "Metadata";
      case ImageAspectFlagBits::ePlane0: return "Plane0";
      case ImageAspectFlagBits::ePlane1: return "Plane1";
      case ImageAspectFlagBits::ePlane2: return "Plane2";
      case ImageAspectFlagBits::eNone: return "None";
      case ImageAspectFlagBits::eMemoryPlane0EXT: return "MemoryPlane0EXT";
      case ImageAspectFlagBits::eMemoryPlane1EXT: return "MemoryPlane1EXT";
      case ImageAspectFlagBits::eMemoryPlane2EXT: return "MemoryPlane2EXT";
      case ImageAspectFlagBits::eMemoryPlane3EXT: return "MemoryPlane3EXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FenceCreateFlagBits value )
  {
    switch ( value )
    {
      case FenceCreateFlagBits::eSignaled: return "Signaled";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( EventCreateFlagBits value )
  {
    switch ( value )
    {
      case EventCreateFlagBits::eDeviceOnly: return "DeviceOnly";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueryPipelineStatisticFlagBits value )
  {
    switch ( value )
    {
      case QueryPipelineStatisticFlagBits::eInputAssemblyVertices: return "InputAssemblyVertices";
      case QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives: return "InputAssemblyPrimitives";
      case QueryPipelineStatisticFlagBits::eVertexShaderInvocations: return "VertexShaderInvocations";
      case QueryPipelineStatisticFlagBits::eGeometryShaderInvocations: return "GeometryShaderInvocations";
      case QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives: return "GeometryShaderPrimitives";
      case QueryPipelineStatisticFlagBits::eClippingInvocations: return "ClippingInvocations";
      case QueryPipelineStatisticFlagBits::eClippingPrimitives: return "ClippingPrimitives";
      case QueryPipelineStatisticFlagBits::eFragmentShaderInvocations: return "FragmentShaderInvocations";
      case QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches: return "TessellationControlShaderPatches";
      case QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations: return "TessellationEvaluationShaderInvocations";
      case QueryPipelineStatisticFlagBits::eComputeShaderInvocations: return "ComputeShaderInvocations";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueryResultFlagBits value )
  {
    switch ( value )
    {
      case QueryResultFlagBits::e64: return "64";
      case QueryResultFlagBits::eWait: return "Wait";
      case QueryResultFlagBits::eWithAvailability: return "WithAvailability";
      case QueryResultFlagBits::ePartial: return "Partial";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueryType value )
  {
    switch ( value )
    {
      case QueryType::eOcclusion: return "Occlusion";
      case QueryType::ePipelineStatistics: return "PipelineStatistics";
      case QueryType::eTimestamp: return "Timestamp";
      case QueryType::ePerformanceQueryKHR: return "PerformanceQueryKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueryPoolCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( BufferCreateFlagBits value )
  {
    switch ( value )
    {
      case BufferCreateFlagBits::eSparseBinding: return "SparseBinding";
      case BufferCreateFlagBits::eSparseResidency: return "SparseResidency";
      case BufferCreateFlagBits::eSparseAliased: return "SparseAliased";
      case BufferCreateFlagBits::eProtected: return "Protected";
      case BufferCreateFlagBits::eDeviceAddressCaptureReplay: return "DeviceAddressCaptureReplay";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( BufferUsageFlagBits value )
  {
    switch ( value )
    {
      case BufferUsageFlagBits::eTransferSrc: return "TransferSrc";
      case BufferUsageFlagBits::eTransferDst: return "TransferDst";
      case BufferUsageFlagBits::eUniformTexelBuffer: return "UniformTexelBuffer";
      case BufferUsageFlagBits::eStorageTexelBuffer: return "StorageTexelBuffer";
      case BufferUsageFlagBits::eUniformBuffer: return "UniformBuffer";
      case BufferUsageFlagBits::eStorageBuffer: return "StorageBuffer";
      case BufferUsageFlagBits::eIndexBuffer: return "IndexBuffer";
      case BufferUsageFlagBits::eVertexBuffer: return "VertexBuffer";
      case BufferUsageFlagBits::eIndirectBuffer: return "IndirectBuffer";
      case BufferUsageFlagBits::eShaderDeviceAddress: return "ShaderDeviceAddress";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SharingMode value )
  {
    switch ( value )
    {
      case SharingMode::eExclusive: return "Exclusive";
      case SharingMode::eConcurrent: return "Concurrent";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( BufferViewCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageLayout value )
  {
    switch ( value )
    {
      case ImageLayout::eUndefined: return "Undefined";
      case ImageLayout::eGeneral: return "General";
      case ImageLayout::eColorAttachmentOptimal: return "ColorAttachmentOptimal";
      case ImageLayout::eDepthStencilAttachmentOptimal: return "DepthStencilAttachmentOptimal";
      case ImageLayout::eDepthStencilReadOnlyOptimal: return "DepthStencilReadOnlyOptimal";
      case ImageLayout::eShaderReadOnlyOptimal: return "ShaderReadOnlyOptimal";
      case ImageLayout::eTransferSrcOptimal: return "TransferSrcOptimal";
      case ImageLayout::eTransferDstOptimal: return "TransferDstOptimal";
      case ImageLayout::ePreinitialized: return "Preinitialized";
      case ImageLayout::eDepthReadOnlyStencilAttachmentOptimal: return "DepthReadOnlyStencilAttachmentOptimal";
      case ImageLayout::eDepthAttachmentStencilReadOnlyOptimal: return "DepthAttachmentStencilReadOnlyOptimal";
      case ImageLayout::eDepthAttachmentOptimal: return "DepthAttachmentOptimal";
      case ImageLayout::eDepthReadOnlyOptimal: return "DepthReadOnlyOptimal";
      case ImageLayout::eStencilAttachmentOptimal: return "StencilAttachmentOptimal";
      case ImageLayout::eStencilReadOnlyOptimal: return "StencilReadOnlyOptimal";
      case ImageLayout::eReadOnlyOptimal: return "ReadOnlyOptimal";
      case ImageLayout::eAttachmentOptimal: return "AttachmentOptimal";
      case ImageLayout::ePresentSrcKHR: return "PresentSrcKHR";
      case ImageLayout::eSharedPresentKHR: return "SharedPresentKHR";
      case ImageLayout::eFragmentShadingRateAttachmentOptimalKHR: return "FragmentShadingRateAttachmentOptimalKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ComponentSwizzle value )
  {
    switch ( value )
    {
      case ComponentSwizzle::eIdentity: return "Identity";
      case ComponentSwizzle::eZero: return "Zero";
      case ComponentSwizzle::eOne: return "One";
      case ComponentSwizzle::eR: return "R";
      case ComponentSwizzle::eG: return "G";
      case ComponentSwizzle::eB: return "B";
      case ComponentSwizzle::eA: return "A";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ImageViewCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( ImageViewType value )
  {
    switch ( value )
    {
      case ImageViewType::e1D: return "1D";
      case ImageViewType::e2D: return "2D";
      case ImageViewType::e3D: return "3D";
      case ImageViewType::eCube: return "Cube";
      case ImageViewType::e1DArray: return "1DArray";
      case ImageViewType::e2DArray: return "2DArray";
      case ImageViewType::eCubeArray: return "CubeArray";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCacheCreateFlagBits value )
  {
    switch ( value )
    {
      case PipelineCacheCreateFlagBits::eExternallySynchronized: return "ExternallySynchronized";
      case PipelineCacheCreateFlagBits::eReadOnly: return "ReadOnly";
      case PipelineCacheCreateFlagBits::eUseApplicationStorage: return "UseApplicationStorage";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( BlendFactor value )
  {
    switch ( value )
    {
      case BlendFactor::eZero: return "Zero";
      case BlendFactor::eOne: return "One";
      case BlendFactor::eSrcColor: return "SrcColor";
      case BlendFactor::eOneMinusSrcColor: return "OneMinusSrcColor";
      case BlendFactor::eDstColor: return "DstColor";
      case BlendFactor::eOneMinusDstColor: return "OneMinusDstColor";
      case BlendFactor::eSrcAlpha: return "SrcAlpha";
      case BlendFactor::eOneMinusSrcAlpha: return "OneMinusSrcAlpha";
      case BlendFactor::eDstAlpha: return "DstAlpha";
      case BlendFactor::eOneMinusDstAlpha: return "OneMinusDstAlpha";
      case BlendFactor::eConstantColor: return "ConstantColor";
      case BlendFactor::eOneMinusConstantColor: return "OneMinusConstantColor";
      case BlendFactor::eConstantAlpha: return "ConstantAlpha";
      case BlendFactor::eOneMinusConstantAlpha: return "OneMinusConstantAlpha";
      case BlendFactor::eSrcAlphaSaturate: return "SrcAlphaSaturate";
      case BlendFactor::eSrc1Color: return "Src1Color";
      case BlendFactor::eOneMinusSrc1Color: return "OneMinusSrc1Color";
      case BlendFactor::eSrc1Alpha: return "Src1Alpha";
      case BlendFactor::eOneMinusSrc1Alpha: return "OneMinusSrc1Alpha";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( BlendOp value )
  {
    switch ( value )
    {
      case BlendOp::eAdd: return "Add";
      case BlendOp::eSubtract: return "Subtract";
      case BlendOp::eReverseSubtract: return "ReverseSubtract";
      case BlendOp::eMin: return "Min";
      case BlendOp::eMax: return "Max";
      case BlendOp::eZeroEXT: return "ZeroEXT";
      case BlendOp::eSrcEXT: return "SrcEXT";
      case BlendOp::eDstEXT: return "DstEXT";
      case BlendOp::eSrcOverEXT: return "SrcOverEXT";
      case BlendOp::eDstOverEXT: return "DstOverEXT";
      case BlendOp::eSrcInEXT: return "SrcInEXT";
      case BlendOp::eDstInEXT: return "DstInEXT";
      case BlendOp::eSrcOutEXT: return "SrcOutEXT";
      case BlendOp::eDstOutEXT: return "DstOutEXT";
      case BlendOp::eSrcAtopEXT: return "SrcAtopEXT";
      case BlendOp::eDstAtopEXT: return "DstAtopEXT";
      case BlendOp::eXorEXT: return "XorEXT";
      case BlendOp::eMultiplyEXT: return "MultiplyEXT";
      case BlendOp::eScreenEXT: return "ScreenEXT";
      case BlendOp::eOverlayEXT: return "OverlayEXT";
      case BlendOp::eDarkenEXT: return "DarkenEXT";
      case BlendOp::eLightenEXT: return "LightenEXT";
      case BlendOp::eColordodgeEXT: return "ColordodgeEXT";
      case BlendOp::eColorburnEXT: return "ColorburnEXT";
      case BlendOp::eHardlightEXT: return "HardlightEXT";
      case BlendOp::eSoftlightEXT: return "SoftlightEXT";
      case BlendOp::eDifferenceEXT: return "DifferenceEXT";
      case BlendOp::eExclusionEXT: return "ExclusionEXT";
      case BlendOp::eInvertEXT: return "InvertEXT";
      case BlendOp::eInvertRgbEXT: return "InvertRgbEXT";
      case BlendOp::eLineardodgeEXT: return "LineardodgeEXT";
      case BlendOp::eLinearburnEXT: return "LinearburnEXT";
      case BlendOp::eVividlightEXT: return "VividlightEXT";
      case BlendOp::eLinearlightEXT: return "LinearlightEXT";
      case BlendOp::ePinlightEXT: return "PinlightEXT";
      case BlendOp::eHardmixEXT: return "HardmixEXT";
      case BlendOp::eHslHueEXT: return "HslHueEXT";
      case BlendOp::eHslSaturationEXT: return "HslSaturationEXT";
      case BlendOp::eHslColorEXT: return "HslColorEXT";
      case BlendOp::eHslLuminosityEXT: return "HslLuminosityEXT";
      case BlendOp::ePlusEXT: return "PlusEXT";
      case BlendOp::ePlusClampedEXT: return "PlusClampedEXT";
      case BlendOp::ePlusClampedAlphaEXT: return "PlusClampedAlphaEXT";
      case BlendOp::ePlusDarkerEXT: return "PlusDarkerEXT";
      case BlendOp::eMinusEXT: return "MinusEXT";
      case BlendOp::eMinusClampedEXT: return "MinusClampedEXT";
      case BlendOp::eContrastEXT: return "ContrastEXT";
      case BlendOp::eInvertOvgEXT: return "InvertOvgEXT";
      case BlendOp::eRedEXT: return "RedEXT";
      case BlendOp::eGreenEXT: return "GreenEXT";
      case BlendOp::eBlueEXT: return "BlueEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ColorComponentFlagBits value )
  {
    switch ( value )
    {
      case ColorComponentFlagBits::eR: return "R";
      case ColorComponentFlagBits::eG: return "G";
      case ColorComponentFlagBits::eB: return "B";
      case ColorComponentFlagBits::eA: return "A";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CompareOp value )
  {
    switch ( value )
    {
      case CompareOp::eNever: return "Never";
      case CompareOp::eLess: return "Less";
      case CompareOp::eEqual: return "Equal";
      case CompareOp::eLessOrEqual: return "LessOrEqual";
      case CompareOp::eGreater: return "Greater";
      case CompareOp::eNotEqual: return "NotEqual";
      case CompareOp::eGreaterOrEqual: return "GreaterOrEqual";
      case CompareOp::eAlways: return "Always";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CullModeFlagBits value )
  {
    switch ( value )
    {
      case CullModeFlagBits::eNone: return "None";
      case CullModeFlagBits::eFront: return "Front";
      case CullModeFlagBits::eBack: return "Back";
      case CullModeFlagBits::eFrontAndBack: return "FrontAndBack";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DynamicState value )
  {
    switch ( value )
    {
      case DynamicState::eViewport: return "Viewport";
      case DynamicState::eScissor: return "Scissor";
      case DynamicState::eLineWidth: return "LineWidth";
      case DynamicState::eDepthBias: return "DepthBias";
      case DynamicState::eBlendConstants: return "BlendConstants";
      case DynamicState::eDepthBounds: return "DepthBounds";
      case DynamicState::eStencilCompareMask: return "StencilCompareMask";
      case DynamicState::eStencilWriteMask: return "StencilWriteMask";
      case DynamicState::eStencilReference: return "StencilReference";
      case DynamicState::eCullMode: return "CullMode";
      case DynamicState::eFrontFace: return "FrontFace";
      case DynamicState::ePrimitiveTopology: return "PrimitiveTopology";
      case DynamicState::eViewportWithCount: return "ViewportWithCount";
      case DynamicState::eScissorWithCount: return "ScissorWithCount";
      case DynamicState::eVertexInputBindingStride: return "VertexInputBindingStride";
      case DynamicState::eDepthTestEnable: return "DepthTestEnable";
      case DynamicState::eDepthWriteEnable: return "DepthWriteEnable";
      case DynamicState::eDepthCompareOp: return "DepthCompareOp";
      case DynamicState::eDepthBoundsTestEnable: return "DepthBoundsTestEnable";
      case DynamicState::eStencilTestEnable: return "StencilTestEnable";
      case DynamicState::eStencilOp: return "StencilOp";
      case DynamicState::eRasterizerDiscardEnable: return "RasterizerDiscardEnable";
      case DynamicState::eDepthBiasEnable: return "DepthBiasEnable";
      case DynamicState::ePrimitiveRestartEnable: return "PrimitiveRestartEnable";
      case DynamicState::eDiscardRectangleEXT: return "DiscardRectangleEXT";
      case DynamicState::eDiscardRectangleEnableEXT: return "DiscardRectangleEnableEXT";
      case DynamicState::eDiscardRectangleModeEXT: return "DiscardRectangleModeEXT";
      case DynamicState::eSampleLocationsEXT: return "SampleLocationsEXT";
      case DynamicState::eFragmentShadingRateKHR: return "FragmentShadingRateKHR";
      case DynamicState::eVertexInputEXT: return "VertexInputEXT";
      case DynamicState::ePatchControlPointsEXT: return "PatchControlPointsEXT";
      case DynamicState::eLogicOpEXT: return "LogicOpEXT";
      case DynamicState::eColorWriteEnableEXT: return "ColorWriteEnableEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FrontFace value )
  {
    switch ( value )
    {
      case FrontFace::eCounterClockwise: return "CounterClockwise";
      case FrontFace::eClockwise: return "Clockwise";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( LogicOp value )
  {
    switch ( value )
    {
      case LogicOp::eClear: return "Clear";
      case LogicOp::eAnd: return "And";
      case LogicOp::eAndReverse: return "AndReverse";
      case LogicOp::eCopy: return "Copy";
      case LogicOp::eAndInverted: return "AndInverted";
      case LogicOp::eNoOp: return "NoOp";
      case LogicOp::eXor: return "Xor";
      case LogicOp::eOr: return "Or";
      case LogicOp::eNor: return "Nor";
      case LogicOp::eEquivalent: return "Equivalent";
      case LogicOp::eInvert: return "Invert";
      case LogicOp::eOrReverse: return "OrReverse";
      case LogicOp::eCopyInverted: return "CopyInverted";
      case LogicOp::eOrInverted: return "OrInverted";
      case LogicOp::eNand: return "Nand";
      case LogicOp::eSet: return "Set";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCreateFlagBits value )
  {
    switch ( value )
    {
      case PipelineCreateFlagBits::eDisableOptimization: return "DisableOptimization";
      case PipelineCreateFlagBits::eViewIndexFromDeviceIndex: return "ViewIndexFromDeviceIndex";
      case PipelineCreateFlagBits::eDispatchBase: return "DispatchBase";
      case PipelineCreateFlagBits::eFailOnPipelineCompileRequired: return "FailOnPipelineCompileRequired";
      case PipelineCreateFlagBits::eEarlyReturnOnFailure: return "EarlyReturnOnFailure";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineShaderStageCreateFlagBits value )
  {
    switch ( value )
    {
      case PipelineShaderStageCreateFlagBits::eAllowVaryingSubgroupSize: return "AllowVaryingSubgroupSize";
      case PipelineShaderStageCreateFlagBits::eRequireFullSubgroups: return "RequireFullSubgroups";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PolygonMode value )
  {
    switch ( value )
    {
      case PolygonMode::eFill: return "Fill";
      case PolygonMode::eLine: return "Line";
      case PolygonMode::ePoint: return "Point";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PrimitiveTopology value )
  {
    switch ( value )
    {
      case PrimitiveTopology::ePointList: return "PointList";
      case PrimitiveTopology::eLineList: return "LineList";
      case PrimitiveTopology::eLineStrip: return "LineStrip";
      case PrimitiveTopology::eTriangleList: return "TriangleList";
      case PrimitiveTopology::eTriangleStrip: return "TriangleStrip";
      case PrimitiveTopology::eTriangleFan: return "TriangleFan";
      case PrimitiveTopology::eLineListWithAdjacency: return "LineListWithAdjacency";
      case PrimitiveTopology::eLineStripWithAdjacency: return "LineStripWithAdjacency";
      case PrimitiveTopology::eTriangleListWithAdjacency: return "TriangleListWithAdjacency";
      case PrimitiveTopology::eTriangleStripWithAdjacency: return "TriangleStripWithAdjacency";
      case PrimitiveTopology::ePatchList: return "PatchList";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ShaderStageFlagBits value )
  {
    switch ( value )
    {
      case ShaderStageFlagBits::eVertex: return "Vertex";
      case ShaderStageFlagBits::eTessellationControl: return "TessellationControl";
      case ShaderStageFlagBits::eTessellationEvaluation: return "TessellationEvaluation";
      case ShaderStageFlagBits::eGeometry: return "Geometry";
      case ShaderStageFlagBits::eFragment: return "Fragment";
      case ShaderStageFlagBits::eCompute: return "Compute";
      case ShaderStageFlagBits::eAllGraphics: return "AllGraphics";
      case ShaderStageFlagBits::eAll: return "All";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( StencilOp value )
  {
    switch ( value )
    {
      case StencilOp::eKeep: return "Keep";
      case StencilOp::eZero: return "Zero";
      case StencilOp::eReplace: return "Replace";
      case StencilOp::eIncrementAndClamp: return "IncrementAndClamp";
      case StencilOp::eDecrementAndClamp: return "DecrementAndClamp";
      case StencilOp::eInvert: return "Invert";
      case StencilOp::eIncrementAndWrap: return "IncrementAndWrap";
      case StencilOp::eDecrementAndWrap: return "DecrementAndWrap";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( VertexInputRate value )
  {
    switch ( value )
    {
      case VertexInputRate::eVertex: return "Vertex";
      case VertexInputRate::eInstance: return "Instance";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineColorBlendStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineDepthStencilStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineDynamicStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineInputAssemblyStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineLayoutCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineMultisampleStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineTessellationStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineVertexInputStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineViewportStateCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( BorderColor value )
  {
    switch ( value )
    {
      case BorderColor::eFloatTransparentBlack: return "FloatTransparentBlack";
      case BorderColor::eIntTransparentBlack: return "IntTransparentBlack";
      case BorderColor::eFloatOpaqueBlack: return "FloatOpaqueBlack";
      case BorderColor::eIntOpaqueBlack: return "IntOpaqueBlack";
      case BorderColor::eFloatOpaqueWhite: return "FloatOpaqueWhite";
      case BorderColor::eIntOpaqueWhite: return "IntOpaqueWhite";
      case BorderColor::eFloatCustomEXT: return "FloatCustomEXT";
      case BorderColor::eIntCustomEXT: return "IntCustomEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( Filter value )
  {
    switch ( value )
    {
      case Filter::eNearest: return "Nearest";
      case Filter::eLinear: return "Linear";
      case Filter::eCubicEXT: return "CubicEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerAddressMode value )
  {
    switch ( value )
    {
      case SamplerAddressMode::eRepeat: return "Repeat";
      case SamplerAddressMode::eMirroredRepeat: return "MirroredRepeat";
      case SamplerAddressMode::eClampToEdge: return "ClampToEdge";
      case SamplerAddressMode::eClampToBorder: return "ClampToBorder";
      case SamplerAddressMode::eMirrorClampToEdge: return "MirrorClampToEdge";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerMipmapMode value )
  {
    switch ( value )
    {
      case SamplerMipmapMode::eNearest: return "Nearest";
      case SamplerMipmapMode::eLinear: return "Linear";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorPoolCreateFlagBits value )
  {
    switch ( value )
    {
      case DescriptorPoolCreateFlagBits::eFreeDescriptorSet: return "FreeDescriptorSet";
      case DescriptorPoolCreateFlagBits::eUpdateAfterBind: return "UpdateAfterBind";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorSetLayoutCreateFlagBits value )
  {
    switch ( value )
    {
      case DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool: return "UpdateAfterBindPool";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorType value )
  {
    switch ( value )
    {
      case DescriptorType::eSampler: return "Sampler";
      case DescriptorType::eCombinedImageSampler: return "CombinedImageSampler";
      case DescriptorType::eSampledImage: return "SampledImage";
      case DescriptorType::eStorageImage: return "StorageImage";
      case DescriptorType::eUniformTexelBuffer: return "UniformTexelBuffer";
      case DescriptorType::eStorageTexelBuffer: return "StorageTexelBuffer";
      case DescriptorType::eUniformBuffer: return "UniformBuffer";
      case DescriptorType::eStorageBuffer: return "StorageBuffer";
      case DescriptorType::eUniformBufferDynamic: return "UniformBufferDynamic";
      case DescriptorType::eStorageBufferDynamic: return "StorageBufferDynamic";
      case DescriptorType::eInputAttachment: return "InputAttachment";
      case DescriptorType::eInlineUniformBlock: return "InlineUniformBlock";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorPoolResetFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( AccessFlagBits value )
  {
    switch ( value )
    {
      case AccessFlagBits::eIndirectCommandRead: return "IndirectCommandRead";
      case AccessFlagBits::eIndexRead: return "IndexRead";
      case AccessFlagBits::eVertexAttributeRead: return "VertexAttributeRead";
      case AccessFlagBits::eUniformRead: return "UniformRead";
      case AccessFlagBits::eInputAttachmentRead: return "InputAttachmentRead";
      case AccessFlagBits::eShaderRead: return "ShaderRead";
      case AccessFlagBits::eShaderWrite: return "ShaderWrite";
      case AccessFlagBits::eColorAttachmentRead: return "ColorAttachmentRead";
      case AccessFlagBits::eColorAttachmentWrite: return "ColorAttachmentWrite";
      case AccessFlagBits::eDepthStencilAttachmentRead: return "DepthStencilAttachmentRead";
      case AccessFlagBits::eDepthStencilAttachmentWrite: return "DepthStencilAttachmentWrite";
      case AccessFlagBits::eTransferRead: return "TransferRead";
      case AccessFlagBits::eTransferWrite: return "TransferWrite";
      case AccessFlagBits::eHostRead: return "HostRead";
      case AccessFlagBits::eHostWrite: return "HostWrite";
      case AccessFlagBits::eMemoryRead: return "MemoryRead";
      case AccessFlagBits::eMemoryWrite: return "MemoryWrite";
      case AccessFlagBits::eNone: return "None";
      case AccessFlagBits::eColorAttachmentReadNoncoherentEXT: return "ColorAttachmentReadNoncoherentEXT";
      case AccessFlagBits::eFragmentShadingRateAttachmentReadKHR: return "FragmentShadingRateAttachmentReadKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( AttachmentDescriptionFlagBits value )
  {
    switch ( value )
    {
      case AttachmentDescriptionFlagBits::eMayAlias: return "MayAlias";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( AttachmentLoadOp value )
  {
    switch ( value )
    {
      case AttachmentLoadOp::eLoad: return "Load";
      case AttachmentLoadOp::eClear: return "Clear";
      case AttachmentLoadOp::eDontCare: return "DontCare";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( AttachmentStoreOp value )
  {
    switch ( value )
    {
      case AttachmentStoreOp::eStore: return "Store";
      case AttachmentStoreOp::eDontCare: return "DontCare";
      case AttachmentStoreOp::eNone: return "None";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DependencyFlagBits value )
  {
    switch ( value )
    {
      case DependencyFlagBits::eByRegion: return "ByRegion";
      case DependencyFlagBits::eDeviceGroup: return "DeviceGroup";
      case DependencyFlagBits::eViewLocal: return "ViewLocal";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FramebufferCreateFlagBits value )
  {
    switch ( value )
    {
      case FramebufferCreateFlagBits::eImageless: return "Imageless";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineBindPoint value )
  {
    switch ( value )
    {
      case PipelineBindPoint::eGraphics: return "Graphics";
      case PipelineBindPoint::eCompute: return "Compute";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( RenderPassCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( SubpassDescriptionFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandPoolCreateFlagBits value )
  {
    switch ( value )
    {
      case CommandPoolCreateFlagBits::eTransient: return "Transient";
      case CommandPoolCreateFlagBits::eResetCommandBuffer: return "ResetCommandBuffer";
      case CommandPoolCreateFlagBits::eProtected: return "Protected";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CommandPoolResetFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( CommandBufferLevel value )
  {
    switch ( value )
    {
      case CommandBufferLevel::ePrimary: return "Primary";
      case CommandBufferLevel::eSecondary: return "Secondary";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CommandBufferResetFlagBits value )
  {
    switch ( value )
    {
      case CommandBufferResetFlagBits::eReleaseResources: return "ReleaseResources";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CommandBufferUsageFlagBits value )
  {
    switch ( value )
    {
      case CommandBufferUsageFlagBits::eOneTimeSubmit: return "OneTimeSubmit";
      case CommandBufferUsageFlagBits::eRenderPassContinue: return "RenderPassContinue";
      case CommandBufferUsageFlagBits::eSimultaneousUse: return "SimultaneousUse";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( QueryControlFlagBits value )
  {
    switch ( value )
    {
      case QueryControlFlagBits::ePrecise: return "Precise";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( IndexType value )
  {
    switch ( value )
    {
      case IndexType::eUint16: return "Uint16";
      case IndexType::eUint32: return "Uint32";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( StencilFaceFlagBits value )
  {
    switch ( value )
    {
      case StencilFaceFlagBits::eFront: return "Front";
      case StencilFaceFlagBits::eBack: return "Back";
      case StencilFaceFlagBits::eFrontAndBack: return "FrontAndBack";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SubpassContents value )
  {
    switch ( value )
    {
      case SubpassContents::eInline: return "Inline";
      case SubpassContents::eSecondaryCommandBuffers: return "SecondaryCommandBuffers";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_VERSION_1_1 ===

  VULKAN_HPP_INLINE std::string to_string( SubgroupFeatureFlagBits value )
  {
    switch ( value )
    {
      case SubgroupFeatureFlagBits::eBasic: return "Basic";
      case SubgroupFeatureFlagBits::eVote: return "Vote";
      case SubgroupFeatureFlagBits::eArithmetic: return "Arithmetic";
      case SubgroupFeatureFlagBits::eBallot: return "Ballot";
      case SubgroupFeatureFlagBits::eShuffle: return "Shuffle";
      case SubgroupFeatureFlagBits::eShuffleRelative: return "ShuffleRelative";
      case SubgroupFeatureFlagBits::eClustered: return "Clustered";
      case SubgroupFeatureFlagBits::eQuad: return "Quad";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PeerMemoryFeatureFlagBits value )
  {
    switch ( value )
    {
      case PeerMemoryFeatureFlagBits::eCopySrc: return "CopySrc";
      case PeerMemoryFeatureFlagBits::eCopyDst: return "CopyDst";
      case PeerMemoryFeatureFlagBits::eGenericSrc: return "GenericSrc";
      case PeerMemoryFeatureFlagBits::eGenericDst: return "GenericDst";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( MemoryAllocateFlagBits value )
  {
    switch ( value )
    {
      case MemoryAllocateFlagBits::eDeviceMask: return "DeviceMask";
      case MemoryAllocateFlagBits::eDeviceAddress: return "DeviceAddress";
      case MemoryAllocateFlagBits::eDeviceAddressCaptureReplay: return "DeviceAddressCaptureReplay";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PointClippingBehavior value )
  {
    switch ( value )
    {
      case PointClippingBehavior::eAllClipPlanes: return "AllClipPlanes";
      case PointClippingBehavior::eUserClipPlanesOnly: return "UserClipPlanesOnly";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( TessellationDomainOrigin value )
  {
    switch ( value )
    {
      case TessellationDomainOrigin::eUpperLeft: return "UpperLeft";
      case TessellationDomainOrigin::eLowerLeft: return "LowerLeft";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerYcbcrModelConversion value )
  {
    switch ( value )
    {
      case SamplerYcbcrModelConversion::eRgbIdentity: return "RgbIdentity";
      case SamplerYcbcrModelConversion::eYcbcrIdentity: return "YcbcrIdentity";
      case SamplerYcbcrModelConversion::eYcbcr709: return "Ycbcr709";
      case SamplerYcbcrModelConversion::eYcbcr601: return "Ycbcr601";
      case SamplerYcbcrModelConversion::eYcbcr2020: return "Ycbcr2020";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerYcbcrRange value )
  {
    switch ( value )
    {
      case SamplerYcbcrRange::eItuFull: return "ItuFull";
      case SamplerYcbcrRange::eItuNarrow: return "ItuNarrow";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ChromaLocation value )
  {
    switch ( value )
    {
      case ChromaLocation::eCositedEven: return "CositedEven";
      case ChromaLocation::eMidpoint: return "Midpoint";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalMemoryHandleTypeFlagBits value )
  {
    switch ( value )
    {
      case ExternalMemoryHandleTypeFlagBits::eOpaqueFd: return "OpaqueFd";
      case ExternalMemoryHandleTypeFlagBits::eOpaqueWin32: return "OpaqueWin32";
      case ExternalMemoryHandleTypeFlagBits::eOpaqueWin32Kmt: return "OpaqueWin32Kmt";
      case ExternalMemoryHandleTypeFlagBits::eD3D11Texture: return "D3D11Texture";
      case ExternalMemoryHandleTypeFlagBits::eD3D11TextureKmt: return "D3D11TextureKmt";
      case ExternalMemoryHandleTypeFlagBits::eD3D12Heap: return "D3D12Heap";
      case ExternalMemoryHandleTypeFlagBits::eD3D12Resource: return "D3D12Resource";
      case ExternalMemoryHandleTypeFlagBits::eDmaBufEXT: return "DmaBufEXT";
      case ExternalMemoryHandleTypeFlagBits::eHostAllocationEXT: return "HostAllocationEXT";
      case ExternalMemoryHandleTypeFlagBits::eHostMappedForeignMemoryEXT: return "HostMappedForeignMemoryEXT";
#if defined( VK_USE_PLATFORM_SCI )
      case ExternalMemoryHandleTypeFlagBits::eSciBufNV: return "SciBufNV";
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      case ExternalMemoryHandleTypeFlagBits::eScreenBufferQNX: return "ScreenBufferQNX";
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalMemoryFeatureFlagBits value )
  {
    switch ( value )
    {
      case ExternalMemoryFeatureFlagBits::eDedicatedOnly: return "DedicatedOnly";
      case ExternalMemoryFeatureFlagBits::eExportable: return "Exportable";
      case ExternalMemoryFeatureFlagBits::eImportable: return "Importable";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalFenceHandleTypeFlagBits value )
  {
    switch ( value )
    {
      case ExternalFenceHandleTypeFlagBits::eOpaqueFd: return "OpaqueFd";
      case ExternalFenceHandleTypeFlagBits::eOpaqueWin32: return "OpaqueWin32";
      case ExternalFenceHandleTypeFlagBits::eOpaqueWin32Kmt: return "OpaqueWin32Kmt";
      case ExternalFenceHandleTypeFlagBits::eSyncFd: return "SyncFd";
#if defined( VK_USE_PLATFORM_SCI )
      case ExternalFenceHandleTypeFlagBits::eSciSyncObjNV: return "SciSyncObjNV";
      case ExternalFenceHandleTypeFlagBits::eSciSyncFenceNV: return "SciSyncFenceNV";
#endif /*VK_USE_PLATFORM_SCI*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalFenceFeatureFlagBits value )
  {
    switch ( value )
    {
      case ExternalFenceFeatureFlagBits::eExportable: return "Exportable";
      case ExternalFenceFeatureFlagBits::eImportable: return "Importable";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FenceImportFlagBits value )
  {
    switch ( value )
    {
      case FenceImportFlagBits::eTemporary: return "Temporary";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreImportFlagBits value )
  {
    switch ( value )
    {
      case SemaphoreImportFlagBits::eTemporary: return "Temporary";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalSemaphoreHandleTypeFlagBits value )
  {
    switch ( value )
    {
      case ExternalSemaphoreHandleTypeFlagBits::eOpaqueFd: return "OpaqueFd";
      case ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32: return "OpaqueWin32";
      case ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32Kmt: return "OpaqueWin32Kmt";
      case ExternalSemaphoreHandleTypeFlagBits::eD3D12Fence: return "D3D12Fence";
      case ExternalSemaphoreHandleTypeFlagBits::eSyncFd: return "SyncFd";
#if defined( VK_USE_PLATFORM_SCI )
      case ExternalSemaphoreHandleTypeFlagBits::eSciSyncObjNV: return "SciSyncObjNV";
#endif /*VK_USE_PLATFORM_SCI*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ExternalSemaphoreFeatureFlagBits value )
  {
    switch ( value )
    {
      case ExternalSemaphoreFeatureFlagBits::eExportable: return "Exportable";
      case ExternalSemaphoreFeatureFlagBits::eImportable: return "Importable";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_VERSION_1_2 ===

  VULKAN_HPP_INLINE std::string to_string( DriverId value )
  {
    switch ( value )
    {
      case DriverId::eAmdProprietary: return "AmdProprietary";
      case DriverId::eAmdOpenSource: return "AmdOpenSource";
      case DriverId::eMesaRadv: return "MesaRadv";
      case DriverId::eNvidiaProprietary: return "NvidiaProprietary";
      case DriverId::eIntelProprietaryWindows: return "IntelProprietaryWindows";
      case DriverId::eIntelOpenSourceMESA: return "IntelOpenSourceMESA";
      case DriverId::eImaginationProprietary: return "ImaginationProprietary";
      case DriverId::eQualcommProprietary: return "QualcommProprietary";
      case DriverId::eArmProprietary: return "ArmProprietary";
      case DriverId::eGoogleSwiftshader: return "GoogleSwiftshader";
      case DriverId::eGgpProprietary: return "GgpProprietary";
      case DriverId::eBroadcomProprietary: return "BroadcomProprietary";
      case DriverId::eMesaLlvmpipe: return "MesaLlvmpipe";
      case DriverId::eMoltenvk: return "Moltenvk";
      case DriverId::eCoreaviProprietary: return "CoreaviProprietary";
      case DriverId::eJuiceProprietary: return "JuiceProprietary";
      case DriverId::eVerisiliconProprietary: return "VerisiliconProprietary";
      case DriverId::eMesaTurnip: return "MesaTurnip";
      case DriverId::eMesaV3Dv: return "MesaV3Dv";
      case DriverId::eMesaPanvk: return "MesaPanvk";
      case DriverId::eSamsungProprietary: return "SamsungProprietary";
      case DriverId::eMesaVenus: return "MesaVenus";
      case DriverId::eMesaDozen: return "MesaDozen";
      case DriverId::eMesaNvk: return "MesaNvk";
      case DriverId::eImaginationOpenSourceMESA: return "ImaginationOpenSourceMESA";
      case DriverId::eMesaAgxv: return "MesaAgxv";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ShaderFloatControlsIndependence value )
  {
    switch ( value )
    {
      case ShaderFloatControlsIndependence::e32BitOnly: return "32BitOnly";
      case ShaderFloatControlsIndependence::eAll: return "All";
      case ShaderFloatControlsIndependence::eNone: return "None";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DescriptorBindingFlagBits value )
  {
    switch ( value )
    {
      case DescriptorBindingFlagBits::eUpdateAfterBind: return "UpdateAfterBind";
      case DescriptorBindingFlagBits::eUpdateUnusedWhilePending: return "UpdateUnusedWhilePending";
      case DescriptorBindingFlagBits::ePartiallyBound: return "PartiallyBound";
      case DescriptorBindingFlagBits::eVariableDescriptorCount: return "VariableDescriptorCount";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ResolveModeFlagBits value )
  {
    switch ( value )
    {
      case ResolveModeFlagBits::eNone: return "None";
      case ResolveModeFlagBits::eSampleZero: return "SampleZero";
      case ResolveModeFlagBits::eAverage: return "Average";
      case ResolveModeFlagBits::eMin: return "Min";
      case ResolveModeFlagBits::eMax: return "Max";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SamplerReductionMode value )
  {
    switch ( value )
    {
      case SamplerReductionMode::eWeightedAverage: return "WeightedAverage";
      case SamplerReductionMode::eMin: return "Min";
      case SamplerReductionMode::eMax: return "Max";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreType value )
  {
    switch ( value )
    {
      case SemaphoreType::eBinary: return "Binary";
      case SemaphoreType::eTimeline: return "Timeline";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SemaphoreWaitFlagBits value )
  {
    switch ( value )
    {
      case SemaphoreWaitFlagBits::eAny: return "Any";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_VERSION_1_3 ===

  VULKAN_HPP_INLINE std::string to_string( PipelineCreationFeedbackFlagBits value )
  {
    switch ( value )
    {
      case PipelineCreationFeedbackFlagBits::eValid: return "Valid";
      case PipelineCreationFeedbackFlagBits::eApplicationPipelineCacheHit: return "ApplicationPipelineCacheHit";
      case PipelineCreationFeedbackFlagBits::eBasePipelineAcceleration: return "BasePipelineAcceleration";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ToolPurposeFlagBits value )
  {
    switch ( value )
    {
      case ToolPurposeFlagBits::eValidation: return "Validation";
      case ToolPurposeFlagBits::eProfiling: return "Profiling";
      case ToolPurposeFlagBits::eTracing: return "Tracing";
      case ToolPurposeFlagBits::eAdditionalFeatures: return "AdditionalFeatures";
      case ToolPurposeFlagBits::eModifyingFeatures: return "ModifyingFeatures";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PrivateDataSlotCreateFlagBits )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineStageFlagBits2 value )
  {
    switch ( value )
    {
      case PipelineStageFlagBits2::eNone: return "None";
      case PipelineStageFlagBits2::eTopOfPipe: return "TopOfPipe";
      case PipelineStageFlagBits2::eDrawIndirect: return "DrawIndirect";
      case PipelineStageFlagBits2::eVertexInput: return "VertexInput";
      case PipelineStageFlagBits2::eVertexShader: return "VertexShader";
      case PipelineStageFlagBits2::eTessellationControlShader: return "TessellationControlShader";
      case PipelineStageFlagBits2::eTessellationEvaluationShader: return "TessellationEvaluationShader";
      case PipelineStageFlagBits2::eGeometryShader: return "GeometryShader";
      case PipelineStageFlagBits2::eFragmentShader: return "FragmentShader";
      case PipelineStageFlagBits2::eEarlyFragmentTests: return "EarlyFragmentTests";
      case PipelineStageFlagBits2::eLateFragmentTests: return "LateFragmentTests";
      case PipelineStageFlagBits2::eColorAttachmentOutput: return "ColorAttachmentOutput";
      case PipelineStageFlagBits2::eComputeShader: return "ComputeShader";
      case PipelineStageFlagBits2::eAllTransfer: return "AllTransfer";
      case PipelineStageFlagBits2::eBottomOfPipe: return "BottomOfPipe";
      case PipelineStageFlagBits2::eHost: return "Host";
      case PipelineStageFlagBits2::eAllGraphics: return "AllGraphics";
      case PipelineStageFlagBits2::eAllCommands: return "AllCommands";
      case PipelineStageFlagBits2::eCopy: return "Copy";
      case PipelineStageFlagBits2::eResolve: return "Resolve";
      case PipelineStageFlagBits2::eBlit: return "Blit";
      case PipelineStageFlagBits2::eClear: return "Clear";
      case PipelineStageFlagBits2::eIndexInput: return "IndexInput";
      case PipelineStageFlagBits2::eVertexAttributeInput: return "VertexAttributeInput";
      case PipelineStageFlagBits2::ePreRasterizationShaders: return "PreRasterizationShaders";
      case PipelineStageFlagBits2::eTransformFeedbackEXT: return "TransformFeedbackEXT";
      case PipelineStageFlagBits2::eConditionalRenderingEXT: return "ConditionalRenderingEXT";
      case PipelineStageFlagBits2::eCommandPreprocessNV: return "CommandPreprocessNV";
      case PipelineStageFlagBits2::eFragmentShadingRateAttachmentKHR: return "FragmentShadingRateAttachmentKHR";
      case PipelineStageFlagBits2::eAccelerationStructureBuildKHR: return "AccelerationStructureBuildKHR";
      case PipelineStageFlagBits2::eRayTracingShaderKHR: return "RayTracingShaderKHR";
      case PipelineStageFlagBits2::eFragmentDensityProcessEXT: return "FragmentDensityProcessEXT";
      case PipelineStageFlagBits2::eTaskShaderEXT: return "TaskShaderEXT";
      case PipelineStageFlagBits2::eMeshShaderEXT: return "MeshShaderEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( AccessFlagBits2 value )
  {
    switch ( value )
    {
      case AccessFlagBits2::eNone: return "None";
      case AccessFlagBits2::eIndirectCommandRead: return "IndirectCommandRead";
      case AccessFlagBits2::eIndexRead: return "IndexRead";
      case AccessFlagBits2::eVertexAttributeRead: return "VertexAttributeRead";
      case AccessFlagBits2::eUniformRead: return "UniformRead";
      case AccessFlagBits2::eInputAttachmentRead: return "InputAttachmentRead";
      case AccessFlagBits2::eShaderRead: return "ShaderRead";
      case AccessFlagBits2::eShaderWrite: return "ShaderWrite";
      case AccessFlagBits2::eColorAttachmentRead: return "ColorAttachmentRead";
      case AccessFlagBits2::eColorAttachmentWrite: return "ColorAttachmentWrite";
      case AccessFlagBits2::eDepthStencilAttachmentRead: return "DepthStencilAttachmentRead";
      case AccessFlagBits2::eDepthStencilAttachmentWrite: return "DepthStencilAttachmentWrite";
      case AccessFlagBits2::eTransferRead: return "TransferRead";
      case AccessFlagBits2::eTransferWrite: return "TransferWrite";
      case AccessFlagBits2::eHostRead: return "HostRead";
      case AccessFlagBits2::eHostWrite: return "HostWrite";
      case AccessFlagBits2::eMemoryRead: return "MemoryRead";
      case AccessFlagBits2::eMemoryWrite: return "MemoryWrite";
      case AccessFlagBits2::eShaderSampledRead: return "ShaderSampledRead";
      case AccessFlagBits2::eShaderStorageRead: return "ShaderStorageRead";
      case AccessFlagBits2::eShaderStorageWrite: return "ShaderStorageWrite";
      case AccessFlagBits2::eTransformFeedbackWriteEXT: return "TransformFeedbackWriteEXT";
      case AccessFlagBits2::eTransformFeedbackCounterReadEXT: return "TransformFeedbackCounterReadEXT";
      case AccessFlagBits2::eTransformFeedbackCounterWriteEXT: return "TransformFeedbackCounterWriteEXT";
      case AccessFlagBits2::eConditionalRenderingReadEXT: return "ConditionalRenderingReadEXT";
      case AccessFlagBits2::eCommandPreprocessReadNV: return "CommandPreprocessReadNV";
      case AccessFlagBits2::eCommandPreprocessWriteNV: return "CommandPreprocessWriteNV";
      case AccessFlagBits2::eFragmentShadingRateAttachmentReadKHR: return "FragmentShadingRateAttachmentReadKHR";
      case AccessFlagBits2::eAccelerationStructureReadKHR: return "AccelerationStructureReadKHR";
      case AccessFlagBits2::eAccelerationStructureWriteKHR: return "AccelerationStructureWriteKHR";
      case AccessFlagBits2::eFragmentDensityMapReadEXT: return "FragmentDensityMapReadEXT";
      case AccessFlagBits2::eColorAttachmentReadNoncoherentEXT: return "ColorAttachmentReadNoncoherentEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SubmitFlagBits value )
  {
    switch ( value )
    {
      case SubmitFlagBits::eProtected: return "Protected";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( RenderingFlagBits value )
  {
    switch ( value )
    {
      case RenderingFlagBits::eContentsSecondaryCommandBuffers: return "ContentsSecondaryCommandBuffers";
      case RenderingFlagBits::eSuspending: return "Suspending";
      case RenderingFlagBits::eResuming: return "Resuming";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FormatFeatureFlagBits2 value )
  {
    switch ( value )
    {
      case FormatFeatureFlagBits2::eSampledImage: return "SampledImage";
      case FormatFeatureFlagBits2::eStorageImage: return "StorageImage";
      case FormatFeatureFlagBits2::eStorageImageAtomic: return "StorageImageAtomic";
      case FormatFeatureFlagBits2::eUniformTexelBuffer: return "UniformTexelBuffer";
      case FormatFeatureFlagBits2::eStorageTexelBuffer: return "StorageTexelBuffer";
      case FormatFeatureFlagBits2::eStorageTexelBufferAtomic: return "StorageTexelBufferAtomic";
      case FormatFeatureFlagBits2::eVertexBuffer: return "VertexBuffer";
      case FormatFeatureFlagBits2::eColorAttachment: return "ColorAttachment";
      case FormatFeatureFlagBits2::eColorAttachmentBlend: return "ColorAttachmentBlend";
      case FormatFeatureFlagBits2::eDepthStencilAttachment: return "DepthStencilAttachment";
      case FormatFeatureFlagBits2::eBlitSrc: return "BlitSrc";
      case FormatFeatureFlagBits2::eBlitDst: return "BlitDst";
      case FormatFeatureFlagBits2::eSampledImageFilterLinear: return "SampledImageFilterLinear";
      case FormatFeatureFlagBits2::eSampledImageFilterCubic: return "SampledImageFilterCubic";
      case FormatFeatureFlagBits2::eTransferSrc: return "TransferSrc";
      case FormatFeatureFlagBits2::eTransferDst: return "TransferDst";
      case FormatFeatureFlagBits2::eSampledImageFilterMinmax: return "SampledImageFilterMinmax";
      case FormatFeatureFlagBits2::eMidpointChromaSamples: return "MidpointChromaSamples";
      case FormatFeatureFlagBits2::eSampledImageYcbcrConversionLinearFilter: return "SampledImageYcbcrConversionLinearFilter";
      case FormatFeatureFlagBits2::eSampledImageYcbcrConversionSeparateReconstructionFilter: return "SampledImageYcbcrConversionSeparateReconstructionFilter";
      case FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicit: return "SampledImageYcbcrConversionChromaReconstructionExplicit";
      case FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable:
        return "SampledImageYcbcrConversionChromaReconstructionExplicitForceable";
      case FormatFeatureFlagBits2::eDisjoint: return "Disjoint";
      case FormatFeatureFlagBits2::eCositedChromaSamples: return "CositedChromaSamples";
      case FormatFeatureFlagBits2::eStorageReadWithoutFormat: return "StorageReadWithoutFormat";
      case FormatFeatureFlagBits2::eStorageWriteWithoutFormat: return "StorageWriteWithoutFormat";
      case FormatFeatureFlagBits2::eSampledImageDepthComparison: return "SampledImageDepthComparison";
      case FormatFeatureFlagBits2::eFragmentShadingRateAttachmentKHR: return "FragmentShadingRateAttachmentKHR";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VKSC_VERSION_1_0 ===

  VULKAN_HPP_INLINE std::string to_string( FaultLevel value )
  {
    switch ( value )
    {
      case FaultLevel::eUnassigned: return "Unassigned";
      case FaultLevel::eCritical: return "Critical";
      case FaultLevel::eRecoverable: return "Recoverable";
      case FaultLevel::eWarning: return "Warning";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FaultType value )
  {
    switch ( value )
    {
      case FaultType::eInvalid: return "Invalid";
      case FaultType::eUnassigned: return "Unassigned";
      case FaultType::eImplementation: return "Implementation";
      case FaultType::eSystem: return "System";
      case FaultType::ePhysicalDevice: return "PhysicalDevice";
      case FaultType::eCommandBufferFull: return "CommandBufferFull";
      case FaultType::eInvalidApiUsage: return "InvalidApiUsage";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( FaultQueryBehavior value )
  {
    switch ( value )
    {
      case FaultQueryBehavior::eGetAndClearAllFaults: return "GetAndClearAllFaults";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineMatchControl value )
  {
    switch ( value )
    {
      case PipelineMatchControl::eApplicationUuidExactMatch: return "ApplicationUuidExactMatch";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineCacheValidationVersion value )
  {
    switch ( value )
    {
      case PipelineCacheValidationVersion::eSafetyCriticalOne: return "SafetyCriticalOne";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_KHR_surface ===

  VULKAN_HPP_INLINE std::string to_string( SurfaceTransformFlagBitsKHR value )
  {
    switch ( value )
    {
      case SurfaceTransformFlagBitsKHR::eIdentity: return "Identity";
      case SurfaceTransformFlagBitsKHR::eRotate90: return "Rotate90";
      case SurfaceTransformFlagBitsKHR::eRotate180: return "Rotate180";
      case SurfaceTransformFlagBitsKHR::eRotate270: return "Rotate270";
      case SurfaceTransformFlagBitsKHR::eHorizontalMirror: return "HorizontalMirror";
      case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90: return "HorizontalMirrorRotate90";
      case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180: return "HorizontalMirrorRotate180";
      case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270: return "HorizontalMirrorRotate270";
      case SurfaceTransformFlagBitsKHR::eInherit: return "Inherit";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PresentModeKHR value )
  {
    switch ( value )
    {
      case PresentModeKHR::eImmediate: return "Immediate";
      case PresentModeKHR::eMailbox: return "Mailbox";
      case PresentModeKHR::eFifo: return "Fifo";
      case PresentModeKHR::eFifoRelaxed: return "FifoRelaxed";
      case PresentModeKHR::eSharedDemandRefresh: return "SharedDemandRefresh";
      case PresentModeKHR::eSharedContinuousRefresh: return "SharedContinuousRefresh";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ColorSpaceKHR value )
  {
    switch ( value )
    {
      case ColorSpaceKHR::eSrgbNonlinear: return "SrgbNonlinear";
      case ColorSpaceKHR::eDisplayP3NonlinearEXT: return "DisplayP3NonlinearEXT";
      case ColorSpaceKHR::eExtendedSrgbLinearEXT: return "ExtendedSrgbLinearEXT";
      case ColorSpaceKHR::eDisplayP3LinearEXT: return "DisplayP3LinearEXT";
      case ColorSpaceKHR::eDciP3NonlinearEXT: return "DciP3NonlinearEXT";
      case ColorSpaceKHR::eBt709LinearEXT: return "Bt709LinearEXT";
      case ColorSpaceKHR::eBt709NonlinearEXT: return "Bt709NonlinearEXT";
      case ColorSpaceKHR::eBt2020LinearEXT: return "Bt2020LinearEXT";
      case ColorSpaceKHR::eHdr10St2084EXT: return "Hdr10St2084EXT";
      case ColorSpaceKHR::eDolbyvisionEXT: return "DolbyvisionEXT";
      case ColorSpaceKHR::eHdr10HlgEXT: return "Hdr10HlgEXT";
      case ColorSpaceKHR::eAdobergbLinearEXT: return "AdobergbLinearEXT";
      case ColorSpaceKHR::eAdobergbNonlinearEXT: return "AdobergbNonlinearEXT";
      case ColorSpaceKHR::ePassThroughEXT: return "PassThroughEXT";
      case ColorSpaceKHR::eExtendedSrgbNonlinearEXT: return "ExtendedSrgbNonlinearEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( CompositeAlphaFlagBitsKHR value )
  {
    switch ( value )
    {
      case CompositeAlphaFlagBitsKHR::eOpaque: return "Opaque";
      case CompositeAlphaFlagBitsKHR::ePreMultiplied: return "PreMultiplied";
      case CompositeAlphaFlagBitsKHR::ePostMultiplied: return "PostMultiplied";
      case CompositeAlphaFlagBitsKHR::eInherit: return "Inherit";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_KHR_swapchain ===

  VULKAN_HPP_INLINE std::string to_string( SwapchainCreateFlagBitsKHR value )
  {
    switch ( value )
    {
      case SwapchainCreateFlagBitsKHR::eSplitInstanceBindRegions: return "SplitInstanceBindRegions";
      case SwapchainCreateFlagBitsKHR::eProtected: return "Protected";
      case SwapchainCreateFlagBitsKHR::eMutableFormat: return "MutableFormat";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceGroupPresentModeFlagBitsKHR value )
  {
    switch ( value )
    {
      case DeviceGroupPresentModeFlagBitsKHR::eLocal: return "Local";
      case DeviceGroupPresentModeFlagBitsKHR::eRemote: return "Remote";
      case DeviceGroupPresentModeFlagBitsKHR::eSum: return "Sum";
      case DeviceGroupPresentModeFlagBitsKHR::eLocalMultiDevice: return "LocalMultiDevice";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_KHR_display ===

  VULKAN_HPP_INLINE std::string to_string( DisplayPlaneAlphaFlagBitsKHR value )
  {
    switch ( value )
    {
      case DisplayPlaneAlphaFlagBitsKHR::eOpaque: return "Opaque";
      case DisplayPlaneAlphaFlagBitsKHR::eGlobal: return "Global";
      case DisplayPlaneAlphaFlagBitsKHR::ePerPixel: return "PerPixel";
      case DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied: return "PerPixelPremultiplied";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DisplayModeCreateFlagBitsKHR )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( DisplaySurfaceCreateFlagBitsKHR )
  {
    return "(void)";
  }

  //=== VK_EXT_display_surface_counter ===

  VULKAN_HPP_INLINE std::string to_string( SurfaceCounterFlagBitsEXT value )
  {
    switch ( value )
    {
      case SurfaceCounterFlagBitsEXT::eVblank: return "Vblank";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_EXT_display_control ===

  VULKAN_HPP_INLINE std::string to_string( DisplayPowerStateEXT value )
  {
    switch ( value )
    {
      case DisplayPowerStateEXT::eOff: return "Off";
      case DisplayPowerStateEXT::eSuspend: return "Suspend";
      case DisplayPowerStateEXT::eOn: return "On";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DeviceEventTypeEXT value )
  {
    switch ( value )
    {
      case DeviceEventTypeEXT::eDisplayHotplug: return "DisplayHotplug";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DisplayEventTypeEXT value )
  {
    switch ( value )
    {
      case DisplayEventTypeEXT::eFirstPixelOut: return "FirstPixelOut";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_EXT_discard_rectangles ===

  VULKAN_HPP_INLINE std::string to_string( DiscardRectangleModeEXT value )
  {
    switch ( value )
    {
      case DiscardRectangleModeEXT::eInclusive: return "Inclusive";
      case DiscardRectangleModeEXT::eExclusive: return "Exclusive";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineDiscardRectangleStateCreateFlagBitsEXT )
  {
    return "(void)";
  }

  //=== VK_EXT_conservative_rasterization ===

  VULKAN_HPP_INLINE std::string to_string( ConservativeRasterizationModeEXT value )
  {
    switch ( value )
    {
      case ConservativeRasterizationModeEXT::eDisabled: return "Disabled";
      case ConservativeRasterizationModeEXT::eOverestimate: return "Overestimate";
      case ConservativeRasterizationModeEXT::eUnderestimate: return "Underestimate";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationConservativeStateCreateFlagBitsEXT )
  {
    return "(void)";
  }

  //=== VK_EXT_depth_clip_enable ===

  VULKAN_HPP_INLINE std::string to_string( PipelineRasterizationDepthClipStateCreateFlagBitsEXT )
  {
    return "(void)";
  }

  //=== VK_KHR_performance_query ===

  VULKAN_HPP_INLINE std::string to_string( PerformanceCounterDescriptionFlagBitsKHR value )
  {
    switch ( value )
    {
      case PerformanceCounterDescriptionFlagBitsKHR::ePerformanceImpacting: return "PerformanceImpacting";
      case PerformanceCounterDescriptionFlagBitsKHR::eConcurrentlyImpacted: return "ConcurrentlyImpacted";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PerformanceCounterScopeKHR value )
  {
    switch ( value )
    {
      case PerformanceCounterScopeKHR::eCommandBuffer: return "CommandBuffer";
      case PerformanceCounterScopeKHR::eRenderPass: return "RenderPass";
      case PerformanceCounterScopeKHR::eCommand: return "Command";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PerformanceCounterStorageKHR value )
  {
    switch ( value )
    {
      case PerformanceCounterStorageKHR::eInt32: return "Int32";
      case PerformanceCounterStorageKHR::eInt64: return "Int64";
      case PerformanceCounterStorageKHR::eUint32: return "Uint32";
      case PerformanceCounterStorageKHR::eUint64: return "Uint64";
      case PerformanceCounterStorageKHR::eFloat32: return "Float32";
      case PerformanceCounterStorageKHR::eFloat64: return "Float64";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( PerformanceCounterUnitKHR value )
  {
    switch ( value )
    {
      case PerformanceCounterUnitKHR::eGeneric: return "Generic";
      case PerformanceCounterUnitKHR::ePercentage: return "Percentage";
      case PerformanceCounterUnitKHR::eNanoseconds: return "Nanoseconds";
      case PerformanceCounterUnitKHR::eBytes: return "Bytes";
      case PerformanceCounterUnitKHR::eBytesPerSecond: return "BytesPerSecond";
      case PerformanceCounterUnitKHR::eKelvin: return "Kelvin";
      case PerformanceCounterUnitKHR::eWatts: return "Watts";
      case PerformanceCounterUnitKHR::eVolts: return "Volts";
      case PerformanceCounterUnitKHR::eAmps: return "Amps";
      case PerformanceCounterUnitKHR::eHertz: return "Hertz";
      case PerformanceCounterUnitKHR::eCycles: return "Cycles";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( AcquireProfilingLockFlagBitsKHR )
  {
    return "(void)";
  }

  //=== VK_EXT_debug_utils ===

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessageSeverityFlagBitsEXT value )
  {
    switch ( value )
    {
      case DebugUtilsMessageSeverityFlagBitsEXT::eVerbose: return "Verbose";
      case DebugUtilsMessageSeverityFlagBitsEXT::eInfo: return "Info";
      case DebugUtilsMessageSeverityFlagBitsEXT::eWarning: return "Warning";
      case DebugUtilsMessageSeverityFlagBitsEXT::eError: return "Error";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessageTypeFlagBitsEXT value )
  {
    switch ( value )
    {
      case DebugUtilsMessageTypeFlagBitsEXT::eGeneral: return "General";
      case DebugUtilsMessageTypeFlagBitsEXT::eValidation: return "Validation";
      case DebugUtilsMessageTypeFlagBitsEXT::ePerformance: return "Performance";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessengerCallbackDataFlagBitsEXT )
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string( DebugUtilsMessengerCreateFlagBitsEXT )
  {
    return "(void)";
  }

  //=== VK_EXT_blend_operation_advanced ===

  VULKAN_HPP_INLINE std::string to_string( BlendOverlapEXT value )
  {
    switch ( value )
    {
      case BlendOverlapEXT::eUncorrelated: return "Uncorrelated";
      case BlendOverlapEXT::eDisjoint: return "Disjoint";
      case BlendOverlapEXT::eConjoint: return "Conjoint";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_KHR_fragment_shading_rate ===

  VULKAN_HPP_INLINE std::string to_string( FragmentShadingRateCombinerOpKHR value )
  {
    switch ( value )
    {
      case FragmentShadingRateCombinerOpKHR::eKeep: return "Keep";
      case FragmentShadingRateCombinerOpKHR::eReplace: return "Replace";
      case FragmentShadingRateCombinerOpKHR::eMin: return "Min";
      case FragmentShadingRateCombinerOpKHR::eMax: return "Max";
      case FragmentShadingRateCombinerOpKHR::eMul: return "Mul";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_EXT_validation_features ===

  VULKAN_HPP_INLINE std::string to_string( ValidationFeatureEnableEXT value )
  {
    switch ( value )
    {
      case ValidationFeatureEnableEXT::eGpuAssisted: return "GpuAssisted";
      case ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot: return "GpuAssistedReserveBindingSlot";
      case ValidationFeatureEnableEXT::eBestPractices: return "BestPractices";
      case ValidationFeatureEnableEXT::eDebugPrintf: return "DebugPrintf";
      case ValidationFeatureEnableEXT::eSynchronizationValidation: return "SynchronizationValidation";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( ValidationFeatureDisableEXT value )
  {
    switch ( value )
    {
      case ValidationFeatureDisableEXT::eAll: return "All";
      case ValidationFeatureDisableEXT::eShaders: return "Shaders";
      case ValidationFeatureDisableEXT::eThreadSafety: return "ThreadSafety";
      case ValidationFeatureDisableEXT::eApiParameters: return "ApiParameters";
      case ValidationFeatureDisableEXT::eObjectLifetimes: return "ObjectLifetimes";
      case ValidationFeatureDisableEXT::eCoreChecks: return "CoreChecks";
      case ValidationFeatureDisableEXT::eUniqueHandles: return "UniqueHandles";
      case ValidationFeatureDisableEXT::eShaderValidationCache: return "ShaderValidationCache";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  //=== VK_EXT_headless_surface ===

  VULKAN_HPP_INLINE std::string to_string( HeadlessSurfaceCreateFlagBitsEXT )
  {
    return "(void)";
  }

  //=== VK_KHR_object_refresh ===

  VULKAN_HPP_INLINE std::string to_string( RefreshObjectFlagBitsKHR )
  {
    return "(void)";
  }

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===

  VULKAN_HPP_INLINE std::string to_string( SciSyncClientTypeNV value )
  {
    switch ( value )
    {
      case SciSyncClientTypeNV::eSignaler: return "Signaler";
      case SciSyncClientTypeNV::eWaiter: return "Waiter";
      case SciSyncClientTypeNV::eSignalerWaiter: return "SignalerWaiter";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  VULKAN_HPP_INLINE std::string to_string( SciSyncPrimitiveTypeNV value )
  {
    switch ( value )
    {
      case SciSyncPrimitiveTypeNV::eFence: return "Fence";
      case SciSyncPrimitiveTypeNV::eSemaphore: return "Semaphore";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }
#endif /*VK_USE_PLATFORM_SCI*/

}  // namespace VULKAN_HPP_NAMESPACE
#endif
