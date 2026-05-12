<!--
SPDX-FileCopyrightText: 2026 The Khronos Group, Inc.
SPDX-License-Identifier: Apache-2.0
-->

# Vulkan-Hpp: C++ bindings for Vulkan

- [Overview](#overview)
- [CI/CD status](#cicd-status)
- [Documentation](#documentation)
- [Installation and usage](#installation-and-usage)
  - [Vulkan SDK](#vulkan-sdk)
  - [vcpkg](#vcpkg)
  - [Conan](#conan)
- [Breaking Changes](#breaking-changes)
  - [v1.4.334](#v14334)
  - [v1.4.324](#v14324)
  - [v1.4.329](#v14329)
- [Contributing](#contributing)
- [License](#license)

## Overview

Vulkan-Hpp provides header-only C++ bindings for the Vulkan C API to improve the developer experience with Vulkan without introducing run-time CPU costs.
It adds features like type safety for enumerations and bit-fields, STL container support, exception support, and several varieties of RAII-capable types.

This repository contains the [generators](./generator/) for Vulkan-Hpp, which accept the [XML specification of Vulkan](https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/refs/heads/main/xml/vk.xml) and emit C++ bindings, which are documented further below.

## CI/CD status

We test a wide range of compilers across Windows, Linux and MacOS (via [MoltenVK](https://github.com/KhronosGroup/MoltenVK)).
Below is a matrix representing all of the runner permutations used for automated testing.
For more details, check out the [workflow](.github/workflows/build.yml) that governs these runners.

| [![CI Status](https://github.com/KhronosGroup/Vulkan-Hpp/actions/workflows/build.yml/badge.svg)](.github/workflows/build.yml) | GCC | Clang | MSVC |
| :--- | :---: | :---: | :---: |
| **Windows 2022** <br> **Windows 2025**            | - <br> - | 17 <br> 18 | VS 17 2022 <br> VS 18 2026 |
| **Ubuntu 22.04** <br> **Ubuntu 24.04**            | 10, 11 <br> 12, 13, 14 | 13, 14, 15 <br> 16, 17, 18 | - <br> - |
| **MacOS 14** <br> **MacOS 15** <br> **MacOS 26**  | - <br> - <br> - | 15 <br> 17 <br> 17 | - <br> - <br> - |

## Documentation

All other documentation is in [`docs`](./docs/):

1. [Building](./docs/Building.md) describes how to configure, build, and generate new headers.
2. [Usage](./docs/Usage.md) explains how to use the various features of Vulkan-Hpp with detailed examples.
3. [Configuration](./docs/Configuration.md) lists all the options available to configure the behaviour and features of Vulkan-Hpp.
4. [Handles](./docs/Handles.md) is an overview of the three different families of handles provided by Vulkan-Hpp. These allow semantics similar to `std::unique_ptr`, `std::shared_ptr`, and also `vk::raii` types, which are [RAII](https://en.cppreference.com/w/cpp/language/raii.html) (resource acquisition is initialisation) handles that offer object-oriented semantics for Vulkan handles.

> [!NOTE]
>
> All macro configuration options [available in the C API](https://docs.vulkan.org/spec/latest/appendices/boilerplate.html) are also available and used in Vulkan-Hpp.

## Installation and usage

### Vulkan SDK

Vulkan-Hpp has been part of the [LunarG Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) since version 1.0.24; this remains the recommended installation method.
If you need a more recent version than the SDK supports, then Vulkan-Hpp is also distributed as part of the [Khronos Group Vulkan-Headers repository](https://github.com/KhronosGroup/Vulkan-Headers).

### vcpkg

As above, Vulkan-Hpp is provided in the [`vulkan-headers`](https://vcpkg.io/en/package/vulkan-headers) port in [vcpkg](https://github.com/Microsoft/vcpkg).
Otherwise, you can also install the [`vulkan-sdk-components`](https://vcpkg.io/en/package/vulkan-sdk-components) port, which is a vcpkg equivalent to the LunarG-distributed Vulkan SDK.

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install vulkan-headers
```

For support, [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.

### Conan

Similar to vcpkg, Vulkan-Hpp can be installed as part of the [`vulkan-headers`](https://conan.io/center/recipes/vulkan-headers) recipe.

The vcpkg and Conan packages are kept up to date by Microsoft and members of the community.
Requests for updates and issues with these packages should be directed to their respective repositories rather than here.

## Breaking Changes

We try to keep the API for all flavours of Vulkan-Hpp constant or backwards compatible. However, we may introduce unavoidable breaking changes, usually to fix bugs. Following is a list of those changes, arranged by version.

### v1.4.351

In order to improve argument safety, the interface of functions taking a C-array of values has changed to take a `std::array`, instead.
The affected functions are:
- `vk::CommandBuffer::setFragmentShadingRateKHR`
- `vk::raii::CommandBuffer::setBlendConstants`
- `vk::raii::CommandBuffer::setFragmentShadingRateKHR`
- `vk::raii::CommandBuffer::setFragmentShadingRateEnumNV`

### v1.4.334

The `vulkan_hpp` C++ named module has been renamed to `vulkan`.

### v1.4.324

With PR [#2226](https://github.com/KhronosGroup/Vulkan-Hpp/pull/2226), the return type of `vk::raii::Device::acquireNextImage2KHR` and `vk::raii::SwapchainKHR::acquireNextImage` has changed from `std::pair<vk::Result,uint32_t>` to the equivalent `vk::ResultValue<uint32_t>`

### v1.4.329

With PR [#2303](https://github.com/KhronosGroup/Vulkan-Hpp/pull/2303), `import std` was made mandatory when using the C++ named module.

## Contributing

Feel free to submit a PR to add to this list.
You should use clang-format version 22.1.1 to format the generated files.

- [Examples](https://github.com/jherico/vulkan) A port of Sascha Willems [examples](https://github.com/SaschaWillems/Vulkan) to Vulkan-Hpp
- [Vookoo](https://github.com/andy-thomason/Vookoo/) Stateful helper classes for Vulkan-Hpp, [Introduction Article](https://accu.org/journals/overload/25/139/overload139.pdf#page=14).

## License

Copyright 2015-2026 The Khronos Group Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
