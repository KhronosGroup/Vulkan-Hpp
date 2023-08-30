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
// VulkanHpp Tests : StructureChain
//                   Compile-test for StructureChains

// ignore warning 4189: local variable is initialized but not referenced
#if defined( _MSC_VER )
#  pragma warning( disable : 4189 )
#elif defined( __GNUC__ )
#  pragma GCC diagnostic ignored "-Wunused-variable"
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <iostream>
#include <vulkan/vulkan.hpp>

static char const * AppName    = "StructureChain";
static char const * EngineName = "Vulkan.hpp";

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

template <typename T>
void unused( T const & )
{
}

int main( int /*argc*/, char ** /*argv*/ )
{
  try
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    vk::ApplicationInfo appInfo( AppName, 1, EngineName, 1, VK_API_VERSION_1_1 );
    vk::UniqueInstance  instance = vk::createInstanceUnique( vk::InstanceCreateInfo( {}, &appInfo ) );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( *instance );
    vk::PhysicalDevice physicalDevice = instance->enumeratePhysicalDevices().front();

    // some valid StructureChains
    vk::StructureChain<vk::PhysicalDeviceProperties2>                                       sc0;
    const vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties> sc1;
    auto                                                                                    pdp = sc1.get<vk::PhysicalDeviceProperties2>();
    unused( pdp );
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceMaintenance3Properties>                                      sc2;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDevicePushDescriptorPropertiesKHR>                                 sc3;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties>      sc4;
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDevicePushDescriptorPropertiesKHR> sc6;
    vk::StructureChain<vk::PhysicalDeviceProperties2,
                       vk::PhysicalDeviceIDProperties,
                       vk::PhysicalDeviceMaintenance3Properties,
                       vk::PhysicalDevicePushDescriptorPropertiesKHR>
      sc7;

#if ( 17 <= VULKAN_HPP_CPP_VERSION )
    // test for structured binding from a StructureChain
    auto const & [p0, p1] = sc1;
    auto & [p2, p3]       = sc2;
#endif

#if !defined( NDEBUG )
    void * pNext = sc7.get<vk::PhysicalDeviceIDProperties>().pNext;
#endif
    sc7.assign<vk::PhysicalDeviceIDProperties>( {} );
    assert( pNext == sc7.get<vk::PhysicalDeviceIDProperties>().pNext );

#if !defined( NDEBUG )
    void * pNext1 = sc7.get<vk::PhysicalDeviceMaintenance3Properties>().pNext;
#endif
    sc7.assign<vk::PhysicalDeviceMaintenance3Properties>( {} ).assign<vk::PhysicalDeviceIDProperties>( {} );
    assert( pNext == sc7.get<vk::PhysicalDeviceIDProperties>().pNext );
    assert( pNext1 == sc7.get<vk::PhysicalDeviceMaintenance3Properties>().pNext );

    // some checks on unmodified chains
    assert( sc7.isLinked<vk::PhysicalDeviceProperties2>() );
    assert( sc7.isLinked<vk::PhysicalDeviceMaintenance3Properties>() );

    // some invalid StructureChains
    // clang-format off
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceMaintenance3Properties> x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties,
    //                   vk::PhysicalDeviceMaintenance3Properties,
    //                   vk::PhysicalDevicePushDescriptorPropertiesKHR>
    //  x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties,
    //                   vk::PhysicalDevicePushDescriptorPropertiesKHR,
    //                   vk::PhysicalDeviceMaintenance3Properties>
    //                                                                                                                  x;
    //vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceIDProperties> x;
    //vk::StructureChain<vk::PhysicalDeviceIDProperties, vk::PhysicalDeviceProperties2>                                 x;
    // clang-format on

    // unlink a struct from a StructureChain
    sc7.unlink<vk::PhysicalDeviceMaintenance3Properties>();
    assert( !sc7.isLinked<vk::PhysicalDeviceMaintenance3Properties>() );

    // some invalid unlink calls
    // clang-format off
    //sc7.unlink<vk::PhysicalDeviceMaintenance3Properties>();  // assertion fires on trying to unlink some already
    //                                                         // unlinked structure
    //sc7.unlink<vk::PhysicalDeviceProperties2>();
    //sc1.unlink<vk::PhysicalDeviceMaintenance3Properties>();
    // clang-format on

    // re-link a struct
    sc7.relink<vk::PhysicalDeviceMaintenance3Properties>();
    assert( sc7.isLinked<vk::PhysicalDeviceMaintenance3Properties>() );

    // invalid re-linking
    // clang-format off
    //sc7.relink<vk::PhysicalDeviceProperties2>();
    //sc1.relink<vk::PhysicalDeviceMaintenance3Properties>();
    //sc1.relink<vk::PhysicalDeviceIDProperties>();  // assertion fires on trying to relink some structure that hasn't been unlinked
    // clang-format on

    // simple call, passing structures in
    vk::PhysicalDeviceFeatures2 pdf;
    physicalDevice.getFeatures2( &pdf );

    // simple calls, getting structure back
    vk::PhysicalDeviceFeatures2 a = physicalDevice.getFeatures2();
    unused( a );

    // complex calls, getting StructureChain back
    auto                          c  = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    vk::PhysicalDeviceFeatures2 & c0 = c.get<vk::PhysicalDeviceFeatures2>();
    unused( c0 );

    auto t0 = c.get<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    unused( t0 );

    auto                          d  = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    vk::PhysicalDeviceFeatures2 & d0 = d.get<vk::PhysicalDeviceFeatures2>();
    unused( d0 );
    vk::PhysicalDeviceVariablePointerFeatures & d1 = d.get<vk::PhysicalDeviceVariablePointerFeatures>();
    unused( d1 );

    auto t1 = d.get<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVariablePointerFeatures>();
    unused( t1 );

    using StructureChain = vk::StructureChain<vk::QueueFamilyProperties2, vk::QueueFamilyCheckpointPropertiesNV>;
    using AllocatorType  = std::vector<StructureChain>::allocator_type;
    auto qfd             = physicalDevice.getQueueFamilyProperties2<StructureChain, AllocatorType>( VULKAN_HPP_DEFAULT_DISPATCHER );
    unused( qfd );

    // some tests with structures with allowDuplicate == true
    // include them as soon as vk.xml has been fixed on attribute "allowduplicate" !
#if 0
    vk::StructureChain<vk::DeviceCreateInfo, vk::DevicePrivateDataCreateInfoEXT, vk::DevicePrivateDataCreateInfoEXT>
         dci0;
    auto dci1( dci0 );

    vk::DeviceCreateInfo               dci;
    vk::DevicePrivateDataCreateInfoEXT dpdci0;
    vk::DevicePrivateDataCreateInfoEXT dpdci1;
    vk::StructureChain<vk::DeviceCreateInfo, vk::DevicePrivateDataCreateInfoEXT, vk::DevicePrivateDataCreateInfoEXT>
      dci2( dci, dpdci0, dpdci1 );

    dci2 = dci1;

    auto &       dpdci  = dci0.get<vk::DevicePrivateDataCreateInfoEXT, 1>();
    auto const & dpdcic = dci0.get<vk::DevicePrivateDataCreateInfoEXT, 1>();

    dci2.unlink<vk::DevicePrivateDataCreateInfoEXT, 1>();
    dci2.relink<vk::DevicePrivateDataCreateInfoEXT, 1>();
#endif

    vk::StructureChain<vk::InstanceCreateInfo,
                       vk::DebugReportCallbackCreateInfoEXT,
                       vk::ValidationFlagsEXT,
                       vk::ValidationFeaturesEXT,
                       vk::DebugUtilsMessengerCreateInfoEXT>
      chain;
    chain.unlink<vk::DebugReportCallbackCreateInfoEXT>();
    chain.unlink<vk::ValidationFlagsEXT>();
    chain.unlink<vk::ValidationFeaturesEXT>();
    chain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
    chain.relink<vk::DebugUtilsMessengerCreateInfoEXT>();
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
