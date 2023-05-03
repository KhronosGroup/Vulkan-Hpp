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

vulkan_hpp__setup_library(
	NAME RAII_utils
	HEADERS shaders.hpp utils.hpp
	FOLDER "RAII_Samples" )

target_link_libraries( RAII_utils INTERFACE utils )
target_compile_definitions( RAII_utils INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )

