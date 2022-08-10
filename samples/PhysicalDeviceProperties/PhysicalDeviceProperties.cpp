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
// VulkanHpp Samples : PhysicalDeviceProperties
//                     Get properties per physical device.

#include "../utils/utils.hpp"

#include <iomanip>
#include <sstream>
#include <vector>
#include <vulkan/vulkan_to_string.hpp>

static char const * AppName    = "PhysicalDeviceProperties";
static char const * EngineName = "Vulkan.hpp";

std::string decodeAPIVersion( uint32_t apiVersion )
{
  return std::to_string( VK_VERSION_MAJOR( apiVersion ) ) + "." + std::to_string( VK_VERSION_MINOR( apiVersion ) ) + "." +
         std::to_string( VK_VERSION_PATCH( apiVersion ) );
}

std::string decodeDriverVersion( uint32_t driverVersion, uint32_t vendorID )
{
  switch ( vendorID )
  {
    case 4318:
      return std::to_string( ( driverVersion >> 22 ) & 0x3FF ) + "." + std::to_string( ( driverVersion >> 14 ) & 0xFF ) + "." +
             std::to_string( ( driverVersion >> 6 ) & 0xFF ) + "." + std::to_string( driverVersion & 0x3F );
    case 0x8086: return std::to_string( ( driverVersion >> 14 ) & 0x3FFFF ) + "." + std::to_string( ( driverVersion & 0x3FFF ) );
    default: return decodeAPIVersion( driverVersion );
  }
}

std::string decodeVendorID( uint32_t vendorID )
{
  // below 0x10000 are the PCI vendor IDs (https://pcisig.com/membership/member-companies)
  if ( vendorID < 0x10000 )
  {
    switch ( vendorID )
    {
      case 0x1022: return "Advanced Micro Devices";
      case 0x10DE: return "NVidia Corporation";
      case 0x8086: return "Intel Corporation";
      default: return std::to_string( vendorID );
    }
  }
  else
  {
    // above 0x10000 should be vkVendorIDs
    return vk::to_string( vk::VendorId( vendorID ) );
  }
}

namespace vk
{
  namespace su
  {
    struct LUID
    {
    public:
      LUID( uint8_t const data[VK_LUID_SIZE] )
      {
        memcpy( m_data, data, VK_LUID_SIZE * sizeof( uint8_t ) );
      }

      uint8_t m_data[VK_LUID_SIZE];
    };

