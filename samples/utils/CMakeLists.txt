# Copyright(c) 2020, NVIDIA CORPORATION. All rights reserved.
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

project(utils)

set(HEADERS
  math.hpp
  shaders.hpp
  utils.hpp
)

set(SOURCES
  math.cpp
  shaders.cpp
  utils.cpp
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

add_library(utils 
  ${SOURCES}
  ${HEADERS}
)

target_link_libraries(utils PUBLIC glfw)
target_link_libraries(utils PUBLIC glslang)
target_link_libraries(utils PUBLIC glslang-default-resource-limits)
target_link_libraries(utils PUBLIC SPIRV)
target_compile_definitions(utils PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

