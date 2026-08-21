// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

module;

// macros aren't exported by modules, so bring in what `import std;`/`import vulkan;` below won't carry across
#include <cassert>
#include <vulkan/vulkan_core.h>

// mark declarations pulled in from vulkan_hpp_macros.hpp as `export`-able for this module purview
#define VULKAN_HPP_CXX_MODULE 1
// make math.hpp / utils.hpp / shaders.hpp pick the `import`, rather than the `#include`, branch below
#define VULKAN_HPP_USE_CXX_MODULE

#include <vulkan/vulkan_hpp_macros.hpp>

// GLFW and glslang are not modularized, so they're included textually here, same as in a non-module build
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>

export module utils;

// vulkan_to_string.hpp (vk::to_string) is already transitively included by vulkan.hpp, and thus re-exported by `vulkan`.
import glm;
import std;
// re-exported so `vk::` types stay visible to anyone who only does `import utils;`
export import vulkan;

#include "math.hpp"
#include "utils.hpp"
#include "shaders.hpp"
#include "geometries.hpp"

module :private;

#include "math.cpp"
#include "shaders.cpp"
#include "utils.cpp"
