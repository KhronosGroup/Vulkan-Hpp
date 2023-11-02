

# vulkan_raii.hpp: a programming guide

## Introduction

vulkan_raii.hpp is a C++ layer on top of vulkan.hpp that follows the RAII-principle (RAII: Resource Acquisition Is Initialization, see [https://en.cppreference.com/w/cpp/language/raii#:~:text=Resource%20Acquisition%20Is%20Initialization%20or,in%20limited%20supply](https://en.cppreference.com/w/cpp/language/raii#:~:text=Resource%20Acquisition%20Is%20Initialization%20or,in%20limited%20supply)). This header-only library uses all the enums and structure wrappers from vulkan.hpp and provides a new set of wrapper classes for the Vulkan handle types. Instead of creating Vulkan handles with vk*Allocate or vk*Create a constructor of the corresponding Vulkan handle wrapper class is used. And instead of destroying Vulkan handles with vk*Free or vk*Destroy, the destructor of that handle class is called.

## General Usage

As a simple example, instead of creating a `vk::Device`

    // create a vk::Device, given a vk::PhysicalDevice physicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
    vk::Device device = physicalDevice.createDevice( deviceCreateInfo );

and destroying it at some point

    // destroy a vk::Device
    device.destroy();

you would create a `vk::raii::Device`

    // create a vk::raii::Device, given a vk::raii::PhysicalDevice physicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
    vk::raii::Device device( physicalDevice, deviceCreateInfo );

That `vk::raii::Device` is automatically destroyed, when its scope is left.

Alternatively, you can use a creation function to create a `vk::raii::Device`:

    // create a vk::raii::Device, given a vk::raii::PhysicalDevice physicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
    vk::raii::Device device = physicalDevice.createDevice( deviceCreateInfo );

Finally, if you have defined `VULKAN_HPP_NO_EXCPETIONS` and compile for at least C++23, the constructors as described above are not available (they would potentially throw an exception which is not allowed then) but you have to use the construction functions. Those functions then do not return the created object, but a `std::expected<vk::raii::Object, vk::Result>`:

    // create a vk::raii::Device, given a vk::raii::PhysicalDevice physicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
	// when VULKAN_HPP_NO_EXCPETIONS is defined and your using at least C++23
	auto deviceExpected = physicalDevice.createDevice( deviceCreateInfo );
	if ( deviceExpected.has_value() )
	{
		device = std::move( *deviceExpected );
	}

In the code snippets in this text, I will consistently use the constructor-approach.


Other than the `vk::Device`, you can assign the `vk::raii::Device` to a smart pointer:

    // create a smart-pointer to a vk::raii::Device, given a smart-pointer to a vk::raii::PhysicalDevice pPhysicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
    std::unique_ptr<vk::raii::Device> pDevice;
    pDevice = std::make_unique<vk::raii::Device>( *pPhysicalDevice, deviceCreateInfo );

Note that the vk::raii objects own the actual Vulkan resource. Therefore, all vk::raii objects that own destructable resources are just movable, but not copyable. Therefore, a few vk::raii objects, like vk::raii::PhysicalDevice are copyable as well.

For simplicity, in the rest of this document a vk::raii object is always directly instantiated on the stack. Obviously, that's not essential. You could assign them as well to a std::unique_ptr, a std::shared_ptr, or any other smart pointer or object managing data structure. And you can even assign them to a dumb pointer by using the new operator.

Similar to a `vk::Device`, a `vk::raii::Device` provides the functions related to that class. But other than the `vk::Device`, you don't need to provide a device-specific dispatcher to those functions to get multi-device functionality. That's already managed by the `vk::raii::Device`.

That is, calling a device-related function is identical for both cases:

    // call waitIdle from a vk::Device
    device.waitIdle();

    // call waitIdle from a vk::raii::Device
    device.waitIdle();

vk::raii goes one step further. In the vk namespace, most of the functions are members of `vk::Device`. In the vk::raii namespace functions strongly related to a non-dispatchable handle are members of the corresponding vi::raii object. For example, to bind memory to a buffer, in vk namespace you write

    // bind vk::DeviceMemory memory to a vk::Buffer buffer, given vk::DeviceSize memoryOffset
    device.bindBufferMemory( buffer, memory, memoryOffset );

In vk::raii namespace you write

    // bind vk::raii::DeviceMemory memory to a vk::raii::Buffer buffer, given vk::DeviceSize memoryOffset
    buffer.bindMemory( *memory, memoryOffset );
    
Note that `vk::raii::Buffer::bindMemory()`takes a `vk::DeviceMemory` as its first argument, not a `vk::raii::DeviceMemory`. From a vk::raii object you get to the corresponding vk object by just dereferencing the vk::raii object.

## First Steps

### 00 Create a vk::raii::Context

The very first step when using classes from the vk::raii namespace is to instantiate a `vk::raii::Context`. This class has no counterpart in either the vk namespace or the pure C-API of Vulkan. It is the handle to the few functions that are not bound to a `VkInstance` or a `VkDevice`:

    // instantiate a vk::raii::Context
    vk::raii::Context context;

To use any of those "global" functions, your code would look like that:

    // get the API version, using that context
    uint32_t apiVersion = context.enumerateInstanceVersion();

### 01 Create a vk::raii::Instance

To pass that information on to a `vk::raii::Instance`, its constructor gets a reference to that `vk::raii::Context`:

    // instantiate a vk::raii::Instance, given a vk::raii::Context context and a vk::InstanceCreateInfo instanceCreateInfo
    vk::raii::Instance instance( context, instanceCreateInfo );

The `vk::raii::Instance` now holds all the instance-related functions. For example, to get all the `vk::PhysicalDeviceGroupProperties` for an instance, your call would look like this:

    // get all vk::PhysicalDeviceGroupProperties from a vk::raii::Instance instance
    std::vector<vk::PhysicalDeviceGroupProperties> physicalDeviceGroupProperties = instance.enumeratePhysicalDeviceGroups();

### 02 Enumerate the vk::raii::PhysicalDevices

Enumerating the physical devices of an instance is slightly different in vk::raii namespace as you might be used to from the vk-namespace or the pure C-API. As there might be multiple physical devices attached, you would instantiate a `vk::raii::PhysicalDevices` (note the trailing 's' here!), which essentially is a `std::vector` of `vk::raii::PhysicalDevice`s (note the trailing 's' here!):

    // enumerate the vk::raii::PhysicalDevices, given a vk::raii::Instance instance
    vk::raii::PhysicalDevices physicalDevices( instance );

As vk::raii::PhysicalDevices is just a `std::vector<vk::raii::PhysicalDevice>`, you can access any specific `vk::raii:PhysicalDevice` by indexing into that `std::vector`:

    // get the vk::LayerProperties of the vk::raii::PhysicalDevice with index physicalDeviceIndex, given a vk::raii::PhysicalDevices physicalDevices
    std::vector<vk::LayerProperties> layerProperties = physicalDevices[physicalDeviceIndex].enumerateDeviceLayerProperties();

You can as well get one `vk::raii::PhysicalDevice` out of a `vk::raii::PhysicalDevices` like this:

    // get the vk::raii::PhysicalDevice with index physicalDeviceIndex, given a vk::raii::PhysicalDevices physicalDevices object:
    vk::raii::PhysicalDevice physicalDevice( std::move( physicalDevices[physicalDeviceIndex] ) );

Note, that even though the actual `VkPhysicalDevice` owned by a `vk::raii::PhysicalDevice` is not a destructible resource, for consistency reasons a `vk::raii::PhysicalDevice` is a movable but not copyable object just like all the other vk::raii objects.

### 03 Create a vk::raii::Device

To create a `vk::raii::Device`, you just instantiate an object of that class:

    // create a vk::raii::Device, given a vk::raii::PhysicalDevice physicalDevice and a vk::DeviceCreateInfo deviceCreateInfo
    vk::raii::Device device( physicalDevice, deviceCreateInfo );

For each instantiated `vk::raii::Device`, the device-specific Vulkan function pointers are resolved. That is, for multi-device programs, you automatically use the correct device-specific function pointers, and organizing a multi-device program is simplified:

    // create a vk::raii::Device per vk::raii::PhysicalDevice, given a vk::raii::PhysicalDevices physicalDevices, and a corresponding array of vk::DeviceCreateInfo deviceCreateInfos
    std::vector<vk::raii::Device> devices;
    for ( size_t i = 0; i < physicalDevices.size(); i++ )
    {
      devices.push_back( vk::raii::Device( physicalDevices[i], deviceCreateInfos[i] ) );
    }

### 04 Create a vk::raii::CommandPool and vk::raii::CommandBuffers

Creating a `vk::raii::CommandPool` is simply done by instantiating such an object:

    // create a vk::raii::CommandPool, given a vk::raii::Device device and a vk::CommandPoolCreateInfo commandPoolCreateInfo
    vk::raii::CommandPool commandPool( device, commandPoolCreateInfo );

As the number of `vk::raii::CommandBuffer`s to allocate from a `vk::raii::CommandPool` is given by the member `commandBufferCount` of a `vk::CommandBufferAllocateInfo` structure, it can't be instantiated as a single object. Instead you get a `vk::raii::CommandBuffers` (note the trailing 's' here!), which essentially is a `std::vector` of `vk::raii::CommandBuffer`s (note the trailing 's' here!).

    // create a vk::raii::CommandBuffers, given a vk::raii::Device device and a vk::CommandBufferAllocateInfo commandBufferAllocateInfo
    vk::raii::CommandBuffers commandBuffers( device, commandBufferAllocateInfo );

Note, that the `vk::CommandBufferAllocateInfo` holds a `vk::CommandPool` member `commandPool`. To assign that from a `vk::raii::CommandPool` you can use the `operator*()`:

    // assign vk::CommandBufferAllocateInfo::commandPool, given a vk::raii::CommandPool commandPool
    commandBufferAllocateInfo.commandPool = *commandPool;

As a `vk::raii::CommandBuffers` is just a `std::vector<vk::raii::CommandBuffer>`, you can access any specific `vk::raii:CommandBuffer` by indexing into that `std::vector`:

    // start recording of the vk::raii::CommandBuffer with index commandBufferIndex, given a vk::raii::CommandBuffers commandBuffers
    commandBuffers[commandBufferIndex].begin();

You can as well get one `vk::raii::CommandBuffer` out of a `vk::raii::CommandBuffers` like this:

    // get the vk::raii::CommandBuffer with index commandBufferIndex, given a vk::raii::CommandBuffers commandBuffers
    vk::raii::CommandBuffer commandBuffer( std::move( commandBuffers[commandBufferIndex] ) );

    // start recording
    commandBuffer.begin();

There is one important thing to note, regarding command pool and command buffer handling. When you destroy a `VkCommandPool`, all `VkCommandBuffer`s allocated from that pool are implicitly freed. That automatism does not work well with the raii-approach. As the `vk::raii::CommandBuffers` are independent objects, they are not automatically destroyed when the `vk::raii::CommandPool` they are created from is destroyed. Instead, their destructor would try to use an invalid `vk::raii::CommandPool`, which obviously is an error.

To handle that correctly, you have to make sure, that all `vk::raii::CommandBuffers` generated from a `vk::raii::CommandPool` are explicitly destroyed before that `vk::raii::CommandPool` is destroyed!

### 05 Create a vk::raii::SwapchainKHR

To initialize a swap chain, you first instantiate a `vk::raii::SwapchainKHR`:

    // create a vk::raii::SwapchainKHR, given a vk::raii::Device device and a vk::SwapchainCreateInfoKHR swapChainCreateInfo
    vk::raii::SwapchainKHR swapchain( device, swapChainCreateInfo );

You can get an array of presentable images associated with that swap chain:

    // get presentable images associated with vk::raii::SwapchainKHR swapchain
    std::vector<VkImage> images = swapchain.getImages();

Note, that you don't get `vk::raii::Image`s here, but plain `VkImage`s. They are controlled by the swap chain, and you should not destroy them.

But you can create `vk::raii::ImageView`s out of them:

    // create a vk::raii::ImageView per VkImage, given a vk::raii::Device sevice, a vector of VkImages images and a vk::ImageViewCreateInfo imageViewCreateInfo
    std::vector<vk::raii::ImageView> imageViews;
    for ( auto image : images )
    {
      imageViewCreatInfo.image = image;
      imageViews.push_back( vk::raii::ImageView( device, imageViewCreateInfo ) );
    }

### 06 Create a Depth Buffer

For a depth buffer, you need an image and some device memory and bind the memory to that image. That is, you first create a vk::raii::Image

    // create a vk::raii::Image image, given a vk::raii::Device device and a vk::ImageCreateInfo imageCreateInfo
    // imageCreateInfo.usage should hold vk::ImageUsageFlagBits::eDepthStencilAttachment
    vk::raii::Image depthImage( device, imageCreateInfo );

To create the corresponding vk::raii::DeviceMemory, you should determine appropriate values for the vk::MemoryAllocateInfo. That is, get the memory requirements from the pDepthImage, and determine some memoryTypeIndex from the pPhysicalDevice's memory properties, requiring vk::MemoryPropertyFlagBits::eDeviceLocal.

    // get the vk::MemoryRequirements of the pDepthImage
    vk::MemoryRequirements memoryRequirements = depthImage.getMemoryRequirements();

    // determine appropriate memory type index, using some helper function determineMemoryTypeIndex
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
    uint32_t memoryTypeIndex = determineMemoryTypeIndex( memoryProperties, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal );

    // create a vk::raii::DeviceMemory depthDeviceMemory for the depth buffer
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    vk::raii::DeviceMemory depthDeviceMemory( device, memoryAllocateInfo );

Then you can bind the depth memory to the depth image

    // bind the pDepthMemory to the pDepthImage
    depthImage.bindMemory( *depthDeviceMemory, 0 );

Finally, you can create an image view on that depth buffer image

    // create a vk::raii::ImageView depthView, given a vk::ImageViewCreateInfo imageViewCreateInfo
    imageViewCreateInfo.image = *depthImage;
    vk::raii::ImageView depthImageView( device, imageViewCreateInfo );

### 07 Create a Uniform Buffer

Initializing a uniform buffer is very similar to initializing a depth buffer as described above. You just instantiate a `vk::raii::Buffer` instead of a `vk::raii::Image`, and a `vk::raii::DeviceMemory`, and bind the memory to the buffer:

    // create a vk::raii::Buffer, given a vk::raii::Device device and a vk::BufferCreateInfo bufferCreateInfo
    vk::raii::Buffer uniformBuffer( device, bufferCreateInfo );

    // get memoryRequirements for this uniform buffer
    vk::MemoryRequirements memoryRequirements = uniformBuffer.getMemoryRequirements();

    // determine appropriate memory type index, using some helper function, given a vk::raii::PhysicalDevice physicalDevice and some memoryPropertyFlags
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
    uint32_t memoryTypeIndex = determineMemoryTypeIndex( memoryProperties, memoryRequirements.memoryTypeBits, memoryPropertyFlags );
    
    // create a vk::raii::DeviceMemory uniformDeviceMemory for the uniform buffer
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    vk::raii::DeviceMemory uniformDeviceMemory( device, memoryAllocateInfo );
    
    // bind the vk::raii::DeviceMemory uniformDeviceMemory to the vk::raii::Buffer uniformBuffer
    uniformBuffer.bindMemory( *uniformDeviceMemory, 0 );

### 08 Create a vk::raii::PipelineLayout

To initialize a Pipeline Layout you just have to instantiate a `vk::raii::DescriptorSetLayout` and a `vk::raii::PipelineLayout` using that `vk::raii::DescriptorSetLayout`:

    // create a vk::raii::DescriptorSetLayout, given a vk::raii::Device device and a vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo
    vk::raii::DescriptorSetLayout descriptorSetLayout( device, descriptorSetLayoutCreateInfo );

    // create a vk::raii::PipelineLayout, given a vk::raii::Device device and a vk::raii::DescriptorSetLayout
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo( {}, *descriptorSetLayout );
    vk::raii::PipelineLayout pipelineLayout( device, pipelineLayoutCreateInfo );

### 09 Create a vk::raii::DescriptorPool and vk::raii::DescriptorSets

The Descriptor Set handling with `vk::raii` requires some special handling that is not needed when using the pure C-API or the vk-namespace!

As a `vk::raii::DescriptorSet` object destroys itself in the destructor, you have to instantiate the corresponding `vk::raii::DescriptorPool` with the `vk::DescriptorPoolCreateInfo::flags` set to (at least) `vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet`. Otherwise, such individual destruction of a `vk::raii::DescriptorSet` would not be allowed!

That is, an instantiation of a `vk::raii::DescriptorPool` would look like this:

    // create a vk::raii::DescriptorPool, given a vk::raii::Device device and a vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo
    assert( descriptorPoolCreateInfo.flags & vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet );
    vk::raii::DescriptorPool descriptorPool( device, descriptorPoolCreateInfo );

To actually instantiate a `vk::raii::DescriptorSet`, you need a `vk::raii::DescriptorPool`, as just described, and a `vk::raii::DescriptorSetLayout`, similar to the one described in the previous section.

Moreover, as the number of `vk::raii::DescriptorSet`s to allocate from a `vk::raii::DescriptorPool` is given by the number of `vk::DescriptorSetLayouts` held by a `vk::DescriptorSetAllocateInfo`, it can't be instantiated as a single object. Instead you get a `vk::raii::DescriptorSets` (note the trailing 's' here!), which essentially is a `std::vector` of `vk::raii::DescriptorSet`s (note the trailing 's' here!).

When you want to create just one `vk::raii::DescriptorSet`, using just one `vk::raii::DescriptorSetLayout`, your code might look like this:

    // create a vk::raii::DescriptorSets, holding a single vk::raii::DescriptorSet, given a vk::raii::Device device, a vk::raii::DescriptorPool descriptorPool, and a single vk::raii::DescriptorSetLayout descriptorSetLayout
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( *descriptorPool, *descriptorSetLayout );
    vk::raii::DescriptorSets pDescriptorSets( device, descriptorSetAllocateInfo );

And, again similar to the vk::raii::CommandBuffers handling described above, you can get one `vk::raii::DescriptorSet` out of a `vk::raii::DescriptorSets` like this:

    // get the vk::raii::DescriptorSet with index descriptorSetIndex, given a vk::raii::DescriptorSets descriptorSets
    vk::raii::DescriptorSet descriptorSet( std::move( descriptorSets[descriptorSetIndex] ) );

### 10 Create a vk::raii::RenderPass

Creating a `vk::raii::RenderPass` is pretty simple, given you already have a meaningful `vk::RenderPassCreateInfo`:

    // create a vk::raii::RenderPass, given a vk::raii::Device device and a vk::RenderPassCreateInfo renderPassCreateInfo
    vk::raii::RenderPass renderPass( device, renderPassCreateInfo );

### 11 Create a vk::raii::ShaderModule

Again, creating a `vk::raii::ShaderModule` is simple, given a `vk::ShaderModuleCreateInfo` with some meaningful code:

    // create a vk::raii::ShaderModule, given a vk::raii::Device device and a vk::ShaderModuleCreateInfo shaderModuleCreateInfo
    vk::raii::ShaderModule shaderModule( device, shaderModuleCreateInfo );

### 12 Create vk::raii::Framebuffers

If you have a `std::vector<vk::raii::ImageView>` as described in chapter 05 above, with one view per `VkImage` that you got from a `vk::raii::SwapchainKHR`; and one `vk::raii::ImageView` as described in chapter 06 above, which is a view on a `vk::raii::Image`, that is supposed to be a depth buffer, you can create a `vk::raii::Framebuffer` per swapchain image.

    // create a vector of vk::raii::Framebuffer, given a vk::raii::ImageView depthImageView, a vector of vk::raii::ImageView swapchainImageViews, a vk::raii::RenderPass renderPass, a vk::raii::Devie device, and some width and height
    // use the depth image view as the second attachment for each vk::raii::Framebuffer
    std::array<vk::ImageView, 2> attachments;
    attachments[1] = *depthImageView;
    std::vector<vk::raii::Framebuffer> framebuffers;
    for ( auto const & imageView : swapchainImageViews )
    {
      // use each image view from the swapchain as the first attachment
      attachments[0] = *imageView;
      vk::FramebufferCreateInfo framebufferCreateInfo( {}, *renderPass, attachments, width, height, 1 );
      framebuffers.push_back( vk::raii::Framebuffer( device, framebufferCreateInfo ) );
    }

### 13 Initialize a Vertex Buffer

To initialize a vertex buffer, you essentially have to combine some of the pieces described in the chapters before. First, you need to create a `vk::raii::Buffer` and a `vk::raii::DeviceMemory` and bind them:

    // create a vk::raii::Buffer vertexBuffer, given a vk::raii::Device device and some vertexData in host memory
    vk::BufferCreateInfo bufferCreateInfo( {}, sizeof( vertexData ), vk::BufferUsageFlagBits::eVertexBuffer );
    vk::raii::Buffer vertexBuffer( device, bufferCreateInfo );

    // create a vk::raii::DeviceMemory vertexDeviceMemory, given a vk::raii::Device device and a uint32_t memoryTypeIndex
    vk::MemoryRequirements memoryRequirements = vertexBuffer.getMemoryRequirements();
    vk::MemoryAllocateInfo memoryAllocateInfo( memoryRequirements.size, memoryTypeIndex );
    vk::raii::DeviceMemory vertexDeviceMemory( device, memoryAllocateInfo );

    // bind the complete device memory to the vertex buffer
    vertexBuffer.bindMemory( *vertexDeviceMemory, 0 );

    // copy the vertex data into the vertexDeviceMemory
    ...

Later on, you can bind that vertex buffer to a command buffer:

    // bind a complete single vk::raii::Buffer vertexBuffer as a vertex buffer, given a vk::raii::CommandBuffer commandBuffer
    commandBuffer.bindVertexBuffer( 0, { *vertexBuffer }, { 0 } );

### 14 Initialize a Graphics Pipeline

Initializing a graphics pipeline is not very raii-specific. Just instantiate it, provided you have a valid vk::GraphicsPipelineCreateInfo:

    // create a vk::raii::Pipeline, given a vk::raii::Device device and a vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo
    vk::raii::Pipeline graphicsPipeline( device, graphicsPipelineCreateInfo );

The only thing to keep in mind here is the dereferencing of raii handles, like `pipelineLayout` or `renderPass` in the `vk::GraphicsPipelineCreateInfo`:

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(
      {},                                    // flags
      pipelineShaderStageCreateInfos,        // stages
      &pipelineVertexInputStateCreateInfo,   // pVertexInputState
      &pipelineInputAssemblyStateCreateInfo, // pInputAssemblyState
      nullptr,                               // pTessellationState
      &pipelineViewportStateCreateInfo,      // pViewportState
      &pipelineRasterizationStateCreateInfo, // pRasterizationState
      &pipelineMultisampleStateCreateInfo,   // pMultisampleState
      &pipelineDepthStencilStateCreateInfo,  // pDepthStencilState
      &pipelineColorBlendStateCreateInfo,    // pColorBlendState
      &pipelineDynamicStateCreateInfo,       // pDynamicState
      *pipelineLayout,                       // layout
      *renderPass                            // renderPass
    );

### 15 Drawing a Cube

Finally, we get all those pieces together and draw a cube.

To do so, you need a `vk::raii::Semaphore`:

    // create a vk::raii::Semaphore, given a vk::raii::Device
    vk::raii::Semaphore imageAcquiredSemphore( device, vk::SemaphoreCreateInfo() );

That semaphore can be used, to acquire the next imageIndex from the `vk::raii::SwapchainKHR` swapchain:

    vk::Result result;
    uint32_t imageIndex;
    std::tie( result, imageIndex ) = swapchain.acquireNextImage( timeout, *imageAcquiredSemaphore );

Note, `vk::raii::SwapchainKHR::acquireNextImage` returns a `std::pair<vk::Result, uint32_t>`, that can nicely be assigned onto two separate values using std::tie().

And also note, the returned `vk::Result` can not only be `vk::Result::eSuccess`, but also `vk::Result::eTimeout`, `vk::Result::eNotReady`, or `vk::Result::eSuboptimalKHR`, which should be handled here accordingly!

Next, you can record some commands into a `vk::raii::CommandBuffer`:

    // open the commandBuffer for recording
    commandBuffer.begin( {} );

    // initialize a vk::RenderPassBeginInfo with the current imageIndex and some appropriate renderArea and clearValues
    vk::RenderPassBeginInfo renderPassBeginInfo( *renderPass, *framebuffers[imageIndex], renderArea, clearValues );

    // begin the render pass with an inlined subpass; no secondary command buffers allowed
    commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );

    // bind the graphics pipeline
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, *graphicsPipeline );

    // bind an appropriate descriptor set
    commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, *pipelineLayout, 0, { *descriptorSet }, nullptr );

    // bind the vertex buffer
    commandBuffer.bindVertexBuffers( 0, { *vertexBuffer }, { 0 } );

    // set viewport and scissor
    commandBuffer.setViewport( 0, viewport );
    commandBuffer.setScissor( renderArea );

    // draw the 12 * 3 vertices once, starting with vertex 0 and instance 0
    commandBuffer.draw( 12 * 3, 1, 0, 0 );

    // end the render pass and stop recording
    commandBuffer.endRenderPass();
    commandBuffer.end();

