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

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#elif defined( __GNUC__ )
// no need to ignore any warnings with GCC
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "utils.hpp"

#include "vulkan/vulkan.hpp"

#include <iomanip>
#include <numeric>

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

namespace vk
{
  namespace su
  {
    vk::UniqueDeviceMemory allocateMemory( vk::UniqueDevice const &                   device,
                                           vk::PhysicalDeviceMemoryProperties const & memoryProperties,
                                           vk::MemoryRequirements const &             memoryRequirements,
                                           vk::MemoryPropertyFlags                    memoryPropertyFlags )
    {
      uint32_t memoryTypeIndex =
        findMemoryType( memoryProperties, memoryRequirements.memoryTypeBits, memoryPropertyFlags );

      return device->allocateMemoryUnique( vk::MemoryAllocateInfo( memoryRequirements.size, memoryTypeIndex ) );
    }

    bool contains( std::vector<vk::ExtensionProperties> const & extensionProperties, std::string const & extensionName )
    {
      return std::find_if( extensionProperties.begin(),
                           extensionProperties.end(),
                           [&extensionName]( vk::ExtensionProperties const & ep ) {
                             return extensionName == ep.extensionName;
                           } ) != extensionProperties.end();
    }

    vk::UniqueCommandPool createCommandPool( vk::UniqueDevice & device, uint32_t queueFamilyIndex )
    {
      vk::CommandPoolCreateInfo commandPoolCreateInfo( vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                       queueFamilyIndex );
      return device->createCommandPoolUnique( commandPoolCreateInfo );
    }

    vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessenger( vk::UniqueInstance & instance )
    {
      vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
      vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
      return instance->createDebugUtilsMessengerEXTUnique( vk::DebugUtilsMessengerCreateInfoEXT(
        {}, severityFlags, messageTypeFlags, &vk::su::debugUtilsMessengerCallback ) );
    }

    vk::UniqueDescriptorPool createDescriptorPool( vk::UniqueDevice &                          device,
                                                   std::vector<vk::DescriptorPoolSize> const & poolSizes )
    {
      assert( !poolSizes.empty() );
      uint32_t maxSets =
        std::accumulate( poolSizes.begin(), poolSizes.end(), 0, []( uint32_t sum, vk::DescriptorPoolSize const & dps ) {
          return sum + dps.descriptorCount;
        } );
      assert( 0 < maxSets );

      vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(
        vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, poolSizes );
      return device->createDescriptorPoolUnique( descriptorPoolCreateInfo );
    }

    vk::UniqueDescriptorSetLayout createDescriptorSetLayout(
      vk::UniqueDevice const &                                                            device,
      std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>> const & bindingData,
      vk::DescriptorSetLayoutCreateFlags                                                  flags )
    {
      std::vector<vk::DescriptorSetLayoutBinding> bindings( bindingData.size() );
      for ( size_t i = 0; i < bindingData.size(); i++ )
      {
        bindings[i] = vk::DescriptorSetLayoutBinding( checked_cast<uint32_t>( i ),
                                                      std::get<0>( bindingData[i] ),
                                                      std::get<1>( bindingData[i] ),
                                                      std::get<2>( bindingData[i] ) );
      }
      return device->createDescriptorSetLayoutUnique( vk::DescriptorSetLayoutCreateInfo( flags, bindings ) );
    }

    vk::UniqueDevice createDevice( vk::PhysicalDevice                 physicalDevice,
                                   uint32_t                           queueFamilyIndex,
                                   std::vector<std::string> const &   extensions,
                                   vk::PhysicalDeviceFeatures const * physicalDeviceFeatures,
                                   void const *                       pNext )
    {
      std::vector<char const *> enabledExtensions;
      enabledExtensions.reserve( extensions.size() );
      for ( auto const & ext : extensions )
      {
        enabledExtensions.push_back( ext.data() );
      }

      // create a UniqueDevice
      float                     queuePriority = 0.0f;
      vk::DeviceQueueCreateInfo deviceQueueCreateInfo(
        vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority );
      vk::DeviceCreateInfo deviceCreateInfo(
        vk::DeviceCreateFlags(), deviceQueueCreateInfo, {}, enabledExtensions, physicalDeviceFeatures );
      deviceCreateInfo.pNext = pNext;
      return physicalDevice.createDeviceUnique( deviceCreateInfo );
    }

    std::vector<vk::UniqueFramebuffer> createFramebuffers( vk::UniqueDevice &                       device,
                                                           vk::UniqueRenderPass &                   renderPass,
                                                           std::vector<vk::UniqueImageView> const & imageViews,
                                                           vk::UniqueImageView const &              depthImageView,
                                                           vk::Extent2D const &                     extent )
    {
      vk::ImageView attachments[2];
      attachments[1] = depthImageView.get();

      vk::FramebufferCreateInfo          framebufferCreateInfo( vk::FramebufferCreateFlags(),
                                                       *renderPass,
                                                       depthImageView ? 2 : 1,
                                                       attachments,
                                                       extent.width,
                                                       extent.height,
                                                       1 );
      std::vector<vk::UniqueFramebuffer> framebuffers;
      framebuffers.reserve( imageViews.size() );
      for ( auto const & view : imageViews )
      {
        attachments[0] = view.get();
        framebuffers.push_back( device->createFramebufferUnique( framebufferCreateInfo ) );
      }

      return framebuffers;
    }

