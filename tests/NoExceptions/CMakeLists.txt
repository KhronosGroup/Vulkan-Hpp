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

# TODO: use VULKAN_HPP_DISPATCH_LOADER_[DYNAMIC|STATIC]_TYPE (PR #2244)
vulkan_hpp__setup_test( NAME NoExceptions COMPILE_DEFINITIONS VULKAN_HPP_NO_EXCEPTIONS VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
vulkan_hpp__setup_test( NAME NoExceptions CXX_MODULE COMPILE_DEFINITIONS VULKAN_HPP_NO_EXCEPTIONS VULKAN_HPP_DISPATCH_LOADER_DYNAMIC )
