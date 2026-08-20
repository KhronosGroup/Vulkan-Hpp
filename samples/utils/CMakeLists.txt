# SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

# common third-party dependencies shared by both utils_static and utils_dynamic
add_library( utils_deps INTERFACE )
target_link_libraries( utils_deps INTERFACE
	glfw
	glm::glm
	glslang::glslang
	glslang::glslang-default-resource-limits
	glslang::SPIRV )

foreach( LINKAGE static dynamic )
	vulkan_hpp__setup_library(
		NAME utils_${LINKAGE}
		HEADERS math.hpp shaders.hpp utils.hpp
		SOURCES math.cpp shaders.cpp utils.cpp
		FOLDER "Samples" )

	if( VULKAN_HPP_RUN_GENERATOR )
		add_dependencies( utils_${LINKAGE} build_vulkan_hpp )
	endif()

	target_link_libraries( utils_${LINKAGE} PUBLIC utils_deps )
	target_precompile_headers( utils_${LINKAGE} PRIVATE "pch.hpp" )
endforeach()

target_compile_definitions( utils_static  PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
target_compile_definitions( utils_dynamic PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )

