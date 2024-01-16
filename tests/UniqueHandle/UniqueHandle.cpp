// Copyright(c) 2022, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Test: Compile test for Unique handles

#define VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST

#include "../../samples/utils/geometries.hpp"
#include "../../samples/utils/shaders.hpp"
#include "../../samples/utils/utils.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <iostream>

static std::string AppName    = "UniqueHandle";
static std::string EngineName = "Vulkan.hpp";

template <typename T>
class MyAllocator : public std::allocator<T>
{
public:
  MyAllocator() = default;

  template <class U>
  MyAllocator( const MyAllocator<U> & /*unused*/ )
  {
  }

  template <class U>
  struct rebind
  {
    using other = MyAllocator<U>;
  };
};

vk::UniqueDescriptorSetLayout createDescriptorSetLayoutUnique( vk::UniqueDevice const &                                                            device,
                                                               std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>> const & bindingData,
                                                               vk::DescriptorSetLayoutCreateFlags                                                  flags = {} )
{
  std::vector<vk::DescriptorSetLayoutBinding> bindings( bindingData.size() );
  for ( size_t i = 0; i < bindingData.size(); i++ )
  {
    bindings[i] = vk::DescriptorSetLayoutBinding(
      vk::su::checked_cast<uint32_t>( i ), std::get<0>( bindingData[i] ), std::get<1>( bindingData[i] ), std::get<2>( bindingData[i] ) );
  }
  return device->createDescriptorSetLayoutUnique( vk::DescriptorSetLayoutCreateInfo( flags, bindings ) );
}

vk::UniqueInstance createInstanceUnique( std::string const &              appName,
                                         std::string const &              engineName,
                                         std::vector<std::string> const & layers,
                                         std::vector<std::string> const & extensions,
                                         uint32_t                         apiVersion = VK_API_VERSION_1_0 )
{
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
  VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif

  vk::ApplicationInfo       applicationInfo( appName.c_str(), 1, engineName.c_str(), 1, apiVersion );
  std::vector<char const *> enabledLayers = vk::su::gatherLayers( layers
#if !defined( NDEBUG )
                                                                  ,
                                                                  vk::enumerateInstanceLayerProperties()
#endif
  );
  std::vector<char const *> enabledExtensions = vk::su::gatherExtensions( extensions
#if !defined( NDEBUG )
                                                                          ,
                                                                          vk::enumerateInstanceExtensionProperties()
#endif
  );

  vk::UniqueInstance instance =
    vk::createInstanceUnique( vk::su::makeInstanceCreateInfoChain( applicationInfo, enabledLayers, enabledExtensions ).get<vk::InstanceCreateInfo>() );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
  // initialize function pointers for instance
  VULKAN_HPP_DEFAULT_DISPATCHER.init( *instance );
#endif

  return instance;
}

vk::UniqueDevice createDeviceUnique( vk::PhysicalDevice const &         physicalDevice,
                                     uint32_t                           queueFamilyIndex,
                                     std::vector<std::string> const &   extensions,
                                     vk::PhysicalDeviceFeatures const * physicalDeviceFeatures = nullptr,
                                     void const *                       pNext                  = nullptr )
{
  std::vector<char const *> enabledExtensions;
  enabledExtensions.reserve( extensions.size() );
  for ( auto const & ext : extensions )
  {
    enabledExtensions.push_back( ext.data() );
  }

  float                     queuePriority = 0.0f;
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo( {}, queueFamilyIndex, 1, &queuePriority );
  vk::DeviceCreateInfo      deviceCreateInfo( {}, deviceQueueCreateInfo, {}, enabledExtensions, physicalDeviceFeatures, pNext );

  vk::UniqueDevice device = physicalDevice.createDeviceUnique( deviceCreateInfo );
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
  // initialize function pointers for instance
  VULKAN_HPP_DEFAULT_DISPATCHER.init( *device );
#endif
  return device;
}

