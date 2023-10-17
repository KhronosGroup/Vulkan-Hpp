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

#include "SPIRV/GlslangToSpv.h"
#include "glslang/Public/ResourceLimits.h"

#include <vulkan/vulkan.hpp>

namespace vk
{
  namespace su
  {
    EShLanguage translateShaderStage( vk::ShaderStageFlagBits stage )
    {
      switch ( stage )
      {
        case vk::ShaderStageFlagBits::eVertex: return EShLangVertex;
        case vk::ShaderStageFlagBits::eTessellationControl: return EShLangTessControl;
        case vk::ShaderStageFlagBits::eTessellationEvaluation: return EShLangTessEvaluation;
        case vk::ShaderStageFlagBits::eGeometry: return EShLangGeometry;
        case vk::ShaderStageFlagBits::eFragment: return EShLangFragment;
        case vk::ShaderStageFlagBits::eCompute: return EShLangCompute;
        case vk::ShaderStageFlagBits::eRaygenNV: return EShLangRayGenNV;
        case vk::ShaderStageFlagBits::eAnyHitNV: return EShLangAnyHitNV;
        case vk::ShaderStageFlagBits::eClosestHitNV: return EShLangClosestHitNV;
        case vk::ShaderStageFlagBits::eMissNV: return EShLangMissNV;
        case vk::ShaderStageFlagBits::eIntersectionNV: return EShLangIntersectNV;
        case vk::ShaderStageFlagBits::eCallableNV: return EShLangCallableNV;
        case vk::ShaderStageFlagBits::eTaskNV: return EShLangTaskNV;
        case vk::ShaderStageFlagBits::eMeshNV: return EShLangMeshNV;
        default: assert( false && "Unknown shader stage" ); return EShLangVertex;
      }
    }

    bool GLSLtoSPV( const vk::ShaderStageFlagBits shaderType, std::string const & glslShader, std::vector<unsigned int> & spvShader )
    {
      EShLanguage stage = translateShaderStage( shaderType );

      const char * shaderStrings[1];
      shaderStrings[0] = glslShader.data();

      glslang::TShader shader( stage );
      shader.setStrings( shaderStrings, 1 );

      // Enable SPIR-V and Vulkan rules when parsing GLSL
      EShMessages messages = (EShMessages)( EShMsgSpvRules | EShMsgVulkanRules );

      if ( !shader.parse( GetDefaultResources(), 100, false, messages ) )
      {
        puts( shader.getInfoLog() );
        puts( shader.getInfoDebugLog() );
        return false;  // something didn't work
      }

      glslang::TProgram program;
      program.addShader( &shader );

      //
      // Program-level processing...
      //

      if ( !program.link( messages ) )
      {
        puts( shader.getInfoLog() );
        puts( shader.getInfoDebugLog() );
        fflush( stdout );
        return false;
      }

      glslang::GlslangToSpv( *program.getIntermediate( stage ), spvShader );
      return true;
    }

    vk::ShaderModule createShaderModule( vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText )
    {
      std::vector<unsigned int> shaderSPV;
      if ( !GLSLtoSPV( shaderStage, shaderText, shaderSPV ) )
      {
        throw std::runtime_error( "Could not convert glsl shader to spir-v -> terminating" );
      }

      return device.createShaderModule( vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
    }
  }  // namespace su
}  // namespace vk
