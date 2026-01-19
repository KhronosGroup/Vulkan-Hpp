// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan;
#else
#  include <iostream>
#  include <vulkan/vulkan.hpp>
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  std::cout << vk::to_string( vk::PresentModeKHR::eFifo ) << std::endl;
  std::cout << vk::to_string( vk::SharingMode::eExclusive ) << std::endl;
  std::cout << vk::to_string( vk::ColorSpaceKHR::eSrgbNonlinear ) << std::endl;
  std::cout << vk::to_string( vk::Format::eA2R10G10B10UnormPack32 ) << std::endl;
  std::cout << vk::to_string( vk::ImageUsageFlagBits::eTransferDst ) << std::endl;
  return 0;
}
