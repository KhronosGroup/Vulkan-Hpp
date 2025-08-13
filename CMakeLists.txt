# Copyright(c) 2015-2018, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppGenerator LANGUAGES CXX )

if( CMAKE_VERSION VERSION_LESS "3.21" )
    # https://cmake.org/cmake/help/latest/variable/PROJECT_IS_TOP_LEVEL.html
    string( COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} PROJECT_IS_TOP_LEVEL )
endif()

if ( NOT DEFINED CMAKE_CXX_STANDARD AND PROJECT_IS_TOP_LEVEL )
	set( CMAKE_CXX_STANDARD 11 )
endif()

# all the options for this project
option( VULKAN_HPP_PRECOMPILE "Precompile vulkan.hpp and vulkan_raii.hpp for sample builds" ON )
option( VULKAN_HPP_RUN_GENERATOR "Run the HPP generator" OFF )
option( VULKAN_HPP_GENERATOR_BUILD "Build the HPP generator" ${PROJECT_IS_TOP_LEVEL} )
option( VULKAN_HPP_SAMPLES_BUILD "Build samples" OFF )
option( VULKAN_HPP_TESTS_BUILD "Build tests" OFF )
option( VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP "Build with local Vulkan headers" ON )
option( VULKAN_HPP_ENABLE_CPP20_MODULES "Build Vulkan-Hpp as C++20 module; requires minimum CMake version 3.28" OFF )
option( VULKAN_HPP_ENABLE_STD_MODULE "Build Vulkan-Hpp with import std; requires minimum CMake version 3.30" OFF )
# options for vulkan hpp compile definitions (see https://github.com/KhronosGroup/Vulkan-Hpp/tree/main?tab=readme-ov-file#configuration-options for details)
option( VULKAN_HPP_DISABLE_ENHANCED_MODE "Disable all enhanced functionality apart from scoped enums, bitmasks, default initialization and vk::StructureChain" OFF )
option( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC "Select the dynamic (ON) or static (OFF) dispatch loader (defaults to VK_NO_PROTOTYPES)" ${VK_NO_PROTOTYPES} )
option( VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC "Change the m_mask access modifier in vk::Flags to public" OFF )
option( VULKAN_HPP_HANDLES_MOVE_EXCHANGE "Enable 'm_handle = exchange( rhs.m_handle, {} )' in move constructors" OFF )
option( VULKAN_HPP_NO_CONSTRUCTORS "Remove constructors from structs and unions to enable C++20 designated initializers" OFF )
option( VULKAN_HPP_NO_EXCEPTIONS "Do not throw exceptions when vulkan functions return an error code" OFF )
option( VULKAN_HPP_NO_NODISCARD_WARNINGS "Remove C++17 [[nodiscard]] from function signatures that return something" OFF )
option( VULKAN_HPP_NO_SETTERS "Remove setters for structs and unions" OFF )
option( VULKAN_HPP_NO_SMART_HANDLE "Remove the vk::UniqueHandle helper class" OFF )
option( VULKAN_HPP_NO_SPACESHIP_OPERATOR "Remove the C++20 <=> (spaceship) operator" OFF )
option( VULKAN_HPP_NO_TO_STRING "Remove inclusion of vulkan_to_string.hpp from vulkan.hpp" OFF )
option( VULKAN_HPP_NO_WIN32_PROTOTYPES "Remove HINSTANCE, LoadLibraryA and other symbols, which are declared if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL is enabled on Win32" OFF )
option( VULKAN_HPP_RAII_NO_EXCEPTIONS "Do not throw exceptions when vulkan functions from vk::raii return an error code." OFF )
option( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST "Enable implicit casts from vk::UniqueHandle and vk::SharedHandle to their matching vk::Handle" OFF )
option( VULKAN_HPP_TYPESAFE_CONVERSION "Enable copy constructors for non-dispatchable handles" ON )
option( VULKAN_HPP_USE_REFLECT "Adds reflect function to structures, which returns an iterable tuple" OFF )

