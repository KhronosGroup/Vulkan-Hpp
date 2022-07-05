# Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.2)

project(Vulkan-Hpp_RAIISamples)

option (SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP "Build with local Vulkan headers" ON)
option (SAMPLES_BUILD_ONLY_DYNAMIC "Build only dynamic. Required in case the Vulkan SDK is not available" OFF)

if(NOT (SAMPLES_BUILD_ONLY_DYNAMIC AND SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP))
  find_package(Vulkan REQUIRED)
endif()

if(MSVC)
  add_compile_options(/W4 /WX /permissive-)
else(MSVC)
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif(MSVC)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-DNOMINMAX -DVK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
else()
  message(FATAL_ERROR, "Vulkan-Hpp: unhandled platform for samples!")
endif()

if (SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP)
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Vulkan-Headers/include")
else()
  include_directories("${Vulkan_INCLUDE_DIRS}")
endif()

add_subdirectory(utils)
add_subdirectory(01_InitInstance)
add_subdirectory(02_EnumerateDevices)
add_subdirectory(03_InitDevice)
add_subdirectory(04_InitCommandBuffer)
add_subdirectory(05_InitSwapchain)
add_subdirectory(06_InitDepthBuffer)
add_subdirectory(07_InitUniformBuffer)
add_subdirectory(08_InitPipelineLayout)
add_subdirectory(09_InitDescriptorSet)
add_subdirectory(10_InitRenderPass)
add_subdirectory(11_InitShaders)
add_subdirectory(12_InitFrameBuffers)
add_subdirectory(13_InitVertexBuffer)
add_subdirectory(14_InitPipeline)
add_subdirectory(15_DrawCube)
add_subdirectory(16_Vulkan_1_1)
add_subdirectory(CopyBlitImage)
add_subdirectory(CreateDebugUtilsMessenger)
add_subdirectory(DebugUtilsObjectName)
add_subdirectory(DrawTexturedCube)
add_subdirectory(DynamicUniform)
add_subdirectory(EnableValidationWithCallback)
add_subdirectory(EnumerateDevicesAdvanced)
add_subdirectory(Events)
add_subdirectory(ImmutableSampler)
add_subdirectory(InitTexture)
add_subdirectory(InputAttachment)
add_subdirectory(InstanceExtensionProperties)
add_subdirectory(InstanceLayerExtensionProperties)
add_subdirectory(InstanceLayerProperties)
add_subdirectory(InstanceVersion)
add_subdirectory(MultipleSets)
add_subdirectory(OcclusionQuery)
add_subdirectory(PhysicalDeviceExtensions)
add_subdirectory(PhysicalDeviceFeatures)
add_subdirectory(PhysicalDeviceGroups)
add_subdirectory(PhysicalDeviceMemoryProperties)
add_subdirectory(PhysicalDeviceProperties)
add_subdirectory(PhysicalDeviceQueueFamilyProperties)
add_subdirectory(PipelineCache)
add_subdirectory(PipelineDerivative)
add_subdirectory(PushConstants)
add_subdirectory(PushDescriptors)
add_subdirectory(RayTracing)
add_subdirectory(SecondaryCommandBuffer)
add_subdirectory(SeparateImageSampler)
add_subdirectory(SurfaceCapabilities)
add_subdirectory(SurfaceFormats)
add_subdirectory(Template)
add_subdirectory(TexelBuffer)