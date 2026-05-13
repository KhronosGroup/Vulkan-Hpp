// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : CppType
//                   Compile test on using vk::CppType


#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <vulkan/vulkan.h>
import vulkan;
#else
#  include <cstdint>
#  include <type_traits>
#  include <vulkan/vulkan.hpp>
#  include <vulkan/vulkan_raii.hpp>
#endif

static_assert( std::is_same<vk::CppType<vk::IndexType, vk::IndexType::eUint16>::Type, uint16_t>::value, "" );
static_assert( std::is_same<vk::CppType<vk::ObjectType, vk::ObjectType::eInstance>::Type, vk::Instance>::value, "" );
static_assert( std::is_same<vk::CppType<vk::DebugReportObjectTypeEXT, vk::DebugReportObjectTypeEXT ::eInstance>::Type, vk::Instance>::value, "" );

#if 20 <= VULKAN_HPP_CPP_VERSION || defined( VULKAN_HPP_USE_CXX_MODULE )
static_assert( std::is_same<vk::CppType<VkAabbPositionsKHR>::Type, vk::AabbPositionsKHR>::value, "" );
static_assert( std::is_same<vk::CppType<VkDeviceOrHostAddressConstKHR>::Type, vk::DeviceOrHostAddressConstKHR>::value, "" );
static_assert( std::is_same<vk::CppType<VkAccelerationStructureGeometryTrianglesDataKHR>::Type, vk::AccelerationStructureGeometryTrianglesDataKHR>::value, "" );
#endif

#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
static_assert( std::is_same<vk::CppType<VkInstance>::Type, vk::Instance>::value, "" );
#endif

static_assert( std::is_same<vk::raii::Instance::CppType, vk::Instance>::value, "" );

int main()
{
  return 0;
}
