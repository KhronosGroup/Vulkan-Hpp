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

add_library(utils 
  math.cpp
  shaders.cpp
  utils.cpp
)

target_link_libraries(utils PUBLIC glfw)
target_link_libraries(utils PUBLIC glslang)
target_link_libraries(utils PUBLIC glslang-default-resource-limits)
target_link_libraries(utils PUBLIC SPIRV)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(utils PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)