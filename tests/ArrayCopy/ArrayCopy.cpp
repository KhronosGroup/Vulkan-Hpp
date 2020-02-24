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
// VulkanHpp Samples : ArrayCopy
//                     Compile test on array copy functions

#include "vulkan/vulkan.hpp"

int main(int /*argc*/, char ** /*argv*/)
{
  struct wrap1D
  {
    wrap1D(const float src[3])
    {
      vk::ConstExpression1DArrayCopy<float, 3>::copy(dst, src);
    }

    wrap1D(std::array<float,3> const& src)
    {
      vk::ConstExpression1DArrayCopy<float, 3>::copy(dst, src);
    }

    float dst[3];
  };

  float src1D[3];
  src1D[0] = 1.0f;
  src1D[1] = 2.0f;
  src1D[2] = 3.0f;

  wrap1D w11(src1D);

  std::array<float, 3> asrc1D;
  asrc1D[0] = 1.0f;
  asrc1D[1] = 2.0f;
  asrc1D[2] = 3.0f;

  wrap1D w12(asrc1D);

  struct wrap2D
  {
    wrap2D(const float src2D[3][4])
    {
      vk::ConstExpression2DArrayCopy<float, 3, 4>::copy(dst, src2D);
    }

    wrap2D(std::array<std::array<float,4>, 3> const& src2D)
    {
      vk::ConstExpression2DArrayCopy<float, 3, 4>::copy(dst, src2D);
    }

    float dst[3][4];
  };

  float src2D[3][4];
  src2D[0][0] = 1.0f;
  src2D[0][1] = 2.0f;
  src2D[0][2] = 3.0f;
  src2D[0][3] = 4.0f;
  src2D[1][0] = 5.0f;
  src2D[1][1] = 6.0f;
  src2D[1][2] = 7.0f;
  src2D[1][3] = 8.0f;
  src2D[2][0] = 9.0f;
  src2D[2][1] = 10.0f;
  src2D[2][2] = 11.0f;
  src2D[2][3] = 12.0f;

  wrap2D w21(src2D);

  std::array<std::array<float,4>,3> asrc;
  asrc[0][0] = 1.0f;
  asrc[0][1] = 2.0f;
  asrc[0][2] = 3.0f;
  asrc[0][3] = 4.0f;
  asrc[1][0] = 5.0f;
  asrc[1][1] = 6.0f;
  asrc[1][2] = 7.0f;
  asrc[1][3] = 8.0f;
  asrc[2][0] = 9.0f;
  asrc[2][1] = 10.0f;
  asrc[2][2] = 11.0f;
  asrc[2][3] = 12.0f;

  wrap2D aw22(asrc);

  return 0;
}
