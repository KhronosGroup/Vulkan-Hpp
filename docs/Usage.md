# Usage

This manual assumes familiarity with Vulkan; it details improvements and differences introduced by Vulkan-Hpp.

- [Naming convention](#naming-convention)
- [Vulkan fundamentals](#vulkan-fundamentals)
  - [Structs](#structs)
    - [Constructors](#constructors)
    - [Designated initialisers](#designated-initialisers)
    - [`ArrayProxy<T>` and `ArrayProxyNoTemporaries<T>`](#arrayproxyt-and-arrayproxynotemporariest)
    - [Builder pattern with setters](#builder-pattern-with-setters)
    - [Structure pointer chains](#structure-pointer-chains)
  - [Handles and functions](#handles-and-functions)
    - [Passing structs to functions](#passing-structs-to-functions)
    - [C/C++ inter-op for handles](#cc-inter-op-for-handles)
  - [Flags](#flags)
  - [Error-handling](#error-handling)
    - [Exceptions](#exceptions)
- [C++17: `[[nodiscard]]`](#c17-nodiscard)
- [Enumerations](#enumerations)
- [UniqueHandle for automatic resource management](#uniquehandle-for-automatic-resource-management)
- [SharedHandle](#sharedhandle)
- [RAII-classes](#raii-classes)
- [Custom allocators](#custom-allocators)
- [Custom assertions](#custom-assertions)
- [Extensions / Per Device function pointers](#extensions--per-device-function-pointers)
- [Type traits](#type-traits)
- [vk::Format trait functions](#vkformat-trait-functions)
- [Hashing Vulkan types](#hashing-vulkan-types)
- [Extension Inspection](#extension-inspection)
- [C++20 named module](#c20-named-module)
  - [Compiler support](#compiler-support)
    - [Usage with CMake](#usage-with-cmake)
    - [Command-line usage](#command-line-usage)
- [Samples and Tests](#samples-and-tests)
- [Compile time issues](#compile-time-issues)
- [Strict aliasing issue](#strict-aliasing-issue)

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

```c++
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

```c++
vk::Image image = device.createImage({{}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
                                     { width, height, 1 },
                                     1, 1, vk::SampleCountFlagBits::e1,
                                     vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment,
                                     vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined});
```

#### Designated initialisers

>[!NOTE]
> This feature requires a compiler supporting at least C++20, and must be **enabled** with the `VULKAN_HPP_NO_CONSTRUCTORS` or `VULKAN_HPP_NO_STRUCT_CONSTRUCTORS` macro.
> This will disable the above-mentioned generated constructors for structs.

C++20 and later versions support [designated initializers](https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers) for aggregate types.
Designated initialisers allow for more expressive code: member variable names are mentioned immediately in plain-text, without needing any static analysis.
The first few lines of Vulkan initialisation might look like the following:

```c++
// initialize the vk::ApplicationInfo structure
vk::ApplicationInfo applicationInfo{ .pApplicationName   = AppName,
                                     .applicationVersion = 1,
                                     .pEngineName        = EngineName,
                                     .engineVersion      = 1,
                                     .apiVersion         = vk::ApiVersion11 };

// initialize vk::InstanceCreateInfo
vk::InstanceCreateInfo instanceCreateInfo{ .pApplicationInfo = &applicationInfo };
```

instead of

```c++
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

<!-- TODO: uncomment with more detail. Fill in when ArrayProxyNoTemporaries is typically expected; function out-parameters? Ask @asuessenbach -->
<!-- Note that `vk::ArrayProxyNoTemporaries<T>` **disallows** temporary values, which may be important for out-parameters. -->

For example, consider the following calls to `vk::CommandBuffer::setScissor`:

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

#### Builder pattern with setters

>[!NOTE]
> This feature may be **disabled** with the `VULKAN_HPP_NO_STRUCT_SETTERS` macro.

Having _no_ constructors means losing a little flexibility compared to the default. For instance, [`ArrayProxy`](#arrayproxyt-and-arrayproxynotemporariest) parameters cannot be used with designated initializers, which fall back to (count, pointer) pairs.

Instead, Vulkan-Hpp provides setter member functions for _all_ member variables, and (count, pointer) pairs are generalised to either `ArrayProxy` or `ArrayProxyNoTemporaries` for all structs.
Combining these and [designated initialisers](#designated-initialisers) might allow for the best balance of readability and ease-of-use, since designated initialisers can be omitted for fields that are irrelevant.
For instance, `vk::ImageCreateInfo` may be constructed like so:

```c++
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

```c++
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
The `pNext` field of each structure is populated automatically.

```c++
vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedAllocateInfo> c = {
  vk::MemoryAllocateInfo(size, type),
  vk::MemoryDedicatedAllocateInfo(image)
};
```
<!-- TODO: need an example here. Also more info and examples: what about the same type, repeated? I don't use structure chains. @asuessenbach? -->
To remove a link in a `StructureChain`, use the function `vk::StructureChain::unlink<Type>()`.
This modifies the chain in-place by modifying the `.pNext` structures, and returns the unlinked structure; note that its memory layout remains unchanged.
To re-insert the same structure into the chain, use `vk::StructureChain::relink<Type>()`.

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

### Handles and functions

<!-- TODO: maybe move this to ./Handles.md? -->

Vulkan-Hpp provides a `vk::` class for each Vulkan handle, such as `vk::Instance`, `vk::Device`, `vk::Buffer`, and so on.
These classes are binary-compatible with their corresponding C handles; they can be cast back and forth without any overhead.

Member functions are also defined corresponding to a C API function which accepts the parent handle as the **first** parameter.
For example, instead of `vkBindBufferMemory(device, ...)` write `device.bindBufferMemory(...)`.

>[!NOTE]
> Note that these handles do **not** support RAII; their lifetimes need to be manually managed, just like in the C API.
> For automatic resource management, refer to [Handles](./Handles.md).

#### Passing structs to functions

<!-- TODO: this section seems poorly worded and titled. -->

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

#### C/C++ inter-op for handles

On 64-bit platforms, Vulkan-Hpp supports implicit conversions between handles provided by the C API and Vulkan-Hpp. On 32-bit platforms, all non-dispatchable handles are defined as `uint64_t`, thus preventing type-conversion checks at compile time which would catch assignments between incompatible handle types. Therefore, Vulkan-Hpp does not enable implicit conversion for 32-bit platforms by default and it is recommended to use a `static_cast`: `VkImage = static_cast<VkImage>(cppImage)` to prevent unintended conversions of untyped integers to handles.

If you are developing on a 64-bit platform but want to compile for a 32-bit platform without adding these verbose explicit casts, define the macro `VULKAN_HPP_TYPESAFE_CONVERSION=1` in your build system, or before writing `#include <vulkan/vulkan.hpp>`. On 64-bit platforms this macro is set to `1` by default and can be set to `0` to disable implicit conversions.

### Flags

Scoped enums add type safety to Vulkan flags, but also prevent bitwise operations with these flag bits.

As a solution, Vulkan-Hpp provides a class template, `vk::Flags<>`. This class is default-initialised to zero, and behaves exactly like a normal bitmask; however, the type safety ensures that it is impossible to set unrelated bits, with errors emitted at compile time.

For example:

```c++
vk::ImageUsageFlags iu1; // Initialize a bitmask with no bit set
vk::ImageUsageFlags iu2 = {}; // Default-initialize with no bit set
vk::ImageUsageFlags iu3 = vk::ImageUsageFlagBits::eColorAttachment; // initialize with a single value
vk::ImageUsageFlags iu4 = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage; // or two bits to get a bitmask
vk::ImageUsageFlags iu5 = iu3 | vk::PipelineStageFlagBits::eVertexShader; // Compile-time error
PipelineShaderStageCreateInfo ci({} /* pass a flag without any bits set */, ...);
```

### Error-handling

#### Exceptions

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

## C++17: `[[nodiscard]]`

With C++17 and above, some functions are attributed with `[[nodiscard]]`, resulting in a warning if you don't use the return value in any way. You can switch those warnings off by defining `VULKAN_HPP_NO_NODISCARD_WARNINGS`.

## Enumerations

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

## UniqueHandle for automatic resource management

Vulkan-Hpp provides a `vk::UniqueHandle<Type, Deleter>` interface. For each Vulkan handle type `vk::Type` there is a unique handle `vk::UniqueType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::UniqueBuffer` is the unique handle for `vk::Buffer`.

For each function which constructs a Vulkan handle of type `vk::Type` Vulkan-Hpp provides a second version which returns a `vk::UniqueType`. E.g. for `vk::Device::createBuffer` there is `vk::Device::createBufferUnique` and for `vk::allocateCommandBuffers` there is `vk::allocateCommandBuffersUnique`.

Note that using `vk::UniqueHandle` comes at a cost since most deleters have to store the `vk::AllocationCallbacks` and parent handle used for construction because they are required for automatic destruction.

## SharedHandle

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

## RAII-classes

In addition to `vk::UniqueHandles` and `vk::SharedHandles`, there's a set of wrapper classes for all the handle types that follow the RAII-paradigm (resource acquisition is initialization), provided in the `vk::raii` namespace.

While a `vk::UniqueHandle` mimics a handle wrapped by a `std::unique_ptr`, and a `vk::SharedHandle` mimics a handle wrapped by a `std::shared_ptr`, including parent information, a `vk::raii::Handle` is just a class that acquires the underlying vk-handle in its constructor and releases it in its destructor. Thus, you're free to use them as values or wrap them with some smart pointer.

Other than the `vk::Handles`, all those handle wrapper classes need to hold additional data, and thus are not binary identical with the vulkan C-handles.

As the `vk::UniqueHandles` and the `vk::SharedHandles` use the same dispatcher as the `vk::Handles`, they can be easily mixed-and-matched. The `vk::raii::Handles` use some slightly different dispatchers and thus are not compatible with the other handles! That is, for the `vk-Handles`, the `vk::UniqueHandles`, and the `vk::SharedHandles`, you need to instantiate a global dispatcher as described in <https://github.com/KhronosGroup/Vulkan-Hpp#extensions--per-device-function-pointers>. For the `vk::raii-Handles`, this is not needed, as they maintain their own dispatchers. The big advantage here is when you have multiple devices: the functions called via the `vk::raii-Handles` always call the device specific functions.

## Custom allocators

Sometimes it is required to use `std::vector` with custom allocators. Vulkan-Hpp supports vectors with custom allocators as input for `vk::ArrayProxy` and for functions which do return a vector. For the latter case, add your favorite custom allocator as template argument to the function call like this:

```c++
std::vector<LayerProperties, MyCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties<MyCustomAllocator>();
```

You can also use a stateful custom allocator by providing it as an argument to those functions. Unfortunately, to make the compilers happy, you also need to explicitly set the Dispatch argument. To get the default there, a simple `{}` would suffice:

```c++
MyStatefulCustomAllocator allocator;
std::vector<LayerProperties, MyStatefulCustomAllocator> properties = physicalDevice.enumerateDeviceLayerProperties(allocator, {});
```

## Custom assertions

All over `vulkan.hpp`, there are a couple of calls to an assert function. By defining `VULKAN_HPP_ASSERT`, you can specifiy your own custom assert function to be called instead.

By default, `VULKAN_HPP_ASSERT_ON_RESULT` will be used for checking results when `VULKAN_HPP_NO_EXCEPTIONS` is defined. If you want to handle errors by yourself, you can disable/customize it just like `VULKAN_HPP_ASSERT`.

There are a couple of static assertions for each handle class and each struct in the file [`vulkan_static_assertions.hpp`](vulkan/vulkan_static_assertions.hpp). You might include that file in at least one of your source files. By defining `VULKAN_HPP_STATIC_ASSERT`, you can specify your own custom static assertion to be used for those cases. That is, by defining it to be a NOP, you can reduce your compilation time a little.

## Extensions / Per Device function pointers

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

1. initialize it with a `vk::Instance` to get all the other function pointers:

```c++
    vk::Instance instance = vk::createInstance({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
```

1. optionally initialize it with a `vk::Device` to get device-specific function pointers

```c++
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    assert(!physicalDevices.empty());
    vk::Device device = physicalDevices[0].createDevice({}, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
```

After the second step above, the dispatcher is fully functional. Adding the third step can potentially result in more efficient code. But if you intend to use multiple devices, you could just omit that third step and let the driver do the device-dispatching.

In some cases the storage for the `vk::detail::DispatchLoaderDynamic` should be embedded in a DLL. For those cases you need to define `VULKAN_HPP_STORAGE_SHARED` to tell Vulkan-Hpp that the storage resides in a DLL. When compiling the DLL with the storage it is also required to define `VULKAN_HPP_STORAGE_SHARED_EXPORT` to export the required symbols.

For all functions, that `VULKAN_HPP_DEFAULT_DISPATCHER` is the default for the last argument to that function. If you want to explicitly provide the dispatcher for each and every function call (when you have multiple dispatchers for different devices, for example) and you want to make sure, that you don't accidentally miss any function call, you can define `VULKAN_HPP_NO_DEFAULT_DISPATCHER` before you include `vulkan.hpp` to remove that default argument.

## Type traits

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

## vk::Format trait functions

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

## Hashing Vulkan types

With the additional header [`vulkan_hash.hpp`](vulkan/vulkan_hash.hpp), you get specializations of `std::hash` for the handle wrapper classes and, with C++14, for the structure wrappers. With `VULKAN_HPP_HASH_COMBINE`, you can define your own hash combining algorithm for the structure elements.

## Extension Inspection

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

## C++20 named module

Vulkan-Hpp provides a [C++ named module](https://en.cppreference.com/w/cpp/language/modules), `vulkan_hpp` in [`vulkan.cppm`](../vulkan/vulkan.cppm).
C++ modules are intended to supersede header files. Modules tend to considerably improve compile times, as declarations and definitions may be easily shared across translation units without repeatedly parsing headers.
This is particularly applicable to Vulkan-Hpp, as the generated headers can be very long.

Users who are able to upgrade to a recent toolchain (detailed below) are recommended to use the module instead of the headers, and report bugs and issues.

Vulkan-Hpp provides a [C++ named module](https://en.cppreference.com/w/cpp/language/modules), `vulkan` in [`vulkan.cppm`](vulkan/vulkan.cppm).
C++ modules are intended to supersede header files. Modules have potential to drastically improve compilation times for large projects, as declarations and definitions may be easily shared across translation units without repeatedly parsing headers.
Vulkan-Hpp has some extremely long headers (e.g. [`vulkan_structs.hpp`](vulkan/vulkan_structs.hpp)), and the C++ module is likely to shorten compile times for projects currently using it.

### Compiler support

The named module requires a recent compiler with complete C++23 support:

- MSVC compiler version 19.40 or later (as reported by `cl.exe`)
- Clang 19.0.0 or later
- GCC 15.1 or later

If you intend to use CMake's C++ module support (and possibly Ninja), then more recent tools are required:

- Visual Studio 2022 17.4 or later (providing `cl.exe` 19.34 or later)
- Clang 17.0.0 or later
- GCC 14.0 or later
- CMake 3.28 or later
- Ninja 1.10.2 or later

> [!WARNING]
> The Vulkan-Hpp C++ named module is still experimental. Some suggested ways to use it in your projects are below. The long-term goal is to submit patches to the CMake [`FindVulkan`](https://cmake.org/cmake/help/latest/module/FindVulkan.html) module so that users may transparently configure the named module, without needing to declare it as an additional library in consumer CMake code.

#### Usage with CMake

CMake is recommended for use with the Vulkan-Hpp named module, as it provides a convenient platform-agnostic way to configure your project.
CMake version 3.28 or later is required to support C++ modules. Refer to the [CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html) on the topic.

CMake provides the [FindVulkan module](https://cmake.org/cmake/help/latest/module/FindVulkan.html), which may be used to source the Vulkan SDK and Vulkan headers on your system.

```cmake# find Vulkan SDK
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

```cmake# Disable exceptions, disable smart handles, disable constructors
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
)# ...
target_link_libraries( YourProject PRIVATE VulkanHppModule )
```

Furthermore, you may also prefer linking `VulkanHppModule` to just the `Vulkan::Headers` target with the `PUBLIC` scope instead of `Vulkan::Vulkan`, so that the `vulkan-1` library is not linked in, and the Vulkan headers are available to your consuming project, as detailed further below.

```cmake
target_link_libraries( VulkanHppModule PUBLIC Vulkan::Headers )
```

Finally, supply the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in your source code, just as in the non-module case. In order to have that macro available, include [`vulkan_hpp_macros.hpp`](vulkan/vulkan_hpp_macros.hpp), a lightweight header providing all Vulkan-Hpp related macros and defines. And as explained above, you need to initialize that dispatcher in two or three steps:

```cpp
import vulkan;

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

#### Command-line usage

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

- [MSVC](https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp?view=msvc-170)
- [Clang](https://clang.llvm.org/docs/StandardCPlusPlusModules.html)

## Samples and Tests

When you configure your project using CMake, you can enable SAMPLES_BUILD to add some sample projects to your solution. Most of them are ports from the LunarG samples, but there are some more, like CreateDebugUtilsMessenger, InstanceVersion, PhysicalDeviceDisplayProperties, PhysicalDeviceExtensions, PhysicalDeviceFeatures, PhysicalDeviceGroups, PhysicalDeviceMemoryProperties, PhysicalDeviceProperties, PhysicalDeviceQueueFamilyProperties, and RayTracing. All those samples should just compile and run.
When you configure your project using CMake, you can enable TESTS_BUILD to add some test projects to your solution. Those tests are just compilation tests and are not required to run.

## Compile time issues

As `vulkan.hpp` is pretty big, some compilers might need some time to digest all that stuff. In order to potentially reduce the time needed to compile that header, a couple of defines will be introduced, that allow you to hide certain features. Whenever you don't need that corresponding feature, defining that value might improve your compile time.
Currently, there are just a couple of such defines:

- `VULKAN_HPP_NO_SPACESHIP_OPERATOR`, which removes the spaceship operator on structures (available with C++20)
- `VULKAN_HPP_NO_TO_STRING`, which removes the various `vk::to_string` functions on enums and bitmasks.
- `VULKAN_HPP_USE_REFLECT`, this one needs to be defined to use the reflection function on structures. It's very slow to compile, though!

## Strict aliasing issue

As Vulkan-Hpp often needs to switch between C++ vk-types and corresponding bit-identical C-types, using `reinterpret_cast`, it is highly recommended to use the compile option `-fno-strict-aliasing` to prevent potentially breaking compile optimizations.
