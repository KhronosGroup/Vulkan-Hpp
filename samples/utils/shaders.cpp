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

#include "shaders.hpp"
#include "vulkan/vulkan.hpp"
#include "SPIRV/GlslangToSpv.h"

namespace vk
{
  namespace su
  {

    EShLanguage translateShaderStage(vk::ShaderStageFlagBits stage)
    {
      switch (stage)
      {
        case vk::ShaderStageFlagBits::eVertex:                  return EShLangVertex;
        case vk::ShaderStageFlagBits::eTessellationControl:     return EShLangTessControl;
        case vk::ShaderStageFlagBits::eTessellationEvaluation:  return EShLangTessEvaluation;
        case vk::ShaderStageFlagBits::eGeometry:                return EShLangGeometry;
        case vk::ShaderStageFlagBits::eFragment:                return EShLangFragment;
        case vk::ShaderStageFlagBits::eCompute:                 return EShLangCompute;
        default:
          assert(false && "Unknown shader stage");
          return EShLangVertex;
      }
    }

    void init(TBuiltInResource & resource)
    {
      resource.maxLights = 32;
      resource.maxClipPlanes = 6;
      resource.maxTextureUnits = 32;
      resource.maxTextureCoords = 32;
      resource.maxVertexAttribs = 64;
      resource.maxVertexUniformComponents = 4096;
      resource.maxVaryingFloats = 64;
      resource.maxVertexTextureImageUnits = 32;
      resource.maxCombinedTextureImageUnits = 80;
      resource.maxTextureImageUnits = 32;
      resource.maxFragmentUniformComponents = 4096;
      resource.maxDrawBuffers = 32;
      resource.maxVertexUniformVectors = 128;
      resource.maxVaryingVectors = 8;
      resource.maxFragmentUniformVectors = 16;
      resource.maxVertexOutputVectors = 16;
      resource.maxFragmentInputVectors = 15;
      resource.minProgramTexelOffset = -8;
      resource.maxProgramTexelOffset = 7;
      resource.maxClipDistances = 8;
      resource.maxComputeWorkGroupCountX = 65535;
      resource.maxComputeWorkGroupCountY = 65535;
      resource.maxComputeWorkGroupCountZ = 65535;
      resource.maxComputeWorkGroupSizeX = 1024;
      resource.maxComputeWorkGroupSizeY = 1024;
      resource.maxComputeWorkGroupSizeZ = 64;
      resource.maxComputeUniformComponents = 1024;
      resource.maxComputeTextureImageUnits = 16;
      resource.maxComputeImageUniforms = 8;
      resource.maxComputeAtomicCounters = 8;
      resource.maxComputeAtomicCounterBuffers = 1;
      resource.maxVaryingComponents = 60;
      resource.maxVertexOutputComponents = 64;
      resource.maxGeometryInputComponents = 64;
      resource.maxGeometryOutputComponents = 128;
      resource.maxFragmentInputComponents = 128;
      resource.maxImageUnits = 8;
      resource.maxCombinedImageUnitsAndFragmentOutputs = 8;
      resource.maxCombinedShaderOutputResources = 8;
      resource.maxImageSamples = 0;
      resource.maxVertexImageUniforms = 0;
      resource.maxTessControlImageUniforms = 0;
      resource.maxTessEvaluationImageUniforms = 0;
      resource.maxGeometryImageUniforms = 0;
      resource.maxFragmentImageUniforms = 8;
      resource.maxCombinedImageUniforms = 8;
      resource.maxGeometryTextureImageUnits = 16;
      resource.maxGeometryOutputVertices = 256;
      resource.maxGeometryTotalOutputComponents = 1024;
      resource.maxGeometryUniformComponents = 1024;
      resource.maxGeometryVaryingComponents = 64;
      resource.maxTessControlInputComponents = 128;
      resource.maxTessControlOutputComponents = 128;
      resource.maxTessControlTextureImageUnits = 16;
      resource.maxTessControlUniformComponents = 1024;
      resource.maxTessControlTotalOutputComponents = 4096;
      resource.maxTessEvaluationInputComponents = 128;
      resource.maxTessEvaluationOutputComponents = 128;
      resource.maxTessEvaluationTextureImageUnits = 16;
      resource.maxTessEvaluationUniformComponents = 1024;
      resource.maxTessPatchComponents = 120;
      resource.maxPatchVertices = 32;
      resource.maxTessGenLevel = 64;
      resource.maxViewports = 16;
      resource.maxVertexAtomicCounters = 0;
      resource.maxTessControlAtomicCounters = 0;
      resource.maxTessEvaluationAtomicCounters = 0;
      resource.maxGeometryAtomicCounters = 0;
      resource.maxFragmentAtomicCounters = 8;
      resource.maxCombinedAtomicCounters = 8;
      resource.maxAtomicCounterBindings = 1;
      resource.maxVertexAtomicCounterBuffers = 0;
      resource.maxTessControlAtomicCounterBuffers = 0;
      resource.maxTessEvaluationAtomicCounterBuffers = 0;
      resource.maxGeometryAtomicCounterBuffers = 0;
      resource.maxFragmentAtomicCounterBuffers = 1;
      resource.maxCombinedAtomicCounterBuffers = 1;
      resource.maxAtomicCounterBufferSize = 16384;
      resource.maxTransformFeedbackBuffers = 4;
      resource.maxTransformFeedbackInterleavedComponents = 64;
      resource.maxCullDistances = 8;
      resource.maxCombinedClipAndCullDistances = 8;
      resource.maxSamples = 4;
      resource.limits.nonInductiveForLoops = 1;
      resource.limits.whileLoops = 1;
      resource.limits.doWhileLoops = 1;
      resource.limits.generalUniformIndexing = 1;
      resource.limits.generalAttributeMatrixVectorIndexing = 1;
      resource.limits.generalVaryingIndexing = 1;
      resource.limits.generalSamplerIndexing = 1;
      resource.limits.generalVariableIndexing = 1;
      resource.limits.generalConstantMatrixVectorIndexing = 1;
    }

    bool GLSLtoSPV(const vk::ShaderStageFlagBits shaderType, std::string const& glslShader, std::vector<unsigned int> &spvShader)
    {
      EShLanguage stage = translateShaderStage(shaderType);

      const char *shaderStrings[1];
      shaderStrings[0] = glslShader.data();

      glslang::TShader shader(stage);
      shader.setStrings(shaderStrings, 1);

      TBuiltInResource resource;
      init(resource);

      // Enable SPIR-V and Vulkan rules when parsing GLSL
      EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

      if (!shader.parse(&resource, 100, false, messages))
      {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        return false;  // something didn't work
      }

      glslang::TProgram program;
      program.addShader(&shader);

      //
      // Program-level processing...
      //

      if (!program.link(messages))
      {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        fflush(stdout);
        return false;
      }

      glslang::GlslangToSpv(*program.getIntermediate(stage), spvShader);
      return true;
    }

    vk::UniqueShaderModule createShaderModule(vk::UniqueDevice &device, vk::ShaderStageFlagBits shaderStage, std::string const& shaderText)
    {
      std::vector<unsigned int> shaderSPV;
      bool ok = GLSLtoSPV(shaderStage, shaderText, shaderSPV);
      assert(ok);

      return device->createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), shaderSPV.size() * sizeof(unsigned int), shaderSPV.data()));
    }
  }
}
