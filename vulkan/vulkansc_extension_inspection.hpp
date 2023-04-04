// Copyright 2015-2023 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_EXTENSION_INSPECTION_HPP
#define VULKAN_EXTENSION_INSPECTION_HPP

#include <vulkan/vulkansc.hpp>

namespace VULKAN_HPP_NAMESPACE
{
  //======================================
  //=== Extension inspection functions ===
  //======================================

  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & name );
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & name );
  VULKAN_HPP_CONSTEXPR_20 bool        isDeviceExtension( std::string const & name );
  VULKAN_HPP_CONSTEXPR_20 bool        isExtensionDeprecated( std::string const & name );
  VULKAN_HPP_CONSTEXPR_20 bool        isExtensionPromoted( std::string const & name );
  VULKAN_HPP_CONSTEXPR_20 bool        isInstanceExtension( std::string const & name );

  //=====================================================
  //=== Extension inspection function implementations ===
  //=====================================================

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & name )
  {
    (void)name;

#if defined( VK_USE_PLATFORM_SCI )
    if ( name == "VK_NV_external_sci_sync" )
    {
      return "VK_NV_external_sci_sync2";
    }
#endif /*VK_USE_PLATFORM_SCI*/

    return "";
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & name )
  {
    if ( name == "VK_EXT_texture_compression_astc_hdr" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_global_priority" )
    {
      return "VK_KHR_global_priority";
    }
    if ( name == "VK_KHR_shader_terminate_invocation" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_subgroup_size_control" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_extended_dynamic_state" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_shader_demote_to_helper_invocation" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_texel_buffer_alignment" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_KHR_synchronization2" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_ycbcr_2plane_444_formats" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_image_robustness" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_KHR_copy_commands2" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_4444_formats" )
    {
      return "VK_VERSION_1_3";
    }
    if ( name == "VK_EXT_extended_dynamic_state2" )
    {
      return "VK_VERSION_1_3";
    }
    return "";
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isDeviceExtension( std::string const & name )
  {
    return ( name == "VK_KHR_swapchain" ) || ( name == "VK_KHR_display_swapchain" ) || ( name == "VK_EXT_depth_range_unrestricted" ) ||
           ( name == "VK_NV_private_vendor_info" ) || ( name == "VK_EXT_texture_compression_astc_hdr" ) || ( name == "VK_EXT_astc_decode_mode" ) ||
           ( name == "VK_KHR_external_memory_fd" ) || ( name == "VK_KHR_external_semaphore_fd" ) || ( name == "VK_KHR_incremental_present" ) ||
           ( name == "VK_EXT_display_control" ) || ( name == "VK_EXT_discard_rectangles" ) || ( name == "VK_EXT_conservative_rasterization" ) ||
           ( name == "VK_EXT_depth_clip_enable" ) || ( name == "VK_EXT_hdr_metadata" ) || ( name == "VK_KHR_shared_presentable_image" ) ||
           ( name == "VK_KHR_external_fence_fd" ) || ( name == "VK_KHR_performance_query" ) || ( name == "VK_EXT_external_memory_dma_buf" ) ||
           ( name == "VK_EXT_queue_family_foreign" ) || ( name == "VK_EXT_shader_stencil_export" ) || ( name == "VK_EXT_sample_locations" ) ||
           ( name == "VK_EXT_blend_operation_advanced" ) || ( name == "VK_EXT_post_depth_coverage" ) || ( name == "VK_EXT_image_drm_format_modifier" ) ||
           ( name == "VK_EXT_filter_cubic" ) || ( name == "VK_EXT_global_priority" ) || ( name == "VK_EXT_external_memory_host" ) ||
           ( name == "VK_KHR_shader_clock" ) || ( name == "VK_EXT_calibrated_timestamps" ) || ( name == "VK_EXT_vertex_attribute_divisor" ) ||
           ( name == "VK_KHR_swapchain_mutable_format" ) || ( name == "VK_EXT_pci_bus_info" ) || ( name == "VK_KHR_shader_terminate_invocation" ) ||
           ( name == "VK_EXT_subgroup_size_control" ) || ( name == "VK_KHR_fragment_shading_rate" ) || ( name == "VK_EXT_shader_image_atomic_int64" ) ||
           ( name == "VK_EXT_memory_budget" ) || ( name == "VK_EXT_fragment_shader_interlock" ) || ( name == "VK_EXT_ycbcr_image_arrays" ) ||
           ( name == "VK_EXT_line_rasterization" ) || ( name == "VK_EXT_shader_atomic_float" ) || ( name == "VK_EXT_index_type_uint8" ) ||
           ( name == "VK_EXT_extended_dynamic_state" ) || ( name == "VK_EXT_shader_demote_to_helper_invocation" ) ||
           ( name == "VK_EXT_texel_buffer_alignment" ) || ( name == "VK_EXT_robustness2" ) || ( name == "VK_EXT_custom_border_color" ) ||
           ( name == "VK_KHR_object_refresh" ) || ( name == "VK_KHR_synchronization2" ) || ( name == "VK_EXT_ycbcr_2plane_444_formats" ) ||
           ( name == "VK_EXT_image_robustness" ) || ( name == "VK_KHR_copy_commands2" ) || ( name == "VK_EXT_4444_formats" ) ||
           ( name == "VK_EXT_vertex_input_dynamic_state" ) ||
#if defined( VK_USE_PLATFORM_SCI )
           ( name == "VK_NV_external_sci_sync" ) || ( name == "VK_NV_external_memory_sci_buf" ) ||
#endif /*VK_USE_PLATFORM_SCI*/
           ( name == "VK_EXT_extended_dynamic_state2" ) || ( name == "VK_EXT_color_write_enable" ) ||
#if defined( VK_USE_PLATFORM_SCI )
           ( name == "VK_NV_external_sci_sync2" )
#endif /*VK_USE_PLATFORM_SCI*/
      ;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isExtensionDeprecated( std::string const & name )
  {
    (void)name;
    return
#if defined( VK_USE_PLATFORM_SCI )
      ( name == "VK_NV_external_sci_sync" ) ||
#endif /*VK_USE_PLATFORM_SCI*/
      false;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isExtensionPromoted( std::string const & name )
  {
    return ( name == "VK_EXT_texture_compression_astc_hdr" ) || ( name == "VK_EXT_global_priority" ) || ( name == "VK_KHR_shader_terminate_invocation" ) ||
           ( name == "VK_EXT_subgroup_size_control" ) || ( name == "VK_EXT_extended_dynamic_state" ) ||
           ( name == "VK_EXT_shader_demote_to_helper_invocation" ) || ( name == "VK_EXT_texel_buffer_alignment" ) || ( name == "VK_KHR_synchronization2" ) ||
           ( name == "VK_EXT_ycbcr_2plane_444_formats" ) || ( name == "VK_EXT_image_robustness" ) || ( name == "VK_KHR_copy_commands2" ) ||
           ( name == "VK_EXT_4444_formats" ) || ( name == "VK_EXT_extended_dynamic_state2" );
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isInstanceExtension( std::string const & name )
  {
    return ( name == "VK_KHR_surface" ) || ( name == "VK_KHR_display" ) || ( name == "VK_EXT_direct_mode_display" ) ||
           ( name == "VK_EXT_display_surface_counter" ) || ( name == "VK_EXT_swapchain_colorspace" ) || ( name == "VK_KHR_get_surface_capabilities2" ) ||
           ( name == "VK_KHR_get_display_properties2" ) || ( name == "VK_EXT_debug_utils" ) || ( name == "VK_EXT_validation_features" ) ||
           ( name == "VK_EXT_headless_surface" ) || ( name == "VK_EXT_application_parameters" );
  }
}  // namespace VULKAN_HPP_NAMESPACE

#endif
