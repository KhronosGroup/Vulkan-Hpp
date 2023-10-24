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
// VulkanHpp Samples : ArrayProxy
//                     Compile test on using vk::ArrayProxy

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
#if ( 20 <= VULKAN_HPP_CPP_VERSION )
#  include <span>
#endif

void fct( vk::ArrayProxy<int> /*ap*/ ) {}

void fctc( vk::ArrayProxy<const int> /*ap*/ ) {}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    // to prevent a warning on unreferenced function vk::getDispatchLoaderStatic, use just one arbitrary vk-function
    (void)vk::enumerateInstanceVersion();

    // nullptr_t
    fct( nullptr );
    fctc( nullptr );

    vk::ArrayProxy<int> ap0 = nullptr;
    assert( ap0.size() == 0 );

    // Type
    // fct(2);  // not supported: cannot convert argument 1 from 'int' to 'vk::ArrayProxy<int>'
    fctc( 1 );

    int i0 = 1;
    fct( i0 );
    fctc( i0 );

    const int i1 = 2;
    // fct(i1);   // not supported: cannot convert argument 1 from 'const int' to 'vk::ArrayProxy<int>'
    fctc( i1 );

    vk::ArrayProxy<const int> ap1 = 1;
    assert( ap1.size() == 1 );
    vk::ArrayProxy<const int> ap2 = i0;
    assert( ap2.size() == 1 );
    vk::ArrayProxy<const int> ap3 = i1;
    assert( ap3.size() == 1 );

    // count, T *
    int * i0p = &i0;
    fct( { 1, i0p } );
    fctc( { 1, i0p } );

    // count, T const*
    int const * i1p = &i1;
    // fct({ 1, i1p });   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxy<int>'
    fctc( { 1, i1p } );

    vk::ArrayProxy<const int> ap4 = { 1, i0p };
    assert( ap4.size() == 1 );
    vk::ArrayProxy<const int> ap5 = { 1, i1p };
    assert( ap5.size() == 1 );

    // T[count]
    int ia0[2] = { 0, 1 };
    fct( ia0 );
    fctc( ia0 );

    // const T[count]
    const int ia1[2] = { 0, 1 };
    // fct( ia1 );   // not supported: cannot convert argument 1 from 'const int [2]' to 'vk::ArrayProxy<int>'
    fctc( ia1 );

    vk::ArrayProxy<const int> ap6 = ia0;
    assert( ap6.size() == 2 );
    vk::ArrayProxy<const int> ap7 = ia1;
    assert( ap7.size() == 2 );

    // std::array<T,N>
    std::array<int, 2> sa0 = { 0, 1 };
    fct( sa0 );
    fctc( sa0 );

    // std::array<T,N> const
    std::array<const int, 2> sa1 = { 0, 1 };
    // fct(sa1); // not supported: cannot convert argument 1 from 'std::array<const int,2>' to 'vk::ArrayProxy<int>'
    fctc( sa1 );

    std::array<int, 2> const sa2 = { 1, 2 };
    // fct(sa2);    // not supported: cannot convert from 'const _Ty *' to 'T *'
    fctc( sa2 );

    std::array<const int, 2> const sa3 = { 1, 2 };
    // fct(sa3); // not supported: cannot convert argument 1 from 'const std::array<const int,2>' to 'vk::ArrayProxy<int>'
    fctc( sa3 );

    vk::ArrayProxy<int> ap8 = sa0;
    assert( ap8.size() == 2 );
    // vk::ArrayProxy<int> ap9 = sa1;  // not supported: cannot convert from 'std::array<const int,2>' to 'vk::ArrayProxy<int>'
    // vk::ArrayProxy<int> ap10 = sa2;  // not supported: cannot convert from '_Ty *' to 'T *'
    // vk::ArrayProxy<int> ap11 = sa3;  // not supported: cannot convert from 'const std::array<const int,2>' to 'vk::ArrayProxy<int>'

    vk::ArrayProxy<const int> ap12 = sa0;
    assert( ap12.size() == 2 );
    vk::ArrayProxy<const int> ap13 = sa1;
    assert( ap13.size() == 2 );
    vk::ArrayProxy<const int> ap14 = sa2;
    assert( ap14.size() == 2 );
    vk::ArrayProxy<const int> ap15 = sa3;
    assert( ap15.size() == 2 );

    // std::vector<T>
    std::vector<int> sv0 = { 0, 1 };
    fct( sv0 );
    fctc( sv0 );

    std::vector<int> const sv1 = { 0, 1 };
    // fct(sv1); // not supported:  cannot convert from 'const _Ty *' to 'T *'
    fctc( sv1 );

    vk::ArrayProxy<int> ap16 = sv0;
    assert( ap16.size() == 2 );
    // vk::ArrayProxy<int> ap17 = sv1;  // not supported: cannot convert from '_Ty *' to 'T *'

    vk::ArrayProxy<const int> ap18 = sv0;
    assert( ap18.size() == 2 );
    vk::ArrayProxy<const int> ap19 = sv1;
    assert( ap19.size() == 2 );

    // std::initializer_list
    fct( {} );
    fctc( {} );

    // fct({ 0, 1 });   // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc( { 0, 1 } );

    int a = 0;
    int b = 1;
    // fct({ a, b });  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc( { a, b } );

    auto il0 = { 0, 1 };  // -> std::initializer_list<int>
    // fct(il0);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc( il0 );

    std::initializer_list<int> il1 = { 0, 1 };
    // fct(il1);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc( il1 );

    std::initializer_list<const int> il2 = { 0, 1 };
    // fct(il2);  // not supported: cannot convert argument 1 from 'std::initializer_list<T>' to 'vk::ArrayProxy<int>'
    fctc( il2 );

    std::initializer_list<int> const il3 = { 0, 1 };
    // fct(il3);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc( il3 );

    std::initializer_list<const int> const il4 = { 0, 1 };
    // fct(il4);  // not supported: cannot convert argument 1 from 'const std::initializer_list<T>' to 'vk::ArrayProxy<int>'
    fctc( il4 );

    // vk::ArrayProxy<int> ap20 = il1;   // not supported: cannot convert from 'const _Elem *' to 'T *'
    // vk::ArrayProxy<int> ap21 = il2;   // not supported: cannot convert from 'std::initializer_list<T>' to 'vk::ArrayProxy<int>'
    // vk::ArrayProxy<int> ap22 = il3;   // not supported: cannot convert from 'const _Elem *' to 'T *'
    // vk::ArrayProxy<int> ap23 = il4;   // not supported: cannot convert from 'const std::initializer_list<T>' to 'vk::ArrayProxy<int>'

    vk::ArrayProxy<const int> ap24 = {};
    assert( ap24.size() == 0 );
    vk::ArrayProxy<const int> ap25 = { 0, 1 };
    assert( ap25.size() == 2 );
    vk::ArrayProxy<const int> ap26 = il1;
    assert( ap26.size() == 2 );
    vk::ArrayProxy<const int> ap27 = il2;
    assert( ap27.size() == 2 );
    vk::ArrayProxy<const int> ap28 = il3;
    assert( ap28.size() == 2 );
    vk::ArrayProxy<const int> ap29 = il4;
    assert( ap29.size() == 2 );

#if defined( VULKAN_HPP_SUPPORT_SPAN )
    // std::span<T, N>
    std::span<int> ss0( sa0.begin(), sa0.size() );
    fct( ss0 );
    fctc( ss0 );

    // std::span<T,N> const
    std::span<const int> ss1( sa1.begin(), sa1.size() );
    // fct(ss1); // not supported: cannot convert from 'std::span<const int>' to 'vk::ArrayProxy<int>'
    fctc( ss1 );

    std::span<int> const ss2;
    fct( ss2 );
    fctc( ss2 );

    std::span<const int> const ss3( sa3.begin(), sa3.end() );
    // fct(ss3); // not supported: cannot convert from 'const std::span<const int>' to 'vk::ArrayProxy<int>'
    fctc( ss3 );

    vk::ArrayProxy<const int> ap30 = ss0;
    assert( ap30.size() == 2 );
    vk::ArrayProxy<const int> ap31 = ss1;
    assert( ap31.size() == 2 );
    vk::ArrayProxy<const int> ap32 = ss2;
    assert( ap32.size() == 2 );
    vk::ArrayProxy<const int> ap33 = ss3;
    assert( ap33.size() == 2 );
#endif
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
