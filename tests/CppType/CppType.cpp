// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// VulkanHpp Tests : CppType
//                   Compile test on using vk::CppType

#include <vulkan/vulkan.hpp>

static_assert( std::is_same<vk::CppType<vk::IndexType, vk::IndexType::eUint16>::Type, uint16_t>::value, "" );
static_assert( std::is_same<vk::CppType<vk::ObjectType, vk::ObjectType::eInstance>::Type, vk::Instance>::value, "" );
static_assert( std::is_same<vk::CppType<vk::DebugReportObjectTypeEXT, vk::DebugReportObjectTypeEXT ::eInstance>::Type, vk::Instance>::value, "" );

#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
static_assert( std::is_same<vk::CppType<VkInstance>::Type, vk::Instance>::value, "" );
#endif

#include <vulkan/vulkan_raii.hpp>

static_assert( std::is_same<vk::raii::Instance::CppType, vk::Instance>::value, "" );

int main( int /*argc*/, char ** /*argv*/ )
{
  return 0;
}