// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_ENUMS_HPP
#define VULKAN_ENUMS_HPP

namespace VULKAN_HPP_NAMESPACE
{
  template <typename FlagBitsType>
  struct FlagTraits
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool isBitmask = false;
  };

  template <typename BitType>
  class Flags
  {
  public:
    using MaskType = typename std::underlying_type<BitType>::type;

    // constructors
    VULKAN_HPP_CONSTEXPR Flags() VULKAN_HPP_NOEXCEPT : m_mask( 0 ) {}

    VULKAN_HPP_CONSTEXPR Flags( BitType bit ) VULKAN_HPP_NOEXCEPT : m_mask( static_cast<MaskType>( bit ) ) {}

    VULKAN_HPP_CONSTEXPR Flags( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    VULKAN_HPP_CONSTEXPR explicit Flags( MaskType flags ) VULKAN_HPP_NOEXCEPT : m_mask( flags ) {}

    // relational operators
#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
    auto operator<=>( Flags<BitType> const & ) const = default;
#else
    VULKAN_HPP_CONSTEXPR bool operator<( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask < rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator<=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask <= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask > rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask >= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator==( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask == rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator!=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask != rhs.m_mask;
    }
#endif

    // logical operator
    VULKAN_HPP_CONSTEXPR bool operator!() const VULKAN_HPP_NOEXCEPT
    {
      return !m_mask;
    }

    // bitwise operators
    VULKAN_HPP_CONSTEXPR Flags<BitType> operator&( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask & rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator|( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask | rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator^( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask ^ rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator~() const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask ^ FlagTraits<BitType>::allFlags.m_mask );
    }

    // assignment operators
    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator|=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator&=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator^=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask ^= rhs.m_mask;
      return *this;
    }

    // cast operators
    explicit VULKAN_HPP_CONSTEXPR operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return !!m_mask;
    }

    explicit VULKAN_HPP_CONSTEXPR operator MaskType() const VULKAN_HPP_NOEXCEPT
    {
      return m_mask;
    }

#if defined( VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC )
  public:
#else
  private:
#endif
    MaskType m_mask;
  };

#if !defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  // relational operators only needed for pre C++20
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator>( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator>=( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator<( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator<=( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator==( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator==( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator!=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator!=( bit );
  }
#endif

  // bitwise operators
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator&( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator&( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator|( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator|( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator^( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator^( bit );
  }

  // bitwise operators on BitType
  template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR Flags<BitType> operator&( BitType lhs, BitType rhs ) VULKAN_HPP_NOEXCEPT
  {
    return Flags<BitType>( lhs ) & rhs;
  }

  template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR Flags<BitType> operator|( BitType lhs, BitType rhs ) VULKAN_HPP_NOEXCEPT
  {
    return Flags<BitType>( lhs ) | rhs;
  }

  template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR Flags<BitType> operator^( BitType lhs, BitType rhs ) VULKAN_HPP_NOEXCEPT
  {
    return Flags<BitType>( lhs ) ^ rhs;
  }

  template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR Flags<BitType> operator~( BitType bit ) VULKAN_HPP_NOEXCEPT
  {
    return ~( Flags<BitType>( bit ) );
  }

  template <typename EnumType, EnumType value>
  struct CppType
  {
  };

  //=============
  //=== ENUMs ===
  //=============

  //=== VK_VERSION_1_0 ===

  enum class Result
  {
    eSuccess                                     = VK_SUCCESS,
    eNotReady                                    = VK_NOT_READY,
    eTimeout                                     = VK_TIMEOUT,
    eEventSet                                    = VK_EVENT_SET,
    eEventReset                                  = VK_EVENT_RESET,
    eIncomplete                                  = VK_INCOMPLETE,
    eErrorOutOfHostMemory                        = VK_ERROR_OUT_OF_HOST_MEMORY,
    eErrorOutOfDeviceMemory                      = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    eErrorInitializationFailed                   = VK_ERROR_INITIALIZATION_FAILED,
    eErrorDeviceLost                             = VK_ERROR_DEVICE_LOST,
    eErrorMemoryMapFailed                        = VK_ERROR_MEMORY_MAP_FAILED,
    eErrorLayerNotPresent                        = VK_ERROR_LAYER_NOT_PRESENT,
    eErrorExtensionNotPresent                    = VK_ERROR_EXTENSION_NOT_PRESENT,
    eErrorFeatureNotPresent                      = VK_ERROR_FEATURE_NOT_PRESENT,
    eErrorIncompatibleDriver                     = VK_ERROR_INCOMPATIBLE_DRIVER,
    eErrorTooManyObjects                         = VK_ERROR_TOO_MANY_OBJECTS,
    eErrorFormatNotSupported                     = VK_ERROR_FORMAT_NOT_SUPPORTED,
    eErrorFragmentedPool                         = VK_ERROR_FRAGMENTED_POOL,
    eErrorUnknown                                = VK_ERROR_UNKNOWN,
    eErrorOutOfPoolMemory                        = VK_ERROR_OUT_OF_POOL_MEMORY,
    eErrorInvalidExternalHandle                  = VK_ERROR_INVALID_EXTERNAL_HANDLE,
    eErrorFragmentation                          = VK_ERROR_FRAGMENTATION,
    eErrorInvalidOpaqueCaptureAddress            = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
    ePipelineCompileRequired                     = VK_PIPELINE_COMPILE_REQUIRED,
    eErrorValidationFailed                       = VK_ERROR_VALIDATION_FAILED,
    eErrorInvalidPipelineCacheData               = VK_ERROR_INVALID_PIPELINE_CACHE_DATA,
    eErrorNoPipelineMatch                        = VK_ERROR_NO_PIPELINE_MATCH,
    eErrorSurfaceLostKHR                         = VK_ERROR_SURFACE_LOST_KHR,
    eErrorNativeWindowInUseKHR                   = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    eSuboptimalKHR                               = VK_SUBOPTIMAL_KHR,
    eErrorOutOfDateKHR                           = VK_ERROR_OUT_OF_DATE_KHR,
    eErrorIncompatibleDisplayKHR                 = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    eErrorInvalidDrmFormatModifierPlaneLayoutEXT = VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
    eErrorNotPermittedEXT                        = VK_ERROR_NOT_PERMITTED_EXT
  };

  enum class StructureType
  {
    eApplicationInfo                                         = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    eInstanceCreateInfo                                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    eDeviceQueueCreateInfo                                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    eDeviceCreateInfo                                        = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    eSubmitInfo                                              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    eMemoryAllocateInfo                                      = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    eMappedMemoryRange                                       = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    eFenceCreateInfo                                         = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    eSemaphoreCreateInfo                                     = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    eEventCreateInfo                                         = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
    eQueryPoolCreateInfo                                     = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
    eBufferCreateInfo                                        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    eBufferViewCreateInfo                                    = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
    eImageCreateInfo                                         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    eImageViewCreateInfo                                     = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    ePipelineCacheCreateInfo                                 = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
    ePipelineShaderStageCreateInfo                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    ePipelineVertexInputStateCreateInfo                      = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    ePipelineInputAssemblyStateCreateInfo                    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    ePipelineTessellationStateCreateInfo                     = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    ePipelineViewportStateCreateInfo                         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    ePipelineRasterizationStateCreateInfo                    = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    ePipelineMultisampleStateCreateInfo                      = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    ePipelineDepthStencilStateCreateInfo                     = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    ePipelineColorBlendStateCreateInfo                       = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    ePipelineDynamicStateCreateInfo                          = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    eGraphicsPipelineCreateInfo                              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    eComputePipelineCreateInfo                               = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    ePipelineLayoutCreateInfo                                = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    eSamplerCreateInfo                                       = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    eDescriptorSetLayoutCreateInfo                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    eDescriptorPoolCreateInfo                                = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    eDescriptorSetAllocateInfo                               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    eWriteDescriptorSet                                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    eCopyDescriptorSet                                       = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,
    eFramebufferCreateInfo                                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    eRenderPassCreateInfo                                    = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    eCommandPoolCreateInfo                                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    eCommandBufferAllocateInfo                               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    eCommandBufferInheritanceInfo                            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
    eCommandBufferBeginInfo                                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    eRenderPassBeginInfo                                     = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    eBufferMemoryBarrier                                     = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    eImageMemoryBarrier                                      = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    eMemoryBarrier                                           = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
    eLoaderInstanceCreateInfo                                = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO,
    eLoaderDeviceCreateInfo                                  = VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO,
    ePhysicalDeviceSubgroupProperties                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES,
    eBindBufferMemoryInfo                                    = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO,
    eBindImageMemoryInfo                                     = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO,
    ePhysicalDevice16BitStorageFeatures                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES,
    eMemoryDedicatedRequirements                             = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS,
    eMemoryDedicatedAllocateInfo                             = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO,
    eMemoryAllocateFlagsInfo                                 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
    eDeviceGroupRenderPassBeginInfo                          = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO,
    eDeviceGroupCommandBufferBeginInfo                       = VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO,
    eDeviceGroupSubmitInfo                                   = VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO,
    eBindBufferMemoryDeviceGroupInfo                         = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO,
    eBindImageMemoryDeviceGroupInfo                          = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO,
    ePhysicalDeviceGroupProperties                           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES,
    eDeviceGroupDeviceCreateInfo                             = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO,
    eBufferMemoryRequirementsInfo2                           = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2,
    eImageMemoryRequirementsInfo2                            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2,
    eMemoryRequirements2                                     = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
    ePhysicalDeviceFeatures2                                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    ePhysicalDeviceProperties2                               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
    eFormatProperties2                                       = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2,
    eImageFormatProperties2                                  = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2,
    ePhysicalDeviceImageFormatInfo2                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2,
    eQueueFamilyProperties2                                  = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
    ePhysicalDeviceMemoryProperties2                         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
    ePhysicalDevicePointClippingProperties                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES,
    eRenderPassInputAttachmentAspectCreateInfo               = VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO,
    eImageViewUsageCreateInfo                                = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO,
    ePipelineTessellationDomainOriginStateCreateInfo         = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO,
    eRenderPassMultiviewCreateInfo                           = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO,
    ePhysicalDeviceMultiviewFeatures                         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
    ePhysicalDeviceMultiviewProperties                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,
    ePhysicalDeviceVariablePointersFeatures                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES,
    eProtectedSubmitInfo                                     = VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO,
    ePhysicalDeviceProtectedMemoryFeatures                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES,
    ePhysicalDeviceProtectedMemoryProperties                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES,
    eDeviceQueueInfo2                                        = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
    eSamplerYcbcrConversionCreateInfo                        = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO,
    eSamplerYcbcrConversionInfo                              = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO,
    eBindImagePlaneMemoryInfo                                = VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO,
    eImagePlaneMemoryRequirementsInfo                        = VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO,
    ePhysicalDeviceSamplerYcbcrConversionFeatures            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES,
    eSamplerYcbcrConversionImageFormatProperties             = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES,
    ePhysicalDeviceExternalImageFormatInfo                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO,
    eExternalImageFormatProperties                           = VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES,
    ePhysicalDeviceExternalBufferInfo                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO,
    eExternalBufferProperties                                = VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES,
    ePhysicalDeviceIdProperties                              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES,
    eExternalMemoryBufferCreateInfo                          = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,
    eExternalMemoryImageCreateInfo                           = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO,
    eExportMemoryAllocateInfo                                = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO,
    ePhysicalDeviceExternalFenceInfo                         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO,
    eExternalFenceProperties                                 = VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES,
    eExportFenceCreateInfo                                   = VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO,
    eExportSemaphoreCreateInfo                               = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO,
    ePhysicalDeviceExternalSemaphoreInfo                     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO,
    eExternalSemaphoreProperties                             = VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES,
    ePhysicalDeviceMaintenance3Properties                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES,
    eDescriptorSetLayoutSupport                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT,
    ePhysicalDeviceShaderDrawParametersFeatures              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES,
    ePhysicalDeviceVulkan11Features                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
    ePhysicalDeviceVulkan11Properties                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
    ePhysicalDeviceVulkan12Features                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
    ePhysicalDeviceVulkan12Properties                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
    eImageFormatListCreateInfo                               = VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO,
    eAttachmentDescription2                                  = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
    eAttachmentReference2                                    = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
    eSubpassDescription2                                     = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
    eSubpassDependency2                                      = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2,
    eRenderPassCreateInfo2                                   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
    eSubpassBeginInfo                                        = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
    eSubpassEndInfo                                          = VK_STRUCTURE_TYPE_SUBPASS_END_INFO,
    ePhysicalDevice8BitStorageFeatures                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES,
    ePhysicalDeviceDriverProperties                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES,
    ePhysicalDeviceShaderAtomicInt64Features                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES,
    ePhysicalDeviceShaderFloat16Int8Features                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES,
    ePhysicalDeviceFloatControlsProperties                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES,
    eDescriptorSetLayoutBindingFlagsCreateInfo               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
    ePhysicalDeviceDescriptorIndexingFeatures                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
    ePhysicalDeviceDescriptorIndexingProperties              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES,
    eDescriptorSetVariableDescriptorCountAllocateInfo        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
    eDescriptorSetVariableDescriptorCountLayoutSupport       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT,
    ePhysicalDeviceDepthStencilResolveProperties             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES,
    eSubpassDescriptionDepthStencilResolve                   = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE,
    ePhysicalDeviceScalarBlockLayoutFeatures                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
    eImageStencilUsageCreateInfo                             = VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO,
    ePhysicalDeviceSamplerFilterMinmaxProperties             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES,
    eSamplerReductionModeCreateInfo                          = VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO,
    ePhysicalDeviceVulkanMemoryModelFeatures                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,
    ePhysicalDeviceImagelessFramebufferFeatures              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES,
    eFramebufferAttachmentsCreateInfo                        = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO,
    eFramebufferAttachmentImageInfo                          = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO,
    eRenderPassAttachmentBeginInfo                           = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO,
    ePhysicalDeviceUniformBufferStandardLayoutFeatures       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES,
    ePhysicalDeviceShaderSubgroupExtendedTypesFeatures       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES,
    ePhysicalDeviceSeparateDepthStencilLayoutsFeatures       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES,
    eAttachmentReferenceStencilLayout                        = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT,
    eAttachmentDescriptionStencilLayout                      = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT,
    ePhysicalDeviceHostQueryResetFeatures                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES,
    ePhysicalDeviceTimelineSemaphoreFeatures                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
    ePhysicalDeviceTimelineSemaphoreProperties               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES,
    eSemaphoreTypeCreateInfo                                 = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
    eTimelineSemaphoreSubmitInfo                             = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
    eSemaphoreWaitInfo                                       = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
    eSemaphoreSignalInfo                                     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
    ePhysicalDeviceBufferDeviceAddressFeatures               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
    eBufferDeviceAddressInfo                                 = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
    eBufferOpaqueCaptureAddressCreateInfo                    = VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO,
    eMemoryOpaqueCaptureAddressAllocateInfo                  = VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO,
    eDeviceMemoryOpaqueCaptureAddressInfo                    = VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO,
    ePhysicalDeviceVulkan13Features                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
    ePhysicalDeviceVulkan13Properties                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES,
    ePipelineCreationFeedbackCreateInfo                      = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO,
    ePhysicalDeviceShaderTerminateInvocationFeatures         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES,
    ePhysicalDeviceToolProperties                            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES,
    ePhysicalDeviceShaderDemoteToHelperInvocationFeatures    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES,
    ePhysicalDevicePrivateDataFeatures                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES,
    eDevicePrivateDataCreateInfo                             = VK_STRUCTURE_TYPE_DEVICE_PRIVATE_DATA_CREATE_INFO,
    ePrivateDataSlotCreateInfo                               = VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO,
    ePhysicalDevicePipelineCreationCacheControlFeatures      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES,
    eMemoryBarrier2                                          = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
    eBufferMemoryBarrier2                                    = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
    eImageMemoryBarrier2                                     = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
    eDependencyInfo                                          = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
    eSubmitInfo2                                             = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
    eSemaphoreSubmitInfo                                     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
    eCommandBufferSubmitInfo                                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
    ePhysicalDeviceSynchronization2Features                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
    ePhysicalDeviceZeroInitializeWorkgroupMemoryFeatures     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES,
    ePhysicalDeviceImageRobustnessFeatures                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES,
    eCopyBufferInfo2                                         = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
    eCopyImageInfo2                                          = VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2,
    eCopyBufferToImageInfo2                                  = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
    eCopyImageToBufferInfo2                                  = VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2,
    eBlitImageInfo2                                          = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
    eResolveImageInfo2                                       = VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2,
    eBufferCopy2                                             = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
    eImageCopy2                                              = VK_STRUCTURE_TYPE_IMAGE_COPY_2,
    eImageBlit2                                              = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
    eBufferImageCopy2                                        = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
    eImageResolve2                                           = VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2,
    ePhysicalDeviceSubgroupSizeControlProperties             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES,
    ePipelineShaderStageRequiredSubgroupSizeCreateInfo       = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO,
    ePhysicalDeviceSubgroupSizeControlFeatures               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES,
    ePhysicalDeviceInlineUniformBlockFeatures                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES,
    ePhysicalDeviceInlineUniformBlockProperties              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES,
    eWriteDescriptorSetInlineUniformBlock                    = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK,
    eDescriptorPoolInlineUniformBlockCreateInfo              = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO,
    ePhysicalDeviceTextureCompressionAstcHdrFeatures         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES,
    eRenderingInfo                                           = VK_STRUCTURE_TYPE_RENDERING_INFO,
    eRenderingAttachmentInfo                                 = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
    ePipelineRenderingCreateInfo                             = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
    ePhysicalDeviceDynamicRenderingFeatures                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
    eCommandBufferInheritanceRenderingInfo                   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO,
    ePhysicalDeviceShaderIntegerDotProductFeatures           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES,
    ePhysicalDeviceShaderIntegerDotProductProperties         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES,
    ePhysicalDeviceTexelBufferAlignmentProperties            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES,
    eFormatProperties3                                       = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3,
    ePhysicalDeviceMaintenance4Features                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES,
    ePhysicalDeviceMaintenance4Properties                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES,
    eDeviceBufferMemoryRequirements                          = VK_STRUCTURE_TYPE_DEVICE_BUFFER_MEMORY_REQUIREMENTS,
    eDeviceImageMemoryRequirements                           = VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS,
    ePhysicalDeviceVulkanSc10Features                        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_SC_1_0_FEATURES,
    ePhysicalDeviceVulkanSc10Properties                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_SC_1_0_PROPERTIES,
    eDeviceObjectReservationCreateInfo                       = VK_STRUCTURE_TYPE_DEVICE_OBJECT_RESERVATION_CREATE_INFO,
    eCommandPoolMemoryReservationCreateInfo                  = VK_STRUCTURE_TYPE_COMMAND_POOL_MEMORY_RESERVATION_CREATE_INFO,
    eCommandPoolMemoryConsumption                            = VK_STRUCTURE_TYPE_COMMAND_POOL_MEMORY_CONSUMPTION,
    ePipelinePoolSize                                        = VK_STRUCTURE_TYPE_PIPELINE_POOL_SIZE,
    eFaultData                                               = VK_STRUCTURE_TYPE_FAULT_DATA,
    eFaultCallbackInfo                                       = VK_STRUCTURE_TYPE_FAULT_CALLBACK_INFO,
    ePipelineOfflineCreateInfo                               = VK_STRUCTURE_TYPE_PIPELINE_OFFLINE_CREATE_INFO,
    eSwapchainCreateInfoKHR                                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    ePresentInfoKHR                                          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    eDeviceGroupPresentCapabilitiesKHR                       = VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR,
    eImageSwapchainCreateInfoKHR                             = VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR,
    eBindImageMemorySwapchainInfoKHR                         = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR,
    eAcquireNextImageInfoKHR                                 = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
    eDeviceGroupPresentInfoKHR                               = VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR,
    eDeviceGroupSwapchainCreateInfoKHR                       = VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR,
    eDisplayModeCreateInfoKHR                                = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
    eDisplaySurfaceCreateInfoKHR                             = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
    eDisplayPresentInfoKHR                                   = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR,
    ePrivateVendorInfoPlaceholderOffset0NV                   = VK_STRUCTURE_TYPE_PRIVATE_VENDOR_INFO_PLACEHOLDER_OFFSET_0_NV,
    ePhysicalDeviceTextureCompressionAstcHdrFeaturesEXT      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT,
    eImageViewAstcDecodeModeEXT                              = VK_STRUCTURE_TYPE_IMAGE_VIEW_ASTC_DECODE_MODE_EXT,
    ePhysicalDeviceAstcDecodeFeaturesEXT                     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT,
    eImportMemoryFdInfoKHR                                   = VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR,
    eMemoryFdPropertiesKHR                                   = VK_STRUCTURE_TYPE_MEMORY_FD_PROPERTIES_KHR,
    eMemoryGetFdInfoKHR                                      = VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR,
    eImportSemaphoreFdInfoKHR                                = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR,
    eSemaphoreGetFdInfoKHR                                   = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR,
    ePresentRegionsKHR                                       = VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR,
    eSurfaceCapabilities2EXT                                 = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT,
    eDisplayPowerInfoEXT                                     = VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT,
    eDeviceEventInfoEXT                                      = VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT,
    eDisplayEventInfoEXT                                     = VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT,
    eSwapchainCounterCreateInfoEXT                           = VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT,
    ePhysicalDeviceDiscardRectanglePropertiesEXT             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT,
    ePipelineDiscardRectangleStateCreateInfoEXT              = VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT,
    ePhysicalDeviceConservativeRasterizationPropertiesEXT    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT,
    ePipelineRasterizationConservativeStateCreateInfoEXT     = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT,
    ePhysicalDeviceDepthClipEnableFeaturesEXT                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT,
    ePipelineRasterizationDepthClipStateCreateInfoEXT        = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT,
    eHdrMetadataEXT                                          = VK_STRUCTURE_TYPE_HDR_METADATA_EXT,
    eSharedPresentSurfaceCapabilitiesKHR                     = VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR,
    eImportFenceFdInfoKHR                                    = VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR,
    eFenceGetFdInfoKHR                                       = VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR,
    ePhysicalDevicePerformanceQueryFeaturesKHR               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR,
    ePhysicalDevicePerformanceQueryPropertiesKHR             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR,
    eQueryPoolPerformanceCreateInfoKHR                       = VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR,
    ePerformanceQuerySubmitInfoKHR                           = VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_SUBMIT_INFO_KHR,
    eAcquireProfilingLockInfoKHR                             = VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR,
    ePerformanceCounterKHR                                   = VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR,
    ePerformanceCounterDescriptionKHR                        = VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR,
    ePerformanceQueryReservationInfoKHR                      = VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_RESERVATION_INFO_KHR,
    ePhysicalDeviceSurfaceInfo2KHR                           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
    eSurfaceCapabilities2KHR                                 = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR,
    eSurfaceFormat2KHR                                       = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR,
    eDisplayProperties2KHR                                   = VK_STRUCTURE_TYPE_DISPLAY_PROPERTIES_2_KHR,
    eDisplayPlaneProperties2KHR                              = VK_STRUCTURE_TYPE_DISPLAY_PLANE_PROPERTIES_2_KHR,
    eDisplayModeProperties2KHR                               = VK_STRUCTURE_TYPE_DISPLAY_MODE_PROPERTIES_2_KHR,
    eDisplayPlaneInfo2KHR                                    = VK_STRUCTURE_TYPE_DISPLAY_PLANE_INFO_2_KHR,
    eDisplayPlaneCapabilities2KHR                            = VK_STRUCTURE_TYPE_DISPLAY_PLANE_CAPABILITIES_2_KHR,
    eDebugUtilsObjectNameInfoEXT                             = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
    eDebugUtilsObjectTagInfoEXT                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT,
    eDebugUtilsLabelEXT                                      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
    eDebugUtilsMessengerCallbackDataEXT                      = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
    eDebugUtilsMessengerCreateInfoEXT                        = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    eSampleLocationsInfoEXT                                  = VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT,
    eRenderPassSampleLocationsBeginInfoEXT                   = VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT,
    ePipelineSampleLocationsStateCreateInfoEXT               = VK_STRUCTURE_TYPE_PIPELINE_SAMPLE_LOCATIONS_STATE_CREATE_INFO_EXT,
    ePhysicalDeviceSampleLocationsPropertiesEXT              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT,
    eMultisamplePropertiesEXT                                = VK_STRUCTURE_TYPE_MULTISAMPLE_PROPERTIES_EXT,
    ePhysicalDeviceBlendOperationAdvancedFeaturesEXT         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT,
    ePhysicalDeviceBlendOperationAdvancedPropertiesEXT       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT,
    ePipelineColorBlendAdvancedStateCreateInfoEXT            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT,
    eDrmFormatModifierPropertiesListEXT                      = VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_EXT,
    ePhysicalDeviceImageDrmFormatModifierInfoEXT             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_DRM_FORMAT_MODIFIER_INFO_EXT,
    eImageDrmFormatModifierListCreateInfoEXT                 = VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_LIST_CREATE_INFO_EXT,
    eImageDrmFormatModifierExplicitCreateInfoEXT             = VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_EXPLICIT_CREATE_INFO_EXT,
    eImageDrmFormatModifierPropertiesEXT                     = VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_PROPERTIES_EXT,
    eDrmFormatModifierPropertiesList2EXT                     = VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_2_EXT,
    ePhysicalDeviceImageViewImageFormatInfoEXT               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_IMAGE_FORMAT_INFO_EXT,
    eFilterCubicImageViewImageFormatPropertiesEXT            = VK_STRUCTURE_TYPE_FILTER_CUBIC_IMAGE_VIEW_IMAGE_FORMAT_PROPERTIES_EXT,
    eDeviceQueueGlobalPriorityCreateInfoEXT                  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO_EXT,
    eImportMemoryHostPointerInfoEXT                          = VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT,
    eMemoryHostPointerPropertiesEXT                          = VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT,
    ePhysicalDeviceExternalMemoryHostPropertiesEXT           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT,
    ePhysicalDeviceShaderClockFeaturesKHR                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR,
    eCalibratedTimestampInfoEXT                              = VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT,
    ePhysicalDeviceVertexAttributeDivisorPropertiesEXT       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT,
    ePipelineVertexInputDivisorStateCreateInfoEXT            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT,
    ePhysicalDeviceVertexAttributeDivisorFeaturesEXT         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT,
    ePhysicalDevicePciBusInfoPropertiesEXT                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT,
    ePhysicalDeviceShaderTerminateInvocationFeaturesKHR      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR,
    ePhysicalDeviceSubgroupSizeControlPropertiesEXT          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT,
    ePipelineShaderStageRequiredSubgroupSizeCreateInfoEXT    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO_EXT,
    ePhysicalDeviceSubgroupSizeControlFeaturesEXT            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT,
    eFragmentShadingRateAttachmentInfoKHR                    = VK_STRUCTURE_TYPE_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR,
    ePipelineFragmentShadingRateStateCreateInfoKHR           = VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_STATE_CREATE_INFO_KHR,
    ePhysicalDeviceFragmentShadingRatePropertiesKHR          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR,
    ePhysicalDeviceFragmentShadingRateFeaturesKHR            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR,
    ePhysicalDeviceFragmentShadingRateKHR                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_KHR,
    ePhysicalDeviceShaderImageAtomicInt64FeaturesEXT         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT,
    ePhysicalDeviceMemoryBudgetPropertiesEXT                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT,
    eValidationFeaturesEXT                                   = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
    ePhysicalDeviceFragmentShaderInterlockFeaturesEXT        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT,
    ePhysicalDeviceYcbcrImageArraysFeaturesEXT               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT,
    eHeadlessSurfaceCreateInfoEXT                            = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT,
    ePhysicalDeviceLineRasterizationFeaturesEXT              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT,
    ePipelineRasterizationLineStateCreateInfoEXT             = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,
    ePhysicalDeviceLineRasterizationPropertiesEXT            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT,
    ePhysicalDeviceShaderAtomicFloatFeaturesEXT              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT,
    ePhysicalDeviceIndexTypeUint8FeaturesEXT                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT,
    ePhysicalDeviceExtendedDynamicStateFeaturesEXT           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
    ePhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT,
    ePhysicalDeviceTexelBufferAlignmentFeaturesEXT           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT,
    ePhysicalDeviceTexelBufferAlignmentPropertiesEXT         = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT,
    ePhysicalDeviceRobustness2FeaturesEXT                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,
    ePhysicalDeviceRobustness2PropertiesEXT                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT,
    eSamplerCustomBorderColorCreateInfoEXT                   = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT,
    ePhysicalDeviceCustomBorderColorPropertiesEXT            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT,
    ePhysicalDeviceCustomBorderColorFeaturesEXT              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT,
    eRefreshObjectListKHR                                    = VK_STRUCTURE_TYPE_REFRESH_OBJECT_LIST_KHR,
    eMemoryBarrier2KHR                                       = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR,
    eBufferMemoryBarrier2KHR                                 = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR,
    eImageMemoryBarrier2KHR                                  = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
    eDependencyInfoKHR                                       = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR,
    eSubmitInfo2KHR                                          = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
    eSemaphoreSubmitInfoKHR                                  = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
    eCommandBufferSubmitInfoKHR                              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR,
    ePhysicalDeviceSynchronization2FeaturesKHR               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,
    eQueueFamilyCheckpointProperties2NV                      = VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_2_NV,
    eCheckpointData2NV                                       = VK_STRUCTURE_TYPE_CHECKPOINT_DATA_2_NV,
    ePhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT,
    ePhysicalDeviceImageRobustnessFeaturesEXT                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT,
    eCopyBufferInfo2KHR                                      = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR,
    eCopyImageInfo2KHR                                       = VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2_KHR,
    eCopyBufferToImageInfo2KHR                               = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2_KHR,
    eCopyImageToBufferInfo2KHR                               = VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2_KHR,
    eBlitImageInfo2KHR                                       = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2_KHR,
    eResolveImageInfo2KHR                                    = VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2_KHR,
    eBufferCopy2KHR                                          = VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR,
    eImageCopy2KHR                                           = VK_STRUCTURE_TYPE_IMAGE_COPY_2_KHR,
    eImageBlit2KHR                                           = VK_STRUCTURE_TYPE_IMAGE_BLIT_2_KHR,
    eBufferImageCopy2KHR                                     = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2_KHR,
    eImageResolve2KHR                                        = VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2_KHR,
    ePhysicalDevice4444FormatsFeaturesEXT                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT,
    ePhysicalDeviceVertexInputDynamicStateFeaturesEXT        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT,
    eVertexInputBindingDescription2EXT                       = VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT,
    eVertexInputAttributeDescription2EXT                     = VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT,
#if defined( VK_USE_PLATFORM_SCI )
    eImportFenceSciSyncInfoNV                     = VK_STRUCTURE_TYPE_IMPORT_FENCE_SCI_SYNC_INFO_NV,
    eExportFenceSciSyncInfoNV                     = VK_STRUCTURE_TYPE_EXPORT_FENCE_SCI_SYNC_INFO_NV,
    eFenceGetSciSyncInfoNV                        = VK_STRUCTURE_TYPE_FENCE_GET_SCI_SYNC_INFO_NV,
    eSciSyncAttributesInfoNV                      = VK_STRUCTURE_TYPE_SCI_SYNC_ATTRIBUTES_INFO_NV,
    eImportSemaphoreSciSyncInfoNV                 = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_SCI_SYNC_INFO_NV,
    eExportSemaphoreSciSyncInfoNV                 = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_SCI_SYNC_INFO_NV,
    eSemaphoreGetSciSyncInfoNV                    = VK_STRUCTURE_TYPE_SEMAPHORE_GET_SCI_SYNC_INFO_NV,
    ePhysicalDeviceExternalSciSyncFeaturesNV      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_FEATURES_NV,
    eImportMemorySciBufInfoNV                     = VK_STRUCTURE_TYPE_IMPORT_MEMORY_SCI_BUF_INFO_NV,
    eExportMemorySciBufInfoNV                     = VK_STRUCTURE_TYPE_EXPORT_MEMORY_SCI_BUF_INFO_NV,
    eMemoryGetSciBufInfoNV                        = VK_STRUCTURE_TYPE_MEMORY_GET_SCI_BUF_INFO_NV,
    eMemorySciBufPropertiesNV                     = VK_STRUCTURE_TYPE_MEMORY_SCI_BUF_PROPERTIES_NV,
    ePhysicalDeviceExternalMemorySciBufFeaturesNV = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_SCI_BUF_FEATURES_NV,
    ePhysicalDeviceExternalSciBufFeaturesNV       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_BUF_FEATURES_NV,
#endif /*VK_USE_PLATFORM_SCI*/
    ePhysicalDeviceExtendedDynamicState2FeaturesEXT = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT,
    ePhysicalDeviceColorWriteEnableFeaturesEXT      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT,
    ePipelineColorWriteCreateInfoEXT                = VK_STRUCTURE_TYPE_PIPELINE_COLOR_WRITE_CREATE_INFO_EXT,
    eApplicationParametersEXT                       = VK_STRUCTURE_TYPE_APPLICATION_PARAMETERS_EXT,
#if defined( VK_USE_PLATFORM_SCI )
    eSemaphoreSciSyncPoolCreateInfoNV                  = VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_POOL_CREATE_INFO_NV,
    eSemaphoreSciSyncCreateInfoNV                      = VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_CREATE_INFO_NV,
    ePhysicalDeviceExternalSciSync2FeaturesNV          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_2_FEATURES_NV,
    eDeviceSemaphoreSciSyncPoolReservationCreateInfoNV = VK_STRUCTURE_TYPE_DEVICE_SEMAPHORE_SCI_SYNC_POOL_RESERVATION_CREATE_INFO_NV,
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    eScreenBufferPropertiesQNX                           = VK_STRUCTURE_TYPE_SCREEN_BUFFER_PROPERTIES_QNX,
    eScreenBufferFormatPropertiesQNX                     = VK_STRUCTURE_TYPE_SCREEN_BUFFER_FORMAT_PROPERTIES_QNX,
    eImportScreenBufferInfoQNX                           = VK_STRUCTURE_TYPE_IMPORT_SCREEN_BUFFER_INFO_QNX,
    eExternalFormatQNX                                   = VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_QNX,
    ePhysicalDeviceExternalMemoryScreenBufferFeaturesQNX = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_SCREEN_BUFFER_FEATURES_QNX
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
  };

  enum class PipelineCacheHeaderVersion
  {
    eOne               = VK_PIPELINE_CACHE_HEADER_VERSION_ONE,
    eSafetyCriticalOne = VK_PIPELINE_CACHE_HEADER_VERSION_SAFETY_CRITICAL_ONE
  };

  enum class ObjectType
  {
    eUnknown                = VK_OBJECT_TYPE_UNKNOWN,
    eInstance               = VK_OBJECT_TYPE_INSTANCE,
    ePhysicalDevice         = VK_OBJECT_TYPE_PHYSICAL_DEVICE,
    eDevice                 = VK_OBJECT_TYPE_DEVICE,
    eQueue                  = VK_OBJECT_TYPE_QUEUE,
    eSemaphore              = VK_OBJECT_TYPE_SEMAPHORE,
    eCommandBuffer          = VK_OBJECT_TYPE_COMMAND_BUFFER,
    eFence                  = VK_OBJECT_TYPE_FENCE,
    eDeviceMemory           = VK_OBJECT_TYPE_DEVICE_MEMORY,
    eBuffer                 = VK_OBJECT_TYPE_BUFFER,
    eImage                  = VK_OBJECT_TYPE_IMAGE,
    eEvent                  = VK_OBJECT_TYPE_EVENT,
    eQueryPool              = VK_OBJECT_TYPE_QUERY_POOL,
    eBufferView             = VK_OBJECT_TYPE_BUFFER_VIEW,
    eImageView              = VK_OBJECT_TYPE_IMAGE_VIEW,
    eShaderModule           = VK_OBJECT_TYPE_SHADER_MODULE,
    ePipelineCache          = VK_OBJECT_TYPE_PIPELINE_CACHE,
    ePipelineLayout         = VK_OBJECT_TYPE_PIPELINE_LAYOUT,
    eRenderPass             = VK_OBJECT_TYPE_RENDER_PASS,
    ePipeline               = VK_OBJECT_TYPE_PIPELINE,
    eDescriptorSetLayout    = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT,
    eSampler                = VK_OBJECT_TYPE_SAMPLER,
    eDescriptorPool         = VK_OBJECT_TYPE_DESCRIPTOR_POOL,
    eDescriptorSet          = VK_OBJECT_TYPE_DESCRIPTOR_SET,
    eFramebuffer            = VK_OBJECT_TYPE_FRAMEBUFFER,
    eCommandPool            = VK_OBJECT_TYPE_COMMAND_POOL,
    eSamplerYcbcrConversion = VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION,
    ePrivateDataSlot        = VK_OBJECT_TYPE_PRIVATE_DATA_SLOT,
    eSurfaceKHR             = VK_OBJECT_TYPE_SURFACE_KHR,
    eSwapchainKHR           = VK_OBJECT_TYPE_SWAPCHAIN_KHR,
    eDisplayKHR             = VK_OBJECT_TYPE_DISPLAY_KHR,
    eDisplayModeKHR         = VK_OBJECT_TYPE_DISPLAY_MODE_KHR,
    eDebugUtilsMessengerEXT = VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT,
#if defined( VK_USE_PLATFORM_SCI )
    eSemaphoreSciSyncPoolNV = VK_OBJECT_TYPE_SEMAPHORE_SCI_SYNC_POOL_NV
#endif /*VK_USE_PLATFORM_SCI*/
  };

  enum class VendorId
  {
    eVIV      = VK_VENDOR_ID_VIV,
    eVSI      = VK_VENDOR_ID_VSI,
    eKazan    = VK_VENDOR_ID_KAZAN,
    eCodeplay = VK_VENDOR_ID_CODEPLAY,
    eMESA     = VK_VENDOR_ID_MESA,
    ePocl     = VK_VENDOR_ID_POCL,
    eMobileye = VK_VENDOR_ID_MOBILEYE
  };

  enum class Format
  {
    eUndefined                               = VK_FORMAT_UNDEFINED,
    eR4G4UnormPack8                          = VK_FORMAT_R4G4_UNORM_PACK8,
    eR4G4B4A4UnormPack16                     = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
    eB4G4R4A4UnormPack16                     = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
    eR5G6B5UnormPack16                       = VK_FORMAT_R5G6B5_UNORM_PACK16,
    eB5G6R5UnormPack16                       = VK_FORMAT_B5G6R5_UNORM_PACK16,
    eR5G5B5A1UnormPack16                     = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
    eB5G5R5A1UnormPack16                     = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
    eA1R5G5B5UnormPack16                     = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
    eR8Unorm                                 = VK_FORMAT_R8_UNORM,
    eR8Snorm                                 = VK_FORMAT_R8_SNORM,
    eR8Uscaled                               = VK_FORMAT_R8_USCALED,
    eR8Sscaled                               = VK_FORMAT_R8_SSCALED,
    eR8Uint                                  = VK_FORMAT_R8_UINT,
    eR8Sint                                  = VK_FORMAT_R8_SINT,
    eR8Srgb                                  = VK_FORMAT_R8_SRGB,
    eR8G8Unorm                               = VK_FORMAT_R8G8_UNORM,
    eR8G8Snorm                               = VK_FORMAT_R8G8_SNORM,
    eR8G8Uscaled                             = VK_FORMAT_R8G8_USCALED,
    eR8G8Sscaled                             = VK_FORMAT_R8G8_SSCALED,
    eR8G8Uint                                = VK_FORMAT_R8G8_UINT,
    eR8G8Sint                                = VK_FORMAT_R8G8_SINT,
    eR8G8Srgb                                = VK_FORMAT_R8G8_SRGB,
    eR8G8B8Unorm                             = VK_FORMAT_R8G8B8_UNORM,
    eR8G8B8Snorm                             = VK_FORMAT_R8G8B8_SNORM,
    eR8G8B8Uscaled                           = VK_FORMAT_R8G8B8_USCALED,
    eR8G8B8Sscaled                           = VK_FORMAT_R8G8B8_SSCALED,
    eR8G8B8Uint                              = VK_FORMAT_R8G8B8_UINT,
    eR8G8B8Sint                              = VK_FORMAT_R8G8B8_SINT,
    eR8G8B8Srgb                              = VK_FORMAT_R8G8B8_SRGB,
    eB8G8R8Unorm                             = VK_FORMAT_B8G8R8_UNORM,
    eB8G8R8Snorm                             = VK_FORMAT_B8G8R8_SNORM,
    eB8G8R8Uscaled                           = VK_FORMAT_B8G8R8_USCALED,
    eB8G8R8Sscaled                           = VK_FORMAT_B8G8R8_SSCALED,
    eB8G8R8Uint                              = VK_FORMAT_B8G8R8_UINT,
    eB8G8R8Sint                              = VK_FORMAT_B8G8R8_SINT,
    eB8G8R8Srgb                              = VK_FORMAT_B8G8R8_SRGB,
    eR8G8B8A8Unorm                           = VK_FORMAT_R8G8B8A8_UNORM,
    eR8G8B8A8Snorm                           = VK_FORMAT_R8G8B8A8_SNORM,
    eR8G8B8A8Uscaled                         = VK_FORMAT_R8G8B8A8_USCALED,
    eR8G8B8A8Sscaled                         = VK_FORMAT_R8G8B8A8_SSCALED,
    eR8G8B8A8Uint                            = VK_FORMAT_R8G8B8A8_UINT,
    eR8G8B8A8Sint                            = VK_FORMAT_R8G8B8A8_SINT,
    eR8G8B8A8Srgb                            = VK_FORMAT_R8G8B8A8_SRGB,
    eB8G8R8A8Unorm                           = VK_FORMAT_B8G8R8A8_UNORM,
    eB8G8R8A8Snorm                           = VK_FORMAT_B8G8R8A8_SNORM,
    eB8G8R8A8Uscaled                         = VK_FORMAT_B8G8R8A8_USCALED,
    eB8G8R8A8Sscaled                         = VK_FORMAT_B8G8R8A8_SSCALED,
    eB8G8R8A8Uint                            = VK_FORMAT_B8G8R8A8_UINT,
    eB8G8R8A8Sint                            = VK_FORMAT_B8G8R8A8_SINT,
    eB8G8R8A8Srgb                            = VK_FORMAT_B8G8R8A8_SRGB,
    eA8B8G8R8UnormPack32                     = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
    eA8B8G8R8SnormPack32                     = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
    eA8B8G8R8UscaledPack32                   = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
    eA8B8G8R8SscaledPack32                   = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
    eA8B8G8R8UintPack32                      = VK_FORMAT_A8B8G8R8_UINT_PACK32,
    eA8B8G8R8SintPack32                      = VK_FORMAT_A8B8G8R8_SINT_PACK32,
    eA8B8G8R8SrgbPack32                      = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
    eA2R10G10B10UnormPack32                  = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
    eA2R10G10B10SnormPack32                  = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
    eA2R10G10B10UscaledPack32                = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
    eA2R10G10B10SscaledPack32                = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
    eA2R10G10B10UintPack32                   = VK_FORMAT_A2R10G10B10_UINT_PACK32,
    eA2R10G10B10SintPack32                   = VK_FORMAT_A2R10G10B10_SINT_PACK32,
    eA2B10G10R10UnormPack32                  = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
    eA2B10G10R10SnormPack32                  = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
    eA2B10G10R10UscaledPack32                = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
    eA2B10G10R10SscaledPack32                = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
    eA2B10G10R10UintPack32                   = VK_FORMAT_A2B10G10R10_UINT_PACK32,
    eA2B10G10R10SintPack32                   = VK_FORMAT_A2B10G10R10_SINT_PACK32,
    eR16Unorm                                = VK_FORMAT_R16_UNORM,
    eR16Snorm                                = VK_FORMAT_R16_SNORM,
    eR16Uscaled                              = VK_FORMAT_R16_USCALED,
    eR16Sscaled                              = VK_FORMAT_R16_SSCALED,
    eR16Uint                                 = VK_FORMAT_R16_UINT,
    eR16Sint                                 = VK_FORMAT_R16_SINT,
    eR16Sfloat                               = VK_FORMAT_R16_SFLOAT,
    eR16G16Unorm                             = VK_FORMAT_R16G16_UNORM,
    eR16G16Snorm                             = VK_FORMAT_R16G16_SNORM,
    eR16G16Uscaled                           = VK_FORMAT_R16G16_USCALED,
    eR16G16Sscaled                           = VK_FORMAT_R16G16_SSCALED,
    eR16G16Uint                              = VK_FORMAT_R16G16_UINT,
    eR16G16Sint                              = VK_FORMAT_R16G16_SINT,
    eR16G16Sfloat                            = VK_FORMAT_R16G16_SFLOAT,
    eR16G16B16Unorm                          = VK_FORMAT_R16G16B16_UNORM,
    eR16G16B16Snorm                          = VK_FORMAT_R16G16B16_SNORM,
    eR16G16B16Uscaled                        = VK_FORMAT_R16G16B16_USCALED,
    eR16G16B16Sscaled                        = VK_FORMAT_R16G16B16_SSCALED,
    eR16G16B16Uint                           = VK_FORMAT_R16G16B16_UINT,
    eR16G16B16Sint                           = VK_FORMAT_R16G16B16_SINT,
    eR16G16B16Sfloat                         = VK_FORMAT_R16G16B16_SFLOAT,
    eR16G16B16A16Unorm                       = VK_FORMAT_R16G16B16A16_UNORM,
    eR16G16B16A16Snorm                       = VK_FORMAT_R16G16B16A16_SNORM,
    eR16G16B16A16Uscaled                     = VK_FORMAT_R16G16B16A16_USCALED,
    eR16G16B16A16Sscaled                     = VK_FORMAT_R16G16B16A16_SSCALED,
    eR16G16B16A16Uint                        = VK_FORMAT_R16G16B16A16_UINT,
    eR16G16B16A16Sint                        = VK_FORMAT_R16G16B16A16_SINT,
    eR16G16B16A16Sfloat                      = VK_FORMAT_R16G16B16A16_SFLOAT,
    eR32Uint                                 = VK_FORMAT_R32_UINT,
    eR32Sint                                 = VK_FORMAT_R32_SINT,
    eR32Sfloat                               = VK_FORMAT_R32_SFLOAT,
    eR32G32Uint                              = VK_FORMAT_R32G32_UINT,
    eR32G32Sint                              = VK_FORMAT_R32G32_SINT,
    eR32G32Sfloat                            = VK_FORMAT_R32G32_SFLOAT,
    eR32G32B32Uint                           = VK_FORMAT_R32G32B32_UINT,
    eR32G32B32Sint                           = VK_FORMAT_R32G32B32_SINT,
    eR32G32B32Sfloat                         = VK_FORMAT_R32G32B32_SFLOAT,
    eR32G32B32A32Uint                        = VK_FORMAT_R32G32B32A32_UINT,
    eR32G32B32A32Sint                        = VK_FORMAT_R32G32B32A32_SINT,
    eR32G32B32A32Sfloat                      = VK_FORMAT_R32G32B32A32_SFLOAT,
    eR64Uint                                 = VK_FORMAT_R64_UINT,
    eR64Sint                                 = VK_FORMAT_R64_SINT,
    eR64Sfloat                               = VK_FORMAT_R64_SFLOAT,
    eR64G64Uint                              = VK_FORMAT_R64G64_UINT,
    eR64G64Sint                              = VK_FORMAT_R64G64_SINT,
    eR64G64Sfloat                            = VK_FORMAT_R64G64_SFLOAT,
    eR64G64B64Uint                           = VK_FORMAT_R64G64B64_UINT,
    eR64G64B64Sint                           = VK_FORMAT_R64G64B64_SINT,
    eR64G64B64Sfloat                         = VK_FORMAT_R64G64B64_SFLOAT,
    eR64G64B64A64Uint                        = VK_FORMAT_R64G64B64A64_UINT,
    eR64G64B64A64Sint                        = VK_FORMAT_R64G64B64A64_SINT,
    eR64G64B64A64Sfloat                      = VK_FORMAT_R64G64B64A64_SFLOAT,
    eB10G11R11UfloatPack32                   = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
    eE5B9G9R9UfloatPack32                    = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
    eD16Unorm                                = VK_FORMAT_D16_UNORM,
    eX8D24UnormPack32                        = VK_FORMAT_X8_D24_UNORM_PACK32,
    eD32Sfloat                               = VK_FORMAT_D32_SFLOAT,
    eS8Uint                                  = VK_FORMAT_S8_UINT,
    eD16UnormS8Uint                          = VK_FORMAT_D16_UNORM_S8_UINT,
    eD24UnormS8Uint                          = VK_FORMAT_D24_UNORM_S8_UINT,
    eD32SfloatS8Uint                         = VK_FORMAT_D32_SFLOAT_S8_UINT,
    eBc1RgbUnormBlock                        = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
    eBc1RgbSrgbBlock                         = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
    eBc1RgbaUnormBlock                       = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
    eBc1RgbaSrgbBlock                        = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
    eBc2UnormBlock                           = VK_FORMAT_BC2_UNORM_BLOCK,
    eBc2SrgbBlock                            = VK_FORMAT_BC2_SRGB_BLOCK,
    eBc3UnormBlock                           = VK_FORMAT_BC3_UNORM_BLOCK,
    eBc3SrgbBlock                            = VK_FORMAT_BC3_SRGB_BLOCK,
    eBc4UnormBlock                           = VK_FORMAT_BC4_UNORM_BLOCK,
    eBc4SnormBlock                           = VK_FORMAT_BC4_SNORM_BLOCK,
    eBc5UnormBlock                           = VK_FORMAT_BC5_UNORM_BLOCK,
    eBc5SnormBlock                           = VK_FORMAT_BC5_SNORM_BLOCK,
    eBc6HUfloatBlock                         = VK_FORMAT_BC6H_UFLOAT_BLOCK,
    eBc6HSfloatBlock                         = VK_FORMAT_BC6H_SFLOAT_BLOCK,
    eBc7UnormBlock                           = VK_FORMAT_BC7_UNORM_BLOCK,
    eBc7SrgbBlock                            = VK_FORMAT_BC7_SRGB_BLOCK,
    eEtc2R8G8B8UnormBlock                    = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
    eEtc2R8G8B8SrgbBlock                     = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
    eEtc2R8G8B8A1UnormBlock                  = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
    eEtc2R8G8B8A1SrgbBlock                   = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
    eEtc2R8G8B8A8UnormBlock                  = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
    eEtc2R8G8B8A8SrgbBlock                   = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
    eEacR11UnormBlock                        = VK_FORMAT_EAC_R11_UNORM_BLOCK,
    eEacR11SnormBlock                        = VK_FORMAT_EAC_R11_SNORM_BLOCK,
    eEacR11G11UnormBlock                     = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
    eEacR11G11SnormBlock                     = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
    eAstc4x4UnormBlock                       = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
    eAstc4x4SrgbBlock                        = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
    eAstc5x4UnormBlock                       = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
    eAstc5x4SrgbBlock                        = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
    eAstc5x5UnormBlock                       = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
    eAstc5x5SrgbBlock                        = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
    eAstc6x5UnormBlock                       = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
    eAstc6x5SrgbBlock                        = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
    eAstc6x6UnormBlock                       = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
    eAstc6x6SrgbBlock                        = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
    eAstc8x5UnormBlock                       = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
    eAstc8x5SrgbBlock                        = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
    eAstc8x6UnormBlock                       = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
    eAstc8x6SrgbBlock                        = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
    eAstc8x8UnormBlock                       = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
    eAstc8x8SrgbBlock                        = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
    eAstc10x5UnormBlock                      = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
    eAstc10x5SrgbBlock                       = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
    eAstc10x6UnormBlock                      = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
    eAstc10x6SrgbBlock                       = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
    eAstc10x8UnormBlock                      = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
    eAstc10x8SrgbBlock                       = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
    eAstc10x10UnormBlock                     = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
    eAstc10x10SrgbBlock                      = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
    eAstc12x10UnormBlock                     = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
    eAstc12x10SrgbBlock                      = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
    eAstc12x12UnormBlock                     = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
    eAstc12x12SrgbBlock                      = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
    eG8B8G8R8422Unorm                        = VK_FORMAT_G8B8G8R8_422_UNORM,
    eB8G8R8G8422Unorm                        = VK_FORMAT_B8G8R8G8_422_UNORM,
    eG8B8R83Plane420Unorm                    = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
    eG8B8R82Plane420Unorm                    = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
    eG8B8R83Plane422Unorm                    = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
    eG8B8R82Plane422Unorm                    = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
    eG8B8R83Plane444Unorm                    = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
    eR10X6UnormPack16                        = VK_FORMAT_R10X6_UNORM_PACK16,
    eR10X6G10X6Unorm2Pack16                  = VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
    eR10X6G10X6B10X6A10X6Unorm4Pack16        = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
    eG10X6B10X6G10X6R10X6422Unorm4Pack16     = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
    eB10X6G10X6R10X6G10X6422Unorm4Pack16     = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
    eG10X6B10X6R10X63Plane420Unorm3Pack16    = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
    eG10X6B10X6R10X62Plane420Unorm3Pack16    = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
    eG10X6B10X6R10X63Plane422Unorm3Pack16    = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
    eG10X6B10X6R10X62Plane422Unorm3Pack16    = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
    eG10X6B10X6R10X63Plane444Unorm3Pack16    = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
    eR12X4UnormPack16                        = VK_FORMAT_R12X4_UNORM_PACK16,
    eR12X4G12X4Unorm2Pack16                  = VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
    eR12X4G12X4B12X4A12X4Unorm4Pack16        = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
    eG12X4B12X4G12X4R12X4422Unorm4Pack16     = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
    eB12X4G12X4R12X4G12X4422Unorm4Pack16     = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
    eG12X4B12X4R12X43Plane420Unorm3Pack16    = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
    eG12X4B12X4R12X42Plane420Unorm3Pack16    = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
    eG12X4B12X4R12X43Plane422Unorm3Pack16    = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
    eG12X4B12X4R12X42Plane422Unorm3Pack16    = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
    eG12X4B12X4R12X43Plane444Unorm3Pack16    = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
    eG16B16G16R16422Unorm                    = VK_FORMAT_G16B16G16R16_422_UNORM,
    eB16G16R16G16422Unorm                    = VK_FORMAT_B16G16R16G16_422_UNORM,
    eG16B16R163Plane420Unorm                 = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
    eG16B16R162Plane420Unorm                 = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
    eG16B16R163Plane422Unorm                 = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
    eG16B16R162Plane422Unorm                 = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
    eG16B16R163Plane444Unorm                 = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
    eG8B8R82Plane444Unorm                    = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM,
    eG10X6B10X6R10X62Plane444Unorm3Pack16    = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
    eG12X4B12X4R12X42Plane444Unorm3Pack16    = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
    eG16B16R162Plane444Unorm                 = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM,
    eA4R4G4B4UnormPack16                     = VK_FORMAT_A4R4G4B4_UNORM_PACK16,
    eA4B4G4R4UnormPack16                     = VK_FORMAT_A4B4G4R4_UNORM_PACK16,
    eAstc4x4SfloatBlock                      = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK,
    eAstc5x4SfloatBlock                      = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK,
    eAstc5x5SfloatBlock                      = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK,
    eAstc6x5SfloatBlock                      = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK,
    eAstc6x6SfloatBlock                      = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK,
    eAstc8x5SfloatBlock                      = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK,
    eAstc8x6SfloatBlock                      = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK,
    eAstc8x8SfloatBlock                      = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK,
    eAstc10x5SfloatBlock                     = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK,
    eAstc10x6SfloatBlock                     = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK,
    eAstc10x8SfloatBlock                     = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK,
    eAstc10x10SfloatBlock                    = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK,
    eAstc12x10SfloatBlock                    = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK,
    eAstc12x12SfloatBlock                    = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK,
    eAstc4x4SfloatBlockEXT                   = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,
    eAstc5x4SfloatBlockEXT                   = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,
    eAstc5x5SfloatBlockEXT                   = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,
    eAstc6x5SfloatBlockEXT                   = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,
    eAstc6x6SfloatBlockEXT                   = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,
    eAstc8x5SfloatBlockEXT                   = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,
    eAstc8x6SfloatBlockEXT                   = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,
    eAstc8x8SfloatBlockEXT                   = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,
    eAstc10x5SfloatBlockEXT                  = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,
    eAstc10x6SfloatBlockEXT                  = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,
    eAstc10x8SfloatBlockEXT                  = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,
    eAstc10x10SfloatBlockEXT                 = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,
    eAstc12x10SfloatBlockEXT                 = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,
    eAstc12x12SfloatBlockEXT                 = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,
    eG8B8R82Plane444UnormEXT                 = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT,
    eG10X6B10X6R10X62Plane444Unorm3Pack16EXT = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT,
    eG12X4B12X4R12X42Plane444Unorm3Pack16EXT = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT,
    eG16B16R162Plane444UnormEXT              = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT,
    eA4R4G4B4UnormPack16EXT                  = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT,
    eA4B4G4R4UnormPack16EXT                  = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT
  };

  enum class FormatFeatureFlagBits : VkFormatFeatureFlags
  {
    eSampledImage                                            = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
    eStorageImage                                            = VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
    eStorageImageAtomic                                      = VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
    eUniformTexelBuffer                                      = VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
    eStorageTexelBuffer                                      = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
    eStorageTexelBufferAtomic                                = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    eVertexBuffer                                            = VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
    eColorAttachment                                         = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
    eColorAttachmentBlend                                    = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
    eDepthStencilAttachment                                  = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
    eBlitSrc                                                 = VK_FORMAT_FEATURE_BLIT_SRC_BIT,
    eBlitDst                                                 = VK_FORMAT_FEATURE_BLIT_DST_BIT,
    eSampledImageFilterLinear                                = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
    eTransferSrc                                             = VK_FORMAT_FEATURE_TRANSFER_SRC_BIT,
    eTransferDst                                             = VK_FORMAT_FEATURE_TRANSFER_DST_BIT,
    eMidpointChromaSamples                                   = VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT,
    eSampledImageYcbcrConversionLinearFilter                 = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,
    eSampledImageYcbcrConversionSeparateReconstructionFilter = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,
    eSampledImageYcbcrConversionChromaReconstructionExplicit = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,
    eSampledImageYcbcrConversionChromaReconstructionExplicitForceable =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,
    eDisjoint                         = VK_FORMAT_FEATURE_DISJOINT_BIT,
    eCositedChromaSamples             = VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT,
    eSampledImageFilterMinmax         = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT,
    eSampledImageFilterCubicEXT       = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT,
    eFragmentShadingRateAttachmentKHR = VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR
  };

  using FormatFeatureFlags = Flags<FormatFeatureFlagBits>;

  template <>
  struct FlagTraits<FormatFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool               isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR FormatFeatureFlags allFlags =
      FormatFeatureFlagBits::eSampledImage | FormatFeatureFlagBits::eStorageImage | FormatFeatureFlagBits::eStorageImageAtomic |
      FormatFeatureFlagBits::eUniformTexelBuffer | FormatFeatureFlagBits::eStorageTexelBuffer | FormatFeatureFlagBits::eStorageTexelBufferAtomic |
      FormatFeatureFlagBits::eVertexBuffer | FormatFeatureFlagBits::eColorAttachment | FormatFeatureFlagBits::eColorAttachmentBlend |
      FormatFeatureFlagBits::eDepthStencilAttachment | FormatFeatureFlagBits::eBlitSrc | FormatFeatureFlagBits::eBlitDst |
      FormatFeatureFlagBits::eSampledImageFilterLinear | FormatFeatureFlagBits::eTransferSrc | FormatFeatureFlagBits::eTransferDst |
      FormatFeatureFlagBits::eMidpointChromaSamples | FormatFeatureFlagBits::eSampledImageYcbcrConversionLinearFilter |
      FormatFeatureFlagBits::eSampledImageYcbcrConversionSeparateReconstructionFilter |
      FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicit |
      FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable | FormatFeatureFlagBits::eDisjoint |
      FormatFeatureFlagBits::eCositedChromaSamples | FormatFeatureFlagBits::eSampledImageFilterMinmax | FormatFeatureFlagBits::eSampledImageFilterCubicEXT |
      FormatFeatureFlagBits::eFragmentShadingRateAttachmentKHR;
  };

  enum class ImageCreateFlagBits : VkImageCreateFlags
  {
    eSparseBinding                     = VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency                   = VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased                     = VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
    eMutableFormat                     = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
    eCubeCompatible                    = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
    eAlias                             = VK_IMAGE_CREATE_ALIAS_BIT,
    eSplitInstanceBindRegions          = VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT,
    e2DArrayCompatible                 = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT,
    eBlockTexelViewCompatible          = VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT,
    eExtendedUsage                     = VK_IMAGE_CREATE_EXTENDED_USAGE_BIT,
    eProtected                         = VK_IMAGE_CREATE_PROTECTED_BIT,
    eDisjoint                          = VK_IMAGE_CREATE_DISJOINT_BIT,
    eSampleLocationsCompatibleDepthEXT = VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT
  };

  using ImageCreateFlags = Flags<ImageCreateFlagBits>;

  template <>
  struct FlagTraits<ImageCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ImageCreateFlags allFlags =
      ImageCreateFlagBits::eSparseBinding | ImageCreateFlagBits::eSparseResidency | ImageCreateFlagBits::eSparseAliased | ImageCreateFlagBits::eMutableFormat |
      ImageCreateFlagBits::eCubeCompatible | ImageCreateFlagBits::eAlias | ImageCreateFlagBits::eSplitInstanceBindRegions |
      ImageCreateFlagBits::e2DArrayCompatible | ImageCreateFlagBits::eBlockTexelViewCompatible | ImageCreateFlagBits::eExtendedUsage |
      ImageCreateFlagBits::eProtected | ImageCreateFlagBits::eDisjoint | ImageCreateFlagBits::eSampleLocationsCompatibleDepthEXT;
  };

  enum class ImageTiling
  {
    eOptimal              = VK_IMAGE_TILING_OPTIMAL,
    eLinear               = VK_IMAGE_TILING_LINEAR,
    eDrmFormatModifierEXT = VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT
  };

  enum class ImageType
  {
    e1D = VK_IMAGE_TYPE_1D,
    e2D = VK_IMAGE_TYPE_2D,
    e3D = VK_IMAGE_TYPE_3D
  };

  enum class ImageUsageFlagBits : VkImageUsageFlags
  {
    eTransferSrc                      = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    eTransferDst                      = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    eSampled                          = VK_IMAGE_USAGE_SAMPLED_BIT,
    eStorage                          = VK_IMAGE_USAGE_STORAGE_BIT,
    eColorAttachment                  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    eDepthStencilAttachment           = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    eTransientAttachment              = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    eInputAttachment                  = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
    eFragmentShadingRateAttachmentKHR = VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR
  };

  using ImageUsageFlags = Flags<ImageUsageFlagBits>;

  template <>
  struct FlagTraits<ImageUsageFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool            isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ImageUsageFlags allFlags =
      ImageUsageFlagBits::eTransferSrc | ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eSampled | ImageUsageFlagBits::eStorage |
      ImageUsageFlagBits::eColorAttachment | ImageUsageFlagBits::eDepthStencilAttachment | ImageUsageFlagBits::eTransientAttachment |
      ImageUsageFlagBits::eInputAttachment | ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR;
  };

  enum class InstanceCreateFlagBits : VkInstanceCreateFlags
  {
  };

  using InstanceCreateFlags = Flags<InstanceCreateFlagBits>;

  template <>
  struct FlagTraits<InstanceCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR InstanceCreateFlags allFlags  = {};
  };

  enum class InternalAllocationType
  {
    eExecutable = VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE
  };

  enum class MemoryHeapFlagBits : VkMemoryHeapFlags
  {
    eDeviceLocal   = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
    eMultiInstance = VK_MEMORY_HEAP_MULTI_INSTANCE_BIT,
    eSeuSafe       = VK_MEMORY_HEAP_SEU_SAFE_BIT
  };

  using MemoryHeapFlags = Flags<MemoryHeapFlagBits>;

  template <>
  struct FlagTraits<MemoryHeapFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool            isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR MemoryHeapFlags allFlags =
      MemoryHeapFlagBits::eDeviceLocal | MemoryHeapFlagBits::eMultiInstance | MemoryHeapFlagBits::eSeuSafe;
  };

  enum class MemoryPropertyFlagBits : VkMemoryPropertyFlags
  {
    eDeviceLocal     = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    eHostVisible     = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    eHostCoherent    = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    eHostCached      = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
    eLazilyAllocated = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
    eProtected       = VK_MEMORY_PROPERTY_PROTECTED_BIT
  };

  using MemoryPropertyFlags = Flags<MemoryPropertyFlagBits>;

  template <>
  struct FlagTraits<MemoryPropertyFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR MemoryPropertyFlags allFlags  = MemoryPropertyFlagBits::eDeviceLocal | MemoryPropertyFlagBits::eHostVisible |
                                                                        MemoryPropertyFlagBits::eHostCoherent | MemoryPropertyFlagBits::eHostCached |
                                                                        MemoryPropertyFlagBits::eLazilyAllocated | MemoryPropertyFlagBits::eProtected;
  };

  enum class PhysicalDeviceType
  {
    eOther         = VK_PHYSICAL_DEVICE_TYPE_OTHER,
    eIntegratedGpu = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    eDiscreteGpu   = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    eVirtualGpu    = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    eCpu           = VK_PHYSICAL_DEVICE_TYPE_CPU
  };

  enum class QueueFlagBits : VkQueueFlags
  {
    eGraphics  = VK_QUEUE_GRAPHICS_BIT,
    eCompute   = VK_QUEUE_COMPUTE_BIT,
    eTransfer  = VK_QUEUE_TRANSFER_BIT,
    eProtected = VK_QUEUE_PROTECTED_BIT
  };

  using QueueFlags = Flags<QueueFlagBits>;

  template <>
  struct FlagTraits<QueueFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool       isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR QueueFlags allFlags =
      QueueFlagBits::eGraphics | QueueFlagBits::eCompute | QueueFlagBits::eTransfer | QueueFlagBits::eProtected;
  };

  enum class SampleCountFlagBits : VkSampleCountFlags
  {
    e1  = VK_SAMPLE_COUNT_1_BIT,
    e2  = VK_SAMPLE_COUNT_2_BIT,
    e4  = VK_SAMPLE_COUNT_4_BIT,
    e8  = VK_SAMPLE_COUNT_8_BIT,
    e16 = VK_SAMPLE_COUNT_16_BIT,
    e32 = VK_SAMPLE_COUNT_32_BIT,
    e64 = VK_SAMPLE_COUNT_64_BIT
  };

  using SampleCountFlags = Flags<SampleCountFlagBits>;

  template <>
  struct FlagTraits<SampleCountFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SampleCountFlags allFlags  = SampleCountFlagBits::e1 | SampleCountFlagBits::e2 | SampleCountFlagBits::e4 |
                                                                     SampleCountFlagBits::e8 | SampleCountFlagBits::e16 | SampleCountFlagBits::e32 |
                                                                     SampleCountFlagBits::e64;
  };

  enum class SystemAllocationScope
  {
    eCommand  = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND,
    eObject   = VK_SYSTEM_ALLOCATION_SCOPE_OBJECT,
    eCache    = VK_SYSTEM_ALLOCATION_SCOPE_CACHE,
    eDevice   = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE,
    eInstance = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE
  };

  enum class DeviceCreateFlagBits : VkDeviceCreateFlags
  {
  };

  using DeviceCreateFlags = Flags<DeviceCreateFlagBits>;

  template <>
  struct FlagTraits<DeviceCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DeviceCreateFlags allFlags  = {};
  };

  enum class DeviceQueueCreateFlagBits : VkDeviceQueueCreateFlags
  {
    eProtected = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT
  };

  using DeviceQueueCreateFlags = Flags<DeviceQueueCreateFlagBits>;

  template <>
  struct FlagTraits<DeviceQueueCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DeviceQueueCreateFlags allFlags  = DeviceQueueCreateFlagBits::eProtected;
  };

  enum class PipelineStageFlagBits : VkPipelineStageFlags
  {
    eTopOfPipe                        = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    eDrawIndirect                     = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
    eVertexInput                      = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    eVertexShader                     = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
    eTessellationControlShader        = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
    eTessellationEvaluationShader     = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
    eGeometryShader                   = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
    eFragmentShader                   = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    eEarlyFragmentTests               = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    eLateFragmentTests                = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
    eColorAttachmentOutput            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    eComputeShader                    = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    eTransfer                         = VK_PIPELINE_STAGE_TRANSFER_BIT,
    eBottomOfPipe                     = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
    eHost                             = VK_PIPELINE_STAGE_HOST_BIT,
    eAllGraphics                      = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
    eAllCommands                      = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    eNone                             = VK_PIPELINE_STAGE_NONE,
    eFragmentShadingRateAttachmentKHR = VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
    eNoneKHR                          = VK_PIPELINE_STAGE_NONE_KHR
  };

  using PipelineStageFlags = Flags<PipelineStageFlagBits>;

  template <>
  struct FlagTraits<PipelineStageFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool               isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineStageFlags allFlags =
      PipelineStageFlagBits::eTopOfPipe | PipelineStageFlagBits::eDrawIndirect | PipelineStageFlagBits::eVertexInput | PipelineStageFlagBits::eVertexShader |
      PipelineStageFlagBits::eTessellationControlShader | PipelineStageFlagBits::eTessellationEvaluationShader | PipelineStageFlagBits::eGeometryShader |
      PipelineStageFlagBits::eFragmentShader | PipelineStageFlagBits::eEarlyFragmentTests | PipelineStageFlagBits::eLateFragmentTests |
      PipelineStageFlagBits::eColorAttachmentOutput | PipelineStageFlagBits::eComputeShader | PipelineStageFlagBits::eTransfer |
      PipelineStageFlagBits::eBottomOfPipe | PipelineStageFlagBits::eHost | PipelineStageFlagBits::eAllGraphics | PipelineStageFlagBits::eAllCommands |
      PipelineStageFlagBits::eNone | PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR;
  };

  enum class MemoryMapFlagBits : VkMemoryMapFlags
  {
  };

  using MemoryMapFlags = Flags<MemoryMapFlagBits>;

  template <>
  struct FlagTraits<MemoryMapFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool           isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR MemoryMapFlags allFlags  = {};
  };

  enum class ImageAspectFlagBits : VkImageAspectFlags
  {
    eColor           = VK_IMAGE_ASPECT_COLOR_BIT,
    eDepth           = VK_IMAGE_ASPECT_DEPTH_BIT,
    eStencil         = VK_IMAGE_ASPECT_STENCIL_BIT,
    eMetadata        = VK_IMAGE_ASPECT_METADATA_BIT,
    ePlane0          = VK_IMAGE_ASPECT_PLANE_0_BIT,
    ePlane1          = VK_IMAGE_ASPECT_PLANE_1_BIT,
    ePlane2          = VK_IMAGE_ASPECT_PLANE_2_BIT,
    eNone            = VK_IMAGE_ASPECT_NONE,
    eMemoryPlane0EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT,
    eMemoryPlane1EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT,
    eMemoryPlane2EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT,
    eMemoryPlane3EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT
  };

  using ImageAspectFlags = Flags<ImageAspectFlagBits>;

  template <>
  struct FlagTraits<ImageAspectFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ImageAspectFlags allFlags = ImageAspectFlagBits::eColor | ImageAspectFlagBits::eDepth | ImageAspectFlagBits::eStencil |
                                                                     ImageAspectFlagBits::eMetadata | ImageAspectFlagBits::ePlane0 |
                                                                     ImageAspectFlagBits::ePlane1 | ImageAspectFlagBits::ePlane2 | ImageAspectFlagBits::eNone |
                                                                     ImageAspectFlagBits::eMemoryPlane0EXT | ImageAspectFlagBits::eMemoryPlane1EXT |
                                                                     ImageAspectFlagBits::eMemoryPlane2EXT | ImageAspectFlagBits::eMemoryPlane3EXT;
  };

  enum class FenceCreateFlagBits : VkFenceCreateFlags
  {
    eSignaled = VK_FENCE_CREATE_SIGNALED_BIT
  };

  using FenceCreateFlags = Flags<FenceCreateFlagBits>;

  template <>
  struct FlagTraits<FenceCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR FenceCreateFlags allFlags  = FenceCreateFlagBits::eSignaled;
  };

  enum class SemaphoreCreateFlagBits : VkSemaphoreCreateFlags
  {
  };

  using SemaphoreCreateFlags = Flags<SemaphoreCreateFlagBits>;

  template <>
  struct FlagTraits<SemaphoreCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SemaphoreCreateFlags allFlags  = {};
  };

  enum class EventCreateFlagBits : VkEventCreateFlags
  {
    eDeviceOnly    = VK_EVENT_CREATE_DEVICE_ONLY_BIT,
    eDeviceOnlyKHR = VK_EVENT_CREATE_DEVICE_ONLY_BIT_KHR
  };

  using EventCreateFlags = Flags<EventCreateFlagBits>;

  template <>
  struct FlagTraits<EventCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR EventCreateFlags allFlags  = EventCreateFlagBits::eDeviceOnly;
  };

  enum class QueryPipelineStatisticFlagBits : VkQueryPipelineStatisticFlags
  {
    eInputAssemblyVertices                   = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
    eInputAssemblyPrimitives                 = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
    eVertexShaderInvocations                 = VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
    eGeometryShaderInvocations               = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
    eGeometryShaderPrimitives                = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
    eClippingInvocations                     = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
    eClippingPrimitives                      = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
    eFragmentShaderInvocations               = VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
    eTessellationControlShaderPatches        = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
    eTessellationEvaluationShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
    eComputeShaderInvocations                = VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT
  };

  using QueryPipelineStatisticFlags = Flags<QueryPipelineStatisticFlagBits>;

  template <>
  struct FlagTraits<QueryPipelineStatisticFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                        isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR QueryPipelineStatisticFlags allFlags =
      QueryPipelineStatisticFlagBits::eInputAssemblyVertices | QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives |
      QueryPipelineStatisticFlagBits::eVertexShaderInvocations | QueryPipelineStatisticFlagBits::eGeometryShaderInvocations |
      QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives | QueryPipelineStatisticFlagBits::eClippingInvocations |
      QueryPipelineStatisticFlagBits::eClippingPrimitives | QueryPipelineStatisticFlagBits::eFragmentShaderInvocations |
      QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches | QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations |
      QueryPipelineStatisticFlagBits::eComputeShaderInvocations;
  };

  enum class QueryResultFlagBits : VkQueryResultFlags
  {
    e64               = VK_QUERY_RESULT_64_BIT,
    eWait             = VK_QUERY_RESULT_WAIT_BIT,
    eWithAvailability = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT,
    ePartial          = VK_QUERY_RESULT_PARTIAL_BIT
  };

  using QueryResultFlags = Flags<QueryResultFlagBits>;

  template <>
  struct FlagTraits<QueryResultFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR QueryResultFlags allFlags =
      QueryResultFlagBits::e64 | QueryResultFlagBits::eWait | QueryResultFlagBits::eWithAvailability | QueryResultFlagBits::ePartial;
  };

  enum class QueryType
  {
    eOcclusion           = VK_QUERY_TYPE_OCCLUSION,
    ePipelineStatistics  = VK_QUERY_TYPE_PIPELINE_STATISTICS,
    eTimestamp           = VK_QUERY_TYPE_TIMESTAMP,
    ePerformanceQueryKHR = VK_QUERY_TYPE_PERFORMANCE_QUERY_KHR
  };

  enum class QueryPoolCreateFlagBits : VkQueryPoolCreateFlags
  {
  };

  using QueryPoolCreateFlags = Flags<QueryPoolCreateFlagBits>;

  template <>
  struct FlagTraits<QueryPoolCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR QueryPoolCreateFlags allFlags  = {};
  };

  enum class BufferCreateFlagBits : VkBufferCreateFlags
  {
    eSparseBinding              = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency            = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased              = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT,
    eProtected                  = VK_BUFFER_CREATE_PROTECTED_BIT,
    eDeviceAddressCaptureReplay = VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT
  };

  using BufferCreateFlags = Flags<BufferCreateFlagBits>;

  template <>
  struct FlagTraits<BufferCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR BufferCreateFlags allFlags  = BufferCreateFlagBits::eSparseBinding | BufferCreateFlagBits::eSparseResidency |
                                                                      BufferCreateFlagBits::eSparseAliased | BufferCreateFlagBits::eProtected |
                                                                      BufferCreateFlagBits::eDeviceAddressCaptureReplay;
  };

  enum class BufferUsageFlagBits : VkBufferUsageFlags
  {
    eTransferSrc         = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    eTransferDst         = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    eUniformTexelBuffer  = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
    eStorageTexelBuffer  = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
    eUniformBuffer       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    eStorageBuffer       = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    eIndexBuffer         = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    eVertexBuffer        = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    eIndirectBuffer      = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
    eShaderDeviceAddress = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
  };

  using BufferUsageFlags = Flags<BufferUsageFlagBits>;

  template <>
  struct FlagTraits<BufferUsageFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR BufferUsageFlags allFlags =
      BufferUsageFlagBits::eTransferSrc | BufferUsageFlagBits::eTransferDst | BufferUsageFlagBits::eUniformTexelBuffer |
      BufferUsageFlagBits::eStorageTexelBuffer | BufferUsageFlagBits::eUniformBuffer | BufferUsageFlagBits::eStorageBuffer | BufferUsageFlagBits::eIndexBuffer |
      BufferUsageFlagBits::eVertexBuffer | BufferUsageFlagBits::eIndirectBuffer | BufferUsageFlagBits::eShaderDeviceAddress;
  };

  enum class SharingMode
  {
    eExclusive  = VK_SHARING_MODE_EXCLUSIVE,
    eConcurrent = VK_SHARING_MODE_CONCURRENT
  };

  enum class BufferViewCreateFlagBits : VkBufferViewCreateFlags
  {
  };

  using BufferViewCreateFlags = Flags<BufferViewCreateFlagBits>;

  template <>
  struct FlagTraits<BufferViewCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR BufferViewCreateFlags allFlags  = {};
  };

  enum class ImageLayout
  {
    eUndefined                               = VK_IMAGE_LAYOUT_UNDEFINED,
    eGeneral                                 = VK_IMAGE_LAYOUT_GENERAL,
    eColorAttachmentOptimal                  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    eDepthStencilAttachmentOptimal           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    eDepthStencilReadOnlyOptimal             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    eShaderReadOnlyOptimal                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    eTransferSrcOptimal                      = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    eTransferDstOptimal                      = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    ePreinitialized                          = VK_IMAGE_LAYOUT_PREINITIALIZED,
    eDepthReadOnlyStencilAttachmentOptimal   = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    eDepthAttachmentStencilReadOnlyOptimal   = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    eDepthAttachmentOptimal                  = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    eDepthReadOnlyOptimal                    = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
    eStencilAttachmentOptimal                = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
    eStencilReadOnlyOptimal                  = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
    eReadOnlyOptimal                         = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
    eAttachmentOptimal                       = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    ePresentSrcKHR                           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    eSharedPresentKHR                        = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR,
    eFragmentShadingRateAttachmentOptimalKHR = VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR,
    eReadOnlyOptimalKHR                      = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR,
    eAttachmentOptimalKHR                    = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR
  };

  enum class ComponentSwizzle
  {
    eIdentity = VK_COMPONENT_SWIZZLE_IDENTITY,
    eZero     = VK_COMPONENT_SWIZZLE_ZERO,
    eOne      = VK_COMPONENT_SWIZZLE_ONE,
    eR        = VK_COMPONENT_SWIZZLE_R,
    eG        = VK_COMPONENT_SWIZZLE_G,
    eB        = VK_COMPONENT_SWIZZLE_B,
    eA        = VK_COMPONENT_SWIZZLE_A
  };

  enum class ImageViewCreateFlagBits : VkImageViewCreateFlags
  {
  };

  using ImageViewCreateFlags = Flags<ImageViewCreateFlagBits>;

  template <>
  struct FlagTraits<ImageViewCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ImageViewCreateFlags allFlags  = {};
  };

  enum class ImageViewType
  {
    e1D        = VK_IMAGE_VIEW_TYPE_1D,
    e2D        = VK_IMAGE_VIEW_TYPE_2D,
    e3D        = VK_IMAGE_VIEW_TYPE_3D,
    eCube      = VK_IMAGE_VIEW_TYPE_CUBE,
    e1DArray   = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    e2DArray   = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    eCubeArray = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
  };

  enum class PipelineCacheCreateFlagBits : VkPipelineCacheCreateFlags
  {
    eExternallySynchronized = VK_PIPELINE_CACHE_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,
    eReadOnly               = VK_PIPELINE_CACHE_CREATE_READ_ONLY_BIT,
    eUseApplicationStorage  = VK_PIPELINE_CACHE_CREATE_USE_APPLICATION_STORAGE_BIT
  };

  using PipelineCacheCreateFlags = Flags<PipelineCacheCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineCacheCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                     isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineCacheCreateFlags allFlags =
      PipelineCacheCreateFlagBits::eExternallySynchronized | PipelineCacheCreateFlagBits::eReadOnly | PipelineCacheCreateFlagBits::eUseApplicationStorage;
  };

  enum class BlendFactor
  {
    eZero                  = VK_BLEND_FACTOR_ZERO,
    eOne                   = VK_BLEND_FACTOR_ONE,
    eSrcColor              = VK_BLEND_FACTOR_SRC_COLOR,
    eOneMinusSrcColor      = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    eDstColor              = VK_BLEND_FACTOR_DST_COLOR,
    eOneMinusDstColor      = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    eSrcAlpha              = VK_BLEND_FACTOR_SRC_ALPHA,
    eOneMinusSrcAlpha      = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    eDstAlpha              = VK_BLEND_FACTOR_DST_ALPHA,
    eOneMinusDstAlpha      = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    eConstantColor         = VK_BLEND_FACTOR_CONSTANT_COLOR,
    eOneMinusConstantColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    eConstantAlpha         = VK_BLEND_FACTOR_CONSTANT_ALPHA,
    eOneMinusConstantAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    eSrcAlphaSaturate      = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    eSrc1Color             = VK_BLEND_FACTOR_SRC1_COLOR,
    eOneMinusSrc1Color     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    eSrc1Alpha             = VK_BLEND_FACTOR_SRC1_ALPHA,
    eOneMinusSrc1Alpha     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
  };

  enum class BlendOp
  {
    eAdd                 = VK_BLEND_OP_ADD,
    eSubtract            = VK_BLEND_OP_SUBTRACT,
    eReverseSubtract     = VK_BLEND_OP_REVERSE_SUBTRACT,
    eMin                 = VK_BLEND_OP_MIN,
    eMax                 = VK_BLEND_OP_MAX,
    eZeroEXT             = VK_BLEND_OP_ZERO_EXT,
    eSrcEXT              = VK_BLEND_OP_SRC_EXT,
    eDstEXT              = VK_BLEND_OP_DST_EXT,
    eSrcOverEXT          = VK_BLEND_OP_SRC_OVER_EXT,
    eDstOverEXT          = VK_BLEND_OP_DST_OVER_EXT,
    eSrcInEXT            = VK_BLEND_OP_SRC_IN_EXT,
    eDstInEXT            = VK_BLEND_OP_DST_IN_EXT,
    eSrcOutEXT           = VK_BLEND_OP_SRC_OUT_EXT,
    eDstOutEXT           = VK_BLEND_OP_DST_OUT_EXT,
    eSrcAtopEXT          = VK_BLEND_OP_SRC_ATOP_EXT,
    eDstAtopEXT          = VK_BLEND_OP_DST_ATOP_EXT,
    eXorEXT              = VK_BLEND_OP_XOR_EXT,
    eMultiplyEXT         = VK_BLEND_OP_MULTIPLY_EXT,
    eScreenEXT           = VK_BLEND_OP_SCREEN_EXT,
    eOverlayEXT          = VK_BLEND_OP_OVERLAY_EXT,
    eDarkenEXT           = VK_BLEND_OP_DARKEN_EXT,
    eLightenEXT          = VK_BLEND_OP_LIGHTEN_EXT,
    eColordodgeEXT       = VK_BLEND_OP_COLORDODGE_EXT,
    eColorburnEXT        = VK_BLEND_OP_COLORBURN_EXT,
    eHardlightEXT        = VK_BLEND_OP_HARDLIGHT_EXT,
    eSoftlightEXT        = VK_BLEND_OP_SOFTLIGHT_EXT,
    eDifferenceEXT       = VK_BLEND_OP_DIFFERENCE_EXT,
    eExclusionEXT        = VK_BLEND_OP_EXCLUSION_EXT,
    eInvertEXT           = VK_BLEND_OP_INVERT_EXT,
    eInvertRgbEXT        = VK_BLEND_OP_INVERT_RGB_EXT,
    eLineardodgeEXT      = VK_BLEND_OP_LINEARDODGE_EXT,
    eLinearburnEXT       = VK_BLEND_OP_LINEARBURN_EXT,
    eVividlightEXT       = VK_BLEND_OP_VIVIDLIGHT_EXT,
    eLinearlightEXT      = VK_BLEND_OP_LINEARLIGHT_EXT,
    ePinlightEXT         = VK_BLEND_OP_PINLIGHT_EXT,
    eHardmixEXT          = VK_BLEND_OP_HARDMIX_EXT,
    eHslHueEXT           = VK_BLEND_OP_HSL_HUE_EXT,
    eHslSaturationEXT    = VK_BLEND_OP_HSL_SATURATION_EXT,
    eHslColorEXT         = VK_BLEND_OP_HSL_COLOR_EXT,
    eHslLuminosityEXT    = VK_BLEND_OP_HSL_LUMINOSITY_EXT,
    ePlusEXT             = VK_BLEND_OP_PLUS_EXT,
    ePlusClampedEXT      = VK_BLEND_OP_PLUS_CLAMPED_EXT,
    ePlusClampedAlphaEXT = VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
    ePlusDarkerEXT       = VK_BLEND_OP_PLUS_DARKER_EXT,
    eMinusEXT            = VK_BLEND_OP_MINUS_EXT,
    eMinusClampedEXT     = VK_BLEND_OP_MINUS_CLAMPED_EXT,
    eContrastEXT         = VK_BLEND_OP_CONTRAST_EXT,
    eInvertOvgEXT        = VK_BLEND_OP_INVERT_OVG_EXT,
    eRedEXT              = VK_BLEND_OP_RED_EXT,
    eGreenEXT            = VK_BLEND_OP_GREEN_EXT,
    eBlueEXT             = VK_BLEND_OP_BLUE_EXT
  };

  enum class ColorComponentFlagBits : VkColorComponentFlags
  {
    eR = VK_COLOR_COMPONENT_R_BIT,
    eG = VK_COLOR_COMPONENT_G_BIT,
    eB = VK_COLOR_COMPONENT_B_BIT,
    eA = VK_COLOR_COMPONENT_A_BIT
  };

  using ColorComponentFlags = Flags<ColorComponentFlagBits>;

  template <>
  struct FlagTraits<ColorComponentFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ColorComponentFlags allFlags =
      ColorComponentFlagBits::eR | ColorComponentFlagBits::eG | ColorComponentFlagBits::eB | ColorComponentFlagBits::eA;
  };

  enum class CompareOp
  {
    eNever          = VK_COMPARE_OP_NEVER,
    eLess           = VK_COMPARE_OP_LESS,
    eEqual          = VK_COMPARE_OP_EQUAL,
    eLessOrEqual    = VK_COMPARE_OP_LESS_OR_EQUAL,
    eGreater        = VK_COMPARE_OP_GREATER,
    eNotEqual       = VK_COMPARE_OP_NOT_EQUAL,
    eGreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
    eAlways         = VK_COMPARE_OP_ALWAYS
  };

  enum class CullModeFlagBits : VkCullModeFlags
  {
    eNone         = VK_CULL_MODE_NONE,
    eFront        = VK_CULL_MODE_FRONT_BIT,
    eBack         = VK_CULL_MODE_BACK_BIT,
    eFrontAndBack = VK_CULL_MODE_FRONT_AND_BACK
  };

  using CullModeFlags = Flags<CullModeFlagBits>;

  template <>
  struct FlagTraits<CullModeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CullModeFlags allFlags =
      CullModeFlagBits::eNone | CullModeFlagBits::eFront | CullModeFlagBits::eBack | CullModeFlagBits::eFrontAndBack;
  };

  enum class DynamicState
  {
    eViewport                    = VK_DYNAMIC_STATE_VIEWPORT,
    eScissor                     = VK_DYNAMIC_STATE_SCISSOR,
    eLineWidth                   = VK_DYNAMIC_STATE_LINE_WIDTH,
    eDepthBias                   = VK_DYNAMIC_STATE_DEPTH_BIAS,
    eBlendConstants              = VK_DYNAMIC_STATE_BLEND_CONSTANTS,
    eDepthBounds                 = VK_DYNAMIC_STATE_DEPTH_BOUNDS,
    eStencilCompareMask          = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
    eStencilWriteMask            = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
    eStencilReference            = VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    eCullMode                    = VK_DYNAMIC_STATE_CULL_MODE,
    eFrontFace                   = VK_DYNAMIC_STATE_FRONT_FACE,
    ePrimitiveTopology           = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
    eViewportWithCount           = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
    eScissorWithCount            = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT,
    eVertexInputBindingStride    = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE,
    eDepthTestEnable             = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
    eDepthWriteEnable            = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
    eDepthCompareOp              = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP,
    eDepthBoundsTestEnable       = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE,
    eStencilTestEnable           = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,
    eStencilOp                   = VK_DYNAMIC_STATE_STENCIL_OP,
    eRasterizerDiscardEnable     = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE,
    eDepthBiasEnable             = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE,
    ePrimitiveRestartEnable      = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE,
    eDiscardRectangleEXT         = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT,
    eDiscardRectangleEnableEXT   = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT,
    eDiscardRectangleModeEXT     = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT,
    eSampleLocationsEXT          = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT,
    eFragmentShadingRateKHR      = VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR,
    eLineStippleEXT              = VK_DYNAMIC_STATE_LINE_STIPPLE_EXT,
    eCullModeEXT                 = VK_DYNAMIC_STATE_CULL_MODE_EXT,
    eFrontFaceEXT                = VK_DYNAMIC_STATE_FRONT_FACE_EXT,
    ePrimitiveTopologyEXT        = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT,
    eViewportWithCountEXT        = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT,
    eScissorWithCountEXT         = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT,
    eVertexInputBindingStrideEXT = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT,
    eDepthTestEnableEXT          = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT,
    eDepthWriteEnableEXT         = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT,
    eDepthCompareOpEXT           = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT,
    eDepthBoundsTestEnableEXT    = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT,
    eStencilTestEnableEXT        = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT,
    eStencilOpEXT                = VK_DYNAMIC_STATE_STENCIL_OP_EXT,
    eVertexInputEXT              = VK_DYNAMIC_STATE_VERTEX_INPUT_EXT,
    ePatchControlPointsEXT       = VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT,
    eRasterizerDiscardEnableEXT  = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE_EXT,
    eDepthBiasEnableEXT          = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE_EXT,
    eLogicOpEXT                  = VK_DYNAMIC_STATE_LOGIC_OP_EXT,
    ePrimitiveRestartEnableEXT   = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE_EXT,
    eColorWriteEnableEXT         = VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT
  };

  enum class FrontFace
  {
    eCounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    eClockwise        = VK_FRONT_FACE_CLOCKWISE
  };

  enum class LogicOp
  {
    eClear        = VK_LOGIC_OP_CLEAR,
    eAnd          = VK_LOGIC_OP_AND,
    eAndReverse   = VK_LOGIC_OP_AND_REVERSE,
    eCopy         = VK_LOGIC_OP_COPY,
    eAndInverted  = VK_LOGIC_OP_AND_INVERTED,
    eNoOp         = VK_LOGIC_OP_NO_OP,
    eXor          = VK_LOGIC_OP_XOR,
    eOr           = VK_LOGIC_OP_OR,
    eNor          = VK_LOGIC_OP_NOR,
    eEquivalent   = VK_LOGIC_OP_EQUIVALENT,
    eInvert       = VK_LOGIC_OP_INVERT,
    eOrReverse    = VK_LOGIC_OP_OR_REVERSE,
    eCopyInverted = VK_LOGIC_OP_COPY_INVERTED,
    eOrInverted   = VK_LOGIC_OP_OR_INVERTED,
    eNand         = VK_LOGIC_OP_NAND,
    eSet          = VK_LOGIC_OP_SET
  };

  enum class PipelineCreateFlagBits : VkPipelineCreateFlags
  {
    eDisableOptimization           = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT,
    eViewIndexFromDeviceIndex      = VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT,
    eDispatchBase                  = VK_PIPELINE_CREATE_DISPATCH_BASE_BIT,
    eFailOnPipelineCompileRequired = VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT,
    eEarlyReturnOnFailure          = VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT
  };

  using PipelineCreateFlags = Flags<PipelineCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineCreateFlags allFlags =
      PipelineCreateFlagBits::eDisableOptimization | PipelineCreateFlagBits::eViewIndexFromDeviceIndex | PipelineCreateFlagBits::eDispatchBase |
      PipelineCreateFlagBits::eFailOnPipelineCompileRequired | PipelineCreateFlagBits::eEarlyReturnOnFailure;
  };

  enum class PipelineShaderStageCreateFlagBits : VkPipelineShaderStageCreateFlags
  {
    eAllowVaryingSubgroupSize    = VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT,
    eRequireFullSubgroups        = VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT,
    eAllowVaryingSubgroupSizeEXT = VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT,
    eRequireFullSubgroupsEXT     = VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT
  };

  using PipelineShaderStageCreateFlags = Flags<PipelineShaderStageCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineShaderStageCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                           isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineShaderStageCreateFlags allFlags =
      PipelineShaderStageCreateFlagBits::eAllowVaryingSubgroupSize | PipelineShaderStageCreateFlagBits::eRequireFullSubgroups;
  };

  enum class PolygonMode
  {
    eFill  = VK_POLYGON_MODE_FILL,
    eLine  = VK_POLYGON_MODE_LINE,
    ePoint = VK_POLYGON_MODE_POINT
  };

  enum class PrimitiveTopology
  {
    ePointList                  = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    eLineList                   = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    eLineStrip                  = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    eTriangleList               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    eTriangleStrip              = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    eTriangleFan                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    eLineListWithAdjacency      = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
    eLineStripWithAdjacency     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
    eTriangleListWithAdjacency  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
    eTriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
    ePatchList                  = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
  };

  enum class ShaderStageFlagBits : VkShaderStageFlags
  {
    eVertex                 = VK_SHADER_STAGE_VERTEX_BIT,
    eTessellationControl    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    eTessellationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    eGeometry               = VK_SHADER_STAGE_GEOMETRY_BIT,
    eFragment               = VK_SHADER_STAGE_FRAGMENT_BIT,
    eCompute                = VK_SHADER_STAGE_COMPUTE_BIT,
    eAllGraphics            = VK_SHADER_STAGE_ALL_GRAPHICS,
    eAll                    = VK_SHADER_STAGE_ALL
  };

  using ShaderStageFlags = Flags<ShaderStageFlagBits>;

  template <>
  struct FlagTraits<ShaderStageFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ShaderStageFlags allFlags =
      ShaderStageFlagBits::eVertex | ShaderStageFlagBits::eTessellationControl | ShaderStageFlagBits::eTessellationEvaluation | ShaderStageFlagBits::eGeometry |
      ShaderStageFlagBits::eFragment | ShaderStageFlagBits::eCompute | ShaderStageFlagBits::eAllGraphics | ShaderStageFlagBits::eAll;
  };

  enum class StencilOp
  {
    eKeep              = VK_STENCIL_OP_KEEP,
    eZero              = VK_STENCIL_OP_ZERO,
    eReplace           = VK_STENCIL_OP_REPLACE,
    eIncrementAndClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
    eDecrementAndClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
    eInvert            = VK_STENCIL_OP_INVERT,
    eIncrementAndWrap  = VK_STENCIL_OP_INCREMENT_AND_WRAP,
    eDecrementAndWrap  = VK_STENCIL_OP_DECREMENT_AND_WRAP
  };

  enum class VertexInputRate
  {
    eVertex   = VK_VERTEX_INPUT_RATE_VERTEX,
    eInstance = VK_VERTEX_INPUT_RATE_INSTANCE
  };

  enum class PipelineColorBlendStateCreateFlagBits : VkPipelineColorBlendStateCreateFlags
  {
  };

  using PipelineColorBlendStateCreateFlags = Flags<PipelineColorBlendStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineColorBlendStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                               isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineColorBlendStateCreateFlags allFlags  = {};
  };

  enum class PipelineDepthStencilStateCreateFlagBits : VkPipelineDepthStencilStateCreateFlags
  {
  };

  using PipelineDepthStencilStateCreateFlags = Flags<PipelineDepthStencilStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineDepthStencilStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineDepthStencilStateCreateFlags allFlags  = {};
  };

  enum class PipelineDynamicStateCreateFlagBits : VkPipelineDynamicStateCreateFlags
  {
  };

  using PipelineDynamicStateCreateFlags = Flags<PipelineDynamicStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineDynamicStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                            isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineDynamicStateCreateFlags allFlags  = {};
  };

  enum class PipelineInputAssemblyStateCreateFlagBits : VkPipelineInputAssemblyStateCreateFlags
  {
  };

  using PipelineInputAssemblyStateCreateFlags = Flags<PipelineInputAssemblyStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineInputAssemblyStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineInputAssemblyStateCreateFlags allFlags  = {};
  };

  enum class PipelineLayoutCreateFlagBits : VkPipelineLayoutCreateFlags
  {
  };

  using PipelineLayoutCreateFlags = Flags<PipelineLayoutCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineLayoutCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                      isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineLayoutCreateFlags allFlags  = {};
  };

  enum class PipelineMultisampleStateCreateFlagBits : VkPipelineMultisampleStateCreateFlags
  {
  };

  using PipelineMultisampleStateCreateFlags = Flags<PipelineMultisampleStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineMultisampleStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineMultisampleStateCreateFlags allFlags  = {};
  };

  enum class PipelineRasterizationStateCreateFlagBits : VkPipelineRasterizationStateCreateFlags
  {
  };

  using PipelineRasterizationStateCreateFlags = Flags<PipelineRasterizationStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineRasterizationStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineRasterizationStateCreateFlags allFlags  = {};
  };

  enum class PipelineTessellationStateCreateFlagBits : VkPipelineTessellationStateCreateFlags
  {
  };

  using PipelineTessellationStateCreateFlags = Flags<PipelineTessellationStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineTessellationStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineTessellationStateCreateFlags allFlags  = {};
  };

  enum class PipelineVertexInputStateCreateFlagBits : VkPipelineVertexInputStateCreateFlags
  {
  };

  using PipelineVertexInputStateCreateFlags = Flags<PipelineVertexInputStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineVertexInputStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineVertexInputStateCreateFlags allFlags  = {};
  };

  enum class PipelineViewportStateCreateFlagBits : VkPipelineViewportStateCreateFlags
  {
  };

  using PipelineViewportStateCreateFlags = Flags<PipelineViewportStateCreateFlagBits>;

  template <>
  struct FlagTraits<PipelineViewportStateCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineViewportStateCreateFlags allFlags  = {};
  };

  enum class BorderColor
  {
    eFloatTransparentBlack = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
    eIntTransparentBlack   = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
    eFloatOpaqueBlack      = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
    eIntOpaqueBlack        = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    eFloatOpaqueWhite      = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    eIntOpaqueWhite        = VK_BORDER_COLOR_INT_OPAQUE_WHITE,
    eFloatCustomEXT        = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT,
    eIntCustomEXT          = VK_BORDER_COLOR_INT_CUSTOM_EXT
  };

  enum class Filter
  {
    eNearest  = VK_FILTER_NEAREST,
    eLinear   = VK_FILTER_LINEAR,
    eCubicEXT = VK_FILTER_CUBIC_EXT
  };

  enum class SamplerAddressMode
  {
    eRepeat            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    eMirroredRepeat    = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    eClampToEdge       = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    eClampToBorder     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    eMirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
  };

  enum class SamplerCreateFlagBits : VkSamplerCreateFlags
  {
  };

  using SamplerCreateFlags = Flags<SamplerCreateFlagBits>;

  template <>
  struct FlagTraits<SamplerCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool               isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SamplerCreateFlags allFlags  = {};
  };

  enum class SamplerMipmapMode
  {
    eNearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    eLinear  = VK_SAMPLER_MIPMAP_MODE_LINEAR
  };

  enum class DescriptorPoolCreateFlagBits : VkDescriptorPoolCreateFlags
  {
    eFreeDescriptorSet = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    eUpdateAfterBind   = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT
  };

  using DescriptorPoolCreateFlags = Flags<DescriptorPoolCreateFlagBits>;

  template <>
  struct FlagTraits<DescriptorPoolCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                      isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DescriptorPoolCreateFlags allFlags =
      DescriptorPoolCreateFlagBits::eFreeDescriptorSet | DescriptorPoolCreateFlagBits::eUpdateAfterBind;
  };

  enum class DescriptorSetLayoutCreateFlagBits : VkDescriptorSetLayoutCreateFlags
  {
    eUpdateAfterBindPool = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT
  };

  using DescriptorSetLayoutCreateFlags = Flags<DescriptorSetLayoutCreateFlagBits>;

  template <>
  struct FlagTraits<DescriptorSetLayoutCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                           isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DescriptorSetLayoutCreateFlags allFlags  = DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool;
  };

  enum class DescriptorType
  {
    eSampler              = VK_DESCRIPTOR_TYPE_SAMPLER,
    eCombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    eSampledImage         = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    eStorageImage         = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    eUniformTexelBuffer   = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    eStorageTexelBuffer   = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    eUniformBuffer        = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    eStorageBuffer        = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    eUniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    eStorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    eInputAttachment      = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    eInlineUniformBlock   = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK
  };

  enum class DescriptorPoolResetFlagBits : VkDescriptorPoolResetFlags
  {
  };

  using DescriptorPoolResetFlags = Flags<DescriptorPoolResetFlagBits>;

  template <>
  struct FlagTraits<DescriptorPoolResetFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                     isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DescriptorPoolResetFlags allFlags  = {};
  };

  enum class AccessFlagBits : VkAccessFlags
  {
    eIndirectCommandRead                  = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    eIndexRead                            = VK_ACCESS_INDEX_READ_BIT,
    eVertexAttributeRead                  = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    eUniformRead                          = VK_ACCESS_UNIFORM_READ_BIT,
    eInputAttachmentRead                  = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    eShaderRead                           = VK_ACCESS_SHADER_READ_BIT,
    eShaderWrite                          = VK_ACCESS_SHADER_WRITE_BIT,
    eColorAttachmentRead                  = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    eColorAttachmentWrite                 = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    eDepthStencilAttachmentRead           = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    eDepthStencilAttachmentWrite          = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    eTransferRead                         = VK_ACCESS_TRANSFER_READ_BIT,
    eTransferWrite                        = VK_ACCESS_TRANSFER_WRITE_BIT,
    eHostRead                             = VK_ACCESS_HOST_READ_BIT,
    eHostWrite                            = VK_ACCESS_HOST_WRITE_BIT,
    eMemoryRead                           = VK_ACCESS_MEMORY_READ_BIT,
    eMemoryWrite                          = VK_ACCESS_MEMORY_WRITE_BIT,
    eNone                                 = VK_ACCESS_NONE,
    eColorAttachmentReadNoncoherentEXT    = VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT,
    eFragmentShadingRateAttachmentReadKHR = VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR,
    eNoneKHR                              = VK_ACCESS_NONE_KHR
  };

  using AccessFlags = Flags<AccessFlagBits>;

  template <>
  struct FlagTraits<AccessFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool        isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR AccessFlags allFlags =
      AccessFlagBits::eIndirectCommandRead | AccessFlagBits::eIndexRead | AccessFlagBits::eVertexAttributeRead | AccessFlagBits::eUniformRead |
      AccessFlagBits::eInputAttachmentRead | AccessFlagBits::eShaderRead | AccessFlagBits::eShaderWrite | AccessFlagBits::eColorAttachmentRead |
      AccessFlagBits::eColorAttachmentWrite | AccessFlagBits::eDepthStencilAttachmentRead | AccessFlagBits::eDepthStencilAttachmentWrite |
      AccessFlagBits::eTransferRead | AccessFlagBits::eTransferWrite | AccessFlagBits::eHostRead | AccessFlagBits::eHostWrite | AccessFlagBits::eMemoryRead |
      AccessFlagBits::eMemoryWrite | AccessFlagBits::eNone | AccessFlagBits::eColorAttachmentReadNoncoherentEXT |
      AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
  };

  enum class AttachmentDescriptionFlagBits : VkAttachmentDescriptionFlags
  {
    eMayAlias = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT
  };

  using AttachmentDescriptionFlags = Flags<AttachmentDescriptionFlagBits>;

  template <>
  struct FlagTraits<AttachmentDescriptionFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                       isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR AttachmentDescriptionFlags allFlags  = AttachmentDescriptionFlagBits::eMayAlias;
  };

  enum class AttachmentLoadOp
  {
    eLoad     = VK_ATTACHMENT_LOAD_OP_LOAD,
    eClear    = VK_ATTACHMENT_LOAD_OP_CLEAR,
    eDontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE
  };

  enum class AttachmentStoreOp
  {
    eStore    = VK_ATTACHMENT_STORE_OP_STORE,
    eDontCare = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    eNone     = VK_ATTACHMENT_STORE_OP_NONE
  };

  enum class DependencyFlagBits : VkDependencyFlags
  {
    eByRegion    = VK_DEPENDENCY_BY_REGION_BIT,
    eDeviceGroup = VK_DEPENDENCY_DEVICE_GROUP_BIT,
    eViewLocal   = VK_DEPENDENCY_VIEW_LOCAL_BIT
  };

  using DependencyFlags = Flags<DependencyFlagBits>;

  template <>
  struct FlagTraits<DependencyFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool            isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DependencyFlags allFlags =
      DependencyFlagBits::eByRegion | DependencyFlagBits::eDeviceGroup | DependencyFlagBits::eViewLocal;
  };

  enum class FramebufferCreateFlagBits : VkFramebufferCreateFlags
  {
    eImageless = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT
  };

  using FramebufferCreateFlags = Flags<FramebufferCreateFlagBits>;

  template <>
  struct FlagTraits<FramebufferCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR FramebufferCreateFlags allFlags  = FramebufferCreateFlagBits::eImageless;
  };

  enum class PipelineBindPoint
  {
    eGraphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
    eCompute  = VK_PIPELINE_BIND_POINT_COMPUTE
  };

  enum class RenderPassCreateFlagBits : VkRenderPassCreateFlags
  {
  };

  using RenderPassCreateFlags = Flags<RenderPassCreateFlagBits>;

  template <>
  struct FlagTraits<RenderPassCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR RenderPassCreateFlags allFlags  = {};
  };

  enum class SubpassDescriptionFlagBits : VkSubpassDescriptionFlags
  {
  };

  using SubpassDescriptionFlags = Flags<SubpassDescriptionFlagBits>;

  template <>
  struct FlagTraits<SubpassDescriptionFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                    isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SubpassDescriptionFlags allFlags  = {};
  };

  enum class CommandPoolCreateFlagBits : VkCommandPoolCreateFlags
  {
    eTransient          = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
    eResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    eProtected          = VK_COMMAND_POOL_CREATE_PROTECTED_BIT
  };

  using CommandPoolCreateFlags = Flags<CommandPoolCreateFlagBits>;

  template <>
  struct FlagTraits<CommandPoolCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CommandPoolCreateFlags allFlags =
      CommandPoolCreateFlagBits::eTransient | CommandPoolCreateFlagBits::eResetCommandBuffer | CommandPoolCreateFlagBits::eProtected;
  };

  enum class CommandPoolResetFlagBits : VkCommandPoolResetFlags
  {
  };

  using CommandPoolResetFlags = Flags<CommandPoolResetFlagBits>;

  template <>
  struct FlagTraits<CommandPoolResetFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CommandPoolResetFlags allFlags  = {};
  };

  enum class CommandBufferLevel
  {
    ePrimary   = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    eSecondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
  };

  enum class CommandBufferResetFlagBits : VkCommandBufferResetFlags
  {
    eReleaseResources = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
  };

  using CommandBufferResetFlags = Flags<CommandBufferResetFlagBits>;

  template <>
  struct FlagTraits<CommandBufferResetFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                    isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CommandBufferResetFlags allFlags  = CommandBufferResetFlagBits::eReleaseResources;
  };

  enum class CommandBufferUsageFlagBits : VkCommandBufferUsageFlags
  {
    eOneTimeSubmit      = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    eRenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
    eSimultaneousUse    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
  };

  using CommandBufferUsageFlags = Flags<CommandBufferUsageFlagBits>;

  template <>
  struct FlagTraits<CommandBufferUsageFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                    isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CommandBufferUsageFlags allFlags =
      CommandBufferUsageFlagBits::eOneTimeSubmit | CommandBufferUsageFlagBits::eRenderPassContinue | CommandBufferUsageFlagBits::eSimultaneousUse;
  };

  enum class QueryControlFlagBits : VkQueryControlFlags
  {
    ePrecise = VK_QUERY_CONTROL_PRECISE_BIT
  };

  using QueryControlFlags = Flags<QueryControlFlagBits>;

  template <>
  struct FlagTraits<QueryControlFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR QueryControlFlags allFlags  = QueryControlFlagBits::ePrecise;
  };

  enum class IndexType
  {
    eUint16   = VK_INDEX_TYPE_UINT16,
    eUint32   = VK_INDEX_TYPE_UINT32,
    eUint8EXT = VK_INDEX_TYPE_UINT8_EXT
  };

  enum class StencilFaceFlagBits : VkStencilFaceFlags
  {
    eFront        = VK_STENCIL_FACE_FRONT_BIT,
    eBack         = VK_STENCIL_FACE_BACK_BIT,
    eFrontAndBack = VK_STENCIL_FACE_FRONT_AND_BACK
  };

  using StencilFaceFlags = Flags<StencilFaceFlagBits>;

  template <>
  struct FlagTraits<StencilFaceFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR StencilFaceFlags allFlags =
      StencilFaceFlagBits::eFront | StencilFaceFlagBits::eBack | StencilFaceFlagBits::eFrontAndBack;
  };

  enum class SubpassContents
  {
    eInline                  = VK_SUBPASS_CONTENTS_INLINE,
    eSecondaryCommandBuffers = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
  };

  //=== VK_VERSION_1_1 ===

  enum class SubgroupFeatureFlagBits : VkSubgroupFeatureFlags
  {
    eBasic           = VK_SUBGROUP_FEATURE_BASIC_BIT,
    eVote            = VK_SUBGROUP_FEATURE_VOTE_BIT,
    eArithmetic      = VK_SUBGROUP_FEATURE_ARITHMETIC_BIT,
    eBallot          = VK_SUBGROUP_FEATURE_BALLOT_BIT,
    eShuffle         = VK_SUBGROUP_FEATURE_SHUFFLE_BIT,
    eShuffleRelative = VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT,
    eClustered       = VK_SUBGROUP_FEATURE_CLUSTERED_BIT,
    eQuad            = VK_SUBGROUP_FEATURE_QUAD_BIT
  };

  using SubgroupFeatureFlags = Flags<SubgroupFeatureFlagBits>;

  template <>
  struct FlagTraits<SubgroupFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SubgroupFeatureFlags allFlags =
      SubgroupFeatureFlagBits::eBasic | SubgroupFeatureFlagBits::eVote | SubgroupFeatureFlagBits::eArithmetic | SubgroupFeatureFlagBits::eBallot |
      SubgroupFeatureFlagBits::eShuffle | SubgroupFeatureFlagBits::eShuffleRelative | SubgroupFeatureFlagBits::eClustered | SubgroupFeatureFlagBits::eQuad;
  };

  enum class PeerMemoryFeatureFlagBits : VkPeerMemoryFeatureFlags
  {
    eCopySrc    = VK_PEER_MEMORY_FEATURE_COPY_SRC_BIT,
    eCopyDst    = VK_PEER_MEMORY_FEATURE_COPY_DST_BIT,
    eGenericSrc = VK_PEER_MEMORY_FEATURE_GENERIC_SRC_BIT,
    eGenericDst = VK_PEER_MEMORY_FEATURE_GENERIC_DST_BIT
  };
  using PeerMemoryFeatureFlagBitsKHR = PeerMemoryFeatureFlagBits;

  using PeerMemoryFeatureFlags    = Flags<PeerMemoryFeatureFlagBits>;
  using PeerMemoryFeatureFlagsKHR = PeerMemoryFeatureFlags;

  template <>
  struct FlagTraits<PeerMemoryFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PeerMemoryFeatureFlags allFlags  = PeerMemoryFeatureFlagBits::eCopySrc | PeerMemoryFeatureFlagBits::eCopyDst |
                                                                           PeerMemoryFeatureFlagBits::eGenericSrc | PeerMemoryFeatureFlagBits::eGenericDst;
  };

  enum class MemoryAllocateFlagBits : VkMemoryAllocateFlags
  {
    eDeviceMask                 = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT,
    eDeviceAddress              = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
    eDeviceAddressCaptureReplay = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT
  };
  using MemoryAllocateFlagBitsKHR = MemoryAllocateFlagBits;

  using MemoryAllocateFlags    = Flags<MemoryAllocateFlagBits>;
  using MemoryAllocateFlagsKHR = MemoryAllocateFlags;

  template <>
  struct FlagTraits<MemoryAllocateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR MemoryAllocateFlags allFlags =
      MemoryAllocateFlagBits::eDeviceMask | MemoryAllocateFlagBits::eDeviceAddress | MemoryAllocateFlagBits::eDeviceAddressCaptureReplay;
  };

  enum class PointClippingBehavior
  {
    eAllClipPlanes      = VK_POINT_CLIPPING_BEHAVIOR_ALL_CLIP_PLANES,
    eUserClipPlanesOnly = VK_POINT_CLIPPING_BEHAVIOR_USER_CLIP_PLANES_ONLY
  };
  using PointClippingBehaviorKHR = PointClippingBehavior;

  enum class TessellationDomainOrigin
  {
    eUpperLeft = VK_TESSELLATION_DOMAIN_ORIGIN_UPPER_LEFT,
    eLowerLeft = VK_TESSELLATION_DOMAIN_ORIGIN_LOWER_LEFT
  };
  using TessellationDomainOriginKHR = TessellationDomainOrigin;

  enum class SamplerYcbcrModelConversion
  {
    eRgbIdentity   = VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY,
    eYcbcrIdentity = VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY,
    eYcbcr709      = VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709,
    eYcbcr601      = VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601,
    eYcbcr2020     = VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020
  };
  using SamplerYcbcrModelConversionKHR = SamplerYcbcrModelConversion;

  enum class SamplerYcbcrRange
  {
    eItuFull   = VK_SAMPLER_YCBCR_RANGE_ITU_FULL,
    eItuNarrow = VK_SAMPLER_YCBCR_RANGE_ITU_NARROW
  };
  using SamplerYcbcrRangeKHR = SamplerYcbcrRange;

  enum class ChromaLocation
  {
    eCositedEven = VK_CHROMA_LOCATION_COSITED_EVEN,
    eMidpoint    = VK_CHROMA_LOCATION_MIDPOINT
  };
  using ChromaLocationKHR = ChromaLocation;

  enum class ExternalMemoryHandleTypeFlagBits : VkExternalMemoryHandleTypeFlags
  {
    eOpaqueFd                   = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT,
    eOpaqueWin32                = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT,
    eOpaqueWin32Kmt             = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT,
    eD3D11Texture               = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT,
    eD3D11TextureKmt            = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT,
    eD3D12Heap                  = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP_BIT,
    eD3D12Resource              = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE_BIT,
    eDmaBufEXT                  = VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT,
    eHostAllocationEXT          = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT,
    eHostMappedForeignMemoryEXT = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_MAPPED_FOREIGN_MEMORY_BIT_EXT,
#if defined( VK_USE_PLATFORM_SCI )
    eSciBufNV = VK_EXTERNAL_MEMORY_HANDLE_TYPE_SCI_BUF_BIT_NV,
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    eScreenBufferQNX = VK_EXTERNAL_MEMORY_HANDLE_TYPE_SCREEN_BUFFER_BIT_QNX
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
  };
  using ExternalMemoryHandleTypeFlagBitsKHR = ExternalMemoryHandleTypeFlagBits;

  using ExternalMemoryHandleTypeFlags    = Flags<ExternalMemoryHandleTypeFlagBits>;
  using ExternalMemoryHandleTypeFlagsKHR = ExternalMemoryHandleTypeFlags;

  template <>
  struct FlagTraits<ExternalMemoryHandleTypeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalMemoryHandleTypeFlags allFlags =
      ExternalMemoryHandleTypeFlagBits::eOpaqueFd | ExternalMemoryHandleTypeFlagBits::eOpaqueWin32 | ExternalMemoryHandleTypeFlagBits::eOpaqueWin32Kmt |
      ExternalMemoryHandleTypeFlagBits::eD3D11Texture | ExternalMemoryHandleTypeFlagBits::eD3D11TextureKmt | ExternalMemoryHandleTypeFlagBits::eD3D12Heap |
      ExternalMemoryHandleTypeFlagBits::eD3D12Resource | ExternalMemoryHandleTypeFlagBits::eDmaBufEXT | ExternalMemoryHandleTypeFlagBits::eHostAllocationEXT |
      ExternalMemoryHandleTypeFlagBits::eHostMappedForeignMemoryEXT
