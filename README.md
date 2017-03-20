# Vulkan-Hpp: C++ Bindings for Vulkan

The goal of the Vulkan-Hpp is to provide header only C++ bindings for the Vulkan C API to improve the developers Vulkan experience without introducing CPU runtime cost. It adds features like type safety for enums and bitfields, STL container support, exceptions and simple enumerations.

# Getting Started
Vulkan-Hpp is part of the LunarG Vulkan SDK since version 1.0.24. Just include ```<vulkan.hpp>``` and you're ready to use the C++ bindings. If you're using a Vulkan version not yet supported by the Vulkan SDK you can find the latest version of the header [here](https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/vulkan/vulkan.hpp).

# Minimum Requirements
Vulkan-Hpp requires a C++11 capable compiler to compile. The following compilers are known to work:
* Visual Studio >=2013
* GCC >= 4.8.2 (earlier version might work, but are untested)
* Clang >= 3.3

# namespace vk
To avoid name collisions with the Vulkan C API the C++ bindings reside in the vk namespace. The following rules apply to the new naming
* All functions, enums, handles, and structs have the Vk prefix removed. In addition to this the first leter of functions is lower case.
  * ```vkCreateImage``` can be accessed as ```vk::createImage```
  * ```VkImageTiling``` can be accessed as ```vk::ImageTiling```
  * ```VkImageCreateInfo``` can be accessed as ```vk::ImageCreateInfo```
* Enums are mapped to scoped enums to provide compile time type safety. The names have been changed to 'e' + CamelCase with the VK_ prefix and type infix removed. In case the enum type is an extension the extension suffix has been removed from the enum values.
 In all other cases the extension suffix has not been removed.
  * ```VK_IMAGETYPE_2D``` is now ```vk::ImageType::e2D```.
  * ```VK_COLOR_SPACE_SRGB_NONLINEAR_KHR``` is now ```vk::ColorSpaceKHR::eSrgbNonlinear```.
  *  ```VK_STRUCTURE_TYPE_PRESENT_INFO_KHR``` is now ```vk::StructureType::ePresentInfoKHR```.
*  Flag bits are handled like scoped enums with the addition that the _BIT suffix has also been removed.

# Handles
Vulkan-Hpp declares a class for all handles to ensure full type safety and to add support for member functions on handles. A member function has been added to a handle class for each
 function which accepts the corresponding handle as first parameter. Instead of ```vkBindBufferMemory(device, ...)``` one can write ```device.bindBufferMemory(...)``` or ```vk::bindBufferMemory(device, ...)```.

# C/C++ Interop for Handles
On 64-bit platforms Vulkan-Hpp supports implicit conversions between C++ Vulkan handles and C Vulkan handles. On 32-bit platforms all non-dispatchable handles are defined as ```uint64_t```,
thus preventing type-conversion checks at compile time which would catch assignments between incompatible handle types..
Due to that Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default and it is recommended to use a static_cast for the conversion like this: ```VkDevice = static_cast<VkDevice>(cppDevice)```
to prevent converting some arbitrary int to a handle or vice versa by accident. If you're developing your code on a 64-bit platform, but want compile your code for a 32-bit platform without adding the explicit casts
you can define ```VULKAN_HPP_TYPESAFE_CONVERSION``` to 1 in your build system or before including ```vulkan.hpp```. On 64-bit platforms this define is set to 1 by default and can be set to 0 to disable implicit conversions.

# Flags
The scoped enum feature adds type safety to the flags, but also prevents using the flag bits as input for bitwise operations like & and |.
As solution Vulkan-Hpp provides a template class ```vk::Flags``` which brings the standard operations like &=, |=, & and | to our scoped enums. Except for the initialization with 0 this class behaves exactly like a normal bitmask
 with the improvement that it is impossible to set bits not specified by the corresponding enum by accident. Here are a few examples for the bitmask handling:

```c++
vk::ImageUsage iu1; // initialize a bitmask with no bit set
vk::ImageUsage iu2 = {}; // initialize a bitmask with no bit set
vk::ImageUsage iu3 = vk::ImageUsage::eColorAttachment; // initialize with a single value
vk::ImageUsage iu4 = vk::ImageUsage::eColorAttachment | vk::ImageUsage::eStorage; // or two bits to get a bitmask
PipelineShaderStageCreateInfo ci( {} /* pass a flag without any bits set, ...);
```

