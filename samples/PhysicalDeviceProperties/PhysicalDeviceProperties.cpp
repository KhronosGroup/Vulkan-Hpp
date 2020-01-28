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
#include "vulkan/vulkan.hpp"
#include <iomanip>
#include <sstream>
#include <vector>

static char const* AppName = "PhysicalDeviceProperties";
static char const* EngineName = "Vulkan.hpp";

std::string decodeAPIVersion(uint32_t apiVersion)
{
  return std::to_string(VK_VERSION_MAJOR(apiVersion)) + "." + std::to_string(VK_VERSION_MINOR(apiVersion)) + "." + std::to_string(VK_VERSION_PATCH(apiVersion));
}

std::string decodeDriverVersion(uint32_t driverVersion, uint32_t vendorID)
{
  switch (vendorID)
  {
    case 4318:
      return std::to_string((driverVersion >> 22) & 0x3FF) + "." + std::to_string((driverVersion >> 14) & 0xFF) + "." + std::to_string((driverVersion >> 6) & 0xFF) + "." + std::to_string(driverVersion & 0x3F);
    case 0x8086:
      return std::to_string((driverVersion >> 14) & 0x3FFFF) + "." + std::to_string((driverVersion & 0x3FFF));
    default:
      return decodeAPIVersion(driverVersion);
  }
}

std::string decodeVendorID(uint32_t vendorID)
{
  // below 0x10000 are the PCI vendor IDs (https://pcisig.com/membership/member-companies)
  if (vendorID < 0x10000)
  {
    switch (vendorID)
    {
    case 0x1022:
      return "Advanced Micro Devices";
    case 0x10DE:
      return "NVidia Corporation";
    case 0x8086:
      return "Intel Corporation";
    default:
      return std::to_string(vendorID);
    }
  }
  else
  {
    // above 0x10000 should be vkVendorIDs
    return vk::to_string(vk::VendorId(vendorID));
  }
}

namespace vk
{
  namespace su
  {
    struct LUID
    {
    public:
      LUID(uint8_t const data[VK_LUID_SIZE])
      {
        memcpy(m_data, data, VK_LUID_SIZE * sizeof(uint8_t));
      }

      uint8_t m_data[VK_LUID_SIZE];
    };

