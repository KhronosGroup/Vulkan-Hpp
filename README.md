# Open-Source Vulkan C++ API

Vulkan is a C API and as such inherits all common pitfalls of using a general C programming library. The motivation of a low-level Vulkan C++ API is to avoid these common pitfalls by applying commonly known C++ features while keeping the overall structure of a Vulkan program and preserving the full freedom it provides as low-level graphics API. An additional guideline we followed was not to introduce additional runtime overhead by providing a header-only library with inline functions.

Have a look at the following piece of code which creates a VkImage:

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

There may be some issues that can happen when filling the structure which cannot be caught at compile time:

* initialization of ```ci.sType``` using wrong enum values
* uninitialized data fields (e.g. missing initialization of ```ci.mipLevels```) 
* use of invalid bits for ```ci.flags``` (no type-safety for bits)
* use of incorrect enums for fields (no type-safety for enums)

These initializations will most likely show up as random runtime errors, which usually are nasty and time-consuming to debug.
Our auto-generated, C++ 11-conform layer uses commonly known C++ features like implicit initialization through constructors
to avoid incorrect or missing  initializations and introduces type-safety with scoped enums to turn explicit initialization
errors into compile errors. Following is a list of features and conventions introduced by our Vulkan C++ layer:

* works along the official C version of the API
* defines all symbols within the 'vk' namespace and to avoid redundancy the vk/Vk/VK_ prefixes have been removed from all symbols, i.e. ```vk::ImageCreateInfo``` for VkImageCreateInfo.
* camel case syntax with an 'e' prefix has been introduced for all enums, i.e. ```vk::ImageType::e2D``` (the prefix was a compromise, more about that later) removes the 'BIT' suffix from all flag related enums, i.e. ```vk::ImageUsage::eColorAttachment```.
* introduces constructors for all structs, which by default set the appropriate ```sType``` and all other values to zero.
* introduces wrapper classes around the vulkan handles, i.e. ```vk::CommandBuffer``` for VkCommandBuffer
* introduces member functions of those wrapper classes, that map to vulkan functions getting the corresponding vulkan handle as its first argument. The type of that handle is stripped from the function name, i.e. ```vk::Device::getProcAddr``` for vkGetDeviceProcAddr. Note the special handling for the class CommandBuffer, where most of the vulkan functions would just include "Cmd", instead of "CommandBuffer", i.e. ```vk::CommandBuffer::bindPipeline``` for vkCmdBindPipeline.
With those changes applied, the updated code snippet looks like this:


```c++
vk::ImageCreateInfo ci;
ci.flags = ...some flags...;
ci.imageType = vk::ImageType::e2D;
ci.format = vk::Format::eR8G8B8A8Unorm;
ci.extent = vk::Extent3D { width, height, 1 };
ci.mipLevels = 1;
ci.arrayLayers = 1;
ci.samples = 1;
ci.tiling = vk::ImageTiling::eOptimal;
ci.usage = vk::ImageUsage::eColorAttachment;
ci.sharingMode = vk::SharingMode::eExclusive;
  // ci.queueFamilyIndexCount = 0	// no need to set, already initialized
  // ci.pQueueFamilyIndices = 0	// no need to set, already initialized
ci.initialLayout = vk::ImageLayout::eUndefined;
device.createImage(&ci, allocator, &image);
```

Which is a total of 13 lines of code, versus 17 lines for the C version. In addition, this code is more robust as described above.

# Type-safe Enums

Splitting up the C enums into a namespace and scoped enums resulted in two compilation issues. 
First some enums started with a digit like ```vk::ImageType::1D``` which resulted in a compilation error. 
Second, there's the risk that upper symbols like ```vk::CompositeAlphaFlagBitsKHR::OPAQUE``` do clash with preprocessor defines.
In the given example ```OPAQUE``` has been defined in ```win32gdi.h``` resulting a compilation error.

To overcome those two issues the symbols have been converted to camel case and the prefix 'e' has been added so that each enum starts with a letter.

# Improvements to Bit Flags

