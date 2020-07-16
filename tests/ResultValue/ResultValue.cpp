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
#include <memory>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

void intByValue( int ) {}
void intByReference( int & ) {}
void intByConstantReference( int const & ) {}
void intByRValueReference( int && ) {}
void intByConstantRValueReference( int const && ) {}

void uniqueIntByValue( std::unique_ptr<int> ) {}
void uniqueIntByReference( std::unique_ptr<int> & ) {}
void uniqueIntByConstantReference( std::unique_ptr<int> const & ) {}
void uniqueIntByRValueReference( std::unique_ptr<int> && ) {}
void uniqueIntByConstantRValueReference( std::unique_ptr<int> const && ) {}

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

  int i = 42;
  intByValue( i );
  intByReference( i );
  intByConstantReference( i );
  intByRValueReference( std::move( i ) );
  intByConstantRValueReference( std::move( i ) );

  const int ci = 42;
  intByValue( ci );
  // intByReference( ci );   // does not compile: "cannot convert argument 1 from 'const int' to 'int &'"
  intByConstantReference( ci );
  // intByRValueReference( std::move( ci ) );    // does not compile with "cannot convert argument 1 from 'const int' to
  // 'int &&'"
  intByConstantRValueReference( std::move( ci ) );

  std::unique_ptr<int> upi = std::unique_ptr<int>( new int(42) );
  // uniqueIntByValue( upi );    // does not compile: "attempting to reference a deleted function"
  uniqueIntByReference( upi );
  uniqueIntByConstantReference( upi );
  uniqueIntByRValueReference( std::move( upi ) );
  uniqueIntByConstantRValueReference( std::move( upi ) );

  vk::ResultValue<int> rvi( {}, 42 );
  intByValue( rvi );
  intByReference( rvi );
  intByConstantReference( rvi );
  intByRValueReference( std::move( rvi ) );
  intByConstantRValueReference( std::move( rvi ) );

  const vk::ResultValue<int> crvi( {}, 42 );
  intByValue( crvi );
  // intByReference(crvi);                        // should not compile
  intByConstantReference( crvi );
  // intByRValueReference( std::move( crvi ) );   // should not compile
  intByConstantRValueReference( std::move( crvi ) );

  const vk::ResultValue<std::vector<int>> crvvi( {}, { 1, 2, 3 } );
  intVectorByValue( crvvi );
  // intVectorByReference( crvvi );                       // should not compile
  intVectorByConstantReference( crvvi );
  // intVectorByRValueReference( std::move( crvvi ) );   // should not compiler
  intVectorByConstantRValueReference( std::move( crvvi ) );

  vk::Pipeline pipe( VkPipeline( 0x8 ) );  // fake a Pipeline here, to have something different from zero
  pipelineByValue( pipe );
  pipelineByReference( pipe );
  pipelineByConstantReference( pipe );
  pipelineByRValueReference( std::move( pipe ) );
  pipelineByConstantRValueReference( std::move( pipe ) );

  vk::ResultValue<vk::Pipeline> rv( {}, pipe );

  vk::Pipeline p0 = rv;
  vk::Pipeline p1;
  p1 = rv;

  pipelineByValue( rv );
  pipelineByReference( rv );
  pipelineByConstantReference( rv );
  pipelineByRValueReference( std::move( rv ) );
  pipelineByConstantRValueReference( std::move( rv ) );

  vk::UniquePipeline pipeline( pipe );
  // uniquePipelineByValue( pipeline );    // should not compile
  uniquePipelineByReference( pipeline );
  uniquePipelineByConstantReference( pipeline );
  pipeline = uniquePipelineByRValueReference( std::move( pipeline ) );
  uniquePipelineByConstantRValueReference( std::move( pipeline ) );

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

  //uniquePipelineByValue( rvu );   // should not compile

  uniquePipelineByReference( rvu );  // does not move out handle
  assert( rvu.value );

  uniquePipelineByConstantReference( rvu );  // does not move out handle
  assert( rvu.value );

  up1 = uniquePipelineByRValueReference( std::move( rvu ) );
  assert( up1 && !rvu.value );
  rvu.value = std::move( up1 );
  assert( !up1 && rvu.value );

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
    pipelines2 = device->createGraphicsPipelines( nullptr, {} );    // !!

    vk::UniquePipeline pipeline3 = device->createGraphicsPipelineUnique( nullptr, {} );

    vk::UniquePipeline pipeline4;
    pipeline4 = std::move(device->createGraphicsPipelineUnique( nullptr, {} ));

    std::vector<vk::UniquePipeline> pipelines3 = device->createGraphicsPipelinesUnique( nullptr, {} );

    std::vector<vk::UniquePipeline> pipelines4;
    pipelines4 = device->createGraphicsPipelinesUnique( nullptr, {} );
  }

  return 0;
}
