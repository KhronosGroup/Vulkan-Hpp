# Usage

This manual assumes familiarity with Vulkan; it details improvements and differences introduced by Vulkan-Hpp.

- [Naming convention](#naming-convention)
- [Vulkan fundamentals](#vulkan-fundamentals)
  - [Structs](#structs)
    - [Constructors](#constructors)
    - [Designated initializers](#designated-initializers)
    - [`ArrayProxy<T>` and `ArrayProxyNoTemporaries<T>`](#arrayproxyt-and-arrayproxynotemporariest)
    - [Builder pattern with setters](#builder-pattern-with-setters)
    - [Structure pointer chains](#structure-pointer-chains)
  - [Handles and functions](#handles-and-functions)
    - [Struct parameters in Vulkan-Hpp functions](#struct-parameters-in-vulkan-hpp-functions)
    - [C/C++ inter-op for handles](#cc-inter-op-for-handles)
  - [Flags](#flags)
  - [Error handling](#error-handling)
    - [Exceptions](#exceptions)
    - [Return value transformations](#return-value-transformations)
    - [`std::expected`](#stdexpected)
  - [Feature and property enumerations](#feature-and-property-enumerations)
  - [Extensions and per-device function pointers](#extensions-and-per-device-function-pointers)
- [C++ language and library features](#c-language-and-library-features)
  - [`[[nodiscard]]`](#nodiscard)
  - [Custom allocators](#custom-allocators)
  - [Custom assertions](#custom-assertions)
  - [Type traits](#type-traits)
    - [`vk::Format` trait functions](#vkformat-trait-functions)
  - [Hashing Vulkan types](#hashing-vulkan-types)
  - [C++20 named module](#c20-named-module)
    - [Compiler support](#compiler-support)
    - [CMake usage](#cmake-usage)
    - [Command-line usage](#command-line-usage)
- [Miscellaneous features and notes](#miscellaneous-features-and-notes)
  - [Extension Inspection](#extension-inspection)
- [Samples and Tests](#samples-and-tests)
- [Compile time issues](#compile-time-issues)
- [Strict aliasing](#strict-aliasing)

## Naming convention

All symbols (functions, handles, structs, enums) of Vulkan-Hpp are defined in the `vk::` namespace.
This can be renamed by end-users to a custom namespace by defining the `VULKAN_HPP_NAMESPACE` macro in the build system.

All symbols have been renamed from the C API in meaningful ways. The following rules apply:

- The `Vk` or `vk` prefix is removed.
- The first character of function names is always lowercase.

For example:

- `vkCreateInstance` corresponds to `vk::createInstance`.
- `VkImageTiling` corresponds to `vk::ImageTiling`.
- `VkImageCreateInfo` corresponds to `vk::ImageCreateInfo`.

Untyped unsigned integer enum values in the C API are mapped to scoped `enum class`es to provide compile-time type safety.
There is a slightly different naming convention:

- The `VK_` prefix and type infix are removed.
- An `e` prefix is added, and the casing is CamelCase.
- If the enum type is an extension, the `extension` suffix has been removed from the enum **values**.
- In all other cases, the extension suffix has not been removed.

For example:

- `VK_IMAGETYPE_2D` corresponds to `vk::ImageType::e2D`.
- `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR` corresponds to `vk::ColorSpaceKHR::eSrgbNonlinear`.
- `VK_STRUCTURE_TYPE_PRESENT_INFO_KHR` corresponds to `vk::StructureType::ePresentInfoKHR`.

Flag bits are handled similarly; additionally, the `_BIT` suffix is removed.

## Vulkan fundamentals

### Structs

#### Constructors

In Vulkan, many operations require that an `Info` struct is populated, and then passed as a pointer to some function; possibly a `vkCreateHandle` or `vkOperationWithHandle`.
This results in the following C-style code:

```cpp
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

This is long, imperative, and error-prone. This field-by-field initialisation has several pitfalls:

- One or more fields may be left uninitialized when a non-default value is intended;
- the `::sType` field may be incorrect;
- flag bits and enums may be initialised to unrelated (and hence invalid) values;
- the pointer-and-count pairs may be inconsistent.

Instead, Vulkan-Hpp provides structs with constructors.
These accept one parameter for each member variable corresponding to their C API, with a few additional rules:

- The `sType` member is omitted, and default-constructed internally;
- If there is a `pNext` member, it is the _last_ parameter in the constructor and defaults to `nullptr`, so that it may be omitted

This allows for more concise and safer code.
When an argument is left out, static analysis tools like IDEs and linters will error immediately, and the code will fail to compile.
For example, the above code may be written with Vulkan-Hpp as:

```cpp
vk::ImageCreateInfo ci({}, // flags
                       vk::ImageType::e2D, // imageType
                       vk::Format::eR8G8B8A8Unorm,
                       { width, height, 1 },
                       1,
                       1,
                       vk::SampleCountFlagBits::e1,
                       vk::ImageTiling::eOptimal,
                       vk::ImageUsageFlagBits::eColorAttachment,
                       vk::SharingMode::eExclusive,
                       0,
                       nullptr,
                       vk::ImageLayout::eUndefined);
```

Use brace-pairs to create and pass structs as temporaries to Vulkan functions, like this:

```cpp
vk::Image image = device.createImage({{}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                                     { width, height, 1 },
                                     1, 1, vk::SampleCountFlagBits::e1,
                                     vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                                     vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined});
```

#### Designated initializers

>[!NOTE]
> This feature requires a compiler supporting at least C++20, and must be **enabled** with the `VULKAN_HPP_NO_CONSTRUCTORS` or `VULKAN_HPP_NO_STRUCT_CONSTRUCTORS` macro.
> This will disable the above-mentioned generated constructors for structs.

C++20 and later versions support [designated initializers](https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers) for aggregate types.
Designated initializers allow for more expressive code: member variable names are mentioned immediately in plain-text, without needing any static analysis.
The first few lines of Vulkan initialisation might look like the following:

```cpp
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo{ .pApplicationName   = AppName,
                                     .applicationVersion = 1,
                                     .pEngineName        = EngineName,
                                     .engineVersion      = 1,
                                     .apiVersion         = vk::ApiVersion11 };

// initialize vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo{ .pApplicationInfo = &applicationInfo };

// initialize vk::InstanceCreateInfo directly within vk::createInstance
instance = vk::createInstance({ .pApplicationInfo = &applicationInfo });
```

instead of

```cpp
// initialize the vk::ApplicationInfo structure.
vk::ApplicationInfo applicationInfo(AppName, 1, EngineName, 1, vk::ApiVersion11);

// initialize the vk::InstanceCreateInfo. What is `{}`?
vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);
```

The designator order needs to match the declaration order.
Additionally, members may be omitted, in which case they are default-initialized.
For instance, note how `::sType` and `::pNext` are omitted above; they are automatically default-initialized to the correct values or `nullptr`, respectively.

#### `ArrayProxy<T>` and `ArrayProxyNoTemporaries<T>`

>[!NOTE]
> This feature may be **disabled** with the `VULKAN_HPP_DISABLE_ENHANCED_MODE` macro.

The Vulkan API has several instances which require (count, pointer) pairs as two parameters.
Instead, Vulkan-Hpp provides two class templates, `vk::ArrayProxy<T>` and `vk::ArrayProxyNotTemporaries<T>`, which generalise the following into a single parameter:

- empty arrays;
- a single value;
- STL containers including `std::initializer_list`, `std::array`, `std::span` and `std::vector`;
- C-style arrays;
- pointers with counts.

<!-- TODO: uncomment with more detail. Fill in when `ArrayProxy` vs `ArrayProxyNoTemporaries` is typically expected; function out-parameters? Ask @asuessenbach -->
<!-- Note that `vk::ArrayProxyNoTemporaries<T>` **disallows** temporary values, which may be important for out-parameters. -->

For example, consider the following calls to `vk::CommandBuffer::setScissor`:

```cpp
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

#### Builder pattern with setters

>[!NOTE]
> This feature may be **disabled** with the `VULKAN_HPP_NO_STRUCT_SETTERS` macro.

Having _no_ constructors means losing a little flexibility compared to the default. For instance, [`ArrayProxy`](#arrayproxyt-and-arrayproxynotemporariest) parameters cannot be used with designated initializers, which fall back to (count, pointer) pairs.

Instead, Vulkan-Hpp provides setter member functions for _all_ member variables, and (count, pointer) pairs are generalised to either `ArrayProxy` or `ArrayProxyNoTemporaries` for all structs.
Combining these and [designated initialisers](#designated-initialisers) might allow for the best balance of readability and ease-of-use, since designated initialisers can be omitted for fields that are irrelevant.
For instance, `vk::ImageCreateInfo` may be constructed like so:

```cpp
std::array<uint32_t, 2> const queueFamilies = { 1U, 2U };
auto ci = vk::ImageCreateInfo{
  .imageType = vk::ImageType::e2D,
  .format = vk::Format::eR8G8B8A8Unorm,
  .extent = { width, height, 1 },
  .mipLevels = 1,
  .arrayLayers = 1,
  .samples = vk::SampleCountFlagBits::e1,
  .tiling = vk::ImageTiling::eOptimal,
  .usage = vk::ImageUsageFlagBits::eColorAttachment,
  .sharingMode = vk::SharingMode::eExclusive,
  .initialLayout = vk::ImageLayout::eUndefined
}.setQueueFamilyIndices(queueFamilies); // ArrayProxyNoTemporaries<uint32_t>
```

Note how `.flags` is omitted, and the word `index` has been pluralised to `indices` for the setter.

#### Structure pointer chains

Vulkan-Hpp provides a variadic class template for structure chains so that `pNext` chains can be created in a type-safe manner: `vk::StructureChain<T1, T2, ..., Tn>`.
This means only chains which are valid according to the Vulkan specification can be created, which is verified at compile time.

For instance:

```cpp
// This will compile successfully.
vk::StructureChain<vk::MemoryAllocateInfo, vk::ImportMemoryFdInfoKHR> c;
vk::MemoryAllocateInfo &allocInfo = c.get<vk::MemoryAllocateInfo>();
vk::ImportMemoryFdInfoKHR &fdInfo = c.get<vk::ImportMemoryFdInfoKHR>();

// This will fail compilation; vk::MemoryDedicatedRequirementsKHR cannot be part of a pNext-chain.
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedRequirementsKHR> c;
vk::MemoryAllocateInfo &allocInfo = c.get<vk::MemoryAllocateInfo>();
vk::ImportMemoryFdInfoKHR &fdInfo = c.get<vk::ImportMemoryFdInfoKHR>();
```

Vulkan-Hpp provides a constructor similar to the `CreateInfo`, which accepts a list of all structures part of the chain.
The `pNext` field of each structure is populated automatically:

```cpp
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedAllocateInfo> c = {
  vk::MemoryAllocateInfo(size, type),
  vk::MemoryDedicatedAllocateInfo(image)
};
```
<!-- TODO: need an example here. Also more info and examples: what about the same type, repeated? I don't use structure chains. @asuessenbach? -->
To remove a link in a `StructureChain`, use the function `vk::StructureChain::unlink<Type>()`.
This modifies the chain in-place by modifying the `.pNext` structures, and returns the unlinked structure; note that its memory layout remains unchanged.
To re-insert the same structure into the chain, use `vk::StructureChain::relink<Type>()`.

Sometimes the user has to pass a preallocated structure chain to query information. For those cases there are two corresponding getter functions.
One with a variadic template generating a structure chain of at least two elements to construct the return value:

```cpp
// Query vk::MemoryRequirements2HR and vk::MemoryDedicatedRequirementsKHR when calling Device::getBufferMemoryRequirements2KHR:
auto result = device.getBufferMemoryRequirements2KHR<vk::MemoryRequirements2KHR, vk::MemoryDedicatedRequirementsKHR>({});
vk::MemoryRequirements2KHR &memReqs = result.get<vk::MemoryRequirements2KHR>();
vk::MemoryDedicatedRequirementsKHR &dedMemReqs = result.get<vk::MemoryDedicatedRequirementsKHR>();
```

To get just the base structure, without chaining, the other getter function provided does not need a template argument for the structure to get:

```cpp
// Query just vk::MemoryRequirements2KHR
vk::MemoryRequirements2KHR memoryRequirements = device.getBufferMemoryRequirements2KHR({});
```

### Handles and functions

Vulkan-Hpp provides a `vk::` class for each Vulkan handle, such as `vk::Instance`, `vk::Device`, `vk::Buffer`, and so on.
When compiled for 64-bit architectures, these classes are binary-compatible with their corresponding C handles; they can be cast back and forth without any overhead.

> [!NOTE]
> For 32-bit architectures, refer to the guidance in [C/C++ inter-op for handles](#cc-inter-op-for-handles).

Member and free functions are also defined corresponding to a C API function which accepts the parent handle as the **first** parameter.
For example, instead of `vkBindBufferMemory(device, ...)`, write `device.bindBufferMemory(...)`.

>[!NOTE]
> Note that the lifetimes of these handles need to be **managed manually** with pairs of `vk::createHandle` and `vk::destroyHandle` functions, just like in the C API.
>
> For automatic resource management, refer to [Handles](./Handles.md).

#### Struct parameters in Vulkan-Hpp functions

In Vulkan-Hpp, functions accept structs as `vk::StructType const&` parameters instead of pointers.
This straightforwardly allows in-line temporaries, which may result in shorter code.

When an input parameter is considered 'optional' in the C API and is documented to accept null pointers, the parameter type is declared as `vk::Optional<T> const&` in the corresponding Vulkan-Hpp function.
This accepts inputs of type `T&` or `nullptr`, and thus allows _optional_ temporary structs.

Consider the following example:

```cpp
// C API
VkImageSubresource subResource;
subResource.aspectMask = 0;
subResource.mipLevel = 0;
subResource.arrayLayer = 0;
VkSubresourceLayout layout;
vkGetImageSubresourceLayout(device, image, &subresource, &layout);

// Vulkan-Hpp: constant named `vk::ImageSubresource` struct
auto const subResourceCppNamed = { {} /* flags*/, 0 /* miplevel */, 0 /* arrayLayer */ };
auto layout1 = device.getImageSubresourceLayout(image, subResourceCpp);

// Vulkan-Hpp: temporary `vk::ImageSubresource` struct constructed in-place
auto layout2 = device.getImageSubresourceLayout(image, { /* everything defaulted, including flags, set to `{}`; same effect as above*/ });

// Vulkan-Hpp: temporary constructed in-place with designated initializers
auto layout3 = device.getImageSubresourceLayout(image, { .mipLevel = 0, .arrayLayer = 0 });
```

#### C/C++ inter-op for handles

On 64-bit platforms, Vulkan-Hpp supports implicit conversions between handles provided by the C API and Vulkan-Hpp.
On 32-bit platforms, all non-dispatchable handles are defined as `uint64_t`, thus preventing type-conversion checks at compile time which would catch assignments between incompatible handle types.
Therefore, Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default, and users should use `static_cast`: `VkImage = static_cast<VkImage>(cppImage)` to prevent unintended conversions of untyped integers to handles.

If you are developing on a 64-bit platform but want to compile for a 32-bit platform without adding these verbose explicit casts, define the macro `VULKAN_HPP_TYPESAFE_CONVERSION=1` in your build system, or before writing `#include <vulkan/vulkan.hpp>`.
On 64-bit platforms this macro is set to `1` by default and can be set to `0` to disable implicit conversions.

### Flags

Scoped enums add type safety to Vulkan-Hpp flags, but also prevent bitwise operations with these flag bits.

As a solution, Vulkan-Hpp provides a class template, `vk::Flags<>`.
This class is default-initialised to zero, and behaves exactly like a normal bitmask; however, the type safety ensures that it is impossible to set unrelated bits, with errors emitted at compile time.

For example:

```cpp
vk::ImageUsageFlags iu1; // Initialize a bitmask with no bit set
vk::ImageUsageFlags iu2 = {}; // Default-initialize with no bit set
vk::ImageUsageFlags iu3 = vk::ImageUsageFlagBits::eColorAttachment; // initialize with a single value
vk::ImageUsageFlags iu4 = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage; // or two bits to get a bitmask
// vk::ImageUsageFlags iu5 = iu3 | vk::PipelineStageFlagBits::eVertexShader; // Compile-time error; unrelated flag bits
PipelineShaderStageCreateInfo ci({} /* pass a flag without any bits set */, ...);
```

### Error handling

Most Vulkan operations return a `VkResult` type, indicating whether the operation was successful, and if not, what kind of error occurred.
This is mapped to `vk::Result` in Vulkan-Hpp.
Vulkan-Hpp provides several ways to handle these result codes.
There are two class templates, `vk::ResultValue<T>` and `vk::ResultValueType<T>`, which wrap return values along with their corresponding `vk::Result` codes.
These class templates may be defined differently depending on the error-handling method chosen.

<!-- TODO: Explain the class templates in greater detail. @asuessenbach? -->

#### Exceptions

>[!NOTE]
> This feature can be **disabled** with the `VULKAN_HPP_NO_EXCEPTIONS` macro.

By default, Vulkan-Hpp uses exceptions to handle errors.
Each operation wraps its corresponding C API call returning a `VkResult`.
This is cast to `vk::Result`.
On success, the operation returns the desired return value directly: that is, the Vulkan handle or struct.
In this case, `vk::ResultValue<SomeType>::type` is defined as the returned type.
If the operation has clearly failed, a `std::runtime_error` will be immediately thrown.

For example, consider creating a `vk::Device` from a `vk::PhysicalDevice`:

```cpp
try {
  vk::Device device = physicalDevice.createDevice(createInfo); // `physicalDevice is a `vk::PhysicalDevice`
}
catch (std::exception const &e) {
  // handle error and free resources
}
```

Some Vulkan operations such as `vkAcquireNextImageKHR` have **multiple** success states instead of only `vk::Result::eSuccess`.
Their equivalents in Vulkan-Hpp always return `vk::ResultValue<SomeType>` which must be inspected, on top of throwing exceptions when 'error states' are reached.

Consider the following example:

```cpp
try {
  vk::ResultValue<uint32_t> result = device.acquireNextImage2KHR(acquireNextImageInfo);
  uint32_t currentBuffer = result.value;

  switch (result.result) {
    case vk::Result::eSuccess:
      // do something meaningful
      break;
    case vk::Result::eTimeout:
    case vk::Result::eNotReady:
    case vk::Result::eSuboptimalKHR:
      // do something meaningful
      break;
    default:
#ifdef __cpp_lib_unreachable
      std::unreachable(); // will never get here, as other return codes are errors, and is guaranteed to throw instead
#elif defined(__GNUC__) || defined(__clang__)
      __builtin_unreachable();
#elif defined(_MSC_VER)
      __assume(0);
#endif
  }
}
catch (std::exception const& e) {
  // handle error and free resources
}
```

As Vulkan is updated, the behaviour of certain operations may be changed; they may specify returning additional success codes compared to previous behaviour.
The C API will be transparent to this, but corresponding Vulkan-Hpp function signatures may change, and hence consumer code will need to be updated.
These changes will be noted in [Breaking Changes](../README.md/#breaking-changes).

#### Return value transformations

As noted above, exceptions may be disabled by defining the `VULKAN_HPP_NO_EXCEPTIONS` macro.
In this case `vk::ResultValue<SomeType>::type` is a struct containing `vk::Result` and `vk::SomeType`.
This struct may be deconstructed as needed.

Consider the following code which creates a shader module with exceptions enabled:

```cpp
vk::ShaderModule shader1;
try
{
  shader1 = device.createShaderModule({...});
}
catch(std::exception const &e)
{
  // handle error and free resources
}
```

The above code may be rewritten without exceptions as follows, by appropriately handling the return values.

1. Manually check the returned `vk::Result` code, and handle errors as needed with the C-style functions:

   ```cpp
   // No exceptions, no return value transformation
   vk::ShaderModuleCreateInfo createInfo(...);
   vk::ShaderModule shader1;
   vk::Result result = device.createShaderModule(&createInfo, allocator, &shader1);
   if (result.result != vk::Result::eSuccess)
   {
     // handle error code;
     // cleanup and/or return?
     // do other things
   }
   ```

2. Use the return value transformation provided by Vulkan-Hpp to deconstruct `vk::ResultValue<T>`:

   ```cpp
   vk::ResultValue<ShaderModule> shaderResult1 = device.createShaderModule({...} /* createInfo temporary */);
   if (shaderResult1.result != vk::Result::eSuccess)
   {
     // handle error code;
     // cleanup and/or return?
     // return?
   }
   ```

   Additionally, `std::tie` may be used to deconstruct the return value into its components:

   ```cpp
   // std::tie support.
   vk::Result result;
   vk::ShaderModule shaderModule2;
   std::tie(result, shaderModule2) = device.createShaderModule({...} /* createInfo temporary */);
   if (result != vk::Result::eSuccess)
   {
     // handle error code;
     // cleanup and/or return?
     // return?
   }
   ```

3. Use structured bindings to deconstruct `vk::ResultValue<T>`.
   The binding initialisation can even be pushed into an [if-initialiser](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r0.html) statement:

   ```cpp
   if (auto const [result, shaderModule1] = device.createShaderModule({...} /* createInfo temporary */);
       result != vk::Result::eSuccess)
   {
     // handle error code;
     // cleanup?
     // return?
   }
   ```

>[!NOTE]
> Structured bindings and if-initialiser statements requires a compiler supporting at least C++17.

#### `std::expected`

<!-- TODO: Need an example. Perhaps use the same shader module. -->
>[!NOTE]
> This feature requires a compiler and standard library supporting at least C++23, and must be explicitly **enabled** with the `VULKAN_HPP_USE_STD_EXPECTED` macro.

When `std::expected` is available, `vk::ResultValue<T>::type` is defined as `std::expected<T, vk::Result>`.
The result can then be monadically handled with `and_then`, `transform`, and other member functions of `std::expected`.

### Feature and property enumerations

Related to error-handing, Vulkan enumerations require more careful handling of return values.
The following code is quite common:

```cpp
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
// The count may have changed here, start again if properties was not big enough
properties.resize(propertyCount);
```

This loop is tedious and error-prone, and therefore Vulkan-Hpp provides a wrapper that directly returns `std::vector`.
The above code can be replaced by the following single line:

```cpp
std::vector<vk::LayerProperties> properties = physicalDevice.enumerateDeviceLayerProperties();
```

### Extensions and per-device function pointers

The default Vulkan loader, `vulkan-1` exposes only the Vulkan core functions and a limited number of extensions.
To use Vulkan-Hpp with extensions, a mechanism must be provided such that _all_ Vulkan operations have corresponding stubs, or can be dispatched to function pointers.
Vulkan-Hpp provides a per-function dispatch mechanism where each function representing a Vulkan operation accepts as its last parameter a dispatch class.
This class must provide a callable type for each Vulkan function that is required.

<!-- TODO: What is this code sample exactly?  -->
```cpp
// Function pointer resolving `vkGetInstanceProcAddr`, only the free functions that do not require a VkInstance or a VkDevice are fetched
vk::detail::DispatchLoaderDynamic dld(getInstanceProcAddr);

// Previously-created `VkInstance` and a function pointer resolving `vkGetInstanceProcAddr`, all functions are fetched
vk::detail::DispatchLoaderDynamic dldi(instance, getInstanceProcAddr);

//  Previously-created `VkDevice` and optionally a function pointer resolving `vkGetDeviceProcAddr`, all functions are fetched as well, but now device-specific functions are fetched via `vkDeviceGetProcAddr`.
vk::detail::DispatchLoaderDynamic dldid(instance, getInstanceProcAddr, device);

// Pass dispatch class to function call as last parameter
device.getQueue(graphics_queue_family_index, 0, &graphics_queue, dldid);
```

Vulkan-Hpp provides a default dispatch loader, `vk::detail::DispatchLoaderDynamic`, which fetches all function pointers known to the library.
To use this as the default dispatcher (that is, it does not need to be explicitly added to every function call), write `#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1`, and have the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in user source code to provide storage for that default dispatcher.
Then, the macro `VULKAN_HPP_DEFAULT_DISPATCHER` may be used freely, as demonstrated below.
Creating a full featured `vk::detail::DispatchLoaderDynamic` is a two- to three-step process, with three choices for the first step:

1. Before any call into a vk-function, initialise the dynamic dispatcher:

   - Let Vulkan-Hpp do all the work with `vk::detail::DynamicLoader`:

     ```cpp
     VULKAN_HPP_DEFAULT_DISPATCHER.init();
     ```

   - Use your own dynamic loader, which needs to provide a templated function `getProcAddress`. Refer to the implementation of `vk::detail::DynamicLoader` in `vulkan.hpp` for an example.

     <!-- TODO: Add a skeleton implementation of `struct YourDynamicLoader`? -->
     ```cpp
     YourDynamicLoader ydl;
     VULKAN_HPP_DEFAULT_DISPATCHER.init(ydl);
     ```

   - Use your own initial function pointer with a signature matching `PFN_vkGetInstanceProcAddr`:

     ```cpp
         PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = your_own_function_pointer_getter();
         VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
     ```

2. Initialise it with a `vk::Instance` to get all the other function pointers:

   ```cpp
       vk::Instance instance = vk::createInstance({}, nullptr);
       VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
   ```

3. Optionally initialise it with a `vk::Device` to get device-specific function pointers:

   ```cpp
       std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
       assert(!physicalDevices.empty());
       vk::Device device = physicalDevices[0].createDevice({}, nullptr);
       VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
   ```

> [!NOTE]
> The dynamic loader object in the example above (`ydl`) must be kept alive until after the last call to a Vulkan function in your program.
> Use a static/singleton instance or global object, or manage its lifetime manually.

After step 2, the dispatcher is fully functional.
Adding the third step can potentially result in more efficient code.
If you intend to use multiple devices, this last step can be omitted, and delegated to the driver instead.

<!-- TODO: what cases? -->
In some cases, `vk::detail::DispatchLoaderDynamic` may need to be stored in a dynamic library.
To support this use-case, define the macro `VULKAN_HPP_STORAGE_SHARED` so that the storage for the dispatcher is set up appropriately.
When compiling the dynamic library with the storage, the macro `VULKAN_HPP_STORAGE_SHARED_EXPORT` must additionally be defined to export the required symbols.

As mentioned above, `VULKAN_HPP_DEFAULT_DISPATCHER` is the defaulted last argument to all Vulkan functions.
To explicitly provide the dispatcher for every function call (for example, to support multiple dispatchers for different devices), define `VULKAN_HPP_NO_DEFAULT_DISPATCHER` preferably in the build system or command-line invocation of the compiler, or before writing `#include <vulkan/vulkan.hpp>` to disable the default argument.

## C++ language and library features

### `[[nodiscard]]`

> [!NOTE]
> This feature requires a compiler supporting at least C++17.
> This feature can be **disabled** with the `VULKAN_HPP_NO_NODISCARD_WARNINGS` macro.

With appropriate compiler support, many Vulkan operations are marked `[[nodiscard]]`.
This will raise a warning if the return values from these operations are left unused.

### Custom allocators

In some cases, developers may require a custom allocator for `std::vector`.
Vulkan-Hpp supports this in `vk::ArrayProxy` constructors, and for functions which return `std::vector`.
In the latter case, add your custom allocator as a template argument to the function call:

```cpp
std::vector<vk::LayerProperties, MyCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties<MyCustomAllocator>();
```

A stateful custom allocator **object** may also be used.
In this case the object is provided as a usual runtime argument to a function; additionally, the `Dispatch` argument must also be defaulted with a pair of braces `{}`.
Consider the following example:

```cpp
MyStatefulCustomAllocator allocator;
std::vector<vk::LayerProperties, MyStatefulCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties(allocator, {});
```

### Custom assertions

Vulkan-Hpp uses assertions liberally to check for programming errors.
The assert function used may be customised by re-defining the macro `VULKAN_HPP_ASSERT`.

By default, `VULKAN_HPP_ASSERT_ON_RESULT` is used to check the results of Vulkan operations, when `VULKAN_HPP_NO_EXCEPTIONS` is defined.
Redefine this macro to customise this behaviour as well.

There are a couple of static assertions for each handle class and each struct in [`vulkan_static_assertions.hpp`](../vulkan/vulkan_static_assertions.hpp). You might include that file in at least one of your source files. By defining `VULKAN_HPP_STATIC_ASSERT`, you can specify your own custom static assertion to be used for those cases. That is, by defining it to be a NOP, you can reduce your compilation time a little.

### Type traits

Vulkan-Hpp provides several type traits, easing template metaprogramming:

| Type trait | Maps from type | To |
| :--- | :--- | :--- |
| `template <typename EnumType, EnumType value> struct CppType` | `IndexType` value<br>`ObjectType` value<br>`DebugReportObjectType` value | `IndexType` traits: `uint16_t`, `uint32_t`, ...<br>`ObjectType` traits: `vk::Instance`, `vk::Device`, ...<br>`DebugReportObjectType` traits: `vk::Instance`, `vk::Device`, ... |
| `template <typename T> struct IndexTypeValue` | `uint16_t`, `uint32_t`, ... | `IndexType::eUint16`, `IndexType::eUint32`, ... |
| `template <typename T> struct isVulkanHandleType` | `vk::Instance`, `vk::Device`, ... | `true` if and only if `T` is a Vulkan handle type, otherwise `false` |
| `vk::Handle::CType` | `vk::Instance`, `vk::Device`, ... | `VkInstance`, `VkDevice`, ... |
| `vk::Handle::objectType` | `vk::Instance`, `vk::Device`, ... | `ObjectType::eInstance`, `ObjectType::eDevice`, ... |
| `vk::Handle::debugReportObjectType` | `vk::Instance`, `vk::Device`, ... | `DebugReportObjectTypeEXT::eInstance`, `DebugReportObjectTypeEXT::eDevice`, ... |

#### `vk::Format` trait functions

[`vulkan_format_traits.hpp`](../vulkan/vulkan_format_traits.hpp) defines trait functions on `vk::Format`.
When compiled as C++14 and above, all these functions are marked as `constexpr`: that is with appropriate arguments, they are resolved at compile time.

<!-- left col 66% of line width -->

| <div style="width:66%; display:inline-block;" >Function</div> | <div style="width:32%; display:inline-block;" >Returns</div> |
| :--- | :--- |
| `std::array<uint8_t, 3> blockExtent( vk::Format format )` | Three-dimensional extent of texel blocks. |
| `uin8_t blockSize( vk::Format format )` | Texel block size of `format` in bytes. |
| `char const * compatibilityClass( VULKAN_HPP_NAMESPACE::Format format )` | The class of `format`. |
| `uint8_t componentBits( vk::Format format, uint8_t component )` | Number of bits in `component`, if not compressed, otherwise 0. |
| `uint8_t componentCount( vk::Format format )` | Number of components of `format`. |
| `char const * componentName( vk::Format format, uint8_t component )` | Name of `component` as a c-string. |
| `char const * componentNumericFormat( vk::Format format, uint8_t component )` | Numeric format of `component` as a c-string. |
| `uint8_t componentPlaneIndex( vk::Format format, uint8_t component )` | Plane index `component` lies in. |
| `bool componentsAreCompressed( vk::Format format )` | `true`, if the components of `format` are compressed, otherwise `false`. |
| `char const * compressionScheme( vk::Format format )` | Textual description of the compression scheme of `format`, or `""` if it is not compressed. |
| `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthFormats()` | All formats with a depth component. |
| `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthStencilFormats()` | All formats with a depth and a stencil component. |
| `std::vector<VULKAN_HPP_NAMESPACE::Format> const & getStencilFormats()` | All formats with a stencil component. |
| `bool hasDepthComponent( VULKAN_HPP_NAMESPACE::Format format )` | `true`, if `format` has a depth component, otherwise `false`. |
| `hasStencilComponent( VULKAN_HPP_NAMESPACE::Format format )` | `true`, if `format` has a stencil component, otherwise `false`. |
| `bool isCompressed( vk::Format format )` | `true`, if `format` is a compressed one, otherwise `false`. |
| `uint8_t packed( vk::Format format )` | Number of bits into which `format` is packed. A single image element in `format` can be stored in the same space as a scalar type of this bit width. |
| `vk::Format planeCompatibleFormat( vk::Format format, uint8_t plane )` | Single-plane format compatible with `plane`. |
| `uint8_t planeCount( vk::Format format )` | Number of image planes of `format`. |
| `uint8_t planeHeightDivisor( vk::Format format, uint8_t plane )` | Relative height of `plane` with respect to `format`. A value of $k$ means that `plane` is $\frac{1}{k}$ the height of `format`. |
| `uint8_t planeWidthDivisor( vk::Format format, uint8_t plane )` | Relative width of `plane` with respect to `format`. A value of $k$ means that `plane` is $\frac{1}{k}$ the width of `format`. |
| `uint8_t texelsPerBlock( vk::Format format )` | Number of texels in a texel block. |

### Hashing Vulkan types

[`vulkan_hash.hpp`](../vulkan/vulkan_hash.hpp) specialises `std::hash` for `vk::Handle` bindings; with C++14, this is extended to the structure bindings too.
Define `VULKAN_HPP_HASH_COMBINE` to customise the hash-combining algorithm for structure elements.

### C++20 named module

> [!WARNING]
> The Vulkan-Hpp C++ named module is still **experimental** and its interface and usage may change often and **without prior notice** to support better usability and integration.
> We strongly suggest using the **latest** possible toolchains and build systems, as older compilers may have incomplete support, or may emit internal compiler errors (ICEs).

Vulkan-Hpp provides two [C++ named modules](https://clang.llvm.org/docs/StandardCPlusPlusModules.html#standard-c-named-modules), `vulkan` and `vulkan_video` with [`vulkan.cppm`](../vulkan/vulkan.cppm) and [`vulkan_video.cppm`](../vulkan/vulkan_video.cppm) respectively.
C++ modules are intended to supersede header files.
Modules tend to considerably improve compile times, as declarations and definitions may be easily shared across translation units without repeatedly parsing headers.
This is particularly applicable to Vulkan-Hpp, as the generated headers are very large.

Users who are able to upgrade to a recent toolchain (detailed below) should try to use the module instead of the headers, and report bugs and issues.

#### Compiler support

The named module has been tested with the following toolchains:

- MSVC 19.44 or later
- Clang 19.0 or later
- GCC 15.1 or later
- CMake version 4.2.1 or later

>[!NOTE]
> Other toolchain versions and platforms may work, but have not been tested.
> Any toolchain **requires** support for `module std` to import the C++ standard library module, which the Vulkan-Hpp module depends on.

#### CMake usage

> [!NOTE]
> The [CI setup](../.github/workflows/) is a useful reference for setting up CMake projects with the Vulkan-Hpp named module.

CMake is recommended to set up the Vulkan-Hpp named module, as it provides a convenient platform-agnostic way to configure your project.
Version **3.30** or later is required, which uses the standard library module feature.
Refer to the [CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html) on the topic.

CMake provides the [FindVulkan module](https://cmake.org/cmake/help/latest/module/FindVulkan.html), which may be used to source the Vulkan SDK and Vulkan headers on your system.

When invoking CMake, make sure to provide the following UUID to enable CMake's experimental support for the C++ standard library module.
This may also be set before your `project()` call, or in a [CMake preset file](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html#configure-preset) in the `cacheVariables` key for a configure preset.
Note that the UUID may change across CMake versions.
To find it for your specific version, check out the correct release tag and look for `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD` in [Help/dev/experimental.rst](https://gitlab.kitware.com/cmake/cmake/-/blob/master/Help/dev/experimental.rst).

```bash
cmake -DCMAKE_EXPERIMENTAL_CXX_IMPORT_STD=d0edc3af-4c50-42ea-a356-e2862fe7a444 ...
```

A complete example `CMakeLists.txt` file for a project using the Vulkan-Hpp named module is provided below.

```cmake
# this UUID is still valid as of CMake 4.2.1
set( CMAKE_EXPERIMENTAL_CXX_IMPORT_STD d0edc3af-4c50-42ea-a356-e2862fe7a444 ) # remove this line if you are setting the UUID at the command-line or in a preset

# UUID has to be set before setting up the project
project( vulkan_hpp_modules_example LANGUAGES CXX )

# find Vulkan SDK (modules were first made available in 1.3.256)
find_package( Vulkan 1.3.256 REQUIRED )

# set up Vulkan C++ module as a static library
add_library( Vulkan-HppModule STATIC )
add_library( Vulkan::HppModule ALIAS Vulkan-HppModule )
target_sources( Vulkan-HppModule PRIVATE
  FILE_SET CXX_MODULES
  TYPE CXX_MODULES
  BASE_DIRS ${Vulkan_INCLUDE_DIR}
  FILES
    ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
    ${Vulkan_INCLUDE_DIR}/vulkan/vulkan_video.cppm
)

# configure the module target
target_compile_features( Vulkan-HppModule
  PRIVATE cxx_std_23 # (currently) requires C++23 for import std
  INTERFACE cxx_std_20 # can still be consumed as only a C++20 module
)
if( TRUE ) # if you want to use the dynamic dispatcher
  target_link_libraries( Vulkan-HppModule PUBLIC Vulkan::Headers )
  target_compile_definitions( Vulkan-HppModule PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
  )
else() # otherwise, use Vulkan::Vulkan to link to vulkan-1
  target_link_libraries( Vulkan-HppModule PUBLIC Vulkan::Vulkan )
endif()
set_target_properties( Vulkan-HppModule PROPERTIES
  CXX_SCAN_FOR_MODULES ON
  CXX_MODULE_STD ON
)

# link Vulkan C++ module into user project
add_executable( YourProject main.cpp Engine.cpp ... )
target_link_libraries( YourProject PRIVATE Vulkan-HppModule )
```

Configuring the named module is straightforward; add any required Vulkan-Hpp feature macros listed in [Configuration](./Configuration.md) (or any C macros) to `target_compile_definitions`.
Ensure the macros are `PUBLIC` so they can be propagated to user code.
For instance:

```cmake
# Disable exceptions, disable smart handles, disable constructors for structs
target_compile_definitions( Vulkan-HppModule PUBLIC
  VULKAN_HPP_NO_EXCEPTIONS
  VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_NO_CONSTRUCTORS
)
```

This may be combined with the following basic `main.cpp` consumer code:

```cpp
#include <vulkan/vulkan_hpp_macros.hpp> // optional: include Vulkan-Hpp configuration macros
import vulkan;

auto main(int argc, char* const argv[]) -> int
{
  // initialize minimal set of function pointers
  VULKAN_HPP_DEFAULT_DISPATCHER.init();

  auto appInfo = vk::ApplicationInfo("My App", 1, "My Engine", 1, vk::makeApiVersion(1, 0, 0, 0));
  // ...
  // load function pointers with created instance
  VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
  // OPTIONAL: load function pointers specifically for a vk::device
  VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
}
```

Note that the default dynamic dispatcher is automatically stored in the module, and there is **no need** to use the `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` macro.

#### Command-line usage

To use the Vulkan-Hpp C++ module without CMake, you must first pre-compile it, and then import it into your project.
You will also need to define any macros that control various features of Vulkan-Hpp, such as `VULKAN_HPP_NO_EXCEPTIONS` and `VULKAN_HPP_NO_SMART_HANDLE`.
Different compilers have different command-lines for module pre-compilation.
Some examples are provided below, assuming the same `main.cpp` consumer as above.
You may consider adapting these to your build system of choice.

> [!NOTE]
> Remember to pass in macro configuration options for Vulkan and Vulkan-Hpp as needed at the command-line, with `-D` for Clang or GCC, and with `/D` for MSVC.

Clang with LLVM `libc++`:

```shell
clang++ -std=c++23 -stdlib=libc++ --precompile -o std.pcm /path/to/std.cppm
clang++ -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm --precompile -o vulkan.pcm -isystem "<path/to/Vulkan-Hpp/Vulkan-Headers/include>" -isystem "<path/to/Vulkan-Hpp/vulkan>" <path/to/Vulkan-Hpp>/vulkan/vulkan.cppm
clang++ -std=c++23 -stdlib=libc++ -fmodule-file=std=std.pcm -fmodule-file=vulkan=vulkan.pcm \
          main.cpp -o main
./main
```

GCC and the GNU `libstdc++`:

```shell
g++ -std=c++23 -fmodules -fsearch-include-path -c bits/std.cc
g++ -std=c++23 -fmodules -fsearch-include-path -c -isystem "<path/to/Vulkan-Hpp/Vulkan-Headers/include>" -isystem "<path/to/Vulkan-Hpp>/vulkan" <path/to/Vulkan-Hpp>/vulkan/vulkan.cppm
g++ -std=c++23 -fmodules main.cpp -o main
./main
```

MSVC, in a `Developer PowerShell` or `x64 Native Tools Command Prompt for VS` instance:

```powershell
> cl.exe /std:c++latest /EHsc /nologo /W4 /c "$Env:VCToolsInstallDir\modules\std.ixx"
> cl.exe /c /std:c++latest /EHsc /nologo /W4 /reference "std=std.ifc" /interface /TP "<path\to\Vulkan-Hpp>\vulkan\vulkan.cppm" /I "<path\to\Vulkan-Hpp>" /I "<path\to\Vulkan-Hpp>\Vulkan-Headers\include\"
> cl.exe /c /std:c++latest /EHsc /nologo /W4 /reference "std=std.ifc" /reference "vulkan=vulkan.ifc" main.cpp
> link.exe .\std.obj .\vulkan.obj .\main.obj /OUT:main.exe
> .\main.exe
```

More information about module compilation may be found in compiler documentation:

- [Clang](https://clang.llvm.org/docs/StandardCPlusPlusModules.html)
- [GCC](https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Modules.html)
- [MSVC](https://learn.microsoft.com/en-gb/cpp/cpp/modules-cpp?view=msvc-170)

## Miscellaneous features and notes

### Extension Inspection

> [!WARNING]
> This feature considerably increases compilation time.
> Use only when really necessary.

[`vulkan_extension_inspection.hpp`](../vulkan/vulkan_extension_inspection.hpp) defines functions to inspect Vulkan extensions on the machine.
With C++20 and above, some of these functions are marked `constexpr`.

In Vulkan, each extension is identified by a string holding its name.
Note that there exists a macro with that name for each extension.
Some functions might provide information that depends on the vulkan version.
As all functions here operate only on strings, the Vulkan versions are encoded by a string prefixed with "VK_VERSION_", followed by the major and the minor version, separated by an underscore, like this: `VK_VERSION_1_0`.

<!-- TODO: the descriptions in this table need to be reworded a bit, it's a little hard to follow. -->

| Function | `constexpr` | Returns |
| :--- | :---: | :--- |
| `std::set<std::string> const& getDeviceExtensions()` | | All device extensions specified for the current platform. Note that not all of them might be supported by the actual devices. |
| `std::set<std::string> const& getInstanceExtensions()` | | All instance extensions specified for the current platform. Note that not all of them might be supported by the actual instances. |
| `std::map<std::string, std::string> const& getDeprecatedExtensions()` | | A map of all deprecated extensions to the extension or Vulkan version that supersedes the extension. |
| `std::map<std::string, std::vector<std::vector<std::string>>> const& getExtensionDepends( std::string const& extension )` | | Some extensions depend on other extensions. These dependencies might differ for different Vulkan versions, and there might be different sets of dependencies for the very same Vulkan version. This function gets a vector of vectors of extensions per Vulkan version that the given extension depends on. |
| `std::pair<bool, std::vector<std::vector<std::string>> const&> getExtensionDepends( std::string const& version, std::string const& extension )` | | The `first` member of the returned `std::pair` is true, if the given extension is specified for the given Vulkan version, otherwise `false`. The `second` member of the returned `std::pair` is a vector of vectors of extensions, listing the separate sets of extensions the given extension depends on for the given Vulkan version. |
| `std::map<std::string, std::string> const& getObsoletedExtensions()` | | A map of all obsoleted extensions to the extension or Vulkan version that has obsoleted that extension. |
| `std::map<std::string, std::string> const& getPromotedExtensions()` | | A map of all extensions that got promoted to another extension or to a Vulkan version to that extension of Vulkan version. |
| `std::string getExtensionDeprecatedBy( std::string const& extension )` | ✅ | The extension or Vulkan version the given extension is deprecated by. |
| `std::string getExtensionObsoletedBy( std::string const& extension )` | ✅ | The extension or Vulkan version the given extension is obsoleted by. |
| `std::string getExtensionPromotedTo( std::string const& extension )` | ✅ | The extension or Vulkan version the given extension is promoted to. |
| `bool isDeprecatedExtension( std::string const& extension )` | ✅ | `true` if the given extension is deprecated by some other extension or Vulkan version. |
| `bool isDeviceExtension( std::string const& extension )` | ✅ | `true` if the given extension is a device extension. |
| `bool isInstanceExtension( std::string const& extension )` | ✅ | `true` if the given extension is an instance extension. |
| `bool isObsoletedExtension( std::string const& extension )` | ✅ | `true` if the given extension is obsoleted by some other extension or Vulkan version. |
| `bool isPromotedExtension( std::string const& extension )` | ✅ | `true` if the given extension is promoted to some other extension or Vulkan version. |

<!-- TODO: probably need to be updated after recent CI changes. -->
## Samples and Tests

When you configure your project using CMake, you can enable SAMPLES_BUILD to add some sample projects to your solution. Most of them are ports from the LunarG samples, but there are some more, like CreateDebugUtilsMessenger, InstanceVersion, PhysicalDeviceDisplayProperties, PhysicalDeviceExtensions, PhysicalDeviceFeatures, PhysicalDeviceGroups, PhysicalDeviceMemoryProperties, PhysicalDeviceProperties, PhysicalDeviceQueueFamilyProperties, and RayTracing. All those samples should just compile and run.
When you configure your project using CMake, you can enable TESTS_BUILD to add some test projects to your solution. Those tests are just compilation tests and are not required to run.

<!-- TODO: Should we move this to `Configuration.md`? -->
## Compile time issues

As mentioned above, Vulkan-Hpp is a large header, and compilation times may increase drastically.
There are a few configurable options to disable features that may not be required, and thus reduce compile times:

- `VULKAN_HPP_NO_SPACESHIP_OPERATOR`, which removes `operator<=>` for structures (available with C++20)
- `VULKAN_HPP_NO_TO_STRING`, which removes the various `vk::to_string` functions on enums and bitmasks
- `VULKAN_HPP_USE_REFLECT`, this one needs to be defined to use the reflection function on structures. It's very slow to compile, though!
- `VULKAN_HPP_USE_STD_EXPECTED`, this one needs to be defined to use `std::expected` for functions that return a value or a `vk::Result`.

## Strict aliasing

As Vulkan-Hpp often needs to switch between C++ vk-types and corresponding bit-identical C-types using `reinterpret_cast`, it is highly recommended to use the compile option `-fno-strict-aliasing` to prevent potentially breaking compiler optimizations.
