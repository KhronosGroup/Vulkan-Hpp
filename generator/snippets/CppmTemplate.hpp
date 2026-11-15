// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

${copyrightMessage}

module;

#define VULKAN_HPP_CXX_MODULE 1

#include <cassert>
#include <cstring>
#include <vulkan/${vulkan_h}>
#include <vulkan/vulkan_hpp_macros.hpp>

#if !defined( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#  define VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING \
  "\n\tThe Vulkan-Hpp C++ named module is experimental. It is subject to change without prior notice.\n" \
  "\tTo silence this warning, define the VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING macro.\n" \
  "\tFor feedback, go to: https://github.com/KhronosGroup/Vulkan-Hpp/issues"

  VULKAN_HPP_COMPILE_WARNING( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#endif

export module ${api};

import std;

VULKAN_HPP_STATIC_ASSERT( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

#include "vulkan/${api}.hpp"
#include "vulkan/${api}_extension_inspection.hpp"
#include "vulkan/${api}_format_traits.hpp"
#include "vulkan/${api}_hash.hpp"
#include "vulkan/${api}_raii.hpp"
#include "vulkan/${api}_shared.hpp"

export
{
// This VkFlags type is used as part of a bitfield in some structures.
// As it can't be mimicked by vk-data types, we need to export just that.
using ::VkGeometryInstanceFlagsKHR;

${pfnCommands}
}
