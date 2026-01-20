# Building

## Minimum requirements

The generator requires a toolchain that supports C++20; the samples and tests may be compiled with C++11. The following compilers are known to work, and tested during releases:

- Visual Studio ≥ 2015
- GCC ≥ 4.8.2
- Clang ≥ 3.3

> [!NOTE]
> Compiling the C++ named module requires the most recent compiler toolchains possible.
> This is detailed in the corresponding [section](./Usage.md/#c20-named-module).

## Build steps

1. Install dependencies:
   - Install CMake and git; ensure both are available from a shell.
   - Optionally install `clang-format` with version at least 11.0 so that any generated headers can be correctly formatted.
   - Additional dependencies are required to build the samples on Linux.
     On Ubuntu, install them with:

     ```shell
     sudo apt update && sudo apt install install lsb-release gpg software-properties-common wget
     ```

2. In a shell, clone the repository:

    ```git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Hpp.git```
3. Change the shell's working directory to the newly created `Vulkan-Hpp` directory.
4. Create a build environment with CMake:
    ```cmake -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_TESTS_BUILD=ON -B build```

    You may need to specify a generator via `-G`; for a full list of generators execute `cmake -G`.

    To regenerate `vulkan.hpp` from the `vk.xml` XML registry file, add the
       `-DVULKAN_HPP_RUN_GENERATOR=ON` option to the CMake command line.
5. The generated project may be opened with an IDE like Visual Studio or CLion, or the various targets may be built directly from a shell:

    ```cmake --build build --parallel```

## Additional notes

### Formatting

If a `clang-format` executable is found by CMake, the variable `CLANG_FORMAT_EXECUTABLE` is set accordingly. In that case, the generated headers are formatted using the `.clang-format` file located in the root directory of this project; otherwise, the formatting is left as hard-coded in the generator.

When submitting merge requests that modify the generated files in [`vulkan`](../vulkan/), ensure `clang-format` version 21.1.0 is in the `PATH` or specify its location at configure time (command-line, preset file, or shell script) with `CLANG_FORMAT_EXECUTABLE`.
This ensures that the generated headers and module interfaces are consistently formatted.

### Custom views of Vulkan-Hpp objects in Visual Studio

[`VulkanHpp.natvis`](../VulkanHpp.natvis) provides a custom view over `vk::Flags` for Visual Studio. Add this file to the directory of your Visual Studio installation (`%USERPROFILE%\Documents\Visual Studio 2022\Visualizers`). `vk::Flags` will be appropriately formatted in the debugger.
