// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

module;

// see utils.cppm for why: macros/GLFW must be brought in here too, since they aren't exported by modules
#include <cassert>
#include <vulkan/vulkan_core.h>

#define VULKAN_HPP_CXX_MODULE 1
#define VULKAN_HPP_USE_CXX_MODULE

#include <vulkan/vulkan_hpp_macros.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module RAII_utils;

import std;
import vulkan;
// re-export `utils`, so importers of RAII_utils automatically get vk::su:: as well as vk::raii::su::
export import utils;

#include "utils.hpp"
#include "shaders.hpp"
