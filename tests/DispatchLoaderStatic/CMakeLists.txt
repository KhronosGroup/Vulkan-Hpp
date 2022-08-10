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

if (NOT TESTS_BUILD_ONLY_DYNAMIC)

  find_package(Vulkan REQUIRED)

  project(DispatchLoaderStatic)

  set(HEADERS
  )

  set(SOURCES
    DispatchLoaderStatic.cpp
  )

  source_group(headers FILES ${HEADERS})
  source_group(sources FILES ${SOURCES})

  add_executable(DispatchLoaderStatic
    ${HEADERS}
    ${SOURCES}
    )

  if (UNIX)
    target_link_libraries(DispatchLoaderStatic PRIVATE ${CMAKE_DL_LIBS})
  endif()

  set_target_properties(DispatchLoaderStatic PROPERTIES FOLDER "Tests")
  target_link_libraries(DispatchLoaderStatic PRIVATE utils ${Vulkan_LIBRARIES})

endif()