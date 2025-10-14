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

vulkan_hpp__setup_ctest( NAME ArrayProxy )
vulkan_hpp__setup_ctest( NAME ArrayProxyNoTemporaries )
vulkan_hpp__setup_ctest( NAME ArrayWrapper )
vulkan_hpp__setup_ctest( NAME CppType )
if( CMAKE_CXX_STANDARD GREATER_EQUAL 20 )
	vulkan_hpp__setup_ctest( NAME DesignatedInitializers )
endif()
vulkan_hpp__setup_ctest( NAME DeviceFunctions )
vulkan_hpp__setup_ctest( NAME DispatchLoaderDynamic )
vulkan_hpp__setup_ctest( NAME DispatchLoaderDynamicSharedLibrary )
vulkan_hpp__setup_ctest( NAME DispatchLoaderStatic )
vulkan_hpp__setup_ctest( NAME EnableBetaExtensions )
if( CMAKE_CXX_STANDARD GREATER_EQUAL 20 )
	vulkan_hpp__setup_ctest( NAME ExtensionInspection )
endif()
vulkan_hpp__setup_ctest( NAME Flags )
vulkan_hpp__setup_ctest( NAME FormatTraits )
vulkan_hpp__setup_ctest( NAME FunctionCalls )
vulkan_hpp__setup_ctest( NAME FunctionCallsRAII )
vulkan_hpp__setup_ctest( NAME Handles )
vulkan_hpp__setup_ctest( NAME HandlesMoveExchange )
vulkan_hpp__setup_ctest( NAME Hash )
vulkan_hpp__setup_ctest( NAME NoExceptions )
if( ( CMAKE_CXX_STANDARD GREATER_EQUAL 23 ) AND NOT ( ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" ) AND ( CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0 ) ) )
	vulkan_hpp__setup_ctest( NAME NoExceptionsRAII ) # errors with clang++13 and clang++14
endif()
vulkan_hpp__setup_ctest( NAME StridedArrayProxy )
vulkan_hpp__setup_ctest( NAME StructureChain )
add_subdirectory( UniqueHandle )
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	vulkan_hpp__setup_ctest( NAME UniqueHandleDefaultArguments )
endif()
vulkan_hpp__setup_ctest( NAME Video )

if( VULKAN_HPP_BUILD_CXX_MODULE )
	if( NOT ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16" ) )
		vulkan_hpp__setup_ctest( NAME ArrayProxy CXX_MODULE ) # gcc <= 15.x fail to ignore -Winit-list-lifetime
	endif()
	vulkan_hpp__setup_ctest( NAME ArrayProxyNoTemporaries CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME ArrayWrapper CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME Cpp20Modules CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME CppType CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME DesignatedInitializers CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME DeviceFunctions CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME DispatchLoaderDynamic CXX_MODULE )
	# vulkan_hpp__setup_ctest( NAME DispatchLoaderDynamicSharedLibrary CXX_MODULE ) # no CXX_MODULE test version yet
	vulkan_hpp__setup_ctest( NAME DispatchLoaderStatic CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME EnableBetaExtensions CXX_MODULE )
	if( CMAKE_CXX_STANDARD GREATER_EQUAL 20 )
		vulkan_hpp__setup_ctest( NAME ExtensionInspection CXX_MODULE )
	endif()
	vulkan_hpp__setup_ctest( NAME Flags CXX_MODULE )
	# vulkan_hpp__setup_ctest( NAME FormatTraits CXX_MODULE ) # module has no access to format traits
	# vulkan_hpp__setup_ctest( NAME Handles CXX_MODULE ) # errors using equality operator with raii handles
	vulkan_hpp__setup_ctest( NAME HandlesMoveExchange CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME Hash CXX_MODULE )
	vulkan_hpp__setup_ctest( NAME NoExceptions CXX_MODULE )
	if( ( CMAKE_CXX_STANDARD GREATER_EQUAL 23 ) AND NOT ( ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" ) AND ( CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0 ) ) )
		vulkan_hpp__setup_ctest( NAME NoExceptionsRAII CXX_MODULE ) # errors with clang++13 and clang++14
	endif()
	if( NOT ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16" ) )
		vulkan_hpp__setup_ctest( NAME StridedArrayProxy CXX_MODULE ) # gcc <= 15.x fail to ignore -Winit-list-lifetime
	endif()
	vulkan_hpp__setup_ctest( NAME StructureChain CXX_MODULE )
	# add_subdirectory( UniqueHandle ) # no CXX_MODULE test version yet
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		vulkan_hpp__setup_ctest( NAME UniqueHandleDefaultArguments CXX_MODULE )
	endif()
	vulkan_hpp__setup_ctest( NAME Video CXX_MODULE )
endif()