# CreateInfo structs
When constructing a handle in Vulkan one usually has to create some ```CreateInfo``` struct which describes the new handle. This can result in quite lengthy code as can be seen in the following Vulkan C example:

```c++
 VkImageCreateInfo ci;
  ci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.flags = ...some flags...;
  ci.imageType = VK_IMAGE_TYPE_2D;
  ci.format = VK_FORMAT_R8G8B8A8_UNORM;
  ci.extent = VkExtent3D { width, height, 1 };
  ci.mipLevels = 1;
  ci.arrayLayers = 1;
  ci.samples = VK_SAMPLE_COUNT_1_BIT;
  ci.tiling = VK_IMAGE_TILING_OPTIMAL;
  ci.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  ci.queueFamilyIndexCount = 0;
  ci.pQueueFamilyIndices = 0;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  vkCreateImage(device, &ci, allocator, &image));
```

There are two typical issues Vulkan developers encounter when filling out a CreateInfo struct field by field
* One or more fields are left uninitialized.
* sType is incorrect.

Especially the first one is hard to detect.

Vulkan-Hpp provides constructors for all CreateInfo objects which accept one parameter for each member variable. This way the compiler throws a compiler error if a value has been forgotten. In addition to this sType is automatically
 filled with the correct value and pNext set to a nullptr by default. Here's how the same code looks with a constructor:

```c++
 vk::ImageCreateInfo ci({}, vk::ImageType::e2D, vk::format::eR8G8B8A8Unorm,
                        { width, height, 1 },
                        1, 1, vk::SampleCount::e1,
                        vk::ImageTiling::eOptimal, vk::ImageUsage:eColorAttachment,
                        vk::SharingMode::eExclusive, 0, 0, vk::Imagelayout::eUndefined);
```

With constructors for CreateInfo structures one can also pass temporaries to Vulkan functions like this:
```c++
  vk::Image image = device.createImage({{}, vk::ImageType::e2D, vk::format::eR8G8B8A8Unorm,
                                       { width, height, 1 },
                                       1, 1, vk::SampleCount::e1,
                                       vk::ImageTiling::eOptimal, vk::ImageUsage:eColorAttachment,
                                       vk::SharingMode::eExclusive, 0, 0, vk::Imagelayout::eUndefined});
```

# Passing Arrays to Functions: The ArrayProxy
The Vulkan API has several places where which require (count,pointer) as two function arguments and C++ has a few containers which map perfectly to this pair. To simplify development the Vulkan-Hpp bindings have replaced those argument pairs
with the ArrayProxy template class which accepts empty arrays and a single value as well as STL containers std::initializer_list, std::array and std::vector as argument for construction. This way a single generated Vulkan version can accept
a variety of inputs without having the combinatoric explosion which would occur when creating a function for each container type.

Here are some code samples on how to use the ArrayProxy:

```c++
vk::CommandBuffer c;

// pass an empty array
c.setScissor(0, nullptr);

// pass a single value. Value is passed as reference
vk::Rect2D scissorRect = { {0, 0}, {640, 480} };
c.setScissor(0, scissorRect);

// pass a temporary value.
c.setScissor(0, { { 0, 0 },{ 640, 480 } });

// generate a std::initializer_list using two rectangles from the stack. This might generate a copy of the rectangles.
vk::Rect2D scissorRect1 = { { 0, 0 },{ 320, 240 } };
vk::Rect2D scissorRect2 = { { 320, 240 },{ 320, 240 } };
c.setScissor(0, { scissorRect, scissorRect2 });

// construct a std::initializer_list using two temporary rectangles.
c.setScissor(0, { { {   0,   0 },{ 320, 240 } },
                { { 320, 240 },{ 320, 240 } }
}
);

// pass a std::array
std::array<vk::Rect2D, 2> arr{ scissorRect1, scissorRect2 };
c.setScissor(0, arr);

// pass a std::vector of dynamic size
std::vector<vk::Rect2D> vec;
vec.push_back(scissorRect1);
vec.push_back(scissorRect2);
c.setScissor(0, vec);
```

# Passing Structs to Functions
Vulkan-Hpp generates references for pointers to structs. This conversion allows passing temporary structs to functions which can result in shorter code. In case the input is optional and thus accepting a null pointer the parameter type will be
a ```vk::Optional<T> const&``` type. This type accepts either a reference to ```T``` or nullptr as input and thus allows optional temporary structs.

