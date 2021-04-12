
# Vulkan-Hpp: C++ Bindings for Vulkan

The goal of the Vulkan-Hpp is to provide header only C++ bindings for the Vulkan C API to improve the developers Vulkan experience without introducing CPU runtime cost. It adds features like type safety for enums and bitfields, STL container support, exceptions and simple enumerations.

| Platform | Build Status |
|:--------:|:------------:|
| Linux    | [![Build Status](https://travis-ci.org/KhronosGroup/Vulkan-Hpp.svg?branch=master)](https://travis-ci.org/KhronosGroup/Vulkan-Hpp) |

## Getting Started

Vulkan-Hpp is part of the LunarG Vulkan SDK since version 1.0.24. Just `#include <vulkan/vulkan.hpp>` and you're ready to use the C++ bindings. If you're using a Vulkan version not yet supported by the Vulkan SDK you can find the latest version of the header [here](https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/vulkan/vulkan.hpp).

### Minimum Requirements

Vulkan-Hpp requires a C++11 capable compiler to compile. The following compilers are known to work:
* Visual Studio >=2015
* GCC >= 4.8.2 (earlier version might work, but are untested)
* Clang >= 3.3

### Building Vulkan-Hpp, Samples, and Tests

To build the local samples and tests you'll have to clone this repository and run CMake to generate the required build files

0.) Ensure that you have CMake and git installed and accessible from a shell. Ensure that you have installed the Vulkan SDK. Optionally install clang-format >= 10.0 to get a nicely formatted Vulkan-Hpp header.
1.) Open a shell which provides git and clone the repository with:
```git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Hpp.git```
2.) Change the current directory to the newly created Vulkan-Hpp directory.
3.) Create a new folder named ```build``` and change the current directory to the newly created folder.
4.) Create a build environment with CMake
```cmake -D DSAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DSAMPLES_BUILD=ON -DTESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DTESTS_BUILD=ON -G "<generator>" ..```
For a full list of generators execute ```cmake -G```.
5.) Either open the generated project with an IDE, e.g. Visual Studio or launch the build process with ```cmake --build .```.

optional) To update the Vulkan-Hpp and its submodules execute ```git pull --recurse-submodules```.

### Optional Features

#### Formatting

If the program clang-format is found by CMake, the define CLANG_FORMAT_EXECUTABLE is set accordingly. In that case, the generated vulkan.hpp is formatted using the .clang-format file located in the root directory of this project. Otherwise it's formatted as hard-coded in the generator.

## Usage

### namespace vk

To avoid name collisions with the Vulkan C API the C++ bindings reside in the vk namespace. The following rules apply to the new naming

* All functions, enums, handles, and structs have the Vk prefix removed. In addition to this the first letter of functions is lower case.
  * `vkCreateInstance` can be accessed as `vk::createInstance`
  * `VkImageTiling` can be accessed as `vk::ImageTiling`
  * `VkImageCreateInfo` can be accessed as `vk::ImageCreateInfo`
* Enums are mapped to scoped enums to provide compile time type safety. The names have been changed to 'e' + CamelCase with the VK_ prefix and type infix removed. In case the enum type is an extension the extension suffix has been removed from the enum values.

In all other cases the extension suffix has not been removed.
  * `VK_IMAGETYPE_2D` is now `vk::ImageType::e2D`.
  * `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR` is now `vk::ColorSpaceKHR::eSrgbNonlinear`.
  *  `VK_STRUCTURE_TYPE_PRESENT_INFO_KHR` is now `vk::StructureType::ePresentInfoKHR`.
* Flag bits are handled like scoped enums with the addition that the `_BIT` suffix has also been removed.

In some cases it might be necessary to move Vulkan-Hpp to a custom namespace. This can be achieved by defining VULKAN_HPP_NAMESPACE before including Vulkan-Hpp.

### Handles

