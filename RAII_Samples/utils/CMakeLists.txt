# SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

vulkan_hpp__setup_library(
	NAME RAII_utils
	HEADERS shaders.hpp utils.hpp
	FOLDER "RAII_Samples" )

target_link_libraries( RAII_utils INTERFACE utils )
target_compile_definitions( RAII_utils INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )

