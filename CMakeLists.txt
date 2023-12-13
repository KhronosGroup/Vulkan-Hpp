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

if ( NOT CMAKE_CXX_STANDARD )
	set( CMAKE_CXX_STANDARD 11 )
endif()

# all the options for this project
option( VULKAN_HPP_PRECOMPILE "Precompile vulkan.hpp and vulkan_raii.hpp for sample builds" ON )
option( VULKAN_HPP_RUN_GENERATOR "Run the HPP generator" OFF )
option( VULKAN_HPP_SAMPLES_BUILD "Build samples" OFF )
option( VULKAN_HPP_TESTS_BUILD "Build tests" OFF )
option( VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC "Build only dynamic. Required in case the Vulkan SDK is not available" OFF )
option( VULKAN_HPP_TESTS_BUILD_ONLY_DYNAMIC "Build only dynamic" OFF )
option( VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP "Build with local Vulkan headers" ON )
option( VULKAN_HPP_ENABLE_CPP20_MODULES "Build Vulkan-Hpp as C++20 module; requires minimum CMake version 3.28" OFF )
option( VULKAN_HPP_CPP20_MODULE_DYNAMIC_DISPATCHER "Build C++20 module with dynamic Dispatcher" ON )

if ( VULKAN_HPP_ENABLE_CPP20_MODULES )
	cmake_minimum_required( VERSION 3.28 )
else()
	cmake_minimum_required( VERSION 3.12 )
endif()

project( VulkanHppGenerator LANGUAGES CXX )

function( vulkan_hpp__setup_platform )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( WIN32 )
		target_compile_definitions( ${TARGET_NAME} PUBLIC NOMINMAX VK_USE_PLATFORM_WIN32_KHR )
	elseif( APPLE )
		target_compile_definitions( ${TARGET_NAME} PUBLIC VK_USE_PLATFORM_MACOS_MVK )
	elseif( UNIX )
		target_compile_definitions( ${TARGET_NAME} PUBLIC VK_USE_PLATFORM_XCB_KHR )
	else()
		message( FATAL_ERROR, "Vulkan-Hpp: unhandled platform!" )
	endif()
endfunction()

