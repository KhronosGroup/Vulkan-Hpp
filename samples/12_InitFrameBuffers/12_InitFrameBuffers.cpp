// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 12_InitFrameBuffers
//                     Initialize framebuffers

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "12_InitFrameBuffers";
static char const * EngineName = "Vulkan.hpp";

int main()
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 64, 64 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::Device                    device = vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device,
                                         surfaceData.surface,
                                         surfaceData.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                         nullptr,
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::RenderPass renderPass = vk::su::createRenderPass( device, swapChainData.colorFormat, depthBufferData.format );

    /* VULKAN_KEY_START */

    std::array<vk::ImageView, 2> attachments;
    attachments[1] = depthBufferData.imageView;

    vk::FramebufferCreateInfo framebufferCreateInfo(
      vk::FramebufferCreateFlags(), renderPass, attachments, surfaceData.extent.width, surfaceData.extent.height, 1 );
    std::vector<vk::Framebuffer> framebuffers;
    framebuffers.reserve( swapChainData.imageViews.size() );
    for ( auto const & imageView : swapChainData.imageViews )
    {
      attachments[0] = imageView;
      framebuffers.push_back( device.createFramebuffer( framebufferCreateInfo ) );
    }

    for ( auto const & framebuffer : framebuffers )
    {
      device.destroyFramebuffer( framebuffer );
    }

    /* VULKAN_KEY_END */

    device.destroyRenderPass( renderPass );
    depthBufferData.clear( device );
    swapChainData.clear( device );
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
