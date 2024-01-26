// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_RAII_HPP
#define VULKAN_RAII_HPP

#include <memory>   // std::unique_ptr
#include <utility>  // std::exchange, std::forward
#include <vulkan/vulkansc.hpp>

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
namespace VULKAN_HPP_NAMESPACE
{
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    template <class T, class U = T>
    VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_INLINE T exchange( T & obj, U && newValue )
    {
#  if ( 14 <= VULKAN_HPP_CPP_VERSION )
      return std::exchange<T>( obj, std::forward<U>( newValue ) );
#  else
      T oldValue = std::move( obj );
      obj        = std::forward<U>( newValue );
      return oldValue;
#  endif
    }

    template <class T>
    class CreateReturnType
    {
    public:
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      using Type = VULKAN_HPP_EXPECTED<T, VULKAN_HPP_NAMESPACE::Result>;
#  else
      using Type                                                                = T;
#  endif
    };

    class ContextDispatcher : public DispatchLoaderBase
    {
    public:
      ContextDispatcher( PFN_vkGetInstanceProcAddr getProcAddr )
        : vkGetInstanceProcAddr( getProcAddr )
        //=== VK_VERSION_1_0 ===
        , vkCreateInstance( PFN_vkCreateInstance( getProcAddr( NULL, "vkCreateInstance" ) ) )
        , vkEnumerateInstanceExtensionProperties( PFN_vkEnumerateInstanceExtensionProperties( getProcAddr( NULL, "vkEnumerateInstanceExtensionProperties" ) ) )
        , vkEnumerateInstanceLayerProperties( PFN_vkEnumerateInstanceLayerProperties( getProcAddr( NULL, "vkEnumerateInstanceLayerProperties" ) ) )
        //=== VK_VERSION_1_1 ===
        , vkEnumerateInstanceVersion( PFN_vkEnumerateInstanceVersion( getProcAddr( NULL, "vkEnumerateInstanceVersion" ) ) )
      {
      }

    public:
      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = 0;

      //=== VK_VERSION_1_0 ===
      PFN_vkCreateInstance                       vkCreateInstance                       = 0;
      PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = 0;
      PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties     = 0;

      //=== VK_VERSION_1_1 ===
      PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = 0;
    };