function( vulkan_hpp__setup_vulkan_include )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs )
	cmake_parse_arguments( TARGET "{options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	
	if( VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP )
		target_include_directories( ${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" )
		target_include_directories( ${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Vulkan-Headers/include" )
		if( VULKAN_HPP_RUN_GENERATOR )
			add_dependencies( ${TARGET_NAME} build_vulkan_hpp )
		endif()
	else()
		target_include_directories( ${TARGET_NAME} PUBLIC "${Vulkan_INCLUDE_DIRS}" )
	endif()
endfunction()

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
		target_compile_options( ${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror )
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
	set( multiValueArgs HEADERS INCLUDE_DIRS LIBS PCH SOURCES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT (VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC AND VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP) )
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

	if( TARGET_INCLUDE_DIRS )
		target_include_directories( ${TARGET_NAME} PUBLIC ${TARGET_INCLUDE_DIRS} )
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

	if( NOT VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC )
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
	endif()
endfunction()

function( vulkan_hpp__setup_sample_dynamic )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs HEADERS INCLUDE_DIRS SOURCES )
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
		INCLUDE_DIRS ${TARGET_INCLUDE_DIRS}
		HEADERS      ${TARGET_HEADERS}
		SOURCES      ${TARGET_SOURCES}
		LIBS         utils )
endfunction()

function( vulkan_hpp__setup_sample_raii )
	set( options )
	set( oneValueArgs NAME )
	set( multiValueArgs HEADERS INCLUDE_DIRS SOURCES )
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
		INCLUDE_DIRS ${TARGET_INCLUDE_DIRS}
		HEADERS      ${TARGET_HEADERS}
		SOURCES      ${TARGET_SOURCES}
		LIBS         utils )
endfunction()

function( vulkan_hpp__setup_test )
	set( options NO_UTILS )
	set( oneValueArgs CXX_STANDARD NAME )
	set( multiValueArgs LIBRARIES )
	cmake_parse_arguments( TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if( NOT (VULKAN_HPP_TESTS_BUILD_ONLY_DYNAMIC AND VULKAN_HPP_BUILD_WITH_LOCAL_VULKAN_HPP) )
		find_package( Vulkan REQUIRED )
	endif()

	if( NOT TARGET_NAME )
		message( FATAL_ERROR "NAME must be defined in vulkan_hpp__setup_test" )
	endif()
	if( NOT TARGET_CXX_STANDARD )
		set( TARGET_CXX_STANDARD ${CMAKE_CXX_STANDARD} )
	endif()

	vulkan_hpp__setup_project( NAME ${TARGET_NAME} )

	add_executable( ${TARGET_NAME} ${TARGET_NAME}.cpp )

	vulkan_hpp__setup_platform( NAME ${TARGET_NAME} )
	vulkan_hpp__setup_vulkan_include( NAME ${TARGET_NAME} )
	vulkan_hpp__setup_warning_level( NAME ${TARGET_NAME} )

	set_target_properties( ${TARGET_NAME} PROPERTIES CXX_STANDARD ${TARGET_CXX_STANDARD} CXX_STANDARD_REQUIRED ON FOLDER "Tests" )
	target_include_directories( ${TARGET_NAME} PUBLIC ${VulkanHeaders_INCLUDE_DIR} )
	target_include_directories( ${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/glm" )
	if ( NOT ${TARGET_NO_UTILS} )
		target_link_libraries( ${TARGET_NAME} PRIVATE utils )
	endif()
	target_link_libraries( ${TARGET_NAME} PRIVATE ${TARGET_LIBRARIES} )
endfunction()

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

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
		add_definitions( -DCLANG_FORMAT_EXECUTABLE="${CLANG_FORMAT_EXECUTABLE}" )
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
		else()
			message(STATUS " Using .clang-format version 15." )
			file( READ ".clang-format_15" clangFormat )
		endif()
		file( WRITE ".clang-format" ${clangFormat} )
	endif()
else()
	message( WARNING " Could not find clang-format. Generated vulkan.hpp and vulkan_raii.hpp will not be nicely formatted." )
endif()

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

# Build Vulkan-Hpp as a module
if( VULKAN_HPP_ENABLE_CPP20_MODULES )
	# create a target to provide VulkanHpp as C++20 module
	add_library( VulkanHppModule )
	set_target_properties( VulkanHppModule PROPERTIES
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF )
	if ( VULKAN_HPP_CPP20_MODULE_DYNAMIC_DISPATCHER )
		target_compile_definitions( VulkanHppModule PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )
	else()
		target_compile_definitions( VulkanHppModule PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
	endif()
	target_compile_features( VulkanHppModule PUBLIC cxx_std_20 )
	target_sources( VulkanHppModule
		PUBLIC
		FILE_SET vulkan_module_file BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} TYPE CXX_MODULES FILES vulkan/vulkan.cppm )
	target_include_directories( VulkanHppModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
	target_include_directories( VulkanHppModule PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan-Headers/include" )
endif()

# The generator executable
add_executable( VulkanHppGenerator VulkanHppGenerator.cpp VulkanHppGenerator.hpp XMLHelper.hpp ${TINYXML2_SOURCES} ${TINYXML2_HEADERS} )
vulkan_hpp__setup_warning_level( NAME VulkanHppGenerator )
target_compile_definitions( VulkanHppGenerator PUBLIC BASE_PATH="${CMAKE_CURRENT_SOURCE_DIR}" VK_SPEC="${vk_spec}" )
target_include_directories( VulkanHppGenerator PRIVATE ${VULKAN_HPP_TINYXML2_SRC_DIR} )
set_target_properties( VulkanHppGenerator PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON )

# The video generator executable
add_executable( VideoHppGenerator VideoHppGenerator.cpp VideoHppGenerator.hpp XMLHelper.hpp ${TINYXML2_SOURCES} ${TINYXML2_HEADERS} )
vulkan_hpp__setup_warning_level( NAME VideoHppGenerator )
file( TO_NATIVE_PATH ${VulkanRegistry_DIR}/video.xml video_spec )
string( REPLACE "\\" "\\\\" video_spec ${video_spec} )
target_compile_definitions( VideoHppGenerator PUBLIC BASE_PATH="${CMAKE_CURRENT_SOURCE_DIR}" VIDEO_SPEC="${video_spec}" )
target_include_directories( VideoHppGenerator PRIVATE  ${VULKAN_HPP_TINYXML2_SRC_DIR} )
set_target_properties( VideoHppGenerator PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON )


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
		COMMAND VulkanHppGenerator -api vulkansc
		OUTPUT "${vulkan_hpp}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "run VulkanHppGenerator"
		DEPENDS VulkanHppGenerator "${vk_spec}" )
	add_custom_target( build_vulkan_hpp ALL DEPENDS "${vulkan_hpp}" "${vk_spec}" )

	add_custom_command(
		COMMAND VideoHppGenerator
		OUTPUT "${vulkan_video_hpp}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "run VulkanVideoGenerator"
		DEPENDS VulkanVideoGenerator "${video_spec}" )
	add_custom_target( build_vulkan_video ALL DEPENDS "${vulkan_video_hpp}" "${video_spec}" )
endif()

if( VULKAN_HPP_SAMPLES_BUILD )
	# external libraries
	add_subdirectory( glm )
	set( GLFW_BUILD_EXAMPLES OFF )
	set( GLFW_BUILD_TESTS OFF )
	add_subdirectory( glfw )
	add_subdirectory( glslang )
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
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_raii.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_static_assertions.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_structs.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan_to_string.hpp
		${VulkanHeaders_INCLUDE_DIR}/vulkan/vulkan.cppm
	)
	install( FILES ${VK_GENERATED_VULKAN_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vulkan )

endif()
