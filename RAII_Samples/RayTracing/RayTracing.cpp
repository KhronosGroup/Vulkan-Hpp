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
// VulkanHpp Samples : RayTracing
//                     Simple sample how to ray trace using Vulkan

#if defined( _MSC_VER )
#  pragma warning( disable : 4201 )  // disable warning C4201: nonstandard extension used: nameless struct/union; needed
                                     // to get glm/detail/type_vec?.hpp without warnings
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wmissing-braces"
#  if ( 10 <= __clang_major__ )
#    pragma clang diagnostic ignored "-Wdeprecated-volatile"  // to keep glm/detail/type_half.inl compiling
#  endif
#elif defined( __GNUC__ )
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

// clang-format off
// we need to include vulkan.hpp before glfw3.h, so stop clang-format to reorder them
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
// clang-format on
#include <numeric>
#include <random>
#include <sstream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "../utils/shaders.hpp"
#include "../utils/utils.hpp"
#include "CameraManipulator.hpp"
#include "SPIRV/GlslangToSpv.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

static char const * AppName    = "RayTracing";
static char const * EngineName = "Vulkan.hpp";

struct GeometryInstanceData
{
  GeometryInstanceData( glm::mat4x4 const & transform_,
                        uint32_t            instanceID_,
                        uint8_t             mask_,
                        uint32_t            instanceOffset_,
                        uint8_t             flags_,
                        uint64_t            accelerationStructureHandle_ )
    : instanceId( instanceID_ )
    , mask( mask_ )
    , instanceOffset( instanceOffset_ )
    , flags( flags_ )
    , accelerationStructureHandle( accelerationStructureHandle_ )
  {
    assert( !( instanceID_ & 0xFF000000 ) && !( instanceOffset_ & 0xFF000000 ) );
    memcpy( transform, &transform_, 12 * sizeof( float ) );
  }

  float    transform[12];                // Transform matrix, containing only the top 3 rows
  uint32_t instanceId : 24;              // Instance index
  uint32_t mask : 8;                     // Visibility mask
  uint32_t instanceOffset : 24;          // Index of the hit group which will be invoked when a ray hits the instance
  uint32_t flags : 8;                    // Instance flags, such as culling
  uint64_t accelerationStructureHandle;  // Opaque handle of the bottom-level acceleration structure
};
static_assert( sizeof( GeometryInstanceData ) == 64, "GeometryInstanceData structure compiles to incorrect size" );

struct AccelerationStructureData
{
  std::shared_ptr<vk::raii::AccelerationStructureNV> acclerationStructure;
  std::unique_ptr<vk::raii::su::BufferData>          scratchBufferData;
  std::unique_ptr<vk::raii::su::BufferData>          resultBufferData;
  std::unique_ptr<vk::raii::su::BufferData>          instanceBufferData;
};

AccelerationStructureData createAccelerationStructureData(
  vk::raii::PhysicalDevice const &                                                                physicalDevice,
  vk::raii::Device const &                                                                        device,
  vk::raii::CommandBuffer const &                                                                 commandBuffer,
  std::vector<std::pair<std::shared_ptr<vk::raii::AccelerationStructureNV>, glm::mat4x4>> const & instances,
  std::vector<vk::GeometryNV> const &                                                             geometries )
{
  assert( instances.empty() ^ geometries.empty() );

  AccelerationStructureData accelerationStructureData;

  vk::AccelerationStructureTypeNV accelerationStructureType =
    instances.empty() ? vk::AccelerationStructureTypeNV::eBottomLevel : vk::AccelerationStructureTypeNV::eTopLevel;
  vk::AccelerationStructureInfoNV accelerationStructureInfo(
    accelerationStructureType, {}, vk::su::checked_cast<uint32_t>( instances.size() ), geometries );
  vk::AccelerationStructureCreateInfoNV accelerationStructureCreateInfoNV( 0, accelerationStructureInfo );
  accelerationStructureData.acclerationStructure =
    std::make_shared<vk::raii::AccelerationStructureNV>( device, accelerationStructureCreateInfoNV );

  vk::AccelerationStructureMemoryRequirementsInfoNV objectRequirements(
    vk::AccelerationStructureMemoryRequirementsTypeNV::eObject, **accelerationStructureData.acclerationStructure );
  vk::DeviceSize resultSizeInBytes =
    device.getAccelerationStructureMemoryRequirementsNV( objectRequirements ).memoryRequirements.size;
  assert( 0 < resultSizeInBytes );
  accelerationStructureData.resultBufferData =
    vk::raii::su::make_unique<vk::raii::su::BufferData>( physicalDevice,
                                                device,
                                                resultSizeInBytes,
                                                vk::BufferUsageFlagBits::eRayTracingNV,
                                                vk::MemoryPropertyFlagBits::eDeviceLocal );

  vk::AccelerationStructureMemoryRequirementsInfoNV buildScratchRequirements(
    vk::AccelerationStructureMemoryRequirementsTypeNV::eBuildScratch,
    **accelerationStructureData.acclerationStructure );
  vk::AccelerationStructureMemoryRequirementsInfoNV updateScratchRequirements(
    vk::AccelerationStructureMemoryRequirementsTypeNV::eUpdateScratch,
    **accelerationStructureData.acclerationStructure );
  vk::DeviceSize scratchSizeInBytes = std::max(
    device.getAccelerationStructureMemoryRequirementsNV( buildScratchRequirements ).memoryRequirements.size,
    device.getAccelerationStructureMemoryRequirementsNV( updateScratchRequirements ).memoryRequirements.size );
  assert( 0 < scratchSizeInBytes );

  accelerationStructureData.scratchBufferData =
    vk::raii::su::make_unique<vk::raii::su::BufferData>( physicalDevice,
                                                device,
                                                scratchSizeInBytes,
                                                vk::BufferUsageFlagBits::eRayTracingNV,
                                                vk::MemoryPropertyFlagBits::eDeviceLocal );

  if ( !instances.empty() )
  {
    accelerationStructureData.instanceBufferData =
      vk::raii::su::make_unique<vk::raii::su::BufferData>( physicalDevice,
                                                  device,
                                                  instances.size() * sizeof( GeometryInstanceData ),
                                                  vk::BufferUsageFlagBits::eRayTracingNV );

    std::vector<GeometryInstanceData> geometryInstanceData;
    for ( size_t i = 0; i < instances.size(); i++ )
    {
      uint64_t accelerationStructureHandle = instances[i].first->getHandle<uint64_t>();

      // For each instance we set its instance index to its index i in the instance vector, and set
      // its hit group index to 2*i. The hit group index defines which entry of the shader binding
      // table will contain the hit group to be executed when hitting this instance. We set this
      // index to 2*i due to the use of 2 types of rays in the scene: the camera rays and the shadow
      // rays. For each instance, the SBT will then have 2 hit groups
      geometryInstanceData.emplace_back( glm::transpose( instances[i].second ),
                                         static_cast<uint32_t>( i ),
                                         static_cast<uint8_t>( 0xFF ),
                                         static_cast<uint32_t>( 2 * i ),
                                         static_cast<uint8_t>( vk::GeometryInstanceFlagBitsNV::eTriangleCullDisable ),
                                         accelerationStructureHandle );
    }
    accelerationStructureData.instanceBufferData->upload( geometryInstanceData );
  }

  device.bindAccelerationStructureMemoryNV( vk::BindAccelerationStructureMemoryInfoNV(
    **accelerationStructureData.acclerationStructure, **accelerationStructureData.resultBufferData->deviceMemory ) );

  vk::Buffer instanceData;
  if ( accelerationStructureData.instanceBufferData )
  {
    instanceData = **accelerationStructureData.instanceBufferData->buffer;
  }
  vk::AccelerationStructureInfoNV accelerationStructureInfoNV(
    accelerationStructureType, {}, vk::su::checked_cast<uint32_t>( instances.size() ), geometries );
  commandBuffer.buildAccelerationStructureNV( accelerationStructureInfoNV,
                                              instanceData,
                                              0,
                                              false,
                                              **accelerationStructureData.acclerationStructure,
                                              nullptr,
                                              **accelerationStructureData.scratchBufferData->buffer,
                                              0 );

  vk::MemoryBarrier memoryBarrier(
    vk::AccessFlagBits::eAccelerationStructureWriteNV | vk::AccessFlagBits::eAccelerationStructureReadNV,
    vk::AccessFlagBits::eAccelerationStructureWriteNV | vk::AccessFlagBits::eAccelerationStructureReadNV );
  commandBuffer.pipelineBarrier( vk::PipelineStageFlagBits::eAccelerationStructureBuildNV,
                                 vk::PipelineStageFlagBits::eAccelerationStructureBuildNV,
                                 {},
                                 memoryBarrier,
                                 {},
                                 {} );

  return accelerationStructureData;
}

