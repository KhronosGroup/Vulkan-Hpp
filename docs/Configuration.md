# Configuration Options

Vulkan-Hpp has several configurable options and features for consumer projects.
These are exposed as the following compiler macros, which be defined in a build system, or in the command-line invocation of a compiler.

- [`VULKAN_HPP_ASSERT`](#vulkan_hpp_assert)
- [`VULKAN_HPP_ASSERT_ON_RESULT`](#vulkan_hpp_assert_on_result)
- [`VULKAN_HPP_DEFAULT_DISPATCHER`](#vulkan_hpp_default_dispatcher)
- [`VULKAN_HPP_DEFAULT_DISPATCHER_TYPE`](#vulkan_hpp_default_dispatcher_type)
- [`VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`](#vulkan_hpp_default_dispatch_loader_dynamic_storage)
- [`VULKAN_HPP_DISABLE_ENHANCED_MODE`](#vulkan_hpp_disable_enhanced_mode)
- [`VULKAN_HPP_DISPATCH_LOADER_DYNAMIC`](#vulkan_hpp_dispatch_loader_dynamic)
- [`VULKAN_HPP_DISPATCH_LOADER_DYNAMIC_TYPE`](#vulkan_hpp_dispatch_loader_dynamic_type)
- [`VULKAN_HPP_DISPATCH_LOADER_STATIC_TYPE`](#vulkan_hpp_dispatch_loader_static_type)
- [`VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL`](#vulkan_hpp_enable_dynamic_loader_tool)
- [`VULKAN_HPP_EXPECTED`](#vulkan_hpp_expected)
- [`VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC`](#vulkan_hpp_flags_mask_type_as_public)
- [`VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS`](#vulkan_hpp_handle_error_out_of_date_as_success)
- [`VULKAN_HPP_HANDLES_MOVE_EXCHANGE`](#vulkan_hpp_handles_move_exchange)
- [`VULKAN_HPP_HASH_COMBINE`](#vulkan_hpp_hash_combine)
- [`VULKAN_HPP_INLINE`](#vulkan_hpp_inline)
- [`VULKAN_HPP_NAMESPACE`](#vulkan_hpp_namespace)
- [`VULKAN_HPP_NO_CONSTRUCTORS`](#vulkan_hpp_no_constructors)
- [`VULKAN_HPP_NO_EXCEPTIONS`](#vulkan_hpp_no_exceptions)
- [`VULKAN_HPP_NO_NODISCARD_WARNINGS`](#vulkan_hpp_no_nodiscard_warnings)
- [`VULKAN_HPP_NO_SETTERS`](#vulkan_hpp_no_setters)
- [`VULKAN_HPP_NO_SMART_HANDLE`](#vulkan_hpp_no_smart_handle)
- [`VULKAN_HPP_NO_SPACESHIP_OPERATOR`](#vulkan_hpp_no_spaceship_operator)
- [`VULKAN_HPP_NO_TO_STRING`](#vulkan_hpp_no_to_string)
- [`VULKAN_HPP_NO_WIN32_PROTOTYPES`](#vulkan_hpp_no_win32_prototypes)
- [`VULKAN_HPP_RAII_NO_EXCEPTIONS`](#vulkan_hpp_raii_no_exceptions)
- [`VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST`](#vulkan_hpp_smart_handle_implicit_cast)
- [`VULKAN_HPP_STORAGE_API`](#vulkan_hpp_storage_api)
- [`VULKAN_HPP_TYPESAFE_CONVERSION`](#vulkan_hpp_typesafe_conversion)
- [`VULKAN_HPP_UNEXPECTED`](#vulkan_hpp_unexpected)
- [`VULKAN_HPP_USE_REFLECT`](#vulkan_hpp_use_reflect)
- [`VULKAN_HPP_USE_STD_EXPECTED`](#vulkan_hpp_use_std_expected)

## `VULKAN_HPP_ASSERT`

At various places in `vulkan.hpp` an assertion statement is used. By default, the standard assert funtions from `<cassert>` is called. By defining `VULKAN_HPP_ASSERT` before including `vulkan.hpp`, you can change that to any function with the very same interface.

## `VULKAN_HPP_ASSERT_ON_RESULT`

If there are no exceptions enabled (see `VULKAN_HPP_NO_EXCEPTIONS`), an assertion statement checks for a valid success code returned from every vulkan call. By default, this is the very same assert function as defined by `VULKAN_HPP_ASSERT`, but by defining `VULKAN_HPP_ASSERT_ON_RESULT` you can replace just those assertions with your own function, using the very same interface.

## `VULKAN_HPP_DEFAULT_DISPATCHER`

Every vk-function gets a Dispatcher as its very last argument, which defaults to `VULKAN_HPP_DEFAULT_DISPATCHER`. If `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` is defined to be `1`, it is `vk::detail::defaultDispatchLoaderDynamic`. This in turn is the dispatcher instance, which is defined by `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`, which has to be used exactly once in your sources. If, on the other hand, `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` is defined to something different from `1`, `VULKAN_HPP_DEFAULT_DISPATCHER` is set to be `vk::detail::DispatchLoaderStatic()`.
You can use your own default dispatcher by setting `VULKAN_HPP_DEFAULT_DISPATCHER` to an object that provides the same API. If you explicitly set `VULKAN_HPP_DEFAULT_DISPATCHER`, you need to set `VULKAN_HPP_DEFAULT_DISPATCHER_TYPE` accordingly as well.

## `VULKAN_HPP_DEFAULT_DISPATCHER_TYPE`

This names the default dispatcher type, as specified by `VULKAN_HPP_DEFAULT_DISPATCHER`. Per default, it is `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC_TYPE` or `VULKAN_HPP_DISPATCH_LOADER_STATIC_TYPE`, depending on `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` being `1` or not `1`, respectively. If you explicitly set `VULKAN_HPP_DEFAULT_DISPATCHER`, you need to set `VULKAN_HPP_DEFAULT_DISPATCHER_TYPE` accordingly as well.

## `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`

If you have not defined your own `VULKAN_HPP_DEFAULT_DISPATCHER`, and have `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC` defined to be `1` (the default), you need to have the macro `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE` exactly once in any of your source files to provide storage for that default dispatcher. `VULKAN_HPP_STORAGE_API` then controls the import/export status of that default dispatcher.

> [!NOTE]
> The [C++ named module](./Usage.md#c20-named-module) declares its own storage for the dynamic dispatcher, so this macro is not needed.

## `VULKAN_HPP_DISABLE_ENHANCED_MODE`

When this is defined before including `vulkan.hpp`, you essentially disable all enhanced functionality. All you then get is:

- improved compile time error detection, via scoped enums;
- usage of the helper class `vk::Flags` for bitmasks;
- wrapper structs for all vulkan structs providing default initialization;
- the helper class `vk::StructureChain` for compile-time construction of structure chains.

If this is not defined, you additionally get:

- enhanced versions of the commands (consuming `vk::ArrayProxy<>`), simplifying handling of array data; returning requested data; throwing exceptions on errors (as long as `VULKAN_HPP_NO_EXCEPTIONS` is not defined);
- enhanced structure constructors (as long as `VULKAN_HPP_NO_STRUCT_CONSTRUCTORS` is not defined) (consuming `vk::ArrayProxyNoTemporaries<>`);
- enhanced setter functions on some members of structs (consuming `vk::ArrayProxyNoTemporaries<>`);
- the helper classes `vk::ArrayProxy<>` and `vk::ArrayProxyNoTemporaries<>`
- all the RAII-stuff in `vulkan_raii.hpp`

## `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC`

This either selects the dynamic (when it's `1`) or the static (when it's not `1`) DispatchLoader as the default one, as long as it's not explicitly specified by `VULKAN_HPP_DEFAULT_DISPATCHER`. By default, this is defined to be `1` if `VK_NO_PROTOTYPES` is defined, otherwise `0`.

## `VULKAN_HPP_DISPATCH_LOADER_DYNAMIC_TYPE`

The type of the dynamic dispatch loader. By default, it's vk::detail::DispatchLoaderDynamic. You can define this before including vulkan.hpp to use your own dynamic dispatcher.

## `VULKAN_HPP_DISPATCH_LOADER_STATIC_TYPE`

The type of the static dispatch loader. By default, it's vk::detail::DispatchLoaderStatic. You can define this before including vulkan.hpp to use your own static dispatcher.

## `VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL`

By default, a little helper class `vk::detail::DynamicLoader` is used to dynamically load the vulkan library. If you set it to something different than `1` before including `vulkan.hpp`, this helper is not available, and you need to explicitly provide your own loader type for the function `vk::detail::DispatchLoaderDynamic::init()`.

## `VULKAN_HPP_EXPECTED`

When this is not externally defined and `VULKAN_HPP_CPP_VERSION` is at least `23`, `VULKAN_HPP_EXPECTED` is defined to be `std::expected`, and `VULKAN_HPP_UNEXPECTED` is defined to be `std::unexpected`.

## `VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC`

By default, the member `m_mask` of the `Flags` class template is private. This is to prevent accidentally setting a `Flags` with some inappropriate value. But it also prevents using a `Flags`, or a structure holding a `Flags`, to be used as a non-type template parameter. If you really need that functionality, and accept the reduced security, you can use this define to change the access specifier for `m_mask` from private to public, which allows using a `Flags` as a non-type template parameter.

## `VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS`

By default, `VK_ERROR_OUT_OF_DATE_KHR` is an error code. To prevent throwing an exception or, in case exceptions are turned off via `VULKAN_HPP_NO_EXCEPTIONS`, firing an assertion on this return value, you can define `VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS`. `VK_ERROR_OUT_OF_DATE_KHR` will then be handled like a success code.

## `VULKAN_HPP_HANDLES_MOVE_EXCHANGE`

This define can be used to enable `m_handle = exchange( rhs.m_handle, {} )` in move constructors of Vulkan-Hpp handles, which default-initializes the `rhs` underlying value. By default Vulkan-Hpp handles behave like trivial types -- move constructors copying value.

## `VULKAN_HPP_HASH_COMBINE`

This define can be used to specify your own hash combiner function. In order to determine the hash of a vk-structure, the hashes of the members of that struct are to be combined. This is done by this define, which by default is identical to what the function `boost::hash_combine()` does. It gets the type of the to-be-combined value, the seed, which is the combined value up to that point, and finally the to-be-combined value. This hash calculation determines a "shallow" hash, as it takes the hashes of any pointer in a structure, and not the hash of a pointed-to value.

## `VULKAN_HPP_INLINE`

This is set to be the compiler-dependent attribute used to mark functions as inline. If your compiler happens to need some different attribute, you can set this define accordingly before including `vulkan.hpp`.

## `VULKAN_HPP_NAMESPACE`

By default, the namespace used with `vulkan.hpp` is `vk`. By defining `VULKAN_HPP_NAMESPACE` before including `vulkan.hpp`, you can adjust this.

## `VULKAN_HPP_NO_CONSTRUCTORS`

With C++20, designated initializers are available. Their use requires the absence of any user-defined constructors. Define `VULKAN_HPP_NO_CONSTRUCTORS` to remove constructors from structs and unions.

## `VULKAN_HPP_NO_EXCEPTIONS`

When a vulkan function returns an error code that is not specified to be a success code, an exception is thrown unless `VULKAN_HPP_NO_EXCEPTIONS` is defined before including `vulkan.hpp`.

## `VULKAN_HPP_NO_NODISCARD_WARNINGS`

With C++17, all vk-functions returning something are declared with the attribute `[[nodiscard]]`. This can be removed by defining `VULKAN_HPP_NO_NODISCARD_WARNINGS` before including `vulkan.hpp`.

## `VULKAN_HPP_NO_SETTERS`

By defining `VULKAN_HPP_NO_SETTERS` before including `vulkan.hpp`, setter member functions will not be available within structs and unions. Modifying their data members will then only be possible via direct assignment.

## `VULKAN_HPP_NO_SMART_HANDLE`

By defining `VULKAN_HPP_NO_SMART_HANDLE` before including `vulkan.hpp`, the helper class `vk::UniqueHandle` and all the unique handle types are not available.

## `VULKAN_HPP_NO_SPACESHIP_OPERATOR`

With C++20, the so-called spaceship-operator `<=>` is introduced. If that operator is supported, all the structs and classes in vulkan.hpp use the default implementation of it. As currently some implementations of this operator are very slow, and others seem to be incomplete, by defining `VULKAN_HPP_NO_SPACESHIP_OPERATOR` before including `vulkan.hpp` you can remove that operator from those structs and classes.

## `VULKAN_HPP_NO_TO_STRING`

By default, the file [`vulkan_to_string.hpp`](../vulkan/vulkan_to_string.hpp) is included by `vulkan.hpp` and provides functions `vk::to_string` for enums and bitmasks. If you don't need those functions, you can define `VULKAN_HPP_NO_TO_STRING` to prevent that inclusion. If you have certain files where you want to use those functions nevertheless, you can explicitly include `vulkan_to_string.hpp` there.

## `VULKAN_HPP_NO_WIN32_PROTOTYPES`

By default, if `VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL` is enabled on Win32, `vulkan.hpp` declares `HINSTANCE`, `LoadLibraryA`, and other required symbols. It could cause conflicts with the `Windows.h` alternatives, such as `WindowsHModular`.
With this define, you can disable these declarations, but you will have to declare them before the `vulkan.hpp` is included.

## `VULKAN_HPP_RAII_NO_EXCEPTIONS`

If both, `VULKAN_HPP_NO_EXCEPTIONS` and `VULKAN_HPP_EXPECTED` are defined, the vk::raii-classes don't throw exceptions. That is, the actual constructors are not available, but the creation-functions must be used. For more details have a look at the [`vk_raii_ProgrammingGuide.md`](vk_raii_ProgrammingGuide.md).

## `VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST`

Even though `vk::UniqueHandle` and `vk::SharedHandle` are semantically close to pointers, an implicit cast operator to the underlying `vk::Handle` might be handy. You can add that implicit cast operator by defining `VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST`.

## `VULKAN_HPP_STORAGE_API`

With this define you can specify whether the `vk::detail::DispatchLoaderDynamic` is imported or exported (see `VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE`). If `VULKAN_HPP_STORAGE_API` is not defined externally, and `VULKAN_HPP_STORAGE_SHARED` is defined, depending on the `VULKAN_HPP_STORAGE_SHARED_EXPORT` being defined, `VULKAN_HPP_STORAGE_API` is either set to `__declspec( dllexport )` (for MSVC) / `__attribute__( ( visibility( "default" ) ) )` (for gcc or clang) or `__declspec( dllimport )` (for MSVC), respectively. For other compilers, you might specify the corresponding storage by defining `VULKAN_HPP_STORAGE_API` on your own.

## `VULKAN_HPP_TYPESAFE_CONVERSION`

32-bit vulkan is not typesafe for non-dispatchable handles, so we don't allow copy constructors on this platform by default. To enable this feature on 32-bit platforms, `#define VULKAN_HPP_TYPESAFE_CONVERSION 1`. To disable this feature on 64-bit platforms, `#define VULKAN_HPP_TYPESAFE_CONVERSION 0`.

## `VULKAN_HPP_UNEXPECTED`

See [`VULKAN_HPP_EXPECTED`](#vulkan_hpp_expected).

## `VULKAN_HPP_USE_REFLECT`

With this define you can include a reflection mechanism on the vk-structures. It adds a function `reflect` that returns a tuple-version of the structure. That tuple then could easily be iterated. But at least for now, that feature takes lots of compile-time resources, so currently it is recommended to enable that feature only if you're willing to pay that price.

## `VULKAN_HPP_USE_STD_EXPECTED`

With this define, you can enable the use of `std::expected` for functions that return a value or a `vk::Result`. This requires C++23 or above.

See [`VULKAN_HPP_EXPECTED`](#vulkan_hpp_expected) and [`VULKAN_HPP_UNEXPECTED`](#vulkan_hpp_unexpected) for more details.
