# Handles in Vulkan-Hpp

The default handle types in Vulkan-Hpp are thin wrappers around the Vulkan C handles.
They provide type safety and convenience functions, but do not manage the lifetime of the underlying Vulkan resources.
To facilitate automatic resource management, Vulkan-Hpp provides two additional handle types: `vk::UniqueHandle` and `vk::SharedHandle`. Additionally, there are RAII-style classes in the `vk::raii` namespace that encapsulate resource management.

> [!NOTE]
> Note that none of the handles listed here are binary-compatible with the underlying Vulkan C handles.

- [`vk::UniqueHandle`](#vkuniquehandle)
- [`vk::SharedHandle`](#vksharedhandle)
- [`vk::raii`](#vkraii)

## `vk::UniqueHandle`

Vulkan-Hpp provides a `vk::UniqueHandle<Type, Deleter>` interface.
This is a smart pointer similar to `std::unique_ptr`, which ensures that the underlying Vulkan handle is automatically destroyed when the `vk::UniqueHandle` goes out of scope.

Vulkan-Hpp defines aliases to this template for each Vulkan handle type.
Each `vk::Type` (or `VkType` in the C interface), has a corresponding `vk::UniqueType`.
That is, `vk::UniqueBuffer` is the unique handle for `vk::Buffer`.
For each function that constructs a `vk::Type`, there is a corresponding function that constructs `vk::UniqueType`.
For example, `vk::Device::createBuffer` maps to `vk::Device::createBufferUnique`, and `vk::allocateCommandBuffers` maps to `vk::allocateCommandBuffersUnique`.

> [!NOTE]
> `vk::UniqueHandle` is _not_ a 'zero-cost abstraction'.
> Most deleters have to store `vk::AllocationCallbacks` and the parent handle used for construction, which are required for automatic destruction on scope exit.
> For example, `vk::UniqueBuffer` stores a reference to the `vk::Device` used to create the buffer.
>
> This implies additional memory overhead, and function pointer chain dereferencing during destruction.

## `vk::SharedHandle`

Vulkan-Hpp provides a `vk::SharedHandle<Type>` interface.
This is a smart pointer similar to `std::shared_ptr`, which ensures that the underlying Vulkan handle is automatically destroyed when the last `vk::SharedHandle` referencing it goes out of scope.
Vulkan-Hpp defines aliases to this template for each Vulkan handle type.
For each Vulkan handle type `vk::Type` there is a shared handle `vk::SharedType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::SharedBuffer` is the shared handle for `vk::Buffer`.

Unlike `vk::UniqueHandle`, `vk::SharedHandle` takes shared ownership of the resource as well as its parent.
This means that the parent handle will not be destroyed until all child resources are deleted.
For instance, if a `vk::SharedBuffer` is created with a `vk::SharedDevice` as its parent, the `vk::SharedDevice` will not be destroyed until all `vk::SharedBuffer` instances created from it are destroyed.
This is useful for resources that are shared between multiple threads or objects.

> [!WARNING]
> Shared handles are not thread-safe.
> Multi-threaded access to the same `vk::SharedHandle` instance must be synchronised by the user.

This mechanism ensures correct destruction order even if destruction of the parent `vk::SharedHandle` is attempted before that of its child handle.
It follows that a `vk::SharedInstance` will be the last object to be destroyed in a Vulkan application using `vk::SharedHandle`s.

Functions which directly construct a `vk::SharedHandle` have not yet been implemented.
Instead, construct a `vk::SharedHandle` from a `vk::Handle`:

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

Create a `vk::SharedHandle` overload for custom handle types or shared handles by providing several template arguments to `SharedHandleBase`:

- A handle type
- A parent handle type or a header structure, that contains the parent
- A class itself for CRTP

With this, provide a custom static destruction function `internalDestroy`, that takes in a parent handle and a handle to destroy.
Add a `friend` declaration for the base class.

```c++
// Example of a custom shared device, that accepts an instance as a parent
class MySharedHandle<VkDevice> : public vk::SharedHandleBase<VkDevice, vk::SharedInstance, MySharedHandle<VkDevice>>
{
  using Base = vk::SharedHandleBase<VkDevice, vk::SharedInstance, MySharedHandle<VkDevice>>;
  friend Base;

public:
  MySharedHandle() = default;
  explicit MySharedHandle(VkDevice handle, vk::SharedInstance parent) noexcept:
    Base(handle, std::move(parent)) {}

  [[nodiscard]]
  const vk::SharedInstance& getParent() const noexcept
  {
    return getHeader();
  }

protected:
  static void internalDestroy([[maybe_unused]] const vk::SharedInstance& control, VkDevice handle) noexcept
  {
    vkDestroyDevice(handle);
  }
};
```

Vulkan-Hpp will be extended to provide creation functions in the future.

## `vk::raii`

In addition to `vk::UniqueHandles` and `vk::SharedHandles`, Vulkan-Hpp provides types for Vulkan handles which follow the RAII (Resource Acquisition Is Initialization) idiom, in the `vk::raii` namespace.

While a `vk::UniqueHandle` mimics a handle wrapped by a `std::unique_ptr`, and a `vk::SharedHandle` mimics a handle wrapped by a `std::shared_ptr`, including parent information, a `vk::raii::Handle` is just a class that acquires the underlying C handle in its constructor and releases it in its destructor.
This allows for a more object-oriented approach to Vulkan resource management.

`vk::UniqueHandle`, `vk::SharedHandle`, and `vk::Handle` types all use the same dispatcher, and these can be straightforwardly mixed.
To use them, initialise a global dispatcher as described in [Usage](./Usage.md#extensions-and-per-device-function-pointers).

`vk::raii` types have a custom dispatcher and are _not_ compatible with the aforementioned types, and maintain their own dispatchers.
With multiple devices in the same application, this is very useful as `vk::raii` member function calls will always be device-specific.

Refer to the [detailed programming guide](./VkRaiiProgrammingGuide.md) for a tutorial on how to use `vk::raii`.
