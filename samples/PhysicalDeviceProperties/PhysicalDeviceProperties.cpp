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
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, {}, VK_API_VERSION_1_3 );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      std::cout << "PhysicalDevice " << i << "\n";

      vk::PhysicalDeviceProperties properties = physicalDevices[i].getProperties();

      {
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
        std::cout << std::string( "\t\t\t" ) << "maxCombinedClipAndCullDistances                 = " << properties.limits.maxCombinedClipAndCullDistances
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxComputeSharedMemorySize                      = " << properties.limits.maxComputeSharedMemorySize << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupCount                        = "
                  << "[" << properties.limits.maxComputeWorkGroupCount[0] << ", " << properties.limits.maxComputeWorkGroupCount[1] << ", "
                  << properties.limits.maxComputeWorkGroupCount[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupInvocations                  = " << properties.limits.maxComputeWorkGroupInvocations
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxComputeWorkGroupSize                         = "
                  << "[" << properties.limits.maxComputeWorkGroupSize[0] << ", " << properties.limits.maxComputeWorkGroupSize[1] << ", "
                  << properties.limits.maxComputeWorkGroupSize[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxCullDistances                                = " << properties.limits.maxCullDistances << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetInputAttachments                = " << properties.limits.maxDescriptorSetInputAttachments
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetSampledImages                   = " << properties.limits.maxDescriptorSetSampledImages << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetSamplers                        = " << properties.limits.maxDescriptorSetSamplers << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageBuffers                  = " << properties.limits.maxDescriptorSetStorageBuffers
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageBuffersDynamic           = " << properties.limits.maxDescriptorSetStorageBuffersDynamic
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetStorageImages                   = " << properties.limits.maxDescriptorSetStorageImages << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxDescriptorSetUniformBuffers                  = " << properties.limits.maxDescriptorSetUniformBuffers
                  << "\n";
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
        std::cout << std::string( "\t\t\t" )
                  << "maxTessellationEvaluationInputComponents        = " << properties.limits.maxTessellationEvaluationInputComponents << "\n";
        std::cout << std::string( "\t\t\t" )
                  << "maxTessellationEvaluationOutputComponents       = " << properties.limits.maxTessellationEvaluationOutputComponents << "\n";
        std::cout << std::string( "\t\t\t" ) << "maxTessellationGenerationLevel                  = " << properties.limits.maxTessellationGenerationLevel
                  << "\n";
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
        std::cout << std::string( "\t\t\t" ) << "minStorageBufferOffsetAlignment                 = " << properties.limits.minStorageBufferOffsetAlignment
                  << "\n";
        std::cout << std::string( "\t\t\t" ) << "minTexelBufferOffsetAlignment                   = " << properties.limits.minTexelBufferOffsetAlignment << "\n";
        std::cout << std::string( "\t\t\t" ) << "minTexelGatherOffset                            = " << properties.limits.minTexelGatherOffset << "\n";
        std::cout << std::string( "\t\t\t" ) << "minTexelOffset                                  = " << properties.limits.minTexelOffset << "\n";
        std::cout << std::string( "\t\t\t" ) << "minUniformBufferOffsetAlignment                 = " << properties.limits.minUniformBufferOffsetAlignment
                  << "\n";
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
        std::cout << std::string( "\t\t\t" ) << "subPixelInterpolationOffsetBits                 = " << properties.limits.subPixelInterpolationOffsetBits
                  << "\n";
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
      }

      // some properties are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      uint32_t apiVersion = properties.apiVersion;
      if ( VK_API_VERSION_1_1 < apiVersion )
      {
        if ( vk::su::contains( extensionProperties, "VK_KHR_acceleration_structure" ) )
        {
          auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceAccelerationStructurePropertiesKHR>();
          vk::PhysicalDeviceAccelerationStructurePropertiesKHR const & accelerationStructureProperties =
            properties2.get<vk::PhysicalDeviceAccelerationStructurePropertiesKHR>();
          std::cout << std::string( "\t" ) << "AccelerationStructureProperties:\n";
          std::cout << std::string( "\t\t" )
                    << "maxGeometryCount                                           = " << accelerationStructureProperties.maxGeometryCount << "\n";
          std::cout << std::string( "\t\t" )
                    << "maxInstanceCount                                           = " << accelerationStructureProperties.maxInstanceCount << "\n";
          std::cout << std::string( "\t\t" )
                    << "maxPrimitiveCount                                          = " << accelerationStructureProperties.maxPrimitiveCount << "\n";
          std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorAccelerationStructures                = "
                    << accelerationStructureProperties.maxPerStageDescriptorAccelerationStructures << "\n";
          std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindAccelerationStructures = "
                    << accelerationStructureProperties.maxPerStageDescriptorUpdateAfterBindAccelerationStructures << "\n";
          std::cout << std::string( "\t\t" )
                    << "maxDescriptorSetAccelerationStructures                     = " << accelerationStructureProperties.maxDescriptorSetAccelerationStructures
                    << "\n";
          std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindAccelerationStructures      = "
                    << accelerationStructureProperties.maxDescriptorSetUpdateAfterBindAccelerationStructures << "\n";
          std::cout << std::string( "\t\t" ) << "minAccelerationStructureScratchOffsetAlignment             = "
                    << accelerationStructureProperties.minAccelerationStructureScratchOffsetAlignment << "\n";
          std::cout << "\n";
        }

        if ( vk::su::contains( extensionProperties, "VK_EXT_blend_operation_advanced" ) )
        {
          auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>();
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
      }

      if ( vk::su::contains( extensionProperties, "VK_HUAWEI_cluster_culling_shader" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceClusterCullingShaderPropertiesHUAWEI>();
        vk::PhysicalDeviceClusterCullingShaderPropertiesHUAWEI clusterCullingShaderProperties =
          properties2.get<vk::PhysicalDeviceClusterCullingShaderPropertiesHUAWEI>();
        std::cout << std::string( "\t" ) << "ClusterCullingShaderProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxWorkGroupCount             = " << clusterCullingShaderProperties.maxWorkGroupCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxWorkGroupSize              = " << clusterCullingShaderProperties.maxWorkGroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxOutputClusterCount         = " << clusterCullingShaderProperties.maxOutputClusterCount << "\n";
        std::cout << std::string( "\t\t" ) << "indirectBufferOffsetAlignment = " << clusterCullingShaderProperties.indirectBufferOffsetAlignment << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_compute_shader_derivatives" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceComputeShaderDerivativesPropertiesKHR>();
        vk::PhysicalDeviceComputeShaderDerivativesPropertiesKHR computeShaderDerivativesProperties =
          properties2.get<vk::PhysicalDeviceComputeShaderDerivativesPropertiesKHR>();
        std::cout << std::string( "\t" ) << "ComputeShaderDerivativesProperties:\n";
        std::cout << std::string( "\t\t" ) << "meshAndTaskShaderDerivatives = " << computeShaderDerivativesProperties.meshAndTaskShaderDerivatives << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_conservative_rasterization" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceConservativeRasterizationPropertiesEXT>();
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
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceCooperativeMatrixPropertiesNV>();
        vk::PhysicalDeviceCooperativeMatrixPropertiesNV const & cooperativeMatrixProperties =
          properties2.get<vk::PhysicalDeviceCooperativeMatrixPropertiesNV>();
        std::cout << std::string( "\t" ) << "CooperativeMatrixProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "cooperativeMatrixSupportedStages = " << vk::to_string( cooperativeMatrixProperties.cooperativeMatrixSupportedStages ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_copy_memory_indirect" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceCopyMemoryIndirectPropertiesNV>();
        vk::PhysicalDeviceCopyMemoryIndirectPropertiesNV const & copyMemoryIndirectProperties =
          properties2.get<vk::PhysicalDeviceCopyMemoryIndirectPropertiesNV>();
        std::cout << std::string( "\t" ) << "CopyMemoryIndirectProperties:\n";
        std::cout << std::string( "\t\t" ) << "supportedQueues = " << vk::to_string( copyMemoryIndirectProperties.supportedQueues ) << "\n";
        std::cout << "\n";
      }

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      if ( vk::su::contains( extensionProperties, "VK_NV_cuda_kernel_launch" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceCudaKernelLaunchPropertiesNV>();
        vk::PhysicalDeviceCudaKernelLaunchPropertiesNV const & cudaKernelLaunchProperties = properties2.get<vk::PhysicalDeviceCudaKernelLaunchPropertiesNV>();
        std::cout << std::string( "\t" ) << "CudaKernelLaunchProperties:\n";
        std::cout << std::string( "\t\t" ) << "computeCapabilityMinor = " << cudaKernelLaunchProperties.computeCapabilityMinor << "\n";
        std::cout << std::string( "\t\t" ) << "computeCapabilityMajor = " << cudaKernelLaunchProperties.computeCapabilityMajor << "\n";
        std::cout << "\n";
      }
#endif

      if ( vk::su::contains( extensionProperties, "VK_EXT_custom_border_color" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceCustomBorderColorPropertiesEXT>();
        vk::PhysicalDeviceCustomBorderColorPropertiesEXT const & customBorderColorProperties =
          properties2.get<vk::PhysicalDeviceCustomBorderColorPropertiesEXT>();
        std::cout << std::string( "\t" ) << "CustomBorderColors:\n";
        std::cout << std::string( "\t\t" ) << "maxCustomBorderColorSamplers = " << customBorderColorProperties.maxCustomBorderColorSamplers << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_2 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_depth_stencil_resolve" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDepthStencilResolveProperties>();
        vk::PhysicalDeviceDepthStencilResolveProperties const & depthStencilResolveProperties =
          properties2.get<vk::PhysicalDeviceDepthStencilResolveProperties>();
        std::cout << std::string( "\t" ) << "DepthStencilResolveProperties:\n";
        std::cout << std::string( "\t\t" ) << "independentResolve            = " << !!depthStencilResolveProperties.independentResolve << "\n";
        std::cout << std::string( "\t\t" ) << "independentResolveNone        = " << !!depthStencilResolveProperties.independentResolveNone << "\n";
        std::cout << std::string( "\t\t" ) << "supportedDepthResolveModes    = " << vk::to_string( depthStencilResolveProperties.supportedDepthResolveModes )
                  << "\n";
        std::cout << std::string( "\t\t" ) << "supportedStencilResolveModes  = " << vk::to_string( depthStencilResolveProperties.supportedStencilResolveModes )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_descriptor_buffer" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT>();
        vk::PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT const & descriptorBufferDensityMapProperties =
          properties2.get<vk::PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT>();
        std::cout << std::string( "\t" ) << "DescriptorBufferDensityMapProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "combinedImageSamplerDensityMapDescriptorSize = " << descriptorBufferDensityMapProperties.combinedImageSamplerDensityMapDescriptorSize
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_descriptor_buffer" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDescriptorBufferPropertiesEXT>();
        vk::PhysicalDeviceDescriptorBufferPropertiesEXT const & descriptorBufferProperties = properties2.get<vk::PhysicalDeviceDescriptorBufferPropertiesEXT>();
        std::cout << std::string( "\t" ) << "DescriptorBufferProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "combinedImageSamplerDescriptorSingleArray            = " << !!descriptorBufferProperties.combinedImageSamplerDescriptorSingleArray
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "bufferlessPushDescriptors                            = " << !!descriptorBufferProperties.bufferlessPushDescriptors << "\n";
        std::cout << std::string( "\t\t" )
                  << "allowSamplerImageViewPostSubmitCreation              = " << !!descriptorBufferProperties.allowSamplerImageViewPostSubmitCreation << "\n";
        std::cout << std::string( "\t\t" )
                  << "descriptorBufferOffsetAlignment                      = " << descriptorBufferProperties.descriptorBufferOffsetAlignment << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorBufferBindings                          = " << descriptorBufferProperties.maxDescriptorBufferBindings << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxResourceDescriptorBufferBindings                  = " << descriptorBufferProperties.maxResourceDescriptorBufferBindings << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxSamplerDescriptorBufferBindings                   = " << descriptorBufferProperties.maxSamplerDescriptorBufferBindings << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxEmbeddedImmutableSamplerBindings                  = " << descriptorBufferProperties.maxEmbeddedImmutableSamplerBindings << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxEmbeddedImmutableSamplers                         = " << descriptorBufferProperties.maxEmbeddedImmutableSamplers << "\n";
        std::cout << std::string( "\t\t" )
                  << "bufferCaptureReplayDescriptorDataSize                = " << descriptorBufferProperties.bufferCaptureReplayDescriptorDataSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "imageCaptureReplayDescriptorDataSize                 = " << descriptorBufferProperties.imageCaptureReplayDescriptorDataSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "imageViewCaptureReplayDescriptorDataSize             = " << descriptorBufferProperties.imageViewCaptureReplayDescriptorDataSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "samplerCaptureReplayDescriptorDataSize               = " << descriptorBufferProperties.samplerCaptureReplayDescriptorDataSize << "\n";
        std::cout << std::string( "\t\t" ) << "accelerationStructureCaptureReplayDescriptorDataSize = "
                  << descriptorBufferProperties.accelerationStructureCaptureReplayDescriptorDataSize << "\n";
        std::cout << std::string( "\t\t" ) << "samplerDescriptorSize                                = " << descriptorBufferProperties.samplerDescriptorSize
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "combinedImageSamplerDescriptorSize                   = " << descriptorBufferProperties.combinedImageSamplerDescriptorSize << "\n";
        std::cout << std::string( "\t\t" ) << "sampledImageDescriptorSize                           = " << descriptorBufferProperties.sampledImageDescriptorSize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "storageImageDescriptorSize                           = " << descriptorBufferProperties.storageImageDescriptorSize
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformTexelBufferDescriptorSize                     = " << descriptorBufferProperties.uniformTexelBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustUniformTexelBufferDescriptorSize               = " << descriptorBufferProperties.robustUniformTexelBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "storageTexelBufferDescriptorSize                     = " << descriptorBufferProperties.storageTexelBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustStorageTexelBufferDescriptorSize               = " << descriptorBufferProperties.robustStorageTexelBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformBufferDescriptorSize                          = " << descriptorBufferProperties.uniformBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustUniformBufferDescriptorSize                    = " << descriptorBufferProperties.robustUniformBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "storageBufferDescriptorSize                          = " << descriptorBufferProperties.storageBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustStorageBufferDescriptorSize                    = " << descriptorBufferProperties.robustStorageBufferDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "inputAttachmentDescriptorSize                        = " << descriptorBufferProperties.inputAttachmentDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "accelerationStructureDescriptorSize                  = " << descriptorBufferProperties.accelerationStructureDescriptorSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxSamplerDescriptorBufferRange                      = " << descriptorBufferProperties.maxSamplerDescriptorBufferRange << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxResourceDescriptorBufferRange                     = " << descriptorBufferProperties.maxResourceDescriptorBufferRange << "\n";
        std::cout << std::string( "\t\t" )
                  << "samplerDescriptorBufferAddressSpaceSize              = " << descriptorBufferProperties.samplerDescriptorBufferAddressSpaceSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "resourceDescriptorBufferAddressSpaceSize             = " << descriptorBufferProperties.resourceDescriptorBufferAddressSpaceSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "descriptorBufferAddressSpaceSize                     = " << descriptorBufferProperties.descriptorBufferAddressSpaceSize << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_2 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_EXT_descriptor_indexing" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDescriptorIndexingProperties>();
        vk::PhysicalDeviceDescriptorIndexingProperties const & descriptorIndexingProperties = properties2.get<vk::PhysicalDeviceDescriptorIndexingProperties>();
        std::cout << std::string( "\t" ) << "DescriptorIndexingProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindInputAttachments      = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindInputAttachments
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSampledImages         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSampledImages
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSamplers              = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSamplers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageBuffers        = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = "
                  << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageImages         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageImages
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindUniformBuffers        = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = "
                  << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindInputAttachments = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindInputAttachments << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindSampledImages    = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSampledImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindSamplers         = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSamplers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindStorageBuffers   = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageBuffers << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindStorageImages    = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageImages << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindUniformBuffers   = "
                  << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindUniformBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageUpdateAfterBindResources                  = " << descriptorIndexingProperties.maxPerStageUpdateAfterBindResources << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxUpdateAfterBindDescriptorsInAllPools              = " << descriptorIndexingProperties.maxUpdateAfterBindDescriptorsInAllPools << "\n";
        std::cout << std::string( "\t\t" )
                  << "quadDivergentImplicitLod                             = " << !!descriptorIndexingProperties.quadDivergentImplicitLod << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustBufferAccessUpdateAfterBind                    = " << !!descriptorIndexingProperties.robustBufferAccessUpdateAfterBind << "\n";
        std::cout << std::string( "\t\t" ) << "shaderInputAttachmentArrayNonUniformIndexingNative   = "
                  << !!descriptorIndexingProperties.shaderInputAttachmentArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderSampledImageArrayNonUniformIndexingNative      = "
                  << !!descriptorIndexingProperties.shaderSampledImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderStorageBufferArrayNonUniformIndexingNative     = "
                  << !!descriptorIndexingProperties.shaderStorageBufferArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderStorageImageArrayNonUniformIndexingNative      = "
                  << !!descriptorIndexingProperties.shaderStorageImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" ) << "shaderUniformBufferArrayNonUniformIndexingNative     = "
                  << !!descriptorIndexingProperties.shaderUniformBufferArrayNonUniformIndexingNative << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_device_generated_commands" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV>();
        vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV const & deviceGeneratedCommandsProperties =
          properties2.get<vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV>();
        std::cout << std::string( "\t" ) << "DeviceGeneratedCommandsProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxGraphicsShaderGroupCount              = " << deviceGeneratedCommandsProperties.maxGraphicsShaderGroupCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxIndirectSequenceCount                 = " << deviceGeneratedCommandsProperties.maxIndirectSequenceCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxIndirectCommandsTokenCount            = " << deviceGeneratedCommandsProperties.maxIndirectCommandsTokenCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxIndirectCommandsStreamCount           = " << deviceGeneratedCommandsProperties.maxIndirectCommandsStreamCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxGraphicsShaderGroupCount              = " << deviceGeneratedCommandsProperties.maxGraphicsShaderGroupCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxIndirectCommandsTokenOffset           = " << deviceGeneratedCommandsProperties.maxIndirectCommandsTokenOffset
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxIndirectCommandsStreamStride          = " << deviceGeneratedCommandsProperties.maxIndirectCommandsStreamStride
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "minSequencesCountBufferOffsetAlignment   = " << deviceGeneratedCommandsProperties.minSequencesCountBufferOffsetAlignment << "\n";
        std::cout << std::string( "\t\t" )
                  << "minSequencesIndexBufferOffsetAlignment   = " << deviceGeneratedCommandsProperties.minSequencesIndexBufferOffsetAlignment << "\n";
        std::cout << std::string( "\t\t" )
                  << "minIndirectCommandsBufferOffsetAlignment = " << deviceGeneratedCommandsProperties.minIndirectCommandsBufferOffsetAlignment << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_discard_rectangles" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDiscardRectanglePropertiesEXT>();
        vk::PhysicalDeviceDiscardRectanglePropertiesEXT const & discardRectangleProperties = properties2.get<vk::PhysicalDeviceDiscardRectanglePropertiesEXT>();
        std::cout << std::string( "\t" ) << "DiscardRectangleProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxDiscardRectangles = " << discardRectangleProperties.maxDiscardRectangles << "\n";
        std::cout << "\n";
      }

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      if ( vk::su::contains( extensionProperties, "VK_NV_displacement_micromap" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDisplacementMicromapPropertiesNV>();
        vk::PhysicalDeviceDisplacementMicromapPropertiesNV const & displacementMicromapProperties =
          properties2.get<vk::PhysicalDeviceDisplacementMicromapPropertiesNV>();
        std::cout << std::string( "\t" ) << "DisplacementMicromapProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxDisplacementMicromapSubdivisionLevel = " << displacementMicromapProperties.maxDisplacementMicromapSubdivisionLevel << "\n";
        std::cout << "\n";
      }
#endif

      if ( ( VK_API_VERSION_1_2 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_driver_properties" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>();
        vk::PhysicalDeviceDriverProperties const & driverProperties = properties2.get<vk::PhysicalDeviceDriverProperties>();
        std::cout << std::string( "\t" ) << "DriverProperties:\n";
        std::cout << std::string( "\t\t" ) << "driverID            = " << vk::to_string( driverProperties.driverID ) << "\n";
        std::cout << std::string( "\t\t" ) << "driverName          = " << driverProperties.driverName << "\n";
        std::cout << std::string( "\t\t" ) << "driverInfo          = " << driverProperties.driverInfo << "\n";
        std::cout << std::string( "\t\t" ) << "conformanceVersion  = " << static_cast<size_t>( driverProperties.conformanceVersion.major ) << "."
                  << static_cast<size_t>( driverProperties.conformanceVersion.minor ) << "."
                  << static_cast<size_t>( driverProperties.conformanceVersion.subminor ) << "."
                  << static_cast<size_t>( driverProperties.conformanceVersion.patch ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_physical_device_drm" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDrmPropertiesEXT>();
        vk::PhysicalDeviceDrmPropertiesEXT const & drmProperties = properties2.get<vk::PhysicalDeviceDrmPropertiesEXT>();
        std::cout << std::string( "\t" ) << "DrmProperties:\n";
        std::cout << std::string( "\t\t" ) << "hasPrimary = " << !!drmProperties.hasPrimary << "\n";
        std::cout << std::string( "\t\t" ) << "hasRender = " << !!drmProperties.hasRender << "\n";
        std::cout << std::string( "\t\t" ) << "primaryMajor = " << drmProperties.primaryMajor << "\n";
        std::cout << std::string( "\t\t" ) << "primaryMinor = " << drmProperties.primaryMinor << "\n";
        std::cout << std::string( "\t\t" ) << "renderMajor = " << drmProperties.renderMajor << "\n";
        std::cout << std::string( "\t\t" ) << "renderMinor = " << drmProperties.renderMinor << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_extended_dynamic_state3" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceExtendedDynamicState3PropertiesEXT>();
        vk::PhysicalDeviceExtendedDynamicState3PropertiesEXT const & extendedDynamicState3Properties =
          properties2.get<vk::PhysicalDeviceExtendedDynamicState3PropertiesEXT>();
        std::cout << std::string( "\t" ) << "ExtendedDynamicState3Properties:\n";
        std::cout << std::string( "\t\t" )
                  << "dynamicPrimitiveTopologyUnrestricted = " << !!extendedDynamicState3Properties.dynamicPrimitiveTopologyUnrestricted << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_extended_sparse_address_space" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceExtendedSparseAddressSpacePropertiesNV>();
        vk::PhysicalDeviceExtendedSparseAddressSpacePropertiesNV const & extendedSparseAddressSpaceProperties =
          properties2.get<vk::PhysicalDeviceExtendedSparseAddressSpacePropertiesNV>();
        std::cout << std::string( "\t" ) << "ExtendedSparseAddressSpaceProperties:\n";
        std::cout << std::string( "\t\t" ) << "extendedSparseAddressSpaceSize = " << extendedSparseAddressSpaceProperties.extendedSparseAddressSpaceSize
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "extendedSparseImageUsageFlags  = " << vk::to_string( extendedSparseAddressSpaceProperties.extendedSparseImageUsageFlags ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "extendedSparseBufferUsageFlags = " << vk::to_string( extendedSparseAddressSpaceProperties.extendedSparseBufferUsageFlags ) << "\n";
        std::cout << "\n";
      }

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
      if ( vk::su::contains( extensionProperties, "VK_ANDROID_external_format_resolve" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceExternalFormatResolvePropertiesANDROID>();
        vk::PhysicalDeviceExternalFormatResolvePropertiesANDROID const & externalFormatResolveProperties =
          properties2.get<vk::PhysicalDeviceExternalFormatResolvePropertiesANDROID>();
        std::cout << std::string( "\t" ) << "ExternalFormatResolveProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "nullColorAttachmentWithExternalFormatResolve = " << !!externalFormatResolveProperties.nullColorAttachmentWithExternalFormatResolve
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "externalFormatResolveChromaOffsetX           = " << vk::to_string( externalFormatResolveProperties.externalFormatResolveChromaOffsetX )
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "externalFormatResolveChromaOffsetY           = " << vk::to_string( externalFormatResolveProperties.externalFormatResolveChromaOffsetY )
                  << "\n";
        std::cout << "\n";
      }
#endif

      if ( vk::su::contains( extensionProperties, "VK_EXT_external_memory_host" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceExternalMemoryHostPropertiesEXT>();
        vk::PhysicalDeviceExternalMemoryHostPropertiesEXT const & externalMemoryHostProperties =
          properties2.get<vk::PhysicalDeviceExternalMemoryHostPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ExternalMemoryHostProperties:\n";
        std::cout << std::string( "\t\t" ) << "minImportedHostPointerAlignment = " << externalMemoryHostProperties.minImportedHostPointerAlignment << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_2 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_shader_float_controls" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFloatControlsProperties>();
        vk::PhysicalDeviceFloatControlsProperties const & floatControlsProperties = properties2.get<vk::PhysicalDeviceFloatControlsProperties>();
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
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentDensityMapPropertiesEXT>();
        vk::PhysicalDeviceFragmentDensityMapPropertiesEXT const & fragmentDensityMapProperties =
          properties2.get<vk::PhysicalDeviceFragmentDensityMapPropertiesEXT>();
        std::cout << std::string( "\t" ) << "FragmentDensityMapProperties:\n";
        std::cout << std::string( "\t\t" ) << "fragmentDensityInvocations  = " << !!fragmentDensityMapProperties.fragmentDensityInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxFragmentDensityTexelSize = " << fragmentDensityMapProperties.maxFragmentDensityTexelSize.width << " x "
                  << fragmentDensityMapProperties.maxFragmentDensityTexelSize.height << "\n";
        std::cout << std::string( "\t\t" ) << "minFragmentDensityTexelSize = " << fragmentDensityMapProperties.minFragmentDensityTexelSize.width << " x "
                  << fragmentDensityMapProperties.minFragmentDensityTexelSize.height << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_density_map2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT>();
        vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT const & fragmentDensityMapProperties =
          properties2.get<vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT>();
        std::cout << std::string( "\t" ) << "FragmentDensityMap2Properties:\n";
        std::cout << std::string( "\t\t" ) << "subsampledLoads = " << !!fragmentDensityMapProperties.subsampledLoads << "\n";
        std::cout << std::string( "\t\t" )
                  << "subsampledCoarseReconstructionEarlyAccess = " << !!fragmentDensityMapProperties.subsampledCoarseReconstructionEarlyAccess << "\n";
        std::cout << std::string( "\t\t" ) << "maxSubsampledArrayLayers = " << fragmentDensityMapProperties.maxSubsampledArrayLayers << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetSubsampledSamplers = " << fragmentDensityMapProperties.maxDescriptorSetSubsampledSamplers
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_QCOM_fragment_density_map_offset" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM>();
        vk::PhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM const & fragmentDensityMapOffsetProperties =
          properties2.get<vk::PhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM>();
        std::cout << std::string( "\t" ) << "FragmentDensityMapOffsetProperties:\n";
        std::cout << std::string( "\t\t" ) << "fragmentDensityOffsetGranularity = "
                  << "[" << fragmentDensityMapOffsetProperties.fragmentDensityOffsetGranularity.width << " x "
                  << fragmentDensityMapOffsetProperties.fragmentDensityOffsetGranularity.height << "]"
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_fragment_shader_barycentric" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentShaderBarycentricPropertiesKHR>();
        vk::PhysicalDeviceFragmentShaderBarycentricPropertiesKHR const & fragmentShaderBarycentricProperties =
          properties2.get<vk::PhysicalDeviceFragmentShaderBarycentricPropertiesKHR>();
        std::cout << std::string( "\t" ) << "FragmentShaderBarycentricProperties:\n";
        std::cout << std::string( "\t\t" ) << "triStripVertexOrderIndependentOfProvokingVertex = "
                  << !!fragmentShaderBarycentricProperties.triStripVertexOrderIndependentOfProvokingVertex << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_fragment_shading_rate_enums" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV>();
        vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV const & fragmentShadingRateEnumsProperties =
          properties2.get<vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV>();
        std::cout << std::string( "\t" ) << "FragmentShadingRateEnumsProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxFragmentShadingRateInvocationCount = " << vk::to_string( fragmentShadingRateEnumsProperties.maxFragmentShadingRateInvocationCount )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_fragment_shading_rate" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceFragmentShadingRatePropertiesKHR>();
        vk::PhysicalDeviceFragmentShadingRatePropertiesKHR const & fragmentShadingRateProperties =
          properties2.get<vk::PhysicalDeviceFragmentShadingRatePropertiesKHR>();
        std::cout << std::string( "\t" ) << "FragmentShadingRateProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "minFragmentShadingRateAttachmentTexelSize            = " << fragmentShadingRateProperties.minFragmentShadingRateAttachmentTexelSize.width
                  << " x " << fragmentShadingRateProperties.minFragmentShadingRateAttachmentTexelSize.height << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxFragmentShadingRateAttachmentTexelSize            = " << fragmentShadingRateProperties.maxFragmentShadingRateAttachmentTexelSize.width
                  << " x " << fragmentShadingRateProperties.maxFragmentShadingRateAttachmentTexelSize.height << "\n";
        std::cout << std::string( "\t\t" ) << "maxFragmentShadingRateAttachmentTexelSizeAspectRatio = "
                  << fragmentShadingRateProperties.maxFragmentShadingRateAttachmentTexelSizeAspectRatio << "\n";
        std::cout << std::string( "\t\t" ) << "primitiveFragmentShadingRateWithMultipleViewports    = "
                  << !!fragmentShadingRateProperties.primitiveFragmentShadingRateWithMultipleViewports << "\n";
        std::cout << std::string( "\t\t" )
                  << "layeredShadingRateAttachments                        = " << !!fragmentShadingRateProperties.layeredShadingRateAttachments << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateNonTrivialCombinerOps             = " << !!fragmentShadingRateProperties.fragmentShadingRateNonTrivialCombinerOps
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxFragmentSize                                      = " << fragmentShadingRateProperties.maxFragmentSize.width
                  << " x " << fragmentShadingRateProperties.maxFragmentSize.height << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxFragmentSizeAspectRatio                           = " << fragmentShadingRateProperties.maxFragmentSizeAspectRatio << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxFragmentShadingRateCoverageSamples                = " << fragmentShadingRateProperties.maxFragmentShadingRateCoverageSamples << "\n";
        std::cout << std::string( "\t\t" ) << "maxFragmentShadingRateRasterizationSamples           = "
                  << vk::to_string( fragmentShadingRateProperties.maxFragmentShadingRateRasterizationSamples ) << "\n";
        std::cout << std::string( "\t\t" ) << "fragmentShadingRateWithShaderDepthStencilWrites      = "
                  << !!fragmentShadingRateProperties.fragmentShadingRateWithShaderDepthStencilWrites << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateWithSampleMask                    = " << !!fragmentShadingRateProperties.fragmentShadingRateWithSampleMask << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateWithShaderSampleMask              = " << !!fragmentShadingRateProperties.fragmentShadingRateWithShaderSampleMask
                  << "\n";
        std::cout << std::string( "\t\t" ) << "fragmentShadingRateWithConservativeRasterization     = "
                  << !!fragmentShadingRateProperties.fragmentShadingRateWithConservativeRasterization << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateWithFragmentShaderInterlock       = " << !!fragmentShadingRateProperties.fragmentShadingRateWithFragmentShaderInterlock
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateWithCustomSampleLocations         = " << !!fragmentShadingRateProperties.fragmentShadingRateWithCustomSampleLocations
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "fragmentShadingRateStrictMultiplyCombiner            = " << !!fragmentShadingRateProperties.fragmentShadingRateStrictMultiplyCombiner
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_graphics_pipeline_library" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT>();
        vk::PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT const & graphicsPipelineLibraryProperties =
          properties2.get<vk::PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT>();
        std::cout << std::string( "\t" ) << "GraphicsPipelineLibraryProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "graphicsPipelineLibraryFastLinking                        = " << !!graphicsPipelineLibraryProperties.graphicsPipelineLibraryFastLinking
                  << "\n";
        std::cout << std::string( "\t\t" ) << "graphicsPipelineLibraryIndependentInterpolationDecoration = "
                  << !!graphicsPipelineLibraryProperties.graphicsPipelineLibraryIndependentInterpolationDecoration << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_host_image_copy" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceHostImageCopyPropertiesEXT>();
        vk::PhysicalDeviceHostImageCopyPropertiesEXT & hostImageCopyProperties = properties2.get<vk::PhysicalDeviceHostImageCopyPropertiesEXT>();

        std::vector<vk::ImageLayout> srcLayouts( hostImageCopyProperties.copySrcLayoutCount );
        std::vector<vk::ImageLayout> dstLayouts( hostImageCopyProperties.copyDstLayoutCount );
        hostImageCopyProperties.pCopySrcLayouts = srcLayouts.data();
        hostImageCopyProperties.pCopyDstLayouts = dstLayouts.data();

        physicalDevices[i].getProperties2( &properties2.get<vk::PhysicalDeviceProperties2>() );

        std::cout << std::string( "\t" ) << "HostImageCopyProperties:\n";
        std::cout << std::string( "\t\t" ) << "copySrcLayoutCount              = " << hostImageCopyProperties.copySrcLayoutCount << "\n";
        std::cout << std::string( "\t\t" ) << "pCopySrcLayouts                 = "
                  << "\n";
        for ( uint32_t j = 0; j < hostImageCopyProperties.copySrcLayoutCount; ++j )
        {
          std::cout << std::string( "\t\t\t" ) << j << " : " << vk::to_string( hostImageCopyProperties.pCopySrcLayouts[j] ) << "\n";
        }
        std::cout << std::string( "\t\t" ) << "copyDstLayoutCount              = " << hostImageCopyProperties.copyDstLayoutCount << "\n";
        std::cout << std::string( "\t\t" ) << "pCopyDstLayouts                 = "
                  << "\n";
        for ( uint32_t j = 0; j < hostImageCopyProperties.copyDstLayoutCount; ++j )
        {
          std::cout << std::string( "\t\t\t" ) << j << " : " << vk::to_string( hostImageCopyProperties.pCopyDstLayouts[j] ) << "\n";
        }
        std::cout << std::string( "\t\t" ) << "optimalTilingLayoutUUID         = " << vk::su::UUID( hostImageCopyProperties.optimalTilingLayoutUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "identicalMemoryTypeRequirements = " << !!hostImageCopyProperties.identicalMemoryTypeRequirements << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_1 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_external_memory_capabilities" ) )
      {
        auto                                   properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties>();
        vk::PhysicalDeviceIDProperties const & idProperties = properties2.get<vk::PhysicalDeviceIDProperties>();
        std::cout << std::string( "\t" ) << "IDProperties:\n";
        std::cout << std::string( "\t\t" ) << "deviceUUID      = " << vk::su::UUID( idProperties.deviceUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "driverUUID      = " << vk::su::UUID( idProperties.driverUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "deviceLUID      = " << vk::su::LUID( idProperties.deviceLUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "deviceNodeMask  = " << std::hex << idProperties.deviceNodeMask << std::dec << "\n";
        std::cout << std::string( "\t\t" ) << "deviceLUIDValid = " << !!idProperties.deviceLUIDValid << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_MESA_image_alignment_control" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceImageAlignmentControlPropertiesMESA>();
        vk::PhysicalDeviceImageAlignmentControlPropertiesMESA const & imageAlignmentControlProperties =
          properties2.get<vk::PhysicalDeviceImageAlignmentControlPropertiesMESA>();
        std::cout << std::string( "\t" ) << "ImageAlignmentControlProperties:\n";
        std::cout << std::string( "\t\t" ) << "supportedImageAlignmentMask = " << imageAlignmentControlProperties.supportedImageAlignmentMask << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_QCOM_image_processing" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceImageProcessingPropertiesQCOM>();
        vk::PhysicalDeviceImageProcessingPropertiesQCOM const & imageProcessingProperties = properties2.get<vk::PhysicalDeviceImageProcessingPropertiesQCOM>();
        std::cout << std::string( "\t" ) << "ImageProcessingProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxWeightFilterPhases    = " << imageProcessingProperties.maxWeightFilterPhases << "\n";
        std::cout << std::string( "\t\t" ) << "maxWeightFilterDimension = " << imageProcessingProperties.maxWeightFilterDimension.width << " x "
                  << imageProcessingProperties.maxWeightFilterDimension.height << "\n";
        std::cout << std::string( "\t\t" ) << "maxBlockMatchRegion      = " << imageProcessingProperties.maxBlockMatchRegion.width << " x "
                  << imageProcessingProperties.maxBlockMatchRegion.height << "\n";
        std::cout << std::string( "\t\t" ) << "maxBoxFilterBlockSize    = " << imageProcessingProperties.maxBoxFilterBlockSize.width << " x "
                  << imageProcessingProperties.maxBoxFilterBlockSize.height << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_QCOM_image_processing2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceImageProcessing2PropertiesQCOM>();
        vk::PhysicalDeviceImageProcessing2PropertiesQCOM const & imageProcessing2Properties =
          properties2.get<vk::PhysicalDeviceImageProcessing2PropertiesQCOM>();
        std::cout << std::string( "\t" ) << "ImageProcessing2Properties:\n";
        std::cout << std::string( "\t\t" ) << "maxBlockMatchWindow = " << imageProcessing2Properties.maxBlockMatchWindow.width << " x "
                  << imageProcessing2Properties.maxBlockMatchWindow.height << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_3 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_EXT_inline_uniform_block" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceInlineUniformBlockProperties>();
        vk::PhysicalDeviceInlineUniformBlockProperties const & inlineUniformBlockProperties = properties2.get<vk::PhysicalDeviceInlineUniformBlockProperties>();
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

      if ( vk::su::contains( extensionProperties, "VK_KHR_maintenance7" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceLayeredApiPropertiesListKHR>();
        vk::PhysicalDeviceLayeredApiPropertiesListKHR const & layeredApiProperties = properties2.get<vk::PhysicalDeviceLayeredApiPropertiesListKHR>();
        std::cout << std::string( "\t" ) << "LayeredApiProperties:\n";
        std::cout << std::string( "\t\t" ) << "layeredApiCount = " << layeredApiProperties.layeredApiCount << "\n";
        for ( uint32_t j = 0; j < layeredApiProperties.layeredApiCount; ++j )
        {
          std::cout << std::string( "\t\t\t" ) << "layeredApi " << j << "\n";
          std::cout << std::string( "\t\t\t\t" ) << "vendorID   = " << decodeVendorID( layeredApiProperties.pLayeredApis[j].vendorID ) << "\n";
          std::cout << std::string( "\t\t\t\t" ) << "deviceID   = " << layeredApiProperties.pLayeredApis[j].deviceID << "\n";
          std::cout << std::string( "\t\t\t\t" ) << "layeredAPI = " << vk::to_string( layeredApiProperties.pLayeredApis[j].layeredAPI ) << "\n";
          std::cout << std::string( "\t\t\t\t" ) << "deviceName = " << layeredApiProperties.pLayeredApis[j].deviceName << "\n";
        }
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_MSFT_layered_driver" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceLayeredDriverPropertiesMSFT>();
        vk::PhysicalDeviceLayeredDriverPropertiesMSFT const & layeredDriverProperties = properties2.get<vk::PhysicalDeviceLayeredDriverPropertiesMSFT>();
        std::cout << std::string( "\t" ) << "LayeredDriverProperties:\n";
        std::cout << std::string( "\t\t" ) << "underlyingAPI = " << vk::to_string( layeredDriverProperties.underlyingAPI ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_legacy_vertex_attributes" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceLegacyVertexAttributesPropertiesEXT>();
        vk::PhysicalDeviceLegacyVertexAttributesPropertiesEXT const & legacyVertexAttributesProperties =
          properties2.get<vk::PhysicalDeviceLegacyVertexAttributesPropertiesEXT>();
        std::cout << std::string( "\t" ) << "LegacyVertexAttributesProperties:\n";
        std::cout << std::string( "\t\t" ) << "nativeUnalignedPerformance = " << !!legacyVertexAttributesProperties.nativeUnalignedPerformance << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_line_rasterization" ) || vk::su::contains( extensionProperties, "VK_KHR_line_rasterization" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceLineRasterizationPropertiesKHR>();
        vk::PhysicalDeviceLineRasterizationPropertiesKHR const & lineRasterizationProperties =
          properties2.get<vk::PhysicalDeviceLineRasterizationPropertiesKHR>();
        std::cout << std::string( "\t" ) << "LineRasterizationProperties:\n";
        std::cout << std::string( "\t\t" ) << "lineSubPixelPrecisionBits = " << lineRasterizationProperties.lineSubPixelPrecisionBits << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_1 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_maintenance3" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance3Properties>();
        vk::PhysicalDeviceMaintenance3Properties const & maintenance3Properties = properties2.get<vk::PhysicalDeviceMaintenance3Properties>();
        std::cout << std::string( "\t" ) << "Maintenance3Properties:\n";
        std::cout << std::string( "\t\t" ) << "maxMemoryAllocationSize = " << maintenance3Properties.maxMemoryAllocationSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerSetDescriptors    = " << maintenance3Properties.maxPerSetDescriptors << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_3 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_maintenance4" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance4Properties>();
        vk::PhysicalDeviceMaintenance4Properties const & maintenance4Properties = properties2.get<vk::PhysicalDeviceMaintenance4Properties>();
        std::cout << std::string( "\t" ) << "Maintenance4Properties:\n";
        std::cout << std::string( "\t\t" ) << "maxBufferSize = " << maintenance4Properties.maxBufferSize << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_maintenance5" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance5PropertiesKHR>();
        vk::PhysicalDeviceMaintenance5PropertiesKHR const & maintenance5Properties = properties2.get<vk::PhysicalDeviceMaintenance5PropertiesKHR>();
        std::cout << std::string( "\t" ) << "Maintenance5Properties:\n";
        std::cout << std::string( "\t\t" )
                  << "earlyFragmentMultisampleCoverageAfterSampleCounting = " << !!maintenance5Properties.earlyFragmentMultisampleCoverageAfterSampleCounting
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "earlyFragmentSampleMaskTestBeforeSampleCounting     = " << !!maintenance5Properties.earlyFragmentSampleMaskTestBeforeSampleCounting
                  << "\n";
        std::cout << std::string( "\t\t" ) << "depthStencilSwizzleOneSupport                       = " << !!maintenance5Properties.depthStencilSwizzleOneSupport
                  << "\n";
        std::cout << std::string( "\t\t" ) << "polygonModePointSize                                = " << !!maintenance5Properties.polygonModePointSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "nonStrictSinglePixelWideLinesUseParallelogram       = " << !!maintenance5Properties.nonStrictSinglePixelWideLinesUseParallelogram << "\n";
        std::cout << std::string( "\t\t" )
                  << "nonStrictWideLinesUseParallelogram                  = " << !!maintenance5Properties.nonStrictWideLinesUseParallelogram << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_maintenance6" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance6PropertiesKHR>();
        vk::PhysicalDeviceMaintenance6PropertiesKHR const & maintenance6Properties = properties2.get<vk::PhysicalDeviceMaintenance6PropertiesKHR>();
        std::cout << std::string( "\t" ) << "Maintenance6Properties:\n";
        std::cout << std::string( "\t\t" ) << "blockTexelViewCompatibleMultipleLayers = " << !!maintenance6Properties.blockTexelViewCompatibleMultipleLayers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxCombinedImageSamplerDescriptorCount = " << maintenance6Properties.maxCombinedImageSamplerDescriptorCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "fragmentShadingRateClampCombinerInputs = " << !!maintenance6Properties.fragmentShadingRateClampCombinerInputs
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_maintenance7" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance7PropertiesKHR>();
        vk::PhysicalDeviceMaintenance7PropertiesKHR const & maintenance7Properties = properties2.get<vk::PhysicalDeviceMaintenance7PropertiesKHR>();
        std::cout << std::string( "\t" ) << "Maintenance7Properties:\n";
        std::cout << std::string( "\t\t" )
                  << "robustFragmentShadingRateAttachmentAccess                 = " << maintenance7Properties.robustFragmentShadingRateAttachmentAccess << "\n";
        std::cout << std::string( "\t\t" )
                  << "separateDepthStencilAttachmentAccess                      = " << maintenance7Properties.separateDepthStencilAttachmentAccess << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetTotalUniformBuffersDynamic                = " << maintenance7Properties.maxDescriptorSetTotalUniformBuffersDynamic
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetTotalStorageBuffersDynamic                = " << maintenance7Properties.maxDescriptorSetTotalStorageBuffersDynamic
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetTotalBuffersDynamic                       = " << maintenance7Properties.maxDescriptorSetTotalBuffersDynamic << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic = "
                  << maintenance7Properties.maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic << "\n"
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindTotalBuffersDynamic        = " << maintenance7Properties.maxDescriptorSetUpdateAfterBindTotalBuffersDynamic
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_map_memory_placed" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMapMemoryPlacedPropertiesEXT>();
        vk::PhysicalDeviceMapMemoryPlacedPropertiesEXT const & mapMemoryPlacedProperties = properties2.get<vk::PhysicalDeviceMapMemoryPlacedPropertiesEXT>();
        std::cout << std::string( "\t" ) << "MapMemoryPlacedProperties:\n";
        std::cout << std::string( "\t\t" ) << "minPlacedMemoryMapAlignment = " << mapMemoryPlacedProperties.minPlacedMemoryMapAlignment << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_memory_decompression" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMemoryDecompressionPropertiesNV>();
        vk::PhysicalDeviceMemoryDecompressionPropertiesNV const & memoryDecompressionProperties =
          properties2.get<vk::PhysicalDeviceMemoryDecompressionPropertiesNV>();
        std::cout << std::string( "\t" ) << "MemoryDecompressionProperties:\n";
        std::cout << std::string( "\t\t" ) << "decompressionMethods          = " << vk::to_string( memoryDecompressionProperties.decompressionMethods ) << "\n";
        std::cout << std::string( "\t\t" ) << "maxDecompressionIndirectCount = " << memoryDecompressionProperties.maxDecompressionIndirectCount << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_mesh_shader" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMeshShaderPropertiesEXT>();
        vk::PhysicalDeviceMeshShaderPropertiesEXT const & meshShaderProperties = properties2.get<vk::PhysicalDeviceMeshShaderPropertiesEXT>();
        std::cout << std::string( "\t" ) << "MeshShaderPropertiesEXT:\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupTotalCount            = " << meshShaderProperties.maxTaskWorkGroupTotalCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupCount                 = "
                  << "[" << meshShaderProperties.maxTaskWorkGroupCount[0] << ", " << meshShaderProperties.maxTaskWorkGroupCount[1] << ", "
                  << meshShaderProperties.maxTaskWorkGroupCount[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupInvocations           = " << meshShaderProperties.maxTaskWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskWorkGroupSize                  = "
                  << "[" << meshShaderProperties.maxTaskWorkGroupSize[0] << ", " << meshShaderProperties.maxTaskWorkGroupSize[1] << ", "
                  << meshShaderProperties.maxTaskWorkGroupSize[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskPayloadSize                    = " << meshShaderProperties.maxTaskPayloadSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskSharedMemorySize               = " << meshShaderProperties.maxTaskSharedMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxTaskPayloadAndSharedMemorySize     = " << meshShaderProperties.maxTaskPayloadAndSharedMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupTotalCount            = " << meshShaderProperties.maxMeshWorkGroupTotalCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupCount                 = "
                  << "[" << meshShaderProperties.maxMeshWorkGroupCount[0] << ", " << meshShaderProperties.maxMeshWorkGroupCount[1] << ", "
                  << meshShaderProperties.maxMeshWorkGroupCount[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupInvocations           = " << meshShaderProperties.maxMeshWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshWorkGroupSize                  = "
                  << "[" << meshShaderProperties.maxMeshWorkGroupSize[0] << ", " << meshShaderProperties.maxMeshWorkGroupSize[1] << ", "
                  << meshShaderProperties.maxMeshWorkGroupSize[2] << "]"
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshSharedMemorySize               = " << meshShaderProperties.maxMeshSharedMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshPayloadAndSharedMemorySize     = " << meshShaderProperties.maxMeshPayloadAndSharedMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputMemorySize               = " << meshShaderProperties.maxMeshOutputMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshPayloadAndOutputMemorySize     = " << meshShaderProperties.maxMeshPayloadAndOutputMemorySize << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputComponents               = " << meshShaderProperties.maxMeshOutputComponents << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputVertices                 = " << meshShaderProperties.maxMeshOutputVertices << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputPrimitives               = " << meshShaderProperties.maxMeshOutputPrimitives << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshOutputLayers                   = " << meshShaderProperties.maxMeshOutputLayers << "\n";
        std::cout << std::string( "\t\t" ) << "maxMeshMultiviewViewCount             = " << meshShaderProperties.maxMeshMultiviewViewCount << "\n";
        std::cout << std::string( "\t\t" ) << "meshOutputPerVertexGranularity        = " << meshShaderProperties.meshOutputPerVertexGranularity << "\n";
        std::cout << std::string( "\t\t" ) << "meshOutputPerPrimitiveGranularity     = " << meshShaderProperties.meshOutputPerPrimitiveGranularity << "\n";
        std::cout << std::string( "\t\t" ) << "maxPreferredTaskWorkGroupInvocations  = " << meshShaderProperties.maxPreferredTaskWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "maxPreferredMeshWorkGroupInvocations  = " << meshShaderProperties.maxPreferredMeshWorkGroupInvocations << "\n";
        std::cout << std::string( "\t\t" ) << "prefersLocalInvocationVertexOutput    = " << !!meshShaderProperties.prefersLocalInvocationVertexOutput << "\n";
        std::cout << std::string( "\t\t" ) << "prefersLocalInvocationPrimitiveOutput = " << !!meshShaderProperties.prefersLocalInvocationPrimitiveOutput
                  << "\n";
        std::cout << std::string( "\t\t" ) << "prefersCompactVertexOutput            = " << !!meshShaderProperties.prefersCompactVertexOutput << "\n";
        std::cout << std::string( "\t\t" ) << "prefersCompactPrimitiveOutput         = " << !!meshShaderProperties.prefersCompactPrimitiveOutput << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_mesh_shader" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMeshShaderPropertiesNV>();
        vk::PhysicalDeviceMeshShaderPropertiesNV const & meshShaderProperties = properties2.get<vk::PhysicalDeviceMeshShaderPropertiesNV>();
        std::cout << std::string( "\t" ) << "MeshShaderPropertiesNV:\n";
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

      if ( vk::su::contains( extensionProperties, "VK_EXT_multi_draw" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMultiDrawPropertiesEXT>();
        vk::PhysicalDeviceMultiDrawPropertiesEXT const & multiDrawProperties = properties2.get<vk::PhysicalDeviceMultiDrawPropertiesEXT>();
        std::cout << std::string( "\t" ) << "MultiDrawProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxMultiDrawCount = " << multiDrawProperties.maxMultiDrawCount << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NVX_multiview_per_view_attributes" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX>();
        vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX const & multiviewPerViewAttributesProperties =
          properties2.get<vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX>();
        std::cout << std::string( "\t" ) << "MultiviewPerViewAttributesProperties:\n";
        std::cout << std::string( "\t\t" ) << "perViewPositionAllComponents = " << !!multiviewPerViewAttributesProperties.perViewPositionAllComponents << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_1 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_multiview" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMultiviewProperties>();
        vk::PhysicalDeviceMultiviewProperties const & multiviewProperties = properties2.get<vk::PhysicalDeviceMultiviewProperties>();
        std::cout << std::string( "\t" ) << "MultiviewProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxMultiviewInstanceIndex = " << multiviewProperties.maxMultiviewInstanceIndex << "\n";
        std::cout << std::string( "\t\t" ) << "maxMultiviewViewCount     = " << multiviewProperties.maxMultiviewViewCount << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_nested_command_buffer" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceNestedCommandBufferPropertiesEXT>();
        vk::PhysicalDeviceNestedCommandBufferPropertiesEXT const & nestedCommandBufferProperties =
          properties2.get<vk::PhysicalDeviceNestedCommandBufferPropertiesEXT>();
        std::cout << std::string( "\t" ) << "NestedCommandBufferProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxCommandBufferNestingLevel = " << nestedCommandBufferProperties.maxCommandBufferNestingLevel << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_opacity_micromap" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceOpacityMicromapPropertiesEXT>();
        vk::PhysicalDeviceOpacityMicromapPropertiesEXT const & opacityMicromapProperties = properties2.get<vk::PhysicalDeviceOpacityMicromapPropertiesEXT>();
        std::cout << std::string( "\t" ) << "OpacityMicromapProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxOpacity2StateSubdivisionLevel = " << opacityMicromapProperties.maxOpacity2StateSubdivisionLevel << "\n";
        std::cout << std::string( "\t\t" ) << "maxOpacity4StateSubdivisionLevel = " << opacityMicromapProperties.maxOpacity4StateSubdivisionLevel << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_optical_flow" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceOpticalFlowPropertiesNV>();
        vk::PhysicalDeviceOpticalFlowPropertiesNV const & opticalFlowProperties = properties2.get<vk::PhysicalDeviceOpticalFlowPropertiesNV>();
        std::cout << std::string( "\t" ) << "OpticalFlowProperties:\n";
        std::cout << std::string( "\t\t" ) << "supportedOutputGridSizes   = " << vk::to_string( opticalFlowProperties.supportedOutputGridSizes ) << "\n";
        std::cout << std::string( "\t\t" ) << "supportedHintGridSizes     = " << vk::to_string( opticalFlowProperties.supportedHintGridSizes ) << "\n";
        std::cout << std::string( "\t\t" ) << "hintSupported              = " << !!opticalFlowProperties.hintSupported << "\n";
        std::cout << std::string( "\t\t" ) << "costSupported              = " << !!opticalFlowProperties.costSupported << "\n";
        std::cout << std::string( "\t\t" ) << "bidirectionalFlowSupported = " << !!opticalFlowProperties.bidirectionalFlowSupported << "\n";
        std::cout << std::string( "\t\t" ) << "globalFlowSupported        = " << !!opticalFlowProperties.globalFlowSupported << "\n";
        std::cout << std::string( "\t\t" ) << "minWidth                   = " << opticalFlowProperties.minWidth << "\n";
        std::cout << std::string( "\t\t" ) << "minHeight                  = " << opticalFlowProperties.minHeight << "\n";
        std::cout << std::string( "\t\t" ) << "maxWidth                   = " << opticalFlowProperties.maxWidth << "\n";
        std::cout << std::string( "\t\t" ) << "maxHeight                  = " << opticalFlowProperties.maxHeight << "\n";
        std::cout << std::string( "\t\t" ) << "maxNumRegionsOfInterest    = " << opticalFlowProperties.maxNumRegionsOfInterest << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_pci_bus_info" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePCIBusInfoPropertiesEXT>();
        vk::PhysicalDevicePCIBusInfoPropertiesEXT const & pciBusInfoProperties = properties2.get<vk::PhysicalDevicePCIBusInfoPropertiesEXT>();
        std::cout << std::string( "\t" ) << "PCIBusInfoProperties:\n";
        std::cout << std::string( "\t\t" ) << "pciDomain   = " << pciBusInfoProperties.pciDomain << "\n";
        std::cout << std::string( "\t\t" ) << "pciBus      = " << pciBusInfoProperties.pciBus << "\n";
        std::cout << std::string( "\t\t" ) << "pciDevice   = " << pciBusInfoProperties.pciDevice << "\n";
        std::cout << std::string( "\t\t" ) << "pciFunction = " << pciBusInfoProperties.pciFunction << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_performance_query" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePerformanceQueryPropertiesKHR>();
        vk::PhysicalDevicePerformanceQueryPropertiesKHR const & performanceQueryProperties = properties2.get<vk::PhysicalDevicePerformanceQueryPropertiesKHR>();
        std::cout << std::string( "\t" ) << "PerformanceQueryProperties:\n";
        std::cout << std::string( "\t\t" ) << "allowCommandBufferQueryCopies = " << !!performanceQueryProperties.allowCommandBufferQueryCopies << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_pipeline_binary" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePipelineBinaryPropertiesKHR>();
        vk::PhysicalDevicePipelineBinaryPropertiesKHR const & pipelineBinaryProperties = properties2.get<vk::PhysicalDevicePipelineBinaryPropertiesKHR>();
        std::cout << std::string( "\t" ) << "PipelineBinaryProperties:\n";
        std::cout << std::string( "\t\t" ) << "pipelineBinaryInternalCache            = " << !!pipelineBinaryProperties.pipelineBinaryInternalCache << "\n";
        std::cout << std::string( "\t\t" ) << "pipelineBinaryInternalCacheControl     = " << !!pipelineBinaryProperties.pipelineBinaryInternalCacheControl
                  << "\n";
        std::cout << std::string( "\t\t" ) << "pipelineBinaryPrefersInternalCache     = " << !!pipelineBinaryProperties.pipelineBinaryPrefersInternalCache
                  << "\n";
        std::cout << std::string( "\t\t" ) << "pipelineBinaryPrecompiledInternalCache = " << !!pipelineBinaryProperties.pipelineBinaryPrecompiledInternalCache
                  << "\n";
        std::cout << std::string( "\t\t" ) << "pipelineBinaryCompressedData           = " << !!pipelineBinaryProperties.pipelineBinaryCompressedData << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_pipeline_robustness" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePipelineRobustnessPropertiesEXT>();
        vk::PhysicalDevicePipelineRobustnessPropertiesEXT const & pipelineRobustnessProperties =
          properties2.get<vk::PhysicalDevicePipelineRobustnessPropertiesEXT>();
        std::cout << std::string( "\t" ) << "PipelineRobustnessProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "defaultRobustnessStorageBuffers = " << vk::to_string( pipelineRobustnessProperties.defaultRobustnessStorageBuffers ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "defaultRobustnessUniformBuffers = " << vk::to_string( pipelineRobustnessProperties.defaultRobustnessUniformBuffers ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "defaultRobustnessVertexInputs   = " << vk::to_string( pipelineRobustnessProperties.defaultRobustnessVertexInputs ) << "\n";
        std::cout << std::string( "\t\t" ) << "defaultRobustnessImages         = " << vk::to_string( pipelineRobustnessProperties.defaultRobustnessImages )
                  << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_1 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_maintenance2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePointClippingProperties>();
        vk::PhysicalDevicePointClippingProperties const & pointClippingProperties = properties2.get<vk::PhysicalDevicePointClippingProperties>();
        std::cout << std::string( "\t" ) << "PointClippingProperties:\n";
        std::cout << std::string( "\t\t" ) << "pointClippingBehavior = " << vk::to_string( pointClippingProperties.pointClippingBehavior ) << "\n";
        std::cout << "\n";
      }

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      if ( vk::su::contains( extensionProperties, "VK_KHR_portability_subset" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePortabilitySubsetPropertiesKHR>();
        vk::PhysicalDevicePortabilitySubsetPropertiesKHR const & portabilitySubsetProperties =
          properties2.get<vk::PhysicalDevicePortabilitySubsetPropertiesKHR>();
        std::cout << std::string( "\t" ) << "PortabilitySubsetProperties:\n";
        std::cout << std::string( "\t\t" ) << "minVertexInputBindingStrideAlignment = " << portabilitySubsetProperties.minVertexInputBindingStrideAlignment
                  << "\n";
        std::cout << "\n";
      }
#endif

      if ( VK_API_VERSION_1_1 <= apiVersion )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceProtectedMemoryProperties>();
        vk::PhysicalDeviceProtectedMemoryProperties const & protectedMemoryProperties = properties2.get<vk::PhysicalDeviceProtectedMemoryProperties>();
        std::cout << std::string( "\t" ) << "ProtectedMemoryProperties:\n";
        std::cout << std::string( "\t\t" ) << "protectedNoFault = " << !!protectedMemoryProperties.protectedNoFault << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_provoking_vertex" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceProvokingVertexPropertiesEXT>();
        vk::PhysicalDeviceProvokingVertexPropertiesEXT const & provokingVertexProperties = properties2.get<vk::PhysicalDeviceProvokingVertexPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ProvokingVertexProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "provokingVertexModePerPipeline                       = " << !!provokingVertexProperties.provokingVertexModePerPipeline << "\n";
        std::cout << std::string( "\t\t" ) << "transformFeedbackPreservesTriangleFanProvokingVertex = "
                  << !!provokingVertexProperties.transformFeedbackPreservesTriangleFanProvokingVertex << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_push_descriptor" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePushDescriptorPropertiesKHR>();
        vk::PhysicalDevicePushDescriptorPropertiesKHR const & pushDescriptorProperties = properties2.get<vk::PhysicalDevicePushDescriptorPropertiesKHR>();
        std::cout << std::string( "\t" ) << "PushDescriptorProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxPushDescriptors = " << pushDescriptorProperties.maxPushDescriptors << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_ARM_render_pass_striped" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRenderPassStripedPropertiesARM>();
        vk::PhysicalDeviceRenderPassStripedPropertiesARM const & renderPassStripedProperties =
          properties2.get<vk::PhysicalDeviceRenderPassStripedPropertiesARM>();
        std::cout << std::string( "\t" ) << "RenderPassStripedPropperties:\n";
        std::cout << std::string( "\t\t" ) << "renderPassStripeGranularity = " << renderPassStripedProperties.renderPassStripeGranularity.width << " x "
                  << renderPassStripedProperties.renderPassStripeGranularity.height << "\n";
        std::cout << std::string( "\t\t" ) << "maxRenderPassStripes        = " << renderPassStripedProperties.maxRenderPassStripes << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_ray_tracing_invocation_reorder" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingInvocationReorderPropertiesNV>();
        vk::PhysicalDeviceRayTracingInvocationReorderPropertiesNV const & rayTracingInvocationReorderProperties =
          properties2.get<vk::PhysicalDeviceRayTracingInvocationReorderPropertiesNV>();
        std::cout << std::string( "\t" ) << "RayTracingInvocationReorderProperties:\n";
        std::cout << std::string( "\t\t" ) << "rayTracingInvocationReorderReorderingHint = "
                  << vk::to_string( rayTracingInvocationReorderProperties.rayTracingInvocationReorderReorderingHint ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_ray_tracing_pipeline" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>();
        vk::PhysicalDeviceRayTracingPipelinePropertiesKHR const & rayTracingPipelineProperties =
          properties2.get<vk::PhysicalDeviceRayTracingPipelinePropertiesKHR>();
        std::cout << std::string( "\t" ) << "RayTracingPipelineProperties:\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupHandleSize              = " << rayTracingPipelineProperties.shaderGroupHandleSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxRayRecursionDepth               = " << rayTracingPipelineProperties.maxRayRecursionDepth << "\n";
        std::cout << std::string( "\t\t" ) << "maxShaderGroupStride               = " << rayTracingPipelineProperties.maxShaderGroupStride << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupBaseAlignment           = " << rayTracingPipelineProperties.shaderGroupBaseAlignment << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupHandleCaptureReplaySize = " << rayTracingPipelineProperties.shaderGroupHandleCaptureReplaySize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxRayDispatchInvocationCount      = " << rayTracingPipelineProperties.maxRayDispatchInvocationCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupHandleAlignment         = " << rayTracingPipelineProperties.shaderGroupHandleAlignment << "\n";
        std::cout << std::string( "\t\t" ) << "maxRayHitAttributeSize             = " << rayTracingPipelineProperties.maxRayHitAttributeSize << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_ray_tracing" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesNV>();
        vk::PhysicalDeviceRayTracingPropertiesNV const & rayTracingProperties = properties2.get<vk::PhysicalDeviceRayTracingPropertiesNV>();
        std::cout << std::string( "\t" ) << "RayTracingProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetAccelerationStructures = " << rayTracingProperties.maxDescriptorSetAccelerationStructures
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxGeometryCount                       = " << rayTracingProperties.maxGeometryCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxInstanceCount                       = " << rayTracingProperties.maxInstanceCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxRecursionDepth                      = " << rayTracingProperties.maxRecursionDepth << "\n";
        std::cout << std::string( "\t\t" ) << "maxShaderGroupStride                   = " << rayTracingProperties.maxShaderGroupStride << "\n";
        std::cout << std::string( "\t\t" ) << "maxTriangleCount                       = " << rayTracingProperties.maxTriangleCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupBaseAlignment               = " << rayTracingProperties.shaderGroupBaseAlignment << "\n";
        std::cout << std::string( "\t\t" ) << "shaderGroupHandleSize                  = " << rayTracingProperties.shaderGroupHandleSize << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_robustness2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRobustness2PropertiesEXT>();
        vk::PhysicalDeviceRobustness2PropertiesEXT const & robustness2Properties = properties2.get<vk::PhysicalDeviceRobustness2PropertiesEXT>();
        std::cout << std::string( "\t" ) << "Robustness2Properties:\n";
        std::cout << std::string( "\t\t" ) << "robustStorageBufferAccessSizeAlignment = " << robustness2Properties.robustStorageBufferAccessSizeAlignment
                  << "\n";
        std::cout << std::string( "\t\t" ) << "robustUniformBufferAccessSizeAlignment = " << robustness2Properties.robustUniformBufferAccessSizeAlignment
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_sample_locations" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSampleLocationsPropertiesEXT>();
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
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT>();
        vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT const & samplerFilterMinmaxProperties =
          properties2.get<vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT>();
        std::cout << std::string( "\t" ) << "SamplerFilterMinmaxProperties:\n";
        std::cout << std::string( "\t\t" ) << "filterMinmaxImageComponentMapping  = " << !!samplerFilterMinmaxProperties.filterMinmaxImageComponentMapping
                  << "\n";
        std::cout << std::string( "\t\t" ) << "filterMinmaxSingleComponentFormats = " << !!samplerFilterMinmaxProperties.filterMinmaxSingleComponentFormats
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_ARM_scheduling_controls" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSchedulingControlsPropertiesARM>();
        vk::PhysicalDeviceSchedulingControlsPropertiesARM const & schedulingControlsProperties =
          properties2.get<vk::PhysicalDeviceSchedulingControlsPropertiesARM>();
        std::cout << std::string( "\t" ) << "SchedulingControlsProperties:\n";
        std::cout << std::string( "\t\t" ) << "schedulingControlsFlags = " << vk::to_string( schedulingControlsProperties.schedulingControlsFlags ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_ARM_shader_core_builtins" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderCoreBuiltinsPropertiesARM>();
        vk::PhysicalDeviceShaderCoreBuiltinsPropertiesARM const & shaderCoreBuiltinsProperties =
          properties2.get<vk::PhysicalDeviceShaderCoreBuiltinsPropertiesARM>();
        std::cout << std::string( "\t" ) << "ShaderCoreBuiltinsProperties:\n";
        std::cout << std::string( "\t\t" ) << "shaderCoreMask     = " << shaderCoreBuiltinsProperties.shaderCoreMask << "\n";
        std::cout << std::string( "\t\t" ) << "shaderCoreCount    = " << shaderCoreBuiltinsProperties.shaderCoreCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderWarpsPerCore = " << shaderCoreBuiltinsProperties.shaderWarpsPerCore << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_shader_core_properties2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderCoreProperties2AMD>();
        vk::PhysicalDeviceShaderCoreProperties2AMD const & shaderCoreProperties2 = properties2.get<vk::PhysicalDeviceShaderCoreProperties2AMD>();
        std::cout << std::string( "\t" ) << "ShaderCoreProperties2:\n";
        std::cout << std::string( "\t\t" ) << "activeComputeUnitCount  = " << shaderCoreProperties2.activeComputeUnitCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderCoreFeatures      = " << vk::to_string( shaderCoreProperties2.shaderCoreFeatures ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_shader_core_properties2" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderCorePropertiesAMD>();
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

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      if ( vk::su::contains( extensionProperties, "VK_AMDX_shader_enqueue" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderEnqueuePropertiesAMDX>();
        vk::PhysicalDeviceShaderEnqueuePropertiesAMDX const & shaderEnqueueProperties = properties2.get<vk::PhysicalDeviceShaderEnqueuePropertiesAMDX>();
        std::cout << std::string( "\t" ) << "ShaderModuleIdentifierProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxExecutionGraphDepth                 = " << shaderEnqueueProperties.maxExecutionGraphDepth << "\n";
        std::cout << std::string( "\t\t" ) << "maxExecutionGraphShaderOutputNodes     = " << shaderEnqueueProperties.maxExecutionGraphShaderOutputNodes << "\n";
        std::cout << std::string( "\t\t" ) << "maxExecutionGraphShaderPayloadSize     = " << shaderEnqueueProperties.maxExecutionGraphShaderPayloadSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxExecutionGraphShaderPayloadCount    = " << shaderEnqueueProperties.maxExecutionGraphShaderPayloadCount
                  << "\n";
        std::cout << std::string( "\t\t" ) << "executionGraphDispatchAddressAlignment = " << shaderEnqueueProperties.executionGraphDispatchAddressAlignment
                  << "\n";
        std::cout << "\n";
      }
#endif

      if ( ( VK_API_VERSION_1_3 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_shader_integer_dot_product" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderIntegerDotProductProperties>();
        vk::PhysicalDeviceShaderIntegerDotProductProperties const & shaderIntegerDotProductProperties =
          properties2.get<vk::PhysicalDeviceShaderIntegerDotProductProperties>();
        std::cout << std::string( "\t" ) << "ShaderIntegerDotProductProperties:\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitUnsignedAccelerated                                      = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct8BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitSignedAccelerated                                        = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct8BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitMixedSignednessAccelerated                               = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct8BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedUnsignedAccelerated                              = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct4x8BitPackedUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedSignedAccelerated                                = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct4x8BitPackedSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedMixedSignednessAccelerated                       = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct4x8BitPackedMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitUnsignedAccelerated                                     = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct16BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitSignedAccelerated                                       = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct16BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitMixedSignednessAccelerated                              = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct16BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitUnsignedAccelerated                                     = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct32BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitSignedAccelerated                                       = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct32BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitMixedSignednessAccelerated                              = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct32BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitUnsignedAccelerated                                     = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct64BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitSignedAccelerated                                       = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct64BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitMixedSignednessAccelerated                              = "
                  << !!shaderIntegerDotProductProperties.integerDotProduct64BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitUnsignedAccelerated                = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitSignedAccelerated                  = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated         = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated        = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated          = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitUnsignedAccelerated               = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitSignedAccelerated                 = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated        = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitUnsignedAccelerated               = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitSignedAccelerated                 = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated        = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitUnsignedAccelerated               = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitSignedAccelerated                 = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated        = "
                  << !!shaderIntegerDotProductProperties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_shader_module_identifier" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderModuleIdentifierPropertiesEXT>();
        vk::PhysicalDeviceShaderModuleIdentifierPropertiesEXT const & shaderModuleIdentifierProperties =
          properties2.get<vk::PhysicalDeviceShaderModuleIdentifierPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ShaderModuleIdentifierProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "shaderModuleIdentifierAlgorithmUUID = " << vk::su::UUID( shaderModuleIdentifierProperties.shaderModuleIdentifierAlgorithmUUID ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_shader_object" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderObjectPropertiesEXT>();
        vk::PhysicalDeviceShaderObjectPropertiesEXT const & shaderObjectProperties = properties2.get<vk::PhysicalDeviceShaderObjectPropertiesEXT>();
        std::cout << std::string( "\t" ) << "ShaderObjectProperties:\n";
        std::cout << std::string( "\t\t" ) << "shaderBinaryUUID    = " << vk::su::UUID( shaderObjectProperties.shaderBinaryUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "shaderBinaryVersion = " << shaderObjectProperties.shaderBinaryVersion << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_sm_builtins" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV>();
        vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV const & shaderSMBuiltinsProperties = properties2.get<vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV>();
        std::cout << std::string( "\t" ) << "ShaderSMBuiltinsProperties:\n";
        std::cout << std::string( "\t\t" ) << "shaderSMCount    = " << shaderSMBuiltinsProperties.shaderSMCount << "\n";
        std::cout << std::string( "\t\t" ) << "shaderWarpsPerSM = " << shaderSMBuiltinsProperties.shaderWarpsPerSM << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_shader_tile_image" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShaderTileImagePropertiesEXT>();
        vk::PhysicalDeviceShaderTileImagePropertiesEXT const & shaderTileImageProperties = properties2.get<vk::PhysicalDeviceShaderTileImagePropertiesEXT>();
        std::cout << std::string( "\t" ) << "ShaderTileImageProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "shaderTileImageCoherentReadAccelerated           = " << !!shaderTileImageProperties.shaderTileImageCoherentReadAccelerated << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderTileImageReadSampleFromPixelRateInvocation = " << !!shaderTileImageProperties.shaderTileImageReadSampleFromPixelRateInvocation
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderTileImageReadFromHelperInvocation          = " << !!shaderTileImageProperties.shaderTileImageReadFromHelperInvocation << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shading_rate_image" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceShadingRateImagePropertiesNV>();
        vk::PhysicalDeviceShadingRateImagePropertiesNV const & shadingRageImageProperties = properties2.get<vk::PhysicalDeviceShadingRateImagePropertiesNV>();
        std::cout << std::string( "\t" ) << "ShadingRateImageProperties:\n";
        std::cout << std::string( "\t\t" ) << "shadingRateMaxCoarseSamples = " << shadingRageImageProperties.shadingRateMaxCoarseSamples << "\n";
        std::cout << std::string( "\t\t" ) << "shadingRatePaletteSize      = " << shadingRageImageProperties.shadingRatePaletteSize << "\n";
        std::cout << std::string( "\t\t" ) << "shadingRateTexelSize        = " << shadingRageImageProperties.shadingRateTexelSize.width << " x "
                  << shadingRageImageProperties.shadingRateTexelSize.height << "\n";
        std::cout << "\n";
      }

      if ( VK_API_VERSION_1_1 <= apiVersion )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSubgroupProperties>();
        vk::PhysicalDeviceSubgroupProperties const & subgroupProperties = properties2.get<vk::PhysicalDeviceSubgroupProperties>();
        std::cout << std::string( "\t" ) << "SubgroupProperties:\n";
        std::cout << std::string( "\t\t" ) << "quadOperationsInAllStages = " << !!subgroupProperties.quadOperationsInAllStages << "\n";
        std::cout << std::string( "\t\t" ) << "subgroupSize              = " << subgroupProperties.subgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "supportedOperations       = " << vk::to_string( subgroupProperties.supportedOperations ) << "\n";
        std::cout << std::string( "\t\t" ) << "supportedStages           = " << vk::to_string( subgroupProperties.supportedStages ) << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_3 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_EXT_subgroup_size_control" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSubgroupSizeControlProperties>();
        vk::PhysicalDeviceSubgroupSizeControlProperties const & subgroupSizeControlProperties =
          properties2.get<vk::PhysicalDeviceSubgroupSizeControlProperties>();
        std::cout << std::string( "\t" ) << "SubgroupSizeControlProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxComputeWorkgroupSubgroups = " << subgroupSizeControlProperties.maxComputeWorkgroupSubgroups << "\n";
        std::cout << std::string( "\t\t" ) << "maxSubgroupSize              = " << subgroupSizeControlProperties.maxSubgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "minSubgroupSize              = " << subgroupSizeControlProperties.minSubgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "requiredSubgroupSizeStages   = " << vk::to_string( subgroupSizeControlProperties.requiredSubgroupSizeStages )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_HUAWEI_subpass_shading" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI>();
        vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI const & subpassShadingProperties = properties2.get<vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI>();
        std::cout << std::string( "\t" ) << "SubpassShadingProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "maxSubpassShadingWorkgroupSizeAspectRatio = " << subpassShadingProperties.maxSubpassShadingWorkgroupSizeAspectRatio << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_2 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_KHR_timeline_semaphore" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceTimelineSemaphoreProperties>();
        vk::PhysicalDeviceTimelineSemaphoreProperties const & timelineSemaphoreProperties = properties2.get<vk::PhysicalDeviceTimelineSemaphoreProperties>();
        std::cout << std::string( "\t" ) << "TimelineSemaphoreProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxTimelineSemaphoreValueDifference = " << timelineSemaphoreProperties.maxTimelineSemaphoreValueDifference
                  << "\n";
        std::cout << "\n";
      }

      if ( ( VK_API_VERSION_1_3 <= apiVersion ) || vk::su::contains( extensionProperties, "VK_EXT_texel_buffer_alignment" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceTexelBufferAlignmentProperties>();
        vk::PhysicalDeviceTexelBufferAlignmentProperties const & texelBufferAlignmentProperties =
          properties2.get<vk::PhysicalDeviceTexelBufferAlignmentProperties>();
        std::cout << std::string( "\t" ) << "TexelBufferAlignmentProperties:\n";
        std::cout << std::string( "\t\t" )
                  << "storageTexelBufferOffsetAlignmentBytes       = " << texelBufferAlignmentProperties.storageTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" )
                  << "storageTexelBufferOffsetSingleTexelAlignment = " << !!texelBufferAlignmentProperties.storageTexelBufferOffsetSingleTexelAlignment << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformTexelBufferOffsetAlignmentBytes       = " << texelBufferAlignmentProperties.uniformTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" )
                  << "uniformTexelBufferOffsetSingleTexelAlignment = " << !!texelBufferAlignmentProperties.uniformTexelBufferOffsetSingleTexelAlignment << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_transform_feedback" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceTransformFeedbackPropertiesEXT>();
        vk::PhysicalDeviceTransformFeedbackPropertiesEXT const & transformFeedbackProperties =
          properties2.get<vk::PhysicalDeviceTransformFeedbackPropertiesEXT>();
        std::cout << std::string( "\t" ) << "TransformFeedbackProperties:\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBufferDataSize         = " << transformFeedbackProperties.maxTransformFeedbackBufferDataSize
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxTransformFeedbackBufferDataStride       = " << transformFeedbackProperties.maxTransformFeedbackBufferDataStride << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBuffers                = " << transformFeedbackProperties.maxTransformFeedbackBuffers
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackBufferSize             = " << transformFeedbackProperties.maxTransformFeedbackBufferSize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackStreamDataSize         = " << transformFeedbackProperties.maxTransformFeedbackStreamDataSize
                  << "\n";
        std::cout << std::string( "\t\t" ) << "maxTransformFeedbackStreams                = " << transformFeedbackProperties.maxTransformFeedbackStreams
                  << "\n";
        std::cout << std::string( "\t\t" ) << "transformFeedbackDraw                      = " << !!transformFeedbackProperties.transformFeedbackDraw << "\n";
        std::cout << std::string( "\t\t" ) << "transformFeedbackQueries                   = " << !!transformFeedbackProperties.transformFeedbackQueries << "\n";
        std::cout << std::string( "\t\t" )
                  << "transformFeedbackRasterizationStreamSelect = " << !!transformFeedbackProperties.transformFeedbackRasterizationStreamSelect << "\n";
        std::cout << std::string( "\t\t" )
                  << "transformFeedbackStreamsLinesTriangles     = " << !!transformFeedbackProperties.transformFeedbackStreamsLinesTriangles << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_vertex_attribute_divisor" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
        vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT const & vertexAttributeDivisorProperties =
          properties2.get<vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
        std::cout << std::string( "\t" ) << "VertexAttributeDivisorPropertiesEXT:\n";
        std::cout << std::string( "\t\t" ) << "maxVertexAttribDivisor = " << vertexAttributeDivisorProperties.maxVertexAttribDivisor << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_vertex_attribute_divisor" ) )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVertexAttributeDivisorPropertiesKHR>();
        vk::PhysicalDeviceVertexAttributeDivisorPropertiesKHR const & vertexAttributeDivisorProperties =
          properties2.get<vk::PhysicalDeviceVertexAttributeDivisorPropertiesKHR>();
        std::cout << std::string( "\t" ) << "VertexAttributeDivisorPropertiesKHR:\n";
        std::cout << std::string( "\t\t" ) << "maxVertexAttribDivisor       = " << vertexAttributeDivisorProperties.maxVertexAttribDivisor << "\n";
        std::cout << std::string( "\t\t" ) << "supportsNonZeroFirstInstance = " << !!vertexAttributeDivisorProperties.supportsNonZeroFirstInstance << "\n";
        std::cout << "\n";
      }

      if ( VK_API_VERSION_1_2 <= apiVersion )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan11Properties>();
        vk::PhysicalDeviceVulkan11Properties const & vulkan11Properties = properties2.get<vk::PhysicalDeviceVulkan11Properties>();
        std::cout << std::string( "\t" ) << "Vulkan11Properties:\n";
        std::cout << std::string( "\t\t" ) << "deviceUUID                        = " << vk::su::UUID( vulkan11Properties.deviceUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "driverUUID                        = " << vk::su::UUID( vulkan11Properties.driverUUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "deviceLUID                        = " << vk::su::LUID( vulkan11Properties.deviceLUID ) << "\n";
        std::cout << std::string( "\t\t" ) << "deviceNodeMask                    = " << vulkan11Properties.deviceNodeMask << "\n";
        std::cout << std::string( "\t\t" ) << "deviceLUIDValid                   = " << !!vulkan11Properties.deviceLUIDValid << "\n";
        std::cout << std::string( "\t\t" ) << "subgroupSize                      = " << vulkan11Properties.subgroupSize << "\n";
        std::cout << std::string( "\t\t" ) << "subgroupSupportedStages           = " << vk::to_string( vulkan11Properties.subgroupSupportedStages ) << "\n";
        std::cout << std::string( "\t\t" ) << "subgroupSupportedOperations       = " << vk::to_string( vulkan11Properties.subgroupSupportedOperations ) << "\n";
        std::cout << std::string( "\t\t" ) << "subgroupQuadOperationsInAllStages = " << !!vulkan11Properties.subgroupQuadOperationsInAllStages << "\n";
        std::cout << std::string( "\t\t" ) << "pointClippingBehavior             = " << vk::to_string( vulkan11Properties.pointClippingBehavior ) << "\n";
        std::cout << std::string( "\t\t" ) << "maxMultiviewViewCount             = " << vulkan11Properties.maxMultiviewViewCount << "\n";
        std::cout << std::string( "\t\t" ) << "maxMultiviewInstanceIndex         = " << vulkan11Properties.maxMultiviewInstanceIndex << "\n";
        std::cout << std::string( "\t\t" ) << "protectedNoFault                  = " << !!vulkan11Properties.protectedNoFault << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerSetDescriptors              = " << vulkan11Properties.maxPerSetDescriptors << "\n";
        std::cout << std::string( "\t\t" ) << "maxMemoryAllocationSize           = " << vulkan11Properties.maxMemoryAllocationSize << "\n";
        std::cout << "\n";
      }

      if ( VK_API_VERSION_1_2 <= apiVersion )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();
        vk::PhysicalDeviceVulkan12Properties const & vulkan12Properties = properties2.get<vk::PhysicalDeviceVulkan12Properties>();
        std::cout << std::string( "\t" ) << "Vulkan12Properties:\n";
        std::cout << std::string( "\t\t" ) << "driverID                                             = " << vk::to_string( vulkan12Properties.driverID ) << "\n";
        std::cout << std::string( "\t\t" ) << "driverName                                           = " << vulkan12Properties.driverName << "\n";
        std::cout << std::string( "\t\t" ) << "driverInfo                                           = " << vulkan12Properties.driverInfo << "\n";
        std::cout << std::string( "\t\t" )
                  << "conformanceVersion                                   = " << static_cast<size_t>( vulkan12Properties.conformanceVersion.major ) << "."
                  << static_cast<size_t>( vulkan12Properties.conformanceVersion.minor ) << "."
                  << static_cast<size_t>( vulkan12Properties.conformanceVersion.subminor ) << "."
                  << static_cast<size_t>( vulkan12Properties.conformanceVersion.patch ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "denormBehaviorIndependence                           = " << vk::to_string( vulkan12Properties.denormBehaviorIndependence ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "roundingModeIndependence                             = " << vk::to_string( vulkan12Properties.roundingModeIndependence ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderSignedZeroInfNanPreserveFloat16                = " << !!vulkan12Properties.shaderSignedZeroInfNanPreserveFloat16 << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderSignedZeroInfNanPreserveFloat32                = " << !!vulkan12Properties.shaderSignedZeroInfNanPreserveFloat32 << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderSignedZeroInfNanPreserveFloat64                = " << !!vulkan12Properties.shaderSignedZeroInfNanPreserveFloat64 << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat16                          = " << !!vulkan12Properties.shaderDenormPreserveFloat16
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat32                          = " << !!vulkan12Properties.shaderDenormPreserveFloat32
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormPreserveFloat64                          = " << !!vulkan12Properties.shaderDenormPreserveFloat64
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat16                       = " << !!vulkan12Properties.shaderDenormFlushToZeroFloat16
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat32                       = " << !!vulkan12Properties.shaderDenormFlushToZeroFloat32
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderDenormFlushToZeroFloat64                       = " << !!vulkan12Properties.shaderDenormFlushToZeroFloat64
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat16                         = " << !!vulkan12Properties.shaderRoundingModeRTEFloat16
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat32                         = " << !!vulkan12Properties.shaderRoundingModeRTEFloat32
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTEFloat64                         = " << !!vulkan12Properties.shaderRoundingModeRTEFloat64
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat16                         = " << !!vulkan12Properties.shaderRoundingModeRTZFloat16
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat32                         = " << !!vulkan12Properties.shaderRoundingModeRTZFloat32
                  << "\n";
        std::cout << std::string( "\t\t" ) << "shaderRoundingModeRTZFloat64                         = " << !!vulkan12Properties.shaderRoundingModeRTZFloat64
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxUpdateAfterBindDescriptorsInAllPools              = " << vulkan12Properties.maxUpdateAfterBindDescriptorsInAllPools << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderUniformBufferArrayNonUniformIndexingNative     = " << !!vulkan12Properties.shaderUniformBufferArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderSampledImageArrayNonUniformIndexingNative      = " << !!vulkan12Properties.shaderSampledImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderStorageBufferArrayNonUniformIndexingNative     = " << !!vulkan12Properties.shaderStorageBufferArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderStorageImageArrayNonUniformIndexingNative      = " << !!vulkan12Properties.shaderStorageImageArrayNonUniformIndexingNative << "\n";
        std::cout << std::string( "\t\t" )
                  << "shaderInputAttachmentArrayNonUniformIndexingNative   = " << !!vulkan12Properties.shaderInputAttachmentArrayNonUniformIndexingNative
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "robustBufferAccessUpdateAfterBind                    = " << !!vulkan12Properties.robustBufferAccessUpdateAfterBind << "\n";
        std::cout << std::string( "\t\t" ) << "quadDivergentImplicitLod                             = " << !!vulkan12Properties.quadDivergentImplicitLod
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindSamplers         = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindSamplers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindUniformBuffers   = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindUniformBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindStorageBuffers   = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindStorageBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindSampledImages    = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindSampledImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindStorageImages    = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindStorageImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageDescriptorUpdateAfterBindInputAttachments = " << vulkan12Properties.maxPerStageDescriptorUpdateAfterBindInputAttachments
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxPerStageUpdateAfterBindResources                  = " << vulkan12Properties.maxPerStageUpdateAfterBindResources << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSamplers              = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindSamplers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindUniformBuffers        = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindUniformBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageBuffers        = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindStorageBuffers << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic
                  << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindSampledImages         = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindSampledImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindStorageImages         = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindStorageImages << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxDescriptorSetUpdateAfterBindInputAttachments      = " << vulkan12Properties.maxDescriptorSetUpdateAfterBindInputAttachments << "\n";
        std::cout << std::string( "\t\t" )
                  << "supportedDepthResolveModes                           = " << vk::to_string( vulkan12Properties.supportedDepthResolveModes ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "supportedStencilResolveModes                         = " << vk::to_string( vulkan12Properties.supportedStencilResolveModes ) << "\n";
        std::cout << std::string( "\t\t" ) << "independentResolveNone                               = " << !!vulkan12Properties.independentResolveNone << "\n";
        std::cout << std::string( "\t\t" ) << "independentResolve                                   = " << !!vulkan12Properties.independentResolve << "\n";
        std::cout << std::string( "\t\t" )
                  << "filterMinmaxSingleComponentFormats                   = " << !!vulkan12Properties.filterMinmaxSingleComponentFormats << "\n";
        std::cout << std::string( "\t\t" )
                  << "filterMinmaxImageComponentMapping                    = " << !!vulkan12Properties.filterMinmaxImageComponentMapping << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxTimelineSemaphoreValueDifference                  = " << vulkan12Properties.maxTimelineSemaphoreValueDifference << "\n";
        std::cout << std::string( "\t\t" )
                  << "framebufferIntegerColorSampleCounts                  = " << vk::to_string( vulkan12Properties.framebufferIntegerColorSampleCounts )
                  << "\n";
        std::cout << "\n";
      }

      if ( VK_API_VERSION_1_3 <= apiVersion )
      {
        auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan13Properties>();
        vk::PhysicalDeviceVulkan13Properties const & vulkan13Properties = properties2.get<vk::PhysicalDeviceVulkan13Properties>();
        std::cout << std::string( "\t" ) << "Vulkan13Properties:\n";
        std::cout << std::string( "\t\t" )
                  << "minSubgroupSize                                                               = " << vulkan13Properties.minSubgroupSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxSubgroupSize                                                               = " << vulkan13Properties.maxSubgroupSize << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxComputeWorkgroupSubgroups                                                  = " << vulkan13Properties.maxComputeWorkgroupSubgroups
                  << "\n";
        std::cout << std::string( "\t\t" ) << "requiredSubgroupSizeStages                                                    = "
                  << vk::to_string( vulkan13Properties.requiredSubgroupSizeStages ) << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxInlineUniformBlockSize                                                     = " << vulkan13Properties.maxInlineUniformBlockSize << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorInlineUniformBlocks                                      = "
                  << vulkan13Properties.maxPerStageDescriptorInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" ) << "maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks                       = "
                  << vulkan13Properties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetInlineUniformBlocks                                           = "
                  << vulkan13Properties.maxDescriptorSetInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" ) << "maxDescriptorSetUpdateAfterBindInlineUniformBlocks                            = "
                  << vulkan13Properties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxInlineUniformTotalSize                                                     = " << vulkan13Properties.maxInlineUniformTotalSize << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitUnsignedAccelerated                                      = "
                  << !!vulkan13Properties.integerDotProduct8BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitSignedAccelerated                                        = "
                  << !!vulkan13Properties.integerDotProduct8BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct8BitMixedSignednessAccelerated                               = "
                  << !!vulkan13Properties.integerDotProduct8BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedUnsignedAccelerated                              = "
                  << !!vulkan13Properties.integerDotProduct4x8BitPackedUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedSignedAccelerated                                = "
                  << !!vulkan13Properties.integerDotProduct4x8BitPackedSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct4x8BitPackedMixedSignednessAccelerated                       = "
                  << !!vulkan13Properties.integerDotProduct4x8BitPackedMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitUnsignedAccelerated                                     = "
                  << !!vulkan13Properties.integerDotProduct16BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitSignedAccelerated                                       = "
                  << !!vulkan13Properties.integerDotProduct16BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct16BitMixedSignednessAccelerated                              = "
                  << !!vulkan13Properties.integerDotProduct16BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitUnsignedAccelerated                                     = "
                  << !!vulkan13Properties.integerDotProduct32BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitSignedAccelerated                                       = "
                  << !!vulkan13Properties.integerDotProduct32BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct32BitMixedSignednessAccelerated                              = "
                  << !!vulkan13Properties.integerDotProduct32BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitUnsignedAccelerated                                     = "
                  << !!vulkan13Properties.integerDotProduct64BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitSignedAccelerated                                       = "
                  << !!vulkan13Properties.integerDotProduct64BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProduct64BitMixedSignednessAccelerated                              = "
                  << !!vulkan13Properties.integerDotProduct64BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitUnsignedAccelerated                = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitSignedAccelerated                  = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating8BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated         = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated        = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated          = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitUnsignedAccelerated               = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitSignedAccelerated                 = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating16BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated        = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitUnsignedAccelerated               = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitSignedAccelerated                 = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating32BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated        = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitUnsignedAccelerated               = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitSignedAccelerated                 = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating64BitSignedAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated        = "
                  << !!vulkan13Properties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated << "\n";
        std::cout << std::string( "\t\t" ) << "storageTexelBufferOffsetAlignmentBytes                                        = "
                  << vulkan13Properties.storageTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" ) << "storageTexelBufferOffsetSingleTexelAlignment                                  = "
                  << !!vulkan13Properties.storageTexelBufferOffsetSingleTexelAlignment << "\n";
        std::cout << std::string( "\t\t" ) << "uniformTexelBufferOffsetAlignmentBytes                                        = "
                  << vulkan13Properties.uniformTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << std::string( "\t\t" ) << "uniformTexelBufferOffsetSingleTexelAlignment                                  = "
                  << !!vulkan13Properties.uniformTexelBufferOffsetSingleTexelAlignment << "\n";
        std::cout << std::string( "\t\t" )
                  << "maxBufferSize                                                                 = " << vulkan13Properties.maxBufferSize << "\n";
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