Vulkan-Hpp declares a class for all handles to ensure full type safety and to add support for member functions on handles. A member function has been added to a handle class for each function which accepts the corresponding handle as first parameter. Instead of `vkBindBufferMemory(device, ...)` one can write `device.bindBufferMemory(...)` or `vk::bindBufferMemory(device, ...)`.

### namespace vk::raii

There is an additional header named vulkan_raii.hpp generated. That header holds raii-compliant wrapper classes for the handle types. That is, for e.g. the handle type VkInstance, there's a raii-compliant wrapper vk::raii::Instance. Please have a look at the samples using those classes in the directory RAII_Samples.

### C/C++ Interop for Handles

On 64-bit platforms Vulkan-Hpp supports implicit conversions between C++ Vulkan handles and C Vulkan handles. On 32-bit platforms all non-dispatchable handles are defined as `uint64_t`, thus preventing type-conversion checks at compile time which would catch assignments between incompatible handle types.. Due to that Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default and it is recommended to use a `static_cast` for the conversion like this: `VkDevice = static_cast<VkDevice>(cppDevice)` to prevent converting some arbitrary int to a handle or vice versa by accident. If you're developing your code on a 64-bit platform, but want compile your code for a 32-bit platform without adding the explicit casts you can define `VULKAN_HPP_TYPESAFE_CONVERSION` to 1 in your build system or before including `vulkan.hpp`. On 64-bit platforms this define is set to 1 by default and can be set to 0 to disable implicit conversions.

### Flags

The scoped enum feature adds type safety to the flags, but also prevents using the flag bits as input for bitwise operations like & and |.

As solution Vulkan-Hpp provides a template class `vk::Flags` which brings the standard operations like `&=`, `|=`, `&` and `|` to our scoped enums. Except for the initialization with 0 this class behaves exactly like a normal bitmask with the improvement that it is impossible to set bits not specified by the corresponding enum by accident. Here are a few examples for the bitmask handling:

```c++
vk::ImageUsageFlags iu1; // initialize a bitmask with no bit set
vk::ImageUsageFlags iu2 = {}; // initialize a bitmask with no bit set
vk::ImageUsageFlags iu3 = vk::ImageUsageFlagBits::eColorAttachment; // initialize with a single value
vk::ImageUsageFlags iu4 = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage; // or two bits to get a bitmask
PipelineShaderStageCreateInfo ci( {} /* pass a flag without any bits set */, ...);
```

### CreateInfo structs

When constructing a handle in Vulkan one usually has to create some `CreateInfo` struct which describes the new handle. This can result in quite lengthy code as can be seen in the following Vulkan C example:

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
* `sType` is incorrect.

Especially the first one is hard to detect.

Vulkan-Hpp provides constructors for all CreateInfo objects which accept one parameter for each member variable. This way the compiler throws a compiler error if a value has been forgotten. In addition to this `sType` is automatically filled with the correct value and `pNext` set to a `nullptr` by default. Here's how the same code looks with a constructor:

```c++
vk::ImageCreateInfo ci({}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                       { width, height, 1 },
                       1, 1, vk::SampleCountFlagBits::e1,
                       vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                       vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined);
vk::Image image = device.createImage(ci);
```

With constructors for CreateInfo structures one can also pass temporaries to Vulkan functions like this:

```c++
vk::Image image = device.createImage({{}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                                     { width, height, 1 },
                                     1, 1, vk::SampleCountFlagBits::e1,
                                     vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                                     vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined});
```

### Designated Initializers

Beginning with C++20, C++ supports designated initializers. As that feature requires to not have any user-declared or inherited constructors, you have to `#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS`, which removes all the structure constructors from vulkan.hpp. Instead you can then use aggregate initialization. The first few vk-lines in your source might then look like
```c++
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo{ .pApplicationName   = AppName,
                                     .applicationVersion = 1,
                                     .pEngineName        = EngineName,
                                     .engineVersion      = 1,
                                     .apiVersion         = VK_API_VERSION_1_1 };
        
// initialize the vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo{ .pApplicationInfo = & applicationInfo };
```
instead of
```c++
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
        
// initialize the vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );
```
Note, that the designator order needs to match the declaration order.
Note as well, that now you can explicitly set the sType member of vk-structures. This is neither neccessary (as they are correctly initialized by default) nor recommended.

