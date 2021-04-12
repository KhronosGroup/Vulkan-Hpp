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

#include "utils.hpp"
#include "../../samples/utils/shaders.hpp"

namespace vk
{
  namespace raii
  {
    namespace su
    {
      template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
      std::unique_ptr<vk::raii::ShaderModule> makeUniqueShaderModule( vk::raii::Device const & device,
                                                                      vk::ShaderStageFlagBits  shaderStage,
                                                                      std::string const &      shaderText )
      {
        std::vector<unsigned int> shaderSPV;
        if ( !vk::su::GLSLtoSPV( shaderStage, shaderText, shaderSPV ) )
        {
          throw std::runtime_error( "Could not convert glsl shader to spir-v -> terminating" );
        }

        return vk::raii::su::make_unique<vk::raii::ShaderModule>(
          device, vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
      }
    }  // namespace su
  }    // namespace raii
}  // namespace vk
