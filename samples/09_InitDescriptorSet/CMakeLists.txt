# Copyright(c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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

project(09_InitDescriptorSet)

set(HEADERS
)

set(SOURCES
  09_InitDescriptorSet.cpp
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

add_executable(09_InitDescriptorSet
  ${HEADERS}
  ${SOURCES}
)

set_target_properties(09_InitDescriptorSet PROPERTIES FOLDER "Samples")
target_link_libraries(09_InitDescriptorSet PRIVATE utils)
