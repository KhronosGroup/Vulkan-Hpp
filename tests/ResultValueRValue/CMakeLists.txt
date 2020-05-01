# Copyright(c) 2020, Collabora Ltd. All rights reserved.
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

project(ResultValueRValue)

set(HEADERS
)

set(SOURCES
  ResultValueRValue.cpp
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

add_library(ResultValueRValue
  ${HEADERS}
  ${SOURCES}
)

if (UNIX)
  target_link_libraries(ResultValueRValue "-ldl")
endif()

set_target_properties(ResultValueRValue PROPERTIES FOLDER "Tests")
