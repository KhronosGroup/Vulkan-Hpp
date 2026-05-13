# SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppTest LANGUAGES CXX )

set(VULKAN_HPP_NO_CONSTRUCTORS ON)
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()

# add_subdirectory cannot use the normal compile definitions
if( NOT VULKAN_HPP_TESTS_CTEST )
    target_compile_definitions( ${TARGET_NAME} PRIVATE VULKAN_HPP_NO_CONSTRUCTORS )
endif()
