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
// VulkanHpp Samples : TexelBuffer
//                     Use a texel buffer to draw a green triangle.

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>
#include <thread>

static char const* AppName = "TexelBuffer";
static char const* EngineName = "Vulkan.hpp";

static const std::string vertexShaderText = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform samplerBuffer texels;
layout (location = 0) out vec4 outColor;

vec2 vertices[3];

void main()
{
  float r = texelFetch(texels, 0).r;
  float g = texelFetch(texels, 1).r;
  float b = texelFetch(texels, 2).r;
  outColor = vec4(r, g, b, 1.0);

  vertices[0] = vec2(-1.0, -1.0);
  vertices[1] = vec2( 1.0, -1.0);
  vertices[2] = vec2( 0.0,  1.0);
  gl_Position = vec4(vertices[gl_VertexIndex % 3], 0.0, 1.0);
}
)";

int main(int /*argc*/, char ** /*argv*/)
{
  const float texels[] = {118.0f / 255.0f, 185.0f / 255.0f, 0.0f};

  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, {}, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger = vk::su::createDebugUtilsMessenger(instance);
#endif

    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
    if (physicalDeviceProperties.limits.maxTexelBufferElements < 4)
    {
      std::cout << "maxTexelBufferElements too small\n";
      exit(-1);
    }

    vk::Format texelFormat = vk::Format::eR32Sfloat;
    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(texelFormat);
    if (!(formatProperties.bufferFeatures & vk::FormatFeatureFlagBits::eUniformTexelBuffer))
    {
      std::cout << "R32_SFLOAT format unsupported for texel buffer\n";
      exit(-1);
    }

    vk::su::SurfaceData surfaceData(instance, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevice, *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    vk::UniqueCommandBuffer commandBuffer = std::move(device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1)).front());

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::su::SwapChainData swapChainData(physicalDevice, device, *surfaceData.surface, surfaceData.extent, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                        vk::UniqueSwapchainKHR(), graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);

    vk::su::BufferData texelBufferData(physicalDevice, device, sizeof(texels), vk::BufferUsageFlagBits::eUniformTexelBuffer);
    texelBufferData.upload(device, texels);

    vk::UniqueBufferView texelBufferView = device->createBufferViewUnique(vk::BufferViewCreateInfo({}, *texelBufferData.buffer, texelFormat, 0, sizeof(texels)));

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device, { {vk::DescriptorType::eUniformTexelBuffer, 1, vk::ShaderStageFlagBits::eVertex} });
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surfaceData.surface.get())).format, vk::Format::eUndefined);

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainData.imageViews, vk::UniqueImageView(), surfaceData.extent);

    vk::UniqueDescriptorPool descriptorPool = vk::su::createDescriptorPool(device, { {vk::DescriptorType::eUniformTexelBuffer, 1} });
    vk::UniqueDescriptorSet descriptorSet = std::move(device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(*descriptorPool, 1, &*descriptorSetLayout)).front());
    vk::su::updateDescriptorSets(device, descriptorSet, {{vk::DescriptorType::eUniformTexelBuffer, texelBufferData.buffer, texelBufferView}}, {});

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
    vk::UniquePipeline graphicsPipeline = vk::su::createGraphicsPipeline(device, pipelineCache, std::make_pair(*vertexShaderModule, nullptr), std::make_pair(*fragmentShaderModule, nullptr),
                                                                         0, {}, vk::FrontFace::eClockwise, false, pipelineLayout, renderPass);

    /* VULKAN_KEY_START */

    // Get the index of the next available swapchain image:
    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
    assert(currentBuffer.result == vk::Result::eSuccess);
    assert(currentBuffer.value < framebuffers.size());

    commandBuffer->begin(vk::CommandBufferBeginInfo());

    vk::ClearValue clearValue;
    clearValue.color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    vk::RenderPassBeginInfo renderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent), 1, &clearValue);

    commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());
    commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSet.get(), nullptr);

    commandBuffer->setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(surfaceData.extent.width), static_cast<float>(surfaceData.extent.height), 0.0f, 1.0f));
    commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent));

    commandBuffer->draw(3, 1, 0, 0);
    commandBuffer->endRenderPass();
    commandBuffer->end();

    vk::UniqueFence drawFence = device->createFenceUnique(vk::FenceCreateInfo());

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo submitInfo(1, &imageAcquiredSemaphore.get(), &waitDestinationStageMask, 1, &commandBuffer.get());
    graphicsQueue.submit(submitInfo, drawFence.get());

    while (vk::Result::eTimeout == device->waitForFences(drawFence.get(), VK_TRUE, vk::su::FenceTimeout))
      ;

    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChainData.swapChain.get(), &currentBuffer.value));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    /* VULKAN_KEY_END */

    device->waitIdle();
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
