// SPDX-FileCopyrightText: 2018-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Samples : 10_InitRenderPass
//                     Initialize a render pass

#if defined( _MSC_VER )
#  pragma warning( disable : 4201 )  // disable warning C4201: nonstandard extension used: nameless struct/union; needed
                                     // to get glm/detail/type_vec?.hpp without warnings
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/utils.hpp"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

static char const * AppName    = "10_InitRenderPass";
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

    vk::Format colorFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format;
    vk::Format depthFormat = vk::Format::eD16Unorm;

    /* VULKAN_HPP_KEY_START */

    std::array<vk::AttachmentDescription, 2> attachmentDescriptions;
    attachmentDescriptions[0] = vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                                           colorFormat,
                                                           vk::SampleCountFlagBits::e1,
                                                           vk::AttachmentLoadOp::eClear,
                                                           vk::AttachmentStoreOp::eStore,
                                                           vk::AttachmentLoadOp::eDontCare,
                                                           vk::AttachmentStoreOp::eDontCare,
                                                           vk::ImageLayout::eUndefined,
                                                           vk::ImageLayout::ePresentSrcKHR );
    attachmentDescriptions[1] = vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                                           depthFormat,
                                                           vk::SampleCountFlagBits::e1,
                                                           vk::AttachmentLoadOp::eClear,
                                                           vk::AttachmentStoreOp::eDontCare,
                                                           vk::AttachmentLoadOp::eDontCare,
                                                           vk::AttachmentStoreOp::eDontCare,
                                                           vk::ImageLayout::eUndefined,
                                                           vk::ImageLayout::eDepthStencilAttachmentOptimal );

    vk::AttachmentReference colorReference( 0, vk::ImageLayout::eColorAttachmentOptimal );
    vk::AttachmentReference depthReference( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );
    vk::SubpassDescription  subpass( vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorReference, {}, &depthReference );

    vk::RenderPass renderPass = device.createRenderPass( vk::RenderPassCreateInfo( vk::RenderPassCreateFlags(), attachmentDescriptions, subpass ) );

    device.destroyRenderPass( renderPass );

    /* VULKAN_HPP_KEY_END */

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
