// SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : Flags
//                   Compile test on using vk::Flags

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

#include "../test_macros.hpp"
#ifdef VULKAN_HPP_USE_CXX_MODULE
#  include <compare> // necessary due to MSVC "design": https://developercommunity.visualstudio.com/t/Template-exports-requiring-importing-of-/1425979#T-N1435887
#  include <vulkan/vulkan_core.h>
import vulkan;
#else
#  include <vulkan/vulkan.hpp>
#endif


int main()
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
  release_assert( mhf4 == ( vk::MemoryHeapFlagBits::eDeviceLocal | vk::MemoryHeapFlagBits::eMultiInstance | vk::MemoryHeapFlagBits::eTileMemoryQCOM ) );

  mhf0 = mhf1;
  release_assert( mhf0 == mhf1 );
  
  mhf0 |= vk::MemoryHeapFlagBits::eMultiInstance;
  release_assert( mhf0 & vk::MemoryHeapFlagBits::eMultiInstance );
  
  mhf0 &= vk::MemoryHeapFlagBits::eDeviceLocal;
  release_assert( mhf0 & vk::MemoryHeapFlagBits::eDeviceLocal );
  
  mhf0 ^= mhf3;
  release_assert( mhf0 == ( vk::MemoryHeapFlagBits::eMultiInstance ) );

  vkmhf = static_cast<VkMemoryHeapFlags>( mhf0 );

  return 0;
}