To submit that command buffer to a `vk::raii::Queue` graphicsQueue you might want to use a `vk::raii::Fence`

    // create a vk::raii::Fence, given a vk::raii::Device device
    vk::raii::Fence fence( device, vk::FenceCreateInfo() );

With that, you can fill a `vk::SubmitInfo` and submit the command buffer

    vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    vk::SubmitInfo submitInfo( *imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer );
    graphicsQueue.submit( submitInfo, *fence );

At some later point, you can wait for that submit being ready by waiting for the fence

    while ( vk::Result::eTimeout == device.waitForFences( { *fence }, VK_TRUE, timeout ) )
      ;

And finally, you can use the `vk::raii::Queue` presentQueue to, well, present that image

    vk::PresentInfoKHR presentInfoKHR( nullptr, *swapChain, imageIndex );
    result = presentQueue.presentKHR( presentInfoKHR );

Note here, again, that `result` can not only be `vk::Result::eSuccess`, but also `vk::Result::eSuboptimalKHR`, which should be handled accordingly.

## Conclusion

With the vk::raii namespace you've got a complete set of Vulkan handle wrapper classes following the RAII-paradigm. That is, they can easily be assigned to a smart pointer. And you can't miss their destruction.

Moreover, the actual function pointer handling is done automatically by `vk::raii::Context`, `vk::raii::Instance`, and `vk::raii::Device`. That is, you always use the correct device-specific functions, no matter how many devices you're using.

Note, though, that there are a few classes, like `vk::raii::CommandPool` and `vk::raii::DescriptorSet`, that need some special handling that deviates from what you can do with the pure C-API or the wrapper classes in the vk-namespace.
