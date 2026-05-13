// SPDX-FileCopyrightText: 2024-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

// VulkanHpp Tests : Video
//                   Compile test on using vk::video

#if defined( _MSC_VER )
#  pragma warning( disable : 4101 )   // 'name': unreference local variable
#endif
#if defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif


#ifdef VULKAN_HPP_USE_CXX_MODULE
import vulkan_video;
#else
#  include <vulkan/vulkan_video.hpp>
#endif


int main()
{
  vk::video::H264ChromaFormatIdc x;
  vk::video::H264ProfileIdc      y;

#if ( 301 < VK_HEADER_VERSION )
  vk::video::EncodeAV1DecoderModelInfo z;
#endif

  return 0;
}