After those changes the code might look more familiar to C++ developers, but there is still no gain with regards to safety.
 With C++ features available we replaced all Vulkan enums with scoped enums to achieve type safety which already uncovered
a few small issues in our code. The good thing with scoped enums is that there is no implicit casts to integer types anymore.
The downside is that OR'ing the bits for the flags does not work anymore without an explicit cast. As a solution to this problem
we have introduced a new ```vk::Flags<T>``` template which is used for all flags. This class supports the standard
operations one usually needs on bitmasks like &=, |=, & and |. Except for the initialization with 0, which is being replaced by
the default constructor, the ```vk::Flags<T>``` class works exactly like a normal bitmask with the improvement that
it is impossible to set bits not specified by the corresponding enum. To generate a bit mask with two bits set write:

```c++
ci.usage = vk::ImageUsage::eColorAttachment | vk::ImageUsage::eStorage;
```

By adding the scoped enums and ```vk::Flags<T>``` the C++ API provides type safety for all enums and flags which is a
big improvement. This leaves the remaining issue that the compiler might not detect uninitialized fields in structs. As a solution
we have added constructors to all structs which accept all values defined by the corresponding struct. 

```c++
vk::ImageCreateInfo ci(
  ...some flags..., vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
  vk::Extent3D { width, height, 1 }, 1, 1,
  vk::SampleCount::e1, vk::ImageTiling::eOptimal,
  vk::ImageUsage::eColorAttachment, vk::SharingMode::eExclusive,
  0, 0, vk::ImageLayout::eUndefined);
```

# String conversions

At development time it can be quite handy to have a utility function that can convert an enum or flags to a string for debugging purposes. To achieve this,
we have implemented ```to_string(type)``` functions for all enums and flags. Calling ```to_string(vk::SharingMode::eExclusive)``` will return 'Exclusive' and calling
```to_string(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)``` will return the concatenated string 'Graphics | Compute'.

# Alternative Initialization of Structs

Another nice feature of those constructors is that sType is being initialized internally and thus is always correct.

Finally, we have added a default constructor to each struct which initializes all values to 0 to allow setting the values with a variant of the named parameter idiom which is similar to the designated initializer list of C99.

```c++
vk::ImageCreateInfo ci = vk::ImageCreateInfo()
  .setFlags(...some flags...)
  .setImageType(vk::ImageType::e2D)
  .setFormat(vk::Format::eR8G8B8A8Unorm)
  .setExtent(vk::Extent3D { width, height, 1 })
  .setMipLevels(1)
  .setArrayLayers(1)
  .setSamples(1)
  .setTiling(vk::ImageTiling::eOptimal)
  .setUsage(vk::ImageUsage::eColorAttachment)
  .setSharingMode(vk::SharingMode::eExclusive)
  // .setQueueFamilyIndexCount(0)	// no need to set, already initialized
  // .setPQueueFamilyIndices(0)	// no need to set, already initialized
  .setInitialLayout(vk::ImageLayout::eUndefined);
device.createImage(&ci, allocator, &image);
```

# Enhancements beyond native Vulkan
To provide a more object oriented feeling we're providing classes for each handle which include all Vulkan functions where the first 
parameter matches the handle. In addition to this we made a few changes to the signatures of the member functions
* To disable the enhanced mode put ```#define VULKAN_HPP_DISABLE_ENHANCED_MODE``` before including ```vulkan.hpp```
* ```(count, T*)``` has been replaced by ```vk::ArrayProxy<T>```, which can be created out of a single T, a (count, T*) pair, a std::array<T,N>, a vector<T>, or an initializer_list<T>.
* ```const char *``` has been replaced by ```const std::string &```
* ```const T *``` has been replaced by ```const T &``` to allow temporary objects. This is useful to pass small structures like ```vk::ClearColorValue``` or ```vk::Extent*```
```commandBuffer.clearColorImage(image, layout, std::array<float, 4>{1.0f, 1.0f, 1.0f, 1.0f}, {...});```
Optional parameters are being replaced by ```Optional<T>``` which accept a type of ```const T```, ```T```, or ```const std::string```. ```nullptr``` can be used to initialize an empty ```Optional<T>```.

