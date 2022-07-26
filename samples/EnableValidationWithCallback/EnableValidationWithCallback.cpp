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
#include "vulkan/vulkan.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

static char const * AppName    = "EnableValidationWithCallback";
static char const * EngineName = "Vulkan.hpp";

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

#if defined( VULKAN_HPP_NO_TO_STRING )
namespace local
{
  std::string to_string( vk::DebugUtilsMessageSeverityFlagBitsEXT value )
  {
    switch ( value )
    {
      case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose: return "Verbose";
      case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo: return "Info";
      case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning: return "Warning";
      case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError: return "Error";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }

  std::string to_string( vk::DebugUtilsMessageTypeFlagsEXT value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral )
      result += "General | ";
    if ( value & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation )
      result += "Validation | ";
    if ( value & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance )
      result += "Performance | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  std::string to_string( vk::ObjectType value )
  {
    switch ( value )
    {
      case vk::ObjectType::eUnknown: return "Unknown";
      case vk::ObjectType::eInstance: return "Instance";
      case vk::ObjectType::ePhysicalDevice: return "PhysicalDevice";
      case vk::ObjectType::eDevice: return "Device";
      case vk::ObjectType::eQueue: return "Queue";
      case vk::ObjectType::eSemaphore: return "Semaphore";
      case vk::ObjectType::eCommandBuffer: return "CommandBuffer";
      case vk::ObjectType::eFence: return "Fence";
      case vk::ObjectType::eDeviceMemory: return "DeviceMemory";
      case vk::ObjectType::eBuffer: return "Buffer";
      case vk::ObjectType::eImage: return "Image";
      case vk::ObjectType::eEvent: return "Event";
      case vk::ObjectType::eQueryPool: return "QueryPool";
      case vk::ObjectType::eBufferView: return "BufferView";
      case vk::ObjectType::eImageView: return "ImageView";
      case vk::ObjectType::eShaderModule: return "ShaderModule";
      case vk::ObjectType::ePipelineCache: return "PipelineCache";
      case vk::ObjectType::ePipelineLayout: return "PipelineLayout";
      case vk::ObjectType::eRenderPass: return "RenderPass";
      case vk::ObjectType::ePipeline: return "Pipeline";
      case vk::ObjectType::eDescriptorSetLayout: return "DescriptorSetLayout";
      case vk::ObjectType::eSampler: return "Sampler";
      case vk::ObjectType::eDescriptorPool: return "DescriptorPool";
      case vk::ObjectType::eDescriptorSet: return "DescriptorSet";
      case vk::ObjectType::eFramebuffer: return "Framebuffer";
      case vk::ObjectType::eCommandPool: return "CommandPool";
      case vk::ObjectType::eSamplerYcbcrConversion: return "SamplerYcbcrConversion";
      case vk::ObjectType::eDescriptorUpdateTemplate: return "DescriptorUpdateTemplate";
      case vk::ObjectType::ePrivateDataSlot: return "PrivateDataSlot";
      case vk::ObjectType::eSurfaceKHR: return "SurfaceKHR";
      case vk::ObjectType::eSwapchainKHR: return "SwapchainKHR";
      case vk::ObjectType::eDisplayKHR: return "DisplayKHR";
      case vk::ObjectType::eDisplayModeKHR: return "DisplayModeKHR";
      case vk::ObjectType::eDebugReportCallbackEXT: return "DebugReportCallbackEXT";
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
      case vk::ObjectType::eVideoSessionKHR: return "VideoSessionKHR";
      case vk::ObjectType::eVideoSessionParametersKHR: return "VideoSessionParametersKHR";
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/
      case vk::ObjectType::eCuModuleNVX: return "CuModuleNVX";
      case vk::ObjectType::eCuFunctionNVX: return "CuFunctionNVX";
      case vk::ObjectType::eDebugUtilsMessengerEXT: return "DebugUtilsMessengerEXT";
      case vk::ObjectType::eAccelerationStructureKHR: return "AccelerationStructureKHR";
      case vk::ObjectType::eValidationCacheEXT: return "ValidationCacheEXT";
      case vk::ObjectType::eAccelerationStructureNV: return "AccelerationStructureNV";
      case vk::ObjectType::ePerformanceConfigurationINTEL: return "PerformanceConfigurationINTEL";
      case vk::ObjectType::eDeferredOperationKHR: return "DeferredOperationKHR";
      case vk::ObjectType::eIndirectCommandsLayoutNV: return "IndirectCommandsLayoutNV";
#  if defined( VK_USE_PLATFORM_FUCHSIA )
      case vk::ObjectType::eBufferCollectionFUCHSIA: return "BufferCollectionFUCHSIA";
#  endif /*VK_USE_PLATFORM_FUCHSIA*/
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }
}  // namespace local
using local::to_string;
#else
using vk::to_string;
#endif

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
  std::string message;

  message += to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": " +
             to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) + ":\n";
  message += std::string( "\t" ) + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
  message += std::string( "\t" ) + "messageIdNumber = " + std::to_string( pCallbackData->messageIdNumber ) + "\n";
  message += std::string( "\t" ) + "message         = <" + pCallbackData->pMessage + ">\n";
  if ( 0 < pCallbackData->queueLabelCount )
  {
    message += std::string( "\t" ) + "Queue Labels:\n";
    for ( uint32_t i = 0; i < pCallbackData->queueLabelCount; i++ )
    {
      message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
    }
  }
  if ( 0 < pCallbackData->cmdBufLabelCount )
  {
    message += std::string( "\t" ) + "CommandBuffer Labels:\n";
    for ( uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ )
    {
      message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
    }
  }
  if ( 0 < pCallbackData->objectCount )
  {
    for ( uint32_t i = 0; i < pCallbackData->objectCount; i++ )
    {
      message += std::string( "\t" ) + "Object " + std::to_string( i ) + "\n";
      message += std::string( "\t\t" ) + "objectType   = " + to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) + "\n";
      message += std::string( "\t\t" ) + "objectHandle = " + std::to_string( pCallbackData->pObjects[i].objectHandle ) + "\n";
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
  return std::all_of( layers.begin(),
                      layers.end(),
                      [&properties]( char const * name )
                      {
                        return std::find_if( properties.begin(),
                                             properties.end(),
                                             [&name]( vk::LayerProperties const & property )
                                             { return strcmp( property.layerName, name ) == 0; } ) != properties.end();
                      } );
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize the DipatchLoaderDynamic to use
    static vk::DynamicLoader  dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
#endif

    std::vector<vk::LayerProperties> instanceLayerProperties = vk::enumerateInstanceLayerProperties();

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
    vk::InstanceCreateInfo instanceCreateInfo( vk::InstanceCreateFlags(), &applicationInfo, instanceLayerNames, instanceExtensionNames );
    vk::Instance           instance = vk::createInstance( instanceCreateInfo );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );
#endif

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

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    // get the index of the first queue family that supports graphics
    uint32_t graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );

    float                     queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), graphicsQueueFamilyIndex, 1, &queuePriority );
    vk::Device                device = physicalDevice.createDevice( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) );

    // Create a CommandPool and don't destroy it, for testing purposes!
    vk::CommandPool commandPool = device.createCommandPool( vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlags(), graphicsQueueFamilyIndex ) );

#if true
    // The commandPool is not destroyed automatically
    // That is, the device is destroyed before the commmand pool and will trigger a validation error.
    std::cout << "*** INTENTIONALLY destroying the Device before destroying a CommandPool ***\n";
    std::cout << "*** The following error message is EXPECTED ***\n";
#else
    device.destroyCommandPool( commandPool );
#endif

    device.destroy();
#if !defined( NDEBUG )
    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
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
