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

project(RAII_08_InitPipelineLayout)

set(HEADERS
)

set(SOURCES
  08_InitPipelineLayout.cpp
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

add_executable(RAII_08_InitPipelineLayout
  ${HEADERS}
  ${SOURCES}
)

set_target_properties(RAII_08_InitPipelineLayout PROPERTIES FOLDER "RAII_Samples")
target_link_libraries(RAII_08_InitPipelineLayout PRIVATE utils)
