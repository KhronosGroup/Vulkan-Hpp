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

#if !defined( VULKAN_HPP_USE_CXX_MODULE )
#include <vulkan/vulkan.hpp>
#endif

#define GLM_FORCE_RADIANS

#if !defined( VULKAN_HPP_USE_CXX_MODULE )
#  if defined( _MSC_VER )
#    pragma warning( push )
#    pragma warning( disable : 4127 )  // conditional expression is constant (glm)
#  endif

#  include <glm/gtc/matrix_transform.hpp>

#  if defined( _MSC_VER )
#    pragma warning( pop )
#  endif
#endif

namespace vk
{
  VULKAN_HPP_EXPORT namespace su
  {
    glm::mat4x4 createModelViewProjectionClipMatrix( vk::Extent2D const & extent );
  }
}  // namespace vk