Here are a few code examples:
```c++
  try {
    VkInstance nativeInstance = nullptr; // Fetch the instance from a favorite toolkit

    // create a vk::Instance handle from a native handle
    vk::Instance i(nativeInstance);

    // operator=(VkInstance const &) is also supported
    i = nativeInstance;

    // Get VkInstance from vk::Instance 
    nativeInstance = i;

    // Get a std::vector as result of an enumeration call.
    std::vector<vk::PhysicalDevice> physicalDevices = i.enumeratePhysicalDevices();
    vk::FormatProperties formatProperties = physicalDevices[0].getFormatProperties(vk::Format::eR8G8B8A8Unorm);

    vk::CommandBuffer commandBuffer = ...;
    vk::Buffer buffer = ...;

    // Accept std::vector as source for updateBuffer
    commandBuffer.updateBuffer(buffer, 0, {some values}); // update buffer with std::vector

    // Sometimes it's necessary to pass a nullptr to a struct. For this case we've added Optional<T>(std::nullptr_t).
    device.allocateMemory(allocateInfo, nullptr);

  }
  catch (const std::exception &e)
  {
    std::cerr << "Vulkan failure: " << e.what() << std::endl;
  }
```
# Exceptions and return types
The wrapper functions will throw a ```std::system_error``` if the result of the wrapped function is not a success code.
By defining ```VULKAN_HPP_NO_EXCEPTIONS``` before include vulkan.hpp, this can be disabled.
Depending on exceptions being enabled or disabled, the return type of some functions change.

With exceptions enabled (the default) there are four different cases on the return types:
* Just one possible success code
* * no output value -> return type is ```void```
* * one output value -> return type is T, which is the type of the output value
* Multiple possible success codes
* * no output value -> return type is ```vk::Result```
* * one output value -> return type is a structure ```vk::ResultValue<T>``` with a member ```result``` of type ```vk::Result``` holding the actual result code, and a member ```value``` of type T, which is the type of the output value, holding that output value.

With exceptions disabled, the return type of those wrapper functions where the wrapped function has just one possible success code is different:
* no output value -> return type is ```vk::Result```
* one output value -> return type is ```vk::ResultValue<T>```, as described above.

Note: With exceptions disabled, it is the user's responsibility to check for errors!    

# Usage
To start with the C++ version of the Vulkan API download header from GIT, put it in a vulkan subdirectory and add
```#include <vulkan/vulkan.hpp>``` to your source code.

To build the header for a given vk.xml specification continue with the following steps:

* Build VulkanHppGenerator
* Grab your favourite version vk.xml from Khronos
* Excute ```VulkanHppGenerator <vk.xml>``` to generate ```vulkan.hpp``` in the current working directory.

# Build instructions for VulkanHppGenerator

* Clone the repository: ```git clone https://github.com/KhronosGroup/vkcpp```
* Update submodules: ```git submodule update --init --recursive```
* Use CMake to generate a solution or makefile for your favourite build environment
* Launch the build

# Samples
Brad Davis started to port Sascha Willems Samples to vulkan.hpp. You can find his work in his [repository](https://github.com/jherico/Vulkan).

# Providing Pull Requests

NVIDIA is happy to review and consider pull requests for merging into the main tree of vulkan.hpp for bug fixes and features. Before providing a pull request to NVIDIA, please note the following:

* A pull request provided to this repo by a developer constitutes permission from the developer for NVIDIA to merge the provided
  changes or any NVIDIA modified version of these changes to the repo. NVIDIA may remove or change the code at any time and in any
  way deemed appropriate. Due to the required paperwork please refrain from providing pull requests for simple changes and file an issue
  describing a bug or the desired change instead.
* Not all pull requests can be or will be accepted. NVIDIA will close pull requests that it does not intend to merge.
* The modified files and any new files must include the unmodified NVIDIA copyright header seen at the top of all shipping files.
