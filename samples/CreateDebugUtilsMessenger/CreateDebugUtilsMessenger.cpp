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
// VulkanHpp Samples : CreateDebugReportMessenger
//                     Draw a cube

#include <iostream>
#include <sstream>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "CreateDebugReportMessenger";
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

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const * pAllocator )
{
  return pfnVkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                 VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                 void * /*pUserData*/ )
{
  std::ostringstream message;

  message << vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) << ": "
          << vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
  message << std::string( "\t" ) << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
  message << std::string( "\t" ) << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
  message << std::string( "\t" ) << "message         = <" << pCallbackData->pMessage << ">\n";
  if ( 0 < pCallbackData->queueLabelCount )
  {
    message << std::string( "\t" ) << "Queue Labels:\n";
    for ( uint32_t i = 0; i < pCallbackData->queueLabelCount; i++ )
    {
      message << std::string( "\t\t" ) << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
    }
  }
  if ( 0 < pCallbackData->cmdBufLabelCount )
  {
    message << std::string( "\t" ) << "CommandBuffer Labels:\n";
    for ( uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ )
    {
      message << std::string( "\t\t" ) << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
    }
  }
  if ( 0 < pCallbackData->objectCount )
  {
    message << std::string( "\t" ) << "Objects:\n";
    for ( uint32_t i = 0; i < pCallbackData->objectCount; i++ )
    {
      message << std::string( "\t\t" ) << "Object " << i << "\n";
      message << std::string( "\t\t\t" ) << "objectType   = " << vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) << "\n";
      message << std::string( "\t\t\t" ) << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
      if ( pCallbackData->pObjects[i].pObjectName )
      {
        message << std::string( "\t\t\t" ) << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
      }
    }
  }

#ifdef _WIN32
  MessageBox( NULL, message.str().c_str(), "Alert", MB_OK );
#else
  std::cout << message.str() << std::endl;
#endif

  return false;
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> props = vk::enumerateInstanceExtensionProperties();

    auto propertyIterator = std::find_if(
      props.begin(), props.end(), []( vk::ExtensionProperties const & ep ) { return strcmp( ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) == 0; } );
    if ( propertyIterator == props.end() )
    {
      std::cout << "Something went very wrong, cannot find " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " extension" << std::endl;
      exit( 1 );
    }

    vk::ApplicationInfo applicationInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    const char *        extensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    vk::Instance        instance      = vk::createInstance( vk::InstanceCreateInfo( vk::InstanceCreateFlags(), &applicationInfo, {}, extensionName ) );

    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( instance.getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
    if ( !pfnVkCreateDebugUtilsMessengerEXT )
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
      exit( 1 );
    }

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>( instance.getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
    if ( !pfnVkDestroyDebugUtilsMessengerEXT )
    {
      std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
      exit( 1 );
    }

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
    vk::DebugUtilsMessengerEXT        debugUtilsMessenger =
      instance.createDebugUtilsMessengerEXT( vk::DebugUtilsMessengerCreateInfoEXT( {}, severityFlags, messageTypeFlags, &debugMessageFunc ) );

    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
    instance.destroy();

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
