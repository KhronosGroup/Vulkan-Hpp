// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
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

// ignore warning 4127: conditional expression is constant
#if defined( _MSC_VER )
#  pragma warning( disable : 4127 )
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "math.hpp"

namespace vk
{
  namespace su
  {
    glm::mat4x4 createModelViewProjectionClipMatrix( vk::Extent2D const & extent )
    {
      float fov = glm::radians( 45.0f );
      if ( extent.width > extent.height )
      {
        fov *= static_cast<float>( extent.height ) / static_cast<float>( extent.width );
      }

      glm::mat4x4 model = glm::mat4x4( 1.0f );
      glm::mat4x4 view =
        glm::lookAt( glm::vec3( -5.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
      glm::mat4x4 projection = glm::perspective( fov, 1.0f, 0.1f, 100.0f );
      // clang-format off
      glm::mat4x4 clip = glm::mat4x4( 1.0f,  0.0f, 0.0f, 0.0f,
                                      0.0f, -1.0f, 0.0f, 0.0f,
                                      0.0f,  0.0f, 0.5f, 0.0f,
                                      0.0f,  0.0f, 0.5f, 1.0f );  // vulkan clip space has inverted y and half z !
      // clang-format on 
      return clip * projection * view * model;
    }
  }  // namespace su
}  // namespace vk