#if defined( VK_USE_PLATFORM_SCI )
      | ExternalMemoryHandleTypeFlagBits::eSciBufNV
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      | ExternalMemoryHandleTypeFlagBits::eScreenBufferQNX
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
      ;
  };

  enum class ExternalMemoryFeatureFlagBits : VkExternalMemoryFeatureFlags
  {
    eDedicatedOnly = VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT,
    eExportable    = VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT,
    eImportable    = VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT
  };
  using ExternalMemoryFeatureFlagBitsKHR = ExternalMemoryFeatureFlagBits;

  using ExternalMemoryFeatureFlags    = Flags<ExternalMemoryFeatureFlagBits>;
  using ExternalMemoryFeatureFlagsKHR = ExternalMemoryFeatureFlags;

  template <>
  struct FlagTraits<ExternalMemoryFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                       isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalMemoryFeatureFlags allFlags =
      ExternalMemoryFeatureFlagBits::eDedicatedOnly | ExternalMemoryFeatureFlagBits::eExportable | ExternalMemoryFeatureFlagBits::eImportable;
  };

  enum class ExternalFenceHandleTypeFlagBits : VkExternalFenceHandleTypeFlags
  {
    eOpaqueFd       = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT,
    eOpaqueWin32    = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT,
    eOpaqueWin32Kmt = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT,
    eSyncFd         = VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT,
#if defined( VK_USE_PLATFORM_SCI )
    eSciSyncObjNV   = VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_OBJ_BIT_NV,
    eSciSyncFenceNV = VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_FENCE_BIT_NV
#endif /*VK_USE_PLATFORM_SCI*/
  };
  using ExternalFenceHandleTypeFlagBitsKHR = ExternalFenceHandleTypeFlagBits;

  using ExternalFenceHandleTypeFlags    = Flags<ExternalFenceHandleTypeFlagBits>;
  using ExternalFenceHandleTypeFlagsKHR = ExternalFenceHandleTypeFlags;

  template <>
  struct FlagTraits<ExternalFenceHandleTypeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                         isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalFenceHandleTypeFlags allFlags =
      ExternalFenceHandleTypeFlagBits::eOpaqueFd | ExternalFenceHandleTypeFlagBits::eOpaqueWin32 | ExternalFenceHandleTypeFlagBits::eOpaqueWin32Kmt |
      ExternalFenceHandleTypeFlagBits::eSyncFd
