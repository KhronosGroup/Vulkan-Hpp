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
// VulkanHpp Samples : SurfaceFormats
//                     Get surface formats.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include <iomanip>
#include <sstream>
#include <vector>

static char const* AppName = "SurfaceFormats";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, {}, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger(instance);
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    vk::su::SurfaceData surfaceData(instance, AppName, vk::Extent2D(500, 500));

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for (size_t i=0 ; i<physicalDevices.size() ; i++)
    {
      std::cout << "PhysicalDevice " << i << "\n";
      std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevices[i].getSurfaceFormatsKHR(*surfaceData.surface);
      for (size_t j = 0; j < surfaceFormats.size(); j++)
      {
        std::cout << "\tFormat " << j << "\n";
        std::cout << "\t\t" << "colorSpace  = " << vk::to_string(surfaceFormats[j].colorSpace) << "\n";
        std::cout << "\t\t" << "format      = " << vk::to_string(surfaceFormats[j].format) << "\n";
        std::cout << "\n";
      }
    }

    /* VULKAN_KEY_END */
  }
  catch (vk::SystemError& err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error& err)
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