```c++
// C
ImageSubResource subResource;
subResource.aspectMask = 0;
subResource.mipLevel = 0;
subResource.arrayLayer = 0;
vkSubresourceLayout layout = vkGetImageSubResourceLayout(image, subresource);

// C++
auto layout = device.getImageSubResourceLayout(image, { {} /* flags*/, 0 /* miplevel */, 0 /* layout */ });
```

# Return values, Error Codes & Exceptions
By default Vulkan-Hpp has exceptions enabled. This means that Vulkan-Hpp checks the return code of each function call which returns a Vk::Result. If Vk::Result is a failure a std::runtime_error will be thrown.
Since there is no need to return the error code anymore the C++ bindings can now return the actual desired return value, i.e. a vulkan handle. In those cases ResultValue <SomeType>::type is defined as the returned type.
 To create a device you can now just write:

```C++
vk::Device device = physicalDevice.createDevice(createInfo);
```

If exception handling is disabled by defining ```VULKAN_HPP_NO_EXCEPTIONS``` the type of ```ResultValue<SomeType>::type``` is a struct holding a ```vk::Result``` and a ```SomeType```.
This struct supports unpacking the return values by using ```std::tie```.

In case you don’t want to use the ```vk::ArrayProxy``` and return value transformation you can still call the plain C-style function. Below are three examples showing the 3 ways to use the API:

The first snippet shows how to use the API without exceptions and the return value transformation:

```c++
// No exceptions, no return value transformation
ShaderModuleCreateInfo createInfo(...);
ShaderModule shader1;
Result result = device.createShaderModule(&createInfo, allocator, &shader1);
if (result.result != VK_SUCCESS)
{
    handle error code;
    cleanup?
    return?
}

ShaderModule shader2;
Result result = device.createShaderModule(&createInfo, allocator, &shader2);
if (result != VK_SUCCESS)
{
    handle error code;
    cleanup?
    return?
}
```

The second snippet shows how to use the API using return value transformation, but without exceptions. It’s already a little bit shorter than the original code:

```c++
ResultValue<ShaderModule> shaderResult1 = device.createShaderModule({...} /* createInfo temporary */);
if (shaderResult1.result != VK_SUCCESS)
{
  handle error code;
  cleanup?
  return?
}

// std::tie support.
vk::Result result;
vk::ShaderModule shaderModule2;
std::tie(result, shaderModule2)  = device.createShaderModule({...} /* createInfo temporary */);
if (shaderResult2.result != VK_SUCCESS)
{
  handle error code;
  cleanup?
  return?
}
```

A nicer way to unpack the result is provided by the structured bindings of C++17. They will allow us to get the result with a single line of code:

```c++
auto [result, shaderModule2] = device.createShaderModule({...} /* createInfo temporary */);
```

Finally, the last code example is using exceptions and return value transformation. This is the default mode of the API.

```c++
 ShaderModule shader1;
 ShaderModule shader2;
 try {
   myHandle = device.createShaderModule({...});
   myHandle2 = device.createShaderModule({...});
 } catch(std::exception const &e) {
   // handle error and free resources
 }
```
Keep in mind that Vulkan-Hpp does not support RAII style handles and that you have to cleanup your resources in the error handler!

# Enumerations
For the return value transformation, there's one special class of return values which require special handling: Enumerations. For enumerations you usually have to write code like this:

```c++
std::vector<LayerProperties,Allocator> properties;
uint32_t propertyCount;
Result result;
do
{
  // determine number of elements to query
  result = static_cast<Result>( vk::enumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, nullptr ) );
  if ( ( result == Result::eSuccess ) && propertyCount )
  {
    // allocate memory & query again
    properties.resize( propertyCount );
    result = static_cast<Result>( vk::enumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast
     <VkLayerProperties*>( properties.data() ) ) );
  }
} while ( result == Result::eIncomplete );
// it's possible that the count has changed, start again if properties was not big enough
properties.resize(propertyCount);
```
Since writing this loop over and over again is tedious and error prone the C++ binding takes care of the enumeration so that you can just write:

std::vector<LayerProperties> properties = physicalDevice.enumerateDeviceLayerProperties();

# Custom allocators
Sometimes it is required to use ```std::vector``` with custom allocators. Vulkan-Hpp supports vectors with custom allocators as input for ```vk::ArrayProx``` and for functions which do return a vector. For the latter case, add your favorite custom allocator as template argument to the function call like this:

```c++
std::vector<LayerProperties, MyCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties<MyCustomAllocator>();
```