vk::UniqueRenderPass createRenderPassUnique( vk::UniqueDevice const & device,
                                             vk::Format               colorFormat,
                                             vk::Format               depthFormat,
                                             vk::AttachmentLoadOp     loadOp           = vk::AttachmentLoadOp::eClear,
                                             vk::ImageLayout          colorFinalLayout = vk::ImageLayout::ePresentSrcKHR )
{
  std::vector<vk::AttachmentDescription> attachmentDescriptions;
  assert( colorFormat != vk::Format::eUndefined );
  attachmentDescriptions.emplace_back( vk::AttachmentDescriptionFlags(),
                                       colorFormat,
                                       vk::SampleCountFlagBits::e1,
                                       loadOp,
                                       vk::AttachmentStoreOp::eStore,
                                       vk::AttachmentLoadOp::eDontCare,
                                       vk::AttachmentStoreOp::eDontCare,
                                       vk::ImageLayout::eUndefined,
                                       colorFinalLayout );
  if ( depthFormat != vk::Format::eUndefined )
  {
    attachmentDescriptions.emplace_back( vk::AttachmentDescriptionFlags(),
                                         depthFormat,
                                         vk::SampleCountFlagBits::e1,
                                         loadOp,
                                         vk::AttachmentStoreOp::eDontCare,
                                         vk::AttachmentLoadOp::eDontCare,
                                         vk::AttachmentStoreOp::eDontCare,
                                         vk::ImageLayout::eUndefined,
                                         vk::ImageLayout::eDepthStencilAttachmentOptimal );
  }
  vk::AttachmentReference colorAttachment( 0, vk::ImageLayout::eColorAttachmentOptimal );
  vk::AttachmentReference depthAttachment( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );
  vk::SubpassDescription  subpassDescription( vk::SubpassDescriptionFlags(),
                                             vk::PipelineBindPoint::eGraphics,
                                              {},
                                             colorAttachment,
                                              {},
                                             ( depthFormat != vk::Format::eUndefined ) ? &depthAttachment : nullptr );
  return device->createRenderPassUnique( vk::RenderPassCreateInfo( vk::RenderPassCreateFlags(), attachmentDescriptions, subpassDescription ) );
}

vk::UniqueShaderModule createShaderModuleUnique( vk::UniqueDevice const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText )
{
  std::vector<unsigned int> shaderSPV;
  if ( !vk::su::GLSLtoSPV( shaderStage, shaderText, shaderSPV ) )
  {
    throw std::runtime_error( "Could not convert glsl shader to spir-v -> terminating" );
  }

  return device->createShaderModuleUnique( vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
}

vk::UniqueSwapchainKHR createSwapchainKHRUnique( vk::PhysicalDevice physicalDevice, vk::UniqueDevice const & device, vk::SurfaceKHR surface )
{
  vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
  vk::SurfaceFormatKHR       surfaceFormat       = vk::su::pickSurfaceFormat( physicalDevice.getSurfaceFormatsKHR( surface ) );
  vk::Extent2D               swapchainExtent;
  if ( surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() )
  {
    // If the surface size is undefined, the size is set to the size of the images requested.
    swapchainExtent.width  = vk::su::clamp<uint32_t>( 64, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width );
    swapchainExtent.height = vk::su::clamp<uint32_t>( 64, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height );
  }
  else
  {
    // If the surface size is defined, the swap chain size must match
    swapchainExtent = surfaceCapabilities.currentExtent;
  }
  vk::SurfaceTransformFlagBitsKHR preTransform = ( surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity )
                                                 ? vk::SurfaceTransformFlagBitsKHR::eIdentity
                                                 : surfaceCapabilities.currentTransform;
  vk::CompositeAlphaFlagBitsKHR   compositeAlpha =
    ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied )    ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
      : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
      : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit )        ? vk::CompositeAlphaFlagBitsKHR::eInherit
                                                                                                         : vk::CompositeAlphaFlagBitsKHR::eOpaque;
  vk::SwapchainCreateInfoKHR swapChainCreateInfo( {},
                                                  surface,
                                                  vk::su::clamp( 3u, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount ),
                                                  surfaceFormat.format,
                                                  surfaceFormat.colorSpace,
                                                  swapchainExtent,
                                                  1,
                                                  vk::ImageUsageFlagBits::eColorAttachment,
                                                  vk::SharingMode::eExclusive,
                                                  {},
                                                  preTransform,
                                                  compositeAlpha,
                                                  vk::PresentModeKHR::eFifo,
                                                  true,
                                                  nullptr );
  return device->createSwapchainKHRUnique( swapChainCreateInfo );
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    vk::UniqueInstance instance = createInstanceUnique( AppName, EngineName, {}, vk::su::getInstanceExtensions() );

    // enumerate the physicalDevices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

    // get the QueueFamilyProperties of the first PhysicalDevice
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevices[0].getQueueFamilyProperties();

    // get the first index into queueFamiliyProperties which supports graphics
    auto   propertyIterator         = std::find_if( queueFamilyProperties.begin(),
                                          queueFamilyProperties.end(),
                                          []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } );
    size_t graphicsQueueFamilyIndex = std::distance( queueFamilyProperties.begin(), propertyIterator );
    assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

    // create a Device
    vk::UniqueDevice device = createDeviceUnique( physicalDevices[0], static_cast<uint32_t>( graphicsQueueFamilyIndex ), vk::su::getDeviceExtensions() );

    // create a PipelineCache
    vk::UniquePipelineCache pipelineCache = device->createPipelineCacheUnique( vk::PipelineCacheCreateInfo() );

    // get some vk::ShaderModules
    glslang::InitializeProcess();
    vk::UniqueShaderModule vertexShaderModule   = createShaderModuleUnique( device, vk::ShaderStageFlagBits::eVertex, vertexShaderText_PC_C );
    vk::UniqueShaderModule fragmentShaderModule = createShaderModuleUnique( device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText_C_C );
    glslang::FinalizeProcess();

    // initialize an array of vk::PipelineShaderStageCreateInfos
    // showing the simplified usage when VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST is defined
