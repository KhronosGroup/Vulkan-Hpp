# Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
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

cmake_minimum_required(VERSION 3.2)

project(VulkanHppGenerator)

# If you wish to target a specific version of the spec, override this value like so 
# cmake -DVK_SPEC_URL:STRING=https://example.com/some/other/url/vk.xml
set(VK_SPEC_URL "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/f4c4113d070c0c9b55b4482055be054dd6466c96/src/spec/vk.xml" CACHE STRING "URL from which to fetch the Vulkan Spec XML")

# Download the request URL specification
file(DOWNLOAD ${VK_SPEC_URL} ${CMAKE_CURRENT_BINARY_DIR}/vk.xml )

# Populate a preprocessor define with the location of the spec
file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/vk.xml VK_SPEC)
string(REPLACE "\\" "\\\\" VK_SPEC ${VK_SPEC})
add_definitions(-DVK_SPEC="${VK_SPEC}")

# Populate a preprocessor define with the desire output location
file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/vulkan.hpp vulkan_hpp)
string(REPLACE "\\" "\\\\" vulkan_hpp ${vulkan_hpp})
add_definitions(-DVULKAN_HPP="${vulkan_hpp}")

# Create the executable project
set(SOURCES VulkanHppGenerator.cpp)
source_group(sources FILES ${SOURCES})
add_executable(VulkanHppGenerator ${SOURCES} )
set_property(TARGET VulkanHppGenerator PROPERTY CXX_STANDARD 11)

if (WIN32)
	include(ExternalProject)
	# Fetch and build tinyxml in order to parse the specification
	ExternalProject_Add(
	    tinyxml2
	    DOWNLOAD_NAME tinyxml2-c8dad95d4488663c0381d502b3274df7dbf2fc55.zip
	    URL https://codeload.github.com/leethomason/tinyxml2/zip/c8dad95d4488663c0381d502b3274df7dbf2fc55
	    URL_MD5 6984b655cae1cd4b54fccaebfe23b35c
	    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
	    LOG_DOWNLOAD 1
	)
	# Grab the include path and library and store them  
	ExternalProject_Get_Property(tinyxml2 INSTALL_DIR)
	# Make sure tinyxml build first, and is available to the generator
	add_dependencies(VulkanHppGenerator tinyxml2)
	target_include_directories(VulkanHppGenerator PRIVATE ${INSTALL_DIR}/include)
	target_link_libraries(VulkanHppGenerator ${INSTALL_DIR}/lib/tinyxml2.lib)
else()
	include(FindPkgConfig)
	pkg_check_modules(TINYXML2 REQUIRED tinyxml2)
	link_directories(${TINYXML2_LIBDIR})
	target_include_directories(VulkanHppGenerator PRIVATE ${TINYXML2_INCLUDEDIR})
	target_link_libraries(VulkanHppGenerator ${TINYXML2_LIBRARIES})
endif()

# Execute the generator
add_custom_command(TARGET VulkanHppGenerator POST_BUILD COMMAND VulkanHppGenerator)

# Install the output file to the destination
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vulkan.hpp" DESTINATION "include/vulkan")
