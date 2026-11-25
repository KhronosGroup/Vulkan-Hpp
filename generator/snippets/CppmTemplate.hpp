// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

${copyrightMessage}

module;

#define VULKAN_HPP_CXX_MODULE 1

#include <cassert>
#include <cstring>
#include <vulkan/${vulkan_h}>
#include <vulkan/vulkan_hpp_macros.hpp>

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