#if defined( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main" ),
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main" )
    };
#else
    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, *vertexShaderModule, "main" ),
      vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main" )
    };
#endif

    vk::VertexInputBindingDescription                  vertexInputBindingDescription( 0, sizeof( coloredCubeData[0] ) );
    std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
      vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 ),
      vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32A32Sfloat, 16 )
    };
    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo( vk::PipelineVertexInputStateCreateFlags(),  // flags
                                                                               vertexInputBindingDescription,              // vertexBindingDescriptions
                                                                               vertexInputAttributeDescriptions            // vertexAttributeDescriptions
    );

    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo( vk::PipelineInputAssemblyStateCreateFlags(),
                                                                                   vk::PrimitiveTopology::eTriangleList );

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo( vk::PipelineRasterizationStateCreateFlags(),  // flags
                                                                                   false,                                        // depthClampEnable
                                                                                   false,                                        // rasterizerDiscardEnable
                                                                                   vk::PolygonMode::eFill,                       // polygonMode
                                                                                   vk::CullModeFlagBits::eBack,                  // cullMode
                                                                                   vk::FrontFace::eClockwise,                    // frontFace
                                                                                   false,                                        // depthBiasEnable
                                                                                   0.0f,                                         // depthBiasConstantFactor
                                                                                   0.0f,                                         // depthBiasClamp
                                                                                   0.0f,                                         // depthBiasSlopeFactor
                                                                                   1.0f                                          // lineWidth
    );

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo( vk::PipelineMultisampleStateCreateFlags(),  // flags
                                                                               vk::SampleCountFlagBits::e1                 // rasterizationSamples
                                                                                                                           // other values can be default
    );

    vk::StencilOpState                      stencilOpState( vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo( vk::PipelineDepthStencilStateCreateFlags(),  // flags
                                                                                 true,                                        // depthTestEnable
                                                                                 true,                                        // depthWriteEnable
                                                                                 vk::CompareOp::eLessOrEqual,                 // depthCompareOp
                                                                                 false,                                       // depthBoundTestEnable
                                                                                 false,                                       // stencilTestEnable
                                                                                 stencilOpState,                              // front
                                                                                 stencilOpState                               // back
    );

    vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                                                 vk::ColorComponentFlagBits::eA );
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState( false,                   // blendEnable
                                                                             vk::BlendFactor::eZero,  // srcColorBlendFactor
                                                                             vk::BlendFactor::eZero,  // dstColorBlendFactor
                                                                             vk::BlendOp::eAdd,       // colorBlendOp
                                                                             vk::BlendFactor::eZero,  // srcAlphaBlendFactor
                                                                             vk::BlendFactor::eZero,  // dstAlphaBlendFactor
                                                                             vk::BlendOp::eAdd,       // alphaBlendOp
                                                                             colorComponentFlags      // colorWriteMask
    );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo( vk::PipelineColorBlendStateCreateFlags(),  // flags
                                                                             false,                                     // logicOpEnable
                                                                             vk::LogicOp::eNoOp,                        // logicOp
                                                                             pipelineColorBlendAttachmentState,         // attachments
                                                                             { { 1.0f, 1.0f, 1.0f, 1.0f } }             // blendConstants
    );

    std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(), dynamicStates );

    vk::UniqueDescriptorSetLayout descriptorSetLayout =
      createDescriptorSetLayoutUnique( device, { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex } } );
    vk::UniquePipelineLayout pipelineLayout =
      device->createPipelineLayoutUnique( vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), *descriptorSetLayout ) );

    vk::su::SurfaceData surfaceData( *instance, AppName, vk::Extent2D( 500, 500 ) );

    vk::UniqueRenderPass renderPass = createRenderPassUnique(
      device, vk::su::pickSurfaceFormat( physicalDevices[0].getSurfaceFormatsKHR( surfaceData.surface ) ).format, vk::Format::eD16Unorm );

    // initialize the vk::GraphicsPipelineCreateInfo
    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo( vk::PipelineCreateFlags(),              // flags
                                                               pipelineShaderStageCreateInfos,         // stages
                                                               &pipelineVertexInputStateCreateInfo,    // pVertexInputState
                                                               &pipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState
                                                               nullptr,                                // pTessellationState
                                                               &pipelineViewportStateCreateInfo,       // pViewportState
                                                               &pipelineRasterizationStateCreateInfo,  // pRasterizationState
                                                               &pipelineMultisampleStateCreateInfo,    // pMultisampleState
                                                               &pipelineDepthStencilStateCreateInfo,   // pDepthStencilState
                                                               &pipelineColorBlendStateCreateInfo,     // pColorBlendState
                                                               &pipelineDynamicStateCreateInfo,        // pDynamicState
                                                               *pipelineLayout,                        // layout
                                                               *renderPass                             // renderPass
    );

    // create a GraphicsPipeline
    vk::ResultValue<vk::UniquePipeline> rv = device->createGraphicsPipelineUnique( *pipelineCache, graphicsPipelineCreateInfo );
