// Copyright(c) 2024, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : Video
//                     Compile test on using vk::video

#if defined( _MSC_VER )
#  pragma warning( disable : 4101 )   // 'name': unreference local variable
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#include <vulkan/vulkan_video.hpp>

int main( int /*argc*/, char ** /*argv*/ )
{
  vk::video::H264ChromaFormatIdc x;
  vk::video::H264ProfileIdc      y;

#if ( 301 < VK_HEADER_VERSION )
  vk::video::EncodeAV1DecoderModelInfo z;
#endif

  return 0;
}
