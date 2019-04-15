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

#include <vulkan/vulkan.hpp>
#define GLM_FORCE_RADIANS
#pragma warning(disable:4201)   // disable warning C4201: nonstandard extension used: nameless struct/union; needed to get glm/detail/type_vec?.hpp without warnings
#include <glm/gtc/matrix_transform.hpp>

namespace vk
{
  namespace su
  {
    glm::mat4x4 createModelViewProjectionClipMatrix(vk::Extent2D const& extent);
  }
}