#if 17 <= VULKAN_HPP_CPP_VERSION
    auto [r, v] = std::move( rv );
#endif
    // auto trv = rv.asTuple();    // asTuple() on an l-value is deprecated !!
    auto trv1 = std::move( rv ).asTuple();

    vk::UniquePipeline graphicsPipeline = device->createGraphicsPipelineUnique( *pipelineCache, graphicsPipelineCreateInfo ).value;

    vk::UniquePipeline graphicsPipeline2 =
      std::move( device->createGraphicsPipelinesUnique<vk::DispatchLoaderDynamic, MyAllocator<vk::UniquePipeline>>( *pipelineCache, graphicsPipelineCreateInfo )
                   .value[0] );

    vk::UniquePipeline graphicsPipeline3 =
      std::move( device->createGraphicsPipelinesUnique<vk::DispatchLoaderDynamic>( *pipelineCache, graphicsPipelineCreateInfo ).value[0] );

    vk::DescriptorPoolSize   poolSize( vk::DescriptorType::eUniformBuffer, 1 );
    vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique( { vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSize } );

    std::vector<vk::UniqueDescriptorSet> descriptorSets = device->allocateDescriptorSetsUnique( { *descriptorPool, *descriptorSetLayout } );

    vk::UniqueSwapchainKHR swapchain = createSwapchainKHRUnique( physicalDevices[0], device, surfaceData.surface );

    // destroy the non-Unique surface used here, but swapchain needs to be destroyed first
    swapchain.reset();
    instance->destroySurfaceKHR( surfaceData.surface );
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
