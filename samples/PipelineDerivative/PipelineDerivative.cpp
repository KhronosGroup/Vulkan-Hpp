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
// VulkanHpp Samples : PipelineDerivative
//                     This sample creates pipeline derivative and draws with it.

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"

// For timestamp code (getMilliseconds)
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

typedef unsigned long long timestamp_t;
timestamp_t getMilliseconds()
{
#ifdef WIN32
  LARGE_INTEGER frequency;
  BOOL useQPC = QueryPerformanceFrequency(&frequency);
  if (useQPC)
  {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (1000LL * now.QuadPart) / frequency.QuadPart;
  }
  else
  {
    return GetTickCount();
  }
#else
  struct timeval now;
  gettimeofday(&now, NULL);
  return (now.tv_usec / 1000) + (timestamp_t)now.tv_sec;
#endif
}


static char const* AppName = "PipelineDerivative";
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

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::su::SwapChainData swapChainData(physicalDevices[0], device, surfaceData.surface, surfaceData.extent, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc
      , graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);

    vk::su::DepthBufferData depthBufferData(physicalDevices[0], device, vk::Format::eD16Unorm, surfaceData.extent);

    vk::su::TextureData textureData(physicalDevices[0], device);
    commandBuffers[0]->begin(vk::CommandBufferBeginInfo());
    textureData.setTexture(device, commandBuffers[0], vk::su::CheckerboardTextureCreator());

    vk::su::BufferData uniformBufferData(physicalDevices[0], device, sizeof(glm::mat4x4), vk::BufferUsageFlagBits::eUniformBuffer);
    vk::su::copyToDevice(device, uniformBufferData.deviceMemory, vk::su::createModelViewProjectionClipMatrix(surfaceData.extent));

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device,
      { {vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex}, {vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment} });
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickSurfaceFormat(physicalDevices[0].getSurfaceFormatsKHR(surfaceData.surface.get())).format, depthBufferData.format);

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent);

    vk::su::BufferData vertexBufferData(physicalDevices[0], device, sizeof(texturedCubeData), vk::BufferUsageFlagBits::eVertexBuffer);
    vk::su::copyToDevice(device, vertexBufferData.deviceMemory, texturedCubeData, sizeof(texturedCubeData) / sizeof(texturedCubeData[0]));

    vk::UniqueDescriptorPool descriptorPool = vk::su::createDescriptorPool(device, { {vk::DescriptorType::eUniformBuffer, 1}, {vk::DescriptorType::eCombinedImageSampler, 1} });
    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(descriptorPool.get(), 1, &descriptorSetLayout.get()));

    vk::DescriptorBufferInfo bufferInfo(uniformBufferData.buffer.get(), 0, sizeof(glm::mat4x4));
    vk::DescriptorImageInfo imageInfo(textureData.textureSampler.get(), textureData.imageData->imageView.get(), vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::su::updateDescriptorSets(device, descriptorSets[0], vk::DescriptorType::eUniformBuffer, &bufferInfo, &imageInfo);

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

    /* VULKAN_KEY_START */

    // Create two pipelines.
    //
    // First pipeline has VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT set.
    // Second pipeline has a modified fragment shader and sets the VK_PIPELINE_CREATE_DERIVATIVE_BIT flag.

    vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] =
    {
      vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"),
      vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main")
    };

    vk::VertexInputBindingDescription vertexInputBindingDescription(0, sizeof(texturedCubeData[0]));
    vk::VertexInputAttributeDescription vertexInputAttributeDescriptions[2] =
    {
      vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
      vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, 16)
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(vk::PipelineVertexInputStateCreateFlags(), 1, &vertexInputBindingDescription, 2, vertexInputAttributeDescriptions);

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(), false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, false, 0.0f, 0.0f, 0.0f, 1.0f);

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

    vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(vk::PipelineDepthStencilStateCreateFlags(), true, true, vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState);

    vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(false, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, colorComponentFlags);
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, 1, &pipelineColorBlendAttachmentState, { { (1.0f, 1.0f, 1.0f, 1.0f) } });

    vk::DynamicState dynamicStates[2] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), 2, dynamicStates);

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlagBits::eAllowDerivatives, 2, pipelineShaderStageCreateInfos, &pipelineVertexInputStateCreateInfo,
      &pipelineInputAssemblyStateCreateInfo, nullptr, &pipelineViewportStateCreateInfo, &pipelineRasterizationStateCreateInfo, &pipelineMultisampleStateCreateInfo,
      &pipelineDepthStencilStateCreateInfo, &pipelineColorBlendStateCreateInfo, &pipelineDynamicStateCreateInfo, pipelineLayout.get(), renderPass.get());

    vk::UniquePipeline basePipeline = device->createGraphicsPipelineUnique(pipelineCache.get(), graphicsPipelineCreateInfo);

    // Now create the derivative pipeline, using a different fragment shader
    // This shader will shade the cube faces with interpolated colors
    const std::string fragmentShaderText_T_C_2 = R"(
