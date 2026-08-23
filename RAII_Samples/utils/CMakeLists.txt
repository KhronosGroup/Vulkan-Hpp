# SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

vulkan_hpp__setup_library(
	NAME RAII_utils
	HEADERS shaders.hpp utils.hpp
	FOLDER "RAII_Samples" )

target_link_libraries( RAII_utils INTERFACE utils )
target_compile_definitions( RAII_utils INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )

# RAII_utils, as a C++ named module (re-exports utils::module, no PCH needed)
if( VULKAN_HPP_BUILD_CXX_MODULE )
	vulkan_hpp__setup_cxx_module(
		NAME RAII_utils_module
		ALIAS RAII_utils::module
		FILE_SET_NAME raii_utils_module_file
		FILE_SET "${CMAKE_CURRENT_SOURCE_DIR}/RAII_utils.cppm"
		LINK_LIBRARIES utils::module )
	# NOTE: no vulkan_hpp__setup_target_defaults here, see samples/utils/CMakeLists.txt for why.
	vulkan_hpp__setup_platform( NAME RAII_utils_module )
	set_target_properties( RAII_utils_module PROPERTIES FOLDER "RAII_Samples" )
endif()