struct PerFrameData
{
  std::unique_ptr<vk::raii::CommandPool>   commandPool;
  std::unique_ptr<vk::raii::CommandBuffer> commandBuffer;
  std::unique_ptr<vk::raii::Fence>         fence;
  std::unique_ptr<vk::raii::Semaphore>     presentCompleteSemaphore;
  std::unique_ptr<vk::raii::Semaphore>     renderCompleteSemaphore;
};

struct UniformBufferObject
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 modelIT;
  glm::mat4 viewInverse;
  glm::mat4 projInverse;
};

struct Material
{
  glm::vec3 diffuse   = glm::vec3( 0.7f, 0.7f, 0.7f );
  int       textureID = -1;
};
const size_t MaterialStride = ( ( sizeof( Material ) + 15 ) / 16 ) * 16;

struct Vertex
{
  Vertex( glm::vec3 const & p, glm::vec3 const & n, glm::vec2 const & tc, int m = 0 )
    : pos( p ), nrm( n ), texCoord( tc ), matID( m )
  {}

  glm::vec3 pos;
  glm::vec3 nrm;
  glm::vec2 texCoord;
  int       matID;
};
const size_t VertexStride = ( ( sizeof( Vertex ) + 15 ) / 16 ) * 16;

static const std::vector<Vertex> cubeData = {
  //        pos                               nrm                             texcoord              matID
  //  front face
  { Vertex( glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  //  back face
  { Vertex( glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  //  left face
  { Vertex( glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  //  right face
  { Vertex( glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  //  top face
  { Vertex( glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  //  bottom face
  { Vertex( glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 1.0f, 0.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 0.0f, 1.0f ), 0 ) },
  { Vertex( glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec2( 0.0f, 0.0f ), 0 ) },
};

static std::string vertexShaderText = R"(
#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 modelIT;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int  inMatID;

layout(location = 0) flat out int  outMatID;
layout(location = 1)      out vec2 outTexCoord;
layout(location = 2)      out vec3 outNormal;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
  outMatID    = inMatID;
  outTexCoord = inTexCoord;
  outNormal   = vec3(ubo.modelIT * vec4(inNormal, 0.0));
}
)";

static std::string fragmentShaderText = R"(
#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) flat in int  matIndex;
layout(location = 1)      in vec2 texCoord;
layout(location = 2)      in vec3 normal;

struct Material
{
  vec3 diffuse;
  int  textureID;
};
const int sizeofMat = 1;

layout(binding = 1) buffer MaterialBufferObject { vec4[] m; } materials;
layout(binding = 2) uniform sampler2D[] textureSamplers;

Material unpackMaterial()
{
  Material m;
  vec4 d0 = materials.m[sizeofMat * matIndex + 0];

  m.diffuse = d0.xyz;
  m.textureID = floatBitsToInt(d0.w);

  return m;
}

layout(location = 0) out vec4 outColor;

void main()
{
  vec3 lightVector = normalize(vec3(5, 4, 3));

  float dot_product = max(dot(lightVector, normalize(normal)), 0.2);

  Material m = unpackMaterial();
  vec3 c = m.diffuse;
  if (0 <= m.textureID)
  {
    c *= texture(textureSamplers[m.textureID], texCoord).xyz;
  }
  c *= dot_product;

  outColor = vec4(c, 1);
}
)";

static std::string raygenShaderText = R"(
#version 460

#extension GL_NV_ray_tracing : require

layout(binding = 0, set = 0) uniform accelerationStructureNV topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D image;

layout(binding=2, set = 0) uniform UniformBufferObject
{
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 modelIT;
  mat4 viewInverse;
  mat4 projInverse;
} cam;

layout(location = 0) rayPayloadNV vec3 hitValue;

void main() 
{
  const vec2 pixelCenter = vec2(gl_LaunchIDNV.xy) + vec2(0.5);
  const vec2 inUV = pixelCenter/vec2(gl_LaunchSizeNV.xy);
  vec2 d = inUV * 2.0 - 1.0;

  vec4 origin = cam.viewInverse*vec4(0,0,0,1);
  vec4 target = cam.projInverse * vec4(d.x, d.y, 1, 1) ;
  vec4 direction = cam.viewInverse*vec4(normalize(target.xyz), 0) ;

  uint rayFlags = gl_RayFlagsOpaqueNV;
  uint cullMask = 0xff;
  float tmin = 0.001;
  float tmax = 10000.0;

  traceNV(topLevelAS, rayFlags, cullMask, 0 /*sbtRecordOffset*/, 0 /*sbtRecordStride*/, 0 /*missIndex*/, origin.xyz, tmin, direction.xyz, tmax, 0 /*payload*/);
  imageStore(image, ivec2(gl_LaunchIDNV.xy), vec4(hitValue, 0.0));
}
)";

static std::string missShaderText = R"(
#version 460

#extension GL_NV_ray_tracing : require

layout(location = 0) rayPayloadInNV vec3 hitValue;

void main()
{
  hitValue = vec3(0.0, 0.1, 0.3);
}
)";

static std::string shadowMissShaderText = R"(
#version 460

#extension GL_NV_ray_tracing : require

layout(location = 2) rayPayloadInNV bool isShadowed;

void main()
{
  isShadowed = false;
})";

static std::string closestHitShaderText = R"(
#version 460

#extension GL_NV_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) rayPayloadInNV vec3 hitValue;
layout(location = 2) rayPayloadNV bool isShadowed;

hitAttributeNV vec3 attribs;
layout(binding = 0, set = 0) uniform accelerationStructureNV topLevelAS;

layout(binding = 3, set = 0) buffer Vertices { vec4 v[]; } vertices;
layout(binding = 4, set = 0) buffer Indices { uint i[]; } indices;

layout(binding = 5, set = 0) buffer MatColorBufferObject { vec4[] m; } materials;
layout(binding = 6, set = 0) uniform sampler2D[] textureSamplers;

struct Vertex
{
  vec3 pos;
  vec3 nrm;
  vec2 texCoord;
  int matIndex;
};
// Number of vec4 values used to represent a vertex
uint vertexSize = 3;

Vertex unpackVertex(uint index)
{
  Vertex v;

  vec4 d0 = vertices.v[vertexSize * index + 0];
  vec4 d1 = vertices.v[vertexSize * index + 1];
  vec4 d2 = vertices.v[vertexSize * index + 2];

  v.pos = d0.xyz;
  v.nrm = vec3(d0.w, d1.xy);
  v.texCoord = d1.zw;
  v.matIndex = floatBitsToInt(d2.x);
  return v;
}

struct Material
{
  vec3 diffuse;
  int textureID;
};
// Number of vec4 values used to represent a material
const int sizeofMat = 1;

Material unpackMaterial(int matIndex)
{
  Material m;
  vec4 d0 = materials.m[sizeofMat * matIndex + 0];

  m.diffuse = d0.xyz;
  m.textureID = floatBitsToInt(d0.w);
  return m;
}

void main()
{
  ivec3 ind = ivec3(indices.i[3 * gl_PrimitiveID], indices.i[3 * gl_PrimitiveID + 1], indices.i[3 * gl_PrimitiveID + 2]);

  Vertex v0 = unpackVertex(ind.x);
  Vertex v1 = unpackVertex(ind.y);
  Vertex v2 = unpackVertex(ind.z);

  const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);

  vec3 normal = normalize(v0.nrm * barycentrics.x + v1.nrm * barycentrics.y + v2.nrm * barycentrics.z);

  vec3 lightVector = normalize(vec3(5, 4, 3));

  float dot_product = max(dot(lightVector, normal), 0.2);

  Material mat = unpackMaterial(v1.matIndex);

  vec3 c = dot_product * mat.diffuse; 
  if (0 <= mat.textureID)
  {
    vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;
    c *= texture(textureSamplers[mat.textureID], texCoord).xyz;
  }
  float tmin = 0.001;
  float tmax = 100.0;
  vec3 origin = gl_WorldRayOriginNV + gl_WorldRayDirectionNV * gl_HitTNV;
  isShadowed = true;
  traceNV(topLevelAS, gl_RayFlagsTerminateOnFirstHitNV|gl_RayFlagsOpaqueNV|gl_RayFlagsSkipClosestHitShaderNV,  0xFF, 1 /* sbtRecordOffset */, 0 /* sbtRecordStride */, 1 /* missIndex */, origin,
          tmin, lightVector, tmax, 2 /*payload location*/);
  hitValue = c;
  if (isShadowed)
  {
    hitValue *= 0.3f;
  }
}
)";

#ifndef IMGUI_VK_QUEUED_FRAMES
#  define IMGUI_VK_QUEUED_FRAMES 2
#endif  // !IMGUI_VK_QUEUED_FRAMES

struct AppInfo
{
  vk::su::CameraManipulator cameraManipulator;
  bool                      useRasterRender = false;
};

static void check_vk_result( VkResult err )
{
  if ( err != 0 )
  {
    std::cerr << AppName << ": Vulkan error " << vk::to_string( static_cast<vk::Result>( err ) );
    if ( err < 0 )
    {
      abort();
    }
  }
}

static void cursorPosCallback( GLFWwindow * window, double mouseX, double mouseY )
{
  vk::su::CameraManipulator::MouseButton mouseButton =
    ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
      ? vk::su::CameraManipulator::MouseButton::Left
    : ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS )
      ? vk::su::CameraManipulator::MouseButton::Middle
    : ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS )
      ? vk::su::CameraManipulator::MouseButton::Right
      : vk::su::CameraManipulator::MouseButton::None;
  if ( mouseButton != vk::su::CameraManipulator::MouseButton::None )
  {
    vk::su::CameraManipulator::ModifierFlags modifiers;
    if ( glfwGetKey( window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS )
    {
      modifiers |= vk::su::CameraManipulator::ModifierFlagBits::Alt;
    }
    if ( glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS )
    {
      modifiers |= vk::su::CameraManipulator::ModifierFlagBits::Ctrl;
    }
    if ( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
    {
      modifiers |= vk::su::CameraManipulator::ModifierFlagBits::Shift;
    }

    vk::su::CameraManipulator & cameraManipulator =
      reinterpret_cast<AppInfo *>( glfwGetWindowUserPointer( window ) )->cameraManipulator;
    cameraManipulator.mouseMove(
      glm::ivec2( static_cast<int>( mouseX ), static_cast<int>( mouseY ) ), mouseButton, modifiers );
  }
}

static void errorCallback( int error, const char * description )
{
  fprintf( stderr, "GLFW Error %d: %s\n", error, description );
}

static void framebufferSizeCallback( GLFWwindow * window, int w, int h )
{
  vk::su::CameraManipulator & cameraManipulator =
    reinterpret_cast<AppInfo *>( glfwGetWindowUserPointer( window ) )->cameraManipulator;
  cameraManipulator.setWindowSize( glm::ivec2( w, h ) );
}

static void keyCallback( GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/ )
{
  if ( action == GLFW_PRESS )
  {
    switch ( key )
    {
      case GLFW_KEY_ESCAPE:
      case 'Q': glfwSetWindowShouldClose( window, 1 ); break;
      case 'R':
      {
        AppInfo * appInfo        = reinterpret_cast<AppInfo *>( glfwGetWindowUserPointer( window ) );
        appInfo->useRasterRender = !appInfo->useRasterRender;
      }
      break;
    }
  }
}

static void mouseButtonCallback( GLFWwindow * window, int /*button*/, int /*action*/, int /*mods*/ )
{
  double xpos, ypos;
  glfwGetCursorPos( window, &xpos, &ypos );

  vk::su::CameraManipulator & cameraManipulator =
    reinterpret_cast<AppInfo *>( glfwGetWindowUserPointer( window ) )->cameraManipulator;
  cameraManipulator.setMousePosition( glm::ivec2( static_cast<int>( xpos ), static_cast<int>( ypos ) ) );
}

static void scrollCallback( GLFWwindow * window, double /*xoffset*/, double yoffset )
{
  vk::su::CameraManipulator & cameraManipulator =
    reinterpret_cast<AppInfo *>( glfwGetWindowUserPointer( window ) )->cameraManipulator;
  cameraManipulator.wheel( static_cast<int>( yoffset ) );
}

// random data and functions
static std::random_device randomDevice;
static std::mt19937       randomGenerator( randomDevice() );

template <typename T>
T random( T minValue = std::numeric_limits<T>::min(), T maxValue = std::numeric_limits<T>::max() )
{
  static_assert( std::numeric_limits<T>::is_integer, "Type T needs to be an integral type!\n" );
  std::uniform_int_distribution<> randomDistribution( minValue, maxValue );

  return static_cast<T>( randomDistribution( randomGenerator ) );
}

glm::vec3 randomVec3( float minValue, float maxValue )
{
  std::uniform_real_distribution<float> randomDistribution( minValue, maxValue );

  return glm::vec3( randomDistribution( randomGenerator ),
                    randomDistribution( randomGenerator ),
                    randomDistribution( randomGenerator ) );
}

uint32_t roundUp( uint32_t value, uint32_t alignment )
{
  return ( ( value + alignment - 1 ) / alignment ) * alignment;
}

int main( int /*argc*/, char ** /*argv*/ )
{
  // number of cubes in x-, y-, and z-direction
  const size_t xMax = 10;
  const size_t yMax = 10;
  const size_t zMax = 10;

  AppInfo appInfo;

  try
  {
    // Setup glfw
    glfwSetErrorCallback( errorCallback );
    if ( !glfwInit() )
    {
      std::cerr << AppName << ": can't initialize glfw!\n";
      return 1;
    }
    if ( !glfwVulkanSupported() )
    {
      std::cerr << AppName << ": Vulkan not supported!\n";
      return 1;
    }

    // create a window using glfw
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    vk::Extent2D windowExtent( 1280, 720 );
    GLFWwindow * window = glfwCreateWindow( windowExtent.width, windowExtent.height, AppName, nullptr, nullptr );

    // install some callbacks
    glfwSetCursorPosCallback( window, cursorPosCallback );
    glfwSetFramebufferSizeCallback( window, framebufferSizeCallback );
    glfwSetKeyCallback( window, keyCallback );
    glfwSetMouseButtonCallback( window, mouseButtonCallback );
    glfwSetScrollCallback( window, scrollCallback );

    // Setup camera and make it available as the userPointer in the glfw window
    appInfo.cameraManipulator.setWindowSize( glm::u32vec2( windowExtent.width, windowExtent.height ) );
    glm::vec3 diagonal =
      3.0f * glm::vec3( static_cast<float>( xMax ), static_cast<float>( yMax ), static_cast<float>( zMax ) );
    appInfo.cameraManipulator.setLookat( 1.5f * diagonal, 0.5f * diagonal, glm::vec3( 0, 1, 0 ) );
    glfwSetWindowUserPointer( window, &appInfo );

    // Create Vulkan Instance with needed extensions
    uint32_t                 glfwExtensionsCount;
    const char **            glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionsCount );
    std::vector<std::string> instanceExtensions;
    instanceExtensions.reserve( glfwExtensionsCount + 1 );
    for ( uint32_t i = 0; i < glfwExtensionsCount; i++ )
    {
      instanceExtensions.push_back( glfwExtensions[i] );
    }
    instanceExtensions.push_back( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME );

    std::unique_ptr<vk::raii::Context>  context = vk::raii::su::make_unique<vk::raii::Context>();
    std::unique_ptr<vk::raii::Instance> instance =
      vk::raii::su::makeUniqueInstance( *context, AppName, EngineName, {}, instanceExtensions );
#if !defined( NDEBUG )
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger =
      vk::raii::su::makeUniqueDebugUtilsMessengerEXT( *instance );
#endif
    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice = vk::raii::su::makeUniquePhysicalDevice( *instance );

    // Create Window Surface (using glfw)
    VkSurfaceKHR glfwSurface;
    VkResult     err = glfwCreateWindowSurface( static_cast<VkInstance>( **instance ), window, nullptr, &glfwSurface );
    check_vk_result( err );
    std::unique_ptr<vk::raii::SurfaceKHR> surface = vk::raii::su::make_unique<vk::raii::SurfaceKHR>( *instance, glfwSurface );

    std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
      vk::raii::su::findGraphicsAndPresentQueueFamilyIndex( *physicalDevice, *surface );

    // Create a Device with ray tracing support (besides some other extensions needed) and needed features
    auto supportedFeatures =
      physicalDevice->getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceDescriptorIndexingFeaturesEXT>();
    std::unique_ptr<vk::raii::Device> device =
      vk::raii::su::makeUniqueDevice( *physicalDevice,
                                      graphicsAndPresentQueueFamilyIndex.first,
                                      { VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                        VK_NV_RAY_TRACING_EXTENSION_NAME,
                                        VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME },
                                      &supportedFeatures.get<vk::PhysicalDeviceFeatures2>().features,
                                      &supportedFeatures.get<vk::PhysicalDeviceDescriptorIndexingFeaturesEXT>() );

    // setup stuff per frame
    std::array<PerFrameData, IMGUI_VK_QUEUED_FRAMES> perFrameData;
    for ( int i = 0; i < IMGUI_VK_QUEUED_FRAMES; i++ )
    {
      perFrameData[i].commandPool =
        vk::raii::su::makeUniqueCommandPool( *device, graphicsAndPresentQueueFamilyIndex.first );
      perFrameData[i].commandBuffer = vk::raii::su::makeUniqueCommandBuffer( *device, *perFrameData[i].commandPool );
      perFrameData[i].fence =
        vk::raii::su::make_unique<vk::raii::Fence>( *device, vk::FenceCreateInfo( vk::FenceCreateFlagBits::eSignaled ) );
      perFrameData[i].presentCompleteSemaphore =
        vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );
      perFrameData[i].renderCompleteSemaphore =
        vk::raii::su::make_unique<vk::raii::Semaphore>( *device, vk::SemaphoreCreateInfo() );
    }

    std::unique_ptr<vk::raii::Queue> graphicsQueue =
      vk::raii::su::make_unique<vk::raii::Queue>( *device, graphicsAndPresentQueueFamilyIndex.first, 0 );
    std::unique_ptr<vk::raii::Queue> presentQueue =
      vk::raii::su::make_unique<vk::raii::Queue>( *device, graphicsAndPresentQueueFamilyIndex.second, 0 );

    // create a descriptor pool with a number of available descriptors
    std::vector<vk::DescriptorPoolSize> poolSizes = {
      { vk::DescriptorType::eCombinedImageSampler, 1000 },
      { vk::DescriptorType::eUniformBuffer, 1000 },
      { vk::DescriptorType::eStorageBuffer, 1000 },
    };
    std::unique_ptr<vk::raii::DescriptorPool> descriptorPool =
      vk::raii::su::makeUniqueDescriptorPool( *device, poolSizes );

    // setup swap chain, render pass, depth buffer and the frame buffers
    vk::raii::su::SwapChainData swapChainData( *physicalDevice,
                                               *device,
                                               *surface,
                                               windowExtent,
                                               vk::ImageUsageFlagBits::eColorAttachment |
                                                 vk::ImageUsageFlagBits::eStorage,
                                               {},
                                               graphicsAndPresentQueueFamilyIndex.first,
                                               graphicsAndPresentQueueFamilyIndex.second );
    vk::SurfaceFormatKHR surfaceFormat = vk::su::pickSurfaceFormat( physicalDevice->getSurfaceFormatsKHR( **surface ) );
    vk::Format           depthFormat   = vk::raii::su::pickDepthFormat( *physicalDevice );

    // setup a render pass
    std::unique_ptr<vk::raii::RenderPass> renderPass =
      vk::raii::su::makeUniqueRenderPass( *device, surfaceFormat.format, depthFormat );

    vk::raii::su::DepthBufferData depthBufferData( *physicalDevice, *device, depthFormat, windowExtent );
    std::vector<std::unique_ptr<vk::raii::Framebuffer>> framebuffers = vk::raii::su::makeUniqueFramebuffers(
      *device, *renderPass, swapChainData.imageViews, depthBufferData.imageView, windowExtent );

    bool samplerAnisotropy = !!supportedFeatures.get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy;

    // create some simple checkerboard textures, randomly sized and colored
    const size_t                           textureCount = 10;
    std::vector<vk::raii::su::TextureData> textures;
    textures.reserve( textureCount );
    for ( size_t i = 0; i < textureCount; i++ )
    {
      textures.emplace_back( *physicalDevice,
                             *device,
                             vk::Extent2D( random<uint32_t>( 2, 8 ) * 16, random<uint32_t>( 2, 8 ) * 16 ),
                             vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                             vk::FormatFeatureFlags(),
                             samplerAnisotropy,
                             true );
    }
    vk::raii::su::oneTimeSubmit(
      *device, *perFrameData[0].commandPool, *graphicsQueue, [&]( vk::raii::CommandBuffer const & commandBuffer ) {
        for ( auto & t : textures )
        {
          t.setImage(
            commandBuffer,
            vk::su::CheckerboardImageGenerator( { random<uint8_t>(), random<uint8_t>(), random<uint8_t>() },
                                                { random<uint8_t>(), random<uint8_t>(), random<uint8_t>() } ) );
        }
      } );

    // create some materials with a random diffuse color, referencing one of the above textures
    const size_t materialCount = 10;
    assert( materialCount == textureCount );
    std::vector<Material> materials( materialCount );
    for ( size_t i = 0; i < materialCount; i++ )
    {
      materials[i].diffuse   = randomVec3( 0.0f, 1.0f );
      materials[i].textureID = vk::su::checked_cast<uint32_t>( i );
    }
    vk::raii::su::BufferData materialBufferData(
      *physicalDevice, *device, materialCount * MaterialStride, vk::BufferUsageFlagBits::eStorageBuffer );
    materialBufferData.upload( materials, MaterialStride );

    // create a a 3D-array of cubes, randomly jittered, using a random material
    std::vector<Vertex> vertices;
    vertices.reserve( xMax * yMax * zMax * cubeData.size() );
    for ( size_t x = 0; x < xMax; x++ )
    {
      for ( size_t y = 0; y < yMax; y++ )
      {
        for ( size_t z = 0; z < zMax; z++ )
        {
          int       m      = random<int>( 0, materialCount - 1 );
          glm::vec3 jitter = randomVec3( 0.0f, 0.6f );
          for ( auto const & v : cubeData )
          {
            vertices.push_back( v );
            vertices.back().pos +=
              3.0f * glm::vec3( static_cast<float>( x ), static_cast<float>( y ), static_cast<float>( z ) ) + jitter;
            vertices.back().matID = static_cast<int>( m );
          }
        }
      }
    }

    // create an 1-1 index buffer
    std::vector<unsigned int> indices( vertices.size() );
    std::iota( indices.begin(), indices.end(), 0 );

    // there's just one vertex- and one index-buffer, but with more complex scene loaders there might be more!
    vk::BufferUsageFlags bufferUsageFlags = vk::BufferUsageFlagBits::eTransferDst |
                                            vk::BufferUsageFlagBits::eVertexBuffer |
                                            vk::BufferUsageFlagBits::eStorageBuffer;
    vk::raii::su::BufferData vertexBufferData( *physicalDevice,
                                               *device,
                                               vertices.size() * VertexStride,
                                               bufferUsageFlags,
                                               vk::MemoryPropertyFlagBits::eDeviceLocal );
    vertexBufferData.upload(
      *physicalDevice, *device, *perFrameData[0].commandPool, *graphicsQueue, vertices, VertexStride );

    bufferUsageFlags = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer |
                       vk::BufferUsageFlagBits::eStorageBuffer;
    vk::raii::su::BufferData indexBufferData( *physicalDevice,
                                              *device,
                                              indices.size() * sizeof( uint32_t ),
                                              bufferUsageFlags,
                                              vk::MemoryPropertyFlagBits::eDeviceLocal );
    indexBufferData.upload(
      *physicalDevice, *device, *perFrameData[0].commandPool, *graphicsQueue, indices, sizeof( uint32_t ) );

    // clang-format off
    glm::mat4x4 transform( glm::mat4x4( 1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f ) );
    // clang-format on

    std::unique_ptr<vk::raii::DescriptorSetLayout> descriptorSetLayout = vk::raii::su::makeUniqueDescriptorSetLayout(
      *device,
      { { vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
        { vk::DescriptorType::eStorageBuffer,
          1,
          vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment },
        { vk::DescriptorType::eCombinedImageSampler,
          static_cast<uint32_t>( textures.size() ),
          vk::ShaderStageFlagBits::eFragment } } );
    std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout =
      vk::raii::su::makeUniquePipelineLayout( *device, *descriptorSetLayout );

    glslang::InitializeProcess();
    std::unique_ptr<vk::raii::ShaderModule> vertexShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eVertex, vertexShaderText );
    std::unique_ptr<vk::raii::ShaderModule> fragmentShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eFragment, fragmentShaderText );
    glslang::FinalizeProcess();

    std::unique_ptr<vk::raii::PipelineCache> pipelineCache =
      vk::raii::su::make_unique<vk::raii::PipelineCache>( *device, vk::PipelineCacheCreateInfo() );
    std::unique_ptr<vk::raii::Pipeline> graphicsPipeline = vk::raii::su::makeUniqueGraphicsPipeline(
      *device,
      *pipelineCache,
      *vertexShaderModule,
      nullptr,
      *fragmentShaderModule,
      nullptr,
      VertexStride,
      { { vk::Format::eR32G32B32Sfloat, vk::su::checked_cast<uint32_t>( offsetof( Vertex, pos ) ) },
        { vk::Format::eR32G32B32Sfloat, vk::su::checked_cast<uint32_t>( offsetof( Vertex, nrm ) ) },
        { vk::Format::eR32G32Sfloat, vk::su::checked_cast<uint32_t>( offsetof( Vertex, texCoord ) ) },
        { vk::Format::eR32Sint, vk::su::checked_cast<uint32_t>( offsetof( Vertex, matID ) ) } },
      vk::FrontFace::eCounterClockwise,
      true,
      *pipelineLayout,
      *renderPass );

    vk::raii::su::BufferData uniformBufferData(
      *physicalDevice, *device, sizeof( UniformBufferObject ), vk::BufferUsageFlagBits::eUniformBuffer );

    std::unique_ptr<vk::raii::DescriptorSet> descriptorSet =
      vk::raii::su::makeUniqueDescriptorSet( *device, *descriptorPool, *descriptorSetLayout );
    vk::raii::su::updateDescriptorSets( *device,
                                        *descriptorSet,
                                        { { vk::DescriptorType::eUniformBuffer, *uniformBufferData.buffer, {} },
                                          { vk::DescriptorType::eStorageBuffer, *materialBufferData.buffer, {} } },
                                        textures );

    // RayTracing specific stuff

    // create acceleration structures: one top-level, and just one bottom-level
    AccelerationStructureData topLevelAS, bottomLevelAS;
    vk::raii::su::oneTimeSubmit(
      *device, *perFrameData[0].commandPool, *graphicsQueue, [&]( vk::raii::CommandBuffer const & commandBuffer ) {
        vk::GeometryDataNV geometryDataNV( vk::GeometryTrianglesNV( **vertexBufferData.buffer,
                                                                    0,
                                                                    vk::su::checked_cast<uint32_t>( vertices.size() ),
                                                                    VertexStride,
                                                                    vk::Format::eR32G32B32Sfloat,
                                                                    **indexBufferData.buffer,
                                                                    0,
                                                                    vk::su::checked_cast<uint32_t>( indices.size() ),
                                                                    vk::IndexType::eUint32 ),
                                           {} );
        bottomLevelAS =
          createAccelerationStructureData( *physicalDevice,
                                           *device,
                                           commandBuffer,
                                           {},
                                           { vk::GeometryNV( vk::GeometryTypeNV::eTriangles, geometryDataNV ) } );

        topLevelAS =
          createAccelerationStructureData( *physicalDevice,
                                           *device,
                                           commandBuffer,
                                           { std::make_pair( bottomLevelAS.acclerationStructure, transform ) },
                                           std::vector<vk::GeometryNV>() );
      } );

    // create raytracing descriptor set
    vk::raii::su::oneTimeSubmit(
      *device, *perFrameData[0].commandPool, *graphicsQueue, [&]( vk::raii::CommandBuffer const & commandBuffer ) {
        vk::BufferMemoryBarrier bufferMemoryBarrier( {},
                                                     vk::AccessFlagBits::eShaderRead,
                                                     VK_QUEUE_FAMILY_IGNORED,
                                                     VK_QUEUE_FAMILY_IGNORED,
                                                     **vertexBufferData.buffer,
                                                     0,
                                                     VK_WHOLE_SIZE );
        commandBuffer.pipelineBarrier( vk::PipelineStageFlagBits::eAllCommands,
                                       vk::PipelineStageFlagBits::eAllCommands,
                                       {},
                                       nullptr,
                                       bufferMemoryBarrier,
                                       nullptr );

        bufferMemoryBarrier.buffer = **indexBufferData.buffer;
        commandBuffer.pipelineBarrier( vk::PipelineStageFlagBits::eAllCommands,
                                       vk::PipelineStageFlagBits::eAllCommands,
                                       {},
                                       nullptr,
                                       bufferMemoryBarrier,
                                       nullptr );
      } );

    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    bindings.emplace_back( 0,
                           vk::DescriptorType::eAccelerationStructureNV,
                           1,
                           vk::ShaderStageFlagBits::eRaygenNV | vk::ShaderStageFlagBits::eClosestHitNV );
    bindings.emplace_back(
      1, vk::DescriptorType::eStorageImage, 1, vk::ShaderStageFlagBits::eRaygenNV );  // raytracing output
    bindings.emplace_back(
      2, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eRaygenNV );  // camera information
    bindings.emplace_back(
      3, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eClosestHitNV );  // vertex buffer
    bindings.emplace_back(
      4, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eClosestHitNV );  // index buffer
    bindings.emplace_back(
      5, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eClosestHitNV );  // material buffer
    bindings.emplace_back( 6,
                           vk::DescriptorType::eCombinedImageSampler,
                           vk::su::checked_cast<uint32_t>( textures.size() ),
                           vk::ShaderStageFlagBits::eClosestHitNV );  // textures

    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
    descriptorPoolSizes.reserve( bindings.size() );
    for ( const auto & b : bindings )
    {
      descriptorPoolSizes.emplace_back(
        b.descriptorType, vk::su::checked_cast<uint32_t>( swapChainData.images.size() ) * b.descriptorCount );
    }
    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
      vk::su::checked_cast<uint32_t>( swapChainData.images.size() ),
      descriptorPoolSizes );
    std::unique_ptr<vk::raii::DescriptorPool> rayTracingDescriptorPool =
      vk::raii::su::make_unique<vk::raii::DescriptorPool>( *device, descriptorPoolCreateInfo );
    vk::DescriptorSetLayoutCreateInfo              descriptorSetLayoutCreateInfo( {}, bindings );
    std::unique_ptr<vk::raii::DescriptorSetLayout> rayTracingDescriptorSetLayout =
      vk::raii::su::make_unique<vk::raii::DescriptorSetLayout>( *device, descriptorSetLayoutCreateInfo );
    std::vector<vk::DescriptorSetLayout> layouts;
    for ( size_t i = 0; i < swapChainData.images.size(); i++ )
    {
      layouts.push_back( **rayTracingDescriptorSetLayout );
    }
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo( **rayTracingDescriptorPool, layouts );
    vk::raii::DescriptorSets      rayTracingDescriptorSets( *device, descriptorSetAllocateInfo );

    // Bind ray tracing specific descriptor sets into pNext of a vk::WriteDescriptorSet
    vk::WriteDescriptorSetAccelerationStructureNV writeDescriptorSetAcceleration( 1,
                                                                                  &**topLevelAS.acclerationStructure );
    std::vector<vk::WriteDescriptorSet>           accelerationDescriptionSets;
    for ( size_t i = 0; i < rayTracingDescriptorSets.size(); i++ )
    {
      accelerationDescriptionSets.emplace_back( *rayTracingDescriptorSets[i], 0, 0, 1, bindings[0].descriptorType );
      accelerationDescriptionSets.back().pNext = &writeDescriptorSetAcceleration;
    }
    device->updateDescriptorSets( accelerationDescriptionSets, nullptr );

    // Bind all the other buffers and images, starting with dstBinding == 2 (dstBinding == 1 is used by the backBuffer
    // view)
    for ( size_t i = 0; i < rayTracingDescriptorSets.size(); i++ )
    {
      vk::raii::su::updateDescriptorSets( *device,
                                          rayTracingDescriptorSets[i],
                                          { { bindings[2].descriptorType, *uniformBufferData.buffer, {} },
                                            { bindings[3].descriptorType, *vertexBufferData.buffer, {} },
                                            { bindings[4].descriptorType, *indexBufferData.buffer, {} },
                                            { bindings[5].descriptorType, *materialBufferData.buffer, {} } },
                                          textures,
                                          2 );
    }

    // create the ray-tracing shader modules
    glslang::InitializeProcess();
    std::unique_ptr<vk::raii::ShaderModule> raygenShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eRaygenNV, raygenShaderText );
    std::unique_ptr<vk::raii::ShaderModule> missShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eMissNV, missShaderText );
    std::unique_ptr<vk::raii::ShaderModule> shadowMissShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eMissNV, shadowMissShaderText );
    std::unique_ptr<vk::raii::ShaderModule> closestHitShaderModule =
      vk::raii::su::makeUniqueShaderModule( *device, vk::ShaderStageFlagBits::eClosestHitNV, closestHitShaderText );
    glslang::FinalizeProcess();

    // create the ray tracing pipeline
    std::vector<vk::PipelineShaderStageCreateInfo>     shaderStages;
    std::vector<vk::RayTracingShaderGroupCreateInfoNV> shaderGroups;

    // We use only one ray generation, that will implement the camera model
    shaderStages.emplace_back(
      vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eRaygenNV, **raygenShaderModule, "main" );
    shaderGroups.emplace_back(
      vk::RayTracingShaderGroupTypeNV::eGeneral, 0, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV );

    // The first miss shader is used to look-up the environment in case the rays from the camera miss the geometry
    shaderStages.emplace_back(
      vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eMissNV, **missShaderModule, "main" );
    shaderGroups.emplace_back(
      vk::RayTracingShaderGroupTypeNV::eGeneral, 1, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV );

    // The second miss shader is invoked when a shadow ray misses the geometry. It simply indicates that no occlusion
    // has been found
    shaderStages.emplace_back(
      vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eMissNV, **shadowMissShaderModule, "main" );
    shaderGroups.emplace_back(
      vk::RayTracingShaderGroupTypeNV::eGeneral, 2, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV );

    // The first hit group defines the shaders invoked when a ray shot from the camera hit the geometry. In this case we
    // only specify the closest hit shader, and rely on the build-in triangle intersection and pass-through any-hit
    // shader. However, explicit intersection and any hit shaders could be added as well.
    shaderStages.emplace_back(
      vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eClosestHitNV, **closestHitShaderModule, "main" );
    shaderGroups.emplace_back( vk::RayTracingShaderGroupTypeNV::eTrianglesHitGroup,
                               VK_SHADER_UNUSED_NV,
                               3,
                               VK_SHADER_UNUSED_NV,
                               VK_SHADER_UNUSED_NV );

    // The second hit group defines the shaders invoked when a shadow ray hits the geometry. For simple shadows we do
    // not need any shader in that group: we will rely on initializing the payload and update it only in the miss shader
    shaderGroups.emplace_back( vk::RayTracingShaderGroupTypeNV::eTrianglesHitGroup,
                               VK_SHADER_UNUSED_NV,
                               VK_SHADER_UNUSED_NV,
                               VK_SHADER_UNUSED_NV,
                               VK_SHADER_UNUSED_NV );

    // Create the layout of the pipeline following the provided descriptor set layout
    std::unique_ptr<vk::raii::PipelineLayout> rayTracingPipelineLayout =
      vk::raii::su::makeUniquePipelineLayout( *device, *rayTracingDescriptorSetLayout );

    // Assemble the shader stages and recursion depth info into the raytracing pipeline
    // The ray tracing process can shoot rays from the camera, and a shadow ray can be shot from the
    // hit points of the camera rays, hence a recursion level of 2. This number should be kept as low
    // as possible for performance reasons. Even recursive ray tracing should be flattened into a loop
    // in the ray generation to avoid deep recursion.
    uint32_t                           maxRecursionDepth = 2;
    vk::RayTracingPipelineCreateInfoNV rayTracingPipelineCreateInfo(
      {}, shaderStages, shaderGroups, maxRecursionDepth, **rayTracingPipelineLayout );
    std::unique_ptr<vk::raii::Pipeline> rayTracingPipeline =
      vk::raii::su::make_unique<vk::raii::Pipeline>( *device, nullptr, rayTracingPipelineCreateInfo );
    switch ( rayTracingPipeline->getConstructorSuccessCode() )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::ePipelineCompileRequiredEXT:
        // something meaningfull here
        break;
      default: assert( false );  // should never happen
    }

    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesNV> propertiesChain =
      physicalDevice->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesNV>();
    uint32_t shaderGroupBaseAlignment =
      propertiesChain.get<vk::PhysicalDeviceRayTracingPropertiesNV>().shaderGroupBaseAlignment;
    uint32_t shaderGroupHandleSize =
      propertiesChain.get<vk::PhysicalDeviceRayTracingPropertiesNV>().shaderGroupHandleSize;

    uint32_t raygenShaderBindingOffset = 0;                      // starting with raygen
    uint32_t raygenShaderTableSize     = shaderGroupHandleSize;  // one raygen shader
    uint32_t missShaderBindingOffset =
      raygenShaderBindingOffset + roundUp( raygenShaderTableSize, shaderGroupBaseAlignment );
    uint32_t missShaderBindingStride = shaderGroupHandleSize;
    uint32_t missShaderTableSize     = 2 * missShaderBindingStride;  // two raygen shaders
    uint32_t hitShaderBindingOffset =
      missShaderBindingOffset + roundUp( missShaderTableSize, shaderGroupBaseAlignment );
    uint32_t hitShaderBindingStride = shaderGroupHandleSize;
    uint32_t hitShaderTableSize     = 2 * hitShaderBindingStride;  // two hit shaders

    uint32_t             shaderBindingTableSize = hitShaderBindingOffset + hitShaderTableSize;
    std::vector<uint8_t> shaderHandleStorage( shaderBindingTableSize );
    memcpy( &shaderHandleStorage[raygenShaderBindingOffset],
            rayTracingPipeline->getRayTracingShaderGroupHandlesKHR<uint8_t>( 0, 1, raygenShaderTableSize ).data(),
            raygenShaderTableSize );
    memcpy( &shaderHandleStorage[missShaderBindingOffset],
            rayTracingPipeline->getRayTracingShaderGroupHandlesKHR<uint8_t>( 1, 2, missShaderTableSize ).data(),
            missShaderTableSize );
    memcpy( &shaderHandleStorage[hitShaderBindingOffset],
            rayTracingPipeline->getRayTracingShaderGroupHandlesKHR<uint8_t>( 3, 2, hitShaderTableSize ).data(),
            hitShaderTableSize );
    assert( shaderHandleStorage.size() == shaderBindingTableSize );

    vk::raii::su::BufferData shaderBindingTableBufferData( *physicalDevice,
                                                           *device,
                                                           shaderBindingTableSize,
                                                           vk::BufferUsageFlagBits::eTransferDst,
                                                           vk::MemoryPropertyFlagBits::eHostVisible );
    shaderBindingTableBufferData.upload( shaderHandleStorage );

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.2f, 0.2f, 0.2f, 0.2f } } ) );
    clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    // Main loop
    uint32_t            frameIndex = 0;
    UniformBufferObject uniformBufferObject;
    uniformBufferObject.model   = glm::mat4( 1 );
    uniformBufferObject.modelIT = glm::inverseTranspose( uniformBufferObject.model );

    double accumulatedTime{ 0.0 };
    size_t frameCount{ 0 };
    while ( !glfwWindowShouldClose( window ) )
    {
      double startTime = glfwGetTime();
      glfwPollEvents();

      vk::raii::CommandBuffer const & commandBuffer = *perFrameData[frameIndex].commandBuffer;

      int w, h;
      glfwGetWindowSize( window, &w, &h );
      if ( ( w != static_cast<int>( windowExtent.width ) ) || ( h != static_cast<int>( windowExtent.height ) ) )
      {
        windowExtent.width  = w;
        windowExtent.height = h;
        device->waitIdle();
        swapChainData =
          vk::raii::su::SwapChainData( *physicalDevice,
                                       *device,
                                       *surface,
                                       windowExtent,
                                       vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage,
                                       swapChainData.swapChain,
                                       graphicsAndPresentQueueFamilyIndex.first,
                                       graphicsAndPresentQueueFamilyIndex.second );
        depthBufferData = vk::raii::su::DepthBufferData(
          *physicalDevice, *device, vk::su::pickDepthFormat( **physicalDevice ), windowExtent );

        vk::raii::su::oneTimeSubmit(
          commandBuffer, *graphicsQueue, [&]( vk::raii::CommandBuffer const & commandBuffer ) {
            vk::raii::su::setImageLayout( commandBuffer,
                                          **depthBufferData.image,
                                          depthFormat,
                                          vk::ImageLayout::eUndefined,
                                          vk::ImageLayout::eDepthStencilAttachmentOptimal );
          } );

        framebuffers = vk::raii::su::makeUniqueFramebuffers(
          *device, *renderPass, swapChainData.imageViews, depthBufferData.imageView, windowExtent );
      }

      // update the uniformBufferObject
      assert( 0 < windowExtent.height );
      uniformBufferObject.view = appInfo.cameraManipulator.getMatrix();
      uniformBufferObject.proj = glm::perspective(
        glm::radians( 65.0f ), windowExtent.width / static_cast<float>( windowExtent.height ), 0.1f, 1000.0f );
      uniformBufferObject.proj[1][1] *= -1;  // Inverting Y for Vulkan
      uniformBufferObject.viewInverse = glm::inverse( uniformBufferObject.view );
      uniformBufferObject.projInverse = glm::inverse( uniformBufferObject.proj );
      uniformBufferData.upload( uniformBufferObject );

      // frame begin
      vk::Result result;
      uint32_t   backBufferIndex;
      std::tie( result, backBufferIndex ) = swapChainData.swapChain->acquireNextImage(
        vk::su::FenceTimeout, **perFrameData[frameIndex].presentCompleteSemaphore );
      assert( result == vk::Result::eSuccess );

      while ( vk::Result::eTimeout ==
              device->waitForFences( { **perFrameData[frameIndex].fence }, VK_TRUE, vk::su::FenceTimeout ) )
        ;
      device->resetFences( { **perFrameData[frameIndex].fence } );

      commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit ) );

      if ( appInfo.useRasterRender )
      {
        vk::RenderPassBeginInfo renderPassBeginInfo( **renderPass,
                                                     **framebuffers[backBufferIndex],
                                                     vk::Rect2D( vk::Offset2D( 0, 0 ), windowExtent ),
                                                     clearValues );
        commandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );

        commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, **graphicsPipeline );
        commandBuffer.bindDescriptorSets(
          vk::PipelineBindPoint::eGraphics, **pipelineLayout, 0, { **descriptorSet }, nullptr );

        commandBuffer.setViewport( 0,
                                   vk::Viewport( 0.0f,
                                                 0.0f,
                                                 static_cast<float>( windowExtent.width ),
                                                 static_cast<float>( windowExtent.height ),
                                                 0.0f,
                                                 1.0f ) );
        commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), windowExtent ) );

        commandBuffer.bindVertexBuffers( 0, { **vertexBufferData.buffer }, { 0 } );
        commandBuffer.bindIndexBuffer( **indexBufferData.buffer, 0, vk::IndexType::eUint32 );
        commandBuffer.drawIndexed( vk::su::checked_cast<uint32_t>( indices.size() ), 1, 0, 0, 0 );

        commandBuffer.endRenderPass();
      }
      else
      {
        vk::DescriptorImageInfo imageInfo(
          nullptr, *swapChainData.imageViews[backBufferIndex], vk::ImageLayout::eGeneral );
        vk::WriteDescriptorSet writeDescriptorSet(
          *rayTracingDescriptorSets[backBufferIndex], 1, 0, bindings[1].descriptorType, imageInfo );
        device->updateDescriptorSets( writeDescriptorSet, nullptr );

        vk::raii::su::setImageLayout( commandBuffer,
                                      static_cast<vk::Image>( swapChainData.images[backBufferIndex] ),
                                      surfaceFormat.format,
                                      vk::ImageLayout::eUndefined,
                                      vk::ImageLayout::eGeneral );

        commandBuffer.bindPipeline( vk::PipelineBindPoint::eRayTracingNV, **rayTracingPipeline );

        commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eRayTracingNV,
                                          **rayTracingPipelineLayout,
                                          0,
                                          { *rayTracingDescriptorSets[backBufferIndex] },
                                          nullptr );

        commandBuffer.traceRaysNV( **shaderBindingTableBufferData.buffer,
                                   raygenShaderBindingOffset,
                                   **shaderBindingTableBufferData.buffer,
                                   missShaderBindingOffset,
                                   missShaderBindingStride,
                                   **shaderBindingTableBufferData.buffer,
                                   hitShaderBindingOffset,
                                   hitShaderBindingStride,
                                   nullptr,
                                   0,
                                   0,
                                   windowExtent.width,
                                   windowExtent.height,
                                   1 );

        vk::raii::su::setImageLayout( commandBuffer,
                                      static_cast<vk::Image>( swapChainData.images[backBufferIndex] ),
                                      surfaceFormat.format,
                                      vk::ImageLayout::eGeneral,
                                      vk::ImageLayout::ePresentSrcKHR );
      }

      // frame end
      commandBuffer.end();
      const vk::PipelineStageFlags waitDestinationStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
      vk::SubmitInfo               submitInfo( **perFrameData[frameIndex].presentCompleteSemaphore,
                                 waitDestinationStageMask,
                                 *commandBuffer,
                                 **perFrameData[frameIndex].renderCompleteSemaphore );
      graphicsQueue->submit( submitInfo, **perFrameData[frameIndex].fence );
      vk::PresentInfoKHR presentInfoKHR(
        **perFrameData[frameIndex].renderCompleteSemaphore, **swapChainData.swapChain, backBufferIndex );
      result = presentQueue->presentKHR( presentInfoKHR );
      switch ( result )
      {
        case vk::Result::eSuccess: break;
        case vk::Result::eSuboptimalKHR:
          std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
          break;
        default: assert( false );  // an unexpected result is returned !
      }
      frameIndex = ( frameIndex + 1 ) % IMGUI_VK_QUEUED_FRAMES;

      double endTime = glfwGetTime();
      accumulatedTime += endTime - startTime;
      ++frameCount;
      if ( 1.0 < accumulatedTime )
      {
        assert( 0 < frameCount );

        std::ostringstream oss;
        oss << AppName << ": " << vertices.size() << " Vertices "
            << ( appInfo.useRasterRender ? "Rastering" : "RayTracing" ) << " ( " << frameCount / accumulatedTime
            << " fps)";
        glfwSetWindowTitle( window, oss.str().c_str() );

        accumulatedTime = 0.0;
        frameCount      = 0;
      }
    }

    // Cleanup
    device->waitIdle();

    glfwDestroyWindow( window );
    glfwTerminate();
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }
  return 0;
}
