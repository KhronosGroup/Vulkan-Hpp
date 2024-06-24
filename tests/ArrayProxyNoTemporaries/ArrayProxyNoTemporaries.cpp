// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
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
// VulkanHpp Samples : ArrayProxyNoTemporaries
//                     Compile test on using vk::ArrayProxyNoTemporaries

#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __clang__ )
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

#include <iostream>
#include <vulkan/vulkan.hpp>

void fct( vk::ArrayProxyNoTemporaries<int> /*ap*/ ) {}

void fctc( vk::ArrayProxyNoTemporaries<const int> /*ap*/ ) {}

int getInt()
{
  return 1;
}

int ( &&getArrayReference() )[2]
{
  static int arr[2] = { 1, 2 };
  return std::move( arr );
}

int const ( &&getConstArrayReference() )[2]
{
  static int const arr[2] = { 1, 2 };
  return std::move( arr );
}

std::array<int, 2> getArray()
{
  return { 1, 2 };
}

std::array<int, 2> const getConstArray()
{
  return { 1, 2 };
}

std::vector<int> getVector()
{
  return { 1, 2 };
}

std::vector<int> const getConstVector()
{
  return { 1, 2 };
}

std::initializer_list<int> getInitializerList()
{
  return { 1, 2 };
}

std::initializer_list<int> const getConstInitializerList()
{
  return { 1, 2 };
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // no args
    fct( {} );
    fctc( {} );

    // nullptr_t
    fct( nullptr );
    fctc( nullptr );

    vk::ArrayProxyNoTemporaries<int> apnt1 = nullptr;
    assert( apnt1.size() == 0 );

    // Type
    // fct(2);   // not supported: cannot convert argument 1 from 'int' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc(1);  // not supported: cannot convert argument 1 from 'int' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    // getInt()
    // fct( getInt() );    // not supported: cannot convert argument 1 from 'int' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getInt() );   // not supported: cannot convert argument 1 from 'int' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    int i0 = 1;
    fct( i0 );
    fctc( i0 );

    const int i1 = 2;
    // fct(i1);   // not supported: cannot convert from 'const int *' to 'T *
    fctc( i1 );

    // vk::ArrayProxyNoTemporaries<int> apnt2 = 1;       // not supported: cannot convert from 'int' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<const int> apnt3 = 1;   // not supported: cannot convert from 'int' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    vk::ArrayProxyNoTemporaries<const int> apnt4 = i0;
    assert( apnt4.size() == 1 );
    vk::ArrayProxyNoTemporaries<const int> apnt5 = i1;
    assert( apnt5.size() == 1 );

    // count, T *
    int * i0p = &i0;
    fct( { 1, i0p } );
    fctc( { 1, i0p } );

    // count, T const*
    int const * i1p = &i1;
    // fct( { 1, i1p } );  // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( { 1, i1p } );

    vk::ArrayProxyNoTemporaries<const int> apnt6 = { 1, i0p };
    assert( apnt6.size() == 1 );
    vk::ArrayProxyNoTemporaries<const int> apnt7 = { 1, i1p };
    assert( apnt7.size() == 1 );

    // T[count]
    int ia0[2] = { 0, 1 };
    fct( ia0 );
    fctc( ia0 );

    // const T[count]
    const int ia1[2] = { 0, 1 };
    // fct( ia1 );   // not supported: cannot convert argument 1 from 'const int [2]' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( ia1 );

    vk::ArrayProxyNoTemporaries<const int> apnt8 = ia0;
    assert( apnt8.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt9 = ia1;
    assert( apnt9.size() == 2 );

    // getArrayReference
    // fct( getConstArrayReference() );    // not supported: cannot convert argument 1 from 'const int [2]' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getConstArrayReference() );   // not supported: attempting to reference a deleted function
    // fct( getArrayReference() );         // not supported: attempting to reference a deleted function
    // fctc( getArrayReference() );        // not supported: attempting to reference a deleted function

    // std::array<T,N>
    std::array<int, 2> sa0 = { 0, 1 };
    fct( sa0 );
    fctc( sa0 );

    // std::array<const T,N>
    std::array<const int, 2> sa1 = { 0, 1 };
    // fct( sa1 );   // not supported: cannot convert argument 1 from 'std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( sa1 );

    // std::array<T,N> const
    std::array<int, 2> const sa2 = { 1, 2 };
    // fct( sa2 );  // not supported: cannot convert argument 1 from 'const std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( sa2 );

    // std::array<const T,N> const
    std::array<const int, 2> const sa3 = { 1, 2 };
    // fct( sa3 );  // not supported: cannot convert argument 1 from 'const std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( sa3 );

    // getArray
    // fct( getConstArray() );    // not supported: cannot convert argument 1 from 'const std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getConstArray() );   // not supported: cannot convert argument 1 from 'const std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<const int32_t>'
    // fct( getArray() );         // not supported: cannot convert argument 1 from 'std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getArray() );        // not supported: cannot convert argument 1 from 'std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    // from std::array constructors
    vk::ArrayProxyNoTemporaries<int> apnt10 = sa0;
    assert( apnt10.size() == 2 );

    // vk::ArrayProxyNoTemporaries<int> apnt11 = sa1;   // not supported: cannot convert from 'std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<int> apnt12 = sa2;  // not supported: cannot convert from 'const std::array<int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<int> apnt13 = sa3;  // not supported: cannot convert from 'const std::array<const int,2>' to
    // 'vk::ArrayProxyNoTemporaries<int>'

    vk::ArrayProxyNoTemporaries<const int> apnt14 = sa0;
    assert( apnt14.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt15 = sa1;
    assert( apnt15.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt16 = sa2;
    assert( apnt16.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt17 = sa3;
    assert( apnt17.size() == 2 );

    // std::vector<T>
    std::vector<int> sv0 = { 0, 1 };
    fct( sv0 );
    fctc( sv0 );

    // std::vector<T> const
    std::vector<int> const sv1 = { 0, 1 };
    // fct( sv1 );  // not supported:  cannot convert argument 1 from 'const std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( sv1 );

    // getVector
    // fct( getConstVector() );    // not supported: cannot convert argument 1 from 'const std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<int>' 
    // fctc( getConstVector() );   // not supported: cannot convert argument 1 from 'const std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<const int32_t>' 
    // fct( getVector() );         // not supported: cannot convert argument 1 from 'std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<int>' 
    // fctc( getVector() );        // not supported: cannot convert argument 1 from 'std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    vk::ArrayProxyNoTemporaries<int> apnt18 = sv0;
    assert( apnt18.size() == 2 );

    // vk::ArrayProxyNoTemporaries<int> apnt19 = sv1;  // not supported: cannot convert from 'const std::vector<int,std::allocator<int>>' to 'vk::ArrayProxyNoTemporaries<int>'

    vk::ArrayProxyNoTemporaries<const int> apnt20 = sv0;
    assert( apnt20.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt21 = sv1;
    assert( apnt21.size() == 2 );

    // std::initializer_list
    // fct( { 0, 1 } );   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( { 0, 1 } );  // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    // int a = 0;
    // int b = 1;
    // fct( { a, b } );   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( { a, b } );  // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    auto il0 = { 0, 1 };  // -> std::initializer_list<int>
    // fct( il0 );        // not supported: cannot convert from 'const int *' to 'int *'
    fctc( il0 );

    std::initializer_list<int> il1 = { 0, 1 };
    // fct( il1 );  // not supported: cannot convert argument 1 from 'std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( il1 );

    std::initializer_list<const int> il2 = { 0, 1 };
    // fct( il2 );  // not supported: cannot convert argument 1 from 'std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( il2 );

    std::initializer_list<int> const il3 = { 0, 1 };
    // fct( il3 );  // not supported: cannot convert argument 1 from 'const std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( il3 );

    std::initializer_list<const int> const il4 = { 0, 1 };
    // fct( il4 );  // not supported: cannot convert argument 1 from 'const std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( il4 );

    // getInitializerList
    // fct( getConstInitializerList() );     // not supported: cannot convert argument 1 from 'const std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getConstInitializerList() );    // not supported: cannot convert argument 1 from 'const std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<const int32_t>'
    // fct( getInitializerList() );          // not supported: cannot convert argument 1 from 'std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc( getInitializerList() );         // not supported: cannot convert argument 1 from 'std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    // vk::ArrayProxyNoTemporaries<int> apnt22 = il1;   // not supported: cannot convert from 'std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<int> apnt23 = il2;   // not supported: cannot convert from 'std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<int> apnt24 = il3;   // not supported: cannot convert from 'const std::initializer_list<_Ty>' to 'vk::ArrayProxyNoTemporaries<int>'
    // vk::ArrayProxyNoTemporaries<int> apnt25 = il4;   // not supported: cannot convert from 'const std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'

    vk::ArrayProxyNoTemporaries<const int> apnt26 = {};
    assert( apnt26.size() == 0 );

    // vk::ArrayProxyNoTemporaries<const int> apnt27 = { 0, 1 };   // not supported: cannot convert from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    vk::ArrayProxyNoTemporaries<const int> apnt28 = il1;
    assert( apnt28.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt29 = il2;
    assert( apnt29.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt30 = il3;
    assert( apnt30.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> apnt31 = il4;
    assert( apnt31.size() == 2 );
  }
  catch ( vk::SystemError const & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  return 0;
}
