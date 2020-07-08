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
// VulkanHpp Samples : PhysicalDeviceFeatures
//                     Get the fine-grained features of the physical devices that can be supported by an implementation.

#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"

#include <vector>

static char const * AppName    = "PhysicalDeviceFeatures";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance( AppName, EngineName );
#if !defined( NDEBUG )
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger( instance );
#endif

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    /* VULKAN_KEY_START */

    std::cout << std::boolalpha;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      // some features are only valid, if a corresponding extension is available!
      std::vector<vk::ExtensionProperties> extensionProperties =
        physicalDevices[i].enumerateDeviceExtensionProperties();

      std::cout << "PhysicalDevice " << i << "\n";
      auto features2 = physicalDevices[i]
                         .getFeatures2<vk::PhysicalDeviceFeatures2,
                                       vk::PhysicalDevice16BitStorageFeatures,
                                       vk::PhysicalDevice8BitStorageFeaturesKHR,
                                       vk::PhysicalDeviceASTCDecodeFeaturesEXT,
                                       vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT,
                                       vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT,
                                       vk::PhysicalDeviceCoherentMemoryFeaturesAMD,
                                       vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV,
                                       vk::PhysicalDeviceConditionalRenderingFeaturesEXT,
                                       vk::PhysicalDeviceCooperativeMatrixFeaturesNV,
                                       vk::PhysicalDeviceCornerSampledImageFeaturesNV,
                                       vk::PhysicalDeviceCoverageReductionModeFeaturesNV,
                                       vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV,
                                       vk::PhysicalDeviceDepthClipEnableFeaturesEXT,
                                       vk::PhysicalDeviceDescriptorIndexingFeaturesEXT,
                                       vk::PhysicalDeviceExclusiveScissorFeaturesNV,
                                       vk::PhysicalDeviceFragmentDensityMapFeaturesEXT,
                                       vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV,
                                       vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT,
                                       vk::PhysicalDeviceHostQueryResetFeaturesEXT,
                                       vk::PhysicalDeviceImagelessFramebufferFeaturesKHR,
                                       vk::PhysicalDeviceIndexTypeUint8FeaturesEXT,
                                       vk::PhysicalDeviceInlineUniformBlockFeaturesEXT,
                                       vk::PhysicalDeviceLineRasterizationFeaturesEXT,
                                       vk::PhysicalDeviceMemoryPriorityFeaturesEXT,
                                       vk::PhysicalDeviceMeshShaderFeaturesNV,
                                       vk::PhysicalDeviceMultiviewFeatures,
                                       vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR,
                                       vk::PhysicalDeviceProtectedMemoryFeatures,
                                       vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV,
                                       vk::PhysicalDeviceSamplerYcbcrConversionFeatures,
                                       vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT,
                                       vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR,
                                       vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT,
                                       vk::PhysicalDeviceShaderDrawParametersFeatures,
                                       vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR,
                                       vk::PhysicalDeviceShaderImageFootprintFeaturesNV,
                                       vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL,
                                       vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV,
                                       vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR,
                                       vk::PhysicalDeviceShadingRateImageFeaturesNV,
                                       vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT,
                                       vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT,
                                       vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT,
                                       vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR,
                                       vk::PhysicalDeviceTransformFeedbackFeaturesEXT,
                                       vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR,
                                       vk::PhysicalDeviceVariablePointersFeatures,
                                       vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT,
                                       vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR,
                                       vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>();
      vk::PhysicalDeviceFeatures const & features = features2.get<vk::PhysicalDeviceFeatures2>().features;
      std::cout << "\tFeatures:\n";
      std::cout << "\t\talphaToOne                              : " << static_cast<bool>( features.alphaToOne ) << "\n";
      std::cout << "\t\tdepthBiasClamp                          : " << static_cast<bool>( features.depthBiasClamp )
                << "\n";
      std::cout << "\t\tdepthBounds                             : " << static_cast<bool>( features.depthBounds )
                << "\n";
      std::cout << "\t\tdepthClamp                              : " << static_cast<bool>( features.depthClamp ) << "\n";
      std::cout << "\t\tdrawIndirectFirstInstance               : "
                << static_cast<bool>( features.drawIndirectFirstInstance ) << "\n";
      std::cout << "\t\tdualSrcBlend                            : " << static_cast<bool>( features.dualSrcBlend )
                << "\n";
      std::cout << "\t\tfillModeNonSolid                        : " << static_cast<bool>( features.fillModeNonSolid )
                << "\n";
      std::cout << "\t\tfragmentStoresAndAtomics                : "
                << static_cast<bool>( features.fragmentStoresAndAtomics ) << "\n";
      std::cout << "\t\tfullDrawIndexUint32                     : " << static_cast<bool>( features.fullDrawIndexUint32 )
                << "\n";
      std::cout << "\t\tgeometryShader                          : " << static_cast<bool>( features.geometryShader )
                << "\n";
      std::cout << "\t\timageCubeArray                          : " << static_cast<bool>( features.imageCubeArray )
                << "\n";
      std::cout << "\t\tindependentBlend                        : " << static_cast<bool>( features.independentBlend )
                << "\n";
      std::cout << "\t\tinheritedQueries                        : " << static_cast<bool>( features.inheritedQueries )
                << "\n";
      std::cout << "\t\tlargePoints                             : " << static_cast<bool>( features.largePoints )
                << "\n";
      std::cout << "\t\tlogicOp                                 : " << static_cast<bool>( features.logicOp ) << "\n";
      std::cout << "\t\tmultiDrawIndirect                       : " << static_cast<bool>( features.multiDrawIndirect )
                << "\n";
      std::cout << "\t\tmultiViewport                           : " << static_cast<bool>( features.multiViewport )
                << "\n";
      std::cout << "\t\tocclusionQueryPrecise                   : "
                << static_cast<bool>( features.occlusionQueryPrecise ) << "\n";
      std::cout << "\t\tpipelineStatisticsQuery                 : "
                << static_cast<bool>( features.pipelineStatisticsQuery ) << "\n";
      std::cout << "\t\trobustBufferAccess                      : " << static_cast<bool>( features.robustBufferAccess )
                << "\n";
      std::cout << "\t\tsamplerAnisotropy                       : " << static_cast<bool>( features.samplerAnisotropy )
                << "\n";
      std::cout << "\t\tsampleRateShading                       : " << static_cast<bool>( features.sampleRateShading )
                << "\n";
      std::cout << "\t\tshaderClipDistance                      : " << static_cast<bool>( features.shaderClipDistance )
                << "\n";
      std::cout << "\t\tshaderCullDistance                      : " << static_cast<bool>( features.shaderCullDistance )
                << "\n";
      std::cout << "\t\tshaderFloat64                           : " << static_cast<bool>( features.shaderFloat64 )
                << "\n";
      std::cout << "\t\tshaderImageGatherExtended               : "
                << static_cast<bool>( features.shaderImageGatherExtended ) << "\n";
      std::cout << "\t\tshaderInt16                             : " << static_cast<bool>( features.shaderInt16 )
                << "\n";
      std::cout << "\t\tshaderInt64                             : " << static_cast<bool>( features.shaderInt64 )
                << "\n";
      std::cout << "\t\tshaderResourceMinLod                    : "
                << static_cast<bool>( features.shaderResourceMinLod ) << "\n";
      std::cout << "\t\tshaderResourceResidency                 : "
                << static_cast<bool>( features.shaderResourceResidency ) << "\n";
      std::cout << "\t\tshaderSampledImageArrayDynamicIndexing  : "
                << static_cast<bool>( features.shaderSampledImageArrayDynamicIndexing ) << "\n";
      std::cout << "\t\tshaderStorageBufferArrayDynamicIndexing : "
                << static_cast<bool>( features.shaderStorageBufferArrayDynamicIndexing ) << "\n";
      std::cout << "\t\tshaderStorageImageArrayDynamicIndexing  : "
                << static_cast<bool>( features.shaderStorageImageArrayDynamicIndexing ) << "\n";
      std::cout << "\t\tshaderStorageImageExtendedFormats       : "
                << static_cast<bool>( features.shaderStorageImageExtendedFormats ) << "\n";
      std::cout << "\t\tshaderStorageImageMultisample           : "
                << static_cast<bool>( features.shaderStorageImageMultisample ) << "\n";
      std::cout << "\t\tshaderStorageImageReadWithoutFormat     : "
                << static_cast<bool>( features.shaderStorageImageReadWithoutFormat ) << "\n";
      std::cout << "\t\tshaderStorageImageWriteWithoutFormat    : "
                << static_cast<bool>( features.shaderStorageImageWriteWithoutFormat ) << "\n";
      std::cout << "\t\tshaderTessellationAndGeometryPointSize  : "
                << static_cast<bool>( features.shaderTessellationAndGeometryPointSize ) << "\n";
      std::cout << "\t\tshaderUniformBufferArrayDynamicIndexing : "
                << static_cast<bool>( features.shaderUniformBufferArrayDynamicIndexing ) << "\n";
      std::cout << "\t\tsparseBinding                           : " << static_cast<bool>( features.sparseBinding )
                << "\n";
      std::cout << "\t\tsparseResidency16Samples                : "
                << static_cast<bool>( features.sparseResidency16Samples ) << "\n";
      std::cout << "\t\tsparseResidency2Samples                 : "
                << static_cast<bool>( features.sparseResidency2Samples ) << "\n";
      std::cout << "\t\tsparseResidency4Samples                 : "
                << static_cast<bool>( features.sparseResidency4Samples ) << "\n";
      std::cout << "\t\tsparseResidency8Samples                 : "
                << static_cast<bool>( features.sparseResidency8Samples ) << "\n";
      std::cout << "\t\tsparseResidencyAliased                  : "
                << static_cast<bool>( features.sparseResidencyAliased ) << "\n";
      std::cout << "\t\tsparseResidencyBuffer                   : "
                << static_cast<bool>( features.sparseResidencyBuffer ) << "\n";
      std::cout << "\t\tsparseResidencyImage2D                  : "
                << static_cast<bool>( features.sparseResidencyImage2D ) << "\n";
      std::cout << "\t\tsparseResidencyImage3D                  : "
                << static_cast<bool>( features.sparseResidencyImage3D ) << "\n";
      std::cout << "\t\ttessellationShader                      : " << static_cast<bool>( features.tessellationShader )
                << "\n";
      std::cout << "\t\ttextureCompressionASTC_LDR              : "
                << static_cast<bool>( features.textureCompressionASTC_LDR ) << "\n";
      std::cout << "\t\ttextureCompressionBC                    : "
                << static_cast<bool>( features.textureCompressionBC ) << "\n";
      std::cout << "\t\ttextureCompressionETC2                  : "
                << static_cast<bool>( features.textureCompressionETC2 ) << "\n";
      std::cout << "\t\tvariableMultisampleRate                 : "
                << static_cast<bool>( features.variableMultisampleRate ) << "\n";
      std::cout << "\t\tvertexPipelineStoresAndAtomics          : "
                << static_cast<bool>( features.vertexPipelineStoresAndAtomics ) << "\n";
      std::cout << "\t\twideLines                               : " << static_cast<bool>( features.wideLines ) << "\n";
      std::cout << "\n";

      vk::PhysicalDevice16BitStorageFeatures const & sixteenBitStorageFeatures =
        features2.get<vk::PhysicalDevice16BitStorageFeatures>();
      std::cout << "\t16BitStorageFeatures:\n";
      std::cout << "\t\tstorageBuffer16BitAccess            : "
                << static_cast<bool>( sixteenBitStorageFeatures.storageBuffer16BitAccess ) << "\n";
      std::cout << "\t\tstorageInputOutput16                : "
                << static_cast<bool>( sixteenBitStorageFeatures.storageInputOutput16 ) << "\n";
      std::cout << "\t\tstoragePushConstant16               : "
                << static_cast<bool>( sixteenBitStorageFeatures.storagePushConstant16 ) << "\n";
      std::cout << "\t\tuniformAndStorageBuffer16BitAccess  : "
                << static_cast<bool>( sixteenBitStorageFeatures.uniformAndStorageBuffer16BitAccess ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_8bit_storage" ) )
      {
        vk::PhysicalDevice8BitStorageFeaturesKHR const & eightBitStorageFeatures =
          features2.get<vk::PhysicalDevice8BitStorageFeaturesKHR>();
        std::cout << "\t8BitStorageFeatures:\n";
        std::cout << "\t\tstorageBuffer8BitAccess           : "
                  << static_cast<bool>( eightBitStorageFeatures.storageBuffer8BitAccess ) << "\n";
        std::cout << "\t\tstoragePushConstant8              : "
                  << static_cast<bool>( eightBitStorageFeatures.storagePushConstant8 ) << "\n";
        std::cout << "\t\tuniformAndStorageBuffer8BitAccess : "
                  << static_cast<bool>( eightBitStorageFeatures.uniformAndStorageBuffer8BitAccess ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_astc_decode_mode" ) )
      {
        vk::PhysicalDeviceASTCDecodeFeaturesEXT const & astcDecodeFeatures =
          features2.get<vk::PhysicalDeviceASTCDecodeFeaturesEXT>();
        std::cout << "\tASTCDecodeFeature:\n";
        std::cout << "\t\tdecodeModeSharedExponent  : "
                  << static_cast<bool>( astcDecodeFeatures.decodeModeSharedExponent ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_blend_operation_advanced" ) )
      {
        vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT const & blendOperationAdvancedFeatures =
          features2.get<vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>();
        std::cout << "\tBlendOperationAdvancedFeatures:\n";
        std::cout << "\t\tadvancedBlendCoherentOperations : "
                  << static_cast<bool>( blendOperationAdvancedFeatures.advancedBlendCoherentOperations ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_buffer_device_address" ) )
      {
        vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT const & bufferDeviceAddressFeatures =
          features2.get<vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT>();
        std::cout << "\tBufferDeviceAddressFeatures:\n";
        std::cout << "\t\tbufferDeviceAddress               : "
                  << static_cast<bool>( bufferDeviceAddressFeatures.bufferDeviceAddress ) << "\n";
        std::cout << "\t\tbufferDeviceAddressCaptureReplay  : "
                  << static_cast<bool>( bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay ) << "\n";
        std::cout << "\t\tbufferDeviceAddressMultiDevice    : "
                  << static_cast<bool>( bufferDeviceAddressFeatures.bufferDeviceAddressMultiDevice ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_device_coherent_memory" ) )
      {
        vk::PhysicalDeviceCoherentMemoryFeaturesAMD const & coherentMemoryFeatures =
          features2.get<vk::PhysicalDeviceCoherentMemoryFeaturesAMD>();
        std::cout << "\tCoherentMemoryFeatures:\n";
        std::cout << "\t\tdeviceCoherentMemory  : " << static_cast<bool>( coherentMemoryFeatures.deviceCoherentMemory )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_compute_shader_derivatives" ) )
      {
        vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV const & computeShaderDerivativesFeatures =
          features2.get<vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV>();
        std::cout << "\tComputeShaderDerivativeFeatures:\n";
        std::cout << "\t\tcomputeDerivativeGroupLinear  : "
                  << static_cast<bool>( computeShaderDerivativesFeatures.computeDerivativeGroupLinear ) << "\n";
        std::cout << "\t\tcomputeDerivativeGroupQuads   : "
                  << static_cast<bool>( computeShaderDerivativesFeatures.computeDerivativeGroupQuads ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_conditional_rendering" ) )
      {
        vk::PhysicalDeviceConditionalRenderingFeaturesEXT const & conditionalRenderingFeatures =
          features2.get<vk::PhysicalDeviceConditionalRenderingFeaturesEXT>();
        std::cout << "\tConditionalRenderingFeatures:\n";
        std::cout << "\t\tconditionalRendering          : "
                  << static_cast<bool>( conditionalRenderingFeatures.conditionalRendering ) << "\n";
        std::cout << "\t\tinheritedConditionalRendering : "
                  << static_cast<bool>( conditionalRenderingFeatures.inheritedConditionalRendering ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_cooperative_matrix" ) )
      {
        vk::PhysicalDeviceCooperativeMatrixFeaturesNV const & cooperativeMatrixFeatures =
          features2.get<vk::PhysicalDeviceCooperativeMatrixFeaturesNV>();
        std::cout << "\tCooperativeMatrixFeatures:\n";
        std::cout << "\t\tcooperativeMatrix                   : "
                  << static_cast<bool>( cooperativeMatrixFeatures.cooperativeMatrix ) << "\n";
        std::cout << "\t\tcooperativeMatrixRobustBufferAccess : "
                  << static_cast<bool>( cooperativeMatrixFeatures.cooperativeMatrixRobustBufferAccess ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_corner_sampled_image" ) )
      {
        vk::PhysicalDeviceCornerSampledImageFeaturesNV const & cornerSampledImageFeatures =
          features2.get<vk::PhysicalDeviceCornerSampledImageFeaturesNV>();
        std::cout << "\tCornerSampledImageFeatures:\n";
        std::cout << "\t\tcornerSampledImage  : " << static_cast<bool>( cornerSampledImageFeatures.cornerSampledImage )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_coverage_reduction_mode" ) )
      {
        vk::PhysicalDeviceCoverageReductionModeFeaturesNV const & coverageReductionModeFeatures =
          features2.get<vk::PhysicalDeviceCoverageReductionModeFeaturesNV>();
        std::cout << "\tCoverageReductionModeFeatures:\n";
        std::cout << "\t\tcoverageReductionMode : "
                  << static_cast<bool>( coverageReductionModeFeatures.coverageReductionMode ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_dedicated_allocation_image_aliasing" ) )
      {
        vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV const & dedicatedAllocationImageAliasingFeatures =
          features2.get<vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV>();
        std::cout << "\tDedicatedAllocationAliasingFeatures:\n";
        std::cout << "\t\tdedicatedAllocationImageAliasing  : "
                  << static_cast<bool>( dedicatedAllocationImageAliasingFeatures.dedicatedAllocationImageAliasing )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_depth_clip_enable" ) )
      {
        vk::PhysicalDeviceDepthClipEnableFeaturesEXT const & depthClipEnabledFeatures =
          features2.get<vk::PhysicalDeviceDepthClipEnableFeaturesEXT>();
        std::cout << "\tDepthClipEnabledFeatures:\n";
        std::cout << "\t\tdepthClipEnable : " << static_cast<bool>( depthClipEnabledFeatures.depthClipEnable ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_descriptor_indexing" ) )
      {
        vk::PhysicalDeviceDescriptorIndexingFeaturesEXT const & descriptorIndexingFeatures =
          features2.get<vk::PhysicalDeviceDescriptorIndexingFeaturesEXT>();
        std::cout << "\tDescriptorIndexingFeatures:\n";
        std::cout << "\t\tdescriptorBindingPartiallyBound                     : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingPartiallyBound ) << "\n";
        std::cout << "\t\tdescriptorBindingSampledImageUpdateAfterBind        : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageBufferUpdateAfterBind       : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageImageUpdateAfterBind        : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageTexelBufferUpdateAfterBind  : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingUniformBufferUpdateAfterBind       : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingUniformTexelBufferUpdateAfterBind  : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tdescriptorBindingUpdateUnusedWhilePending           : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending ) << "\n";
        std::cout << "\t\tdescriptorBindingVariableDescriptorCount            : "
                  << static_cast<bool>( descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount ) << "\n";
        std::cout << "\t\truntimeDescriptorArray                              : "
                  << static_cast<bool>( descriptorIndexingFeatures.runtimeDescriptorArray ) << "\n";
        std::cout << "\t\tshaderInputAttachmentArrayDynamicIndexing           : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderInputAttachmentArrayDynamicIndexing ) << "\n";
        std::cout << "\t\tshaderInputAttachmentArrayNonUniformIndexing        : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderInputAttachmentArrayNonUniformIndexing )
                  << "\n";
        std::cout << "\t\tshaderSampledImageArrayNonUniformIndexing           : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing ) << "\n";
        std::cout << "\t\tshaderStorageBufferArrayNonUniformIndexing          : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing ) << "\n";
        std::cout << "\t\tshaderStorageImageArrayNonUniformIndexing           : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderStorageImageArrayNonUniformIndexing ) << "\n";
        std::cout << "\t\tshaderStorageTexelBufferArrayDynamicIndexing        : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderStorageTexelBufferArrayDynamicIndexing )
                  << "\n";
        std::cout << "\t\tshaderStorageTexelBufferArrayNonUniformIndexing     : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderStorageTexelBufferArrayNonUniformIndexing )
                  << "\n";
        std::cout << "\t\tshaderUniformBufferArrayNonUniformIndexing          : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing ) << "\n";
        std::cout << "\t\tshaderUniformTexelBufferArrayDynamicIndexing        : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderUniformTexelBufferArrayDynamicIndexing )
                  << "\n";
        std::cout << "\t\tshaderUniformTexelBufferArrayNonUniformIndexing     : "
                  << static_cast<bool>( descriptorIndexingFeatures.shaderUniformTexelBufferArrayNonUniformIndexing )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_scissor_exclusive" ) )
      {
        vk::PhysicalDeviceExclusiveScissorFeaturesNV const & exclusiveScissorFeatures =
          features2.get<vk::PhysicalDeviceExclusiveScissorFeaturesNV>();
        std::cout << "\tExclusiveScissorFeatures:\n";
        std::cout << "\t\texclusiveScissor  : " << static_cast<bool>( exclusiveScissorFeatures.exclusiveScissor )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_density_map" ) )
      {
        vk::PhysicalDeviceFragmentDensityMapFeaturesEXT const & fragmentDensityMapFeatures =
          features2.get<vk::PhysicalDeviceFragmentDensityMapFeaturesEXT>();
        std::cout << "\tFragmentDensityMapFeatures:\n";
        std::cout << "\t\tfragmentDensityMap                    : "
                  << static_cast<bool>( fragmentDensityMapFeatures.fragmentDensityMap ) << "\n";
        std::cout << "\t\tfragmentDensityMapDynamic             : "
                  << static_cast<bool>( fragmentDensityMapFeatures.fragmentDensityMapDynamic ) << "\n";
        std::cout << "\t\tfragmentDensityMapNonSubsampledImages : "
                  << static_cast<bool>( fragmentDensityMapFeatures.fragmentDensityMapNonSubsampledImages ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_fragment_shader_barycentric" ) )
      {
        vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV const & fragmentShaderBarycentricFeatures =
          features2.get<vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV>();
        std::cout << "\tFragmentShaderBarycentricFeatures:\n";
        std::cout << "\t\tfragmentShaderBarycentric : "
                  << static_cast<bool>( fragmentShaderBarycentricFeatures.fragmentShaderBarycentric ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_shader_interlock" ) )
      {
        vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT const & fragmentShaderInterlockFeatures =
          features2.get<vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>();
        std::cout << "\tFragmentShaderInterlockFeatures:\n";
        std::cout << "\t\tfragmentShaderPixelInterlock        : "
                  << static_cast<bool>( fragmentShaderInterlockFeatures.fragmentShaderPixelInterlock ) << "\n";
        std::cout << "\t\tfragmentShaderSampleInterlock       : "
                  << static_cast<bool>( fragmentShaderInterlockFeatures.fragmentShaderSampleInterlock ) << "\n";
        std::cout << "\t\tfragmentShaderShadingRateInterlock  : "
                  << static_cast<bool>( fragmentShaderInterlockFeatures.fragmentShaderShadingRateInterlock ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_host_query_reset" ) )
      {
        vk::PhysicalDeviceHostQueryResetFeaturesEXT const & hostQueryResetFeatures =
          features2.get<vk::PhysicalDeviceHostQueryResetFeaturesEXT>();
        std::cout << "\tHostQueryResetFeatures:\n";
        std::cout << "\t\thostQueryReset  : " << static_cast<bool>( hostQueryResetFeatures.hostQueryReset ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_imageless_framebuffer" ) )
      {
        vk::PhysicalDeviceImagelessFramebufferFeaturesKHR const & imagelessFramebufferFeatures =
          features2.get<vk::PhysicalDeviceImagelessFramebufferFeaturesKHR>();
        std::cout << "\tImagelessFramebufferFeatures:\n";
        std::cout << "\t\timagelessFramebuffer  : "
                  << static_cast<bool>( imagelessFramebufferFeatures.imagelessFramebuffer ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_index_type_uint8" ) )
      {
        vk::PhysicalDeviceIndexTypeUint8FeaturesEXT const & indexTypeUint8Features =
          features2.get<vk::PhysicalDeviceIndexTypeUint8FeaturesEXT>();
        std::cout << "\tIndexTypeUint8Features:\n";
        std::cout << "\t\tindexTypeUint8  : " << static_cast<bool>( indexTypeUint8Features.indexTypeUint8 ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_inline_uniform_block" ) )
      {
        vk::PhysicalDeviceInlineUniformBlockFeaturesEXT const & inlineUniformBlockFeatures =
          features2.get<vk::PhysicalDeviceInlineUniformBlockFeaturesEXT>();
        std::cout << "\tInlineUniformBlockFeatures:\n";
        std::cout << "\t\tdescriptorBindingInlineUniformBlockUpdateAfterBind  : "
                  << static_cast<bool>( inlineUniformBlockFeatures.descriptorBindingInlineUniformBlockUpdateAfterBind )
                  << "\n";
        std::cout << "\t\tinlineUniformBlock                                  : "
                  << static_cast<bool>( inlineUniformBlockFeatures.inlineUniformBlock ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_line_rasterization" ) )
      {
        vk::PhysicalDeviceLineRasterizationFeaturesEXT const & lineRasterizationFeatures =
          features2.get<vk::PhysicalDeviceLineRasterizationFeaturesEXT>();
        std::cout << "\tLineRasterizationFeatures:\n";
        std::cout << "\t\tbresenhamLines            : " << static_cast<bool>( lineRasterizationFeatures.bresenhamLines )
                  << "\n";
        std::cout << "\t\trectangularLines          : "
                  << static_cast<bool>( lineRasterizationFeatures.rectangularLines ) << "\n";
        std::cout << "\t\tsmoothLines               : " << static_cast<bool>( lineRasterizationFeatures.smoothLines )
                  << "\n";
        std::cout << "\t\tstippledBresenhamLines    : "
                  << static_cast<bool>( lineRasterizationFeatures.stippledBresenhamLines ) << "\n";
        std::cout << "\t\tstippledRectangularLines  : "
                  << static_cast<bool>( lineRasterizationFeatures.stippledRectangularLines ) << "\n";
        std::cout << "\t\tstippledSmoothLines       : "
                  << static_cast<bool>( lineRasterizationFeatures.stippledSmoothLines ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_memory_priority" ) )
      {
        vk::PhysicalDeviceMemoryPriorityFeaturesEXT const & memoryPriorityFeatures =
          features2.get<vk::PhysicalDeviceMemoryPriorityFeaturesEXT>();
        std::cout << "\tMemoryPriorityFeatures:\n";
        std::cout << "\t\tmemoryPriority  : " << static_cast<bool>( memoryPriorityFeatures.memoryPriority ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_mesh_shader" ) )
      {
        vk::PhysicalDeviceMeshShaderFeaturesNV const & meshShaderFeatures =
          features2.get<vk::PhysicalDeviceMeshShaderFeaturesNV>();
        std::cout << "\tMeshShaderFeatures:\n";
        std::cout << "\t\tmeshShader  : " << static_cast<bool>( meshShaderFeatures.meshShader ) << "\n";
        std::cout << "\t\ttaskShader  : " << static_cast<bool>( meshShaderFeatures.taskShader ) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMultiviewFeatures const & multiviewFeatures =
        features2.get<vk::PhysicalDeviceMultiviewFeatures>();
      std::cout << "\tMultiviewFeatures:\n";
      std::cout << "\t\tmultiview                   : " << static_cast<bool>( multiviewFeatures.multiview ) << "\n";
      std::cout << "\t\tmultiviewGeometryShader     : "
                << static_cast<bool>( multiviewFeatures.multiviewGeometryShader ) << "\n";
      std::cout << "\t\tmultiviewTessellationShader : "
                << static_cast<bool>( multiviewFeatures.multiviewTessellationShader ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_pipeline_executable_properties" ) )
      {
        vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR const & pipelineExecutablePropertiesFeatures =
          features2.get<vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR>();
        std::cout << "\tPipelineExectuablePropertiesFeatures:\n";
        std::cout << "\t\tpipelineExecutableInfo  : "
                  << static_cast<bool>( pipelineExecutablePropertiesFeatures.pipelineExecutableInfo ) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceProtectedMemoryFeatures const & protectedMemoryFeatures =
        features2.get<vk::PhysicalDeviceProtectedMemoryFeatures>();
      std::cout << "\tProtectedMemoryFeatures:\n";
      std::cout << "\t\tprotectedMemory : " << static_cast<bool>( protectedMemoryFeatures.protectedMemory ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_NV_representative_fragment_test" ) )
      {
        vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV const & representativeFragmentTestFeatures =
          features2.get<vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV>();
        std::cout << "\tRepresentativeFragmentTestFeatures:\n";
        std::cout << "\t\trepresentativeFragmentTest  : "
                  << static_cast<bool>( representativeFragmentTestFeatures.representativeFragmentTest ) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceSamplerYcbcrConversionFeatures const & samplerYcbcrConversionFeatures =
        features2.get<vk::PhysicalDeviceSamplerYcbcrConversionFeatures>();
      std::cout << "\tSamplerYcbcrConversionFeatures:\n";
      std::cout << "\t\tsamplerYcbcrConversion  : "
                << static_cast<bool>( samplerYcbcrConversionFeatures.samplerYcbcrConversion ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_scalar_block_layout" ) )
      {
        vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT const & scalarBlockLayoutFeatures =
          features2.get<vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT>();
        std::cout << "\tScalarBlockLayoutFeatures:\n";
        std::cout << "\t\tscalarBlockLayout : " << static_cast<bool>( scalarBlockLayoutFeatures.scalarBlockLayout )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_atomic_int64" ) )
      {
        vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR const & shaderAtomicInt64Features =
          features2.get<vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR>();
        std::cout << "\tShaderAtomicInt64Features:\n";
        std::cout << "\t\tshaderBufferInt64Atomics  : "
                  << static_cast<bool>( shaderAtomicInt64Features.shaderBufferInt64Atomics ) << "\n";
        std::cout << "\t\tshaderSharedInt64Atomics  : "
                  << static_cast<bool>( shaderAtomicInt64Features.shaderSharedInt64Atomics ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_shader_demote_to_helper_invocation" ) )
      {
        vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT const & shaderDemoteToHelperInvocationFeatures =
          features2.get<vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT>();
        std::cout << "\tShaderDemoteToHelperInvocationFeatures:\n";
        std::cout << "\t\tshaderDemoteToHelperInvocation  : "
                  << static_cast<bool>( shaderDemoteToHelperInvocationFeatures.shaderDemoteToHelperInvocation ) << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceShaderDrawParametersFeatures const & shaderDrawParametersFeature =
        features2.get<vk::PhysicalDeviceShaderDrawParametersFeatures>();
      std::cout << "\tShaderDrawParametersFeature:\n";
      std::cout << "\t\tshaderDrawParameters  : "
                << static_cast<bool>( shaderDrawParametersFeature.shaderDrawParameters ) << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_float16_int8" ) )
      {
        vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR const & shaderFloat16Int8Features =
          features2.get<vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR>();
        std::cout << "\tShaderFloat16Int8Features:\n";
        std::cout << "\t\tshaderFloat16 : " << static_cast<bool>( shaderFloat16Int8Features.shaderFloat16 ) << "\n";
        std::cout << "\t\tshaderInt8    : " << static_cast<bool>( shaderFloat16Int8Features.shaderInt8 ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_image_footprint" ) )
      {
        vk::PhysicalDeviceShaderImageFootprintFeaturesNV const & shaderImageFootprintFeatures =
          features2.get<vk::PhysicalDeviceShaderImageFootprintFeaturesNV>();
        std::cout << "\tShaderImageFootprintFeatures:\n";
        std::cout << "\t\timageFootprint  : " << static_cast<bool>( shaderImageFootprintFeatures.imageFootprint )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_INTEL_shader_integer_functions2" ) )
      {
        vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL const & shaderIntegerFunctions2Features =
          features2.get<vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL>();
        std::cout << "\tShaderIntegerFunctions2Features:\n";
        std::cout << "\t\tshaderIntegerFunctions2 : "
                  << static_cast<bool>( shaderIntegerFunctions2Features.shaderIntegerFunctions2 ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_sm_builtins" ) )
      {
        vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV const & shaderSMBuiltinsFeatures =
          features2.get<vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV>();
        std::cout << "\tShaderSMBuiltinsFeatures:\n";
        std::cout << "\t\tshaderSMBuiltins  : " << static_cast<bool>( shaderSMBuiltinsFeatures.shaderSMBuiltins )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_subgroup_extended_types" ) )
      {
        vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR const & shaderSubgroupExtendedTypesFeatures =
          features2.get<vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR>();
        std::cout << "\tShaderSubgroupExtendedTypeFeatures:\n";
        std::cout << "\t\tshaderSubgroupExtendedTypes : "
                  << static_cast<bool>( shaderSubgroupExtendedTypesFeatures.shaderSubgroupExtendedTypes ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shading_rate_image" ) )
      {
        vk::PhysicalDeviceShadingRateImageFeaturesNV const & shadingRateImageFeatures =
          features2.get<vk::PhysicalDeviceShadingRateImageFeaturesNV>();
        std::cout << "\tShadingRateImageFeatures:\n";
        std::cout << "\t\tshadingRateCoarseSampleOrder  : "
                  << static_cast<bool>( shadingRateImageFeatures.shadingRateCoarseSampleOrder ) << "\n";
        std::cout << "\t\tshadingRateImage              : "
                  << static_cast<bool>( shadingRateImageFeatures.shadingRateImage ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_subgroup_size_control" ) )
      {
        vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT const & subgroupSizeControlFeatures =
          features2.get<vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT>();
        std::cout << "\tSubgroupSizeControlFeatures:\n";
        std::cout << "\t\tcomputeFullSubgroups  : "
                  << static_cast<bool>( subgroupSizeControlFeatures.computeFullSubgroups ) << "\n";
        std::cout << "\t\tsubgroupSizeControl   : "
                  << static_cast<bool>( subgroupSizeControlFeatures.subgroupSizeControl ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_texel_buffer_alignment" ) )
      {
        vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT const & texelBufferAlignmentFeatures =
          features2.get<vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>();
        std::cout << "\tTexelBufferAlignmentFeatures:\n";
        std::cout << "\t\ttexelBufferAlignment  : "
                  << static_cast<bool>( texelBufferAlignmentFeatures.texelBufferAlignment ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_texture_compression_astc_hdr" ) )
      {
        vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT const & textureCompressionASTCHDRFeatures =
          features2.get<vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT>();
        std::cout << "\tTextureCompressionASTCHHRFeatures:\n";
        std::cout << "\t\ttextureCompressionASTC_HDR  : "
                  << static_cast<bool>( textureCompressionASTCHDRFeatures.textureCompressionASTC_HDR ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_timeline_semaphore" ) )
      {
        vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR const & timelineSemaphoreFeatures =
          features2.get<vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR>();
        std::cout << "\tTimelineSemaphoreFeatures:\n";
        std::cout << "\t\ttimelineSemaphore :" << static_cast<bool>( timelineSemaphoreFeatures.timelineSemaphore )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_transform_feedback" ) )
      {
        vk::PhysicalDeviceTransformFeedbackFeaturesEXT const & transformFeedbackFeatures =
          features2.get<vk::PhysicalDeviceTransformFeedbackFeaturesEXT>();
        std::cout << "\tTransformFeedbackFeatures:\n";
        std::cout << "\t\tgeometryStreams : " << static_cast<bool>( transformFeedbackFeatures.geometryStreams ) << "\n";
        std::cout << "\t\ttransformFeedback : " << static_cast<bool>( transformFeedbackFeatures.transformFeedback )
                  << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_uniform_buffer_standard_layout" ) )
      {
        vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR const & uniformBufferStandardLayoutFeatures =
          features2.get<vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR>();
        std::cout << "\tUniformBufferStandardLayoutFeatures:\n";
        std::cout << "\t\tuniformBufferStandardLayout : "
                  << static_cast<bool>( uniformBufferStandardLayoutFeatures.uniformBufferStandardLayout ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_variable_pointers" ) )
      {
        vk::PhysicalDeviceVariablePointersFeatures const & variablePointersFeatures =
          features2.get<vk::PhysicalDeviceVariablePointersFeatures>();
        std::cout << "\tVariablePointersFeatures:\n";
        std::cout << "\t\tvariablePointers              : "
                  << static_cast<bool>( variablePointersFeatures.variablePointers ) << "\n";
        std::cout << "\t\tvariablePointersStorageBuffer : "
                  << static_cast<bool>( variablePointersFeatures.variablePointersStorageBuffer ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_vertex_attribute_divisor" ) )
      {
        vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT const & vertexAttributeDivisorFeatures =
          features2.get<vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT>();
        std::cout << "\tVertexAttributeDivisorFeature:\n";
        std::cout << "\t\tvertexAttributeInstanceRateDivisor      : "
                  << static_cast<bool>( vertexAttributeDivisorFeatures.vertexAttributeInstanceRateDivisor ) << "\n";
        std::cout << "\t\tvertexAttributeInstanceRateZeroDivisor  : "
                  << static_cast<bool>( vertexAttributeDivisorFeatures.vertexAttributeInstanceRateZeroDivisor ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_vulkan_memory_model" ) )
      {
        vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR const & vulkanMemoryModelFeatures =
          features2.get<vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR>();
        std::cout << "\tVulkanMemoryModelFeatures:\n";
        std::cout << "\t\tvulkanMemoryModel                             : "
                  << static_cast<bool>( vulkanMemoryModelFeatures.vulkanMemoryModel ) << "\n";
        std::cout << "\t\tvulkanMemoryModelAvailabilityVisibilityChains : "
                  << static_cast<bool>( vulkanMemoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains )
                  << "\n";
        std::cout << "\t\tvulkanMemoryModelDeviceScope                  : "
                  << static_cast<bool>( vulkanMemoryModelFeatures.vulkanMemoryModelDeviceScope ) << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_sampler_ycbcr_conversion" ) )
      {
        vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT const & ycbcrImageArraysFeatures =
          features2.get<vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>();
        std::cout << "\tYcbcrImageArraysFeatures:\n";
        std::cout << "\t\tycbcrImageArrays  : " << static_cast<bool>( ycbcrImageArraysFeatures.ycbcrImageArrays )
                  << "\n";
        std::cout << "\n";
      }
    }

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
