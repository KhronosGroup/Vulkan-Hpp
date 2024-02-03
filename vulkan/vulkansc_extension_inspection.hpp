// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_EXTENSION_INSPECTION_HPP
#define VULKAN_EXTENSION_INSPECTION_HPP

#include <map>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkansc.hpp>

namespace VULKAN_HPP_NAMESPACE
{
  //======================================
  //=== Extension inspection functions ===
  //======================================

  std::set<std::string> const &                                        getDeviceExtensions();
  std::set<std::string> const &                                        getInstanceExtensions();
  std::map<std::string, std::string> const &                           getDeprecatedExtensions();
  std::map<std::string, std::vector<std::vector<std::string>>> const & getExtensionDepends( std::string const & extension );

  std::map<std::string, std::string> const & getObsoletedExtensions();
  std::map<std::string, std::string> const & getPromotedExtensions();
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionObsoletedBy( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isDeprecatedExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isDeviceExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isInstanceExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isObsoletedExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isPromotedExtension( std::string const & extension );

  //=====================================================
  //=== Extension inspection function implementations ===
  //=====================================================

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getDeprecatedExtensions()
  {
    static std::map<std::string, std::string> deprecatedExtensions = {
      { "VK_EXT_validation_features", "VK_EXT_layer_settings" },
#if defined( VK_USE_PLATFORM_SCI )
      { "VK_NV_external_sci_sync", "VK_NV_external_sci_sync2" }
#endif /*VK_USE_PLATFORM_SCI*/
    };
    return deprecatedExtensions;
  }

  VULKAN_HPP_INLINE std::set<std::string> const & getDeviceExtensions()
  {
    static std::set<std::string> deviceExtensions = {
      "VK_KHR_swapchain",
      "VK_KHR_display_swapchain",
      "VK_EXT_depth_range_unrestricted",
      "VK_NV_private_vendor_info",
      "VK_EXT_texture_compression_astc_hdr",
      "VK_EXT_astc_decode_mode",
      "VK_KHR_external_memory_fd",
      "VK_KHR_external_semaphore_fd",
      "VK_KHR_incremental_present",
      "VK_EXT_display_control",
      "VK_EXT_discard_rectangles",
      "VK_EXT_conservative_rasterization",
      "VK_EXT_depth_clip_enable",
      "VK_EXT_hdr_metadata",
      "VK_KHR_shared_presentable_image",
      "VK_KHR_external_fence_fd",
      "VK_KHR_performance_query",
      "VK_EXT_external_memory_dma_buf",
      "VK_EXT_queue_family_foreign",
      "VK_EXT_shader_stencil_export",
      "VK_EXT_sample_locations",
      "VK_EXT_blend_operation_advanced",
      "VK_EXT_post_depth_coverage",
      "VK_EXT_image_drm_format_modifier",
      "VK_EXT_filter_cubic",
      "VK_EXT_global_priority",
      "VK_EXT_external_memory_host",
      "VK_KHR_shader_clock",
      "VK_EXT_calibrated_timestamps",
      "VK_EXT_vertex_attribute_divisor",
      "VK_KHR_swapchain_mutable_format",
      "VK_EXT_pci_bus_info",
      "VK_KHR_shader_terminate_invocation",
      "VK_EXT_subgroup_size_control",
      "VK_KHR_fragment_shading_rate",
      "VK_EXT_shader_image_atomic_int64",
      "VK_EXT_memory_budget",
      "VK_EXT_fragment_shader_interlock",
      "VK_EXT_ycbcr_image_arrays",
      "VK_EXT_line_rasterization",
      "VK_EXT_shader_atomic_float",
      "VK_EXT_index_type_uint8",
      "VK_EXT_extended_dynamic_state",
      "VK_EXT_shader_demote_to_helper_invocation",
      "VK_EXT_texel_buffer_alignment",
      "VK_EXT_robustness2",
      "VK_EXT_custom_border_color",
      "VK_KHR_object_refresh",
      "VK_KHR_synchronization2",
      "VK_EXT_ycbcr_2plane_444_formats",
      "VK_EXT_image_robustness",
      "VK_KHR_copy_commands2",
      "VK_EXT_4444_formats",
      "VK_EXT_vertex_input_dynamic_state",
#if defined( VK_USE_PLATFORM_SCI )
      "VK_NV_external_sci_sync",
      "VK_NV_external_memory_sci_buf",
#endif /*VK_USE_PLATFORM_SCI*/
      "VK_EXT_extended_dynamic_state2",
      "VK_EXT_color_write_enable",
#if defined( VK_USE_PLATFORM_SCI )
      "VK_NV_external_sci_sync2",
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      "VK_QNX_external_memory_screen_buffer"
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
    };
    return deviceExtensions;
  }

  VULKAN_HPP_INLINE std::set<std::string> const & getInstanceExtensions()
  {
    static std::set<std::string> instanceExtensions = { "VK_KHR_surface",
                                                        "VK_KHR_display",
                                                        "VK_EXT_direct_mode_display",
                                                        "VK_EXT_display_surface_counter",
                                                        "VK_EXT_swapchain_colorspace",
                                                        "VK_KHR_get_surface_capabilities2",
                                                        "VK_KHR_get_display_properties2",
                                                        "VK_EXT_debug_utils",
                                                        "VK_EXT_validation_features",
                                                        "VK_EXT_headless_surface",
                                                        "VK_EXT_application_parameters" };
    return instanceExtensions;
  }

  VULKAN_HPP_INLINE std::map<std::string, std::vector<std::vector<std::string>>> const & getExtensionDepends( std::string const & extension )
  {
    static std::map<std::string, std::vector<std::vector<std::string>>>                        noDependencies;
    static std::map<std::string, std::map<std::string, std::vector<std::vector<std::string>>>> dependencies = {
      { "VK_KHR_swapchain",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_surface",
            } } } } },
      { "VK_KHR_display",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_surface",
            } } } } },
      { "VK_KHR_display_swapchain",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_swapchain",
              "VK_KHR_display",
            } } } } },
      { "VK_EXT_texture_compression_astc_hdr",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_astc_decode_mode",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_external_memory_fd",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_memory",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_external_semaphore_fd",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_semaphore",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_incremental_present",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_swapchain",
            } } } } },
      { "VK_EXT_direct_mode_display",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_display",
            } } } } },
      { "VK_EXT_display_surface_counter",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_display",
            } } } } },
      { "VK_EXT_display_control",
        { { "VK_VERSION_1_0",
            { {
              "VK_EXT_display_surface_counter",
              "VK_KHR_swapchain",
            } } } } },
      { "VK_EXT_discard_rectangles",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_conservative_rasterization",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_depth_clip_enable",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_swapchain_colorspace",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_surface",
            } } } } },
      { "VK_EXT_hdr_metadata",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_swapchain",
            } } } } },
      { "VK_KHR_shared_presentable_image",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_swapchain",
              "VK_KHR_get_surface_capabilities2",
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1",
            { {
              "VK_KHR_swapchain",
              "VK_KHR_get_surface_capabilities2",
            } } } } },
      { "VK_KHR_external_fence_fd",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_fence",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_performance_query",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_get_surface_capabilities2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_surface",
            } } } } },
      { "VK_KHR_get_display_properties2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_display",
            } } } } },
      { "VK_EXT_external_memory_dma_buf",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_memory_fd",
            } } } } },
      { "VK_EXT_queue_family_foreign",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_memory",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_sample_locations",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_blend_operation_advanced",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_image_drm_format_modifier",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_bind_memory2",
              "VK_KHR_get_physical_device_properties2",
              "VK_KHR_sampler_ycbcr_conversion",
              "VK_KHR_image_format_list",
            } } },
          { "VK_VERSION_1_1",
            { {
              "VK_KHR_image_format_list",
            } } },
          { "VK_VERSION_1_2", { {} } } } },
      { "VK_EXT_external_memory_host",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_external_memory",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_shader_clock",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_calibrated_timestamps",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_vertex_attribute_divisor",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_swapchain_mutable_format",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_swapchain",
              "VK_KHR_maintenance2",
              "VK_KHR_image_format_list",
            } } },
          { "VK_VERSION_1_1",
            { {
              "VK_KHR_swapchain",
              "VK_KHR_image_format_list",
            } } },
          { "VK_VERSION_1_2",
            { {
              "VK_KHR_swapchain",
            } } } } },
      { "VK_EXT_pci_bus_info",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_shader_terminate_invocation",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_subgroup_size_control", { { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_fragment_shading_rate",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_create_renderpass2",
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1",
            { {
              "VK_KHR_create_renderpass2",
            } } },
          { "VK_VERSION_1_2", { {} } } } },
      { "VK_EXT_shader_image_atomic_int64",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_memory_budget",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_fragment_shader_interlock",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_ycbcr_image_arrays",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_sampler_ycbcr_conversion",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_headless_surface",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_surface",
            } } } } },
      { "VK_EXT_line_rasterization",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_shader_atomic_float",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_index_type_uint8",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_extended_dynamic_state",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_shader_demote_to_helper_invocation",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_texel_buffer_alignment",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_robustness2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_custom_border_color",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_synchronization2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_ycbcr_2plane_444_formats",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_sampler_ycbcr_conversion",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_image_robustness",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_KHR_copy_commands2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_4444_formats",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_vertex_input_dynamic_state",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
#if defined( VK_USE_PLATFORM_SCI )
      { "VK_NV_external_sci_sync", { { "VK_VERSION_1_1", { {} } } } },
      { "VK_NV_external_memory_sci_buf", { { "VK_VERSION_1_1", { {} } } } },
#endif /*VK_USE_PLATFORM_SCI*/
      { "VK_EXT_extended_dynamic_state2",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
      { "VK_EXT_color_write_enable",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_get_physical_device_properties2",
            } } },
          { "VK_VERSION_1_1", { {} } } } },
