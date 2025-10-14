# Copyright(c) 2022, NVIDIA CORPORATION. All rights reserved.
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

set(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ON)
set(VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST ON)
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()

# TODO: this test should be simplified to remove the additional deps such as glsl (linked via utils)
target_link_libraries( UniqueHandle PRIVATE utils )

# add_subdirectory cannot use the normal compile definitions
if( NOT VULKAN_HPP_TESTS_BUILD_CTEST )
    target_compile_definitions( UniqueHandle PRIVATE VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
endif()
