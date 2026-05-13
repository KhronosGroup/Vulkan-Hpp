# SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

vulkan_hpp__setup_library(
	NAME utils_static
	HEADERS math.hpp shaders.hpp utils.hpp
	SOURCES math.cpp shaders.cpp utils.cpp
	FOLDER "Samples" )

vulkan_hpp__setup_library(
	NAME utils_dynamic
	HEADERS math.hpp shaders.hpp utils.hpp
	SOURCES math.cpp shaders.cpp utils.cpp
	FOLDER "Samples" )

if( VULKAN_HPP_RUN_GENERATOR )
	add_dependencies( utils_static build_vulkan_hpp )
	add_dependencies( utils_dynamic build_vulkan_hpp )
endif()

target_link_libraries( utils_static PUBLIC glfw )
target_link_libraries( utils_static PUBLIC glm::glm )
target_link_libraries( utils_static PUBLIC glslang::glslang )
target_link_libraries( utils_static PUBLIC glslang::glslang-default-resource-limits )
target_link_libraries( utils_static PUBLIC glslang::SPIRV )
target_compile_definitions( utils_static PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
target_precompile_headers( utils_static PRIVATE "pch.hpp" )

target_link_libraries( utils_dynamic PUBLIC glfw )
target_link_libraries( utils_dynamic PUBLIC glm::glm )
target_link_libraries( utils_dynamic PUBLIC glslang::glslang )
target_link_libraries( utils_dynamic PUBLIC glslang::glslang-default-resource-limits )
target_link_libraries( utils_dynamic PUBLIC glslang::SPIRV )
target_compile_definitions( utils_dynamic PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )
target_precompile_headers( utils_dynamic PRIVATE "pch.hpp" )

