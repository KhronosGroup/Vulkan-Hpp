# Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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

vulkan_hpp__setup_test( NAME ArrayProxy )
vulkan_hpp__setup_test( NAME ArrayProxyNoTemporaries )
vulkan_hpp__setup_test( NAME ArrayWrapper )
vulkan_hpp__setup_test( NAME CppType )
if( CMAKE_CXX_STANDARD GREATER_EQUAL 20 )
	vulkan_hpp__setup_test( NAME DesignatedInitializers )
	vulkan_hpp__setup_test( NAME ExtensionInspection )
endif()
# can only run these tests with VULKAN_HPP_DISPATCH_LOADER_DYNAMIC disabled
if( NOT VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
	vulkan_hpp__setup_test( NAME DeviceFunctions )
	vulkan_hpp__setup_test( NAME DispatchLoaderStatic )
endif()
# can only run these tests with VULKAN_HPP_DISPATCH_LOADER_DYNAMIC enabled
if( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
	vulkan_hpp__setup_test( NAME DispatchLoaderDynamic )
	vulkan_hpp__setup_test( NAME DispatchLoaderDynamicSharedLibraryClient )
	vulkan_hpp__setup_test( NAME NoExceptions )
endif()
vulkan_hpp__setup_test( NAME EnableBetaExtensions )
vulkan_hpp__setup_test( NAME Flags )
vulkan_hpp__setup_test( NAME FormatTraits )
vulkan_hpp__setup_test( NAME FunctionCalls )
vulkan_hpp__setup_test( NAME FunctionCallsRAII )
vulkan_hpp__setup_test( NAME Handles )
vulkan_hpp__setup_test( NAME HandlesMoveExchange )
vulkan_hpp__setup_test( NAME Hash )
vulkan_hpp__setup_test( NAME NoDefaultDispatcher )
if( NOT ( ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" ) AND ( CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0 ) ) )
	vulkan_hpp__setup_test( NAME NoExceptionsRAII ) # errors with clang++13 and clang++14
endif()
vulkan_hpp__setup_test( NAME NoSmartHandle )
vulkan_hpp__setup_test( NAME StridedArrayProxy )
vulkan_hpp__setup_test( NAME StructureChain )
# add_subdirectory( UniqueHandle ) # really messy setup, test needs to be shortened
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	vulkan_hpp__setup_test( NAME UniqueHandleDefaultArguments )
endif()
vulkan_hpp__setup_test( NAME Video )

if( VULKAN_HPP_TESTS_CTEST AND VULKAN_HPP_BUILD_CXX_MODULE )
 	# gcc <= 15.x fail to ignore -Winit-list-lifetime
	if( NOT( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16" ) )
		vulkan_hpp__setup_test( NAME ArrayProxy CXX_MODULE )
		vulkan_hpp__setup_test( NAME StridedArrayProxy CXX_MODULE )
	endif()
	vulkan_hpp__setup_test( NAME ArrayProxyNoTemporaries CXX_MODULE )
	vulkan_hpp__setup_test( NAME ArrayWrapper CXX_MODULE )
	vulkan_hpp__setup_test( NAME CppType CXX_MODULE )
	vulkan_hpp__setup_test( NAME DesignatedInitializers CXX_MODULE )
	# CI cannot run these tests, as it does not have access to the Vulkan package
	if( NOT VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
		vulkan_hpp__setup_test( NAME DeviceFunctions CXX_MODULE )
		vulkan_hpp__setup_test( NAME DispatchLoaderStatic CXX_MODULE )
	endif()
	# vulkan_hpp__setup_test( NAME DispatchLoaderDynamicSharedLibrary CXX_MODULE ) # no CXX_MODULE test version yet
	vulkan_hpp__setup_test( NAME DispatchLoaderDynamic CXX_MODULE )
	vulkan_hpp__setup_test( NAME EnableBetaExtensions CXX_MODULE )
    vulkan_hpp__setup_test( NAME ExtensionInspection CXX_MODULE )
	vulkan_hpp__setup_test( NAME Flags CXX_MODULE )
	vulkan_hpp__setup_test( NAME FormatTraits CXX_MODULE )
	if( NOT( MSVC AND MSVC_VERSION VERSION_LESS "1950" ) )
		vulkan_hpp__setup_test( NAME FunctionCalls CXX_MODULE )
	endif()
	vulkan_hpp__setup_test( NAME FunctionCallsRAII CXX_MODULE )
	vulkan_hpp__setup_test( NAME Handles CXX_MODULE )
	vulkan_hpp__setup_test( NAME HandlesMoveExchange CXX_MODULE )
	vulkan_hpp__setup_test( NAME Hash CXX_MODULE )
	vulkan_hpp__setup_test( NAME NoDefaultDispatcher CXX_MODULE )
	vulkan_hpp__setup_test( NAME NoExceptions CXX_MODULE )
	vulkan_hpp__setup_test( NAME NoExceptionsRAII CXX_MODULE )
	vulkan_hpp__setup_test( NAME NoSmartHandle CXX_MODULE )
	vulkan_hpp__setup_test( NAME StructureChain CXX_MODULE )
	# add_subdirectory( UniqueHandle ) # no CXX_MODULE test version yet
	vulkan_hpp__setup_test( NAME UniqueHandleDefaultArguments CXX_MODULE )
	vulkan_hpp__setup_test( NAME Video CXX_MODULE )
endif()
