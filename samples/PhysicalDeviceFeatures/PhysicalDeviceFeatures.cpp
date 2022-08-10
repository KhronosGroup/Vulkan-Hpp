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

// ignore warning 4503: decorated name length exceeded, name was truncated
#if defined( _MSC_VER )
#  pragma warning( disable : 4503 )
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/utils.hpp"

#include <vector>

static char const * AppName    = "PhysicalDeviceFeatures";
static char const * EngineName = "Vulkan.hpp";

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

      std::cout << "PhysicalDevice " << i << " :\n";
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
      std::cout << "\t\talphaToOne                              : " << !!features.alphaToOne << "\n";
      std::cout << "\t\tdepthBiasClamp                          : " << !!features.depthBiasClamp << "\n";
      std::cout << "\t\tdepthBounds                             : " << !!features.depthBounds << "\n";
      std::cout << "\t\tdepthClamp                              : " << !!features.depthClamp << "\n";
      std::cout << "\t\tdrawIndirectFirstInstance               : " << !!features.drawIndirectFirstInstance << "\n";
      std::cout << "\t\tdualSrcBlend                            : " << !!features.dualSrcBlend << "\n";
      std::cout << "\t\tfillModeNonSolid                        : " << !!features.fillModeNonSolid << "\n";
      std::cout << "\t\tfragmentStoresAndAtomics                : " << !!features.fragmentStoresAndAtomics << "\n";
      std::cout << "\t\tfullDrawIndexUint32                     : " << !!features.fullDrawIndexUint32 << "\n";
      std::cout << "\t\tgeometryShader                          : " << !!features.geometryShader << "\n";
      std::cout << "\t\timageCubeArray                          : " << !!features.imageCubeArray << "\n";
      std::cout << "\t\tindependentBlend                        : " << !!features.independentBlend << "\n";
      std::cout << "\t\tinheritedQueries                        : " << !!features.inheritedQueries << "\n";
      std::cout << "\t\tlargePoints                             : " << !!features.largePoints << "\n";
      std::cout << "\t\tlogicOp                                 : " << !!features.logicOp << "\n";
      std::cout << "\t\tmultiDrawIndirect                       : " << !!features.multiDrawIndirect << "\n";
      std::cout << "\t\tmultiViewport                           : " << !!features.multiViewport << "\n";
      std::cout << "\t\tocclusionQueryPrecise                   : " << !!features.occlusionQueryPrecise << "\n";
      std::cout << "\t\tpipelineStatisticsQuery                 : " << !!features.pipelineStatisticsQuery << "\n";
      std::cout << "\t\trobustBufferAccess                      : " << !!features.robustBufferAccess << "\n";
      std::cout << "\t\tsamplerAnisotropy                       : " << !!features.samplerAnisotropy << "\n";
      std::cout << "\t\tsampleRateShading                       : " << !!features.sampleRateShading << "\n";
      std::cout << "\t\tshaderClipDistance                      : " << !!features.shaderClipDistance << "\n";
      std::cout << "\t\tshaderCullDistance                      : " << !!features.shaderCullDistance << "\n";
      std::cout << "\t\tshaderFloat64                           : " << !!features.shaderFloat64 << "\n";
      std::cout << "\t\tshaderImageGatherExtended               : " << !!features.shaderImageGatherExtended << "\n";
      std::cout << "\t\tshaderInt16                             : " << !!features.shaderInt16 << "\n";
      std::cout << "\t\tshaderInt64                             : " << !!features.shaderInt64 << "\n";
      std::cout << "\t\tshaderResourceMinLod                    : " << !!features.shaderResourceMinLod << "\n";
      std::cout << "\t\tshaderResourceResidency                 : " << !!features.shaderResourceResidency << "\n";
      std::cout << "\t\tshaderSampledImageArrayDynamicIndexing  : " << !!features.shaderSampledImageArrayDynamicIndexing << "\n";
      std::cout << "\t\tshaderStorageBufferArrayDynamicIndexing : " << !!features.shaderStorageBufferArrayDynamicIndexing << "\n";
      std::cout << "\t\tshaderStorageImageArrayDynamicIndexing  : " << !!features.shaderStorageImageArrayDynamicIndexing << "\n";
      std::cout << "\t\tshaderStorageImageExtendedFormats       : " << !!features.shaderStorageImageExtendedFormats << "\n";
      std::cout << "\t\tshaderStorageImageMultisample           : " << !!features.shaderStorageImageMultisample << "\n";
      std::cout << "\t\tshaderStorageImageReadWithoutFormat     : " << !!features.shaderStorageImageReadWithoutFormat << "\n";
      std::cout << "\t\tshaderStorageImageWriteWithoutFormat    : " << !!features.shaderStorageImageWriteWithoutFormat << "\n";
      std::cout << "\t\tshaderTessellationAndGeometryPointSize  : " << !!features.shaderTessellationAndGeometryPointSize << "\n";
      std::cout << "\t\tshaderUniformBufferArrayDynamicIndexing : " << !!features.shaderUniformBufferArrayDynamicIndexing << "\n";
      std::cout << "\t\tsparseBinding                           : " << !!features.sparseBinding << "\n";
      std::cout << "\t\tsparseResidency16Samples                : " << !!features.sparseResidency16Samples << "\n";
      std::cout << "\t\tsparseResidency2Samples                 : " << !!features.sparseResidency2Samples << "\n";
      std::cout << "\t\tsparseResidency4Samples                 : " << !!features.sparseResidency4Samples << "\n";
      std::cout << "\t\tsparseResidency8Samples                 : " << !!features.sparseResidency8Samples << "\n";
      std::cout << "\t\tsparseResidencyAliased                  : " << !!features.sparseResidencyAliased << "\n";
      std::cout << "\t\tsparseResidencyBuffer                   : " << !!features.sparseResidencyBuffer << "\n";
      std::cout << "\t\tsparseResidencyImage2D                  : " << !!features.sparseResidencyImage2D << "\n";
      std::cout << "\t\tsparseResidencyImage3D                  : " << !!features.sparseResidencyImage3D << "\n";
      std::cout << "\t\ttessellationShader                      : " << !!features.tessellationShader << "\n";
      std::cout << "\t\ttextureCompressionASTC_LDR              : " << !!features.textureCompressionASTC_LDR << "\n";
      std::cout << "\t\ttextureCompressionBC                    : " << !!features.textureCompressionBC << "\n";
      std::cout << "\t\ttextureCompressionETC2                  : " << !!features.textureCompressionETC2 << "\n";
      std::cout << "\t\tvariableMultisampleRate                 : " << !!features.variableMultisampleRate << "\n";
      std::cout << "\t\tvertexPipelineStoresAndAtomics          : " << !!features.vertexPipelineStoresAndAtomics << "\n";
      std::cout << "\t\twideLines                               : " << !!features.wideLines << "\n";
      std::cout << "\n";

      vk::PhysicalDevice16BitStorageFeatures const & sixteenBitStorageFeatures = features2.get<vk::PhysicalDevice16BitStorageFeatures>();
      std::cout << "\t16BitStorageFeatures:\n";
      std::cout << "\t\tstorageBuffer16BitAccess            : " << !!sixteenBitStorageFeatures.storageBuffer16BitAccess << "\n";
      std::cout << "\t\tstorageInputOutput16                : " << !!sixteenBitStorageFeatures.storageInputOutput16 << "\n";
      std::cout << "\t\tstoragePushConstant16               : " << !!sixteenBitStorageFeatures.storagePushConstant16 << "\n";
      std::cout << "\t\tuniformAndStorageBuffer16BitAccess  : " << !!sixteenBitStorageFeatures.uniformAndStorageBuffer16BitAccess << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_8bit_storage" ) )
      {
        vk::PhysicalDevice8BitStorageFeaturesKHR const & eightBitStorageFeatures = features2.get<vk::PhysicalDevice8BitStorageFeaturesKHR>();
        std::cout << "\t8BitStorageFeatures:\n";
        std::cout << "\t\tstorageBuffer8BitAccess           : " << !!eightBitStorageFeatures.storageBuffer8BitAccess << "\n";
        std::cout << "\t\tstoragePushConstant8              : " << !!eightBitStorageFeatures.storagePushConstant8 << "\n";
        std::cout << "\t\tuniformAndStorageBuffer8BitAccess : " << !!eightBitStorageFeatures.uniformAndStorageBuffer8BitAccess << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_astc_decode_mode" ) )
      {
        vk::PhysicalDeviceASTCDecodeFeaturesEXT const & astcDecodeFeatures = features2.get<vk::PhysicalDeviceASTCDecodeFeaturesEXT>();
        std::cout << "\tASTCDecodeFeature:\n";
        std::cout << "\t\tdecodeModeSharedExponent  : " << !!astcDecodeFeatures.decodeModeSharedExponent << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_blend_operation_advanced" ) )
      {
        vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT const & blendOperationAdvancedFeatures =
          features2.get<vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>();
        std::cout << "\tBlendOperationAdvancedFeatures:\n";
        std::cout << "\t\tadvancedBlendCoherentOperations : " << !!blendOperationAdvancedFeatures.advancedBlendCoherentOperations << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_buffer_device_address" ) )
      {
        vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT const & bufferDeviceAddressFeatures =
          features2.get<vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT>();
        std::cout << "\tBufferDeviceAddressFeatures:\n";
        std::cout << "\t\tbufferDeviceAddress               : " << !!bufferDeviceAddressFeatures.bufferDeviceAddress << "\n";
        std::cout << "\t\tbufferDeviceAddressCaptureReplay  : " << !!bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay << "\n";
        std::cout << "\t\tbufferDeviceAddressMultiDevice    : " << !!bufferDeviceAddressFeatures.bufferDeviceAddressMultiDevice << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_AMD_device_coherent_memory" ) )
      {
        vk::PhysicalDeviceCoherentMemoryFeaturesAMD const & coherentMemoryFeatures = features2.get<vk::PhysicalDeviceCoherentMemoryFeaturesAMD>();
        std::cout << "\tCoherentMemoryFeatures:\n";
        std::cout << "\t\tdeviceCoherentMemory  : " << !!coherentMemoryFeatures.deviceCoherentMemory << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_compute_shader_derivatives" ) )
      {
        vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV const & computeShaderDerivativesFeatures =
          features2.get<vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV>();
        std::cout << "\tComputeShaderDerivativeFeatures:\n";
        std::cout << "\t\tcomputeDerivativeGroupLinear  : " << !!computeShaderDerivativesFeatures.computeDerivativeGroupLinear << "\n";
        std::cout << "\t\tcomputeDerivativeGroupQuads   : " << !!computeShaderDerivativesFeatures.computeDerivativeGroupQuads << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_conditional_rendering" ) )
      {
        vk::PhysicalDeviceConditionalRenderingFeaturesEXT const & conditionalRenderingFeatures =
          features2.get<vk::PhysicalDeviceConditionalRenderingFeaturesEXT>();
        std::cout << "\tConditionalRenderingFeatures:\n";
        std::cout << "\t\tconditionalRendering          : " << !!conditionalRenderingFeatures.conditionalRendering << "\n";
        std::cout << "\t\tinheritedConditionalRendering : " << !!conditionalRenderingFeatures.inheritedConditionalRendering << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_cooperative_matrix" ) )
      {
        vk::PhysicalDeviceCooperativeMatrixFeaturesNV const & cooperativeMatrixFeatures = features2.get<vk::PhysicalDeviceCooperativeMatrixFeaturesNV>();
        std::cout << "\tCooperativeMatrixFeatures:\n";
        std::cout << "\t\tcooperativeMatrix                   : " << !!cooperativeMatrixFeatures.cooperativeMatrix << "\n";
        std::cout << "\t\tcooperativeMatrixRobustBufferAccess : " << !!cooperativeMatrixFeatures.cooperativeMatrixRobustBufferAccess << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_corner_sampled_image" ) )
      {
        vk::PhysicalDeviceCornerSampledImageFeaturesNV const & cornerSampledImageFeatures = features2.get<vk::PhysicalDeviceCornerSampledImageFeaturesNV>();
        std::cout << "\tCornerSampledImageFeatures:\n";
        std::cout << "\t\tcornerSampledImage  : " << !!cornerSampledImageFeatures.cornerSampledImage << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_coverage_reduction_mode" ) )
      {
        vk::PhysicalDeviceCoverageReductionModeFeaturesNV const & coverageReductionModeFeatures =
          features2.get<vk::PhysicalDeviceCoverageReductionModeFeaturesNV>();
        std::cout << "\tCoverageReductionModeFeatures:\n";
        std::cout << "\t\tcoverageReductionMode : " << !!coverageReductionModeFeatures.coverageReductionMode << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_dedicated_allocation_image_aliasing" ) )
      {
        vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV const & dedicatedAllocationImageAliasingFeatures =
          features2.get<vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV>();
        std::cout << "\tDedicatedAllocationAliasingFeatures:\n";
        std::cout << "\t\tdedicatedAllocationImageAliasing  : " << !!dedicatedAllocationImageAliasingFeatures.dedicatedAllocationImageAliasing << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_depth_clip_enable" ) )
      {
        vk::PhysicalDeviceDepthClipEnableFeaturesEXT const & depthClipEnabledFeatures = features2.get<vk::PhysicalDeviceDepthClipEnableFeaturesEXT>();
        std::cout << "\tDepthClipEnabledFeatures:\n";
        std::cout << "\t\tdepthClipEnable : " << !!depthClipEnabledFeatures.depthClipEnable << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_descriptor_indexing" ) )
      {
        vk::PhysicalDeviceDescriptorIndexingFeaturesEXT const & descriptorIndexingFeatures = features2.get<vk::PhysicalDeviceDescriptorIndexingFeaturesEXT>();
        std::cout << "\tDescriptorIndexingFeatures:\n";
        std::cout << "\t\tdescriptorBindingPartiallyBound                     : " << !!descriptorIndexingFeatures.descriptorBindingPartiallyBound << "\n";
        std::cout << "\t\tdescriptorBindingSampledImageUpdateAfterBind        : " << !!descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageBufferUpdateAfterBind       : " << !!descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageImageUpdateAfterBind        : " << !!descriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind
                  << "\n";
        std::cout << "\t\tdescriptorBindingStorageTexelBufferUpdateAfterBind  : "
                  << !!descriptorIndexingFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind << "\n";
        std::cout << "\t\tdescriptorBindingUniformBufferUpdateAfterBind       : " << !!descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind
                  << "\n";
        std::cout << "\t\tdescriptorBindingUniformTexelBufferUpdateAfterBind  : "
                  << !!descriptorIndexingFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind << "\n";
        std::cout << "\t\tdescriptorBindingUpdateUnusedWhilePending           : " << !!descriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending
                  << "\n";
        std::cout << "\t\tdescriptorBindingVariableDescriptorCount            : " << !!descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount
                  << "\n";
        std::cout << "\t\truntimeDescriptorArray                              : " << !!descriptorIndexingFeatures.runtimeDescriptorArray << "\n";
        std::cout << "\t\tshaderInputAttachmentArrayDynamicIndexing           : " << !!descriptorIndexingFeatures.shaderInputAttachmentArrayDynamicIndexing
                  << "\n";
        std::cout << "\t\tshaderInputAttachmentArrayNonUniformIndexing        : " << !!descriptorIndexingFeatures.shaderInputAttachmentArrayNonUniformIndexing
                  << "\n";
        std::cout << "\t\tshaderSampledImageArrayNonUniformIndexing           : " << !!descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing
                  << "\n";
        std::cout << "\t\tshaderStorageBufferArrayNonUniformIndexing          : " << !!descriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing
                  << "\n";
        std::cout << "\t\tshaderStorageImageArrayNonUniformIndexing           : " << !!descriptorIndexingFeatures.shaderStorageImageArrayNonUniformIndexing
                  << "\n";
        std::cout << "\t\tshaderStorageTexelBufferArrayDynamicIndexing        : " << !!descriptorIndexingFeatures.shaderStorageTexelBufferArrayDynamicIndexing
                  << "\n";
        std::cout << "\t\tshaderStorageTexelBufferArrayNonUniformIndexing     : "
                  << !!descriptorIndexingFeatures.shaderStorageTexelBufferArrayNonUniformIndexing << "\n";
        std::cout << "\t\tshaderUniformBufferArrayNonUniformIndexing          : " << !!descriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing
                  << "\n";
        std::cout << "\t\tshaderUniformTexelBufferArrayDynamicIndexing        : " << !!descriptorIndexingFeatures.shaderUniformTexelBufferArrayDynamicIndexing
                  << "\n";
        std::cout << "\t\tshaderUniformTexelBufferArrayNonUniformIndexing     : "
                  << !!descriptorIndexingFeatures.shaderUniformTexelBufferArrayNonUniformIndexing << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_scissor_exclusive" ) )
      {
        vk::PhysicalDeviceExclusiveScissorFeaturesNV const & exclusiveScissorFeatures = features2.get<vk::PhysicalDeviceExclusiveScissorFeaturesNV>();
        std::cout << "\tExclusiveScissorFeatures:\n";
        std::cout << "\t\texclusiveScissor  : " << !!exclusiveScissorFeatures.exclusiveScissor << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_density_map" ) )
      {
        vk::PhysicalDeviceFragmentDensityMapFeaturesEXT const & fragmentDensityMapFeatures = features2.get<vk::PhysicalDeviceFragmentDensityMapFeaturesEXT>();
        std::cout << "\tFragmentDensityMapFeatures:\n";
        std::cout << "\t\tfragmentDensityMap                    : " << !!fragmentDensityMapFeatures.fragmentDensityMap << "\n";
        std::cout << "\t\tfragmentDensityMapDynamic             : " << !!fragmentDensityMapFeatures.fragmentDensityMapDynamic << "\n";
        std::cout << "\t\tfragmentDensityMapNonSubsampledImages : " << !!fragmentDensityMapFeatures.fragmentDensityMapNonSubsampledImages << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_fragment_shader_barycentric" ) )
      {
        vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV const & fragmentShaderBarycentricFeatures =
          features2.get<vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV>();
        std::cout << "\tFragmentShaderBarycentricFeatures:\n";
        std::cout << "\t\tfragmentShaderBarycentric : " << !!fragmentShaderBarycentricFeatures.fragmentShaderBarycentric << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_fragment_shader_interlock" ) )
      {
        vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT const & fragmentShaderInterlockFeatures =
          features2.get<vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>();
        std::cout << "\tFragmentShaderInterlockFeatures:\n";
        std::cout << "\t\tfragmentShaderPixelInterlock        : " << !!fragmentShaderInterlockFeatures.fragmentShaderPixelInterlock << "\n";
        std::cout << "\t\tfragmentShaderSampleInterlock       : " << !!fragmentShaderInterlockFeatures.fragmentShaderSampleInterlock << "\n";
        std::cout << "\t\tfragmentShaderShadingRateInterlock  : " << !!fragmentShaderInterlockFeatures.fragmentShaderShadingRateInterlock << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_host_query_reset" ) )
      {
        vk::PhysicalDeviceHostQueryResetFeaturesEXT const & hostQueryResetFeatures = features2.get<vk::PhysicalDeviceHostQueryResetFeaturesEXT>();
        std::cout << "\tHostQueryResetFeatures:\n";
        std::cout << "\t\thostQueryReset  : " << !!hostQueryResetFeatures.hostQueryReset << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_imageless_framebuffer" ) )
      {
        vk::PhysicalDeviceImagelessFramebufferFeaturesKHR const & imagelessFramebufferFeatures =
          features2.get<vk::PhysicalDeviceImagelessFramebufferFeaturesKHR>();
        std::cout << "\tImagelessFramebufferFeatures:\n";
        std::cout << "\t\timagelessFramebuffer  : " << !!imagelessFramebufferFeatures.imagelessFramebuffer << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_index_type_uint8" ) )
      {
        vk::PhysicalDeviceIndexTypeUint8FeaturesEXT const & indexTypeUint8Features = features2.get<vk::PhysicalDeviceIndexTypeUint8FeaturesEXT>();
        std::cout << "\tIndexTypeUint8Features:\n";
        std::cout << "\t\tindexTypeUint8  : " << !!indexTypeUint8Features.indexTypeUint8 << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_inline_uniform_block" ) )
      {
        vk::PhysicalDeviceInlineUniformBlockFeaturesEXT const & inlineUniformBlockFeatures = features2.get<vk::PhysicalDeviceInlineUniformBlockFeaturesEXT>();
        std::cout << "\tInlineUniformBlockFeatures:\n";
        std::cout << "\t\tdescriptorBindingInlineUniformBlockUpdateAfterBind  : "
                  << !!inlineUniformBlockFeatures.descriptorBindingInlineUniformBlockUpdateAfterBind << "\n";
        std::cout << "\t\tinlineUniformBlock                                  : " << !!inlineUniformBlockFeatures.inlineUniformBlock << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_line_rasterization" ) )
      {
        vk::PhysicalDeviceLineRasterizationFeaturesEXT const & lineRasterizationFeatures = features2.get<vk::PhysicalDeviceLineRasterizationFeaturesEXT>();
        std::cout << "\tLineRasterizationFeatures:\n";
        std::cout << "\t\tbresenhamLines            : " << !!lineRasterizationFeatures.bresenhamLines << "\n";
        std::cout << "\t\trectangularLines          : " << !!lineRasterizationFeatures.rectangularLines << "\n";
        std::cout << "\t\tsmoothLines               : " << !!lineRasterizationFeatures.smoothLines << "\n";
        std::cout << "\t\tstippledBresenhamLines    : " << !!lineRasterizationFeatures.stippledBresenhamLines << "\n";
        std::cout << "\t\tstippledRectangularLines  : " << !!lineRasterizationFeatures.stippledRectangularLines << "\n";
        std::cout << "\t\tstippledSmoothLines       : " << !!lineRasterizationFeatures.stippledSmoothLines << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_memory_priority" ) )
      {
        vk::PhysicalDeviceMemoryPriorityFeaturesEXT const & memoryPriorityFeatures = features2.get<vk::PhysicalDeviceMemoryPriorityFeaturesEXT>();
        std::cout << "\tMemoryPriorityFeatures:\n";
        std::cout << "\t\tmemoryPriority  : " << !!memoryPriorityFeatures.memoryPriority << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_mesh_shader" ) )
      {
        vk::PhysicalDeviceMeshShaderFeaturesNV const & meshShaderFeatures = features2.get<vk::PhysicalDeviceMeshShaderFeaturesNV>();
        std::cout << "\tMeshShaderFeatures:\n";
        std::cout << "\t\tmeshShader  : " << !!meshShaderFeatures.meshShader << "\n";
        std::cout << "\t\ttaskShader  : " << !!meshShaderFeatures.taskShader << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceMultiviewFeatures const & multiviewFeatures = features2.get<vk::PhysicalDeviceMultiviewFeatures>();
      std::cout << "\tMultiviewFeatures:\n";
      std::cout << "\t\tmultiview                   : " << !!multiviewFeatures.multiview << "\n";
      std::cout << "\t\tmultiviewGeometryShader     : " << !!multiviewFeatures.multiviewGeometryShader << "\n";
      std::cout << "\t\tmultiviewTessellationShader : " << !!multiviewFeatures.multiviewTessellationShader << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_pipeline_executable_properties" ) )
      {
        vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR const & pipelineExecutablePropertiesFeatures =
          features2.get<vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR>();
        std::cout << "\tPipelineExectuablePropertiesFeatures:\n";
        std::cout << "\t\tpipelineExecutableInfo  : " << !!pipelineExecutablePropertiesFeatures.pipelineExecutableInfo << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceProtectedMemoryFeatures const & protectedMemoryFeatures = features2.get<vk::PhysicalDeviceProtectedMemoryFeatures>();
      std::cout << "\tProtectedMemoryFeatures:\n";
      std::cout << "\t\tprotectedMemory : " << !!protectedMemoryFeatures.protectedMemory << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_NV_representative_fragment_test" ) )
      {
        vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV const & representativeFragmentTestFeatures =
          features2.get<vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV>();
        std::cout << "\tRepresentativeFragmentTestFeatures:\n";
        std::cout << "\t\trepresentativeFragmentTest  : " << !!representativeFragmentTestFeatures.representativeFragmentTest << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceSamplerYcbcrConversionFeatures const & samplerYcbcrConversionFeatures =
        features2.get<vk::PhysicalDeviceSamplerYcbcrConversionFeatures>();
      std::cout << "\tSamplerYcbcrConversionFeatures:\n";
      std::cout << "\t\tsamplerYcbcrConversion  : " << !!samplerYcbcrConversionFeatures.samplerYcbcrConversion << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_EXT_scalar_block_layout" ) )
      {
        vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT const & scalarBlockLayoutFeatures = features2.get<vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT>();
        std::cout << "\tScalarBlockLayoutFeatures:\n";
        std::cout << "\t\tscalarBlockLayout : " << !!scalarBlockLayoutFeatures.scalarBlockLayout << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_atomic_int64" ) )
      {
        vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR const & shaderAtomicInt64Features = features2.get<vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR>();
        std::cout << "\tShaderAtomicInt64Features:\n";
        std::cout << "\t\tshaderBufferInt64Atomics  : " << !!shaderAtomicInt64Features.shaderBufferInt64Atomics << "\n";
        std::cout << "\t\tshaderSharedInt64Atomics  : " << !!shaderAtomicInt64Features.shaderSharedInt64Atomics << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_shader_demote_to_helper_invocation" ) )
      {
        vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT const & shaderDemoteToHelperInvocationFeatures =
          features2.get<vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT>();
        std::cout << "\tShaderDemoteToHelperInvocationFeatures:\n";
        std::cout << "\t\tshaderDemoteToHelperInvocation  : " << !!shaderDemoteToHelperInvocationFeatures.shaderDemoteToHelperInvocation << "\n";
        std::cout << "\n";
      }

      vk::PhysicalDeviceShaderDrawParametersFeatures const & shaderDrawParametersFeature = features2.get<vk::PhysicalDeviceShaderDrawParametersFeatures>();
      std::cout << "\tShaderDrawParametersFeature:\n";
      std::cout << "\t\tshaderDrawParameters  : " << !!shaderDrawParametersFeature.shaderDrawParameters << "\n";
      std::cout << "\n";

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_float16_int8" ) )
      {
        vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR const & shaderFloat16Int8Features = features2.get<vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR>();
        std::cout << "\tShaderFloat16Int8Features:\n";
        std::cout << "\t\tshaderFloat16 : " << !!shaderFloat16Int8Features.shaderFloat16 << "\n";
        std::cout << "\t\tshaderInt8    : " << !!shaderFloat16Int8Features.shaderInt8 << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_image_footprint" ) )
      {
        vk::PhysicalDeviceShaderImageFootprintFeaturesNV const & shaderImageFootprintFeatures =
          features2.get<vk::PhysicalDeviceShaderImageFootprintFeaturesNV>();
        std::cout << "\tShaderImageFootprintFeatures:\n";
        std::cout << "\t\timageFootprint  : " << !!shaderImageFootprintFeatures.imageFootprint << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_INTEL_shader_integer_functions2" ) )
      {
        vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL const & shaderIntegerFunctions2Features =
          features2.get<vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL>();
        std::cout << "\tShaderIntegerFunctions2Features:\n";
        std::cout << "\t\tshaderIntegerFunctions2 : " << !!shaderIntegerFunctions2Features.shaderIntegerFunctions2 << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shader_sm_builtins" ) )
      {
        vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV const & shaderSMBuiltinsFeatures = features2.get<vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV>();
        std::cout << "\tShaderSMBuiltinsFeatures:\n";
        std::cout << "\t\tshaderSMBuiltins  : " << !!shaderSMBuiltinsFeatures.shaderSMBuiltins << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_shader_subgroup_extended_types" ) )
      {
        vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR const & shaderSubgroupExtendedTypesFeatures =
          features2.get<vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR>();
        std::cout << "\tShaderSubgroupExtendedTypeFeatures:\n";
        std::cout << "\t\tshaderSubgroupExtendedTypes : " << !!shaderSubgroupExtendedTypesFeatures.shaderSubgroupExtendedTypes << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_NV_shading_rate_image" ) )
      {
        vk::PhysicalDeviceShadingRateImageFeaturesNV const & shadingRateImageFeatures = features2.get<vk::PhysicalDeviceShadingRateImageFeaturesNV>();
        std::cout << "\tShadingRateImageFeatures:\n";
        std::cout << "\t\tshadingRateCoarseSampleOrder  : " << !!shadingRateImageFeatures.shadingRateCoarseSampleOrder << "\n";
        std::cout << "\t\tshadingRateImage              : " << !!shadingRateImageFeatures.shadingRateImage << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_subgroup_size_control" ) )
      {
        vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT const & subgroupSizeControlFeatures =
          features2.get<vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT>();
        std::cout << "\tSubgroupSizeControlFeatures:\n";
        std::cout << "\t\tcomputeFullSubgroups  : " << !!subgroupSizeControlFeatures.computeFullSubgroups << "\n";
        std::cout << "\t\tsubgroupSizeControl   : " << !!subgroupSizeControlFeatures.subgroupSizeControl << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_texel_buffer_alignment" ) )
      {
        vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT const & texelBufferAlignmentFeatures =
          features2.get<vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>();
        std::cout << "\tTexelBufferAlignmentFeatures:\n";
        std::cout << "\t\ttexelBufferAlignment  : " << !!texelBufferAlignmentFeatures.texelBufferAlignment << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_texture_compression_astc_hdr" ) )
      {
        vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT const & textureCompressionASTCHDRFeatures =
          features2.get<vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT>();
        std::cout << "\tTextureCompressionASTCHHRFeatures:\n";
        std::cout << "\t\ttextureCompressionASTC_HDR  : " << !!textureCompressionASTCHDRFeatures.textureCompressionASTC_HDR << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_timeline_semaphore" ) )
      {
        vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR const & timelineSemaphoreFeatures = features2.get<vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR>();
        std::cout << "\tTimelineSemaphoreFeatures:\n";
        std::cout << "\t\ttimelineSemaphore :" << !!timelineSemaphoreFeatures.timelineSemaphore << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_transform_feedback" ) )
      {
        vk::PhysicalDeviceTransformFeedbackFeaturesEXT const & transformFeedbackFeatures = features2.get<vk::PhysicalDeviceTransformFeedbackFeaturesEXT>();
        std::cout << "\tTransformFeedbackFeatures:\n";
        std::cout << "\t\tgeometryStreams : " << !!transformFeedbackFeatures.geometryStreams << "\n";
        std::cout << "\t\ttransformFeedback : " << !!transformFeedbackFeatures.transformFeedback << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_uniform_buffer_standard_layout" ) )
      {
        vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR const & uniformBufferStandardLayoutFeatures =
          features2.get<vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR>();
        std::cout << "\tUniformBufferStandardLayoutFeatures:\n";
        std::cout << "\t\tuniformBufferStandardLayout : " << !!uniformBufferStandardLayoutFeatures.uniformBufferStandardLayout << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_variable_pointers" ) )
      {
        vk::PhysicalDeviceVariablePointersFeatures const & variablePointersFeatures = features2.get<vk::PhysicalDeviceVariablePointersFeatures>();
        std::cout << "\tVariablePointersFeatures:\n";
        std::cout << "\t\tvariablePointers              : " << !!variablePointersFeatures.variablePointers << "\n";
        std::cout << "\t\tvariablePointersStorageBuffer : " << !!variablePointersFeatures.variablePointersStorageBuffer << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_EXT_vertex_attribute_divisor" ) )
      {
        vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT const & vertexAttributeDivisorFeatures =
          features2.get<vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT>();
        std::cout << "\tVertexAttributeDivisorFeature:\n";
        std::cout << "\t\tvertexAttributeInstanceRateDivisor      : " << !!vertexAttributeDivisorFeatures.vertexAttributeInstanceRateDivisor << "\n";
        std::cout << "\t\tvertexAttributeInstanceRateZeroDivisor  : " << !!vertexAttributeDivisorFeatures.vertexAttributeInstanceRateZeroDivisor << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_vulkan_memory_model" ) )
      {
        vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR const & vulkanMemoryModelFeatures = features2.get<vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR>();
        std::cout << "\tVulkanMemoryModelFeatures:\n";
        std::cout << "\t\tvulkanMemoryModel                             : " << !!vulkanMemoryModelFeatures.vulkanMemoryModel << "\n";
        std::cout << "\t\tvulkanMemoryModelAvailabilityVisibilityChains : " << !!vulkanMemoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains
                  << "\n";
        std::cout << "\t\tvulkanMemoryModelDeviceScope                  : " << !!vulkanMemoryModelFeatures.vulkanMemoryModelDeviceScope << "\n";
        std::cout << "\n";
      }

      if ( vk::su::contains( extensionProperties, "VK_KHR_sampler_ycbcr_conversion" ) )
      {
        vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT const & ycbcrImageArraysFeatures = features2.get<vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>();
        std::cout << "\tYcbcrImageArraysFeatures:\n";
        std::cout << "\t\tycbcrImageArrays  : " << !!ycbcrImageArraysFeatures.ycbcrImageArrays << "\n";
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
