// Copyright(c) 2020, Collabora Ltd. All rights reserved.
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
// VulkanHpp Tests : ResultValue
//                   Compile-test for ResultValue

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "vulkan/vulkan.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

void intByValue( int ) {}
void intByReference( int & ) {}
void intByConstantReference( const int & ) {}
void intByRValueReference( int && ) {}
void intByConstantRValueReference( const int && ) {}

void intVectorByValue( std::vector<int> ) {}
void intVectorByReference( std::vector<int> & ) {}
void intVectorByConstantReference( std::vector<int> const & ) {}
void intVectorByRValueReference( std::vector<int> && ) {}
void intVectorByConstantRValueReference( std::vector<int> const && ) {}

void pipelineByValue( vk::Pipeline ) {}
void pipelineByReference( vk::Pipeline & ) {}
void pipelineByConstantReference( vk::Pipeline const & ) {}
void pipelineByRValueReference( vk::Pipeline && ) {}
void pipelineByConstantRValueReference( vk::Pipeline const && ) {}

void               uniquePipelineByValue( vk::UniquePipeline ) {}
void               uniquePipelineByReference( vk::UniquePipeline & ) {}
void               uniquePipelineByConstantReference( vk::UniquePipeline const & ) {}
vk::UniquePipeline uniquePipelineByRValueReference( vk::UniquePipeline && up )
{
  return std::move( up );
}
void uniquePipelineByConstantRValueReference( vk::UniquePipeline const && ) {}

int main( int /*argc*/, char ** /*argv*/ )
{
#if defined( VULKAN_DISABLE_IMPLICIT_RESULT_VALUE_CAST )
  static_assert( false, "Conversions not enabled" );
#endif

  vk::ResultValue<int> rvi( {}, 42 );
  intByValue( rvi );
  intByReference( rvi );
  intByConstantReference( rvi );
  intByRValueReference( std::move( rvi ) );
  intByConstantRValueReference( std::move( rvi ) );

  const vk::ResultValue<int> crvi( {}, 42 );
  intByValue( crvi );
  // intByReference(crvi); // should fail with "cannot convert argument 1 from 'const vk::ResultValue<int>' to 'int &'"
  intByConstantReference( crvi );
  intByRValueReference( std::move( crvi ) );
  intByConstantRValueReference( std::move( crvi ) );

  vk::ResultValue<std::vector<int>> rvvi( {}, { 1, 2, 3 } );
  intVectorByValue( rvvi );
  intVectorByReference( rvvi );
  intVectorByConstantReference( rvvi );
  intVectorByRValueReference( std::move( rvvi ) );
  intVectorByConstantRValueReference( std::move( rvvi ) );

  const vk::ResultValue<std::vector<int>> crvvi( {}, { 1, 2, 3 } );
  intVectorByValue( crvvi );

  // should fail with "cannot convert argument 1 from 'const vk::ResultValue<std::vector<int,std::allocator<int>>>' to
  // 'std::vector<int,std::allocator<int>> &'"
  // intVectorByReference( crvvi );

  intVectorByConstantReference( crvvi );

  // should fail with "cannot convert argument 1 from 'const vk::ResultValue<std::vector<int,std::allocator<int>>>' to
  // 'std::vector<int,std::allocator<int>> &&'"
  // intVectorByRValueReference( std::move( crvvi ) );

  intVectorByConstantRValueReference( std::move( crvvi ) );

  vk::Pipeline pipe( VkPipeline( 0x8 ) );  // fake a Pipeline here, to have something different from zero
  vk::ResultValue<vk::Pipeline> rv( {}, pipe );

  vk::Pipeline p0 = rv;
  vk::Pipeline p1;
  p1 = rv;

  pipelineByValue( rv );
  pipelineByReference( rv );
  pipelineByConstantReference( rv );

  // should not compile with "cannot convert argument 1 from 'vk::ResultValue<vk::Pipeline>' to 'vk::Pipeline &&'"
  // pipelineByRValueReference( rv );

  pipelineByRValueReference( std::move( rv ) );

  // should not compile with "cannot convert argument 1 from 'vk::ResultValue<vk::Pipeline>' to 'const vk::Pipeline &&'"
  // pipelineByConstantRValueReference( rv );

  pipelineByConstantRValueReference( std::move( rv ) );

  vk::UniquePipeline                  pipeline( pipe );
  vk::ResultValue<vk::UniquePipeline> rvu( {}, std::move( pipeline ) );

  vk::UniquePipeline up0 = std::move( rvu );
  assert( up0 && !rvu.value );
  rvu.value = std::move( up0 );
  assert( !up0 && rvu.value );

  vk::UniquePipeline up1;
  up1 = std::move( rvu );
  assert( up1 && !rvu.value );
  rvu.value = std::move( up1 );
  assert( !up1 && rvu.value );

  // should not compile with "cannot convert argument 1 from 'vk::ResultValue<vk::UniquePipeline>' to
  // 'vk::UniquePipeline'"
  // uniquePipelineByValue( rvu );

  uniquePipelineByReference( rvu );  // does not move out handle
  assert( rvu.value );

  uniquePipelineByConstantReference( rvu );  // does not move out handle
  assert( rvu.value );

  // should not compile with "cannot convert argument 1 from 'vk::ResultValue<vk::UniquePipeline>' to
  // 'vk::UniquePipeline &&'"
  // uniquePipelineByRValueReference( rvu );

  vk::UniquePipeline up = uniquePipelineByRValueReference( std::move( rvu ) );
  assert( up && !rvu.value );
  rvu.value = std::move( up );

  // should not compile with something like "cannot convert argument 1 from
  // 'vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>' to 'const vk::UniquePipeline &&'"
  // but, well, it compiles; accepted, for now
  // uniquePipelineByConstantRValueReference( rvu );

  if ( false )
  {
    // this compiles, but would not survive destruction of the faked pipeline
    uniquePipelineByConstantRValueReference( std::move( rvu ) );
  }

  auto p = std::move( rvu.value );
  p.release();  // release the faked Pipeline, to prevent error on trying to destroy it

  if ( false )
  {
    // this is just a compile test!
    vk::UniqueDevice device;

    vk::Pipeline pipeline1 = device->createGraphicsPipeline( nullptr, {} );

    vk::Pipeline pipeline2;
    pipeline2 = device->createGraphicsPipeline( nullptr, {} );

    std::vector<vk::Pipeline> pipelines1 = device->createGraphicsPipelines( nullptr, {} );

    std::vector<vk::Pipeline> pipelines2;
    pipelines2 = device->createGraphicsPipelines( nullptr, {} );

    vk::UniquePipeline pipeline3 = device->createGraphicsPipelineUnique( nullptr, {} );

    vk::UniquePipeline pipeline4;
    pipeline4 = device->createGraphicsPipelineUnique( nullptr, {} );

    std::vector<vk::UniquePipeline> pipelines3 = device->createGraphicsPipelinesUnique( nullptr, {} );

    std::vector<vk::UniquePipeline> pipelines4;
    pipelines4 = device->createGraphicsPipelinesUnique( nullptr, {} );
  }

  return 0;
}
