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

if( NOT VULKAN_HPP_BUILD_CXX_MODULE )
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
		if( VULKAN_HPP_PRECOMPILE )
			target_precompile_headers( utils_${LINKAGE} PRIVATE "pch.hpp" )
		endif()
	endforeach()

	target_compile_definitions( utils_static  PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
	target_compile_definitions( utils_dynamic PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )
else()
	# utils, as a C++ named module (no PCH: the module interface uses `import std;` instead)
	vulkan_hpp__setup_cxx_module(
		NAME utils_module
		ALIAS utils::module
		FILE_SET_NAME utils_module_file
		FILE_SET "${CMAKE_CURRENT_SOURCE_DIR}/utils.cppm"
		LINK_LIBRARIES utils_deps Vulkan::HppModule glm::module )
	# no vulkan_hpp__setup_target_defaults (warning level) here: those flags leak into CMake's synthetic
	# rebuilds of transitively-imported modules (std, glm), whose headers aren't warning-clean (same as VulkanHppModule/glm_module)
	vulkan_hpp__setup_platform( NAME utils_module )
	set_target_properties( utils_module PROPERTIES FOLDER "Samples" )

	if( VULKAN_HPP_RUN_GENERATOR )
		add_dependencies( utils_module build_vulkan_hpp )
	endif()
endif()
