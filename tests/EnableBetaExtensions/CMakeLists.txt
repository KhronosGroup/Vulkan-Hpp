# SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppTest LANGUAGES CXX )

include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()

# no native way to set VK_ENABLE_BETA_EXTENSIONS yet
target_compile_definitions( ${TARGET_NAME} PRIVATE VK_ENABLE_BETA_EXTENSIONS )
if( VULKAN_HPP_BUILD_CXX_MODULE )
    target_compile_definitions( VulkanHppModule PRIVATE VK_ENABLE_BETA_EXTENSIONS )
endif()