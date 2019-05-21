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
// VulkanHpp Samples : InputAttachment
//                     Use an input attachment to draw a yellow triangle

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"
#include <iostream>

static char const* AppName = "InputAttachment";
static char const* EngineName = "Vulkan.hpp";

static std::string vertexShaderText = R"(
#version 450

vec2 vertices[3];

void main()
{
  vertices[0] = vec2(-1.0f, -1.0f);
  vertices[1] = vec2( 1.0f, -1.0f);
  vertices[2] = vec2( 0.0f,  1.0f);

  gl_Position = vec4(vertices[gl_VertexIndex % 3], 0.0f, 1.0f);
}
)";

// Use subpassLoad to read from input attachment
static const char *fragmentShaderText = R"(
#version 450

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inputAttachment;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = subpassLoad(inputAttachment);
}
)";
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

    vk::FormatProperties formatProperties = physicalDevices[0].getFormatProperties(vk::Format::eR8G8B8A8Unorm);
    if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment))
    {
      std::cout << "vk::Format::eR8G8B8A8Unorm format unsupported for input attachment\n";
      exit(-1);
    }

    vk::su::SurfaceData surfaceData(instance, AppName, AppName, vk::Extent2D(500, 500));

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevices[0], *surfaceData.surface);
    vk::UniqueDevice device = vk::su::createDevice(physicalDevices[0], graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

    vk::UniqueCommandPool commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
    std::vector<vk::UniqueCommandBuffer> commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

    vk::Queue graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
    vk::Queue presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);

    vk::su::SwapChainData swapChainData(physicalDevices[0], device, surfaceData.surface, surfaceData.extent, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
      graphicsAndPresentQueueFamilyIndex.first, graphicsAndPresentQueueFamilyIndex.second);

    /* VULKAN_KEY_START */

    // Create a framebuffer with 2 attachments, one the color attachment the shaders render into, and the other an input attachment which
    // will be cleared to yellow, and then used by the shaders to color the drawn triangle. Final result should be a yellow triangle

    // Create the image that will be used as the input attachment
    // The image for the color attachment is the presentable image already created as part of the SwapChainData
    vk::UniqueImage inputImage = device->createImageUnique(vk::ImageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, swapChainData.colorFormat, vk::Extent3D(surfaceData.extent, 1), 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eInputAttachment | vk::ImageUsageFlagBits::eTransferDst));

    vk::MemoryRequirements memoryRequirements = device->getImageMemoryRequirements(inputImage.get());
    uint32_t memoryTypeIndex = vk::su::findMemoryType(physicalDevices[0].getMemoryProperties(), memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlags());
    vk::UniqueDeviceMemory inputMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex));
    device->bindImageMemory(inputImage.get(), inputMemory.get(), 0);

    // Set the image layout to TRANSFER_DST_OPTIMAL to be ready for clear
    commandBuffers[0]->begin(vk::CommandBufferBeginInfo());
    vk::su::setImageLayout(commandBuffers[0], inputImage.get(), swapChainData.colorFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

    commandBuffers[0]->clearColorImage(inputImage.get(), vk::ImageLayout::eTransferDstOptimal, vk::ClearColorValue(std::array<float, 4>({ {1.0f, 1.0f, 0.0f, 0.0f} })),
      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS));

    // Set the image layout to SHADER_READONLY_OPTIMAL for use by the shaders
    vk::su::setImageLayout(commandBuffers[0], inputImage.get(), swapChainData.colorFormat, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), inputImage.get(), vk::ImageViewType::e2D, swapChainData.colorFormat, componentMapping, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
    vk::UniqueImageView inputAttachmentView = device->createImageViewUnique(imageViewCreateInfo);

    vk::DescriptorSetLayoutBinding layoutBinding(0, vk::DescriptorType::eInputAttachment, 1, vk::ShaderStageFlagBits::eFragment);
    vk::UniqueDescriptorSetLayout descriptorSetLayout = device->createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &layoutBinding));

    vk::UniquePipelineLayout pipelineLayout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));

    vk::AttachmentDescription attachments[2] =
    {
      // First attachment is the color attachment - clear at the beginning of the renderpass and transition layout to PRESENT_SRC_KHR at the end of renderpass
      vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), swapChainData.colorFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR),
      // Second attachment is input attachment.  Once cleared it should have width*height yellow pixels.
      // Doing a subpassLoad in the fragment shader should give the shader the color at the fragments x,y location from the input attachment
      vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), swapChainData.colorFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eDontCare,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageLayout::eShaderReadOnlyOptimal)
    };
    vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference inputReference(1, vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::SubpassDescription subPass(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 1, &inputReference, 1, &colorReference);
    vk::UniqueRenderPass renderPass = device->createRenderPassUnique(vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), 2, attachments, 1, &subPass));

    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eVertex, vertexShaderText);
    vk::UniqueShaderModule fragmentShaderModule = vk::su::createShaderModule(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText);
    glslang::FinalizeProcess();

    std::vector<vk::UniqueFramebuffer> framebuffers = vk::su::createFramebuffers(device, renderPass, swapChainData.imageViews, inputAttachmentView, surfaceData.extent);

    vk::DescriptorPoolSize poolSize(vk::DescriptorType::eInputAttachment, 1);
    vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique(vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, 1, &poolSize));

    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(descriptorPool.get(), 1, &descriptorSetLayout.get()));

    vk::DescriptorImageInfo inputImageInfo(nullptr, inputAttachmentView.get(), vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::WriteDescriptorSet writeDescriptorSet(descriptorSets[0].get(), 0, 0, 1, vk::DescriptorType::eInputAttachment, &inputImageInfo);
    device->updateDescriptorSets(vk::ArrayProxy<const vk::WriteDescriptorSet>(1, &writeDescriptorSet), nullptr);

    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
    vk::UniquePipeline graphicsPipeline = vk::su::createGraphicsPipeline(device, pipelineCache, vertexShaderModule, fragmentShaderModule, 0, false, false, pipelineLayout, renderPass);

    vk::UniqueSemaphore imageAcquiredSemaphore = device->createSemaphoreUnique(vk::SemaphoreCreateInfo());

    vk::ResultValue<uint32_t> result = device->acquireNextImage2KHR(vk::AcquireNextImageInfoKHR(swapChainData.swapChain.get(), UINT64_MAX, imageAcquiredSemaphore.get(), {}, 1));
    assert(result.result == vk::Result::eSuccess);
    uint32_t currentBuffer = result.value;

    vk::ClearValue clearValue;
    clearValue.color = vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f }));
    commandBuffers[0]->beginRenderPass(vk::RenderPassBeginInfo(renderPass.get(), framebuffers[currentBuffer].get(), vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent), 1, &clearValue), vk::SubpassContents::eInline);
    commandBuffers[0]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());
    commandBuffers[0]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[0].get(), nullptr);

    vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(surfaceData.extent.width), static_cast<float>(surfaceData.extent.height), 0.0f, 1.0f);
    commandBuffers[0]->setViewport(0, viewport);

    vk::Rect2D scissor(vk::Offset2D(0, 0), surfaceData.extent);
    commandBuffers[0]->setScissor(0, scissor);

    commandBuffers[0]->draw(3, 1, 0, 0);
    commandBuffers[0]->endRenderPass();
    commandBuffers[0]->end();

    /* VULKAN_KEY_END */

    vk::su::submitAndWait(device, graphicsQueue, commandBuffers[0]);

    presentQueue.presentKHR(vk::PresentInfoKHR(0, nullptr, 1, &swapChainData.swapChain.get(), &currentBuffer));
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