#if defined( VK_USE_PLATFORM_SCI )
      { "VK_NV_external_sci_sync2", { { "VK_VERSION_1_1", { {} } } } },
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      { "VK_QNX_external_memory_screen_buffer",
        { { "VK_VERSION_1_0",
            { {
              "VK_KHR_sampler_ycbcr_conversion",
              "VK_KHR_external_memory",
              "VK_KHR_dedicated_allocation",
            } } },
          { "VK_VERSION_1_1",
            { {
              "VK_EXT_queue_family_foreign",
            } } } } }
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
    };
    auto depIt = dependencies.find( extension );
    return ( depIt != dependencies.end() ) ? depIt->second : noDependencies;
  }

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getObsoletedExtensions()
  {
    static std::map<std::string, std::string> obsoletedExtensions = {};
    return obsoletedExtensions;
  }

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getPromotedExtensions()
  {
    static std::map<std::string, std::string> promotedExtensions = { { "VK_EXT_texture_compression_astc_hdr", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_global_priority", "VK_KHR_global_priority" },
                                                                     { "VK_EXT_calibrated_timestamps", "VK_KHR_calibrated_timestamps" },
                                                                     { "VK_EXT_vertex_attribute_divisor", "VK_KHR_vertex_attribute_divisor" },
                                                                     { "VK_KHR_shader_terminate_invocation", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_subgroup_size_control", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_line_rasterization", "VK_KHR_line_rasterization" },
                                                                     { "VK_EXT_index_type_uint8", "VK_KHR_index_type_uint8" },
                                                                     { "VK_EXT_extended_dynamic_state", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_shader_demote_to_helper_invocation", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_texel_buffer_alignment", "VK_VERSION_1_3" },
                                                                     { "VK_KHR_synchronization2", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_ycbcr_2plane_444_formats", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_image_robustness", "VK_VERSION_1_3" },
                                                                     { "VK_KHR_copy_commands2", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_4444_formats", "VK_VERSION_1_3" },
                                                                     { "VK_EXT_extended_dynamic_state2", "VK_VERSION_1_3" } };
    return promotedExtensions;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & extension )
  {
    (void)extension;

    if ( extension == "VK_EXT_validation_features" )
    {
      return "VK_EXT_layer_settings";
    }
#if defined( VK_USE_PLATFORM_SCI )
    if ( extension == "VK_NV_external_sci_sync" )
    {
      return "VK_NV_external_sci_sync2";
    }
#endif /*VK_USE_PLATFORM_SCI*/

    return "";
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionObsoletedBy( std::string const & extension )
  {
    (void)extension;

    return "";
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & extension )
  {
    if ( extension == "VK_EXT_texture_compression_astc_hdr" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_global_priority" )
    {
      return "VK_KHR_global_priority";
    }
    if ( extension == "VK_EXT_calibrated_timestamps" )
    {
      return "VK_KHR_calibrated_timestamps";
    }
    if ( extension == "VK_EXT_vertex_attribute_divisor" )
    {
      return "VK_KHR_vertex_attribute_divisor";
    }
    if ( extension == "VK_KHR_shader_terminate_invocation" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_subgroup_size_control" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_line_rasterization" )
    {
      return "VK_KHR_line_rasterization";
    }
    if ( extension == "VK_EXT_index_type_uint8" )
    {
      return "VK_KHR_index_type_uint8";
    }
    if ( extension == "VK_EXT_extended_dynamic_state" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_shader_demote_to_helper_invocation" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_texel_buffer_alignment" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_KHR_synchronization2" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_ycbcr_2plane_444_formats" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_image_robustness" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_KHR_copy_commands2" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_4444_formats" )
    {
      return "VK_VERSION_1_3";
    }
    if ( extension == "VK_EXT_extended_dynamic_state2" )
    {
      return "VK_VERSION_1_3";
    }
    return "";
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isDeprecatedExtension( std::string const & extension )
  {
    (void)extension;
    return ( extension == "VK_EXT_validation_features" ) ||
#if defined( VK_USE_PLATFORM_SCI )
           ( extension == "VK_NV_external_sci_sync" )
#endif /*VK_USE_PLATFORM_SCI*/
      ;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isDeviceExtension( std::string const & extension )
  {
    return ( extension == "VK_KHR_swapchain" ) || ( extension == "VK_KHR_display_swapchain" ) || ( extension == "VK_EXT_depth_range_unrestricted" ) ||
           ( extension == "VK_NV_private_vendor_info" ) || ( extension == "VK_EXT_texture_compression_astc_hdr" ) ||
           ( extension == "VK_EXT_astc_decode_mode" ) || ( extension == "VK_KHR_external_memory_fd" ) || ( extension == "VK_KHR_external_semaphore_fd" ) ||
           ( extension == "VK_KHR_incremental_present" ) || ( extension == "VK_EXT_display_control" ) || ( extension == "VK_EXT_discard_rectangles" ) ||
           ( extension == "VK_EXT_conservative_rasterization" ) || ( extension == "VK_EXT_depth_clip_enable" ) || ( extension == "VK_EXT_hdr_metadata" ) ||
           ( extension == "VK_KHR_shared_presentable_image" ) || ( extension == "VK_KHR_external_fence_fd" ) || ( extension == "VK_KHR_performance_query" ) ||
           ( extension == "VK_EXT_external_memory_dma_buf" ) || ( extension == "VK_EXT_queue_family_foreign" ) ||
           ( extension == "VK_EXT_shader_stencil_export" ) || ( extension == "VK_EXT_sample_locations" ) ||
           ( extension == "VK_EXT_blend_operation_advanced" ) || ( extension == "VK_EXT_post_depth_coverage" ) ||
           ( extension == "VK_EXT_image_drm_format_modifier" ) || ( extension == "VK_EXT_filter_cubic" ) || ( extension == "VK_EXT_global_priority" ) ||
           ( extension == "VK_EXT_external_memory_host" ) || ( extension == "VK_KHR_shader_clock" ) || ( extension == "VK_EXT_calibrated_timestamps" ) ||
           ( extension == "VK_EXT_vertex_attribute_divisor" ) || ( extension == "VK_KHR_swapchain_mutable_format" ) || ( extension == "VK_EXT_pci_bus_info" ) ||
           ( extension == "VK_KHR_shader_terminate_invocation" ) || ( extension == "VK_EXT_subgroup_size_control" ) ||
           ( extension == "VK_KHR_fragment_shading_rate" ) || ( extension == "VK_EXT_shader_image_atomic_int64" ) || ( extension == "VK_EXT_memory_budget" ) ||
           ( extension == "VK_EXT_fragment_shader_interlock" ) || ( extension == "VK_EXT_ycbcr_image_arrays" ) ||
           ( extension == "VK_EXT_line_rasterization" ) || ( extension == "VK_EXT_shader_atomic_float" ) || ( extension == "VK_EXT_index_type_uint8" ) ||
           ( extension == "VK_EXT_extended_dynamic_state" ) || ( extension == "VK_EXT_shader_demote_to_helper_invocation" ) ||
           ( extension == "VK_EXT_texel_buffer_alignment" ) || ( extension == "VK_EXT_robustness2" ) || ( extension == "VK_EXT_custom_border_color" ) ||
           ( extension == "VK_KHR_object_refresh" ) || ( extension == "VK_KHR_synchronization2" ) || ( extension == "VK_EXT_ycbcr_2plane_444_formats" ) ||
           ( extension == "VK_EXT_image_robustness" ) || ( extension == "VK_KHR_copy_commands2" ) || ( extension == "VK_EXT_4444_formats" ) ||
           ( extension == "VK_EXT_vertex_input_dynamic_state" )
#if defined( VK_USE_PLATFORM_SCI )
        || ( extension == "VK_NV_external_sci_sync" ) || ( extension == "VK_NV_external_memory_sci_buf" )
#endif /*VK_USE_PLATFORM_SCI*/
        || ( extension == "VK_EXT_extended_dynamic_state2" ) || ( extension == "VK_EXT_color_write_enable" )
#if defined( VK_USE_PLATFORM_SCI )
        || ( extension == "VK_NV_external_sci_sync2" )
#endif /*VK_USE_PLATFORM_SCI*/
#if defined( VK_USE_PLATFORM_SCREEN_QNX )
        || ( extension == "VK_QNX_external_memory_screen_buffer" )
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
      ;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isInstanceExtension( std::string const & extension )
  {
    return ( extension == "VK_KHR_surface" ) || ( extension == "VK_KHR_display" ) || ( extension == "VK_EXT_direct_mode_display" ) ||
           ( extension == "VK_EXT_display_surface_counter" ) || ( extension == "VK_EXT_swapchain_colorspace" ) ||
           ( extension == "VK_KHR_get_surface_capabilities2" ) || ( extension == "VK_KHR_get_display_properties2" ) || ( extension == "VK_EXT_debug_utils" ) ||
           ( extension == "VK_EXT_validation_features" ) || ( extension == "VK_EXT_headless_surface" ) || ( extension == "VK_EXT_application_parameters" );
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isObsoletedExtension( std::string const & extension )
  {
    (void)extension;
    return false;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isPromotedExtension( std::string const & extension )
  {
    return ( extension == "VK_EXT_texture_compression_astc_hdr" ) || ( extension == "VK_EXT_global_priority" ) ||
           ( extension == "VK_EXT_calibrated_timestamps" ) || ( extension == "VK_EXT_vertex_attribute_divisor" ) ||
           ( extension == "VK_KHR_shader_terminate_invocation" ) || ( extension == "VK_EXT_subgroup_size_control" ) ||
           ( extension == "VK_EXT_line_rasterization" ) || ( extension == "VK_EXT_index_type_uint8" ) || ( extension == "VK_EXT_extended_dynamic_state" ) ||
           ( extension == "VK_EXT_shader_demote_to_helper_invocation" ) || ( extension == "VK_EXT_texel_buffer_alignment" ) ||
           ( extension == "VK_KHR_synchronization2" ) || ( extension == "VK_EXT_ycbcr_2plane_444_formats" ) || ( extension == "VK_EXT_image_robustness" ) ||
           ( extension == "VK_KHR_copy_commands2" ) || ( extension == "VK_EXT_4444_formats" ) || ( extension == "VK_EXT_extended_dynamic_state2" );
  }
}  // namespace VULKAN_HPP_NAMESPACE

#endif
