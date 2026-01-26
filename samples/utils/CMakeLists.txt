# Copyright(c) 2020, NVIDIA CORPORATION. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

