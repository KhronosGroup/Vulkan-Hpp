// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : ArrayWrapper
//                     Compile test on using vk::ArrayWrapper1D

#include <iostream>
#include <string>
#include <vulkan/vulkan.hpp>

void f( std::string const & s )
{
  std::cout << "<" << s << ">" << std::endl;
}

int main( int /*argc*/, char ** /*argv*/ )
{
  vk::ArrayWrapper1D<char, 10> awc1( { 'f', 'o', 'o', 'b', 'a', 'h' } );
  std::string                  s1 = awc1;
  assert( s1.length() == 6 );
  std::cout << "<" << awc1 << ">" << std::endl;

  // s1 = awc1;  // 'operator =' is ambiguous

  std::string foobah = "foobah";

  vk::ArrayWrapper1D<char, 20> awc2( foobah );
  f( awc2 );

  vk::ArrayWrapper1D<char, 5> awc3( { 'f', 'o', 'o', 'b', 'a', 'h' } );
  std::string                 s3 = awc3;
  assert( s3.length() == 4 );
  std::cout << "<" << s3 << ">" << std::endl;

  vk::ArrayWrapper1D<char, 5> awc4( foobah );
  std::string                 s4 = awc4;
  assert( s4.length() == 4 );

#if 17 <= VULKAN_HPP_CPP_VERSION
  std::cout << std::boolalpha << std::is_convertible_v<vk::ArrayWrapper1D<char, 10>, std::string_view> << std::endl;

  std::string_view sv1 = awc1;
  assert( sv1.size() == 6 );
  sv1 = awc2;
  assert( sv1.size() == 6 );
  sv1 = awc3;
  assert( sv1.size() == 5 );

  vk::ArrayWrapper1D<char, 8> awc5( sv1 );
#endif

  assert( awc1 == awc2 );
  assert( awc3 == awc4 );
  assert( foobah == awc2 );
  assert( foobah > awc4 );

  return 0;
}
