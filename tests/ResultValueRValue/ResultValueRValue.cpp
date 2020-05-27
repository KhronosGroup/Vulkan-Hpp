// Copyright(c) 2020, Collabora Ltd. All rights reserved.
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
// VulkanHpp Test: Compile only test for issue 589.

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "vulkan/vulkan.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#if defined(VULKAN_DISABLE_IMPLICIT_RESULT_VALUE_CAST)
  static_assert(false, "Conversions not enabled");
#endif

void as_value(int)  {}
void as_ref(int&) {}
void as_cref(const int&) {}
void as_rvref(int&&)  {}
void as_crvref(const int&&)  {}

void test()
{
  using result = vk::ResultValue<int>;

  auto val        = result {vk::Result{}, 42};
  const auto cval = result {vk::Result{}, 42}; 

  as_value(val);
  as_value(cval);

  as_ref(val);
  //as_ref(cval); // should fail
  as_cref(val);
  as_cref(cval);

  as_rvref(std::move(val));
  //as_rvref(std::move(cval)); // should fail
  as_crvref(std::move(val));
  as_crvref(std::move(cval));
}
