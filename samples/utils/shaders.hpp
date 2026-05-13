// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace vk
{
  namespace su
  {
    vk::ShaderModule createShaderModule( vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText );

    bool GLSLtoSPV( const vk::ShaderStageFlagBits shaderType, std::string const & glslShader, std::vector<unsigned int> & spvShader );
  }  // namespace su
}  // namespace vk

// vertex shader with (P)osition and (C)olor in and (C)olor out
const std::string vertexShaderText_PC_C = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, binding = 0) uniform buffer
{
  mat4 mvp;
} uniformBuffer;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = inColor;
  gl_Position = uniformBuffer.mvp * pos;
}
)";

// vertex shader with (P)osition and (T)exCoord in and (T)exCoord out
const std::string vertexShaderText_PT_T = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, binding = 0) uniform buffer
{
  mat4 mvp;
} uniformBuffer;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 inTexCoord;

layout (location = 0) out vec2 outTexCoord;

void main()
{
  outTexCoord = inTexCoord;
  gl_Position = uniformBuffer.mvp * pos;
}
)";

// fragment shader with (C)olor in and (C)olor out
const std::string fragmentShaderText_C_C = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 color;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = color;
}
)";

// fragment shader with (T)exCoord in and (C)olor out
const std::string fragmentShaderText_T_C = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D tex;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = texture(tex, inTexCoord);
}
)";
