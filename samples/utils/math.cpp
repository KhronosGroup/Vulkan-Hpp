// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

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

      glm::mat4x4 model      = glm::mat4x4( 1.0f );
      glm::mat4x4 view       = glm::lookAt( glm::vec3( -5.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
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