### Passing Arrays to Functions using ArrayProxy

The Vulkan API has several places where which require (count,pointer) as two function arguments and C++ has a few containers which map perfectly to this pair. To simplify development the Vulkan-Hpp bindings have replaced those argument pairs with the `ArrayProxy` template class which accepts empty arrays and a single value as well as STL containers `std::initializer_list`, `std::array` and `std::vector` as argument for construction. This way a single generated Vulkan version can accept a variety of inputs without having the combinatoric explosion which would occur when creating a function for each container type.

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

### Passing Structs to Functions

Vulkan-Hpp generates references for pointers to structs. This conversion allows passing temporary structs to functions which can result in shorter code. In case the input is optional and thus accepting a null pointer the parameter type will be a `vk::Optional<T> const&` type. This type accepts either a reference to `T` or nullptr as input and thus allows optional temporary structs.

```c++
// C
VkImageSubresource subResource;
subResource.aspectMask = 0;
subResource.mipLevel = 0;
subResource.arrayLayer = 0;
VkSubresourceLayout layout;
vkGetImageSubresourceLayout(device, image, &subresource, &layout);

// C++
auto layout = device.getImageSubresourceLayout(image, { {} /* flags*/, 0 /* miplevel */, 0 /* arrayLayer */ });
```

### Structure Pointer Chains

Vulkan allows chaining of structures through the pNext pointer. Vulkan-Hpp has a variadic template class which allows constructing of such structure chains with minimal efforts. In addition to this it checks at compile time if the spec allows the construction of such a `pNext` chain.

```c++
// This will compile successfully.
vk::StructureChain<vk::MemoryAllocateInfo, vk::ImportMemoryFdInfoKHR> c;
vk::MemoryAllocateInfo &allocInfo = c.get<vk::MemoryAllocateInfo>();
vk::ImportMemoryFdInfoKHR &fdInfo = c.get<vk::ImportMemoryFdInfoKHR>();

// This will fail compilation since it's not valid according to the spec.
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedRequirementsKHR> c;
vk::MemoryAllocateInfo &allocInfo = c.get<vk::MemoryAllocateInfo>();
vk::ImportMemoryFdInfoKHR &fdInfo = c.get<vk::ImportMemoryFdInfoKHR>();
```

Vulkan-Hpp provides a constructor for these chains similar to the CreateInfo objects which accepts a list of all structures part of the chain. The `pNext` field is automatically set to the correct value:

```c++
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedAllocateInfo> c = {
  vk::MemoryAllocateInfo(size, type),
  vk::MemoryDedicatedAllocateInfo(image)
};
```

If one of the structures of a StructureChain is to be removed, maybe due to some optional settings, you can use the function ```vk::StructureChain::unlink<ClassType>()```. It modifies the StructureChain such that the specified structure isn't part of the pNext-chain any more. Note, that the actual memory layout of the StructureChain is not modified by that function.
In case that very same structure has to be re-added to the StructureChain again, use ```vk::StructureChain::relink<ClassType>()```.

Sometimes the user has to pass a preallocated structure chain to query information. For those cases there are two corresponding getter functions. One with a variadic template generating a structure chain of at least two elements to construct the return value:

```c++
// Query vk::MemoryRequirements2HR and vk::MemoryDedicatedRequirementsKHR when calling Device::getBufferMemoryRequirements2KHR:
auto result = device.getBufferMemoryRequirements2KHR<vk::MemoryRequirements2KHR, vk::MemoryDedicatedRequirementsKHR>({});
vk::MemoryRequirements2KHR &memReqs = result.get<vk::MemoryRequirements2KHR>();
vk::MemoryDedicatedRequirementsKHR &dedMemReqs = result.get<vk::MemoryDedicatedRequirementsKHR>();
```

