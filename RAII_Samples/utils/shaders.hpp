// SPDX-FileCopyrightText: 2019-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if !defined( VULKAN_HPP_USE_CXX_MODULE )
#include "../../samples/utils/shaders.hpp"
#include "utils.hpp"
#endif

namespace vk
{
  namespace raii
  {
    VULKAN_HPP_EXPORT namespace su
    {
      template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
      vk::raii::ShaderModule makeShaderModule( vk::raii::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText )
      {
        std::vector<unsigned int> shaderSPV;
        if ( !vk::su::GLSLtoSPV( shaderStage, shaderText, shaderSPV ) )
        {
          throw std::runtime_error( "Could not convert glsl shader to spir-v -> terminating" );
        }

        return vk::raii::ShaderModule( device, vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
      }
    }  // namespace su
  }    // namespace raii
}  // namespace vk
