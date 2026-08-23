// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : CreateDebugReportMessenger
//                     Draw a cube

#if defined( VULKAN_HPP_USE_CXX_MODULE )
#include <vulkan/vulkan_core.h>
import RAII_utils;
import std;
import vulkan;
#else
#include "../utils/utils.hpp"
#include <iostream>
#include <sstream>
#include <vulkan/vulkan_to_string.hpp>
#endif


static char const * AppName    = "CreateDebugReportMessenger";
static char const * EngineName = "Vulkan.hpp";

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc( vk::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                 vk::DebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                 vk::DebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                 [[maybe_unused]] void * pUserData )
{
  std::ostringstream message;

  message << vk::to_string( messageSeverity ) << ": " << vk::to_string( messageTypes ) << ":\n";
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
      message << std::string( "\t\t\t" ) << "objectType   = " << vk::to_string( pCallbackData->pObjects[i].objectType ) << "\n";
      message << std::string( "\t\t\t" ) << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
      if ( pCallbackData->pObjects[i].pObjectName )
      {
        message << std::string( "\t\t\t" ) << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
      }
    }
  }

  std::cout << message.str() << std::endl;

  return false;
}

int main()
{
  try
  {
    vk::raii::Context context;

    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> props = context.enumerateInstanceExtensionProperties();

    auto propsIterator = std::find_if(
      props.begin(), props.end(), []( vk::ExtensionProperties const & ep ) { return strcmp( ep.extensionName, vk::EXTDebugUtilsExtensionName ) == 0; } );
    if ( propsIterator == props.end() )
    {
      std::cout << "Something went very wrong, cannot find " << vk::EXTDebugUtilsExtensionName << " extension" << std::endl;
      std::exit( 1 );
    }

    vk::ApplicationInfo    applicationInfo( AppName, 1, EngineName, 1, vk::ApiVersion11 );
    const char *           extensionName = vk::EXTDebugUtilsExtensionName;
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo, {}, extensionName );
    vk::raii::Instance     instance( context, instanceCreateInfo );

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
    vk::DebugUtilsMessageTypeFlagsEXT    messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT( {}, severityFlags, messageTypeFlags, &debugMessageFunc );
    vk::raii::DebugUtilsMessengerEXT     debugUtilsMessenger( instance, debugUtilsMessengerCreateInfoEXT );

    /* VULKAN_KEY_END */
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    std::exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    std::exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    std::exit( -1 );
  }
  return 0;
}
