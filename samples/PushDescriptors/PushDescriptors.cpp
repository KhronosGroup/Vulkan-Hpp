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
// VulkanHpp Samples : PushDescriptors
//                     Use Push Descriptors to Draw Textured Cube

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "PushDescriptors";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    /* VULKAN_KEY_START */

    // To use PUSH_DESCRIPTOR, you must also specify GET_PHYSICAL_DEVICE_PROPERTIES_2
    std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
    if (std::find_if(extensionProperties.begin(), extensionProperties.end(), [](auto ep) { return (strcmp(ep.extensionName,VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0); }) == extensionProperties.end())
    {
      std::cout << "No GET_PHYSICAL_DEVICE_PROPERTIES_2 extension" << std::endl;
      return 0;
    }

    std::vector<std::string> instanceExtensions = vk::su::getInstanceExtensions();
    instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    vk::UniqueInstance instance = vk::su::createInstance(AppName, EngineName, instanceExtensions);
#if !defined(NDEBUG)
    vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);
#endif

    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    assert(!physicalDevices.empty());

    // Once instance is created, need to make sure the extension is available
    extensionProperties = physicalDevices[0].enumerateDeviceExtensionProperties();
    if (std::find_if(extensionProperties.begin(), extensionProperties.end(), [](auto ep) { return (strcmp(ep.extensionName,VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME) == 0); }) == extensionProperties.end())
    {
      std::cout << "No extension for push descriptors" << std::endl;
      return 0;
    }

    std::vector<std::string> deviceExtensions = vk::su::getDeviceExtensions();
    deviceExtensions.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, deviceExtensions);

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

    // Need to specify that descriptor set layout will be for push descriptors
    vk::UniqueDescriptorSetLayout descriptorSetLayout = vk::su::createDescriptorSetLayout(device,
      { {vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex}, {vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment} },
      vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR);
    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    vk::UniqueRenderPass renderPass = vk::su::createRenderPass(device, vk::su::pickSurfaceFormat(physicalDevices[0].getSurfaceFormatsKHR(surfaceData.surface.get())).format, depthBufferData.format);

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent);

    vk::su::BufferData vertexBufferData(physicalDevices[0], device, sizeof(texturedCubeData), vk::BufferUsageFlagBits::eVertexBuffer);
    vk::su::copyToDevice(device, vertexBufferData.deviceMemory, texturedCubeData, sizeof(texturedCubeData) / sizeof(texturedCubeData[0]));

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
    vk::UniquePipeline graphicsPipeline = vk::su::createGraphicsPipeline(device, pipelineCache, vertexShaderModule, fragmentShaderModule, sizeof(texturedCubeData[0]), true, true, pipelineLayout, renderPass);

    // Get the index of the next available swapchain image:
    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());
    vk::ResultValue<uint32_t> currentBuffer = device->acquireNextImageKHR(swapChainData.swapChain.get(), vk::su::FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
    assert(currentBuffer.result == vk::Result::eSuccess);
    assert(currentBuffer.value < framebuffers.size());

    vk::ClearValue clearValues[2];
    clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
    vk::RenderPassBeginInfo renderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer.value].get(), vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent), 2, clearValues);
    commandBuffers[0]->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffers[0]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());

    vk::DescriptorBufferInfo bufferInfo(uniformBufferData.buffer.get(), 0, sizeof(glm::mat4x4));
    vk::DescriptorImageInfo imageInfo(textureData.textureSampler.get(), textureData.imageData->imageView.get(), vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::WriteDescriptorSet writeDescriptorSets[2] =
    {
      vk::WriteDescriptorSet({}, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo),
      vk::WriteDescriptorSet({}, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo)
    };

    // this call is from an extension and needs the dynamic dispatcher !!
    commandBuffers[0]->pushDescriptorSetKHR(vk::PipelineBindPoint::eGraphics, *pipelineLayout, 0, { 2, writeDescriptorSets }, vk::DispatchLoaderDynamic(*instance, *device));

    vk::DeviceSize offset = 0;
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

    /* VULKAN_KEY_END */

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
