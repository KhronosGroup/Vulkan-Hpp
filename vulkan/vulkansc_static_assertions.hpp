// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_STATIC_ASSERTIONS_HPP
#define VULKAN_STATIC_ASSERTIONS_HPP

#include <vulkan/vulkansc.hpp>

//=========================
//=== static_assertions ===
//=========================

//=== VK_VERSION_1_0 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Extent2D ) == sizeof( VkExtent2D ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Extent2D>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Extent2D>::value, "Extent2D is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Extent3D ) == sizeof( VkExtent3D ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Extent3D>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Extent3D>::value, "Extent3D is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Offset2D ) == sizeof( VkOffset2D ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Offset2D>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Offset2D>::value, "Offset2D is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Offset3D ) == sizeof( VkOffset3D ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Offset3D>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Offset3D>::value, "Offset3D is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Rect2D ) == sizeof( VkRect2D ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Rect2D>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Rect2D>::value, "Rect2D is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BaseInStructure ) == sizeof( VkBaseInStructure ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BaseInStructure>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BaseInStructure>::value,
                          "BaseInStructure is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BaseOutStructure ) == sizeof( VkBaseOutStructure ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BaseOutStructure>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BaseOutStructure>::value,
                          "BaseOutStructure is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferMemoryBarrier ) == sizeof( VkBufferMemoryBarrier ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier>::value,
                          "BufferMemoryBarrier is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DispatchIndirectCommand ) == sizeof( VkDispatchIndirectCommand ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DispatchIndirectCommand>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DispatchIndirectCommand>::value,
                          "DispatchIndirectCommand is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand ) == sizeof( VkDrawIndexedIndirectCommand ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrawIndexedIndirectCommand>::value,
                          "DrawIndexedIndirectCommand is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrawIndirectCommand ) == sizeof( VkDrawIndirectCommand ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrawIndirectCommand>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrawIndirectCommand>::value,
                          "DrawIndirectCommand is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageMemoryBarrier ) == sizeof( VkImageMemoryBarrier ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier>::value,
                          "ImageMemoryBarrier is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryBarrier ) == sizeof( VkMemoryBarrier ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryBarrier>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryBarrier>::value, "MemoryBarrier is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne ) == sizeof( VkPipelineCacheHeaderVersionOne ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionOne>::value,
                          "PipelineCacheHeaderVersionOne is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AllocationCallbacks ) == sizeof( VkAllocationCallbacks ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AllocationCallbacks>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AllocationCallbacks>::value,
                          "AllocationCallbacks is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ApplicationInfo ) == sizeof( VkApplicationInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ApplicationInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ApplicationInfo>::value,
                          "ApplicationInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FormatProperties ) == sizeof( VkFormatProperties ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FormatProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FormatProperties>::value,
                          "FormatProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageFormatProperties ) == sizeof( VkImageFormatProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageFormatProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageFormatProperties>::value,
                          "ImageFormatProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Instance ) == sizeof( VkInstance ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Instance>::value, "Instance is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::InstanceCreateInfo ) == sizeof( VkInstanceCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::InstanceCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::InstanceCreateInfo>::value,
                          "InstanceCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryHeap ) == sizeof( VkMemoryHeap ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryHeap>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryHeap>::value, "MemoryHeap is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryType ) == sizeof( VkMemoryType ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryType>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryType>::value, "MemoryType is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevice ) == sizeof( VkPhysicalDevice ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevice>::value,
                          "PhysicalDevice is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures ) == sizeof( VkPhysicalDeviceFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures>::value,
                          "PhysicalDeviceFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits ) == sizeof( VkPhysicalDeviceLimits ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits>::value,
                          "PhysicalDeviceLimits is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties ) == sizeof( VkPhysicalDeviceMemoryProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties>::value,
                          "PhysicalDeviceMemoryProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties ) == sizeof( VkPhysicalDeviceProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties>::value,
                          "PhysicalDeviceProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties ) == sizeof( VkPhysicalDeviceSparseProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties>::value,
                          "PhysicalDeviceSparseProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueueFamilyProperties ) == sizeof( VkQueueFamilyProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::QueueFamilyProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueueFamilyProperties>::value,
                          "QueueFamilyProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Device ) == sizeof( VkDevice ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Device>::value, "Device is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceCreateInfo ) == sizeof( VkDeviceCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceCreateInfo>::value,
                          "DeviceCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo ) == sizeof( VkDeviceQueueCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo>::value,
                          "DeviceQueueCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExtensionProperties ) == sizeof( VkExtensionProperties ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExtensionProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExtensionProperties>::value,
                          "ExtensionProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::LayerProperties ) == sizeof( VkLayerProperties ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::LayerProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::LayerProperties>::value,
                          "LayerProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Queue ) == sizeof( VkQueue ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Queue>::value, "Queue is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubmitInfo ) == sizeof( VkSubmitInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubmitInfo>::value, "SubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MappedMemoryRange ) == sizeof( VkMappedMemoryRange ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MappedMemoryRange>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MappedMemoryRange>::value,
                          "MappedMemoryRange is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryAllocateInfo ) == sizeof( VkMemoryAllocateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryAllocateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryAllocateInfo>::value,
                          "MemoryAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceMemory ) == sizeof( VkDeviceMemory ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceMemory>::value, "DeviceMemory is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryRequirements ) == sizeof( VkMemoryRequirements ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryRequirements>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryRequirements>::value,
                          "MemoryRequirements is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageSubresource ) == sizeof( VkImageSubresource ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageSubresource>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageSubresource>::value,
                          "ImageSubresource is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Fence ) == sizeof( VkFence ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Fence>::value, "Fence is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FenceCreateInfo ) == sizeof( VkFenceCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FenceCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FenceCreateInfo>::value,
                          "FenceCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Semaphore ) == sizeof( VkSemaphore ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Semaphore>::value, "Semaphore is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo ) == sizeof( VkSemaphoreCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo>::value,
                          "SemaphoreCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Event ) == sizeof( VkEvent ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Event>::value, "Event is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::EventCreateInfo ) == sizeof( VkEventCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::EventCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::EventCreateInfo>::value,
                          "EventCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueryPool ) == sizeof( VkQueryPool ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueryPool>::value, "QueryPool is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo ) == sizeof( VkQueryPoolCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo>::value,
                          "QueryPoolCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Buffer ) == sizeof( VkBuffer ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Buffer>::value, "Buffer is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferCreateInfo ) == sizeof( VkBufferCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferCreateInfo>::value,
                          "BufferCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferView ) == sizeof( VkBufferView ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferView>::value, "BufferView is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferViewCreateInfo ) == sizeof( VkBufferViewCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferViewCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferViewCreateInfo>::value,
                          "BufferViewCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Image ) == sizeof( VkImage ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Image>::value, "Image is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageCreateInfo ) == sizeof( VkImageCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageCreateInfo>::value,
                          "ImageCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubresourceLayout ) == sizeof( VkSubresourceLayout ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubresourceLayout>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubresourceLayout>::value,
                          "SubresourceLayout is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ComponentMapping ) == sizeof( VkComponentMapping ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ComponentMapping>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ComponentMapping>::value,
                          "ComponentMapping is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageSubresourceRange ) == sizeof( VkImageSubresourceRange ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageSubresourceRange>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageSubresourceRange>::value,
                          "ImageSubresourceRange is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageView ) == sizeof( VkImageView ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageView>::value, "ImageView is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageViewCreateInfo ) == sizeof( VkImageViewCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageViewCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageViewCreateInfo>::value,
                          "ImageViewCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ShaderModule ) == sizeof( VkShaderModule ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ShaderModule>::value, "ShaderModule is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCache ) == sizeof( VkPipelineCache ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCache>::value, "PipelineCache is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo ) == sizeof( VkPipelineCacheCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo>::value,
                          "PipelineCacheCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo ) == sizeof( VkComputePipelineCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo>::value,
                          "ComputePipelineCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo ) == sizeof( VkGraphicsPipelineCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo>::value,
                          "GraphicsPipelineCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Pipeline ) == sizeof( VkPipeline ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Pipeline>::value, "Pipeline is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState ) == sizeof( VkPipelineColorBlendAttachmentState ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineColorBlendAttachmentState>::value,
                          "PipelineColorBlendAttachmentState is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo ) == sizeof( VkPipelineColorBlendStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo>::value,
                          "PipelineColorBlendStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo ) == sizeof( VkPipelineDepthStencilStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo>::value,
                          "PipelineDepthStencilStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo ) == sizeof( VkPipelineDynamicStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo>::value,
                          "PipelineDynamicStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo ) == sizeof( VkPipelineInputAssemblyStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo>::value,
                          "PipelineInputAssemblyStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo ) == sizeof( VkPipelineMultisampleStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo>::value,
                          "PipelineMultisampleStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo ) == sizeof( VkPipelineRasterizationStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo>::value,
                          "PipelineRasterizationStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo ) == sizeof( VkPipelineShaderStageCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo>::value,
                          "PipelineShaderStageCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo ) == sizeof( VkPipelineTessellationStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo>::value,
                          "PipelineTessellationStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo ) == sizeof( VkPipelineVertexInputStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo>::value,
                          "PipelineVertexInputStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo ) == sizeof( VkPipelineViewportStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo>::value,
                          "PipelineViewportStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SpecializationInfo ) == sizeof( VkSpecializationInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SpecializationInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SpecializationInfo>::value,
                          "SpecializationInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SpecializationMapEntry ) == sizeof( VkSpecializationMapEntry ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SpecializationMapEntry>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SpecializationMapEntry>::value,
                          "SpecializationMapEntry is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::StencilOpState ) == sizeof( VkStencilOpState ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::StencilOpState>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::StencilOpState>::value,
                          "StencilOpState is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription ) == sizeof( VkVertexInputAttributeDescription ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription>::value,
                          "VertexInputAttributeDescription is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::VertexInputBindingDescription ) == sizeof( VkVertexInputBindingDescription ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription>::value,
                          "VertexInputBindingDescription is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Viewport ) == sizeof( VkViewport ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::Viewport>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Viewport>::value, "Viewport is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineLayout ) == sizeof( VkPipelineLayout ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineLayout>::value,
                          "PipelineLayout is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo ) == sizeof( VkPipelineLayoutCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo>::value,
                          "PipelineLayoutCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PushConstantRange ) == sizeof( VkPushConstantRange ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PushConstantRange>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PushConstantRange>::value,
                          "PushConstantRange is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Sampler ) == sizeof( VkSampler ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Sampler>::value, "Sampler is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerCreateInfo ) == sizeof( VkSamplerCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerCreateInfo>::value,
                          "SamplerCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CopyDescriptorSet ) == sizeof( VkCopyDescriptorSet ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CopyDescriptorSet>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CopyDescriptorSet>::value,
                          "CopyDescriptorSet is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorBufferInfo ) == sizeof( VkDescriptorBufferInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorBufferInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorBufferInfo>::value,
                          "DescriptorBufferInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorImageInfo ) == sizeof( VkDescriptorImageInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorImageInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorImageInfo>::value,
                          "DescriptorImageInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorPool ) == sizeof( VkDescriptorPool ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorPool>::value,
                          "DescriptorPool is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo ) == sizeof( VkDescriptorPoolCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo>::value,
                          "DescriptorPoolCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorPoolSize ) == sizeof( VkDescriptorPoolSize ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorPoolSize>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorPoolSize>::value,
                          "DescriptorPoolSize is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSet ) == sizeof( VkDescriptorSet ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSet>::value, "DescriptorSet is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo ) == sizeof( VkDescriptorSetAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo>::value,
                          "DescriptorSetAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetLayout ) == sizeof( VkDescriptorSetLayout ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetLayout>::value,
                          "DescriptorSetLayout is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding ) == sizeof( VkDescriptorSetLayoutBinding ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBinding>::value,
                          "DescriptorSetLayoutBinding is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo ) == sizeof( VkDescriptorSetLayoutCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo>::value,
                          "DescriptorSetLayoutCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::WriteDescriptorSet ) == sizeof( VkWriteDescriptorSet ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::WriteDescriptorSet>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::WriteDescriptorSet>::value,
                          "WriteDescriptorSet is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentDescription ) == sizeof( VkAttachmentDescription ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentDescription>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentDescription>::value,
                          "AttachmentDescription is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentReference ) == sizeof( VkAttachmentReference ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentReference>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentReference>::value,
                          "AttachmentReference is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::Framebuffer ) == sizeof( VkFramebuffer ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::Framebuffer>::value, "Framebuffer is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FramebufferCreateInfo ) == sizeof( VkFramebufferCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FramebufferCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FramebufferCreateInfo>::value,
                          "FramebufferCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPass ) == sizeof( VkRenderPass ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPass>::value, "RenderPass is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo ) == sizeof( VkRenderPassCreateInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo>::value,
                          "RenderPassCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassDependency ) == sizeof( VkSubpassDependency ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassDependency>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassDependency>::value,
                          "SubpassDependency is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassDescription ) == sizeof( VkSubpassDescription ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassDescription>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassDescription>::value,
                          "SubpassDescription is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandPool ) == sizeof( VkCommandPool ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandPool>::value, "CommandPool is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo ) == sizeof( VkCommandPoolCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo>::value,
                          "CommandPoolCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBuffer ) == sizeof( VkCommandBuffer ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBuffer>::value, "CommandBuffer is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo ) == sizeof( VkCommandBufferAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo>::value,
                          "CommandBufferAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo ) == sizeof( VkCommandBufferBeginInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo>::value,
                          "CommandBufferBeginInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo ) == sizeof( VkCommandBufferInheritanceInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceInfo>::value,
                          "CommandBufferInheritanceInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferCopy ) == sizeof( VkBufferCopy ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferCopy>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferCopy>::value, "BufferCopy is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferImageCopy ) == sizeof( VkBufferImageCopy ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferImageCopy>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferImageCopy>::value,
                          "BufferImageCopy is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ClearAttachment ) == sizeof( VkClearAttachment ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ClearAttachment>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ClearAttachment>::value,
                          "ClearAttachment is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ClearColorValue ) == sizeof( VkClearColorValue ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ClearColorValue>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ClearColorValue>::value,
                          "ClearColorValue is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ClearDepthStencilValue ) == sizeof( VkClearDepthStencilValue ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ClearDepthStencilValue>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ClearDepthStencilValue>::value,
                          "ClearDepthStencilValue is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ClearRect ) == sizeof( VkClearRect ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ClearRect>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ClearRect>::value, "ClearRect is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ClearValue ) == sizeof( VkClearValue ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ClearValue>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ClearValue>::value, "ClearValue is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageBlit ) == sizeof( VkImageBlit ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageBlit>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageBlit>::value, "ImageBlit is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageCopy ) == sizeof( VkImageCopy ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageCopy>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageCopy>::value, "ImageCopy is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageResolve ) == sizeof( VkImageResolve ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageResolve>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageResolve>::value, "ImageResolve is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageSubresourceLayers ) == sizeof( VkImageSubresourceLayers ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageSubresourceLayers>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageSubresourceLayers>::value,
                          "ImageSubresourceLayers is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassBeginInfo ) == sizeof( VkRenderPassBeginInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassBeginInfo>::value,
                          "RenderPassBeginInfo is not nothrow_move_constructible!" );