To get just the base structure, without chaining, the other getter function provided does not need a template argument for the structure to get:

```
// Query just vk::MemoryRequirements2KHR
vk::MemoryRequirements2KHR memoryRequirements = device.getBufferMemoryRequirements2KHR({});
```

### Return values, Error Codes & Exceptions

By default Vulkan-Hpp has exceptions enabled. This means that Vulkan-Hpp checks the return code of each function call which returns a Vk::Result. If Vk::Result is a failure a std::runtime_error will be thrown. Since there is no need to return the error code anymore the C++ bindings can now return the actual desired return value, i.e. a vulkan handle. In those cases ResultValue <SomeType>::type is defined as the returned type.

To create a device you can now just write:

```C++
vk::Device device = physicalDevice.createDevice(createInfo);
```

Some functions allow more than just `vk::Result::eSuccess` to be considered as a success code. For those functions, we always return a `ResultValue<SomeType>`. An example is `acquireNextImage2KHR`, that can be used like this:

```C++
vk::ResultValue<uint32_t> result = device->acquireNextImage2KHR(acquireNextImageInfo);
switch (result.result)
{
	case vk::Result::eSuccess:
		currentBuffer = result.value;
		break;
	case vk::Result::eTimeout:
	case vk::Result::eNotReady:
	case vk::Result::eSuboptimalKHR:
		// do something meaningfull
		break;
	default:
		// should not happen, as other return codes are considered to be an error and throw an exception
		break;
}
```

As time passes, some vulkan functions might change, such that they start to support more result codes than `vk::Result::eSuccess` as a success code. That logical change would not be visible in the C-API, but in the C++-API, as such a function would now return a `vk::ResultValue<SomeType>` instead of just `SomeType`. In such (rare) cases, you would have to adjust your cpp-sources to reflect that API change.

If exception handling is disabled by defining `VULKAN_HPP_NO_EXCEPTIONS` the type of `ResultValue<SomeType>::type` is a struct holding a `vk::Result` and a `SomeType`. This struct supports unpacking the return values by using `std::tie`.

In case you don’t want to use the `vk::ArrayProxy` and return value transformation you can still call the plain C-style function. Below are three examples showing the 3 ways to use the API:

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
   shader1 = device.createShaderModule({...});
   shader2 = device.createShaderModule({...});
 } catch(std::exception const &e) {
   // handle error and free resources
 }
