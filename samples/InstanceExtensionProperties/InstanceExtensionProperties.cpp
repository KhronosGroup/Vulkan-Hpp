// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : InstanceExtensionProperties
//                     Get global extension properties to know what extension are available to enable at CreateInstance time.

#include "vulkan/vulkan.hpp"
#include <iostream>
#include <sstream>

static char const* AppName = "InstanceExtensionProperties";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();

    std::cout << "Instance Extensions:" << std::endl;
    for (auto const& instanceExtensionProperty : instanceExtensionProperties)
    {
      std::cout << instanceExtensionProperty.extensionName << ":" << std::endl;
      std::cout << "\tVersion: " << instanceExtensionProperty.specVersion << std::endl;
      std::cout << std::endl;
    }

    /* VULKAN_KEY_END */
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error err)
  {
    std::cout << "std::runtime_error: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
