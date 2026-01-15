# Handles in Vulkan-Hpp

The default handle types in Vulkan-Hpp are thin wrappers around the Vulkan C handles. They provide type safety and convenience functions, but do not manage the lifetime of the underlying Vulkan resources.
To facilitate automatic resource management, Vulkan-Hpp provides two additional handle types: `vk::UniqueHandle` and `vk::SharedHandle`. Additionally, there are RAII-style classes in the `vk::raii` namespace that encapsulate resource management.

- [`vk::UniqueHandle`](#vkuniquehandle)
- [`vk::SharedHandle`](#vksharedhandle)
- [`vk::raii`](#vkraii)

## `vk::UniqueHandle`

Vulkan-Hpp provides a `vk::UniqueHandle<Type, Deleter>` interface. For each Vulkan handle type `vk::Type` there is a unique handle `vk::UniqueType` which will delete the underlying Vulkan resource upon destruction, e.g. `vk::UniqueBuffer` is the unique handle for `vk::Buffer`.

For each function which constructs a Vulkan handle of type `vk::Type` Vulkan-Hpp provides a second version which returns a `vk::UniqueType`. E.g. for `vk::Device::createBuffer` there is `vk::Device::createBufferUnique` and for `vk::allocateCommandBuffers` there is `vk::allocateCommandBuffersUnique`.

Note that using `vk::UniqueHandle` comes at a cost since most deleters have to store the `vk::AllocationCallbacks` and parent handle used for construction because they are required for automatic destruction.

## `vk::SharedHandle`

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

## `vk::raii`

In addition to `vk::UniqueHandles` and `vk::SharedHandles`, there's a set of wrapper classes for all the handle types that follow the RAII-paradigm (resource acquisition is initialization), provided in the `vk::raii` namespace.

While a `vk::UniqueHandle` mimics a handle wrapped by a `std::unique_ptr`, and a `vk::SharedHandle` mimics a handle wrapped by a `std::shared_ptr`, including parent information, a `vk::raii::Handle` is just a class that acquires the underlying vk-handle in its constructor and releases it in its destructor. Thus, you're free to use them as values or wrap them with some smart pointer.

Other than the `vk::Handles`, all those handle wrapper classes need to hold additional data, and thus are not binary identical with the vulkan C-handles.

As the `vk::UniqueHandles` and the `vk::SharedHandles` use the same dispatcher as the `vk::Handles`, they can be easily mixed-and-matched. The `vk::raii::Handles` use some slightly different dispatchers and thus are not compatible with the other handles! That is, for the `vk-Handles`, the `vk::UniqueHandles`, and the `vk::SharedHandles`, you need to instantiate a global dispatcher as described in <https://github.com/KhronosGroup/Vulkan-Hpp#extensions--per-device-function-pointers>. For the `vk::raii-Handles`, this is not needed, as they maintain their own dispatchers. The big advantage here is when you have multiple devices: the functions called via the `vk::raii-Handles` always call the device specific functions.
