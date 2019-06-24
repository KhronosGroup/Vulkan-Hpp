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
// VulkanHpp Samples : ImmutableSampler
//                     Use an immutable sampler to texture a cube.

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "ImmutableSampler";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevice, *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    vk::UniqueCommandBuffer commandBuffer = std::move(device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1)).front());

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::su::SwapChainData swapChainData(physicalDevice, device, *surfaceData.surface, surfaceData.extent, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                        vk::UniqueSwapchainKHR(), graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);

    vk::su::DepthBufferData depthBufferData(physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent);

    vk::su::BufferData uniformBufferData(physicalDevice, device, sizeof(glm::mat4x4), vk::BufferUsageFlagBits::eUniformBuffer);
    vk::su::copyToDevice(device, uniformBufferData.deviceMemory, vk::su::createModelViewProjectionClipMatrix(surfaceData.extent));

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surfaceData.surface.get())).format, depthBufferData.format);

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent);

    vk::su::BufferData vertexBufferData(physicalDevice, device, sizeof(texturedCubeData), vk::BufferUsageFlagBits::eVertexBuffer);
    vk::su::copyToDevice(device, vertexBufferData.deviceMemory, texturedCubeData, sizeof(texturedCubeData) / sizeof(texturedCubeData[0]));

    /* VULKAN_KEY_START */

    vk::su::TextureData textureData(physicalDevice, device);

    commandBuffer->begin(vk::CommandBufferBeginInfo());
    textureData.setImage(device, commandBuffer, vk::su::CheckerboardImageGenerator());

    vk::DescriptorSetLayoutBinding bindings[2] =
    {
      vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex),
      vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, &textureData.textureSampler.get())
    };
    vk::UniqueDescriptorSetLayout descriptorSetLayout = device->createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), 2, bindings));

    // Create pipeline layout
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    // Create a single pool to contain data for our descriptor set
    vk::DescriptorPoolSize poolSizes[2] =
    {
      vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1),
      vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1)
    };
    vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique(vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, 2, poolSizes));

    // Populate descriptor sets
    vk::UniqueDescriptorSet descriptorSet = std::move(device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(*descriptorPool, 1, &*descriptorSetLayout)).front());

    vk::DescriptorBufferInfo bufferInfo(uniformBufferData.buffer.get(), 0, sizeof(glm::mat4x4));
    vk::DescriptorImageInfo imageInfo(textureData.textureSampler.get(), textureData.imageData->imageView.get(), vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::WriteDescriptorSet writeDescriptorSets[2] =
    {
      vk::WriteDescriptorSet(descriptorSet.get(), 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo),
      vk::WriteDescriptorSet(descriptorSet.get(), 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo)
    };
    device->updateDescriptorSets(vk::ArrayProxy<const vk::WriteDescriptorSet>(2, writeDescriptorSets), nullptr);

    /* VULKAN_KEY_END */

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
    vk::UniquePipeline graphicsPipeline = vk::su::createGraphicsPipeline(device, pipelineCache, std::make_pair(*vertexShaderModule, nullptr), std::make_pair(*fragmentShaderModule, nullptr),
                                                                         sizeof(texturedCubeData[0]), { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                         vk::FrontFace::eClockwise, true, pipelineLayout, renderPass);

    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
    assert(currentBuffer.result == vk::Result::eSuccess);
    assert(currentBuffer.value < framebuffers.size());

    vk::ClearValue clearValues[2];
    clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
    vk::RenderPassBeginInfo renderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent), 2, clearValues);
    commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());
    commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSet.get(), nullptr);

    commandBuffer->bindVertexBuffers(0, *vertexBufferData.buffer, {0});
    commandBuffer->setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(surfaceData.extent.width), static_cast<float>(surfaceData.extent.height), 0.0f, 1.0f));
    commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent));

    commandBuffer->draw(12 * 3, 1, 0, 0);
    commandBuffer->endRenderPass();
    commandBuffer->end();

    vk::su::submitAndWait(device, graphicsQueue, commandBuffer);

    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChainData.swapChain.get(), &currentBuffer.value));
    Sleep(1000);

    device->waitIdle();
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
