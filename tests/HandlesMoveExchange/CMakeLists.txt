# Copyright(c) 2024, NVIDIA CORPORATION. All rights reserved.
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

cmake_minimum_required( VERSION 3.12 )
project( VulkanHppTest LANGUAGES CXX )

set(VULKAN_HPP_HANDLES_MOVE_EXCHANGE ON)
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt )
vulkan_hpp__setup_test_project()

# add_subdirectory cannot use the normal compile definitions
if( NOT VULKAN_HPP_TESTS_BUILD_CTEST )
    target_compile_definitions( HandlesMoveExchange PRIVATE VULKAN_HPP_HANDLES_MOVE_EXCHANGE )
endif()