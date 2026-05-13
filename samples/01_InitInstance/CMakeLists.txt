# SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

find_package( Vulkan QUIET )
if ( Vulkan_FOUND )
	vulkan_hpp__setup_sample_static( NAME 01_InitInstance )
endif()