```

Keep in mind that Vulkan-Hpp does not support RAII style handles and that you have to cleanup your resources in the error handler!

### C++17: [[nodiscard]]

With C++17 and above, some functions are attributed with [[nodiscard]], resulting in a warning if you don't use the return value in any way. You can switch those warnings off by defining VULKAN_HPP_NO_NODISCARD_WARNINGS.

### Enumerations

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

```c++
std::vector<LayerProperties> properties = physicalDevice.enumerateDeviceLayerProperties();
```

### UniqueHandle for automatic resource management

Vulkan-Hpp provides a `vk::UniqueHandle<Type, Deleter>` interface. For each Vulkan handle type `vk::Type` there is a unique handle `vk::UniqueType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::UniqueBuffer ` is the unique handle for `vk::Buffer`.

For each function which constructs a Vulkan handle of type `vk::Type` Vulkan-Hpp provides a second version which returns a `vk::UniqueType`. E.g. for `vk::Device::createBuffer` there is `vk::Device::createBufferUnique` and for `vk::allocateCommandBuffers` there is `vk::allocateCommandBuffersUnique`.

Note that using `vk::UniqueHandle` comes at a cost since most deleters have to store the `vk::AllocationCallbacks` and parent handle used for construction because they are required for automatic destruction.

### Custom allocators

Sometimes it is required to use `std::vector` with custom allocators. Vulkan-Hpp supports vectors with custom allocators as input for `vk::ArrayProxy` and for functions which do return a vector. For the latter case, add your favorite custom allocator as template argument to the function call like this:

```c++
std::vector<LayerProperties, MyCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties<MyCustomAllocator>();
```

You can as well use a stateful custom allocator by providing it as an argument to those functions. Unfortunately, to make the compilers happy, you also need to explicitly set the Dispatch argument. To get the default there, a simple ´´´{}´´´ would suffice:

```c++
MyStatefulCustomAllocator allocator;
std::vector<LayerProperties, MyStatefulCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties( allocator, {} );
```


### Custom assertions

All over vulkan.hpp, there are a couple of calls to an assert function. By defining `VULKAN_HPP_ASSERT`, you can specifiy your own custom assert function to be called instead.

By default, `VULKAN_HPP_ASSERT_ON_RESULT` will be used for checking results when `VULKAN_HPP_NO_EXCEPTIONS` is defined. If you want to handle errors by yourself, you can disable/customize it just like `VULKAN_HPP_ASSERT`.

### Extensions / Per Device function pointers

The Vulkan loader exposes only the Vulkan core functions and a limited number of extensions. To use Vulkan-Hpp with extensions it's required to have either a library which provides stubs to all used Vulkan functions or to tell Vulkan-Hpp to dispatch those functions pointers. Vulkan-Hpp provides a per-function dispatch mechanism by accepting a dispatch class as last parameter in each function call. The dispatch class must provide a callable type for each used Vulkan function. Vulkan-Hpp provides one implementation, ```DispatchLoaderDynamic```, which fetches all function pointers known to the library.

```c++
// Providing a function pointer resolving vkGetInstanceProcAddr, just the few functions not depending an an instance or a device are fetched
vk::DispatchLoaderDynamic dld( getInstanceProcAddr );

// Providing an already created VkInstance and a function pointer resolving vkGetInstanceProcAddr, all functions are fetched
vk::DispatchLoaderDynamic dldi( instance, getInstanceProcAddr );

// Providing also an already created VkDevice and optionally a function pointer resolving vkGetDeviceProcAddr, all functions are fetched as well, but now device-specific functions are fetched via vkDeviceGetProcAddr.
vk::DispatchLoaderDynamic dldid( instance, getInstanceProcAddr, device );

// Pass dispatch class to function call as last parameter
device.getQueue(graphics_queue_family_index, 0, &graphics_queue, dldid);
```

To use the ```DispatchLoaderDynamic``` as the default dispatcher (means: you don't need to explicitly add it to every function call),  you need to  ```#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1```, and have the macro ```VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE``` excactly once in your source code to provide storage for that default dispatcher. Then you can use it by the macro ```VULKAN_HPP_DEFAULT_DISPATCHER```, as is shown in the code snippets below.
To ease creating such a ```DispatchLoaderDynamic```, there is a little helper class ```DynamicLoader```.
Creating a full featured ```DispatchLoaderDynamic``` is a two- to three-step process:
1. initialize it with a function pointer of type PFN_vkGetInstanceProcAddr, to get the instance independent function pointers:
```c++
    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
```
2. initialize it with a vk::Instance to get all the other function pointers:
```c++
    vk::Instance instance = vk::createInstance({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
```
3. optionally initialize it with a vk::Device to get device-specific function pointers
```c++
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    assert(!physicalDevices.empty());
    vk::Device device = physicalDevices[0].createDevice({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
```
After the second step above, the dispatcher is fully functional. Adding the third step can potentially result in more efficient code.

