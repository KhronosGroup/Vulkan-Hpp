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
// VulkanHpp Samples : NoExceptions
//                     Compile test with VULKAN_HPP_NO_EXCEPTIONS set
//                     Note: this is _no_ functional test!! Don't ever code this way!!

#define VULKAN_HPP_NO_EXCEPTIONS

#include "vulkan/vulkan.hpp"

static char const* AppName = "Constexpr";
static char const* EngineName = "Vulkan.hpp";

constexpr vk::AccessFlags foo(vk::AccessFlags rhs)
{
  vk::AccessFlags accessFlags;
  accessFlags = rhs;
  return accessFlags;
}

int main(int /*argc*/, char ** /*argv*/)
{
  constexpr vk::AccessFlags accessFlags0;
  constexpr vk::AccessFlags accessFlags1(vk::AccessFlagBits::eIndirectCommandRead);
  constexpr vk::AccessFlags accessFlags2(accessFlags1);
  constexpr vk::AccessFlags accessFlags3(VK_ACCESS_INDIRECT_COMMAND_READ_BIT);

  constexpr vk::AccessFlags accessFlags4 = accessFlags3 | accessFlags2;
  constexpr vk::AccessFlags accessFlags5 = accessFlags3 & accessFlags2;
  constexpr vk::AccessFlags accessFlags6 = accessFlags3 ^ accessFlags2;

  constexpr bool b0 = !accessFlags6;
  //constexpr vk::AccessFlags accessFlags7 = ~accessFlags6;   // expression did not evaluate to a constant

  constexpr bool b1 = (accessFlags6 == accessFlags5);
  constexpr bool b2 = (accessFlags6 != accessFlags5);

  constexpr bool b3 = static_cast<bool>(accessFlags6);
  constexpr VkAccessFlags vkAccessFlags = static_cast<VkAccessFlags>(accessFlags6);

  constexpr vk::AccessFlags accessFlags8 = vk::AccessFlagBits::eIndirectCommandRead | accessFlags6;
  constexpr vk::AccessFlags accessFlags9 = vk::AccessFlagBits::eIndirectCommandRead & accessFlags6;
  constexpr vk::AccessFlags accessFlags10 = vk::AccessFlagBits::eIndirectCommandRead ^ accessFlags6;

  constexpr vk::AccessFlags accessFlags11 = foo(accessFlags10);

  return 0;
}