    std::ostream& operator<<(std::ostream& os, LUID const& uuid)
    {
      os << std::setfill('0') << std::hex;
      for (int j = 0; j < VK_LUID_SIZE; ++j)
      {
        os << std::setw(2) << static_cast<uint32_t>(uuid.m_data[j]);
        if (j == 3 || j == 5)
        {
          std::cout << '-';
        }
      }
      os << std::setfill(' ') << std::dec;
      return os;
    }
  }
}

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName);
#if !defined(NDEBUG)
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger(instance);
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for (size_t i=0 ; i<physicalDevices.size() ; i++)
    {
      // some properties are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties = physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";
      auto properties2 = physicalDevices[i].getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT,
                                                           vk::PhysicalDeviceConservativeRasterizationPropertiesEXT, vk::PhysicalDeviceCooperativeMatrixPropertiesNV,
                                                           vk::PhysicalDeviceDepthStencilResolvePropertiesKHR, vk::PhysicalDeviceDescriptorIndexingPropertiesEXT,
                                                           vk::PhysicalDeviceDiscardRectanglePropertiesEXT, vk::PhysicalDeviceDriverPropertiesKHR,
                                                           vk::PhysicalDeviceExternalMemoryHostPropertiesEXT, vk::PhysicalDeviceFloatControlsPropertiesKHR,
                                                           vk::PhysicalDeviceFragmentDensityMapPropertiesEXT, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceInlineUniformBlockPropertiesEXT,
                                                           vk::PhysicalDeviceLineRasterizationPropertiesEXT, vk::PhysicalDeviceMaintenance3Properties, vk::PhysicalDeviceMeshShaderPropertiesNV,
                                                           vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX, vk::PhysicalDeviceMultiviewProperties,
                                                           vk::PhysicalDevicePCIBusInfoPropertiesEXT, vk::PhysicalDevicePointClippingProperties, vk::PhysicalDeviceProtectedMemoryProperties,
                                                           vk::PhysicalDevicePushDescriptorPropertiesKHR, vk::PhysicalDeviceRayTracingPropertiesNV, vk::PhysicalDeviceSampleLocationsPropertiesEXT,
                                                           vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT, vk::PhysicalDeviceShaderCoreProperties2AMD,
                                                           vk::PhysicalDeviceShaderCorePropertiesAMD, vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV,
                                                           vk::PhysicalDeviceShadingRateImagePropertiesNV, vk::PhysicalDeviceSubgroupProperties,
                                                           vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT, vk::PhysicalDeviceTimelineSemaphorePropertiesKHR,
                                                           vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT, vk::PhysicalDeviceTransformFeedbackPropertiesEXT,
                                                           vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
      vk::PhysicalDeviceProperties const& properties = properties2.get<vk::PhysicalDeviceProperties2>().properties;
      std::cout << "\t" << "Properties:\n";
      std::cout << "\t\t" << "apiVersion        = " << decodeAPIVersion(properties.apiVersion) << "\n";
      std::cout << "\t\t" << "driverVersion     = " << decodeDriverVersion(properties.driverVersion, properties.vendorID) << "\n";
      std::cout << "\t\t" << "vendorID          = " << decodeVendorID(properties.vendorID) << "\n";
      std::cout << "\t\t" << "deviceID          = " << properties.deviceID << "\n";
      std::cout << "\t\t" << "deviceType        = " << vk::to_string(properties.deviceType) << "\n";
      std::cout << "\t\t" << "deviceName        = " << properties.deviceName << "\n";
      std::cout << "\t\t" << "pipelineCacheUUID = " << vk::su::UUID(properties.pipelineCacheUUID) << "\n";
      std::cout << "\t\t" << "limits:\n";
      std::cout << "\t\t\t" << "bufferImageGranularity                          = " << properties.limits.bufferImageGranularity << "\n";
      std::cout << "\t\t\t" << "discreteQueuePriorities                         = " << properties.limits.discreteQueuePriorities << "\n";
      std::cout << "\t\t\t" << "framebufferColorSampleCounts                    = " << vk::to_string(properties.limits.framebufferColorSampleCounts) << "\n";
      std::cout << "\t\t\t" << "framebufferDepthSampleCounts                    = " << vk::to_string(properties.limits.framebufferDepthSampleCounts) << "\n";
      std::cout << "\t\t\t" << "framebufferNoAttachmentsSampleCounts            = " << vk::to_string(properties.limits.framebufferNoAttachmentsSampleCounts) << "\n";
      std::cout << "\t\t\t" << "framebufferStencilSampleCounts                  = " << vk::to_string(properties.limits.framebufferStencilSampleCounts) << "\n";
      std::cout << "\t\t\t" << "lineWidthGranularity                            = " << properties.limits.lineWidthGranularity << "\n";
      std::cout << "\t\t\t" << "lineWidthRange                                  = " << "[" << properties.limits.lineWidthRange[0] << ", " << properties.limits.lineWidthRange[1] << "]" << "\n";
      std::cout << "\t\t\t" << "maxBoundDescriptorSets                          = " << properties.limits.maxBoundDescriptorSets << "\n";
      std::cout << "\t\t\t" << "maxClipDistances                                = " << properties.limits.maxClipDistances << "\n";
      std::cout << "\t\t\t" << "maxColorAttachments                             = " << properties.limits.maxColorAttachments << "\n";
      std::cout << "\t\t\t" << "maxCombinedClipAndCullDistances                 = " << properties.limits.maxCombinedClipAndCullDistances << "\n";
      std::cout << "\t\t\t" << "maxComputeSharedMemorySize                      = " << properties.limits.maxComputeSharedMemorySize << "\n";
      std::cout << "\t\t\t" << "maxComputeWorkGroupCount                        = " << "[" << properties.limits.maxComputeWorkGroupCount[0] << ", " << properties.limits.maxComputeWorkGroupCount[1] << ", " << properties.limits.maxComputeWorkGroupCount[2] << "]" << "\n";
      std::cout << "\t\t\t" << "maxComputeWorkGroupInvocations                  = " << properties.limits.maxComputeWorkGroupInvocations << "\n";
      std::cout << "\t\t\t" << "maxComputeWorkGroupSize                         = " << "[" << properties.limits.maxComputeWorkGroupSize[0] << ", " << properties.limits.maxComputeWorkGroupSize[1] << ", " << properties.limits.maxComputeWorkGroupSize[2] << "]" << "\n";
      std::cout << "\t\t\t" << "maxCullDistances                                = " << properties.limits.maxCullDistances << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetInputAttachments                = " << properties.limits.maxDescriptorSetInputAttachments << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetSampledImages                   = " << properties.limits.maxDescriptorSetSampledImages << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetSamplers                        = " << properties.limits.maxDescriptorSetSamplers << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetStorageBuffers                  = " << properties.limits.maxDescriptorSetStorageBuffers << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetStorageBuffersDynamic           = " << properties.limits.maxDescriptorSetStorageBuffersDynamic << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetStorageImages                   = " << properties.limits.maxDescriptorSetStorageImages << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetUniformBuffers                  = " << properties.limits.maxDescriptorSetUniformBuffers << "\n";
      std::cout << "\t\t\t" << "maxDescriptorSetUniformBuffersDynamic           = " << properties.limits.maxDescriptorSetUniformBuffersDynamic << "\n";
      std::cout << "\t\t\t" << "maxDrawIndexedIndexValue                        = " << properties.limits.maxDrawIndexedIndexValue << "\n";
      std::cout << "\t\t\t" << "maxDrawIndirectCount                            = " << properties.limits.maxDrawIndirectCount << "\n";
      std::cout << "\t\t\t" << "maxFragmentCombinedOutputResources              = " << properties.limits.maxFragmentCombinedOutputResources << "\n";
      std::cout << "\t\t\t" << "maxFragmentDualSrcAttachments                   = " << properties.limits.maxFragmentDualSrcAttachments << "\n";
      std::cout << "\t\t\t" << "maxFragmentInputComponents                      = " << properties.limits.maxFragmentInputComponents << "\n";
      std::cout << "\t\t\t" << "maxFragmentOutputAttachments                    = " << properties.limits.maxFragmentOutputAttachments << "\n";
      std::cout << "\t\t\t" << "maxFramebufferHeight                            = " << properties.limits.maxFramebufferHeight << "\n";
      std::cout << "\t\t\t" << "maxFramebufferLayers                            = " << properties.limits.maxFramebufferLayers << "\n";
      std::cout << "\t\t\t" << "maxFramebufferWidth                             = " << properties.limits.maxFramebufferWidth << "\n";
      std::cout << "\t\t\t" << "maxGeometryInputComponents                      = " << properties.limits.maxGeometryInputComponents << "\n";
      std::cout << "\t\t\t" << "maxGeometryOutputComponents                     = " << properties.limits.maxGeometryOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxGeometryOutputVertices                       = " << properties.limits.maxGeometryOutputVertices << "\n";
      std::cout << "\t\t\t" << "maxGeometryShaderInvocations                    = " << properties.limits.maxGeometryShaderInvocations << "\n";
      std::cout << "\t\t\t" << "maxGeometryTotalOutputComponents                = " << properties.limits.maxGeometryTotalOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxImageArrayLayers                             = " << properties.limits.maxImageArrayLayers << "\n";
      std::cout << "\t\t\t" << "maxImageDimension1D                             = " << properties.limits.maxImageDimension1D << "\n";
      std::cout << "\t\t\t" << "maxImageDimension2D                             = " << properties.limits.maxImageDimension2D << "\n";
      std::cout << "\t\t\t" << "maxImageDimension3D                             = " << properties.limits.maxImageDimension3D << "\n";
      std::cout << "\t\t\t" << "maxImageDimensionCube                           = " << properties.limits.maxImageDimensionCube << "\n";
      std::cout << "\t\t\t" << "maxInterpolationOffset                          = " << properties.limits.maxInterpolationOffset << "\n";
      std::cout << "\t\t\t" << "maxMemoryAllocationCount                        = " << properties.limits.maxMemoryAllocationCount << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorInputAttachments           = " << properties.limits.maxPerStageDescriptorInputAttachments << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorSampledImages              = " << properties.limits.maxPerStageDescriptorSampledImages << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorSamplers                   = " << properties.limits.maxPerStageDescriptorSamplers << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorStorageBuffers             = " << properties.limits.maxPerStageDescriptorStorageBuffers << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorStorageImages              = " << properties.limits.maxPerStageDescriptorStorageImages << "\n";
      std::cout << "\t\t\t" << "maxPerStageDescriptorUniformBuffers             = " << properties.limits.maxPerStageDescriptorUniformBuffers << "\n";
      std::cout << "\t\t\t" << "maxPerStageResources                            = " << properties.limits.maxPerStageResources << "\n";
      std::cout << "\t\t\t" << "maxPushConstantsSize                            = " << properties.limits.maxPushConstantsSize << "\n";
      std::cout << "\t\t\t" << "maxSampleMaskWords                              = " << properties.limits.maxSampleMaskWords << "\n";
      std::cout << "\t\t\t" << "maxSamplerAllocationCount                       = " << properties.limits.maxSamplerAllocationCount << "\n";
      std::cout << "\t\t\t" << "maxSamplerAnisotropy                            = " << properties.limits.maxSamplerAnisotropy << "\n";
      std::cout << "\t\t\t" << "maxSamplerLodBias                               = " << properties.limits.maxSamplerLodBias << "\n";
      std::cout << "\t\t\t" << "maxStorageBufferRange                           = " << properties.limits.maxStorageBufferRange << "\n";
      std::cout << "\t\t\t" << "maxTessellationControlPerPatchOutputComponents  = " << properties.limits.maxTessellationControlPerPatchOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationControlPerVertexInputComponents  = " << properties.limits.maxTessellationControlPerVertexInputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationControlPerVertexOutputComponents = " << properties.limits.maxTessellationControlPerVertexOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationControlTotalOutputComponents     = " << properties.limits.maxTessellationControlTotalOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationEvaluationInputComponents        = " << properties.limits.maxTessellationEvaluationInputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationEvaluationOutputComponents       = " << properties.limits.maxTessellationEvaluationOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxTessellationGenerationLevel                  = " << properties.limits.maxTessellationGenerationLevel << "\n";
      std::cout << "\t\t\t" << "maxTessellationPatchSize                        = " << properties.limits.maxTessellationPatchSize << "\n";
      std::cout << "\t\t\t" << "maxTexelBufferElements                          = " << properties.limits.maxTexelBufferElements << "\n";
      std::cout << "\t\t\t" << "maxTexelGatherOffset                            = " << properties.limits.maxTexelGatherOffset << "\n";
      std::cout << "\t\t\t" << "maxTexelOffset                                  = " << properties.limits.maxTexelOffset << "\n";
      std::cout << "\t\t\t" << "maxUniformBufferRange                           = " << properties.limits.maxUniformBufferRange << "\n";
      std::cout << "\t\t\t" << "maxVertexInputAttributeOffset                   = " << properties.limits.maxVertexInputAttributeOffset << "\n";
      std::cout << "\t\t\t" << "maxVertexInputAttributes                        = " << properties.limits.maxVertexInputAttributes << "\n";
      std::cout << "\t\t\t" << "maxVertexInputBindings                          = " << properties.limits.maxVertexInputBindings << "\n";
      std::cout << "\t\t\t" << "maxVertexInputBindingStride                     = " << properties.limits.maxVertexInputBindingStride << "\n";
      std::cout << "\t\t\t" << "maxVertexOutputComponents                       = " << properties.limits.maxVertexOutputComponents << "\n";
      std::cout << "\t\t\t" << "maxViewportDimensions                           = " << "[" << properties.limits.maxViewportDimensions[0] << ", " << properties.limits.maxViewportDimensions[1] << "]" << "\n";
      std::cout << "\t\t\t" << "maxViewports                                    = " << properties.limits.maxViewports << "\n";
      std::cout << "\t\t\t" << "minInterpolationOffset                          = " << properties.limits.minInterpolationOffset << "\n";
      std::cout << "\t\t\t" << "minMemoryMapAlignment                           = " << properties.limits.minMemoryMapAlignment << "\n";
      std::cout << "\t\t\t" << "minStorageBufferOffsetAlignment                 = " << properties.limits.minStorageBufferOffsetAlignment << "\n";
      std::cout << "\t\t\t" << "minTexelBufferOffsetAlignment                   = " << properties.limits.minTexelBufferOffsetAlignment << "\n";
      std::cout << "\t\t\t" << "minTexelGatherOffset                            = " << properties.limits.minTexelGatherOffset << "\n";
      std::cout << "\t\t\t" << "minTexelOffset                                  = " << properties.limits.minTexelOffset << "\n";
      std::cout << "\t\t\t" << "minUniformBufferOffsetAlignment                 = " << properties.limits.minUniformBufferOffsetAlignment << "\n";
      std::cout << "\t\t\t" << "mipmapPrecisionBits                             = " << properties.limits.mipmapPrecisionBits << "\n";
      std::cout << "\t\t\t" << "nonCoherentAtomSize                             = " << properties.limits.nonCoherentAtomSize << "\n";
      std::cout << "\t\t\t" << "optimalBufferCopyOffsetAlignment                = " << properties.limits.optimalBufferCopyOffsetAlignment << "\n";
      std::cout << "\t\t\t" << "optimalBufferCopyRowPitchAlignment              = " << properties.limits.optimalBufferCopyRowPitchAlignment << "\n";
      std::cout << "\t\t\t" << "pointSizeGranularity                            = " << properties.limits.pointSizeGranularity << "\n";
      std::cout << "\t\t\t" << "pointSizeRange                                  = " << "[" << properties.limits.pointSizeRange[0] << ", " << properties.limits.pointSizeRange[1] << "]" << "\n";
      std::cout << "\t\t\t" << "sampledImageColorSampleCounts                   = " << vk::to_string(properties.limits.sampledImageColorSampleCounts) << "\n";
      std::cout << "\t\t\t" << "sampledImageDepthSampleCounts                   = " << vk::to_string(properties.limits.sampledImageDepthSampleCounts) << "\n";
      std::cout << "\t\t\t" << "sampledImageIntegerSampleCounts                 = " << vk::to_string(properties.limits.sampledImageIntegerSampleCounts) << "\n";
      std::cout << "\t\t\t" << "sampledImageStencilSampleCounts                 = " << vk::to_string(properties.limits.sampledImageStencilSampleCounts) << "\n";
      std::cout << "\t\t\t" << "sparseAddressSpaceSize                          = " << properties.limits.sparseAddressSpaceSize << "\n";
      std::cout << "\t\t\t" << "standardSampleLocations                         = " << static_cast<bool>(properties.limits.standardSampleLocations) << "\n";
      std::cout << "\t\t\t" << "storageImageSampleCounts                        = " << vk::to_string(properties.limits.storageImageSampleCounts) << "\n";
      std::cout << "\t\t\t" << "strictLines                                     = " << static_cast<bool>(properties.limits.strictLines) << "\n";
      std::cout << "\t\t\t" << "subPixelInterpolationOffsetBits                 = " << properties.limits.subPixelInterpolationOffsetBits << "\n";
      std::cout << "\t\t\t" << "subPixelPrecisionBits                           = " << properties.limits.subPixelPrecisionBits << "\n";
      std::cout << "\t\t\t" << "subTexelPrecisionBits                           = " << properties.limits.subTexelPrecisionBits << "\n";
      std::cout << "\t\t\t" << "timestampComputeAndGraphics                     = " << static_cast<bool>(properties.limits.timestampComputeAndGraphics) << "\n";
      std::cout << "\t\t\t" << "timestampPeriod                                 = " << properties.limits.timestampPeriod << "\n";
      std::cout << "\t\t\t" << "viewportBoundsRange                             = " << "[" << properties.limits.viewportBoundsRange[0] << ", " << properties.limits.viewportBoundsRange[1] << "]" << "\n";
      std::cout << "\t\t\t" << "viewportSubPixelBits                            = " << properties.limits.viewportSubPixelBits << "\n";
      std::cout << "\t\t" << "sparseProperties:\n";
      std::cout << "\t\t\t" << "residencyAlignedMipSize                   = " << static_cast<bool>(properties.sparseProperties.residencyAlignedMipSize) << "\n";
      std::cout << "\t\t\t" << "residencyNonResidentStrict                = " << static_cast<bool>(properties.sparseProperties.residencyNonResidentStrict) << "\n";
      std::cout << "\t\t\t" << "residencyStandard2DBlockShape             = " << static_cast<bool>(properties.sparseProperties.residencyStandard2DBlockShape) << "\n";
      std::cout << "\t\t\t" << "residencyStandard2DMultisampleBlockShape  = " << static_cast<bool>(properties.sparseProperties.residencyStandard2DMultisampleBlockShape) << "\n";
      std::cout << "\t\t\t" << "residencyStandard3DBlockShape             = " << static_cast<bool>(properties.sparseProperties.residencyStandard3DBlockShape) << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_EXT_blend_operation_advanced"))
      {
        vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT const& blendOperationAdvancedProperties = properties2.get<vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>();
        std::cout << "\t" << "BlendOperationAdvancedProperties:\n";
        std::cout << "\t\t" << "advancedBlendAllOperations            = " << static_cast<bool>(blendOperationAdvancedProperties.advancedBlendAllOperations) << "\n";
        std::cout << "\t\t" << "advancedBlendCorrelatedOverlap        = " << static_cast<bool>(blendOperationAdvancedProperties.advancedBlendCorrelatedOverlap) << "\n";
        std::cout << "\t\t" << "advancedBlendIndependentBlend         = " << static_cast<bool>(blendOperationAdvancedProperties.advancedBlendIndependentBlend) << "\n";
        std::cout << "\t\t" << "advancedBlendMaxColorAttachments      = " << blendOperationAdvancedProperties.advancedBlendMaxColorAttachments << "\n";
        std::cout << "\t\t" << "advancedBlendNonPremultipliedDstColor = " << static_cast<bool>(blendOperationAdvancedProperties.advancedBlendNonPremultipliedDstColor) << "\n";
        std::cout << "\t\t" << "advancedBlendNonPremultipliedSrcColor = " << static_cast<bool>(blendOperationAdvancedProperties.advancedBlendNonPremultipliedSrcColor) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_conservative_rasterization"))
      {
        vk::PhysicalDeviceConservativeRasterizationPropertiesEXT const& conservativeRasterizationProperties = properties2.get<vk::PhysicalDeviceConservativeRasterizationPropertiesEXT>();
        std::cout << "\t" << "ConservativeRasterizationProperties:\n";
        std::cout << "\t\t" << "conservativePointAndLineRasterization       = " << static_cast<bool>(conservativeRasterizationProperties.conservativePointAndLineRasterization) << "\n";
        std::cout << "\t\t" << "conservativeRasterizationPostDepthCoverage  = " << static_cast<bool>(conservativeRasterizationProperties.conservativeRasterizationPostDepthCoverage) << "\n";
        std::cout << "\t\t" << "degenerateLinesRasterized                   = " << static_cast<bool>(conservativeRasterizationProperties.degenerateLinesRasterized) << "\n";
        std::cout << "\t\t" << "degenerateTrianglesRasterized               = " << static_cast<bool>(conservativeRasterizationProperties.degenerateTrianglesRasterized) << "\n";
        std::cout << "\t\t" << "extraPrimitiveOverestimationSizeGranularity = " << conservativeRasterizationProperties.extraPrimitiveOverestimationSizeGranularity << "\n";
        std::cout << "\t\t" << "fullyCoveredFragmentShaderInputVariable     = " << static_cast<bool>(conservativeRasterizationProperties.fullyCoveredFragmentShaderInputVariable) << "\n";
        std::cout << "\t\t" << "maxExtraPrimitiveOverestimationSize         = " << conservativeRasterizationProperties.maxExtraPrimitiveOverestimationSize << "\n";
        std::cout << "\t\t" << "primitiveOverestimationSize                 = " << conservativeRasterizationProperties.primitiveOverestimationSize << "\n";
        std::cout << "\t\t" << "primitiveUnderestimation                    = " << static_cast<bool>(conservativeRasterizationProperties.primitiveUnderestimation) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_NV_cooperative_matrix"))
      {
        vk::PhysicalDeviceCooperativeMatrixPropertiesNV const& cooperativeMatrixProperties = properties2.get<vk::PhysicalDeviceCooperativeMatrixPropertiesNV>();
        std::cout << "\t" << "CooperativeMatrixProperties:\n";
        std::cout << "\t\t" << "cooperativeMatrixSupportedStages  = " << vk::to_string(cooperativeMatrixProperties.cooperativeMatrixSupportedStages) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_KHR_depth_stencil_resolve"))
      {
        vk::PhysicalDeviceDepthStencilResolvePropertiesKHR const& depthStencilResolveProperties = properties2.get<vk::PhysicalDeviceDepthStencilResolvePropertiesKHR>();
        std::cout << "\t" << "DepthStencilResolveProperties:\n";
        std::cout << "\t\t" << "independentResolve            = " << static_cast<bool>(depthStencilResolveProperties.independentResolve) << "\n";
        std::cout << "\t\t" << "independentResolveNone        = " << static_cast<bool>(depthStencilResolveProperties.independentResolveNone) << "\n";
        std::cout << "\t\t" << "supportedDepthResolveModes    = " << vk::to_string(depthStencilResolveProperties.supportedDepthResolveModes) << "\n";
        std::cout << "\t\t" << "supportedStencilResolveModes  = " << vk::to_string(depthStencilResolveProperties.supportedStencilResolveModes) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_descriptor_indexing"))
      {
        vk::PhysicalDeviceDescriptorIndexingPropertiesEXT const& descriptorIndexingProperties = properties2.get<vk::PhysicalDeviceDescriptorIndexingPropertiesEXT>();
        std::cout << "\t" << "DescriptorIndexingProperties:\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindInputAttachments       = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindInputAttachments << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindSampledImages          = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSampledImages << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindSamplers               = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindSamplers << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindStorageBuffers         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffers << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic  = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindStorageImages          = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindStorageImages << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindUniformBuffers         = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffers << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic  = " << descriptorIndexingProperties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindInputAttachments  = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindInputAttachments << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindSampledImages     = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSampledImages << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindSamplers          = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindSamplers << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindStorageBuffers    = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageBuffers << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindStorageImages     = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindStorageImages << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindUniformBuffers    = " << descriptorIndexingProperties.maxPerStageDescriptorUpdateAfterBindUniformBuffers << "\n";
        std::cout << "\t\t" << "maxPerStageUpdateAfterBindResources                   = " << descriptorIndexingProperties.maxPerStageUpdateAfterBindResources << "\n";
        std::cout << "\t\t" << "maxUpdateAfterBindDescriptorsInAllPools               = " << descriptorIndexingProperties.maxUpdateAfterBindDescriptorsInAllPools << "\n";
        std::cout << "\t\t" << "quadDivergentImplicitLod                              = " << static_cast<bool>(descriptorIndexingProperties.quadDivergentImplicitLod) << "\n";
        std::cout << "\t\t" << "robustBufferAccessUpdateAfterBind                     = " << static_cast<bool>(descriptorIndexingProperties.robustBufferAccessUpdateAfterBind) << "\n";
        std::cout << "\t\t" << "shaderInputAttachmentArrayNonUniformIndexingNative    = " << static_cast<bool>(descriptorIndexingProperties.shaderInputAttachmentArrayNonUniformIndexingNative) << "\n";
        std::cout << "\t\t" << "shaderSampledImageArrayNonUniformIndexingNative       = " << static_cast<bool>(descriptorIndexingProperties.shaderSampledImageArrayNonUniformIndexingNative) << "\n";
        std::cout << "\t\t" << "shaderStorageBufferArrayNonUniformIndexingNative      = " << static_cast<bool>(descriptorIndexingProperties.shaderStorageBufferArrayNonUniformIndexingNative) << "\n";
        std::cout << "\t\t" << "shaderStorageImageArrayNonUniformIndexingNative       = " << static_cast<bool>(descriptorIndexingProperties.shaderStorageImageArrayNonUniformIndexingNative) << "\n";
        std::cout << "\t\t" << "shaderUniformBufferArrayNonUniformIndexingNative      = " << static_cast<bool>(descriptorIndexingProperties.shaderUniformBufferArrayNonUniformIndexingNative) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_discard_rectangles"))
      {
        vk::PhysicalDeviceDiscardRectanglePropertiesEXT const& discardRectangleProperties = properties2.get<vk::PhysicalDeviceDiscardRectanglePropertiesEXT>();
        std::cout << "\t" << "DiscardRectangleProperties:\n";
        std::cout << "\t\t" << "maxDiscardRectangles  = " << discardRectangleProperties.maxDiscardRectangles << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_KHR_driver_properties"))
      {
        vk::PhysicalDeviceDriverPropertiesKHR const& driverProperties = properties2.get<vk::PhysicalDeviceDriverPropertiesKHR>();
        std::cout << "\t" << "DriverProperties:\n";
        std::cout << "\t\t" << "driverID            = " << vk::to_string(driverProperties.driverID) << "\n";
        std::cout << "\t\t" << "driverName          = " << driverProperties.driverName << "\n";
        std::cout << "\t\t" << "driverInfo          = " << driverProperties.driverInfo << "\n";
        std::cout << "\t\t" << "conformanceVersion  = " << static_cast<uint32_t>(driverProperties.conformanceVersion.major) << "." << static_cast<uint32_t>(driverProperties.conformanceVersion.minor) << "." << static_cast<uint32_t>(driverProperties.conformanceVersion.subminor) << "." << static_cast<uint32_t>(driverProperties.conformanceVersion.patch) << std::dec << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_external_memory_host"))
      {
        vk::PhysicalDeviceExternalMemoryHostPropertiesEXT const& externalMemoryHostProperties = properties2.get<vk::PhysicalDeviceExternalMemoryHostPropertiesEXT>();
        std::cout << "\t" << "ExternalMemoryHostProperties:\n";
        std::cout << "\t\t" << "minImportedHostPointerAlignment = " << externalMemoryHostProperties.minImportedHostPointerAlignment << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_KHR_shader_float_controls"))
      {
        vk::PhysicalDeviceFloatControlsPropertiesKHR const& floatControlsProperties = properties2.get<vk::PhysicalDeviceFloatControlsPropertiesKHR>();
        std::cout << "\t" << "FloatControlsProperties:\n";
        std::cout << "\t\t" << "denormBehaviorIndependence            = " << vk::to_string(floatControlsProperties.denormBehaviorIndependence) << "\n";
        std::cout << "\t\t" << "roundingModeIndependence              = " << vk::to_string(floatControlsProperties.roundingModeIndependence) << "\n";
        std::cout << "\t\t" << "shaderDenormFlushToZeroFloat16        = " << static_cast<bool>(floatControlsProperties.shaderDenormFlushToZeroFloat16) << "\n";
        std::cout << "\t\t" << "shaderDenormFlushToZeroFloat32        = " << static_cast<bool>(floatControlsProperties.shaderDenormFlushToZeroFloat32) << "\n";
        std::cout << "\t\t" << "shaderDenormFlushToZeroFloat64        = " << static_cast<bool>(floatControlsProperties.shaderDenormFlushToZeroFloat64) << "\n";
        std::cout << "\t\t" << "shaderDenormPreserveFloat16           = " << static_cast<bool>(floatControlsProperties.shaderDenormPreserveFloat16) << "\n";
        std::cout << "\t\t" << "shaderDenormPreserveFloat32           = " << static_cast<bool>(floatControlsProperties.shaderDenormPreserveFloat32) << "\n";
        std::cout << "\t\t" << "shaderDenormPreserveFloat64           = " << static_cast<bool>(floatControlsProperties.shaderDenormPreserveFloat64) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTEFloat16          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTEFloat16) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTEFloat32          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTEFloat32) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTEFloat64          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTEFloat64) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTZFloat16          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTZFloat16) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTZFloat32          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTZFloat32) << "\n";
        std::cout << "\t\t" << "shaderRoundingModeRTZFloat64          = " << static_cast<bool>(floatControlsProperties.shaderRoundingModeRTZFloat64) << "\n";
        std::cout << "\t\t" << "shaderSignedZeroInfNanPreserveFloat16 = " << static_cast<bool>(floatControlsProperties.shaderSignedZeroInfNanPreserveFloat16) << "\n";
        std::cout << "\t\t" << "shaderSignedZeroInfNanPreserveFloat32 = " << static_cast<bool>(floatControlsProperties.shaderSignedZeroInfNanPreserveFloat32) << "\n";
        std::cout << "\t\t" << "shaderSignedZeroInfNanPreserveFloat64 = " << static_cast<bool>(floatControlsProperties.shaderSignedZeroInfNanPreserveFloat64) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_fragment_density_map"))
      {
        vk::PhysicalDeviceFragmentDensityMapPropertiesEXT const& fragmentDensityMapProperties = properties2.get<vk::PhysicalDeviceFragmentDensityMapPropertiesEXT>();
        std::cout << "\t" << "FragmentDensityProperties:\n";
        std::cout << "\t\t" << "fragmentDensityInvocations  = " << static_cast<bool>(fragmentDensityMapProperties.fragmentDensityInvocations) << "\n";
        std::cout << "\t\t" << "maxFragmentDensityTexelSize = " << fragmentDensityMapProperties.maxFragmentDensityTexelSize.width << " x " << fragmentDensityMapProperties.maxFragmentDensityTexelSize.height << "\n";
        std::cout << "\t\t" << "minFragmentDensityTexelSize = " << fragmentDensityMapProperties.minFragmentDensityTexelSize.width << " x " << fragmentDensityMapProperties.minFragmentDensityTexelSize.height << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceIDProperties const& idProperties = properties2.get<vk::PhysicalDeviceIDProperties>();
      std::cout << "\t" << "IDProperties:\n";
      std::cout << "\t\t" << "deviceUUID      = " << vk::su::UUID(idProperties.deviceUUID) << "\n";
      std::cout << "\t\t" << "driverUUID      = " << vk::su::UUID(idProperties.driverUUID) << "\n";
      std::cout << "\t\t" << "deviceLUID      = " << vk::su::LUID(idProperties.deviceLUID) << "\n";
      std::cout << "\t\t" << "deviceNodeMask  = " << std::hex << idProperties.deviceNodeMask << std::dec << "\n";
      std::cout << "\t\t" << "deviceLUIDValid = " << static_cast<bool>(idProperties.deviceLUIDValid) << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_EXT_inline_uniform_block"))
      {
        vk::PhysicalDeviceInlineUniformBlockPropertiesEXT const& inlineUniformBlockProperties = properties2.get<vk::PhysicalDeviceInlineUniformBlockPropertiesEXT>();
        std::cout << "\t" << "InlineUniformBlockProperties:\n";
        std::cout << "\t\t" << "maxDescriptorSetInlineUniformBlocks                     = " << inlineUniformBlockProperties.maxDescriptorSetInlineUniformBlocks << "\n";
        std::cout << "\t\t" << "maxDescriptorSetUpdateAfterBindInlineUniformBlocks      = " << inlineUniformBlockProperties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << "\t\t" << "maxInlineUniformBlockSize                               = " << inlineUniformBlockProperties.maxInlineUniformBlockSize << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorInlineUniformBlocks                = " << inlineUniformBlockProperties.maxPerStageDescriptorInlineUniformBlocks << "\n";
        std::cout << "\t\t" << "maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = " << inlineUniformBlockProperties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_line_rasterization"))
      {
        vk::PhysicalDeviceLineRasterizationPropertiesEXT const& lineRasterizationProperties = properties2.get<vk::PhysicalDeviceLineRasterizationPropertiesEXT>();
        std::cout << "\t" << "LineRasterizationProperties:\n";
        std::cout << "\t\t" << "lineSubPixelPrecisionBits = " << lineRasterizationProperties.lineSubPixelPrecisionBits << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMaintenance3Properties const& maintenance3Properties = properties2.get<vk::PhysicalDeviceMaintenance3Properties>();
      std::cout << "\t" << "Maintenance3Properties:\n";
      std::cout << "\t\t" << "maxMemoryAllocationSize = " << maintenance3Properties.maxMemoryAllocationSize << "\n";
      std::cout << "\t\t" << "maxPerSetDescriptors    = " << maintenance3Properties.maxPerSetDescriptors << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_NV_mesh_shader"))
      {
        vk::PhysicalDeviceMeshShaderPropertiesNV const& meshShaderProperties = properties2.get<vk::PhysicalDeviceMeshShaderPropertiesNV>();
        std::cout << "\t" << "MeshShaderProperties:\n";
        std::cout << "\t\t" << "maxDrawMeshTasksCount             = " << meshShaderProperties.maxDrawMeshTasksCount << "\n";
        std::cout << "\t\t" << "maxMeshMultiviewViewCount         = " << meshShaderProperties.maxMeshMultiviewViewCount << "\n";
        std::cout << "\t\t" << "maxMeshOutputPrimitives           = " << meshShaderProperties.maxMeshOutputPrimitives << "\n";
        std::cout << "\t\t" << "maxMeshOutputVertices             = " << meshShaderProperties.maxMeshOutputVertices << "\n";
        std::cout << "\t\t" << "maxMeshTotalMemorySize            = " << meshShaderProperties.maxMeshTotalMemorySize << "\n";
        std::cout << "\t\t" << "maxMeshWorkGroupInvocations       = " << meshShaderProperties.maxMeshWorkGroupInvocations << "\n";
        std::cout << "\t\t" << "maxMeshWorkGroupSize              = " << "[" << meshShaderProperties.maxMeshWorkGroupSize[0] << ", " << meshShaderProperties.maxMeshWorkGroupSize[1] << ", " << meshShaderProperties.maxMeshWorkGroupSize[2] << "]" << "\n";
        std::cout << "\t\t" << "maxTaskOutputCount                = " << meshShaderProperties.maxTaskOutputCount << "\n";
        std::cout << "\t\t" << "maxTaskTotalMemorySize            = " << meshShaderProperties.maxTaskTotalMemorySize << "\n";
        std::cout << "\t\t" << "maxTaskWorkGroupInvocations       = " << meshShaderProperties.maxTaskWorkGroupInvocations << "\n";
        std::cout << "\t\t" << "maxTaskWorkGroupSize              = " << "[" << meshShaderProperties.maxTaskWorkGroupSize[0] << ", " << meshShaderProperties.maxTaskWorkGroupSize[1] << ", " << meshShaderProperties.maxTaskWorkGroupSize[2] << "]" << "\n";
        std::cout << "\t\t" << "meshOutputPerPrimitiveGranularity = " << meshShaderProperties.meshOutputPerPrimitiveGranularity << "\n";
        std::cout << "\t\t" << "meshOutputPerVertexGranularity    = " << meshShaderProperties.meshOutputPerVertexGranularity << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_NVX_multiview_per_view_attributes"))
      {
        vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX const& multiviewPerViewAttributesProperties = properties2.get<vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX>();
        std::cout << "\t" << "MultiviewPerViewAttributesProperties:\n";
        std::cout << "\t\t" << "perViewPositionAllComponents  = " << static_cast<bool>(multiviewPerViewAttributesProperties.perViewPositionAllComponents) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMultiviewProperties const& multiviewProperties = properties2.get<vk::PhysicalDeviceMultiviewProperties>();
      std::cout << "\t" << "MultiviewProperties:\n";
      std::cout << "\t\t" << "maxMultiviewInstanceIndex = " << multiviewProperties.maxMultiviewInstanceIndex << "\n";
      std::cout << "\t\t" << "maxMultiviewViewCount     = " << multiviewProperties.maxMultiviewViewCount << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_EXT_pci_bus_info"))
      {
        vk::PhysicalDevicePCIBusInfoPropertiesEXT const& pciBusInfoProperties = properties2.get<vk::PhysicalDevicePCIBusInfoPropertiesEXT>();
        std::cout << "\t" << "PCIBusInfoProperties:\n";
        std::cout << "\t\t" << "pciDomain   = " << pciBusInfoProperties.pciDomain << "\n";
        std::cout << "\t\t" << "pciBus      = " << pciBusInfoProperties.pciBus << "\n";
        std::cout << "\t\t" << "pciDevice   = " << pciBusInfoProperties.pciDevice << "\n";
        std::cout << "\t\t" << "pciFunction = " << pciBusInfoProperties.pciFunction << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_KHR_maintenance2"))
      {
        vk::PhysicalDevicePointClippingProperties const& pointClippingProperties = properties2.get<vk::PhysicalDevicePointClippingProperties>();
        std::cout << "\t" << "PointClippingProperties:\n";
        std::cout << "\t\t" << "pointClippingBehavior = " << vk::to_string(pointClippingProperties.pointClippingBehavior) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceProtectedMemoryProperties const& protectedMemoryProperties = properties2.get<vk::PhysicalDeviceProtectedMemoryProperties>();
      std::cout << "\t" << "ProtectedMemoryProperties:\n";
      std::cout << "\t\t" << "protectedNoFault  = " << static_cast<bool>(protectedMemoryProperties.protectedNoFault) << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_KHR_push_descriptor"))
      {
        vk::PhysicalDevicePushDescriptorPropertiesKHR const& pushDescriptorProperties = properties2.get<vk::PhysicalDevicePushDescriptorPropertiesKHR>();
        std::cout << "\t" << "PushDescriptorProperties:\n";
        std::cout << "\t\t" << "maxPushDescriptors  = " << pushDescriptorProperties.maxPushDescriptors << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_NV_ray_tracing"))
      {
        vk::PhysicalDeviceRayTracingPropertiesNV const& rayTracingProperties = properties2.get<vk::PhysicalDeviceRayTracingPropertiesNV>();
        std::cout << "\t" << "RayTracingProperties:\n";
        std::cout << "\t\t" << "maxDescriptorSetAccelerationStructures  = " << rayTracingProperties.maxDescriptorSetAccelerationStructures << "\n";
        std::cout << "\t\t" << "maxGeometryCount                        = " << rayTracingProperties.maxGeometryCount << "\n";
        std::cout << "\t\t" << "maxInstanceCount                        = " << rayTracingProperties.maxInstanceCount << "\n";
        std::cout << "\t\t" << "maxRecursionDepth                       = " << rayTracingProperties.maxRecursionDepth << "\n";
        std::cout << "\t\t" << "maxShaderGroupStride                    = " << rayTracingProperties.maxShaderGroupStride << "\n";
        std::cout << "\t\t" << "maxTriangleCount                        = " << rayTracingProperties.maxTriangleCount << "\n";
        std::cout << "\t\t" << "shaderGroupBaseAlignment                = " << rayTracingProperties.shaderGroupBaseAlignment << "\n";
        std::cout << "\t\t" << "shaderGroupHandleSize                   = " << rayTracingProperties.shaderGroupHandleSize << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_sample_locations"))
      {
        vk::PhysicalDeviceSampleLocationsPropertiesEXT const& sampleLocationProperties = properties2.get<vk::PhysicalDeviceSampleLocationsPropertiesEXT>();
        std::cout << "\t" << "SampleLocationProperties:\n";
        std::cout << "\t\t" << "maxSampleLocationGridSize     = " << sampleLocationProperties.maxSampleLocationGridSize.width << " x " << sampleLocationProperties.maxSampleLocationGridSize.height << "\n";
        std::cout << "\t\t" << "sampleLocationCoordinateRange = " << "[" << sampleLocationProperties.sampleLocationCoordinateRange[0] << ", " << sampleLocationProperties.sampleLocationCoordinateRange[1] << "]" << "\n";
        std::cout << "\t\t" << "sampleLocationSampleCounts    = " << vk::to_string(sampleLocationProperties.sampleLocationSampleCounts) << "\n";
        std::cout << "\t\t" << "sampleLocationSubPixelBits    = " << sampleLocationProperties.sampleLocationSubPixelBits << "\n";
        std::cout << "\t\t" << "variableSampleLocations       = " << static_cast<bool>(sampleLocationProperties.variableSampleLocations) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_sampler_filter_minmax"))
      {
        vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT const& samplerFilterMinmaxProperties = properties2.get<vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT>();
        std::cout << "\t" << "SamplerFilterMinmaxProperties:\n";
        std::cout << "\t\t" << "filterMinmaxImageComponentMapping   = " << static_cast<bool>(samplerFilterMinmaxProperties.filterMinmaxImageComponentMapping) << "\n";
        std::cout << "\t\t" << "filterMinmaxSingleComponentFormats  = " << static_cast<bool>(samplerFilterMinmaxProperties.filterMinmaxSingleComponentFormats) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_AMD_shader_core_properties2"))
      {
        vk::PhysicalDeviceShaderCoreProperties2AMD const& shaderCoreProperties2 = properties2.get<vk::PhysicalDeviceShaderCoreProperties2AMD>();
        std::cout << "\t" << "ShaderCoreProperties2:\n";
        std::cout << "\t\t" << "activeComputeUnitCount  = " << shaderCoreProperties2.activeComputeUnitCount << "\n";
        std::cout << "\t\t" << "shaderCoreFeatures      = " << vk::to_string(shaderCoreProperties2.shaderCoreFeatures) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_AMD_shader_core_properties2"))
      {
        vk::PhysicalDeviceShaderCorePropertiesAMD const& shaderCoreProperties = properties2.get<vk::PhysicalDeviceShaderCorePropertiesAMD>();
        std::cout << "\t" << "ShaderCoreProperties:\n";
        std::cout << "\t\t" << "computeUnitsPerShaderArray  = " << shaderCoreProperties.computeUnitsPerShaderArray << "\n";
        std::cout << "\t\t" << "maxSgprAllocation           = " << shaderCoreProperties.maxSgprAllocation << "\n";
        std::cout << "\t\t" << "maxVgprAllocation           = " << shaderCoreProperties.maxVgprAllocation << "\n";
        std::cout << "\t\t" << "minSgprAllocation           = " << shaderCoreProperties.minSgprAllocation << "\n";
        std::cout << "\t\t" << "minVgprAllocation           = " << shaderCoreProperties.minVgprAllocation << "\n";
        std::cout << "\t\t" << "sgprAllocationGranularity   = " << shaderCoreProperties.sgprAllocationGranularity << "\n";
        std::cout << "\t\t" << "sgprsPerSimd                = " << shaderCoreProperties.sgprsPerSimd << "\n";
        std::cout << "\t\t" << "shaderArraysPerEngineCount  = " << shaderCoreProperties.shaderArraysPerEngineCount << "\n";
        std::cout << "\t\t" << "shaderEngineCount           = " << shaderCoreProperties.shaderEngineCount << "\n";
        std::cout << "\t\t" << "simdPerComputeUnit          = " << shaderCoreProperties.simdPerComputeUnit << "\n";
        std::cout << "\t\t" << "vgprAllocationGranularity   = " << shaderCoreProperties.vgprAllocationGranularity << "\n";
        std::cout << "\t\t" << "vgprsPerSimd                = " << shaderCoreProperties.vgprsPerSimd << "\n";
        std::cout << "\t\t" << "wavefrontSize               = " << shaderCoreProperties.wavefrontSize << "\n";
        std::cout << "\t\t" << "wavefrontsPerSimd           = " << shaderCoreProperties.wavefrontsPerSimd << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_NV_shader_sm_builtins"))
      {
        vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV const& shaderSMBuiltinsProperties = properties2.get<vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV>();
        std::cout << "\t" << "ShaderSMBuiltinsProperties:\n";
        std::cout << "\t\t" << "shaderSMCount     = " << shaderSMBuiltinsProperties.shaderSMCount << "\n";
        std::cout << "\t\t" << "shaderWarpsPerSM  = " << shaderSMBuiltinsProperties.shaderWarpsPerSM << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_NV_shading_rate_image"))
      {
        vk::PhysicalDeviceShadingRateImagePropertiesNV const& shadingRageImageProperties = properties2.get<vk::PhysicalDeviceShadingRateImagePropertiesNV>();
        std::cout << "\t" << "ShadingRateImageProperties:\n";
        std::cout << "\t\t" << "shadingRateMaxCoarseSamples = " << shadingRageImageProperties.shadingRateMaxCoarseSamples << "\n";
        std::cout << "\t\t" << "shadingRatePaletteSize      = " << shadingRageImageProperties.shadingRatePaletteSize << "\n";
        std::cout << "\t\t" << "shadingRatePaletteSize      = " << "[" << shadingRageImageProperties.shadingRateTexelSize.width << " x " << shadingRageImageProperties.shadingRateTexelSize.height << "]" << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceSubgroupProperties const& subgroupProperties = properties2.get<vk::PhysicalDeviceSubgroupProperties>();
      std::cout << "\t" << "SubgroupProperties:\n";
      std::cout << "\t\t" << "quadOperationsInAllStages = " << static_cast<bool>(subgroupProperties.quadOperationsInAllStages) << "\n";
      std::cout << "\t\t" << "subgroupSize              = " << subgroupProperties.subgroupSize << "\n";
      std::cout << "\t\t" << "supportedOperations       = " << vk::to_string(subgroupProperties.supportedOperations) << "\n";
      std::cout << "\t\t" << "supportedStages           = " << vk::to_string(subgroupProperties.supportedStages) << "\n";
      std::cout << "\n";

      if (vk::su::contains(extensionProperties, "VK_EXT_subgroup_size_control"))
      {
        vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT const& subgroupSizeControlProperties = properties2.get<vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT>();
        std::cout << "\t" << "SubgroupSizeControlProperties:\n";
        std::cout << "\t\t" << "maxComputeWorkgroupSubgroups  = " << subgroupSizeControlProperties.maxComputeWorkgroupSubgroups << "\n";
        std::cout << "\t\t" << "maxSubgroupSize               = " << subgroupSizeControlProperties.maxSubgroupSize << "\n";
        std::cout << "\t\t" << "minSubgroupSize               = " << subgroupSizeControlProperties.minSubgroupSize << "\n";
        std::cout << "\t\t" << "requiredSubgroupSizeStages    = " << vk::to_string(subgroupSizeControlProperties.requiredSubgroupSizeStages) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_KHR_timeline_semaphore"))
      {
        vk::PhysicalDeviceTimelineSemaphorePropertiesKHR const& timelineSemaphoreProperties = properties2.get<vk::PhysicalDeviceTimelineSemaphorePropertiesKHR>();
        std::cout << "\t" << "TimelineSemaphoreProperties:\n";
        std::cout << "\t\t" << "maxTimelineSemaphoreValueDifference = " << timelineSemaphoreProperties.maxTimelineSemaphoreValueDifference << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_texel_buffer_alignment"))
      {
        vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT const& texelBufferAlignmentProperties = properties2.get<vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT>();
        std::cout << "\t" << "TexelBufferAlignmentProperties:\n";
        std::cout << "\t\t" << "storageTexelBufferOffsetAlignmentBytes        = " << texelBufferAlignmentProperties.storageTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << "\t\t" << "storageTexelBufferOffsetSingleTexelAlignment  = " << static_cast<bool>(texelBufferAlignmentProperties.storageTexelBufferOffsetSingleTexelAlignment) << "\n";
        std::cout << "\t\t" << "uniformTexelBufferOffsetAlignmentBytes        = " << texelBufferAlignmentProperties.uniformTexelBufferOffsetAlignmentBytes << "\n";
        std::cout << "\t\t" << "uniformTexelBufferOffsetSingleTexelAlignment  = " << static_cast<bool>(texelBufferAlignmentProperties.uniformTexelBufferOffsetSingleTexelAlignment) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_transform_feedback"))
      {
        vk::PhysicalDeviceTransformFeedbackPropertiesEXT const& transformFeedbackProperties = properties2.get<vk::PhysicalDeviceTransformFeedbackPropertiesEXT>();
        std::cout << "\t" << "TransformFeedbackProperties:\n";
        std::cout << "\t\t" << "maxTransformFeedbackBufferDataSize          = " << transformFeedbackProperties.maxTransformFeedbackBufferDataSize << "\n";
        std::cout << "\t\t" << "maxTransformFeedbackBufferDataStride        = " << transformFeedbackProperties.maxTransformFeedbackBufferDataStride << "\n";
        std::cout << "\t\t" << "maxTransformFeedbackBuffers                 = " << transformFeedbackProperties.maxTransformFeedbackBuffers << "\n";
        std::cout << "\t\t" << "maxTransformFeedbackBufferSize              = " << transformFeedbackProperties.maxTransformFeedbackBufferSize << "\n";
        std::cout << "\t\t" << "maxTransformFeedbackStreamDataSize          = " << transformFeedbackProperties.maxTransformFeedbackStreamDataSize << "\n";
        std::cout << "\t\t" << "maxTransformFeedbackStreams                 = " << transformFeedbackProperties.maxTransformFeedbackStreams << "\n";
        std::cout << "\t\t" << "transformFeedbackDraw                       = " << static_cast<bool>(transformFeedbackProperties.transformFeedbackDraw) << "\n";
        std::cout << "\t\t" << "transformFeedbackQueries                    = " << static_cast<bool>(transformFeedbackProperties.transformFeedbackQueries) << "\n";
        std::cout << "\t\t" << "transformFeedbackRasterizationStreamSelect  = " << static_cast<bool>(transformFeedbackProperties.transformFeedbackRasterizationStreamSelect) << "\n";
        std::cout << "\t\t" << "transformFeedbackStreamsLinesTriangles      = " << static_cast<bool>(transformFeedbackProperties.transformFeedbackStreamsLinesTriangles) << "\n";
        std::cout << "\n";
      }

      if (vk::su::contains(extensionProperties, "VK_EXT_vertex_attribute_divisor"))
      {
        vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT const& vertexAttributeDivisorProperties = properties2.get<vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>();
        std::cout << "\t" << "VertexAttributeDivisorProperties:\n";
        std::cout << "\t\t" << "maxVertexAttribDivisor  = " << vertexAttributeDivisorProperties.maxVertexAttribDivisor << "\n";
        std::cout << "\n";
      }
    }

    /* VULKAN_KEY_END */
  }
  catch (vk::SystemError& err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error& err)
  {
    std::cout << "std::runtime_error: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}
