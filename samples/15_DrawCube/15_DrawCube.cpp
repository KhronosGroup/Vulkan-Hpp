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
// VulkanHpp Samples : 15_DrawCube
//                     Draw a cube

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "15_DrawCube";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char * /*argv[]*/)
{
  try
  {
    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, vk::su::getInstanceExtensions());
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevices[0].getMemoryProperties();

    uint32_t width = 500;
    uint32_t height = 500;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HWND window = vk::su::initializeWindow(AppName, AppName, width, height);
    vk::UniqueSurfaceKHR surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR({}, GetModuleHandle(nullptr), window));
#else
#pragma error "unhandled platform"
#endif

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::Format colorFormat = vk::su::pickColorFormat(physicalDevices[0].getSurfaceFormatsKHR(surface.get()));
    vk::UniqueSwapchainKHR swapChain = vk::su::createSwapChain(physicalDevices[0], surface, device, width, height, colorFormat, graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);
    std::vector<vk::UniqueImageView> swapChainImageViews = vk::su::createSwapChainImageViews(device, swapChain, colorFormat);

    vk::Format depthFormat = vk::Format::eD16Unorm;
    vk::UniqueImage depthImage = vk::su::createImage(device, depthFormat, width, height);
    vk::UniqueDeviceMemory depthMemory = vk::su::allocateMemory(device, memoryProperties, device->getImageMemoryRequirements(depthImage.get()), vk::MemoryPropertyFlagBits::eDeviceLocal);
    device->bindImageMemory(depthImage.get(), depthMemory.get(), 0);
    vk::UniqueImageView depthViewImage = vk::su::createImageView(device, depthImage, depthFormat);

    glm::mat4x4 mvpc = vk::su::createModelViewProjectionClipMatrix();
    vk::UniqueBuffer uniformBuffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), sizeof(mvpc), vk::BufferUsageFlagBits::eUniformBuffer));
    vk::UniqueDeviceMemory uniformMemory = vk::su::allocateMemory(device, physicalDevices[0].getMemoryProperties(), device->getBufferMemoryRequirements(uniformBuffer.get()), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    vk::su::copyToDevice(device, uniformMemory, mvpc);
    device->bindBufferMemory(uniformBuffer.get(), uniformMemory.get(), 0);

    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device);
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickColorFormat(physicalDevices[0].getSurfaceFormatsKHR(surface.get())), vk::Format::eD16Unorm);

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainImageViews, depthViewImage, width, height);

    vk::UniqueBuffer vertexBuffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), sizeof(coloredCubeData), vk::BufferUsageFlagBits::eVertexBuffer));
    vk::MemoryRequirements memoryRequirements = device->getBufferMemoryRequirements(vertexBuffer.get());
    vk::UniqueDeviceMemory vertexDeviceMemory = vk::su::allocateMemory(device, memoryProperties, memoryRequirements, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    vk::su::copyToDevice(device, vertexDeviceMemory, coloredCubeData, sizeof(coloredCubeData) / sizeof(coloredCubeData[0]));
    device->bindBufferMemory(vertexBuffer.get(), vertexDeviceMemory.get(), 0);

    vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, 1);
    vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique(vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, 1, &poolSize));
    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(descriptorPool.get(), 1, &descriptorSetLayout.get()));
    vk::DescriptorBufferInfo descriptorBufferInfo(uniformBuffer.get(), 0, sizeof(mvpc));
    device->updateDescriptorSets(vk::WriteDescriptorSet(descriptorSets[0].get(), 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &descriptorBufferInfo), nullptr);

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
    vk::UniquePipeline graphicsPipeline = vk::su::createGraphicsPipeline(device, pipelineCache, vertexShaderModule, fragmentShaderModule, sizeof(coloredCubeData[0]), pipelineLayout, renderPass);

    /* VULKAN_KEY_START */

    // Get the index of the next available swapchain image:
    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
    assert(currentBuffer.result == vk::Result::eSuccess);
    assert(currentBuffer.value < framebuffers.size());

    commandBuffers[0]->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

    vk::ClearValue clearValues[2];
    clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
    vk::RenderPassBeginInfo renderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(width, height)), 2, clearValues);
    commandBuffers[0]->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffers[0]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());
    commandBuffers[0]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[0].get(), nullptr);

    vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f);
    commandBuffers[0]->setViewport(0, viewport);

    vk::Rect2D scissor(vk::Offset2D(0, 0), vk::Extent2D(width, height));
    commandBuffers[0]->setScissor(0, scissor);

    vk::DeviceSize offset = 0;
    commandBuffers[0]->bindVertexBuffers(0, vertexBuffer.get(), offset);

    commandBuffers[0]->draw(12 * 3, 1, 0, 0);
    commandBuffers[0]->endRenderPass();
    commandBuffers[0]->end();

    vk::UniqueFence drawFence = device->createFenceUnique(vk::FenceCreateInfo());

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo submitInfo(1, &imageAcquiredSemaphore.get(), &waitDestinationStageMask, 1, &commandBuffers[0].get());
    graphicsQueue.submit(submitInfo, drawFence.get());

    while (vk::Result::eTimeout == device->waitForFences(drawFence.get(), VK_TRUE, vk::su::FenceTimeout))
      ;

    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChain.get(), &currentBuffer.value));
    Sleep(1000);

    /* VULKAN_KEY_END */

    device->waitIdle();
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    DestroyWindow(window);
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