    class InstanceDispatcher : public DispatchLoaderBase
    {
    public:
      InstanceDispatcher( PFN_vkGetInstanceProcAddr getProcAddr, VkInstance instance ) : vkGetInstanceProcAddr( getProcAddr )
      {
        //=== VK_VERSION_1_0 ===
        vkDestroyInstance           = PFN_vkDestroyInstance( vkGetInstanceProcAddr( instance, "vkDestroyInstance" ) );
        vkEnumeratePhysicalDevices  = PFN_vkEnumeratePhysicalDevices( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDevices" ) );
        vkGetPhysicalDeviceFeatures = PFN_vkGetPhysicalDeviceFeatures( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures" ) );
        vkGetPhysicalDeviceFormatProperties =
          PFN_vkGetPhysicalDeviceFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties" ) );
        vkGetPhysicalDeviceImageFormatProperties =
          PFN_vkGetPhysicalDeviceImageFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties" ) );
        vkGetPhysicalDeviceProperties = PFN_vkGetPhysicalDeviceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties" ) );
        vkGetPhysicalDeviceQueueFamilyProperties =
          PFN_vkGetPhysicalDeviceQueueFamilyProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties" ) );
        vkGetPhysicalDeviceMemoryProperties =
          PFN_vkGetPhysicalDeviceMemoryProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties" ) );
        vkCreateDevice = PFN_vkCreateDevice( vkGetInstanceProcAddr( instance, "vkCreateDevice" ) );
        vkEnumerateDeviceExtensionProperties =
          PFN_vkEnumerateDeviceExtensionProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceExtensionProperties" ) );
        vkEnumerateDeviceLayerProperties = PFN_vkEnumerateDeviceLayerProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceLayerProperties" ) );

        //=== VK_VERSION_1_1 ===
        vkEnumeratePhysicalDeviceGroups = PFN_vkEnumeratePhysicalDeviceGroups( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceGroups" ) );
        vkGetPhysicalDeviceFeatures2    = PFN_vkGetPhysicalDeviceFeatures2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures2" ) );
        vkGetPhysicalDeviceProperties2  = PFN_vkGetPhysicalDeviceProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties2" ) );
        vkGetPhysicalDeviceFormatProperties2 =
          PFN_vkGetPhysicalDeviceFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties2" ) );
        vkGetPhysicalDeviceImageFormatProperties2 =
          PFN_vkGetPhysicalDeviceImageFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties2" ) );
        vkGetPhysicalDeviceQueueFamilyProperties2 =
          PFN_vkGetPhysicalDeviceQueueFamilyProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties2" ) );
        vkGetPhysicalDeviceMemoryProperties2 =
          PFN_vkGetPhysicalDeviceMemoryProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties2" ) );
        vkGetPhysicalDeviceExternalBufferProperties =
          PFN_vkGetPhysicalDeviceExternalBufferProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalBufferProperties" ) );
        vkGetPhysicalDeviceExternalFenceProperties =
          PFN_vkGetPhysicalDeviceExternalFenceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalFenceProperties" ) );
        vkGetPhysicalDeviceExternalSemaphoreProperties =
          PFN_vkGetPhysicalDeviceExternalSemaphoreProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalSemaphoreProperties" ) );

        //=== VK_VERSION_1_3 ===
        vkGetPhysicalDeviceToolProperties = PFN_vkGetPhysicalDeviceToolProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceToolProperties" ) );

        //=== VK_KHR_surface ===
        vkDestroySurfaceKHR = PFN_vkDestroySurfaceKHR( vkGetInstanceProcAddr( instance, "vkDestroySurfaceKHR" ) );
        vkGetPhysicalDeviceSurfaceSupportKHR =
          PFN_vkGetPhysicalDeviceSurfaceSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceSupportKHR" ) );
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR =
          PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" ) );
        vkGetPhysicalDeviceSurfaceFormatsKHR =
          PFN_vkGetPhysicalDeviceSurfaceFormatsKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormatsKHR" ) );
        vkGetPhysicalDeviceSurfacePresentModesKHR =
          PFN_vkGetPhysicalDeviceSurfacePresentModesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfacePresentModesKHR" ) );

        //=== VK_KHR_swapchain ===
        vkGetPhysicalDevicePresentRectanglesKHR =
          PFN_vkGetPhysicalDevicePresentRectanglesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDevicePresentRectanglesKHR" ) );

        //=== VK_KHR_display ===
        vkGetPhysicalDeviceDisplayPropertiesKHR =
          PFN_vkGetPhysicalDeviceDisplayPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPropertiesKHR" ) );
        vkGetPhysicalDeviceDisplayPlanePropertiesKHR =
          PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR" ) );
        vkGetDisplayPlaneSupportedDisplaysKHR =
          PFN_vkGetDisplayPlaneSupportedDisplaysKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneSupportedDisplaysKHR" ) );
        vkGetDisplayModePropertiesKHR    = PFN_vkGetDisplayModePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModePropertiesKHR" ) );
        vkCreateDisplayModeKHR           = PFN_vkCreateDisplayModeKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayModeKHR" ) );
        vkGetDisplayPlaneCapabilitiesKHR = PFN_vkGetDisplayPlaneCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilitiesKHR" ) );
        vkCreateDisplayPlaneSurfaceKHR   = PFN_vkCreateDisplayPlaneSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayPlaneSurfaceKHR" ) );

        //=== VK_EXT_direct_mode_display ===
        vkReleaseDisplayEXT = PFN_vkReleaseDisplayEXT( vkGetInstanceProcAddr( instance, "vkReleaseDisplayEXT" ) );

        //=== VK_EXT_display_surface_counter ===
        vkGetPhysicalDeviceSurfaceCapabilities2EXT =
          PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT" ) );

        //=== VK_KHR_performance_query ===
        vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
          vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR" ) );
        vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
          vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR" ) );

        //=== VK_KHR_get_surface_capabilities2 ===
        vkGetPhysicalDeviceSurfaceCapabilities2KHR =
          PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR" ) );
        vkGetPhysicalDeviceSurfaceFormats2KHR =
          PFN_vkGetPhysicalDeviceSurfaceFormats2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormats2KHR" ) );

        //=== VK_KHR_get_display_properties2 ===
        vkGetPhysicalDeviceDisplayProperties2KHR =
          PFN_vkGetPhysicalDeviceDisplayProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayProperties2KHR" ) );
        vkGetPhysicalDeviceDisplayPlaneProperties2KHR =
          PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR" ) );
        vkGetDisplayModeProperties2KHR    = PFN_vkGetDisplayModeProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModeProperties2KHR" ) );
        vkGetDisplayPlaneCapabilities2KHR = PFN_vkGetDisplayPlaneCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilities2KHR" ) );

        //=== VK_EXT_debug_utils ===
        vkCreateDebugUtilsMessengerEXT  = PFN_vkCreateDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
        vkDestroyDebugUtilsMessengerEXT = PFN_vkDestroyDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
        vkSubmitDebugUtilsMessageEXT    = PFN_vkSubmitDebugUtilsMessageEXT( vkGetInstanceProcAddr( instance, "vkSubmitDebugUtilsMessageEXT" ) );

        //=== VK_EXT_sample_locations ===
        vkGetPhysicalDeviceMultisamplePropertiesEXT =
          PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT" ) );

        //=== VK_EXT_calibrated_timestamps ===
        vkGetPhysicalDeviceCalibrateableTimeDomainsEXT =
          PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT" ) );
        if ( !vkGetPhysicalDeviceCalibrateableTimeDomainsKHR )
          vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;

        //=== VK_KHR_fragment_shading_rate ===
        vkGetPhysicalDeviceFragmentShadingRatesKHR =
          PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFragmentShadingRatesKHR" ) );

        //=== VK_EXT_headless_surface ===
        vkCreateHeadlessSurfaceEXT = PFN_vkCreateHeadlessSurfaceEXT( vkGetInstanceProcAddr( instance, "vkCreateHeadlessSurfaceEXT" ) );

        //=== VK_KHR_object_refresh ===
        vkGetPhysicalDeviceRefreshableObjectTypesKHR =
          PFN_vkGetPhysicalDeviceRefreshableObjectTypesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceRefreshableObjectTypesKHR" ) );

#  if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_sci_sync ===
        vkGetPhysicalDeviceSciSyncAttributesNV =
          PFN_vkGetPhysicalDeviceSciSyncAttributesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSciSyncAttributesNV" ) );
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_memory_sci_buf ===
        vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV =
          PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV" ) );
        vkGetPhysicalDeviceSciBufAttributesNV =
          PFN_vkGetPhysicalDeviceSciBufAttributesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSciBufAttributesNV" ) );
#  endif /*VK_USE_PLATFORM_SCI*/

        vkGetDeviceProcAddr = PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr" ) );
      }

    public:
      //=== VK_VERSION_1_0 ===
      PFN_vkDestroyInstance                        vkDestroyInstance                        = 0;
      PFN_vkEnumeratePhysicalDevices               vkEnumeratePhysicalDevices               = 0;
      PFN_vkGetPhysicalDeviceFeatures              vkGetPhysicalDeviceFeatures              = 0;
      PFN_vkGetPhysicalDeviceFormatProperties      vkGetPhysicalDeviceFormatProperties      = 0;
      PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = 0;
      PFN_vkGetPhysicalDeviceProperties            vkGetPhysicalDeviceProperties            = 0;
      PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = 0;
      PFN_vkGetPhysicalDeviceMemoryProperties      vkGetPhysicalDeviceMemoryProperties      = 0;
      PFN_vkGetInstanceProcAddr                    vkGetInstanceProcAddr                    = 0;
      PFN_vkCreateDevice                           vkCreateDevice                           = 0;
      PFN_vkEnumerateDeviceExtensionProperties     vkEnumerateDeviceExtensionProperties     = 0;
      PFN_vkEnumerateDeviceLayerProperties         vkEnumerateDeviceLayerProperties         = 0;

      //=== VK_VERSION_1_1 ===
      PFN_vkEnumeratePhysicalDeviceGroups                vkEnumeratePhysicalDeviceGroups                = 0;
      PFN_vkGetPhysicalDeviceFeatures2                   vkGetPhysicalDeviceFeatures2                   = 0;
      PFN_vkGetPhysicalDeviceProperties2                 vkGetPhysicalDeviceProperties2                 = 0;
      PFN_vkGetPhysicalDeviceFormatProperties2           vkGetPhysicalDeviceFormatProperties2           = 0;
      PFN_vkGetPhysicalDeviceImageFormatProperties2      vkGetPhysicalDeviceImageFormatProperties2      = 0;
      PFN_vkGetPhysicalDeviceQueueFamilyProperties2      vkGetPhysicalDeviceQueueFamilyProperties2      = 0;
      PFN_vkGetPhysicalDeviceMemoryProperties2           vkGetPhysicalDeviceMemoryProperties2           = 0;
      PFN_vkGetPhysicalDeviceExternalBufferProperties    vkGetPhysicalDeviceExternalBufferProperties    = 0;
      PFN_vkGetPhysicalDeviceExternalFenceProperties     vkGetPhysicalDeviceExternalFenceProperties     = 0;
      PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties = 0;

      //=== VK_VERSION_1_3 ===
      PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties = 0;

      //=== VK_KHR_surface ===
      PFN_vkDestroySurfaceKHR                       vkDestroySurfaceKHR                       = 0;
      PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR      = 0;
      PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = 0;
      PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR      = 0;
      PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = 0;

      //=== VK_KHR_swapchain ===
      PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = 0;

      //=== VK_KHR_display ===
      PFN_vkGetPhysicalDeviceDisplayPropertiesKHR      vkGetPhysicalDeviceDisplayPropertiesKHR      = 0;
      PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = 0;
      PFN_vkGetDisplayPlaneSupportedDisplaysKHR        vkGetDisplayPlaneSupportedDisplaysKHR        = 0;
      PFN_vkGetDisplayModePropertiesKHR                vkGetDisplayModePropertiesKHR                = 0;
      PFN_vkCreateDisplayModeKHR                       vkCreateDisplayModeKHR                       = 0;
      PFN_vkGetDisplayPlaneCapabilitiesKHR             vkGetDisplayPlaneCapabilitiesKHR             = 0;
      PFN_vkCreateDisplayPlaneSurfaceKHR               vkCreateDisplayPlaneSurfaceKHR               = 0;

      //=== VK_EXT_direct_mode_display ===
      PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT = 0;

      //=== VK_EXT_display_surface_counter ===
      PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT = 0;

      //=== VK_KHR_performance_query ===
      PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = 0;
      PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         = 0;

      //=== VK_KHR_get_surface_capabilities2 ===
      PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR = 0;
      PFN_vkGetPhysicalDeviceSurfaceFormats2KHR      vkGetPhysicalDeviceSurfaceFormats2KHR      = 0;

      //=== VK_KHR_get_display_properties2 ===
      PFN_vkGetPhysicalDeviceDisplayProperties2KHR      vkGetPhysicalDeviceDisplayProperties2KHR      = 0;
      PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR = 0;
      PFN_vkGetDisplayModeProperties2KHR                vkGetDisplayModeProperties2KHR                = 0;
      PFN_vkGetDisplayPlaneCapabilities2KHR             vkGetDisplayPlaneCapabilities2KHR             = 0;

      //=== VK_EXT_debug_utils ===
      PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT  = 0;
      PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = 0;
      PFN_vkSubmitDebugUtilsMessageEXT    vkSubmitDebugUtilsMessageEXT    = 0;

      //=== VK_EXT_sample_locations ===
      PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT = 0;

      //=== VK_EXT_calibrated_timestamps ===
      PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = 0;

      //=== VK_KHR_fragment_shading_rate ===
      PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR = 0;

      //=== VK_EXT_headless_surface ===
      PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT = 0;

      //=== VK_KHR_object_refresh ===
      PFN_vkGetPhysicalDeviceRefreshableObjectTypesKHR vkGetPhysicalDeviceRefreshableObjectTypesKHR = 0;

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===
      PFN_vkGetPhysicalDeviceSciSyncAttributesNV vkGetPhysicalDeviceSciSyncAttributesNV = 0;
#  else
      PFN_dummy vkGetPhysicalDeviceSciSyncAttributesNV_placeholder              = 0;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===
      PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV = 0;
      PFN_vkGetPhysicalDeviceSciBufAttributesNV               vkGetPhysicalDeviceSciBufAttributesNV               = 0;
#  else
      PFN_dummy vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV_placeholder = 0;
      PFN_dummy vkGetPhysicalDeviceSciBufAttributesNV_placeholder               = 0;
#  endif /*VK_USE_PLATFORM_SCI*/

      PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = 0;
    };

    class DeviceDispatcher : public DispatchLoaderBase
    {
    public:
      DeviceDispatcher( PFN_vkGetDeviceProcAddr getProcAddr, VkDevice device ) : vkGetDeviceProcAddr( getProcAddr )
      {
        //=== VK_VERSION_1_0 ===
        vkGetDeviceProcAddr            = PFN_vkGetDeviceProcAddr( vkGetDeviceProcAddr( device, "vkGetDeviceProcAddr" ) );
        vkDestroyDevice                = PFN_vkDestroyDevice( vkGetDeviceProcAddr( device, "vkDestroyDevice" ) );
        vkGetDeviceQueue               = PFN_vkGetDeviceQueue( vkGetDeviceProcAddr( device, "vkGetDeviceQueue" ) );
        vkQueueSubmit                  = PFN_vkQueueSubmit( vkGetDeviceProcAddr( device, "vkQueueSubmit" ) );
        vkQueueWaitIdle                = PFN_vkQueueWaitIdle( vkGetDeviceProcAddr( device, "vkQueueWaitIdle" ) );
        vkDeviceWaitIdle               = PFN_vkDeviceWaitIdle( vkGetDeviceProcAddr( device, "vkDeviceWaitIdle" ) );
        vkAllocateMemory               = PFN_vkAllocateMemory( vkGetDeviceProcAddr( device, "vkAllocateMemory" ) );
        vkMapMemory                    = PFN_vkMapMemory( vkGetDeviceProcAddr( device, "vkMapMemory" ) );
        vkUnmapMemory                  = PFN_vkUnmapMemory( vkGetDeviceProcAddr( device, "vkUnmapMemory" ) );
        vkFlushMappedMemoryRanges      = PFN_vkFlushMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkFlushMappedMemoryRanges" ) );
        vkInvalidateMappedMemoryRanges = PFN_vkInvalidateMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkInvalidateMappedMemoryRanges" ) );
        vkGetDeviceMemoryCommitment    = PFN_vkGetDeviceMemoryCommitment( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryCommitment" ) );
        vkBindBufferMemory             = PFN_vkBindBufferMemory( vkGetDeviceProcAddr( device, "vkBindBufferMemory" ) );
        vkBindImageMemory              = PFN_vkBindImageMemory( vkGetDeviceProcAddr( device, "vkBindImageMemory" ) );
        vkGetBufferMemoryRequirements  = PFN_vkGetBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements" ) );
        vkGetImageMemoryRequirements   = PFN_vkGetImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements" ) );
        vkCreateFence                  = PFN_vkCreateFence( vkGetDeviceProcAddr( device, "vkCreateFence" ) );
        vkDestroyFence                 = PFN_vkDestroyFence( vkGetDeviceProcAddr( device, "vkDestroyFence" ) );
        vkResetFences                  = PFN_vkResetFences( vkGetDeviceProcAddr( device, "vkResetFences" ) );
        vkGetFenceStatus               = PFN_vkGetFenceStatus( vkGetDeviceProcAddr( device, "vkGetFenceStatus" ) );
        vkWaitForFences                = PFN_vkWaitForFences( vkGetDeviceProcAddr( device, "vkWaitForFences" ) );
        vkCreateSemaphore              = PFN_vkCreateSemaphore( vkGetDeviceProcAddr( device, "vkCreateSemaphore" ) );
        vkDestroySemaphore             = PFN_vkDestroySemaphore( vkGetDeviceProcAddr( device, "vkDestroySemaphore" ) );
        vkCreateEvent                  = PFN_vkCreateEvent( vkGetDeviceProcAddr( device, "vkCreateEvent" ) );
        vkDestroyEvent                 = PFN_vkDestroyEvent( vkGetDeviceProcAddr( device, "vkDestroyEvent" ) );
        vkGetEventStatus               = PFN_vkGetEventStatus( vkGetDeviceProcAddr( device, "vkGetEventStatus" ) );
        vkSetEvent                     = PFN_vkSetEvent( vkGetDeviceProcAddr( device, "vkSetEvent" ) );
        vkResetEvent                   = PFN_vkResetEvent( vkGetDeviceProcAddr( device, "vkResetEvent" ) );
        vkCreateQueryPool              = PFN_vkCreateQueryPool( vkGetDeviceProcAddr( device, "vkCreateQueryPool" ) );
        vkGetQueryPoolResults          = PFN_vkGetQueryPoolResults( vkGetDeviceProcAddr( device, "vkGetQueryPoolResults" ) );
        vkCreateBuffer                 = PFN_vkCreateBuffer( vkGetDeviceProcAddr( device, "vkCreateBuffer" ) );
        vkDestroyBuffer                = PFN_vkDestroyBuffer( vkGetDeviceProcAddr( device, "vkDestroyBuffer" ) );
        vkCreateBufferView             = PFN_vkCreateBufferView( vkGetDeviceProcAddr( device, "vkCreateBufferView" ) );
        vkDestroyBufferView            = PFN_vkDestroyBufferView( vkGetDeviceProcAddr( device, "vkDestroyBufferView" ) );
        vkCreateImage                  = PFN_vkCreateImage( vkGetDeviceProcAddr( device, "vkCreateImage" ) );
        vkDestroyImage                 = PFN_vkDestroyImage( vkGetDeviceProcAddr( device, "vkDestroyImage" ) );
        vkGetImageSubresourceLayout    = PFN_vkGetImageSubresourceLayout( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout" ) );
        vkCreateImageView              = PFN_vkCreateImageView( vkGetDeviceProcAddr( device, "vkCreateImageView" ) );
        vkDestroyImageView             = PFN_vkDestroyImageView( vkGetDeviceProcAddr( device, "vkDestroyImageView" ) );
        vkCreatePipelineCache          = PFN_vkCreatePipelineCache( vkGetDeviceProcAddr( device, "vkCreatePipelineCache" ) );
        vkDestroyPipelineCache         = PFN_vkDestroyPipelineCache( vkGetDeviceProcAddr( device, "vkDestroyPipelineCache" ) );
        vkCreateGraphicsPipelines      = PFN_vkCreateGraphicsPipelines( vkGetDeviceProcAddr( device, "vkCreateGraphicsPipelines" ) );
        vkCreateComputePipelines       = PFN_vkCreateComputePipelines( vkGetDeviceProcAddr( device, "vkCreateComputePipelines" ) );
        vkDestroyPipeline              = PFN_vkDestroyPipeline( vkGetDeviceProcAddr( device, "vkDestroyPipeline" ) );
        vkCreatePipelineLayout         = PFN_vkCreatePipelineLayout( vkGetDeviceProcAddr( device, "vkCreatePipelineLayout" ) );
        vkDestroyPipelineLayout        = PFN_vkDestroyPipelineLayout( vkGetDeviceProcAddr( device, "vkDestroyPipelineLayout" ) );
        vkCreateSampler                = PFN_vkCreateSampler( vkGetDeviceProcAddr( device, "vkCreateSampler" ) );
        vkDestroySampler               = PFN_vkDestroySampler( vkGetDeviceProcAddr( device, "vkDestroySampler" ) );
        vkCreateDescriptorSetLayout    = PFN_vkCreateDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkCreateDescriptorSetLayout" ) );
        vkDestroyDescriptorSetLayout   = PFN_vkDestroyDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkDestroyDescriptorSetLayout" ) );
        vkCreateDescriptorPool         = PFN_vkCreateDescriptorPool( vkGetDeviceProcAddr( device, "vkCreateDescriptorPool" ) );
        vkResetDescriptorPool          = PFN_vkResetDescriptorPool( vkGetDeviceProcAddr( device, "vkResetDescriptorPool" ) );
        vkAllocateDescriptorSets       = PFN_vkAllocateDescriptorSets( vkGetDeviceProcAddr( device, "vkAllocateDescriptorSets" ) );
        vkFreeDescriptorSets           = PFN_vkFreeDescriptorSets( vkGetDeviceProcAddr( device, "vkFreeDescriptorSets" ) );
        vkUpdateDescriptorSets         = PFN_vkUpdateDescriptorSets( vkGetDeviceProcAddr( device, "vkUpdateDescriptorSets" ) );
        vkCreateFramebuffer            = PFN_vkCreateFramebuffer( vkGetDeviceProcAddr( device, "vkCreateFramebuffer" ) );
        vkDestroyFramebuffer           = PFN_vkDestroyFramebuffer( vkGetDeviceProcAddr( device, "vkDestroyFramebuffer" ) );
        vkCreateRenderPass             = PFN_vkCreateRenderPass( vkGetDeviceProcAddr( device, "vkCreateRenderPass" ) );
        vkDestroyRenderPass            = PFN_vkDestroyRenderPass( vkGetDeviceProcAddr( device, "vkDestroyRenderPass" ) );
        vkGetRenderAreaGranularity     = PFN_vkGetRenderAreaGranularity( vkGetDeviceProcAddr( device, "vkGetRenderAreaGranularity" ) );
        vkCreateCommandPool            = PFN_vkCreateCommandPool( vkGetDeviceProcAddr( device, "vkCreateCommandPool" ) );
        vkResetCommandPool             = PFN_vkResetCommandPool( vkGetDeviceProcAddr( device, "vkResetCommandPool" ) );
        vkAllocateCommandBuffers       = PFN_vkAllocateCommandBuffers( vkGetDeviceProcAddr( device, "vkAllocateCommandBuffers" ) );
        vkFreeCommandBuffers           = PFN_vkFreeCommandBuffers( vkGetDeviceProcAddr( device, "vkFreeCommandBuffers" ) );
        vkBeginCommandBuffer           = PFN_vkBeginCommandBuffer( vkGetDeviceProcAddr( device, "vkBeginCommandBuffer" ) );
        vkEndCommandBuffer             = PFN_vkEndCommandBuffer( vkGetDeviceProcAddr( device, "vkEndCommandBuffer" ) );
        vkResetCommandBuffer           = PFN_vkResetCommandBuffer( vkGetDeviceProcAddr( device, "vkResetCommandBuffer" ) );
        vkCmdBindPipeline              = PFN_vkCmdBindPipeline( vkGetDeviceProcAddr( device, "vkCmdBindPipeline" ) );
        vkCmdSetViewport               = PFN_vkCmdSetViewport( vkGetDeviceProcAddr( device, "vkCmdSetViewport" ) );
        vkCmdSetScissor                = PFN_vkCmdSetScissor( vkGetDeviceProcAddr( device, "vkCmdSetScissor" ) );
        vkCmdSetLineWidth              = PFN_vkCmdSetLineWidth( vkGetDeviceProcAddr( device, "vkCmdSetLineWidth" ) );
        vkCmdSetDepthBias              = PFN_vkCmdSetDepthBias( vkGetDeviceProcAddr( device, "vkCmdSetDepthBias" ) );
        vkCmdSetBlendConstants         = PFN_vkCmdSetBlendConstants( vkGetDeviceProcAddr( device, "vkCmdSetBlendConstants" ) );
        vkCmdSetDepthBounds            = PFN_vkCmdSetDepthBounds( vkGetDeviceProcAddr( device, "vkCmdSetDepthBounds" ) );
        vkCmdSetStencilCompareMask     = PFN_vkCmdSetStencilCompareMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilCompareMask" ) );
        vkCmdSetStencilWriteMask       = PFN_vkCmdSetStencilWriteMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilWriteMask" ) );
        vkCmdSetStencilReference       = PFN_vkCmdSetStencilReference( vkGetDeviceProcAddr( device, "vkCmdSetStencilReference" ) );
        vkCmdBindDescriptorSets        = PFN_vkCmdBindDescriptorSets( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorSets" ) );
        vkCmdBindIndexBuffer           = PFN_vkCmdBindIndexBuffer( vkGetDeviceProcAddr( device, "vkCmdBindIndexBuffer" ) );
        vkCmdBindVertexBuffers         = PFN_vkCmdBindVertexBuffers( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers" ) );
        vkCmdDraw                      = PFN_vkCmdDraw( vkGetDeviceProcAddr( device, "vkCmdDraw" ) );
        vkCmdDrawIndexed               = PFN_vkCmdDrawIndexed( vkGetDeviceProcAddr( device, "vkCmdDrawIndexed" ) );
        vkCmdDrawIndirect              = PFN_vkCmdDrawIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndirect" ) );
        vkCmdDrawIndexedIndirect       = PFN_vkCmdDrawIndexedIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirect" ) );
        vkCmdDispatch                  = PFN_vkCmdDispatch( vkGetDeviceProcAddr( device, "vkCmdDispatch" ) );
        vkCmdDispatchIndirect          = PFN_vkCmdDispatchIndirect( vkGetDeviceProcAddr( device, "vkCmdDispatchIndirect" ) );
        vkCmdCopyBuffer                = PFN_vkCmdCopyBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer" ) );
        vkCmdCopyImage                 = PFN_vkCmdCopyImage( vkGetDeviceProcAddr( device, "vkCmdCopyImage" ) );
        vkCmdBlitImage                 = PFN_vkCmdBlitImage( vkGetDeviceProcAddr( device, "vkCmdBlitImage" ) );
        vkCmdCopyBufferToImage         = PFN_vkCmdCopyBufferToImage( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage" ) );
        vkCmdCopyImageToBuffer         = PFN_vkCmdCopyImageToBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer" ) );
        vkCmdUpdateBuffer              = PFN_vkCmdUpdateBuffer( vkGetDeviceProcAddr( device, "vkCmdUpdateBuffer" ) );
        vkCmdFillBuffer                = PFN_vkCmdFillBuffer( vkGetDeviceProcAddr( device, "vkCmdFillBuffer" ) );
        vkCmdClearColorImage           = PFN_vkCmdClearColorImage( vkGetDeviceProcAddr( device, "vkCmdClearColorImage" ) );
        vkCmdClearDepthStencilImage    = PFN_vkCmdClearDepthStencilImage( vkGetDeviceProcAddr( device, "vkCmdClearDepthStencilImage" ) );
        vkCmdClearAttachments          = PFN_vkCmdClearAttachments( vkGetDeviceProcAddr( device, "vkCmdClearAttachments" ) );
        vkCmdResolveImage              = PFN_vkCmdResolveImage( vkGetDeviceProcAddr( device, "vkCmdResolveImage" ) );
        vkCmdSetEvent                  = PFN_vkCmdSetEvent( vkGetDeviceProcAddr( device, "vkCmdSetEvent" ) );
        vkCmdResetEvent                = PFN_vkCmdResetEvent( vkGetDeviceProcAddr( device, "vkCmdResetEvent" ) );
        vkCmdWaitEvents                = PFN_vkCmdWaitEvents( vkGetDeviceProcAddr( device, "vkCmdWaitEvents" ) );
        vkCmdPipelineBarrier           = PFN_vkCmdPipelineBarrier( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier" ) );
        vkCmdBeginQuery                = PFN_vkCmdBeginQuery( vkGetDeviceProcAddr( device, "vkCmdBeginQuery" ) );
        vkCmdEndQuery                  = PFN_vkCmdEndQuery( vkGetDeviceProcAddr( device, "vkCmdEndQuery" ) );
        vkCmdResetQueryPool            = PFN_vkCmdResetQueryPool( vkGetDeviceProcAddr( device, "vkCmdResetQueryPool" ) );
        vkCmdWriteTimestamp            = PFN_vkCmdWriteTimestamp( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp" ) );
        vkCmdCopyQueryPoolResults      = PFN_vkCmdCopyQueryPoolResults( vkGetDeviceProcAddr( device, "vkCmdCopyQueryPoolResults" ) );
        vkCmdPushConstants             = PFN_vkCmdPushConstants( vkGetDeviceProcAddr( device, "vkCmdPushConstants" ) );
        vkCmdBeginRenderPass           = PFN_vkCmdBeginRenderPass( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass" ) );
        vkCmdNextSubpass               = PFN_vkCmdNextSubpass( vkGetDeviceProcAddr( device, "vkCmdNextSubpass" ) );
        vkCmdEndRenderPass             = PFN_vkCmdEndRenderPass( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass" ) );
        vkCmdExecuteCommands           = PFN_vkCmdExecuteCommands( vkGetDeviceProcAddr( device, "vkCmdExecuteCommands" ) );

        //=== VK_VERSION_1_1 ===
        vkBindBufferMemory2                = PFN_vkBindBufferMemory2( vkGetDeviceProcAddr( device, "vkBindBufferMemory2" ) );
        vkBindImageMemory2                 = PFN_vkBindImageMemory2( vkGetDeviceProcAddr( device, "vkBindImageMemory2" ) );
        vkGetDeviceGroupPeerMemoryFeatures = PFN_vkGetDeviceGroupPeerMemoryFeatures( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPeerMemoryFeatures" ) );
        vkCmdSetDeviceMask                 = PFN_vkCmdSetDeviceMask( vkGetDeviceProcAddr( device, "vkCmdSetDeviceMask" ) );
        vkCmdDispatchBase                  = PFN_vkCmdDispatchBase( vkGetDeviceProcAddr( device, "vkCmdDispatchBase" ) );
        vkGetImageMemoryRequirements2      = PFN_vkGetImageMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements2" ) );
        vkGetBufferMemoryRequirements2     = PFN_vkGetBufferMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements2" ) );
        vkGetDeviceQueue2                  = PFN_vkGetDeviceQueue2( vkGetDeviceProcAddr( device, "vkGetDeviceQueue2" ) );
        vkCreateSamplerYcbcrConversion     = PFN_vkCreateSamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkCreateSamplerYcbcrConversion" ) );
        vkDestroySamplerYcbcrConversion    = PFN_vkDestroySamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkDestroySamplerYcbcrConversion" ) );
        vkGetDescriptorSetLayoutSupport    = PFN_vkGetDescriptorSetLayoutSupport( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutSupport" ) );

        //=== VK_VERSION_1_2 ===
        vkCmdDrawIndirectCount          = PFN_vkCmdDrawIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectCount" ) );
        vkCmdDrawIndexedIndirectCount   = PFN_vkCmdDrawIndexedIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirectCount" ) );
        vkCreateRenderPass2             = PFN_vkCreateRenderPass2( vkGetDeviceProcAddr( device, "vkCreateRenderPass2" ) );
        vkCmdBeginRenderPass2           = PFN_vkCmdBeginRenderPass2( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass2" ) );
        vkCmdNextSubpass2               = PFN_vkCmdNextSubpass2( vkGetDeviceProcAddr( device, "vkCmdNextSubpass2" ) );
        vkCmdEndRenderPass2             = PFN_vkCmdEndRenderPass2( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass2" ) );
        vkResetQueryPool                = PFN_vkResetQueryPool( vkGetDeviceProcAddr( device, "vkResetQueryPool" ) );
        vkGetSemaphoreCounterValue      = PFN_vkGetSemaphoreCounterValue( vkGetDeviceProcAddr( device, "vkGetSemaphoreCounterValue" ) );
        vkWaitSemaphores                = PFN_vkWaitSemaphores( vkGetDeviceProcAddr( device, "vkWaitSemaphores" ) );
        vkSignalSemaphore               = PFN_vkSignalSemaphore( vkGetDeviceProcAddr( device, "vkSignalSemaphore" ) );
        vkGetBufferDeviceAddress        = PFN_vkGetBufferDeviceAddress( vkGetDeviceProcAddr( device, "vkGetBufferDeviceAddress" ) );
        vkGetBufferOpaqueCaptureAddress = PFN_vkGetBufferOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetBufferOpaqueCaptureAddress" ) );
        vkGetDeviceMemoryOpaqueCaptureAddress =
          PFN_vkGetDeviceMemoryOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryOpaqueCaptureAddress" ) );

        //=== VK_VERSION_1_3 ===
        vkCreatePrivateDataSlot             = PFN_vkCreatePrivateDataSlot( vkGetDeviceProcAddr( device, "vkCreatePrivateDataSlot" ) );
        vkDestroyPrivateDataSlot            = PFN_vkDestroyPrivateDataSlot( vkGetDeviceProcAddr( device, "vkDestroyPrivateDataSlot" ) );
        vkSetPrivateData                    = PFN_vkSetPrivateData( vkGetDeviceProcAddr( device, "vkSetPrivateData" ) );
        vkGetPrivateData                    = PFN_vkGetPrivateData( vkGetDeviceProcAddr( device, "vkGetPrivateData" ) );
        vkCmdSetEvent2                      = PFN_vkCmdSetEvent2( vkGetDeviceProcAddr( device, "vkCmdSetEvent2" ) );
        vkCmdResetEvent2                    = PFN_vkCmdResetEvent2( vkGetDeviceProcAddr( device, "vkCmdResetEvent2" ) );
        vkCmdWaitEvents2                    = PFN_vkCmdWaitEvents2( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2" ) );
        vkCmdPipelineBarrier2               = PFN_vkCmdPipelineBarrier2( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2" ) );
        vkCmdWriteTimestamp2                = PFN_vkCmdWriteTimestamp2( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2" ) );
        vkQueueSubmit2                      = PFN_vkQueueSubmit2( vkGetDeviceProcAddr( device, "vkQueueSubmit2" ) );
        vkCmdCopyBuffer2                    = PFN_vkCmdCopyBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2" ) );
        vkCmdCopyImage2                     = PFN_vkCmdCopyImage2( vkGetDeviceProcAddr( device, "vkCmdCopyImage2" ) );
        vkCmdCopyBufferToImage2             = PFN_vkCmdCopyBufferToImage2( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2" ) );
        vkCmdCopyImageToBuffer2             = PFN_vkCmdCopyImageToBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2" ) );
        vkCmdBlitImage2                     = PFN_vkCmdBlitImage2( vkGetDeviceProcAddr( device, "vkCmdBlitImage2" ) );
        vkCmdResolveImage2                  = PFN_vkCmdResolveImage2( vkGetDeviceProcAddr( device, "vkCmdResolveImage2" ) );
        vkCmdBeginRendering                 = PFN_vkCmdBeginRendering( vkGetDeviceProcAddr( device, "vkCmdBeginRendering" ) );
        vkCmdEndRendering                   = PFN_vkCmdEndRendering( vkGetDeviceProcAddr( device, "vkCmdEndRendering" ) );
        vkCmdSetCullMode                    = PFN_vkCmdSetCullMode( vkGetDeviceProcAddr( device, "vkCmdSetCullMode" ) );
        vkCmdSetFrontFace                   = PFN_vkCmdSetFrontFace( vkGetDeviceProcAddr( device, "vkCmdSetFrontFace" ) );
        vkCmdSetPrimitiveTopology           = PFN_vkCmdSetPrimitiveTopology( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopology" ) );
        vkCmdSetViewportWithCount           = PFN_vkCmdSetViewportWithCount( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCount" ) );
        vkCmdSetScissorWithCount            = PFN_vkCmdSetScissorWithCount( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCount" ) );
        vkCmdBindVertexBuffers2             = PFN_vkCmdBindVertexBuffers2( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2" ) );
        vkCmdSetDepthTestEnable             = PFN_vkCmdSetDepthTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnable" ) );
        vkCmdSetDepthWriteEnable            = PFN_vkCmdSetDepthWriteEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnable" ) );
        vkCmdSetDepthCompareOp              = PFN_vkCmdSetDepthCompareOp( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOp" ) );
        vkCmdSetDepthBoundsTestEnable       = PFN_vkCmdSetDepthBoundsTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnable" ) );
        vkCmdSetStencilTestEnable           = PFN_vkCmdSetStencilTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnable" ) );
        vkCmdSetStencilOp                   = PFN_vkCmdSetStencilOp( vkGetDeviceProcAddr( device, "vkCmdSetStencilOp" ) );
        vkCmdSetRasterizerDiscardEnable     = PFN_vkCmdSetRasterizerDiscardEnable( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnable" ) );
        vkCmdSetDepthBiasEnable             = PFN_vkCmdSetDepthBiasEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnable" ) );
        vkCmdSetPrimitiveRestartEnable      = PFN_vkCmdSetPrimitiveRestartEnable( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnable" ) );
        vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceBufferMemoryRequirements" ) );
        vkGetDeviceImageMemoryRequirements  = PFN_vkGetDeviceImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageMemoryRequirements" ) );
        vkGetDeviceImageSparseMemoryRequirements =
          PFN_vkGetDeviceImageSparseMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageSparseMemoryRequirements" ) );

        //=== VKSC_VERSION_1_0 ===
        vkGetCommandPoolMemoryConsumption = PFN_vkGetCommandPoolMemoryConsumption( vkGetDeviceProcAddr( device, "vkGetCommandPoolMemoryConsumption" ) );
        vkGetFaultData                    = PFN_vkGetFaultData( vkGetDeviceProcAddr( device, "vkGetFaultData" ) );

        //=== VK_KHR_swapchain ===
        vkCreateSwapchainKHR    = PFN_vkCreateSwapchainKHR( vkGetDeviceProcAddr( device, "vkCreateSwapchainKHR" ) );
        vkGetSwapchainImagesKHR = PFN_vkGetSwapchainImagesKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainImagesKHR" ) );
        vkAcquireNextImageKHR   = PFN_vkAcquireNextImageKHR( vkGetDeviceProcAddr( device, "vkAcquireNextImageKHR" ) );
        vkQueuePresentKHR       = PFN_vkQueuePresentKHR( vkGetDeviceProcAddr( device, "vkQueuePresentKHR" ) );
        vkGetDeviceGroupPresentCapabilitiesKHR =
          PFN_vkGetDeviceGroupPresentCapabilitiesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPresentCapabilitiesKHR" ) );
        vkGetDeviceGroupSurfacePresentModesKHR =
          PFN_vkGetDeviceGroupSurfacePresentModesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupSurfacePresentModesKHR" ) );
        vkAcquireNextImage2KHR = PFN_vkAcquireNextImage2KHR( vkGetDeviceProcAddr( device, "vkAcquireNextImage2KHR" ) );

        //=== VK_KHR_display_swapchain ===
        vkCreateSharedSwapchainsKHR = PFN_vkCreateSharedSwapchainsKHR( vkGetDeviceProcAddr( device, "vkCreateSharedSwapchainsKHR" ) );

        //=== VK_KHR_external_memory_fd ===
        vkGetMemoryFdKHR           = PFN_vkGetMemoryFdKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdKHR" ) );
        vkGetMemoryFdPropertiesKHR = PFN_vkGetMemoryFdPropertiesKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdPropertiesKHR" ) );

        //=== VK_KHR_external_semaphore_fd ===
        vkImportSemaphoreFdKHR = PFN_vkImportSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkImportSemaphoreFdKHR" ) );
        vkGetSemaphoreFdKHR    = PFN_vkGetSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkGetSemaphoreFdKHR" ) );

        //=== VK_EXT_display_control ===
        vkDisplayPowerControlEXT  = PFN_vkDisplayPowerControlEXT( vkGetDeviceProcAddr( device, "vkDisplayPowerControlEXT" ) );
        vkRegisterDeviceEventEXT  = PFN_vkRegisterDeviceEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDeviceEventEXT" ) );
        vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDisplayEventEXT" ) );
        vkGetSwapchainCounterEXT  = PFN_vkGetSwapchainCounterEXT( vkGetDeviceProcAddr( device, "vkGetSwapchainCounterEXT" ) );

        //=== VK_EXT_discard_rectangles ===
        vkCmdSetDiscardRectangleEXT       = PFN_vkCmdSetDiscardRectangleEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEXT" ) );
        vkCmdSetDiscardRectangleEnableEXT = PFN_vkCmdSetDiscardRectangleEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEnableEXT" ) );
        vkCmdSetDiscardRectangleModeEXT   = PFN_vkCmdSetDiscardRectangleModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleModeEXT" ) );

        //=== VK_EXT_hdr_metadata ===
        vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT( vkGetDeviceProcAddr( device, "vkSetHdrMetadataEXT" ) );

        //=== VK_KHR_shared_presentable_image ===
        vkGetSwapchainStatusKHR = PFN_vkGetSwapchainStatusKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainStatusKHR" ) );

        //=== VK_KHR_external_fence_fd ===
        vkImportFenceFdKHR = PFN_vkImportFenceFdKHR( vkGetDeviceProcAddr( device, "vkImportFenceFdKHR" ) );
        vkGetFenceFdKHR    = PFN_vkGetFenceFdKHR( vkGetDeviceProcAddr( device, "vkGetFenceFdKHR" ) );

        //=== VK_KHR_performance_query ===
        vkAcquireProfilingLockKHR = PFN_vkAcquireProfilingLockKHR( vkGetDeviceProcAddr( device, "vkAcquireProfilingLockKHR" ) );
        vkReleaseProfilingLockKHR = PFN_vkReleaseProfilingLockKHR( vkGetDeviceProcAddr( device, "vkReleaseProfilingLockKHR" ) );

        //=== VK_EXT_debug_utils ===
        vkSetDebugUtilsObjectNameEXT    = PFN_vkSetDebugUtilsObjectNameEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectNameEXT" ) );
        vkSetDebugUtilsObjectTagEXT     = PFN_vkSetDebugUtilsObjectTagEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectTagEXT" ) );
        vkQueueBeginDebugUtilsLabelEXT  = PFN_vkQueueBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueBeginDebugUtilsLabelEXT" ) );
        vkQueueEndDebugUtilsLabelEXT    = PFN_vkQueueEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueEndDebugUtilsLabelEXT" ) );
        vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueInsertDebugUtilsLabelEXT" ) );
        vkCmdBeginDebugUtilsLabelEXT    = PFN_vkCmdBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdBeginDebugUtilsLabelEXT" ) );
        vkCmdEndDebugUtilsLabelEXT      = PFN_vkCmdEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdEndDebugUtilsLabelEXT" ) );
        vkCmdInsertDebugUtilsLabelEXT   = PFN_vkCmdInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdInsertDebugUtilsLabelEXT" ) );

        //=== VK_EXT_sample_locations ===
        vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT( vkGetDeviceProcAddr( device, "vkCmdSetSampleLocationsEXT" ) );

        //=== VK_EXT_image_drm_format_modifier ===
        vkGetImageDrmFormatModifierPropertiesEXT =
          PFN_vkGetImageDrmFormatModifierPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetImageDrmFormatModifierPropertiesEXT" ) );

        //=== VK_EXT_external_memory_host ===
        vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetMemoryHostPointerPropertiesEXT" ) );

        //=== VK_EXT_calibrated_timestamps ===
        vkGetCalibratedTimestampsEXT = PFN_vkGetCalibratedTimestampsEXT( vkGetDeviceProcAddr( device, "vkGetCalibratedTimestampsEXT" ) );
        if ( !vkGetCalibratedTimestampsKHR )
          vkGetCalibratedTimestampsKHR = vkGetCalibratedTimestampsEXT;

        //=== VK_KHR_fragment_shading_rate ===
        vkCmdSetFragmentShadingRateKHR = PFN_vkCmdSetFragmentShadingRateKHR( vkGetDeviceProcAddr( device, "vkCmdSetFragmentShadingRateKHR" ) );

        //=== VK_EXT_line_rasterization ===
        vkCmdSetLineStippleEXT = PFN_vkCmdSetLineStippleEXT( vkGetDeviceProcAddr( device, "vkCmdSetLineStippleEXT" ) );
        if ( !vkCmdSetLineStippleKHR )
          vkCmdSetLineStippleKHR = vkCmdSetLineStippleEXT;

        //=== VK_EXT_extended_dynamic_state ===
        vkCmdSetCullModeEXT = PFN_vkCmdSetCullModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetCullModeEXT" ) );
        if ( !vkCmdSetCullMode )
          vkCmdSetCullMode = vkCmdSetCullModeEXT;
        vkCmdSetFrontFaceEXT = PFN_vkCmdSetFrontFaceEXT( vkGetDeviceProcAddr( device, "vkCmdSetFrontFaceEXT" ) );
        if ( !vkCmdSetFrontFace )
          vkCmdSetFrontFace = vkCmdSetFrontFaceEXT;
        vkCmdSetPrimitiveTopologyEXT = PFN_vkCmdSetPrimitiveTopologyEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopologyEXT" ) );
        if ( !vkCmdSetPrimitiveTopology )
          vkCmdSetPrimitiveTopology = vkCmdSetPrimitiveTopologyEXT;
        vkCmdSetViewportWithCountEXT = PFN_vkCmdSetViewportWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCountEXT" ) );
        if ( !vkCmdSetViewportWithCount )
          vkCmdSetViewportWithCount = vkCmdSetViewportWithCountEXT;
        vkCmdSetScissorWithCountEXT = PFN_vkCmdSetScissorWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCountEXT" ) );
        if ( !vkCmdSetScissorWithCount )
          vkCmdSetScissorWithCount = vkCmdSetScissorWithCountEXT;
        vkCmdBindVertexBuffers2EXT = PFN_vkCmdBindVertexBuffers2EXT( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2EXT" ) );
        if ( !vkCmdBindVertexBuffers2 )
          vkCmdBindVertexBuffers2 = vkCmdBindVertexBuffers2EXT;
        vkCmdSetDepthTestEnableEXT = PFN_vkCmdSetDepthTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnableEXT" ) );
        if ( !vkCmdSetDepthTestEnable )
          vkCmdSetDepthTestEnable = vkCmdSetDepthTestEnableEXT;
        vkCmdSetDepthWriteEnableEXT = PFN_vkCmdSetDepthWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnableEXT" ) );
        if ( !vkCmdSetDepthWriteEnable )
          vkCmdSetDepthWriteEnable = vkCmdSetDepthWriteEnableEXT;
        vkCmdSetDepthCompareOpEXT = PFN_vkCmdSetDepthCompareOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOpEXT" ) );
        if ( !vkCmdSetDepthCompareOp )
          vkCmdSetDepthCompareOp = vkCmdSetDepthCompareOpEXT;
        vkCmdSetDepthBoundsTestEnableEXT = PFN_vkCmdSetDepthBoundsTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnableEXT" ) );
        if ( !vkCmdSetDepthBoundsTestEnable )
          vkCmdSetDepthBoundsTestEnable = vkCmdSetDepthBoundsTestEnableEXT;
        vkCmdSetStencilTestEnableEXT = PFN_vkCmdSetStencilTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnableEXT" ) );
        if ( !vkCmdSetStencilTestEnable )
          vkCmdSetStencilTestEnable = vkCmdSetStencilTestEnableEXT;
        vkCmdSetStencilOpEXT = PFN_vkCmdSetStencilOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilOpEXT" ) );
        if ( !vkCmdSetStencilOp )
          vkCmdSetStencilOp = vkCmdSetStencilOpEXT;

        //=== VK_KHR_object_refresh ===
        vkCmdRefreshObjectsKHR = PFN_vkCmdRefreshObjectsKHR( vkGetDeviceProcAddr( device, "vkCmdRefreshObjectsKHR" ) );

        //=== VK_KHR_synchronization2 ===
        vkCmdSetEvent2KHR = PFN_vkCmdSetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdSetEvent2KHR" ) );
        if ( !vkCmdSetEvent2 )
          vkCmdSetEvent2 = vkCmdSetEvent2KHR;
        vkCmdResetEvent2KHR = PFN_vkCmdResetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdResetEvent2KHR" ) );
        if ( !vkCmdResetEvent2 )
          vkCmdResetEvent2 = vkCmdResetEvent2KHR;
        vkCmdWaitEvents2KHR = PFN_vkCmdWaitEvents2KHR( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2KHR" ) );
        if ( !vkCmdWaitEvents2 )
          vkCmdWaitEvents2 = vkCmdWaitEvents2KHR;
        vkCmdPipelineBarrier2KHR = PFN_vkCmdPipelineBarrier2KHR( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2KHR" ) );
        if ( !vkCmdPipelineBarrier2 )
          vkCmdPipelineBarrier2 = vkCmdPipelineBarrier2KHR;
        vkCmdWriteTimestamp2KHR = PFN_vkCmdWriteTimestamp2KHR( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2KHR" ) );
        if ( !vkCmdWriteTimestamp2 )
          vkCmdWriteTimestamp2 = vkCmdWriteTimestamp2KHR;
        vkQueueSubmit2KHR = PFN_vkQueueSubmit2KHR( vkGetDeviceProcAddr( device, "vkQueueSubmit2KHR" ) );
        if ( !vkQueueSubmit2 )
          vkQueueSubmit2 = vkQueueSubmit2KHR;
        vkCmdWriteBufferMarker2AMD  = PFN_vkCmdWriteBufferMarker2AMD( vkGetDeviceProcAddr( device, "vkCmdWriteBufferMarker2AMD" ) );
        vkGetQueueCheckpointData2NV = PFN_vkGetQueueCheckpointData2NV( vkGetDeviceProcAddr( device, "vkGetQueueCheckpointData2NV" ) );

        //=== VK_KHR_copy_commands2 ===
        vkCmdCopyBuffer2KHR = PFN_vkCmdCopyBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2KHR" ) );
        if ( !vkCmdCopyBuffer2 )
          vkCmdCopyBuffer2 = vkCmdCopyBuffer2KHR;
        vkCmdCopyImage2KHR = PFN_vkCmdCopyImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImage2KHR" ) );
        if ( !vkCmdCopyImage2 )
          vkCmdCopyImage2 = vkCmdCopyImage2KHR;
        vkCmdCopyBufferToImage2KHR = PFN_vkCmdCopyBufferToImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2KHR" ) );
        if ( !vkCmdCopyBufferToImage2 )
          vkCmdCopyBufferToImage2 = vkCmdCopyBufferToImage2KHR;
        vkCmdCopyImageToBuffer2KHR = PFN_vkCmdCopyImageToBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2KHR" ) );
        if ( !vkCmdCopyImageToBuffer2 )
          vkCmdCopyImageToBuffer2 = vkCmdCopyImageToBuffer2KHR;
        vkCmdBlitImage2KHR = PFN_vkCmdBlitImage2KHR( vkGetDeviceProcAddr( device, "vkCmdBlitImage2KHR" ) );
        if ( !vkCmdBlitImage2 )
          vkCmdBlitImage2 = vkCmdBlitImage2KHR;
        vkCmdResolveImage2KHR = PFN_vkCmdResolveImage2KHR( vkGetDeviceProcAddr( device, "vkCmdResolveImage2KHR" ) );
        if ( !vkCmdResolveImage2 )
          vkCmdResolveImage2 = vkCmdResolveImage2KHR;

        //=== VK_EXT_vertex_input_dynamic_state ===
        vkCmdSetVertexInputEXT = PFN_vkCmdSetVertexInputEXT( vkGetDeviceProcAddr( device, "vkCmdSetVertexInputEXT" ) );

#  if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_sci_sync ===
        vkGetFenceSciSyncFenceNV      = PFN_vkGetFenceSciSyncFenceNV( vkGetDeviceProcAddr( device, "vkGetFenceSciSyncFenceNV" ) );
        vkGetFenceSciSyncObjNV        = PFN_vkGetFenceSciSyncObjNV( vkGetDeviceProcAddr( device, "vkGetFenceSciSyncObjNV" ) );
        vkImportFenceSciSyncFenceNV   = PFN_vkImportFenceSciSyncFenceNV( vkGetDeviceProcAddr( device, "vkImportFenceSciSyncFenceNV" ) );
        vkImportFenceSciSyncObjNV     = PFN_vkImportFenceSciSyncObjNV( vkGetDeviceProcAddr( device, "vkImportFenceSciSyncObjNV" ) );
        vkGetSemaphoreSciSyncObjNV    = PFN_vkGetSemaphoreSciSyncObjNV( vkGetDeviceProcAddr( device, "vkGetSemaphoreSciSyncObjNV" ) );
        vkImportSemaphoreSciSyncObjNV = PFN_vkImportSemaphoreSciSyncObjNV( vkGetDeviceProcAddr( device, "vkImportSemaphoreSciSyncObjNV" ) );
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_memory_sci_buf ===
        vkGetMemorySciBufNV = PFN_vkGetMemorySciBufNV( vkGetDeviceProcAddr( device, "vkGetMemorySciBufNV" ) );
#  endif /*VK_USE_PLATFORM_SCI*/

        //=== VK_EXT_extended_dynamic_state2 ===
        vkCmdSetPatchControlPointsEXT      = PFN_vkCmdSetPatchControlPointsEXT( vkGetDeviceProcAddr( device, "vkCmdSetPatchControlPointsEXT" ) );
        vkCmdSetRasterizerDiscardEnableEXT = PFN_vkCmdSetRasterizerDiscardEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnableEXT" ) );
        if ( !vkCmdSetRasterizerDiscardEnable )
          vkCmdSetRasterizerDiscardEnable = vkCmdSetRasterizerDiscardEnableEXT;
        vkCmdSetDepthBiasEnableEXT = PFN_vkCmdSetDepthBiasEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnableEXT" ) );
        if ( !vkCmdSetDepthBiasEnable )
          vkCmdSetDepthBiasEnable = vkCmdSetDepthBiasEnableEXT;
        vkCmdSetLogicOpEXT                = PFN_vkCmdSetLogicOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetLogicOpEXT" ) );
        vkCmdSetPrimitiveRestartEnableEXT = PFN_vkCmdSetPrimitiveRestartEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnableEXT" ) );
        if ( !vkCmdSetPrimitiveRestartEnable )
          vkCmdSetPrimitiveRestartEnable = vkCmdSetPrimitiveRestartEnableEXT;

        //=== VK_EXT_color_write_enable ===
        vkCmdSetColorWriteEnableEXT = PFN_vkCmdSetColorWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorWriteEnableEXT" ) );

#  if defined( VK_USE_PLATFORM_SCI )
        //=== VK_NV_external_sci_sync2 ===
        vkCreateSemaphoreSciSyncPoolNV = PFN_vkCreateSemaphoreSciSyncPoolNV( vkGetDeviceProcAddr( device, "vkCreateSemaphoreSciSyncPoolNV" ) );
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
        //=== VK_QNX_external_memory_screen_buffer ===
        vkGetScreenBufferPropertiesQNX = PFN_vkGetScreenBufferPropertiesQNX( vkGetDeviceProcAddr( device, "vkGetScreenBufferPropertiesQNX" ) );
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/
      }

    public:
      //=== VK_VERSION_1_0 ===
      PFN_vkGetDeviceProcAddr            vkGetDeviceProcAddr            = 0;
      PFN_vkDestroyDevice                vkDestroyDevice                = 0;
      PFN_vkGetDeviceQueue               vkGetDeviceQueue               = 0;
      PFN_vkQueueSubmit                  vkQueueSubmit                  = 0;
      PFN_vkQueueWaitIdle                vkQueueWaitIdle                = 0;
      PFN_vkDeviceWaitIdle               vkDeviceWaitIdle               = 0;
      PFN_vkAllocateMemory               vkAllocateMemory               = 0;
      PFN_vkMapMemory                    vkMapMemory                    = 0;
      PFN_vkUnmapMemory                  vkUnmapMemory                  = 0;
      PFN_vkFlushMappedMemoryRanges      vkFlushMappedMemoryRanges      = 0;
      PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = 0;
      PFN_vkGetDeviceMemoryCommitment    vkGetDeviceMemoryCommitment    = 0;
      PFN_vkBindBufferMemory             vkBindBufferMemory             = 0;
      PFN_vkBindImageMemory              vkBindImageMemory              = 0;
      PFN_vkGetBufferMemoryRequirements  vkGetBufferMemoryRequirements  = 0;
      PFN_vkGetImageMemoryRequirements   vkGetImageMemoryRequirements   = 0;
      PFN_vkCreateFence                  vkCreateFence                  = 0;
      PFN_vkDestroyFence                 vkDestroyFence                 = 0;
      PFN_vkResetFences                  vkResetFences                  = 0;
      PFN_vkGetFenceStatus               vkGetFenceStatus               = 0;
      PFN_vkWaitForFences                vkWaitForFences                = 0;
      PFN_vkCreateSemaphore              vkCreateSemaphore              = 0;
      PFN_vkDestroySemaphore             vkDestroySemaphore             = 0;
      PFN_vkCreateEvent                  vkCreateEvent                  = 0;
      PFN_vkDestroyEvent                 vkDestroyEvent                 = 0;
      PFN_vkGetEventStatus               vkGetEventStatus               = 0;
      PFN_vkSetEvent                     vkSetEvent                     = 0;
      PFN_vkResetEvent                   vkResetEvent                   = 0;
      PFN_vkCreateQueryPool              vkCreateQueryPool              = 0;
      PFN_vkGetQueryPoolResults          vkGetQueryPoolResults          = 0;
      PFN_vkCreateBuffer                 vkCreateBuffer                 = 0;
      PFN_vkDestroyBuffer                vkDestroyBuffer                = 0;
      PFN_vkCreateBufferView             vkCreateBufferView             = 0;
      PFN_vkDestroyBufferView            vkDestroyBufferView            = 0;
      PFN_vkCreateImage                  vkCreateImage                  = 0;
      PFN_vkDestroyImage                 vkDestroyImage                 = 0;
      PFN_vkGetImageSubresourceLayout    vkGetImageSubresourceLayout    = 0;
      PFN_vkCreateImageView              vkCreateImageView              = 0;
      PFN_vkDestroyImageView             vkDestroyImageView             = 0;
      PFN_vkCreatePipelineCache          vkCreatePipelineCache          = 0;
      PFN_vkDestroyPipelineCache         vkDestroyPipelineCache         = 0;
      PFN_vkCreateGraphicsPipelines      vkCreateGraphicsPipelines      = 0;
      PFN_vkCreateComputePipelines       vkCreateComputePipelines       = 0;
      PFN_vkDestroyPipeline              vkDestroyPipeline              = 0;
      PFN_vkCreatePipelineLayout         vkCreatePipelineLayout         = 0;
      PFN_vkDestroyPipelineLayout        vkDestroyPipelineLayout        = 0;
      PFN_vkCreateSampler                vkCreateSampler                = 0;
      PFN_vkDestroySampler               vkDestroySampler               = 0;
      PFN_vkCreateDescriptorSetLayout    vkCreateDescriptorSetLayout    = 0;
      PFN_vkDestroyDescriptorSetLayout   vkDestroyDescriptorSetLayout   = 0;
      PFN_vkCreateDescriptorPool         vkCreateDescriptorPool         = 0;
      PFN_vkResetDescriptorPool          vkResetDescriptorPool          = 0;
      PFN_vkAllocateDescriptorSets       vkAllocateDescriptorSets       = 0;
      PFN_vkFreeDescriptorSets           vkFreeDescriptorSets           = 0;
      PFN_vkUpdateDescriptorSets         vkUpdateDescriptorSets         = 0;
      PFN_vkCreateFramebuffer            vkCreateFramebuffer            = 0;
      PFN_vkDestroyFramebuffer           vkDestroyFramebuffer           = 0;
      PFN_vkCreateRenderPass             vkCreateRenderPass             = 0;
      PFN_vkDestroyRenderPass            vkDestroyRenderPass            = 0;
      PFN_vkGetRenderAreaGranularity     vkGetRenderAreaGranularity     = 0;
      PFN_vkCreateCommandPool            vkCreateCommandPool            = 0;
      PFN_vkResetCommandPool             vkResetCommandPool             = 0;
      PFN_vkAllocateCommandBuffers       vkAllocateCommandBuffers       = 0;
      PFN_vkFreeCommandBuffers           vkFreeCommandBuffers           = 0;
      PFN_vkBeginCommandBuffer           vkBeginCommandBuffer           = 0;
      PFN_vkEndCommandBuffer             vkEndCommandBuffer             = 0;
      PFN_vkResetCommandBuffer           vkResetCommandBuffer           = 0;
      PFN_vkCmdBindPipeline              vkCmdBindPipeline              = 0;
      PFN_vkCmdSetViewport               vkCmdSetViewport               = 0;
      PFN_vkCmdSetScissor                vkCmdSetScissor                = 0;
      PFN_vkCmdSetLineWidth              vkCmdSetLineWidth              = 0;
      PFN_vkCmdSetDepthBias              vkCmdSetDepthBias              = 0;
      PFN_vkCmdSetBlendConstants         vkCmdSetBlendConstants         = 0;
      PFN_vkCmdSetDepthBounds            vkCmdSetDepthBounds            = 0;
      PFN_vkCmdSetStencilCompareMask     vkCmdSetStencilCompareMask     = 0;
      PFN_vkCmdSetStencilWriteMask       vkCmdSetStencilWriteMask       = 0;
      PFN_vkCmdSetStencilReference       vkCmdSetStencilReference       = 0;
      PFN_vkCmdBindDescriptorSets        vkCmdBindDescriptorSets        = 0;
      PFN_vkCmdBindIndexBuffer           vkCmdBindIndexBuffer           = 0;
      PFN_vkCmdBindVertexBuffers         vkCmdBindVertexBuffers         = 0;
      PFN_vkCmdDraw                      vkCmdDraw                      = 0;
      PFN_vkCmdDrawIndexed               vkCmdDrawIndexed               = 0;
      PFN_vkCmdDrawIndirect              vkCmdDrawIndirect              = 0;
      PFN_vkCmdDrawIndexedIndirect       vkCmdDrawIndexedIndirect       = 0;
      PFN_vkCmdDispatch                  vkCmdDispatch                  = 0;
      PFN_vkCmdDispatchIndirect          vkCmdDispatchIndirect          = 0;
      PFN_vkCmdCopyBuffer                vkCmdCopyBuffer                = 0;
      PFN_vkCmdCopyImage                 vkCmdCopyImage                 = 0;
      PFN_vkCmdBlitImage                 vkCmdBlitImage                 = 0;
      PFN_vkCmdCopyBufferToImage         vkCmdCopyBufferToImage         = 0;
      PFN_vkCmdCopyImageToBuffer         vkCmdCopyImageToBuffer         = 0;
      PFN_vkCmdUpdateBuffer              vkCmdUpdateBuffer              = 0;
      PFN_vkCmdFillBuffer                vkCmdFillBuffer                = 0;
      PFN_vkCmdClearColorImage           vkCmdClearColorImage           = 0;
      PFN_vkCmdClearDepthStencilImage    vkCmdClearDepthStencilImage    = 0;
      PFN_vkCmdClearAttachments          vkCmdClearAttachments          = 0;
      PFN_vkCmdResolveImage              vkCmdResolveImage              = 0;
      PFN_vkCmdSetEvent                  vkCmdSetEvent                  = 0;
      PFN_vkCmdResetEvent                vkCmdResetEvent                = 0;
      PFN_vkCmdWaitEvents                vkCmdWaitEvents                = 0;
      PFN_vkCmdPipelineBarrier           vkCmdPipelineBarrier           = 0;
      PFN_vkCmdBeginQuery                vkCmdBeginQuery                = 0;
      PFN_vkCmdEndQuery                  vkCmdEndQuery                  = 0;
      PFN_vkCmdResetQueryPool            vkCmdResetQueryPool            = 0;
      PFN_vkCmdWriteTimestamp            vkCmdWriteTimestamp            = 0;
      PFN_vkCmdCopyQueryPoolResults      vkCmdCopyQueryPoolResults      = 0;
      PFN_vkCmdPushConstants             vkCmdPushConstants             = 0;
      PFN_vkCmdBeginRenderPass           vkCmdBeginRenderPass           = 0;
      PFN_vkCmdNextSubpass               vkCmdNextSubpass               = 0;
      PFN_vkCmdEndRenderPass             vkCmdEndRenderPass             = 0;
      PFN_vkCmdExecuteCommands           vkCmdExecuteCommands           = 0;

      //=== VK_VERSION_1_1 ===
      PFN_vkBindBufferMemory2                vkBindBufferMemory2                = 0;
      PFN_vkBindImageMemory2                 vkBindImageMemory2                 = 0;
      PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures = 0;
      PFN_vkCmdSetDeviceMask                 vkCmdSetDeviceMask                 = 0;
      PFN_vkCmdDispatchBase                  vkCmdDispatchBase                  = 0;
      PFN_vkGetImageMemoryRequirements2      vkGetImageMemoryRequirements2      = 0;
      PFN_vkGetBufferMemoryRequirements2     vkGetBufferMemoryRequirements2     = 0;
      PFN_vkGetDeviceQueue2                  vkGetDeviceQueue2                  = 0;
      PFN_vkCreateSamplerYcbcrConversion     vkCreateSamplerYcbcrConversion     = 0;
      PFN_vkDestroySamplerYcbcrConversion    vkDestroySamplerYcbcrConversion    = 0;
      PFN_vkGetDescriptorSetLayoutSupport    vkGetDescriptorSetLayoutSupport    = 0;

      //=== VK_VERSION_1_2 ===
      PFN_vkCmdDrawIndirectCount                vkCmdDrawIndirectCount                = 0;
      PFN_vkCmdDrawIndexedIndirectCount         vkCmdDrawIndexedIndirectCount         = 0;
      PFN_vkCreateRenderPass2                   vkCreateRenderPass2                   = 0;
      PFN_vkCmdBeginRenderPass2                 vkCmdBeginRenderPass2                 = 0;
      PFN_vkCmdNextSubpass2                     vkCmdNextSubpass2                     = 0;
      PFN_vkCmdEndRenderPass2                   vkCmdEndRenderPass2                   = 0;
      PFN_vkResetQueryPool                      vkResetQueryPool                      = 0;
      PFN_vkGetSemaphoreCounterValue            vkGetSemaphoreCounterValue            = 0;
      PFN_vkWaitSemaphores                      vkWaitSemaphores                      = 0;
      PFN_vkSignalSemaphore                     vkSignalSemaphore                     = 0;
      PFN_vkGetBufferDeviceAddress              vkGetBufferDeviceAddress              = 0;
      PFN_vkGetBufferOpaqueCaptureAddress       vkGetBufferOpaqueCaptureAddress       = 0;
      PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress = 0;

      //=== VK_VERSION_1_3 ===
      PFN_vkCreatePrivateDataSlot                  vkCreatePrivateDataSlot                  = 0;
      PFN_vkDestroyPrivateDataSlot                 vkDestroyPrivateDataSlot                 = 0;
      PFN_vkSetPrivateData                         vkSetPrivateData                         = 0;
      PFN_vkGetPrivateData                         vkGetPrivateData                         = 0;
      PFN_vkCmdSetEvent2                           vkCmdSetEvent2                           = 0;
      PFN_vkCmdResetEvent2                         vkCmdResetEvent2                         = 0;
      PFN_vkCmdWaitEvents2                         vkCmdWaitEvents2                         = 0;
      PFN_vkCmdPipelineBarrier2                    vkCmdPipelineBarrier2                    = 0;
      PFN_vkCmdWriteTimestamp2                     vkCmdWriteTimestamp2                     = 0;
      PFN_vkQueueSubmit2                           vkQueueSubmit2                           = 0;
      PFN_vkCmdCopyBuffer2                         vkCmdCopyBuffer2                         = 0;
      PFN_vkCmdCopyImage2                          vkCmdCopyImage2                          = 0;
      PFN_vkCmdCopyBufferToImage2                  vkCmdCopyBufferToImage2                  = 0;
      PFN_vkCmdCopyImageToBuffer2                  vkCmdCopyImageToBuffer2                  = 0;
      PFN_vkCmdBlitImage2                          vkCmdBlitImage2                          = 0;
      PFN_vkCmdResolveImage2                       vkCmdResolveImage2                       = 0;
      PFN_vkCmdBeginRendering                      vkCmdBeginRendering                      = 0;
      PFN_vkCmdEndRendering                        vkCmdEndRendering                        = 0;
      PFN_vkCmdSetCullMode                         vkCmdSetCullMode                         = 0;
      PFN_vkCmdSetFrontFace                        vkCmdSetFrontFace                        = 0;
      PFN_vkCmdSetPrimitiveTopology                vkCmdSetPrimitiveTopology                = 0;
      PFN_vkCmdSetViewportWithCount                vkCmdSetViewportWithCount                = 0;
      PFN_vkCmdSetScissorWithCount                 vkCmdSetScissorWithCount                 = 0;
      PFN_vkCmdBindVertexBuffers2                  vkCmdBindVertexBuffers2                  = 0;
      PFN_vkCmdSetDepthTestEnable                  vkCmdSetDepthTestEnable                  = 0;
      PFN_vkCmdSetDepthWriteEnable                 vkCmdSetDepthWriteEnable                 = 0;
      PFN_vkCmdSetDepthCompareOp                   vkCmdSetDepthCompareOp                   = 0;
      PFN_vkCmdSetDepthBoundsTestEnable            vkCmdSetDepthBoundsTestEnable            = 0;
      PFN_vkCmdSetStencilTestEnable                vkCmdSetStencilTestEnable                = 0;
      PFN_vkCmdSetStencilOp                        vkCmdSetStencilOp                        = 0;
      PFN_vkCmdSetRasterizerDiscardEnable          vkCmdSetRasterizerDiscardEnable          = 0;
      PFN_vkCmdSetDepthBiasEnable                  vkCmdSetDepthBiasEnable                  = 0;
      PFN_vkCmdSetPrimitiveRestartEnable           vkCmdSetPrimitiveRestartEnable           = 0;
      PFN_vkGetDeviceBufferMemoryRequirements      vkGetDeviceBufferMemoryRequirements      = 0;
      PFN_vkGetDeviceImageMemoryRequirements       vkGetDeviceImageMemoryRequirements       = 0;
      PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements = 0;

      //=== VKSC_VERSION_1_0 ===
      PFN_vkGetCommandPoolMemoryConsumption vkGetCommandPoolMemoryConsumption = 0;
      PFN_vkGetFaultData                    vkGetFaultData                    = 0;

      //=== VK_KHR_swapchain ===
      PFN_vkCreateSwapchainKHR                   vkCreateSwapchainKHR                   = 0;
      PFN_vkGetSwapchainImagesKHR                vkGetSwapchainImagesKHR                = 0;
      PFN_vkAcquireNextImageKHR                  vkAcquireNextImageKHR                  = 0;
      PFN_vkQueuePresentKHR                      vkQueuePresentKHR                      = 0;
      PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR = 0;
      PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR = 0;
      PFN_vkAcquireNextImage2KHR                 vkAcquireNextImage2KHR                 = 0;

      //=== VK_KHR_display_swapchain ===
      PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = 0;

      //=== VK_KHR_external_memory_fd ===
      PFN_vkGetMemoryFdKHR           vkGetMemoryFdKHR           = 0;
      PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR = 0;

      //=== VK_KHR_external_semaphore_fd ===
      PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR = 0;
      PFN_vkGetSemaphoreFdKHR    vkGetSemaphoreFdKHR    = 0;

      //=== VK_EXT_display_control ===
      PFN_vkDisplayPowerControlEXT  vkDisplayPowerControlEXT  = 0;
      PFN_vkRegisterDeviceEventEXT  vkRegisterDeviceEventEXT  = 0;
      PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT = 0;
      PFN_vkGetSwapchainCounterEXT  vkGetSwapchainCounterEXT  = 0;

      //=== VK_EXT_discard_rectangles ===
      PFN_vkCmdSetDiscardRectangleEXT       vkCmdSetDiscardRectangleEXT       = 0;
      PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT = 0;
      PFN_vkCmdSetDiscardRectangleModeEXT   vkCmdSetDiscardRectangleModeEXT   = 0;

      //=== VK_EXT_hdr_metadata ===
      PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT = 0;

      //=== VK_KHR_shared_presentable_image ===
      PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR = 0;

      //=== VK_KHR_external_fence_fd ===
      PFN_vkImportFenceFdKHR vkImportFenceFdKHR = 0;
      PFN_vkGetFenceFdKHR    vkGetFenceFdKHR    = 0;

      //=== VK_KHR_performance_query ===
      PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR = 0;
      PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR = 0;

      //=== VK_EXT_debug_utils ===
      PFN_vkSetDebugUtilsObjectNameEXT    vkSetDebugUtilsObjectNameEXT    = 0;
      PFN_vkSetDebugUtilsObjectTagEXT     vkSetDebugUtilsObjectTagEXT     = 0;
      PFN_vkQueueBeginDebugUtilsLabelEXT  vkQueueBeginDebugUtilsLabelEXT  = 0;
      PFN_vkQueueEndDebugUtilsLabelEXT    vkQueueEndDebugUtilsLabelEXT    = 0;
      PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = 0;
      PFN_vkCmdBeginDebugUtilsLabelEXT    vkCmdBeginDebugUtilsLabelEXT    = 0;
      PFN_vkCmdEndDebugUtilsLabelEXT      vkCmdEndDebugUtilsLabelEXT      = 0;
      PFN_vkCmdInsertDebugUtilsLabelEXT   vkCmdInsertDebugUtilsLabelEXT   = 0;

      //=== VK_EXT_sample_locations ===
      PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT = 0;

      //=== VK_EXT_image_drm_format_modifier ===
      PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT = 0;

      //=== VK_EXT_external_memory_host ===
      PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = 0;

      //=== VK_EXT_calibrated_timestamps ===
      PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT = 0;

      //=== VK_KHR_fragment_shading_rate ===
      PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR = 0;

      //=== VK_EXT_line_rasterization ===
      PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT = 0;

      //=== VK_EXT_extended_dynamic_state ===
      PFN_vkCmdSetCullModeEXT              vkCmdSetCullModeEXT              = 0;
      PFN_vkCmdSetFrontFaceEXT             vkCmdSetFrontFaceEXT             = 0;
      PFN_vkCmdSetPrimitiveTopologyEXT     vkCmdSetPrimitiveTopologyEXT     = 0;
      PFN_vkCmdSetViewportWithCountEXT     vkCmdSetViewportWithCountEXT     = 0;
      PFN_vkCmdSetScissorWithCountEXT      vkCmdSetScissorWithCountEXT      = 0;
      PFN_vkCmdBindVertexBuffers2EXT       vkCmdBindVertexBuffers2EXT       = 0;
      PFN_vkCmdSetDepthTestEnableEXT       vkCmdSetDepthTestEnableEXT       = 0;
      PFN_vkCmdSetDepthWriteEnableEXT      vkCmdSetDepthWriteEnableEXT      = 0;
      PFN_vkCmdSetDepthCompareOpEXT        vkCmdSetDepthCompareOpEXT        = 0;
      PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT = 0;
      PFN_vkCmdSetStencilTestEnableEXT     vkCmdSetStencilTestEnableEXT     = 0;
      PFN_vkCmdSetStencilOpEXT             vkCmdSetStencilOpEXT             = 0;

      //=== VK_KHR_object_refresh ===
      PFN_vkCmdRefreshObjectsKHR vkCmdRefreshObjectsKHR = 0;

      //=== VK_KHR_synchronization2 ===
      PFN_vkCmdSetEvent2KHR           vkCmdSetEvent2KHR           = 0;
      PFN_vkCmdResetEvent2KHR         vkCmdResetEvent2KHR         = 0;
      PFN_vkCmdWaitEvents2KHR         vkCmdWaitEvents2KHR         = 0;
      PFN_vkCmdPipelineBarrier2KHR    vkCmdPipelineBarrier2KHR    = 0;
      PFN_vkCmdWriteTimestamp2KHR     vkCmdWriteTimestamp2KHR     = 0;
      PFN_vkQueueSubmit2KHR           vkQueueSubmit2KHR           = 0;
      PFN_vkCmdWriteBufferMarker2AMD  vkCmdWriteBufferMarker2AMD  = 0;
      PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV = 0;

      //=== VK_KHR_copy_commands2 ===
      PFN_vkCmdCopyBuffer2KHR        vkCmdCopyBuffer2KHR        = 0;
      PFN_vkCmdCopyImage2KHR         vkCmdCopyImage2KHR         = 0;
      PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR = 0;
      PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR = 0;
      PFN_vkCmdBlitImage2KHR         vkCmdBlitImage2KHR         = 0;
      PFN_vkCmdResolveImage2KHR      vkCmdResolveImage2KHR      = 0;

      //=== VK_EXT_vertex_input_dynamic_state ===
      PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT = 0;

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===
      PFN_vkGetFenceSciSyncFenceNV      vkGetFenceSciSyncFenceNV      = 0;
      PFN_vkGetFenceSciSyncObjNV        vkGetFenceSciSyncObjNV        = 0;
      PFN_vkImportFenceSciSyncFenceNV   vkImportFenceSciSyncFenceNV   = 0;
      PFN_vkImportFenceSciSyncObjNV     vkImportFenceSciSyncObjNV     = 0;
      PFN_vkGetSemaphoreSciSyncObjNV    vkGetSemaphoreSciSyncObjNV    = 0;
      PFN_vkImportSemaphoreSciSyncObjNV vkImportSemaphoreSciSyncObjNV = 0;
#  else
      PFN_dummy vkGetFenceSciSyncFenceNV_placeholder                            = 0;
      PFN_dummy vkGetFenceSciSyncObjNV_placeholder                              = 0;
      PFN_dummy vkImportFenceSciSyncFenceNV_placeholder                         = 0;
      PFN_dummy vkImportFenceSciSyncObjNV_placeholder                           = 0;
      PFN_dummy vkGetSemaphoreSciSyncObjNV_placeholder                          = 0;
      PFN_dummy vkImportSemaphoreSciSyncObjNV_placeholder                       = 0;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===
      PFN_vkGetMemorySciBufNV vkGetMemorySciBufNV = 0;
#  else
      PFN_dummy vkGetMemorySciBufNV_placeholder                                 = 0;
#  endif /*VK_USE_PLATFORM_SCI*/

      //=== VK_EXT_extended_dynamic_state2 ===
      PFN_vkCmdSetPatchControlPointsEXT      vkCmdSetPatchControlPointsEXT      = 0;
      PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT = 0;
      PFN_vkCmdSetDepthBiasEnableEXT         vkCmdSetDepthBiasEnableEXT         = 0;
      PFN_vkCmdSetLogicOpEXT                 vkCmdSetLogicOpEXT                 = 0;
      PFN_vkCmdSetPrimitiveRestartEnableEXT  vkCmdSetPrimitiveRestartEnableEXT  = 0;

      //=== VK_EXT_color_write_enable ===
      PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT = 0;

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync2 ===
      PFN_vkCreateSemaphoreSciSyncPoolNV vkCreateSemaphoreSciSyncPoolNV = 0;
#  else
      PFN_dummy vkCreateSemaphoreSciSyncPoolNV_placeholder                      = 0;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===
      PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX = 0;
#  else
      PFN_dummy vkGetScreenBufferPropertiesQNX_placeholder                      = 0;
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/
    };

    //========================================
    //=== RAII HANDLE forward declarations ===
    //========================================

    //=== VK_VERSION_1_0 ===
    class Instance;
    class PhysicalDevice;
    class Device;
    class Queue;
    class DeviceMemory;
    class Fence;
    class Semaphore;
    class Event;
    class QueryPool;
    class Buffer;
    class BufferView;
    class Image;
    class ImageView;
    class ShaderModule;
    class PipelineCache;
    class Pipeline;
    class PipelineLayout;
    class Sampler;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class Framebuffer;
    class RenderPass;
    class CommandPool;
    class CommandBuffer;

    //=== VK_VERSION_1_1 ===
    class SamplerYcbcrConversion;

    //=== VK_VERSION_1_3 ===
    class PrivateDataSlot;

    //=== VK_KHR_surface ===
    class SurfaceKHR;

    //=== VK_KHR_swapchain ===
    class SwapchainKHR;

    //=== VK_KHR_display ===
    class DisplayKHR;
    class DisplayModeKHR;

    //=== VK_EXT_debug_utils ===
    class DebugUtilsMessengerEXT;

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync2 ===
    class SemaphoreSciSyncPoolNV;
#  endif /*VK_USE_PLATFORM_SCI*/

    //====================
    //=== RAII HANDLES ===
    //====================

    class Context
    {
    public:
#  if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      Context()
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher(
            m_dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" ) ) )
#  else
      Context( PFN_vkGetInstanceProcAddr getInstanceProcAddr )
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher( getInstanceProcAddr ) )
#  endif
      {
      }

      ~Context() = default;

      Context( Context const & ) = delete;
      Context( Context && rhs ) VULKAN_HPP_NOEXCEPT
#  if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
        : m_dynamicLoader( std::move( rhs.m_dynamicLoader ) )
        , m_dispatcher( rhs.m_dispatcher.release() )
#  else
        : m_dispatcher( rhs.m_dispatcher.release() )
#  endif
      {
      }
      Context & operator=( Context const & ) = delete;

      Context & operator=( Context && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
#  if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
          m_dynamicLoader = std::move( rhs.m_dynamicLoader );
#  endif
          m_dispatcher.reset( rhs.m_dispatcher.release() );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return &*m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context & rhs )
      {
#  if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
        std::swap( m_dynamicLoader, rhs.m_dynamicLoader );
#  endif
        m_dispatcher.swap( rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance>::Type
                           createInstance( VULKAN_HPP_NAMESPACE::InstanceCreateInfo const &                                createInfo,
                                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties>
                           enumerateInstanceExtensionProperties( Optional<const std::string> layerName VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> enumerateInstanceLayerProperties() const;

      //=== VK_VERSION_1_1 ===

      VULKAN_HPP_NODISCARD uint32_t enumerateInstanceVersion() const;

    private:
#  if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      VULKAN_HPP_NAMESPACE::DynamicLoader m_dynamicLoader;
#  endif
      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher> m_dispatcher;
    };

    class Instance
    {
    public:
      using CType   = VkInstance;
      using CppType = vk::Instance;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eInstance;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eInstance;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Instance( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context const &                context,
                VULKAN_HPP_NAMESPACE::InstanceCreateInfo const &                                createInfo,
                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = context.createInstance( createInfo, allocator );
      }
#  endif

      Instance( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context const &                context,
                VkInstance                                                                      instance,
                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_instance( instance ), m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
      {
        m_dispatcher.reset( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher( context.getDispatcher()->vkGetInstanceProcAddr,
                                                                                                     static_cast<VkInstance>( m_instance ) ) );
      }

      Instance( std::nullptr_t ) {}

      ~Instance()
      {
        clear();
      }

      Instance()                   = delete;
      Instance( Instance const & ) = delete;

      Instance( Instance && rhs ) VULKAN_HPP_NOEXCEPT
        : m_instance( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_instance, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( rhs.m_dispatcher.release() )
      {
      }

      Instance & operator=( Instance const & ) = delete;

      Instance & operator=( Instance && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_instance, rhs.m_instance );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Instance const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_instance;
      }

      operator VULKAN_HPP_NAMESPACE::Instance() const VULKAN_HPP_NOEXCEPT
      {
        return m_instance;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_instance )
        {
          getDispatcher()->vkDestroyInstance( static_cast<VkInstance>( m_instance ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_instance   = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Instance release()
      {
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_instance, nullptr );
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return &*m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_instance, rhs.m_instance );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice>>::Type
        enumeratePhysicalDevices() const;

      VULKAN_HPP_NODISCARD PFN_vkVoidFunction getProcAddr( const std::string & name ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_1 ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties> enumeratePhysicalDeviceGroups() const;

      //=== VK_KHR_display ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR>::Type
                           createDisplayPlaneSurfaceKHR( VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR const &                       createInfo,
                                                         VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      //=== VK_EXT_debug_utils ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT>::Type
        createDebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT const &                  createInfo,
                                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      void submitDebugUtilsMessageEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                       VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT              messageTypes,
                                       const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT & callbackData ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_headless_surface ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR>::Type
                           createHeadlessSurfaceEXT( VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT const &                      createInfo,
                                                     VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Instance                                                       m_instance  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                                    m_allocator = {};
      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher> m_dispatcher;
    };

    class PhysicalDevice
    {
    public:
      using CType   = VkPhysicalDevice;
      using CppType = vk::PhysicalDevice;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::ePhysicalDevice;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePhysicalDevice;

    public:
      PhysicalDevice( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const & instance, VkPhysicalDevice physicalDevice )
        : m_physicalDevice( physicalDevice ), m_dispatcher( instance.getDispatcher() )
      {
      }

      PhysicalDevice( std::nullptr_t ) {}

      ~PhysicalDevice()
      {
        clear();
      }

      PhysicalDevice() = delete;

      PhysicalDevice( PhysicalDevice const & rhs ) : m_physicalDevice( rhs.m_physicalDevice ), m_dispatcher( rhs.m_dispatcher ) {}

      PhysicalDevice( PhysicalDevice && rhs ) VULKAN_HPP_NOEXCEPT
        : m_physicalDevice( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_physicalDevice, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      PhysicalDevice & operator=( PhysicalDevice const & rhs )
      {
        m_physicalDevice = rhs.m_physicalDevice;
        m_dispatcher     = rhs.m_dispatcher;
        return *this;
      }

      PhysicalDevice & operator=( PhysicalDevice && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_physicalDevice, rhs.m_physicalDevice );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::PhysicalDevice const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_physicalDevice;
      }

      operator VULKAN_HPP_NAMESPACE::PhysicalDevice() const VULKAN_HPP_NOEXCEPT
      {
        return m_physicalDevice;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        m_physicalDevice = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::PhysicalDevice release()
      {
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_physicalDevice, nullptr );
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_physicalDevice, rhs.m_physicalDevice );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures getFeatures() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::FormatProperties getFormatProperties( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ImageFormatProperties
                           getImageFormatProperties( VULKAN_HPP_NAMESPACE::Format                 format,
                                                     VULKAN_HPP_NAMESPACE::ImageType              type,
                                                     VULKAN_HPP_NAMESPACE::ImageTiling            tiling,
                                                     VULKAN_HPP_NAMESPACE::ImageUsageFlags        usage,
                                                     VULKAN_HPP_NAMESPACE::ImageCreateFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties getProperties() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties> getQueueFamilyProperties() const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties getMemoryProperties() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device>::Type
                           createDevice( VULKAN_HPP_NAMESPACE::DeviceCreateInfo const &                                  createInfo,
                                         VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties>
                           enumerateDeviceExtensionProperties( Optional<const std::string> layerName VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> enumerateDeviceLayerProperties() const;

      //=== VK_VERSION_1_1 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 getFeatures2() const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> getFeatures2() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 getProperties2() const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> getProperties2() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::FormatProperties2 getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ImageFormatProperties2
                           getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo ) const;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> getQueueFamilyProperties2() const;

      template <typename StructureChain>
      VULKAN_HPP_NODISCARD std::vector<StructureChain> getQueueFamilyProperties2() const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 getMemoryProperties2() const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> getMemoryProperties2() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ExternalBufferProperties
        getExternalBufferProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo & externalBufferInfo ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ExternalFenceProperties
        getExternalFenceProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo & externalFenceInfo ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties
        getExternalSemaphoreProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo & externalSemaphoreInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_3 ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties> getToolProperties() const;

      //=== VK_KHR_surface ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Bool32 getSurfaceSupportKHR( uint32_t queueFamilyIndex, VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>
                           getSurfaceFormatsKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR>
                           getSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      //=== VK_KHR_swapchain ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::Rect2D> getPresentRectanglesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const;

      //=== VK_KHR_display ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR> getDisplayPropertiesKHR() const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR> getDisplayPlanePropertiesKHR() const;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR>>::Type
        getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex ) const;

      //=== VK_EXT_display_surface_counter ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT getSurfaceCapabilities2EXT( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const;

      //=== VK_KHR_performance_query ===

      VULKAN_HPP_NODISCARD
        std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>, std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>>
        enumerateQueueFamilyPerformanceQueryCountersKHR( uint32_t queueFamilyIndex ) const;

      VULKAN_HPP_NODISCARD uint32_t getQueueFamilyPerformanceQueryPassesKHR(
        const VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR & performanceQueryCreateInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_get_surface_capabilities2 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR
                           getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>
                           getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const;

      template <typename StructureChain>
      VULKAN_HPP_NODISCARD std::vector<StructureChain> getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const;

      //=== VK_KHR_get_display_properties2 ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR> getDisplayProperties2KHR() const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR> getDisplayPlaneProperties2KHR() const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR
                           getDisplayPlaneCapabilities2KHR( const VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR & displayPlaneInfo ) const;

      //=== VK_EXT_sample_locations ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT
                           getMultisamplePropertiesEXT( VULKAN_HPP_NAMESPACE::SampleCountFlagBits samples ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_calibrated_timestamps ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::TimeDomainKHR> getCalibrateableTimeDomainsEXT() const;

      //=== VK_KHR_fragment_shading_rate ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR> getFragmentShadingRatesKHR() const;

      //=== VK_KHR_object_refresh ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::ObjectType> getRefreshableObjectTypesKHR() const;

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===

      void getSciSyncAttributesNV( const VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV & sciSyncAttributesInfo, NvSciSyncAttrList pAttributes ) const;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV
                           getExternalMemorySciBufPropertiesNV( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle ) const;

      void getSciBufAttributesNV( NvSciBufAttrList pAttributes ) const;
#  endif /*VK_USE_PLATFORM_SCI*/

    private:
      VULKAN_HPP_NAMESPACE::PhysicalDevice                                        m_physicalDevice = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher     = nullptr;
    };

    class PhysicalDevices : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      PhysicalDevices( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const & instance )
      {
        *this = instance.enumeratePhysicalDevices();
      }
#  endif

      PhysicalDevices( std::nullptr_t ) {}

      PhysicalDevices()                                      = delete;
      PhysicalDevices( PhysicalDevices const & )             = delete;
      PhysicalDevices( PhysicalDevices && rhs )              = default;
      PhysicalDevices & operator=( PhysicalDevices const & ) = delete;
      PhysicalDevices & operator=( PhysicalDevices && rhs )  = default;

    private:
      PhysicalDevices( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    class Device
    {
    public:
      using CType   = VkDevice;
      using CppType = vk::Device;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDevice;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDevice;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice const &         physicalDevice,
              VULKAN_HPP_NAMESPACE::DeviceCreateInfo const &                                  createInfo,
              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = physicalDevice.createDevice( createInfo, allocator );
      }
#  endif

      Device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice const &         physicalDevice,
              VkDevice                                                                        device,
              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device ), m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
      {
        m_dispatcher.reset( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher( physicalDevice.getDispatcher()->vkGetDeviceProcAddr,
                                                                                                   static_cast<VkDevice>( m_device ) ) );
      }

      Device( std::nullptr_t ) {}

      ~Device()
      {
        clear();
      }

      Device()                 = delete;
      Device( Device const & ) = delete;

      Device( Device && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( rhs.m_dispatcher.release() )
      {
      }

      Device & operator=( Device const & ) = delete;

      Device & operator=( Device && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Device const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_device;
      }

      operator VULKAN_HPP_NAMESPACE::Device() const VULKAN_HPP_NOEXCEPT
      {
        return m_device;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_device )
        {
          getDispatcher()->vkDestroyDevice( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Device release()
      {
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_device, nullptr );
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return &*m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD PFN_vkVoidFunction getProcAddr( const std::string & name ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue>::Type
                           getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      void waitIdle() const;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceMemory>::Type
        allocateMemory( VULKAN_HPP_NAMESPACE::MemoryAllocateInfo const &                                allocateInfo,
                        VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      void flushMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges ) const;

      void invalidateMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                           createFence( VULKAN_HPP_NAMESPACE::FenceCreateInfo const &                                   createInfo,
                                        VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      void resetFences( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result waitForFences( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences,
                                                                       VULKAN_HPP_NAMESPACE::Bool32                                                waitAll,
                                                                       uint64_t timeout ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Semaphore>::Type
                           createSemaphore( VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo const &                               createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Event>::Type
                           createEvent( VULKAN_HPP_NAMESPACE::EventCreateInfo const &                                   createInfo,
                                        VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::QueryPool>::Type
                           createQueryPool( VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo const &                               createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Buffer>::Type
                           createBuffer( VULKAN_HPP_NAMESPACE::BufferCreateInfo const &                                  createInfo,
                                         VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::BufferView>::Type
                           createBufferView( VULKAN_HPP_NAMESPACE::BufferViewCreateInfo const &                              createInfo,
                                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Image>::Type
                           createImage( VULKAN_HPP_NAMESPACE::ImageCreateInfo const &                                   createInfo,
                                        VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ImageView>::Type
                           createImageView( VULKAN_HPP_NAMESPACE::ImageViewCreateInfo const &                               createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache>::Type
        createPipelineCache( VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo const &                           createInfo,
                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineLayout>::Type
        createPipelineLayout( VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo const &                          createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Sampler>::Type
                           createSampler( VULKAN_HPP_NAMESPACE::SamplerCreateInfo const &                                 createInfo,
                                          VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSetLayout>::Type
        createDescriptorSetLayout( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo const &                     createInfo,
                                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorPool>::Type
        createDescriptorPool( VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo const &                          createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet>>::Type
        allocateDescriptorSets( VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo const & allocateInfo ) const;

      void updateDescriptorSets( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::WriteDescriptorSet> const & descriptorWrites,
                                 VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CopyDescriptorSet> const &  descriptorCopies ) const
        VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Framebuffer>::Type
                           createFramebuffer( VULKAN_HPP_NAMESPACE::FramebufferCreateInfo const &                             createInfo,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass>::Type
                           createRenderPass( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo const &                              createInfo,
                                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandPool>::Type
                           createCommandPool( VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo const &                             createInfo,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer>>::Type
        allocateCommandBuffers( VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo const & allocateInfo ) const;

      //=== VK_VERSION_1_1 ===

      void bindBufferMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo> const & bindInfos ) const;

      void bindImageMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindImageMemoryInfo> const & bindInfos ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags
                           getGroupPeerMemoryFeatures( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements2
                           getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements2
                           getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue>::Type
                           getQueue2( VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 const & queueInfo ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SamplerYcbcrConversion>::Type
        createSamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo const &                  createInfo,
                                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport
                           getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_2 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass>::Type
                           createRenderPass2( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 const &                             createInfo,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result waitSemaphores( const VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo & waitInfo, uint64_t timeout ) const;

      void signalSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo & signalInfo ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DeviceAddress
                           getBufferAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD uint64_t getBufferOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD uint64_t
        getMemoryOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo & info ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_3 ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PrivateDataSlot>::Type
        createPrivateDataSlot( VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo const &                         createInfo,
                               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      void setPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                           uint64_t                              objectHandle,
                           VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                           uint64_t                              data ) const;

      VULKAN_HPP_NODISCARD uint64_t getPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                    uint64_t                              objectHandle,
                                                    VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements2
                           getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements2
                           getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                           getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2>
                           getImageSparseMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const;

      //=== VKSC_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData>>
                           getFaultData( VULKAN_HPP_NAMESPACE::FaultQueryBehavior faultQueryBehavior ) const;

      //=== VK_KHR_swapchain ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>::Type
        createSwapchainKHR( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const &                            createInfo,
                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR getGroupPresentCapabilitiesKHR() const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR
                           getGroupSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const;

      VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, uint32_t>
                           acquireNextImage2KHR( const VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR & acquireInfo ) const;

      //=== VK_KHR_display_swapchain ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>>::Type
        createSharedSwapchainsKHR( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>        allocator = nullptr ) const;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>::Type
        createSharedSwapchainKHR( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const &                            createInfo,
                                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      //=== VK_KHR_external_memory_fd ===

      VULKAN_HPP_NODISCARD int getMemoryFdKHR( const VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR & getFdInfo ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR
                           getMemoryFdPropertiesKHR( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, int fd ) const;

      //=== VK_KHR_external_semaphore_fd ===

      void importSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR & importSemaphoreFdInfo ) const;

      VULKAN_HPP_NODISCARD int getSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR & getFdInfo ) const;

      //=== VK_EXT_display_control ===

      void displayPowerControlEXT( VULKAN_HPP_NAMESPACE::DisplayKHR display, const VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT & displayPowerInfo ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                           registerEventEXT( VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT const &                                deviceEventInfo,
                                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                           registerDisplayEventEXT( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR const &             display,
                                                    VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT const &                               displayEventInfo,
                                                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      //=== VK_EXT_hdr_metadata ===

      void setHdrMetadataEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainKHR> const &   swapchains,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::HdrMetadataEXT> const & metadata ) const;

      //=== VK_KHR_external_fence_fd ===

      void importFenceFdKHR( const VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR & importFenceFdInfo ) const;

      VULKAN_HPP_NODISCARD int getFenceFdKHR( const VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR & getFdInfo ) const;

      //=== VK_KHR_performance_query ===

      void acquireProfilingLockKHR( const VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR & info ) const;

      void releaseProfilingLockKHR() const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_debug_utils ===

      void setDebugUtilsObjectNameEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT & nameInfo ) const;

      void setDebugUtilsObjectTagEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT & tagInfo ) const;

      //=== VK_EXT_external_memory_host ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT
        getMemoryHostPointerPropertiesEXT( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, const void * pHostPointer ) const;

      //=== VK_EXT_calibrated_timestamps ===

      VULKAN_HPP_NODISCARD std::pair<std::vector<uint64_t>, uint64_t>
        getCalibratedTimestampsEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoKHR> const & timestampInfos ) const;

      VULKAN_HPP_NODISCARD std::pair<uint64_t, uint64_t>
                           getCalibratedTimestampEXT( const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoKHR & timestampInfo ) const;

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===

      template <typename HandleType>
      VULKAN_HPP_NODISCARD HandleType getFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo ) const;

      template <typename HandleType>
      VULKAN_HPP_NODISCARD HandleType getFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo ) const;

      void importFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo ) const;

      void importFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo ) const;

      template <typename HandleType>
      VULKAN_HPP_NODISCARD HandleType getSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV & getSciSyncInfo ) const;

      void importSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV & importSemaphoreSciSyncInfo ) const;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===

      VULKAN_HPP_NODISCARD NvSciBufObj getMemorySciBufNV( const VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV & getSciBufInfo ) const;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync2 ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SemaphoreSciSyncPoolNV>::Type
        createSemaphoreSciSyncPoolNV( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV const &                  createInfo,
                                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer ) const;

      template <typename X, typename Y, typename... Z>
      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer ) const;
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

    private:
      VULKAN_HPP_NAMESPACE::Device                                                       m_device    = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                                  m_allocator = {};
      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher> m_dispatcher;
    };

    class Buffer
    {
    public:
      using CType   = VkBuffer;
      using CppType = vk::Buffer;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eBuffer;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eBuffer;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Buffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
              VULKAN_HPP_NAMESPACE::BufferCreateInfo const &                                  createInfo,
              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createBuffer( createInfo, allocator );
      }
#  endif

      Buffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
              VkBuffer                                                                        buffer,
              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_buffer( buffer )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Buffer( std::nullptr_t ) {}

      ~Buffer()
      {
        clear();
      }

      Buffer()                 = delete;
      Buffer( Buffer const & ) = delete;

      Buffer( Buffer && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_buffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_buffer, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Buffer & operator=( Buffer const & ) = delete;

      Buffer & operator=( Buffer && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_buffer, rhs.m_buffer );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Buffer const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_buffer;
      }

      operator VULKAN_HPP_NAMESPACE::Buffer() const VULKAN_HPP_NOEXCEPT
      {
        return m_buffer;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_buffer )
        {
          getDispatcher()->vkDestroyBuffer(
            static_cast<VkDevice>( m_device ), static_cast<VkBuffer>( m_buffer ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_buffer     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Buffer release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_buffer, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Buffer & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_buffer, rhs.m_buffer );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void bindMemory( VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements getMemoryRequirements() const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Buffer                                              m_buffer     = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class BufferView
    {
    public:
      using CType   = VkBufferView;
      using CppType = vk::BufferView;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eBufferView;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eBufferView;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      BufferView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                  VULKAN_HPP_NAMESPACE::BufferViewCreateInfo const &                              createInfo,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createBufferView( createInfo, allocator );
      }
#  endif

      BufferView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                  VkBufferView                                                                    bufferView,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_bufferView( bufferView )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      BufferView( std::nullptr_t ) {}

      ~BufferView()
      {
        clear();
      }

      BufferView()                     = delete;
      BufferView( BufferView const & ) = delete;

      BufferView( BufferView && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_bufferView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_bufferView, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      BufferView & operator=( BufferView const & ) = delete;

      BufferView & operator=( BufferView && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_bufferView, rhs.m_bufferView );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::BufferView const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_bufferView;
      }

      operator VULKAN_HPP_NAMESPACE::BufferView() const VULKAN_HPP_NOEXCEPT
      {
        return m_bufferView;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_bufferView )
        {
          getDispatcher()->vkDestroyBufferView(
            static_cast<VkDevice>( m_device ), static_cast<VkBufferView>( m_bufferView ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_bufferView = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::BufferView release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_bufferView, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::BufferView & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_bufferView, rhs.m_bufferView );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::BufferView                                          m_bufferView = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class CommandPool
    {
    public:
      using CType   = VkCommandPool;
      using CppType = vk::CommandPool;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eCommandPool;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eCommandPool;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      CommandPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                   VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo const &                             createInfo,
                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createCommandPool( createInfo, allocator );
      }
#  endif

      CommandPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                   VkCommandPool                                                                   commandPool,
                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_commandPool( commandPool )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      CommandPool( std::nullptr_t ) {}

      ~CommandPool()
      {
        clear();
      }

      CommandPool()                      = delete;
      CommandPool( CommandPool const & ) = delete;

      CommandPool( CommandPool && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_commandPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_commandPool, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      CommandPool & operator=( CommandPool const & ) = delete;

      CommandPool & operator=( CommandPool && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_commandPool, rhs.m_commandPool );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::CommandPool const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_commandPool;
      }

      operator VULKAN_HPP_NAMESPACE::CommandPool() const VULKAN_HPP_NOEXCEPT
      {
        return m_commandPool;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_commandPool )
        {
          getDispatcher()->vkDestroyCommandPool(
            static_cast<VkDevice>( m_device ), static_cast<VkCommandPool>( m_commandPool ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device      = nullptr;
        m_commandPool = nullptr;
        m_allocator   = nullptr;
        m_dispatcher  = nullptr;
      }

      VULKAN_HPP_NAMESPACE::CommandPool release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_commandPool, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandPool & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_commandPool, rhs.m_commandPool );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void reset( VULKAN_HPP_NAMESPACE::CommandPoolResetFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      //=== VKSC_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption
        getMemoryConsumption( VULKAN_HPP_NAMESPACE::CommandBuffer commandBuffer VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device      = {};
      VULKAN_HPP_NAMESPACE::CommandPool                                         m_commandPool = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator   = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher  = nullptr;
    };

    class CommandBuffer
    {
    public:
      using CType   = VkCommandBuffer;
      using CppType = vk::CommandBuffer;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eCommandBuffer;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eCommandBuffer;

    public:
      CommandBuffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device, VkCommandBuffer commandBuffer, VkCommandPool commandPool )
        : m_device( device ), m_commandPool( commandPool ), m_commandBuffer( commandBuffer ), m_dispatcher( device.getDispatcher() )
      {
      }

      CommandBuffer( std::nullptr_t ) {}

      ~CommandBuffer()
      {
        clear();
      }

      CommandBuffer()                        = delete;
      CommandBuffer( CommandBuffer const & ) = delete;

      CommandBuffer( CommandBuffer && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_commandPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_commandPool, {} ) )
        , m_commandBuffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_commandBuffer, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      CommandBuffer & operator=( CommandBuffer const & ) = delete;

      CommandBuffer & operator=( CommandBuffer && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_commandPool, rhs.m_commandPool );
          std::swap( m_commandBuffer, rhs.m_commandBuffer );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::CommandBuffer const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_commandBuffer;
      }

      operator VULKAN_HPP_NAMESPACE::CommandBuffer() const VULKAN_HPP_NOEXCEPT
      {
        return m_commandBuffer;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_commandBuffer )
        {
          getDispatcher()->vkFreeCommandBuffers(
            static_cast<VkDevice>( m_device ), static_cast<VkCommandPool>( m_commandPool ), 1, reinterpret_cast<VkCommandBuffer const *>( &m_commandBuffer ) );
        }
        m_device        = nullptr;
        m_commandPool   = nullptr;
        m_commandBuffer = nullptr;
        m_dispatcher    = nullptr;
      }

      VULKAN_HPP_NAMESPACE::CommandBuffer release()
      {
        m_device      = nullptr;
        m_commandPool = nullptr;
        m_dispatcher  = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_commandBuffer, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_commandPool, rhs.m_commandPool );
        std::swap( m_commandBuffer, rhs.m_commandBuffer );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void begin( const VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo & beginInfo ) const;

      void end() const;

      void reset( VULKAN_HPP_NAMESPACE::CommandBufferResetFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      void bindPipeline( VULKAN_HPP_NAMESPACE::PipelineBindPoint pipelineBindPoint, VULKAN_HPP_NAMESPACE::Pipeline pipeline ) const VULKAN_HPP_NOEXCEPT;

      void setViewport( uint32_t                                                                       firstViewport,
                        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT;

      void setScissor( uint32_t firstScissor, VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT;

      void setLineWidth( float lineWidth ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) const VULKAN_HPP_NOEXCEPT;

      void setBlendConstants( const float blendConstants[4] ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBounds( float minDepthBounds, float maxDepthBounds ) const VULKAN_HPP_NOEXCEPT;

      void setStencilCompareMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t compareMask ) const VULKAN_HPP_NOEXCEPT;

      void setStencilWriteMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t writeMask ) const VULKAN_HPP_NOEXCEPT;

      void setStencilReference( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t reference ) const VULKAN_HPP_NOEXCEPT;

      void bindDescriptorSets( VULKAN_HPP_NAMESPACE::PipelineBindPoint                                             pipelineBindPoint,
                               VULKAN_HPP_NAMESPACE::PipelineLayout                                                layout,
                               uint32_t                                                                            firstSet,
                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DescriptorSet> const & descriptorSets,
                               VULKAN_HPP_NAMESPACE::ArrayProxy<const uint32_t> const &                            dynamicOffsets ) const VULKAN_HPP_NOEXCEPT;

      void bindIndexBuffer( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                            VULKAN_HPP_NAMESPACE::DeviceSize offset,
                            VULKAN_HPP_NAMESPACE::IndexType  indexType ) const VULKAN_HPP_NOEXCEPT;

      void bindVertexBuffers( uint32_t                                                                         firstBinding,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets ) const;

      void draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const VULKAN_HPP_NOEXCEPT;

      void
        drawIndexed( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) const VULKAN_HPP_NOEXCEPT;

      void drawIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                         VULKAN_HPP_NAMESPACE::DeviceSize offset,
                         uint32_t                         drawCount,
                         uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT;

      void drawIndexedIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                uint32_t                         drawCount,
                                uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT;

      void dispatch( uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT;

      void dispatchIndirect( VULKAN_HPP_NAMESPACE::Buffer buffer, VULKAN_HPP_NAMESPACE::DeviceSize offset ) const VULKAN_HPP_NOEXCEPT;

      void copyBuffer( VULKAN_HPP_NAMESPACE::Buffer                                                     srcBuffer,
                       VULKAN_HPP_NAMESPACE::Buffer                                                     dstBuffer,
                       VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferCopy> const & regions ) const VULKAN_HPP_NOEXCEPT;

      void copyImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                      VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                      VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                      VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT;

      void blitImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                      VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                      VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                      VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageBlit> const & regions,
                      VULKAN_HPP_NAMESPACE::Filter                                                    filter ) const VULKAN_HPP_NOEXCEPT;

      void copyBufferToImage( VULKAN_HPP_NAMESPACE::Buffer                                                          srcBuffer,
                              VULKAN_HPP_NAMESPACE::Image                                                           dstImage,
                              VULKAN_HPP_NAMESPACE::ImageLayout                                                     dstImageLayout,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT;

      void copyImageToBuffer( VULKAN_HPP_NAMESPACE::Image                                                           srcImage,
                              VULKAN_HPP_NAMESPACE::ImageLayout                                                     srcImageLayout,
                              VULKAN_HPP_NAMESPACE::Buffer                                                          dstBuffer,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT;

      template <typename DataType>
      void updateBuffer( VULKAN_HPP_NAMESPACE::Buffer                             dstBuffer,
                         VULKAN_HPP_NAMESPACE::DeviceSize                         dstOffset,
                         VULKAN_HPP_NAMESPACE::ArrayProxy<const DataType> const & data ) const VULKAN_HPP_NOEXCEPT;

      void fillBuffer( VULKAN_HPP_NAMESPACE::Buffer     dstBuffer,
                       VULKAN_HPP_NAMESPACE::DeviceSize dstOffset,
                       VULKAN_HPP_NAMESPACE::DeviceSize size,
                       uint32_t                         data ) const VULKAN_HPP_NOEXCEPT;

      void clearColorImage( VULKAN_HPP_NAMESPACE::Image                                                                 image,
                            VULKAN_HPP_NAMESPACE::ImageLayout                                                           imageLayout,
                            const VULKAN_HPP_NAMESPACE::ClearColorValue &                                               color,
                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges ) const VULKAN_HPP_NOEXCEPT;

      void
        clearDepthStencilImage( VULKAN_HPP_NAMESPACE::Image                                                                 image,
                                VULKAN_HPP_NAMESPACE::ImageLayout                                                           imageLayout,
                                const VULKAN_HPP_NAMESPACE::ClearDepthStencilValue &                                        depthStencil,
                                VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges ) const VULKAN_HPP_NOEXCEPT;

      void clearAttachments( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearAttachment> const & attachments,
                             VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearRect> const &       rects ) const VULKAN_HPP_NOEXCEPT;

      void resolveImage( VULKAN_HPP_NAMESPACE::Image                                                        srcImage,
                         VULKAN_HPP_NAMESPACE::ImageLayout                                                  srcImageLayout,
                         VULKAN_HPP_NAMESPACE::Image                                                        dstImage,
                         VULKAN_HPP_NAMESPACE::ImageLayout                                                  dstImageLayout,
                         VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageResolve> const & regions ) const VULKAN_HPP_NOEXCEPT;

      void setEvent( VULKAN_HPP_NAMESPACE::Event                        event,
                     VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void resetEvent( VULKAN_HPP_NAMESPACE::Event                        event,
                       VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void waitEvents( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &               events,
                       VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
                       VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
                       VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
                       VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
                       VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const & imageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT;

      void pipelineBarrier( VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
                            VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
                            VULKAN_HPP_NAMESPACE::DependencyFlags                                                     dependencyFlags,
                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const &  imageMemoryBarriers ) const
        VULKAN_HPP_NOEXCEPT;

      void beginQuery( VULKAN_HPP_NAMESPACE::QueryPool               queryPool,
                       uint32_t                                      query,
                       VULKAN_HPP_NAMESPACE::QueryControlFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void endQuery( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT;

      void resetQueryPool( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT;

      void writeTimestamp( VULKAN_HPP_NAMESPACE::PipelineStageFlagBits pipelineStage,
                           VULKAN_HPP_NAMESPACE::QueryPool             queryPool,
                           uint32_t                                    query ) const VULKAN_HPP_NOEXCEPT;

      void copyQueryPoolResults( VULKAN_HPP_NAMESPACE::QueryPool              queryPool,
                                 uint32_t                                     firstQuery,
                                 uint32_t                                     queryCount,
                                 VULKAN_HPP_NAMESPACE::Buffer                 dstBuffer,
                                 VULKAN_HPP_NAMESPACE::DeviceSize             dstOffset,
                                 VULKAN_HPP_NAMESPACE::DeviceSize             stride,
                                 VULKAN_HPP_NAMESPACE::QueryResultFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      template <typename ValuesType>
      void pushConstants( VULKAN_HPP_NAMESPACE::PipelineLayout                       layout,
                          VULKAN_HPP_NAMESPACE::ShaderStageFlags                     stageFlags,
                          uint32_t                                                   offset,
                          VULKAN_HPP_NAMESPACE::ArrayProxy<const ValuesType> const & values ) const VULKAN_HPP_NOEXCEPT;

      void beginRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                            VULKAN_HPP_NAMESPACE::SubpassContents             contents ) const VULKAN_HPP_NOEXCEPT;

      void nextSubpass( VULKAN_HPP_NAMESPACE::SubpassContents contents ) const VULKAN_HPP_NOEXCEPT;

      void endRenderPass() const VULKAN_HPP_NOEXCEPT;

      void executeCommands( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CommandBuffer> const & commandBuffers ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_1 ===

      void setDeviceMask( uint32_t deviceMask ) const VULKAN_HPP_NOEXCEPT;

      void dispatchBase( uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const
        VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_2 ===

      void drawIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                              VULKAN_HPP_NAMESPACE::DeviceSize offset,
                              VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                              VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                              uint32_t                         maxDrawCount,
                              uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT;

      void drawIndexedIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                     VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                     VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                                     VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                                     uint32_t                         maxDrawCount,
                                     uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT;

      void beginRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                             const VULKAN_HPP_NAMESPACE::SubpassBeginInfo &    subpassBeginInfo ) const VULKAN_HPP_NOEXCEPT;

      void nextSubpass2( const VULKAN_HPP_NAMESPACE::SubpassBeginInfo & subpassBeginInfo,
                         const VULKAN_HPP_NAMESPACE::SubpassEndInfo &   subpassEndInfo ) const VULKAN_HPP_NOEXCEPT;

      void endRenderPass2( const VULKAN_HPP_NAMESPACE::SubpassEndInfo & subpassEndInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_VERSION_1_3 ===

      void setEvent2( VULKAN_HPP_NAMESPACE::Event event, const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT;

      void resetEvent2( VULKAN_HPP_NAMESPACE::Event                         event,
                        VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void waitEvents2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos ) const;

      void pipelineBarrier2( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT;

      void
        writeTimestamp2( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage, VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT;

      void copyBuffer2( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyImage2( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyBufferToImage2( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyImageToBuffer2( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT;

      void blitImage2( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void resolveImage2( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void beginRendering( const VULKAN_HPP_NAMESPACE::RenderingInfo & renderingInfo ) const VULKAN_HPP_NOEXCEPT;

      void endRendering() const VULKAN_HPP_NOEXCEPT;

      void setCullMode( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void setFrontFace( VULKAN_HPP_NAMESPACE::FrontFace frontFace ) const VULKAN_HPP_NOEXCEPT;

      void setPrimitiveTopology( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT;

      void setViewportWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT;

      void setScissorWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT;

      void bindVertexBuffers2(
        uint32_t                                                                                 firstBinding,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &             buffers,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const &         offsets,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes   VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT ) const;

      void setDepthTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthWriteEnable( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthCompareOp( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBoundsTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setStencilTestEnable( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setStencilOp( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                         VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                         VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                         VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                         VULKAN_HPP_NAMESPACE::CompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT;

      void setRasterizerDiscardEnable( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBiasEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT;

      void setPrimitiveRestartEnable( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_discard_rectangles ===

      void setDiscardRectangleEXT( uint32_t                                                                     firstDiscardRectangle,
                                   VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & discardRectangles ) const VULKAN_HPP_NOEXCEPT;

      void setDiscardRectangleEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 discardRectangleEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDiscardRectangleModeEXT( VULKAN_HPP_NAMESPACE::DiscardRectangleModeEXT discardRectangleMode ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_debug_utils ===

      void beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT;

      void endDebugUtilsLabelEXT() const VULKAN_HPP_NOEXCEPT;

      void insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_sample_locations ===

      void setSampleLocationsEXT( const VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT & sampleLocationsInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_fragment_shading_rate ===

      void setFragmentShadingRateKHR( const VULKAN_HPP_NAMESPACE::Extent2D &                       fragmentSize,
                                      const VULKAN_HPP_NAMESPACE::FragmentShadingRateCombinerOpKHR combinerOps[2] ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_line_rasterization ===

      void setLineStippleEXT( uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_extended_dynamic_state ===

      void setCullModeEXT( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void setFrontFaceEXT( VULKAN_HPP_NAMESPACE::FrontFace frontFace ) const VULKAN_HPP_NOEXCEPT;

      void setPrimitiveTopologyEXT( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT;

      void setViewportWithCountEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT;

      void setScissorWithCountEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT;

      void bindVertexBuffers2EXT(
        uint32_t                                                                                 firstBinding,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &             buffers,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const &         offsets,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes   VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT ) const;

      void setDepthTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthWriteEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthCompareOpEXT( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBoundsTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setStencilTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT;

      void setStencilOpEXT( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                            VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                            VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                            VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                            VULKAN_HPP_NAMESPACE::CompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_object_refresh ===

      void refreshObjectsKHR( const VULKAN_HPP_NAMESPACE::RefreshObjectListKHR & refreshObjects ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_synchronization2 ===

      void setEvent2KHR( VULKAN_HPP_NAMESPACE::Event event, const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT;

      void resetEvent2KHR( VULKAN_HPP_NAMESPACE::Event                         event,
                           VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

      void waitEvents2KHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos ) const;

      void pipelineBarrier2KHR( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT;

      void writeTimestamp2KHR( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                               VULKAN_HPP_NAMESPACE::QueryPool           queryPool,
                               uint32_t                                  query ) const VULKAN_HPP_NOEXCEPT;

      void writeBufferMarker2AMD( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                  VULKAN_HPP_NAMESPACE::Buffer              dstBuffer,
                                  VULKAN_HPP_NAMESPACE::DeviceSize          dstOffset,
                                  uint32_t                                  marker ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_copy_commands2 ===

      void copyBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyImage2KHR( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyBufferToImage2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void copyImageToBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT;

      void blitImage2KHR( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo ) const VULKAN_HPP_NOEXCEPT;

      void resolveImage2KHR( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_vertex_input_dynamic_state ===

      void setVertexInputEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT> const & vertexBindingDescriptions,
                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT> const &
                                vertexAttributeDescriptions ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_extended_dynamic_state2 ===

      void setPatchControlPointsEXT( uint32_t patchControlPoints ) const VULKAN_HPP_NOEXCEPT;

      void setRasterizerDiscardEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT;

      void setDepthBiasEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT;

      void setLogicOpEXT( VULKAN_HPP_NAMESPACE::LogicOp logicOp ) const VULKAN_HPP_NOEXCEPT;

      void setPrimitiveRestartEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_color_write_enable ===

      void setColorWriteEnableEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Bool32> const & colorWriteEnables ) const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device        = {};
      VULKAN_HPP_NAMESPACE::CommandPool                                         m_commandPool   = {};
      VULKAN_HPP_NAMESPACE::CommandBuffer                                       m_commandBuffer = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher    = nullptr;
    };

    class CommandBuffers : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      CommandBuffers( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device,
                      VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo const &         allocateInfo )
      {
        *this = device.allocateCommandBuffers( allocateInfo );
      }
#  endif

      CommandBuffers( std::nullptr_t ) {}

      CommandBuffers()                                     = delete;
      CommandBuffers( CommandBuffers const & )             = delete;
      CommandBuffers( CommandBuffers && rhs )              = default;
      CommandBuffers & operator=( CommandBuffers const & ) = delete;
      CommandBuffers & operator=( CommandBuffers && rhs )  = default;

    private:
      CommandBuffers( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    class DebugUtilsMessengerEXT
    {
    public:
      using CType   = VkDebugUtilsMessengerEXT;
      using CppType = vk::DebugUtilsMessengerEXT;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDebugUtilsMessengerEXT;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const &               instance,
                              VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT const &                  createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = instance.createDebugUtilsMessengerEXT( createInfo, allocator );
      }
#  endif

      DebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const &               instance,
                              VkDebugUtilsMessengerEXT                                                        messenger,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_instance( instance )
        , m_messenger( messenger )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( instance.getDispatcher() )
      {
      }

      DebugUtilsMessengerEXT( std::nullptr_t ) {}

      ~DebugUtilsMessengerEXT()
      {
        clear();
      }

      DebugUtilsMessengerEXT()                                 = delete;
      DebugUtilsMessengerEXT( DebugUtilsMessengerEXT const & ) = delete;

      DebugUtilsMessengerEXT( DebugUtilsMessengerEXT && rhs ) VULKAN_HPP_NOEXCEPT
        : m_instance( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_instance, {} ) )
        , m_messenger( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_messenger, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DebugUtilsMessengerEXT & operator=( DebugUtilsMessengerEXT const & ) = delete;

      DebugUtilsMessengerEXT & operator=( DebugUtilsMessengerEXT && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_instance, rhs.m_instance );
          std::swap( m_messenger, rhs.m_messenger );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_messenger;
      }

      operator VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT() const VULKAN_HPP_NOEXCEPT
      {
        return m_messenger;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_messenger )
        {
          getDispatcher()->vkDestroyDebugUtilsMessengerEXT( static_cast<VkInstance>( m_instance ),
                                                            static_cast<VkDebugUtilsMessengerEXT>( m_messenger ),
                                                            reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_instance   = nullptr;
        m_messenger  = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT release()
      {
        m_instance   = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_messenger, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Instance getInstance() const
      {
        return m_instance;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_instance, rhs.m_instance );
        std::swap( m_messenger, rhs.m_messenger );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Instance                                              m_instance   = {};
      VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT                                m_messenger  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                           m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher = nullptr;
    };

    class DescriptorPool
    {
    public:
      using CType   = VkDescriptorPool;
      using CppType = vk::DescriptorPool;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorPool;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorPool;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DescriptorPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                      VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo const &                          createInfo,
                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createDescriptorPool( createInfo, allocator );
      }
#  endif

      DescriptorPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                      VkDescriptorPool                                                                descriptorPool,
                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_descriptorPool( descriptorPool )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      DescriptorPool( std::nullptr_t ) {}

      ~DescriptorPool()
      {
        clear();
      }

      DescriptorPool()                         = delete;
      DescriptorPool( DescriptorPool const & ) = delete;

      DescriptorPool( DescriptorPool && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_descriptorPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_descriptorPool, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DescriptorPool & operator=( DescriptorPool const & ) = delete;

      DescriptorPool & operator=( DescriptorPool && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_descriptorPool, rhs.m_descriptorPool );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DescriptorPool const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorPool;
      }

      operator VULKAN_HPP_NAMESPACE::DescriptorPool() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorPool;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_descriptorPool )
        {
          getDispatcher()->vkDestroyDescriptorPool( static_cast<VkDevice>( m_device ),
                                                    static_cast<VkDescriptorPool>( m_descriptorPool ),
                                                    reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device         = nullptr;
        m_descriptorPool = nullptr;
        m_allocator      = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DescriptorPool release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_descriptorPool, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorPool & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_descriptorPool, rhs.m_descriptorPool );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void reset( VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device         = {};
      VULKAN_HPP_NAMESPACE::DescriptorPool                                      m_descriptorPool = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator      = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher     = nullptr;
    };

    class DescriptorSet
    {
    public:
      using CType   = VkDescriptorSet;
      using CppType = vk::DescriptorSet;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorSet;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorSet;

    public:
      DescriptorSet( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device, VkDescriptorSet descriptorSet, VkDescriptorPool descriptorPool )
        : m_device( device ), m_descriptorPool( descriptorPool ), m_descriptorSet( descriptorSet ), m_dispatcher( device.getDispatcher() )
      {
      }

      DescriptorSet( std::nullptr_t ) {}

      ~DescriptorSet()
      {
        clear();
      }

      DescriptorSet()                        = delete;
      DescriptorSet( DescriptorSet const & ) = delete;

      DescriptorSet( DescriptorSet && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_descriptorPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_descriptorPool, {} ) )
        , m_descriptorSet( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_descriptorSet, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DescriptorSet & operator=( DescriptorSet const & ) = delete;

      DescriptorSet & operator=( DescriptorSet && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_descriptorPool, rhs.m_descriptorPool );
          std::swap( m_descriptorSet, rhs.m_descriptorSet );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DescriptorSet const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorSet;
      }

      operator VULKAN_HPP_NAMESPACE::DescriptorSet() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorSet;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_descriptorSet )
        {
          getDispatcher()->vkFreeDescriptorSets( static_cast<VkDevice>( m_device ),
                                                 static_cast<VkDescriptorPool>( m_descriptorPool ),
                                                 1,
                                                 reinterpret_cast<VkDescriptorSet const *>( &m_descriptorSet ) );
        }
        m_device         = nullptr;
        m_descriptorPool = nullptr;
        m_descriptorSet  = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DescriptorSet release()
      {
        m_device         = nullptr;
        m_descriptorPool = nullptr;
        m_dispatcher     = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_descriptorSet, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_descriptorPool, rhs.m_descriptorPool );
        std::swap( m_descriptorSet, rhs.m_descriptorSet );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device         = {};
      VULKAN_HPP_NAMESPACE::DescriptorPool                                      m_descriptorPool = {};
      VULKAN_HPP_NAMESPACE::DescriptorSet                                       m_descriptorSet  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher     = nullptr;
    };

    class DescriptorSets : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DescriptorSets( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device,
                      VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo const &         allocateInfo )
      {
        *this = device.allocateDescriptorSets( allocateInfo );
      }
#  endif

      DescriptorSets( std::nullptr_t ) {}

      DescriptorSets()                                     = delete;
      DescriptorSets( DescriptorSets const & )             = delete;
      DescriptorSets( DescriptorSets && rhs )              = default;
      DescriptorSets & operator=( DescriptorSets const & ) = delete;
      DescriptorSets & operator=( DescriptorSets && rhs )  = default;

    private:
      DescriptorSets( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    class DescriptorSetLayout
    {
    public:
      using CType   = VkDescriptorSetLayout;
      using CppType = vk::DescriptorSetLayout;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDescriptorSetLayout;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDescriptorSetLayout;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DescriptorSetLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                           VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo const &                     createInfo,
                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createDescriptorSetLayout( createInfo, allocator );
      }
#  endif

      DescriptorSetLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                           VkDescriptorSetLayout                                                           descriptorSetLayout,
                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_descriptorSetLayout( descriptorSetLayout )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      DescriptorSetLayout( std::nullptr_t ) {}

      ~DescriptorSetLayout()
      {
        clear();
      }

      DescriptorSetLayout()                              = delete;
      DescriptorSetLayout( DescriptorSetLayout const & ) = delete;

      DescriptorSetLayout( DescriptorSetLayout && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_descriptorSetLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_descriptorSetLayout, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DescriptorSetLayout & operator=( DescriptorSetLayout const & ) = delete;

      DescriptorSetLayout & operator=( DescriptorSetLayout && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_descriptorSetLayout, rhs.m_descriptorSetLayout );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DescriptorSetLayout const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorSetLayout;
      }

      operator VULKAN_HPP_NAMESPACE::DescriptorSetLayout() const VULKAN_HPP_NOEXCEPT
      {
        return m_descriptorSetLayout;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_descriptorSetLayout )
        {
          getDispatcher()->vkDestroyDescriptorSetLayout( static_cast<VkDevice>( m_device ),
                                                         static_cast<VkDescriptorSetLayout>( m_descriptorSetLayout ),
                                                         reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device              = nullptr;
        m_descriptorSetLayout = nullptr;
        m_allocator           = nullptr;
        m_dispatcher          = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DescriptorSetLayout release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_descriptorSetLayout, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSetLayout & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_descriptorSetLayout, rhs.m_descriptorSetLayout );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device              = {};
      VULKAN_HPP_NAMESPACE::DescriptorSetLayout                                 m_descriptorSetLayout = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator           = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher          = nullptr;
    };

    class DeviceMemory
    {
    public:
      using CType   = VkDeviceMemory;
      using CppType = vk::DeviceMemory;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDeviceMemory;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDeviceMemory;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DeviceMemory( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VULKAN_HPP_NAMESPACE::MemoryAllocateInfo const &                                allocateInfo,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.allocateMemory( allocateInfo, allocator );
      }
#  endif

      DeviceMemory( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VkDeviceMemory                                                                  memory,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_memory( memory )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      DeviceMemory( std::nullptr_t ) {}

      ~DeviceMemory()
      {
        clear();
      }

      DeviceMemory()                       = delete;
      DeviceMemory( DeviceMemory const & ) = delete;

      DeviceMemory( DeviceMemory && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_memory( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_memory, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DeviceMemory & operator=( DeviceMemory const & ) = delete;

      DeviceMemory & operator=( DeviceMemory && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_memory, rhs.m_memory );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DeviceMemory const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_memory;
      }

      operator VULKAN_HPP_NAMESPACE::DeviceMemory() const VULKAN_HPP_NOEXCEPT
      {
        return m_memory;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_memory )
        {
          getDispatcher()->vkFreeMemory(
            static_cast<VkDevice>( m_device ), static_cast<VkDeviceMemory>( m_memory ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_memory     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DeviceMemory release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_memory, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceMemory & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_memory, rhs.m_memory );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD void * mapMemory( VULKAN_HPP_NAMESPACE::DeviceSize           offset,
                                             VULKAN_HPP_NAMESPACE::DeviceSize           size,
                                             VULKAN_HPP_NAMESPACE::MemoryMapFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      void unmapMemory() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DeviceSize getCommitment() const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::DeviceMemory                                        m_memory     = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class DisplayKHR
    {
    public:
      using CType   = VkDisplayKHR;
      using CppType = vk::DisplayKHR;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDisplayKHR;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDisplayKHR;

    public:
      DisplayKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice const & physicalDevice, VkDisplayKHR display )
        : m_physicalDevice( physicalDevice ), m_display( display ), m_dispatcher( physicalDevice.getDispatcher() )
      {
      }

      DisplayKHR( std::nullptr_t ) {}

      ~DisplayKHR()
      {
        clear();
      }

      DisplayKHR()                     = delete;
      DisplayKHR( DisplayKHR const & ) = delete;

      DisplayKHR( DisplayKHR && rhs ) VULKAN_HPP_NOEXCEPT
        : m_physicalDevice( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_physicalDevice, {} ) )
        , m_display( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_display, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DisplayKHR & operator=( DisplayKHR const & ) = delete;

      DisplayKHR & operator=( DisplayKHR && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_physicalDevice, rhs.m_physicalDevice );
          std::swap( m_display, rhs.m_display );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DisplayKHR const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_display;
      }

      operator VULKAN_HPP_NAMESPACE::DisplayKHR() const VULKAN_HPP_NOEXCEPT
      {
        return m_display;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_display )
        {
          getDispatcher()->vkReleaseDisplayEXT( static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkDisplayKHR>( m_display ) );
        }
        m_physicalDevice = nullptr;
        m_display        = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DisplayKHR release()
      {
        m_physicalDevice = nullptr;
        m_dispatcher     = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_display, nullptr );
      }

      VULKAN_HPP_NAMESPACE::PhysicalDevice getPhysicalDevice() const
      {
        return m_physicalDevice;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_physicalDevice, rhs.m_physicalDevice );
        std::swap( m_display, rhs.m_display );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_KHR_display ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR> getModeProperties() const;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayModeKHR>::Type
        createMode( VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR const &                          createInfo,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      //=== VK_KHR_get_display_properties2 ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR> getModeProperties2() const;

    private:
      VULKAN_HPP_NAMESPACE::PhysicalDevice                                        m_physicalDevice = {};
      VULKAN_HPP_NAMESPACE::DisplayKHR                                            m_display        = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher     = nullptr;
    };

    class DisplayKHRs : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DisplayKHRs( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice const & physicalDevice, uint32_t planeIndex )
      {
        *this = physicalDevice.getDisplayPlaneSupportedDisplaysKHR( planeIndex );
      }
#  endif

      DisplayKHRs( std::nullptr_t ) {}

      DisplayKHRs()                                  = delete;
      DisplayKHRs( DisplayKHRs const & )             = delete;
      DisplayKHRs( DisplayKHRs && rhs )              = default;
      DisplayKHRs & operator=( DisplayKHRs const & ) = delete;
      DisplayKHRs & operator=( DisplayKHRs && rhs )  = default;

    private:
      DisplayKHRs( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    class DisplayModeKHR
    {
    public:
      using CType   = VkDisplayModeKHR;
      using CppType = vk::DisplayModeKHR;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eDisplayModeKHR;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eDisplayModeKHR;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      DisplayModeKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR const &             display,
                      VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR const &                          createInfo,
                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = display.createMode( createInfo, allocator );
      }
#  endif

      DisplayModeKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR const & display, VkDisplayModeKHR displayModeKHR )
        : m_physicalDevice( display.getPhysicalDevice() ), m_displayModeKHR( displayModeKHR ), m_dispatcher( display.getDispatcher() )
      {
      }

      DisplayModeKHR( std::nullptr_t ) {}

      ~DisplayModeKHR()
      {
        clear();
      }

      DisplayModeKHR() = delete;

      DisplayModeKHR( DisplayModeKHR const & rhs ) : m_displayModeKHR( rhs.m_displayModeKHR ), m_dispatcher( rhs.m_dispatcher ) {}

      DisplayModeKHR( DisplayModeKHR && rhs ) VULKAN_HPP_NOEXCEPT
        : m_physicalDevice( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_physicalDevice, {} ) )
        , m_displayModeKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_displayModeKHR, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      DisplayModeKHR & operator=( DisplayModeKHR const & rhs )
      {
        m_displayModeKHR = rhs.m_displayModeKHR;
        m_dispatcher     = rhs.m_dispatcher;
        return *this;
      }

      DisplayModeKHR & operator=( DisplayModeKHR && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_physicalDevice, rhs.m_physicalDevice );
          std::swap( m_displayModeKHR, rhs.m_displayModeKHR );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::DisplayModeKHR const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_displayModeKHR;
      }

      operator VULKAN_HPP_NAMESPACE::DisplayModeKHR() const VULKAN_HPP_NOEXCEPT
      {
        return m_displayModeKHR;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        m_physicalDevice = nullptr;
        m_displayModeKHR = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::DisplayModeKHR release()
      {
        m_physicalDevice = nullptr;
        m_dispatcher     = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_displayModeKHR, nullptr );
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayModeKHR & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_physicalDevice, rhs.m_physicalDevice );
        std::swap( m_displayModeKHR, rhs.m_displayModeKHR );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_KHR_display ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities( uint32_t planeIndex ) const;

    private:
      VULKAN_HPP_NAMESPACE::PhysicalDevice                                        m_physicalDevice = {};
      VULKAN_HPP_NAMESPACE::DisplayModeKHR                                        m_displayModeKHR = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher     = nullptr;
    };

    class Event
    {
    public:
      using CType   = VkEvent;
      using CppType = vk::Event;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eEvent;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eEvent;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Event( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VULKAN_HPP_NAMESPACE::EventCreateInfo const &                                   createInfo,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createEvent( createInfo, allocator );
      }
#  endif

      Event( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VkEvent                                                                         event,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_event( event )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Event( std::nullptr_t ) {}

      ~Event()
      {
        clear();
      }

      Event()                = delete;
      Event( Event const & ) = delete;

      Event( Event && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_event( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_event, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Event & operator=( Event const & ) = delete;

      Event & operator=( Event && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_event, rhs.m_event );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Event const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_event;
      }

      operator VULKAN_HPP_NAMESPACE::Event() const VULKAN_HPP_NOEXCEPT
      {
        return m_event;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_event )
        {
          getDispatcher()->vkDestroyEvent(
            static_cast<VkDevice>( m_device ), static_cast<VkEvent>( m_event ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_event      = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Event release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_event, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Event & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_event, rhs.m_event );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result getStatus() const;

      void set() const;

      void reset() const;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Event                                               m_event      = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class Fence
    {
    public:
      using CType   = VkFence;
      using CppType = vk::Fence;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eFence;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eFence;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Fence( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VULKAN_HPP_NAMESPACE::FenceCreateInfo const &                                   createInfo,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createFence( createInfo, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Fence( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT const &                                deviceEventInfo,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.registerEventEXT( deviceEventInfo, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Fence( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR const &             display,
             VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT const &                               displayEventInfo,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.registerDisplayEventEXT( display, displayEventInfo, allocator );
      }
#  endif

      Fence( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VkFence                                                                         fence,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_fence( fence )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Fence( std::nullptr_t ) {}

      ~Fence()
      {
        clear();
      }

      Fence()                = delete;
      Fence( Fence const & ) = delete;

      Fence( Fence && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_fence( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_fence, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Fence & operator=( Fence const & ) = delete;

      Fence & operator=( Fence && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_fence, rhs.m_fence );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Fence const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_fence;
      }

      operator VULKAN_HPP_NAMESPACE::Fence() const VULKAN_HPP_NOEXCEPT
      {
        return m_fence;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_fence )
        {
          getDispatcher()->vkDestroyFence(
            static_cast<VkDevice>( m_device ), static_cast<VkFence>( m_fence ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_fence      = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Fence release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_fence, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_fence, rhs.m_fence );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result getStatus() const;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Fence                                               m_fence      = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class Framebuffer
    {
    public:
      using CType   = VkFramebuffer;
      using CppType = vk::Framebuffer;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eFramebuffer;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eFramebuffer;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Framebuffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                   VULKAN_HPP_NAMESPACE::FramebufferCreateInfo const &                             createInfo,
                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createFramebuffer( createInfo, allocator );
      }
#  endif

      Framebuffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                   VkFramebuffer                                                                   framebuffer,
                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_framebuffer( framebuffer )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Framebuffer( std::nullptr_t ) {}

      ~Framebuffer()
      {
        clear();
      }

      Framebuffer()                      = delete;
      Framebuffer( Framebuffer const & ) = delete;

      Framebuffer( Framebuffer && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_framebuffer( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_framebuffer, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Framebuffer & operator=( Framebuffer const & ) = delete;

      Framebuffer & operator=( Framebuffer && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_framebuffer, rhs.m_framebuffer );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Framebuffer const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_framebuffer;
      }

      operator VULKAN_HPP_NAMESPACE::Framebuffer() const VULKAN_HPP_NOEXCEPT
      {
        return m_framebuffer;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_framebuffer )
        {
          getDispatcher()->vkDestroyFramebuffer(
            static_cast<VkDevice>( m_device ), static_cast<VkFramebuffer>( m_framebuffer ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device      = nullptr;
        m_framebuffer = nullptr;
        m_allocator   = nullptr;
        m_dispatcher  = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Framebuffer release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_framebuffer, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Framebuffer & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_framebuffer, rhs.m_framebuffer );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device      = {};
      VULKAN_HPP_NAMESPACE::Framebuffer                                         m_framebuffer = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator   = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher  = nullptr;
    };

    class Image
    {
    public:
      using CType   = VkImage;
      using CppType = vk::Image;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eImage;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eImage;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Image( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VULKAN_HPP_NAMESPACE::ImageCreateInfo const &                                   createInfo,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createImage( createInfo, allocator );
      }
#  endif

      Image( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
             VkImage                                                                         image,
             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_image( image )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Image( std::nullptr_t ) {}

      ~Image()
      {
        clear();
      }

      Image()                = delete;
      Image( Image const & ) = delete;

      Image( Image && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_image( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_image, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Image & operator=( Image const & ) = delete;

      Image & operator=( Image && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_image, rhs.m_image );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Image const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_image;
      }

      operator VULKAN_HPP_NAMESPACE::Image() const VULKAN_HPP_NOEXCEPT
      {
        return m_image;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_image )
        {
          getDispatcher()->vkDestroyImage(
            static_cast<VkDevice>( m_device ), static_cast<VkImage>( m_image ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_image      = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Image release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_image, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Image & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_image, rhs.m_image );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void bindMemory( VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::MemoryRequirements getMemoryRequirements() const VULKAN_HPP_NOEXCEPT;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::SubresourceLayout
                           getSubresourceLayout( const VULKAN_HPP_NAMESPACE::ImageSubresource & subresource ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_EXT_image_drm_format_modifier ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT getDrmFormatModifierPropertiesEXT() const;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Image                                               m_image      = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class ImageView
    {
    public:
      using CType   = VkImageView;
      using CppType = vk::ImageView;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eImageView;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eImageView;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      ImageView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VULKAN_HPP_NAMESPACE::ImageViewCreateInfo const &                               createInfo,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createImageView( createInfo, allocator );
      }
#  endif

      ImageView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VkImageView                                                                     imageView,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_imageView( imageView )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      ImageView( std::nullptr_t ) {}

      ~ImageView()
      {
        clear();
      }

      ImageView()                    = delete;
      ImageView( ImageView const & ) = delete;

      ImageView( ImageView && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_imageView( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_imageView, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      ImageView & operator=( ImageView const & ) = delete;

      ImageView & operator=( ImageView && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_imageView, rhs.m_imageView );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::ImageView const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_imageView;
      }

      operator VULKAN_HPP_NAMESPACE::ImageView() const VULKAN_HPP_NOEXCEPT
      {
        return m_imageView;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_imageView )
        {
          getDispatcher()->vkDestroyImageView(
            static_cast<VkDevice>( m_device ), static_cast<VkImageView>( m_imageView ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_imageView  = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::ImageView release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_imageView, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ImageView & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_imageView, rhs.m_imageView );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::ImageView                                           m_imageView  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class PipelineCache
    {
    public:
      using CType   = VkPipelineCache;
      using CppType = vk::PipelineCache;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::ePipelineCache;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipelineCache;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      PipelineCache( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                     VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo const &                           createInfo,
                     VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createPipelineCache( createInfo, allocator );
      }
#  endif

      PipelineCache( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                     VkPipelineCache                                                                 pipelineCache,
                     VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_pipelineCache( pipelineCache )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      PipelineCache( std::nullptr_t ) {}

      ~PipelineCache()
      {
        clear();
      }

      PipelineCache()                        = delete;
      PipelineCache( PipelineCache const & ) = delete;

      PipelineCache( PipelineCache && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_pipelineCache( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_pipelineCache, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      PipelineCache & operator=( PipelineCache const & ) = delete;

      PipelineCache & operator=( PipelineCache && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_pipelineCache, rhs.m_pipelineCache );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::PipelineCache const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipelineCache;
      }

      operator VULKAN_HPP_NAMESPACE::PipelineCache() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipelineCache;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_pipelineCache )
        {
          getDispatcher()->vkDestroyPipelineCache( static_cast<VkDevice>( m_device ),
                                                   static_cast<VkPipelineCache>( m_pipelineCache ),
                                                   reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device        = nullptr;
        m_pipelineCache = nullptr;
        m_allocator     = nullptr;
        m_dispatcher    = nullptr;
      }

      VULKAN_HPP_NAMESPACE::PipelineCache release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_pipelineCache, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_pipelineCache, rhs.m_pipelineCache );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>>::Type
        createGraphicsPipelines( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>            allocator = nullptr ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>::Type
                           createGraphicsPipeline( VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo const &                        createInfo,
                                                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

      VULKAN_HPP_NODISCARD
        VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>>::Type
        createComputePipelines( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>           allocator = nullptr ) const;

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>::Type
                           createComputePipeline( VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo const &                         createInfo,
                                                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr ) const
        VULKAN_HPP_RAII_CREATE_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device        = {};
      VULKAN_HPP_NAMESPACE::PipelineCache                                       m_pipelineCache = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator     = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher    = nullptr;
    };

    class Pipeline
    {
    public:
      using CType   = VkPipeline;
      using CppType = vk::Pipeline;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::ePipeline;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipeline;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Pipeline( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache const &          pipelineCache,
                VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo const &                         createInfo,
                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createComputePipeline( pipelineCache, createInfo, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Pipeline( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache const &          pipelineCache,
                VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo const &                        createInfo,
                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createGraphicsPipeline( pipelineCache, createInfo, allocator );
      }
#  endif

      Pipeline( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                VkPipeline                                                                      pipeline,
                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator   = nullptr,
                VULKAN_HPP_NAMESPACE::Result                                                    successCode = VULKAN_HPP_NAMESPACE::Result::eSuccess )
        : m_device( device )
        , m_pipeline( pipeline )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_constructorSuccessCode( successCode )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Pipeline( std::nullptr_t ) {}

      ~Pipeline()
      {
        clear();
      }

      Pipeline()                   = delete;
      Pipeline( Pipeline const & ) = delete;

      Pipeline( Pipeline && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_pipeline( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_pipeline, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_constructorSuccessCode( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_constructorSuccessCode, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Pipeline & operator=( Pipeline const & ) = delete;

      Pipeline & operator=( Pipeline && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_pipeline, rhs.m_pipeline );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Pipeline const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipeline;
      }

      operator VULKAN_HPP_NAMESPACE::Pipeline() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipeline;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_pipeline )
        {
          getDispatcher()->vkDestroyPipeline(
            static_cast<VkDevice>( m_device ), static_cast<VkPipeline>( m_pipeline ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device                 = nullptr;
        m_pipeline               = nullptr;
        m_allocator              = nullptr;
        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;
        m_dispatcher             = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Pipeline release()
      {
        m_device                 = nullptr;
        m_allocator              = nullptr;
        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;
        m_dispatcher             = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_pipeline, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Result getConstructorSuccessCode() const
      {
        return m_constructorSuccessCode;
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_pipeline, rhs.m_pipeline );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device                 = {};
      VULKAN_HPP_NAMESPACE::Pipeline                                            m_pipeline               = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator              = {};
      VULKAN_HPP_NAMESPACE::Result                                              m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher             = nullptr;
    };

    class Pipelines : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Pipelines( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                           device,
                 VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache const &                    pipelineCache,
                 VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>           allocator = nullptr )
      {
        *this = device.createComputePipelines( pipelineCache, createInfos, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Pipelines( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                            device,
                 VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache const &                     pipelineCache,
                 VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>            allocator = nullptr )
      {
        *this = device.createGraphicsPipelines( pipelineCache, createInfos, allocator );
      }
#  endif

      Pipelines( std::nullptr_t ) {}

      Pipelines()                                = delete;
      Pipelines( Pipelines const & )             = delete;
      Pipelines( Pipelines && rhs )              = default;
      Pipelines & operator=( Pipelines const & ) = delete;
      Pipelines & operator=( Pipelines && rhs )  = default;

    private:
      Pipelines( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    class PipelineLayout
    {
    public:
      using CType   = VkPipelineLayout;
      using CppType = vk::PipelineLayout;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::ePipelineLayout;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::ePipelineLayout;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      PipelineLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                      VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo const &                          createInfo,
                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createPipelineLayout( createInfo, allocator );
      }
#  endif

      PipelineLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                      VkPipelineLayout                                                                pipelineLayout,
                      VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_pipelineLayout( pipelineLayout )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      PipelineLayout( std::nullptr_t ) {}

      ~PipelineLayout()
      {
        clear();
      }

      PipelineLayout()                         = delete;
      PipelineLayout( PipelineLayout const & ) = delete;

      PipelineLayout( PipelineLayout && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_pipelineLayout( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_pipelineLayout, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      PipelineLayout & operator=( PipelineLayout const & ) = delete;

      PipelineLayout & operator=( PipelineLayout && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_pipelineLayout, rhs.m_pipelineLayout );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::PipelineLayout const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipelineLayout;
      }

      operator VULKAN_HPP_NAMESPACE::PipelineLayout() const VULKAN_HPP_NOEXCEPT
      {
        return m_pipelineLayout;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_pipelineLayout )
        {
          getDispatcher()->vkDestroyPipelineLayout( static_cast<VkDevice>( m_device ),
                                                    static_cast<VkPipelineLayout>( m_pipelineLayout ),
                                                    reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device         = nullptr;
        m_pipelineLayout = nullptr;
        m_allocator      = nullptr;
        m_dispatcher     = nullptr;
      }

      VULKAN_HPP_NAMESPACE::PipelineLayout release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_pipelineLayout, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineLayout & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_pipelineLayout, rhs.m_pipelineLayout );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device         = {};
      VULKAN_HPP_NAMESPACE::PipelineLayout                                      m_pipelineLayout = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator      = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher     = nullptr;
    };

    class PrivateDataSlot
    {
    public:
      using CType   = VkPrivateDataSlot;
      using CppType = vk::PrivateDataSlot;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::ePrivateDataSlot;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      PrivateDataSlot( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                       VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo const &                         createInfo,
                       VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createPrivateDataSlot( createInfo, allocator );
      }
#  endif

      PrivateDataSlot( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                       VkPrivateDataSlot                                                               privateDataSlot,
                       VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_privateDataSlot( privateDataSlot )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      PrivateDataSlot( std::nullptr_t ) {}

      ~PrivateDataSlot()
      {
        clear();
      }

      PrivateDataSlot()                          = delete;
      PrivateDataSlot( PrivateDataSlot const & ) = delete;

      PrivateDataSlot( PrivateDataSlot && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_privateDataSlot( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_privateDataSlot, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      PrivateDataSlot & operator=( PrivateDataSlot const & ) = delete;

      PrivateDataSlot & operator=( PrivateDataSlot && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_privateDataSlot, rhs.m_privateDataSlot );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::PrivateDataSlot const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_privateDataSlot;
      }

      operator VULKAN_HPP_NAMESPACE::PrivateDataSlot() const VULKAN_HPP_NOEXCEPT
      {
        return m_privateDataSlot;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_privateDataSlot )
        {
          getDispatcher()->vkDestroyPrivateDataSlot( static_cast<VkDevice>( m_device ),
                                                     static_cast<VkPrivateDataSlot>( m_privateDataSlot ),
                                                     reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device          = nullptr;
        m_privateDataSlot = nullptr;
        m_allocator       = nullptr;
        m_dispatcher      = nullptr;
      }

      VULKAN_HPP_NAMESPACE::PrivateDataSlot release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_privateDataSlot, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PrivateDataSlot & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_privateDataSlot, rhs.m_privateDataSlot );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device          = {};
      VULKAN_HPP_NAMESPACE::PrivateDataSlot                                     m_privateDataSlot = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator       = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher      = nullptr;
    };

    class QueryPool
    {
    public:
      using CType   = VkQueryPool;
      using CppType = vk::QueryPool;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eQueryPool;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eQueryPool;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      QueryPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo const &                               createInfo,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createQueryPool( createInfo, allocator );
      }
#  endif

      QueryPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VkQueryPool                                                                     queryPool,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_queryPool( queryPool )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      QueryPool( std::nullptr_t ) {}

      ~QueryPool()
      {
        clear();
      }

      QueryPool()                    = delete;
      QueryPool( QueryPool const & ) = delete;

      QueryPool( QueryPool && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_queryPool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_queryPool, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      QueryPool & operator=( QueryPool const & ) = delete;

      QueryPool & operator=( QueryPool && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_queryPool, rhs.m_queryPool );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::QueryPool const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_queryPool;
      }

      operator VULKAN_HPP_NAMESPACE::QueryPool() const VULKAN_HPP_NOEXCEPT
      {
        return m_queryPool;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_queryPool )
        {
          getDispatcher()->vkDestroyQueryPool(
            static_cast<VkDevice>( m_device ), static_cast<VkQueryPool>( m_queryPool ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_queryPool  = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::QueryPool release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_queryPool, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::QueryPool & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_queryPool, rhs.m_queryPool );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      template <typename DataType>
      VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, std::vector<DataType>>
                           getResults( uint32_t                                     firstQuery,
                                       uint32_t                                     queryCount,
                                       size_t                                       dataSize,
                                       VULKAN_HPP_NAMESPACE::DeviceSize             stride,
                                       VULKAN_HPP_NAMESPACE::QueryResultFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      template <typename DataType>
      VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, DataType>
                           getResult( uint32_t                                     firstQuery,
                                      uint32_t                                     queryCount,
                                      VULKAN_HPP_NAMESPACE::DeviceSize             stride,
                                      VULKAN_HPP_NAMESPACE::QueryResultFlags flags VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      //=== VK_VERSION_1_2 ===

      void reset( uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::QueryPool                                           m_queryPool  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class Queue
    {
    public:
      using CType   = VkQueue;
      using CppType = vk::Queue;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eQueue;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eQueue;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Queue( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device, uint32_t queueFamilyIndex, uint32_t queueIndex )
      {
        *this = device.getQueue( queueFamilyIndex, queueIndex );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Queue( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device, VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 const & queueInfo )
      {
        *this = device.getQueue2( queueInfo );
      }
#  endif

      Queue( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const & device, VkQueue queue ) : m_queue( queue ), m_dispatcher( device.getDispatcher() )
      {
      }

      Queue( std::nullptr_t ) {}

      ~Queue()
      {
        clear();
      }

      Queue() = delete;

      Queue( Queue const & rhs ) : m_queue( rhs.m_queue ), m_dispatcher( rhs.m_dispatcher ) {}

      Queue( Queue && rhs ) VULKAN_HPP_NOEXCEPT
        : m_queue( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_queue, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Queue & operator=( Queue const & rhs )
      {
        m_queue      = rhs.m_queue;
        m_dispatcher = rhs.m_dispatcher;
        return *this;
      }

      Queue & operator=( Queue && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_queue, rhs.m_queue );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Queue const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_queue;
      }

      operator VULKAN_HPP_NAMESPACE::Queue() const VULKAN_HPP_NOEXCEPT
      {
        return m_queue;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        m_queue      = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Queue release()
      {
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_queue, nullptr );
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_queue, rhs.m_queue );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      void submit( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo> const & submits,
                   VULKAN_HPP_NAMESPACE::Fence fence                                                VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      void waitIdle() const;

      //=== VK_VERSION_1_3 ===

      void submit2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits,
                    VULKAN_HPP_NAMESPACE::Fence fence                                                 VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      //=== VK_KHR_swapchain ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result presentKHR( const VULKAN_HPP_NAMESPACE::PresentInfoKHR & presentInfo ) const;

      //=== VK_EXT_debug_utils ===

      void beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT;

      void endDebugUtilsLabelEXT() const VULKAN_HPP_NOEXCEPT;

      void insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT;

      //=== VK_KHR_synchronization2 ===

      void submit2KHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits,
                       VULKAN_HPP_NAMESPACE::Fence fence                                                 VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV> getCheckpointData2NV() const;

    private:
      VULKAN_HPP_NAMESPACE::Queue                                               m_queue      = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class RenderPass
    {
    public:
      using CType   = VkRenderPass;
      using CppType = vk::RenderPass;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eRenderPass;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eRenderPass;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      RenderPass( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                  VULKAN_HPP_NAMESPACE::RenderPassCreateInfo const &                              createInfo,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createRenderPass( createInfo, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      RenderPass( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                  VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 const &                             createInfo,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createRenderPass2( createInfo, allocator );
      }
#  endif

      RenderPass( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                  VkRenderPass                                                                    renderPass,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_renderPass( renderPass )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      RenderPass( std::nullptr_t ) {}

      ~RenderPass()
      {
        clear();
      }

      RenderPass()                     = delete;
      RenderPass( RenderPass const & ) = delete;

      RenderPass( RenderPass && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_renderPass( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_renderPass, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      RenderPass & operator=( RenderPass const & ) = delete;

      RenderPass & operator=( RenderPass && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_renderPass, rhs.m_renderPass );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::RenderPass const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_renderPass;
      }

      operator VULKAN_HPP_NAMESPACE::RenderPass() const VULKAN_HPP_NOEXCEPT
      {
        return m_renderPass;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_renderPass )
        {
          getDispatcher()->vkDestroyRenderPass(
            static_cast<VkDevice>( m_device ), static_cast<VkRenderPass>( m_renderPass ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_renderPass = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::RenderPass release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_renderPass, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_renderPass, rhs.m_renderPass );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Extent2D getRenderAreaGranularity() const VULKAN_HPP_NOEXCEPT;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::RenderPass                                          m_renderPass = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class Sampler
    {
    public:
      using CType   = VkSampler;
      using CppType = vk::Sampler;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSampler;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSampler;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Sampler( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
               VULKAN_HPP_NAMESPACE::SamplerCreateInfo const &                                 createInfo,
               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createSampler( createInfo, allocator );
      }
#  endif

      Sampler( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
               VkSampler                                                                       sampler,
               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_sampler( sampler )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Sampler( std::nullptr_t ) {}

      ~Sampler()
      {
        clear();
      }

      Sampler()                  = delete;
      Sampler( Sampler const & ) = delete;

      Sampler( Sampler && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_sampler( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_sampler, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Sampler & operator=( Sampler const & ) = delete;

      Sampler & operator=( Sampler && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_sampler, rhs.m_sampler );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Sampler const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_sampler;
      }

      operator VULKAN_HPP_NAMESPACE::Sampler() const VULKAN_HPP_NOEXCEPT
      {
        return m_sampler;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_sampler )
        {
          getDispatcher()->vkDestroySampler(
            static_cast<VkDevice>( m_device ), static_cast<VkSampler>( m_sampler ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_sampler    = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Sampler release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_sampler, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Sampler & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_sampler, rhs.m_sampler );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Sampler                                             m_sampler    = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class SamplerYcbcrConversion
    {
    public:
      using CType   = VkSamplerYcbcrConversion;
      using CppType = vk::SamplerYcbcrConversion;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSamplerYcbcrConversion;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                              VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo const &                  createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createSamplerYcbcrConversion( createInfo, allocator );
      }
#  endif

      SamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                              VkSamplerYcbcrConversion                                                        ycbcrConversion,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_ycbcrConversion( ycbcrConversion )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      SamplerYcbcrConversion( std::nullptr_t ) {}

      ~SamplerYcbcrConversion()
      {
        clear();
      }

      SamplerYcbcrConversion()                                 = delete;
      SamplerYcbcrConversion( SamplerYcbcrConversion const & ) = delete;

      SamplerYcbcrConversion( SamplerYcbcrConversion && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_ycbcrConversion( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_ycbcrConversion, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      SamplerYcbcrConversion & operator=( SamplerYcbcrConversion const & ) = delete;

      SamplerYcbcrConversion & operator=( SamplerYcbcrConversion && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_ycbcrConversion, rhs.m_ycbcrConversion );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_ycbcrConversion;
      }

      operator VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion() const VULKAN_HPP_NOEXCEPT
      {
        return m_ycbcrConversion;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_ycbcrConversion )
        {
          getDispatcher()->vkDestroySamplerYcbcrConversion( static_cast<VkDevice>( m_device ),
                                                            static_cast<VkSamplerYcbcrConversion>( m_ycbcrConversion ),
                                                            reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device          = nullptr;
        m_ycbcrConversion = nullptr;
        m_allocator       = nullptr;
        m_dispatcher      = nullptr;
      }

      VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_ycbcrConversion, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SamplerYcbcrConversion & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_ycbcrConversion, rhs.m_ycbcrConversion );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device          = {};
      VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion                              m_ycbcrConversion = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator       = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher      = nullptr;
    };

    class Semaphore
    {
    public:
      using CType   = VkSemaphore;
      using CppType = vk::Semaphore;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSemaphore;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSemaphore;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      Semaphore( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo const &                               createInfo,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createSemaphore( createInfo, allocator );
      }
#  endif

      Semaphore( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                 VkSemaphore                                                                     semaphore,
                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_semaphore( semaphore )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      Semaphore( std::nullptr_t ) {}

      ~Semaphore()
      {
        clear();
      }

      Semaphore()                    = delete;
      Semaphore( Semaphore const & ) = delete;

      Semaphore( Semaphore && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_semaphore( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_semaphore, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      Semaphore & operator=( Semaphore const & ) = delete;

      Semaphore & operator=( Semaphore && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_semaphore, rhs.m_semaphore );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::Semaphore const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_semaphore;
      }

      operator VULKAN_HPP_NAMESPACE::Semaphore() const VULKAN_HPP_NOEXCEPT
      {
        return m_semaphore;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_semaphore )
        {
          getDispatcher()->vkDestroySemaphore(
            static_cast<VkDevice>( m_device ), static_cast<VkSemaphore>( m_semaphore ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_semaphore  = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::Semaphore release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_semaphore, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Semaphore & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_semaphore, rhs.m_semaphore );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_VERSION_1_2 ===

      VULKAN_HPP_NODISCARD uint64_t getCounterValue() const;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::Semaphore                                           m_semaphore  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

#  if defined( VK_USE_PLATFORM_SCI )
    class SemaphoreSciSyncPoolNV
    {
    public:
      using CType   = VkSemaphoreSciSyncPoolNV;
      using CppType = vk::SemaphoreSciSyncPoolNV;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSemaphoreSciSyncPoolNV;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;

    public:
#    if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SemaphoreSciSyncPoolNV( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                              VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV const &                  createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createSemaphoreSciSyncPoolNV( createInfo, allocator );
      }
#    endif

      SemaphoreSciSyncPoolNV( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                              VkSemaphoreSciSyncPoolNV                                                        semaphorePool,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_semaphorePool( semaphorePool )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      SemaphoreSciSyncPoolNV( std::nullptr_t ) {}

      ~SemaphoreSciSyncPoolNV()
      {
        clear();
      }

      SemaphoreSciSyncPoolNV()                                 = delete;
      SemaphoreSciSyncPoolNV( SemaphoreSciSyncPoolNV const & ) = delete;

      SemaphoreSciSyncPoolNV( SemaphoreSciSyncPoolNV && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_semaphorePool( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_semaphorePool, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      SemaphoreSciSyncPoolNV & operator=( SemaphoreSciSyncPoolNV const & ) = delete;

      SemaphoreSciSyncPoolNV & operator=( SemaphoreSciSyncPoolNV && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_semaphorePool, rhs.m_semaphorePool );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_semaphorePool;
      }

      operator VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV() const VULKAN_HPP_NOEXCEPT
      {
        return m_semaphorePool;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_semaphorePool )
        {
          getDispatcher()->vkDestroySemaphoreSciSyncPoolNV( static_cast<VkDevice>( m_device ),
                                                            static_cast<VkSemaphoreSciSyncPoolNV>( m_semaphorePool ),
                                                            reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device        = nullptr;
        m_semaphorePool = nullptr;
        m_allocator     = nullptr;
        m_dispatcher    = nullptr;
      }

      VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_semaphorePool, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SemaphoreSciSyncPoolNV & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_semaphorePool, rhs.m_semaphorePool );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device        = {};
      VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV                              m_semaphorePool = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator     = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher    = nullptr;
    };
#  endif /*VK_USE_PLATFORM_SCI*/

    class ShaderModule
    {
    public:
      using CType   = VkShaderModule;
      using CppType = vk::ShaderModule;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eShaderModule;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eShaderModule;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      ShaderModule( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VULKAN_HPP_NAMESPACE::ShaderModuleCreateInfo const &                            createInfo,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createShaderModule( createInfo, allocator );
      }
#  endif

      ShaderModule( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VkShaderModule                                                                  shaderModule,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_shaderModule( shaderModule )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      ShaderModule( std::nullptr_t ) {}

      ~ShaderModule()
      {
        clear();
      }

      ShaderModule()                       = delete;
      ShaderModule( ShaderModule const & ) = delete;

      ShaderModule( ShaderModule && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_shaderModule( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_shaderModule, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      ShaderModule & operator=( ShaderModule const & ) = delete;

      ShaderModule & operator=( ShaderModule && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_shaderModule, rhs.m_shaderModule );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::ShaderModule const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_shaderModule;
      }

      operator VULKAN_HPP_NAMESPACE::ShaderModule() const VULKAN_HPP_NOEXCEPT
      {
        return m_shaderModule;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_shaderModule )
        {
          getDispatcher()->vkDestroyShaderModule(
            static_cast<VkDevice>( m_device ), static_cast<VkShaderModule>( m_shaderModule ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device       = nullptr;
        m_shaderModule = nullptr;
        m_allocator    = nullptr;
        m_dispatcher   = nullptr;
      }

      VULKAN_HPP_NAMESPACE::ShaderModule release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_shaderModule, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ShaderModule & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_shaderModule, rhs.m_shaderModule );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device       = {};
      VULKAN_HPP_NAMESPACE::ShaderModule                                        m_shaderModule = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator    = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher   = nullptr;
    };

    class SurfaceKHR
    {
    public:
      using CType   = VkSurfaceKHR;
      using CppType = vk::SurfaceKHR;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSurfaceKHR;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSurfaceKHR;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SurfaceKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const &               instance,
                  VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR const &                       createInfo,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = instance.createDisplayPlaneSurfaceKHR( createInfo, allocator );
      }
#  endif

#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SurfaceKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const &               instance,
                  VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT const &                      createInfo,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = instance.createHeadlessSurfaceEXT( createInfo, allocator );
      }
#  endif

      SurfaceKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance const &               instance,
                  VkSurfaceKHR                                                                    surface,
                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_instance( instance )
        , m_surface( surface )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( instance.getDispatcher() )
      {
      }

      SurfaceKHR( std::nullptr_t ) {}

      ~SurfaceKHR()
      {
        clear();
      }

      SurfaceKHR()                     = delete;
      SurfaceKHR( SurfaceKHR const & ) = delete;

      SurfaceKHR( SurfaceKHR && rhs ) VULKAN_HPP_NOEXCEPT
        : m_instance( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_instance, {} ) )
        , m_surface( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_surface, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      SurfaceKHR & operator=( SurfaceKHR const & ) = delete;

      SurfaceKHR & operator=( SurfaceKHR && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_instance, rhs.m_instance );
          std::swap( m_surface, rhs.m_surface );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::SurfaceKHR const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_surface;
      }

      operator VULKAN_HPP_NAMESPACE::SurfaceKHR() const VULKAN_HPP_NOEXCEPT
      {
        return m_surface;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_surface )
        {
          getDispatcher()->vkDestroySurfaceKHR(
            static_cast<VkInstance>( m_instance ), static_cast<VkSurfaceKHR>( m_surface ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_instance   = nullptr;
        m_surface    = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::SurfaceKHR release()
      {
        m_instance   = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_surface, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Instance getInstance() const
      {
        return m_instance;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_instance, rhs.m_instance );
        std::swap( m_surface, rhs.m_surface );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

    private:
      VULKAN_HPP_NAMESPACE::Instance                                              m_instance   = {};
      VULKAN_HPP_NAMESPACE::SurfaceKHR                                            m_surface    = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                           m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher = nullptr;
    };

    class SwapchainKHR
    {
    public:
      using CType   = VkSwapchainKHR;
      using CppType = vk::SwapchainKHR;

      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::eSwapchainKHR;
      static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType =
        VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eSwapchainKHR;

    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SwapchainKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const &                            createInfo,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
      {
        *this = device.createSwapchainKHR( createInfo, allocator );
      }
#  endif

      SwapchainKHR( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                 device,
                    VkSwapchainKHR                                                                  swapchain,
                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr )
        : m_device( device )
        , m_swapchain( swapchain )
        , m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )
        , m_dispatcher( device.getDispatcher() )
      {
      }

      SwapchainKHR( std::nullptr_t ) {}

      ~SwapchainKHR()
      {
        clear();
      }

      SwapchainKHR()                       = delete;
      SwapchainKHR( SwapchainKHR const & ) = delete;

      SwapchainKHR( SwapchainKHR && rhs ) VULKAN_HPP_NOEXCEPT
        : m_device( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_device, {} ) )
        , m_swapchain( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_swapchain, {} ) )
        , m_allocator( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_allocator, {} ) )
        , m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )
      {
      }

      SwapchainKHR & operator=( SwapchainKHR const & ) = delete;

      SwapchainKHR & operator=( SwapchainKHR && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
          std::swap( m_device, rhs.m_device );
          std::swap( m_swapchain, rhs.m_swapchain );
          std::swap( m_allocator, rhs.m_allocator );
          std::swap( m_dispatcher, rhs.m_dispatcher );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::SwapchainKHR const & operator*() const VULKAN_HPP_NOEXCEPT
      {
        return m_swapchain;
      }

      operator VULKAN_HPP_NAMESPACE::SwapchainKHR() const VULKAN_HPP_NOEXCEPT
      {
        return m_swapchain;
      }

      void clear() VULKAN_HPP_NOEXCEPT
      {
        if ( m_swapchain )
        {
          getDispatcher()->vkDestroySwapchainKHR(
            static_cast<VkDevice>( m_device ), static_cast<VkSwapchainKHR>( m_swapchain ), reinterpret_cast<const VkAllocationCallbacks *>( m_allocator ) );
        }
        m_device     = nullptr;
        m_swapchain  = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
      }

      VULKAN_HPP_NAMESPACE::SwapchainKHR release()
      {
        m_device     = nullptr;
        m_allocator  = nullptr;
        m_dispatcher = nullptr;
        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_swapchain, nullptr );
      }

      VULKAN_HPP_NAMESPACE::Device getDevice() const
      {
        return m_device;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR & rhs ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_device, rhs.m_device );
        std::swap( m_swapchain, rhs.m_swapchain );
        std::swap( m_allocator, rhs.m_allocator );
        std::swap( m_dispatcher, rhs.m_dispatcher );
      }

      //=== VK_KHR_swapchain ===

      VULKAN_HPP_NODISCARD std::vector<VULKAN_HPP_NAMESPACE::Image> getImages() const;

      VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, uint32_t>
                           acquireNextImage( uint64_t                                  timeout,
                                             VULKAN_HPP_NAMESPACE::Semaphore semaphore VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT,
                                             VULKAN_HPP_NAMESPACE::Fence fence         VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT ) const;

      //=== VK_EXT_display_control ===

      VULKAN_HPP_NODISCARD uint64_t getCounterEXT( VULKAN_HPP_NAMESPACE::SurfaceCounterFlagBitsEXT counter ) const;

      //=== VK_KHR_shared_presentable_image ===

      VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result getStatus() const;

    private:
      VULKAN_HPP_NAMESPACE::Device                                              m_device     = {};
      VULKAN_HPP_NAMESPACE::SwapchainKHR                                        m_swapchain  = {};
      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *                         m_allocator  = {};
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;
    };

    class SwapchainKHRs : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>
    {
    public:
#  if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
      SwapchainKHRs( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device const &                        device,
                     VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                     VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>        allocator = nullptr )
      {
        *this = device.createSharedSwapchainsKHR( createInfos, allocator );
      }
#  endif

      SwapchainKHRs( std::nullptr_t ) {}

      SwapchainKHRs()                                    = delete;
      SwapchainKHRs( SwapchainKHRs const & )             = delete;
      SwapchainKHRs( SwapchainKHRs && rhs )              = default;
      SwapchainKHRs & operator=( SwapchainKHRs const & ) = delete;
      SwapchainKHRs & operator=( SwapchainKHRs && rhs )  = default;

    private:
      SwapchainKHRs( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR> && rhs )
      {
        std::swap( *this, rhs );
      }
    };

    //===========================
    //=== COMMAND Definitions ===
    //===========================

    //=== VK_VERSION_1_0 ===

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance>::Type
                        Context::createInstance( VULKAN_HPP_NAMESPACE::InstanceCreateInfo const &                                createInfo,
                               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Instance instance;
      VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateInstance(
        reinterpret_cast<const VkInstanceCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkInstance *>( &instance ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Context::createInstance" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Instance( *this, *reinterpret_cast<VkInstance *>( &instance ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice>>::Type
      Instance::enumeratePhysicalDevices() const
    {
      std::vector<VULKAN_HPP_NAMESPACE::PhysicalDevice> physicalDevices;
      uint32_t                                          physicalDeviceCount;
      VULKAN_HPP_NAMESPACE::Result                      result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkEnumeratePhysicalDevices( static_cast<VkInstance>( m_instance ), &physicalDeviceCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceCount )
        {
          physicalDevices.resize( physicalDeviceCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumeratePhysicalDevices(
            static_cast<VkInstance>( m_instance ), &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice *>( physicalDevices.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::eIncomplete ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Instance::enumeratePhysicalDevices" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PhysicalDevice> physicalDevicesRAII;
      physicalDevicesRAII.reserve( physicalDevices.size() );
      for ( auto & physicalDevice : physicalDevices )
      {
        physicalDevicesRAII.emplace_back( *this, *reinterpret_cast<VkPhysicalDevice *>( &physicalDevice ) );
      }
      return physicalDevicesRAII;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures PhysicalDevice::getFeatures() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFeatures && "Function <vkGetPhysicalDeviceFeatures> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures features;
      getDispatcher()->vkGetPhysicalDeviceFeatures( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                    reinterpret_cast<VkPhysicalDeviceFeatures *>( &features ) );

      return features;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::FormatProperties
                                           PhysicalDevice::getFormatProperties( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFormatProperties && "Function <vkGetPhysicalDeviceFormatProperties> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::FormatProperties formatProperties;
      getDispatcher()->vkGetPhysicalDeviceFormatProperties(
        static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties *>( &formatProperties ) );

      return formatProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ImageFormatProperties
                                           PhysicalDevice::getImageFormatProperties( VULKAN_HPP_NAMESPACE::Format           format,
                                                VULKAN_HPP_NAMESPACE::ImageType        type,
                                                VULKAN_HPP_NAMESPACE::ImageTiling      tiling,
                                                VULKAN_HPP_NAMESPACE::ImageUsageFlags  usage,
                                                VULKAN_HPP_NAMESPACE::ImageCreateFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceImageFormatProperties &&
                         "Function <vkGetPhysicalDeviceImageFormatProperties> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::ImageFormatProperties imageFormatProperties;
      VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceImageFormatProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                   static_cast<VkFormat>( format ),
                                                                   static_cast<VkImageType>( type ),
                                                                   static_cast<VkImageTiling>( tiling ),
                                                                   static_cast<VkImageUsageFlags>( usage ),
                                                                   static_cast<VkImageCreateFlags>( flags ),
                                                                   reinterpret_cast<VkImageFormatProperties *>( &imageFormatProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties" );

      return imageFormatProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties PhysicalDevice::getProperties() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceProperties && "Function <vkGetPhysicalDeviceProperties> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties properties;
      getDispatcher()->vkGetPhysicalDeviceProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                      reinterpret_cast<VkPhysicalDeviceProperties *>( &properties ) );

      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties &&
                         "Function <vkGetPhysicalDeviceQueueFamilyProperties> requires <VK_VERSION_1_0>" );

      std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties> queueFamilyProperties;
      uint32_t                                                 queueFamilyPropertyCount;
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ), &queueFamilyPropertyCount, nullptr );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                 &queueFamilyPropertyCount,
                                                                 reinterpret_cast<VkQueueFamilyProperties *>( queueFamilyProperties.data() ) );

      VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
      if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
      {
        queueFamilyProperties.resize( queueFamilyPropertyCount );
      }
      return queueFamilyProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceMemoryProperties && "Function <vkGetPhysicalDeviceMemoryProperties> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties memoryProperties;
      getDispatcher()->vkGetPhysicalDeviceMemoryProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                            reinterpret_cast<VkPhysicalDeviceMemoryProperties *>( &memoryProperties ) );

      return memoryProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE PFN_vkVoidFunction Instance::getProcAddr( const std::string & name ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetInstanceProcAddr && "Function <vkGetInstanceProcAddr> requires <VK_VERSION_1_0>" );

      PFN_vkVoidFunction result = getDispatcher()->vkGetInstanceProcAddr( static_cast<VkInstance>( m_instance ), name.c_str() );

      return result;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE PFN_vkVoidFunction Device::getProcAddr( const std::string & name ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceProcAddr && "Function <vkGetDeviceProcAddr> requires <VK_VERSION_1_0>" );

      PFN_vkVoidFunction result = getDispatcher()->vkGetDeviceProcAddr( static_cast<VkDevice>( m_device ), name.c_str() );

      return result;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device>::Type
                        PhysicalDevice::createDevice( VULKAN_HPP_NAMESPACE::DeviceCreateInfo const &                                  createInfo,
                                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Device device;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDevice(
        static_cast<VkPhysicalDevice>( m_physicalDevice ),
        reinterpret_cast<const VkDeviceCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDevice *>( &device ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PhysicalDevice::createDevice" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Device( *this, *reinterpret_cast<VkDevice *>( &device ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties>
                                           Context::enumerateInstanceExtensionProperties( Optional<const std::string> layerName ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumerateInstanceExtensionProperties &&
                         "Function <vkEnumerateInstanceExtensionProperties> requires <VK_VERSION_1_0>" );

      std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties> properties;
      uint32_t                                               propertyCount;
      VULKAN_HPP_NAMESPACE::Result                           result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkEnumerateInstanceExtensionProperties( layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumerateInstanceExtensionProperties(
            layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Context::enumerateInstanceExtensionProperties" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties>
                                           PhysicalDevice::enumerateDeviceExtensionProperties( Optional<const std::string> layerName ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumerateDeviceExtensionProperties && "Function <vkEnumerateDeviceExtensionProperties> requires <VK_VERSION_1_0>" );

      std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties> properties;
      uint32_t                                               propertyCount;
      VULKAN_HPP_NAMESPACE::Result                           result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumerateDeviceExtensionProperties(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkEnumerateDeviceExtensionProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                   layerName ? layerName->c_str() : nullptr,
                                                                   &propertyCount,
                                                                   reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateDeviceExtensionProperties" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> Context::enumerateInstanceLayerProperties() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumerateInstanceLayerProperties && "Function <vkEnumerateInstanceLayerProperties> requires <VK_VERSION_1_0>" );

      std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> properties;
      uint32_t                                           propertyCount;
      VULKAN_HPP_NAMESPACE::Result                       result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumerateInstanceLayerProperties( &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Context::enumerateInstanceLayerProperties" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> PhysicalDevice::enumerateDeviceLayerProperties() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumerateDeviceLayerProperties && "Function <vkEnumerateDeviceLayerProperties> requires <VK_VERSION_1_0>" );

      std::vector<VULKAN_HPP_NAMESPACE::LayerProperties> properties;
      uint32_t                                           propertyCount;
      getDispatcher()->vkEnumerateDeviceLayerProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, nullptr );
      properties.resize( propertyCount );
      getDispatcher()->vkEnumerateDeviceLayerProperties(
        static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) );

      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue>::Type
                        Device::getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Queue queue;
      getDispatcher()->vkGetDeviceQueue( static_cast<VkDevice>( m_device ), queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue *>( &queue ) );

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue( *this, *reinterpret_cast<VkQueue *>( &queue ) );
    }

    VULKAN_HPP_INLINE void Queue::submit( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo> const & submits,
                                          VULKAN_HPP_NAMESPACE::Fence                                                      fence ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueSubmit && "Function <vkQueueSubmit> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkQueueSubmit(
        static_cast<VkQueue>( m_queue ), submits.size(), reinterpret_cast<const VkSubmitInfo *>( submits.data() ), static_cast<VkFence>( fence ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit" );
    }

    VULKAN_HPP_INLINE void Queue::waitIdle() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueWaitIdle && "Function <vkQueueWaitIdle> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkQueueWaitIdle( static_cast<VkQueue>( m_queue ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::waitIdle" );
    }

    VULKAN_HPP_INLINE void Device::waitIdle() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkDeviceWaitIdle && "Function <vkDeviceWaitIdle> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkDeviceWaitIdle( static_cast<VkDevice>( m_device ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitIdle" );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceMemory>::Type
                        Device::allocateMemory( VULKAN_HPP_NAMESPACE::MemoryAllocateInfo const &                                allocateInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::DeviceMemory memory;
      VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkAllocateMemory(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkMemoryAllocateInfo *>( &allocateInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDeviceMemory *>( &memory ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::allocateMemory" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceMemory( *this, *reinterpret_cast<VkDeviceMemory *>( &memory ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE void * DeviceMemory::mapMemory( VULKAN_HPP_NAMESPACE::DeviceSize     offset,
                                                                           VULKAN_HPP_NAMESPACE::DeviceSize     size,
                                                                           VULKAN_HPP_NAMESPACE::MemoryMapFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkMapMemory && "Function <vkMapMemory> requires <VK_VERSION_1_0>" );

      void *                       pData;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkMapMemory( static_cast<VkDevice>( m_device ),
                                                                                                                     static_cast<VkDeviceMemory>( m_memory ),
                                                                                                                     static_cast<VkDeviceSize>( offset ),
                                                                                                                     static_cast<VkDeviceSize>( size ),
                                                                                                                     static_cast<VkMemoryMapFlags>( flags ),
                                                                                                                     &pData ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::DeviceMemory::mapMemory" );

      return pData;
    }

    VULKAN_HPP_INLINE void DeviceMemory::unmapMemory() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkUnmapMemory && "Function <vkUnmapMemory> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkUnmapMemory( static_cast<VkDevice>( m_device ), static_cast<VkDeviceMemory>( m_memory ) );
    }

    VULKAN_HPP_INLINE void
      Device::flushMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkFlushMappedMemoryRanges && "Function <vkFlushMappedMemoryRanges> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkFlushMappedMemoryRanges(
        static_cast<VkDevice>( m_device ), memoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange *>( memoryRanges.data() ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::flushMappedMemoryRanges" );
    }

    VULKAN_HPP_INLINE void
      Device::invalidateMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkInvalidateMappedMemoryRanges && "Function <vkInvalidateMappedMemoryRanges> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkInvalidateMappedMemoryRanges(
        static_cast<VkDevice>( m_device ), memoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange *>( memoryRanges.data() ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::invalidateMappedMemoryRanges" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceSize DeviceMemory::getCommitment() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceMemoryCommitment && "Function <vkGetDeviceMemoryCommitment> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::DeviceSize committedMemoryInBytes;
      getDispatcher()->vkGetDeviceMemoryCommitment(
        static_cast<VkDevice>( m_device ), static_cast<VkDeviceMemory>( m_memory ), reinterpret_cast<VkDeviceSize *>( &committedMemoryInBytes ) );

      return committedMemoryInBytes;
    }

    VULKAN_HPP_INLINE void Buffer::bindMemory( VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkBindBufferMemory && "Function <vkBindBufferMemory> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkBindBufferMemory( static_cast<VkDevice>( m_device ),
                                                                                        static_cast<VkBuffer>( m_buffer ),
                                                                                        static_cast<VkDeviceMemory>( memory ),
                                                                                        static_cast<VkDeviceSize>( memoryOffset ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Buffer::bindMemory" );
    }

    VULKAN_HPP_INLINE void Image::bindMemory( VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkBindImageMemory && "Function <vkBindImageMemory> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkBindImageMemory( static_cast<VkDevice>( m_device ),
                                                                                       static_cast<VkImage>( m_image ),
                                                                                       static_cast<VkDeviceMemory>( memory ),
                                                                                       static_cast<VkDeviceSize>( memoryOffset ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Image::bindMemory" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements Buffer::getMemoryRequirements() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetBufferMemoryRequirements && "Function <vkGetBufferMemoryRequirements> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements memoryRequirements;
      getDispatcher()->vkGetBufferMemoryRequirements(
        static_cast<VkDevice>( m_device ), static_cast<VkBuffer>( m_buffer ), reinterpret_cast<VkMemoryRequirements *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements Image::getMemoryRequirements() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetImageMemoryRequirements && "Function <vkGetImageMemoryRequirements> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements memoryRequirements;
      getDispatcher()->vkGetImageMemoryRequirements(
        static_cast<VkDevice>( m_device ), static_cast<VkImage>( m_image ), reinterpret_cast<VkMemoryRequirements *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                        Device::createFence( VULKAN_HPP_NAMESPACE::FenceCreateInfo const &                                   createInfo,
                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Fence  fence;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateFence(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkFenceCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkFence *>( &fence ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createFence" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence( *this, *reinterpret_cast<VkFence *>( &fence ), allocator );
    }

    VULKAN_HPP_INLINE void Device::resetFences( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetFences && "Function <vkResetFences> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkResetFences( static_cast<VkDevice>( m_device ), fences.size(), reinterpret_cast<const VkFence *>( fences.data() ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::resetFences" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Fence::getStatus() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetFenceStatus && "Function <vkGetFenceStatus> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetFenceStatus( static_cast<VkDevice>( m_device ), static_cast<VkFence>( m_fence ) ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::Fence::getStatus", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Device::waitForFences(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences, VULKAN_HPP_NAMESPACE::Bool32 waitAll, uint64_t timeout ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkWaitForFences && "Function <vkWaitForFences> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkWaitForFences(
        static_cast<VkDevice>( m_device ), fences.size(), reinterpret_cast<const VkFence *>( fences.data() ), static_cast<VkBool32>( waitAll ), timeout ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitForFences", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eTimeout } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Semaphore>::Type
                        Device::createSemaphore( VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo const &                               createInfo,
                               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Semaphore semaphore;
      VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSemaphore(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkSemaphoreCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSemaphore *>( &semaphore ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSemaphore" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Semaphore( *this, *reinterpret_cast<VkSemaphore *>( &semaphore ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Event>::Type
                        Device::createEvent( VULKAN_HPP_NAMESPACE::EventCreateInfo const &                                   createInfo,
                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Event  event;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateEvent(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkEventCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkEvent *>( &event ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createEvent" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Event( *this, *reinterpret_cast<VkEvent *>( &event ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Event::getStatus() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetEventStatus && "Function <vkGetEventStatus> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetEventStatus( static_cast<VkDevice>( m_device ), static_cast<VkEvent>( m_event ) ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::Event::getStatus", { VULKAN_HPP_NAMESPACE::Result::eEventSet, VULKAN_HPP_NAMESPACE::Result::eEventReset } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    VULKAN_HPP_INLINE void Event::set() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSetEvent && "Function <vkSetEvent> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkSetEvent( static_cast<VkDevice>( m_device ), static_cast<VkEvent>( m_event ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Event::set" );
    }

    VULKAN_HPP_INLINE void Event::reset() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetEvent && "Function <vkResetEvent> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkResetEvent( static_cast<VkDevice>( m_device ), static_cast<VkEvent>( m_event ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Event::reset" );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::QueryPool>::Type
                        Device::createQueryPool( VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo const &                               createInfo,
                               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::QueryPool queryPool;
      VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateQueryPool(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkQueryPoolCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkQueryPool *>( &queryPool ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createQueryPool" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::QueryPool( *this, *reinterpret_cast<VkQueryPool *>( &queryPool ), allocator );
    }

    template <typename DataType>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, std::vector<DataType>> QueryPool::getResults(
      uint32_t firstQuery, uint32_t queryCount, size_t dataSize, VULKAN_HPP_NAMESPACE::DeviceSize stride, VULKAN_HPP_NAMESPACE::QueryResultFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetQueryPoolResults && "Function <vkGetQueryPoolResults> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_ASSERT( dataSize % sizeof( DataType ) == 0 );
      std::vector<DataType>        data( dataSize / sizeof( DataType ) );
      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetQueryPoolResults( static_cast<VkDevice>( m_device ),
                                                                                           static_cast<VkQueryPool>( m_queryPool ),
                                                                                           firstQuery,
                                                                                           queryCount,
                                                                                           data.size() * sizeof( DataType ),
                                                                                           reinterpret_cast<void *>( data.data() ),
                                                                                           static_cast<VkDeviceSize>( stride ),
                                                                                           static_cast<VkQueryResultFlags>( flags ) ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::QueryPool::getResults", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

      return std::make_pair( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), data );
    }

    template <typename DataType>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, DataType> QueryPool::getResult(
      uint32_t firstQuery, uint32_t queryCount, VULKAN_HPP_NAMESPACE::DeviceSize stride, VULKAN_HPP_NAMESPACE::QueryResultFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetQueryPoolResults && "Function <vkGetQueryPoolResults> requires <VK_VERSION_1_0>" );

      DataType                     data;
      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetQueryPoolResults( static_cast<VkDevice>( m_device ),
                                                                                           static_cast<VkQueryPool>( m_queryPool ),
                                                                                           firstQuery,
                                                                                           queryCount,
                                                                                           sizeof( DataType ),
                                                                                           reinterpret_cast<void *>( &data ),
                                                                                           static_cast<VkDeviceSize>( stride ),
                                                                                           static_cast<VkQueryResultFlags>( flags ) ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::QueryPool::getResult", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

      return std::make_pair( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), data );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Buffer>::Type
                        Device::createBuffer( VULKAN_HPP_NAMESPACE::BufferCreateInfo const &                                  createInfo,
                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Buffer buffer;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateBuffer(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkBufferCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkBuffer *>( &buffer ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createBuffer" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Buffer( *this, *reinterpret_cast<VkBuffer *>( &buffer ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::BufferView>::Type
                        Device::createBufferView( VULKAN_HPP_NAMESPACE::BufferViewCreateInfo const &                              createInfo,
                                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::BufferView view;
      VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateBufferView(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkBufferViewCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkBufferView *>( &view ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createBufferView" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::BufferView( *this, *reinterpret_cast<VkBufferView *>( &view ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Image>::Type
                        Device::createImage( VULKAN_HPP_NAMESPACE::ImageCreateInfo const &                                   createInfo,
                           VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Image  image;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateImage(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkImageCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkImage *>( &image ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createImage" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Image( *this, *reinterpret_cast<VkImage *>( &image ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::SubresourceLayout
                                           Image::getSubresourceLayout( const VULKAN_HPP_NAMESPACE::ImageSubresource & subresource ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetImageSubresourceLayout && "Function <vkGetImageSubresourceLayout> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::SubresourceLayout layout;
      getDispatcher()->vkGetImageSubresourceLayout( static_cast<VkDevice>( m_device ),
                                                    static_cast<VkImage>( m_image ),
                                                    reinterpret_cast<const VkImageSubresource *>( &subresource ),
                                                    reinterpret_cast<VkSubresourceLayout *>( &layout ) );

      return layout;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ImageView>::Type
                        Device::createImageView( VULKAN_HPP_NAMESPACE::ImageViewCreateInfo const &                               createInfo,
                               VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::ImageView view;
      VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateImageView(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkImageViewCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkImageView *>( &view ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createImageView" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ImageView( *this, *reinterpret_cast<VkImageView *>( &view ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache>::Type
                        Device::createPipelineCache( VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo const &                           createInfo,
                                   VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::PipelineCache pipelineCache;
      VULKAN_HPP_NAMESPACE::Result        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreatePipelineCache(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkPipelineCacheCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipelineCache *>( &pipelineCache ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createPipelineCache" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineCache( *this, *reinterpret_cast<VkPipelineCache *>( &pipelineCache ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>>::Type
      PipelineCache::createGraphicsPipelines( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>            allocator ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
      VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateGraphicsPipelines(
        static_cast<VkDevice>( m_device ),
        static_cast<VkPipelineCache>( m_pipelineCache ),
        createInfos.size(),
        reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( createInfos.data() ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PipelineCache::createGraphicsPipelines" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline> pipelinesRAII;
      pipelinesRAII.reserve( pipelines.size() );
      for ( auto & pipeline : pipelines )
      {
        pipelinesRAII.emplace_back( *this, *reinterpret_cast<VkPipeline *>( &pipeline ), allocator, result );
      }
      return pipelinesRAII;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>::Type
                        PipelineCache::createGraphicsPipeline( VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo const &                        createInfo,
                                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Pipeline pipeline;
      VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateGraphicsPipelines(
        static_cast<VkDevice>( m_device ),
        static_cast<VkPipelineCache>( m_pipelineCache ),
        1,
        reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipeline *>( &pipeline ) ) );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PipelineCache::createGraphicsPipeline" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline( *this, *reinterpret_cast<VkPipeline *>( &pipeline ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>>::Type
      PipelineCache::createComputePipelines( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>           allocator ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
      VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateComputePipelines(
        static_cast<VkDevice>( m_device ),
        static_cast<VkPipelineCache>( m_pipelineCache ),
        createInfos.size(),
        reinterpret_cast<const VkComputePipelineCreateInfo *>( createInfos.data() ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PipelineCache::createComputePipelines" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline> pipelinesRAII;
      pipelinesRAII.reserve( pipelines.size() );
      for ( auto & pipeline : pipelines )
      {
        pipelinesRAII.emplace_back( *this, *reinterpret_cast<VkPipeline *>( &pipeline ), allocator, result );
      }
      return pipelinesRAII;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline>::Type
                        PipelineCache::createComputePipeline( VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo const &                         createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Pipeline pipeline;
      VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateComputePipelines(
        static_cast<VkDevice>( m_device ),
        static_cast<VkPipelineCache>( m_pipelineCache ),
        1,
        reinterpret_cast<const VkComputePipelineCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipeline *>( &pipeline ) ) );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PipelineCache::createComputePipeline" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Pipeline( *this, *reinterpret_cast<VkPipeline *>( &pipeline ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineLayout>::Type
                        Device::createPipelineLayout( VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo const &                          createInfo,
                                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::PipelineLayout pipelineLayout;
      VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreatePipelineLayout(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkPipelineLayoutCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPipelineLayout *>( &pipelineLayout ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createPipelineLayout" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PipelineLayout( *this, *reinterpret_cast<VkPipelineLayout *>( &pipelineLayout ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Sampler>::Type
                        Device::createSampler( VULKAN_HPP_NAMESPACE::SamplerCreateInfo const &                                 createInfo,
                             VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Sampler sampler;
      VULKAN_HPP_NAMESPACE::Result  result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSampler(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkSamplerCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSampler *>( &sampler ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSampler" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Sampler( *this, *reinterpret_cast<VkSampler *>( &sampler ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSetLayout>::Type
      Device::createDescriptorSetLayout( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo const &                     createInfo,
                                         VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::DescriptorSetLayout setLayout;
      VULKAN_HPP_NAMESPACE::Result              result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDescriptorSetLayout(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDescriptorSetLayout *>( &setLayout ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createDescriptorSetLayout" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSetLayout( *this, *reinterpret_cast<VkDescriptorSetLayout *>( &setLayout ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorPool>::Type
                        Device::createDescriptorPool( VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo const &                          createInfo,
                                    VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::DescriptorPool descriptorPool;
      VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDescriptorPool(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkDescriptorPoolCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDescriptorPool *>( &descriptorPool ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createDescriptorPool" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorPool( *this, *reinterpret_cast<VkDescriptorPool *>( &descriptorPool ), allocator );
    }

    VULKAN_HPP_INLINE void DescriptorPool::reset( VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetDescriptorPool && "Function <vkResetDescriptorPool> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkResetDescriptorPool(
        static_cast<VkDevice>( m_device ), static_cast<VkDescriptorPool>( m_descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet>>::Type
      Device::allocateDescriptorSets( VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo const & allocateInfo ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet> descriptorSets( allocateInfo.descriptorSetCount );
      VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkAllocateDescriptorSets( static_cast<VkDevice>( m_device ),
                                                   reinterpret_cast<const VkDescriptorSetAllocateInfo *>( &allocateInfo ),
                                                   reinterpret_cast<VkDescriptorSet *>( descriptorSets.data() ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::allocateDescriptorSets" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DescriptorSet> descriptorSetsRAII;
      descriptorSetsRAII.reserve( descriptorSets.size() );
      for ( auto & descriptorSet : descriptorSets )
      {
        descriptorSetsRAII.emplace_back(
          *this, *reinterpret_cast<VkDescriptorSet *>( &descriptorSet ), static_cast<VkDescriptorPool>( allocateInfo.descriptorPool ) );
      }
      return descriptorSetsRAII;
    }

    VULKAN_HPP_INLINE void Device::updateDescriptorSets(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::WriteDescriptorSet> const & descriptorWrites,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CopyDescriptorSet> const &  descriptorCopies ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkUpdateDescriptorSets && "Function <vkUpdateDescriptorSets> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkUpdateDescriptorSets( static_cast<VkDevice>( m_device ),
                                               descriptorWrites.size(),
                                               reinterpret_cast<const VkWriteDescriptorSet *>( descriptorWrites.data() ),
                                               descriptorCopies.size(),
                                               reinterpret_cast<const VkCopyDescriptorSet *>( descriptorCopies.data() ) );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Framebuffer>::Type
                        Device::createFramebuffer( VULKAN_HPP_NAMESPACE::FramebufferCreateInfo const &                             createInfo,
                                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Framebuffer framebuffer;
      VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateFramebuffer(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkFramebufferCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkFramebuffer *>( &framebuffer ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createFramebuffer" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Framebuffer( *this, *reinterpret_cast<VkFramebuffer *>( &framebuffer ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass>::Type
                        Device::createRenderPass( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo const &                              createInfo,
                                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::RenderPass renderPass;
      VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateRenderPass(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkRenderPassCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createRenderPass" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass( *this, *reinterpret_cast<VkRenderPass *>( &renderPass ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Extent2D RenderPass::getRenderAreaGranularity() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetRenderAreaGranularity && "Function <vkGetRenderAreaGranularity> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Extent2D granularity;
      getDispatcher()->vkGetRenderAreaGranularity(
        static_cast<VkDevice>( m_device ), static_cast<VkRenderPass>( m_renderPass ), reinterpret_cast<VkExtent2D *>( &granularity ) );

      return granularity;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandPool>::Type
                        Device::createCommandPool( VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo const &                             createInfo,
                                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::CommandPool commandPool;
      VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateCommandPool(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkCommandPoolCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkCommandPool *>( &commandPool ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createCommandPool" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandPool( *this, *reinterpret_cast<VkCommandPool *>( &commandPool ), allocator );
    }

    VULKAN_HPP_INLINE void CommandPool::reset( VULKAN_HPP_NAMESPACE::CommandPoolResetFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetCommandPool && "Function <vkResetCommandPool> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkResetCommandPool(
        static_cast<VkDevice>( m_device ), static_cast<VkCommandPool>( m_commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandPool::reset" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer>>::Type
      Device::allocateCommandBuffers( VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo const & allocateInfo ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer> commandBuffers( allocateInfo.commandBufferCount );
      VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkAllocateCommandBuffers( static_cast<VkDevice>( m_device ),
                                                   reinterpret_cast<const VkCommandBufferAllocateInfo *>( &allocateInfo ),
                                                   reinterpret_cast<VkCommandBuffer *>( commandBuffers.data() ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::allocateCommandBuffers" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CommandBuffer> commandBuffersRAII;
      commandBuffersRAII.reserve( commandBuffers.size() );
      for ( auto & commandBuffer : commandBuffers )
      {
        commandBuffersRAII.emplace_back(
          *this, *reinterpret_cast<VkCommandBuffer *>( &commandBuffer ), static_cast<VkCommandPool>( allocateInfo.commandPool ) );
      }
      return commandBuffersRAII;
    }

    VULKAN_HPP_INLINE void CommandBuffer::begin( const VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo & beginInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkBeginCommandBuffer && "Function <vkBeginCommandBuffer> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkBeginCommandBuffer(
        static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkCommandBufferBeginInfo *>( &beginInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::begin" );
    }

    VULKAN_HPP_INLINE void CommandBuffer::end() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEndCommandBuffer && "Function <vkEndCommandBuffer> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEndCommandBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::end" );
    }

    VULKAN_HPP_INLINE void CommandBuffer::reset( VULKAN_HPP_NAMESPACE::CommandBufferResetFlags flags ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetCommandBuffer && "Function <vkResetCommandBuffer> requires <VK_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkResetCommandBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkCommandBufferResetFlags>( flags ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::reset" );
    }

    VULKAN_HPP_INLINE void CommandBuffer::bindPipeline( VULKAN_HPP_NAMESPACE::PipelineBindPoint pipelineBindPoint,
                                                        VULKAN_HPP_NAMESPACE::Pipeline          pipeline ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindPipeline && "Function <vkCmdBindPipeline> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBindPipeline(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipeline>( pipeline ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setViewport( uint32_t                                                                       firstViewport,
                                  VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetViewport && "Function <vkCmdSetViewport> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetViewport(
        static_cast<VkCommandBuffer>( m_commandBuffer ), firstViewport, viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setScissor( uint32_t                                                                     firstScissor,
                                 VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetScissor && "Function <vkCmdSetScissor> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetScissor(
        static_cast<VkCommandBuffer>( m_commandBuffer ), firstScissor, scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setLineWidth( float lineWidth ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetLineWidth && "Function <vkCmdSetLineWidth> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetLineWidth( static_cast<VkCommandBuffer>( m_commandBuffer ), lineWidth );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBias && "Function <vkCmdSetDepthBias> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetDepthBias( static_cast<VkCommandBuffer>( m_commandBuffer ), depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setBlendConstants( const float blendConstants[4] ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetBlendConstants && "Function <vkCmdSetBlendConstants> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetBlendConstants( static_cast<VkCommandBuffer>( m_commandBuffer ), blendConstants );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthBounds( float minDepthBounds, float maxDepthBounds ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBounds && "Function <vkCmdSetDepthBounds> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetDepthBounds( static_cast<VkCommandBuffer>( m_commandBuffer ), minDepthBounds, maxDepthBounds );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilCompareMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                                                                 uint32_t                               compareMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilCompareMask && "Function <vkCmdSetStencilCompareMask> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetStencilCompareMask( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilWriteMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t writeMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilWriteMask && "Function <vkCmdSetStencilWriteMask> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetStencilWriteMask( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilReference( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t reference ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilReference && "Function <vkCmdSetStencilReference> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetStencilReference( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkStencilFaceFlags>( faceMask ), reference );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::bindDescriptorSets( VULKAN_HPP_NAMESPACE::PipelineBindPoint                                             pipelineBindPoint,
                                         VULKAN_HPP_NAMESPACE::PipelineLayout                                                layout,
                                         uint32_t                                                                            firstSet,
                                         VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DescriptorSet> const & descriptorSets,
                                         VULKAN_HPP_NAMESPACE::ArrayProxy<const uint32_t> const & dynamicOffsets ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindDescriptorSets && "Function <vkCmdBindDescriptorSets> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBindDescriptorSets( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                static_cast<VkPipelineBindPoint>( pipelineBindPoint ),
                                                static_cast<VkPipelineLayout>( layout ),
                                                firstSet,
                                                descriptorSets.size(),
                                                reinterpret_cast<const VkDescriptorSet *>( descriptorSets.data() ),
                                                dynamicOffsets.size(),
                                                dynamicOffsets.data() );
    }

    VULKAN_HPP_INLINE void CommandBuffer::bindIndexBuffer( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                           VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                           VULKAN_HPP_NAMESPACE::IndexType  indexType ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindIndexBuffer && "Function <vkCmdBindIndexBuffer> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBindIndexBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                             static_cast<VkBuffer>( buffer ),
                                             static_cast<VkDeviceSize>( offset ),
                                             static_cast<VkIndexType>( indexType ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers( uint32_t                                                                         firstBinding,
                                                             VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                                             VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindVertexBuffers && "Function <vkCmdBindVertexBuffers> requires <VK_VERSION_1_0>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
#  else
      if ( buffers.size() != offsets.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers: buffers.size() != offsets.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkCmdBindVertexBuffers( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               firstBinding,
                                               buffers.size(),
                                               reinterpret_cast<const VkBuffer *>( buffers.data() ),
                                               reinterpret_cast<const VkDeviceSize *>( offsets.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDraw && "Function <vkCmdDraw> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDraw( static_cast<VkCommandBuffer>( m_commandBuffer ), vertexCount, instanceCount, firstVertex, firstInstance );
    }

    VULKAN_HPP_INLINE void CommandBuffer::drawIndexed(
      uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDrawIndexed && "Function <vkCmdDrawIndexed> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDrawIndexed( static_cast<VkCommandBuffer>( m_commandBuffer ), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
    }

    VULKAN_HPP_INLINE void CommandBuffer::drawIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                        VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                        uint32_t                         drawCount,
                                                        uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDrawIndirect && "Function <vkCmdDrawIndirect> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDrawIndirect(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ), drawCount, stride );
    }

    VULKAN_HPP_INLINE void CommandBuffer::drawIndexedIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                               VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                               uint32_t                         drawCount,
                                                               uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDrawIndexedIndirect && "Function <vkCmdDrawIndexedIndirect> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDrawIndexedIndirect(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ), drawCount, stride );
    }

    VULKAN_HPP_INLINE void CommandBuffer::dispatch( uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDispatch && "Function <vkCmdDispatch> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDispatch( static_cast<VkCommandBuffer>( m_commandBuffer ), groupCountX, groupCountY, groupCountZ );
    }

    VULKAN_HPP_INLINE void CommandBuffer::dispatchIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                            VULKAN_HPP_NAMESPACE::DeviceSize offset ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDispatchIndirect && "Function <vkCmdDispatchIndirect> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdDispatchIndirect(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyBuffer( VULKAN_HPP_NAMESPACE::Buffer                                                     srcBuffer,
                                 VULKAN_HPP_NAMESPACE::Buffer                                                     dstBuffer,
                                 VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferCopy> const & regions ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBuffer && "Function <vkCmdCopyBuffer> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdCopyBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                        static_cast<VkBuffer>( srcBuffer ),
                                        static_cast<VkBuffer>( dstBuffer ),
                                        regions.size(),
                                        reinterpret_cast<const VkBufferCopy *>( regions.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                                VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                                VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                                VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                                VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImage && "Function <vkCmdCopyImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdCopyImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                       static_cast<VkImage>( srcImage ),
                                       static_cast<VkImageLayout>( srcImageLayout ),
                                       static_cast<VkImage>( dstImage ),
                                       static_cast<VkImageLayout>( dstImageLayout ),
                                       regions.size(),
                                       reinterpret_cast<const VkImageCopy *>( regions.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::blitImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                                                     VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                                                     VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                                                     VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageBlit> const & regions,
                                                     VULKAN_HPP_NAMESPACE::Filter filter ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBlitImage && "Function <vkCmdBlitImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBlitImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                       static_cast<VkImage>( srcImage ),
                                       static_cast<VkImageLayout>( srcImageLayout ),
                                       static_cast<VkImage>( dstImage ),
                                       static_cast<VkImageLayout>( dstImageLayout ),
                                       regions.size(),
                                       reinterpret_cast<const VkImageBlit *>( regions.data() ),
                                       static_cast<VkFilter>( filter ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage(
      VULKAN_HPP_NAMESPACE::Buffer                                                          srcBuffer,
      VULKAN_HPP_NAMESPACE::Image                                                           dstImage,
      VULKAN_HPP_NAMESPACE::ImageLayout                                                     dstImageLayout,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBufferToImage && "Function <vkCmdCopyBufferToImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdCopyBufferToImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               static_cast<VkBuffer>( srcBuffer ),
                                               static_cast<VkImage>( dstImage ),
                                               static_cast<VkImageLayout>( dstImageLayout ),
                                               regions.size(),
                                               reinterpret_cast<const VkBufferImageCopy *>( regions.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer(
      VULKAN_HPP_NAMESPACE::Image                                                           srcImage,
      VULKAN_HPP_NAMESPACE::ImageLayout                                                     srcImageLayout,
      VULKAN_HPP_NAMESPACE::Buffer                                                          dstBuffer,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImageToBuffer && "Function <vkCmdCopyImageToBuffer> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdCopyImageToBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               static_cast<VkImage>( srcImage ),
                                               static_cast<VkImageLayout>( srcImageLayout ),
                                               static_cast<VkBuffer>( dstBuffer ),
                                               regions.size(),
                                               reinterpret_cast<const VkBufferImageCopy *>( regions.data() ) );
    }

    template <typename DataType>
    VULKAN_HPP_INLINE void CommandBuffer::updateBuffer( VULKAN_HPP_NAMESPACE::Buffer                             dstBuffer,
                                                        VULKAN_HPP_NAMESPACE::DeviceSize                         dstOffset,
                                                        VULKAN_HPP_NAMESPACE::ArrayProxy<const DataType> const & data ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdUpdateBuffer && "Function <vkCmdUpdateBuffer> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdUpdateBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                          static_cast<VkBuffer>( dstBuffer ),
                                          static_cast<VkDeviceSize>( dstOffset ),
                                          data.size() * sizeof( DataType ),
                                          reinterpret_cast<const void *>( data.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::fillBuffer( VULKAN_HPP_NAMESPACE::Buffer     dstBuffer,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize dstOffset,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize size,
                                                      uint32_t                         data ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdFillBuffer && "Function <vkCmdFillBuffer> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdFillBuffer( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                        static_cast<VkBuffer>( dstBuffer ),
                                        static_cast<VkDeviceSize>( dstOffset ),
                                        static_cast<VkDeviceSize>( size ),
                                        data );
    }

    VULKAN_HPP_INLINE void CommandBuffer::clearColorImage(
      VULKAN_HPP_NAMESPACE::Image                                                                 image,
      VULKAN_HPP_NAMESPACE::ImageLayout                                                           imageLayout,
      const VULKAN_HPP_NAMESPACE::ClearColorValue &                                               color,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdClearColorImage && "Function <vkCmdClearColorImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdClearColorImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                             static_cast<VkImage>( image ),
                                             static_cast<VkImageLayout>( imageLayout ),
                                             reinterpret_cast<const VkClearColorValue *>( &color ),
                                             ranges.size(),
                                             reinterpret_cast<const VkImageSubresourceRange *>( ranges.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::clearDepthStencilImage(
      VULKAN_HPP_NAMESPACE::Image                                                                 image,
      VULKAN_HPP_NAMESPACE::ImageLayout                                                           imageLayout,
      const VULKAN_HPP_NAMESPACE::ClearDepthStencilValue &                                        depthStencil,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdClearDepthStencilImage && "Function <vkCmdClearDepthStencilImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdClearDepthStencilImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                    static_cast<VkImage>( image ),
                                                    static_cast<VkImageLayout>( imageLayout ),
                                                    reinterpret_cast<const VkClearDepthStencilValue *>( &depthStencil ),
                                                    ranges.size(),
                                                    reinterpret_cast<const VkImageSubresourceRange *>( ranges.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::clearAttachments( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearAttachment> const & attachments,
                                       VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearRect> const &       rects ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdClearAttachments && "Function <vkCmdClearAttachments> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdClearAttachments( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                              attachments.size(),
                                              reinterpret_cast<const VkClearAttachment *>( attachments.data() ),
                                              rects.size(),
                                              reinterpret_cast<const VkClearRect *>( rects.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::resolveImage( VULKAN_HPP_NAMESPACE::Image                                                        srcImage,
                                   VULKAN_HPP_NAMESPACE::ImageLayout                                                  srcImageLayout,
                                   VULKAN_HPP_NAMESPACE::Image                                                        dstImage,
                                   VULKAN_HPP_NAMESPACE::ImageLayout                                                  dstImageLayout,
                                   VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageResolve> const & regions ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResolveImage && "Function <vkCmdResolveImage> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdResolveImage( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                          static_cast<VkImage>( srcImage ),
                                          static_cast<VkImageLayout>( srcImageLayout ),
                                          static_cast<VkImage>( dstImage ),
                                          static_cast<VkImageLayout>( dstImageLayout ),
                                          regions.size(),
                                          reinterpret_cast<const VkImageResolve *>( regions.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setEvent( VULKAN_HPP_NAMESPACE::Event              event,
                                                    VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetEvent && "Function <vkCmdSetEvent> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdSetEvent(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::resetEvent( VULKAN_HPP_NAMESPACE::Event              event,
                                                      VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResetEvent && "Function <vkCmdResetEvent> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdResetEvent(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::waitEvents(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &               events,
      VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
      VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const &  imageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWaitEvents && "Function <vkCmdWaitEvents> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdWaitEvents( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                        events.size(),
                                        reinterpret_cast<const VkEvent *>( events.data() ),
                                        static_cast<VkPipelineStageFlags>( srcStageMask ),
                                        static_cast<VkPipelineStageFlags>( dstStageMask ),
                                        memoryBarriers.size(),
                                        reinterpret_cast<const VkMemoryBarrier *>( memoryBarriers.data() ),
                                        bufferMemoryBarriers.size(),
                                        reinterpret_cast<const VkBufferMemoryBarrier *>( bufferMemoryBarriers.data() ),
                                        imageMemoryBarriers.size(),
                                        reinterpret_cast<const VkImageMemoryBarrier *>( imageMemoryBarriers.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier(
      VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
      VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
      VULKAN_HPP_NAMESPACE::DependencyFlags                                                     dependencyFlags,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const &  imageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdPipelineBarrier && "Function <vkCmdPipelineBarrier> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdPipelineBarrier( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                             static_cast<VkPipelineStageFlags>( srcStageMask ),
                                             static_cast<VkPipelineStageFlags>( dstStageMask ),
                                             static_cast<VkDependencyFlags>( dependencyFlags ),
                                             memoryBarriers.size(),
                                             reinterpret_cast<const VkMemoryBarrier *>( memoryBarriers.data() ),
                                             bufferMemoryBarriers.size(),
                                             reinterpret_cast<const VkBufferMemoryBarrier *>( bufferMemoryBarriers.data() ),
                                             imageMemoryBarriers.size(),
                                             reinterpret_cast<const VkImageMemoryBarrier *>( imageMemoryBarriers.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::beginQuery( VULKAN_HPP_NAMESPACE::QueryPool         queryPool,
                                                      uint32_t                                query,
                                                      VULKAN_HPP_NAMESPACE::QueryControlFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBeginQuery && "Function <vkCmdBeginQuery> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBeginQuery(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkQueryPool>( queryPool ), query, static_cast<VkQueryControlFlags>( flags ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::endQuery( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdEndQuery && "Function <vkCmdEndQuery> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdEndQuery( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkQueryPool>( queryPool ), query );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::resetQueryPool( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResetQueryPool && "Function <vkCmdResetQueryPool> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdResetQueryPool( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
    }

    VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp( VULKAN_HPP_NAMESPACE::PipelineStageFlagBits pipelineStage,
                                                          VULKAN_HPP_NAMESPACE::QueryPool             queryPool,
                                                          uint32_t                                    query ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWriteTimestamp && "Function <vkCmdWriteTimestamp> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdWriteTimestamp(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPipelineStageFlagBits>( pipelineStage ), static_cast<VkQueryPool>( queryPool ), query );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyQueryPoolResults( VULKAN_HPP_NAMESPACE::QueryPool        queryPool,
                                                                uint32_t                               firstQuery,
                                                                uint32_t                               queryCount,
                                                                VULKAN_HPP_NAMESPACE::Buffer           dstBuffer,
                                                                VULKAN_HPP_NAMESPACE::DeviceSize       dstOffset,
                                                                VULKAN_HPP_NAMESPACE::DeviceSize       stride,
                                                                VULKAN_HPP_NAMESPACE::QueryResultFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyQueryPoolResults && "Function <vkCmdCopyQueryPoolResults> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdCopyQueryPoolResults( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                  static_cast<VkQueryPool>( queryPool ),
                                                  firstQuery,
                                                  queryCount,
                                                  static_cast<VkBuffer>( dstBuffer ),
                                                  static_cast<VkDeviceSize>( dstOffset ),
                                                  static_cast<VkDeviceSize>( stride ),
                                                  static_cast<VkQueryResultFlags>( flags ) );
    }

    template <typename ValuesType>
    VULKAN_HPP_INLINE void CommandBuffer::pushConstants( VULKAN_HPP_NAMESPACE::PipelineLayout                       layout,
                                                         VULKAN_HPP_NAMESPACE::ShaderStageFlags                     stageFlags,
                                                         uint32_t                                                   offset,
                                                         VULKAN_HPP_NAMESPACE::ArrayProxy<const ValuesType> const & values ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdPushConstants && "Function <vkCmdPushConstants> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdPushConstants( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                           static_cast<VkPipelineLayout>( layout ),
                                           static_cast<VkShaderStageFlags>( stageFlags ),
                                           offset,
                                           values.size() * sizeof( ValuesType ),
                                           reinterpret_cast<const void *>( values.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                                                           VULKAN_HPP_NAMESPACE::SubpassContents             contents ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBeginRenderPass && "Function <vkCmdBeginRenderPass> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdBeginRenderPass( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                             reinterpret_cast<const VkRenderPassBeginInfo *>( &renderPassBegin ),
                                             static_cast<VkSubpassContents>( contents ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::nextSubpass( VULKAN_HPP_NAMESPACE::SubpassContents contents ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdNextSubpass && "Function <vkCmdNextSubpass> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdNextSubpass( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkSubpassContents>( contents ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::endRenderPass() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdEndRenderPass && "Function <vkCmdEndRenderPass> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdEndRenderPass( static_cast<VkCommandBuffer>( m_commandBuffer ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::executeCommands(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CommandBuffer> const & commandBuffers ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdExecuteCommands && "Function <vkCmdExecuteCommands> requires <VK_VERSION_1_0>" );

      getDispatcher()->vkCmdExecuteCommands(
        static_cast<VkCommandBuffer>( m_commandBuffer ), commandBuffers.size(), reinterpret_cast<const VkCommandBuffer *>( commandBuffers.data() ) );
    }

    //=== VK_VERSION_1_1 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint32_t Context::enumerateInstanceVersion() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumerateInstanceVersion && "Function <vkEnumerateInstanceVersion> requires <VK_VERSION_1_1>" );

      uint32_t                     apiVersion;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumerateInstanceVersion( &apiVersion ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Context::enumerateInstanceVersion" );

      return apiVersion;
    }

    VULKAN_HPP_INLINE void
      Device::bindBufferMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo> const & bindInfos ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkBindBufferMemory2 && "Function <vkBindBufferMemory2> requires <VK_KHR_bind_memory2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkBindBufferMemory2(
        static_cast<VkDevice>( m_device ), bindInfos.size(), reinterpret_cast<const VkBindBufferMemoryInfo *>( bindInfos.data() ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindBufferMemory2" );
    }

    VULKAN_HPP_INLINE void Device::bindImageMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindImageMemoryInfo> const & bindInfos ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkBindImageMemory2 && "Function <vkBindImageMemory2> requires <VK_KHR_bind_memory2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkBindImageMemory2(
        static_cast<VkDevice>( m_device ), bindInfos.size(), reinterpret_cast<const VkBindImageMemoryInfo *>( bindInfos.data() ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindImageMemory2" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags
      Device::getGroupPeerMemoryFeatures( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceGroupPeerMemoryFeatures &&
                         "Function <vkGetDeviceGroupPeerMemoryFeatures> requires <VK_KHR_device_group> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags peerMemoryFeatures;
      getDispatcher()->vkGetDeviceGroupPeerMemoryFeatures( static_cast<VkDevice>( m_device ),
                                                           heapIndex,
                                                           localDeviceIndex,
                                                           remoteDeviceIndex,
                                                           reinterpret_cast<VkPeerMemoryFeatureFlags *>( &peerMemoryFeatures ) );

      return peerMemoryFeatures;
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDeviceMask( uint32_t deviceMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDeviceMask && "Function <vkCmdSetDeviceMask> requires <VK_KHR_device_group> or <VK_VERSION_1_1>" );

      getDispatcher()->vkCmdSetDeviceMask( static_cast<VkCommandBuffer>( m_commandBuffer ), deviceMask );
    }

    VULKAN_HPP_INLINE void CommandBuffer::dispatchBase( uint32_t baseGroupX,
                                                        uint32_t baseGroupY,
                                                        uint32_t baseGroupZ,
                                                        uint32_t groupCountX,
                                                        uint32_t groupCountY,
                                                        uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDispatchBase && "Function <vkCmdDispatchBase> requires <VK_KHR_device_group> or <VK_VERSION_1_1>" );

      getDispatcher()->vkCmdDispatchBase(
        static_cast<VkCommandBuffer>( m_commandBuffer ), baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties> Instance::enumeratePhysicalDeviceGroups() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumeratePhysicalDeviceGroups &&
                         "Function <vkEnumeratePhysicalDeviceGroups> requires <VK_KHR_device_group_creation> or <VK_VERSION_1_1>" );

      std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties> physicalDeviceGroupProperties;
      uint32_t                                                         physicalDeviceGroupCount;
      VULKAN_HPP_NAMESPACE::Result                                     result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkEnumeratePhysicalDeviceGroups( static_cast<VkInstance>( m_instance ), &physicalDeviceGroupCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceGroupCount )
        {
          physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkEnumeratePhysicalDeviceGroups( static_cast<VkInstance>( m_instance ),
                                                              &physicalDeviceGroupCount,
                                                              reinterpret_cast<VkPhysicalDeviceGroupProperties *>( physicalDeviceGroupProperties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::enumeratePhysicalDeviceGroups" );
      VULKAN_HPP_ASSERT( physicalDeviceGroupCount <= physicalDeviceGroupProperties.size() );
      if ( physicalDeviceGroupCount < physicalDeviceGroupProperties.size() )
      {
        physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
      }
      return physicalDeviceGroupProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
      Device::getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetImageMemoryRequirements2 &&
                         "Function <vkGetImageMemoryRequirements2> requires <VK_KHR_get_memory_requirements2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
      getDispatcher()->vkGetImageMemoryRequirements2( static_cast<VkDevice>( m_device ),
                                                      reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>( &info ),
                                                      reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      Device::getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetImageMemoryRequirements2 &&
                         "Function <vkGetImageMemoryRequirements2> requires <VK_KHR_get_memory_requirements2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
      getDispatcher()->vkGetImageMemoryRequirements2( static_cast<VkDevice>( m_device ),
                                                      reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>( &info ),
                                                      reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
      Device::getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetBufferMemoryRequirements2 &&
                         "Function <vkGetBufferMemoryRequirements2> requires <VK_KHR_get_memory_requirements2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
      getDispatcher()->vkGetBufferMemoryRequirements2( static_cast<VkDevice>( m_device ),
                                                       reinterpret_cast<const VkBufferMemoryRequirementsInfo2 *>( &info ),
                                                       reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      Device::getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetBufferMemoryRequirements2 &&
                         "Function <vkGetBufferMemoryRequirements2> requires <VK_KHR_get_memory_requirements2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
      getDispatcher()->vkGetBufferMemoryRequirements2( static_cast<VkDevice>( m_device ),
                                                       reinterpret_cast<const VkBufferMemoryRequirementsInfo2 *>( &info ),
                                                       reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 PhysicalDevice::getFeatures2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFeatures2 &&
                         "Function <vkGetPhysicalDeviceFeatures2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 features;
      getDispatcher()->vkGetPhysicalDeviceFeatures2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                     reinterpret_cast<VkPhysicalDeviceFeatures2 *>( &features ) );

      return features;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> PhysicalDevice::getFeatures2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFeatures2 &&
                         "Function <vkGetPhysicalDeviceFeatures2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 &  features = structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2>();
      getDispatcher()->vkGetPhysicalDeviceFeatures2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                     reinterpret_cast<VkPhysicalDeviceFeatures2 *>( &features ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 PhysicalDevice::getProperties2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceProperties2 &&
                         "Function <vkGetPhysicalDeviceProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 properties;
      getDispatcher()->vkGetPhysicalDeviceProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                       reinterpret_cast<VkPhysicalDeviceProperties2 *>( &properties ) );

      return properties;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> PhysicalDevice::getProperties2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceProperties2 &&
                         "Function <vkGetPhysicalDeviceProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>  structureChain;
      VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 & properties = structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2>();
      getDispatcher()->vkGetPhysicalDeviceProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                       reinterpret_cast<VkPhysicalDeviceProperties2 *>( &properties ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::FormatProperties2
                                           PhysicalDevice::getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFormatProperties2 &&
                         "Function <vkGetPhysicalDeviceFormatProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::FormatProperties2 formatProperties;
      getDispatcher()->vkGetPhysicalDeviceFormatProperties2(
        static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2 *>( &formatProperties ) );

      return formatProperties;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                           PhysicalDevice::getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFormatProperties2 &&
                         "Function <vkGetPhysicalDeviceFormatProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::FormatProperties2 &        formatProperties = structureChain.template get<VULKAN_HPP_NAMESPACE::FormatProperties2>();
      getDispatcher()->vkGetPhysicalDeviceFormatProperties2(
        static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2 *>( &formatProperties ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ImageFormatProperties2
      PhysicalDevice::getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceImageFormatProperties2 &&
                         "Function <vkGetPhysicalDeviceImageFormatProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::ImageFormatProperties2 imageFormatProperties;
      VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceImageFormatProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2 *>( &imageFormatInfo ),
                                                                    reinterpret_cast<VkImageFormatProperties2 *>( &imageFormatProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties2" );

      return imageFormatProperties;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      PhysicalDevice::getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceImageFormatProperties2 &&
                         "Function <vkGetPhysicalDeviceImageFormatProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      StructureChain<X, Y, Z...>                     structureChain;
      VULKAN_HPP_NAMESPACE::ImageFormatProperties2 & imageFormatProperties = structureChain.template get<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>();
      VULKAN_HPP_NAMESPACE::Result                   result                = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceImageFormatProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2 *>( &imageFormatInfo ),
                                                                    reinterpret_cast<VkImageFormatProperties2 *>( &imageFormatProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties2" );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> PhysicalDevice::getQueueFamilyProperties2() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2 &&
                         "Function <vkGetPhysicalDeviceQueueFamilyProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> queueFamilyProperties;
      uint32_t                                                  queueFamilyPropertyCount;
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ), &queueFamilyPropertyCount, nullptr );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                  &queueFamilyPropertyCount,
                                                                  reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

      VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
      if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
      {
        queueFamilyProperties.resize( queueFamilyPropertyCount );
      }
      return queueFamilyProperties;
    }

    template <typename StructureChain>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain> PhysicalDevice::getQueueFamilyProperties2() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2 &&
                         "Function <vkGetPhysicalDeviceQueueFamilyProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      std::vector<StructureChain>                               structureChains;
      std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> queueFamilyProperties;
      uint32_t                                                  queueFamilyPropertyCount;
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ), &queueFamilyPropertyCount, nullptr );
      structureChains.resize( queueFamilyPropertyCount );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
      {
        queueFamilyProperties[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>().pNext;
      }
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                  &queueFamilyPropertyCount,
                                                                  reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

      VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
      if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
      {
        structureChains.resize( queueFamilyPropertyCount );
      }
      for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
      {
        structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>() = queueFamilyProperties[i];
      }
      return structureChains;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2
                                           PhysicalDevice::getMemoryProperties2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceMemoryProperties2 &&
                         "Function <vkGetPhysicalDeviceMemoryProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 memoryProperties;
      getDispatcher()->vkGetPhysicalDeviceMemoryProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                             reinterpret_cast<VkPhysicalDeviceMemoryProperties2 *>( &memoryProperties ) );

      return memoryProperties;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> PhysicalDevice::getMemoryProperties2() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceMemoryProperties2 &&
                         "Function <vkGetPhysicalDeviceMemoryProperties2> requires <VK_KHR_get_physical_device_properties2> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>        structureChain;
      VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 & memoryProperties =
        structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2>();
      getDispatcher()->vkGetPhysicalDeviceMemoryProperties2( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                             reinterpret_cast<VkPhysicalDeviceMemoryProperties2 *>( &memoryProperties ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue>::Type
                        Device::getQueue2( VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 const & queueInfo ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Queue queue;
      getDispatcher()->vkGetDeviceQueue2(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkDeviceQueueInfo2 *>( &queueInfo ), reinterpret_cast<VkQueue *>( &queue ) );

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Queue( *this, *reinterpret_cast<VkQueue *>( &queue ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SamplerYcbcrConversion>::Type
      Device::createSamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo const &                  createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion ycbcrConversion;
      VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSamplerYcbcrConversion(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkSamplerYcbcrConversionCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSamplerYcbcrConversion *>( &ycbcrConversion ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSamplerYcbcrConversion" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SamplerYcbcrConversion(
        *this, *reinterpret_cast<VkSamplerYcbcrConversion *>( &ycbcrConversion ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalBufferProperties
      PhysicalDevice::getExternalBufferProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo & externalBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceExternalBufferProperties &&
                         "Function <vkGetPhysicalDeviceExternalBufferProperties> requires <VK_KHR_external_memory_capabilities> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::ExternalBufferProperties externalBufferProperties;
      getDispatcher()->vkGetPhysicalDeviceExternalBufferProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    reinterpret_cast<const VkPhysicalDeviceExternalBufferInfo *>( &externalBufferInfo ),
                                                                    reinterpret_cast<VkExternalBufferProperties *>( &externalBufferProperties ) );

      return externalBufferProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalFenceProperties
      PhysicalDevice::getExternalFenceProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo & externalFenceInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceExternalFenceProperties &&
                         "Function <vkGetPhysicalDeviceExternalFenceProperties> requires <VK_KHR_external_fence_capabilities> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::ExternalFenceProperties externalFenceProperties;
      getDispatcher()->vkGetPhysicalDeviceExternalFenceProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                   reinterpret_cast<const VkPhysicalDeviceExternalFenceInfo *>( &externalFenceInfo ),
                                                                   reinterpret_cast<VkExternalFenceProperties *>( &externalFenceProperties ) );

      return externalFenceProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties PhysicalDevice::getExternalSemaphoreProperties(
      const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo & externalSemaphoreInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceExternalSemaphoreProperties &&
                         "Function <vkGetPhysicalDeviceExternalSemaphoreProperties> requires <VK_KHR_external_semaphore_capabilities> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties externalSemaphoreProperties;
      getDispatcher()->vkGetPhysicalDeviceExternalSemaphoreProperties(
        static_cast<VkPhysicalDevice>( m_physicalDevice ),
        reinterpret_cast<const VkPhysicalDeviceExternalSemaphoreInfo *>( &externalSemaphoreInfo ),
        reinterpret_cast<VkExternalSemaphoreProperties *>( &externalSemaphoreProperties ) );

      return externalSemaphoreProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport
      Device::getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDescriptorSetLayoutSupport &&
                         "Function <vkGetDescriptorSetLayoutSupport> requires <VK_KHR_maintenance3> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport support;
      getDispatcher()->vkGetDescriptorSetLayoutSupport( static_cast<VkDevice>( m_device ),
                                                        reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ),
                                                        reinterpret_cast<VkDescriptorSetLayoutSupport *>( &support ) );

      return support;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      Device::getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDescriptorSetLayoutSupport &&
                         "Function <vkGetDescriptorSetLayoutSupport> requires <VK_KHR_maintenance3> or <VK_VERSION_1_1>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>   structureChain;
      VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport & support = structureChain.template get<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport>();
      getDispatcher()->vkGetDescriptorSetLayoutSupport( static_cast<VkDevice>( m_device ),
                                                        reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ),
                                                        reinterpret_cast<VkDescriptorSetLayoutSupport *>( &support ) );

      return structureChain;
    }

    //=== VK_VERSION_1_2 ===

    VULKAN_HPP_INLINE void CommandBuffer::drawIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                             VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                             VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                                                             VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                                                             uint32_t                         maxDrawCount,
                                                             uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDrawIndirectCount &&
                         "Function <vkCmdDrawIndirectCount> requires <VK_AMD_draw_indirect_count> or <VK_KHR_draw_indirect_count> or <VK_VERSION_1_2>" );

      getDispatcher()->vkCmdDrawIndirectCount( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               static_cast<VkBuffer>( buffer ),
                                               static_cast<VkDeviceSize>( offset ),
                                               static_cast<VkBuffer>( countBuffer ),
                                               static_cast<VkDeviceSize>( countBufferOffset ),
                                               maxDrawCount,
                                               stride );
    }

    VULKAN_HPP_INLINE void CommandBuffer::drawIndexedIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                                    VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                                    VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                                                                    VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                                                                    uint32_t                         maxDrawCount,
                                                                    uint32_t                         stride ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdDrawIndexedIndirectCount &&
                         "Function <vkCmdDrawIndexedIndirectCount> requires <VK_AMD_draw_indirect_count> or <VK_KHR_draw_indirect_count> or <VK_VERSION_1_2>" );

      getDispatcher()->vkCmdDrawIndexedIndirectCount( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                      static_cast<VkBuffer>( buffer ),
                                                      static_cast<VkDeviceSize>( offset ),
                                                      static_cast<VkBuffer>( countBuffer ),
                                                      static_cast<VkDeviceSize>( countBufferOffset ),
                                                      maxDrawCount,
                                                      stride );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass>::Type
                        Device::createRenderPass2( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 const &                             createInfo,
                                 VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::RenderPass renderPass;
      VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateRenderPass2(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkRenderPassCreateInfo2 *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createRenderPass2" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::RenderPass( *this, *reinterpret_cast<VkRenderPass *>( &renderPass ), allocator );
    }

    VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                                                            const VULKAN_HPP_NAMESPACE::SubpassBeginInfo &    subpassBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBeginRenderPass2 &&
                         "Function <vkCmdBeginRenderPass2> requires <VK_KHR_create_renderpass2> or <VK_VERSION_1_2>" );

      getDispatcher()->vkCmdBeginRenderPass2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                              reinterpret_cast<const VkRenderPassBeginInfo *>( &renderPassBegin ),
                                              reinterpret_cast<const VkSubpassBeginInfo *>( &subpassBeginInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::nextSubpass2( const VULKAN_HPP_NAMESPACE::SubpassBeginInfo & subpassBeginInfo,
                                                        const VULKAN_HPP_NAMESPACE::SubpassEndInfo &   subpassEndInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdNextSubpass2 && "Function <vkCmdNextSubpass2> requires <VK_KHR_create_renderpass2> or <VK_VERSION_1_2>" );

      getDispatcher()->vkCmdNextSubpass2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                          reinterpret_cast<const VkSubpassBeginInfo *>( &subpassBeginInfo ),
                                          reinterpret_cast<const VkSubpassEndInfo *>( &subpassEndInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::endRenderPass2( const VULKAN_HPP_NAMESPACE::SubpassEndInfo & subpassEndInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdEndRenderPass2 && "Function <vkCmdEndRenderPass2> requires <VK_KHR_create_renderpass2> or <VK_VERSION_1_2>" );

      getDispatcher()->vkCmdEndRenderPass2( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkSubpassEndInfo *>( &subpassEndInfo ) );
    }

    VULKAN_HPP_INLINE void QueryPool::reset( uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkResetQueryPool && "Function <vkResetQueryPool> requires <VK_EXT_host_query_reset> or <VK_VERSION_1_2>" );

      getDispatcher()->vkResetQueryPool( static_cast<VkDevice>( m_device ), static_cast<VkQueryPool>( m_queryPool ), firstQuery, queryCount );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t Semaphore::getCounterValue() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSemaphoreCounterValue &&
                         "Function <vkGetSemaphoreCounterValue> requires <VK_KHR_timeline_semaphore> or <VK_VERSION_1_2>" );

      uint64_t                     value;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetSemaphoreCounterValue( static_cast<VkDevice>( m_device ), static_cast<VkSemaphore>( m_semaphore ), &value ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Semaphore::getCounterValue" );

      return value;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Device::waitSemaphores( const VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo & waitInfo,
                                                                                                uint64_t                                        timeout ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkWaitSemaphores && "Function <vkWaitSemaphores> requires <VK_KHR_timeline_semaphore> or <VK_VERSION_1_2>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkWaitSemaphores( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkSemaphoreWaitInfo *>( &waitInfo ), timeout ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitSemaphores", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eTimeout } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    VULKAN_HPP_INLINE void Device::signalSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo & signalInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSignalSemaphore && "Function <vkSignalSemaphore> requires <VK_KHR_timeline_semaphore> or <VK_VERSION_1_2>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkSignalSemaphore( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkSemaphoreSignalInfo *>( &signalInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::signalSemaphore" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceAddress
                                           Device::getBufferAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetBufferDeviceAddress &&
                         "Function <vkGetBufferDeviceAddress> requires <VK_EXT_buffer_device_address> or <VK_KHR_buffer_device_address> or <VK_VERSION_1_2>" );

      VkDeviceAddress result =
        getDispatcher()->vkGetBufferDeviceAddress( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkBufferDeviceAddressInfo *>( &info ) );

      return static_cast<VULKAN_HPP_NAMESPACE::DeviceAddress>( result );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t
      Device::getBufferOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetBufferOpaqueCaptureAddress &&
                         "Function <vkGetBufferOpaqueCaptureAddress> requires <VK_KHR_buffer_device_address> or <VK_VERSION_1_2>" );

      uint64_t result =
        getDispatcher()->vkGetBufferOpaqueCaptureAddress( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkBufferDeviceAddressInfo *>( &info ) );

      return result;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t
      Device::getMemoryOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceMemoryOpaqueCaptureAddress &&
                         "Function <vkGetDeviceMemoryOpaqueCaptureAddress> requires <VK_KHR_buffer_device_address> or <VK_VERSION_1_2>" );

      uint64_t result = getDispatcher()->vkGetDeviceMemoryOpaqueCaptureAddress( static_cast<VkDevice>( m_device ),
                                                                                reinterpret_cast<const VkDeviceMemoryOpaqueCaptureAddressInfo *>( &info ) );

      return result;
    }

    //=== VK_VERSION_1_3 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties> PhysicalDevice::getToolProperties() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceToolProperties &&
                         "Function <vkGetPhysicalDeviceToolProperties> requires <VK_EXT_tooling_info> or <VK_VERSION_1_3>" );

      std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties> toolProperties;
      uint32_t                                                        toolCount;
      VULKAN_HPP_NAMESPACE::Result                                    result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceToolProperties( static_cast<VkPhysicalDevice>( m_physicalDevice ), &toolCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && toolCount )
        {
          toolProperties.resize( toolCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceToolProperties(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &toolCount, reinterpret_cast<VkPhysicalDeviceToolProperties *>( toolProperties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getToolProperties" );
      VULKAN_HPP_ASSERT( toolCount <= toolProperties.size() );
      if ( toolCount < toolProperties.size() )
      {
        toolProperties.resize( toolCount );
      }
      return toolProperties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PrivateDataSlot>::Type
      Device::createPrivateDataSlot( VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo const &                         createInfo,
                                     VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot;
      VULKAN_HPP_NAMESPACE::Result          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreatePrivateDataSlot(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkPrivateDataSlotCreateInfo *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkPrivateDataSlot *>( &privateDataSlot ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createPrivateDataSlot" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::PrivateDataSlot( *this, *reinterpret_cast<VkPrivateDataSlot *>( &privateDataSlot ), allocator );
    }

    VULKAN_HPP_INLINE void Device::setPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                   uint64_t                              objectHandle,
                                                   VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                                                   uint64_t                              data ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSetPrivateData && "Function <vkSetPrivateData> requires <VK_EXT_private_data> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkSetPrivateData(
        static_cast<VkDevice>( m_device ), static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), data ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setPrivateData" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t Device::getPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                                            uint64_t                              objectHandle,
                                                                            VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPrivateData && "Function <vkGetPrivateData> requires <VK_EXT_private_data> or <VK_VERSION_1_3>" );

      uint64_t data;
      getDispatcher()->vkGetPrivateData(
        static_cast<VkDevice>( m_device ), static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), &data );

      return data;
    }

    VULKAN_HPP_INLINE void CommandBuffer::setEvent2( VULKAN_HPP_NAMESPACE::Event                  event,
                                                     const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetEvent2 && "Function <vkCmdSetEvent2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetEvent2(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::resetEvent2( VULKAN_HPP_NAMESPACE::Event               event,
                                                       VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResetEvent2 && "Function <vkCmdResetEvent2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdResetEvent2(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags2>( stageMask ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::waitEvents2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                                  VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWaitEvents2 && "Function <vkCmdWaitEvents2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( events.size() == dependencyInfos.size() );
#  else
      if ( events.size() != dependencyInfos.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::waitEvents2: events.size() != dependencyInfos.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkCmdWaitEvents2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                         events.size(),
                                         reinterpret_cast<const VkEvent *>( events.data() ),
                                         reinterpret_cast<const VkDependencyInfo *>( dependencyInfos.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdPipelineBarrier2 && "Function <vkCmdPipelineBarrier2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdPipelineBarrier2( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp2( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                           VULKAN_HPP_NAMESPACE::QueryPool           queryPool,
                                                           uint32_t                                  query ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWriteTimestamp2 && "Function <vkCmdWriteTimestamp2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdWriteTimestamp2(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPipelineStageFlags2>( stage ), static_cast<VkQueryPool>( queryPool ), query );
    }

    VULKAN_HPP_INLINE void Queue::submit2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits,
                                           VULKAN_HPP_NAMESPACE::Fence                                                       fence ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueSubmit2 && "Function <vkQueueSubmit2> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkQueueSubmit2(
        static_cast<VkQueue>( m_queue ), submits.size(), reinterpret_cast<const VkSubmitInfo2 *>( submits.data() ), static_cast<VkFence>( fence ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit2" );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBuffer2 && "Function <vkCmdCopyBuffer2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyBuffer2( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkCopyBufferInfo2 *>( &copyBufferInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyImage2( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImage2 && "Function <vkCmdCopyImage2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyImage2( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkCopyImageInfo2 *>( &copyImageInfo ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyBufferToImage2( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBufferToImage2 &&
                         "Function <vkCmdCopyBufferToImage2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyBufferToImage2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                reinterpret_cast<const VkCopyBufferToImageInfo2 *>( &copyBufferToImageInfo ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyImageToBuffer2( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImageToBuffer2 &&
                         "Function <vkCmdCopyImageToBuffer2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyImageToBuffer2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                reinterpret_cast<const VkCopyImageToBufferInfo2 *>( &copyImageToBufferInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::blitImage2( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBlitImage2 && "Function <vkCmdBlitImage2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdBlitImage2( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkBlitImageInfo2 *>( &blitImageInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::resolveImage2( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResolveImage2 && "Function <vkCmdResolveImage2> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdResolveImage2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                           reinterpret_cast<const VkResolveImageInfo2 *>( &resolveImageInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::beginRendering( const VULKAN_HPP_NAMESPACE::RenderingInfo & renderingInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBeginRendering && "Function <vkCmdBeginRendering> requires <VK_KHR_dynamic_rendering> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdBeginRendering( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkRenderingInfo *>( &renderingInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::endRendering() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdEndRendering && "Function <vkCmdEndRendering> requires <VK_KHR_dynamic_rendering> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdEndRendering( static_cast<VkCommandBuffer>( m_commandBuffer ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setCullMode( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetCullMode &&
                         "Function <vkCmdSetCullMode> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetCullMode( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkCullModeFlags>( cullMode ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setFrontFace( VULKAN_HPP_NAMESPACE::FrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetFrontFace &&
                         "Function <vkCmdSetFrontFace> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetFrontFace( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkFrontFace>( frontFace ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveTopology( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetPrimitiveTopology &&
                         "Function <vkCmdSetPrimitiveTopology> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetPrimitiveTopology( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPrimitiveTopology>( primitiveTopology ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setViewportWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetViewportWithCount &&
                         "Function <vkCmdSetViewportWithCount> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetViewportWithCount(
        static_cast<VkCommandBuffer>( m_commandBuffer ), viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setScissorWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetScissorWithCount &&
                         "Function <vkCmdSetScissorWithCount> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetScissorWithCount(
        static_cast<VkCommandBuffer>( m_commandBuffer ), scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers2( uint32_t                                                                         firstBinding,
                                                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets,
                                                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes,
                                                              VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindVertexBuffers2 &&
                         "Function <vkCmdBindVertexBuffers2> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
      VULKAN_HPP_ASSERT( sizes.empty() || buffers.size() == sizes.size() );
      VULKAN_HPP_ASSERT( strides.empty() || buffers.size() == strides.size() );
#  else
      if ( buffers.size() != offsets.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != offsets.size()" );
      }
      if ( !sizes.empty() && buffers.size() != sizes.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != sizes.size()" );
      }
      if ( !strides.empty() && buffers.size() != strides.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != strides.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkCmdBindVertexBuffers2( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                firstBinding,
                                                buffers.size(),
                                                reinterpret_cast<const VkBuffer *>( buffers.data() ),
                                                reinterpret_cast<const VkDeviceSize *>( offsets.data() ),
                                                reinterpret_cast<const VkDeviceSize *>( sizes.data() ),
                                                reinterpret_cast<const VkDeviceSize *>( strides.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthTestEnable &&
                         "Function <vkCmdSetDepthTestEnable> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthTestEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthWriteEnable( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthWriteEnable &&
                         "Function <vkCmdSetDepthWriteEnable> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthWriteEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthWriteEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthCompareOp( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthCompareOp &&
                         "Function <vkCmdSetDepthCompareOp> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthCompareOp( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkCompareOp>( depthCompareOp ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthBoundsTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBoundsTestEnable &&
                         "Function <vkCmdSetDepthBoundsTestEnable> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthBoundsTestEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthBoundsTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilTestEnable( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilTestEnable &&
                         "Function <vkCmdSetStencilTestEnable> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetStencilTestEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( stencilTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilOp( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                                                        VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                                                        VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                                                        VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                                                        VULKAN_HPP_NAMESPACE::CompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilOp &&
                         "Function <vkCmdSetStencilOp> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetStencilOp( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                          static_cast<VkStencilFaceFlags>( faceMask ),
                                          static_cast<VkStencilOp>( failOp ),
                                          static_cast<VkStencilOp>( passOp ),
                                          static_cast<VkStencilOp>( depthFailOp ),
                                          static_cast<VkCompareOp>( compareOp ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setRasterizerDiscardEnable( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetRasterizerDiscardEnable &&
                         "Function <vkCmdSetRasterizerDiscardEnable> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetRasterizerDiscardEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( rasterizerDiscardEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthBiasEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBiasEnable &&
                         "Function <vkCmdSetDepthBiasEnable> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthBiasEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthBiasEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveRestartEnable( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetPrimitiveRestartEnable &&
                         "Function <vkCmdSetPrimitiveRestartEnable> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetPrimitiveRestartEnable( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( primitiveRestartEnable ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
      Device::getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceBufferMemoryRequirements &&
                         "Function <vkGetDeviceBufferMemoryRequirements> requires <VK_KHR_maintenance4> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
      getDispatcher()->vkGetDeviceBufferMemoryRequirements( static_cast<VkDevice>( m_device ),
                                                            reinterpret_cast<const VkDeviceBufferMemoryRequirements *>( &info ),
                                                            reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      Device::getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceBufferMemoryRequirements &&
                         "Function <vkGetDeviceBufferMemoryRequirements> requires <VK_KHR_maintenance4> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
      getDispatcher()->vkGetDeviceBufferMemoryRequirements( static_cast<VkDevice>( m_device ),
                                                            reinterpret_cast<const VkDeviceBufferMemoryRequirements *>( &info ),
                                                            reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
      Device::getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceImageMemoryRequirements &&
                         "Function <vkGetDeviceImageMemoryRequirements> requires <VK_KHR_maintenance4> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
      getDispatcher()->vkGetDeviceImageMemoryRequirements( static_cast<VkDevice>( m_device ),
                                                           reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ),
                                                           reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return memoryRequirements;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      Device::getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceImageMemoryRequirements &&
                         "Function <vkGetDeviceImageMemoryRequirements> requires <VK_KHR_maintenance4> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
      VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
      getDispatcher()->vkGetDeviceImageMemoryRequirements( static_cast<VkDevice>( m_device ),
                                                           reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ),
                                                           reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2>
                                           Device::getImageSparseMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceImageSparseMemoryRequirements &&
                         "Function <vkGetDeviceImageSparseMemoryRequirements> requires <VK_KHR_maintenance4> or <VK_VERSION_1_3>" );

      std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2> sparseMemoryRequirements;
      uint32_t                                                          sparseMemoryRequirementCount;
      getDispatcher()->vkGetDeviceImageSparseMemoryRequirements(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ), &sparseMemoryRequirementCount, nullptr );
      sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
      getDispatcher()->vkGetDeviceImageSparseMemoryRequirements( static_cast<VkDevice>( m_device ),
                                                                 reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ),
                                                                 &sparseMemoryRequirementCount,
                                                                 reinterpret_cast<VkSparseImageMemoryRequirements2 *>( sparseMemoryRequirements.data() ) );

      VULKAN_HPP_ASSERT( sparseMemoryRequirementCount <= sparseMemoryRequirements.size() );
      if ( sparseMemoryRequirementCount < sparseMemoryRequirements.size() )
      {
        sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
      }
      return sparseMemoryRequirements;
    }

    //=== VKSC_VERSION_1_0 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption
                                           CommandPool::getMemoryConsumption( VULKAN_HPP_NAMESPACE::CommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetCommandPoolMemoryConsumption && "Function <vkGetCommandPoolMemoryConsumption> requires <VKSC_VERSION_1_0>" );

      VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption consumption;
      getDispatcher()->vkGetCommandPoolMemoryConsumption( static_cast<VkDevice>( m_device ),
                                                          static_cast<VkCommandPool>( m_commandPool ),
                                                          static_cast<VkCommandBuffer>( commandBuffer ),
                                                          reinterpret_cast<VkCommandPoolMemoryConsumption *>( &consumption ) );

      return consumption;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData>>
                                           Device::getFaultData( VULKAN_HPP_NAMESPACE::FaultQueryBehavior faultQueryBehavior ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetFaultData && "Function <vkGetFaultData> requires <VKSC_VERSION_1_0>" );

      std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData>> data_;
      VULKAN_HPP_NAMESPACE::Bool32 &                                                        unrecordedFaults = data_.first;
      std::vector<VULKAN_HPP_NAMESPACE::FaultData> &                                        faults           = data_.second;
      uint32_t                                                                              faultCount;
      VULKAN_HPP_NAMESPACE::Result                                                          result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetFaultData( static_cast<VkDevice>( m_device ),
                                                                                             static_cast<VkFaultQueryBehavior>( faultQueryBehavior ),
                                                                                             reinterpret_cast<VkBool32 *>( &unrecordedFaults ),
                                                                                             &faultCount,
                                                                                             nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && faultCount )
        {
          faults.resize( faultCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetFaultData( static_cast<VkDevice>( m_device ),
                                                                                               static_cast<VkFaultQueryBehavior>( faultQueryBehavior ),
                                                                                               reinterpret_cast<VkBool32 *>( &unrecordedFaults ),
                                                                                               &faultCount,
                                                                                               reinterpret_cast<VkFaultData *>( faults.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFaultData" );

      return data_;
    }

    //=== VK_KHR_surface ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Bool32 PhysicalDevice::getSurfaceSupportKHR( uint32_t                         queueFamilyIndex,
                                                                                                              VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceSupportKHR && "Function <vkGetPhysicalDeviceSurfaceSupportKHR> requires <VK_KHR_surface>" );

      VULKAN_HPP_NAMESPACE::Bool32 supported;
      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceSurfaceSupportKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                                                          queueFamilyIndex,
                                                                                                          static_cast<VkSurfaceKHR>( surface ),
                                                                                                          reinterpret_cast<VkBool32 *>( &supported ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceSupportKHR" );

      return supported;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR
                                           PhysicalDevice::getSurfaceCapabilitiesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR &&
                         "Function <vkGetPhysicalDeviceSurfaceCapabilitiesKHR> requires <VK_KHR_surface>" );

      VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR surfaceCapabilities;
      VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    static_cast<VkSurfaceKHR>( surface ),
                                                                    reinterpret_cast<VkSurfaceCapabilitiesKHR *>( &surfaceCapabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilitiesKHR" );

      return surfaceCapabilities;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>
                                           PhysicalDevice::getSurfaceFormatsKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceFormatsKHR && "Function <vkGetPhysicalDeviceSurfaceFormatsKHR> requires <VK_KHR_surface>" );

      std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR> surfaceFormats;
      uint32_t                                            surfaceFormatCount;
      VULKAN_HPP_NAMESPACE::Result                        result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceSurfaceFormatsKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
        {
          surfaceFormats.resize( surfaceFormatCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDeviceSurfaceFormatsKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                   static_cast<VkSurfaceKHR>( surface ),
                                                                   &surfaceFormatCount,
                                                                   reinterpret_cast<VkSurfaceFormatKHR *>( surfaceFormats.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormatsKHR" );
      VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
      if ( surfaceFormatCount < surfaceFormats.size() )
      {
        surfaceFormats.resize( surfaceFormatCount );
      }
      return surfaceFormats;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR>
                                           PhysicalDevice::getSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfacePresentModesKHR &&
                         "Function <vkGetPhysicalDeviceSurfacePresentModesKHR> requires <VK_KHR_surface>" );

      std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR> presentModes;
      uint32_t                                          presentModeCount;
      VULKAN_HPP_NAMESPACE::Result                      result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceSurfacePresentModesKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkSurfaceKHR>( surface ), &presentModeCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && presentModeCount )
        {
          presentModes.resize( presentModeCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDeviceSurfacePresentModesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                        static_cast<VkSurfaceKHR>( surface ),
                                                                        &presentModeCount,
                                                                        reinterpret_cast<VkPresentModeKHR *>( presentModes.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfacePresentModesKHR" );
      VULKAN_HPP_ASSERT( presentModeCount <= presentModes.size() );
      if ( presentModeCount < presentModes.size() )
      {
        presentModes.resize( presentModeCount );
      }
      return presentModes;
    }

    //=== VK_KHR_swapchain ===

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>::Type
                        Device::createSwapchainKHR( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const &                            createInfo,
                                  VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
      VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSwapchainKHR(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSwapchainKHR" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR( *this, *reinterpret_cast<VkSwapchainKHR *>( &swapchain ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::Image> SwapchainKHR::getImages() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSwapchainImagesKHR && "Function <vkGetSwapchainImagesKHR> requires <VK_KHR_swapchain>" );

      std::vector<VULKAN_HPP_NAMESPACE::Image> swapchainImages;
      uint32_t                                 swapchainImageCount;
      VULKAN_HPP_NAMESPACE::Result             result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetSwapchainImagesKHR(
          static_cast<VkDevice>( m_device ), static_cast<VkSwapchainKHR>( m_swapchain ), &swapchainImageCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && swapchainImageCount )
        {
          swapchainImages.resize( swapchainImageCount );
          result =
            static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetSwapchainImagesKHR( static_cast<VkDevice>( m_device ),
                                                                                                 static_cast<VkSwapchainKHR>( m_swapchain ),
                                                                                                 &swapchainImageCount,
                                                                                                 reinterpret_cast<VkImage *>( swapchainImages.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::SwapchainKHR::getImages" );
      VULKAN_HPP_ASSERT( swapchainImageCount <= swapchainImages.size() );
      if ( swapchainImageCount < swapchainImages.size() )
      {
        swapchainImages.resize( swapchainImageCount );
      }
      return swapchainImages;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, uint32_t>
      SwapchainKHR::acquireNextImage( uint64_t timeout, VULKAN_HPP_NAMESPACE::Semaphore semaphore, VULKAN_HPP_NAMESPACE::Fence fence ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkAcquireNextImageKHR && "Function <vkAcquireNextImageKHR> requires <VK_KHR_swapchain>" );

      uint32_t                     imageIndex;
      VULKAN_HPP_NAMESPACE::Result result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkAcquireNextImageKHR( static_cast<VkDevice>( m_device ),
                                                                                           static_cast<VkSwapchainKHR>( m_swapchain ),
                                                                                           timeout,
                                                                                           static_cast<VkSemaphore>( semaphore ),
                                                                                           static_cast<VkFence>( fence ),
                                                                                           &imageIndex ) );
      resultCheck( result,
                   VULKAN_HPP_NAMESPACE_STRING "::SwapchainKHR::acquireNextImage",
                   { VULKAN_HPP_NAMESPACE::Result::eSuccess,
                     VULKAN_HPP_NAMESPACE::Result::eTimeout,
                     VULKAN_HPP_NAMESPACE::Result::eNotReady,
                     VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

      return std::make_pair( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), imageIndex );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Queue::presentKHR( const VULKAN_HPP_NAMESPACE::PresentInfoKHR & presentInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueuePresentKHR && "Function <vkQueuePresentKHR> requires <VK_KHR_swapchain>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkQueuePresentKHR( static_cast<VkQueue>( m_queue ), reinterpret_cast<const VkPresentInfoKHR *>( &presentInfo ) ) );
      resultCheck(
        result, VULKAN_HPP_NAMESPACE_STRING "::Queue::presentKHR", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR Device::getGroupPresentCapabilitiesKHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceGroupPresentCapabilitiesKHR &&
                         "Function <vkGetDeviceGroupPresentCapabilitiesKHR> requires <VK_KHR_device_group> or <VK_KHR_swapchain>" );

      VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR deviceGroupPresentCapabilities;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetDeviceGroupPresentCapabilitiesKHR(
        static_cast<VkDevice>( m_device ), reinterpret_cast<VkDeviceGroupPresentCapabilitiesKHR *>( &deviceGroupPresentCapabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getGroupPresentCapabilitiesKHR" );

      return deviceGroupPresentCapabilities;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR
                                           Device::getGroupSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDeviceGroupSurfacePresentModesKHR &&
                         "Function <vkGetDeviceGroupSurfacePresentModesKHR> requires <VK_KHR_device_group> or <VK_KHR_swapchain>" );

      VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR modes;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetDeviceGroupSurfacePresentModesKHR(
        static_cast<VkDevice>( m_device ), static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkDeviceGroupPresentModeFlagsKHR *>( &modes ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getGroupSurfacePresentModesKHR" );

      return modes;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::Rect2D>
                                           PhysicalDevice::getPresentRectanglesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDevicePresentRectanglesKHR &&
                         "Function <vkGetPhysicalDevicePresentRectanglesKHR> requires <VK_KHR_device_group> or <VK_KHR_swapchain>" );

      std::vector<VULKAN_HPP_NAMESPACE::Rect2D> rects;
      uint32_t                                  rectCount;
      VULKAN_HPP_NAMESPACE::Result              result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDevicePresentRectanglesKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkSurfaceKHR>( surface ), &rectCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && rectCount )
        {
          rects.resize( rectCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDevicePresentRectanglesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                      static_cast<VkSurfaceKHR>( surface ),
                                                                      &rectCount,
                                                                      reinterpret_cast<VkRect2D *>( rects.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getPresentRectanglesKHR" );
      VULKAN_HPP_ASSERT( rectCount <= rects.size() );
      if ( rectCount < rects.size() )
      {
        rects.resize( rectCount );
      }
      return rects;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, uint32_t>
                                           Device::acquireNextImage2KHR( const VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR & acquireInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkAcquireNextImage2KHR && "Function <vkAcquireNextImage2KHR> requires <VK_KHR_device_group> or <VK_KHR_swapchain>" );

      uint32_t                     imageIndex;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkAcquireNextImage2KHR(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkAcquireNextImageInfoKHR *>( &acquireInfo ), &imageIndex ) );
      resultCheck( result,
                   VULKAN_HPP_NAMESPACE_STRING "::Device::acquireNextImage2KHR",
                   { VULKAN_HPP_NAMESPACE::Result::eSuccess,
                     VULKAN_HPP_NAMESPACE::Result::eTimeout,
                     VULKAN_HPP_NAMESPACE::Result::eNotReady,
                     VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

      return std::make_pair( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), imageIndex );
    }

    //=== VK_KHR_display ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR> PhysicalDevice::getDisplayPropertiesKHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceDisplayPropertiesKHR &&
                         "Function <vkGetPhysicalDeviceDisplayPropertiesKHR> requires <VK_KHR_display>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR> properties;
      uint32_t                                                propertyCount;
      VULKAN_HPP_NAMESPACE::Result                            result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceDisplayPropertiesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceDisplayPropertiesKHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPropertiesKHR" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlanePropertiesKHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceDisplayPlanePropertiesKHR &&
                         "Function <vkGetPhysicalDeviceDisplayPlanePropertiesKHR> requires <VK_KHR_display>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR> properties;
      uint32_t                                                     propertyCount;
      VULKAN_HPP_NAMESPACE::Result                                 result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceDisplayPlanePropertiesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlanePropertiesKHR" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR>>::Type
      PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::DisplayKHR> displays;
      uint32_t                                      displayCount;
      VULKAN_HPP_NAMESPACE::Result                  result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetDisplayPlaneSupportedDisplaysKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ), planeIndex, &displayCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && displayCount )
        {
          displays.resize( displayCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetDisplayPlaneSupportedDisplaysKHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR *>( displays.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      if ( ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( result != VULKAN_HPP_NAMESPACE::Result::eIncomplete ) )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR> displaysRAII;
      displaysRAII.reserve( displays.size() );
      for ( auto & display : displays )
      {
        displaysRAII.emplace_back( *this, *reinterpret_cast<VkDisplayKHR *>( &display ) );
      }
      return displaysRAII;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR> DisplayKHR::getModeProperties() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDisplayModePropertiesKHR && "Function <vkGetDisplayModePropertiesKHR> requires <VK_KHR_display>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR> properties;
      uint32_t                                                    propertyCount;
      VULKAN_HPP_NAMESPACE::Result                                result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetDisplayModePropertiesKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkDisplayKHR>( m_display ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetDisplayModePropertiesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                            static_cast<VkDisplayKHR>( m_display ),
                                                            &propertyCount,
                                                            reinterpret_cast<VkDisplayModePropertiesKHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::DisplayKHR::getModeProperties" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayModeKHR>::Type
                        DisplayKHR::createMode( VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR const &                          createInfo,
                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::DisplayModeKHR mode;
      VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDisplayModeKHR(
        static_cast<VkPhysicalDevice>( m_physicalDevice ),
        static_cast<VkDisplayKHR>( m_display ),
        reinterpret_cast<const VkDisplayModeCreateInfoKHR *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDisplayModeKHR *>( &mode ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "DisplayKHR::createMode" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayModeKHR( *this, *reinterpret_cast<VkDisplayModeKHR *>( &mode ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR
                                           DisplayModeKHR::getDisplayPlaneCapabilities( uint32_t planeIndex ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDisplayPlaneCapabilitiesKHR && "Function <vkGetDisplayPlaneCapabilitiesKHR> requires <VK_KHR_display>" );

      VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR capabilities;
      VULKAN_HPP_NAMESPACE::Result                      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetDisplayPlaneCapabilitiesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                           static_cast<VkDisplayModeKHR>( m_displayModeKHR ),
                                                           planeIndex,
                                                           reinterpret_cast<VkDisplayPlaneCapabilitiesKHR *>( &capabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::DisplayModeKHR::getDisplayPlaneCapabilities" );

      return capabilities;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR>::Type
                        Instance::createDisplayPlaneSurfaceKHR( VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR const &                       createInfo,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
      VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDisplayPlaneSurfaceKHR(
        static_cast<VkInstance>( m_instance ),
        reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Instance::createDisplayPlaneSurfaceKHR" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR( *this, *reinterpret_cast<VkSurfaceKHR *>( &surface ), allocator );
    }

    //=== VK_KHR_display_swapchain ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>>::Type
      Device::createSharedSwapchainsKHR( VULKAN_HPP_NAMESPACE::ArrayProxy<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                         VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>        allocator ) const
    {
      std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR> swapchains( createInfos.size() );
      VULKAN_HPP_NAMESPACE::Result                    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSharedSwapchainsKHR(
        static_cast<VkDevice>( m_device ),
        createInfos.size(),
        reinterpret_cast<const VkSwapchainCreateInfoKHR *>( createInfos.data() ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSwapchainKHR *>( swapchains.data() ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSharedSwapchainsKHR" );
#  endif
      }

      std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR> swapchainsRAII;
      swapchainsRAII.reserve( swapchains.size() );
      for ( auto & swapchain : swapchains )
      {
        swapchainsRAII.emplace_back( *this, *reinterpret_cast<VkSwapchainKHR *>( &swapchain ), allocator );
      }
      return swapchainsRAII;
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR>::Type
                        Device::createSharedSwapchainKHR( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR const &                            createInfo,
                                        VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
      VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSharedSwapchainsKHR(
        static_cast<VkDevice>( m_device ),
        1,
        reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSharedSwapchainKHR" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SwapchainKHR( *this, *reinterpret_cast<VkSwapchainKHR *>( &swapchain ), allocator );
    }

    //=== VK_KHR_external_memory_fd ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE int Device::getMemoryFdKHR( const VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR & getFdInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetMemoryFdKHR && "Function <vkGetMemoryFdKHR> requires <VK_KHR_external_memory_fd>" );

      int                          fd;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetMemoryFdKHR( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkMemoryGetFdInfoKHR *>( &getFdInfo ), &fd ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryFdKHR" );

      return fd;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR
                                           Device::getMemoryFdPropertiesKHR( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, int fd ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetMemoryFdPropertiesKHR && "Function <vkGetMemoryFdPropertiesKHR> requires <VK_KHR_external_memory_fd>" );

      VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR memoryFdProperties;
      VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetMemoryFdPropertiesKHR( static_cast<VkDevice>( m_device ),
                                                     static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                     fd,
                                                     reinterpret_cast<VkMemoryFdPropertiesKHR *>( &memoryFdProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryFdPropertiesKHR" );

      return memoryFdProperties;
    }

    //=== VK_KHR_external_semaphore_fd ===

    VULKAN_HPP_INLINE void Device::importSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR & importSemaphoreFdInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkImportSemaphoreFdKHR && "Function <vkImportSemaphoreFdKHR> requires <VK_KHR_external_semaphore_fd>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkImportSemaphoreFdKHR(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkImportSemaphoreFdInfoKHR *>( &importSemaphoreFdInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importSemaphoreFdKHR" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE int Device::getSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR & getFdInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSemaphoreFdKHR && "Function <vkGetSemaphoreFdKHR> requires <VK_KHR_external_semaphore_fd>" );

      int                          fd;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetSemaphoreFdKHR( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkSemaphoreGetFdInfoKHR *>( &getFdInfo ), &fd ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSemaphoreFdKHR" );

      return fd;
    }

    //=== VK_EXT_display_surface_counter ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT
                                           PhysicalDevice::getSurfaceCapabilities2EXT( VULKAN_HPP_NAMESPACE::SurfaceKHR surface ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2EXT &&
                         "Function <vkGetPhysicalDeviceSurfaceCapabilities2EXT> requires <VK_EXT_display_surface_counter>" );

      VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT surfaceCapabilities;
      VULKAN_HPP_NAMESPACE::Result                  result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2EXT( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                     static_cast<VkSurfaceKHR>( surface ),
                                                                     reinterpret_cast<VkSurfaceCapabilities2EXT *>( &surfaceCapabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2EXT" );

      return surfaceCapabilities;
    }

    //=== VK_EXT_display_control ===

    VULKAN_HPP_INLINE void Device::displayPowerControlEXT( VULKAN_HPP_NAMESPACE::DisplayKHR                  display,
                                                           const VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT & displayPowerInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkDisplayPowerControlEXT && "Function <vkDisplayPowerControlEXT> requires <VK_EXT_display_control>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkDisplayPowerControlEXT(
        static_cast<VkDevice>( m_device ), static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayPowerInfoEXT *>( &displayPowerInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::displayPowerControlEXT" );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                        Device::registerEventEXT( VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT const &                                deviceEventInfo,
                                VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Fence  fence;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkRegisterDeviceEventEXT(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkDeviceEventInfoEXT *>( &deviceEventInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkFence *>( &fence ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::registerEventEXT" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence( *this, *reinterpret_cast<VkFence *>( &fence ), allocator );
    }

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence>::Type
                        Device::registerDisplayEventEXT( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DisplayKHR const &             display,
                                       VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT const &                               displayEventInfo,
                                       VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::Fence  fence;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkRegisterDisplayEventEXT(
        static_cast<VkDevice>( m_device ),
        static_cast<VkDisplayKHR>( *display ),
        reinterpret_cast<const VkDisplayEventInfoEXT *>( &displayEventInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkFence *>( &fence ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::registerDisplayEventEXT" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Fence( *this, *reinterpret_cast<VkFence *>( &fence ), allocator );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t SwapchainKHR::getCounterEXT( VULKAN_HPP_NAMESPACE::SurfaceCounterFlagBitsEXT counter ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSwapchainCounterEXT && "Function <vkGetSwapchainCounterEXT> requires <VK_EXT_display_control>" );

      uint64_t                     counterValue;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetSwapchainCounterEXT(
        static_cast<VkDevice>( m_device ), static_cast<VkSwapchainKHR>( m_swapchain ), static_cast<VkSurfaceCounterFlagBitsEXT>( counter ), &counterValue ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::SwapchainKHR::getCounterEXT" );

      return counterValue;
    }

    //=== VK_EXT_discard_rectangles ===

    VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleEXT(
      uint32_t firstDiscardRectangle, VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & discardRectangles ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDiscardRectangleEXT && "Function <vkCmdSetDiscardRectangleEXT> requires <VK_EXT_discard_rectangles>" );

      getDispatcher()->vkCmdSetDiscardRectangleEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                    firstDiscardRectangle,
                                                    discardRectangles.size(),
                                                    reinterpret_cast<const VkRect2D *>( discardRectangles.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 discardRectangleEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDiscardRectangleEnableEXT &&
                         "Function <vkCmdSetDiscardRectangleEnableEXT> requires <VK_EXT_discard_rectangles>" );

      getDispatcher()->vkCmdSetDiscardRectangleEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( discardRectangleEnable ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setDiscardRectangleModeEXT( VULKAN_HPP_NAMESPACE::DiscardRectangleModeEXT discardRectangleMode ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDiscardRectangleModeEXT &&
                         "Function <vkCmdSetDiscardRectangleModeEXT> requires <VK_EXT_discard_rectangles>" );

      getDispatcher()->vkCmdSetDiscardRectangleModeEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                        static_cast<VkDiscardRectangleModeEXT>( discardRectangleMode ) );
    }

    //=== VK_EXT_hdr_metadata ===

    VULKAN_HPP_INLINE void Device::setHdrMetadataEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainKHR> const &   swapchains,
                                                      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::HdrMetadataEXT> const & metadata ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSetHdrMetadataEXT && "Function <vkSetHdrMetadataEXT> requires <VK_EXT_hdr_metadata>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( swapchains.size() == metadata.size() );
#  else
      if ( swapchains.size() != metadata.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::Device::setHdrMetadataEXT: swapchains.size() != metadata.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkSetHdrMetadataEXT( static_cast<VkDevice>( m_device ),
                                            swapchains.size(),
                                            reinterpret_cast<const VkSwapchainKHR *>( swapchains.data() ),
                                            reinterpret_cast<const VkHdrMetadataEXT *>( metadata.data() ) );
    }

    //=== VK_KHR_shared_presentable_image ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result SwapchainKHR::getStatus() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSwapchainStatusKHR && "Function <vkGetSwapchainStatusKHR> requires <VK_KHR_shared_presentable_image>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetSwapchainStatusKHR( static_cast<VkDevice>( m_device ), static_cast<VkSwapchainKHR>( m_swapchain ) ) );
      resultCheck( result,
                   VULKAN_HPP_NAMESPACE_STRING "::SwapchainKHR::getStatus",
                   { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

      return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
    }

    //=== VK_KHR_external_fence_fd ===

    VULKAN_HPP_INLINE void Device::importFenceFdKHR( const VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR & importFenceFdInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkImportFenceFdKHR && "Function <vkImportFenceFdKHR> requires <VK_KHR_external_fence_fd>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkImportFenceFdKHR( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkImportFenceFdInfoKHR *>( &importFenceFdInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceFdKHR" );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE int Device::getFenceFdKHR( const VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR & getFdInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetFenceFdKHR && "Function <vkGetFenceFdKHR> requires <VK_KHR_external_fence_fd>" );

      int                          fd;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetFenceFdKHR( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkFenceGetFdInfoKHR *>( &getFdInfo ), &fd ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceFdKHR" );

      return fd;
    }

    //=== VK_KHR_performance_query ===

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>, std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>>
                        PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR( uint32_t queueFamilyIndex ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR &&
                         "Function <vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR> requires <VK_KHR_performance_query>" );

      std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>, std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>> data_;
      std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR> &            counters            = data_.first;
      std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR> & counterDescriptions = data_.second;
      uint32_t                                                              counterCount;
      VULKAN_HPP_NAMESPACE::Result                                          result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), queueFamilyIndex, &counterCount, nullptr, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && counterCount )
        {
          counters.resize( counterCount );
          counterDescriptions.resize( counterCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ),
            queueFamilyIndex,
            &counterCount,
            reinterpret_cast<VkPerformanceCounterKHR *>( counters.data() ),
            reinterpret_cast<VkPerformanceCounterDescriptionKHR *>( counterDescriptions.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR" );
      VULKAN_HPP_ASSERT( counterCount <= counters.size() );
      if ( counterCount < counters.size() )
      {
        counters.resize( counterCount );
        counterDescriptions.resize( counterCount );
      }
      return data_;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint32_t PhysicalDevice::getQueueFamilyPerformanceQueryPassesKHR(
      const VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR & performanceQueryCreateInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR &&
                         "Function <vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR> requires <VK_KHR_performance_query>" );

      uint32_t numPasses;
      getDispatcher()->vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
        static_cast<VkPhysicalDevice>( m_physicalDevice ),
        reinterpret_cast<const VkQueryPoolPerformanceCreateInfoKHR *>( &performanceQueryCreateInfo ),
        &numPasses );

      return numPasses;
    }

    VULKAN_HPP_INLINE void Device::acquireProfilingLockKHR( const VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR & info ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkAcquireProfilingLockKHR && "Function <vkAcquireProfilingLockKHR> requires <VK_KHR_performance_query>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkAcquireProfilingLockKHR( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkAcquireProfilingLockInfoKHR *>( &info ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::acquireProfilingLockKHR" );
    }

    VULKAN_HPP_INLINE void Device::releaseProfilingLockKHR() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkReleaseProfilingLockKHR && "Function <vkReleaseProfilingLockKHR> requires <VK_KHR_performance_query>" );

      getDispatcher()->vkReleaseProfilingLockKHR( static_cast<VkDevice>( m_device ) );
    }

    //=== VK_KHR_get_surface_capabilities2 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR
      PhysicalDevice::getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2KHR &&
                         "Function <vkGetPhysicalDeviceSurfaceCapabilities2KHR> requires <VK_KHR_get_surface_capabilities2>" );

      VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR surfaceCapabilities;
      VULKAN_HPP_NAMESPACE::Result                  result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                     reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                     reinterpret_cast<VkSurfaceCapabilities2KHR *>( &surfaceCapabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2KHR" );

      return surfaceCapabilities;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
      PhysicalDevice::getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2KHR &&
                         "Function <vkGetPhysicalDeviceSurfaceCapabilities2KHR> requires <VK_KHR_get_surface_capabilities2>" );

      StructureChain<X, Y, Z...>                      structureChain;
      VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR & surfaceCapabilities = structureChain.template get<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>();
      VULKAN_HPP_NAMESPACE::Result                    result              = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceSurfaceCapabilities2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                     reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                     reinterpret_cast<VkSurfaceCapabilities2KHR *>( &surfaceCapabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2KHR" );

      return structureChain;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>
      PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR &&
                         "Function <vkGetPhysicalDeviceSurfaceFormats2KHR> requires <VK_KHR_get_surface_capabilities2>" );

      std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR> surfaceFormats;
      uint32_t                                             surfaceFormatCount;
      VULKAN_HPP_NAMESPACE::Result                         result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                  reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                  &surfaceFormatCount,
                                                                  nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
        {
          surfaceFormats.resize( surfaceFormatCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                    &surfaceFormatCount,
                                                                    reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
      VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
      if ( surfaceFormatCount < surfaceFormats.size() )
      {
        surfaceFormats.resize( surfaceFormatCount );
      }
      return surfaceFormats;
    }

    template <typename StructureChain>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain>
      PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR &&
                         "Function <vkGetPhysicalDeviceSurfaceFormats2KHR> requires <VK_KHR_get_surface_capabilities2>" );

      std::vector<StructureChain>                          structureChains;
      std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR> surfaceFormats;
      uint32_t                                             surfaceFormatCount;
      VULKAN_HPP_NAMESPACE::Result                         result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                  reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                  &surfaceFormatCount,
                                                                  nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
        {
          structureChains.resize( surfaceFormatCount );
          surfaceFormats.resize( surfaceFormatCount );
          for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
          {
            surfaceFormats[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>().pNext;
          }
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDeviceSurfaceFormats2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                                    &surfaceFormatCount,
                                                                    reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
      VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
      if ( surfaceFormatCount < surfaceFormats.size() )
      {
        structureChains.resize( surfaceFormatCount );
      }
      for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
      {
        structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>() = surfaceFormats[i];
      }
      return structureChains;
    }

    //=== VK_KHR_get_display_properties2 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR> PhysicalDevice::getDisplayProperties2KHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceDisplayProperties2KHR &&
                         "Function <vkGetPhysicalDeviceDisplayProperties2KHR> requires <VK_KHR_get_display_properties2>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR> properties;
      uint32_t                                                 propertyCount;
      VULKAN_HPP_NAMESPACE::Result                             result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceDisplayProperties2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceDisplayProperties2KHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, reinterpret_cast<VkDisplayProperties2KHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayProperties2KHR" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR> PhysicalDevice::getDisplayPlaneProperties2KHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceDisplayPlaneProperties2KHR &&
                         "Function <vkGetPhysicalDeviceDisplayPlaneProperties2KHR> requires <VK_KHR_get_display_properties2>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR> properties;
      uint32_t                                                      propertyCount;
      VULKAN_HPP_NAMESPACE::Result                                  result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceDisplayPlaneProperties2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &propertyCount, reinterpret_cast<VkDisplayPlaneProperties2KHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneProperties2KHR" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR> DisplayKHR::getModeProperties2() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDisplayModeProperties2KHR &&
                         "Function <vkGetDisplayModeProperties2KHR> requires <VK_KHR_get_display_properties2>" );

      std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR> properties;
      uint32_t                                                     propertyCount;
      VULKAN_HPP_NAMESPACE::Result                                 result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetDisplayModeProperties2KHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), static_cast<VkDisplayKHR>( m_display ), &propertyCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetDisplayModeProperties2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                             static_cast<VkDisplayKHR>( m_display ),
                                                             &propertyCount,
                                                             reinterpret_cast<VkDisplayModeProperties2KHR *>( properties.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::DisplayKHR::getModeProperties2" );
      VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
      if ( propertyCount < properties.size() )
      {
        properties.resize( propertyCount );
      }
      return properties;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR
      PhysicalDevice::getDisplayPlaneCapabilities2KHR( const VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR & displayPlaneInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetDisplayPlaneCapabilities2KHR &&
                         "Function <vkGetDisplayPlaneCapabilities2KHR> requires <VK_KHR_get_display_properties2>" );

      VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR capabilities;
      VULKAN_HPP_NAMESPACE::Result                       result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetDisplayPlaneCapabilities2KHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                            reinterpret_cast<const VkDisplayPlaneInfo2KHR *>( &displayPlaneInfo ),
                                                            reinterpret_cast<VkDisplayPlaneCapabilities2KHR *>( &capabilities ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneCapabilities2KHR" );

      return capabilities;
    }

    //=== VK_EXT_debug_utils ===

    VULKAN_HPP_INLINE void Device::setDebugUtilsObjectNameEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT & nameInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSetDebugUtilsObjectNameEXT && "Function <vkSetDebugUtilsObjectNameEXT> requires <VK_EXT_debug_utils>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkSetDebugUtilsObjectNameEXT(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkDebugUtilsObjectNameInfoEXT *>( &nameInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setDebugUtilsObjectNameEXT" );
    }

    VULKAN_HPP_INLINE void Device::setDebugUtilsObjectTagEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT & tagInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSetDebugUtilsObjectTagEXT && "Function <vkSetDebugUtilsObjectTagEXT> requires <VK_EXT_debug_utils>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkSetDebugUtilsObjectTagEXT( static_cast<VkDevice>( m_device ), reinterpret_cast<const VkDebugUtilsObjectTagInfoEXT *>( &tagInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setDebugUtilsObjectTagEXT" );
    }

    VULKAN_HPP_INLINE void Queue::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueBeginDebugUtilsLabelEXT && "Function <vkQueueBeginDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkQueueBeginDebugUtilsLabelEXT( static_cast<VkQueue>( m_queue ), reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
    }

    VULKAN_HPP_INLINE void Queue::endDebugUtilsLabelEXT() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueEndDebugUtilsLabelEXT && "Function <vkQueueEndDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkQueueEndDebugUtilsLabelEXT( static_cast<VkQueue>( m_queue ) );
    }

    VULKAN_HPP_INLINE void Queue::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueInsertDebugUtilsLabelEXT && "Function <vkQueueInsertDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkQueueInsertDebugUtilsLabelEXT( static_cast<VkQueue>( m_queue ), reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBeginDebugUtilsLabelEXT && "Function <vkCmdBeginDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkCmdBeginDebugUtilsLabelEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                     reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::endDebugUtilsLabelEXT() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdEndDebugUtilsLabelEXT && "Function <vkCmdEndDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkCmdEndDebugUtilsLabelEXT( static_cast<VkCommandBuffer>( m_commandBuffer ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdInsertDebugUtilsLabelEXT && "Function <vkCmdInsertDebugUtilsLabelEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkCmdInsertDebugUtilsLabelEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                      reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT>::Type
      Instance::createDebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT const &                  createInfo,
                                              VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT messenger;
      VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateDebugUtilsMessengerEXT(
        static_cast<VkInstance>( m_instance ),
        reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkDebugUtilsMessengerEXT *>( &messenger ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Instance::createDebugUtilsMessengerEXT" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT(
        *this, *reinterpret_cast<VkDebugUtilsMessengerEXT *>( &messenger ), allocator );
    }

    VULKAN_HPP_INLINE void
      Instance::submitDebugUtilsMessageEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                            VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT              messageTypes,
                                            const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT & callbackData ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkSubmitDebugUtilsMessageEXT && "Function <vkSubmitDebugUtilsMessageEXT> requires <VK_EXT_debug_utils>" );

      getDispatcher()->vkSubmitDebugUtilsMessageEXT( static_cast<VkInstance>( m_instance ),
                                                     static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ),
                                                     static_cast<VkDebugUtilsMessageTypeFlagsEXT>( messageTypes ),
                                                     reinterpret_cast<const VkDebugUtilsMessengerCallbackDataEXT *>( &callbackData ) );
    }

    //=== VK_EXT_sample_locations ===

    VULKAN_HPP_INLINE void
      CommandBuffer::setSampleLocationsEXT( const VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT & sampleLocationsInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetSampleLocationsEXT && "Function <vkCmdSetSampleLocationsEXT> requires <VK_EXT_sample_locations>" );

      getDispatcher()->vkCmdSetSampleLocationsEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                   reinterpret_cast<const VkSampleLocationsInfoEXT *>( &sampleLocationsInfo ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT
      PhysicalDevice::getMultisamplePropertiesEXT( VULKAN_HPP_NAMESPACE::SampleCountFlagBits samples ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceMultisamplePropertiesEXT &&
                         "Function <vkGetPhysicalDeviceMultisamplePropertiesEXT> requires <VK_EXT_sample_locations>" );

      VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT multisampleProperties;
      getDispatcher()->vkGetPhysicalDeviceMultisamplePropertiesEXT( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                    static_cast<VkSampleCountFlagBits>( samples ),
                                                                    reinterpret_cast<VkMultisamplePropertiesEXT *>( &multisampleProperties ) );

      return multisampleProperties;
    }

    //=== VK_EXT_image_drm_format_modifier ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT Image::getDrmFormatModifierPropertiesEXT() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetImageDrmFormatModifierPropertiesEXT &&
                         "Function <vkGetImageDrmFormatModifierPropertiesEXT> requires <VK_EXT_image_drm_format_modifier>" );

      VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT properties;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetImageDrmFormatModifierPropertiesEXT(
        static_cast<VkDevice>( m_device ), static_cast<VkImage>( m_image ), reinterpret_cast<VkImageDrmFormatModifierPropertiesEXT *>( &properties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Image::getDrmFormatModifierPropertiesEXT" );

      return properties;
    }

    //=== VK_EXT_external_memory_host ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT
      Device::getMemoryHostPointerPropertiesEXT( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, const void * pHostPointer ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetMemoryHostPointerPropertiesEXT &&
                         "Function <vkGetMemoryHostPointerPropertiesEXT> requires <VK_EXT_external_memory_host>" );

      VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT memoryHostPointerProperties;
      VULKAN_HPP_NAMESPACE::Result                         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetMemoryHostPointerPropertiesEXT( static_cast<VkDevice>( m_device ),
                                                              static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                              pHostPointer,
                                                              reinterpret_cast<VkMemoryHostPointerPropertiesEXT *>( &memoryHostPointerProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryHostPointerPropertiesEXT" );

      return memoryHostPointerProperties;
    }

    //=== VK_EXT_calibrated_timestamps ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::TimeDomainKHR> PhysicalDevice::getCalibrateableTimeDomainsEXT() const
    {
      VULKAN_HPP_ASSERT(
        getDispatcher()->vkGetPhysicalDeviceCalibrateableTimeDomainsEXT &&
        "Function <vkGetPhysicalDeviceCalibrateableTimeDomainsEXT> requires <VK_EXT_calibrated_timestamps> or <VK_KHR_calibrated_timestamps>" );

      std::vector<VULKAN_HPP_NAMESPACE::TimeDomainKHR> timeDomains;
      uint32_t                                         timeDomainCount;
      VULKAN_HPP_NAMESPACE::Result                     result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          getDispatcher()->vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( static_cast<VkPhysicalDevice>( m_physicalDevice ), &timeDomainCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && timeDomainCount )
        {
          timeDomains.resize( timeDomainCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(
            static_cast<VkPhysicalDevice>( m_physicalDevice ), &timeDomainCount, reinterpret_cast<VkTimeDomainKHR *>( timeDomains.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getCalibrateableTimeDomainsEXT" );
      VULKAN_HPP_ASSERT( timeDomainCount <= timeDomains.size() );
      if ( timeDomainCount < timeDomains.size() )
      {
        timeDomains.resize( timeDomainCount );
      }
      return timeDomains;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<std::vector<uint64_t>, uint64_t> Device::getCalibratedTimestampsEXT(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoKHR> const & timestampInfos ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetCalibratedTimestampsEXT &&
                         "Function <vkGetCalibratedTimestampsEXT> requires <VK_EXT_calibrated_timestamps> or <VK_KHR_calibrated_timestamps>" );

      std::pair<std::vector<uint64_t>, uint64_t> data_( std::piecewise_construct, std::forward_as_tuple( timestampInfos.size() ), std::forward_as_tuple( 0 ) );
      std::vector<uint64_t> &                    timestamps   = data_.first;
      uint64_t &                                 maxDeviation = data_.second;
      VULKAN_HPP_NAMESPACE::Result               result       = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetCalibratedTimestampsEXT( static_cast<VkDevice>( m_device ),
                                                       timestampInfos.size(),
                                                       reinterpret_cast<const VkCalibratedTimestampInfoKHR *>( timestampInfos.data() ),
                                                       timestamps.data(),
                                                       &maxDeviation ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getCalibratedTimestampsEXT" );

      return data_;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<uint64_t, uint64_t>
                                           Device::getCalibratedTimestampEXT( const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoKHR & timestampInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetCalibratedTimestampsEXT &&
                         "Function <vkGetCalibratedTimestampsEXT> requires <VK_EXT_calibrated_timestamps> or <VK_KHR_calibrated_timestamps>" );

      std::pair<uint64_t, uint64_t> data_;
      uint64_t &                    timestamp    = data_.first;
      uint64_t &                    maxDeviation = data_.second;
      VULKAN_HPP_NAMESPACE::Result  result       = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetCalibratedTimestampsEXT(
        static_cast<VkDevice>( m_device ), 1, reinterpret_cast<const VkCalibratedTimestampInfoKHR *>( &timestampInfo ), &timestamp, &maxDeviation ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getCalibratedTimestampEXT" );

      return data_;
    }

    //=== VK_KHR_fragment_shading_rate ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR>
                                           PhysicalDevice::getFragmentShadingRatesKHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceFragmentShadingRatesKHR &&
                         "Function <vkGetPhysicalDeviceFragmentShadingRatesKHR> requires <VK_KHR_fragment_shading_rate>" );

      std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR> fragmentShadingRates;
      uint32_t                                                                fragmentShadingRateCount;
      VULKAN_HPP_NAMESPACE::Result                                            result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceFragmentShadingRatesKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), &fragmentShadingRateCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && fragmentShadingRateCount )
        {
          fragmentShadingRates.resize( fragmentShadingRateCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceFragmentShadingRatesKHR(
            static_cast<VkPhysicalDevice>( m_physicalDevice ),
            &fragmentShadingRateCount,
            reinterpret_cast<VkPhysicalDeviceFragmentShadingRateKHR *>( fragmentShadingRates.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getFragmentShadingRatesKHR" );
      VULKAN_HPP_ASSERT( fragmentShadingRateCount <= fragmentShadingRates.size() );
      if ( fragmentShadingRateCount < fragmentShadingRates.size() )
      {
        fragmentShadingRates.resize( fragmentShadingRateCount );
      }
      return fragmentShadingRates;
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setFragmentShadingRateKHR( const VULKAN_HPP_NAMESPACE::Extent2D &                       fragmentSize,
                                                const VULKAN_HPP_NAMESPACE::FragmentShadingRateCombinerOpKHR combinerOps[2] ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetFragmentShadingRateKHR &&
                         "Function <vkCmdSetFragmentShadingRateKHR> requires <VK_KHR_fragment_shading_rate>" );

      getDispatcher()->vkCmdSetFragmentShadingRateKHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                       reinterpret_cast<const VkExtent2D *>( &fragmentSize ),
                                                       reinterpret_cast<const VkFragmentShadingRateCombinerOpKHR *>( combinerOps ) );
    }

    //=== VK_EXT_headless_surface ===

    VULKAN_HPP_NODISCARD
      VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR>::Type
                        Instance::createHeadlessSurfaceEXT( VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT const &                      createInfo,
                                          VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
      VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateHeadlessSurfaceEXT(
        static_cast<VkInstance>( m_instance ),
        reinterpret_cast<const VkHeadlessSurfaceCreateInfoEXT *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#  if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#  else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Instance::createHeadlessSurfaceEXT" );
#  endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SurfaceKHR( *this, *reinterpret_cast<VkSurfaceKHR *>( &surface ), allocator );
    }

    //=== VK_EXT_line_rasterization ===

    VULKAN_HPP_INLINE void CommandBuffer::setLineStippleEXT( uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetLineStippleEXT &&
                         "Function <vkCmdSetLineStippleEXT> requires <VK_EXT_line_rasterization> or <VK_KHR_line_rasterization>" );

      getDispatcher()->vkCmdSetLineStippleEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), lineStippleFactor, lineStipplePattern );
    }

    //=== VK_EXT_extended_dynamic_state ===

    VULKAN_HPP_INLINE void CommandBuffer::setCullModeEXT( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetCullModeEXT &&
                         "Function <vkCmdSetCullModeEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetCullModeEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkCullModeFlags>( cullMode ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setFrontFaceEXT( VULKAN_HPP_NAMESPACE::FrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetFrontFaceEXT &&
                         "Function <vkCmdSetFrontFaceEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetFrontFaceEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkFrontFace>( frontFace ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveTopologyEXT( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetPrimitiveTopologyEXT &&
                         "Function <vkCmdSetPrimitiveTopologyEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetPrimitiveTopologyEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPrimitiveTopology>( primitiveTopology ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setViewportWithCountEXT(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetViewportWithCountEXT &&
                         "Function <vkCmdSetViewportWithCountEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetViewportWithCountEXT(
        static_cast<VkCommandBuffer>( m_commandBuffer ), viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::setScissorWithCountEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetScissorWithCountEXT &&
                         "Function <vkCmdSetScissorWithCountEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetScissorWithCountEXT(
        static_cast<VkCommandBuffer>( m_commandBuffer ), scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::bindVertexBuffers2EXT( uint32_t                                                                         firstBinding,
                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets,
                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes,
                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBindVertexBuffers2EXT &&
                         "Function <vkCmdBindVertexBuffers2EXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
      VULKAN_HPP_ASSERT( sizes.empty() || buffers.size() == sizes.size() );
      VULKAN_HPP_ASSERT( strides.empty() || buffers.size() == strides.size() );
#  else
      if ( buffers.size() != offsets.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != offsets.size()" );
      }
      if ( !sizes.empty() && buffers.size() != sizes.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != sizes.size()" );
      }
      if ( !strides.empty() && buffers.size() != strides.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != strides.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkCmdBindVertexBuffers2EXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                   firstBinding,
                                                   buffers.size(),
                                                   reinterpret_cast<const VkBuffer *>( buffers.data() ),
                                                   reinterpret_cast<const VkDeviceSize *>( offsets.data() ),
                                                   reinterpret_cast<const VkDeviceSize *>( sizes.data() ),
                                                   reinterpret_cast<const VkDeviceSize *>( strides.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthTestEnableEXT &&
                         "Function <vkCmdSetDepthTestEnableEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthTestEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthWriteEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthWriteEnableEXT &&
                         "Function <vkCmdSetDepthWriteEnableEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthWriteEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthWriteEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthCompareOpEXT( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthCompareOpEXT &&
                         "Function <vkCmdSetDepthCompareOpEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthCompareOpEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkCompareOp>( depthCompareOp ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthBoundsTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBoundsTestEnableEXT &&
                         "Function <vkCmdSetDepthBoundsTestEnableEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthBoundsTestEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthBoundsTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilTestEnableEXT &&
                         "Function <vkCmdSetStencilTestEnableEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetStencilTestEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( stencilTestEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setStencilOpEXT( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                                                           VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                                                           VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                                                           VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                                                           VULKAN_HPP_NAMESPACE::CompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetStencilOpEXT &&
                         "Function <vkCmdSetStencilOpEXT> requires <VK_EXT_extended_dynamic_state> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetStencilOpEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                             static_cast<VkStencilFaceFlags>( faceMask ),
                                             static_cast<VkStencilOp>( failOp ),
                                             static_cast<VkStencilOp>( passOp ),
                                             static_cast<VkStencilOp>( depthFailOp ),
                                             static_cast<VkCompareOp>( compareOp ) );
    }

    //=== VK_KHR_object_refresh ===

    VULKAN_HPP_INLINE void CommandBuffer::refreshObjectsKHR( const VULKAN_HPP_NAMESPACE::RefreshObjectListKHR & refreshObjects ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdRefreshObjectsKHR && "Function <vkCmdRefreshObjectsKHR> requires <VK_KHR_object_refresh>" );

      getDispatcher()->vkCmdRefreshObjectsKHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               reinterpret_cast<const VkRefreshObjectListKHR *>( &refreshObjects ) );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::ObjectType> PhysicalDevice::getRefreshableObjectTypesKHR() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceRefreshableObjectTypesKHR &&
                         "Function <vkGetPhysicalDeviceRefreshableObjectTypesKHR> requires <VK_KHR_object_refresh>" );

      std::vector<VULKAN_HPP_NAMESPACE::ObjectType> refreshableObjectTypes;
      uint32_t                                      refreshableObjectTypeCount;
      VULKAN_HPP_NAMESPACE::Result                  result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceRefreshableObjectTypesKHR(
          static_cast<VkPhysicalDevice>( m_physicalDevice ), &refreshableObjectTypeCount, nullptr ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && refreshableObjectTypeCount )
        {
          refreshableObjectTypes.resize( refreshableObjectTypeCount );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
            getDispatcher()->vkGetPhysicalDeviceRefreshableObjectTypesKHR( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                           &refreshableObjectTypeCount,
                                                                           reinterpret_cast<VkObjectType *>( refreshableObjectTypes.data() ) ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );

      VULKAN_HPP_ASSERT( refreshableObjectTypeCount <= refreshableObjectTypes.size() );
      if ( refreshableObjectTypeCount < refreshableObjectTypes.size() )
      {
        refreshableObjectTypes.resize( refreshableObjectTypeCount );
      }
      return refreshableObjectTypes;
    }

    //=== VK_KHR_synchronization2 ===

    VULKAN_HPP_INLINE void CommandBuffer::setEvent2KHR( VULKAN_HPP_NAMESPACE::Event                  event,
                                                        const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetEvent2KHR && "Function <vkCmdSetEvent2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetEvent2KHR(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::resetEvent2KHR( VULKAN_HPP_NAMESPACE::Event               event,
                                                          VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResetEvent2KHR && "Function <vkCmdResetEvent2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdResetEvent2KHR(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags2>( stageMask ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::waitEvents2KHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWaitEvents2KHR && "Function <vkCmdWaitEvents2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_ASSERT( events.size() == dependencyInfos.size() );
#  else
      if ( events.size() != dependencyInfos.size() )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::waitEvents2KHR: events.size() != dependencyInfos.size()" );
      }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

      getDispatcher()->vkCmdWaitEvents2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                            events.size(),
                                            reinterpret_cast<const VkEvent *>( events.data() ),
                                            reinterpret_cast<const VkDependencyInfo *>( dependencyInfos.data() ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2KHR( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdPipelineBarrier2KHR &&
                         "Function <vkCmdPipelineBarrier2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdPipelineBarrier2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                 reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp2KHR( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                              VULKAN_HPP_NAMESPACE::QueryPool           queryPool,
                                                              uint32_t                                  query ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWriteTimestamp2KHR &&
                         "Function <vkCmdWriteTimestamp2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdWriteTimestamp2KHR(
        static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkPipelineStageFlags2>( stage ), static_cast<VkQueryPool>( queryPool ), query );
    }

    VULKAN_HPP_INLINE void Queue::submit2KHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits,
                                              VULKAN_HPP_NAMESPACE::Fence                                                       fence ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkQueueSubmit2KHR && "Function <vkQueueSubmit2KHR> requires <VK_KHR_synchronization2> or <VK_VERSION_1_3>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkQueueSubmit2KHR(
        static_cast<VkQueue>( m_queue ), submits.size(), reinterpret_cast<const VkSubmitInfo2 *>( submits.data() ), static_cast<VkFence>( fence ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit2KHR" );
    }

    VULKAN_HPP_INLINE void CommandBuffer::writeBufferMarker2AMD( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                                 VULKAN_HPP_NAMESPACE::Buffer              dstBuffer,
                                                                 VULKAN_HPP_NAMESPACE::DeviceSize          dstOffset,
                                                                 uint32_t                                  marker ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdWriteBufferMarker2AMD && "Function <vkCmdWriteBufferMarker2AMD> requires <VK_KHR_synchronization2>" );

      getDispatcher()->vkCmdWriteBufferMarker2AMD( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                   static_cast<VkPipelineStageFlags2>( stage ),
                                                   static_cast<VkBuffer>( dstBuffer ),
                                                   static_cast<VkDeviceSize>( dstOffset ),
                                                   marker );
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV> Queue::getCheckpointData2NV() const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetQueueCheckpointData2NV && "Function <vkGetQueueCheckpointData2NV> requires <VK_KHR_synchronization2>" );

      std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV> checkpointData;
      uint32_t                                             checkpointDataCount;
      getDispatcher()->vkGetQueueCheckpointData2NV( static_cast<VkQueue>( m_queue ), &checkpointDataCount, nullptr );
      checkpointData.resize( checkpointDataCount );
      getDispatcher()->vkGetQueueCheckpointData2NV(
        static_cast<VkQueue>( m_queue ), &checkpointDataCount, reinterpret_cast<VkCheckpointData2NV *>( checkpointData.data() ) );

      VULKAN_HPP_ASSERT( checkpointDataCount <= checkpointData.size() );
      if ( checkpointDataCount < checkpointData.size() )
      {
        checkpointData.resize( checkpointDataCount );
      }
      return checkpointData;
    }

    //=== VK_KHR_copy_commands2 ===

    VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBuffer2KHR && "Function <vkCmdCopyBuffer2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyBuffer2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkCopyBufferInfo2 *>( &copyBufferInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::copyImage2KHR( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImage2KHR && "Function <vkCmdCopyImage2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyImage2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkCopyImageInfo2 *>( &copyImageInfo ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyBufferToImage2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyBufferToImage2KHR &&
                         "Function <vkCmdCopyBufferToImage2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyBufferToImage2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                   reinterpret_cast<const VkCopyBufferToImageInfo2 *>( &copyBufferToImageInfo ) );
    }

    VULKAN_HPP_INLINE void
      CommandBuffer::copyImageToBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdCopyImageToBuffer2KHR &&
                         "Function <vkCmdCopyImageToBuffer2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdCopyImageToBuffer2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                                   reinterpret_cast<const VkCopyImageToBufferInfo2 *>( &copyImageToBufferInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::blitImage2KHR( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdBlitImage2KHR && "Function <vkCmdBlitImage2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdBlitImage2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ), reinterpret_cast<const VkBlitImageInfo2 *>( &blitImageInfo ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::resolveImage2KHR( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdResolveImage2KHR && "Function <vkCmdResolveImage2KHR> requires <VK_KHR_copy_commands2> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdResolveImage2KHR( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                              reinterpret_cast<const VkResolveImageInfo2 *>( &resolveImageInfo ) );
    }

    //=== VK_EXT_vertex_input_dynamic_state ===

    VULKAN_HPP_INLINE void CommandBuffer::setVertexInputEXT(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT> const &   vertexBindingDescriptions,
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT> const & vertexAttributeDescriptions ) const
      VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetVertexInputEXT &&
                         "Function <vkCmdSetVertexInputEXT> requires <VK_EXT_shader_object> or <VK_EXT_vertex_input_dynamic_state>" );

      getDispatcher()->vkCmdSetVertexInputEXT( static_cast<VkCommandBuffer>( m_commandBuffer ),
                                               vertexBindingDescriptions.size(),
                                               reinterpret_cast<const VkVertexInputBindingDescription2EXT *>( vertexBindingDescriptions.data() ),
                                               vertexAttributeDescriptions.size(),
                                               reinterpret_cast<const VkVertexInputAttributeDescription2EXT *>( vertexAttributeDescriptions.data() ) );
    }

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync ===

    template <typename HandleType>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE HandleType
      Device::getFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetFenceSciSyncFenceNV &&
                         "Function <vkGetFenceSciSyncFenceNV> requires <VK_NV_external_sci_sync> or <VK_NV_external_sci_sync2>" );

      HandleType                   handle;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetFenceSciSyncFenceNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( &getSciSyncHandleInfo ), &handle ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceSciSyncFenceNV" );

      return handle;
    }

    template <typename HandleType>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE HandleType
      Device::getFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetFenceSciSyncObjNV &&
                         "Function <vkGetFenceSciSyncObjNV> requires <VK_NV_external_sci_sync> or <VK_NV_external_sci_sync2>" );

      HandleType                   handle;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetFenceSciSyncObjNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( &getSciSyncHandleInfo ), &handle ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceSciSyncObjNV" );

      return handle;
    }

    VULKAN_HPP_INLINE void Device::importFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkImportFenceSciSyncFenceNV &&
                         "Function <vkImportFenceSciSyncFenceNV> requires <VK_NV_external_sci_sync> or <VK_NV_external_sci_sync2>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkImportFenceSciSyncFenceNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( &importFenceSciSyncInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceSciSyncFenceNV" );
    }

    VULKAN_HPP_INLINE void Device::importFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkImportFenceSciSyncObjNV &&
                         "Function <vkImportFenceSciSyncObjNV> requires <VK_NV_external_sci_sync> or <VK_NV_external_sci_sync2>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkImportFenceSciSyncObjNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( &importFenceSciSyncInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceSciSyncObjNV" );
    }

    VULKAN_HPP_INLINE void PhysicalDevice::getSciSyncAttributesNV( const VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV & sciSyncAttributesInfo,
                                                                   NvSciSyncAttrList                                     pAttributes ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSciSyncAttributesNV &&
                         "Function <vkGetPhysicalDeviceSciSyncAttributesNV> requires <VK_NV_external_sci_sync> or <VK_NV_external_sci_sync2>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetPhysicalDeviceSciSyncAttributesNV(
        static_cast<VkPhysicalDevice>( m_physicalDevice ), reinterpret_cast<const VkSciSyncAttributesInfoNV *>( &sciSyncAttributesInfo ), pAttributes ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSciSyncAttributesNV" );
    }

    template <typename HandleType>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE HandleType
      Device::getSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV & getSciSyncInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetSemaphoreSciSyncObjNV && "Function <vkGetSemaphoreSciSyncObjNV> requires <VK_NV_external_sci_sync>" );

      HandleType                   handle;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetSemaphoreSciSyncObjNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkSemaphoreGetSciSyncInfoNV *>( &getSciSyncInfo ), &handle ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSemaphoreSciSyncObjNV" );

      return handle;
    }

    VULKAN_HPP_INLINE void Device::importSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV & importSemaphoreSciSyncInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkImportSemaphoreSciSyncObjNV && "Function <vkImportSemaphoreSciSyncObjNV> requires <VK_NV_external_sci_sync>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkImportSemaphoreSciSyncObjNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkImportSemaphoreSciSyncInfoNV *>( &importSemaphoreSciSyncInfo ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importSemaphoreSciSyncObjNV" );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_memory_sci_buf ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE NvSciBufObj Device::getMemorySciBufNV( const VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV & getSciBufInfo ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetMemorySciBufNV && "Function <vkGetMemorySciBufNV> requires <VK_NV_external_memory_sci_buf>" );

      NvSciBufObj                  handle;
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetMemorySciBufNV(
        static_cast<VkDevice>( m_device ), reinterpret_cast<const VkMemoryGetSciBufInfoNV *>( &getSciBufInfo ), &handle ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemorySciBufNV" );

      return handle;
    }

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV
      PhysicalDevice::getExternalMemorySciBufPropertiesNV( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV &&
                         "Function <vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV> requires <VK_NV_external_memory_sci_buf>" );

      VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV memorySciBufProperties;
      VULKAN_HPP_NAMESPACE::Result                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( static_cast<VkPhysicalDevice>( m_physicalDevice ),
                                                                              static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                                              handle,
                                                                              reinterpret_cast<VkMemorySciBufPropertiesNV *>( &memorySciBufProperties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getExternalMemorySciBufPropertiesNV" );

      return memorySciBufProperties;
    }

    VULKAN_HPP_INLINE void PhysicalDevice::getSciBufAttributesNV( NvSciBufAttrList pAttributes ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetPhysicalDeviceSciBufAttributesNV &&
                         "Function <vkGetPhysicalDeviceSciBufAttributesNV> requires <VK_NV_external_memory_sci_buf>" );

      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        getDispatcher()->vkGetPhysicalDeviceSciBufAttributesNV( static_cast<VkPhysicalDevice>( m_physicalDevice ), pAttributes ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSciBufAttributesNV" );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

    //=== VK_EXT_extended_dynamic_state2 ===

    VULKAN_HPP_INLINE void CommandBuffer::setPatchControlPointsEXT( uint32_t patchControlPoints ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetPatchControlPointsEXT &&
                         "Function <vkCmdSetPatchControlPointsEXT> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object>" );

      getDispatcher()->vkCmdSetPatchControlPointsEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), patchControlPoints );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setRasterizerDiscardEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(
        getDispatcher()->vkCmdSetRasterizerDiscardEnableEXT &&
        "Function <vkCmdSetRasterizerDiscardEnableEXT> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetRasterizerDiscardEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( rasterizerDiscardEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setDepthBiasEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetDepthBiasEnableEXT &&
                         "Function <vkCmdSetDepthBiasEnableEXT> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetDepthBiasEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( depthBiasEnable ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setLogicOpEXT( VULKAN_HPP_NAMESPACE::LogicOp logicOp ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetLogicOpEXT &&
                         "Function <vkCmdSetLogicOpEXT> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object>" );

      getDispatcher()->vkCmdSetLogicOpEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkLogicOp>( logicOp ) );
    }

    VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveRestartEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(
        getDispatcher()->vkCmdSetPrimitiveRestartEnableEXT &&
        "Function <vkCmdSetPrimitiveRestartEnableEXT> requires <VK_EXT_extended_dynamic_state2> or <VK_EXT_shader_object> or <VK_VERSION_1_3>" );

      getDispatcher()->vkCmdSetPrimitiveRestartEnableEXT( static_cast<VkCommandBuffer>( m_commandBuffer ), static_cast<VkBool32>( primitiveRestartEnable ) );
    }

    //=== VK_EXT_color_write_enable ===

    VULKAN_HPP_INLINE void CommandBuffer::setColorWriteEnableEXT(
      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Bool32> const & colorWriteEnables ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkCmdSetColorWriteEnableEXT && "Function <vkCmdSetColorWriteEnableEXT> requires <VK_EXT_color_write_enable>" );

      getDispatcher()->vkCmdSetColorWriteEnableEXT(
        static_cast<VkCommandBuffer>( m_commandBuffer ), colorWriteEnables.size(), reinterpret_cast<const VkBool32 *>( colorWriteEnables.data() ) );
    }

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync2 ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::CreateReturnType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SemaphoreSciSyncPoolNV>::Type
      Device::createSemaphoreSciSyncPoolNV( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV const &                  createInfo,
                                            VULKAN_HPP_NAMESPACE::Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator ) const
      VULKAN_HPP_RAII_CREATE_NOEXCEPT
    {
      VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV semaphorePool;
      VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkCreateSemaphoreSciSyncPoolNV(
        static_cast<VkDevice>( m_device ),
        reinterpret_cast<const VkSemaphoreSciSyncPoolCreateInfoNV *>( &createInfo ),
        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
        reinterpret_cast<VkSemaphoreSciSyncPoolNV *>( &semaphorePool ) ) );
      if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
#    if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
        return VULKAN_HPP_UNEXPECTED( result );
#    else
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "Device::createSemaphoreSciSyncPoolNV" );
#    endif
      }

      return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::SemaphoreSciSyncPoolNV(
        *this, *reinterpret_cast<VkSemaphoreSciSyncPoolNV *>( &semaphorePool ), allocator );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
    //=== VK_QNX_external_memory_screen_buffer ===

    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX
                                           Device::getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetScreenBufferPropertiesQNX &&
                         "Function <vkGetScreenBufferPropertiesQNX> requires <VK_QNX_external_memory_screen_buffer>" );

      VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX properties;
      VULKAN_HPP_NAMESPACE::Result                    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetScreenBufferPropertiesQNX(
        static_cast<VkDevice>( m_device ), &buffer, reinterpret_cast<VkScreenBufferPropertiesQNX *>( &properties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getScreenBufferPropertiesQNX" );

      return properties;
    }

    template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                           Device::getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer ) const
    {
      VULKAN_HPP_ASSERT( getDispatcher()->vkGetScreenBufferPropertiesQNX &&
                         "Function <vkGetScreenBufferPropertiesQNX> requires <VK_QNX_external_memory_screen_buffer>" );

      StructureChain<X, Y, Z...>                        structureChain;
      VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX & properties = structureChain.template get<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>();
      VULKAN_HPP_NAMESPACE::Result                      result     = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->vkGetScreenBufferPropertiesQNX(
        static_cast<VkDevice>( m_device ), &buffer, reinterpret_cast<VkScreenBufferPropertiesQNX *>( &properties ) ) );
      resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getScreenBufferPropertiesQNX" );

      return structureChain;
    }
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

    //====================
    //=== RAII Helpers ===
    //====================

    template <typename RAIIType>
    std::vector<typename RAIIType::CppType> filterCppTypes( std::vector<RAIIType> const & raiiTypes )
    {
      std::vector<typename RAIIType::CppType> cppTypes( raiiTypes.size() );
      std::transform( raiiTypes.begin(), raiiTypes.end(), cppTypes.begin(), []( RAIIType const & d ) { return *d; } );
      return cppTypes;
    }

    template <typename RAIIType, class UnaryPredicate>
    std::vector<typename RAIIType::CppType> filterCppTypes( std::vector<RAIIType> const & raiiTypes, UnaryPredicate p )
    {
      std::vector<typename RAIIType::CppType> cppTypes;
      for ( auto const & t : raiiTypes )
      {
        if ( p( t ) )
        {
          cppTypes.push_back( *t );
        }
      }
      return cppTypes;
    }

  }  // namespace VULKAN_HPP_RAII_NAMESPACE
}  // namespace VULKAN_HPP_NAMESPACE
#endif
#endif
