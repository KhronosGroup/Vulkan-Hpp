// Copyright 2015-2023 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

module;

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

export module vulkan;

export namespace VULKAN_HPP_NAMESPACE
{

  export namespace VULKAN_HPP_RAII_NAMESPACE
  {
    using VULKAN_HPP_RAII_NAMESPACE::Context;
    using VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher;
    using VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher;
    using VULKAN_HPP_RAII_NAMESPACE::exchange;
    using VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher;

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
    using VULKAN_HPP_RAII_NAMESPACE::DescriptorUpdateTemplate;
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

    //=== VK_EXT_debug_report ===
    using VULKAN_HPP_RAII_NAMESPACE::DebugReportCallbackEXT;

    //=== VK_KHR_video_queue ===
    using VULKAN_HPP_RAII_NAMESPACE::VideoSessionKHR;
    using VULKAN_HPP_RAII_NAMESPACE::VideoSessionParametersKHR;

    //=== VK_NVX_binary_import ===
    using VULKAN_HPP_RAII_NAMESPACE::CuFunctionNVX;
    using VULKAN_HPP_RAII_NAMESPACE::CuModuleNVX;

    //=== VK_EXT_debug_utils ===
    using VULKAN_HPP_RAII_NAMESPACE::DebugUtilsMessengerEXT;

    //=== VK_KHR_acceleration_structure ===
    using VULKAN_HPP_RAII_NAMESPACE::AccelerationStructureKHR;

    //=== VK_EXT_validation_cache ===
    using VULKAN_HPP_RAII_NAMESPACE::ValidationCacheEXT;

    //=== VK_NV_ray_tracing ===
    using VULKAN_HPP_RAII_NAMESPACE::AccelerationStructureNV;

    //=== VK_INTEL_performance_query ===
    using VULKAN_HPP_RAII_NAMESPACE::PerformanceConfigurationINTEL;

    //=== VK_KHR_deferred_host_operations ===
    using VULKAN_HPP_RAII_NAMESPACE::DeferredOperationKHR;

    //=== VK_NV_device_generated_commands ===
    using VULKAN_HPP_RAII_NAMESPACE::IndirectCommandsLayoutNV;

#if defined( VK_USE_PLATFORM_FUCHSIA )
    //=== VK_FUCHSIA_buffer_collection ===
    using VULKAN_HPP_RAII_NAMESPACE::BufferCollectionFUCHSIA;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

       //=== VK_EXT_opacity_micromap ===
    using VULKAN_HPP_RAII_NAMESPACE::MicromapEXT;

    //=== VK_NV_optical_flow ===
    using VULKAN_HPP_RAII_NAMESPACE::OpticalFlowSessionNV;

    //=== VK_EXT_shader_object ===
    using VULKAN_HPP_RAII_NAMESPACE::ShaderEXT;
    using VULKAN_HPP_RAII_NAMESPACE::ShaderEXTs;

  }  // namespace VULKAN_HPP_RAII_NAMESPACE

}  // namespace VULKAN_HPP_NAMESPACE
