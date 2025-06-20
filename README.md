
# Vulkan-Hpp: C++ Bindings for Vulkan

The goal of the Vulkan-Hpp is to provide header only C++ bindings for the Vulkan C API to improve the developers Vulkan experience without introducing CPU runtime cost. It adds features like type safety for enums and bitfields, STL container support, exceptions and simple enumerations.

| Platform | Build Status |
|:--------:|:------------:|
| Windows  | [![CI Windows](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-windows.yml) |
| Linux    | [![CI Ubuntu 22](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-ubuntu-22.yml/badge.svg)](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-ubuntu-22.yml) <br> [![CI Ubuntu 24](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-ubuntu-24.yml/badge.svg)](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-ubuntu-24.yml) |
| MacOS    | [![CI MacOS](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-macos.yml/badge.svg)](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/ci-macos.yml) |

# Table of Contents
- [Getting Started](#getting_started)
	- [Minimum Requirements](#minimum_requirements)
	- [Building Vulkan-Hpp, Samples, and Tests](#building)
	- [Installing vulkan-hpp using vcpkg](#installing)
	- [Optional Features](#optional_features)
		- [Formatting](#formatting)
		- [Custom views of Vulkan-Hpp objects in Visual Studio](#custom_views)
- [Usage](#usage)
	- [namespace vk](#namespace_vk)
	- [Handles](#handles)
	- [namespace vk::raii](#namespace_vk_raii)
	- [C/C++ Interop for Handles](#c_cpp_interop)
	- [Flags](#flags)
	- [CreateInfo structs](#createinfo_structs)
	- [Designated Initializers](#designated_initializers)
	- [Passing Arrays to Functions using ArrayProxy](#passing_arrays)
	- [Passing Structs to Functions](#passing_structs)
	- [Structure Pointer Chains](#structure_chains)
	- [Return values, Error Codes & Exceptions](#return_values)
	- [C++17: [[nodiscard]]](#nodiscard)
	- [Enumerations](#enumerations)
	- [UniqueHandle for automatic resource management](#unique_handle)
	- [SharedHandle](#shared_handle)
	- [RAII-classes](#raii_classes)
	- [Custom allocators](#custom_allocators)
	- [Custom assertions](#custom_assertions)
	- [ Extensions / Per Device function pointers](#extensions)
	- [Type traits](#type_traits)
	- [vk::Format trait functions](#vk_format_traits)
	- [Hashing Vulkan types](#hashing)
	- [Extension Inspection](#extension_inspection)
	- [C++20 named module](#named_module)
		- [Overview](#named_module_overview)
		- [Compiler support](#compiler_support)
			- [Usage with CMake](#cmake_usage)
			- [Command-line usage](#command_line_usage)
	- [Samples and Tests](#samples_and_tests)
	- [Compile time issues](#compile_time_issues)
	- [Strict aliasing issue](#strict_aliasing_issue)
- [Configuration Options](#options)
	- [VULKAN_HPP_ASSERT](#assert)
	- [VULKAN_HPP_ASSERT_ON_RESULT](#assert_on_result)
	- [VULKAN_HPP_DEFAULT_DISPATCHER](#default_dispatcher)
	- [VULKAN_HPP_DEFAULT_DISPATCHER_TYPE](#default_dispatcher_type)
	- [VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE](#default_dispatch_loader_dynamic_storage)
	- [VULKAN_HPP_DISABLE_ENHANCED_MODE](#disable_enhanced_mode)
	- [VULKAN_HPP_DISPATCH_LOADER_DYNAMIC](#dispatch_loader_dynamic)
	- [VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL](#enable_dynamic_loader_tool)
	- [VULKAN_HPP_EXPECTED](#expected)
	- [VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC](#flags_mask_type_as_public)
	- [VULKAN_HPP_HANDLES_MOVE_EXCHANGE](#handles_move_exchange)
	- [VULKAN_HPP_HASH_COMBINE](#hash_combine)
	- [VULKAN_HPP_INLINE](#inline)
	- [VULKAN_HPP_NAMESPACE](#namespace)
	- [VULKAN_HPP_NO_CONSTRUCTORS](#no_constructors)
	- [VULKAN_HPP_NO_EXCEPTIONS](#no_exceptions)
	- [VULKAN_HPP_NO_NODISCARD_WARNINGS](#no_discard_warnings)
	- [VULKAN_HPP_NO_SETTERS](#no_setters)
	- [VULKAN_HPP_NO_SMART_HANDLE](#no_smart_handle)
	- [VULKAN_HPP_NO_SPACESHIP_OPERATOR](#no_spaceship_operator)
	- [VULKAN_HPP_NO_TO_STRING](#no_to_string)
	- [VULKAN_HPP_NO_WIN32_PROTOTYPES](#no_win32_prototypes)
	- [VULKAN_HPP_RAII_NO_EXCEPTIONS](#raii_no_exceptions)
	- [VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST](#smart_handle_implicit_cast)
	- [VULKAN_HPP_STORAGE_API](#storage_api)
	- [VULKAN_HPP_TYPESAFE_CONVERSION](#typesafe_conversion)
	- [VULKAN_HPP_UNEXPECTED](#unexpected)
	- [VULKAN_HPP_USE_REFLECT](#use_reflect)
- [See Also](#see_also)
- [License](#license)

## Getting Started <a id='getting_started'>

Vulkan-Hpp is part of the [LunarG Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) since version 1.0.24. Just `#include <vulkan/vulkan.hpp>` and you're ready to use the C++ bindings. If you're using a Vulkan version not yet supported by the Vulkan SDK, you can find the latest version of the headers [here](vulkan).

### Minimum Requirements <a id='minimum_requirements'>

Vulkan-Hpp requires a C++11 capable compiler to compile. The following compilers are known to work:
* Visual Studio >= 2015
* GCC >= 4.8.2 (earlier version might work, but are untested)
* Clang >= 3.3

### Building Vulkan-Hpp, Samples, and Tests <a id="building">

To build the local samples and tests you'll have to clone this repository and run CMake to generate the required build files

0. Install dependencies.
   * Ensure that you have CMake and git installed and accessible from a shell.
   * Ensure that you have installed the Vulkan SDK.
   * Optionally install clang-format >= 11.0 to get a nicely formatted Vulkan-Hpp header.
1. Open a shell which provides git and clone the repository with:

    `git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Hpp.git`
2. Change the current directory to the newly created Vulkan-Hpp directory.
3. Create a build environment with CMake:

    `cmake -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build`

    You might need to specify a generator via `-G`, for a full list of generators execute `cmake -G`.
    *  To rebuild `vulkan.hpp` from the `vk.xml` XML registry file, add the
       `-DVULKAN_HPP_RUN_GENERATOR=ON` option to the CMake command line.
4. Either open the generated project with an IDE, e.g. Visual Studio or launch the build process with `cmake --build build --parallel`.

Optional: To update the Vulkan-Hpp and its submodules execute `git pull --recurse-submodules`.

### Installing vulkan-hpp using vcpkg <a id='installing'>

You can download and install vulkan-hpp using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install vulkan-headers
```

The vulkan-hpp port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.

### Optional Features <a id='optional_features'>

#### Formatting <a id='formatting'>

If the program clang-format is found by CMake, the define `CLANG_FORMAT_EXECUTABLE` is set accordingly. In that case, the generated `vulkan.hpp` is formatted using the `.clang-format` file located in the root directory of this project. Otherwise, it's formatted as hard-coded in the generator.

#### Custom views of Vulkan-Hpp objects in Visual Studio <a id='custom_views'>

The file `VulkanHpp.natvis` provides a custom view on `vk::Flags` for Visual Studio. If you add this file to the user-specific natvis directory of your Visual Studio installation (%USERPROFILE%\Documents\Visual Studio 2022\Visualizers), you get `vk::Flags` nicely formatted in your debugger with all your Visual Studio projects.

## Usage <a id='usage'>

### namespace vk <a id='namespace_vk'>

To avoid name collisions with the Vulkan C API, the C++ bindings reside in the `vk` namespace. The following rules apply to the new naming:

* All functions, enums, handles, and structs have the `Vk` prefix removed. In addition to this the first letter of functions is lower case.
  * `vkCreateInstance` can be accessed as `vk::createInstance`.
  * `VkImageTiling` can be accessed as `vk::ImageTiling`.
  * `VkImageCreateInfo` can be accessed as `vk::ImageCreateInfo`.
* Enums are mapped to scoped enums to provide compile time type safety. The names have been changed to 'e' + CamelCase with the `VK_` prefix and type infix removed. If the enum type is an extension, the extension suffix has been removed from the enum values.

In all other cases the extension suffix has not been removed.
  * `VK_IMAGETYPE_2D` is now `vk::ImageType::e2D`.
  * `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR` is now `vk::ColorSpaceKHR::eSrgbNonlinear`.
  * `VK_STRUCTURE_TYPE_PRESENT_INFO_KHR` is now `vk::StructureType::ePresentInfoKHR`.
* Flag bits are handled like scoped enums with the addition that the `_BIT` suffix has also been removed.

In some cases it might be necessary to move Vulkan-Hpp to a custom namespace. This can be achieved by defining `VULKAN_HPP_NAMESPACE` before including Vulkan-Hpp.

### Handles <a id='handles'>

Vulkan-Hpp declares a class for all handles to ensure full type safety and to add support for member functions on handles. A member function has been added to a handle class for each function which accepts the corresponding handle as first parameter. Instead of `vkBindBufferMemory(device, ...)` one can write `device.bindBufferMemory(...)` or `vk::bindBufferMemory(device, ...)`.

### namespace vk::raii <a id='namespace_vk_raii'>

There is an additional header named [`vulkan_raii.hpp`](vulkan/vulkan_raii.hpp) generated. That header holds raii-compliant wrapper classes for the handle types. That is, for e.g. the handle type `VkInstance`, there's a raii-compliant wrapper `vk::raii::Instance`. Please have a look at the samples using those classes in the directory [RAII_Samples](RAII_Samples).

### C/C++ Interop for Handles <a id='c_cpp_interop'>

On 64-bit platforms Vulkan-Hpp supports implicit conversions between C++ Vulkan handles and C Vulkan handles. On 32-bit platforms all non-dispatchable handles are defined as `uint64_t`, thus preventing type-conversion checks at compile time which would catch assignments between incompatible handle types. Due to that Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default and it is recommended to use a `static_cast` for the conversion like this: `VkImage = static_cast<VkImage>(cppImage)` to prevent converting some arbitrary int to a handle or vice versa by accident. If you're developing your code on a 64-bit platform, but want to compile your code for a 32-bit platform without adding the explicit casts, you can define `VULKAN_HPP_TYPESAFE_CONVERSION` to `1` in your build system or before including `vulkan.hpp`. On 64-bit platforms this define is set to `1` by default and can be set to `0` to disable implicit conversions.

### Flags <a id='flags'>

The scoped enum feature adds type safety to the flags, but also prevents using the flag bits as input for bitwise operations such as `&` and `|`.

As solution Vulkan-Hpp provides a class template `vk::Flags` which brings the standard operations like `&=`, `|=`, `&`, and `|` to our scoped enums. Except for the initialization with `0` this class behaves exactly like a normal bitmask with the improvement that it is impossible to set bits not specified by the corresponding enum by accident. Here are a few examples for the bitmask handling:

```c++
vk::ImageUsageFlags iu1; // initialize a bitmask with no bit set
vk::ImageUsageFlags iu2 = {}; // initialize a bitmask with no bit set
vk::ImageUsageFlags iu3 = vk::ImageUsageFlagBits::eColorAttachment; // initialize with a single value
vk::ImageUsageFlags iu4 = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage; // or two bits to get a bitmask
PipelineShaderStageCreateInfo ci({} /* pass a flag without any bits set */, ...);
```

### CreateInfo structs <a id='createinfo_structs'>

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
vkCreateImage(device, &ci, allocator, &image);
```

There are two typical issues Vulkan developers encounter when filling out a `CreateInfo` struct field by field:
* One or more fields are left uninitialized.
* `sType` is incorrect.

Especially the first one is hard to detect.

Vulkan-Hpp provides constructors for all `CreateInfo` objects which accept one parameter for each member variable. This way the compiler throws a compiler error if a value has been forgotten. In addition to this `sType` is automatically filled with the correct value and `pNext` set to a `nullptr` by default. Here's how the same code looks with a constructor:

```c++
vk::ImageCreateInfo ci({}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                       { width, height, 1 },
                       1, 1, vk::SampleCountFlagBits::e1,
                       vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                       vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined);
vk::Image image = device.createImage(ci);
```

With constructors for `CreateInfo` structures, one can also pass temporaries to Vulkan functions like this:

```c++
vk::Image image = device.createImage({{}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                                     { width, height, 1 },
                                     1, 1, vk::SampleCountFlagBits::e1,
                                     vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                                     vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined});
```

### Designated Initializers <a id='designated_initializers'>

Beginning with C++20, C++ supports designated initializers. As that feature requires to not have any user-declared or inherited constructors, you have to `#define VULKAN_HPP_NO_CONSTRUCTORS`, which removes all the structure and union constructors from `vulkan.hpp`. Instead you can then use aggregate initialization. The first few vk-lines in your source might then look like:

```c++
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo{ .pApplicationName   = AppName,
                                     .applicationVersion = 1,
                                     .pEngineName        = EngineName,
                                     .engineVersion      = 1,
                                     .apiVersion         = VK_API_VERSION_1_1 };

// initialize the vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo{ .pApplicationInfo = &applicationInfo };
```

instead of

```c++
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo(AppName, 1, EngineName, 1, VK_API_VERSION_1_1);

// initialize the vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);
```

Note, that the designator order needs to match the declaration order.
Note as well, that now you can explicitly set the `sType` member of vk-structures. This is neither neccessary (as they are correctly initialized by default) nor recommended.

### Passing Arrays to Functions using ArrayProxy <a id='passing_arrays'>

The Vulkan API has several places which require (count, pointer) as two function arguments and C++ has a few containers which map perfectly to this pair. To simplify development the Vulkan-Hpp bindings have replaced those argument pairs with the `vk::ArrayProxy` class template which accepts empty arrays and a single value as well as STL containers `std::initializer_list`, `std::array` and `std::vector` as argument for construction. This way a single generated Vulkan version can accept a variety of inputs without having the combinatoric explosion which would occur when creating a function for each container type.

Here are some code samples on how to use the `vk::ArrayProxy`:

```c++
vk::CommandBuffer c;

// pass an empty array
c.setScissor(0, nullptr);

// pass a single value. Value is passed as reference
vk::Rect2D scissorRect = { { 0, 0 }, { 640, 480 } };
c.setScissor(0, scissorRect);

// pass a temporary value.
c.setScissor(0, { { 0, 0 }, { 640, 480 } });

// pass a fixed size array
vk::Rect2D scissorRects[2] = { { { 0, 0 }, { 320, 240 } }, { { 320, 240 }, { 320, 240 } } };
c.setScissor(0, scissorRects);

// generate a std::initializer_list using two rectangles from the stack. This might generate a copy of the rectangles.
vk::Rect2D scissorRect1 = { { 0, 0 }, { 320, 240 } };
vk::Rect2D scissorRect2 = { { 320, 240 }, { 320, 240 } };
c.setScissor(0, { scissorRect, scissorRect2 });

// construct a std::initializer_list using two temporary rectangles.
c.setScissor(0, { { { 0, 0 }, { 320, 240 } },
                { { 320, 240 }, { 320, 240 } } });

// pass a std::array
std::array<vk::Rect2D, 2> arr{ scissorRect1, scissorRect2 };
c.setScissor(0, arr);

// pass a std::vector of dynamic size
std::vector<vk::Rect2D> vec;
vec.push_back(scissorRect1);
vec.push_back(scissorRect2);
c.setScissor(0, vec);
```

### Passing Structs to Functions <a id='passing_structs'>

Vulkan-Hpp generates references for pointers to structs. This conversion allows passing temporary structs to functions which can result in shorter code. In case the input is optional and thus accepting a null pointer, the parameter type will be `vk::Optional<T> const&`. This type accepts either a reference to `T` or `nullptr` as input and thus allows optional temporary structs.

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

### Structure Pointer Chains <a id='structure_chains'>

Vulkan allows chaining of structures through the `pNext` pointer. Vulkan-Hpp has a variadic class template which allows constructing of such structure chains with minimal efforts. In addition to this it checks at compile time if the spec allows the construction of such a `pNext` chain.

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

Vulkan-Hpp provides a constructor for these chains similar to the `CreateInfo` objects which accepts a list of all structures part of the chain. The `pNext` field is automatically set to the correct value:

```c++
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedAllocateInfo> c = {
  vk::MemoryAllocateInfo(size, type),
  vk::MemoryDedicatedAllocateInfo(image)
};
```

If one of the structures of a StructureChain is to be removed, maybe due to some optional settings, you can use the function `vk::StructureChain::unlink<ClassType>()`. It modifies the StructureChain such that the specified structure isn't part of the pNext-chain any more. Note, that the actual memory layout of the StructureChain is not modified by that function.
In case that very same structure has to be re-added to the StructureChain again, use `vk::StructureChain::relink<ClassType>()`.

Sometimes the user has to pass a preallocated structure chain to query information. For those cases there are two corresponding getter functions. One with a variadic template generating a structure chain of at least two elements to construct the return value:

```c++
// Query vk::MemoryRequirements2HR and vk::MemoryDedicatedRequirementsKHR when calling Device::getBufferMemoryRequirements2KHR:
auto result = device.getBufferMemoryRequirements2KHR<vk::MemoryRequirements2KHR, vk::MemoryDedicatedRequirementsKHR>({});
vk::MemoryRequirements2KHR &memReqs = result.get<vk::MemoryRequirements2KHR>();
vk::MemoryDedicatedRequirementsKHR &dedMemReqs = result.get<vk::MemoryDedicatedRequirementsKHR>();
```

To get just the base structure, without chaining, the other getter function provided does not need a template argument for the structure to get:

```c++
// Query just vk::MemoryRequirements2KHR
vk::MemoryRequirements2KHR memoryRequirements = device.getBufferMemoryRequirements2KHR({});
```

### Return values, Error Codes & Exceptions <a id='return_values'>

By default Vulkan-Hpp has exceptions enabled. This means that Vulkan-Hpp checks the return code of each function call which returns a `vk::Result`. If `vk::Result` is a failure a `std::runtime_error` will be thrown. Since there is no need to return the error code anymore the C++ bindings can now return the actual desired return value, i.e. a vulkan handle. In those cases `vk::ResultValue<SomeType>::type` is defined as the returned type.

To create a device you can now just write:

```c++
vk::Device device = physicalDevice.createDevice(createInfo);
```

Some functions allow more than just `vk::Result::eSuccess` to be considered as a success code. For those functions, we always return a `vk::ResultValue<SomeType>`. An example is `acquireNextImage2KHR`, that can be used like this:

```c++
vk::ResultValue<uint32_t> result = device->acquireNextImage2KHR(acquireNextImageInfo);
switch (result.result)
{
	case vk::Result::eSuccess:
		currentBuffer = result.value;
		break;
	case vk::Result::eTimeout:
	case vk::Result::eNotReady:
	case vk::Result::eSuboptimalKHR:
		// do something meaningful
		break;
	default:
		// should not happen, as other return codes are considered to be an error and throw an exception
		break;
}
```

As time passes, some vulkan functions might change, such that they start to support more result codes than `vk::Result::eSuccess` as a success code. That logical change would not be visible in the C API, but in the C++ API, as such a function would now return a `vk::ResultValue<SomeType>` instead of just `SomeType`. In such (rare) cases, you would have to adjust your cpp-sources to reflect that API change.

If exception handling is disabled by defining `VULKAN_HPP_NO_EXCEPTIONS` the type of `vk::ResultValue<SomeType>::type` is a struct holding a `vk::Result` and a `SomeType`. This struct supports unpacking the return values by using `std::tie`.

In case you don’t want to use the `vk::ArrayProxy` and return value transformation, you can still call the plain C-style function. Below are three examples showing the 3 ways to use the API:

The first snippet shows how to use the API without exceptions and the return value transformation:

```c++
// No exceptions, no return value transformation
vk::ShaderModuleCreateInfo createInfo(...);
vk::ShaderModule shader1;
vk::Result result = device.createShaderModule(&createInfo, allocator, &shader1);
if (result.result != vk::Result::eSuccess)
{
  handle error code;
  cleanup?
  return?
}

vk::ShaderModule shader2;
vk::Result result = device.createShaderModule(&createInfo, allocator, &shader2);
if (result != vk::Result::eSuccess)
{
  handle error code;
  cleanup?
  return?
}
```

The second snippet shows how to use the API using return value transformation, but without exceptions. It’s already a little bit shorter than the original code:

```c++
vk::ResultValue<ShaderModule> shaderResult1 = device.createShaderModule({...} /* createInfo temporary */);
if (shaderResult1.result != vk::Result::eSuccess)
{
  handle error code;
  cleanup?
  return?
}

// std::tie support.
vk::Result result;
vk::ShaderModule shaderModule2;
std::tie(result, shaderModule2) = device.createShaderModule({...} /* createInfo temporary */);
if (result != vk::Result::eSuccess)
{
  handle error code;
  cleanup?
  return?
}
```

A nicer way to unpack the result is using structured bindings in C++17. They will allow us to get the result with a single line of code:

```c++
auto [result, shaderModule2] = device.createShaderModule({...} /* createInfo temporary */);
```

Finally, the last code example is using exceptions and return value transformation. This is the default mode of the API.

```c++
vk::ShaderModule shader1;
vk::ShaderModule shader2;
try
{
  shader1 = device.createShaderModule({...});
  shader2 = device.createShaderModule({...});
}
catch(std::exception const &e)
{
  // handle error and free resources
}
```

> [!IMPORTANT]
> The vulkan handles in the `vk`-namespace do not support RAII, hence you need to cleanup your resources in the error handler! Instead, you could use the handle wrapper classes in the `vk::raii`-namespace.

### C++17: [[nodiscard]] <a id='nodiscard'>

With C++17 and above, some functions are attributed with `[[nodiscard]]`, resulting in a warning if you don't use the return value in any way. You can switch those warnings off by defining `VULKAN_HPP_NO_NODISCARD_WARNINGS`.

### Enumerations <a id='enumerations'>

For the return value transformation, there's one special class of return values which require special handling: Enumerations. For enumerations you usually have to write code like this:

```c++
std::vector<LayerProperties, Allocator> properties;
uint32_t propertyCount;
vk::Result result;
do
{
  // determine number of elements to query
  result = static_cast<vk::Result>(vk::enumerateDeviceLayerProperties(m_physicalDevice, &propertyCount, nullptr));
  if ((result == vk::Result::eSuccess) && propertyCount)
  {
    // allocate memory & query again
    properties.resize(propertyCount);
    result = static_cast<vk::Result>(vk::enumerateDeviceLayerProperties(m_physicalDevice, &propertyCount, reinterpret_cast
     <VkLayerProperties*>(properties.data())));
  }
} while (result == vk::Result::eIncomplete);
// it's possible that the count has changed, start again if properties was not big enough
properties.resize(propertyCount);
```

Since writing this loop over and over again is tedious and error prone the C++ binding takes care of the enumeration so that you can just write:

```c++
std::vector<LayerProperties> properties = physicalDevice.enumerateDeviceLayerProperties();
```

### UniqueHandle for automatic resource management <a id='unique_handle'>

Vulkan-Hpp provides a `vk::UniqueHandle<Type, Deleter>` interface. For each Vulkan handle type `vk::Type` there is a unique handle `vk::UniqueType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::UniqueBuffer ` is the unique handle for `vk::Buffer`.

For each function which constructs a Vulkan handle of type `vk::Type` Vulkan-Hpp provides a second version which returns a `vk::UniqueType`. E.g. for `vk::Device::createBuffer` there is `vk::Device::createBufferUnique` and for `vk::allocateCommandBuffers` there is `vk::allocateCommandBuffersUnique`.

Note that using `vk::UniqueHandle` comes at a cost since most deleters have to store the `vk::AllocationCallbacks` and parent handle used for construction because they are required for automatic destruction.

### SharedHandle <a id='shared_handle'>

Vulkan-Hpp provides a `vk::SharedHandle<Type>` interface. For each Vulkan handle type `vk::Type` there is a shared handle `vk::SharedType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::SharedBuffer` is the shared handle for `vk::Buffer`.

Unlike `vk::UniqueHandle`, `vk::SharedHandle` takes shared ownership of the resource as well as its parent. This means that the parent handle will not be destroyed until all child resources are deleted. This is useful for resources that are shared between multiple threads or objects.

This mechanism ensures correct destruction order even if the parent `vk::SharedHandle` is destroyed before its child handle. Otherwise, the handle behaves like `std::shared_ptr`. `vk::SharedInstance` or any of its child object should be last to delete (first created, first in class declaration).

There are no functions which return a `vk::SharedHandle` directly yet. Instead, you can construct a `vk::SharedHandle` from a `vk::Handle`:

```c++
vk::Buffer buffer = device.createBuffer(...);
vk::SharedBuffer sharedBuffer(buffer, device); // sharedBuffer now owns the buffer
```

There are several specializations of `vk::SharedHandle` for different handle types. For example, `vk::SharedImage` may take an additional argument to specify if the image is owned by swapchain:

```c++
vk::Image image = swapchain.getImages(...)[0]; // get the first image from the swapchain
vk::SharedImage sharedImage(image, device, SwapChainOwns::yes); // sharedImage now owns the image, but won't destroy it
```

There is also a specialization for `vk::SwapchainKHR` which takes an additional argument to specify a surface:

```c++
vk::SwapchainKHR swapchain = device.createSwapchainKHR(...);
vk::SharedSwapchainKHR sharedSwapchain(swapchain, device, surface); // sharedSwapchain now owns the swapchain and surface
```

You can create a `vk::SharedHandle` overload for your own handle type or own shared handles by providing several template arguments to `SharedHandleBase`:
 - A handle type
 - A parent handle type or a header structure, that contains the parent
 - A class itself for CRTP

With this, provide a custom static destruction function `internalDestroy`, that takes in a parent handle and a handle to destroy. Don't forget to add a friend declaration for the base class.

```c++
// Example of a custom shared device, that takes in an instance as a parent
class shared_handle<VkDevice> : public vk::SharedHandleBase<VkDevice, vk::SharedInstance, shared_handle<VkDevice>>
{
  using base = vk::SharedHandleBase<VkDevice, vk::SharedInstance, shared_handle<VkDevice>>;
  friend base;

public:
  shared_handle() = default;
  explicit shared_handle(VkDevice handle, vk::SharedInstance parent) noexcept
    : base(handle, std::move(parent)) {}

  const auto& getParent() const noexcept
  {
    return getHeader();
  }

protected:
  static void internalDestroy(const vk::SharedInstance& /*control*/, VkDevice handle) noexcept
  {
    kDestroyDevice(handle);
  }
};
```

The API will be extended to provide creation functions in the future.

### RAII-classes <a id='raii_classes'>

In addition to `vk::UniqueHandles` and `vk::SharedHandles`, there's a set of wrapper classes for all the handle types that follow the RAII-paradigm (resource acquisition is initialization), provided in the `vk::raii` namespace.

While a `vk::UniqueHandle` mimics a handle wrapped by a `std::unique_ptr`, and a `vk::SharedHandle` mimics a handle wrapped by a `std::shared_ptr`, including parent information, a `vk::raii::Handle` is just a class that acquires the underlying vk-handle in its constructor and releases it in its destructor. Thus, you're free to use them as values or wrap them with some smart pointer.

Other than the `vk::Handles`, all those handle wrapper classes need to hold additional data, and thus are not binary identical with the vulkan C-handles.

As the `vk::UniqueHandles` and the `vk::SharedHandles` use the same dispatcher as the `vk::Handles`, they can be easily mixed-and-matched. The `vk::raii::Handles` use some slightly different dispatchers and thus are not compatible with the other handles! That is, for the `vk-Handles`, the `vk::UniqueHandles`, and the `vk::SharedHandles`, you need to instantiate a global dispatcher as described in https://github.com/KhronosGroup/Vulkan-Hpp#extensions--per-device-function-pointers. For the `vk::raii-Handles`, this is not needed, as they maintain their own dispatchers. The big advantage here is when you have multiple devices: the functions called via the `vk::raii-Handles` always call the device specific functions.

### Custom allocators <a id='custom_allocators'>

Sometimes it is required to use `std::vector` with custom allocators. Vulkan-Hpp supports vectors with custom allocators as input for `vk::ArrayProxy` and for functions which do return a vector. For the latter case, add your favorite custom allocator as template argument to the function call like this:

```c++
std::vector<LayerProperties, MyCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties<MyCustomAllocator>();
```

You can also use a stateful custom allocator by providing it as an argument to those functions. Unfortunately, to make the compilers happy, you also need to explicitly set the Dispatch argument. To get the default there, a simple `{}` would suffice:

```c++
MyStatefulCustomAllocator allocator;
std::vector<LayerProperties, MyStatefulCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties(allocator, {});
```

### Custom assertions <a id='custom_assertions'>

All over `vulkan.hpp`, there are a couple of calls to an assert function. By defining `VULKAN_HPP_ASSERT`, you can specifiy your own custom assert function to be called instead.

By default, `VULKAN_HPP_ASSERT_ON_RESULT` will be used for checking results when `VULKAN_HPP_NO_EXCEPTIONS` is defined. If you want to handle errors by yourself, you can disable/customize it just like `VULKAN_HPP_ASSERT`.

There are a couple of static assertions for each handle class and each struct in the file [`vulkan_static_assertions.hpp`](vulkan/vulkan_static_assertions.hpp). You might include that file in at least one of your source files. By defining `VULKAN_HPP_STATIC_ASSERT`, you can specify your own custom static assertion to be used for those cases. That is, by defining it to be a NOP, you can reduce your compilation time a little.

### Extensions / Per Device function pointers <a id='extensions'>

The Vulkan loader exposes only the Vulkan core functions and a limited number of extensions. To use Vulkan-Hpp with extensions it's required to have either a library which provides stubs to all used Vulkan functions or to tell Vulkan-Hpp to dispatch those functions pointers. Vulkan-Hpp provides a per-function dispatch mechanism by accepting a dispatch class as last parameter in each function call. The dispatch class must provide a callable type for each used Vulkan function. Vulkan-Hpp provides one implementation, `vk::detail::DispatchLoaderDynamic`, which fetches all function pointers known to the library.

```c++
// Providing a function pointer resolving vkGetInstanceProcAddr, just the few functions not depending an an instance or a device are fetched
vk::detail::DispatchLoaderDynamic dld(getInstanceProcAddr);

// Providing an already created VkInstance and a function pointer resolving vkGetInstanceProcAddr, all functions are fetched
vk::detail::DispatchLoaderDynamic dldi(instance, getInstanceProcAddr);

// Providing also an already created VkDevice and optionally a function pointer resolving vkGetDeviceProcAddr, all functions are fetched as well, but now device-specific functions are fetched via vkDeviceGetProcAddr.
vk::detail::DispatchLoaderDynamic dldid( nstance, getInstanceProcAddr, device);

// Pass dispatch class to function call as last parameter
device.getQueue(graphics_queue_family_index, 0, &graphics_queue, dldid);
```

To use the `vk::detail::DispatchLoaderDynamic` as the default dispatcher (means: you don't need to explicitly add it to every function call),  you need to  `#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1`, and have the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in your source code to provide storage for that default dispatcher. Then you can use it by the macro `VULKAN_HPP_DEFAULT_DISPATCHER`, as is shown in the code snippets below.
Creating a full featured `vk::detail::DispatchLoaderDynamic` is a two- to three-step process, where you have three choices for the first step:
1. Before any call into a vk-function you need to initialize the dynamic dispatcher by one of three methods
- Let Vulkan-Hpp do all the work by internally using a little helper class `vk::detail::DynamicLoader`:
```c++
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
```
- Use your own dynamic loader, which just needs to provide a templated function `getProcAddress` (compare with `vk::detail::DynamicLoader` in `vulkan.hpp`):
```c++
    YourDynamicLoader ydl;
    VULKAN_HPP_DEFAULT_DISPATCHER.init(ydl);
```
> [!NOTE]
> You need to keep that dynamic loader object alive until after the last call to a vulkan function in your program. For example by making it static, or storing it globally.
- Use your own initial function pointer of type `PFN_vkGetInstanceProcAddr`:
```c++
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = your_own_function_pointer_getter();
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
```
2. initialize it with a `vk::Instance` to get all the other function pointers:
```c++
    vk::Instance instance = vk::createInstance({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
```
3. optionally initialize it with a `vk::Device` to get device-specific function pointers
```c++
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    assert(!physicalDevices.empty());
    vk::Device device = physicalDevices[0].createDevice({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
```
After the second step above, the dispatcher is fully functional. Adding the third step can potentially result in more efficient code. But if you intend to use multiple devices, you could just omit that third step and let the driver do the device-dispatching.

In some cases the storage for the `vk::detail::DispatchLoaderDynamic` should be embedded in a DLL. For those cases you need to define `VULKAN_HPP_STORAGE_SHARED` to tell Vulkan-Hpp that the storage resides in a DLL. When compiling the DLL with the storage it is also required to define `VULKAN_HPP_STORAGE_SHARED_EXPORT` to export the required symbols.

For all functions, that `VULKAN_HPP_DEFAULT_DISPATCHER` is the default for the last argument to that function. If you want to explicitly provide the dispatcher for each and every function call (when you have multiple dispatchers for different devices, for example) and you want to make sure, that you don't accidentally miss any function call, you can define `VULKAN_HPP_NO_DEFAULT_DISPATCHER` before you include `vulkan.hpp` to remove that default argument.

### Type traits <a id='type_traits'>

`vulkan.hpp` provides a couple of type traits, easing template metaprogramming:
- `template <typename EnumType, EnumType value> struct CppType`
	Maps `IndexType` values (`IndexType::eUint16`, `IndexType::eUint32`, ...) to the corresponding type (`uint16_t`, `uint32_t`, ...) by the member type `Type`;
	Maps `ObjectType` values (`ObjectType::eInstance`, `ObjectType::eDevice`, ...) to the corresponding type (`vk::Instance`, `vk::Device`, ...) by the member type `Type`;
	Maps `DebugReportObjectType` values (`DebugReportObjectTypeEXT::eInstance`, `DebugReportObjectTypeEXT::eDevice`, ...) to the corresponding type (`vk::Instance`, `vk::Device`, ...) by the member type `Type`;
- `template <typename T> struct IndexTypeValue`
	Maps scalar types (`uint16_t`, `uint32_t`, ...) to the corresponding `IndexType` value (`IndexType::eUint16`, `IndexType::eUint32`, ...).
- `template <typename T> struct isVulkanHandleType`
	Maps a type to `true` if and only if it's a handle class (`vk::Instance`, `vk::Device`, ...) by the static member `value`.
- `HandleClass::CType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding C-type (`VkInstance`, `VkDevice`, ...) by the member type `CType`.
- `HandleClass::objectType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding `ObjectType` value (`ObjectType::eInstance`, `ObjectType::eDevice`, ...) by the static member `objectType`.
- `HandleClass::debugReportObjectType`
	Maps a handle class (`vk::Instance`, `vk::Device`, ...) to the corresponding `DebugReportObjectTypeEXT` value (`DebugReportObjectTypeEXT::eInstance`, `DebugReportObjectTypeEXT::eDevice`, ...) by the static member `debugReportObjectType`.

### vk::Format trait functions <a id='vk_format_traits'>

With the additional header [`vulkan_format_traits.hpp`](vulkan/vulkan_format_traits.hpp), a couple of trait functions on `vk::Format` are provided. With C++14 and above, all those functions are marked as `constexpr`, that is with appropriate arguments, they are resolved at compile time.
- `std::array<uint8_t, 3> blockExtent( vk::Format format );`
	Gets the three-dimensional extent of texel blocks.
- `uin8_t blockSize( vk::Format format );`
	Gets the texel block size of this format in bytes.
- `char const * compatibilityClass( VULKAN_HPP_NAMESPACE::Format format );`
	The class of the format (can't be just named "class"!)
- `uint8_t componentBits( vk::Format format, uint8_t component );`
	Gets the number of bits in this component, if not compressed, otherwise 0.
- `uint8_t componentCount( vk::Format format );`
	Gets the number of components of this format.
- `char const * componentName( vk::Format format, uint8_t component );`
	Gets the name of this component as a c-string.
- `char const * componentNumericFormat( vk::Format format, uint8_t component );`
	Gets the numeric format of this component as a c-string.
- `uint8_t componentPlaneIndex( vk::Format format, uint8_t component );`
	Gets the plane index, this component lies in.
- `bool componentsAreCompressed( vk::Format format );`
	True, if the components of this format are compressed, otherwise False.
- `char const * compressionScheme( vk::Format format );`
	Gets a textual description of the compression scheme of this format, or an empty text if it is not compressed.
- `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthFormats();`
	Get all formats with a depth component
- `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthStencilFormats();`
	Get all formats with a depth and a stencil component
- `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getStencilFormats();`
	Get all formats with a stencil component
- `bool hasDepthComponent( VULKAN_HPP_NAMESPACE::Format format );`
	True, if this format has a depth component, otherwise false.
- `hasStencilComponent( VULKAN_HPP_NAMESPACE::Format format );`
	True, if this format has a stencil component, otherwise false.
- `bool isCompressed( vk::Format format );`
	True, if this format is a compressed one, otherwise false.
- `uint8_t packed( vk::Format format );`
	Gets the number of bits into which the format is packed. A single image element in this format can be stored in the same space as a scalar type of this bit width.
- `vk::Format planeCompatibleFormat( vk::Format format, uint8_t plane );`
	Gets a single-plane format compatible with this plane.
- `uint8_t planeCount( vk::Format format );`
	Gets the number of image planes of this format.
- `uint8_t planeHeightDivisor( vk::Format format, uint8_t plane );`
	Gets the relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
- `uint8_t planeWidthDivisor( vk::Format format, uint8_t plane );`
	Gets the relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
- `uint8_t texelsPerBlock( vk::Format format );`
	Gets the number of texels in a texel block.

### Hashing Vulkan types <a id='hashing'>

With the additional header [`vulkan_hash.hpp`](vulkan/vulkan_hash.hpp), you get specializations of `std::hash` for the handle wrapper classes and, with C++14, for the structure wrappers. With `VULKAN_HPP_HASH_COMBINE`, you can define your own hash combining algorithm for the structure elements.

### Extension Inspection <a id='extension_inspection'>

With the additional header [`vulkan_extension_inspection.hpp`](vulkan/vulkan_extension_inspection.hpp), some functions to inspect extensions are provided. With C++20 and above, some of those functions are marked as `constexpr`, that is with appropriate arguments, they are resolved at compile time.
Each extension is identified by a string holding its name. Note that there exists a define with that name for each extension.
Some functions might provide information that depends on the vulkan version. As all functions here work solely on strings, the vulkan versions are encoded by a string beginning with "VK_VERSION_", followed by the major and the minor version, separated by an undersore, like this: "VK_VERSION_1_0".
- `std::set<std::string> const & getDeviceExtensions();`
	Gets all device extensions specified for the current platform. Note, that not all of them might be supported by the actual devices.
- `std::set<std::string> const & getInstanceExtensions();`
	Gets all instance extensions specified for the current platform. Note, that not all of them might be supported by the actual instances.
- `std::map<std::string, std::string> const & getDeprecatedExtensions();`
	Gets a map of all deprecated extensions to the extension or vulkan version that is supposed to replace that functionality.
- `std::map<std::string, std::vector<std::vector<std::string>>> const & getExtensionDepends( std::string const & extension );`
	Some extensions depend on other extensions. These dependencies might differ for different vulkan versions, and there might be different sets of dependencies for the very same vulkan version. This function gets a vector of vectors of extensions per vulkan version that the given extension depends on.
- `std::pair<bool, std::vector<std::vector<std::string>> const &> getExtensionDepends( std::string const & version, std::string const & extension );`
	The `first` member of the returned `std::pair` is true, if the given extension is specified for the given vulkan version, otherwise `false`. The `second` member of the returned `std::pair` is a vector of vectors of extensions, listing the separate sets of extensions the given extension depends on for the given vulkan version.
- `std::map<std::string, std::string> const & getObsoletedExtensions();`
	Gets a map of all obsoleted extensions to the extension or vulkan version that has obsoleted that extension.
- `std::map<std::string, std::string> const & getPromotedExtensions();`
	Gets a map of all extensions that got promoted to another extension or to a vulkan version to that extension of vulkan version.
- `VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & extension );`
	Gets the extension or vulkan version the given extension is deprecated by.
- `VULKAN_HPP_CONSTEXPR_20 std::string getExtensionObsoletedBy( std::string const & extension );`
	Gets the extension or vulkan version the given extension is obsoleted by.
- `VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & extension );`
	Gets the extension or vulkan version the given extension is promoted to.
- `VULKAN_HPP_CONSTEXPR_20 bool isDeprecatedExtension( std::string const & extension );`
	Returns `true` if the given extension is deprecated by some other extension or vulkan version.
- `VULKAN_HPP_CONSTEXPR_20 bool isDeviceExtension( std::string const & extension );`
	Returns `true` if the given extension is a device extension.
- `VULKAN_HPP_CONSTEXPR_20 bool isInstanceExtension( std::string const & extension );`
	Returns `true` if the given extension is an instance extension.
- `VULKAN_HPP_CONSTEXPR_20 bool isObsoletedExtension( std::string const & extension );`
	Returns `true` if the given extension is obsoleted by some other extension or vulkan version.
- `VULKAN_HPP_CONSTEXPR_20 bool isPromotedExtension( std::string const & extension );`
	Returns `true` if the given extension is promoted to some other extension or vulkan version.

### C++20 named module <a id='named_module'>

> [!WARNING]
> The current version of Microsoft Visual Studio 2022 is not able to handle the vulkan.cppm module. A bug is filed (<https://developercommunity.visualstudio.com/t/On-building-a-C20-module:-fatal--error/10469799#T-ND10485943>). You can at least use this feature if you don't need or want to use `vk::UniqueHandle` or `vk::SharedHandle` by defining `VULKAN_HPP_NO_SMART_HANDLE`.

#### Overview <a id='named_module_overview'>

Vulkan-Hpp provides a [C++ named module](https://en.cppreference.com/w/cpp/language/modules), `vulkan_hpp` in [`vulkan.cppm`](vulkan/vulkan.cppm).
C++ modules are intended to supersede header files. Modules have potential to drastically improve compilation times for large projects, as declarations and definitions may be easily shared across translation units without repeatedly parsing headers.
Vulkan-Hpp has some extremely long headers (e.g. [`vulkan_structs.hpp`](vulkan/vulkan_structs.hpp)), and the C++ module is likely to shorten compile times for projects currently using it.

#### Compiler support <a id='compiler_support'>

This feature requires a recent compiler with complete C++20 support:

* Visual Studio 2019 16.10 or later (providing `cl.exe` 19.28 or later)
* Clang 15.0.0 or later

If you intend to use CMake's C++ module support (and possibly Ninja), then more recent tools are required:

* Visual Studio 2022 17.4 or later (providing `cl.exe` 19.34 or later)
* Clang 17.0.0 or later
* GCC 14.0 or later
* CMake 3.28 or later
* Ninja 1.10.2 or later

> [!WARNING]
> The Vulkan-Hpp C++ named module is still experimental. Some suggested ways to use it in your projects are below. The long-term goal is to submit patches to the CMake [`FindVulkan`](https://cmake.org/cmake/help/latest/module/FindVulkan.html) module so that users may transparently configure the named module, without needing to declare it as an additional library in consumer CMake code.

##### Usage with CMake <a id='cmake_usage'>

CMake is recommended for use with the Vulkan-Hpp named module, as it provides a convenient platform-agnostic way to configure your project.
CMake version 3.28 or later is required to support C++ modules. Refer to the [CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html) on the topic.

CMake provides the [FindVulkan module](https://cmake.org/cmake/help/latest/module/FindVulkan.html), which may be used to source the Vulkan SDK and Vulkan headers on your system.

```cmake
# find Vulkan SDK
find_package( Vulkan REQUIRED )

# Require Vulkan version ≥ 1.3.256 (earliest version when the Vulkan module was available)
if( ${Vulkan_VERSION} VERSION_LESS "1.3.256" )
  message( FATAL_ERROR "Minimum required Vulkan version for C++ modules is 1.3.256. "
           "Found ${Vulkan_VERSION}."
  )
endif()

# set up Vulkan C++ module as a library
add_library( VulkanHppModule )
target_sources( VulkanHppModule PRIVATE
  FILE_SET CXX_MODULES
  BASE_DIRS ${Vulkan_INCLUDE_DIR}
  FILES ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
)
target_compile_features( VulkanHppModule PUBLIC cxx_std_20 )
target_link_libraries( VulkanHppModule PUBLIC Vulkan::Vulkan )

# link Vulkan C++ module into your project
add_executable( YourProject main.cpp )
target_link_libraries( YourProject PRIVATE VulkanHppModule )
```

Configuring the named module is straightforward; add any required Vulkan-Hpp feature macros (listed in [Configuration Options](#configuration-options)) to `target_compile_definitions`. For instance:

```cmake
# Disable exceptions, disable smart handles, disable constructors
target_compile_definitions( VulkanHppModule PRIVATE
  VULKAN_HPP_NO_EXCEPTIONS
  VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_NO_CONSTRUCTORS
)
```

It is important to have `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` defined equally for both the module and an importing project. To use the [dynamic dispatcher](#extensions--per-device-function-pointers), set it to `1`; otherwise, leave it undefined or set it to `0`. In CMake, do this in a single line with `target_compile_definitions` and the `PUBLIC` scope:

```cmake
target_compile_definitions( VulkanHppModule PUBLIC
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)
# ...
target_link_libraries( YourProject PRIVATE VulkanHppModule )
```

Furthermore, you may also prefer linking `VulkanHppModule` to just the `Vulkan::Headers` target with the `PUBLIC` scope instead of `Vulkan::Vulkan`, so that the `vulkan-1` library is not linked in, and the Vulkan headers are available to your consuming project, as detailed further below.

```cmake
target_link_libraries( VulkanHppModule PUBLIC Vulkan::Headers )
```

Finally, supply the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in your source code, just as in the non-module case. In order to have that macro available, include [`vulkan_hpp_macros.hpp`](vulkan/vulkan_hpp_macros.hpp), a lightweight header providing all Vulkan-Hpp related macros and defines. And as explained above, you need to initialize that dispatcher in two or three steps:

```cpp
import vulkan_hpp;

#include <vulkan/vulkan_hpp_macros.hpp>

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

auto main(int argc, char* const argv[]) -> int
{
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize minimal set of function pointers
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif

  auto appInfo = vk::ApplicationInfo("My App", 1, "My Engine", 1, vk::makeApiVersion(1, 0, 0, 0));
  // ...
}
```

An example is provided in [`tests/Cpp20Modules/Cpp20Modules.cpp`](tests/Cpp20Modules/Cpp20Modules.cpp).

Finally, you can configure and build your project as usual.
Note that CMake currently only supports the Ninja and Visual Studio generators for C++ modules.

##### Command-line usage <a id='command_line_usage'>

If you want to use the Vulkan-Hpp C++ module without CMake, you must first pre-compile it, and then import it into your project.
You will also need to define any macros that control various features of Vulkan-Hpp, such as `VULKAN_HPP_NO_EXCEPTIONS` and `VULKAN_HPP_NO_SMART_HANDLE`.
Different compilers have different command-lines for module pre-compilation; however, for initial use, some examples are provided below, assuming the same `main.cpp` consumer as above.

For MSVC, source `vcvars64.bat` or use a Developer Command Prompt/Developer PowerShell instance, and run the following:

```cmd
cl.exe /std:c++20 /interface /TP <path-to-vulkan-hpp>\vulkan.cppm
cl.exe /std:c++20 /reference vulkan=vulkan.ifc main.cpp vulkan.obj
.\main.exe
```

For Clang, run the following:

```shell
clang++ -std=c++20 <path-to-vulkan-hpp>/vulkan.cppm -precompile -o vulkan.pcm
clang++ -std=c++20 -fprebuilt-module-path=. main.cpp vulkan.pcm -o main
./main
```

More information about module compilation may be found at the respective compiler's documentation:

* [MSVC](https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp?view=msvc-170)
* [Clang](https://clang.llvm.org/docs/StandardCPlusPlusModules.html)

### Samples and Tests <a id='samples_and_tests'>

When you configure your project using CMake, you can enable SAMPLES_BUILD to add some sample projects to your solution. Most of them are ports from the LunarG samples, but there are some more, like CreateDebugUtilsMessenger, InstanceVersion, PhysicalDeviceDisplayProperties, PhysicalDeviceExtensions, PhysicalDeviceFeatures, PhysicalDeviceGroups, PhysicalDeviceMemoryProperties, PhysicalDeviceProperties, PhysicalDeviceQueueFamilyProperties, and RayTracing. All those samples should just compile and run.
When you configure your project using CMake, you can enable TESTS_BUILD to add some test projects to your solution. Those tests are just compilation tests and are not required to run.

### Compile time issues <a id='compile_time_issues'>

As `vulkan.hpp` is pretty big, some compilers might need some time to digest all that stuff. In order to potentially reduce the time needed to compile that header, a couple of defines will be introduced, that allow you to hide certain features. Whenever you don't need that corresponding feature, defining that value might improve your compile time.
Currently, there are just a couple of such defines:
- `VULKAN_HPP_NO_SPACESHIP_OPERATOR`, which removes the spaceship operator on structures (available with C++20)
- `VULKAN_HPP_NO_TO_STRING`, which removes the various `vk::to_string` functions on enums and bitmasks.
- `VULKAN_HPP_USE_REFLECT`, this one needs to be defined to use the reflection function on structures. It's very slow to compile, though!

### Strict aliasing issue <a id='strict_aliasing_issue'>

As Vulkan-Hpp often needs to switch between C++ vk-types and corresponding bit-identical C-types, using `reinterpret_cast`, it is highly recommended to use the compile option `-fno-strict-aliasing` to prevent potentially breaking compile optimizations.

## Configuration Options <a id='options'>

There are a couple of defines you can use to control the feature set and behaviour of `vulkan.hpp`:

#### VULKAN_HPP_ASSERT <a id='assert'>

At various places in `vulkan.hpp` an assertion statement is used. By default, the standard assert funtions from `<cassert>` is called. By defining `VULKAN_HPP_ASSERT` before including `vulkan.hpp`, you can change that to any function with the very same interface.

#### VULKAN_HPP_ASSERT_ON_RESULT <a id='assert_on_result'>

If there are no exceptions enabled (see `VULKAN_HPP_NO_EXCEPTIONS`), an assertion statement checks for a valid success code returned from every vulkan call. By default, this is the very same assert function as defined by `VULKAN_HPP_ASSERT`, but by defining `VULKAN_HPP_ASSERT_ON_RESULT` you can replace just those assertions with your own function, using the very same interface.

#### VULKAN_HPP_DEFAULT_DISPATCHER <a id='default_dispatcher'>

Every vk-function gets a Dispatcher as its very last argument, which defaults to `VULKAN_HPP_DEFAULT_DISPATCHER`. If `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` is defined to be `1`, it is `vk::detail::defaultDispatchLoaderDynamic`. This in turn is the dispatcher instance, which is defined by `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`, which has to be used exactly once in your sources. If, on the other hand, `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` is defined to something different from `1`, `VULKAN_HPP_DEFAULT_DISPATCHER` is set to be `vk::detail::DispatchLoaderStatic()`.
You can use your own default dispatcher by setting `VULKAN_HPP_DEFAULT_DISPATCHER` to an object that provides the same API. If you explicitly set `VULKAN_HPP_DEFAULT_DISPATCHER`, you need to set `VULKAN_HPP_DEFAULT_DISPATCHER_TYPE` accordingly as well.

#### VULKAN_HPP_DEFAULT_DISPATCHER_TYPE <a id='default_dispatcher_type'>

This names the default dispatcher type, as specified by `VULKAN_HPP_DEFAULT_DISPATCHER`. Per default, it is `vk::detail::DispatchLoaderDynamic` or `vk::detail::DispatchLoaderStatic`, depending on `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` being `1` or not `1`, respectively. If you explicitly set `VULKAN_HPP_DEFAULT_DISPATCHER`, you need to set `VULKAN_HPP_DEFAULT_DISPATCHER_TYPE` accordingly as well.

#### VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE <a id='default_dispatch_loader_dynamic_storage'>

If you have not defined your own `VULKAN_HPP_DEFAULT_DISPATCHER`, and have `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` defined to be `1` (the default), you need to have the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in any of your source files to provide storage for that default dispatcher. `VULKAN_HPP_STORAGE_API` then controls the import/export status of that default dispatcher.

#### VULKAN_HPP_DISABLE_ENHANCED_MODE <a id='disable_enhanced_mode'>

When this is defined before including `vulkan.hpp`, you essentially disable all enhanced functionality. All you then get is:
* improved compile time error detection, via scoped enums;
* usage of the helper class `vk::Flags` for bitmasks;
* wrapper structs for all vulkan structs providing default initialization;
* the helper class `vk::StructureChain` for compile-time construction of structure chains.

If this is not defined, you additionally get:
* enhanced versions of the commands (consuming `vk::ArrayProxy<>`), simplifying handling of array data; returning requested data; throwing exceptions on errors (as long as `VULKAN_HPP_NO_EXCEPTIONS` is not defined);
* enhanced structure constructors (as long as `VULKAN_HPP_NO_STRUCT_CONSTRUCTORS` is not defined) (consuming `vk::ArrayProxyNoTemporaries<>`);
* enhanced setter functions on some members of structs (consuming `vk::ArrayProxyNoTemporaries<>`);
* the helper classes `vk::ArrayProxy<>` and `vk::ArrayProxyNoTemporaries<>`
* all the RAII-stuff in `vulkan_raii.hpp`

#### VULKAN_HPP_DISPATCH_LOADER_DYNAMIC <a id='dispatch_loader_dynamic'>

This either selects the dynamic (when it's `1`) or the static (when it's not `1`) DispatchLoader as the default one, as long as it's not explicitly specified by `VULKAN_HPP_DEFAULT_DISPATCHER`. By default, this is defined to be `1` if `VK_NO_PROTOTYPES` is defined, otherwise `0`.

#### VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL <a id='enable_dynamic_loader_tool'>

By default, a little helper class `vk::detail::DynamicLoader` is used to dynamically load the vulkan library. If you set it to something different than `1` before including `vulkan.hpp`, this helper is not available, and you need to explicitly provide your own loader type for the function `vk::detail::DispatchLoaderDynamic::init()`.

#### VULKAN_HPP_EXPECTED <a id='expected'>

When this is not externally defined and `VULKAN_HPP_CPP_VERSION` is at least `23`, `VULKAN_HPP_EXPECTED` is defined to be `std::expected`, and `VULKAN_HPP_UNEXPECTED` is defined to be `std::unexpected`.

#### VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC <a id='flags_mask_type_as_public'>

By default, the member `m_mask` of the `Flags` class template is private. This is to prevent accidentally setting a `Flags` with some inappropriate value. But it also prevents using a `Flags`, or a structure holding a `Flags`, to be used as a non-type template parameter. If you really need that functionality, and accept the reduced security, you can use this define to change the access specifier for `m_mask` from private to public, which allows using a `Flags` as a non-type template parameter.

#### VULKAN_HPP_HANDLES_MOVE_EXCHANGE <a id='handles_move_exchange'>

This define can be used to enable `m_handle = exchange( rhs.m_handle, {} )` in move constructors of Vulkan-Hpp handles, which default-initializes the `rhs` underlying value. By default Vulkan-Hpp handles behave like trivial types -- move constructors copying value.

#### VULKAN_HPP_HASH_COMBINE <a id='hash_combine'>

This define can be used to specify your own hash combiner function. In order to determine the hash of a vk-structure, the hashes of the members of that struct are to be combined. This is done by this define, which by default is identical to what the function `boost::hash_combine()` does. It gets the type of the to-be-combined value, the seed, which is the combined value up to that point, and finally the to-be-combined value. This hash calculation determines a "shallow" hash, as it takes the hashes of any pointer in a structure, and not the hash of a pointed-to value.

#### VULKAN_HPP_INLINE <a id='inline'>

This is set to be the compiler-dependent attribute used to mark functions as inline. If your compiler happens to need some different attribute, you can set this define accordingly before including `vulkan.hpp`.

#### VULKAN_HPP_NAMESPACE <a id='namespace'>

By default, the namespace used with `vulkan.hpp` is `vk`. By defining `VULKAN_HPP_NAMESPACE` before including `vulkan.hpp`, you can adjust this.

#### VULKAN_HPP_NO_CONSTRUCTORS <a id='no_constructors'>

With C++20, designated initializers are available. Their use requires the absence of any user-defined constructors. Define `VULKAN_HPP_NO_CONSTRUCTORS` to remove constructors from structs and unions.

#### VULKAN_HPP_NO_EXCEPTIONS <a id='no_exceptions'>

When a vulkan function returns an error code that is not specified to be a success code, an exception is thrown unless `VULKAN_HPP_NO_EXCEPTIONS` is defined before including `vulkan.hpp`.

#### VULKAN_HPP_NO_NODISCARD_WARNINGS <a id='no_discard_warnings'>

With C++17, all vk-functions returning something are declared with the attribute `[[nodiscard]]`. This can be removed by defining `VULKAN_HPP_NO_NODISCARD_WARNINGS` before including `vulkan.hpp`.

#### VULKAN_HPP_NO_SETTERS <a id='no_setters'>

By defining `VULKAN_HPP_NO_SETTERS` before including `vulkan.hpp`, setter member functions will not be available within structs and unions. Modifying their data members will then only be possible via direct assignment.

#### VULKAN_HPP_NO_SMART_HANDLE <a id='no_smart_handle'>

By defining `VULKAN_HPP_NO_SMART_HANDLE` before including `vulkan.hpp`, the helper class `vk::UniqueHandle` and all the unique handle types are not available.

#### VULKAN_HPP_NO_SPACESHIP_OPERATOR <a id='no_spaceship_operator'>

With C++20, the so-called spaceship-operator `<=>` is introduced. If that operator is supported, all the structs and classes in vulkan.hpp use the default implementation of it. As currently some implementations of this operator are very slow, and others seem to be incomplete, by defining `VULKAN_HPP_NO_SPACESHIP_OPERATOR` before including `vulkan.hpp` you can remove that operator from those structs and classes.

#### VULKAN_HPP_NO_TO_STRING <a id='no_to_string'>

By default, the file [`vulkan_to_string.hpp`](vulkan/vulkan_to_string.hpp) is included by `vulkan.hpp` and provides functions `vk::to_string` for enums and bitmasks. If you don't need those functions, you can define `VULKAN_HPP_NO_TO_STRING` to prevent that inclusion. If you have certain files where you want to use those functions nevertheless, you can explicitly include `vulkan_to_string.hpp` there.

#### VULKAN_HPP_NO_WIN32_PROTOTYPES <a id='no_win32_prototypes'>

By default, if `VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL` is enabled on Win32, `vulkan.hpp` declares `HINSTANCE`, `LoadLibraryA`, and other required symbols. It could cause conflicts with the `Windows.h` alternatives, such as `WindowsHModular`.
With this define, you can disable these declarations, but you will have to declare them before the `vulkan.hpp` is included.

#### VULKAN_HPP_RAII_NO_EXCEPTIONS <a id='raii_no_exceptions'>

If both, `VULKAN_HPP_NO_EXCEPTIONS` and `VULKAN_HPP_EXPECTED` are defined, the vk::raii-classes don't throw exceptions. That is, the actual constructors are not available, but the creation-functions must be used. For more details have a look at the [`vk_raii_ProgrammingGuide.md`](vk_raii_ProgrammingGuide.md).

#### VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST <a id='smart_handle_implicit_cast'>

Even though `vk::UniqueHandle` and `vk::SharedHandle` are semantically close to pointers, an implicit cast operator to the underlying `vk::Handle` might be handy. You can add that implicit cast operator by defining `VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST`.

#### VULKAN_HPP_STORAGE_API <a id='storage_api'>

With this define you can specify whether the `vk::detail::DispatchLoaderDynamic` is imported or exported (see `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`). If `VULKAN_HPP_STORAGE_API` is not defined externally, and `VULKAN_HPP_STORAGE_SHARED` is defined, depending on the `VULKAN_HPP_STORAGE_SHARED_EXPORT` being defined, `VULKAN_HPP_STORAGE_API` is either set to `__declspec( dllexport )` (for MSVC) / `__attribute__( ( visibility( "default" ) ) )` (for gcc or clang) or `__declspec( dllimport )` (for MSVC), respectively. For other compilers, you might specify the corresponding storage by defining `VULKAN_HPP_STORAGE_API` on your own.

#### VULKAN_HPP_TYPESAFE_CONVERSION <a id='typesafe_conversion'>

32-bit vulkan is not typesafe for non-dispatchable handles, so we don't allow copy constructors on this platform by default. To enable this feature on 32-bit platforms, `#define VULKAN_HPP_TYPESAFE_CONVERSION 1`. To disable this feature on 64-bit platforms, `#define VULKAN_HPP_TYPESAFE_CONVERSION 0`.

#### VULKAN_HPP_UNEXPECTED <a id='unexpected'>

See [`VULKAN_HPP_EXPECTED`](#expected).

#### VULKAN_HPP_USE_REFLECT <a id='use_reflect'>

With this define you can include a reflection mechanism on the vk-structures. It adds a function `reflect` that returns a tuple-version of the structure. That tuple then could easily be iterated. But at least for now, that feature takes lots of compile-time resources, so currently it is recommended to enable that feature only if you're willing to pay that price.

## See Also <a id='see_also'>

Feel free to submit a PR to add to this list.

- [Examples](https://github.com/jherico/vulkan) A port of Sascha Willems [examples](https://github.com/SaschaWillems/Vulkan) to Vulkan-Hpp
- [Vookoo](https://github.com/andy-thomason/Vookoo/) Stateful helper classes for Vulkan-Hpp, [Introduction Article](https://accu.org/journals/overload/25/139/overload139.pdf#page=14).

## License <a id='license'>

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
