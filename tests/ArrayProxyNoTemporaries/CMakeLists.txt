# SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppTest LANGUAGES CXX )

include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()