    std::ostream & operator<<( std::ostream & os, LUID const & uuid )
    {
      os << std::setfill( '0' ) << std::hex;
      for ( uint32_t j = 0; j < VK_LUID_SIZE; ++j )
      {
        os << std::setw( 2 ) << static_cast<uint32_t>( uuid.m_data[j] );
        if ( j == 3 || j == 5 )
        {
          std::cout << '-';
        }
      }
      os << std::setfill( ' ' ) << std::dec;
      return os;
    }
  }  // namespace su
}  // namespace vk

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
      // some properties are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";
      auto properties2 = physicalDevices[i]
                           .getProperties2<vk::PhysicalDeviceProperties2,
                                           vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT,
                                           vk::PhysicalDeviceConservativeRasterizationPropertiesEXT,
                                           vk::PhysicalDeviceCooperativeMatrixPropertiesNV,
                                           vk::PhysicalDeviceDepthStencilResolvePropertiesKHR,
                                           vk::PhysicalDeviceDescriptorIndexingPropertiesEXT,
                                           vk::PhysicalDeviceDiscardRectanglePropertiesEXT,
                                           vk::PhysicalDeviceDriverPropertiesKHR,
                                           vk::PhysicalDeviceExternalMemoryHostPropertiesEXT,
                                           vk::PhysicalDeviceFloatControlsPropertiesKHR,
                                           vk::PhysicalDeviceFragmentDensityMapPropertiesEXT,
                                           vk::PhysicalDeviceIDProperties,
                                           vk::PhysicalDeviceInlineUniformBlockPropertiesEXT,
                                           vk::PhysicalDeviceLineRasterizationPropertiesEXT,
                                           vk::PhysicalDeviceMaintenance3Properties,
                                           vk::PhysicalDeviceMeshShaderPropertiesNV,
                                           vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX,
                                           vk::PhysicalDeviceMultiviewProperties,
                                           vk::PhysicalDevicePCIBusInfoPropertiesEXT,
                                           vk::PhysicalDevicePointClippingProperties,
                                           vk::PhysicalDeviceProtectedMemoryProperties,
                                           vk::PhysicalDevicePushDescriptorPropertiesKHR,
                                           vk::PhysicalDeviceRayTracingPropertiesNV,
                                           vk::PhysicalDeviceSampleLocationsPropertiesEXT,
                                           vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT,
                                           vk::PhysicalDeviceShaderCoreProperties2AMD,
                                           vk::PhysicalDeviceShaderCorePropertiesAMD,
                                           vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV,
                                           vk::PhysicalDeviceShadingRateImagePropertiesNV,
                                           vk::PhysicalDeviceSubgroupProperties,
                                           vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT,
                                           vk::PhysicalDeviceTimelineSemaphorePropertiesKHR,
                                           vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT,
                                           vk::PhysicalDeviceTransformFeedbackPropertiesEXT,
                                           vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
      vk::PhysicalDeviceProperties const & properties = properties2.get<vk::PhysicalDeviceProperties2>().properties;
      std::cout << std::string( "\t" ) << "Properties:\n";
      std::cout << std::string( "\t\t" ) << "apiVersion        = " << decodeAPIVersion( properties.apiVersion ) << "\n";
      std::cout << std::string( "\t\t" ) << "driverVersion     = " << decodeDriverVersion( properties.driverVersion, properties.vendorID ) << "\n";
      std::cout << std::string( "\t\t" ) << "vendorID          = " << decodeVendorID( properties.vendorID ) << "\n";
      std::cout << std::string( "\t\t" ) << "deviceID          = " << properties.deviceID << "\n";
      std::cout << std::string( "\t\t" ) << "deviceType        = " << vk::to_string( properties.deviceType ) << "\n";
      std::cout << std::string( "\t\t" ) << "deviceName        = " << properties.deviceName << "\n";
      std::cout << std::string( "\t\t" ) << "pipelineCacheUUID = " << vk::su::UUID( properties.pipelineCacheUUID ) << "\n";
      std::cout << std::string( "\t\t" ) << "limits:\n";
      std::cout << std::string( "\t\t\t" ) << "bufferImageGranularity                          = " << properties.limits.bufferImageGranularity << "\n";
      std::cout << std::string( "\t\t\t" ) << "discreteQueuePriorities                         = " << properties.limits.discreteQueuePriorities << "\n";
      std::cout << std::string( "\t\t\t" )
                << "framebufferColorSampleCounts                    = " << vk::to_string( properties.limits.framebufferColorSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "framebufferDepthSampleCounts                    = " << vk::to_string( properties.limits.framebufferDepthSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "framebufferNoAttachmentsSampleCounts            = " << vk::to_string( properties.limits.framebufferNoAttachmentsSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "framebufferStencilSampleCounts                  = " << vk::to_string( properties.limits.framebufferStencilSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" ) << "lineWidthGranularity                            = " << properties.limits.lineWidthGranularity << "\n";
      std::cout << std::string( "\t\t\t" ) << "lineWidthRange                                  = "
                << "[" << properties.limits.lineWidthRange[0] << ", " << properties.limits.lineWidthRange[1] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxBoundDescriptorSets                          = " << properties.limits.maxBoundDescriptorSets << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxClipDistances                                = " << properties.limits.maxClipDistances << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxColorAttachments                             = " << properties.limits.maxColorAttachments << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxCombinedClipAndCullDistances                 = " << properties.limits.maxCombinedClipAndCullDistances << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxComputeSharedMemorySize                      = " << properties.limits.maxComputeSharedMemorySize << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupCount                        = "
                << "[" << properties.limits.maxComputeWorkGroupCount[0] << ", " << properties.limits.maxComputeWorkGroupCount[1] << ", "
                << properties.limits.maxComputeWorkGroupCount[2] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupInvocations                  = " << properties.limits.maxComputeWorkGroupInvocations << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupSize                         = "
                << "[" << properties.limits.maxComputeWorkGroupSize[0] << ", " << properties.limits.maxComputeWorkGroupSize[1] << ", "
                << properties.limits.maxComputeWorkGroupSize[2] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxCullDistances                                = " << properties.limits.maxCullDistances << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetInputAttachments                = " << properties.limits.maxDescriptorSetInputAttachments
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetSampledImages                   = " << properties.limits.maxDescriptorSetSampledImages << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetSamplers                        = " << properties.limits.maxDescriptorSetSamplers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageBuffers                  = " << properties.limits.maxDescriptorSetStorageBuffers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageBuffersDynamic           = " << properties.limits.maxDescriptorSetStorageBuffersDynamic
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageImages                   = " << properties.limits.maxDescriptorSetStorageImages << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetUniformBuffers                  = " << properties.limits.maxDescriptorSetUniformBuffers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetUniformBuffersDynamic           = " << properties.limits.maxDescriptorSetUniformBuffersDynamic
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDrawIndexedIndexValue                        = " << properties.limits.maxDrawIndexedIndexValue << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxDrawIndirectCount                            = " << properties.limits.maxDrawIndirectCount << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFragmentCombinedOutputResources              = " << properties.limits.maxFragmentCombinedOutputResources
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFragmentDualSrcAttachments                   = " << properties.limits.maxFragmentDualSrcAttachments << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFragmentInputComponents                      = " << properties.limits.maxFragmentInputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFragmentOutputAttachments                    = " << properties.limits.maxFragmentOutputAttachments << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFramebufferHeight                            = " << properties.limits.maxFramebufferHeight << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFramebufferLayers                            = " << properties.limits.maxFramebufferLayers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxFramebufferWidth                             = " << properties.limits.maxFramebufferWidth << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxGeometryInputComponents                      = " << properties.limits.maxGeometryInputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxGeometryOutputComponents                     = " << properties.limits.maxGeometryOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxGeometryOutputVertices                       = " << properties.limits.maxGeometryOutputVertices << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxGeometryShaderInvocations                    = " << properties.limits.maxGeometryShaderInvocations << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxGeometryTotalOutputComponents                = " << properties.limits.maxGeometryTotalOutputComponents
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxImageArrayLayers                             = " << properties.limits.maxImageArrayLayers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxImageDimension1D                             = " << properties.limits.maxImageDimension1D << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxImageDimension2D                             = " << properties.limits.maxImageDimension2D << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxImageDimension3D                             = " << properties.limits.maxImageDimension3D << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxImageDimensionCube                           = " << properties.limits.maxImageDimensionCube << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxInterpolationOffset                          = " << properties.limits.maxInterpolationOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxMemoryAllocationCount                        = " << properties.limits.maxMemoryAllocationCount << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorInputAttachments           = " << properties.limits.maxPerStageDescriptorInputAttachments
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorSampledImages              = " << properties.limits.maxPerStageDescriptorSampledImages
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorSamplers                   = " << properties.limits.maxPerStageDescriptorSamplers << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorStorageBuffers             = " << properties.limits.maxPerStageDescriptorStorageBuffers
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorStorageImages              = " << properties.limits.maxPerStageDescriptorStorageImages
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageDescriptorUniformBuffers             = " << properties.limits.maxPerStageDescriptorUniformBuffers
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPerStageResources                            = " << properties.limits.maxPerStageResources << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxPushConstantsSize                            = " << properties.limits.maxPushConstantsSize << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxSampleMaskWords                              = " << properties.limits.maxSampleMaskWords << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxSamplerAllocationCount                       = " << properties.limits.maxSamplerAllocationCount << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxSamplerAnisotropy                            = " << properties.limits.maxSamplerAnisotropy << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxSamplerLodBias                               = " << properties.limits.maxSamplerLodBias << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxStorageBufferRange                           = " << properties.limits.maxStorageBufferRange << "\n";
      std::cout << std::string( "\t\t\t" )
                << "maxTessellationControlPerPatchOutputComponents  = " << properties.limits.maxTessellationControlPerPatchOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" )
                << "maxTessellationControlPerVertexInputComponents  = " << properties.limits.maxTessellationControlPerVertexInputComponents << "\n";
      std::cout << std::string( "\t\t\t" )
                << "maxTessellationControlPerVertexOutputComponents = " << properties.limits.maxTessellationControlPerVertexOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" )
                << "maxTessellationControlTotalOutputComponents     = " << properties.limits.maxTessellationControlTotalOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTessellationEvaluationInputComponents        = " << properties.limits.maxTessellationEvaluationInputComponents
                << "\n";
      std::cout << std::string( "\t\t\t" )
                << "maxTessellationEvaluationOutputComponents       = " << properties.limits.maxTessellationEvaluationOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTessellationGenerationLevel                  = " << properties.limits.maxTessellationGenerationLevel << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTessellationPatchSize                        = " << properties.limits.maxTessellationPatchSize << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTexelBufferElements                          = " << properties.limits.maxTexelBufferElements << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTexelGatherOffset                            = " << properties.limits.maxTexelGatherOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxTexelOffset                                  = " << properties.limits.maxTexelOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxUniformBufferRange                           = " << properties.limits.maxUniformBufferRange << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxVertexInputAttributeOffset                   = " << properties.limits.maxVertexInputAttributeOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxVertexInputAttributes                        = " << properties.limits.maxVertexInputAttributes << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxVertexInputBindings                          = " << properties.limits.maxVertexInputBindings << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxVertexInputBindingStride                     = " << properties.limits.maxVertexInputBindingStride << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxVertexOutputComponents                       = " << properties.limits.maxVertexOutputComponents << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxViewportDimensions                           = "
                << "[" << properties.limits.maxViewportDimensions[0] << ", " << properties.limits.maxViewportDimensions[1] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "maxViewports                                    = " << properties.limits.maxViewports << "\n";
      std::cout << std::string( "\t\t\t" ) << "minInterpolationOffset                          = " << properties.limits.minInterpolationOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "minMemoryMapAlignment                           = " << properties.limits.minMemoryMapAlignment << "\n";
      std::cout << std::string( "\t\t\t" ) << "minStorageBufferOffsetAlignment                 = " << properties.limits.minStorageBufferOffsetAlignment << "\n";
      std::cout << std::string( "\t\t\t" ) << "minTexelBufferOffsetAlignment                   = " << properties.limits.minTexelBufferOffsetAlignment << "\n";
      std::cout << std::string( "\t\t\t" ) << "minTexelGatherOffset                            = " << properties.limits.minTexelGatherOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "minTexelOffset                                  = " << properties.limits.minTexelOffset << "\n";
      std::cout << std::string( "\t\t\t" ) << "minUniformBufferOffsetAlignment                 = " << properties.limits.minUniformBufferOffsetAlignment << "\n";
      std::cout << std::string( "\t\t\t" ) << "mipmapPrecisionBits                             = " << properties.limits.mipmapPrecisionBits << "\n";
      std::cout << std::string( "\t\t\t" ) << "nonCoherentAtomSize                             = " << properties.limits.nonCoherentAtomSize << "\n";
      std::cout << std::string( "\t\t\t" ) << "optimalBufferCopyOffsetAlignment                = " << properties.limits.optimalBufferCopyOffsetAlignment
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "optimalBufferCopyRowPitchAlignment              = " << properties.limits.optimalBufferCopyRowPitchAlignment
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "pointSizeGranularity                            = " << properties.limits.pointSizeGranularity << "\n";
      std::cout << std::string( "\t\t\t" ) << "pointSizeRange                                  = "
                << "[" << properties.limits.pointSizeRange[0] << ", " << properties.limits.pointSizeRange[1] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" )
                << "sampledImageColorSampleCounts                   = " << vk::to_string( properties.limits.sampledImageColorSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "sampledImageDepthSampleCounts                   = " << vk::to_string( properties.limits.sampledImageDepthSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "sampledImageIntegerSampleCounts                 = " << vk::to_string( properties.limits.sampledImageIntegerSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" )
                << "sampledImageStencilSampleCounts                 = " << vk::to_string( properties.limits.sampledImageStencilSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" ) << "sparseAddressSpaceSize                          = " << properties.limits.sparseAddressSpaceSize << "\n";
      std::cout << std::string( "\t\t\t" ) << "standardSampleLocations                         = " << !!properties.limits.standardSampleLocations << "\n";
      std::cout << std::string( "\t\t\t" )
                << "storageImageSampleCounts                        = " << vk::to_string( properties.limits.storageImageSampleCounts ) << "\n";
      std::cout << std::string( "\t\t\t" ) << "strictLines                                     = " << !!properties.limits.strictLines << "\n";
      std::cout << std::string( "\t\t\t" ) << "subPixelInterpolationOffsetBits                 = " << properties.limits.subPixelInterpolationOffsetBits << "\n";
      std::cout << std::string( "\t\t\t" ) << "subPixelPrecisionBits                           = " << properties.limits.subPixelPrecisionBits << "\n";
      std::cout << std::string( "\t\t\t" ) << "subTexelPrecisionBits                           = " << properties.limits.subTexelPrecisionBits << "\n";
      std::cout << std::string( "\t\t\t" ) << "timestampComputeAndGraphics                     = " << !!properties.limits.timestampComputeAndGraphics << "\n";
      std::cout << std::string( "\t\t\t" ) << "timestampPeriod                                 = " << properties.limits.timestampPeriod << "\n";
      std::cout << std::string( "\t\t\t" ) << "viewportBoundsRange                             = "
                << "[" << properties.limits.viewportBoundsRange[0] << ", " << properties.limits.viewportBoundsRange[1] << "]"
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "viewportSubPixelBits                            = " << properties.limits.viewportSubPixelBits << "\n";
      std::cout << std::string( "\t\t" ) << "sparseProperties:\n";
      std::cout << std::string( "\t\t\t" ) << "residencyAlignedMipSize                   = " << !!properties.sparseProperties.residencyAlignedMipSize << "\n";
      std::cout << std::string( "\t\t\t" ) << "residencyNonResidentStrict                = " << !!properties.sparseProperties.residencyNonResidentStrict
                << "\n";
      std::cout << std::string( "\t\t\t" ) << "residencyStandard2DBlockShape             = " << !!properties.sparseProperties.residencyStandard2DBlockShape
                << "\n";
      std::cout << std::string( "\t\t\t" )
                << "residencyStandard2DMultisampleBlockShape  = " << !!properties.sparseProperties.residencyStandard2DMultisampleBlockShape << "\n";
      std::cout << std::string( "\t\t\t" ) << "residencyStandard3DBlockShape             = " << !!properties.sparseProperties.residencyStandard3DBlockShape
                << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_blend_operation_advanced" ) )
      {
        vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT const & blendOperationAdvancedProperties =
          properties2.get<vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>();
        std::cout << std::string( "\t" ) << "BlendOperationAdvancedProperties:\n";
        std::cout << std::string( "\t\t" ) << "advancedBlendAllOperations            = " << !!blendOperationAdvancedProperties.advancedBlendAllOperations
                  << "\n";
        std::cout << std::string( "\t\t" ) << "advancedBlendCorrelatedOverlap        = " << !!blendOperationAdvancedProperties.advancedBlendCorrelatedOverlap
                  << "\n";
        std::cout << std::string( "\t\t" ) << "advancedBlendIndependentBlend         = " << !!blendOperationAdvancedProperties.advancedBlendIndependentBlend
                  << "\n";
        std::cout << std::string( "\t\t" ) << "advancedBlendMaxColorAttachments      = " << blendOperationAdvancedProperties.advancedBlendMaxColorAttachments
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "advancedBlendNonPremultipliedDstColor = " << !!blendOperationAdvancedProperties.advancedBlendNonPremultipliedDstColor << "\n";
        std::cout << std::string( "\t\t" )
                  << "advancedBlendNonPremultipliedSrcColor = " << !!blendOperationAdvancedProperties.advancedBlendNonPremultipliedSrcColor << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_conservative_rasterization" ) )
      {
        vk::PhysicalDeviceConservativeRasterizationPropertiesEXT const & conservativeRasterizationProperties =
          properties2.get<vk::PhysicalDeviceConservativeRasterizationPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ConservativeRasterizationProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "conservativePointAndLineRasterization       = " << !!conservativeRasterizationProperties.conservativePointAndLineRasterization << "\n";
        std::cout << std::string( "\t\t" )
                  << "conservativeRasterizationPostDepthCoverage  = " << !!conservativeRasterizationProperties.conservativeRasterizationPostDepthCoverage
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "degenerateLinesRasterized                   = " << !!conservativeRasterizationProperties.degenerateLinesRasterized << "\n";
        std::cout << std::string( "\t\t" )
                  << "degenerateTrianglesRasterized               = " << !!conservativeRasterizationProperties.degenerateTrianglesRasterized << "\n";
        std::cout << std::string( "\t\t" )
                  << "extraPrimitiveOverestimationSizeGranularity = " << conservativeRasterizationProperties.extraPrimitiveOverestimationSizeGranularity
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "fullyCoveredFragmentShaderInputVariable     = " << !!conservativeRasterizationProperties.fullyCoveredFragmentShaderInputVariable << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxExtraPrimitiveOverestimationSize         = " << conservativeRasterizationProperties.maxExtraPrimitiveOverestimationSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "primitiveOverestimationSize                 = " << conservativeRasterizationProperties.primitiveOverestimationSize << "\n";
        std::cout << std::string( "\t\t" ) << "primitiveUnderestimation                    = " << !!conservativeRasterizationProperties.primitiveUnderestimation
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_cooperative_matrix" ) )
      {
        vk::PhysicalDeviceCooperativeMatrixPropertiesNV const & cooperativeMatrixProperties =
          properties2.get<vk::PhysicalDeviceCooperativeMatrixPropertiesNV>();
        std::cout << std::string( "\t" ) << "CooperativeMatrixProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "cooperativeMatrixSupportedStages  = " << vk::to_string( cooperativeMatrixProperties.cooperativeMatrixSupportedStages ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_depth_stencil_resolve" ) )
      {
        vk::PhysicalDeviceDepthStencilResolvePropertiesKHR const & depthStencilResolveProperties =
          properties2.get<vk::PhysicalDeviceDepthStencilResolvePropertiesKHR>();
        std::cout << std::string( "\t" ) << "DepthStencilResolveProperties:\n";
        std::cout << std::string( "\t\t" ) << "independentResolve            = " << !!depthStencilResolveProperties.independentResolve << "\n";
        std::cout << std::string( "\t\t" ) << "independentResolveNone        = " << !!depthStencilResolveProperties.independentResolveNone << "\n";
        std::cout << std::string( "\t\t" ) << "supportedDepthResolveModes    = " << vk::to_string( depthStencilResolveProperties.supportedDepthResolveModes )
                  << "\n";
        std::cout << std::string( "\t\t" ) << "supportedStencilResolveModes  = " << vk::to_string( depthStencilResolveProperties.supportedStencilResolveModes )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_descriptor_indexing" ) )
      {
        vk::PhysicalDeviceDescriptorIndexingPropertiesEXT const & descriptorIndexingProperties =
          properties2.get<vk::PhysicalDeviceDescriptorIndexingPropertiesEXT>();
        std::cout << std::string( "\t" ) << "DescriptorIndexingProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindInputAttachments       = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindInputAttachments
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSampledImages          = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSampledImages
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSamplers               = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSamplers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageBuffers         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic  = "
                  << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageImages          = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageImages
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindUniformBuffers         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic  = "
                  << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindInputAttachments  = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindInputAttachments << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindSampledImages     = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSampledImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindSamplers          = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSamplers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindStorageBuffers    = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageBuffers << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindStorageImages     = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageImages << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindUniformBuffers    = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindUniformBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageUpdateAfterBindResources                   = " << descriptorIndexingProperties.maxPerStageUpdateAfterBindResources << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxUpdateAfterBindDescriptorsInAllPools               = " << descriptorIndexingProperties.maxUpdateAfterBindDescriptorsInAllPools << "\n";
        std::cout << std::string( "\t\t" )
                  << "quadDivergentImplicitLod                              = " << !!descriptorIndexingProperties.quadDivergentImplicitLod << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustBufferAccessUpdateAfterBind                     = " << !!descriptorIndexingProperties.robustBufferAccessUpdateAfterBind << "\n";
        std::cout << std::string( "\t\t" ) << "shaderInputAttachmentArrayNonUniformIndexingNative    = "
                  << !!descriptorIndexingProperties.shaderInputAttachmentArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderSampledImageArrayNonUniformIndexingNative       = "
                  << !!descriptorIndexingProperties.shaderSampledImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderStorageBufferArrayNonUniformIndexingNative      = "
                  << !!descriptorIndexingProperties.shaderStorageBufferArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderStorageImageArrayNonUniformIndexingNative       = "
                  << !!descriptorIndexingProperties.shaderStorageImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderUniformBufferArrayNonUniformIndexingNative      = "
                  << !!descriptorIndexingProperties.shaderUniformBufferArrayNonUniformIndexingNative << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_discard_rectangles" ) )
      {
        vk::PhysicalDeviceDiscardRectanglePropertiesEXT const & discardRectangleProperties = properties2.get<vk::PhysicalDeviceDiscardRectanglePropertiesEXT>();
        std::cout << std::string( "\t" ) << "DiscardRectangleProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxDiscardRectangles  = " << discardRectangleProperties.maxDiscardRectangles << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_driver_properties" ) )
      {
        vk::PhysicalDeviceDriverPropertiesKHR const & driverProperties = properties2.get<vk::PhysicalDeviceDriverPropertiesKHR>();
        std::cout << std::string( "\t" ) << "DriverProperties:\n";
        std::cout << std::string( "\t\t" ) << "driverID            = " << vk::to_string( driverProperties.driverID ) << "\n";
        std::cout << std::string( "\t\t" ) << "driverName          = " << driverProperties.driverName << "\n";
        std::cout << std::string( "\t\t" ) << "driverInfo          = " << driverProperties.driverInfo << "\n";
        std::cout << std::string( "\t\t" ) << "conformanceVersion  = " << static_cast<uint32_t>( driverProperties.conformanceVersion.major ) << "."
                  << static_cast<uint32_t>( driverProperties.conformanceVersion.minor ) << "."
                  << static_cast<uint32_t>( driverProperties.conformanceVersion.subminor ) << "."
                  << static_cast<uint32_t>( driverProperties.conformanceVersion.patch ) << std::dec << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_external_memory_host" ) )
      {
        vk::PhysicalDeviceExternalMemoryHostPropertiesEXT const & externalMemoryHostProperties =
          properties2.get<vk::PhysicalDeviceExternalMemoryHostPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ExternalMemoryHostProperties:\n";
        std::cout << std::string( "\t\t" ) << "minImportedHostPointerAlignment = " << externalMemoryHostProperties.minImportedHostPointerAlignment << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_float_controls" ) )
      {
        vk::PhysicalDeviceFloatControlsPropertiesKHR const & floatControlsProperties = properties2.get<vk::PhysicalDeviceFloatControlsPropertiesKHR>();
        std::cout << std::string( "\t" ) << "FloatControlsProperties:\n";
        std::cout << std::string( "\t\t" ) << "denormBehaviorIndependence            = " << vk::to_string( floatControlsProperties.denormBehaviorIndependence )
                  << "\n";
        std::cout << std::string( "\t\t" ) << "roundingModeIndependence              = " << vk::to_string( floatControlsProperties.roundingModeIndependence )
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat16        = " << !!floatControlsProperties.shaderDenormFlushToZeroFloat16 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat32        = " << !!floatControlsProperties.shaderDenormFlushToZeroFloat32 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat64        = " << !!floatControlsProperties.shaderDenormFlushToZeroFloat64 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat16           = " << !!floatControlsProperties.shaderDenormPreserveFloat16 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat32           = " << !!floatControlsProperties.shaderDenormPreserveFloat32 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat64           = " << !!floatControlsProperties.shaderDenormPreserveFloat64 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat16          = " << !!floatControlsProperties.shaderRoundingModeRTEFloat16 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat32          = " << !!floatControlsProperties.shaderRoundingModeRTEFloat32 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat64          = " << !!floatControlsProperties.shaderRoundingModeRTEFloat64 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat16          = " << !!floatControlsProperties.shaderRoundingModeRTZFloat16 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat32          = " << !!floatControlsProperties.shaderRoundingModeRTZFloat32 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat64          = " << !!floatControlsProperties.shaderRoundingModeRTZFloat64 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderSignedZeroInfNanPreserveFloat16 = " << !!floatControlsProperties.shaderSignedZeroInfNanPreserveFloat16
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderSignedZeroInfNanPreserveFloat32 = " << !!floatControlsProperties.shaderSignedZeroInfNanPreserveFloat32
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderSignedZeroInfNanPreserveFloat64 = " << !!floatControlsProperties.shaderSignedZeroInfNanPreserveFloat64
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_density_map" ) )
      {
        vk::PhysicalDeviceFragmentDensityMapPropertiesEXT const & fragmentDensityMapProperties =
          properties2.get<vk::PhysicalDeviceFragmentDensityMapPropertiesEXT>();
        std::cout << std::string( "\t" ) << "FragmentDensityProperties:\n";
        std::cout << std::string( "\t\t" ) << "fragmentDensityInvocations  = " << !!fragmentDensityMapProperties.fragmentDensityInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxFragmentDensityTexelSize = " << fragmentDensityMapProperties.maxFragmentDensityTexelSize.width << " x "
                  << fragmentDensityMapProperties.maxFragmentDensityTexelSize.height << "\n";
        std::cout << std::string( "\t\t" ) << "minFragmentDensityTexelSize = " << fragmentDensityMapProperties.minFragmentDensityTexelSize.width << " x "
                  << fragmentDensityMapProperties.minFragmentDensityTexelSize.height << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceIDProperties const & idProperties = properties2.get<vk::PhysicalDeviceIDProperties>();
      std::cout << std::string( "\t" ) << "IDProperties:\n";
      std::cout << std::string( "\t\t" ) << "deviceUUID      = " << vk::su::UUID( idProperties.deviceUUID ) << "\n";
      std::cout << std::string( "\t\t" ) << "driverUUID      = " << vk::su::UUID( idProperties.driverUUID ) << "\n";
      std::cout << std::string( "\t\t" ) << "deviceLUID      = " << vk::su::LUID( idProperties.deviceLUID ) << "\n";
      std::cout << std::string( "\t\t" ) << "deviceNodeMask  = " << std::hex << idProperties.deviceNodeMask << std::dec << "\n";
      std::cout << std::string( "\t\t" ) << "deviceLUIDValid = " << !!idProperties.deviceLUIDValid << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_inline_uniform_block" ) )
      {
        vk::PhysicalDeviceInlineUniformBlockPropertiesEXT const & inlineUniformBlockProperties =
          properties2.get<vk::PhysicalDeviceInlineUniformBlockPropertiesEXT>();
        std::cout << std::string( "\t" ) << "InlineUniformBlockProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetInlineUniformBlocks                     = " << inlineUniformBlockProperties.maxDescriptorSetInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindInlineUniformBlocks      = "
                  << inlineUniformBlockProperties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxInlineUniformBlockSize                               = " << inlineUniformBlockProperties.maxInlineUniformBlockSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorInlineUniformBlocks                = " << inlineUniformBlockProperties.maxPerStageDescriptorInlineUniformBlocks
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = "
                  << inlineUniformBlockProperties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_line_rasterization" ) )
      {
        vk::PhysicalDeviceLineRasterizationPropertiesEXT const & lineRasterizationProperties =
          properties2.get<vk::PhysicalDeviceLineRasterizationPropertiesEXT>();
        std::cout << std::string( "\t" ) << "LineRasterizationProperties:\n";
        std::cout << std::string( "\t\t" ) << "lineSubPixelPrecisionBits = " << lineRasterizationProperties.lineSubPixelPrecisionBits << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMaintenance3Properties const & maintenance3Properties = properties2.get<vk::PhysicalDeviceMaintenance3Properties>();
      std::cout << std::string( "\t" ) << "Maintenance3Properties:\n";
      std::cout << std::string( "\t\t" ) << "maxMemoryAllocationSize = " << maintenance3Properties.maxMemoryAllocationSize << "\n";
      std::cout << std::string( "\t\t" ) << "maxPerSetDescriptors    = " << maintenance3Properties.maxPerSetDescriptors << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_NV_mesh_shader" ) )
      {
        vk::PhysicalDeviceMeshShaderPropertiesNV const & meshShaderProperties = properties2.get<vk::PhysicalDeviceMeshShaderPropertiesNV>();
        std::cout << std::string( "\t" ) << "MeshShaderProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxDrawMeshTasksCount             = " << meshShaderProperties.maxDrawMeshTasksCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshMultiviewViewCount         = " << meshShaderProperties.maxMeshMultiviewViewCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputPrimitives           = " << meshShaderProperties.maxMeshOutputPrimitives << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputVertices             = " << meshShaderProperties.maxMeshOutputVertices << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshTotalMemorySize            = " << meshShaderProperties.maxMeshTotalMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupInvocations       = " << meshShaderProperties.maxMeshWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupSize              = "
                  << "[" << meshShaderProperties.maxMeshWorkGroupSize[0] << ", " << meshShaderProperties.maxMeshWorkGroupSize[1] << ", "
                  << meshShaderProperties.maxMeshWorkGroupSize[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskOutputCount                = " << meshShaderProperties.maxTaskOutputCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskTotalMemorySize            = " << meshShaderProperties.maxTaskTotalMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupInvocations       = " << meshShaderProperties.maxTaskWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupSize              = "
                  << "[" << meshShaderProperties.maxTaskWorkGroupSize[0] << ", " << meshShaderProperties.maxTaskWorkGroupSize[1] << ", "
                  << meshShaderProperties.maxTaskWorkGroupSize[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "meshOutputPerPrimitiveGranularity = " << meshShaderProperties.meshOutputPerPrimitiveGranularity << "\n";
        std::cout << std::string( "\t\t" ) << "meshOutputPerVertexGranularity    = " << meshShaderProperties.meshOutputPerVertexGranularity << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NVX_multiview_per_view_attributes" ) )
      {
        vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX const & multiviewPerViewAttributesProperties =
          properties2.get<vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX>();
        std::cout << std::string( "\t" ) << "MultiviewPerViewAttributesProperties:\n";
        std::cout << std::string( "\t\t" ) << "perViewPositionAllComponents  = " << !!multiviewPerViewAttributesProperties.perViewPositionAllComponents << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMultiviewProperties const & multiviewProperties = properties2.get<vk::PhysicalDeviceMultiviewProperties>();
      std::cout << std::string( "\t" ) << "MultiviewProperties:\n";
      std::cout << std::string( "\t\t" ) << "maxMultiviewInstanceIndex = " << multiviewProperties.maxMultiviewInstanceIndex << "\n";
      std::cout << std::string( "\t\t" ) << "maxMultiviewViewCount     = " << multiviewProperties.maxMultiviewViewCount << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_pci_bus_info" ) )
      {
        vk::PhysicalDevicePCIBusInfoPropertiesEXT const & pciBusInfoProperties = properties2.get<vk::PhysicalDevicePCIBusInfoPropertiesEXT>();
        std::cout << std::string( "\t" ) << "PCIBusInfoProperties:\n";
        std::cout << std::string( "\t\t" ) << "pciDomain   = " << pciBusInfoProperties.pciDomain << "\n";
        std::cout << std::string( "\t\t" ) << "pciBus      = " << pciBusInfoProperties.pciBus << "\n";
        std::cout << std::string( "\t\t" ) << "pciDevice   = " << pciBusInfoProperties.pciDevice << "\n";
        std::cout << std::string( "\t\t" ) << "pciFunction = " << pciBusInfoProperties.pciFunction << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_maintenance2" ) )
      {
        vk::PhysicalDevicePointClippingProperties const & pointClippingProperties = properties2.get<vk::PhysicalDevicePointClippingProperties>();
        std::cout << std::string( "\t" ) << "PointClippingProperties:\n";
        std::cout << std::string( "\t\t" ) << "pointClippingBehavior = " << vk::to_string( pointClippingProperties.pointClippingBehavior ) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceProtectedMemoryProperties const & protectedMemoryProperties = properties2.get<vk::PhysicalDeviceProtectedMemoryProperties>();
      std::cout << std::string( "\t" ) << "ProtectedMemoryProperties:\n";
      std::cout << std::string( "\t\t" ) << "protectedNoFault  = " << !!protectedMemoryProperties.protectedNoFault << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_push_descriptor" ) )
      {
        vk::PhysicalDevicePushDescriptorPropertiesKHR const & pushDescriptorProperties = properties2.get<vk::PhysicalDevicePushDescriptorPropertiesKHR>();
        std::cout << std::string( "\t" ) << "PushDescriptorProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxPushDescriptors  = " << pushDescriptorProperties.maxPushDescriptors << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_ray_tracing" ) )
      {
        vk::PhysicalDeviceRayTracingPropertiesNV const & rayTracingProperties = properties2.get<vk::PhysicalDeviceRayTracingPropertiesNV>();
        std::cout << std::string( "\t" ) << "RayTracingProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetAccelerationStructures  = " << rayTracingProperties.maxDescriptorSetAccelerationStructures
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxGeometryCount                        = " << rayTracingProperties.maxGeometryCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxInstanceCount                        = " << rayTracingProperties.maxInstanceCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxRecursionDepth                       = " << rayTracingProperties.maxRecursionDepth << "\n";
        std::cout << std::string( "\t\t" ) << "maxShaderGroupStride                    = " << rayTracingProperties.maxShaderGroupStride << "\n";
        std::cout << std::string( "\t\t" ) << "maxTriangleCount                        = " << rayTracingProperties.maxTriangleCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupBaseAlignment                = " << rayTracingProperties.shaderGroupBaseAlignment << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupHandleSize                   = " << rayTracingProperties.shaderGroupHandleSize << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_sample_locations" ) )
      {
        vk::PhysicalDeviceSampleLocationsPropertiesEXT const & sampleLocationProperties = properties2.get<vk::PhysicalDeviceSampleLocationsPropertiesEXT>();
        std::cout << std::string( "\t" ) << "SampleLocationProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxSampleLocationGridSize     = " << sampleLocationProperties.maxSampleLocationGridSize.width << " x "
                  << sampleLocationProperties.maxSampleLocationGridSize.height << "\n";
        std::cout << std::string( "\t\t" ) << "sampleLocationCoordinateRange = "
                  << "[" << sampleLocationProperties.sampleLocationCoordinateRange[0] << ", " << sampleLocationProperties.sampleLocationCoordinateRange[1]
                  << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "sampleLocationSampleCounts    = " << vk::to_string( sampleLocationProperties.sampleLocationSampleCounts )
                  << "\n";
        std::cout << std::string( "\t\t" ) << "sampleLocationSubPixelBits    = " << sampleLocationProperties.sampleLocationSubPixelBits << "\n";
        std::cout << std::string( "\t\t" ) << "variableSampleLocations       = " << !!sampleLocationProperties.variableSampleLocations << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_sampler_filter_minmax" ) )
      {
        vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT const & samplerFilterMinmaxProperties =
          properties2.get<vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT>();
        std::cout << std::string( "\t" ) << "SamplerFilterMinmaxProperties:\n";
        std::cout << std::string( "\t\t" ) << "filterMinmaxImageComponentMapping   = " << !!samplerFilterMinmaxProperties.filterMinmaxImageComponentMapping
                  << "\n";
        std::cout << std::string( "\t\t" ) << "filterMinmaxSingleComponentFormats  = " << !!samplerFilterMinmaxProperties.filterMinmaxSingleComponentFormats
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_shader_core_properties2" ) )
      {
        vk::PhysicalDeviceShaderCoreProperties2AMD const & shaderCoreProperties2 = properties2.get<vk::PhysicalDeviceShaderCoreProperties2AMD>();
        std::cout << std::string( "\t" ) << "ShaderCoreProperties2:\n";
        std::cout << std::string( "\t\t" ) << "activeComputeUnitCount  = " << shaderCoreProperties2.activeComputeUnitCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderCoreFeatures      = " << vk::to_string( shaderCoreProperties2.shaderCoreFeatures ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_shader_core_properties2" ) )
      {
        vk::PhysicalDeviceShaderCorePropertiesAMD const & shaderCoreProperties = properties2.get<vk::PhysicalDeviceShaderCorePropertiesAMD>();
        std::cout << std::string( "\t" ) << "ShaderCoreProperties:\n";
        std::cout << std::string( "\t\t" ) << "computeUnitsPerShaderArray  = " << shaderCoreProperties.computeUnitsPerShaderArray << "\n";
        std::cout << std::string( "\t\t" ) << "maxSgprAllocation           = " << shaderCoreProperties.maxSgprAllocation << "\n";
        std::cout << std::string( "\t\t" ) << "maxVgprAllocation           = " << shaderCoreProperties.maxVgprAllocation << "\n";
        std::cout << std::string( "\t\t" ) << "minSgprAllocation           = " << shaderCoreProperties.minSgprAllocation << "\n";
        std::cout << std::string( "\t\t" ) << "minVgprAllocation           = " << shaderCoreProperties.minVgprAllocation << "\n";
        std::cout << std::string( "\t\t" ) << "sgprAllocationGranularity   = " << shaderCoreProperties.sgprAllocationGranularity << "\n";
        std::cout << std::string( "\t\t" ) << "sgprsPerSimd                = " << shaderCoreProperties.sgprsPerSimd << "\n";
        std::cout << std::string( "\t\t" ) << "shaderArraysPerEngineCount  = " << shaderCoreProperties.shaderArraysPerEngineCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderEngineCount           = " << shaderCoreProperties.shaderEngineCount << "\n";
        std::cout << std::string( "\t\t" ) << "simdPerComputeUnit          = " << shaderCoreProperties.simdPerComputeUnit << "\n";
        std::cout << std::string( "\t\t" ) << "vgprAllocationGranularity   = " << shaderCoreProperties.vgprAllocationGranularity << "\n";
        std::cout << std::string( "\t\t" ) << "vgprsPerSimd                = " << shaderCoreProperties.vgprsPerSimd << "\n";
        std::cout << std::string( "\t\t" ) << "wavefrontSize               = " << shaderCoreProperties.wavefrontSize << "\n";
        std::cout << std::string( "\t\t" ) << "wavefrontsPerSimd           = " << shaderCoreProperties.wavefrontsPerSimd << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_sm_builtins" ) )
      {
        vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV const & shaderSMBuiltinsProperties = properties2.get<vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV>();
        std::cout << std::string( "\t" ) << "ShaderSMBuiltinsProperties:\n";
        std::cout << std::string( "\t\t" ) << "shaderSMCount     = " << shaderSMBuiltinsProperties.shaderSMCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderWarpsPerSM  = " << shaderSMBuiltinsProperties.shaderWarpsPerSM << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shading_rate_image" ) )
      {
        vk::PhysicalDeviceShadingRateImagePropertiesNV const & shadingRageImageProperties = properties2.get<vk::PhysicalDeviceShadingRateImagePropertiesNV>();
        std::cout << std::string( "\t" ) << "ShadingRateImageProperties:\n";
        std::cout << std::string( "\t\t" ) << "shadingRateMaxCoarseSamples = " << shadingRageImageProperties.shadingRateMaxCoarseSamples << "\n";
        std::cout << std::string( "\t\t" ) << "shadingRatePaletteSize      = " << shadingRageImageProperties.shadingRatePaletteSize << "\n";
        std::cout << std::string( "\t\t" ) << "shadingRatePaletteSize      = "
                  << "[" << shadingRageImageProperties.shadingRateTexelSize.width << " x " << shadingRageImageProperties.shadingRateTexelSize.height << "]"
                  << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceSubgroupProperties const & subgroupProperties = properties2.get<vk::PhysicalDeviceSubgroupProperties>();
      std::cout << std::string( "\t" ) << "SubgroupProperties:\n";
      std::cout << std::string( "\t\t" ) << "quadOperationsInAllStages = " << !!subgroupProperties.quadOperationsInAllStages << "\n";
      std::cout << std::string( "\t\t" ) << "subgroupSize              = " << subgroupProperties.subgroupSize << "\n";
      std::cout << std::string( "\t\t" ) << "supportedOperations       = " << vk::to_string( subgroupProperties.supportedOperations ) << "\n";
      std::cout << std::string( "\t\t" ) << "supportedStages           = " << vk::to_string( subgroupProperties.supportedStages ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_subgroup_size_control" ) )
      {
        vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT const & subgroupSizeControlProperties =
          properties2.get<vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT>();
        std::cout << std::string( "\t" ) << "SubgroupSizeControlProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxComputeWorkgroupSubgroups  = " << subgroupSizeControlProperties.maxComputeWorkgroupSubgroups << "\n";
        std::cout << std::string( "\t\t" ) << "maxSubgroupSize               = " << subgroupSizeControlProperties.maxSubgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "minSubgroupSize               = " << subgroupSizeControlProperties.minSubgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "requiredSubgroupSizeStages    = " << vk::to_string( subgroupSizeControlProperties.requiredSubgroupSizeStages )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_timeline_semaphore" ) )
      {
        vk::PhysicalDeviceTimelineSemaphorePropertiesKHR const & timelineSemaphoreProperties =
          properties2.get<vk::PhysicalDeviceTimelineSemaphorePropertiesKHR>();
        std::cout << std::string( "\t" ) << "TimelineSemaphoreProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxTimelineSemaphoreValueDifference = " << timelineSemaphoreProperties.maxTimelineSemaphoreValueDifference
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_texel_buffer_alignment" ) )
      {
        vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT const & texelBufferAlignmentProperties =
          properties2.get<vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT>();
        std::cout << std::string( "\t" ) << "TexelBufferAlignmentProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "storageTexelBufferOffsetAlignmentBytes        = " << texelBufferAlignmentProperties.storageTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" )
                  << "storageTexelBufferOffsetSingleTexelAlignment  = " << !!texelBufferAlignmentProperties.storageTexelBufferOffsetSingleTexelAlignment
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformTexelBufferOffsetAlignmentBytes        = " << texelBufferAlignmentProperties.uniformTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformTexelBufferOffsetSingleTexelAlignment  = " << !!texelBufferAlignmentProperties.uniformTexelBufferOffsetSingleTexelAlignment
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_transform_feedback" ) )
      {
        vk::PhysicalDeviceTransformFeedbackPropertiesEXT const & transformFeedbackProperties =
          properties2.get<vk::PhysicalDeviceTransformFeedbackPropertiesEXT>();
        std::cout << std::string( "\t" ) << "TransformFeedbackProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBufferDataSize          = " << transformFeedbackProperties.maxTransformFeedbackBufferDataSize
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxTransformFeedbackBufferDataStride        = " << transformFeedbackProperties.maxTransformFeedbackBufferDataStride << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBuffers                 = " << transformFeedbackProperties.maxTransformFeedbackBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBufferSize              = " << transformFeedbackProperties.maxTransformFeedbackBufferSize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackStreamDataSize          = " << transformFeedbackProperties.maxTransformFeedbackStreamDataSize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackStreams                 = " << transformFeedbackProperties.maxTransformFeedbackStreams
                  << "\n";
        std::cout << std::string( "\t\t" ) << "transformFeedbackDraw                       = " << !!transformFeedbackProperties.transformFeedbackDraw << "\n";
        std::cout << std::string( "\t\t" ) << "transformFeedbackQueries                    = " << !!transformFeedbackProperties.transformFeedbackQueries
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "transformFeedbackRasterizationStreamSelect  = " << !!transformFeedbackProperties.transformFeedbackRasterizationStreamSelect << "\n";
        std::cout << std::string( "\t\t" )
                  << "transformFeedbackStreamsLinesTriangles      = " << !!transformFeedbackProperties.transformFeedbackStreamsLinesTriangles << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_vertex_attribute_divisor" ) )
      {
        vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT const & vertexAttributeDivisorProperties =
          properties2.get<vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
        std::cout << std::string( "\t" ) << "VertexAttributeDivisorProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxVertexAttribDivisor  = " << vertexAttributeDivisorProperties.maxVertexAttribDivisor << "\n";
        std::cout << "\n";
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
