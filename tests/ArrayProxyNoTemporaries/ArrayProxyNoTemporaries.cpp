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

#include "vulkan/vulkan.hpp"
#include <iostream>

static char const* AppName = "ArrayProxyNoTemporaries";
static char const* EngineName = "Vulkan.hpp";

void fct(vk::ArrayProxyNoTemporaries<int> /*ap*/)
{}

void fctc(vk::ArrayProxyNoTemporaries<const int> /*ap*/)
{}

int getInt()
{
  return 1;
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
    // nullptr_t
    fct(nullptr);
    fctc(nullptr);

    vk::ArrayProxyNoTemporaries<int> ap0 = nullptr;
    assert(ap0.size() == 0);

    // Type
    //fct(2);   // not supported: attempting to reference a deleted function
    //fctc(1);  // not supported: attempting to reference a deleted function

    // getInt()
    //fct( getInt() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( T && value )
    //fctc( getInt() );   // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( typename std::remove_const<T>::type && value )

    int i0 = 1;
    fct(i0);
    fctc(i0);

    const int i1 = 2;
    //fct(i1);   // not supported: cannot convert argument 1 from 'const int' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc(i1);

    // count, T *
    int* i0p = &i0;
    fct({ 1, i0p });
    fctc({ 1, i0p });

    // count, T const*
    int const* i1p = &i1;
    //fct({ 1, i1p });   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc({ 1, i1p });

    // std::array<T,N>
    std::array<int, 2> sa0 = { 0, 1 };
    fct(sa0);
    fctc(sa0);

    // std::array<const T,N>
    std::array<const int, 2> sa1 = { 0, 1 };
    //fct(sa1); // not supported: cannot convert argument 1 from 'std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc(sa1);

    // std::array<T,N> const
    std::array<int, 2> const sa2 = { 1, 2 };
    //fct(sa2);    // not supported: cannot convert from 'const int *' to 'int *'
    fctc(sa2);

    // std::array<const T,N> const
    std::array<const int, 2> const sa3 = { 1, 2 };
    //fct(sa3); // not supported: cannot convert argument 1 from 'const std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc(sa3);

    // getArray
    //fct( getConstArray() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::array<T, N> const && data )
    //fctc( getConstArray() );    // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type> const && data ) 
    //fct( getArray() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::array<T, N> && data )
    //fctc( getArray() );   // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> && data )

    // from std::array constructors
    vk::ArrayProxyNoTemporaries<int> ap2 = sa0;
    assert(ap2.size() == 2);
    //vk::ArrayProxyNoTemporaries<int> ap3 = sa1;  // not supported: cannot convert from 'std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'
    //vk::ArrayProxyNoTemporaries<int> ap4 = sa2;  // not supported: cannot convert from 'int *' to 'int *'
    //vk::ArrayProxyNoTemporaries<int> ap5 = sa3;  // not supported: cannot convert from 'const std::array<const int,2>' to 'vk::ArrayProxyNoTemporaries<int>'

    vk::ArrayProxyNoTemporaries<const int> ap6 = sa0;
    assert(ap6.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap7 = sa1;
    assert(ap7.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap8 = sa2;
    assert(ap8.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap9 = sa3;
    assert(ap9.size() == 2);

    // std::vector<T>
    std::vector<int> sv0 = { 0, 1 };
    fct(sv0);
    fctc(sv0);

    // std::vector<T> const
    std::vector<int> const sv1 = { 0, 1 };
    //fct(sv1); // not supported:  cannot convert from 'const int *' to 'int *'
    fctc(sv1);

    vk::ArrayProxyNoTemporaries<int> ap10 = sv0;
    assert(ap10.size() == 2);
    //vk::ArrayProxyNoTemporaries<int> ap11 = sv1;  // not supported: cannot convert from 'int *' to 'int *'

    // getVector
    //fct( getConstVector() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::vector<T, Allocator> const && data )
    //fctc( getConstVector() );    // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> const && data )
    //fct( getVector() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::vector<T> && data )
    //fctc( getVector() );   // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type> && data )

    vk::ArrayProxyNoTemporaries<const int> ap12 = sv0;
    assert(ap12.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap13 = sv1;
    assert(ap13.size() == 2);

    // std::initializer_list
    fct({});
    fctc({});

    //fct({ 0, 1 });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    //fctc({ 0, 1 });   // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    //int a = 0;
    //int b = 1;
    //fct({ a, b });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<int>'
    //fctc({ a,b });    // not supported: cannot convert argument 1 from 'initializer list' to 'vk::ArrayProxyNoTemporaries<const int32_t>'

    auto il0 = { 0, 1 };   // -> std::initializer_list<int>
    //fct(il0);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc(il0);

    std::initializer_list<int> il1 = { 0, 1 };
    //fct(il1);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc(il1);

    std::initializer_list<const int> il2 = { 0, 1 };
    //fct(il2);  // not supported: cannot convert argument 1 from 'std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc(il2);

    std::initializer_list<int> const il3 = { 0, 1 };
    //fct(il3);  // not supported: cannot convert from 'const int *' to 'int *'
    fctc(il3);

    std::initializer_list<const int> const il4 = { 0, 1 };
    //fct(il4);  // not supported: cannot convert argument 1 from 'const std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    fctc(il4);

    // getInitializerList
    //fct( getConstInitializerList() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::initializer_list<T> const && list )
    //fctc( getConstInitializerList() );    // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const && list ) 
    //fct( getInitializerList() );    // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::initializer_list<T> && list )
    //fctc( getInitializerList() );   // // not supported: attempting to reference a deleted function: ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> && list )

    //vk::ArrayProxyNoTemporaries<int> ap14 = il1;   // not supported: cannot convert from 'const int *' to 'int *'
    //vk::ArrayProxyNoTemporaries<int> ap15 = il2;   // not supported: cannot convert from 'std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'
    //vk::ArrayProxyNoTemporaries<int> ap16 = il3;   // not supported: cannot convert from 'const int *' to 'int *'
    //vk::ArrayProxyNoTemporaries<int> ap17 = il4;   // not supported: cannot convert from 'const std::initializer_list<const int>' to 'vk::ArrayProxyNoTemporaries<int>'

    vk::ArrayProxyNoTemporaries<const int> ap18 = il1;
    assert(ap18.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap19 = il2;
    assert(ap19.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap20 = il3;
    assert(ap20.size() == 2);
    vk::ArrayProxyNoTemporaries<const int> ap21 = il4;
    assert(ap21.size() == 2);
  }
  catch (vk::SystemError const& err)
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  }
  catch (...)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }

  return 0;
}
