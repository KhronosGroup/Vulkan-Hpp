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
// VulkanHpp Samples : PipelineCache
//                     This sample tries to save and reuse pipeline cache data between runs.

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "../utils/geometries.hpp"
#include "../utils/math.hpp"
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <fstream>
#include <iomanip>
#include <thread>

// For timestamp code (getMilliseconds)
#ifdef WIN32
#  include <Windows.h>
#else
#  include <sys/time.h>
#endif

typedef unsigned long long timestamp_t;
timestamp_t                getMilliseconds()
{
#ifdef WIN32
  LARGE_INTEGER frequency;
  BOOL          useQPC = QueryPerformanceFrequency( &frequency );
  if ( useQPC )
  {
    LARGE_INTEGER now;
    QueryPerformanceCounter( &now );
    return ( 1000LL * now.QuadPart ) / frequency.QuadPart;
  }
  else
  {
    return GetTickCount();
  }
#else
  struct timeval now;
  gettimeofday( &now, NULL );
  return ( now.tv_usec / 1000 ) + (timestamp_t)now.tv_sec;
#endif
}

static char const * AppName    = "PipelineCache";
static char const * EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::Instance instance = vk::su::createInstance( AppName, EngineName, {}, vk::su::getInstanceExtensions() );
#if !defined( NDEBUG )
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = instance.createDebugUtilsMessengerEXT( vk::su::makeDebugUtilsMessengerCreateInfoEXT() );
#endif

    vk::PhysicalDevice           physicalDevice = instance.enumeratePhysicalDevices().front();
    vk::PhysicalDeviceProperties properties     = physicalDevice.getProperties();

    vk::su::SurfaceData surfaceData( instance, AppName, vk::Extent2D( 500, 500 ) );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex( physicalDevice, surfaceData.surface );
    vk::Device                    device = vk::su::createDevice( physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions() );

    vk::CommandPool   commandPool = device.createCommandPool( { {}, graphicsAndPresentQueueFamilyIndex.first } );
    vk::CommandBuffer commandBuffer =
      device.allocateCommandBuffers( vk::CommandBufferAllocateInfo( commandPool, vk::CommandBufferLevel::ePrimary, 1 ) ).front();

    vk::Queue graphicsQueue = device.getQueue( graphicsAndPresentQueueFamilyIndex.first, 0 );
    vk::Queue presentQueue  = device.getQueue( graphicsAndPresentQueueFamilyIndex.second, 0 );

    vk::su::SwapChainData swapChainData( physicalDevice,
                                         device,
                                         surfaceData.surface,
                                         surfaceData.extent,
                                         vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
                                         {},
                                         graphicsAndPresentQueueFamilyIndex.first,
                                         graphicsAndPresentQueueFamilyIndex.second );

    vk::su::DepthBufferData depthBufferData( physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent );

    vk::su::TextureData textureData( physicalDevice, device );
    commandBuffer.begin( vk::CommandBufferBeginInfo() );
    textureData.setImage( device, commandBuffer, vk::su::MonochromeImageGenerator( { 118, 185, 0 } ) );

    vk::su::BufferData uniformBufferData( physicalDevice, device, sizeof( glm::mat4x4 ), vk::BufferUsageFlagBits::eUniformBuffer );
    glm::mat4x4        mvpcMatrix = vk::su::createModelViewProjectionClipMatrix( surfaceData.extent );
    vk::su::copyToDevice( device, uniformBufferData.deviceMemory, mvpcMatrix );

    vk::DescriptorSetLayout descriptorSetLayout =
      vk::su::createDescriptorSetLayout( device,
                                         { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
                                           { vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment } } );
    vk::PipelineLayout pipelineLayout = device.createPipelineLayout( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), descriptorSetLayout ) );

    vk::RenderPass renderPass = vk::su::createRenderPass(
      device, vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surfaceData.surface ) ).format, depthBufferData.format );

    glslang::InitializeProcess();
    vk::ShaderModule vertexShaderModule   = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PT_T );
    vk::ShaderModule fragmentShaderModule = vk::su::createShaderModule( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_T_C );
    glslang::FinalizeProcess();

    std::vector<vk::Framebuffer> framebuffers =
      vk::su::createFramebuffers( device, renderPass, swapChainData.imageViews, depthBufferData.imageView, surfaceData.extent );

    vk::su::BufferData vertexBufferData( physicalDevice, device, sizeof( texturedCubeData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::su::copyToDevice( device, vertexBufferData.deviceMemory, texturedCubeData, sizeof( texturedCubeData ) / sizeof( texturedCubeData[0] ) );

    vk::DescriptorPool descriptorPool =
      vk::su::createDescriptorPool( device, { { vk::DescriptorType::eUniformBuffer, 1 }, { vk::DescriptorType::eCombinedImageSampler, 1 } } );
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( descriptorPool, descriptorSetLayout );
    vk::DescriptorSet             descriptorSet = device.allocateDescriptorSets( descriptorSetAllocateInfo ).front();

    vk::su::updateDescriptorSets( device, descriptorSet, { { vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, {} } }, textureData );

    /* VULKAN_KEY_START */

    // Check disk for existing cache data
    size_t startCacheSize = 0;
    char * startCacheData = nullptr;

    std::string   cacheFileName = "pipeline_cache_data.bin";
    std::ifstream readCacheStream( cacheFileName, std::ios_base::in | std::ios_base::binary );
    if ( readCacheStream.good() )
    {
      // Determine cache size
      readCacheStream.seekg( 0, readCacheStream.end );
      startCacheSize = static_cast<size_t>( readCacheStream.tellg() );
      readCacheStream.seekg( 0, readCacheStream.beg );

      // Allocate memory to hold the initial cache data
      startCacheData = (char *)std::malloc( startCacheSize );

      // Read the data into our buffer
      readCacheStream.read( startCacheData, startCacheSize );

      // Clean up and print results
      readCacheStream.close();
      std::cout << "  Pipeline cache HIT!\n";
      std::cout << "  cacheData loaded from " << cacheFileName << "\n";
    }
    else
    {
      // No cache found on disk
      std::cout << "  Pipeline cache miss!\n";
    }

    if ( startCacheData != nullptr )
    {
      // Check for cache validity
      //
      // TODO: Update this as the spec evolves. The fields are not defined by the header.
      //
      // The code below supports SDK 0.10 Vulkan spec, which contains the following table:
      //
      // Offset	 Size            Meaning
      // ------    ------------    ------------------------------------------------------------------
      //      0               4    a device ID equal to VkPhysicalDeviceProperties::DeviceId written
      //                           as a stream of bytes, with the least significant byte first
      //
      //      4    VK_UUID_SIZE    a pipeline cache ID equal to VkPhysicalDeviceProperties::pipelineCacheUUID
      //
      //
      // The code must be updated for latest Vulkan spec, which contains the following table:
      //
      // Offset	 Size            Meaning
      // ------    ------------    ------------------------------------------------------------------
      //      0               4    length in bytes of the entire pipeline cache header written as a
      //                           stream of bytes, with the least significant byte first
      //      4               4    a VkPipelineCacheHeaderVersion value written as a stream of bytes,
      //                           with the least significant byte first
      //      8               4    a vendor ID equal to VkPhysicalDeviceProperties::vendorID written
      //                           as a stream of bytes, with the least significant byte first
      //     12               4    a device ID equal to VkPhysicalDeviceProperties::deviceID written
      //                           as a stream of bytes, with the least significant byte first
      //     16    VK_UUID_SIZE    a pipeline cache ID equal to VkPhysicalDeviceProperties::pipelineCacheUUID

      uint32_t headerLength                    = 0;
      uint32_t cacheHeaderVersion              = 0;
      uint32_t vendorID                        = 0;
      uint32_t deviceID                        = 0;
      uint8_t  pipelineCacheUUID[VK_UUID_SIZE] = {};

      memcpy( &headerLength, (uint8_t *)startCacheData + 0, 4 );
      memcpy( &cacheHeaderVersion, (uint8_t *)startCacheData + 4, 4 );
      memcpy( &vendorID, (uint8_t *)startCacheData + 8, 4 );
      memcpy( &deviceID, (uint8_t *)startCacheData + 12, 4 );
      memcpy( pipelineCacheUUID, (uint8_t *)startCacheData + 16, VK_UUID_SIZE );

      // Check each field and report bad values before freeing existing cache
      bool badCache = false;

      if ( headerLength <= 0 )
      {
        badCache = true;
        std::cout << "  Bad header length in " << cacheFileName << ".\n";
        std::cout << "    Cache contains: " << std::hex << std::setw( 8 ) << headerLength << "\n";
      }

      if ( cacheHeaderVersion != VK_PIPELINE_CACHE_HEADER_VERSION_ONE )
      {
        badCache = true;
        std::cout << "  Unsupported cache header version in " << cacheFileName << ".\n";
        std::cout << "    Cache contains: " << std::hex << std::setw( 8 ) << cacheHeaderVersion << "\n";
      }

      if ( vendorID != properties.vendorID )
      {
        badCache = true;
        std::cout << "  Vender ID mismatch in " << cacheFileName << ".\n";
        std::cout << "    Cache contains: " << std::hex << std::setw( 8 ) << vendorID << "\n";
        std::cout << "    Driver expects: " << std::hex << std::setw( 8 ) << properties.vendorID << "\n";
      }

      if ( deviceID != properties.deviceID )
      {
        badCache = true;
        std::cout << "  Device ID mismatch in " << cacheFileName << ".\n";
        std::cout << "    Cache contains: " << std::hex << std::setw( 8 ) << deviceID << "\n";
        std::cout << "    Driver expects: " << std::hex << std::setw( 8 ) << properties.deviceID << "\n";
      }

      if ( memcmp( pipelineCacheUUID, properties.pipelineCacheUUID, sizeof( pipelineCacheUUID ) ) != 0 )
      {
        badCache = true;
        std::cout << "  UUID mismatch in " << cacheFileName << ".\n";
        std::cout << "    Cache contains: " << vk::su::UUID( pipelineCacheUUID ) << "\n";
        std::cout << "    Driver expects: " << vk::su::UUID( properties.pipelineCacheUUID ) << "\n";
      }

      if ( badCache )
      {
        // Don't submit initial cache data if any version info is incorrect
        free( startCacheData );
        startCacheSize = 0;
        startCacheData = nullptr;

        // And clear out the old cache file for use in next run
        std::cout << "  Deleting cache entry " << cacheFileName << " to repopulate.\n";
        if ( remove( cacheFileName.c_str() ) != 0 )
        {
          std::cerr << "Reading error";
          exit( EXIT_FAILURE );
        }
      }
    }

    // Feed the initial cache data into cache creation
    vk::PipelineCache pipelineCache =
      device.createPipelineCache( vk::PipelineCacheCreateInfo( vk::PipelineCacheCreateFlags(), startCacheSize, startCacheData ) );

    // Free our initialData now that pipeline cache has been created
    free( startCacheData );
    startCacheData = NULL;

    // Time (roughly) taken to create the graphics pipeline
    timestamp_t  start            = getMilliseconds();
    vk::Pipeline graphicsPipeline = vk::su::createGraphicsPipeline( device,
                                                                    pipelineCache,
                                                                    std::make_pair( vertexShaderModule, nullptr ),
                                                                    std::make_pair( fragmentShaderModule, nullptr ),
                                                                    sizeof( texturedCubeData[0] ),
                                                                    { { vk::Format::eR32G32B32A32Sfloat, 0 }, { vk::Format::eR32G32Sfloat, 16 } },
                                                                    vk::FrontFace::eClockwise,
                                                                    true,
                                                                    pipelineLayout,
                                                                    renderPass );
    timestamp_t  elapsed          = getMilliseconds() - start;
    std::cout << "  vkCreateGraphicsPipeline time: " << (double)elapsed << " ms\n";

    vk::Semaphore imageAcquiredSemaphore = device.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) );

    // Get the index of the next available swapchain image:
    vk::ResultValue<uint32_t> currentBuffer = device.acquireNextImageKHR( swapChainData.swapChain, UINT64_MAX, imageAcquiredSemaphore, nullptr );
    assert( currentBuffer.result == vk::Result::eSuccess );
    assert( currentBuffer.value < framebuffers.size() );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( 0.2f, 0.2f, 0.2f, 0.2f );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    commandBuffer.beginRenderPass(
      vk::RenderPassBeginInfo( renderPass, framebuffers[currentBuffer.value], vk::Rect2D( vk::Offset2D(), surfaceData.extent ), clearValues ),
      vk::SubpassContents::eInline );
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, graphicsPipeline );
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, {} );

    commandBuffer.bindVertexBuffers( 0, vertexBufferData.buffer, { 0 } );
    commandBuffer.setViewport(
      0, vk::Viewport( 0.0f, 0.0f, static_cast<float>( surfaceData.extent.width ), static_cast<float>( surfaceData.extent.height ), 0.0f, 1.0f ) );
    commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), surfaceData.extent ) );

    commandBuffer.draw( 12 * 3, 1, 0, 0 );
    commandBuffer.endRenderPass();
    commandBuffer.end();

    vk::Fence drawFence = device.createFence( vk::FenceCreateInfo() );

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo         submitInfo( imageAcquiredSemaphore, waitDestinationStageMask, commandBuffer );
    graphicsQueue.submit( submitInfo, drawFence );

    while ( vk::Result::eTimeout == device.waitForFences( drawFence, VK_TRUE, vk::su::FenceTimeout ) )
      ;

    vk::Result result = presentQueue.presentKHR( vk::PresentInfoKHR( {}, swapChainData.swapChain, currentBuffer.value ) );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
      default: assert( false );  // an unexpected result is returned !
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    // Store away the cache that we've populated.  This could conceivably happen
    // earlier, depends on when the pipeline cache stops being populated
    // internally.
    std::vector<uint8_t> endCacheData = device.getPipelineCacheData( pipelineCache );

    // Write the file to disk, overwriting whatever was there
    std::ofstream writeCacheStream( cacheFileName, std::ios_base::out | std::ios_base::binary );
    if ( writeCacheStream.good() )
    {
      writeCacheStream.write( reinterpret_cast<char const *>( endCacheData.data() ), endCacheData.size() );
      writeCacheStream.close();
      std::cout << "  cacheData written to " << cacheFileName << "\n";
    }
    else
    {
      // Something bad happened
      std::cout << "  Unable to write cache data to disk!\n";
    }

    /* VULKAN_KEY_END */

    device.destroyFence( drawFence );
    device.destroySemaphore( imageAcquiredSemaphore );
    device.destroyPipeline( graphicsPipeline );
    device.destroyPipelineCache( pipelineCache );
    device.freeDescriptorSets( descriptorPool, descriptorSet );
    device.destroyDescriptorPool( descriptorPool );
    vertexBufferData.clear( device );
    for ( auto framebuffer : framebuffers )
    {
      device.destroyFramebuffer( framebuffer );
    }
    device.destroyShaderModule( fragmentShaderModule );
    device.destroyShaderModule( vertexShaderModule );
    device.destroyRenderPass( renderPass );
    device.destroyPipelineLayout( pipelineLayout );
    device.destroyDescriptorSetLayout( descriptorSetLayout );
    uniformBufferData.clear( device );
    textureData.clear( device );
    depthBufferData.clear( device );
    swapChainData.clear( device );
    device.freeCommandBuffers( commandPool, commandBuffer );
    device.destroyCommandPool( commandPool );
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
