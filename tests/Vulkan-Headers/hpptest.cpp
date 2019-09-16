/*
 * Copyright (c) 2019 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This is a trivial test program to help verify that newly
 * generated versions of vulkan.hpp actually compile. It is
 * intended for use only when creating updates to the
 * Vulkan-Headers repository that accompany new Vulkan
 * Specification updates.
 */

#include <vulkan/vulkan.hpp>
int main()
{
    auto const instance_info = vk::InstanceCreateInfo();
    vk::Instance instance;
    vk::createInstance(&instance_info, nullptr, &instance);
}
