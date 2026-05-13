# SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
# SPDX-License-Identifier: Apache-2.0

# Win32 exclusive vk::SurfaceCapabilitiesFullScreenExclusiveEXT is used
if(WIN32)
  vulkan_hpp__setup_sample_raii( NAME SurfaceCapabilities )
endif()