#if defined( VK_USE_PLATFORM_SCI )
      | ExternalFenceHandleTypeFlagBits::eSciSyncObjNV | ExternalFenceHandleTypeFlagBits::eSciSyncFenceNV
#endif /*VK_USE_PLATFORM_SCI*/
      ;
  };

  enum class ExternalFenceFeatureFlagBits : VkExternalFenceFeatureFlags
  {
    eExportable = VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT,
    eImportable = VK_EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT
  };
  using ExternalFenceFeatureFlagBitsKHR = ExternalFenceFeatureFlagBits;

  using ExternalFenceFeatureFlags    = Flags<ExternalFenceFeatureFlagBits>;
  using ExternalFenceFeatureFlagsKHR = ExternalFenceFeatureFlags;

  template <>
  struct FlagTraits<ExternalFenceFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                      isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalFenceFeatureFlags allFlags =
      ExternalFenceFeatureFlagBits::eExportable | ExternalFenceFeatureFlagBits::eImportable;
  };

  enum class FenceImportFlagBits : VkFenceImportFlags
  {
    eTemporary = VK_FENCE_IMPORT_TEMPORARY_BIT
  };
  using FenceImportFlagBitsKHR = FenceImportFlagBits;

  using FenceImportFlags    = Flags<FenceImportFlagBits>;
  using FenceImportFlagsKHR = FenceImportFlags;

  template <>
  struct FlagTraits<FenceImportFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR FenceImportFlags allFlags  = FenceImportFlagBits::eTemporary;
  };

  enum class SemaphoreImportFlagBits : VkSemaphoreImportFlags
  {
    eTemporary = VK_SEMAPHORE_IMPORT_TEMPORARY_BIT
  };
  using SemaphoreImportFlagBitsKHR = SemaphoreImportFlagBits;

  using SemaphoreImportFlags    = Flags<SemaphoreImportFlagBits>;
  using SemaphoreImportFlagsKHR = SemaphoreImportFlags;

  template <>
  struct FlagTraits<SemaphoreImportFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SemaphoreImportFlags allFlags  = SemaphoreImportFlagBits::eTemporary;
  };

  enum class ExternalSemaphoreHandleTypeFlagBits : VkExternalSemaphoreHandleTypeFlags
  {
    eOpaqueFd       = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT,
    eOpaqueWin32    = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT,
    eOpaqueWin32Kmt = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT,
    eD3D12Fence     = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT,
    eD3D11Fence     = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D11_FENCE_BIT,
    eSyncFd         = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT,
#if defined( VK_USE_PLATFORM_SCI )
    eSciSyncObjNV = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SCI_SYNC_OBJ_BIT_NV
#endif /*VK_USE_PLATFORM_SCI*/
  };
  using ExternalSemaphoreHandleTypeFlagBitsKHR = ExternalSemaphoreHandleTypeFlagBits;

  using ExternalSemaphoreHandleTypeFlags    = Flags<ExternalSemaphoreHandleTypeFlagBits>;
  using ExternalSemaphoreHandleTypeFlagsKHR = ExternalSemaphoreHandleTypeFlags;

  template <>
  struct FlagTraits<ExternalSemaphoreHandleTypeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalSemaphoreHandleTypeFlags allFlags =
      ExternalSemaphoreHandleTypeFlagBits::eOpaqueFd | ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32 |
      ExternalSemaphoreHandleTypeFlagBits::eOpaqueWin32Kmt | ExternalSemaphoreHandleTypeFlagBits::eD3D12Fence | ExternalSemaphoreHandleTypeFlagBits::eSyncFd
