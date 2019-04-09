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

struct VertexPC
{
  float x, y, z, w;   // Position
  float r, g, b, a;   // Color
};

struct VertexPT
{
  float x, y, z, w;   // Position data
  float u, v;         // texture u,v
};


static const VertexPC coloredCubeData[] =
{
  // red face
  { -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
  // green face
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
  // blue face
  { -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
  // yellow face
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
  // magenta face
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
  // cyan face
  {  1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
};

static const VertexPT texturedCubeData[] =
{
  // left face
  { -1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
  // front face
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
  // top face
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
  // bottom face
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
  { -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
  { -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
  // right face
  {  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
  {  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
  // back face
  { -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
  { -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
  {  1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
  {  1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
};

