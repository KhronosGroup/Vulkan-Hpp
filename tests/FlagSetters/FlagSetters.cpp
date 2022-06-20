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
//
// VulkanHpp Samples : FlagSetters
//                     Compile test on using flag setting functions on structs

#include <vulkan/vulkan.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  auto bufferCreateInfo = vk::BufferCreateInfo();
  bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eStorageBuffer);
  assert( bufferCreateInfo.usage == vk::BufferUsageFlagBits::eStorageBuffer );
  bufferCreateInfo.addUsage(vk::BufferUsageFlagBits::eStorageTexelBuffer);
  assert( bufferCreateInfo.usage == (vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eStorageTexelBuffer) );
  bufferCreateInfo.addUsage(vk::BufferUsageFlagBits::eStorageTexelBuffer);
  assert( bufferCreateInfo.usage == (vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eStorageTexelBuffer) );
  bufferCreateInfo.removeUsage(vk::BufferUsageFlagBits::eStorageBuffer);
  assert( bufferCreateInfo.usage == vk::BufferUsageFlagBits::eStorageTexelBuffer );
  bufferCreateInfo.removeUsage(vk::BufferUsageFlagBits::eStorageBuffer);
  assert( bufferCreateInfo.usage == vk::BufferUsageFlagBits::eStorageTexelBuffer );


  auto accStructInst = vk::AccelerationStructureInstanceKHR();
  accStructInst.setFlags(vk::GeometryInstanceFlagBitsKHR::eForceOpaque);
  assert( accStructInst.flags ==  VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR );
  accStructInst.addFlags(vk::GeometryInstanceFlagBitsKHR::eTriangleFlipFacing);
  assert( accStructInst.flags == (VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR | VK_GEOMETRY_INSTANCE_TRIANGLE_FLIP_FACING_BIT_KHR) );
  accStructInst.addFlags(vk::GeometryInstanceFlagBitsKHR::eTriangleFlipFacing);
  assert( accStructInst.flags == (VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR | VK_GEOMETRY_INSTANCE_TRIANGLE_FLIP_FACING_BIT_KHR) );
  accStructInst.removeFlags(vk::GeometryInstanceFlagBitsKHR::eForceOpaque);
  assert( accStructInst.flags == VK_GEOMETRY_INSTANCE_TRIANGLE_FLIP_FACING_BIT_KHR );
  accStructInst.removeFlags(vk::GeometryInstanceFlagBitsKHR::eForceOpaque);
  assert( accStructInst.flags == VK_GEOMETRY_INSTANCE_TRIANGLE_FLIP_FACING_BIT_KHR );

  return 0;
}