//=== VK_VERSION_1_1 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties ) == sizeof( VkPhysicalDeviceSubgroupProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupProperties>::value,
                          "PhysicalDeviceSubgroupProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo ) == sizeof( VkBindBufferMemoryInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo>::value,
                          "BindBufferMemoryInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindImageMemoryInfo ) == sizeof( VkBindImageMemoryInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindImageMemoryInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindImageMemoryInfo>::value,
                          "BindImageMemoryInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures ) == sizeof( VkPhysicalDevice16BitStorageFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevice16BitStorageFeatures>::value,
                          "PhysicalDevice16BitStorageFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements ) == sizeof( VkMemoryDedicatedRequirements ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryDedicatedRequirements>::value,
                          "MemoryDedicatedRequirements is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo ) == sizeof( VkMemoryDedicatedAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryDedicatedAllocateInfo>::value,
                          "MemoryDedicatedAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo ) == sizeof( VkMemoryAllocateFlagsInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryAllocateFlagsInfo>::value,
                          "MemoryAllocateFlagsInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo ) == sizeof( VkDeviceGroupRenderPassBeginInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupRenderPassBeginInfo>::value,
                          "DeviceGroupRenderPassBeginInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo ) == sizeof( VkDeviceGroupCommandBufferBeginInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupCommandBufferBeginInfo>::value,
                          "DeviceGroupCommandBufferBeginInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo ) == sizeof( VkDeviceGroupSubmitInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupSubmitInfo>::value,
                          "DeviceGroupSubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo ) == sizeof( VkBindBufferMemoryDeviceGroupInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindBufferMemoryDeviceGroupInfo>::value,
                          "BindBufferMemoryDeviceGroupInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo ) == sizeof( VkBindImageMemoryDeviceGroupInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindImageMemoryDeviceGroupInfo>::value,
                          "BindImageMemoryDeviceGroupInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties ) == sizeof( VkPhysicalDeviceGroupProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties>::value,
                          "PhysicalDeviceGroupProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo ) == sizeof( VkDeviceGroupDeviceCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupDeviceCreateInfo>::value,
                          "DeviceGroupDeviceCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 ) == sizeof( VkBufferMemoryRequirementsInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2>::value,
                          "BufferMemoryRequirementsInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 ) == sizeof( VkImageMemoryRequirementsInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2>::value,
                          "ImageMemoryRequirementsInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryRequirements2 ) == sizeof( VkMemoryRequirements2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryRequirements2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryRequirements2>::value,
                          "MemoryRequirements2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 ) == sizeof( VkPhysicalDeviceFeatures2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2>::value,
                          "PhysicalDeviceFeatures2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 ) == sizeof( VkPhysicalDeviceProperties2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2>::value,
                          "PhysicalDeviceProperties2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FormatProperties2 ) == sizeof( VkFormatProperties2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FormatProperties2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FormatProperties2>::value,
                          "FormatProperties2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageFormatProperties2 ) == sizeof( VkImageFormatProperties2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>::value,
                          "ImageFormatProperties2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 ) == sizeof( VkPhysicalDeviceImageFormatInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2>::value,
                          "PhysicalDeviceImageFormatInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueueFamilyProperties2 ) == sizeof( VkQueueFamilyProperties2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>::value,
                          "QueueFamilyProperties2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 ) == sizeof( VkPhysicalDeviceMemoryProperties2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2>::value,
                          "PhysicalDeviceMemoryProperties2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties ) == sizeof( VkPhysicalDevicePointClippingProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePointClippingProperties>::value,
                          "PhysicalDevicePointClippingProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo ) == sizeof( VkRenderPassInputAttachmentAspectCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassInputAttachmentAspectCreateInfo>::value,
                          "RenderPassInputAttachmentAspectCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference ) == sizeof( VkInputAttachmentAspectReference ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::InputAttachmentAspectReference>::value,
                          "InputAttachmentAspectReference is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo ) == sizeof( VkImageViewUsageCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageViewUsageCreateInfo>::value,
                          "ImageViewUsageCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo ) ==
                            sizeof( VkPipelineTessellationDomainOriginStateCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineTessellationDomainOriginStateCreateInfo>::value,
                          "PipelineTessellationDomainOriginStateCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo ) == sizeof( VkRenderPassMultiviewCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassMultiviewCreateInfo>::value,
                          "RenderPassMultiviewCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures ) == sizeof( VkPhysicalDeviceMultiviewFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewFeatures>::value,
                          "PhysicalDeviceMultiviewFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties ) == sizeof( VkPhysicalDeviceMultiviewProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMultiviewProperties>::value,
                          "PhysicalDeviceMultiviewProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures ) == sizeof( VkPhysicalDeviceVariablePointersFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVariablePointersFeatures>::value,
                          "PhysicalDeviceVariablePointersFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures ) == sizeof( VkPhysicalDeviceProtectedMemoryFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryFeatures>::value,
                          "PhysicalDeviceProtectedMemoryFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties ) == sizeof( VkPhysicalDeviceProtectedMemoryProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceProtectedMemoryProperties>::value,
                          "PhysicalDeviceProtectedMemoryProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 ) == sizeof( VkDeviceQueueInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceQueueInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceQueueInfo2>::value,
                          "DeviceQueueInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo ) == sizeof( VkProtectedSubmitInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ProtectedSubmitInfo>::value,
                          "ProtectedSubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo ) == sizeof( VkSamplerYcbcrConversionCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo>::value,
                          "SamplerYcbcrConversionCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo ) == sizeof( VkSamplerYcbcrConversionInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionInfo>::value,
                          "SamplerYcbcrConversionInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo ) == sizeof( VkBindImagePlaneMemoryInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindImagePlaneMemoryInfo>::value,
                          "BindImagePlaneMemoryInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo ) == sizeof( VkImagePlaneMemoryRequirementsInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImagePlaneMemoryRequirementsInfo>::value,
                          "ImagePlaneMemoryRequirementsInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures ) ==
                            sizeof( VkPhysicalDeviceSamplerYcbcrConversionFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerYcbcrConversionFeatures>::value,
                          "PhysicalDeviceSamplerYcbcrConversionFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties ) ==
                            sizeof( VkSamplerYcbcrConversionImageFormatProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionImageFormatProperties>::value,
                          "SamplerYcbcrConversionImageFormatProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion ) == sizeof( VkSamplerYcbcrConversion ),
                          "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion>::value,
                          "SamplerYcbcrConversion is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalMemoryProperties ) == sizeof( VkExternalMemoryProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalMemoryProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalMemoryProperties>::value,
                          "ExternalMemoryProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo ) == sizeof( VkPhysicalDeviceExternalImageFormatInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalImageFormatInfo>::value,
                          "PhysicalDeviceExternalImageFormatInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties ) == sizeof( VkExternalImageFormatProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalImageFormatProperties>::value,
                          "ExternalImageFormatProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo ) == sizeof( VkPhysicalDeviceExternalBufferInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo>::value,
                          "PhysicalDeviceExternalBufferInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalBufferProperties ) == sizeof( VkExternalBufferProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalBufferProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalBufferProperties>::value,
                          "ExternalBufferProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties ) == sizeof( VkPhysicalDeviceIDProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceIDProperties>::value,
                          "PhysicalDeviceIDProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo ) == sizeof( VkExternalMemoryImageCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalMemoryImageCreateInfo>::value,
                          "ExternalMemoryImageCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo ) == sizeof( VkExternalMemoryBufferCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalMemoryBufferCreateInfo>::value,
                          "ExternalMemoryBufferCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo ) == sizeof( VkExportMemoryAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportMemoryAllocateInfo>::value,
                          "ExportMemoryAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo ) == sizeof( VkPhysicalDeviceExternalFenceInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo>::value,
                          "PhysicalDeviceExternalFenceInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalFenceProperties ) == sizeof( VkExternalFenceProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalFenceProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalFenceProperties>::value,
                          "ExternalFenceProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo ) == sizeof( VkExportFenceCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportFenceCreateInfo>::value,
                          "ExportFenceCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo ) == sizeof( VkExportSemaphoreCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportSemaphoreCreateInfo>::value,
                          "ExportSemaphoreCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo ) == sizeof( VkPhysicalDeviceExternalSemaphoreInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo>::value,
                          "PhysicalDeviceExternalSemaphoreInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties ) == sizeof( VkExternalSemaphoreProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties>::value,
                          "ExternalSemaphoreProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties ) == sizeof( VkPhysicalDeviceMaintenance3Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance3Properties>::value,
                          "PhysicalDeviceMaintenance3Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport ) == sizeof( VkDescriptorSetLayoutSupport ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport>::value,
                          "DescriptorSetLayoutSupport is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures ) == sizeof( VkPhysicalDeviceShaderDrawParametersFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDrawParametersFeatures>::value,
                          "PhysicalDeviceShaderDrawParametersFeatures is not nothrow_move_constructible!" );

//=== VK_VERSION_1_2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features ) == sizeof( VkPhysicalDeviceVulkan11Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Features>::value,
                          "PhysicalDeviceVulkan11Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties ) == sizeof( VkPhysicalDeviceVulkan11Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan11Properties>::value,
                          "PhysicalDeviceVulkan11Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features ) == sizeof( VkPhysicalDeviceVulkan12Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Features>::value,
                          "PhysicalDeviceVulkan12Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties ) == sizeof( VkPhysicalDeviceVulkan12Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan12Properties>::value,
                          "PhysicalDeviceVulkan12Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo ) == sizeof( VkImageFormatListCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageFormatListCreateInfo>::value,
                          "ImageFormatListCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 ) == sizeof( VkRenderPassCreateInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2>::value,
                          "RenderPassCreateInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentDescription2 ) == sizeof( VkAttachmentDescription2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentDescription2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentDescription2>::value,
                          "AttachmentDescription2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentReference2 ) == sizeof( VkAttachmentReference2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentReference2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentReference2>::value,
                          "AttachmentReference2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassDescription2 ) == sizeof( VkSubpassDescription2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassDescription2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassDescription2>::value,
                          "SubpassDescription2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassDependency2 ) == sizeof( VkSubpassDependency2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassDependency2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassDependency2>::value,
                          "SubpassDependency2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassBeginInfo ) == sizeof( VkSubpassBeginInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassBeginInfo>::value,
                          "SubpassBeginInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassEndInfo ) == sizeof( VkSubpassEndInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassEndInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassEndInfo>::value,
                          "SubpassEndInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures ) == sizeof( VkPhysicalDevice8BitStorageFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevice8BitStorageFeatures>::value,
                          "PhysicalDevice8BitStorageFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ConformanceVersion ) == sizeof( VkConformanceVersion ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ConformanceVersion>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ConformanceVersion>::value,
                          "ConformanceVersion is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties ) == sizeof( VkPhysicalDeviceDriverProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDriverProperties>::value,
                          "PhysicalDeviceDriverProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features ) == sizeof( VkPhysicalDeviceShaderAtomicInt64Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicInt64Features>::value,
                          "PhysicalDeviceShaderAtomicInt64Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features ) == sizeof( VkPhysicalDeviceShaderFloat16Int8Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderFloat16Int8Features>::value,
                          "PhysicalDeviceShaderFloat16Int8Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties ) == sizeof( VkPhysicalDeviceFloatControlsProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFloatControlsProperties>::value,
                          "PhysicalDeviceFloatControlsProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo ) == sizeof( VkDescriptorSetLayoutBindingFlagsCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutBindingFlagsCreateInfo>::value,
                          "DescriptorSetLayoutBindingFlagsCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures ) == sizeof( VkPhysicalDeviceDescriptorIndexingFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingFeatures>::value,
                          "PhysicalDeviceDescriptorIndexingFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties ) == sizeof( VkPhysicalDeviceDescriptorIndexingProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDescriptorIndexingProperties>::value,
                          "PhysicalDeviceDescriptorIndexingProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo ) ==
                            sizeof( VkDescriptorSetVariableDescriptorCountAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountAllocateInfo>::value,
                          "DescriptorSetVariableDescriptorCountAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport ) ==
                            sizeof( VkDescriptorSetVariableDescriptorCountLayoutSupport ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorSetVariableDescriptorCountLayoutSupport>::value,
                          "DescriptorSetVariableDescriptorCountLayoutSupport is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve ) == sizeof( VkSubpassDescriptionDepthStencilResolve ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassDescriptionDepthStencilResolve>::value,
                          "SubpassDescriptionDepthStencilResolve is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties ) ==
                            sizeof( VkPhysicalDeviceDepthStencilResolveProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthStencilResolveProperties>::value,
                          "PhysicalDeviceDepthStencilResolveProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures ) == sizeof( VkPhysicalDeviceScalarBlockLayoutFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceScalarBlockLayoutFeatures>::value,
                          "PhysicalDeviceScalarBlockLayoutFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo ) == sizeof( VkImageStencilUsageCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageStencilUsageCreateInfo>::value,
                          "ImageStencilUsageCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo ) == sizeof( VkSamplerReductionModeCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerReductionModeCreateInfo>::value,
                          "SamplerReductionModeCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties ) ==
                            sizeof( VkPhysicalDeviceSamplerFilterMinmaxProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSamplerFilterMinmaxProperties>::value,
                          "PhysicalDeviceSamplerFilterMinmaxProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures ) == sizeof( VkPhysicalDeviceVulkanMemoryModelFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanMemoryModelFeatures>::value,
                          "PhysicalDeviceVulkanMemoryModelFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures ) == sizeof( VkPhysicalDeviceImagelessFramebufferFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceImagelessFramebufferFeatures>::value,
                          "PhysicalDeviceImagelessFramebufferFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo ) == sizeof( VkFramebufferAttachmentsCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FramebufferAttachmentsCreateInfo>::value,
                          "FramebufferAttachmentsCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo ) == sizeof( VkFramebufferAttachmentImageInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FramebufferAttachmentImageInfo>::value,
                          "FramebufferAttachmentImageInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo ) == sizeof( VkRenderPassAttachmentBeginInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassAttachmentBeginInfo>::value,
                          "RenderPassAttachmentBeginInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures ) ==
                            sizeof( VkPhysicalDeviceUniformBufferStandardLayoutFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceUniformBufferStandardLayoutFeatures>::value,
                          "PhysicalDeviceUniformBufferStandardLayoutFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures ) ==
                            sizeof( VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>::value,
                          "PhysicalDeviceShaderSubgroupExtendedTypesFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures ) ==
                            sizeof( VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>::value,
                          "PhysicalDeviceSeparateDepthStencilLayoutsFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout ) == sizeof( VkAttachmentReferenceStencilLayout ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentReferenceStencilLayout>::value,
                          "AttachmentReferenceStencilLayout is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout ) == sizeof( VkAttachmentDescriptionStencilLayout ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentDescriptionStencilLayout>::value,
                          "AttachmentDescriptionStencilLayout is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures ) == sizeof( VkPhysicalDeviceHostQueryResetFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceHostQueryResetFeatures>::value,
                          "PhysicalDeviceHostQueryResetFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures ) == sizeof( VkPhysicalDeviceTimelineSemaphoreFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreFeatures>::value,
                          "PhysicalDeviceTimelineSemaphoreFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties ) == sizeof( VkPhysicalDeviceTimelineSemaphoreProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceTimelineSemaphoreProperties>::value,
                          "PhysicalDeviceTimelineSemaphoreProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo ) == sizeof( VkSemaphoreTypeCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreTypeCreateInfo>::value,
                          "SemaphoreTypeCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo ) == sizeof( VkTimelineSemaphoreSubmitInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::TimelineSemaphoreSubmitInfo>::value,
                          "TimelineSemaphoreSubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo ) == sizeof( VkSemaphoreWaitInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo>::value,
                          "SemaphoreWaitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo ) == sizeof( VkSemaphoreSignalInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo>::value,
                          "SemaphoreSignalInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures ) == sizeof( VkPhysicalDeviceBufferDeviceAddressFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceBufferDeviceAddressFeatures>::value,
                          "PhysicalDeviceBufferDeviceAddressFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo ) == sizeof( VkBufferDeviceAddressInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo>::value,
                          "BufferDeviceAddressInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo ) == sizeof( VkBufferOpaqueCaptureAddressCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferOpaqueCaptureAddressCreateInfo>::value,
                          "BufferOpaqueCaptureAddressCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo ) == sizeof( VkMemoryOpaqueCaptureAddressAllocateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryOpaqueCaptureAddressAllocateInfo>::value,
                          "MemoryOpaqueCaptureAddressAllocateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo ) == sizeof( VkDeviceMemoryOpaqueCaptureAddressInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo>::value,
                          "DeviceMemoryOpaqueCaptureAddressInfo is not nothrow_move_constructible!" );

//=== VK_VERSION_1_3 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features ) == sizeof( VkPhysicalDeviceVulkan13Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Features>::value,
                          "PhysicalDeviceVulkan13Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties ) == sizeof( VkPhysicalDeviceVulkan13Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkan13Properties>::value,
                          "PhysicalDeviceVulkan13Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo ) == sizeof( VkPipelineCreationFeedbackCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCreationFeedbackCreateInfo>::value,
                          "PipelineCreationFeedbackCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCreationFeedback ) == sizeof( VkPipelineCreationFeedback ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCreationFeedback>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCreationFeedback>::value,
                          "PipelineCreationFeedback is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures ) ==
                            sizeof( VkPhysicalDeviceShaderTerminateInvocationFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderTerminateInvocationFeatures>::value,
                          "PhysicalDeviceShaderTerminateInvocationFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties ) == sizeof( VkPhysicalDeviceToolProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties>::value,
                          "PhysicalDeviceToolProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures ) ==
                            sizeof( VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderDemoteToHelperInvocationFeatures>::value,
                          "PhysicalDeviceShaderDemoteToHelperInvocationFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures ) == sizeof( VkPhysicalDevicePrivateDataFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePrivateDataFeatures>::value,
                          "PhysicalDevicePrivateDataFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo ) == sizeof( VkDevicePrivateDataCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DevicePrivateDataCreateInfo>::value,
                          "DevicePrivateDataCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo ) == sizeof( VkPrivateDataSlotCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo>::value,
                          "PrivateDataSlotCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PrivateDataSlot ) == sizeof( VkPrivateDataSlot ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PrivateDataSlot>::value,
                          "PrivateDataSlot is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures ) ==
                            sizeof( VkPhysicalDevicePipelineCreationCacheControlFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePipelineCreationCacheControlFeatures>::value,
                          "PhysicalDevicePipelineCreationCacheControlFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryBarrier2 ) == sizeof( VkMemoryBarrier2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryBarrier2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryBarrier2>::value,
                          "MemoryBarrier2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2 ) == sizeof( VkBufferMemoryBarrier2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferMemoryBarrier2>::value,
                          "BufferMemoryBarrier2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2 ) == sizeof( VkImageMemoryBarrier2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageMemoryBarrier2>::value,
                          "ImageMemoryBarrier2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DependencyInfo ) == sizeof( VkDependencyInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DependencyInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DependencyInfo>::value,
                          "DependencyInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubmitInfo2 ) == sizeof( VkSubmitInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubmitInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubmitInfo2>::value, "SubmitInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo ) == sizeof( VkSemaphoreSubmitInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreSubmitInfo>::value,
                          "SemaphoreSubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo ) == sizeof( VkCommandBufferSubmitInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBufferSubmitInfo>::value,
                          "CommandBufferSubmitInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features ) == sizeof( VkPhysicalDeviceSynchronization2Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSynchronization2Features>::value,
                          "PhysicalDeviceSynchronization2Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures ) ==
                            sizeof( VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures>::value,
                          "PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures ) == sizeof( VkPhysicalDeviceImageRobustnessFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageRobustnessFeatures>::value,
                          "PhysicalDeviceImageRobustnessFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CopyBufferInfo2 ) == sizeof( VkCopyBufferInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CopyBufferInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CopyBufferInfo2>::value,
                          "CopyBufferInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CopyImageInfo2 ) == sizeof( VkCopyImageInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CopyImageInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CopyImageInfo2>::value,
                          "CopyImageInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 ) == sizeof( VkCopyBufferToImageInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2>::value,
                          "CopyBufferToImageInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 ) == sizeof( VkCopyImageToBufferInfo2 ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2>::value,
                          "CopyImageToBufferInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BlitImageInfo2 ) == sizeof( VkBlitImageInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BlitImageInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BlitImageInfo2>::value,
                          "BlitImageInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ResolveImageInfo2 ) == sizeof( VkResolveImageInfo2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ResolveImageInfo2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ResolveImageInfo2>::value,
                          "ResolveImageInfo2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferCopy2 ) == sizeof( VkBufferCopy2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferCopy2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferCopy2>::value, "BufferCopy2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageCopy2 ) == sizeof( VkImageCopy2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageCopy2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageCopy2>::value, "ImageCopy2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageBlit2 ) == sizeof( VkImageBlit2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageBlit2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageBlit2>::value, "ImageBlit2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BufferImageCopy2 ) == sizeof( VkBufferImageCopy2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BufferImageCopy2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BufferImageCopy2>::value,
                          "BufferImageCopy2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageResolve2 ) == sizeof( VkImageResolve2 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageResolve2>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageResolve2>::value, "ImageResolve2 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures ) == sizeof( VkPhysicalDeviceSubgroupSizeControlFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlFeatures>::value,
                          "PhysicalDeviceSubgroupSizeControlFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties ) ==
                            sizeof( VkPhysicalDeviceSubgroupSizeControlProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSubgroupSizeControlProperties>::value,
                          "PhysicalDeviceSubgroupSizeControlProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo ) ==
                            sizeof( VkPipelineShaderStageRequiredSubgroupSizeCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineShaderStageRequiredSubgroupSizeCreateInfo>::value,
                          "PipelineShaderStageRequiredSubgroupSizeCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures ) == sizeof( VkPhysicalDeviceInlineUniformBlockFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockFeatures>::value,
                          "PhysicalDeviceInlineUniformBlockFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties ) == sizeof( VkPhysicalDeviceInlineUniformBlockProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceInlineUniformBlockProperties>::value,
                          "PhysicalDeviceInlineUniformBlockProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock ) == sizeof( VkWriteDescriptorSetInlineUniformBlock ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::WriteDescriptorSetInlineUniformBlock>::value,
                          "WriteDescriptorSetInlineUniformBlock is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo ) == sizeof( VkDescriptorPoolInlineUniformBlockCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DescriptorPoolInlineUniformBlockCreateInfo>::value,
                          "DescriptorPoolInlineUniformBlockCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures ) ==
                            sizeof( VkPhysicalDeviceTextureCompressionASTCHDRFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceTextureCompressionASTCHDRFeatures>::value,
                          "PhysicalDeviceTextureCompressionASTCHDRFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderingInfo ) == sizeof( VkRenderingInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderingInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderingInfo>::value, "RenderingInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderingAttachmentInfo ) == sizeof( VkRenderingAttachmentInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderingAttachmentInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderingAttachmentInfo>::value,
                          "RenderingAttachmentInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo ) == sizeof( VkPipelineRenderingCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineRenderingCreateInfo>::value,
                          "PipelineRenderingCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures ) == sizeof( VkPhysicalDeviceDynamicRenderingFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDynamicRenderingFeatures>::value,
                          "PhysicalDeviceDynamicRenderingFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo ) == sizeof( VkCommandBufferInheritanceRenderingInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandBufferInheritanceRenderingInfo>::value,
                          "CommandBufferInheritanceRenderingInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures ) ==
                            sizeof( VkPhysicalDeviceShaderIntegerDotProductFeatures ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductFeatures>::value,
                          "PhysicalDeviceShaderIntegerDotProductFeatures is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties ) ==
                            sizeof( VkPhysicalDeviceShaderIntegerDotProductProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderIntegerDotProductProperties>::value,
                          "PhysicalDeviceShaderIntegerDotProductProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties ) ==
                            sizeof( VkPhysicalDeviceTexelBufferAlignmentProperties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentProperties>::value,
                          "PhysicalDeviceTexelBufferAlignmentProperties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FormatProperties3 ) == sizeof( VkFormatProperties3 ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FormatProperties3>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FormatProperties3>::value,
                          "FormatProperties3 is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features ) == sizeof( VkPhysicalDeviceMaintenance4Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Features>::value,
                          "PhysicalDeviceMaintenance4Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties ) == sizeof( VkPhysicalDeviceMaintenance4Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMaintenance4Properties>::value,
                          "PhysicalDeviceMaintenance4Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements ) == sizeof( VkDeviceBufferMemoryRequirements ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements>::value,
                          "DeviceBufferMemoryRequirements is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements ) == sizeof( VkDeviceImageMemoryRequirements ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements>::value,
                          "DeviceImageMemoryRequirements is not nothrow_move_constructible!" );

//=== VKSC_VERSION_1_0 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features ) == sizeof( VkPhysicalDeviceVulkanSC10Features ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Features>::value,
                          "PhysicalDeviceVulkanSC10Features is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties ) == sizeof( VkPhysicalDeviceVulkanSC10Properties ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVulkanSC10Properties>::value,
                          "PhysicalDeviceVulkanSC10Properties is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo ) == sizeof( VkDeviceObjectReservationCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceObjectReservationCreateInfo>::value,
                          "DeviceObjectReservationCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo ) == sizeof( VkCommandPoolMemoryReservationCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandPoolMemoryReservationCreateInfo>::value,
                          "CommandPoolMemoryReservationCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption ) == sizeof( VkCommandPoolMemoryConsumption ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption>::value,
                          "CommandPoolMemoryConsumption is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelinePoolSize ) == sizeof( VkPipelinePoolSize ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelinePoolSize>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelinePoolSize>::value,
                          "PipelinePoolSize is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FaultData ) == sizeof( VkFaultData ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FaultData>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FaultData>::value, "FaultData is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FaultCallbackInfo ) == sizeof( VkFaultCallbackInfo ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FaultCallbackInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FaultCallbackInfo>::value,
                          "FaultCallbackInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo ) == sizeof( VkPipelineOfflineCreateInfo ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineOfflineCreateInfo>::value,
                          "PipelineOfflineCreateInfo is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry ) == sizeof( VkPipelineCacheStageValidationIndexEntry ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCacheStageValidationIndexEntry>::value,
                          "PipelineCacheStageValidationIndexEntry is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry ) == sizeof( VkPipelineCacheSafetyCriticalIndexEntry ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCacheSafetyCriticalIndexEntry>::value,
                          "PipelineCacheSafetyCriticalIndexEntry is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne ) ==
                            sizeof( VkPipelineCacheHeaderVersionSafetyCriticalOne ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineCacheHeaderVersionSafetyCriticalOne>::value,
                          "PipelineCacheHeaderVersionSafetyCriticalOne is not nothrow_move_constructible!" );

//=== VK_KHR_surface ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceKHR ) == sizeof( VkSurfaceKHR ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceKHR>::value, "SurfaceKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR ) == sizeof( VkSurfaceCapabilitiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR>::value,
                          "SurfaceCapabilitiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceFormatKHR ) == sizeof( VkSurfaceFormatKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>::value,
                          "SurfaceFormatKHR is not nothrow_move_constructible!" );

//=== VK_KHR_swapchain ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR ) == sizeof( VkSwapchainCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR>::value,
                          "SwapchainCreateInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SwapchainKHR ) == sizeof( VkSwapchainKHR ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SwapchainKHR>::value, "SwapchainKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PresentInfoKHR ) == sizeof( VkPresentInfoKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PresentInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PresentInfoKHR>::value,
                          "PresentInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR ) == sizeof( VkImageSwapchainCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageSwapchainCreateInfoKHR>::value,
                          "ImageSwapchainCreateInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR ) == sizeof( VkBindImageMemorySwapchainInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::BindImageMemorySwapchainInfoKHR>::value,
                          "BindImageMemorySwapchainInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR ) == sizeof( VkAcquireNextImageInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR>::value,
                          "AcquireNextImageInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR ) == sizeof( VkDeviceGroupPresentCapabilitiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR>::value,
                          "DeviceGroupPresentCapabilitiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR ) == sizeof( VkDeviceGroupPresentInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupPresentInfoKHR>::value,
                          "DeviceGroupPresentInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR ) == sizeof( VkDeviceGroupSwapchainCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceGroupSwapchainCreateInfoKHR>::value,
                          "DeviceGroupSwapchainCreateInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_display ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayKHR ) == sizeof( VkDisplayKHR ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayKHR>::value, "DisplayKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR ) == sizeof( VkDisplayModeCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR>::value,
                          "DisplayModeCreateInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayModeKHR ) == sizeof( VkDisplayModeKHR ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayModeKHR>::value,
                          "DisplayModeKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR ) == sizeof( VkDisplayModeParametersKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayModeParametersKHR>::value,
                          "DisplayModeParametersKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR ) == sizeof( VkDisplayModePropertiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR>::value,
                          "DisplayModePropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR ) == sizeof( VkDisplayPlaneCapabilitiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR>::value,
                          "DisplayPlaneCapabilitiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR ) == sizeof( VkDisplayPlanePropertiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR>::value,
                          "DisplayPlanePropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR ) == sizeof( VkDisplayPropertiesKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR>::value,
                          "DisplayPropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR ) == sizeof( VkDisplaySurfaceCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR>::value,
                          "DisplaySurfaceCreateInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_display_swapchain ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR ) == sizeof( VkDisplayPresentInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPresentInfoKHR>::value,
                          "DisplayPresentInfoKHR is not nothrow_move_constructible!" );

//=== VK_EXT_astc_decode_mode ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT ) == sizeof( VkImageViewASTCDecodeModeEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageViewASTCDecodeModeEXT>::value,
                          "ImageViewASTCDecodeModeEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT ) == sizeof( VkPhysicalDeviceASTCDecodeFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceASTCDecodeFeaturesEXT>::value,
                          "PhysicalDeviceASTCDecodeFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_KHR_external_memory_fd ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR ) == sizeof( VkImportMemoryFdInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportMemoryFdInfoKHR>::value,
                          "ImportMemoryFdInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR ) == sizeof( VkMemoryFdPropertiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR>::value,
                          "MemoryFdPropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR ) == sizeof( VkMemoryGetFdInfoKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR>::value,
                          "MemoryGetFdInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_external_semaphore_fd ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR ) == sizeof( VkImportSemaphoreFdInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR>::value,
                          "ImportSemaphoreFdInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR ) == sizeof( VkSemaphoreGetFdInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR>::value,
                          "SemaphoreGetFdInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_incremental_present ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PresentRegionsKHR ) == sizeof( VkPresentRegionsKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PresentRegionsKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PresentRegionsKHR>::value,
                          "PresentRegionsKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PresentRegionKHR ) == sizeof( VkPresentRegionKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PresentRegionKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PresentRegionKHR>::value,
                          "PresentRegionKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RectLayerKHR ) == sizeof( VkRectLayerKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RectLayerKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RectLayerKHR>::value, "RectLayerKHR is not nothrow_move_constructible!" );

//=== VK_EXT_display_surface_counter ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT ) == sizeof( VkSurfaceCapabilities2EXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT>::value,
                          "SurfaceCapabilities2EXT is not nothrow_move_constructible!" );

//=== VK_EXT_display_control ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT ) == sizeof( VkDisplayPowerInfoEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT>::value,
                          "DisplayPowerInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT ) == sizeof( VkDeviceEventInfoEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT>::value,
                          "DeviceEventInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT ) == sizeof( VkDisplayEventInfoEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT>::value,
                          "DisplayEventInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT ) == sizeof( VkSwapchainCounterCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SwapchainCounterCreateInfoEXT>::value,
                          "SwapchainCounterCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_discard_rectangles ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceDiscardRectanglePropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDiscardRectanglePropertiesEXT>::value,
                          "PhysicalDeviceDiscardRectanglePropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT ) == sizeof( VkPipelineDiscardRectangleStateCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineDiscardRectangleStateCreateInfoEXT>::value,
                          "PipelineDiscardRectangleStateCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_conservative_rasterization ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceConservativeRasterizationPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceConservativeRasterizationPropertiesEXT>::value,
                          "PhysicalDeviceConservativeRasterizationPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT ) ==
                            sizeof( VkPipelineRasterizationConservativeStateCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineRasterizationConservativeStateCreateInfoEXT>::value,
                          "PipelineRasterizationConservativeStateCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_depth_clip_enable ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT ) == sizeof( VkPhysicalDeviceDepthClipEnableFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceDepthClipEnableFeaturesEXT>::value,
                          "PhysicalDeviceDepthClipEnableFeaturesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT ) ==
                            sizeof( VkPipelineRasterizationDepthClipStateCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineRasterizationDepthClipStateCreateInfoEXT>::value,
                          "PipelineRasterizationDepthClipStateCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_hdr_metadata ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::HdrMetadataEXT ) == sizeof( VkHdrMetadataEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::HdrMetadataEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::HdrMetadataEXT>::value,
                          "HdrMetadataEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::XYColorEXT ) == sizeof( VkXYColorEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::XYColorEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::XYColorEXT>::value, "XYColorEXT is not nothrow_move_constructible!" );

//=== VK_KHR_shared_presentable_image ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR ) == sizeof( VkSharedPresentSurfaceCapabilitiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SharedPresentSurfaceCapabilitiesKHR>::value,
                          "SharedPresentSurfaceCapabilitiesKHR is not nothrow_move_constructible!" );

//=== VK_KHR_external_fence_fd ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR ) == sizeof( VkImportFenceFdInfoKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR>::value,
                          "ImportFenceFdInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR ) == sizeof( VkFenceGetFdInfoKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR>::value,
                          "FenceGetFdInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_performance_query ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR ) == sizeof( VkPhysicalDevicePerformanceQueryFeaturesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryFeaturesKHR>::value,
                          "PhysicalDevicePerformanceQueryFeaturesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR ) ==
                            sizeof( VkPhysicalDevicePerformanceQueryPropertiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePerformanceQueryPropertiesKHR>::value,
                          "PhysicalDevicePerformanceQueryPropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PerformanceCounterKHR ) == sizeof( VkPerformanceCounterKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>::value,
                          "PerformanceCounterKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR ) == sizeof( VkPerformanceCounterDescriptionKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>::value,
                          "PerformanceCounterDescriptionKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR ) == sizeof( VkQueryPoolPerformanceCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR>::value,
                          "QueryPoolPerformanceCreateInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PerformanceCounterResultKHR ) == sizeof( VkPerformanceCounterResultKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PerformanceCounterResultKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PerformanceCounterResultKHR>::value,
                          "PerformanceCounterResultKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR ) == sizeof( VkAcquireProfilingLockInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR>::value,
                          "AcquireProfilingLockInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR ) == sizeof( VkPerformanceQuerySubmitInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PerformanceQuerySubmitInfoKHR>::value,
                          "PerformanceQuerySubmitInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR ) == sizeof( VkPerformanceQueryReservationInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PerformanceQueryReservationInfoKHR>::value,
                          "PerformanceQueryReservationInfoKHR is not nothrow_move_constructible!" );

//=== VK_KHR_get_surface_capabilities2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR ) == sizeof( VkPhysicalDeviceSurfaceInfo2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR>::value,
                          "PhysicalDeviceSurfaceInfo2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR ) == sizeof( VkSurfaceCapabilities2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>::value,
                          "SurfaceCapabilities2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR ) == sizeof( VkSurfaceFormat2KHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>::value,
                          "SurfaceFormat2KHR is not nothrow_move_constructible!" );

//=== VK_KHR_get_display_properties2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayProperties2KHR ) == sizeof( VkDisplayProperties2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR>::value,
                          "DisplayProperties2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR ) == sizeof( VkDisplayPlaneProperties2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR>::value,
                          "DisplayPlaneProperties2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR ) == sizeof( VkDisplayModeProperties2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR>::value,
                          "DisplayModeProperties2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR ) == sizeof( VkDisplayPlaneInfo2KHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR>::value,
                          "DisplayPlaneInfo2KHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR ) == sizeof( VkDisplayPlaneCapabilities2KHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR>::value,
                          "DisplayPlaneCapabilities2KHR is not nothrow_move_constructible!" );

//=== VK_EXT_debug_utils ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT ) == sizeof( VkDebugUtilsLabelEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT>::value,
                          "DebugUtilsLabelEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT ) == sizeof( VkDebugUtilsMessengerCallbackDataEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT>::value,
                          "DebugUtilsMessengerCallbackDataEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT ) == sizeof( VkDebugUtilsMessengerCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT>::value,
                          "DebugUtilsMessengerCreateInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT ) == sizeof( VkDebugUtilsMessengerEXT ),
                          "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT>::value,
                          "DebugUtilsMessengerEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT ) == sizeof( VkDebugUtilsObjectNameInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT>::value,
                          "DebugUtilsObjectNameInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT ) == sizeof( VkDebugUtilsObjectTagInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT>::value,
                          "DebugUtilsObjectTagInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_sample_locations ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SampleLocationEXT ) == sizeof( VkSampleLocationEXT ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SampleLocationEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SampleLocationEXT>::value,
                          "SampleLocationEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT ) == sizeof( VkSampleLocationsInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT>::value,
                          "SampleLocationsInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT ) == sizeof( VkAttachmentSampleLocationsEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::AttachmentSampleLocationsEXT>::value,
                          "AttachmentSampleLocationsEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT ) == sizeof( VkSubpassSampleLocationsEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SubpassSampleLocationsEXT>::value,
                          "SubpassSampleLocationsEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT ) == sizeof( VkRenderPassSampleLocationsBeginInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RenderPassSampleLocationsBeginInfoEXT>::value,
                          "RenderPassSampleLocationsBeginInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT ) == sizeof( VkPipelineSampleLocationsStateCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineSampleLocationsStateCreateInfoEXT>::value,
                          "PipelineSampleLocationsStateCreateInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT ) == sizeof( VkPhysicalDeviceSampleLocationsPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceSampleLocationsPropertiesEXT>::value,
                          "PhysicalDeviceSampleLocationsPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT ) == sizeof( VkMultisamplePropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT>::value,
                          "MultisamplePropertiesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_blend_operation_advanced ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>::value,
                          "PhysicalDeviceBlendOperationAdvancedFeaturesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>::value,
                          "PhysicalDeviceBlendOperationAdvancedPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT ) ==
                            sizeof( VkPipelineColorBlendAdvancedStateCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineColorBlendAdvancedStateCreateInfoEXT>::value,
                          "PipelineColorBlendAdvancedStateCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_image_drm_format_modifier ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT ) == sizeof( VkDrmFormatModifierPropertiesListEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesListEXT>::value,
                          "DrmFormatModifierPropertiesListEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT ) == sizeof( VkDrmFormatModifierPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesEXT>::value,
                          "DrmFormatModifierPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT ) ==
                            sizeof( VkPhysicalDeviceImageDrmFormatModifierInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageDrmFormatModifierInfoEXT>::value,
                          "PhysicalDeviceImageDrmFormatModifierInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT ) == sizeof( VkImageDrmFormatModifierListCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierListCreateInfoEXT>::value,
                          "ImageDrmFormatModifierListCreateInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT ) ==
                            sizeof( VkImageDrmFormatModifierExplicitCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierExplicitCreateInfoEXT>::value,
                          "ImageDrmFormatModifierExplicitCreateInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT ) == sizeof( VkImageDrmFormatModifierPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT>::value,
                          "ImageDrmFormatModifierPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT ) == sizeof( VkDrmFormatModifierPropertiesList2EXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrmFormatModifierPropertiesList2EXT>::value,
                          "DrmFormatModifierPropertiesList2EXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT ) == sizeof( VkDrmFormatModifierProperties2EXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DrmFormatModifierProperties2EXT>::value,
                          "DrmFormatModifierProperties2EXT is not nothrow_move_constructible!" );

//=== VK_EXT_filter_cubic ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT ) == sizeof( VkPhysicalDeviceImageViewImageFormatInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceImageViewImageFormatInfoEXT>::value,
                          "PhysicalDeviceImageViewImageFormatInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT ) ==
                            sizeof( VkFilterCubicImageViewImageFormatPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FilterCubicImageViewImageFormatPropertiesEXT>::value,
                          "FilterCubicImageViewImageFormatPropertiesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_external_memory_host ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT ) == sizeof( VkImportMemoryHostPointerInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportMemoryHostPointerInfoEXT>::value,
                          "ImportMemoryHostPointerInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT ) == sizeof( VkMemoryHostPointerPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT>::value,
                          "MemoryHostPointerPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceExternalMemoryHostPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryHostPropertiesEXT>::value,
                          "PhysicalDeviceExternalMemoryHostPropertiesEXT is not nothrow_move_constructible!" );

//=== VK_KHR_shader_clock ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR ) == sizeof( VkPhysicalDeviceShaderClockFeaturesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderClockFeaturesKHR>::value,
                          "PhysicalDeviceShaderClockFeaturesKHR is not nothrow_move_constructible!" );

//=== VK_EXT_vertex_attribute_divisor ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>::value,
                          "PhysicalDeviceVertexAttributeDivisorPropertiesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_pci_bus_info ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT ) == sizeof( VkPhysicalDevicePCIBusInfoPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevicePCIBusInfoPropertiesEXT>::value,
                          "PhysicalDevicePCIBusInfoPropertiesEXT is not nothrow_move_constructible!" );

//=== VK_KHR_fragment_shading_rate ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR ) == sizeof( VkFragmentShadingRateAttachmentInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FragmentShadingRateAttachmentInfoKHR>::value,
                          "FragmentShadingRateAttachmentInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR ) ==
                            sizeof( VkPipelineFragmentShadingRateStateCreateInfoKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineFragmentShadingRateStateCreateInfoKHR>::value,
                          "PipelineFragmentShadingRateStateCreateInfoKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR ) ==
                            sizeof( VkPhysicalDeviceFragmentShadingRateFeaturesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateFeaturesKHR>::value,
                          "PhysicalDeviceFragmentShadingRateFeaturesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR ) ==
                            sizeof( VkPhysicalDeviceFragmentShadingRatePropertiesKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRatePropertiesKHR>::value,
                          "PhysicalDeviceFragmentShadingRatePropertiesKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR ) == sizeof( VkPhysicalDeviceFragmentShadingRateKHR ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR>::value,
                          "PhysicalDeviceFragmentShadingRateKHR is not nothrow_move_constructible!" );

//=== VK_EXT_shader_image_atomic_int64 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT>::value,
                          "PhysicalDeviceShaderImageAtomicInt64FeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_memory_budget ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT ) == sizeof( VkPhysicalDeviceMemoryBudgetPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryBudgetPropertiesEXT>::value,
                          "PhysicalDeviceMemoryBudgetPropertiesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_validation_features ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT ) == sizeof( VkValidationFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ValidationFeaturesEXT>::value,
                          "ValidationFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_fragment_shader_interlock ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>::value,
                          "PhysicalDeviceFragmentShaderInterlockFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_ycbcr_image_arrays ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT ) == sizeof( VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcrImageArraysFeaturesEXT>::value,
                          "PhysicalDeviceYcbcrImageArraysFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_headless_surface ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT ) == sizeof( VkHeadlessSurfaceCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT>::value,
                          "HeadlessSurfaceCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_shader_atomic_float ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT ) == sizeof( VkPhysicalDeviceShaderAtomicFloatFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceShaderAtomicFloatFeaturesEXT>::value,
                          "PhysicalDeviceShaderAtomicFloatFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_extended_dynamic_state ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceExtendedDynamicStateFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicStateFeaturesEXT>::value,
                          "PhysicalDeviceExtendedDynamicStateFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_texel_buffer_alignment ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>::value,
                          "PhysicalDeviceTexelBufferAlignmentFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_robustness2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT ) == sizeof( VkPhysicalDeviceRobustness2FeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2FeaturesEXT>::value,
                          "PhysicalDeviceRobustness2FeaturesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT ) == sizeof( VkPhysicalDeviceRobustness2PropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceRobustness2PropertiesEXT>::value,
                          "PhysicalDeviceRobustness2PropertiesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_custom_border_color ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SamplerCustomBorderColorCreateInfoEXT ) == sizeof( VkSamplerCustomBorderColorCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SamplerCustomBorderColorCreateInfoEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SamplerCustomBorderColorCreateInfoEXT>::value,
                          "SamplerCustomBorderColorCreateInfoEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT ) ==
                            sizeof( VkPhysicalDeviceCustomBorderColorPropertiesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorPropertiesEXT>::value,
                          "PhysicalDeviceCustomBorderColorPropertiesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT ) == sizeof( VkPhysicalDeviceCustomBorderColorFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceCustomBorderColorFeaturesEXT>::value,
                          "PhysicalDeviceCustomBorderColorFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_KHR_object_refresh ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RefreshObjectListKHR ) == sizeof( VkRefreshObjectListKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RefreshObjectListKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RefreshObjectListKHR>::value,
                          "RefreshObjectListKHR is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::RefreshObjectKHR ) == sizeof( VkRefreshObjectKHR ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::RefreshObjectKHR>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::RefreshObjectKHR>::value,
                          "RefreshObjectKHR is not nothrow_move_constructible!" );

//=== VK_KHR_synchronization2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV ) == sizeof( VkQueueFamilyCheckpointProperties2NV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::QueueFamilyCheckpointProperties2NV>::value,
                          "QueueFamilyCheckpointProperties2NV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::CheckpointData2NV ) == sizeof( VkCheckpointData2NV ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::CheckpointData2NV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::CheckpointData2NV>::value,
                          "CheckpointData2NV is not nothrow_move_constructible!" );

//=== VK_EXT_ycbcr_2plane_444_formats ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT>::value,
                          "PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_4444_formats ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT ) == sizeof( VkPhysicalDevice4444FormatsFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDevice4444FormatsFeaturesEXT>::value,
                          "PhysicalDevice4444FormatsFeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_vertex_input_dynamic_state ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceVertexInputDynamicStateFeaturesEXT>::value,
                          "PhysicalDeviceVertexInputDynamicStateFeaturesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT ) == sizeof( VkVertexInputBindingDescription2EXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT>::value,
                          "VertexInputBindingDescription2EXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT ) == sizeof( VkVertexInputAttributeDescription2EXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT>::value,
                          "VertexInputAttributeDescription2EXT is not nothrow_move_constructible!" );

#if defined( VK_USE_PLATFORM_SCI )
//=== VK_NV_external_sci_sync ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV ) == sizeof( VkExportFenceSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportFenceSciSyncInfoNV>::value,
                          "ExportFenceSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV ) == sizeof( VkImportFenceSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV>::value,
                          "ImportFenceSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV ) == sizeof( VkFenceGetSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV>::value,
                          "FenceGetSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV ) == sizeof( VkSciSyncAttributesInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV>::value,
                          "SciSyncAttributesInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV ) == sizeof( VkExportSemaphoreSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportSemaphoreSciSyncInfoNV>::value,
                          "ExportSemaphoreSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV ) == sizeof( VkImportSemaphoreSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV>::value,
                          "ImportSemaphoreSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV ) == sizeof( VkSemaphoreGetSciSyncInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV>::value,
                          "SemaphoreGetSciSyncInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV ) == sizeof( VkPhysicalDeviceExternalSciSyncFeaturesNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSyncFeaturesNV>::value,
                          "PhysicalDeviceExternalSciSyncFeaturesNV is not nothrow_move_constructible!" );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
//=== VK_NV_external_memory_sci_buf ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV ) == sizeof( VkExportMemorySciBufInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExportMemorySciBufInfoNV>::value,
                          "ExportMemorySciBufInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV ) == sizeof( VkImportMemorySciBufInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportMemorySciBufInfoNV>::value,
                          "ImportMemorySciBufInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV ) == sizeof( VkMemoryGetSciBufInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV>::value,
                          "MemoryGetSciBufInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV ) == sizeof( VkMemorySciBufPropertiesNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV>::value,
                          "MemorySciBufPropertiesNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV ) ==
                            sizeof( VkPhysicalDeviceExternalMemorySciBufFeaturesNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemorySciBufFeaturesNV>::value,
                          "PhysicalDeviceExternalMemorySciBufFeaturesNV is not nothrow_move_constructible!" );
#endif /*VK_USE_PLATFORM_SCI*/

//=== VK_EXT_extended_dynamic_state2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT ) ==
                            sizeof( VkPhysicalDeviceExtendedDynamicState2FeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExtendedDynamicState2FeaturesEXT>::value,
                          "PhysicalDeviceExtendedDynamicState2FeaturesEXT is not nothrow_move_constructible!" );

//=== VK_EXT_color_write_enable ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT ) == sizeof( VkPhysicalDeviceColorWriteEnableFeaturesEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceColorWriteEnableFeaturesEXT>::value,
                          "PhysicalDeviceColorWriteEnableFeaturesEXT is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT ) == sizeof( VkPipelineColorWriteCreateInfoEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PipelineColorWriteCreateInfoEXT>::value,
                          "PipelineColorWriteCreateInfoEXT is not nothrow_move_constructible!" );

//=== VK_EXT_application_parameters ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ApplicationParametersEXT ) == sizeof( VkApplicationParametersEXT ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ApplicationParametersEXT>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ApplicationParametersEXT>::value,
                          "ApplicationParametersEXT is not nothrow_move_constructible!" );

#if defined( VK_USE_PLATFORM_SCI )
//=== VK_NV_external_sci_sync2 ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV ) == sizeof( VkSemaphoreSciSyncPoolNV ),
                          "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV>::value,
                          "SemaphoreSciSyncPoolNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV ) == sizeof( VkPhysicalDeviceExternalSciSync2FeaturesNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSciSync2FeaturesNV>::value,
                          "PhysicalDeviceExternalSciSync2FeaturesNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV ) == sizeof( VkSemaphoreSciSyncPoolCreateInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV>::value,
                          "SemaphoreSciSyncPoolCreateInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV ) == sizeof( VkSemaphoreSciSyncCreateInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncCreateInfoNV>::value,
                          "SemaphoreSciSyncCreateInfoNV is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV ) ==
                            sizeof( VkDeviceSemaphoreSciSyncPoolReservationCreateInfoNV ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::DeviceSemaphoreSciSyncPoolReservationCreateInfoNV>::value,
                          "DeviceSemaphoreSciSyncPoolReservationCreateInfoNV is not nothrow_move_constructible!" );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
//=== VK_QNX_external_memory_screen_buffer ===

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX ) == sizeof( VkScreenBufferPropertiesQNX ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>::value,
                          "ScreenBufferPropertiesQNX is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX ) == sizeof( VkScreenBufferFormatPropertiesQNX ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ScreenBufferFormatPropertiesQNX>::value,
                          "ScreenBufferFormatPropertiesQNX is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX ) == sizeof( VkImportScreenBufferInfoQNX ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ImportScreenBufferInfoQNX>::value,
                          "ImportScreenBufferInfoQNX is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::ExternalFormatQNX ) == sizeof( VkExternalFormatQNX ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::ExternalFormatQNX>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::ExternalFormatQNX>::value,
                          "ExternalFormatQNX is not nothrow_move_constructible!" );

VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX ) ==
                            sizeof( VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX ),
                          "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX>::value,
                          "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX>::value,
                          "PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX is not nothrow_move_constructible!" );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

#endif
