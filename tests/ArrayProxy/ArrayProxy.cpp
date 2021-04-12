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

#include "vulkan/vulkan.hpp"
#include <iostream>

void fct(vk::ArrayProxy<int> /*ap*/)
{}

void fctc(vk::ArrayProxy<const int> /*ap*/)
{}

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    // nullptr_t
    fct(nullptr);
    fctc(nullptr);

    vk::ArrayProxy<int> ap0 = nullptr;
    assert(ap0.size() == 0);

    // Type
    //fct(2);  // not supported: cannot convert from 'const int *' to 'T *'
    fctc(1);

    int i0 = 1;
    fct(i0);
    fctc(i0);

    const int i1 = 2;
    //fct(i1);   // not supported: cannot convert from 'const int *' to 'T *'
    fctc(i1);

    // count, T *
    int* i0p = &i0;
    fct({ 1, i0p });
    fctc({ 1, i0p });

    // count, T const*
    int const* i1p = &i1;
    //fct({ 1, i1p });   // not supported: cannot convert from 'const int *' to 'T *'
    fctc({ 1, i1p });

    // std::array<T,N>
    std::array<int, 2> sa0 = { 0, 1 };
    fct(sa0);
    fctc(sa0);

    // std::array<T,N> const
    std::array<const int, 2> sa1 = { 0, 1 };
    //fct(sa1); // not supported: cannot convert from '_Ty *' to 'T *'
    fctc(sa1);

    std::array<int, 2> const sa2 = { 1, 2 };
    //fct(sa2);    // not supported: cannot convert from 'const _Ty *' to 'T *'
    fctc(sa2);

    std::array<const int, 2> const sa3 = { 1, 2 };
    //fct(sa3); // not supported: cannot convert from '_Ty *' to 'T *'
    fctc(sa3);

    vk::ArrayProxy<int> ap2 = sa0;
    assert(ap2.size() == 2);
    //vk::ArrayProxy<int> ap3 = sa1;  // not supported: cannot convert from '_Ty *' to 'T *'
    //vk::ArrayProxy<int> ap4 = sa2;  // not supported: cannot convert from '_Ty *' to 'T *'
    //vk::ArrayProxy<int> ap5 = sa3;  // not supported: cannot convert from '_Ty *' to 'T *'

    vk::ArrayProxy<const int> ap6 = sa0;
    assert(ap6.size() == 2);
    vk::ArrayProxy<const int> ap7 = sa1;
    assert(ap7.size() == 2);
    vk::ArrayProxy<const int> ap8 = sa2;
    assert(ap8.size() == 2);
    vk::ArrayProxy<const int> ap9 = sa3;
    assert(ap9.size() == 2);

    // std::vector<T>
    std::vector<int> sv0 = { 0, 1 };
    fct(sv0);
    fctc(sv0);

    std::vector<int> const sv1 = { 0, 1 };
    //fct(sv1); // not supported:  cannot convert from 'const _Ty *' to 'T *'
    fctc(sv1);

    vk::ArrayProxy<int> ap10 = sv0;
    assert(ap10.size() == 2);
    //vk::ArrayProxy<int> ap11 = sv1;  // not supported: cannot convert from '_Ty *' to 'T *'

    vk::ArrayProxy<const int> ap12 = sv0;
    assert(ap12.size() == 2);
    vk::ArrayProxy<const int> ap13 = sv1;
    assert(ap13.size() == 2);

    // std::initializer_list
    fct({});
    fctc({});

    //fct({ 0, 1 });   // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc({ 0, 1 });

    int a = 0;
    int b = 1;
    //fct({ a, b });  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc({ a,b });

    auto il0 = { 0, 1 };   // -> std::initializer_list<int>
    //fct(il0);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc(il0);

    std::initializer_list<int> il1 = { 0, 1 };
    //fct(il1);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc(il1);

    std::initializer_list<const int> il2 = { 0, 1 };
    //fct(il2);  // not supported: cannot convert from '_Elem *' to 'T *'
    fctc(il2);

    std::initializer_list<int> const il3 = { 0, 1 };
    //fct(il3);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc(il3);

    std::initializer_list<const int> const il4 = { 0, 1 };
    //fct(il4);  // not supported: cannot convert from 'const _Elem *' to 'T *'
    fctc(il4);

    //vk::ArrayProxy<int> ap14 = il1;   // not supported: cannot convert from 'const _Elem *' to 'T *'
    //vk::ArrayProxy<int> ap15 = il2;   // not supported: cannot convert from '_Ty *' to 'T *'
    //vk::ArrayProxy<int> ap16 = il3;   // not supported: cannot convert from '_Ty *' to 'T *'
    //vk::ArrayProxy<int> ap17 = il4;   // not supported: cannot convert from '_Ty *' to 'T *'

    vk::ArrayProxy<const int> ap18 = il1;
    assert(ap18.size() == 2);
    vk::ArrayProxy<const int> ap19 = il2;
    assert(ap19.size() == 2);
    vk::ArrayProxy<const int> ap20 = il3;
    assert(ap20.size() == 2);
    vk::ArrayProxy<const int> ap21 = il4;
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
