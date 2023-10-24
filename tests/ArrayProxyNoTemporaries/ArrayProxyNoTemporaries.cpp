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

int( &&getArrayReference() )[2]
{
  static int arr[2] = { 1, 2 };
  return std::move( arr );
}

int const( &&getConstArrayReference() )[2]
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

//std::initializer_list<int> getInitializerList()
//{
//  return { 1, 2 };
//}

//std::initializer_list<int> const getConstInitializerList()
//{
//  return { 1, 2 };
//}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // nullptr_t
    fct( nullptr );
    fctc( nullptr );

    vk::ArrayProxyNoTemporaries<int> ap0 = nullptr;
    assert( ap0.size() == 0 );

    // Type
    // fct(2);   // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<int>(int &&)
    // fctc(1);  // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<T,0>(int &&)

    // getInt()
    // fct( getInt() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<int>(int &&)
    // fctc( getInt() );   // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<T,0>(int &&)

    int i0 = 1;
    fct( i0 );
    fctc( i0 );

    const int i1 = 2;
    // fct(i1);   // not supported: ArrayProxyNoTemporaries<const int&>(const int &)
    fctc( i1 );

    // vk::ArrayProxyNoTemporaries<const int> ap1 = 1;   // not supported: ArrayProxyNoTemporaries<const int>::ArrayProxyNoTemporaries<T,0>(int &&)

    vk::ArrayProxyNoTemporaries<const int> ap2 = i0;
    assert( ap2.size() == 1 );
    vk::ArrayProxyNoTemporaries<const int> ap3 = i1;
    assert( ap3.size() == 1 );

    // count, T *
    int * i0p = &i0;
    fct( { 1, i0p } );
    fctc( { 1, i0p } );

    // count, T const*
    int const * i1p = &i1;
    // fct({ 1, i1p });   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( { 1, i1p } );

    vk::ArrayProxyNoTemporaries<const int> ap4 = { 1, i0p };
    assert( ap4.size() == 1 );
    vk::ArrayProxyNoTemporaries<const int> ap5 = { 1, i1p };
    assert( ap5.size() == 1 );

    // T[count]
    int ia0[2] = { 0, 1 };
    fct( ia0 );
    fctc( ia0 );

    // const T[count]
    const int ia1[2] = { 0, 1 };
    // fct( ia1 );   // not supported: cannot convert argument 1 from 'const int [2]' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc( ia1 );

    vk::ArrayProxyNoTemporaries<const int> ap6 = ia0;
    assert( ap6.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap7 = ia1;
    assert( ap7.size() == 2 );

    // getArrayReference
    // fct( getConstArrayReference() );    // not supported
    // fctc( getConstArrayReference() );   // not supported
    // fct( getArrayReference() );         // not supported
    // fctc( getArrayReference() );        // not supported

    // std::array<T,N>
    std::array<int, 2> sa0 = { 0, 1 };
    fct( sa0 );
    fctc( sa0 );

    // std::array<const T,N>
    std::array<const int, 2> sa1 = { 0, 1 };
    // fct(sa1); // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<std::array<const int,2>&>(V)
    fctc( sa1 );

    // std::array<T,N> const
    std::array<int, 2> const sa2 = { 1, 2 };
    // fct(sa2);    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::array<int,2>&>(V)
    fctc( sa2 );

    // std::array<const T,N> const
    std::array<const int, 2> const sa3 = { 1, 2 };
    // fct(sa3); // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::array<const int,2>&>(V)
    fctc( sa3 );

    // getArray
    // fct( getConstArray() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::array<int,2>>(V &&)
    // fctc( getConstArray() );   // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::array<int,2>>(V &&)
    // fct( getArray() );         // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<std::array<int,2>>(V &&)
    // fctc( getArray() );        // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<std::array<int,2>>(V &&)

    // from std::array constructors
    vk::ArrayProxyNoTemporaries<int> ap8 = sa0;
    assert( ap8.size() == 2 );

    // vk::ArrayProxyNoTemporaries<int> ap9 = sa1;  // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<std::array<const
    // int,2>&>(V) vk::ArrayProxyNoTemporaries<int> ap10 = sa2;  // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const
    // std::array<int,2>&>(V) vk::ArrayProxyNoTemporaries<int> ap11 = sa3;  // not supported: attempting to reference a deleted function:
    // ArrayProxyNoTemporaries<const std::array<const int,2>&>(V)

    vk::ArrayProxyNoTemporaries<const int> ap12 = sa0;
    assert( ap12.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap13 = sa1;
    assert( ap13.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap14 = sa2;
    assert( ap14.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap15 = sa3;
    assert( ap15.size() == 2 );

    // std::vector<T>
    std::vector<int> sv0 = { 0, 1 };
    fct( sv0 );
    fctc( sv0 );

    // std::vector<T> const
    std::vector<int> const sv1 = { 0, 1 };
    // fct(sv1); // not supported:  attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::vector<int,std::allocator<int>>&>(V)
    fctc( sv1 );

    // getVector
    // fct( getConstVector() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const
    // std::vector<int,std::allocator<int>>>(V &&) fctc( getConstVector() );   // not supported: attempting to reference a deleted function:
    // ArrayProxyNoTemporaries<const std::vector<int,std::allocator<int>>>(V &&) fct( getVector() );         // not supported: attempting to reference a deleted
    // function: ArrayProxyNoTemporaries<std::vector<int,std::allocator<int>>>(V &&) fctc( getVector() );        // not supported: attempting to reference a
    // deleted function: ArrayProxyNoTemporaries<std::vector<int,std::allocator<int>>>(V &&)

    vk::ArrayProxyNoTemporaries<int> ap16 = sv0;
    assert( ap16.size() == 2 );

    // vk::ArrayProxyNoTemporaries<int> ap17 = sv1;  // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const
    // std::vector<int,std::allocator<int>>&>(V)

    vk::ArrayProxyNoTemporaries<const int> ap18 = sv0;
    assert( ap18.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap19 = sv1;
    assert( ap19.size() == 2 );

    // std::initializer_list
    fct( {} );
    fctc( {} );

    // fct({ 0, 1 });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc({ 0, 1 });   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    // int a = 0;
    // int b = 1;
    // fct({ a, b });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    // fctc({ a,b });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    auto il0 = { 0, 1 };  // -> std::initializer_list<int>
    // fct(il0);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc( il0 );

    std::initializer_list<int> il1 = { 0, 1 };
    // fct(il1);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc( il1 );

    std::initializer_list<const int> il2 = { 0, 1 };
    // fct(il2);  // not supported: attempting to reference a deleted function : ArrayProxyNoTemporaries<std::initializer_list<T>&>(V)
    fctc( il2 );

    std::initializer_list<int> const il3 = { 0, 1 };
    // fct(il3);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc( il3 );

    std::initializer_list<const int> const il4 = { 0, 1 };
    // fct(il4);  // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const std::initializer_list<T>&>(V)
    fctc( il4 );

    // getInitializerList
    // fct( getConstInitializerList() );     // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries(const
    // std::initializer_list<int> &&) fctc( getConstInitializerList() );    // not supported: attempting to reference a deleted function:
    // ArrayProxyNoTemporaries<T,0>(const std::initializer_list<int> &&) fct( getInitializerList() );          // not supported: attempting to reference a
    // deleted function: ArrayProxyNoTemporaries(std::initializer_list<int> &&) fctc( getInitializerList() );         // not supported: attempting to reference
    // a deleted function: ArrayProxyNoTemporaries<T,0>(std::initializer_list<int> &&)

    // vk::ArrayProxyNoTemporaries<int> ap20 = il1;   // not supported: cannot convert from 'const int *' to 'int *'
    // vk::ArrayProxyNoTemporaries<int> ap21 = il2;   // not supported: attempting to reference a deleted function:
    // ArrayProxyNoTemporaries<std::initializer_list<T>&>(V) vk::ArrayProxyNoTemporaries<int> ap22 = il3;   // not supported: cannot convert from 'const int *'
    // to 'int *' vk::ArrayProxyNoTemporaries<int> ap23 = il4;   // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries<const
    // std::initializer_list<T>&>(V)

    vk::ArrayProxyNoTemporaries<const int> ap24 = {};
    assert( ap24.size() == 0 );

    // vk::ArrayProxyNoTemporaries<const int> ap25 = { 0, 1 };   // not supported

    vk::ArrayProxyNoTemporaries<const int> ap26 = il1;
    assert( ap26.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap27 = il2;
    assert( ap27.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap28 = il3;
    assert( ap28.size() == 2 );
    vk::ArrayProxyNoTemporaries<const int> ap29 = il4;
    assert( ap29.size() == 2 );
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
