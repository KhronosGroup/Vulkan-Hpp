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

#include "../utils/utils.hpp"

#include <iostream>
#include <sstream>

static char const * AppName    = "CreateDebugReportMessenger";
static char const * EngineName = "Vulkan.hpp";

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

  std::string to_string( vk::Result value )
  {
    switch ( value )
    {
      case vk::Result::eSuccess: return "Success";
      case vk::Result::eNotReady: return "NotReady";
      case vk::Result::eTimeout: return "Timeout";
      case vk::Result::eEventSet: return "EventSet";
      case vk::Result::eEventReset: return "EventReset";
      case vk::Result::eIncomplete: return "Incomplete";
      case vk::Result::eErrorOutOfHostMemory: return "ErrorOutOfHostMemory";
      case vk::Result::eErrorOutOfDeviceMemory: return "ErrorOutOfDeviceMemory";
      case vk::Result::eErrorInitializationFailed: return "ErrorInitializationFailed";
      case vk::Result::eErrorDeviceLost: return "ErrorDeviceLost";
      case vk::Result::eErrorMemoryMapFailed: return "ErrorMemoryMapFailed";
      case vk::Result::eErrorLayerNotPresent: return "ErrorLayerNotPresent";
      case vk::Result::eErrorExtensionNotPresent: return "ErrorExtensionNotPresent";
      case vk::Result::eErrorFeatureNotPresent: return "ErrorFeatureNotPresent";
      case vk::Result::eErrorIncompatibleDriver: return "ErrorIncompatibleDriver";
      case vk::Result::eErrorTooManyObjects: return "ErrorTooManyObjects";
      case vk::Result::eErrorFormatNotSupported: return "ErrorFormatNotSupported";
      case vk::Result::eErrorFragmentedPool: return "ErrorFragmentedPool";
      case vk::Result::eErrorUnknown: return "ErrorUnknown";
      case vk::Result::eErrorOutOfPoolMemory: return "ErrorOutOfPoolMemory";
      case vk::Result::eErrorInvalidExternalHandle: return "ErrorInvalidExternalHandle";
      case vk::Result::eErrorFragmentation: return "ErrorFragmentation";
      case vk::Result::eErrorInvalidOpaqueCaptureAddress: return "ErrorInvalidOpaqueCaptureAddress";
      case vk::Result::ePipelineCompileRequired: return "PipelineCompileRequired";
      case vk::Result::eErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
      case vk::Result::eErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
      case vk::Result::eSuboptimalKHR: return "SuboptimalKHR";
      case vk::Result::eErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
      case vk::Result::eErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
      case vk::Result::eErrorValidationFailedEXT: return "ErrorValidationFailedEXT";
      case vk::Result::eErrorInvalidShaderNV: return "ErrorInvalidShaderNV";
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
      case vk::Result::eErrorImageUsageNotSupportedKHR: return "ErrorImageUsageNotSupportedKHR";
      case vk::Result::eErrorVideoPictureLayoutNotSupportedKHR: return "ErrorVideoPictureLayoutNotSupportedKHR";
      case vk::Result::eErrorVideoProfileOperationNotSupportedKHR: return "ErrorVideoProfileOperationNotSupportedKHR";
      case vk::Result::eErrorVideoProfileFormatNotSupportedKHR: return "ErrorVideoProfileFormatNotSupportedKHR";
      case vk::Result::eErrorVideoProfileCodecNotSupportedKHR: return "ErrorVideoProfileCodecNotSupportedKHR";
      case vk::Result::eErrorVideoStdVersionNotSupportedKHR: return "ErrorVideoStdVersionNotSupportedKHR";
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/
      case vk::Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT: return "ErrorInvalidDrmFormatModifierPlaneLayoutEXT";
      case vk::Result::eErrorNotPermittedKHR: return "ErrorNotPermittedKHR";
#  if defined( VK_USE_PLATFORM_WIN32_KHR )
      case vk::Result::eErrorFullScreenExclusiveModeLostEXT: return "ErrorFullScreenExclusiveModeLostEXT";
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/
      case vk::Result::eThreadIdleKHR: return "ThreadIdleKHR";
      case vk::Result::eThreadDoneKHR: return "ThreadDoneKHR";
      case vk::Result::eOperationDeferredKHR: return "OperationDeferredKHR";
      case vk::Result::eOperationNotDeferredKHR: return "OperationNotDeferredKHR";
      case vk::Result::eErrorCompressionExhaustedEXT: return "ErrorCompressionExhaustedEXT";
      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
  }
}  // namespace local
using local::to_string;
#else
using vk::to_string;
#endif

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                 VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                 void * /*pUserData*/ )
{
  std::ostringstream message;

  message << to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) << ": "
          << to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
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
      message << std::string( "\t\t\t" ) << "objectType   = " << to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) << "\n";
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
    vk::raii::Context context;

    /* VULKAN_KEY_START */

    std::vector<vk::ExtensionProperties> props = context.enumerateInstanceExtensionProperties();

    auto propsIterator = std::find_if(
      props.begin(), props.end(), []( vk::ExtensionProperties const & ep ) { return strcmp( ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) == 0; } );
    if ( propsIterator == props.end() )
    {
      std::cout << "Something went very wrong, cannot find " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " extension" << std::endl;
      exit( 1 );
    }

    vk::ApplicationInfo    applicationInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    const char *           extensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
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
