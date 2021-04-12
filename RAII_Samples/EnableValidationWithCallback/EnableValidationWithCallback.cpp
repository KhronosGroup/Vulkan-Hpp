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
// VulkanHpp Samples : EnableValidationWithCallback
//                     Show how to enable validation layers and provide callback

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/utils.hpp"
#include "vulkan/vulkan_raii.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

static char const * AppName    = "EnableValidationWithCallback";
static char const * EngineName = "Vulkan.hpp";

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                                               const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                                               const VkAllocationCallbacks *              pAllocator,
                                                               VkDebugUtilsMessengerEXT *                 pMessenger )
{
  return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance                    instance,
                                                            VkDebugUtilsMessengerEXT      messenger,
                                                            VkAllocationCallbacks const * pAllocator )
{
  return pfnVkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                 VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                 void * /*pUserData*/ )
{
  std::string message;

  message += vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": " +
             vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) + ":\n";
  message += std::string( "\t" ) + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
  message += std::string( "\t" ) + "messageIdNumber = " + std::to_string( pCallbackData->messageIdNumber ) + "\n";
  message += std::string( "\t" ) + "message         = <" + pCallbackData->pMessage + ">\n";
  if ( 0 < pCallbackData->queueLabelCount )
  {
    message += std::string( "\t" ) + "Queue Labels:\n";
    for ( uint8_t i = 0; i < pCallbackData->queueLabelCount; i++ )
    {
      message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
    }
  }
  if ( 0 < pCallbackData->cmdBufLabelCount )
  {
    message += std::string( "\t" ) + "CommandBuffer Labels:\n";
    for ( uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ )
    {
      message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
    }
  }
  if ( 0 < pCallbackData->objectCount )
  {
    for ( uint8_t i = 0; i < pCallbackData->objectCount; i++ )
    {
      message += std::string( "\t" ) + "Object " + std::to_string( i ) + "\n";
      message += std::string( "\t\t" ) + "objectType   = " +
                 vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) + "\n";
      message +=
        std::string( "\t\t" ) + "objectHandle = " + std::to_string( pCallbackData->pObjects[i].objectHandle ) + "\n";
      if ( pCallbackData->pObjects[i].pObjectName )
      {
        message += std::string( "\t\t" ) + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
      }
    }
  }

#ifdef _WIN32
  MessageBox( NULL, message.c_str(), "Alert", MB_OK );
#else
  std::cout << message << std::endl;
#endif

  return false;
}

bool checkLayers( std::vector<char const *> const & layers, std::vector<vk::LayerProperties> const & properties )
{
  // return true if all layers are listed in the properties
  return std::all_of( layers.begin(), layers.end(), [&properties]( char const * name ) {
    return std::find_if( properties.begin(), properties.end(), [&name]( vk::LayerProperties const & property ) {
             return strcmp( property.layerName, name ) == 0;
           } ) != properties.end();
  } );
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    std::unique_ptr<vk::raii::Context> context = vk::raii::su::make_unique<vk::raii::Context>();

    std::vector<vk::LayerProperties> instanceLayerProperties = context->enumerateInstanceLayerProperties();

    /* VULKAN_KEY_START */

    // Use standard_validation meta layer that enables all recommended validation layers
    std::vector<char const *> instanceLayerNames;
    instanceLayerNames.push_back( "VK_LAYER_KHRONOS_validation" );
    if ( !checkLayers( instanceLayerNames, instanceLayerProperties ) )
    {
      std::cout << "Set the environment variable VK_LAYER_PATH to point to the location of your layers" << std::endl;
      exit( 1 );
    }

    /* Enable debug callback extension */
    std::vector<char const *> instanceExtensionNames;
    instanceExtensionNames.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

    vk::ApplicationInfo    applicationInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo, instanceLayerNames, instanceExtensionNames );
    std::unique_ptr<vk::raii::Instance> instance = vk::raii::su::make_unique<vk::raii::Instance>( *context, instanceCreateInfo );

    pfnVkCreateDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( instance->getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
    if ( !pfnVkCreateDebugUtilsMessengerEXT )
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
      exit( 1 );
    }

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      instance->getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
    if ( !pfnVkDestroyDebugUtilsMessengerEXT )
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
      exit( 1 );
    }

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
    vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
    vk::DebugUtilsMessengerCreateInfoEXT  debugUtilsMessengerCreateInfoEXT(
      {}, severityFlags, messageTypeFlags, &debugMessageFunc );
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::make_unique<vk::raii::DebugUtilsMessengerEXT>( *instance, debugUtilsMessengerCreateInfoEXT );

    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::makeUniquePhysicalDevice( *instance );

    // get the index of the first queue family that supports graphics
    uint32_t graphicsQueueFamilyIndex =
      vk::su::findGraphicsQueueFamilyIndex( physicalDevice->getQueueFamilyProperties() );

    float                             queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo         deviceQueueCreateInfo( {}, graphicsQueueFamilyIndex, 1, &queuePriority );
    vk::DeviceCreateInfo              deviceCreateInfo( {}, deviceQueueCreateInfo );
    std::unique_ptr<vk::raii::Device> device = vk::raii::su::make_unique<vk::raii::Device>( *physicalDevice, deviceCreateInfo );

    // Create a vk::CommandPool (not a vk::raii::CommandPool, for testing purposes!)
    vk::CommandPoolCreateInfo commandPoolCreateInfo( {}, graphicsQueueFamilyIndex );
    vk::CommandPool           commandPool =
      ( **device ).createCommandPool( commandPoolCreateInfo, nullptr, *device->getDispatcher() );

    // The commandPool is not destroyed automatically (as it's not a UniqueCommandPool.
    // That is, the device is destroyed before the commmand pool and will trigger a validation error.
    std::cout << "*** INTENTIONALLY destroying the Device before destroying a CommandPool ***\n";
    std::cout << "*** The following error message is EXPECTED ***\n";

    /* VULKAN_KEY_END */
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }
  return 0;
}