#version 450

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(inTexCoord.x, inTexCoord.y, 1.0f - inTexCoord.x - inTexCoord.y, 1.0f);
}
)";

    // Convert GLSL to SPIR-V
    glslang::InitializeProcess();
    vk::UniqueShaderModule fragmentShaderModule2 = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C_2);
    glslang::FinalizeProcess();

    // Modify pipeline info to reflect derivation
    pipelineShaderStageCreateInfos[1] = vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule2.get(), "main");
    graphicsPipelineCreateInfo.flags = vk::PipelineCreateFlagBits::eDerivative;
    graphicsPipelineCreateInfo.basePipelineHandle = basePipeline.get();
    graphicsPipelineCreateInfo.basePipelineIndex = -1;

    // And create the derived pipeline
    vk::UniquePipeline derivedPipeline = device->createGraphicsPipelineUnique(pipelineCache.get(), graphicsPipelineCreateInfo);

    /* VULKAN_KEY_END */

    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));

    // Get the index of the next available swapchain image
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(swapChainData.swapChain.get(), UINT64_MAX, imageAcquiredSemaphore.get(), nullptr);
    assert(currentBuffer.result == vk::Result::eSuccess);
    assert(currentBuffer.value < framebuffers.size());

    vk::ClearValue clearValues[2];
    clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    commandBuffers[0]->beginRenderPass(vk::RenderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D(vk::Offset2D(), surfaceData.extent), 2, clearValues), vk::SubpassContents::eInline);
    commandBuffers[0]->bindPipeline(vk::PipelineBindPoint::eGraphics, derivedPipeline.get());
    commandBuffers[0]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[0].get(), {});

    VkDeviceSize offset = 0;
    commandBuffers[0]->bindVertexBuffers(0, vertexBufferData.buffer.get(), offset);

    vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(surfaceData.extent.width), static_cast<float>(surfaceData.extent.height), 0.0f, 1.0f);
    commandBuffers[0]->setViewport(0, viewport);

    vk::Rect2D scissor(vk::Offset2D(0, 0), surfaceData.extent);
    commandBuffers[0]->setScissor(0, scissor);

    commandBuffers[0]->draw(12 * 3, 1, 0, 0);
    commandBuffers[0]->endRenderPass();
    commandBuffers[0]->end();

    vk::UniqueFence drawFence = device->createFenceUnique(vk::FenceCreateInfo());

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo submitInfo(1, &imageAcquiredSemaphore.get(), &waitDestinationStageMask, 1, &commandBuffers[0].get());
    graphicsQueue.submit(submitInfo, drawFence.get());

    while (vk::Result::eTimeout == device->waitForFences(drawFence.get(), VK_TRUE, vk::su::FenceTimeout))
      ;

    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChainData.swapChain.get(), &currentBuffer.value));
    Sleep(1000);

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