    vk::UniquePipeline
      createGraphicsPipeline( vk::UniqueDevice const &                                            device,
                              vk::UniquePipelineCache const &                                     pipelineCache,
                              std::pair<vk::ShaderModule, vk::SpecializationInfo const *> const & vertexShaderData,
                              std::pair<vk::ShaderModule, vk::SpecializationInfo const *> const & fragmentShaderData,
                              uint32_t                                                            vertexStride,
                              std::vector<std::pair<vk::Format, uint32_t>> const & vertexInputAttributeFormatOffset,
                              vk::FrontFace                                        frontFace,
                              bool                                                 depthBuffered,
                              vk::UniquePipelineLayout const &                     pipelineLayout,
                              vk::UniqueRenderPass const &                         renderPass )
    {
      std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
        vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(),
                                           vk::ShaderStageFlagBits::eVertex,
                                           vertexShaderData.first,
                                           "main",
                                           vertexShaderData.second ),
        vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(),
                                           vk::ShaderStageFlagBits::eFragment,
                                           fragmentShaderData.first,
                                           "main",
                                           fragmentShaderData.second )
      };

      std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;
      vk::PipelineVertexInputStateCreateInfo           pipelineVertexInputStateCreateInfo;
      vk::VertexInputBindingDescription                vertexInputBindingDescription( 0, vertexStride );

      if ( 0 < vertexStride )
      {
        vertexInputAttributeDescriptions.reserve( vertexInputAttributeFormatOffset.size() );
        for ( uint32_t i = 0; i < vertexInputAttributeFormatOffset.size(); i++ )
        {
          vertexInputAttributeDescriptions.push_back( vk::VertexInputAttributeDescription(
            i, 0, vertexInputAttributeFormatOffset[i].first, vertexInputAttributeFormatOffset[i].second ) );
        }
        pipelineVertexInputStateCreateInfo.setVertexBindingDescriptions( vertexInputBindingDescription );
        pipelineVertexInputStateCreateInfo.setVertexAttributeDescriptions( vertexInputAttributeDescriptions );
      }

      vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
        vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList );

      vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
        vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

      vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
        vk::PipelineRasterizationStateCreateFlags(),
        false,
        false,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        frontFace,
        false,
        0.0f,
        0.0f,
        0.0f,
        1.0f );

      vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo( {}, vk::SampleCountFlagBits::e1 );

      vk::StencilOpState stencilOpState(
        vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
      vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
        vk::PipelineDepthStencilStateCreateFlags(),
        depthBuffered,
        depthBuffered,
        vk::CompareOp::eLessOrEqual,
        false,
        false,
        stencilOpState,
        stencilOpState );

      vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                   vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA );
      vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,
                                                                               vk::BlendFactor::eZero,
                                                                               vk::BlendFactor::eZero,
                                                                               vk::BlendOp::eAdd,
                                                                               vk::BlendFactor::eZero,
                                                                               vk::BlendFactor::eZero,
                                                                               vk::BlendOp::eAdd,
                                                                               colorComponentFlags );
      vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo( vk::PipelineColorBlendStateCreateFlags(),
                                                                               false,
                                                                               vk::LogicOp::eNoOp,
                                                                               pipelineColorBlendAttachmentState,
                                                                               { { 1.0f, 1.0f, 1.0f, 1.0f } } );

      std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
      vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(),
                                                                         dynamicStates );

      vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo( vk::PipelineCreateFlags(),
                                                                 pipelineShaderStageCreateInfos,
                                                                 &pipelineVertexInputStateCreateInfo,
                                                                 &pipelineInputAssemblyStateCreateInfo,
                                                                 nullptr,
                                                                 &pipelineViewportStateCreateInfo,
                                                                 &pipelineRasterizationStateCreateInfo,
                                                                 &pipelineMultisampleStateCreateInfo,
                                                                 &pipelineDepthStencilStateCreateInfo,
                                                                 &pipelineColorBlendStateCreateInfo,
                                                                 &pipelineDynamicStateCreateInfo,
                                                                 pipelineLayout.get(),
                                                                 renderPass.get() );

      auto result = device->createGraphicsPipelineUnique( pipelineCache.get(), graphicsPipelineCreateInfo );
      assert( result.result == vk::Result::eSuccess );
      return std::move( result.value );
    }

    vk::UniqueInstance createInstance( std::string const &              appName,
                                       std::string const &              engineName,
                                       std::vector<std::string> const & layers,
                                       std::vector<std::string> const & extensions,
                                       uint32_t                         apiVersion )
    {
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
      static vk::DynamicLoader  dl;
      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
        dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
#endif

#if !defined( NDEBUG )
      std::vector<vk::LayerProperties>     layerProperties     = vk::enumerateInstanceLayerProperties();
      std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
#endif

      std::vector<char const *> enabledLayers;
      enabledLayers.reserve( layers.size() );
      for ( auto const & layer : layers )
      {
        assert(
          std::find_if( layerProperties.begin(), layerProperties.end(), [layer]( vk::LayerProperties const & lp ) {
            return layer == lp.layerName;
          } ) != layerProperties.end() );
        enabledLayers.push_back( layer.data() );
      }
#if !defined( NDEBUG )
      // Enable standard validation layer to find as much errors as possible!
      if ( std::find( layers.begin(), layers.end(), "VK_LAYER_KHRONOS_validation" ) == layers.end() &&
           std::find_if( layerProperties.begin(), layerProperties.end(), []( vk::LayerProperties const & lp ) {
             return ( strcmp( "VK_LAYER_KHRONOS_validation", lp.layerName ) == 0 );
           } ) != layerProperties.end() )
      {
        enabledLayers.push_back( "VK_LAYER_KHRONOS_validation" );
      }
      if ( std::find( layers.begin(), layers.end(), "VK_LAYER_LUNARG_assistant_layer" ) == layers.end() &&
           std::find_if( layerProperties.begin(), layerProperties.end(), []( vk::LayerProperties const & lp ) {
             return ( strcmp( "VK_LAYER_LUNARG_assistant_layer", lp.layerName ) == 0 );
           } ) != layerProperties.end() )
      {
        enabledLayers.push_back( "VK_LAYER_LUNARG_assistant_layer" );
      }
#endif

      std::vector<char const *> enabledExtensions;
      enabledExtensions.reserve( extensions.size() );
      for ( auto const & ext : extensions )
      {
        assert( std::find_if(
                  extensionProperties.begin(), extensionProperties.end(), [ext]( vk::ExtensionProperties const & ep ) {
                    return ext == ep.extensionName;
                  } ) != extensionProperties.end() );
        enabledExtensions.push_back( ext.data() );
      }
#if !defined( NDEBUG )
      if ( std::find( extensions.begin(), extensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) == extensions.end() &&
           std::find_if(
             extensionProperties.begin(), extensionProperties.end(), []( vk::ExtensionProperties const & ep ) {
               return ( strcmp( VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName ) == 0 );
             } ) != extensionProperties.end() )
      {
        enabledExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
      }
#endif

      // create a UniqueInstance
      vk::ApplicationInfo applicationInfo( appName.c_str(), 1, engineName.c_str(), 1, apiVersion );
#if defined( NDEBUG )
      // in non-debug mode just use the InstanceCreateInfo for instance creation
      vk::StructureChain<vk::InstanceCreateInfo> instanceCreateInfo(
        { {}, &applicationInfo, enabledLayers, enabledExtensions } );
#else
      // in debug mode, addionally use the debugUtilsMessengerCallback in instance creation!
      vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
      vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
      vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
        { {}, &applicationInfo, enabledLayers, enabledExtensions },
        { {}, severityFlags, messageTypeFlags, &vk::su::debugUtilsMessengerCallback } );
