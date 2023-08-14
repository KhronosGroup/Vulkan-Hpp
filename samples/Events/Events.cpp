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
// VulkanHpp Samples : Events
//                     Use basic events

#include "../utils/utils.hpp"

#include <iostream>

static char const * AppName    = "Events";
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

    uint32_t   graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex( physicalDevice.getQueueFamilyProperties() );
    vk::Device device                   = vk::su::createDevice( physicalDevice, graphicsQueueFamilyIndex );

    vk::CommandPool   commandPool = device.createCommandPool( { {}, graphicsQueueFamilyIndex } );
    vk::CommandBuffer commandBuffer =
      device.allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 ) ).front();

    vk::Queue graphicsQueue = device.getQueue( graphicsQueueFamilyIndex, 0 );

    /* VULKAN_KEY_START */

    // Start with a trivial command buffer and make sure fence wait doesn't time out
    commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    commandBuffer.setViewport( 0, vk::Viewport( 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 1.0f ) );
    commandBuffer.end();

    vk::Fence fence = device.createFence( vk::FenceCreateInfo() );

    vk::SubmitInfo submitInfo( {}, {}, commandBuffer );
    graphicsQueue.submit( submitInfo, fence );

    // Make sure timeout is long enough for a simple command buffer without waiting for an event
    vk::Result result;
    int        timeouts = -1;
    do
    {
      result = device.waitForFences( fence, true, vk::su::FenceTimeout );
      timeouts++;
    } while ( result == vk::Result::eTimeout );
    assert( result == vk::Result::eSuccess );
    if ( timeouts != 0 )
    {
      std::cout << "Unsuitable timeout value, exiting\n";
      exit( -1 );
    }

    // Now create an event and wait for it on the GPU
    vk::Event event = device.createEvent( vk::EventCreateInfo( vk::EventCreateFlags() ) );

    // reset the command buffer by resetting the complete command pool
    device.resetCommandPool( commandPool );

    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    commandBuffer.waitEvents( event, vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eBottomOfPipe, nullptr, nullptr, nullptr );
    commandBuffer.end();
    device.resetFences( fence );

    // Note that stepping through this code in the debugger is a bad idea because the GPU can TDR waiting for the event.
    // Execute the code from vk::Queue::submit() through vk::Device::setEvent() without breakpoints
    graphicsQueue.submit( submitInfo, fence );

    // We should timeout waiting for the fence because the GPU should be waiting on the event
    result = device.waitForFences( fence, true, vk::su::FenceTimeout );
    if ( result != vk::Result::eTimeout )
    {
      std::cout << "Didn't get expected timeout in vk::Device::waitForFences, exiting\n";
      exit( -1 );
    }

    // Set the event from the CPU and wait for the fence.
    // This should succeed since we set the event
    device.setEvent( event );
    do
    {
      result = device.waitForFences( fence, true, vk::su::FenceTimeout );
    } while ( result == vk::Result::eTimeout );
    assert( result == vk::Result::eSuccess );

    device.resetFences( fence );
    device.resetEvent( event );

    // reset the command buffer by resetting the complete command pool
    device.resetCommandPool( commandPool );

    // Now set the event from the GPU and wait on the CPU
    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    commandBuffer.setEvent( event, vk::PipelineStageFlagBits::eBottomOfPipe );
    commandBuffer.end();

    // Look for the event on the CPU. It should be vk::Result::eEventReset since we haven't sent the command buffer yet.
    result = device.getEventStatus( event );
    assert( result == vk::Result::eEventReset );

    // Send the command buffer and loop waiting for the event
    graphicsQueue.submit( submitInfo, fence );

    int polls = 0;
    do
    {
      result = device.getEventStatus( event );
      polls++;
    } while ( result != vk::Result::eEventSet );
    printf( "%d polls to find the event set\n", polls );

    do
    {
      result = device.waitForFences( fence, true, vk::su::FenceTimeout );
    } while ( result == vk::Result::eTimeout );
    assert( result == vk::Result::eSuccess );

    device.destroyEvent( event );
    device.destroyFence( fence );

    /* VULKAN_KEY_END */

    device.freeCommandBuffers( commandPool, commandBuffer );
    device.destroyCommandPool( commandPool );
    device.destroy();
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
