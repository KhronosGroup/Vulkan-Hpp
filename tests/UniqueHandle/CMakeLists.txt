# SPDX-FileCopyrightText: 2022-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

set(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ON)
set(VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST ON)
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()

# TODO: this test should be simplified to remove the additional deps such as glsl (linked via utils)
target_link_libraries( UniqueHandle PRIVATE utils )

# add_subdirectory cannot use the normal compile definitions
if( NOT VULKAN_HPP_TESTS_CTEST )
    target_compile_definitions( ${TARGET_NAME} PRIVATE VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
endif()
