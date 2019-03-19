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

cmake_minimum_required(VERSION 3.2)

find_package(Vulkan REQUIRED)

project(Vulkan-Hpp_Samples)

option (SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP OFF)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-DNOMINMAX -DVK_USE_PLATFORM_WIN32_KHR)
else()
  error("unhandled platform !")
endif()

FILE (GLOB linkunits ${CMAKE_CURRENT_SOURCE_DIR}/*)

if (SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP)
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Vulkan-Docs/include")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
else()
  include_directories("${Vulkan_INCLUDE_DIRS}")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../glm")

FOREACH( linkunit ${linkunits} )
  if( IS_DIRECTORY ${linkunit} )
    if( EXISTS ${linkunit}/CMakeLists.txt )
      string( REGEX REPLACE "^.*/([^/]*)$" "\\1" LINK_NAME ${linkunit} )
      add_subdirectory( ${LINK_NAME} )
    endif()
  endif()
ENDFOREACH( linkunit ${linkunits} )