#endif
      vk::UniqueInstance instance = vk::createInstanceUnique( instanceCreateInfo.get<vk::InstanceCreateInfo>() );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
      // initialize function pointers for instance
      VULKAN_HPP_DEFAULT_DISPATCHER.init( *instance );
#endif

      return instance;
    }

    vk::UniqueRenderPass createRenderPass( vk::UniqueDevice &   device,
                                           vk::Format           colorFormat,
                                           vk::Format           depthFormat,
                                           vk::AttachmentLoadOp loadOp,
                                           vk::ImageLayout      colorFinalLayout )
    {
      std::vector<vk::AttachmentDescription> attachmentDescriptions;
      assert( colorFormat != vk::Format::eUndefined );
      attachmentDescriptions.push_back( vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                                                   colorFormat,
                                                                   vk::SampleCountFlagBits::e1,
                                                                   loadOp,
                                                                   vk::AttachmentStoreOp::eStore,
                                                                   vk::AttachmentLoadOp::eDontCare,
                                                                   vk::AttachmentStoreOp::eDontCare,
                                                                   vk::ImageLayout::eUndefined,
                                                                   colorFinalLayout ) );
      if ( depthFormat != vk::Format::eUndefined )
      {
        attachmentDescriptions.push_back(
          vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                     depthFormat,
                                     vk::SampleCountFlagBits::e1,
                                     loadOp,
                                     vk::AttachmentStoreOp::eDontCare,
                                     vk::AttachmentLoadOp::eDontCare,
                                     vk::AttachmentStoreOp::eDontCare,
                                     vk::ImageLayout::eUndefined,
                                     vk::ImageLayout::eDepthStencilAttachmentOptimal ) );
      }
      vk::AttachmentReference colorAttachment( 0, vk::ImageLayout::eColorAttachmentOptimal );
      vk::AttachmentReference depthAttachment( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );
      vk::SubpassDescription  subpassDescription( vk::SubpassDescriptionFlags(),
                                                 vk::PipelineBindPoint::eGraphics,
                                                 {},
                                                 colorAttachment,
                                                 {},
                                                 ( depthFormat != vk::Format::eUndefined ) ? &depthAttachment
                                                                                           : nullptr );
      return device->createRenderPassUnique(
        vk::RenderPassCreateInfo( vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription ) );
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL
      debugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                   VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                   VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                   void * /*pUserData*/ )
    {
      std::cerr << vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) << ": "
                << vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
      std::cerr << "\t"
                << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
      std::cerr << "\t"
                << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
      std::cerr << "\t"
                << "message         = <" << pCallbackData->pMessage << ">\n";
      if ( 0 < pCallbackData->queueLabelCount )
      {
        std::cerr << "\t"
                  << "Queue Labels:\n";
        for ( uint8_t i = 0; i < pCallbackData->queueLabelCount; i++ )
        {
          std::cerr << "\t\t"
                    << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
        }
      }
      if ( 0 < pCallbackData->cmdBufLabelCount )
      {
        std::cerr << "\t"
                  << "CommandBuffer Labels:\n";
        for ( uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ )
        {
          std::cerr << "\t\t"
                    << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
        }
      }
      if ( 0 < pCallbackData->objectCount )
      {
        std::cerr << "\t"
                  << "Objects:\n";
        for ( uint8_t i = 0; i < pCallbackData->objectCount; i++ )
        {
          std::cerr << "\t\t"
                    << "Object " << i << "\n";
          std::cerr << "\t\t\t"
                    << "objectType   = "
                    << vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) << "\n";
          std::cerr << "\t\t\t"
                    << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
          if ( pCallbackData->pObjects[i].pObjectName )
          {
            std::cerr << "\t\t\t"
                      << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
          }
        }
      }
      return VK_TRUE;
    }

    uint32_t findGraphicsQueueFamilyIndex( std::vector<vk::QueueFamilyProperties> const & queueFamilyProperties )
    {
      // get the first index into queueFamiliyProperties which supports graphics
      size_t graphicsQueueFamilyIndex = std::distance(
        queueFamilyProperties.begin(),
        std::find_if(
          queueFamilyProperties.begin(), queueFamilyProperties.end(), []( vk::QueueFamilyProperties const & qfp ) {
            return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
          } ) );
      assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

      return checked_cast<uint32_t>( graphicsQueueFamilyIndex );
    }

    std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex( vk::PhysicalDevice     physicalDevice,
                                                                          vk::SurfaceKHR const & surface )
    {
      std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
      assert( queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max() );

      uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex( queueFamilyProperties );
      if ( physicalDevice.getSurfaceSupportKHR( graphicsQueueFamilyIndex, surface ) )
      {
        return std::make_pair(
          graphicsQueueFamilyIndex,
          graphicsQueueFamilyIndex );  // the first graphicsQueueFamilyIndex does also support presents
      }

      // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
      // graphics and present
      for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
      {
        if ( ( queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics ) &&
             physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
        {
          return std::make_pair( static_cast<uint32_t>( i ), static_cast<uint32_t>( i ) );
        }
      }

      // there's nothing like a single family index that supports both graphics and present -> look for an other family
      // index that supports present
      for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
      {
        if ( physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
        {
          return std::make_pair( graphicsQueueFamilyIndex, static_cast<uint32_t>( i ) );
        }
      }

      throw std::runtime_error( "Could not find queues for both graphics or present -> terminating" );
    }

    uint32_t findMemoryType( vk::PhysicalDeviceMemoryProperties const & memoryProperties,
                             uint32_t                                   typeBits,
                             vk::MemoryPropertyFlags                    requirementsMask )
    {
      uint32_t typeIndex = uint32_t( ~0 );
      for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++ )
      {
        if ( ( typeBits & 1 ) &&
             ( ( memoryProperties.memoryTypes[i].propertyFlags & requirementsMask ) == requirementsMask ) )
        {
          typeIndex = i;
          break;
        }
        typeBits >>= 1;
      }
      assert( typeIndex != uint32_t(~0) );
      return typeIndex;
    }

    std::vector<std::string> getDeviceExtensions()
    {
      return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    }

    std::vector<std::string> getInstanceExtensions()
    {
      std::vector<std::string> extensions;
      extensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
      extensions.push_back( VK_KHR_ANDROID_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_IOS_MVK )
      extensions.push_back( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
      extensions.push_back( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_MIR_KHR )
      extensions.push_back( VK_KHR_MIR_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_VI_NN )
      extensions.push_back( VK_NN_VI_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
      extensions.push_back( VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_WIN32_KHR )
      extensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_XCB_KHR )
      extensions.push_back( VK_KHR_XCB_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
      extensions.push_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#elif defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
      extensions.push_back( VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME );
#endif
      return extensions;
    }

    vk::Format pickDepthFormat( vk::PhysicalDevice const & physicalDevice )
    {
      std::vector<vk::Format> candidates = { vk::Format::eD32Sfloat,
                                             vk::Format::eD32SfloatS8Uint,
                                             vk::Format::eD24UnormS8Uint };
      for ( vk::Format format : candidates )
      {
        vk::FormatProperties props = physicalDevice.getFormatProperties( format );

        if ( props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
        {
          return format;
        }
      }
      throw std::runtime_error( "failed to find supported format!" );
    }

    vk::PresentModeKHR pickPresentMode( std::vector<vk::PresentModeKHR> const & presentModes )
    {
      vk::PresentModeKHR pickedMode = vk::PresentModeKHR::eFifo;
      for ( const auto & presentMode : presentModes )
      {
        if ( presentMode == vk::PresentModeKHR::eMailbox )
        {
          pickedMode = presentMode;
          break;
        }

        if ( presentMode == vk::PresentModeKHR::eImmediate )
        {
          pickedMode = presentMode;
        }
      }
      return pickedMode;
    }

    vk::SurfaceFormatKHR pickSurfaceFormat( std::vector<vk::SurfaceFormatKHR> const & formats )
    {
      assert( !formats.empty() );
      vk::SurfaceFormatKHR pickedFormat = formats[0];
      if ( formats.size() == 1 )
      {
        if ( formats[0].format == vk::Format::eUndefined )
        {
          pickedFormat.format     = vk::Format::eB8G8R8A8Unorm;
          pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        }
      }
      else
      {
        // request several formats, the first found will be used
        vk::Format requestedFormats[] = {
          vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm
        };
        vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        for ( size_t i = 0; i < sizeof( requestedFormats ) / sizeof( requestedFormats[0] ); i++ )
        {
          vk::Format requestedFormat = requestedFormats[i];
          auto       it              = std::find_if(
            formats.begin(), formats.end(), [requestedFormat, requestedColorSpace]( vk::SurfaceFormatKHR const & f ) {
              return ( f.format == requestedFormat ) && ( f.colorSpace == requestedColorSpace );
            } );
          if ( it != formats.end() )
          {
            pickedFormat = *it;
            break;
          }
        }
      }
      assert( pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear );
      return pickedFormat;
    }

    void setImageLayout( vk::UniqueCommandBuffer const & commandBuffer,
                         vk::Image                       image,
                         vk::Format                      format,
                         vk::ImageLayout                 oldImageLayout,
                         vk::ImageLayout                 newImageLayout )
    {
      vk::AccessFlags sourceAccessMask;
      switch ( oldImageLayout )
      {
        case vk::ImageLayout::eTransferDstOptimal: sourceAccessMask = vk::AccessFlagBits::eTransferWrite; break;
        case vk::ImageLayout::ePreinitialized: sourceAccessMask = vk::AccessFlagBits::eHostWrite; break;
        case vk::ImageLayout::eGeneral:  // sourceAccessMask is empty
        case vk::ImageLayout::eUndefined: break;
        default: assert( false ); break;
      }

      vk::PipelineStageFlags sourceStage;
      switch ( oldImageLayout )
      {
        case vk::ImageLayout::eGeneral:
        case vk::ImageLayout::ePreinitialized: sourceStage = vk::PipelineStageFlagBits::eHost; break;
        case vk::ImageLayout::eTransferDstOptimal: sourceStage = vk::PipelineStageFlagBits::eTransfer; break;
        case vk::ImageLayout::eUndefined: sourceStage = vk::PipelineStageFlagBits::eTopOfPipe; break;
        default: assert( false ); break;
      }

      vk::AccessFlags destinationAccessMask;
      switch ( newImageLayout )
      {
        case vk::ImageLayout::eColorAttachmentOptimal:
          destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
          break;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
          destinationAccessMask =
            vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
          break;
        case vk::ImageLayout::eGeneral:  // empty destinationAccessMask
        case vk::ImageLayout::ePresentSrcKHR: break;
        case vk::ImageLayout::eShaderReadOnlyOptimal: destinationAccessMask = vk::AccessFlagBits::eShaderRead; break;
        case vk::ImageLayout::eTransferSrcOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferRead; break;
        case vk::ImageLayout::eTransferDstOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferWrite; break;
        default: assert( false ); break;
      }

      vk::PipelineStageFlags destinationStage;
      switch ( newImageLayout )
      {
        case vk::ImageLayout::eColorAttachmentOptimal:
          destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
          break;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
          destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
          break;
        case vk::ImageLayout::eGeneral: destinationStage = vk::PipelineStageFlagBits::eHost; break;
        case vk::ImageLayout::ePresentSrcKHR: destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe; break;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
          destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
          break;
        case vk::ImageLayout::eTransferDstOptimal:
        case vk::ImageLayout::eTransferSrcOptimal: destinationStage = vk::PipelineStageFlagBits::eTransfer; break;
        default: assert( false ); break;
      }

      vk::ImageAspectFlags aspectMask;
      if ( newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal )
      {
        aspectMask = vk::ImageAspectFlagBits::eDepth;
        if ( format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint )
        {
          aspectMask |= vk::ImageAspectFlagBits::eStencil;
        }
      }
      else
      {
        aspectMask = vk::ImageAspectFlagBits::eColor;
      }

      vk::ImageSubresourceRange imageSubresourceRange( aspectMask, 0, 1, 0, 1 );
      vk::ImageMemoryBarrier    imageMemoryBarrier( sourceAccessMask,
                                                 destinationAccessMask,
                                                 oldImageLayout,
                                                 newImageLayout,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 image,
                                                 imageSubresourceRange );
      return commandBuffer->pipelineBarrier( sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier );
    }

    void submitAndWait( vk::UniqueDevice & device, vk::Queue queue, vk::UniqueCommandBuffer & commandBuffer )
    {
      vk::UniqueFence fence = device->createFenceUnique( vk::FenceCreateInfo() );
      queue.submit( vk::SubmitInfo( {}, {}, *commandBuffer ), fence.get() );
      while ( vk::Result::eTimeout == device->waitForFences( fence.get(), VK_TRUE, vk::su::FenceTimeout ) )
        ;
    }

    void updateDescriptorSets(
      vk::UniqueDevice const &        device,
      vk::UniqueDescriptorSet const & descriptorSet,
      std::vector<std::tuple<vk::DescriptorType, vk::UniqueBuffer const &, vk::UniqueBufferView const &>> const &
                                  bufferData,
      vk::su::TextureData const & textureData,
      uint32_t                    bindingOffset )
    {
      std::vector<vk::DescriptorBufferInfo> bufferInfos;
      bufferInfos.reserve( bufferData.size() );

      std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
      writeDescriptorSets.reserve( bufferData.size() + 1 );
      uint32_t dstBinding = bindingOffset;
      for ( auto const & bd : bufferData )
      {
        bufferInfos.push_back( vk::DescriptorBufferInfo( *std::get<1>( bd ), 0, VK_WHOLE_SIZE ) );
        writeDescriptorSets.push_back( vk::WriteDescriptorSet( *descriptorSet,
                                                               dstBinding++,
                                                               0,
                                                               1,
                                                               std::get<0>( bd ),
                                                               nullptr,
                                                               &bufferInfos.back(),
                                                               std::get<2>( bd ) ? &*std::get<2>( bd ) : nullptr ) );
      }

      vk::DescriptorImageInfo imageInfo(
        *textureData.textureSampler, *textureData.imageData->imageView, vk::ImageLayout::eShaderReadOnlyOptimal );
      writeDescriptorSets.push_back( vk::WriteDescriptorSet(
        *descriptorSet, dstBinding, 0, vk::DescriptorType::eCombinedImageSampler, imageInfo, {}, nullptr ) );

      device->updateDescriptorSets( writeDescriptorSets, nullptr );
    }

    void updateDescriptorSets(
      vk::UniqueDevice const &        device,
      vk::UniqueDescriptorSet const & descriptorSet,
      std::vector<std::tuple<vk::DescriptorType, vk::UniqueBuffer const &, vk::UniqueBufferView const &>> const &
                                               bufferData,
      std::vector<vk::su::TextureData> const & textureData,
      uint32_t                                 bindingOffset )
    {
      std::vector<vk::DescriptorBufferInfo> bufferInfos;
      bufferInfos.reserve( bufferData.size() );

      std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
      writeDescriptorSets.reserve( bufferData.size() + ( textureData.empty() ? 0 : 1 ) );
      uint32_t dstBinding = bindingOffset;
      for ( auto const & bd : bufferData )
      {
        bufferInfos.push_back( vk::DescriptorBufferInfo( *std::get<1>( bd ), 0, VK_WHOLE_SIZE ) );
        writeDescriptorSets.push_back( vk::WriteDescriptorSet( *descriptorSet,
                                                               dstBinding++,
                                                               0,
                                                               1,
                                                               std::get<0>( bd ),
                                                               nullptr,
                                                               &bufferInfos.back(),
                                                               std::get<2>( bd ) ? &*std::get<2>( bd ) : nullptr ) );
      }

      std::vector<vk::DescriptorImageInfo> imageInfos;
      if ( !textureData.empty() )
      {
        imageInfos.reserve( textureData.size() );
        for ( auto const & td : textureData )
        {
          imageInfos.push_back( vk::DescriptorImageInfo(
            *td.textureSampler, *td.imageData->imageView, vk::ImageLayout::eShaderReadOnlyOptimal ) );
        }
        writeDescriptorSets.push_back( vk::WriteDescriptorSet( *descriptorSet,
                                                               dstBinding,
                                                               0,
                                                               checked_cast<uint32_t>( imageInfos.size() ),
                                                               vk::DescriptorType::eCombinedImageSampler,
                                                               imageInfos.data(),
                                                               nullptr,
                                                               nullptr ) );
      }

      device->updateDescriptorSets( writeDescriptorSets, nullptr );
    }

    BufferData::BufferData( vk::PhysicalDevice const & physicalDevice,
                            vk::UniqueDevice const &   device,
                            vk::DeviceSize             size,
                            vk::BufferUsageFlags       usage,
                            vk::MemoryPropertyFlags    propertyFlags )
#if !defined( NDEBUG )
      : m_size( size ), m_usage( usage ), m_propertyFlags( propertyFlags )
#endif
    {
      buffer       = device->createBufferUnique( vk::BufferCreateInfo( vk::BufferCreateFlags(), size, usage ) );
      deviceMemory = vk::su::allocateMemory( device,
                                             physicalDevice.getMemoryProperties(),
                                             device->getBufferMemoryRequirements( buffer.get() ),
                                             propertyFlags );
      device->bindBufferMemory( buffer.get(), deviceMemory.get(), 0 );
    }

    DepthBufferData::DepthBufferData( vk::PhysicalDevice & physicalDevice,
                                      vk::UniqueDevice &   device,
                                      vk::Format           format,
                                      vk::Extent2D const & extent )
      : ImageData( physicalDevice,
                   device,
                   format,
                   extent,
                   vk::ImageTiling::eOptimal,
                   vk::ImageUsageFlagBits::eDepthStencilAttachment,
                   vk::ImageLayout::eUndefined,
                   vk::MemoryPropertyFlagBits::eDeviceLocal,
                   vk::ImageAspectFlagBits::eDepth )
    {}

    ImageData::ImageData( vk::PhysicalDevice const & physicalDevice,
                          vk::UniqueDevice const &   device,
                          vk::Format                 format_,
                          vk::Extent2D const &       extent,
                          vk::ImageTiling            tiling,
                          vk::ImageUsageFlags        usage,
                          vk::ImageLayout            initialLayout,
                          vk::MemoryPropertyFlags    memoryProperties,
                          vk::ImageAspectFlags       aspectMask )
      : format( format_ )
    {
      vk::ImageCreateInfo imageCreateInfo( vk::ImageCreateFlags(),
                                           vk::ImageType::e2D,
                                           format,
                                           vk::Extent3D( extent, 1 ),
                                           1,
                                           1,
                                           vk::SampleCountFlagBits::e1,
                                           tiling,
                                           usage | vk::ImageUsageFlagBits::eSampled,
                                           vk::SharingMode::eExclusive,
                                           {},
                                           initialLayout );
      image = device->createImageUnique( imageCreateInfo );

      deviceMemory = vk::su::allocateMemory( device,
                                             physicalDevice.getMemoryProperties(),
                                             device->getImageMemoryRequirements( image.get() ),
                                             memoryProperties );

      device->bindImageMemory( image.get(), deviceMemory.get(), 0 );

      vk::ComponentMapping componentMapping(
        ComponentSwizzle::eR, ComponentSwizzle::eG, ComponentSwizzle::eB, ComponentSwizzle::eA );
      vk::ImageViewCreateInfo imageViewCreateInfo( vk::ImageViewCreateFlags(),
                                                   image.get(),
                                                   vk::ImageViewType::e2D,
                                                   format,
                                                   componentMapping,
                                                   vk::ImageSubresourceRange( aspectMask, 0, 1, 0, 1 ) );
      imageView = device->createImageViewUnique( imageViewCreateInfo );
    }

    SurfaceData::SurfaceData( vk::UniqueInstance & instance,
                              std::string const &  windowName,
                              vk::Extent2D const & extent_ )
      : extent( extent_ ), window( vk::su::createWindow( windowName, extent ) )
    {
      VkSurfaceKHR _surface;
      VkResult     err = glfwCreateWindowSurface( VkInstance( instance.get() ), window.handle, nullptr, &_surface );
      if ( err != VK_SUCCESS )
        throw std::runtime_error( "Failed to create window!" );
      vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter( instance.get() );
      surface = vk::UniqueSurfaceKHR( vk::SurfaceKHR( _surface ), _deleter );
    }

    SwapChainData::SwapChainData( vk::PhysicalDevice const &     physicalDevice,
                                  vk::UniqueDevice const &       device,
                                  vk::SurfaceKHR const &         surface,
                                  vk::Extent2D const &           extent,
                                  vk::ImageUsageFlags            usage,
                                  vk::UniqueSwapchainKHR const & oldSwapChain,
                                  uint32_t                       graphicsQueueFamilyIndex,
                                  uint32_t                       presentQueueFamilyIndex )
    {
      vk::SurfaceFormatKHR surfaceFormat = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surface ) );
      colorFormat                        = surfaceFormat.format;

      vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
      VkExtent2D                 swapchainExtent;
      if ( surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() )
      {
        // If the surface size is undefined, the size is set to the size of the images requested.
        swapchainExtent.width =
          clamp( extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width );
        swapchainExtent.height =
          clamp( extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height );
      }
      else
      {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfaceCapabilities.currentExtent;
      }
      vk::SurfaceTransformFlagBitsKHR preTransform =
        ( surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity )
          ? vk::SurfaceTransformFlagBitsKHR::eIdentity
          : surfaceCapabilities.currentTransform;
      vk::CompositeAlphaFlagBitsKHR compositeAlpha =
        ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied )
          ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
          : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied )
              ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
              : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit )
                  ? vk::CompositeAlphaFlagBitsKHR::eInherit
                  : vk::CompositeAlphaFlagBitsKHR::eOpaque;
      vk::PresentModeKHR presentMode = vk::su::pickPresentMode( physicalDevice.getSurfacePresentModesKHR( surface ) );
      vk::SwapchainCreateInfoKHR swapChainCreateInfo( {},
                                                      surface,
                                                      surfaceCapabilities.minImageCount,
                                                      colorFormat,
                                                      surfaceFormat.colorSpace,
                                                      swapchainExtent,
                                                      1,
                                                      usage,
                                                      vk::SharingMode::eExclusive,
                                                      {},
                                                      preTransform,
                                                      compositeAlpha,
                                                      presentMode,
                                                      true,
                                                      *oldSwapChain );
      if ( graphicsQueueFamilyIndex != presentQueueFamilyIndex )
      {
        uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
        // If the graphics and present queues are from different queue families, we either have to explicitly transfer
        // ownership of images between the queues, or we have to create the swapchain with imageSharingMode as
        // vk::SharingMode::eConcurrent
        swapChainCreateInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
      }
      swapChain = device->createSwapchainKHRUnique( swapChainCreateInfo );

      images = device->getSwapchainImagesKHR( swapChain.get() );

      imageViews.reserve( images.size() );
      vk::ComponentMapping componentMapping(
        vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
      vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
      for ( auto image : images )
      {
        vk::ImageViewCreateInfo imageViewCreateInfo(
          vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, colorFormat, componentMapping, subResourceRange );
        imageViews.push_back( device->createImageViewUnique( imageViewCreateInfo ) );
      }
    }

    CheckerboardImageGenerator::CheckerboardImageGenerator( std::array<uint8_t, 3> const & rgb0,
                                                            std::array<uint8_t, 3> const & rgb1 )
      : m_rgb0( rgb0 ), m_rgb1( rgb1 )
    {}

    void CheckerboardImageGenerator::operator()( void * data, vk::Extent2D & extent ) const
    {
      // Checkerboard of 16x16 pixel squares
      uint8_t * pImageMemory = static_cast<uint8_t *>( data );
      for ( uint32_t row = 0; row < extent.height; row++ )
      {
        for ( uint32_t col = 0; col < extent.width; col++ )
        {
          std::array<uint8_t, 3> const & rgb = ( ( ( row & 0x10 ) == 0 ) ^ ( ( col & 0x10 ) == 0 ) ) ? m_rgb1 : m_rgb0;
          pImageMemory[0]                    = rgb[0];
          pImageMemory[1]                    = rgb[1];
          pImageMemory[2]                    = rgb[2];
          pImageMemory[3]                    = 255;
          pImageMemory += 4;
        }
      }
    }

    MonochromeImageGenerator::MonochromeImageGenerator( std::array<unsigned char, 3> const & rgb ) : m_rgb( rgb ) {}

    void MonochromeImageGenerator::operator()( void * data, vk::Extent2D const & extent ) const
    {
      // fill in with the monochrome color
      unsigned char * pImageMemory = static_cast<unsigned char *>( data );
      for ( uint32_t row = 0; row < extent.height; row++ )
      {
        for ( uint32_t col = 0; col < extent.width; col++ )
        {
          pImageMemory[0] = m_rgb[0];
          pImageMemory[1] = m_rgb[1];
          pImageMemory[2] = m_rgb[2];
          pImageMemory[3] = 255;
          pImageMemory += 4;
        }
      }
    }

    PixelsImageGenerator::PixelsImageGenerator( vk::Extent2D const &  extent,
                                                size_t                channels,
                                                unsigned char const * pixels )
      : m_extent( extent ), m_channels( channels ), m_pixels( pixels )
    {
      assert( m_channels == 4 );
    }

    void PixelsImageGenerator::operator()( void * data, vk::Extent2D const & extent ) const
    {
      assert( extent == m_extent );
      memcpy( data, m_pixels, extent.width * extent.height * m_channels );
    }

    TextureData::TextureData( vk::PhysicalDevice const & physicalDevice,
                              vk::UniqueDevice const &   device,
                              vk::Extent2D const &       extent_,
                              vk::ImageUsageFlags        usageFlags,
                              vk::FormatFeatureFlags     formatFeatureFlags,
                              bool                       anisotropyEnable,
                              bool                       forceStaging )
      : format( vk::Format::eR8G8B8A8Unorm ), extent( extent_ )
    {
      vk::FormatProperties               formatProperties = physicalDevice.getFormatProperties( format );

      formatFeatureFlags |= vk::FormatFeatureFlagBits::eSampledImage;
      needsStaging =
        forceStaging || ( ( formatProperties.linearTilingFeatures & formatFeatureFlags ) != formatFeatureFlags );
      vk::ImageTiling         imageTiling;
      vk::ImageLayout         initialLayout;
      vk::MemoryPropertyFlags requirements;
      if ( needsStaging )
      {
        assert( ( formatProperties.optimalTilingFeatures & formatFeatureFlags ) == formatFeatureFlags );
        stagingBufferData = std::unique_ptr<BufferData>( new BufferData(
          physicalDevice, device, extent.width * extent.height * 4, vk::BufferUsageFlagBits::eTransferSrc ) );
        imageTiling       = vk::ImageTiling::eOptimal;
        usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
        initialLayout = vk::ImageLayout::eUndefined;
      }
      else
      {
        imageTiling   = vk::ImageTiling::eLinear;
        initialLayout = vk::ImageLayout::ePreinitialized;
        requirements  = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
      }
      imageData = std::unique_ptr<ImageData>( new ImageData( physicalDevice,
                                                             device,
                                                             format,
                                                             extent,
                                                             imageTiling,
                                                             usageFlags | vk::ImageUsageFlagBits::eSampled,
                                                             initialLayout,
                                                             requirements,
                                                             vk::ImageAspectFlagBits::eColor ) );

      textureSampler = device->createSamplerUnique( vk::SamplerCreateInfo( vk::SamplerCreateFlags(),
                                                                           vk::Filter::eLinear,
                                                                           vk::Filter::eLinear,
                                                                           vk::SamplerMipmapMode::eLinear,
                                                                           vk::SamplerAddressMode::eRepeat,
                                                                           vk::SamplerAddressMode::eRepeat,
                                                                           vk::SamplerAddressMode::eRepeat,
                                                                           0.0f,
                                                                           anisotropyEnable,
                                                                           16.0f,
                                                                           false,
                                                                           vk::CompareOp::eNever,
                                                                           0.0f,
                                                                           0.0f,
                                                                           vk::BorderColor::eFloatOpaqueBlack ) );
    }

    UUID::UUID( uint8_t const data[VK_UUID_SIZE] )
    {
      memcpy( m_data, data, VK_UUID_SIZE * sizeof( uint8_t ) );
    }

    WindowData::WindowData( GLFWwindow * wnd, std::string const & name, vk::Extent2D const & extent )
      : handle{ wnd }, name{ name }, extent{ extent }
    {}

    WindowData::WindowData( WindowData && other ) : handle{}, name{}, extent{}
    {
      std::swap( handle, other.handle );
      std::swap( name, other.name );
      std::swap( extent, other.extent );
    }

    WindowData::~WindowData() noexcept
    {
      glfwDestroyWindow( handle );
    }

    WindowData createWindow( std::string const & windowName, vk::Extent2D const & extent )
    {
      struct glfwContext
      {
        glfwContext()
        {
          glfwInit();
          glfwSetErrorCallback( []( int error, const char * msg ) {
            std::cerr << "glfw: "
                      << "(" << error << ") " << msg << std::endl;
          } );
        }

        ~glfwContext()
        {
          glfwTerminate();
        }
      };

      static auto glfwCtx = glfwContext();
      (void)glfwCtx;

      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
      GLFWwindow * window = glfwCreateWindow( extent.width, extent.height, windowName.c_str(), nullptr, nullptr );
      return WindowData( window, windowName, extent );
    }
  }  // namespace su
}  // namespace vk

std::ostream & operator<<( std::ostream & os, vk::su::UUID const & uuid )
{
  os << std::setfill( '0' ) << std::hex;
  for ( int j = 0; j < VK_UUID_SIZE; ++j )
  {
    os << std::setw( 2 ) << static_cast<uint32_t>( uuid.m_data[j] );
    if ( j == 3 || j == 5 || j == 7 || j == 9 )
    {
      std::cout << '-';
    }
  }
  os << std::setfill( ' ' ) << std::dec;
  return os;
}