In some cases the storage for the DispatchLoaderDynamic should be embedded in a DLL. For those cases you need to define ```VULKAN_HPP_STORAGE_SHARED``` to tell Vulkan-Hpp that the storage resides in a DLL. When compiling the DLL with the storage it is also required to define ```VULKAN_HPP_STORAGE_SHARED_EXPORT``` to export the required symbols.

For all functions, that VULKAN_HPP_DEFAULT_DISPATCHER is the default for the last argument to that function. In case you want to explicitly provide the dispatcher for each and every function call (when you have multiple dispatchers for different devices, for example) and you want to make sure, that you don't accidentally miss any function call, you can define VULKAN_HPP_NO_DEFAULT_DISPATCHER before you include vulkan.hpp to remove that default argument.

### Type traits

vulkan.hpp provides a couple of type traits, easing template programming:
- `template <typename EnumType, EnumType value> struct CppType`
	Maps `IndexType` values (`IndexType::eUint16`, `IndexType::eUint32`, ...) to the corresponding type (`uint16_t`, `uint32_t`, ...) by the member type `Type`;
	Maps `ObjectType` values (`ObjectType::eInstance`, `ObjectType::eDevice`, ...) to the corresponding type (`vk::Instance`, `vk::Device`, ...) by the member type `Type`;
	Maps `DebugReportObjectType` values (`DebugReportObjectTypeEXT::eInstance`, `DebugReportObjectTypeEXT::eDevice`, ...) to the corresponding type (`vk::Instance`, `vk::Device`, ...) by the member type `Type`;
- `template <typename T> struct IndexTypeValue`
	Maps scalar types (`uint16_t`, `uint32_t`, ...) to the corresponding `IndexType` value (`IndexType::eUint16`, `IndexType::eUint3`2, ...).
- `template <typename T> struct isVulkanHandleType`
	Maps a type to `true` if and only if it's a handle class (`vk::Instance`, `vk::Device`, ...) by the static member `value`.