#if defined( VK_USE_PLATFORM_SCI )
      | ExternalSemaphoreHandleTypeFlagBits::eSciSyncObjNV
#endif /*VK_USE_PLATFORM_SCI*/
      ;
  };

  enum class ExternalSemaphoreFeatureFlagBits : VkExternalSemaphoreFeatureFlags
  {
    eExportable = VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT,
    eImportable = VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT
  };
  using ExternalSemaphoreFeatureFlagBitsKHR = ExternalSemaphoreFeatureFlagBits;

  using ExternalSemaphoreFeatureFlags    = Flags<ExternalSemaphoreFeatureFlagBits>;
  using ExternalSemaphoreFeatureFlagsKHR = ExternalSemaphoreFeatureFlags;

  template <>
  struct FlagTraits<ExternalSemaphoreFeatureFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ExternalSemaphoreFeatureFlags allFlags =
      ExternalSemaphoreFeatureFlagBits::eExportable | ExternalSemaphoreFeatureFlagBits::eImportable;
  };

  //=== VK_VERSION_1_2 ===

  enum class DriverId
  {
    eAmdProprietary            = VK_DRIVER_ID_AMD_PROPRIETARY,
    eAmdOpenSource             = VK_DRIVER_ID_AMD_OPEN_SOURCE,
    eMesaRadv                  = VK_DRIVER_ID_MESA_RADV,
    eNvidiaProprietary         = VK_DRIVER_ID_NVIDIA_PROPRIETARY,
    eIntelProprietaryWindows   = VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS,
    eIntelOpenSourceMESA       = VK_DRIVER_ID_INTEL_OPEN_SOURCE_MESA,
    eImaginationProprietary    = VK_DRIVER_ID_IMAGINATION_PROPRIETARY,
    eQualcommProprietary       = VK_DRIVER_ID_QUALCOMM_PROPRIETARY,
    eArmProprietary            = VK_DRIVER_ID_ARM_PROPRIETARY,
    eGoogleSwiftshader         = VK_DRIVER_ID_GOOGLE_SWIFTSHADER,
    eGgpProprietary            = VK_DRIVER_ID_GGP_PROPRIETARY,
    eBroadcomProprietary       = VK_DRIVER_ID_BROADCOM_PROPRIETARY,
    eMesaLlvmpipe              = VK_DRIVER_ID_MESA_LLVMPIPE,
    eMoltenvk                  = VK_DRIVER_ID_MOLTENVK,
    eCoreaviProprietary        = VK_DRIVER_ID_COREAVI_PROPRIETARY,
    eJuiceProprietary          = VK_DRIVER_ID_JUICE_PROPRIETARY,
    eVerisiliconProprietary    = VK_DRIVER_ID_VERISILICON_PROPRIETARY,
    eMesaTurnip                = VK_DRIVER_ID_MESA_TURNIP,
    eMesaV3Dv                  = VK_DRIVER_ID_MESA_V3DV,
    eMesaPanvk                 = VK_DRIVER_ID_MESA_PANVK,
    eSamsungProprietary        = VK_DRIVER_ID_SAMSUNG_PROPRIETARY,
    eMesaVenus                 = VK_DRIVER_ID_MESA_VENUS,
    eMesaDozen                 = VK_DRIVER_ID_MESA_DOZEN,
    eMesaNvk                   = VK_DRIVER_ID_MESA_NVK,
    eImaginationOpenSourceMESA = VK_DRIVER_ID_IMAGINATION_OPEN_SOURCE_MESA,
    eMesaAgxv                  = VK_DRIVER_ID_MESA_AGXV
  };
  using DriverIdKHR = DriverId;

  enum class ShaderFloatControlsIndependence
  {
    e32BitOnly = VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_32_BIT_ONLY,
    eAll       = VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_ALL,
    eNone      = VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_NONE
  };
  using ShaderFloatControlsIndependenceKHR = ShaderFloatControlsIndependence;

  enum class DescriptorBindingFlagBits : VkDescriptorBindingFlags
  {
    eUpdateAfterBind          = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT,
    eUpdateUnusedWhilePending = VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT,
    ePartiallyBound           = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT,
    eVariableDescriptorCount  = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
  };
  using DescriptorBindingFlagBitsEXT = DescriptorBindingFlagBits;

  using DescriptorBindingFlags    = Flags<DescriptorBindingFlagBits>;
  using DescriptorBindingFlagsEXT = DescriptorBindingFlags;

  template <>
  struct FlagTraits<DescriptorBindingFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DescriptorBindingFlags allFlags =
      DescriptorBindingFlagBits::eUpdateAfterBind | DescriptorBindingFlagBits::eUpdateUnusedWhilePending | DescriptorBindingFlagBits::ePartiallyBound |
      DescriptorBindingFlagBits::eVariableDescriptorCount;
  };

  enum class ResolveModeFlagBits : VkResolveModeFlags
  {
    eNone       = VK_RESOLVE_MODE_NONE,
    eSampleZero = VK_RESOLVE_MODE_SAMPLE_ZERO_BIT,
    eAverage    = VK_RESOLVE_MODE_AVERAGE_BIT,
    eMin        = VK_RESOLVE_MODE_MIN_BIT,
    eMax        = VK_RESOLVE_MODE_MAX_BIT
  };
  using ResolveModeFlagBitsKHR = ResolveModeFlagBits;

  using ResolveModeFlags    = Flags<ResolveModeFlagBits>;
  using ResolveModeFlagsKHR = ResolveModeFlags;

  template <>
  struct FlagTraits<ResolveModeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ResolveModeFlags allFlags =
      ResolveModeFlagBits::eNone | ResolveModeFlagBits::eSampleZero | ResolveModeFlagBits::eAverage | ResolveModeFlagBits::eMin | ResolveModeFlagBits::eMax;
  };

  enum class SamplerReductionMode
  {
    eWeightedAverage = VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE,
    eMin             = VK_SAMPLER_REDUCTION_MODE_MIN,
    eMax             = VK_SAMPLER_REDUCTION_MODE_MAX
  };
  using SamplerReductionModeEXT = SamplerReductionMode;

  enum class SemaphoreType
  {
    eBinary   = VK_SEMAPHORE_TYPE_BINARY,
    eTimeline = VK_SEMAPHORE_TYPE_TIMELINE
  };
  using SemaphoreTypeKHR = SemaphoreType;

  enum class SemaphoreWaitFlagBits : VkSemaphoreWaitFlags
  {
    eAny = VK_SEMAPHORE_WAIT_ANY_BIT
  };
  using SemaphoreWaitFlagBitsKHR = SemaphoreWaitFlagBits;

  using SemaphoreWaitFlags    = Flags<SemaphoreWaitFlagBits>;
  using SemaphoreWaitFlagsKHR = SemaphoreWaitFlags;

  template <>
  struct FlagTraits<SemaphoreWaitFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool               isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SemaphoreWaitFlags allFlags  = SemaphoreWaitFlagBits::eAny;
  };

  //=== VK_VERSION_1_3 ===

  enum class PipelineCreationFeedbackFlagBits : VkPipelineCreationFeedbackFlags
  {
    eValid                       = VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT,
    eApplicationPipelineCacheHit = VK_PIPELINE_CREATION_FEEDBACK_APPLICATION_PIPELINE_CACHE_HIT_BIT,
    eBasePipelineAcceleration    = VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT
  };
  using PipelineCreationFeedbackFlagBitsEXT = PipelineCreationFeedbackFlagBits;

  using PipelineCreationFeedbackFlags    = Flags<PipelineCreationFeedbackFlagBits>;
  using PipelineCreationFeedbackFlagsEXT = PipelineCreationFeedbackFlags;

  template <>
  struct FlagTraits<PipelineCreationFeedbackFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineCreationFeedbackFlags allFlags  = PipelineCreationFeedbackFlagBits::eValid |
                                                                                  PipelineCreationFeedbackFlagBits::eApplicationPipelineCacheHit |
                                                                                  PipelineCreationFeedbackFlagBits::eBasePipelineAcceleration;
  };

  enum class ToolPurposeFlagBits : VkToolPurposeFlags
  {
    eValidation         = VK_TOOL_PURPOSE_VALIDATION_BIT,
    eProfiling          = VK_TOOL_PURPOSE_PROFILING_BIT,
    eTracing            = VK_TOOL_PURPOSE_TRACING_BIT,
    eAdditionalFeatures = VK_TOOL_PURPOSE_ADDITIONAL_FEATURES_BIT,
    eModifyingFeatures  = VK_TOOL_PURPOSE_MODIFYING_FEATURES_BIT
  };
  using ToolPurposeFlagBitsEXT = ToolPurposeFlagBits;

  using ToolPurposeFlags    = Flags<ToolPurposeFlagBits>;
  using ToolPurposeFlagsEXT = ToolPurposeFlags;

  template <>
  struct FlagTraits<ToolPurposeFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool             isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ToolPurposeFlags allFlags  = ToolPurposeFlagBits::eValidation | ToolPurposeFlagBits::eProfiling |
                                                                     ToolPurposeFlagBits::eTracing | ToolPurposeFlagBits::eAdditionalFeatures |
                                                                     ToolPurposeFlagBits::eModifyingFeatures;
  };

  enum class PrivateDataSlotCreateFlagBits : VkPrivateDataSlotCreateFlags
  {
  };
  using PrivateDataSlotCreateFlagBitsEXT = PrivateDataSlotCreateFlagBits;

  using PrivateDataSlotCreateFlags    = Flags<PrivateDataSlotCreateFlagBits>;
  using PrivateDataSlotCreateFlagsEXT = PrivateDataSlotCreateFlags;

  template <>
  struct FlagTraits<PrivateDataSlotCreateFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                       isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PrivateDataSlotCreateFlags allFlags  = {};
  };

  enum class PipelineStageFlagBits2 : VkPipelineStageFlags2
  {
    eNone                             = VK_PIPELINE_STAGE_2_NONE,
    eTopOfPipe                        = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
    eDrawIndirect                     = VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT,
    eVertexInput                      = VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT,
    eVertexShader                     = VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT,
    eTessellationControlShader        = VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT,
    eTessellationEvaluationShader     = VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT,
    eGeometryShader                   = VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT,
    eFragmentShader                   = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
    eEarlyFragmentTests               = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
    eLateFragmentTests                = VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
    eColorAttachmentOutput            = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    eComputeShader                    = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
    eAllTransfer                      = VK_PIPELINE_STAGE_2_ALL_TRANSFER_BIT,
    eTransfer                         = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
    eBottomOfPipe                     = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
    eHost                             = VK_PIPELINE_STAGE_2_HOST_BIT,
    eAllGraphics                      = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
    eAllCommands                      = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    eCopy                             = VK_PIPELINE_STAGE_2_COPY_BIT,
    eResolve                          = VK_PIPELINE_STAGE_2_RESOLVE_BIT,
    eBlit                             = VK_PIPELINE_STAGE_2_BLIT_BIT,
    eClear                            = VK_PIPELINE_STAGE_2_CLEAR_BIT,
    eIndexInput                       = VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT,
    eVertexAttributeInput             = VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT,
    ePreRasterizationShaders          = VK_PIPELINE_STAGE_2_PRE_RASTERIZATION_SHADERS_BIT,
    eTransformFeedbackEXT             = VK_PIPELINE_STAGE_2_TRANSFORM_FEEDBACK_BIT_EXT,
    eConditionalRenderingEXT          = VK_PIPELINE_STAGE_2_CONDITIONAL_RENDERING_BIT_EXT,
    eCommandPreprocessNV              = VK_PIPELINE_STAGE_2_COMMAND_PREPROCESS_BIT_NV,
    eFragmentShadingRateAttachmentKHR = VK_PIPELINE_STAGE_2_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
    eShadingRateImageNV               = VK_PIPELINE_STAGE_2_SHADING_RATE_IMAGE_BIT_NV,
    eAccelerationStructureBuildKHR    = VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
    eRayTracingShaderKHR              = VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR,
    eRayTracingShaderNV               = VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_NV,
    eAccelerationStructureBuildNV     = VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
    eFragmentDensityProcessEXT        = VK_PIPELINE_STAGE_2_FRAGMENT_DENSITY_PROCESS_BIT_EXT,
    eTaskShaderNV                     = VK_PIPELINE_STAGE_2_TASK_SHADER_BIT_NV,
    eMeshShaderNV                     = VK_PIPELINE_STAGE_2_MESH_SHADER_BIT_NV,
    eTaskShaderEXT                    = VK_PIPELINE_STAGE_2_TASK_SHADER_BIT_EXT,
    eMeshShaderEXT                    = VK_PIPELINE_STAGE_2_MESH_SHADER_BIT_EXT
  };
  using PipelineStageFlagBits2KHR = PipelineStageFlagBits2;

  using PipelineStageFlags2    = Flags<PipelineStageFlagBits2>;
  using PipelineStageFlags2KHR = PipelineStageFlags2;

  template <>
  struct FlagTraits<PipelineStageFlagBits2>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineStageFlags2 allFlags =
      PipelineStageFlagBits2::eNone | PipelineStageFlagBits2::eTopOfPipe | PipelineStageFlagBits2::eDrawIndirect | PipelineStageFlagBits2::eVertexInput |
      PipelineStageFlagBits2::eVertexShader | PipelineStageFlagBits2::eTessellationControlShader | PipelineStageFlagBits2::eTessellationEvaluationShader |
      PipelineStageFlagBits2::eGeometryShader | PipelineStageFlagBits2::eFragmentShader | PipelineStageFlagBits2::eEarlyFragmentTests |
      PipelineStageFlagBits2::eLateFragmentTests | PipelineStageFlagBits2::eColorAttachmentOutput | PipelineStageFlagBits2::eComputeShader |
      PipelineStageFlagBits2::eAllTransfer | PipelineStageFlagBits2::eBottomOfPipe | PipelineStageFlagBits2::eHost | PipelineStageFlagBits2::eAllGraphics |
      PipelineStageFlagBits2::eAllCommands | PipelineStageFlagBits2::eCopy | PipelineStageFlagBits2::eResolve | PipelineStageFlagBits2::eBlit |
      PipelineStageFlagBits2::eClear | PipelineStageFlagBits2::eIndexInput | PipelineStageFlagBits2::eVertexAttributeInput |
      PipelineStageFlagBits2::ePreRasterizationShaders | PipelineStageFlagBits2::eTransformFeedbackEXT | PipelineStageFlagBits2::eConditionalRenderingEXT |
      PipelineStageFlagBits2::eCommandPreprocessNV | PipelineStageFlagBits2::eFragmentShadingRateAttachmentKHR |
      PipelineStageFlagBits2::eAccelerationStructureBuildKHR | PipelineStageFlagBits2::eRayTracingShaderKHR |
      PipelineStageFlagBits2::eFragmentDensityProcessEXT | PipelineStageFlagBits2::eTaskShaderEXT | PipelineStageFlagBits2::eMeshShaderEXT;
  };

  enum class AccessFlagBits2 : VkAccessFlags2
  {
    eNone                                 = VK_ACCESS_2_NONE,
    eIndirectCommandRead                  = VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT,
    eIndexRead                            = VK_ACCESS_2_INDEX_READ_BIT,
    eVertexAttributeRead                  = VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT,
    eUniformRead                          = VK_ACCESS_2_UNIFORM_READ_BIT,
    eInputAttachmentRead                  = VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT,
    eShaderRead                           = VK_ACCESS_2_SHADER_READ_BIT,
    eShaderWrite                          = VK_ACCESS_2_SHADER_WRITE_BIT,
    eColorAttachmentRead                  = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT,
    eColorAttachmentWrite                 = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
    eDepthStencilAttachmentRead           = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    eDepthStencilAttachmentWrite          = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    eTransferRead                         = VK_ACCESS_2_TRANSFER_READ_BIT,
    eTransferWrite                        = VK_ACCESS_2_TRANSFER_WRITE_BIT,
    eHostRead                             = VK_ACCESS_2_HOST_READ_BIT,
    eHostWrite                            = VK_ACCESS_2_HOST_WRITE_BIT,
    eMemoryRead                           = VK_ACCESS_2_MEMORY_READ_BIT,
    eMemoryWrite                          = VK_ACCESS_2_MEMORY_WRITE_BIT,
    eShaderSampledRead                    = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT,
    eShaderStorageRead                    = VK_ACCESS_2_SHADER_STORAGE_READ_BIT,
    eShaderStorageWrite                   = VK_ACCESS_2_SHADER_STORAGE_WRITE_BIT,
    eTransformFeedbackWriteEXT            = VK_ACCESS_2_TRANSFORM_FEEDBACK_WRITE_BIT_EXT,
    eTransformFeedbackCounterReadEXT      = VK_ACCESS_2_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT,
    eTransformFeedbackCounterWriteEXT     = VK_ACCESS_2_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT,
    eConditionalRenderingReadEXT          = VK_ACCESS_2_CONDITIONAL_RENDERING_READ_BIT_EXT,
    eCommandPreprocessReadNV              = VK_ACCESS_2_COMMAND_PREPROCESS_READ_BIT_NV,
    eCommandPreprocessWriteNV             = VK_ACCESS_2_COMMAND_PREPROCESS_WRITE_BIT_NV,
    eFragmentShadingRateAttachmentReadKHR = VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR,
    eShadingRateImageReadNV               = VK_ACCESS_2_SHADING_RATE_IMAGE_READ_BIT_NV,
    eAccelerationStructureReadKHR         = VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR,
    eAccelerationStructureWriteKHR        = VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR,
    eAccelerationStructureReadNV          = VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_NV,
    eAccelerationStructureWriteNV         = VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_NV,
    eFragmentDensityMapReadEXT            = VK_ACCESS_2_FRAGMENT_DENSITY_MAP_READ_BIT_EXT,
    eColorAttachmentReadNoncoherentEXT    = VK_ACCESS_2_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT
  };
  using AccessFlagBits2KHR = AccessFlagBits2;

  using AccessFlags2    = Flags<AccessFlagBits2>;
  using AccessFlags2KHR = AccessFlags2;

  template <>
  struct FlagTraits<AccessFlagBits2>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool         isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR AccessFlags2 allFlags =
      AccessFlagBits2::eNone | AccessFlagBits2::eIndirectCommandRead | AccessFlagBits2::eIndexRead | AccessFlagBits2::eVertexAttributeRead |
      AccessFlagBits2::eUniformRead | AccessFlagBits2::eInputAttachmentRead | AccessFlagBits2::eShaderRead | AccessFlagBits2::eShaderWrite |
      AccessFlagBits2::eColorAttachmentRead | AccessFlagBits2::eColorAttachmentWrite | AccessFlagBits2::eDepthStencilAttachmentRead |
      AccessFlagBits2::eDepthStencilAttachmentWrite | AccessFlagBits2::eTransferRead | AccessFlagBits2::eTransferWrite | AccessFlagBits2::eHostRead |
      AccessFlagBits2::eHostWrite | AccessFlagBits2::eMemoryRead | AccessFlagBits2::eMemoryWrite | AccessFlagBits2::eShaderSampledRead |
      AccessFlagBits2::eShaderStorageRead | AccessFlagBits2::eShaderStorageWrite | AccessFlagBits2::eTransformFeedbackWriteEXT |
      AccessFlagBits2::eTransformFeedbackCounterReadEXT | AccessFlagBits2::eTransformFeedbackCounterWriteEXT | AccessFlagBits2::eConditionalRenderingReadEXT |
      AccessFlagBits2::eCommandPreprocessReadNV | AccessFlagBits2::eCommandPreprocessWriteNV | AccessFlagBits2::eFragmentShadingRateAttachmentReadKHR |
      AccessFlagBits2::eAccelerationStructureReadKHR | AccessFlagBits2::eAccelerationStructureWriteKHR | AccessFlagBits2::eFragmentDensityMapReadEXT |
      AccessFlagBits2::eColorAttachmentReadNoncoherentEXT;
  };

  enum class SubmitFlagBits : VkSubmitFlags
  {
    eProtected = VK_SUBMIT_PROTECTED_BIT
  };
  using SubmitFlagBitsKHR = SubmitFlagBits;

  using SubmitFlags    = Flags<SubmitFlagBits>;
  using SubmitFlagsKHR = SubmitFlags;

  template <>
  struct FlagTraits<SubmitFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool        isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SubmitFlags allFlags  = SubmitFlagBits::eProtected;
  };

  enum class RenderingFlagBits : VkRenderingFlags
  {
    eContentsSecondaryCommandBuffers = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT,
    eSuspending                      = VK_RENDERING_SUSPENDING_BIT,
    eResuming                        = VK_RENDERING_RESUMING_BIT
  };
  using RenderingFlagBitsKHR = RenderingFlagBits;

  using RenderingFlags    = Flags<RenderingFlagBits>;
  using RenderingFlagsKHR = RenderingFlags;

  template <>
  struct FlagTraits<RenderingFlagBits>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool           isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR RenderingFlags allFlags =
      RenderingFlagBits::eContentsSecondaryCommandBuffers | RenderingFlagBits::eSuspending | RenderingFlagBits::eResuming;
  };

  enum class FormatFeatureFlagBits2 : VkFormatFeatureFlags2
  {
    eSampledImage                                            = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_BIT,
    eStorageImage                                            = VK_FORMAT_FEATURE_2_STORAGE_IMAGE_BIT,
    eStorageImageAtomic                                      = VK_FORMAT_FEATURE_2_STORAGE_IMAGE_ATOMIC_BIT,
    eUniformTexelBuffer                                      = VK_FORMAT_FEATURE_2_UNIFORM_TEXEL_BUFFER_BIT,
    eStorageTexelBuffer                                      = VK_FORMAT_FEATURE_2_STORAGE_TEXEL_BUFFER_BIT,
    eStorageTexelBufferAtomic                                = VK_FORMAT_FEATURE_2_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    eVertexBuffer                                            = VK_FORMAT_FEATURE_2_VERTEX_BUFFER_BIT,
    eColorAttachment                                         = VK_FORMAT_FEATURE_2_COLOR_ATTACHMENT_BIT,
    eColorAttachmentBlend                                    = VK_FORMAT_FEATURE_2_COLOR_ATTACHMENT_BLEND_BIT,
    eDepthStencilAttachment                                  = VK_FORMAT_FEATURE_2_DEPTH_STENCIL_ATTACHMENT_BIT,
    eBlitSrc                                                 = VK_FORMAT_FEATURE_2_BLIT_SRC_BIT,
    eBlitDst                                                 = VK_FORMAT_FEATURE_2_BLIT_DST_BIT,
    eSampledImageFilterLinear                                = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
    eSampledImageFilterCubic                                 = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_CUBIC_BIT,
    eSampledImageFilterCubicEXT                              = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT,
    eTransferSrc                                             = VK_FORMAT_FEATURE_2_TRANSFER_SRC_BIT,
    eTransferDst                                             = VK_FORMAT_FEATURE_2_TRANSFER_DST_BIT,
    eSampledImageFilterMinmax                                = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_MINMAX_BIT,
    eMidpointChromaSamples                                   = VK_FORMAT_FEATURE_2_MIDPOINT_CHROMA_SAMPLES_BIT,
    eSampledImageYcbcrConversionLinearFilter                 = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,
    eSampledImageYcbcrConversionSeparateReconstructionFilter = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,
    eSampledImageYcbcrConversionChromaReconstructionExplicit = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,
    eSampledImageYcbcrConversionChromaReconstructionExplicitForceable =
      VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,
    eDisjoint                         = VK_FORMAT_FEATURE_2_DISJOINT_BIT,
    eCositedChromaSamples             = VK_FORMAT_FEATURE_2_COSITED_CHROMA_SAMPLES_BIT,
    eStorageReadWithoutFormat         = VK_FORMAT_FEATURE_2_STORAGE_READ_WITHOUT_FORMAT_BIT,
    eStorageWriteWithoutFormat        = VK_FORMAT_FEATURE_2_STORAGE_WRITE_WITHOUT_FORMAT_BIT,
    eSampledImageDepthComparison      = VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_DEPTH_COMPARISON_BIT,
    eFragmentShadingRateAttachmentKHR = VK_FORMAT_FEATURE_2_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR
  };
  using FormatFeatureFlagBits2KHR = FormatFeatureFlagBits2;

  using FormatFeatureFlags2    = Flags<FormatFeatureFlagBits2>;
  using FormatFeatureFlags2KHR = FormatFeatureFlags2;

  template <>
  struct FlagTraits<FormatFeatureFlagBits2>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR FormatFeatureFlags2 allFlags =
      FormatFeatureFlagBits2::eSampledImage | FormatFeatureFlagBits2::eStorageImage | FormatFeatureFlagBits2::eStorageImageAtomic |
      FormatFeatureFlagBits2::eUniformTexelBuffer | FormatFeatureFlagBits2::eStorageTexelBuffer | FormatFeatureFlagBits2::eStorageTexelBufferAtomic |
      FormatFeatureFlagBits2::eVertexBuffer | FormatFeatureFlagBits2::eColorAttachment | FormatFeatureFlagBits2::eColorAttachmentBlend |
      FormatFeatureFlagBits2::eDepthStencilAttachment | FormatFeatureFlagBits2::eBlitSrc | FormatFeatureFlagBits2::eBlitDst |
      FormatFeatureFlagBits2::eSampledImageFilterLinear | FormatFeatureFlagBits2::eSampledImageFilterCubic | FormatFeatureFlagBits2::eTransferSrc |
      FormatFeatureFlagBits2::eTransferDst | FormatFeatureFlagBits2::eSampledImageFilterMinmax | FormatFeatureFlagBits2::eMidpointChromaSamples |
      FormatFeatureFlagBits2::eSampledImageYcbcrConversionLinearFilter | FormatFeatureFlagBits2::eSampledImageYcbcrConversionSeparateReconstructionFilter |
      FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicit |
      FormatFeatureFlagBits2::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable | FormatFeatureFlagBits2::eDisjoint |
      FormatFeatureFlagBits2::eCositedChromaSamples | FormatFeatureFlagBits2::eStorageReadWithoutFormat | FormatFeatureFlagBits2::eStorageWriteWithoutFormat |
      FormatFeatureFlagBits2::eSampledImageDepthComparison | FormatFeatureFlagBits2::eFragmentShadingRateAttachmentKHR;
  };

  //=== VKSC_VERSION_1_0 ===

  enum class FaultLevel
  {
    eUnassigned  = VK_FAULT_LEVEL_UNASSIGNED,
    eCritical    = VK_FAULT_LEVEL_CRITICAL,
    eRecoverable = VK_FAULT_LEVEL_RECOVERABLE,
    eWarning     = VK_FAULT_LEVEL_WARNING
  };

  enum class FaultType
  {
    eInvalid           = VK_FAULT_TYPE_INVALID,
    eUnassigned        = VK_FAULT_TYPE_UNASSIGNED,
    eImplementation    = VK_FAULT_TYPE_IMPLEMENTATION,
    eSystem            = VK_FAULT_TYPE_SYSTEM,
    ePhysicalDevice    = VK_FAULT_TYPE_PHYSICAL_DEVICE,
    eCommandBufferFull = VK_FAULT_TYPE_COMMAND_BUFFER_FULL,
    eInvalidApiUsage   = VK_FAULT_TYPE_INVALID_API_USAGE
  };

  enum class FaultQueryBehavior
  {
    eGetAndClearAllFaults = VK_FAULT_QUERY_BEHAVIOR_GET_AND_CLEAR_ALL_FAULTS
  };

  enum class PipelineMatchControl
  {
    eApplicationUuidExactMatch = VK_PIPELINE_MATCH_CONTROL_APPLICATION_UUID_EXACT_MATCH
  };

  enum class PipelineCacheValidationVersion
  {
    eSafetyCriticalOne = VK_PIPELINE_CACHE_VALIDATION_VERSION_SAFETY_CRITICAL_ONE
  };

  //=== VK_KHR_surface ===

  enum class SurfaceTransformFlagBitsKHR : VkSurfaceTransformFlagsKHR
  {
    eIdentity                  = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    eRotate90                  = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
    eRotate180                 = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
    eRotate270                 = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
    eHorizontalMirror          = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
    eHorizontalMirrorRotate90  = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
    eHorizontalMirrorRotate180 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
    eHorizontalMirrorRotate270 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
    eInherit                   = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR
  };

  using SurfaceTransformFlagsKHR = Flags<SurfaceTransformFlagBitsKHR>;

  template <>
  struct FlagTraits<SurfaceTransformFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                     isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SurfaceTransformFlagsKHR allFlags =
      SurfaceTransformFlagBitsKHR::eIdentity | SurfaceTransformFlagBitsKHR::eRotate90 | SurfaceTransformFlagBitsKHR::eRotate180 |
      SurfaceTransformFlagBitsKHR::eRotate270 | SurfaceTransformFlagBitsKHR::eHorizontalMirror | SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90 |
      SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180 | SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270 | SurfaceTransformFlagBitsKHR::eInherit;
  };

  enum class PresentModeKHR
  {
    eImmediate               = VK_PRESENT_MODE_IMMEDIATE_KHR,
    eMailbox                 = VK_PRESENT_MODE_MAILBOX_KHR,
    eFifo                    = VK_PRESENT_MODE_FIFO_KHR,
    eFifoRelaxed             = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
    eSharedDemandRefresh     = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR,
    eSharedContinuousRefresh = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR
  };

  enum class ColorSpaceKHR
  {
    eSrgbNonlinear            = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    eDisplayP3NonlinearEXT    = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,
    eExtendedSrgbLinearEXT    = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
    eDisplayP3LinearEXT       = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
    eDciP3NonlinearEXT        = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,
    eBt709LinearEXT           = VK_COLOR_SPACE_BT709_LINEAR_EXT,
    eBt709NonlinearEXT        = VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
    eBt2020LinearEXT          = VK_COLOR_SPACE_BT2020_LINEAR_EXT,
    eHdr10St2084EXT           = VK_COLOR_SPACE_HDR10_ST2084_EXT,
    eDolbyvisionEXT           = VK_COLOR_SPACE_DOLBYVISION_EXT,
    eHdr10HlgEXT              = VK_COLOR_SPACE_HDR10_HLG_EXT,
    eAdobergbLinearEXT        = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,
    eAdobergbNonlinearEXT     = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,
    ePassThroughEXT           = VK_COLOR_SPACE_PASS_THROUGH_EXT,
    eExtendedSrgbNonlinearEXT = VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT
  };

  enum class CompositeAlphaFlagBitsKHR : VkCompositeAlphaFlagsKHR
  {
    eOpaque         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    ePreMultiplied  = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    ePostMultiplied = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    eInherit        = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
  };

  using CompositeAlphaFlagsKHR = Flags<CompositeAlphaFlagBitsKHR>;

  template <>
  struct FlagTraits<CompositeAlphaFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR CompositeAlphaFlagsKHR allFlags  = CompositeAlphaFlagBitsKHR::eOpaque | CompositeAlphaFlagBitsKHR::ePreMultiplied |
                                                                           CompositeAlphaFlagBitsKHR::ePostMultiplied | CompositeAlphaFlagBitsKHR::eInherit;
  };

  //=== VK_KHR_swapchain ===

  enum class SwapchainCreateFlagBitsKHR : VkSwapchainCreateFlagsKHR
  {
    eSplitInstanceBindRegions = VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR,
    eProtected                = VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR,
    eMutableFormat            = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR
  };

  using SwapchainCreateFlagsKHR = Flags<SwapchainCreateFlagBitsKHR>;

  template <>
  struct FlagTraits<SwapchainCreateFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                    isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SwapchainCreateFlagsKHR allFlags =
      SwapchainCreateFlagBitsKHR::eSplitInstanceBindRegions | SwapchainCreateFlagBitsKHR::eProtected | SwapchainCreateFlagBitsKHR::eMutableFormat;
  };

  enum class DeviceGroupPresentModeFlagBitsKHR : VkDeviceGroupPresentModeFlagsKHR
  {
    eLocal            = VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_BIT_KHR,
    eRemote           = VK_DEVICE_GROUP_PRESENT_MODE_REMOTE_BIT_KHR,
    eSum              = VK_DEVICE_GROUP_PRESENT_MODE_SUM_BIT_KHR,
    eLocalMultiDevice = VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_MULTI_DEVICE_BIT_KHR
  };

  using DeviceGroupPresentModeFlagsKHR = Flags<DeviceGroupPresentModeFlagBitsKHR>;

  template <>
  struct FlagTraits<DeviceGroupPresentModeFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                           isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DeviceGroupPresentModeFlagsKHR allFlags =
      DeviceGroupPresentModeFlagBitsKHR::eLocal | DeviceGroupPresentModeFlagBitsKHR::eRemote | DeviceGroupPresentModeFlagBitsKHR::eSum |
      DeviceGroupPresentModeFlagBitsKHR::eLocalMultiDevice;
  };

  //=== VK_KHR_display ===

  enum class DisplayPlaneAlphaFlagBitsKHR : VkDisplayPlaneAlphaFlagsKHR
  {
    eOpaque                = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR,
    eGlobal                = VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR,
    ePerPixel              = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR,
    ePerPixelPremultiplied = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR
  };

  using DisplayPlaneAlphaFlagsKHR = Flags<DisplayPlaneAlphaFlagBitsKHR>;

  template <>
  struct FlagTraits<DisplayPlaneAlphaFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                      isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DisplayPlaneAlphaFlagsKHR allFlags  = DisplayPlaneAlphaFlagBitsKHR::eOpaque | DisplayPlaneAlphaFlagBitsKHR::eGlobal |
                                                                              DisplayPlaneAlphaFlagBitsKHR::ePerPixel |
                                                                              DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied;
  };

  enum class DisplayModeCreateFlagBitsKHR : VkDisplayModeCreateFlagsKHR
  {
  };

  using DisplayModeCreateFlagsKHR = Flags<DisplayModeCreateFlagBitsKHR>;

  template <>
  struct FlagTraits<DisplayModeCreateFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                      isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DisplayModeCreateFlagsKHR allFlags  = {};
  };

  enum class DisplaySurfaceCreateFlagBitsKHR : VkDisplaySurfaceCreateFlagsKHR
  {
  };

  using DisplaySurfaceCreateFlagsKHR = Flags<DisplaySurfaceCreateFlagBitsKHR>;

  template <>
  struct FlagTraits<DisplaySurfaceCreateFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                         isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DisplaySurfaceCreateFlagsKHR allFlags  = {};
  };

  //=== VK_EXT_display_surface_counter ===

  enum class SurfaceCounterFlagBitsEXT : VkSurfaceCounterFlagsEXT
  {
    eVblank = VK_SURFACE_COUNTER_VBLANK_BIT_EXT
  };

  using SurfaceCounterFlagsEXT = Flags<SurfaceCounterFlagBitsEXT>;

  template <>
  struct FlagTraits<SurfaceCounterFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                   isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR SurfaceCounterFlagsEXT allFlags  = SurfaceCounterFlagBitsEXT::eVblank;
  };

  //=== VK_EXT_display_control ===

  enum class DisplayPowerStateEXT
  {
    eOff     = VK_DISPLAY_POWER_STATE_OFF_EXT,
    eSuspend = VK_DISPLAY_POWER_STATE_SUSPEND_EXT,
    eOn      = VK_DISPLAY_POWER_STATE_ON_EXT
  };

  enum class DeviceEventTypeEXT
  {
    eDisplayHotplug = VK_DEVICE_EVENT_TYPE_DISPLAY_HOTPLUG_EXT
  };

  enum class DisplayEventTypeEXT
  {
    eFirstPixelOut = VK_DISPLAY_EVENT_TYPE_FIRST_PIXEL_OUT_EXT
  };

  //=== VK_EXT_discard_rectangles ===

  enum class DiscardRectangleModeEXT
  {
    eInclusive = VK_DISCARD_RECTANGLE_MODE_INCLUSIVE_EXT,
    eExclusive = VK_DISCARD_RECTANGLE_MODE_EXCLUSIVE_EXT
  };

  enum class PipelineDiscardRectangleStateCreateFlagBitsEXT : VkPipelineDiscardRectangleStateCreateFlagsEXT
  {
  };

  using PipelineDiscardRectangleStateCreateFlagsEXT = Flags<PipelineDiscardRectangleStateCreateFlagBitsEXT>;

  template <>
  struct FlagTraits<PipelineDiscardRectangleStateCreateFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                        isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineDiscardRectangleStateCreateFlagsEXT allFlags  = {};
  };

  //=== VK_EXT_conservative_rasterization ===

  enum class ConservativeRasterizationModeEXT
  {
    eDisabled      = VK_CONSERVATIVE_RASTERIZATION_MODE_DISABLED_EXT,
    eOverestimate  = VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT,
    eUnderestimate = VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT
  };

  enum class PipelineRasterizationConservativeStateCreateFlagBitsEXT : VkPipelineRasterizationConservativeStateCreateFlagsEXT
  {
  };

  using PipelineRasterizationConservativeStateCreateFlagsEXT = Flags<PipelineRasterizationConservativeStateCreateFlagBitsEXT>;

  template <>
  struct FlagTraits<PipelineRasterizationConservativeStateCreateFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                                 isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineRasterizationConservativeStateCreateFlagsEXT allFlags  = {};
  };

  //=== VK_EXT_depth_clip_enable ===

  enum class PipelineRasterizationDepthClipStateCreateFlagBitsEXT : VkPipelineRasterizationDepthClipStateCreateFlagsEXT
  {
  };

  using PipelineRasterizationDepthClipStateCreateFlagsEXT = Flags<PipelineRasterizationDepthClipStateCreateFlagBitsEXT>;

  template <>
  struct FlagTraits<PipelineRasterizationDepthClipStateCreateFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PipelineRasterizationDepthClipStateCreateFlagsEXT allFlags  = {};
  };

  //=== VK_KHR_performance_query ===

  enum class PerformanceCounterDescriptionFlagBitsKHR : VkPerformanceCounterDescriptionFlagsKHR
  {
    ePerformanceImpacting = VK_PERFORMANCE_COUNTER_DESCRIPTION_PERFORMANCE_IMPACTING_BIT_KHR,
    eConcurrentlyImpacted = VK_PERFORMANCE_COUNTER_DESCRIPTION_CONCURRENTLY_IMPACTED_BIT_KHR
  };

  using PerformanceCounterDescriptionFlagsKHR = Flags<PerformanceCounterDescriptionFlagBitsKHR>;

  template <>
  struct FlagTraits<PerformanceCounterDescriptionFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR PerformanceCounterDescriptionFlagsKHR allFlags =
      PerformanceCounterDescriptionFlagBitsKHR::ePerformanceImpacting | PerformanceCounterDescriptionFlagBitsKHR::eConcurrentlyImpacted;
  };

  enum class PerformanceCounterScopeKHR
  {
    eCommandBuffer             = VK_PERFORMANCE_COUNTER_SCOPE_COMMAND_BUFFER_KHR,
    eRenderPass                = VK_PERFORMANCE_COUNTER_SCOPE_RENDER_PASS_KHR,
    eCommand                   = VK_PERFORMANCE_COUNTER_SCOPE_COMMAND_KHR,
    eVkQueryScopeCommandBuffer = VK_QUERY_SCOPE_COMMAND_BUFFER_KHR,
    eVkQueryScopeRenderPass    = VK_QUERY_SCOPE_RENDER_PASS_KHR,
    eVkQueryScopeCommand       = VK_QUERY_SCOPE_COMMAND_KHR
  };

  enum class PerformanceCounterStorageKHR
  {
    eInt32   = VK_PERFORMANCE_COUNTER_STORAGE_INT32_KHR,
    eInt64   = VK_PERFORMANCE_COUNTER_STORAGE_INT64_KHR,
    eUint32  = VK_PERFORMANCE_COUNTER_STORAGE_UINT32_KHR,
    eUint64  = VK_PERFORMANCE_COUNTER_STORAGE_UINT64_KHR,
    eFloat32 = VK_PERFORMANCE_COUNTER_STORAGE_FLOAT32_KHR,
    eFloat64 = VK_PERFORMANCE_COUNTER_STORAGE_FLOAT64_KHR
  };

  enum class PerformanceCounterUnitKHR
  {
    eGeneric        = VK_PERFORMANCE_COUNTER_UNIT_GENERIC_KHR,
    ePercentage     = VK_PERFORMANCE_COUNTER_UNIT_PERCENTAGE_KHR,
    eNanoseconds    = VK_PERFORMANCE_COUNTER_UNIT_NANOSECONDS_KHR,
    eBytes          = VK_PERFORMANCE_COUNTER_UNIT_BYTES_KHR,
    eBytesPerSecond = VK_PERFORMANCE_COUNTER_UNIT_BYTES_PER_SECOND_KHR,
    eKelvin         = VK_PERFORMANCE_COUNTER_UNIT_KELVIN_KHR,
    eWatts          = VK_PERFORMANCE_COUNTER_UNIT_WATTS_KHR,
    eVolts          = VK_PERFORMANCE_COUNTER_UNIT_VOLTS_KHR,
    eAmps           = VK_PERFORMANCE_COUNTER_UNIT_AMPS_KHR,
    eHertz          = VK_PERFORMANCE_COUNTER_UNIT_HERTZ_KHR,
    eCycles         = VK_PERFORMANCE_COUNTER_UNIT_CYCLES_KHR
  };

  enum class AcquireProfilingLockFlagBitsKHR : VkAcquireProfilingLockFlagsKHR
  {
  };

  using AcquireProfilingLockFlagsKHR = Flags<AcquireProfilingLockFlagBitsKHR>;

  template <>
  struct FlagTraits<AcquireProfilingLockFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                         isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR AcquireProfilingLockFlagsKHR allFlags  = {};
  };

  //=== VK_EXT_debug_utils ===

  enum class DebugUtilsMessageSeverityFlagBitsEXT : VkDebugUtilsMessageSeverityFlagsEXT
  {
    eVerbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
    eInfo    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
    eWarning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
    eError   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
  };

  using DebugUtilsMessageSeverityFlagsEXT = Flags<DebugUtilsMessageSeverityFlagBitsEXT>;

  template <>
  struct FlagTraits<DebugUtilsMessageSeverityFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugUtilsMessageSeverityFlagsEXT allFlags =
      DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | DebugUtilsMessageSeverityFlagBitsEXT::eInfo | DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      DebugUtilsMessageSeverityFlagBitsEXT::eError;
  };

  enum class DebugUtilsMessageTypeFlagBitsEXT : VkDebugUtilsMessageTypeFlagsEXT
  {
    eGeneral     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
    eValidation  = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
    ePerformance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
  };

  using DebugUtilsMessageTypeFlagsEXT = Flags<DebugUtilsMessageTypeFlagBitsEXT>;

  template <>
  struct FlagTraits<DebugUtilsMessageTypeFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugUtilsMessageTypeFlagsEXT allFlags =
      DebugUtilsMessageTypeFlagBitsEXT::eGeneral | DebugUtilsMessageTypeFlagBitsEXT::eValidation | DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  };

  enum class DebugUtilsMessengerCallbackDataFlagBitsEXT : VkDebugUtilsMessengerCallbackDataFlagsEXT
  {
  };

  using DebugUtilsMessengerCallbackDataFlagsEXT = Flags<DebugUtilsMessengerCallbackDataFlagBitsEXT>;

  template <>
  struct FlagTraits<DebugUtilsMessengerCallbackDataFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                                    isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugUtilsMessengerCallbackDataFlagsEXT allFlags  = {};
  };

  enum class DebugUtilsMessengerCreateFlagBitsEXT : VkDebugUtilsMessengerCreateFlagsEXT
  {
  };

  using DebugUtilsMessengerCreateFlagsEXT = Flags<DebugUtilsMessengerCreateFlagBitsEXT>;

  template <>
  struct FlagTraits<DebugUtilsMessengerCreateFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                              isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugUtilsMessengerCreateFlagsEXT allFlags  = {};
  };

  //=== VK_EXT_blend_operation_advanced ===

  enum class BlendOverlapEXT
  {
    eUncorrelated = VK_BLEND_OVERLAP_UNCORRELATED_EXT,
    eDisjoint     = VK_BLEND_OVERLAP_DISJOINT_EXT,
    eConjoint     = VK_BLEND_OVERLAP_CONJOINT_EXT
  };

  //=== VK_KHR_fragment_shading_rate ===

  enum class FragmentShadingRateCombinerOpKHR
  {
    eKeep    = VK_FRAGMENT_SHADING_RATE_COMBINER_OP_KEEP_KHR,
    eReplace = VK_FRAGMENT_SHADING_RATE_COMBINER_OP_REPLACE_KHR,
    eMin     = VK_FRAGMENT_SHADING_RATE_COMBINER_OP_MIN_KHR,
    eMax     = VK_FRAGMENT_SHADING_RATE_COMBINER_OP_MAX_KHR,
    eMul     = VK_FRAGMENT_SHADING_RATE_COMBINER_OP_MUL_KHR
  };

  //=== VK_EXT_validation_features ===

  enum class ValidationFeatureEnableEXT
  {
    eGpuAssisted                   = VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
    eGpuAssistedReserveBindingSlot = VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
    eBestPractices                 = VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
    eDebugPrintf                   = VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
    eSynchronizationValidation     = VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT
  };

  enum class ValidationFeatureDisableEXT
  {
    eAll                   = VK_VALIDATION_FEATURE_DISABLE_ALL_EXT,
    eShaders               = VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT,
    eThreadSafety          = VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT,
    eApiParameters         = VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT,
    eObjectLifetimes       = VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT,
    eCoreChecks            = VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT,
    eUniqueHandles         = VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT,
    eShaderValidationCache = VK_VALIDATION_FEATURE_DISABLE_SHADER_VALIDATION_CACHE_EXT
  };

  //=== VK_EXT_headless_surface ===

  enum class HeadlessSurfaceCreateFlagBitsEXT : VkHeadlessSurfaceCreateFlagsEXT
  {
  };

  using HeadlessSurfaceCreateFlagsEXT = Flags<HeadlessSurfaceCreateFlagBitsEXT>;

  template <>
  struct FlagTraits<HeadlessSurfaceCreateFlagBitsEXT>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                          isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR HeadlessSurfaceCreateFlagsEXT allFlags  = {};
  };

  //=== VK_KHR_object_refresh ===

  enum class RefreshObjectFlagBitsKHR : VkRefreshObjectFlagsKHR
  {
  };

  using RefreshObjectFlagsKHR = Flags<RefreshObjectFlagBitsKHR>;

  template <>
  struct FlagTraits<RefreshObjectFlagBitsKHR>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool                  isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR RefreshObjectFlagsKHR allFlags  = {};
  };

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===

  enum class SciSyncClientTypeNV
  {
    eSignaler       = VK_SCI_SYNC_CLIENT_TYPE_SIGNALER_NV,
    eWaiter         = VK_SCI_SYNC_CLIENT_TYPE_WAITER_NV,
    eSignalerWaiter = VK_SCI_SYNC_CLIENT_TYPE_SIGNALER_WAITER_NV
  };

  enum class SciSyncPrimitiveTypeNV
  {
    eFence     = VK_SCI_SYNC_PRIMITIVE_TYPE_FENCE_NV,
    eSemaphore = VK_SCI_SYNC_PRIMITIVE_TYPE_SEMAPHORE_NV
  };
