// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : Flags
//                     Compile test on using vk::Flags

// for test purposes, define this to allow access to Flags::m_mask
// You should not use that in production code!!

// in this test, we ignore some warnings
// You should not do that in production code !!
#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <compare> // necessary due to MSVC "design": https://developercommunity.visualstudio.com/t/Template-exports-requiring-importing-of-/1425979#T-N1435887
#  include <vulkan/vulkan_core.h>
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
#endif

template<typename T> void release_assert( const T &condition )
{
  if ( !condition ) throw std::runtime_error( "failed assert" );
}


int main( int /*argc*/, char ** /*argv*/ )
{
  vk::MemoryHeapFlags mhf0;
  release_assert( mhf0.m_mask == 0 );

  vk::MemoryHeapFlags mhf1( vk::MemoryHeapFlagBits::eDeviceLocal );
  release_assert( mhf1.m_mask == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );

  vk::MemoryHeapFlags mhf2( mhf1 );
  release_assert( mhf2.m_mask == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );

  VkMemoryHeapFlags   vkmhf = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT | VK_MEMORY_HEAP_MULTI_INSTANCE_BIT;
  vk::MemoryHeapFlags mhf3( vkmhf );
  release_assert( mhf3.m_mask == ( VK_MEMORY_HEAP_DEVICE_LOCAL_BIT | VK_MEMORY_HEAP_MULTI_INSTANCE_BIT ) );

  // error
  // vk::MemoryHeapFlags mhf4( vk::MemoryHeapFlagBits::eDeviceLocal | vk::ImageAspectFlagBits::eDepth );
  release_assert( mhf0 < mhf1 );
  release_assert( !( mhf1 < mhf2 ) );

  release_assert( mhf0 <= mhf1 );
  release_assert( mhf1 <= mhf2 );
  release_assert( !( mhf3 <= mhf2 ) );

  release_assert( mhf1 > mhf0 );
  release_assert( !( mhf2 > mhf1 ) );

  release_assert( mhf1 >= mhf0 );
  release_assert( mhf2 >= mhf1 );
  release_assert( !( mhf2 >= mhf3 ) );

  release_assert( !( mhf0 == mhf1 ) );
  release_assert( mhf1 == mhf2 );

  release_assert( mhf0 != mhf1 );
  release_assert( !( mhf1 != mhf2 ) );

  release_assert( !mhf0 );
  release_assert( !!mhf1 );

  release_assert( mhf1 & vk::MemoryHeapFlagBits::eDeviceLocal );
  release_assert( !( mhf0 & vk::MemoryHeapFlagBits::eDeviceLocal ) );
  
  release_assert( ( mhf0 | vk::MemoryHeapFlagBits::eDeviceLocal ) == vk::MemoryHeapFlagBits::eDeviceLocal );
  release_assert( ( mhf1 | vk::MemoryHeapFlagBits::eDeviceLocal ) == vk::MemoryHeapFlagBits::eDeviceLocal );

  release_assert( ( mhf0 ^ mhf1 ) == mhf1 );
  release_assert( ( mhf1 ^ mhf2 ) == mhf0 );

  vk::MemoryHeapFlags mhf4( ~mhf0 );
  release_assert( mhf4 == ( vk::MemoryHeapFlagBits::eDeviceLocal | vk::MemoryHeapFlagBits::eMultiInstance ) );
  
  mhf0 = mhf1;
  release_assert( mhf0 == mhf1 );
  
  mhf0 |= vk::MemoryHeapFlagBits::eMultiInstance;
  release_assert( mhf0 & vk::MemoryHeapFlagBits::eMultiInstance );
  
  mhf0 &= vk::MemoryHeapFlagBits::eDeviceLocal;
  release_assert( mhf0 & vk::MemoryHeapFlagBits::eDeviceLocal );
  
  mhf0 ^= mhf3;
  release_assert( mhf0 == ( vk::MemoryHeapFlagBits::eMultiInstance ) );

  vkmhf = static_cast<VkMemoryHeapFlags>(mhf0);

  return 0;
}
