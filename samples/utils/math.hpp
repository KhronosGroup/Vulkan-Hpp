// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if defined( _MSC_VER )
#  pragma warning( disable : 4201 )  // disable warning C4201: nonstandard extension used: nameless struct/union; needed
                                     // to get glm/detail/type_vec?.hpp without warnings
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS

#if defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4127 )  // conditional expression is constant (glm)
#endif

#include <glm/gtc/matrix_transform.hpp>

#if defined( _MSC_VER )
#  pragma warning( pop )
#endif

namespace vk
{
  namespace su
  {
    glm::mat4x4 createModelViewProjectionClipMatrix( vk::Extent2D const & extent );
  }
}  // namespace vk
