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

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "14_InitPipeline";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickSurfaceFormat(physicalDevices[0].getSurfaceFormatsKHR(surfaceData.surface.get())).format, vk::Format::eD16Unorm);

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device, { {vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex} });
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C);
    glslang::FinalizeProcess();

    /* VULKAN_KEY_START */

    vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] =
    {
      vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"),
      vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main")
    };

    vk::VertexInputBindingDescription vertexInputBindingDescription(0, sizeof(coloredCubeData[0]));
    vk::VertexInputAttributeDescription vertexInputAttributeDescriptions[2] =
    {
      vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
      vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, 16)
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo
    (
      vk::PipelineVertexInputStateCreateFlags(),  // flags
      1,                                          // vertexBindingDescriptionCount
      &vertexInputBindingDescription,             // pVertexBindingDescription
      2,                                          // vertexAttributeDescriptionCount
      vertexInputAttributeDescriptions            // pVertexAttributeDescriptions
    );

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo
    (
      vk::PipelineRasterizationStateCreateFlags(),  // flags
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

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

    vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo
    (
      vk::PipelineDepthStencilStateCreateFlags(), // flags
      true,                                       // depthTestEnable
      true,                                       // depthWriteEnable
      vk::CompareOp::eLessOrEqual,                // depthCompareOp
      false,                                      // depthBoundTestEnable
      false,                                      // stencilTestEnable
      stencilOpState,                             // front
      stencilOpState                              // back
    );

    vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState
    (
      false,                      // blendEnable
      vk::BlendFactor::eZero,     // srcColorBlendFactor
      vk::BlendFactor::eZero,     // dstColorBlendFactor
      vk::BlendOp::eAdd,          // colorBlendOp
      vk::BlendFactor::eZero,     // srcAlphaBlendFactor
      vk::BlendFactor::eZero,     // dstAlphaBlendFactor
      vk::BlendOp::eAdd,          // alphaBlendOp
      colorComponentFlags         // colorWriteMask
    );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo
    (
      vk::PipelineColorBlendStateCreateFlags(),   // flags
      false,                                      // logicOpEnable
      vk::LogicOp::eNoOp,                         // logicOp
      1,                                          // attachmentCount
      &pipelineColorBlendAttachmentState,         // pAttachments
      { { (1.0f, 1.0f, 1.0f, 1.0f) } }            // blendConstants
    );

    vk::DynamicState dynamicStates[2] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), 2, dynamicStates);

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo
    (
      vk::PipelineCreateFlags(),                  // flags
      2,                                          // stageCount
      pipelineShaderStageCreateInfos,             // pStages
      &pipelineVertexInputStateCreateInfo,        // pVertexInputState
      &pipelineInputAssemblyStateCreateInfo,      // pInputAssemblyState
      nullptr,                                    // pTessellationState
      &pipelineViewportStateCreateInfo,           // pViewportState
      &pipelineRasterizationStateCreateInfo,      // pRasterizationState
      &pipelineMultisampleStateCreateInfo,        // pMultisampleState
      &pipelineDepthStencilStateCreateInfo,       // pDepthStencilState
      &pipelineColorBlendStateCreateInfo,         // pColorBlendState
      &pipelineDynamicStateCreateInfo,            // pDynamicState
      pipelineLayout.get(),                       // layout
      renderPass.get()                            // renderPass
    );

    vk::UniquePipeline pipeline = device->createGraphicsPipelineUnique(nullptr, graphicsPipelineCreateInfo);

    /* VULKAN_KEY_END */

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    DestroyWindow(surfaceData.window);
#else
#pragma error "unhandled platform"
#endif
  }
  catch (vk::SystemError err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (std::runtime_error err)
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