- `HandleClass::CType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding C-type (`VkInstance`, `VkDevice`, ...) by the member type `CType`.
- `HandleClass::objectType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding `ObjectType` value (`ObjectType::eInstance`, `ObjectType::eDevice`, ...) by the static member `objectType`.
- `HandleClass::debugReportObjectType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding `DebugReportObjectTypeEXT` value (`DebugReportObjectTypeEXT::eInstance`, `DebugReportObjectTypeEXT::eDevice`, ...) by the static member `debugReportObjectType`.

### Samples and Tests

When you configure your project using CMake, you can enable SAMPLES_BUILD to add some sample projects to your solution. Most of them are ports from the LunarG samples, but there are some more, like CreateDebugUtilsMessenger, InstanceVersion, PhysicalDeviceDisplayProperties, PhysicalDeviceExtensions, PhysicalDeviceFeatures, PhysicalDeviceGroups, PhysicalDeviceMemoryProperties, PhysicalDeviceProperties, PhysicalDeviceQueueFamilyProperties, and RayTracing. All those samples should just compile and run.
When you configure your project using CMake, you can enable TESTS_BUILD to add some test projects to your solution. Those tests are just compilation tests and are not required to run.

## Configuration Options

There are a couple of defines you can use to control the feature set and behaviour of vulkan.hpp:

#### VULKAN_HPP_ASSERT

At various places in vulkan.hpp an assertion statement is used. By default, the standard assert funtions from ```<cassert>``` is called. By defining ```VULKAN_HPP_ASSERT``` before including vulkan.hpp, you can change that to any function with the very same interface.

#### VULKAN_HPP_ASSERT_ON_RESULT

If there are no exceptions enabled (see ```VULKAN_HPP_NO_EXCEPTIONS```), an assertion statement checks for a valid success code returned from every vulkan call. By default, this is the very same assert function as defined by ```VULKAN_HPP_ASSERT```, but by defining ```VULKAN_HPP_ASSERT_ON_RESULT``` you can replace just those assertions with your own function, using the very same interface.

#### VULKAN_HPP_DEFAULT_DISPATCHER

Every vk-function gets a Dispatcher as its very last argument, which defaults to ```VULKAN_HPP_DEFAULT_DISPATCHER```. If ```VULKAN_HPP_DISPATCH_LOADER_DYNAMIC``` is defined to be 1, it is ```defaultDispatchLoaderDynamic```. This in turn is the dispatcher instance, which is defined by ```VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE```, which has to be used exactly once in your sources. If, on the other hand, ```VULKAN_HPP_DISPATCH_LOADER_DYNAMIC``` is defined to something different from 1, ```VULKAN_HPP_DEFAULT_DISPATCHER``` is set to be ```DispatchLoaderStatic()```.
You can use your own default dispatcher by setting ```VULKAN_HPP_DEFAULT_DISPATCHER``` to an object that provides the same API. If you explicitly set ```VULKAN_HPP_DEFAULT_DISPATCHER```, you need to set ```VULKAN_HPP_DEFAULT_DISPATCHER_TYPE``` accordingly as well.

#### VULKAN_HPP_DEFAULT_DISPATCHER_TYPE

This names the default dispatcher type, as specified by ```VULKAN_HPP_DEFAULT_DISPATCHER```. Per default, it is DispatchLoaderDynamic or DispatchLoaderStatic, depending on ```VULKAN_HPP_DISPATCH_LOADER_DYNAMIC``` being 1 or not 1, respectively. If you explicitly set ```VULKAN_HPP_DEFAULT_DISPATCHER```, you need to set ```VULKAN_HPP_DEFAULT_DISPATCHER_TYPE``` accordingly as well.

#### VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

If you have not defined your own ```VULKAN_HPP_DEFAULT_DISPATCHER```, and have ```VULKAN_HPP_DISPATCH_LOADER_DYNAMIC``` defined to be 1 (the default), you need to have the macro ```VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE``` exactly once in any of your source files to provide storage for that default dispatcher. ```VULKAN_HPP_STORAGE_API``` then controls the import/export status of that default dispatcher.

#### VULKAN_HPP_DISABLE_ENHANCED_MODE

When this is defined before including vulkan.hpp, you essentially disable all enhanced functionality. All you then get is improved compile time error detection, via scoped enums, usage of the helper class ```vk::Flags``` for bitmasks, wrapper structs for all vulkan structs providing default initialization, and the helper class ```vk::StructureChain``` for compile-time construction of structure chains.

#### VULKAN_HPP_DISPATCH_LOADER_DYNAMIC

This either selects the dynamic (when it's 1) or the static (when it's not 1) DispatchLoader as the default one, as long as it's not explicitly specified by ```VULKAN_HPP_DEFAULT_DISPATCHER```. By default, this is defined to be 1 if ```VK_NO_PROTOTYPES``` is defined, otherwise 0.

#### VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL

By default, a little helper class ```DynamicLoader``` is used to dynamically load the vulkan library. If you set it to something different than 1 before including vulkan.hpp, this helper is not available, and you need to explicitly provide your own loader type for the function ```DispatchLoaderDynamic::init()```.

#### VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC

By default, the member ```m_mask``` of the ```Flags``` template class is private. This is to prevent accidentally setting a ```Flags``` with some inappropriate value. But it also prevents using a ```Flags```, or a structure holding a ```Flags```, to be used as a non-type template parameter. If you really need that functionality, and accept the reduced security, you can use this define to change the access specifier for m_mask from private to public, which allows using a ```Flags``` as a non-type template parameter.

#### VULKAN_HPP_INLINE

This is set to be the compiler-dependent attribute used to mark functions as inline. If your compiler happens to need some different attribute, you can set this define accordingly before including vulkan.hpp.

#### VULKAN_HPP_NAMESPACE

By default, the namespace used with vulkan.hpp is ```vk```. By defining ```VULKAN_HPP_NAMESPACE``` before including vulkan.hpp, you can adjust this.

#### VULKAN_HPP_NO_EXCEPTIONS

When a vulkan function returns an error code that is not specified to be a success code, an exception is thrown unless ```VULKAN_HPP_NO_EXCEPTIONS``` is defined before including vulkan.hpp.

#### VULKAN_HPP_NO_NODISCARD_WARNINGS

With C++17, all ```vk```-functions returning something are declared with the attribute ```[[nodiscard]]```. This can be removed by defining ```VULKAN_HPP_NO_NODISCARD_WARNINGS``` before including vulkan.hpp.

#### VULKAN_HPP_NO_SMART_HANDLE

By defining ```VULKAN_HPP_NO_SMART_HANDLE``` before including vulkan.hpp, the helper class ```UniqueHandle``` and all the unique handle types are not available.

### VULKAN_HPP_NO_SPACESHIP_OPERATOR

With C++20, the so-called spaceship-operator ```<=>``` is introduced. If that operator is supported, all the structs and classes in vulkan.hpp use the default implementation of it. As currently some implementations of this operator are very slow, and others seem to be incomplete, by defining ```VULKAN_HPP_NO_SPACESHIP_OPERATOR``` before including vulkan.hpp you can remove that operator from those structs and classes.

#### VULKAN_HPP_STORAGE_API

With this define you can specify whether the ```DispatchLoaderDynamic``` is imported or exported (see ```VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE```). If ```VULKAN_HPP_STORAGE_API``` is not defined externally, and ```VULKAN_HPP_STORAGE_SHARED``` is defined, depending on the ```VULKAN_HPP_STORAGE_SHARED_EXPORT``` being defined, ```VULKAN_HPP_STORAGE_API``` is either set to ```__declspec( dllexport )``` (for MSVC) / ```__attribute__( ( visibility( "default" ) ) )``` (for gcc or clang) or ```__declspec( dllimport )``` (for MSVC), respectively. For other compilers, you might specify the corresponding storage by defining ```VULKAN_HPP_STORAGE_API``` on your own.


#### VULKAN_HPP_TYPESAFE_CONVERSION

32-bit vulkan is not typesafe for handles, so we don't allow copy constructors on this platform by default. To enable this feature on 32-bit platforms define ```VULKAN_HPP_TYPESAFE_CONVERSION```.

## Deprecated elements

There are a couple of elements in vulkan.hpp that are marked as deprecated (with C++14 and above):

The implicit cast operators on ```vk::ResultValue``` are potentially wrong under certain circumstances. You should access the value explicitly as the member of the ```vk::ResultValue``` instead
	
The type traits ```cpp_type<ObjectType::eObjectTypeID>``` are replaced by the more general type traits ```CppType<Type, Type::eTypeID>```.

Some functions (listed below) provide an interface that does not fit to the general approach in vulkan.hpp, where values you get from a function are supposed to be returned. Use the corresponding functions with the same name, that actually return those values, instead.
The affected functions are
```
	Device::getAccelerationStructureHandleNV
	Device::getCalibratedTimestampsEXT
	Device::getQueryPoolResults
	Device::getRayTracingCaptureReplayShaderGroupHandlesKHR
	Device::getRayTracingShaderGroupHandlesKHR
	Device::getRayTracingShaderGroupHandlesNV
	Device::writeAccelerationStructuresPropertiesKHR
	PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR
```
All those elements will be removed around November 2021.

## See Also

Feel free to submit a PR to add to this list.

- [Examples](https://github.com/jherico/vulkan) A port of Sascha Willems [examples](https://github.com/SaschaWillems/Vulkan) to Vulkan-Hpp 
- [Vookoo](https://github.com/andy-thomason/Vookoo/) Stateful helper classes for Vulkan-Hpp, [Introduction Article](https://accu.org/journals/overload/25/139/overload139.pdf#page=14).

## License

Copyright 2015-2020 The Khronos Group Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
