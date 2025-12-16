# Building

## Minimum requirements

The generator, samples, and tests requires a C++11 compiler. The following compilers are known to work, and tested during releases:

- Visual Studio ≥ 2015
- GCC ≥ 4.8.2
- Clang ≥ 3.3

> [!NOTE]
> Compiling the C++ named module requires the most recent compiler toolchains possible.
> This is detailed in the corresponding [section](./Usage.md/#c20-named-module).

## Build steps

1. Install dependencies:
   - Install CMake and git; ensure both are available from a shell.
   - Install the LunarG Vulkan SDK.
   - Optionally install `clang-format` with version at least 11.0 so that any generated headers can be correctly formatted.
2. In a shell, clone the repository:

    ```git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Hpp.git```
3. Change the shell's working directory to the newly created `Vulkan-Hpp` directory.
4. Create a build environment with CMake:
    ```cmake -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build```

    You may need to specify a generator via `-G`; for a full list of generators execute `cmake -G`.

    To regenerate `vulkan.hpp` from the `vk.xml` XML registry file, add the
       `-DVULKAN_HPP_RUN_GENERATOR=ON` option to the CMake command line.
5. The generated project may be opened with an IDE like Visual Studio or CLion, or the various targets may be built directly from a shell:

    ```cmake --build build --parallel```

## Additional features

### Formatting

If a `clang-format` executable is found by CMake, the define `CLANG_FORMAT_EXECUTABLE` is set accordingly. In that case, the generated headers are formatted using the `.clang-format` file located in the root directory of this project; otherwise, the formatting is left as hard-coded in the generator.

### Custom views of Vulkan-Hpp objects in Visual Studio

[`VulkanHpp.natvis`](../VulkanHpp.natvis) provides a custom view over `vk::Flags` for Visual Studio. Add this file to the directory of your Visual Studio installation (`%USERPROFILE%\Documents\Visual Studio 2022\Visualizers`). `vk::Flags` will be appropriately formatted in the debugger.
