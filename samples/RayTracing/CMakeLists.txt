# Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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

project(RayTracing)

set(HEADERS
  CameraManipulator.hpp
  ../utils/shaders.hpp
  ../utils/utils.hpp
)

set(SOURCES
  CameraManipulator.cpp
  RayTracing.cpp
  ../utils/shaders.cpp
  ../utils/utils.cpp
  ../../glslang/StandAlone/ResourceLimits.cpp
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

add_executable(RayTracing
  ${HEADERS}
  ${SOURCES}
)

set_target_properties(RayTracing PROPERTIES FOLDER "Samples")
target_include_directories(RayTracing PUBLIC
  ${CMAKE_SOURCE_DIR}/glslang
  ${CMAKE_SOURCE_DIR}/glfw/include
  ${CMAKE_SOURCE_DIR}/samples/RayTracing/common
  ${CMAKE_SOURCE_DIR}/samples/RayTracing/vulkannv 
  ${CMAKE_SOURCE_DIR}/stb
  ${CMAKE_SOURCE_DIR}/tinyobjloader
  )

target_link_libraries(RayTracing PUBLIC glslang SPIRV
  legacy_stdio_definitions.lib
  ${CMAKE_SOURCE_DIR}/builds/VS2017_64/glfw/src/Debug/glfw3.lib
  "${Vulkan_LIBRARIES}"
  )
