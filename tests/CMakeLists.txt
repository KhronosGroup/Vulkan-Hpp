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

add_subdirectory( ArrayProxy )
add_subdirectory( ArrayProxyNoTemporaries )
add_subdirectory( ArrayWrapper )
if( VULKAN_HPP_ENABLE_CPP20_MODULES )
	add_subdirectory( Cpp20Modules )
endif()
add_subdirectory( DesignatedInitializers )
add_subdirectory( DeviceFunctions )
add_subdirectory( DispatchLoaderDynamic )
add_subdirectory( DispatchLoaderDynamicSharedLibrary )
add_subdirectory( DispatchLoaderDynamicSharedLibraryClient )
add_subdirectory( DispatchLoaderStatic )
add_subdirectory( EnableBetaExtensions )
add_subdirectory( ExtensionInspection )
add_subdirectory( Flags )
add_subdirectory( FormatTraits )
add_subdirectory( Handles )
add_subdirectory( Hash )
add_subdirectory( NoExceptions )
if("cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	add_subdirectory( NoExceptionsRAII )
endif()
add_subdirectory( StridedArrayProxy )
add_subdirectory( StructureChain )
add_subdirectory( UniqueHandle )
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_subdirectory( UniqueHandleDefaultArguments )
endif()
