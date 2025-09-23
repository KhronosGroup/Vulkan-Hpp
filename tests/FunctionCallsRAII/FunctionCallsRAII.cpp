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
// VulkanHpp Samples : FunctionCallsRAII
//                     Compile test on using function calls to catch API changes

#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )  // local variable is initialized but not referenced
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#include <vulkan/vulkan_raii.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  //=== VK_VERSION_1_0 ===
  // Device initialization
  {
    vk::raii::Context      context;
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::raii::Instance     instance = context.createInstance( instanceCreateInfo );
  }
  {
    vk::raii::Context      context;
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::raii::Instance     instance( context, instanceCreateInfo );
  }

  {
    vk::raii::Instance                    instance        = nullptr;
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
  }
  {
    vk::raii::Instance        instance = nullptr;
    vk::raii::PhysicalDevices physicalDevices( instance );
  }

  {
    vk::raii::PhysicalDevice   physicalDevice         = nullptr;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
  }

  {
    vk::raii::PhysicalDevice physicalDevice   = nullptr;
    vk::Format               format           = {};
    vk::FormatProperties     formatProperties = physicalDevice.getFormatProperties( format );
  }

  {
    vk::raii::PhysicalDevice  physicalDevice = nullptr;
    vk::Format                format         = {};
    vk::ImageType             imageType      = {};
    vk::ImageTiling           imageTiling    = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties =
      physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags );
  }

  {
    vk::raii::PhysicalDevice     physicalDevice           = nullptr;
    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
  }

  {
    vk::raii::PhysicalDevice               physicalDevice        = nullptr;
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  }

  {
    vk::raii::PhysicalDevice           physicalDevice   = nullptr;
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
  }

  {
    vk::raii::Instance   instance     = nullptr;
    vk::PFN_VoidFunction voidFunction = instance.getProcAddr( "vkCreateInstance" );
  }

  {
    vk::raii::Device     device       = nullptr;
    vk::PFN_VoidFunction voidFunction = device.getProcAddr( "vkCreateInstance" );
  }

  return 0;
}
