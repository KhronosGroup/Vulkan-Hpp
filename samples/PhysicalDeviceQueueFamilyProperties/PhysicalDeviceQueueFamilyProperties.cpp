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
// VulkanHpp Samples : PhysicalDeviceQueueFamilyProperties
//                     Get queue family properties per physical device.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

static char const * AppName    = "PhysicalDeviceQueueFamilyProperties";
static char const * EngineName = "Vulkan.hpp";

#if defined( VULKAN_HPP_NO_TO_STRING )
namespace local
{
  std::string to_string( vk::PipelineStageFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::PipelineStageFlagBits::eTopOfPipe )
      result += "TopOfPipe | ";
    if ( value & vk::PipelineStageFlagBits::eDrawIndirect )
      result += "DrawIndirect | ";
    if ( value & vk::PipelineStageFlagBits::eVertexInput )
      result += "VertexInput | ";
    if ( value & vk::PipelineStageFlagBits::eVertexShader )
      result += "VertexShader | ";
    if ( value & vk::PipelineStageFlagBits::eTessellationControlShader )
      result += "TessellationControlShader | ";
    if ( value & vk::PipelineStageFlagBits::eTessellationEvaluationShader )
      result += "TessellationEvaluationShader | ";
    if ( value & vk::PipelineStageFlagBits::eGeometryShader )
      result += "GeometryShader | ";
    if ( value & vk::PipelineStageFlagBits::eFragmentShader )
      result += "FragmentShader | ";
    if ( value & vk::PipelineStageFlagBits::eEarlyFragmentTests )
      result += "EarlyFragmentTests | ";
    if ( value & vk::PipelineStageFlagBits::eLateFragmentTests )
      result += "LateFragmentTests | ";
    if ( value & vk::PipelineStageFlagBits::eColorAttachmentOutput )
      result += "ColorAttachmentOutput | ";
    if ( value & vk::PipelineStageFlagBits::eComputeShader )
      result += "ComputeShader | ";
    if ( value & vk::PipelineStageFlagBits::eTransfer )
      result += "Transfer | ";
    if ( value & vk::PipelineStageFlagBits::eBottomOfPipe )
      result += "BottomOfPipe | ";
    if ( value & vk::PipelineStageFlagBits::eHost )
      result += "Host | ";
    if ( value & vk::PipelineStageFlagBits::eAllGraphics )
      result += "AllGraphics | ";
    if ( value & vk::PipelineStageFlagBits::eAllCommands )
      result += "AllCommands | ";
    if ( value & vk::PipelineStageFlagBits::eTransformFeedbackEXT )
      result += "TransformFeedbackEXT | ";
    if ( value & vk::PipelineStageFlagBits::eConditionalRenderingEXT )
      result += "ConditionalRenderingEXT | ";
    if ( value & vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR )
      result += "AccelerationStructureBuildKHR | ";
    if ( value & vk::PipelineStageFlagBits::eRayTracingShaderKHR )
      result += "RayTracingShaderKHR | ";
    if ( value & vk::PipelineStageFlagBits::eTaskShaderNV )
      result += "TaskShaderNV | ";
    if ( value & vk::PipelineStageFlagBits::eMeshShaderNV )
      result += "MeshShaderNV | ";
    if ( value & vk::PipelineStageFlagBits::eFragmentDensityProcessEXT )
      result += "FragmentDensityProcessEXT | ";
    if ( value & vk::PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR )
      result += "FragmentShadingRateAttachmentKHR | ";
    if ( value & vk::PipelineStageFlagBits::eCommandPreprocessNV )
      result += "CommandPreprocessNV | ";

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }

  std::string to_string( vk::QueueFlags value )
  {
    if ( !value )
      return "{}";

    std::string result;
    if ( value & vk::QueueFlagBits::eGraphics )
      result += "Graphics | ";
    if ( value & vk::QueueFlagBits::eCompute )
      result += "Compute | ";
    if ( value & vk::QueueFlagBits::eTransfer )
      result += "Transfer | ";
    if ( value & vk::QueueFlagBits::eSparseBinding )
      result += "SparseBinding | ";
    if ( value & vk::QueueFlagBits::eProtected )
      result += "Protected | ";
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
    if ( value & vk::QueueFlagBits::eVideoDecodeKHR )
      result += "VideoDecodeKHR | ";
    if ( value & vk::QueueFlagBits::eVideoEncodeKHR )
      result += "VideoEncodeKHR | ";
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }
}  // namespace local
using local::to_string;
#else
using vk::to_string;
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, {}, VK_API_VERSION_1_1 );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      // some features are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";

      // need to explicitly specify all the template arguments for getQueueFamilyProperties2 to make the compiler happy
      using Chain                 = vk::StructureChain<vk::QueueFamilyProperties2, vk::QueueFamilyCheckpointPropertiesNV>;
      auto queueFamilyProperties2 = physicalDevices[i].getQueueFamilyProperties2<Chain, std::allocator<Chain>, vk::DispatchLoaderDynamic>();
      for ( size_t j = 0; j < queueFamilyProperties2.size(); j++ )
      {
        std::cout << std::string( "\t" ) << "QueueFamily " << j << "\n";
        vk::QueueFamilyProperties const & properties = queueFamilyProperties2[j].get<vk::QueueFamilyProperties2>().queueFamilyProperties;
        std::cout << std::string( "\t\t" ) << "QueueFamilyProperties:\n";
        std::cout << std::string( "\t\t\t" ) << "queueFlags                  = " << to_string( properties.queueFlags ) << "\n";
        std::cout << std::string( "\t\t\t" ) << "queueCount                  = " << properties.queueCount << "\n";
        std::cout << std::string( "\t\t\t" ) << "timestampValidBits          = " << properties.timestampValidBits << "\n";
        std::cout << std::string( "\t\t\t" ) << "minImageTransferGranularity = " << properties.minImageTransferGranularity.width << " x "
                  << properties.minImageTransferGranularity.height << " x " << properties.minImageTransferGranularity.depth << "\n";
        std::cout << "\n";

        if ( vk::su::contains( extensionProperties, "VK_NV_device_diagnostic_checkpoints" ) )
        {
          vk::QueueFamilyCheckpointPropertiesNV const & checkpointProperties = queueFamilyProperties2[j].get<vk::QueueFamilyCheckpointPropertiesNV>();
          std::cout << std::string( "\t\t" ) << "CheckPointPropertiesNV:\n";
          std::cout << std::string( "\t\t\t" ) << "checkpointExecutionStageMask  = " << to_string( checkpointProperties.checkpointExecutionStageMask ) << "\n";
          std::cout << "\n";
        }
      }
    }

    /* VULKAN_KEY_END */

#if !defined( NDEBUG )
    instance.destroyDebugUtilsMessengerEXT( debugUtilsMessenger );
#endif
    instance.destroy();
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
