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
// VulkanHpp Samples : 14_InitPipeline
//                     Initialize graphics pipeline

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
// no need to ignore any warnings with GCC
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"
#include "glslang/Public/ShaderLang.h"

#include <iostream>

static char const * AppName    = "14_InitPipeline";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::Device                    device = vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::RenderPass renderPass =
      vk::su::createRenderPass( device, vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format, vk::Format::eD16Unorm );

    vk::DescriptorSetLayout descriptorSetLayout =
      vk::su::createDescriptorSetLayout( device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    /* VULKAN_KEY_START */

    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main" ),
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main" )
    };

    vk::VertexInputBindingDescription                  vertexInputBindingDescription( 0, sizeof( coloredCubeData[0] ) );
    std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
      vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 ),
      vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32A32Sfloat, 16 )
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo( vk::PipelineVertexInputStateCreateFlags(),  // flags
                                                                               vertexInputBindingDescription,              // vertexBindingDescriptions
                                                                               vertexInputAttributeDescriptions            // vertexAttributeDescriptions
    );

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo( vk::PipelineInputAssemblyStateCreateFlags(),
                                                                                   vk::PrimitiveTopology::eTriangleList );

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo( vk::PipelineRasterizationStateCreateFlags(),  // flags
                                                                                   false,                                        // depthClampEnable
                                                                                   false,                                        // rasterizerDiscardEnable
                                                                                   vk::PolygonMode::eFill,                       // polygonMode
                                                                                   vk::CullModeFlagBits::eBack,                  // cullMode
                                                                                   vk::FrontFace::eClockwise,                    // frontFace
                                                                                   false,                                        // depthBiasEnable
                                                                                   0.0f,                                         // depthBiasConstantFactor
                                                                                   0.0f,                                         // depthBiasClamp
                                                                                   0.0f,                                         // depthBiasSlopeFactor
                                                                                   1.0f                                          // lineWidth
    );

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo( vk::PipelineMultisampleStateCreateFlags(),  // flags
                                                                               vk::SampleCountFlagBits::e1                 // rasterizationSamples
                                                                                                                           // other values can be default
    );

    vk::StencilOpState                      stencilOpState( vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo( vk::PipelineDepthStencilStateCreateFlags(),  // flags
                                                                                 true,                                        // depthTestEnable
                                                                                 true,                                        // depthWriteEnable
                                                                                 vk::CompareOp::eLessOrEqual,                 // depthCompareOp
                                                                                 false,                                       // depthBoundTestEnable
                                                                                 false,                                       // stencilTestEnable
                                                                                 stencilOpState,                              // front
                                                                                 stencilOpState                               // back
    );

    vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                                                 vk::ColorComponentFlagBits::eA );
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,                   // blendEnable
                                                                             vk::BlendFactor::eZero,  // srcColorBlendFactor
                                                                             vk::BlendFactor::eZero,  // dstColorBlendFactor
                                                                             vk::BlendOp::eAdd,       // colorBlendOp
                                                                             vk::BlendFactor::eZero,  // srcAlphaBlendFactor
                                                                             vk::BlendFactor::eZero,  // dstAlphaBlendFactor
                                                                             vk::BlendOp::eAdd,       // alphaBlendOp
                                                                             colorComponentFlags      // colorWriteMask
    );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo( vk::PipelineColorBlendStateCreateFlags(),  // flags
                                                                             false,                                     // logicOpEnable
                                                                             vk::LogicOp::eNoOp,                        // logicOp
                                                                             pipelineColorBlendAttachmentState,         // attachments
                                                                             { { 1.0f, 1.0f, 1.0f, 1.0f } }             // blendConstants
    );

    std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(), dynamicStates );

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo( vk::PipelineCreateFlags(),              // flags
                                                               pipelineShaderStageCreateInfos,         // stages
                                                               &pipelineVertexInputStateCreateInfo,    // pVertexInputState
                                                               &pipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState
                                                               nullptr,                                // pTessellationState
                                                               &pipelineViewportStateCreateInfo,       // pViewportState
                                                               &pipelineRasterizationStateCreateInfo,  // pRasterizationState
                                                               &pipelineMultisampleStateCreateInfo,    // pMultisampleState
                                                               &pipelineDepthStencilStateCreateInfo,   // pDepthStencilState
                                                               &pipelineColorBlendStateCreateInfo,     // pColorBlendState
                                                               &pipelineDynamicStateCreateInfo,        // pDynamicState
                                                               pipelineLayout,                         // layout
                                                               renderPass                              // renderPass
    );

    vk::Result   result;
    vk::Pipeline pipeline;
    std::tie( result, pipeline ) = device.createGraphicsPipeline( nullptr, graphicsPipelineCreateInfo );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::ePipelineCompileRequiredEXT:
        // something meaningfull here
        break;
      default: assert( false );  // should never happen
    }

    device.destroyPipeline( pipeline );

    /* VULKAN_KEY_END */

    device.destroyShaderModule( fragmentShaderModule );
    device.destroyShaderModule( vertexShaderModule );
    device.destroyPipelineLayout( pipelineLayout );
    device.destroyDescriptorSetLayout( descriptorSetLayout );
    device.destroyRenderPass( renderPass );
    device.destroy();
    instance.destroySurfaceKHR( surfaceData.surface );
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