# manually set VK_NO_PROTOTYPES if undefined
if ( NOT DEFINED VK_NO_PROTOTYPES )
	set ( VK_NO_PROTOTYPES ${VULKAN_HPP_DISPATCH_LOADER_DYNAMIC} )
endif()

function( vulkan_hpp__setup_warning_level )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( MSVC )
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX )
		if( MSVC_VER GREATER_EQUAL 1910 )
			target_compile_options( ${TARGET_NAME} PRIVATE /permissive- )
		endif()
	else()
		target_compile_options( ${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror )
	endif()
endfunction()

# Build Vulkan-Hpp and Video-Hpp generators
if ( VULKAN_HPP_GENERATOR_BUILD )
	set_property( GLOBAL PROPERTY USE_FOLDERS ON )

	# look for the file vk.xml, the ultimate source of truth for vulkan, to generate the headers from
	if( NOT DEFINED VulkanRegistry_DIR )
		if( DEFINED VULKAN_HPP_VULKAN_HEADERS_SRC_DIR )
			set( VulkanRegistry_DIR "${VULKAN_HPP_VULKAN_HEADERS_SRC_DIR}/registry" )
		else()
			set( VulkanRegistry_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan-Headers/registry" )
		endif()
	endif()
	file( TO_NATIVE_PATH ${VulkanRegistry_DIR}/vk.xml vk_spec )
	string( REPLACE "\\" "\\\\" vk_spec ${vk_spec} )

	# gather the tinyxml2 sources, to be used directly in the generator project
	if( NOT DEFINED VULKAN_HPP_TINYXML2_SRC_DIR )
		set( VULKAN_HPP_TINYXML2_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2" )
	endif()
	set( TINYXML2_SOURCES ${VULKAN_HPP_TINYXML2_SRC_DIR}/tinyxml2.cpp )
	set( TINYXML2_HEADERS ${VULKAN_HPP_TINYXML2_SRC_DIR}/tinyxml2.h )
	source_group( TinyXML2 FILES ${TINYXML2_HEADERS} ${TINYXML2_SOURCES} )

	# The generator executable
	add_executable( VulkanHppGenerator VulkanHppGenerator.cpp VulkanHppGenerator.hpp XMLHelper.hpp ${TINYXML2_SOURCES} ${TINYXML2_HEADERS} )
	vulkan_hpp__setup_warning_level( NAME VulkanHppGenerator )
	target_compile_definitions( VulkanHppGenerator PUBLIC BASE_PATH="${CMAKE_CURRENT_SOURCE_DIR}" VK_SPEC="${vk_spec}" )
	target_include_directories( VulkanHppGenerator PRIVATE ${VULKAN_HPP_TINYXML2_SRC_DIR} )
	set_target_properties( VulkanHppGenerator PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON )
	if( UNIX )
		target_link_libraries( VulkanHppGenerator PUBLIC pthread )
	endif()

	# The video generator executable
	add_executable( VideoHppGenerator VideoHppGenerator.cpp VideoHppGenerator.hpp XMLHelper.hpp ${TINYXML2_SOURCES} ${TINYXML2_HEADERS} )
	vulkan_hpp__setup_warning_level( NAME VideoHppGenerator )
	file( TO_NATIVE_PATH ${VulkanRegistry_DIR}/video.xml video_spec )
	string( REPLACE "\\" "\\\\" video_spec ${video_spec} )
	target_compile_definitions( VideoHppGenerator PUBLIC BASE_PATH="${CMAKE_CURRENT_SOURCE_DIR}" VIDEO_SPEC="${video_spec}" )
	target_include_directories( VideoHppGenerator PRIVATE  ${VULKAN_HPP_TINYXML2_SRC_DIR} )
	set_target_properties( VideoHppGenerator PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON )

	# find a clang-format version to format the generated header files
	find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
	if( CLANG_FORMAT_EXECUTABLE )
		# get the clang-format version string
		execute_process( COMMAND ${CLANG_FORMAT_EXECUTABLE} "--version" OUTPUT_VARIABLE clangFormatVersion )
		# filter out the actual version
		string( REGEX MATCH [0123456789.]+ clangFormatVersion "${clangFormatVersion}" )
		# we need at least version 7.0.0 !
		if( clangFormatVersion VERSION_LESS 7.0.0 )
			message( WARNING " Found too old clang-format version <" ${clangFormatVersion} ">, we need version 7 and up to nicely format vulkan.hpp and vulkan_raii.hpp" )
		else()
			message( STATUS " Found clang-format version <" ${clangFormatVersion} ">." )
			if( clangFormatVersion VERSION_LESS 11.0.0 )
				message( STATUS " Using .clang-format version 7." )
				file( READ ".clang-format_7" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 12.0.0 )
				message( STATUS " Using .clang-format version 11." )
				file( READ ".clang-format_11" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 13.0.0 )
				message( STATUS " Using .clang-format version 12." )
				file( READ ".clang-format_12" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 14.0.0 )
				message( STATUS " Using .clang-format version 13." )
				file( READ ".clang-format_13" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 15.0.0 )
				message( STATUS " Using .clang-format version 14." )
				file( READ ".clang-format_14" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 16.0.0 )
				message( STATUS " Using .clang-format version 15." )
				file( READ ".clang-format_15" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 17.0.0 )
				message( STATUS " Using .clang-format version 16." )
				file( READ ".clang-format_16" clangFormat )
			elseif( clangFormatVersion VERSION_LESS 18.0.0 )
				message( STATUS " Using .clang-format version 17." )
				file( READ ".clang-format_17" clangFormat )
			else()
				message(STATUS " Using .clang-format version 18." )
				file( READ ".clang-format_18" clangFormat )
			endif()
			file( WRITE ".clang-format" ${clangFormat} )
			target_compile_definitions( VulkanHppGenerator PUBLIC CLANG_FORMAT_EXECUTABLE="${CLANG_FORMAT_EXECUTABLE}" )
			target_compile_definitions( VideoHppGenerator PUBLIC CLANG_FORMAT_EXECUTABLE="${CLANG_FORMAT_EXECUTABLE}" )
		endif()
	else()
		message( WARNING " Could not find clang-format. Generated vulkan.hpp and vulkan_raii.hpp will not be nicely formatted." )
	endif()
endif()

# if the generators are to be run, add a custom commands and targets
if( VULKAN_HPP_RUN_GENERATOR )
	if( NOT DEFINED VulkanHeaders_INCLUDE_DIR )
		if( DEFINED VULKAN_HPP_PATH )
			set( VulkanHeaders_INCLUDE_DIR ${VULKAN_HPP_PATH} )
		else()
			set( VulkanHeaders_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
		endif()
	endif()
	
	file( TO_NATIVE_PATH ${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan.hpp vulkan_hpp )
	string( REPLACE "\\" "\\\\" vulkan_hpp ${vulkan_hpp} )

	add_custom_command(
		COMMAND VulkanHppGenerator
		OUTPUT "${vulkan_hpp}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "run VulkanHppGenerator"
		DEPENDS VulkanHppGenerator "${vk_spec}" )
	add_custom_target( build_vulkan_hpp ALL DEPENDS "${vulkan_hpp}" "${vk_spec}" )

	file( TO_NATIVE_PATH ${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_video.hpp vulkan_video_hpp )
	string( REPLACE "\\" "\\\\" vulkan_video_hpp ${vulkan_video_hpp} )

	add_custom_command(
		COMMAND VideoHppGenerator
		OUTPUT "${vulkan_video_hpp}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "run VideoHppGenerator"
		DEPENDS VideoHppGenerator "${video_spec}" )
	add_custom_target( build_video_hpp ALL DEPENDS "${vulkan_video_hpp}" "${video_spec}" )
endif()

# Create Vulkan-Hpp interface target
add_library( VulkanHpp INTERFACE )
add_library( Vulkan::Hpp ALIAS VulkanHpp )
target_include_directories( VulkanHpp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}" )
if( VULKAN_HPP_RUN_GENERATOR )
	add_dependencies( VulkanHpp build_vulkan_hpp build_video_hpp )
endif()

# set up compile definitions
if( VULKAN_HPP_DISABLE_ENHANCED_MODE )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_DISABLE_ENHANCED_MODE" )
endif()
if( VK_NO_PROTOTYPES )
	target_compile_definitions(VulkanHpp INTERFACE "VK_NO_PROTOTYPES" )
endif()
if( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_DISPATCH_LOADER_DYNAMIC" )
	if( UNIX )
		target_link_libraries( VulkanHpp INTERFACE ${CMAKE_DL_LIBS} )
	endif()
else()
	find_package( Vulkan )
	if ( Vulkan_FOUND )
		target_link_libraries( VulkanHpp INTERFACE ${Vulkan_LIBRARIES} )
	else()
		message(WARNING "Could not link to vulkan-1 libraries, consider using VULKAN_HPP_DISPATCH_LOADER_DYNAMIC")
	endif()
endif()
if( VULKAN_HPP_USE_REFLECT )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_USE_REFLECT" )
endif()
if( NOT VULKAN_HPP_TYPESAFE_CONVERSION )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_TYPESAFE_CONVERSION=0" )
endif()
if( VULKAN_HPP_HANDLES_MOVE_EXCHANGE )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_HANDLES_MOVE_EXCHANGE" )
endif()
if( VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC" )
endif()
if( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST" )
endif()
if( VULKAN_HPP_NO_SETTERS )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_SETTERS" )
endif()
if( VULKAN_HPP_NO_TO_STRING )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_TO_STRING" )
endif()
if( VULKAN_HPP_NO_EXCEPTIONS )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_EXCEPTIONS" )
endif()
if( VULKAN_HPP_NO_CONSTRUCTORS )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_CONSTRUCTORS" )
endif()
if( VULKAN_HPP_NO_SMART_HANDLE )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_SMART_HANDLE" )
endif()
if( VULKAN_HPP_RAII_NO_EXCEPTIONS )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_RAII_NO_EXCEPTIONS" )
endif()
if( VULKAN_HPP_NO_WIN32_PROTOTYPES )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_WIN32_PROTOTYPES" )
endif()
if( VULKAN_HPP_NO_NODISCARD_WARNINGS )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_NODISCARD_WARNINGS" )
endif()
if( VULKAN_HPP_NO_SPACESHIP_OPERATOR )
	target_compile_definitions(VulkanHpp INTERFACE "VULKAN_HPP_NO_SPACESHIP_OPERATOR" )
endif()

function( vulkan_hpp__setup_vulkan_include )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	
	if( VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP )
		# Vulkan C++ headers
		target_link_libraries( ${TARGET_NAME} PUBLIC Vulkan::Hpp )

		# Vulkan C headers
		if ( EXISTS "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Vulkan-Headers/include" )
			target_include_directories( VulkanHpp INTERFACE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Vulkan-Headers/include" )
		else()
			message( WARNING "Could not find vulkan C headers locally, pull the Vulkan-Headers submodule or disable VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP" )
		endif()
	else()
		find_package( Vulkan REQUIRED )
		target_include_directories( ${TARGET_NAME} PUBLIC "${Vulkan_INCLUDE_DIRS}" )
	endif()
endfunction()

function( vulkan_hpp__setup_vulkan_module )
	set( options )
	set( oneValueArgs NAME FILE_SET_NAME )
	set( multiValueArgs FILE_SET ALIAS )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if ( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_module" )
	endif()

	add_library( ${TARGET_NAME} )
	foreach( TARGET_ALIAS_NAME IN LISTS TARGET_ALIAS )
		add_library(${TARGET_ALIAS_NAME} ALIAS ${TARGET_NAME})
	endforeach()

	# Clang 16's module support can be broken with extensions enabled
	if ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "16.0" )
		set_target_properties( ${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF )
	endif()

	if ( VULKAN_HPP_ENABLE_STD_MODULE )
		target_compile_features( ${TARGET_NAME} PUBLIC cxx_std_23 )
		set_target_properties( ${TARGET_NAME} PROPERTIES CXX_MODULE_STD ON )
	else()
		target_compile_features( ${TARGET_NAME} PUBLIC cxx_std_20 )
	endif()

	if ( NOT TARGET_FILE_SET )
		message( FATAL_ERROR "FILE_SET must be defined in vulkan_hpp__setup_module" )
	endif()

	list(LENGTH TARGET_FILE_SET TARGET_FILE_SET_LENGTH)
	if ( TARGET_FILE_SET_LENGTH LESS 1 )
		message(FATAL_ERROR "FILE_SET in vulkan_hpp__setup_module must at least one source file")
	endif()

	target_sources( ${TARGET_NAME} PUBLIC
		FILE_SET ${TARGET_FILE_SET_NAME}
		BASE_DIRS ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
		TYPE CXX_MODULES
		FILES ${TARGET_FILE_SET} )
	vulkan_hpp__setup_vulkan_include( NAME ${TARGET_NAME} )
endfunction()

# Build Vulkan-Hpp as a module
if( VULKAN_HPP_ENABLE_CPP20_MODULES )
	if ( VULKAN_HPP_ENABLE_STD_MODULE AND CMAKE_VERSION VERSION_LESS "3.30" )
		message( FATAL_ERROR "Vulkan-Hpp: C++20 modules with import std require CMake 3.30 or later" )
	elseif ( CMAKE_VERSION VERSION_LESS "3.28" )
		message( FATAL_ERROR "Vulkan-Hpp: C++20 modules require CMake 3.28 or later" )
	endif()

	# create targets providing VulkanHpp and VulkanVideoHpp as C++20 modules
	vulkan_hpp__setup_vulkan_module( NAME VulkanHppModule
		FILE_SET_NAME vulkan_module_file
		FILE_SET vulkan/vulkan.cppm
		ALIAS Vulkan::HppModule ) # TODO: respect VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP
	vulkan_hpp__setup_vulkan_module( NAME VulkanVideoHppModule
		FILE_SET_NAME vulkan_video_module_file
		FILE_SET vulkan/vulkan_video.cppm
		ALIAS Vulkan::VideoHppModule )
endif()

function( vulkan_hpp__setup_platform )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( WIN32 )
		target_compile_definitions( ${TARGET_NAME} PUBLIC VK_USE_PLATFORM_WIN32_KHR )
	elseif( APPLE )
		target_compile_definitions( ${TARGET_NAME} PUBLIC VK_USE_PLATFORM_METAL_EXT )
	elseif( UNIX )
		target_compile_definitions( ${TARGET_NAME} PUBLIC VK_USE_PLATFORM_XCB_KHR )
	else()
		message( FATAL_ERROR, "Vulkan-Hpp: unhandled platform!" )
	endif()
	if( !MSVC )
		target_compile_options( ${TARGET_NAME} PRIVATE -fno-strict-aliasing )
	endif()
endfunction()

function( vulkan_hpp__setup_project )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	project( ${TARGET_NAME} LANGUAGES CXX )
endfunction()

function( vulkan_hpp__setup_library )
	set( options SHARED )
	set( oneValueArgs FOLDER NAME )
	set( multiValueArgs HEADERS SOURCES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	vulkan_hpp__setup_project( NAME ${TARGET_NAME} )

	if( "${TARGET_SOURCES}" STREQUAL "" )
		add_library( ${TARGET_NAME} INTERFACE ${TARGET_HEADERS} )
	else()
		if( ${TARGET_SHARED} )
			add_library( ${TARGET_NAME} SHARED ${TARGET_SOURCES} ${TARGET_HEADERS} )
		else()
			add_library( ${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS} )
		endif()
		vulkan_hpp__setup_platform( NAME ${TARGET_NAME} )
		vulkan_hpp__setup_vulkan_include( NAME ${TARGET_NAME} )
		vulkan_hpp__setup_warning_level( NAME ${TARGET_NAME} )
		set_target_properties( ${TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON )
	endif()
	set_target_properties( ${TARGET_NAME} PROPERTIES FOLDER ${TARGET_FOLDER} )
endfunction()

function( vulkan_hpp__setup_sample )
	set( options )
	set( oneValueArgs FOLDER NAME PCH_REUSE )
	set( multiValueArgs HEADERS LIBS PCH SOURCES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP )
		find_package( Vulkan REQUIRED )
	endif()

	vulkan_hpp__setup_project( NAME ${TARGET_NAME} )

	add_executable( ${TARGET_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES} )

	vulkan_hpp__setup_platform( NAME ${TARGET_NAME} )
	vulkan_hpp__setup_vulkan_include( NAME ${TARGET_NAME} )
	vulkan_hpp__setup_warning_level( NAME ${TARGET_NAME} )

	set_target_properties( ${TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON )

	if( TARGET_FOLDER )
		set_target_properties( ${TARGET_NAME} PROPERTIES FOLDER "${TARGET_FOLDER}" )
	endif()

	if( TARGET_LIBS )
		target_link_libraries( ${TARGET_NAME} PRIVATE "${TARGET_LIBS}" )
	endif()

	if( VULKAN_HPP_PRECOMPILE )
		if( TARGET_PCH_REUSE )
			target_precompile_headers( ${TARGET_NAME} REUSE_FROM "${TARGET_PCH_REUSE}" )
		elseif( TARGET_PCH )
			target_precompile_headers( ${TARGET_NAME} PRIVATE "${TARGET_PCH}" )
		endif()
	endif()
endfunction()

function( vulkan_hpp__setup_sample_static )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_sample_static" )
	endif()

	find_package( Vulkan REQUIRED )

	vulkan_hpp__setup_sample(
		NAME         ${TARGET_NAME}
		FOLDER       Samples
		PCH          <vulkan/vulkan.hpp>
		SOURCES      ${TARGET_NAME}.cpp
		LIBS         ${Vulkan_LIBRARIES} )

	target_compile_definitions( ${TARGET_NAME} PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
endfunction()

function( vulkan_hpp__setup_sample_dynamic )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs HEADERS SOURCES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_sample_dynamic" )
	endif()
	if( NOT TARGET_SOURCES )
		set( TARGET_SOURCES ${TARGET_NAME}.cpp )
	endif()

	vulkan_hpp__setup_sample(
		NAME         ${TARGET_NAME}
		FOLDER       Samples
		PCH_REUSE    utils
		HEADERS      ${TARGET_HEADERS}
		SOURCES      ${TARGET_SOURCES}
		LIBS         utils )
endfunction()

function( vulkan_hpp__setup_sample_raii )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs HEADERS SOURCES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_sample_raii" )
	endif()
	if( NOT TARGET_SOURCES )
		set( TARGET_SOURCES ${TARGET_NAME}.cpp )
	endif()

	vulkan_hpp__setup_sample(
		NAME         RAII_${TARGET_NAME}
		FOLDER       RAII_Samples
		PCH_REUSE    utils
		HEADERS      ${TARGET_HEADERS}
		SOURCES      ${TARGET_SOURCES}
		LIBS         utils )
endfunction()

function( vulkan_hpp__setup_test )
	set( options CXX_MODULE )
	set( oneValueArgs NAME )
	set( multiValueArgs LIBRARIES COMPILE_DEFINITIONS )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_test" )
	endif()

	vulkan_hpp__setup_project( NAME ${TARGET_NAME} )

	# use a variant for the module version
	if( TARGET_CXX_MODULE )
		add_executable( ${TARGET_NAME}_module ${TARGET_NAME}.cpp )
		set( TARGET_NAME ${TARGET_NAME}_module )
		set_target_properties( ${TARGET_NAME} PROPERTIES CXX_SCAN_FOR_MODULES ON )

		# set up new vulkan module when custom compile definitions are needed
		list( LENGTH TARGET_COMPILE_DEFINITIONS COMPILE_DEFINITIONS_LENGTH )
		if ( COMPILE_DEFINITIONS_LENGTH GREATER 0 )
			vulkan_hpp__setup_vulkan_module( NAME ${TARGET_NAME}_VulkanHppModule
				FILE_SET_NAME vulkan_module_file
				FILE_SET "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/vulkan/vulkan.cppm" ) # TODO: respect VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP
			target_compile_definitions( ${TARGET_NAME}_VulkanHppModule PUBLIC ${TARGET_COMPILE_DEFINITIONS} )
			target_link_libraries( ${TARGET_NAME} PRIVATE ${TARGET_NAME}_VulkanHppModule )
		else()
			target_link_libraries( ${TARGET_NAME} PRIVATE Vulkan::HppModule )
		endif()
		# set macro to hint usage of the vulkan module, it is up to the test source file to respect this
		target_compile_definitions( ${TARGET_NAME} PUBLIC VULKAN_HPP_USE_CXX_MODULE )
	else()
		add_executable( ${TARGET_NAME} ${TARGET_NAME}.cpp )
		vulkan_hpp__setup_vulkan_include( NAME ${TARGET_NAME} )
		target_compile_definitions( ${TARGET_NAME} PUBLIC ${TARGET_COMPILE_DEFINITIONS} )
	endif()

	vulkan_hpp__setup_platform( NAME ${TARGET_NAME} )
	vulkan_hpp__setup_warning_level( NAME ${TARGET_NAME} )

	set_target_properties( ${TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON FOLDER "Tests" )
	target_link_libraries( ${TARGET_NAME} PRIVATE ${TARGET_LIBRARIES} )
endfunction()

if( VULKAN_HPP_SAMPLES_BUILD OR VULKAN_HPP_TESTS_BUILD )
	# external libraries
	find_package( glm QUIET )
	if( NOT glm_FOUND )
		add_subdirectory( glm )
	endif()

	find_package( glfw3 QUIET )
	if( NOT glfw3_FOUND )
		set( GLFW_BUILD_EXAMPLES OFF )
		set( GLFW_BUILD_TESTS OFF )
		add_subdirectory( glfw )
	endif()

	find_package( glslang QUIET )
	if( NOT glslang_FOUND )
		set( ENABLE_OPT OFF ) # could use ALLOW_EXTERNAL_SPIRV_TOOLS=ON instead
		add_subdirectory( glslang )
	endif()

	add_subdirectory( samples/utils )
	add_subdirectory( RAII_Samples/utils )
endif()

if( VULKAN_HPP_SAMPLES_BUILD )
	# samples
	add_subdirectory( samples )
	add_subdirectory( RAII_Samples )
endif()

if( VULKAN_HPP_TESTS_BUILD )
	add_subdirectory( tests )
endif()

if( ${VULKAN_HPP_INSTALL} )
	include( GNUInstallDirs )

	set( VK_GENERATED_VULKAN_HEADERS
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_enums.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_format_traits.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_funcs.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_handles.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_hash.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_hpp_macros.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_raii.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_static_assertions.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_structs.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_to_string.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_video.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan.cppm
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_video.cppm
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan.hpp
	)
	install( FILES ${VK_GENERATED_VULKAN_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vulkan )

endif()
