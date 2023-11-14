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
  vk::ArrayWrapper1D<char, 10> aw1( { 'f', 'o', 'o', 'b', 'a', 'h' } );
  std::string                  s1 = aw1;
  assert( s1.length() == 6 );
  std::cout << "<" << aw1 << ">" << std::endl;

  // s1 = aw1;  // 'operator =' is ambiguous

  std::string foobah = "foobah";

  vk::ArrayWrapper1D<char, 20> aw2( foobah );
  f( aw2 );

  vk::ArrayWrapper1D<char, 5> aw3( { 'f', 'o', 'o', 'b', 'a', 'h' } );
  std::string                 s3 = aw3;
  assert( s3.length() == 5 );
  std::cout << "<" << s3 << ">" << std::endl;

  vk::ArrayWrapper1D<char, 5> aw4( foobah );
  std::string                 s4 = aw4;
  assert( s4.length() == 5 );

#if 17 <= VULKAN_HPP_CPP_VERSION
  std::cout << std::boolalpha << std::is_convertible_v<vk::ArrayWrapper1D<char, 10>, std::string_view> << std::endl;

  std::string_view sv1 = aw1;
  assert( sv1.size() == 6 );
  sv1 = aw2;
  assert( sv1.size() == 6 );
  sv1 = aw3;
  assert( sv1.size() == 5 );

  vk::ArrayWrapper1D<char, 8> aw5( sv1 );
#endif

  return 0;
}
