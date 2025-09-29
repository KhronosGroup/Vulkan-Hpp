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
// VulkanHpp Samples : FunctionCalls
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

#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main( int /*argc*/, char ** /*argv*/ )
{
  //=== VK_VERSION_1_0 ===
  // Device initialization
  {
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::Instance           instance;
    vk::Result             result = vk::createInstance( &instanceCreateInfo, nullptr, &instance );
  }
  {
    vk::InstanceCreateInfo instanceCreateInfo;
    vk::Instance           instance = vk::createInstance( instanceCreateInfo );
  }

  {
    vk::Instance instance;
    instance.destroy();
  }

  {
    vk::Instance instance;
    uint32_t     physicalDeviceCount;
    vk::Result   result = instance.enumeratePhysicalDevices( &physicalDeviceCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::PhysicalDevice> physicalDevices( physicalDeviceCount );
      result = instance.enumeratePhysicalDevices( &physicalDeviceCount, physicalDevices.data() );
    }
  }
  {
    vk::Instance                    instance;
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
  }
  {
    vk::Instance                       instance;
    std::allocator<vk::PhysicalDevice> allocator;
    std::vector<vk::PhysicalDevice>    physicalDevices = instance.enumeratePhysicalDevices( allocator );
  }

  {
    vk::PhysicalDevice         physicalDevice;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures;
    physicalDevice.getFeatures( &physicalDeviceFeatures );
  }
  {
    vk::PhysicalDevice         physicalDevice;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
  }

  {
    vk::PhysicalDevice   physicalDevice;
    vk::Format           format = {};
    vk::FormatProperties formatProperties;
    physicalDevice.getFormatProperties( format, &formatProperties );
  }
  {
    vk::PhysicalDevice   physicalDevice;
    vk::Format           format           = {};
    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( format );
  }

  {
    vk::PhysicalDevice        physicalDevice;
    vk::Format                format      = {};
    vk::ImageType             imageType   = {};
    vk::ImageTiling           imageTiling = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties;
    vk::Result result = physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags, &imageFormatProperties );
  }
  {
    vk::PhysicalDevice        physicalDevice;
    vk::Format                format      = {};
    vk::ImageType             imageType   = {};
    vk::ImageTiling           imageTiling = {};
    vk::ImageUsageFlags       imageUsageFlags;
    vk::ImageCreateFlags      imageCreateFlags;
    vk::ImageFormatProperties imageFormatProperties =
      physicalDevice.getImageFormatProperties( format, imageType, imageTiling, imageUsageFlags, imageCreateFlags );
  }

  {
    vk::PhysicalDevice           physicalDevice;
    vk::PhysicalDeviceProperties physicalDeviceProperties;
    physicalDevice.getProperties( &physicalDeviceProperties );
  }
  {
    vk::PhysicalDevice           physicalDevice;
    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
  }

  {
    vk::PhysicalDevice physicalDevice;
    uint32_t           queueFamilyPropertyCount;
    physicalDevice.getQueueFamilyProperties( &queueFamilyPropertyCount, nullptr );
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties( queueFamilyPropertyCount );
    physicalDevice.getQueueFamilyProperties( &queueFamilyPropertyCount, queueFamilyProperties.data() );
  }
  {
    vk::PhysicalDevice                     physicalDevice;
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  }
  {
    vk::PhysicalDevice                        physicalDevice;
    std::allocator<vk::QueueFamilyProperties> allocator;
    std::vector<vk::QueueFamilyProperties>    queueFamilyProperties = physicalDevice.getQueueFamilyProperties( allocator );
  }

  {
    vk::PhysicalDevice                 physicalDevice;
    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties( &memoryProperties );
  }
  {
    vk::PhysicalDevice                 physicalDevice;
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
  }

  {
    vk::Instance       instance;
    PFN_vkVoidFunction voidFunction = instance.getProcAddr( "vkCreateInstance" );
  }
  {
    vk::Instance       instance;
    std::string        name         = "vkCreateInstance";
    PFN_vkVoidFunction voidFunction = instance.getProcAddr( name );
  }

  {
    vk::Device         device;
    PFN_vkVoidFunction voidFunction = device.getProcAddr( "vkCreateInstance" );
  }
  {
    vk::Device         device;
    std::string        name         = "vkCreateInstance";
    PFN_vkVoidFunction voidFunction = device.getProcAddr( name );
  }

  // Device commands
  {
    vk::PhysicalDevice   physicalDevice;
    vk::DeviceCreateInfo deviceCreateInfo;
    vk::Device           device;
    vk::Result           result = physicalDevice.createDevice( &deviceCreateInfo, nullptr, &device );
  }
  {
    vk::PhysicalDevice   physicalDevice;
    vk::DeviceCreateInfo deviceCreateInfo;
    vk::Device           device = physicalDevice.createDevice( deviceCreateInfo );
  }

  {
    vk::Device device;
    device.destroy();
  }

  // Extension discovery commands
  {
    uint32_t   propertyCount;
    vk::Result result = vk::enumerateInstanceExtensionProperties( nullptr, &propertyCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::ExtensionProperties> properties( propertyCount );
      result = vk::enumerateInstanceExtensionProperties( nullptr, &propertyCount, properties.data() );
    }
  }
  {
    std::vector<vk::ExtensionProperties> properties = vk::enumerateInstanceExtensionProperties( nullptr );
  }
  {
    using Allocator = std::allocator<vk::ExtensionProperties>;
    Allocator                                       allocator;
    std::vector<vk::ExtensionProperties, Allocator> properties = vk::enumerateInstanceExtensionProperties( nullptr, allocator );
  }

#if 0
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    vk::Bool32         supported;
    uint32_t           queueFamilyIndex;
    vk::Result         result = physicalDevice.getSurfaceSupportKHR( queueFamilyIndex, surface, &supported );
  }
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    uint32_t           queueFamilyIndex;
    vk::Bool32         supported = physicalDevice.getSurfaceSupportKHR( queueFamilyIndex, surface );
  }
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR     surface;
    uint32_t           formatCount;
    vk::Result         result = physicalDevice.getSurfaceFormatsKHR( surface, &formatCount, nullptr );
    if ( result == vk::Result::eSuccess )
    {
      std::vector<vk::SurfaceFormatKHR> formats( formatCount );
      result = physicalDevice.getSurfaceFormatsKHR( surface, &formatCount, formats.data() );
    }
  }
  {
    vk::PhysicalDevice                physicalDevice;
    vk::SurfaceKHR                    surface;
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR( surface );
  }
  {
    vk::PhysicalDevice                   physicalDevice;
    vk::SurfaceKHR                       surface;
    std::allocator<vk::SurfaceFormatKHR> allocator;
    std::vector<vk::SurfaceFormatKHR>    formats = physicalDevice.getSurfaceFormatsKHR( surface, allocator );
  }
#endif

  return 0;
}
