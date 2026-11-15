// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

${copyrightMessage}

module;

#define VULKAN_HPP_CXX_MODULE 1

${includes}

// clang-format off
#include <vulkan/vulkan_hpp_macros.hpp>
// clang-format on

export module vulkan_video;

import std;
import vulkan;

#include "vulkan/vulkan_video.hpp"
