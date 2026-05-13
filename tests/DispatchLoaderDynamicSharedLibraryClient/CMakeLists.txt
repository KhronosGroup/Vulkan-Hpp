# SPDX-FileCopyrightText: 2020-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppTest LANGUAGES CXX )

set(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ON)
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )

vulkan_hpp__setup_library(
	SHARED
	NAME DispatchLoaderDynamicSharedLibrary
	SOURCES DispatchLoaderDynamicSharedLibrary.cpp
	FOLDER "Tests" )
target_compile_definitions( DispatchLoaderDynamicSharedLibrary PUBLIC VULKAN_HPP_STORAGE_SHARED VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
target_compile_definitions( DispatchLoaderDynamicSharedLibrary PRIVATE VULKAN_HPP_STORAGE_SHARED_EXPORT )

vulkan_hpp__setup_test_project()
target_compile_definitions( DispatchLoaderDynamicSharedLibraryClient PRIVATE "VULKAN_HPP_DISPATCH_LOADER_DYNAMIC" )
target_link_libraries( DispatchLoaderDynamicSharedLibraryClient PRIVATE DispatchLoaderDynamicSharedLibrary )