#endif /*VK_USE_PLATFORM_SCI*/

  //=========================
  //=== Index Type Traits ===
  //=========================

  template <typename T>
  struct IndexTypeValue
  {
  };

  template <>
  struct IndexTypeValue<uint16_t>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR IndexType value = IndexType::eUint16;
  };

  template <>
  struct CppType<IndexType, IndexType::eUint16>
  {
    using Type = uint16_t;
  };

  template <>
  struct IndexTypeValue<uint32_t>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR IndexType value = IndexType::eUint32;
  };

  template <>
  struct CppType<IndexType, IndexType::eUint32>
  {
    using Type = uint32_t;
  };

  //===========================================================
  //=== Mapping from ObjectType to DebugReportObjectTypeEXT ===
  //===========================================================

  VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType( VULKAN_HPP_NAMESPACE::ObjectType objectType )
  {
    switch ( objectType )
    {
        //=== VK_VERSION_1_0 ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eInstance: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eInstance;
      case VULKAN_HPP_NAMESPACE::ObjectType::ePhysicalDevice: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePhysicalDevice;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDevice: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDevice;
      case VULKAN_HPP_NAMESPACE::ObjectType::eQueue: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eQueue;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDeviceMemory: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDeviceMemory;
      case VULKAN_HPP_NAMESPACE::ObjectType::eFence: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eFence;
      case VULKAN_HPP_NAMESPACE::ObjectType::eSemaphore: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSemaphore;
      case VULKAN_HPP_NAMESPACE::ObjectType::eEvent: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eEvent;
      case VULKAN_HPP_NAMESPACE::ObjectType::eQueryPool: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eQueryPool;
      case VULKAN_HPP_NAMESPACE::ObjectType::eBuffer: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eBuffer;
      case VULKAN_HPP_NAMESPACE::ObjectType::eBufferView: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eBufferView;
      case VULKAN_HPP_NAMESPACE::ObjectType::eImage: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eImage;
      case VULKAN_HPP_NAMESPACE::ObjectType::eImageView: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eImageView;
      case VULKAN_HPP_NAMESPACE::ObjectType::eShaderModule: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eShaderModule;
      case VULKAN_HPP_NAMESPACE::ObjectType::ePipelineCache: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipelineCache;
      case VULKAN_HPP_NAMESPACE::ObjectType::ePipeline: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipeline;
      case VULKAN_HPP_NAMESPACE::ObjectType::ePipelineLayout: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipelineLayout;
      case VULKAN_HPP_NAMESPACE::ObjectType::eSampler: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSampler;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorPool: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorPool;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorSet: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorSet;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorSetLayout: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorSetLayout;
      case VULKAN_HPP_NAMESPACE::ObjectType::eFramebuffer: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eFramebuffer;
      case VULKAN_HPP_NAMESPACE::ObjectType::eRenderPass: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eRenderPass;
      case VULKAN_HPP_NAMESPACE::ObjectType::eCommandPool: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eCommandPool;
      case VULKAN_HPP_NAMESPACE::ObjectType::eCommandBuffer:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eCommandBuffer;

        //=== VK_VERSION_1_1 ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eSamplerYcbcrConversion:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

        //=== VK_VERSION_1_3 ===
      case VULKAN_HPP_NAMESPACE::ObjectType::ePrivateDataSlot:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

        //=== VK_KHR_surface ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eSurfaceKHR:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSurfaceKHR;

        //=== VK_KHR_swapchain ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eSwapchainKHR:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSwapchainKHR;

        //=== VK_KHR_display ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eDisplayKHR: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDisplayKHR;
      case VULKAN_HPP_NAMESPACE::ObjectType::eDisplayModeKHR:
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDisplayModeKHR;

        //=== VK_EXT_debug_utils ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eDebugUtilsMessengerEXT: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

#if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_sci_sync2 ===
      case VULKAN_HPP_NAMESPACE::ObjectType::eSemaphoreSciSyncPoolNV: return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;
#endif /*VK_USE_PLATFORM_SCI*/

      default: VULKAN_HPP_ASSERT( false && "unknown ObjectType" ); return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;
    }
  }

}  // namespace VULKAN_HPP_NAMESPACE
#endif
