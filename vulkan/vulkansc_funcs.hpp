// Copyright 2015-2023 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_FUNCS_HPP
#define VULKAN_FUNCS_HPP

namespace VULKAN_HPP_NAMESPACE
{

  //===========================
  //=== COMMAND Definitions ===
  //===========================

  //=== VK_VERSION_1_0 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result createInstance( const VULKAN_HPP_NAMESPACE::InstanceCreateInfo *  pCreateInfo,
                                                                const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                VULKAN_HPP_NAMESPACE::Instance *                  pInstance,
                                                                Dispatch const &                                  d ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo *>( pCreateInfo ),
                                                    reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                    reinterpret_cast<VkInstance *>( pInstance ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Instance>::type createInstance(
    const VULKAN_HPP_NAMESPACE::InstanceCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Instance instance;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo *>( &createInfo ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                          reinterpret_cast<VkInstance *>( &instance ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::createInstance" );

    return createResultValueType( result, instance );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Instance, Dispatch>>::type createInstanceUnique(
    const VULKAN_HPP_NAMESPACE::InstanceCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Instance instance;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo *>( &createInfo ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                          reinterpret_cast<VkInstance *>( &instance ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::createInstanceUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Instance, Dispatch>( instance, ObjectDestroy<NoParent, Dispatch>( allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyInstance( m_instance, reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyInstance( m_instance,
                         reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Instance::enumeratePhysicalDevices( uint32_t *                             pPhysicalDeviceCount,
                                                                                    VULKAN_HPP_NAMESPACE::PhysicalDevice * pPhysicalDevices,
                                                                                    Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEnumeratePhysicalDevices( m_instance, pPhysicalDeviceCount, reinterpret_cast<VkPhysicalDevice *>( pPhysicalDevices ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PhysicalDeviceAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDevice, PhysicalDeviceAllocator>>::type
                       Instance::enumeratePhysicalDevices( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDevice, PhysicalDeviceAllocator> physicalDevices;
    uint32_t                                                                   physicalDeviceCount;
    VULKAN_HPP_NAMESPACE::Result                                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceCount )
      {
        physicalDevices.resize( physicalDeviceCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice *>( physicalDevices.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::enumeratePhysicalDevices" );
    VULKAN_HPP_ASSERT( physicalDeviceCount <= physicalDevices.size() );
    if ( physicalDeviceCount < physicalDevices.size() )
    {
      physicalDevices.resize( physicalDeviceCount );
    }
    return createResultValueType( result, physicalDevices );
  }

  template <typename PhysicalDeviceAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PhysicalDevice>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDevice, PhysicalDeviceAllocator>>::type
                       Instance::enumeratePhysicalDevices( PhysicalDeviceAllocator & physicalDeviceAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDevice, PhysicalDeviceAllocator> physicalDevices( physicalDeviceAllocator );
    uint32_t                                                                   physicalDeviceCount;
    VULKAN_HPP_NAMESPACE::Result                                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceCount )
      {
        physicalDevices.resize( physicalDeviceCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice *>( physicalDevices.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::enumeratePhysicalDevices" );
    VULKAN_HPP_ASSERT( physicalDeviceCount <= physicalDevices.size() );
    if ( physicalDeviceCount < physicalDevices.size() )
    {
      physicalDevices.resize( physicalDeviceCount );
    }
    return createResultValueType( result, physicalDevices );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getFeatures( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures * pFeatures, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures *>( pFeatures ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures
                                         PhysicalDevice::getFeatures( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures features;
    d.vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures *>( &features ) );

    return features;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getFormatProperties( VULKAN_HPP_NAMESPACE::Format             format,
                                                              VULKAN_HPP_NAMESPACE::FormatProperties * pFormatProperties,
                                                              Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties *>( pFormatProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::FormatProperties
    PhysicalDevice::getFormatProperties( VULKAN_HPP_NAMESPACE::Format format, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::FormatProperties formatProperties;
    d.vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties *>( &formatProperties ) );

    return formatProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getImageFormatProperties( VULKAN_HPP_NAMESPACE::Format                  format,
                                                                                          VULKAN_HPP_NAMESPACE::ImageType               type,
                                                                                          VULKAN_HPP_NAMESPACE::ImageTiling             tiling,
                                                                                          VULKAN_HPP_NAMESPACE::ImageUsageFlags         usage,
                                                                                          VULKAN_HPP_NAMESPACE::ImageCreateFlags        flags,
                                                                                          VULKAN_HPP_NAMESPACE::ImageFormatProperties * pImageFormatProperties,
                                                                                          Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice,
                                                                            static_cast<VkFormat>( format ),
                                                                            static_cast<VkImageType>( type ),
                                                                            static_cast<VkImageTiling>( tiling ),
                                                                            static_cast<VkImageUsageFlags>( usage ),
                                                                            static_cast<VkImageCreateFlags>( flags ),
                                                                            reinterpret_cast<VkImageFormatProperties *>( pImageFormatProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::ImageFormatProperties>::type
                       PhysicalDevice::getImageFormatProperties( VULKAN_HPP_NAMESPACE::Format           format,
                                              VULKAN_HPP_NAMESPACE::ImageType        type,
                                              VULKAN_HPP_NAMESPACE::ImageTiling      tiling,
                                              VULKAN_HPP_NAMESPACE::ImageUsageFlags  usage,
                                              VULKAN_HPP_NAMESPACE::ImageCreateFlags flags,
                                              Dispatch const &                       d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ImageFormatProperties imageFormatProperties;
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice,
                                                  static_cast<VkFormat>( format ),
                                                  static_cast<VkImageType>( type ),
                                                  static_cast<VkImageTiling>( tiling ),
                                                  static_cast<VkImageUsageFlags>( usage ),
                                                  static_cast<VkImageCreateFlags>( flags ),
                                                  reinterpret_cast<VkImageFormatProperties *>( &imageFormatProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties" );

    return createResultValueType( result, imageFormatProperties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getProperties( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties * pProperties,
                                                        Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties *>( pProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties
                                         PhysicalDevice::getProperties( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties properties;
    d.vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties *>( &properties ) );

    return properties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getQueueFamilyProperties( uint32_t *                                    pQueueFamilyPropertyCount,
                                                                   VULKAN_HPP_NAMESPACE::QueueFamilyProperties * pQueueFamilyProperties,
                                                                   Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceQueueFamilyProperties(
      m_physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties *>( pQueueFamilyProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename QueueFamilyPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties, QueueFamilyPropertiesAllocator>
                                         PhysicalDevice::getQueueFamilyProperties( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties, QueueFamilyPropertiesAllocator> queueFamilyProperties;
    uint32_t                                                                                 queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    d.vkGetPhysicalDeviceQueueFamilyProperties(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      queueFamilyProperties.resize( queueFamilyPropertyCount );
    }
    return queueFamilyProperties;
  }

  template <typename QueueFamilyPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::QueueFamilyProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties, QueueFamilyPropertiesAllocator>
    PhysicalDevice::getQueueFamilyProperties( QueueFamilyPropertiesAllocator & queueFamilyPropertiesAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties, QueueFamilyPropertiesAllocator> queueFamilyProperties( queueFamilyPropertiesAllocator );
    uint32_t                                                                                 queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    d.vkGetPhysicalDeviceQueueFamilyProperties(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      queueFamilyProperties.resize( queueFamilyPropertyCount );
    }
    return queueFamilyProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getMemoryProperties( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties * pMemoryProperties,
                                                              Dispatch const &                                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties *>( pMemoryProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties
                                         PhysicalDevice::getMemoryProperties( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties memoryProperties;
    d.vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties *>( &memoryProperties ) );

    return memoryProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE PFN_vkVoidFunction Instance::getProcAddr( const char * pName, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return d.vkGetInstanceProcAddr( m_instance, pName );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE PFN_vkVoidFunction Instance::getProcAddr( const std::string & name, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    PFN_vkVoidFunction result = d.vkGetInstanceProcAddr( m_instance, name.c_str() );

    return result;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE PFN_vkVoidFunction Device::getProcAddr( const char * pName, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return d.vkGetDeviceProcAddr( m_device, pName );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE PFN_vkVoidFunction Device::getProcAddr( const std::string & name, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    PFN_vkVoidFunction result = d.vkGetDeviceProcAddr( m_device, name.c_str() );

    return result;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::createDevice( const VULKAN_HPP_NAMESPACE::DeviceCreateInfo *    pCreateInfo,
                                                                              const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                              VULKAN_HPP_NAMESPACE::Device *                    pDevice,
                                                                              Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDevice( m_physicalDevice,
                                                  reinterpret_cast<const VkDeviceCreateInfo *>( pCreateInfo ),
                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                  reinterpret_cast<VkDevice *>( pDevice ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Device>::type PhysicalDevice::createDevice(
    const VULKAN_HPP_NAMESPACE::DeviceCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Device device;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDevice( m_physicalDevice,
                        reinterpret_cast<const VkDeviceCreateInfo *>( &createInfo ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                        reinterpret_cast<VkDevice *>( &device ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::createDevice" );

    return createResultValueType( result, device );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Device, Dispatch>>::type
                       PhysicalDevice::createDeviceUnique( const VULKAN_HPP_NAMESPACE::DeviceCreateInfo &            createInfo,
                                        Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                        Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Device device;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDevice( m_physicalDevice,
                        reinterpret_cast<const VkDeviceCreateInfo *>( &createInfo ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                        reinterpret_cast<VkDevice *>( &device ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::createDeviceUnique" );

    return createResultValueType( result, UniqueHandle<VULKAN_HPP_NAMESPACE::Device, Dispatch>( device, ObjectDestroy<NoParent, Dispatch>( allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyDevice( m_device, reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyDevice( m_device,
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result enumerateInstanceExtensionProperties( const char *                                pLayerName,
                                                                                      uint32_t *                                  pPropertyCount,
                                                                                      VULKAN_HPP_NAMESPACE::ExtensionProperties * pProperties,
                                                                                      Dispatch const &                            d ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename ExtensionPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator>>::type
                       enumerateInstanceExtensionProperties( Optional<const std::string> layerName, Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator> properties;
    uint32_t                                                                             propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumerateInstanceExtensionProperties( layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateInstanceExtensionProperties(
          layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::enumerateInstanceExtensionProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename ExtensionPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::ExtensionProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator>>::type
                       enumerateInstanceExtensionProperties( Optional<const std::string>    layerName,
                                                             ExtensionPropertiesAllocator & extensionPropertiesAllocator,
                                                             Dispatch const &               d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator> properties( extensionPropertiesAllocator );
    uint32_t                                                                             propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumerateInstanceExtensionProperties( layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateInstanceExtensionProperties(
          layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::enumerateInstanceExtensionProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::enumerateDeviceExtensionProperties( const char *                                pLayerName,
                                                                                                    uint32_t *                                  pPropertyCount,
                                                                                                    VULKAN_HPP_NAMESPACE::ExtensionProperties * pProperties,
                                                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkEnumerateDeviceExtensionProperties( m_physicalDevice, pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename ExtensionPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator>>::type
                       PhysicalDevice::enumerateDeviceExtensionProperties( Optional<const std::string> layerName, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator> properties;
    uint32_t                                                                             propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateDeviceExtensionProperties(
          m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateDeviceExtensionProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename ExtensionPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::ExtensionProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator>>::type
                       PhysicalDevice::enumerateDeviceExtensionProperties( Optional<const std::string>    layerName,
                                                        ExtensionPropertiesAllocator & extensionPropertiesAllocator,
                                                        Dispatch const &               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ExtensionProperties, ExtensionPropertiesAllocator> properties( extensionPropertiesAllocator );
    uint32_t                                                                             propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateDeviceExtensionProperties(
          m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateDeviceExtensionProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result enumerateInstanceLayerProperties( uint32_t *                              pPropertyCount,
                                                                                  VULKAN_HPP_NAMESPACE::LayerProperties * pProperties,
                                                                                  Dispatch const &                        d ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEnumerateInstanceLayerProperties( pPropertyCount, reinterpret_cast<VkLayerProperties *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename LayerPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator>>::type
                       enumerateInstanceLayerProperties( Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator> properties;
    uint32_t                                                                     propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                 result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateInstanceLayerProperties( &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::enumerateInstanceLayerProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename LayerPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::LayerProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator>>::type
                       enumerateInstanceLayerProperties( LayerPropertiesAllocator & layerPropertiesAllocator, Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator> properties( layerPropertiesAllocator );
    uint32_t                                                                     propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                 result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateInstanceLayerProperties( &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::enumerateInstanceLayerProperties" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE Result PhysicalDevice::enumerateDeviceLayerProperties( uint32_t *                              pPropertyCount,
                                                                           VULKAN_HPP_NAMESPACE::LayerProperties * pProperties,
                                                                           Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEnumerateDeviceLayerProperties( m_physicalDevice, pPropertyCount, reinterpret_cast<VkLayerProperties *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename LayerPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator>
                                         PhysicalDevice::enumerateDeviceLayerProperties( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator> properties;
    uint32_t                                                                     propertyCount;
    d.vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, nullptr );
    properties.resize( propertyCount );
    d.vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) );

    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return properties;
  }

  template <typename LayerPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::LayerProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator>
    PhysicalDevice::enumerateDeviceLayerProperties( LayerPropertiesAllocator & layerPropertiesAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::LayerProperties, LayerPropertiesAllocator> properties( layerPropertiesAllocator );
    uint32_t                                                                     propertyCount;
    d.vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, nullptr );
    properties.resize( propertyCount );
    d.vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast<VkLayerProperties *>( properties.data() ) );

    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return properties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    Device::getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex, VULKAN_HPP_NAMESPACE::Queue * pQueue, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue *>( pQueue ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Queue
                                         Device::getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Queue queue;
    d.vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue *>( &queue ) );

    return queue;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Queue::submit( uint32_t                                 submitCount,
                                                               const VULKAN_HPP_NAMESPACE::SubmitInfo * pSubmits,
                                                               VULKAN_HPP_NAMESPACE::Fence              fence,
                                                               Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkQueueSubmit( m_queue, submitCount, reinterpret_cast<const VkSubmitInfo *>( pSubmits ), static_cast<VkFence>( fence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Queue::submit(
    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo> const & submits, VULKAN_HPP_NAMESPACE::Fence fence, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkQueueSubmit( m_queue, submits.size(), reinterpret_cast<const VkSubmitInfo *>( submits.data() ), static_cast<VkFence>( fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Queue::waitIdle( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkQueueWaitIdle( m_queue ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Queue::waitIdle( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkQueueWaitIdle( m_queue ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::waitIdle" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::waitIdle( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkDeviceWaitIdle( m_device ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::waitIdle( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkDeviceWaitIdle( m_device ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitIdle" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::allocateMemory( const VULKAN_HPP_NAMESPACE::MemoryAllocateInfo *  pAllocateInfo,
                                                                        const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                        VULKAN_HPP_NAMESPACE::DeviceMemory *              pMemory,
                                                                        Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAllocateMemory( m_device,
                                                    reinterpret_cast<const VkMemoryAllocateInfo *>( pAllocateInfo ),
                                                    reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                    reinterpret_cast<VkDeviceMemory *>( pMemory ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DeviceMemory>::type
                       Device::allocateMemory( const VULKAN_HPP_NAMESPACE::MemoryAllocateInfo &          allocateInfo,
                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                            Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DeviceMemory memory;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkAllocateMemory( m_device,
                          reinterpret_cast<const VkMemoryAllocateInfo *>( &allocateInfo ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                          reinterpret_cast<VkDeviceMemory *>( &memory ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateMemory" );

    return createResultValueType( result, memory );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::DeviceMemory, Dispatch>>::type
                       Device::allocateMemoryUnique( const VULKAN_HPP_NAMESPACE::MemoryAllocateInfo &          allocateInfo,
                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                  Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DeviceMemory memory;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkAllocateMemory( m_device,
                          reinterpret_cast<const VkMemoryAllocateInfo *>( &allocateInfo ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                          reinterpret_cast<VkDeviceMemory *>( &memory ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateMemoryUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::DeviceMemory, Dispatch>( memory, ObjectFree<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::mapMemory( VULKAN_HPP_NAMESPACE::DeviceMemory   memory,
                                                                   VULKAN_HPP_NAMESPACE::DeviceSize     offset,
                                                                   VULKAN_HPP_NAMESPACE::DeviceSize     size,
                                                                   VULKAN_HPP_NAMESPACE::MemoryMapFlags flags,
                                                                   void **                              ppData,
                                                                   Dispatch const &                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkMapMemory( m_device,
                                               static_cast<VkDeviceMemory>( memory ),
                                               static_cast<VkDeviceSize>( offset ),
                                               static_cast<VkDeviceSize>( size ),
                                               static_cast<VkMemoryMapFlags>( flags ),
                                               ppData ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<void *>::type Device::mapMemory( VULKAN_HPP_NAMESPACE::DeviceMemory   memory,
                                                                                                   VULKAN_HPP_NAMESPACE::DeviceSize     offset,
                                                                                                   VULKAN_HPP_NAMESPACE::DeviceSize     size,
                                                                                                   VULKAN_HPP_NAMESPACE::MemoryMapFlags flags,
                                                                                                   Dispatch const &                     d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    void *                       pData;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkMapMemory( m_device,
                                                                                                    static_cast<VkDeviceMemory>( memory ),
                                                                                                    static_cast<VkDeviceSize>( offset ),
                                                                                                    static_cast<VkDeviceSize>( size ),
                                                                                                    static_cast<VkMemoryMapFlags>( flags ),
                                                                                                    &pData ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::mapMemory" );

    return createResultValueType( result, pData );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::unmapMemory( VULKAN_HPP_NAMESPACE::DeviceMemory memory, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkUnmapMemory( m_device, static_cast<VkDeviceMemory>( memory ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::flushMappedMemoryRanges( uint32_t                                        memoryRangeCount,
                                                                                 const VULKAN_HPP_NAMESPACE::MappedMemoryRange * pMemoryRanges,
                                                                                 Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkFlushMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange *>( pMemoryRanges ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::flushMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges,
                                     Dispatch const &                                                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkFlushMappedMemoryRanges( m_device, memoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange *>( memoryRanges.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::flushMappedMemoryRanges" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::invalidateMappedMemoryRanges( uint32_t                                        memoryRangeCount,
                                                                                      const VULKAN_HPP_NAMESPACE::MappedMemoryRange * pMemoryRanges,
                                                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkInvalidateMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange *>( pMemoryRanges ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::invalidateMappedMemoryRanges( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MappedMemoryRange> const & memoryRanges,
                                          Dispatch const &                                                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkInvalidateMappedMemoryRanges( m_device, memoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange *>( memoryRanges.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::invalidateMappedMemoryRanges" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getMemoryCommitment( VULKAN_HPP_NAMESPACE::DeviceMemory memory,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize * pCommittedMemoryInBytes,
                                                      Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<VkDeviceSize *>( pCommittedMemoryInBytes ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceSize Device::getMemoryCommitment( VULKAN_HPP_NAMESPACE::DeviceMemory memory,
                                                                                                       Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DeviceSize committedMemoryInBytes;
    d.vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<VkDeviceSize *>( &committedMemoryInBytes ) );

    return committedMemoryInBytes;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::bindBufferMemory( VULKAN_HPP_NAMESPACE::Buffer       buffer,
                                                                          VULKAN_HPP_NAMESPACE::DeviceMemory memory,
                                                                          VULKAN_HPP_NAMESPACE::DeviceSize   memoryOffset,
                                                                          Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), static_cast<VkDeviceSize>( memoryOffset ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::bindBufferMemory(
    VULKAN_HPP_NAMESPACE::Buffer buffer, VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), static_cast<VkDeviceSize>( memoryOffset ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindBufferMemory" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::bindImageMemory( VULKAN_HPP_NAMESPACE::Image        image,
                                                                         VULKAN_HPP_NAMESPACE::DeviceMemory memory,
                                                                         VULKAN_HPP_NAMESPACE::DeviceSize   memoryOffset,
                                                                         Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), static_cast<VkDeviceSize>( memoryOffset ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::bindImageMemory(
    VULKAN_HPP_NAMESPACE::Image image, VULKAN_HPP_NAMESPACE::DeviceMemory memory, VULKAN_HPP_NAMESPACE::DeviceSize memoryOffset, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), static_cast<VkDeviceSize>( memoryOffset ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindImageMemory" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getBufferMemoryRequirements( VULKAN_HPP_NAMESPACE::Buffer               buffer,
                                                              VULKAN_HPP_NAMESPACE::MemoryRequirements * pMemoryRequirements,
                                                              Dispatch const &                           d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements
    Device::getBufferMemoryRequirements( VULKAN_HPP_NAMESPACE::Buffer buffer, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements memoryRequirements;
    d.vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements *>( &memoryRequirements ) );

    return memoryRequirements;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getImageMemoryRequirements( VULKAN_HPP_NAMESPACE::Image                image,
                                                             VULKAN_HPP_NAMESPACE::MemoryRequirements * pMemoryRequirements,
                                                             Dispatch const &                           d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements
                                         Device::getImageMemoryRequirements( VULKAN_HPP_NAMESPACE::Image image, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements memoryRequirements;
    d.vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements *>( &memoryRequirements ) );

    return memoryRequirements;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createFence( const VULKAN_HPP_NAMESPACE::FenceCreateInfo *     pCreateInfo,
                                                                     const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                     VULKAN_HPP_NAMESPACE::Fence *                     pFence,
                                                                     Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateFence( m_device,
                                                 reinterpret_cast<const VkFenceCreateInfo *>( pCreateInfo ),
                                                 reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                 reinterpret_cast<VkFence *>( pFence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Fence>::type Device::createFence(
    const VULKAN_HPP_NAMESPACE::FenceCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateFence( m_device,
                       reinterpret_cast<const VkFenceCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createFence" );

    return createResultValueType( result, fence );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>>::type Device::createFenceUnique(
    const VULKAN_HPP_NAMESPACE::FenceCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateFence( m_device,
                       reinterpret_cast<const VkFenceCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createFenceUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>( fence, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyFence( VULKAN_HPP_NAMESPACE::Fence                       fence,
                                               const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                               Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyFence( VULKAN_HPP_NAMESPACE::Fence                               fence,
                                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                               Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyFence( m_device,
                      static_cast<VkFence>( fence ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Fence                       fence,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Fence                               fence,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyFence( m_device,
                      static_cast<VkFence>( fence ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::resetFences( uint32_t                            fenceCount,
                                                                     const VULKAN_HPP_NAMESPACE::Fence * pFences,
                                                                     Dispatch const &                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkResetFences( m_device, fenceCount, reinterpret_cast<const VkFence *>( pFences ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::resetFences( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkResetFences( m_device, fences.size(), reinterpret_cast<const VkFence *>( fences.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::resetFences" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getFenceStatus( VULKAN_HPP_NAMESPACE::Fence fence, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Device::getFenceStatus( VULKAN_HPP_NAMESPACE::Fence fence, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceStatus", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::waitForFences( uint32_t                            fenceCount,
                                                                       const VULKAN_HPP_NAMESPACE::Fence * pFences,
                                                                       VULKAN_HPP_NAMESPACE::Bool32        waitAll,
                                                                       uint64_t                            timeout,
                                                                       Dispatch const &                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkWaitForFences( m_device, fenceCount, reinterpret_cast<const VkFence *>( pFences ), static_cast<VkBool32>( waitAll ), timeout ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result
                                         Device::waitForFences( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Fence> const & fences,
                           VULKAN_HPP_NAMESPACE::Bool32                                                waitAll,
                           uint64_t                                                                    timeout,
                           Dispatch const &                                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkWaitForFences( m_device, fences.size(), reinterpret_cast<const VkFence *>( fences.data() ), static_cast<VkBool32>( waitAll ), timeout ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitForFences", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eTimeout } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo * pCreateInfo,
                                                                         const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                         VULKAN_HPP_NAMESPACE::Semaphore *                 pSemaphore,
                                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSemaphore( m_device,
                                                     reinterpret_cast<const VkSemaphoreCreateInfo *>( pCreateInfo ),
                                                     reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                     reinterpret_cast<VkSemaphore *>( pSemaphore ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Semaphore>::type
                       Device::createSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo &         createInfo,
                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                             Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Semaphore semaphore;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSemaphore( m_device,
                           reinterpret_cast<const VkSemaphoreCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkSemaphore *>( &semaphore ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSemaphore" );

    return createResultValueType( result, semaphore );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Semaphore, Dispatch>>::type
                       Device::createSemaphoreUnique( const VULKAN_HPP_NAMESPACE::SemaphoreCreateInfo &         createInfo,
                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                   Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Semaphore semaphore;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSemaphore( m_device,
                           reinterpret_cast<const VkSemaphoreCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkSemaphore *>( &semaphore ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSemaphoreUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::Semaphore, Dispatch>( semaphore, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySemaphore( VULKAN_HPP_NAMESPACE::Semaphore                   semaphore,
                                                   const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                   Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySemaphore( VULKAN_HPP_NAMESPACE::Semaphore                           semaphore,
                                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                   Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySemaphore( m_device,
                          static_cast<VkSemaphore>( semaphore ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Semaphore                   semaphore,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Semaphore                           semaphore,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySemaphore( m_device,
                          static_cast<VkSemaphore>( semaphore ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createEvent( const VULKAN_HPP_NAMESPACE::EventCreateInfo *     pCreateInfo,
                                                                     const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                     VULKAN_HPP_NAMESPACE::Event *                     pEvent,
                                                                     Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateEvent( m_device,
                                                 reinterpret_cast<const VkEventCreateInfo *>( pCreateInfo ),
                                                 reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                 reinterpret_cast<VkEvent *>( pEvent ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Event>::type Device::createEvent(
    const VULKAN_HPP_NAMESPACE::EventCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Event  event;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateEvent( m_device,
                       reinterpret_cast<const VkEventCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkEvent *>( &event ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createEvent" );

    return createResultValueType( result, event );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Event, Dispatch>>::type Device::createEventUnique(
    const VULKAN_HPP_NAMESPACE::EventCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Event  event;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateEvent( m_device,
                       reinterpret_cast<const VkEventCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkEvent *>( &event ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createEventUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Event, Dispatch>( event, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyEvent( VULKAN_HPP_NAMESPACE::Event                       event,
                                               const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                               Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyEvent( VULKAN_HPP_NAMESPACE::Event                               event,
                                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                               Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyEvent( m_device,
                      static_cast<VkEvent>( event ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Event                       event,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Event                               event,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyEvent( m_device,
                      static_cast<VkEvent>( event ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getEventStatus( VULKAN_HPP_NAMESPACE::Event event, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Device::getEventStatus( VULKAN_HPP_NAMESPACE::Event event, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Device::getEventStatus", { VULKAN_HPP_NAMESPACE::Result::eEventSet, VULKAN_HPP_NAMESPACE::Result::eEventReset } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::setEvent( VULKAN_HPP_NAMESPACE::Event event, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::setEvent( VULKAN_HPP_NAMESPACE::Event event,
                                                                                                                   Dispatch const &            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setEvent" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::resetEvent( VULKAN_HPP_NAMESPACE::Event event, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::resetEvent( VULKAN_HPP_NAMESPACE::Event event, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::resetEvent" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createQueryPool( const VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo * pCreateInfo,
                                                                         const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                         VULKAN_HPP_NAMESPACE::QueryPool *                 pQueryPool,
                                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateQueryPool( m_device,
                                                     reinterpret_cast<const VkQueryPoolCreateInfo *>( pCreateInfo ),
                                                     reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                     reinterpret_cast<VkQueryPool *>( pQueryPool ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::QueryPool>::type
                       Device::createQueryPool( const VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo &         createInfo,
                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                             Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::QueryPool queryPool;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateQueryPool( m_device,
                           reinterpret_cast<const VkQueryPoolCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkQueryPool *>( &queryPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createQueryPool" );

    return createResultValueType( result, queryPool );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::QueryPool, Dispatch>>::type
                       Device::createQueryPoolUnique( const VULKAN_HPP_NAMESPACE::QueryPoolCreateInfo &         createInfo,
                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                   Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::QueryPool queryPool;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateQueryPool( m_device,
                           reinterpret_cast<const VkQueryPoolCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkQueryPool *>( &queryPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createQueryPoolUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::QueryPool, Dispatch>( queryPool, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getQueryPoolResults( VULKAN_HPP_NAMESPACE::QueryPool        queryPool,
                                                                             uint32_t                               firstQuery,
                                                                             uint32_t                               queryCount,
                                                                             size_t                                 dataSize,
                                                                             void *                                 pData,
                                                                             VULKAN_HPP_NAMESPACE::DeviceSize       stride,
                                                                             VULKAN_HPP_NAMESPACE::QueryResultFlags flags,
                                                                             Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetQueryPoolResults( m_device,
                                                         static_cast<VkQueryPool>( queryPool ),
                                                         firstQuery,
                                                         queryCount,
                                                         dataSize,
                                                         pData,
                                                         static_cast<VkDeviceSize>( stride ),
                                                         static_cast<VkQueryResultFlags>( flags ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DataType, typename DataTypeAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<DataType, DataTypeAllocator>>
                                         Device::getQueryPoolResults( VULKAN_HPP_NAMESPACE::QueryPool        queryPool,
                                 uint32_t                               firstQuery,
                                 uint32_t                               queryCount,
                                 size_t                                 dataSize,
                                 VULKAN_HPP_NAMESPACE::DeviceSize       stride,
                                 VULKAN_HPP_NAMESPACE::QueryResultFlags flags,
                                 Dispatch const &                       d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_ASSERT( dataSize % sizeof( DataType ) == 0 );
    std::vector<DataType, DataTypeAllocator> data( dataSize / sizeof( DataType ) );
    VULKAN_HPP_NAMESPACE::Result             result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetQueryPoolResults( m_device,
                                                                                                              static_cast<VkQueryPool>( queryPool ),
                                                                                                              firstQuery,
                                                                                                              queryCount,
                                                                                                              data.size() * sizeof( DataType ),
                                                                                                              reinterpret_cast<void *>( data.data() ),
                                                                                                              static_cast<VkDeviceSize>( stride ),
                                                                                                              static_cast<VkQueryResultFlags>( flags ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::getQueryPoolResults",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

    return ResultValue<std::vector<DataType, DataTypeAllocator>>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), data );
  }

  template <typename DataType, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<DataType> Device::getQueryPoolResult( VULKAN_HPP_NAMESPACE::QueryPool        queryPool,
                                                                                           uint32_t                               firstQuery,
                                                                                           uint32_t                               queryCount,
                                                                                           VULKAN_HPP_NAMESPACE::DeviceSize       stride,
                                                                                           VULKAN_HPP_NAMESPACE::QueryResultFlags flags,
                                                                                           Dispatch const &                       d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    DataType                     data;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetQueryPoolResults( m_device,
                                                                                                              static_cast<VkQueryPool>( queryPool ),
                                                                                                              firstQuery,
                                                                                                              queryCount,
                                                                                                              sizeof( DataType ),
                                                                                                              reinterpret_cast<void *>( &data ),
                                                                                                              static_cast<VkDeviceSize>( stride ),
                                                                                                              static_cast<VkQueryResultFlags>( flags ) ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Device::getQueryPoolResult", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eNotReady } );

    return ResultValue<DataType>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), data );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createBuffer( const VULKAN_HPP_NAMESPACE::BufferCreateInfo *    pCreateInfo,
                                                                      const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                      VULKAN_HPP_NAMESPACE::Buffer *                    pBuffer,
                                                                      Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateBuffer( m_device,
                                                  reinterpret_cast<const VkBufferCreateInfo *>( pCreateInfo ),
                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                  reinterpret_cast<VkBuffer *>( pBuffer ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Buffer>::type Device::createBuffer(
    const VULKAN_HPP_NAMESPACE::BufferCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Buffer buffer;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateBuffer( m_device,
                        reinterpret_cast<const VkBufferCreateInfo *>( &createInfo ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                        reinterpret_cast<VkBuffer *>( &buffer ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createBuffer" );

    return createResultValueType( result, buffer );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Buffer, Dispatch>>::type Device::createBufferUnique(
    const VULKAN_HPP_NAMESPACE::BufferCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Buffer buffer;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateBuffer( m_device,
                        reinterpret_cast<const VkBufferCreateInfo *>( &createInfo ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                        reinterpret_cast<VkBuffer *>( &buffer ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createBufferUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Buffer, Dispatch>( buffer, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyBuffer( VULKAN_HPP_NAMESPACE::Buffer                      buffer,
                                                const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyBuffer( VULKAN_HPP_NAMESPACE::Buffer                              buffer,
                                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyBuffer( m_device,
                       static_cast<VkBuffer>( buffer ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Buffer                      buffer,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Buffer                              buffer,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyBuffer( m_device,
                       static_cast<VkBuffer>( buffer ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createBufferView( const VULKAN_HPP_NAMESPACE::BufferViewCreateInfo * pCreateInfo,
                                                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks *  pAllocator,
                                                                          VULKAN_HPP_NAMESPACE::BufferView *                 pView,
                                                                          Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateBufferView( m_device,
                                                      reinterpret_cast<const VkBufferViewCreateInfo *>( pCreateInfo ),
                                                      reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                      reinterpret_cast<VkBufferView *>( pView ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::BufferView>::type
                       Device::createBufferView( const VULKAN_HPP_NAMESPACE::BufferViewCreateInfo &        createInfo,
                              Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                              Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::BufferView view;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateBufferView( m_device,
                            reinterpret_cast<const VkBufferViewCreateInfo *>( &createInfo ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                            reinterpret_cast<VkBufferView *>( &view ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createBufferView" );

    return createResultValueType( result, view );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::BufferView, Dispatch>>::type
                       Device::createBufferViewUnique( const VULKAN_HPP_NAMESPACE::BufferViewCreateInfo &        createInfo,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                    Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::BufferView view;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateBufferView( m_device,
                            reinterpret_cast<const VkBufferViewCreateInfo *>( &createInfo ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                            reinterpret_cast<VkBufferView *>( &view ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createBufferViewUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::BufferView, Dispatch>( view, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyBufferView( VULKAN_HPP_NAMESPACE::BufferView                  bufferView,
                                                    const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                    Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyBufferView( VULKAN_HPP_NAMESPACE::BufferView                          bufferView,
                                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                    Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyBufferView( m_device,
                           static_cast<VkBufferView>( bufferView ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::BufferView                  bufferView,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::BufferView                          bufferView,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyBufferView( m_device,
                           static_cast<VkBufferView>( bufferView ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createImage( const VULKAN_HPP_NAMESPACE::ImageCreateInfo *     pCreateInfo,
                                                                     const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                     VULKAN_HPP_NAMESPACE::Image *                     pImage,
                                                                     Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateImage( m_device,
                                                 reinterpret_cast<const VkImageCreateInfo *>( pCreateInfo ),
                                                 reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                 reinterpret_cast<VkImage *>( pImage ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Image>::type Device::createImage(
    const VULKAN_HPP_NAMESPACE::ImageCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Image  image;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateImage( m_device,
                       reinterpret_cast<const VkImageCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkImage *>( &image ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createImage" );

    return createResultValueType( result, image );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Image, Dispatch>>::type Device::createImageUnique(
    const VULKAN_HPP_NAMESPACE::ImageCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Image  image;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateImage( m_device,
                       reinterpret_cast<const VkImageCreateInfo *>( &createInfo ),
                       reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                       reinterpret_cast<VkImage *>( &image ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createImageUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Image, Dispatch>( image, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyImage( VULKAN_HPP_NAMESPACE::Image                       image,
                                               const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                               Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyImage( VULKAN_HPP_NAMESPACE::Image                               image,
                                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                               Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyImage( m_device,
                      static_cast<VkImage>( image ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Image                       image,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Image                               image,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyImage( m_device,
                      static_cast<VkImage>( image ),
                      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getImageSubresourceLayout( VULKAN_HPP_NAMESPACE::Image                    image,
                                                            const VULKAN_HPP_NAMESPACE::ImageSubresource * pSubresource,
                                                            VULKAN_HPP_NAMESPACE::SubresourceLayout *      pLayout,
                                                            Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetImageSubresourceLayout( m_device,
                                   static_cast<VkImage>( image ),
                                   reinterpret_cast<const VkImageSubresource *>( pSubresource ),
                                   reinterpret_cast<VkSubresourceLayout *>( pLayout ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::SubresourceLayout Device::getImageSubresourceLayout(
    VULKAN_HPP_NAMESPACE::Image image, const VULKAN_HPP_NAMESPACE::ImageSubresource & subresource, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SubresourceLayout layout;
    d.vkGetImageSubresourceLayout( m_device,
                                   static_cast<VkImage>( image ),
                                   reinterpret_cast<const VkImageSubresource *>( &subresource ),
                                   reinterpret_cast<VkSubresourceLayout *>( &layout ) );

    return layout;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createImageView( const VULKAN_HPP_NAMESPACE::ImageViewCreateInfo * pCreateInfo,
                                                                         const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                         VULKAN_HPP_NAMESPACE::ImageView *                 pView,
                                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateImageView( m_device,
                                                     reinterpret_cast<const VkImageViewCreateInfo *>( pCreateInfo ),
                                                     reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                     reinterpret_cast<VkImageView *>( pView ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::ImageView>::type
                       Device::createImageView( const VULKAN_HPP_NAMESPACE::ImageViewCreateInfo &         createInfo,
                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                             Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ImageView view;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateImageView( m_device,
                           reinterpret_cast<const VkImageViewCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkImageView *>( &view ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createImageView" );

    return createResultValueType( result, view );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::ImageView, Dispatch>>::type
                       Device::createImageViewUnique( const VULKAN_HPP_NAMESPACE::ImageViewCreateInfo &         createInfo,
                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                   Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ImageView view;
    VULKAN_HPP_NAMESPACE::Result    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateImageView( m_device,
                           reinterpret_cast<const VkImageViewCreateInfo *>( &createInfo ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                           reinterpret_cast<VkImageView *>( &view ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createImageViewUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::ImageView, Dispatch>( view, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyImageView( VULKAN_HPP_NAMESPACE::ImageView                   imageView,
                                                   const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                   Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyImageView( VULKAN_HPP_NAMESPACE::ImageView                           imageView,
                                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                   Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyImageView( m_device,
                          static_cast<VkImageView>( imageView ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::ImageView                   imageView,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::ImageView                           imageView,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyImageView( m_device,
                          static_cast<VkImageView>( imageView ),
                          reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createPipelineCache( const VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo * pCreateInfo,
                                                                             const VULKAN_HPP_NAMESPACE::AllocationCallbacks *     pAllocator,
                                                                             VULKAN_HPP_NAMESPACE::PipelineCache *                 pPipelineCache,
                                                                             Dispatch const &                                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreatePipelineCache( m_device,
                                                         reinterpret_cast<const VkPipelineCacheCreateInfo *>( pCreateInfo ),
                                                         reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                         reinterpret_cast<VkPipelineCache *>( pPipelineCache ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::PipelineCache>::type
                       Device::createPipelineCache( const VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo &     createInfo,
                                 Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                 Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PipelineCache pipelineCache;
    VULKAN_HPP_NAMESPACE::Result        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePipelineCache( m_device,
                               reinterpret_cast<const VkPipelineCacheCreateInfo *>( &createInfo ),
                               reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                               reinterpret_cast<VkPipelineCache *>( &pipelineCache ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPipelineCache" );

    return createResultValueType( result, pipelineCache );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::PipelineCache, Dispatch>>::type
                       Device::createPipelineCacheUnique( const VULKAN_HPP_NAMESPACE::PipelineCacheCreateInfo &     createInfo,
                                       Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                       Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PipelineCache pipelineCache;
    VULKAN_HPP_NAMESPACE::Result        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePipelineCache( m_device,
                               reinterpret_cast<const VkPipelineCacheCreateInfo *>( &createInfo ),
                               reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                               reinterpret_cast<VkPipelineCache *>( &pipelineCache ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPipelineCacheUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::PipelineCache, Dispatch>( pipelineCache, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipelineCache( VULKAN_HPP_NAMESPACE::PipelineCache               pipelineCache,
                                                       const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                       Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipelineCache( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                                       Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                       Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipelineCache( m_device,
                              static_cast<VkPipelineCache>( pipelineCache ),
                              reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PipelineCache               pipelineCache,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipelineCache( m_device,
                              static_cast<VkPipelineCache>( pipelineCache ),
                              reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createGraphicsPipelines( VULKAN_HPP_NAMESPACE::PipelineCache                      pipelineCache,
                                                                                 uint32_t                                                 createInfoCount,
                                                                                 const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo * pCreateInfos,
                                                                                 const VULKAN_HPP_NAMESPACE::AllocationCallbacks *        pAllocator,
                                                                                 VULKAN_HPP_NAMESPACE::Pipeline *                         pPipelines,
                                                                                 Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateGraphicsPipelines( m_device,
                                                             static_cast<VkPipelineCache>( pipelineCache ),
                                                             createInfoCount,
                                                             reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( pCreateInfos ),
                                                             reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                             reinterpret_cast<VkPipeline *>( pPipelines ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PipelineAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>
                                         Device::createGraphicsPipelines( VULKAN_HPP_NAMESPACE::PipelineCache                                                              pipelineCache,
                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                        allocator,
                                     Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipelines",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipelines );
  }

  template <typename PipelineAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, VULKAN_HPP_NAMESPACE::Pipeline>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>
                                         Device::createGraphicsPipelines( VULKAN_HPP_NAMESPACE::PipelineCache                                                              pipelineCache,
                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                        allocator,
                                     PipelineAllocator &                                                                              pipelineAllocator,
                                     Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator> pipelines( createInfos.size(), pipelineAllocator );
    VULKAN_HPP_NAMESPACE::Result                                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipelines",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipelines );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<VULKAN_HPP_NAMESPACE::Pipeline>
                                         Device::createGraphicsPipeline( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                    const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo &  createInfo,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                    Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Pipeline pipeline;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      1,
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( &pipeline ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipeline",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<VULKAN_HPP_NAMESPACE::Pipeline>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipeline );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch, typename PipelineAllocator>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>
                                         Device::createGraphicsPipelinesUnique( VULKAN_HPP_NAMESPACE::PipelineCache                                                              pipelineCache,
                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                           Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                        allocator,
                                           Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipelinesUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator> uniquePipelines;
    uniquePipelines.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & pipeline : pipelines )
    {
      uniquePipelines.push_back( UniqueHandle<Pipeline, Dispatch>( pipeline, deleter ) );
    }
    return ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), std::move( uniquePipelines ) );
  }

  template <typename Dispatch,
            typename PipelineAllocator,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>
                                         Device::createGraphicsPipelinesUnique( VULKAN_HPP_NAMESPACE::PipelineCache                                                              pipelineCache,
                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo> const & createInfos,
                                           Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                        allocator,
                                           PipelineAllocator &                                                                              pipelineAllocator,
                                           Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipelinesUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator> uniquePipelines( pipelineAllocator );
    uniquePipelines.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & pipeline : pipelines )
    {
      uniquePipelines.push_back( UniqueHandle<Pipeline, Dispatch>( pipeline, deleter ) );
    }
    return ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), std::move( uniquePipelines ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>
                                         Device::createGraphicsPipelineUnique( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                          const VULKAN_HPP_NAMESPACE::GraphicsPipelineCreateInfo &  createInfo,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Pipeline pipeline;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateGraphicsPipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      1,
      reinterpret_cast<const VkGraphicsPipelineCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( &pipeline ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createGraphicsPipelineUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ),
      UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>( pipeline, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createComputePipelines( VULKAN_HPP_NAMESPACE::PipelineCache                     pipelineCache,
                                                                                uint32_t                                                createInfoCount,
                                                                                const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo * pCreateInfos,
                                                                                const VULKAN_HPP_NAMESPACE::AllocationCallbacks *       pAllocator,
                                                                                VULKAN_HPP_NAMESPACE::Pipeline *                        pPipelines,
                                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateComputePipelines( m_device,
                                                            static_cast<VkPipelineCache>( pipelineCache ),
                                                            createInfoCount,
                                                            reinterpret_cast<const VkComputePipelineCreateInfo *>( pCreateInfos ),
                                                            reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                            reinterpret_cast<VkPipeline *>( pPipelines ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PipelineAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>
                                         Device::createComputePipelines( VULKAN_HPP_NAMESPACE::PipelineCache                                                             pipelineCache,
                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                       allocator,
                                    Dispatch const &                                                                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkComputePipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipelines",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipelines );
  }

  template <typename PipelineAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, VULKAN_HPP_NAMESPACE::Pipeline>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>
                                         Device::createComputePipelines( VULKAN_HPP_NAMESPACE::PipelineCache                                                             pipelineCache,
                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                       allocator,
                                    PipelineAllocator &                                                                             pipelineAllocator,
                                    Dispatch const &                                                                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator> pipelines( createInfos.size(), pipelineAllocator );
    VULKAN_HPP_NAMESPACE::Result                                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkComputePipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipelines",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<std::vector<VULKAN_HPP_NAMESPACE::Pipeline, PipelineAllocator>>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipelines );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<VULKAN_HPP_NAMESPACE::Pipeline>
                                         Device::createComputePipeline( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                   const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo &   createInfo,
                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                   Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Pipeline pipeline;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      1,
      reinterpret_cast<const VkComputePipelineCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( &pipeline ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipeline",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<VULKAN_HPP_NAMESPACE::Pipeline>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), pipeline );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch, typename PipelineAllocator>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>
                                         Device::createComputePipelinesUnique( VULKAN_HPP_NAMESPACE::PipelineCache                                                             pipelineCache,
                                          VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                       allocator,
                                          Dispatch const &                                                                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkComputePipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipelinesUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator> uniquePipelines;
    uniquePipelines.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & pipeline : pipelines )
    {
      uniquePipelines.push_back( UniqueHandle<Pipeline, Dispatch>( pipeline, deleter ) );
    }
    return ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), std::move( uniquePipelines ) );
  }

  template <typename Dispatch,
            typename PipelineAllocator,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>
                                         Device::createComputePipelinesUnique( VULKAN_HPP_NAMESPACE::PipelineCache                                                             pipelineCache,
                                          VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo> const & createInfos,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                       allocator,
                                          PipelineAllocator &                                                                             pipelineAllocator,
                                          Dispatch const &                                                                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Pipeline> pipelines( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      createInfos.size(),
      reinterpret_cast<const VkComputePipelineCreateInfo *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( pipelines.data() ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipelinesUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator> uniquePipelines( pipelineAllocator );
    uniquePipelines.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & pipeline : pipelines )
    {
      uniquePipelines.push_back( UniqueHandle<Pipeline, Dispatch>( pipeline, deleter ) );
    }
    return ResultValue<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>, PipelineAllocator>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), std::move( uniquePipelines ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>
                                         Device::createComputePipelineUnique( VULKAN_HPP_NAMESPACE::PipelineCache                       pipelineCache,
                                         const VULKAN_HPP_NAMESPACE::ComputePipelineCreateInfo &   createInfo,
                                         Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                         Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Pipeline pipeline;
    VULKAN_HPP_NAMESPACE::Result   result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateComputePipelines(
      m_device,
      static_cast<VkPipelineCache>( pipelineCache ),
      1,
      reinterpret_cast<const VkComputePipelineCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkPipeline *>( &pipeline ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::createComputePipelineUnique",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::ePipelineCompileRequiredEXT } );

    return ResultValue<UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>>(
      static_cast<VULKAN_HPP_NAMESPACE::Result>( result ),
      UniqueHandle<VULKAN_HPP_NAMESPACE::Pipeline, Dispatch>( pipeline, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipeline( VULKAN_HPP_NAMESPACE::Pipeline                    pipeline,
                                                  const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                  Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipeline( VULKAN_HPP_NAMESPACE::Pipeline                            pipeline,
                                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                  Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipeline( m_device,
                         static_cast<VkPipeline>( pipeline ),
                         reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Pipeline                    pipeline,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Pipeline                            pipeline,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipeline( m_device,
                         static_cast<VkPipeline>( pipeline ),
                         reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createPipelineLayout( const VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo * pCreateInfo,
                                                                              const VULKAN_HPP_NAMESPACE::AllocationCallbacks *      pAllocator,
                                                                              VULKAN_HPP_NAMESPACE::PipelineLayout *                 pPipelineLayout,
                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreatePipelineLayout( m_device,
                                                          reinterpret_cast<const VkPipelineLayoutCreateInfo *>( pCreateInfo ),
                                                          reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                          reinterpret_cast<VkPipelineLayout *>( pPipelineLayout ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::PipelineLayout>::type
                       Device::createPipelineLayout( const VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo &    createInfo,
                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                  Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PipelineLayout pipelineLayout;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePipelineLayout( m_device,
                                reinterpret_cast<const VkPipelineLayoutCreateInfo *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkPipelineLayout *>( &pipelineLayout ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPipelineLayout" );

    return createResultValueType( result, pipelineLayout );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::PipelineLayout, Dispatch>>::type
                       Device::createPipelineLayoutUnique( const VULKAN_HPP_NAMESPACE::PipelineLayoutCreateInfo &    createInfo,
                                        Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                        Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PipelineLayout pipelineLayout;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePipelineLayout( m_device,
                                reinterpret_cast<const VkPipelineLayoutCreateInfo *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkPipelineLayout *>( &pipelineLayout ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPipelineLayoutUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::PipelineLayout, Dispatch>( pipelineLayout, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipelineLayout( VULKAN_HPP_NAMESPACE::PipelineLayout              pipelineLayout,
                                                        const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                        Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPipelineLayout( VULKAN_HPP_NAMESPACE::PipelineLayout                      pipelineLayout,
                                                        Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                        Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipelineLayout( m_device,
                               static_cast<VkPipelineLayout>( pipelineLayout ),
                               reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PipelineLayout              pipelineLayout,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PipelineLayout                      pipelineLayout,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPipelineLayout( m_device,
                               static_cast<VkPipelineLayout>( pipelineLayout ),
                               reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createSampler( const VULKAN_HPP_NAMESPACE::SamplerCreateInfo *   pCreateInfo,
                                                                       const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                       VULKAN_HPP_NAMESPACE::Sampler *                   pSampler,
                                                                       Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSampler( m_device,
                                                   reinterpret_cast<const VkSamplerCreateInfo *>( pCreateInfo ),
                                                   reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                   reinterpret_cast<VkSampler *>( pSampler ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Sampler>::type Device::createSampler(
    const VULKAN_HPP_NAMESPACE::SamplerCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Sampler sampler;
    VULKAN_HPP_NAMESPACE::Result  result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSampler( m_device,
                         reinterpret_cast<const VkSamplerCreateInfo *>( &createInfo ),
                         reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                         reinterpret_cast<VkSampler *>( &sampler ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSampler" );

    return createResultValueType( result, sampler );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Sampler, Dispatch>>::type Device::createSamplerUnique(
    const VULKAN_HPP_NAMESPACE::SamplerCreateInfo & createInfo, Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Sampler sampler;
    VULKAN_HPP_NAMESPACE::Result  result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSampler( m_device,
                         reinterpret_cast<const VkSamplerCreateInfo *>( &createInfo ),
                         reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                         reinterpret_cast<VkSampler *>( &sampler ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSamplerUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Sampler, Dispatch>( sampler, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySampler( VULKAN_HPP_NAMESPACE::Sampler                     sampler,
                                                 const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                 Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySampler( VULKAN_HPP_NAMESPACE::Sampler                             sampler,
                                                 Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                 Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySampler( m_device,
                        static_cast<VkSampler>( sampler ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Sampler                     sampler,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Sampler                             sampler,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySampler( m_device,
                        static_cast<VkSampler>( sampler ),
                        reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createDescriptorSetLayout( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo * pCreateInfo,
                                                                                   const VULKAN_HPP_NAMESPACE::AllocationCallbacks *           pAllocator,
                                                                                   VULKAN_HPP_NAMESPACE::DescriptorSetLayout *                 pSetLayout,
                                                                                   Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDescriptorSetLayout( m_device,
                                                               reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( pCreateInfo ),
                                                               reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                               reinterpret_cast<VkDescriptorSetLayout *>( pSetLayout ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DescriptorSetLayout>::type
                       Device::createDescriptorSetLayout( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo,
                                       Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>   allocator,
                                       Dispatch const &                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DescriptorSetLayout setLayout;
    VULKAN_HPP_NAMESPACE::Result              result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDescriptorSetLayout(
      m_device,
      reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkDescriptorSetLayout *>( &setLayout ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createDescriptorSetLayout" );

    return createResultValueType( result, setLayout );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSetLayout, Dispatch>>::type
                       Device::createDescriptorSetLayoutUnique( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo,
                                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>   allocator,
                                             Dispatch const &                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DescriptorSetLayout setLayout;
    VULKAN_HPP_NAMESPACE::Result              result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDescriptorSetLayout(
      m_device,
      reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkDescriptorSetLayout *>( &setLayout ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createDescriptorSetLayoutUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSetLayout, Dispatch>( setLayout, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyDescriptorSetLayout( VULKAN_HPP_NAMESPACE::DescriptorSetLayout         descriptorSetLayout,
                                                             const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                             Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyDescriptorSetLayout(
      m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyDescriptorSetLayout( VULKAN_HPP_NAMESPACE::DescriptorSetLayout                 descriptorSetLayout,
                                                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                             Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyDescriptorSetLayout(
      m_device,
      static_cast<VkDescriptorSetLayout>( descriptorSetLayout ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::DescriptorSetLayout         descriptorSetLayout,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyDescriptorSetLayout(
      m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::DescriptorSetLayout                 descriptorSetLayout,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyDescriptorSetLayout(
      m_device,
      static_cast<VkDescriptorSetLayout>( descriptorSetLayout ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createDescriptorPool( const VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo * pCreateInfo,
                                                                              const VULKAN_HPP_NAMESPACE::AllocationCallbacks *      pAllocator,
                                                                              VULKAN_HPP_NAMESPACE::DescriptorPool *                 pDescriptorPool,
                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDescriptorPool( m_device,
                                                          reinterpret_cast<const VkDescriptorPoolCreateInfo *>( pCreateInfo ),
                                                          reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                          reinterpret_cast<VkDescriptorPool *>( pDescriptorPool ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DescriptorPool>::type
                       Device::createDescriptorPool( const VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo &    createInfo,
                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                  Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DescriptorPool descriptorPool;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDescriptorPool( m_device,
                                reinterpret_cast<const VkDescriptorPoolCreateInfo *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkDescriptorPool *>( &descriptorPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createDescriptorPool" );

    return createResultValueType( result, descriptorPool );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorPool, Dispatch>>::type
                       Device::createDescriptorPoolUnique( const VULKAN_HPP_NAMESPACE::DescriptorPoolCreateInfo &    createInfo,
                                        Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                        Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DescriptorPool descriptorPool;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDescriptorPool( m_device,
                                reinterpret_cast<const VkDescriptorPoolCreateInfo *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkDescriptorPool *>( &descriptorPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createDescriptorPoolUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorPool, Dispatch>( descriptorPool, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE Result Device::resetDescriptorPool( VULKAN_HPP_NAMESPACE::DescriptorPool           descriptorPool,
                                                        VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlags flags,
                                                        Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::resetDescriptorPool( VULKAN_HPP_NAMESPACE::DescriptorPool           descriptorPool,
                                                      VULKAN_HPP_NAMESPACE::DescriptorPoolResetFlags flags,
                                                      Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::allocateDescriptorSets( const VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo * pAllocateInfo,
                                                                                VULKAN_HPP_NAMESPACE::DescriptorSet *                   pDescriptorSets,
                                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAllocateDescriptorSets(
      m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo *>( pAllocateInfo ), reinterpret_cast<VkDescriptorSet *>( pDescriptorSets ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DescriptorSetAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet, DescriptorSetAllocator>>::type
                       Device::allocateDescriptorSets( const VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo & allocateInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet, DescriptorSetAllocator> descriptorSets( allocateInfo.descriptorSetCount );
    VULKAN_HPP_NAMESPACE::Result                                             result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateDescriptorSets(
      m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkDescriptorSet *>( descriptorSets.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateDescriptorSets" );

    return createResultValueType( result, descriptorSets );
  }

  template <typename DescriptorSetAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, VULKAN_HPP_NAMESPACE::DescriptorSet>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet, DescriptorSetAllocator>>::type
                       Device::allocateDescriptorSets( const VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo & allocateInfo,
                                    DescriptorSetAllocator &                                descriptorSetAllocator,
                                    Dispatch const &                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet, DescriptorSetAllocator> descriptorSets( allocateInfo.descriptorSetCount, descriptorSetAllocator );
    VULKAN_HPP_NAMESPACE::Result                                             result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateDescriptorSets(
      m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkDescriptorSet *>( descriptorSets.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateDescriptorSets" );

    return createResultValueType( result, descriptorSets );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch, typename DescriptorSetAllocator>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSet, Dispatch>, DescriptorSetAllocator>>::type
    Device::allocateDescriptorSetsUnique( const VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo & allocateInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet> descriptorSets( allocateInfo.descriptorSetCount );
    VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateDescriptorSets(
      m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkDescriptorSet *>( descriptorSets.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateDescriptorSetsUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSet, Dispatch>, DescriptorSetAllocator> uniqueDescriptorSets;
    uniqueDescriptorSets.reserve( allocateInfo.descriptorSetCount );
    PoolFree<Device, DescriptorPool, Dispatch> deleter( *this, allocateInfo.descriptorPool, d );
    for ( auto const & descriptorSet : descriptorSets )
    {
      uniqueDescriptorSets.push_back( UniqueHandle<DescriptorSet, Dispatch>( descriptorSet, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueDescriptorSets ) );
  }

  template <typename Dispatch,
            typename DescriptorSetAllocator,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSet, Dispatch>>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSet, Dispatch>, DescriptorSetAllocator>>::type
    Device::allocateDescriptorSetsUnique( const VULKAN_HPP_NAMESPACE::DescriptorSetAllocateInfo & allocateInfo,
                                          DescriptorSetAllocator &                                descriptorSetAllocator,
                                          Dispatch const &                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DescriptorSet> descriptorSets( allocateInfo.descriptorSetCount );
    VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateDescriptorSets(
      m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkDescriptorSet *>( descriptorSets.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateDescriptorSetsUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::DescriptorSet, Dispatch>, DescriptorSetAllocator> uniqueDescriptorSets( descriptorSetAllocator );
    uniqueDescriptorSets.reserve( allocateInfo.descriptorSetCount );
    PoolFree<Device, DescriptorPool, Dispatch> deleter( *this, allocateInfo.descriptorPool, d );
    for ( auto const & descriptorSet : descriptorSets )
    {
      uniqueDescriptorSets.push_back( UniqueHandle<DescriptorSet, Dispatch>( descriptorSet, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueDescriptorSets ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE Result Device::freeDescriptorSets( VULKAN_HPP_NAMESPACE::DescriptorPool        descriptorPool,
                                                       uint32_t                                    descriptorSetCount,
                                                       const VULKAN_HPP_NAMESPACE::DescriptorSet * pDescriptorSets,
                                                       Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkFreeDescriptorSets(
      m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSetCount, reinterpret_cast<const VkDescriptorSet *>( pDescriptorSets ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::freeDescriptorSets( VULKAN_HPP_NAMESPACE::DescriptorPool                                                descriptorPool,
                                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DescriptorSet> const & descriptorSets,
                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkFreeDescriptorSets(
      m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSets.size(), reinterpret_cast<const VkDescriptorSet *>( descriptorSets.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE Result( Device::free )( VULKAN_HPP_NAMESPACE::DescriptorPool        descriptorPool,
                                            uint32_t                                    descriptorSetCount,
                                            const VULKAN_HPP_NAMESPACE::DescriptorSet * pDescriptorSets,
                                            Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkFreeDescriptorSets(
      m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSetCount, reinterpret_cast<const VkDescriptorSet *>( pDescriptorSets ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void( Device::free )( VULKAN_HPP_NAMESPACE::DescriptorPool                                                descriptorPool,
                                          VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DescriptorSet> const & descriptorSets,
                                          Dispatch const &                                                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkFreeDescriptorSets(
      m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSets.size(), reinterpret_cast<const VkDescriptorSet *>( descriptorSets.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::updateDescriptorSets( uint32_t                                         descriptorWriteCount,
                                                       const VULKAN_HPP_NAMESPACE::WriteDescriptorSet * pDescriptorWrites,
                                                       uint32_t                                         descriptorCopyCount,
                                                       const VULKAN_HPP_NAMESPACE::CopyDescriptorSet *  pDescriptorCopies,
                                                       Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkUpdateDescriptorSets( m_device,
                              descriptorWriteCount,
                              reinterpret_cast<const VkWriteDescriptorSet *>( pDescriptorWrites ),
                              descriptorCopyCount,
                              reinterpret_cast<const VkCopyDescriptorSet *>( pDescriptorCopies ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    Device::updateDescriptorSets( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::WriteDescriptorSet> const & descriptorWrites,
                                  VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CopyDescriptorSet> const &  descriptorCopies,
                                  Dispatch const &                                                                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkUpdateDescriptorSets( m_device,
                              descriptorWrites.size(),
                              reinterpret_cast<const VkWriteDescriptorSet *>( descriptorWrites.data() ),
                              descriptorCopies.size(),
                              reinterpret_cast<const VkCopyDescriptorSet *>( descriptorCopies.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createFramebuffer( const VULKAN_HPP_NAMESPACE::FramebufferCreateInfo * pCreateInfo,
                                                                           const VULKAN_HPP_NAMESPACE::AllocationCallbacks *   pAllocator,
                                                                           VULKAN_HPP_NAMESPACE::Framebuffer *                 pFramebuffer,
                                                                           Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateFramebuffer( m_device,
                                                       reinterpret_cast<const VkFramebufferCreateInfo *>( pCreateInfo ),
                                                       reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                       reinterpret_cast<VkFramebuffer *>( pFramebuffer ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Framebuffer>::type
                       Device::createFramebuffer( const VULKAN_HPP_NAMESPACE::FramebufferCreateInfo &       createInfo,
                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                               Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Framebuffer framebuffer;
    VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateFramebuffer( m_device,
                             reinterpret_cast<const VkFramebufferCreateInfo *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkFramebuffer *>( &framebuffer ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createFramebuffer" );

    return createResultValueType( result, framebuffer );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Framebuffer, Dispatch>>::type
                       Device::createFramebufferUnique( const VULKAN_HPP_NAMESPACE::FramebufferCreateInfo &       createInfo,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                     Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Framebuffer framebuffer;
    VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateFramebuffer( m_device,
                             reinterpret_cast<const VkFramebufferCreateInfo *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkFramebuffer *>( &framebuffer ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createFramebufferUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::Framebuffer, Dispatch>( framebuffer, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyFramebuffer( VULKAN_HPP_NAMESPACE::Framebuffer                 framebuffer,
                                                     const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                     Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyFramebuffer( VULKAN_HPP_NAMESPACE::Framebuffer                         framebuffer,
                                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                     Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyFramebuffer( m_device,
                            static_cast<VkFramebuffer>( framebuffer ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Framebuffer                 framebuffer,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::Framebuffer                         framebuffer,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyFramebuffer( m_device,
                            static_cast<VkFramebuffer>( framebuffer ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo * pCreateInfo,
                                                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks *  pAllocator,
                                                                          VULKAN_HPP_NAMESPACE::RenderPass *                 pRenderPass,
                                                                          Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateRenderPass( m_device,
                                                      reinterpret_cast<const VkRenderPassCreateInfo *>( pCreateInfo ),
                                                      reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                      reinterpret_cast<VkRenderPass *>( pRenderPass ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::RenderPass>::type
                       Device::createRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo &        createInfo,
                              Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                              Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::RenderPass renderPass;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateRenderPass( m_device,
                            reinterpret_cast<const VkRenderPassCreateInfo *>( &createInfo ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                            reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createRenderPass" );

    return createResultValueType( result, renderPass );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::RenderPass, Dispatch>>::type
                       Device::createRenderPassUnique( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo &        createInfo,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                    Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::RenderPass renderPass;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateRenderPass( m_device,
                            reinterpret_cast<const VkRenderPassCreateInfo *>( &createInfo ),
                            reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                            reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createRenderPassUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::RenderPass, Dispatch>( renderPass, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyRenderPass( VULKAN_HPP_NAMESPACE::RenderPass                  renderPass,
                                                    const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                    Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyRenderPass( VULKAN_HPP_NAMESPACE::RenderPass                          renderPass,
                                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                    Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyRenderPass( m_device,
                           static_cast<VkRenderPass>( renderPass ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::RenderPass                  renderPass,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::RenderPass                          renderPass,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyRenderPass( m_device,
                           static_cast<VkRenderPass>( renderPass ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getRenderAreaGranularity( VULKAN_HPP_NAMESPACE::RenderPass renderPass,
                                                           VULKAN_HPP_NAMESPACE::Extent2D * pGranularity,
                                                           Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D *>( pGranularity ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Extent2D Device::getRenderAreaGranularity( VULKAN_HPP_NAMESPACE::RenderPass renderPass,
                                                                                                          Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Extent2D granularity;
    d.vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D *>( &granularity ) );

    return granularity;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createCommandPool( const VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo * pCreateInfo,
                                                                           const VULKAN_HPP_NAMESPACE::AllocationCallbacks *   pAllocator,
                                                                           VULKAN_HPP_NAMESPACE::CommandPool *                 pCommandPool,
                                                                           Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateCommandPool( m_device,
                                                       reinterpret_cast<const VkCommandPoolCreateInfo *>( pCreateInfo ),
                                                       reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                       reinterpret_cast<VkCommandPool *>( pCommandPool ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::CommandPool>::type
                       Device::createCommandPool( const VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo &       createInfo,
                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                               Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::CommandPool commandPool;
    VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateCommandPool( m_device,
                             reinterpret_cast<const VkCommandPoolCreateInfo *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkCommandPool *>( &commandPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createCommandPool" );

    return createResultValueType( result, commandPool );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::CommandPool, Dispatch>>::type
                       Device::createCommandPoolUnique( const VULKAN_HPP_NAMESPACE::CommandPoolCreateInfo &       createInfo,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                     Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::CommandPool commandPool;
    VULKAN_HPP_NAMESPACE::Result      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateCommandPool( m_device,
                             reinterpret_cast<const VkCommandPoolCreateInfo *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkCommandPool *>( &commandPool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createCommandPoolUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::CommandPool, Dispatch>( commandPool, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::resetCommandPool( VULKAN_HPP_NAMESPACE::CommandPool           commandPool,
                                                                          VULKAN_HPP_NAMESPACE::CommandPoolResetFlags flags,
                                                                          Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::resetCommandPool( VULKAN_HPP_NAMESPACE::CommandPool commandPool, VULKAN_HPP_NAMESPACE::CommandPoolResetFlags flags, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::resetCommandPool" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::allocateCommandBuffers( const VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo * pAllocateInfo,
                                                                                VULKAN_HPP_NAMESPACE::CommandBuffer *                   pCommandBuffers,
                                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAllocateCommandBuffers(
      m_device, reinterpret_cast<const VkCommandBufferAllocateInfo *>( pAllocateInfo ), reinterpret_cast<VkCommandBuffer *>( pCommandBuffers ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename CommandBufferAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer, CommandBufferAllocator>>::type
                       Device::allocateCommandBuffers( const VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo & allocateInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer, CommandBufferAllocator> commandBuffers( allocateInfo.commandBufferCount );
    VULKAN_HPP_NAMESPACE::Result                                             result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateCommandBuffers(
      m_device, reinterpret_cast<const VkCommandBufferAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkCommandBuffer *>( commandBuffers.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateCommandBuffers" );

    return createResultValueType( result, commandBuffers );
  }

  template <typename CommandBufferAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, VULKAN_HPP_NAMESPACE::CommandBuffer>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer, CommandBufferAllocator>>::type
                       Device::allocateCommandBuffers( const VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo & allocateInfo,
                                    CommandBufferAllocator &                                commandBufferAllocator,
                                    Dispatch const &                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer, CommandBufferAllocator> commandBuffers( allocateInfo.commandBufferCount, commandBufferAllocator );
    VULKAN_HPP_NAMESPACE::Result                                             result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateCommandBuffers(
      m_device, reinterpret_cast<const VkCommandBufferAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkCommandBuffer *>( commandBuffers.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateCommandBuffers" );

    return createResultValueType( result, commandBuffers );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch, typename CommandBufferAllocator>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::CommandBuffer, Dispatch>, CommandBufferAllocator>>::type
    Device::allocateCommandBuffersUnique( const VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo & allocateInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer> commandBuffers( allocateInfo.commandBufferCount );
    VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateCommandBuffers(
      m_device, reinterpret_cast<const VkCommandBufferAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkCommandBuffer *>( commandBuffers.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateCommandBuffersUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::CommandBuffer, Dispatch>, CommandBufferAllocator> uniqueCommandBuffers;
    uniqueCommandBuffers.reserve( allocateInfo.commandBufferCount );
    PoolFree<Device, CommandPool, Dispatch> deleter( *this, allocateInfo.commandPool, d );
    for ( auto const & commandBuffer : commandBuffers )
    {
      uniqueCommandBuffers.push_back( UniqueHandle<CommandBuffer, Dispatch>( commandBuffer, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueCommandBuffers ) );
  }

  template <typename Dispatch,
            typename CommandBufferAllocator,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, UniqueHandle<VULKAN_HPP_NAMESPACE::CommandBuffer, Dispatch>>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::CommandBuffer, Dispatch>, CommandBufferAllocator>>::type
    Device::allocateCommandBuffersUnique( const VULKAN_HPP_NAMESPACE::CommandBufferAllocateInfo & allocateInfo,
                                          CommandBufferAllocator &                                commandBufferAllocator,
                                          Dispatch const &                                        d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CommandBuffer> commandBuffers( allocateInfo.commandBufferCount );
    VULKAN_HPP_NAMESPACE::Result                     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAllocateCommandBuffers(
      m_device, reinterpret_cast<const VkCommandBufferAllocateInfo *>( &allocateInfo ), reinterpret_cast<VkCommandBuffer *>( commandBuffers.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::allocateCommandBuffersUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::CommandBuffer, Dispatch>, CommandBufferAllocator> uniqueCommandBuffers( commandBufferAllocator );
    uniqueCommandBuffers.reserve( allocateInfo.commandBufferCount );
    PoolFree<Device, CommandPool, Dispatch> deleter( *this, allocateInfo.commandPool, d );
    for ( auto const & commandBuffer : commandBuffers )
    {
      uniqueCommandBuffers.push_back( UniqueHandle<CommandBuffer, Dispatch>( commandBuffer, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueCommandBuffers ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::freeCommandBuffers( VULKAN_HPP_NAMESPACE::CommandPool           commandPool,
                                                     uint32_t                                    commandBufferCount,
                                                     const VULKAN_HPP_NAMESPACE::CommandBuffer * pCommandBuffers,
                                                     Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkFreeCommandBuffers(
      m_device, static_cast<VkCommandPool>( commandPool ), commandBufferCount, reinterpret_cast<const VkCommandBuffer *>( pCommandBuffers ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::freeCommandBuffers( VULKAN_HPP_NAMESPACE::CommandPool                                                   commandPool,
                                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CommandBuffer> const & commandBuffers,
                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkFreeCommandBuffers(
      m_device, static_cast<VkCommandPool>( commandPool ), commandBuffers.size(), reinterpret_cast<const VkCommandBuffer *>( commandBuffers.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void( Device::free )( VULKAN_HPP_NAMESPACE::CommandPool           commandPool,
                                          uint32_t                                    commandBufferCount,
                                          const VULKAN_HPP_NAMESPACE::CommandBuffer * pCommandBuffers,
                                          Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkFreeCommandBuffers(
      m_device, static_cast<VkCommandPool>( commandPool ), commandBufferCount, reinterpret_cast<const VkCommandBuffer *>( pCommandBuffers ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void( Device::free )( VULKAN_HPP_NAMESPACE::CommandPool                                                   commandPool,
                                          VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CommandBuffer> const & commandBuffers,
                                          Dispatch const &                                                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkFreeCommandBuffers(
      m_device, static_cast<VkCommandPool>( commandPool ), commandBuffers.size(), reinterpret_cast<const VkCommandBuffer *>( commandBuffers.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result CommandBuffer::begin( const VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo * pBeginInfo,
                                                                      Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo *>( pBeginInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
                                          CommandBuffer::begin( const VULKAN_HPP_NAMESPACE::CommandBufferBeginInfo & beginInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo *>( &beginInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::begin" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result CommandBuffer::end( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEndCommandBuffer( m_commandBuffer ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type CommandBuffer::end( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEndCommandBuffer( m_commandBuffer ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::end" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result CommandBuffer::reset( VULKAN_HPP_NAMESPACE::CommandBufferResetFlags flags,
                                                                      Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type CommandBuffer::reset( VULKAN_HPP_NAMESPACE::CommandBufferResetFlags flags, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::reset" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindPipeline( VULKAN_HPP_NAMESPACE::PipelineBindPoint pipelineBindPoint,
                                                      VULKAN_HPP_NAMESPACE::Pipeline          pipeline,
                                                      Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindPipeline( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipeline>( pipeline ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewport( uint32_t                               firstViewport,
                                                     uint32_t                               viewportCount,
                                                     const VULKAN_HPP_NAMESPACE::Viewport * pViewports,
                                                     Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetViewport( m_commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport *>( pViewports ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewport( uint32_t                                                                       firstViewport,
                                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports,
                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetViewport( m_commandBuffer, firstViewport, viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setScissor( uint32_t                             firstScissor,
                                                    uint32_t                             scissorCount,
                                                    const VULKAN_HPP_NAMESPACE::Rect2D * pScissors,
                                                    Dispatch const &                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetScissor( m_commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D *>( pScissors ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setScissor( uint32_t                                                                     firstScissor,
                                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors,
                                                    Dispatch const &                                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetScissor( m_commandBuffer, firstScissor, scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setLineWidth( float lineWidth, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetLineWidth( m_commandBuffer, lineWidth );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBias( m_commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setBlendConstants( const float blendConstants[4], Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetBlendConstants( m_commandBuffer, blendConstants );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthBounds( float minDepthBounds, float maxDepthBounds, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBounds( m_commandBuffer, minDepthBounds, maxDepthBounds );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setStencilCompareMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t compareMask, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilCompareMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setStencilWriteMask( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t writeMask, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilWriteMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setStencilReference( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask, uint32_t reference, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilReference( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindDescriptorSets( VULKAN_HPP_NAMESPACE::PipelineBindPoint     pipelineBindPoint,
                                                            VULKAN_HPP_NAMESPACE::PipelineLayout        layout,
                                                            uint32_t                                    firstSet,
                                                            uint32_t                                    descriptorSetCount,
                                                            const VULKAN_HPP_NAMESPACE::DescriptorSet * pDescriptorSets,
                                                            uint32_t                                    dynamicOffsetCount,
                                                            const uint32_t *                            pDynamicOffsets,
                                                            Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindDescriptorSets( m_commandBuffer,
                               static_cast<VkPipelineBindPoint>( pipelineBindPoint ),
                               static_cast<VkPipelineLayout>( layout ),
                               firstSet,
                               descriptorSetCount,
                               reinterpret_cast<const VkDescriptorSet *>( pDescriptorSets ),
                               dynamicOffsetCount,
                               pDynamicOffsets );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindDescriptorSets( VULKAN_HPP_NAMESPACE::PipelineBindPoint pipelineBindPoint,
                                                            VULKAN_HPP_NAMESPACE::PipelineLayout    layout,
                                                            uint32_t                                firstSet,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DescriptorSet> const & descriptorSets,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const uint32_t> const &                            dynamicOffsets,
                                                            Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBindDescriptorSets( m_commandBuffer,
                               static_cast<VkPipelineBindPoint>( pipelineBindPoint ),
                               static_cast<VkPipelineLayout>( layout ),
                               firstSet,
                               descriptorSets.size(),
                               reinterpret_cast<const VkDescriptorSet *>( descriptorSets.data() ),
                               dynamicOffsets.size(),
                               dynamicOffsets.data() );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindIndexBuffer( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                         VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                         VULKAN_HPP_NAMESPACE::IndexType  indexType,
                                                         Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindIndexBuffer( m_commandBuffer, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ), static_cast<VkIndexType>( indexType ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers( uint32_t                                 firstBinding,
                                                           uint32_t                                 bindingCount,
                                                           const VULKAN_HPP_NAMESPACE::Buffer *     pBuffers,
                                                           const VULKAN_HPP_NAMESPACE::DeviceSize * pOffsets,
                                                           Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindVertexBuffers(
      m_commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer *>( pBuffers ), reinterpret_cast<const VkDeviceSize *>( pOffsets ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers( uint32_t                                                                         firstBinding,
                                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets,
                                                           Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
#  else
    if ( buffers.size() != offsets.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers: buffers.size() != offsets.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkCmdBindVertexBuffers( m_commandBuffer,
                              firstBinding,
                              buffers.size(),
                              reinterpret_cast<const VkBuffer *>( buffers.data() ),
                              reinterpret_cast<const VkDeviceSize *>( offsets.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::draw(
    uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDraw( m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::drawIndexed( uint32_t         indexCount,
                                                     uint32_t         instanceCount,
                                                     uint32_t         firstIndex,
                                                     int32_t          vertexOffset,
                                                     uint32_t         firstInstance,
                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDrawIndexed( m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::drawIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                      uint32_t                         drawCount,
                                                      uint32_t                         stride,
                                                      Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDrawIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ), drawCount, stride );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::drawIndexedIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                             VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                             uint32_t                         drawCount,
                                                             uint32_t                         stride,
                                                             Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDrawIndexedIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ), drawCount, stride );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::dispatch( uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDispatch( m_commandBuffer, groupCountX, groupCountY, groupCountZ );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::dispatchIndirect( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                          VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                          Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDispatchIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceSize>( offset ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer( VULKAN_HPP_NAMESPACE::Buffer             srcBuffer,
                                                    VULKAN_HPP_NAMESPACE::Buffer             dstBuffer,
                                                    uint32_t                                 regionCount,
                                                    const VULKAN_HPP_NAMESPACE::BufferCopy * pRegions,
                                                    Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBuffer( m_commandBuffer,
                       static_cast<VkBuffer>( srcBuffer ),
                       static_cast<VkBuffer>( dstBuffer ),
                       regionCount,
                       reinterpret_cast<const VkBufferCopy *>( pRegions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer( VULKAN_HPP_NAMESPACE::Buffer                                                     srcBuffer,
                                                    VULKAN_HPP_NAMESPACE::Buffer                                                     dstBuffer,
                                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferCopy> const & regions,
                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBuffer( m_commandBuffer,
                       static_cast<VkBuffer>( srcBuffer ),
                       static_cast<VkBuffer>( dstBuffer ),
                       regions.size(),
                       reinterpret_cast<const VkBufferCopy *>( regions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage( VULKAN_HPP_NAMESPACE::Image             srcImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout       srcImageLayout,
                                                   VULKAN_HPP_NAMESPACE::Image             dstImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout       dstImageLayout,
                                                   uint32_t                                regionCount,
                                                   const VULKAN_HPP_NAMESPACE::ImageCopy * pRegions,
                                                   Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImage( m_commandBuffer,
                      static_cast<VkImage>( srcImage ),
                      static_cast<VkImageLayout>( srcImageLayout ),
                      static_cast<VkImage>( dstImage ),
                      static_cast<VkImageLayout>( dstImageLayout ),
                      regionCount,
                      reinterpret_cast<const VkImageCopy *>( pRegions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                                                   VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                                                   VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageCopy> const & regions,
                                                   Dispatch const &                                                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImage( m_commandBuffer,
                      static_cast<VkImage>( srcImage ),
                      static_cast<VkImageLayout>( srcImageLayout ),
                      static_cast<VkImage>( dstImage ),
                      static_cast<VkImageLayout>( dstImageLayout ),
                      regions.size(),
                      reinterpret_cast<const VkImageCopy *>( regions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage( VULKAN_HPP_NAMESPACE::Image             srcImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout       srcImageLayout,
                                                   VULKAN_HPP_NAMESPACE::Image             dstImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout       dstImageLayout,
                                                   uint32_t                                regionCount,
                                                   const VULKAN_HPP_NAMESPACE::ImageBlit * pRegions,
                                                   VULKAN_HPP_NAMESPACE::Filter            filter,
                                                   Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBlitImage( m_commandBuffer,
                      static_cast<VkImage>( srcImage ),
                      static_cast<VkImageLayout>( srcImageLayout ),
                      static_cast<VkImage>( dstImage ),
                      static_cast<VkImageLayout>( dstImageLayout ),
                      regionCount,
                      reinterpret_cast<const VkImageBlit *>( pRegions ),
                      static_cast<VkFilter>( filter ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage( VULKAN_HPP_NAMESPACE::Image                                                     srcImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout                                               srcImageLayout,
                                                   VULKAN_HPP_NAMESPACE::Image                                                     dstImage,
                                                   VULKAN_HPP_NAMESPACE::ImageLayout                                               dstImageLayout,
                                                   VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageBlit> const & regions,
                                                   VULKAN_HPP_NAMESPACE::Filter                                                    filter,
                                                   Dispatch const &                                                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBlitImage( m_commandBuffer,
                      static_cast<VkImage>( srcImage ),
                      static_cast<VkImageLayout>( srcImageLayout ),
                      static_cast<VkImage>( dstImage ),
                      static_cast<VkImageLayout>( dstImageLayout ),
                      regions.size(),
                      reinterpret_cast<const VkImageBlit *>( regions.data() ),
                      static_cast<VkFilter>( filter ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage( VULKAN_HPP_NAMESPACE::Buffer                  srcBuffer,
                                                           VULKAN_HPP_NAMESPACE::Image                   dstImage,
                                                           VULKAN_HPP_NAMESPACE::ImageLayout             dstImageLayout,
                                                           uint32_t                                      regionCount,
                                                           const VULKAN_HPP_NAMESPACE::BufferImageCopy * pRegions,
                                                           Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBufferToImage( m_commandBuffer,
                              static_cast<VkBuffer>( srcBuffer ),
                              static_cast<VkImage>( dstImage ),
                              static_cast<VkImageLayout>( dstImageLayout ),
                              regionCount,
                              reinterpret_cast<const VkBufferImageCopy *>( pRegions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage( VULKAN_HPP_NAMESPACE::Buffer                                                          srcBuffer,
                                                           VULKAN_HPP_NAMESPACE::Image                                                           dstImage,
                                                           VULKAN_HPP_NAMESPACE::ImageLayout                                                     dstImageLayout,
                                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions,
                                                           Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBufferToImage( m_commandBuffer,
                              static_cast<VkBuffer>( srcBuffer ),
                              static_cast<VkImage>( dstImage ),
                              static_cast<VkImageLayout>( dstImageLayout ),
                              regions.size(),
                              reinterpret_cast<const VkBufferImageCopy *>( regions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer( VULKAN_HPP_NAMESPACE::Image                   srcImage,
                                                           VULKAN_HPP_NAMESPACE::ImageLayout             srcImageLayout,
                                                           VULKAN_HPP_NAMESPACE::Buffer                  dstBuffer,
                                                           uint32_t                                      regionCount,
                                                           const VULKAN_HPP_NAMESPACE::BufferImageCopy * pRegions,
                                                           Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImageToBuffer( m_commandBuffer,
                              static_cast<VkImage>( srcImage ),
                              static_cast<VkImageLayout>( srcImageLayout ),
                              static_cast<VkBuffer>( dstBuffer ),
                              regionCount,
                              reinterpret_cast<const VkBufferImageCopy *>( pRegions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer( VULKAN_HPP_NAMESPACE::Image                                                           srcImage,
                                                           VULKAN_HPP_NAMESPACE::ImageLayout                                                     srcImageLayout,
                                                           VULKAN_HPP_NAMESPACE::Buffer                                                          dstBuffer,
                                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferImageCopy> const & regions,
                                                           Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImageToBuffer( m_commandBuffer,
                              static_cast<VkImage>( srcImage ),
                              static_cast<VkImageLayout>( srcImageLayout ),
                              static_cast<VkBuffer>( dstBuffer ),
                              regions.size(),
                              reinterpret_cast<const VkBufferImageCopy *>( regions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::updateBuffer( VULKAN_HPP_NAMESPACE::Buffer     dstBuffer,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize dstOffset,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize dataSize,
                                                      const void *                     pData,
                                                      Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdUpdateBuffer(
      m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), static_cast<VkDeviceSize>( dstOffset ), static_cast<VkDeviceSize>( dataSize ), pData );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DataType, typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::updateBuffer( VULKAN_HPP_NAMESPACE::Buffer                             dstBuffer,
                                                      VULKAN_HPP_NAMESPACE::DeviceSize                         dstOffset,
                                                      VULKAN_HPP_NAMESPACE::ArrayProxy<const DataType> const & data,
                                                      Dispatch const &                                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdUpdateBuffer( m_commandBuffer,
                         static_cast<VkBuffer>( dstBuffer ),
                         static_cast<VkDeviceSize>( dstOffset ),
                         data.size() * sizeof( DataType ),
                         reinterpret_cast<const void *>( data.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::fillBuffer( VULKAN_HPP_NAMESPACE::Buffer     dstBuffer,
                                                    VULKAN_HPP_NAMESPACE::DeviceSize dstOffset,
                                                    VULKAN_HPP_NAMESPACE::DeviceSize size,
                                                    uint32_t                         data,
                                                    Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdFillBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), static_cast<VkDeviceSize>( dstOffset ), static_cast<VkDeviceSize>( size ), data );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::clearColorImage( VULKAN_HPP_NAMESPACE::Image                         image,
                                                         VULKAN_HPP_NAMESPACE::ImageLayout                   imageLayout,
                                                         const VULKAN_HPP_NAMESPACE::ClearColorValue *       pColor,
                                                         uint32_t                                            rangeCount,
                                                         const VULKAN_HPP_NAMESPACE::ImageSubresourceRange * pRanges,
                                                         Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdClearColorImage( m_commandBuffer,
                            static_cast<VkImage>( image ),
                            static_cast<VkImageLayout>( imageLayout ),
                            reinterpret_cast<const VkClearColorValue *>( pColor ),
                            rangeCount,
                            reinterpret_cast<const VkImageSubresourceRange *>( pRanges ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::clearColorImage( VULKAN_HPP_NAMESPACE::Image                   image,
                                                         VULKAN_HPP_NAMESPACE::ImageLayout             imageLayout,
                                                         const VULKAN_HPP_NAMESPACE::ClearColorValue & color,
                                                         VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges,
                                                         Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdClearColorImage( m_commandBuffer,
                            static_cast<VkImage>( image ),
                            static_cast<VkImageLayout>( imageLayout ),
                            reinterpret_cast<const VkClearColorValue *>( &color ),
                            ranges.size(),
                            reinterpret_cast<const VkImageSubresourceRange *>( ranges.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::clearDepthStencilImage( VULKAN_HPP_NAMESPACE::Image                          image,
                                                                VULKAN_HPP_NAMESPACE::ImageLayout                    imageLayout,
                                                                const VULKAN_HPP_NAMESPACE::ClearDepthStencilValue * pDepthStencil,
                                                                uint32_t                                             rangeCount,
                                                                const VULKAN_HPP_NAMESPACE::ImageSubresourceRange *  pRanges,
                                                                Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdClearDepthStencilImage( m_commandBuffer,
                                   static_cast<VkImage>( image ),
                                   static_cast<VkImageLayout>( imageLayout ),
                                   reinterpret_cast<const VkClearDepthStencilValue *>( pDepthStencil ),
                                   rangeCount,
                                   reinterpret_cast<const VkImageSubresourceRange *>( pRanges ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::clearDepthStencilImage( VULKAN_HPP_NAMESPACE::Image                                                                 image,
                                           VULKAN_HPP_NAMESPACE::ImageLayout                                                           imageLayout,
                                           const VULKAN_HPP_NAMESPACE::ClearDepthStencilValue &                                        depthStencil,
                                           VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageSubresourceRange> const & ranges,
                                           Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdClearDepthStencilImage( m_commandBuffer,
                                   static_cast<VkImage>( image ),
                                   static_cast<VkImageLayout>( imageLayout ),
                                   reinterpret_cast<const VkClearDepthStencilValue *>( &depthStencil ),
                                   ranges.size(),
                                   reinterpret_cast<const VkImageSubresourceRange *>( ranges.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::clearAttachments( uint32_t                                      attachmentCount,
                                                          const VULKAN_HPP_NAMESPACE::ClearAttachment * pAttachments,
                                                          uint32_t                                      rectCount,
                                                          const VULKAN_HPP_NAMESPACE::ClearRect *       pRects,
                                                          Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdClearAttachments( m_commandBuffer,
                             attachmentCount,
                             reinterpret_cast<const VkClearAttachment *>( pAttachments ),
                             rectCount,
                             reinterpret_cast<const VkClearRect *>( pRects ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::clearAttachments( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearAttachment> const & attachments,
                                                          VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ClearRect> const &       rects,
                                                          Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdClearAttachments( m_commandBuffer,
                             attachments.size(),
                             reinterpret_cast<const VkClearAttachment *>( attachments.data() ),
                             rects.size(),
                             reinterpret_cast<const VkClearRect *>( rects.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage( VULKAN_HPP_NAMESPACE::Image                srcImage,
                                                      VULKAN_HPP_NAMESPACE::ImageLayout          srcImageLayout,
                                                      VULKAN_HPP_NAMESPACE::Image                dstImage,
                                                      VULKAN_HPP_NAMESPACE::ImageLayout          dstImageLayout,
                                                      uint32_t                                   regionCount,
                                                      const VULKAN_HPP_NAMESPACE::ImageResolve * pRegions,
                                                      Dispatch const &                           d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResolveImage( m_commandBuffer,
                         static_cast<VkImage>( srcImage ),
                         static_cast<VkImageLayout>( srcImageLayout ),
                         static_cast<VkImage>( dstImage ),
                         static_cast<VkImageLayout>( dstImageLayout ),
                         regionCount,
                         reinterpret_cast<const VkImageResolve *>( pRegions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage( VULKAN_HPP_NAMESPACE::Image                                                        srcImage,
                                                      VULKAN_HPP_NAMESPACE::ImageLayout                                                  srcImageLayout,
                                                      VULKAN_HPP_NAMESPACE::Image                                                        dstImage,
                                                      VULKAN_HPP_NAMESPACE::ImageLayout                                                  dstImageLayout,
                                                      VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageResolve> const & regions,
                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdResolveImage( m_commandBuffer,
                         static_cast<VkImage>( srcImage ),
                         static_cast<VkImageLayout>( srcImageLayout ),
                         static_cast<VkImage>( dstImage ),
                         static_cast<VkImageLayout>( dstImageLayout ),
                         regions.size(),
                         reinterpret_cast<const VkImageResolve *>( regions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setEvent( VULKAN_HPP_NAMESPACE::Event              event,
                                                  VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask,
                                                  Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resetEvent( VULKAN_HPP_NAMESPACE::Event              event,
                                                    VULKAN_HPP_NAMESPACE::PipelineStageFlags stageMask,
                                                    Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::waitEvents( uint32_t                                          eventCount,
                                                    const VULKAN_HPP_NAMESPACE::Event *               pEvents,
                                                    VULKAN_HPP_NAMESPACE::PipelineStageFlags          srcStageMask,
                                                    VULKAN_HPP_NAMESPACE::PipelineStageFlags          dstStageMask,
                                                    uint32_t                                          memoryBarrierCount,
                                                    const VULKAN_HPP_NAMESPACE::MemoryBarrier *       pMemoryBarriers,
                                                    uint32_t                                          bufferMemoryBarrierCount,
                                                    const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier * pBufferMemoryBarriers,
                                                    uint32_t                                          imageMemoryBarrierCount,
                                                    const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier *  pImageMemoryBarriers,
                                                    Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWaitEvents( m_commandBuffer,
                       eventCount,
                       reinterpret_cast<const VkEvent *>( pEvents ),
                       static_cast<VkPipelineStageFlags>( srcStageMask ),
                       static_cast<VkPipelineStageFlags>( dstStageMask ),
                       memoryBarrierCount,
                       reinterpret_cast<const VkMemoryBarrier *>( pMemoryBarriers ),
                       bufferMemoryBarrierCount,
                       reinterpret_cast<const VkBufferMemoryBarrier *>( pBufferMemoryBarriers ),
                       imageMemoryBarrierCount,
                       reinterpret_cast<const VkImageMemoryBarrier *>( pImageMemoryBarriers ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::waitEvents( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &               events,
                               VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
                               VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const &  imageMemoryBarriers,
                               Dispatch const &                                                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdWaitEvents( m_commandBuffer,
                       events.size(),
                       reinterpret_cast<const VkEvent *>( events.data() ),
                       static_cast<VkPipelineStageFlags>( srcStageMask ),
                       static_cast<VkPipelineStageFlags>( dstStageMask ),
                       memoryBarriers.size(),
                       reinterpret_cast<const VkMemoryBarrier *>( memoryBarriers.data() ),
                       bufferMemoryBarriers.size(),
                       reinterpret_cast<const VkBufferMemoryBarrier *>( bufferMemoryBarriers.data() ),
                       imageMemoryBarriers.size(),
                       reinterpret_cast<const VkImageMemoryBarrier *>( imageMemoryBarriers.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier( VULKAN_HPP_NAMESPACE::PipelineStageFlags          srcStageMask,
                                                         VULKAN_HPP_NAMESPACE::PipelineStageFlags          dstStageMask,
                                                         VULKAN_HPP_NAMESPACE::DependencyFlags             dependencyFlags,
                                                         uint32_t                                          memoryBarrierCount,
                                                         const VULKAN_HPP_NAMESPACE::MemoryBarrier *       pMemoryBarriers,
                                                         uint32_t                                          bufferMemoryBarrierCount,
                                                         const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier * pBufferMemoryBarriers,
                                                         uint32_t                                          imageMemoryBarrierCount,
                                                         const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier *  pImageMemoryBarriers,
                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdPipelineBarrier( m_commandBuffer,
                            static_cast<VkPipelineStageFlags>( srcStageMask ),
                            static_cast<VkPipelineStageFlags>( dstStageMask ),
                            static_cast<VkDependencyFlags>( dependencyFlags ),
                            memoryBarrierCount,
                            reinterpret_cast<const VkMemoryBarrier *>( pMemoryBarriers ),
                            bufferMemoryBarrierCount,
                            reinterpret_cast<const VkBufferMemoryBarrier *>( pBufferMemoryBarriers ),
                            imageMemoryBarrierCount,
                            reinterpret_cast<const VkImageMemoryBarrier *>( pImageMemoryBarriers ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::pipelineBarrier( VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  srcStageMask,
                                    VULKAN_HPP_NAMESPACE::PipelineStageFlags                                                  dstStageMask,
                                    VULKAN_HPP_NAMESPACE::DependencyFlags                                                     dependencyFlags,
                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::MemoryBarrier> const &       memoryBarriers,
                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BufferMemoryBarrier> const & bufferMemoryBarriers,
                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::ImageMemoryBarrier> const &  imageMemoryBarriers,
                                    Dispatch const &                                                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdPipelineBarrier( m_commandBuffer,
                            static_cast<VkPipelineStageFlags>( srcStageMask ),
                            static_cast<VkPipelineStageFlags>( dstStageMask ),
                            static_cast<VkDependencyFlags>( dependencyFlags ),
                            memoryBarriers.size(),
                            reinterpret_cast<const VkMemoryBarrier *>( memoryBarriers.data() ),
                            bufferMemoryBarriers.size(),
                            reinterpret_cast<const VkBufferMemoryBarrier *>( bufferMemoryBarriers.data() ),
                            imageMemoryBarriers.size(),
                            reinterpret_cast<const VkImageMemoryBarrier *>( imageMemoryBarriers.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginQuery( VULKAN_HPP_NAMESPACE::QueryPool         queryPool,
                                                    uint32_t                                query,
                                                    VULKAN_HPP_NAMESPACE::QueryControlFlags flags,
                                                    Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBeginQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query, static_cast<VkQueryControlFlags>( flags ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endQuery( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t query, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdEndQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resetQueryPool( VULKAN_HPP_NAMESPACE::QueryPool queryPool,
                                                        uint32_t                        firstQuery,
                                                        uint32_t                        queryCount,
                                                        Dispatch const &                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResetQueryPool( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp( VULKAN_HPP_NAMESPACE::PipelineStageFlagBits pipelineStage,
                                                        VULKAN_HPP_NAMESPACE::QueryPool             queryPool,
                                                        uint32_t                                    query,
                                                        Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWriteTimestamp( m_commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), static_cast<VkQueryPool>( queryPool ), query );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyQueryPoolResults( VULKAN_HPP_NAMESPACE::QueryPool        queryPool,
                                                              uint32_t                               firstQuery,
                                                              uint32_t                               queryCount,
                                                              VULKAN_HPP_NAMESPACE::Buffer           dstBuffer,
                                                              VULKAN_HPP_NAMESPACE::DeviceSize       dstOffset,
                                                              VULKAN_HPP_NAMESPACE::DeviceSize       stride,
                                                              VULKAN_HPP_NAMESPACE::QueryResultFlags flags,
                                                              Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyQueryPoolResults( m_commandBuffer,
                                 static_cast<VkQueryPool>( queryPool ),
                                 firstQuery,
                                 queryCount,
                                 static_cast<VkBuffer>( dstBuffer ),
                                 static_cast<VkDeviceSize>( dstOffset ),
                                 static_cast<VkDeviceSize>( stride ),
                                 static_cast<VkQueryResultFlags>( flags ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pushConstants( VULKAN_HPP_NAMESPACE::PipelineLayout   layout,
                                                       VULKAN_HPP_NAMESPACE::ShaderStageFlags stageFlags,
                                                       uint32_t                               offset,
                                                       uint32_t                               size,
                                                       const void *                           pValues,
                                                       Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdPushConstants( m_commandBuffer, static_cast<VkPipelineLayout>( layout ), static_cast<VkShaderStageFlags>( stageFlags ), offset, size, pValues );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename ValuesType, typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pushConstants( VULKAN_HPP_NAMESPACE::PipelineLayout                       layout,
                                                       VULKAN_HPP_NAMESPACE::ShaderStageFlags                     stageFlags,
                                                       uint32_t                                                   offset,
                                                       VULKAN_HPP_NAMESPACE::ArrayProxy<const ValuesType> const & values,
                                                       Dispatch const &                                           d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdPushConstants( m_commandBuffer,
                          static_cast<VkPipelineLayout>( layout ),
                          static_cast<VkShaderStageFlags>( stageFlags ),
                          offset,
                          values.size() * sizeof( ValuesType ),
                          reinterpret_cast<const void *>( values.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo * pRenderPassBegin,
                                                         VULKAN_HPP_NAMESPACE::SubpassContents             contents,
                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo *>( pRenderPassBegin ), static_cast<VkSubpassContents>( contents ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                                                         VULKAN_HPP_NAMESPACE::SubpassContents             contents,
                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo *>( &renderPassBegin ), static_cast<VkSubpassContents>( contents ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::nextSubpass( VULKAN_HPP_NAMESPACE::SubpassContents contents, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdNextSubpass( m_commandBuffer, static_cast<VkSubpassContents>( contents ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endRenderPass( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdEndRenderPass( m_commandBuffer );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::executeCommands( uint32_t                                    commandBufferCount,
                                                         const VULKAN_HPP_NAMESPACE::CommandBuffer * pCommandBuffers,
                                                         Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdExecuteCommands( m_commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer *>( pCommandBuffers ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::executeCommands( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CommandBuffer> const & commandBuffers,
                                                         Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdExecuteCommands( m_commandBuffer, commandBuffers.size(), reinterpret_cast<const VkCommandBuffer *>( commandBuffers.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_VERSION_1_1 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result enumerateInstanceVersion( uint32_t * pApiVersion, Dispatch const & d ) VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEnumerateInstanceVersion( pApiVersion ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<uint32_t>::type enumerateInstanceVersion( Dispatch const & d )
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint32_t                     apiVersion;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumerateInstanceVersion( &apiVersion ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::enumerateInstanceVersion" );

    return createResultValueType( result, apiVersion );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::bindBufferMemory2( uint32_t                                           bindInfoCount,
                                                                           const VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo * pBindInfos,
                                                                           Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkBindBufferMemory2( m_device, bindInfoCount, reinterpret_cast<const VkBindBufferMemoryInfo *>( pBindInfos ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::bindBufferMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindBufferMemoryInfo> const & bindInfos, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkBindBufferMemory2( m_device, bindInfos.size(), reinterpret_cast<const VkBindBufferMemoryInfo *>( bindInfos.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindBufferMemory2" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::bindImageMemory2( uint32_t                                          bindInfoCount,
                                                                          const VULKAN_HPP_NAMESPACE::BindImageMemoryInfo * pBindInfos,
                                                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkBindImageMemory2( m_device, bindInfoCount, reinterpret_cast<const VkBindImageMemoryInfo *>( pBindInfos ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::bindImageMemory2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::BindImageMemoryInfo> const & bindInfos, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkBindImageMemory2( m_device, bindInfos.size(), reinterpret_cast<const VkBindImageMemoryInfo *>( bindInfos.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::bindImageMemory2" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getGroupPeerMemoryFeatures( uint32_t                                       heapIndex,
                                                             uint32_t                                       localDeviceIndex,
                                                             uint32_t                                       remoteDeviceIndex,
                                                             VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags * pPeerMemoryFeatures,
                                                             Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceGroupPeerMemoryFeatures(
      m_device, heapIndex, localDeviceIndex, remoteDeviceIndex, reinterpret_cast<VkPeerMemoryFeatureFlags *>( pPeerMemoryFeatures ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags Device::getGroupPeerMemoryFeatures(
    uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PeerMemoryFeatureFlags peerMemoryFeatures;
    d.vkGetDeviceGroupPeerMemoryFeatures(
      m_device, heapIndex, localDeviceIndex, remoteDeviceIndex, reinterpret_cast<VkPeerMemoryFeatureFlags *>( &peerMemoryFeatures ) );

    return peerMemoryFeatures;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDeviceMask( uint32_t deviceMask, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDeviceMask( m_commandBuffer, deviceMask );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::dispatchBase( uint32_t         baseGroupX,
                                                      uint32_t         baseGroupY,
                                                      uint32_t         baseGroupZ,
                                                      uint32_t         groupCountX,
                                                      uint32_t         groupCountY,
                                                      uint32_t         groupCountZ,
                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDispatchBase( m_commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    Instance::enumeratePhysicalDeviceGroups( uint32_t *                                            pPhysicalDeviceGroupCount,
                                             VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties,
                                             Dispatch const &                                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkEnumeratePhysicalDeviceGroups(
      m_instance, pPhysicalDeviceGroupCount, reinterpret_cast<VkPhysicalDeviceGroupProperties *>( pPhysicalDeviceGroupProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PhysicalDeviceGroupPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties, PhysicalDeviceGroupPropertiesAllocator>>::type
    Instance::enumeratePhysicalDeviceGroups( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties, PhysicalDeviceGroupPropertiesAllocator> physicalDeviceGroupProperties;
    uint32_t                                                                                                 physicalDeviceGroupCount;
    VULKAN_HPP_NAMESPACE::Result                                                                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceGroups( m_instance, &physicalDeviceGroupCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceGroupCount )
      {
        physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceGroups(
          m_instance, &physicalDeviceGroupCount, reinterpret_cast<VkPhysicalDeviceGroupProperties *>( physicalDeviceGroupProperties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::enumeratePhysicalDeviceGroups" );
    VULKAN_HPP_ASSERT( physicalDeviceGroupCount <= physicalDeviceGroupProperties.size() );
    if ( physicalDeviceGroupCount < physicalDeviceGroupProperties.size() )
    {
      physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
    }
    return createResultValueType( result, physicalDeviceGroupProperties );
  }

  template <typename PhysicalDeviceGroupPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties, PhysicalDeviceGroupPropertiesAllocator>>::type
    Instance::enumeratePhysicalDeviceGroups( PhysicalDeviceGroupPropertiesAllocator & physicalDeviceGroupPropertiesAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceGroupProperties, PhysicalDeviceGroupPropertiesAllocator> physicalDeviceGroupProperties(
      physicalDeviceGroupPropertiesAllocator );
    uint32_t                     physicalDeviceGroupCount;
    VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceGroups( m_instance, &physicalDeviceGroupCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && physicalDeviceGroupCount )
      {
        physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceGroups(
          m_instance, &physicalDeviceGroupCount, reinterpret_cast<VkPhysicalDeviceGroupProperties *>( physicalDeviceGroupProperties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::enumeratePhysicalDeviceGroups" );
    VULKAN_HPP_ASSERT( physicalDeviceGroupCount <= physicalDeviceGroupProperties.size() );
    if ( physicalDeviceGroupCount < physicalDeviceGroupProperties.size() )
    {
      physicalDeviceGroupProperties.resize( physicalDeviceGroupCount );
    }
    return createResultValueType( result, physicalDeviceGroupProperties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 * pInfo,
                                                              VULKAN_HPP_NAMESPACE::MemoryRequirements2 *                pMemoryRequirements,
                                                              Dispatch const &                                           d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetImageMemoryRequirements2(
      m_device, reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>( pInfo ), reinterpret_cast<VkMemoryRequirements2 *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
    Device::getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
    d.vkGetImageMemoryRequirements2(
      m_device, reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return memoryRequirements;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
    Device::getImageMemoryRequirements2( const VULKAN_HPP_NAMESPACE::ImageMemoryRequirementsInfo2 & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
    d.vkGetImageMemoryRequirements2(
      m_device, reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 * pInfo,
                                                               VULKAN_HPP_NAMESPACE::MemoryRequirements2 *                 pMemoryRequirements,
                                                               Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetBufferMemoryRequirements2(
      m_device, reinterpret_cast<const VkBufferMemoryRequirementsInfo2 *>( pInfo ), reinterpret_cast<VkMemoryRequirements2 *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
    Device::getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
    d.vkGetBufferMemoryRequirements2(
      m_device, reinterpret_cast<const VkBufferMemoryRequirementsInfo2 *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return memoryRequirements;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
    Device::getBufferMemoryRequirements2( const VULKAN_HPP_NAMESPACE::BufferMemoryRequirementsInfo2 & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
    d.vkGetBufferMemoryRequirements2(
      m_device, reinterpret_cast<const VkBufferMemoryRequirementsInfo2 *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getFeatures2( VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 * pFeatures, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceFeatures2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures2 *>( pFeatures ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2
                                         PhysicalDevice::getFeatures2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 features;
    d.vkGetPhysicalDeviceFeatures2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures2 *>( &features ) );

    return features;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                         PhysicalDevice::getFeatures2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2 &  features = structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures2>();
    d.vkGetPhysicalDeviceFeatures2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures2 *>( &features ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getProperties2( VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 * pProperties,
                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties2 *>( pProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2
                                         PhysicalDevice::getProperties2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 properties;
    d.vkGetPhysicalDeviceProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties2 *>( &properties ) );

    return properties;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                         PhysicalDevice::getProperties2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>  structureChain;
    VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2 & properties = structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceProperties2>();
    d.vkGetPhysicalDeviceProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties2 *>( &properties ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getFormatProperties2( VULKAN_HPP_NAMESPACE::Format              format,
                                                               VULKAN_HPP_NAMESPACE::FormatProperties2 * pFormatProperties,
                                                               Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceFormatProperties2( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2 *>( pFormatProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::FormatProperties2
    PhysicalDevice::getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::FormatProperties2 formatProperties;
    d.vkGetPhysicalDeviceFormatProperties2( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2 *>( &formatProperties ) );

    return formatProperties;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
    PhysicalDevice::getFormatProperties2( VULKAN_HPP_NAMESPACE::Format format, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::FormatProperties2 &        formatProperties = structureChain.template get<VULKAN_HPP_NAMESPACE::FormatProperties2>();
    d.vkGetPhysicalDeviceFormatProperties2( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2 *>( &formatProperties ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 * pImageFormatInfo,
                                               VULKAN_HPP_NAMESPACE::ImageFormatProperties2 *               pImageFormatProperties,
                                               Dispatch const &                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceImageFormatProperties2( m_physicalDevice,
                                                                             reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2 *>( pImageFormatInfo ),
                                                                             reinterpret_cast<VkImageFormatProperties2 *>( pImageFormatProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>::type
    PhysicalDevice::getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ImageFormatProperties2 imageFormatProperties;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceImageFormatProperties2( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2 *>( &imageFormatInfo ),
                                                   reinterpret_cast<VkImageFormatProperties2 *>( &imageFormatProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties2" );

    return createResultValueType( result, imageFormatProperties );
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>>::type
    PhysicalDevice::getImageFormatProperties2( const VULKAN_HPP_NAMESPACE::PhysicalDeviceImageFormatInfo2 & imageFormatInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    StructureChain<X, Y, Z...>                     structureChain;
    VULKAN_HPP_NAMESPACE::ImageFormatProperties2 & imageFormatProperties = structureChain.template get<VULKAN_HPP_NAMESPACE::ImageFormatProperties2>();
    VULKAN_HPP_NAMESPACE::Result                   result                = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceImageFormatProperties2( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2 *>( &imageFormatInfo ),
                                                   reinterpret_cast<VkImageFormatProperties2 *>( &imageFormatProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getImageFormatProperties2" );

    return createResultValueType( result, structureChain );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getQueueFamilyProperties2( uint32_t *                                     pQueueFamilyPropertyCount,
                                                                    VULKAN_HPP_NAMESPACE::QueueFamilyProperties2 * pQueueFamilyProperties,
                                                                    Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceQueueFamilyProperties2(
      m_physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2 *>( pQueueFamilyProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename QueueFamilyProperties2Allocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2, QueueFamilyProperties2Allocator>
                                         PhysicalDevice::getQueueFamilyProperties2( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2, QueueFamilyProperties2Allocator> queueFamilyProperties;
    uint32_t                                                                                   queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties2( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    d.vkGetPhysicalDeviceQueueFamilyProperties2(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      queueFamilyProperties.resize( queueFamilyPropertyCount );
    }
    return queueFamilyProperties;
  }

  template <typename QueueFamilyProperties2Allocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2, QueueFamilyProperties2Allocator>
    PhysicalDevice::getQueueFamilyProperties2( QueueFamilyProperties2Allocator & queueFamilyProperties2Allocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2, QueueFamilyProperties2Allocator> queueFamilyProperties( queueFamilyProperties2Allocator );
    uint32_t                                                                                   queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties2( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    d.vkGetPhysicalDeviceQueueFamilyProperties2(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      queueFamilyProperties.resize( queueFamilyPropertyCount );
    }
    return queueFamilyProperties;
  }

  template <typename StructureChain, typename StructureChainAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain, StructureChainAllocator>
                                         PhysicalDevice::getQueueFamilyProperties2( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<StructureChain, StructureChainAllocator>      structureChains;
    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> queueFamilyProperties;
    uint32_t                                                  queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties2( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    structureChains.resize( queueFamilyPropertyCount );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
    {
      queueFamilyProperties[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>().pNext;
    }
    d.vkGetPhysicalDeviceQueueFamilyProperties2(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      structureChains.resize( queueFamilyPropertyCount );
    }
    for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
    {
      structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>() = queueFamilyProperties[i];
    }
    return structureChains;
  }

  template <typename StructureChain,
            typename StructureChainAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, StructureChain>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain, StructureChainAllocator>
    PhysicalDevice::getQueueFamilyProperties2( StructureChainAllocator & structureChainAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<StructureChain, StructureChainAllocator>      structureChains( structureChainAllocator );
    std::vector<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2> queueFamilyProperties;
    uint32_t                                                  queueFamilyPropertyCount;
    d.vkGetPhysicalDeviceQueueFamilyProperties2( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
    structureChains.resize( queueFamilyPropertyCount );
    queueFamilyProperties.resize( queueFamilyPropertyCount );
    for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
    {
      queueFamilyProperties[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>().pNext;
    }
    d.vkGetPhysicalDeviceQueueFamilyProperties2(
      m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2 *>( queueFamilyProperties.data() ) );

    VULKAN_HPP_ASSERT( queueFamilyPropertyCount <= queueFamilyProperties.size() );
    if ( queueFamilyPropertyCount < queueFamilyProperties.size() )
    {
      structureChains.resize( queueFamilyPropertyCount );
    }
    for ( uint32_t i = 0; i < queueFamilyPropertyCount; i++ )
    {
      structureChains[i].template get<VULKAN_HPP_NAMESPACE::QueueFamilyProperties2>() = queueFamilyProperties[i];
    }
    return structureChains;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getMemoryProperties2( VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 * pMemoryProperties,
                                                               Dispatch const &                                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceMemoryProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties2 *>( pMemoryProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2
                                         PhysicalDevice::getMemoryProperties2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 memoryProperties;
    d.vkGetPhysicalDeviceMemoryProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties2 *>( &memoryProperties ) );

    return memoryProperties;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                         PhysicalDevice::getMemoryProperties2( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>        structureChain;
    VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2 & memoryProperties =
      structureChain.template get<VULKAN_HPP_NAMESPACE::PhysicalDeviceMemoryProperties2>();
    d.vkGetPhysicalDeviceMemoryProperties2( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties2 *>( &memoryProperties ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getQueue2( const VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 * pQueueInfo,
                                            VULKAN_HPP_NAMESPACE::Queue *                  pQueue,
                                            Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceQueue2( m_device, reinterpret_cast<const VkDeviceQueueInfo2 *>( pQueueInfo ), reinterpret_cast<VkQueue *>( pQueue ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Queue Device::getQueue2( const VULKAN_HPP_NAMESPACE::DeviceQueueInfo2 & queueInfo,
                                                                                        Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Queue queue;
    d.vkGetDeviceQueue2( m_device, reinterpret_cast<const VkDeviceQueueInfo2 *>( &queueInfo ), reinterpret_cast<VkQueue *>( &queue ) );

    return queue;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    Device::createSamplerYcbcrConversion( const VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo * pCreateInfo,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks *              pAllocator,
                                          VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion *                 pYcbcrConversion,
                                          Dispatch const &                                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSamplerYcbcrConversion( m_device,
                                                                  reinterpret_cast<const VkSamplerYcbcrConversionCreateInfo *>( pCreateInfo ),
                                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                                  reinterpret_cast<VkSamplerYcbcrConversion *>( pYcbcrConversion ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion>::type
                       Device::createSamplerYcbcrConversion( const VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo & createInfo,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                          Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion ycbcrConversion;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSamplerYcbcrConversion(
      m_device,
      reinterpret_cast<const VkSamplerYcbcrConversionCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSamplerYcbcrConversion *>( &ycbcrConversion ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSamplerYcbcrConversion" );

    return createResultValueType( result, ycbcrConversion );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion, Dispatch>>::type
                       Device::createSamplerYcbcrConversionUnique( const VULKAN_HPP_NAMESPACE::SamplerYcbcrConversionCreateInfo & createInfo,
                                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                                Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion ycbcrConversion;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSamplerYcbcrConversion(
      m_device,
      reinterpret_cast<const VkSamplerYcbcrConversionCreateInfo *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSamplerYcbcrConversion *>( &ycbcrConversion ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSamplerYcbcrConversionUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion, Dispatch>( ycbcrConversion, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion      ycbcrConversion,
                                                                const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySamplerYcbcrConversion(
      m_device, static_cast<VkSamplerYcbcrConversion>( ycbcrConversion ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroySamplerYcbcrConversion( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion              ycbcrConversion,
                                                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                                Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySamplerYcbcrConversion(
      m_device,
      static_cast<VkSamplerYcbcrConversion>( ycbcrConversion ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion      ycbcrConversion,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySamplerYcbcrConversion(
      m_device, static_cast<VkSamplerYcbcrConversion>( ycbcrConversion ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::SamplerYcbcrConversion              ycbcrConversion,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySamplerYcbcrConversion(
      m_device,
      static_cast<VkSamplerYcbcrConversion>( ycbcrConversion ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getExternalBufferProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
                                                                      VULKAN_HPP_NAMESPACE::ExternalBufferProperties *               pExternalBufferProperties,
                                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceExternalBufferProperties( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceExternalBufferInfo *>( pExternalBufferInfo ),
                                                   reinterpret_cast<VkExternalBufferProperties *>( pExternalBufferProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalBufferProperties
    PhysicalDevice::getExternalBufferProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalBufferInfo & externalBufferInfo,
                                                 Dispatch const &                                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ExternalBufferProperties externalBufferProperties;
    d.vkGetPhysicalDeviceExternalBufferProperties( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceExternalBufferInfo *>( &externalBufferInfo ),
                                                   reinterpret_cast<VkExternalBufferProperties *>( &externalBufferProperties ) );

    return externalBufferProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getExternalFenceProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
                                                                     VULKAN_HPP_NAMESPACE::ExternalFenceProperties *               pExternalFenceProperties,
                                                                     Dispatch const &                                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceExternalFenceProperties( m_physicalDevice,
                                                  reinterpret_cast<const VkPhysicalDeviceExternalFenceInfo *>( pExternalFenceInfo ),
                                                  reinterpret_cast<VkExternalFenceProperties *>( pExternalFenceProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalFenceProperties
    PhysicalDevice::getExternalFenceProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalFenceInfo & externalFenceInfo,
                                                Dispatch const &                                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ExternalFenceProperties externalFenceProperties;
    d.vkGetPhysicalDeviceExternalFenceProperties( m_physicalDevice,
                                                  reinterpret_cast<const VkPhysicalDeviceExternalFenceInfo *>( &externalFenceInfo ),
                                                  reinterpret_cast<VkExternalFenceProperties *>( &externalFenceProperties ) );

    return externalFenceProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    PhysicalDevice::getExternalSemaphoreProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
                                                    VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties *               pExternalSemaphoreProperties,
                                                    Dispatch const &                                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceExternalSemaphoreProperties( m_physicalDevice,
                                                      reinterpret_cast<const VkPhysicalDeviceExternalSemaphoreInfo *>( pExternalSemaphoreInfo ),
                                                      reinterpret_cast<VkExternalSemaphoreProperties *>( pExternalSemaphoreProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties
    PhysicalDevice::getExternalSemaphoreProperties( const VULKAN_HPP_NAMESPACE::PhysicalDeviceExternalSemaphoreInfo & externalSemaphoreInfo,
                                                    Dispatch const &                                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ExternalSemaphoreProperties externalSemaphoreProperties;
    d.vkGetPhysicalDeviceExternalSemaphoreProperties( m_physicalDevice,
                                                      reinterpret_cast<const VkPhysicalDeviceExternalSemaphoreInfo *>( &externalSemaphoreInfo ),
                                                      reinterpret_cast<VkExternalSemaphoreProperties *>( &externalSemaphoreProperties ) );

    return externalSemaphoreProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo * pCreateInfo,
                                                                VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport *          pSupport,
                                                                Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDescriptorSetLayoutSupport(
      m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( pCreateInfo ), reinterpret_cast<VkDescriptorSetLayoutSupport *>( pSupport ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport
                                         Device::getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo,
                                           Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport support;
    d.vkGetDescriptorSetLayoutSupport(
      m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ), reinterpret_cast<VkDescriptorSetLayoutSupport *>( &support ) );

    return support;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
                                         Device::getDescriptorSetLayoutSupport( const VULKAN_HPP_NAMESPACE::DescriptorSetLayoutCreateInfo & createInfo,
                                           Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>   structureChain;
    VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport & support = structureChain.template get<VULKAN_HPP_NAMESPACE::DescriptorSetLayoutSupport>();
    d.vkGetDescriptorSetLayoutSupport(
      m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo *>( &createInfo ), reinterpret_cast<VkDescriptorSetLayoutSupport *>( &support ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_VERSION_1_2 ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::drawIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                           VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                           VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                                                           VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                                                           uint32_t                         maxDrawCount,
                                                           uint32_t                         stride,
                                                           Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDrawIndirectCount( m_commandBuffer,
                              static_cast<VkBuffer>( buffer ),
                              static_cast<VkDeviceSize>( offset ),
                              static_cast<VkBuffer>( countBuffer ),
                              static_cast<VkDeviceSize>( countBufferOffset ),
                              maxDrawCount,
                              stride );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::drawIndexedIndirectCount( VULKAN_HPP_NAMESPACE::Buffer     buffer,
                                                                  VULKAN_HPP_NAMESPACE::DeviceSize offset,
                                                                  VULKAN_HPP_NAMESPACE::Buffer     countBuffer,
                                                                  VULKAN_HPP_NAMESPACE::DeviceSize countBufferOffset,
                                                                  uint32_t                         maxDrawCount,
                                                                  uint32_t                         stride,
                                                                  Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdDrawIndexedIndirectCount( m_commandBuffer,
                                     static_cast<VkBuffer>( buffer ),
                                     static_cast<VkDeviceSize>( offset ),
                                     static_cast<VkBuffer>( countBuffer ),
                                     static_cast<VkDeviceSize>( countBufferOffset ),
                                     maxDrawCount,
                                     stride );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 * pCreateInfo,
                                                                           const VULKAN_HPP_NAMESPACE::AllocationCallbacks *   pAllocator,
                                                                           VULKAN_HPP_NAMESPACE::RenderPass *                  pRenderPass,
                                                                           Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateRenderPass2( m_device,
                                                       reinterpret_cast<const VkRenderPassCreateInfo2 *>( pCreateInfo ),
                                                       reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                       reinterpret_cast<VkRenderPass *>( pRenderPass ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::RenderPass>::type
                       Device::createRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 &       createInfo,
                               Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                               Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::RenderPass renderPass;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateRenderPass2( m_device,
                             reinterpret_cast<const VkRenderPassCreateInfo2 *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createRenderPass2" );

    return createResultValueType( result, renderPass );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::RenderPass, Dispatch>>::type
                       Device::createRenderPass2Unique( const VULKAN_HPP_NAMESPACE::RenderPassCreateInfo2 &       createInfo,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                     Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::RenderPass renderPass;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateRenderPass2( m_device,
                             reinterpret_cast<const VkRenderPassCreateInfo2 *>( &createInfo ),
                             reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                             reinterpret_cast<VkRenderPass *>( &renderPass ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createRenderPass2Unique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::RenderPass, Dispatch>( renderPass, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo * pRenderPassBegin,
                                                          const VULKAN_HPP_NAMESPACE::SubpassBeginInfo *    pSubpassBeginInfo,
                                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBeginRenderPass2(
      m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo *>( pRenderPassBegin ), reinterpret_cast<const VkSubpassBeginInfo *>( pSubpassBeginInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRenderPass2( const VULKAN_HPP_NAMESPACE::RenderPassBeginInfo & renderPassBegin,
                                                          const VULKAN_HPP_NAMESPACE::SubpassBeginInfo &    subpassBeginInfo,
                                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBeginRenderPass2(
      m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo *>( &renderPassBegin ), reinterpret_cast<const VkSubpassBeginInfo *>( &subpassBeginInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::nextSubpass2( const VULKAN_HPP_NAMESPACE::SubpassBeginInfo * pSubpassBeginInfo,
                                                      const VULKAN_HPP_NAMESPACE::SubpassEndInfo *   pSubpassEndInfo,
                                                      Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdNextSubpass2(
      m_commandBuffer, reinterpret_cast<const VkSubpassBeginInfo *>( pSubpassBeginInfo ), reinterpret_cast<const VkSubpassEndInfo *>( pSubpassEndInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::nextSubpass2( const VULKAN_HPP_NAMESPACE::SubpassBeginInfo & subpassBeginInfo,
                                                      const VULKAN_HPP_NAMESPACE::SubpassEndInfo &   subpassEndInfo,
                                                      Dispatch const &                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdNextSubpass2(
      m_commandBuffer, reinterpret_cast<const VkSubpassBeginInfo *>( &subpassBeginInfo ), reinterpret_cast<const VkSubpassEndInfo *>( &subpassEndInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endRenderPass2( const VULKAN_HPP_NAMESPACE::SubpassEndInfo * pSubpassEndInfo,
                                                        Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdEndRenderPass2( m_commandBuffer, reinterpret_cast<const VkSubpassEndInfo *>( pSubpassEndInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endRenderPass2( const VULKAN_HPP_NAMESPACE::SubpassEndInfo & subpassEndInfo,
                                                        Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdEndRenderPass2( m_commandBuffer, reinterpret_cast<const VkSubpassEndInfo *>( &subpassEndInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    Device::resetQueryPool( VULKAN_HPP_NAMESPACE::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkResetQueryPool( m_device, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSemaphoreCounterValue( VULKAN_HPP_NAMESPACE::Semaphore semaphore,
                                                                                  uint64_t *                      pValue,
                                                                                  Dispatch const &                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetSemaphoreCounterValue( m_device, static_cast<VkSemaphore>( semaphore ), pValue ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<uint64_t>::type Device::getSemaphoreCounterValue( VULKAN_HPP_NAMESPACE::Semaphore semaphore,
                                                                                                                    Dispatch const &                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint64_t                     value;
    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetSemaphoreCounterValue( m_device, static_cast<VkSemaphore>( semaphore ), &value ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSemaphoreCounterValue" );

    return createResultValueType( result, value );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::waitSemaphores( const VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo * pWaitInfo,
                                                                        uint64_t                                        timeout,
                                                                        Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkWaitSemaphores( m_device, reinterpret_cast<const VkSemaphoreWaitInfo *>( pWaitInfo ), timeout ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result
    Device::waitSemaphores( const VULKAN_HPP_NAMESPACE::SemaphoreWaitInfo & waitInfo, uint64_t timeout, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkWaitSemaphores( m_device, reinterpret_cast<const VkSemaphoreWaitInfo *>( &waitInfo ), timeout ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Device::waitSemaphores", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eTimeout } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::signalSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo * pSignalInfo,
                                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkSignalSemaphore( m_device, reinterpret_cast<const VkSemaphoreSignalInfo *>( pSignalInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
                                          Device::signalSemaphore( const VULKAN_HPP_NAMESPACE::SemaphoreSignalInfo & signalInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkSignalSemaphore( m_device, reinterpret_cast<const VkSemaphoreSignalInfo *>( &signalInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::signalSemaphore" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE DeviceAddress Device::getBufferAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo * pInfo,
                                                            Dispatch const &                                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<DeviceAddress>( d.vkGetBufferDeviceAddress( m_device, reinterpret_cast<const VkBufferDeviceAddressInfo *>( pInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DeviceAddress Device::getBufferAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info,
                                                                                  Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VkDeviceAddress result = d.vkGetBufferDeviceAddress( m_device, reinterpret_cast<const VkBufferDeviceAddressInfo *>( &info ) );

    return static_cast<VULKAN_HPP_NAMESPACE::DeviceAddress>( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE uint64_t Device::getBufferOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo * pInfo,
                                                                    Dispatch const &                                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return d.vkGetBufferOpaqueCaptureAddress( m_device, reinterpret_cast<const VkBufferDeviceAddressInfo *>( pInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE uint64_t Device::getBufferOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::BufferDeviceAddressInfo & info,
                                                                    Dispatch const &                                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint64_t result = d.vkGetBufferOpaqueCaptureAddress( m_device, reinterpret_cast<const VkBufferDeviceAddressInfo *>( &info ) );

    return result;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE uint64_t Device::getMemoryOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo * pInfo,
                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return d.vkGetDeviceMemoryOpaqueCaptureAddress( m_device, reinterpret_cast<const VkDeviceMemoryOpaqueCaptureAddressInfo *>( pInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE uint64_t Device::getMemoryOpaqueCaptureAddress( const VULKAN_HPP_NAMESPACE::DeviceMemoryOpaqueCaptureAddressInfo & info,
                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint64_t result = d.vkGetDeviceMemoryOpaqueCaptureAddress( m_device, reinterpret_cast<const VkDeviceMemoryOpaqueCaptureAddressInfo *>( &info ) );

    return result;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_VERSION_1_3 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getToolProperties( uint32_t *                                           pToolCount,
                                                                                   VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties * pToolProperties,
                                                                                   Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceToolProperties( m_physicalDevice, pToolCount, reinterpret_cast<VkPhysicalDeviceToolProperties *>( pToolProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PhysicalDeviceToolPropertiesAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties, PhysicalDeviceToolPropertiesAllocator>>::type
    PhysicalDevice::getToolProperties( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties, PhysicalDeviceToolPropertiesAllocator> toolProperties;
    uint32_t                                                                                               toolCount;
    VULKAN_HPP_NAMESPACE::Result                                                                           result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceToolProperties( m_physicalDevice, &toolCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && toolCount )
      {
        toolProperties.resize( toolCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceToolProperties( m_physicalDevice, &toolCount, reinterpret_cast<VkPhysicalDeviceToolProperties *>( toolProperties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getToolProperties" );
    VULKAN_HPP_ASSERT( toolCount <= toolProperties.size() );
    if ( toolCount < toolProperties.size() )
    {
      toolProperties.resize( toolCount );
    }
    return createResultValueType( result, toolProperties );
  }

  template <typename PhysicalDeviceToolPropertiesAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties, PhysicalDeviceToolPropertiesAllocator>>::type
    PhysicalDevice::getToolProperties( PhysicalDeviceToolPropertiesAllocator & physicalDeviceToolPropertiesAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceToolProperties, PhysicalDeviceToolPropertiesAllocator> toolProperties(
      physicalDeviceToolPropertiesAllocator );
    uint32_t                     toolCount;
    VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceToolProperties( m_physicalDevice, &toolCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && toolCount )
      {
        toolProperties.resize( toolCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceToolProperties( m_physicalDevice, &toolCount, reinterpret_cast<VkPhysicalDeviceToolProperties *>( toolProperties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getToolProperties" );
    VULKAN_HPP_ASSERT( toolCount <= toolProperties.size() );
    if ( toolCount < toolProperties.size() )
    {
      toolProperties.resize( toolCount );
    }
    return createResultValueType( result, toolProperties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createPrivateDataSlot( const VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo * pCreateInfo,
                                                                               const VULKAN_HPP_NAMESPACE::AllocationCallbacks *       pAllocator,
                                                                               VULKAN_HPP_NAMESPACE::PrivateDataSlot *                 pPrivateDataSlot,
                                                                               Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreatePrivateDataSlot( m_device,
                                                           reinterpret_cast<const VkPrivateDataSlotCreateInfo *>( pCreateInfo ),
                                                           reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                           reinterpret_cast<VkPrivateDataSlot *>( pPrivateDataSlot ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::PrivateDataSlot>::type
                       Device::createPrivateDataSlot( const VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo &   createInfo,
                                   Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                   Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot;
    VULKAN_HPP_NAMESPACE::Result          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePrivateDataSlot( m_device,
                                 reinterpret_cast<const VkPrivateDataSlotCreateInfo *>( &createInfo ),
                                 reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                 reinterpret_cast<VkPrivateDataSlot *>( &privateDataSlot ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPrivateDataSlot" );

    return createResultValueType( result, privateDataSlot );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::PrivateDataSlot, Dispatch>>::type
                       Device::createPrivateDataSlotUnique( const VULKAN_HPP_NAMESPACE::PrivateDataSlotCreateInfo &   createInfo,
                                         Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                         Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot;
    VULKAN_HPP_NAMESPACE::Result          result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreatePrivateDataSlot( m_device,
                                 reinterpret_cast<const VkPrivateDataSlotCreateInfo *>( &createInfo ),
                                 reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                 reinterpret_cast<VkPrivateDataSlot *>( &privateDataSlot ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createPrivateDataSlotUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::PrivateDataSlot, Dispatch>( privateDataSlot, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPrivateDataSlot( VULKAN_HPP_NAMESPACE::PrivateDataSlot             privateDataSlot,
                                                         const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                         Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPrivateDataSlot( m_device, static_cast<VkPrivateDataSlot>( privateDataSlot ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroyPrivateDataSlot( VULKAN_HPP_NAMESPACE::PrivateDataSlot                     privateDataSlot,
                                                         Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                         Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPrivateDataSlot(
      m_device,
      static_cast<VkPrivateDataSlot>( privateDataSlot ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PrivateDataSlot             privateDataSlot,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyPrivateDataSlot( m_device, static_cast<VkPrivateDataSlot>( privateDataSlot ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::destroy( VULKAN_HPP_NAMESPACE::PrivateDataSlot                     privateDataSlot,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyPrivateDataSlot(
      m_device,
      static_cast<VkPrivateDataSlot>( privateDataSlot ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::setPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                                        uint64_t                              objectHandle,
                                                                        VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                                                                        uint64_t                              data,
                                                                        Dispatch const &                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkSetPrivateData( m_device, static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), data ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::setPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                                                 uint64_t                              objectHandle,
                                                                                 VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                                                                                 uint64_t                              data,
                                                                                 Dispatch const &                      d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkSetPrivateData( m_device, static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), data ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setPrivateData" );

    return createResultValueType( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                 uint64_t                              objectHandle,
                                                 VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                                                 uint64_t *                            pData,
                                                 Dispatch const &                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPrivateData( m_device, static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), pData );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint64_t Device::getPrivateData( VULKAN_HPP_NAMESPACE::ObjectType      objectType_,
                                                                          uint64_t                              objectHandle,
                                                                          VULKAN_HPP_NAMESPACE::PrivateDataSlot privateDataSlot,
                                                                          Dispatch const &                      d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint64_t data;
    d.vkGetPrivateData( m_device, static_cast<VkObjectType>( objectType_ ), objectHandle, static_cast<VkPrivateDataSlot>( privateDataSlot ), &data );

    return data;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setEvent2( VULKAN_HPP_NAMESPACE::Event                  event,
                                                   const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfo,
                                                   Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetEvent2( m_commandBuffer, static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( pDependencyInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setEvent2( VULKAN_HPP_NAMESPACE::Event                  event,
                                                   const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo,
                                                   Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetEvent2( m_commandBuffer, static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resetEvent2( VULKAN_HPP_NAMESPACE::Event               event,
                                                     VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask,
                                                     Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResetEvent2( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags2>( stageMask ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::waitEvents2( uint32_t                                     eventCount,
                                                     const VULKAN_HPP_NAMESPACE::Event *          pEvents,
                                                     const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfos,
                                                     Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWaitEvents2(
      m_commandBuffer, eventCount, reinterpret_cast<const VkEvent *>( pEvents ), reinterpret_cast<const VkDependencyInfo *>( pDependencyInfos ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::waitEvents2( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                                                     VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos,
                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( events.size() == dependencyInfos.size() );
#  else
    if ( events.size() != dependencyInfos.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::waitEvents2: events.size() != dependencyInfos.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkCmdWaitEvents2( m_commandBuffer,
                        events.size(),
                        reinterpret_cast<const VkEvent *>( events.data() ),
                        reinterpret_cast<const VkDependencyInfo *>( dependencyInfos.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2( const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfo,
                                                          Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdPipelineBarrier2( m_commandBuffer, reinterpret_cast<const VkDependencyInfo *>( pDependencyInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo,
                                                          Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdPipelineBarrier2( m_commandBuffer, reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp2( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                         VULKAN_HPP_NAMESPACE::QueryPool           queryPool,
                                                         uint32_t                                  query,
                                                         Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWriteTimestamp2( m_commandBuffer, static_cast<VkPipelineStageFlags2>( stage ), static_cast<VkQueryPool>( queryPool ), query );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Queue::submit2( uint32_t                                  submitCount,
                                                                const VULKAN_HPP_NAMESPACE::SubmitInfo2 * pSubmits,
                                                                VULKAN_HPP_NAMESPACE::Fence               fence,
                                                                Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkQueueSubmit2( m_queue, submitCount, reinterpret_cast<const VkSubmitInfo2 *>( pSubmits ), static_cast<VkFence>( fence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Queue::submit2(
    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits, VULKAN_HPP_NAMESPACE::Fence fence, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkQueueSubmit2( m_queue, submits.size(), reinterpret_cast<const VkSubmitInfo2 *>( submits.data() ), static_cast<VkFence>( fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit2" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 * pCopyBufferInfo,
                                                     Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBuffer2( m_commandBuffer, reinterpret_cast<const VkCopyBufferInfo2 *>( pCopyBufferInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo,
                                                     Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBuffer2( m_commandBuffer, reinterpret_cast<const VkCopyBufferInfo2 *>( &copyBufferInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage2( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 * pCopyImageInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImage2( m_commandBuffer, reinterpret_cast<const VkCopyImageInfo2 *>( pCopyImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage2( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImage2( m_commandBuffer, reinterpret_cast<const VkCopyImageInfo2 *>( &copyImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage2( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 * pCopyBufferToImageInfo,
                                                            Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBufferToImage2( m_commandBuffer, reinterpret_cast<const VkCopyBufferToImageInfo2 *>( pCopyBufferToImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage2( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo,
                                                            Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBufferToImage2( m_commandBuffer, reinterpret_cast<const VkCopyBufferToImageInfo2 *>( &copyBufferToImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer2( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 * pCopyImageToBufferInfo,
                                                            Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImageToBuffer2( m_commandBuffer, reinterpret_cast<const VkCopyImageToBufferInfo2 *>( pCopyImageToBufferInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer2( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo,
                                                            Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImageToBuffer2( m_commandBuffer, reinterpret_cast<const VkCopyImageToBufferInfo2 *>( &copyImageToBufferInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage2( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 * pBlitImageInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBlitImage2( m_commandBuffer, reinterpret_cast<const VkBlitImageInfo2 *>( pBlitImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage2( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBlitImage2( m_commandBuffer, reinterpret_cast<const VkBlitImageInfo2 *>( &blitImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage2( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 * pResolveImageInfo,
                                                       Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResolveImage2( m_commandBuffer, reinterpret_cast<const VkResolveImageInfo2 *>( pResolveImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage2( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo,
                                                       Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdResolveImage2( m_commandBuffer, reinterpret_cast<const VkResolveImageInfo2 *>( &resolveImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRendering( const VULKAN_HPP_NAMESPACE::RenderingInfo * pRenderingInfo,
                                                        Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBeginRendering( m_commandBuffer, reinterpret_cast<const VkRenderingInfo *>( pRenderingInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginRendering( const VULKAN_HPP_NAMESPACE::RenderingInfo & renderingInfo,
                                                        Dispatch const &                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBeginRendering( m_commandBuffer, reinterpret_cast<const VkRenderingInfo *>( &renderingInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endRendering( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdEndRendering( m_commandBuffer );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setCullMode( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetCullMode( m_commandBuffer, static_cast<VkCullModeFlags>( cullMode ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setFrontFace( VULKAN_HPP_NAMESPACE::FrontFace frontFace, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetFrontFace( m_commandBuffer, static_cast<VkFrontFace>( frontFace ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveTopology( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology,
                                                              Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetPrimitiveTopology( m_commandBuffer, static_cast<VkPrimitiveTopology>( primitiveTopology ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewportWithCount( uint32_t                               viewportCount,
                                                              const VULKAN_HPP_NAMESPACE::Viewport * pViewports,
                                                              Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetViewportWithCount( m_commandBuffer, viewportCount, reinterpret_cast<const VkViewport *>( pViewports ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewportWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports,
                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetViewportWithCount( m_commandBuffer, viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setScissorWithCount( uint32_t scissorCount, const VULKAN_HPP_NAMESPACE::Rect2D * pScissors, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetScissorWithCount( m_commandBuffer, scissorCount, reinterpret_cast<const VkRect2D *>( pScissors ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setScissorWithCount( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors,
                                                             Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetScissorWithCount( m_commandBuffer, scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers2( uint32_t                                 firstBinding,
                                                            uint32_t                                 bindingCount,
                                                            const VULKAN_HPP_NAMESPACE::Buffer *     pBuffers,
                                                            const VULKAN_HPP_NAMESPACE::DeviceSize * pOffsets,
                                                            const VULKAN_HPP_NAMESPACE::DeviceSize * pSizes,
                                                            const VULKAN_HPP_NAMESPACE::DeviceSize * pStrides,
                                                            Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindVertexBuffers2( m_commandBuffer,
                               firstBinding,
                               bindingCount,
                               reinterpret_cast<const VkBuffer *>( pBuffers ),
                               reinterpret_cast<const VkDeviceSize *>( pOffsets ),
                               reinterpret_cast<const VkDeviceSize *>( pSizes ),
                               reinterpret_cast<const VkDeviceSize *>( pStrides ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers2( uint32_t                                                                         firstBinding,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes,
                                                            VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides,
                                                            Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
    VULKAN_HPP_ASSERT( sizes.empty() || buffers.size() == sizes.size() );
    VULKAN_HPP_ASSERT( strides.empty() || buffers.size() == strides.size() );
#  else
    if ( buffers.size() != offsets.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != offsets.size()" );
    }
    if ( !sizes.empty() && buffers.size() != sizes.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != sizes.size()" );
    }
    if ( !strides.empty() && buffers.size() != strides.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2: buffers.size() != strides.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkCmdBindVertexBuffers2( m_commandBuffer,
                               firstBinding,
                               buffers.size(),
                               reinterpret_cast<const VkBuffer *>( buffers.data() ),
                               reinterpret_cast<const VkDeviceSize *>( offsets.data() ),
                               reinterpret_cast<const VkDeviceSize *>( sizes.data() ),
                               reinterpret_cast<const VkDeviceSize *>( strides.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthTestEnable( m_commandBuffer, static_cast<VkBool32>( depthTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthWriteEnable( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthWriteEnable( m_commandBuffer, static_cast<VkBool32>( depthWriteEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthCompareOp( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthCompareOp( m_commandBuffer, static_cast<VkCompareOp>( depthCompareOp ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthBoundsTestEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable,
                                                                  Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBoundsTestEnable( m_commandBuffer, static_cast<VkBool32>( depthBoundsTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setStencilTestEnable( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilTestEnable( m_commandBuffer, static_cast<VkBool32>( stencilTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setStencilOp( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                                                      VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                                                      VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                                                      VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                                                      VULKAN_HPP_NAMESPACE::CompareOp        compareOp,
                                                      Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilOp( m_commandBuffer,
                         static_cast<VkStencilFaceFlags>( faceMask ),
                         static_cast<VkStencilOp>( failOp ),
                         static_cast<VkStencilOp>( passOp ),
                         static_cast<VkStencilOp>( depthFailOp ),
                         static_cast<VkCompareOp>( compareOp ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setRasterizerDiscardEnable( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable,
                                                                    Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetRasterizerDiscardEnable( m_commandBuffer, static_cast<VkBool32>( rasterizerDiscardEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthBiasEnable( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBiasEnable( m_commandBuffer, static_cast<VkBool32>( depthBiasEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveRestartEnable( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable,
                                                                   Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetPrimitiveRestartEnable( m_commandBuffer, static_cast<VkBool32>( primitiveRestartEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements * pInfo,
                                                              VULKAN_HPP_NAMESPACE::MemoryRequirements2 *                  pMemoryRequirements,
                                                              Dispatch const &                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceBufferMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceBufferMemoryRequirements *>( pInfo ), reinterpret_cast<VkMemoryRequirements2 *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
    Device::getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
    d.vkGetDeviceBufferMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceBufferMemoryRequirements *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return memoryRequirements;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
    Device::getBufferMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceBufferMemoryRequirements & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
    d.vkGetDeviceBufferMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceBufferMemoryRequirements *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements * pInfo,
                                                             VULKAN_HPP_NAMESPACE::MemoryRequirements2 *                 pMemoryRequirements,
                                                             Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceImageMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceImageMemoryRequirements *>( pInfo ), reinterpret_cast<VkMemoryRequirements2 *>( pMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MemoryRequirements2
    Device::getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryRequirements2 memoryRequirements;
    d.vkGetDeviceImageMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return memoryRequirements;
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>
    Device::getImageMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...> structureChain;
    VULKAN_HPP_NAMESPACE::MemoryRequirements2 &      memoryRequirements = structureChain.template get<VULKAN_HPP_NAMESPACE::MemoryRequirements2>();
    d.vkGetDeviceImageMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ), reinterpret_cast<VkMemoryRequirements2 *>( &memoryRequirements ) );

    return structureChain;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getImageSparseMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements * pInfo,
                                                                   uint32_t *                                                  pSparseMemoryRequirementCount,
                                                                   VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2 *      pSparseMemoryRequirements,
                                                                   Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetDeviceImageSparseMemoryRequirements( m_device,
                                                reinterpret_cast<const VkDeviceImageMemoryRequirements *>( pInfo ),
                                                pSparseMemoryRequirementCount,
                                                reinterpret_cast<VkSparseImageMemoryRequirements2 *>( pSparseMemoryRequirements ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename SparseImageMemoryRequirements2Allocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2, SparseImageMemoryRequirements2Allocator>
    Device::getImageSparseMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2, SparseImageMemoryRequirements2Allocator> sparseMemoryRequirements;
    uint32_t                                                                                                   sparseMemoryRequirementCount;
    d.vkGetDeviceImageSparseMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ), &sparseMemoryRequirementCount, nullptr );
    sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
    d.vkGetDeviceImageSparseMemoryRequirements( m_device,
                                                reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ),
                                                &sparseMemoryRequirementCount,
                                                reinterpret_cast<VkSparseImageMemoryRequirements2 *>( sparseMemoryRequirements.data() ) );

    VULKAN_HPP_ASSERT( sparseMemoryRequirementCount <= sparseMemoryRequirements.size() );
    if ( sparseMemoryRequirementCount < sparseMemoryRequirements.size() )
    {
      sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
    }
    return sparseMemoryRequirements;
  }

  template <typename SparseImageMemoryRequirements2Allocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2, SparseImageMemoryRequirements2Allocator>
                                         Device::getImageSparseMemoryRequirements( const VULKAN_HPP_NAMESPACE::DeviceImageMemoryRequirements & info,
                                              SparseImageMemoryRequirements2Allocator &                   sparseImageMemoryRequirements2Allocator,
                                              Dispatch const &                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SparseImageMemoryRequirements2, SparseImageMemoryRequirements2Allocator> sparseMemoryRequirements(
      sparseImageMemoryRequirements2Allocator );
    uint32_t sparseMemoryRequirementCount;
    d.vkGetDeviceImageSparseMemoryRequirements(
      m_device, reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ), &sparseMemoryRequirementCount, nullptr );
    sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
    d.vkGetDeviceImageSparseMemoryRequirements( m_device,
                                                reinterpret_cast<const VkDeviceImageMemoryRequirements *>( &info ),
                                                &sparseMemoryRequirementCount,
                                                reinterpret_cast<VkSparseImageMemoryRequirements2 *>( sparseMemoryRequirements.data() ) );

    VULKAN_HPP_ASSERT( sparseMemoryRequirementCount <= sparseMemoryRequirements.size() );
    if ( sparseMemoryRequirementCount < sparseMemoryRequirements.size() )
    {
      sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
    }
    return sparseMemoryRequirements;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VKSC_VERSION_1_0 ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::getCommandPoolMemoryConsumption( VULKAN_HPP_NAMESPACE::CommandPool                    commandPool,
                                                                  VULKAN_HPP_NAMESPACE::CommandBuffer                  commandBuffer,
                                                                  VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption * pConsumption,
                                                                  Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetCommandPoolMemoryConsumption( m_device,
                                         static_cast<VkCommandPool>( commandPool ),
                                         static_cast<VkCommandBuffer>( commandBuffer ),
                                         reinterpret_cast<VkCommandPoolMemoryConsumption *>( pConsumption ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption Device::getCommandPoolMemoryConsumption(
    VULKAN_HPP_NAMESPACE::CommandPool commandPool, VULKAN_HPP_NAMESPACE::CommandBuffer commandBuffer, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::CommandPoolMemoryConsumption consumption;
    d.vkGetCommandPoolMemoryConsumption( m_device,
                                         static_cast<VkCommandPool>( commandPool ),
                                         static_cast<VkCommandBuffer>( commandBuffer ),
                                         reinterpret_cast<VkCommandPoolMemoryConsumption *>( &consumption ) );

    return consumption;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getFaultData( VULKAN_HPP_NAMESPACE::FaultQueryBehavior faultQueryBehavior,
                                                                      VULKAN_HPP_NAMESPACE::Bool32 *           pUnrecordedFaults,
                                                                      uint32_t *                               pFaultCount,
                                                                      VULKAN_HPP_NAMESPACE::FaultData *        pFaults,
                                                                      Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetFaultData( m_device,
                                                  static_cast<VkFaultQueryBehavior>( faultQueryBehavior ),
                                                  reinterpret_cast<VkBool32 *>( pUnrecordedFaults ),
                                                  pFaultCount,
                                                  reinterpret_cast<VkFaultData *>( pFaults ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename FaultDataAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator>>>::type
    Device::getFaultData( VULKAN_HPP_NAMESPACE::FaultQueryBehavior faultQueryBehavior, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator>> data_;
    VULKAN_HPP_NAMESPACE::Bool32 &                                                                            unrecordedFaults = data_.first;
    std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator> &                                        faults           = data_.second;
    uint32_t                                                                                                  faultCount;
    VULKAN_HPP_NAMESPACE::Result                                                                              result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFaultData(
        m_device, static_cast<VkFaultQueryBehavior>( faultQueryBehavior ), reinterpret_cast<VkBool32 *>( &unrecordedFaults ), &faultCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && faultCount )
      {
        faults.resize( faultCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFaultData( m_device,
                                                                              static_cast<VkFaultQueryBehavior>( faultQueryBehavior ),
                                                                              reinterpret_cast<VkBool32 *>( &unrecordedFaults ),
                                                                              &faultCount,
                                                                              reinterpret_cast<VkFaultData *>( faults.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFaultData" );

    return createResultValueType( result, data_ );
  }

  template <typename FaultDataAllocator,
            typename Dispatch,
            typename B2,
            typename std::enable_if<std::is_same<typename B2::value_type, VULKAN_HPP_NAMESPACE::FaultData>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator>>>::type
    Device::getFaultData( VULKAN_HPP_NAMESPACE::FaultQueryBehavior faultQueryBehavior, FaultDataAllocator & faultDataAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<VULKAN_HPP_NAMESPACE::Bool32, std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator>> data_(
      std::piecewise_construct, std::forward_as_tuple(), std::forward_as_tuple( faultDataAllocator ) );
    VULKAN_HPP_NAMESPACE::Bool32 &                                     unrecordedFaults = data_.first;
    std::vector<VULKAN_HPP_NAMESPACE::FaultData, FaultDataAllocator> & faults           = data_.second;
    uint32_t                                                           faultCount;
    VULKAN_HPP_NAMESPACE::Result                                       result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFaultData(
        m_device, static_cast<VkFaultQueryBehavior>( faultQueryBehavior ), reinterpret_cast<VkBool32 *>( &unrecordedFaults ), &faultCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && faultCount )
      {
        faults.resize( faultCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFaultData( m_device,
                                                                              static_cast<VkFaultQueryBehavior>( faultQueryBehavior ),
                                                                              reinterpret_cast<VkBool32 *>( &unrecordedFaults ),
                                                                              &faultCount,
                                                                              reinterpret_cast<VkFaultData *>( faults.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFaultData" );

    return createResultValueType( result, data_ );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_surface ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroySurfaceKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR                  surface,
                                                      const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                      Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroySurfaceKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR                          surface,
                                                      Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                      Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySurfaceKHR( m_instance,
                           static_cast<VkSurfaceKHR>( surface ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( VULKAN_HPP_NAMESPACE::SurfaceKHR                  surface,
                                            const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                            Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( VULKAN_HPP_NAMESPACE::SurfaceKHR                          surface,
                                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                            Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroySurfaceKHR( m_instance,
                           static_cast<VkSurfaceKHR>( surface ),
                           reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSurfaceSupportKHR( uint32_t                         queueFamilyIndex,
                                                                                      VULKAN_HPP_NAMESPACE::SurfaceKHR surface,
                                                                                      VULKAN_HPP_NAMESPACE::Bool32 *   pSupported,
                                                                                      Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceSupportKHR(
      m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkBool32 *>( pSupported ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Bool32>::type
                       PhysicalDevice::getSurfaceSupportKHR( uint32_t queueFamilyIndex, VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Bool32 supported;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceSupportKHR(
      m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkBool32 *>( &supported ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceSupportKHR" );

    return createResultValueType( result, supported );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSurfaceCapabilitiesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR               surface,
                                                                                           VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR * pSurfaceCapabilities,
                                                                                           Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR *>( pSurfaceCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR>::type
                       PhysicalDevice::getSurfaceCapabilitiesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceCapabilitiesKHR surfaceCapabilities;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR *>( &surfaceCapabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilitiesKHR" );

    return createResultValueType( result, surfaceCapabilities );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSurfaceFormatsKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR         surface,
                                                                                      uint32_t *                               pSurfaceFormatCount,
                                                                                      VULKAN_HPP_NAMESPACE::SurfaceFormatKHR * pSurfaceFormats,
                                                                                      Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceFormatsKHR(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pSurfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR *>( pSurfaceFormats ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename SurfaceFormatKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR, SurfaceFormatKHRAllocator>>::type
                       PhysicalDevice::getSurfaceFormatsKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR, SurfaceFormatKHRAllocator> surfaceFormats;
    uint32_t                                                                       surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                                                   result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        surfaceFormats.resize( surfaceFormatCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormatsKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormatsKHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      surfaceFormats.resize( surfaceFormatCount );
    }
    return createResultValueType( result, surfaceFormats );
  }

  template <typename SurfaceFormatKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::SurfaceFormatKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR, SurfaceFormatKHRAllocator>>::type
                       PhysicalDevice::getSurfaceFormatsKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface,
                                          SurfaceFormatKHRAllocator &      surfaceFormatKHRAllocator,
                                          Dispatch const &                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormatKHR, SurfaceFormatKHRAllocator> surfaceFormats( surfaceFormatKHRAllocator );
    uint32_t                                                                       surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                                                   result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        surfaceFormats.resize( surfaceFormatCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormatsKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormatsKHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      surfaceFormats.resize( surfaceFormatCount );
    }
    return createResultValueType( result, surfaceFormats );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR       surface,
                                                                                           uint32_t *                             pPresentModeCount,
                                                                                           VULKAN_HPP_NAMESPACE::PresentModeKHR * pPresentModes,
                                                                                           Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfacePresentModesKHR(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pPresentModeCount, reinterpret_cast<VkPresentModeKHR *>( pPresentModes ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PresentModeKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR, PresentModeKHRAllocator>>::type
                       PhysicalDevice::getSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR, PresentModeKHRAllocator> presentModes;
    uint32_t                                                                   presentModeCount;
    VULKAN_HPP_NAMESPACE::Result                                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && presentModeCount )
      {
        presentModes.resize( presentModeCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfacePresentModesKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, reinterpret_cast<VkPresentModeKHR *>( presentModes.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfacePresentModesKHR" );
    VULKAN_HPP_ASSERT( presentModeCount <= presentModes.size() );
    if ( presentModeCount < presentModes.size() )
    {
      presentModes.resize( presentModeCount );
    }
    return createResultValueType( result, presentModes );
  }

  template <typename PresentModeKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PresentModeKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR, PresentModeKHRAllocator>>::type
                       PhysicalDevice::getSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface,
                                               PresentModeKHRAllocator &        presentModeKHRAllocator,
                                               Dispatch const &                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PresentModeKHR, PresentModeKHRAllocator> presentModes( presentModeKHRAllocator );
    uint32_t                                                                   presentModeCount;
    VULKAN_HPP_NAMESPACE::Result                                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && presentModeCount )
      {
        presentModes.resize( presentModeCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfacePresentModesKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, reinterpret_cast<VkPresentModeKHR *>( presentModes.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfacePresentModesKHR" );
    VULKAN_HPP_ASSERT( presentModeCount <= presentModes.size() );
    if ( presentModeCount < presentModes.size() )
    {
      presentModes.resize( presentModeCount );
    }
    return createResultValueType( result, presentModes );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_swapchain ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createSwapchainKHR( const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR * pCreateInfo,
                                                                            const VULKAN_HPP_NAMESPACE::AllocationCallbacks *    pAllocator,
                                                                            VULKAN_HPP_NAMESPACE::SwapchainKHR *                 pSwapchain,
                                                                            Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSwapchainKHR( m_device,
                                                        reinterpret_cast<const VkSwapchainCreateInfoKHR *>( pCreateInfo ),
                                                        reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                        reinterpret_cast<VkSwapchainKHR *>( pSwapchain ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SwapchainKHR>::type
                       Device::createSwapchainKHR( const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR &      createInfo,
                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSwapchainKHR( m_device,
                              reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
                              reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                              reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSwapchainKHR" );

    return createResultValueType( result, swapchain );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>>::type
                       Device::createSwapchainKHRUnique( const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR &      createInfo,
                                      Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                      Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateSwapchainKHR( m_device,
                              reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
                              reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                              reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSwapchainKHRUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>( swapchain, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSwapchainImagesKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain,
                                                                               uint32_t *                         pSwapchainImageCount,
                                                                               VULKAN_HPP_NAMESPACE::Image *      pSwapchainImages,
                                                                               Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), pSwapchainImageCount, reinterpret_cast<VkImage *>( pSwapchainImages ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename ImageAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::Image, ImageAllocator>>::type
                       Device::getSwapchainImagesKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Image, ImageAllocator> swapchainImages;
    uint32_t                                                 swapchainImageCount;
    VULKAN_HPP_NAMESPACE::Result                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && swapchainImageCount )
      {
        swapchainImages.resize( swapchainImageCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetSwapchainImagesKHR(
          m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, reinterpret_cast<VkImage *>( swapchainImages.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSwapchainImagesKHR" );
    VULKAN_HPP_ASSERT( swapchainImageCount <= swapchainImages.size() );
    if ( swapchainImageCount < swapchainImages.size() )
    {
      swapchainImages.resize( swapchainImageCount );
    }
    return createResultValueType( result, swapchainImages );
  }

  template <typename ImageAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::Image>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::Image, ImageAllocator>>::type
                       Device::getSwapchainImagesKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain, ImageAllocator & imageAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Image, ImageAllocator> swapchainImages( imageAllocator );
    uint32_t                                                 swapchainImageCount;
    VULKAN_HPP_NAMESPACE::Result                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && swapchainImageCount )
      {
        swapchainImages.resize( swapchainImageCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetSwapchainImagesKHR(
          m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, reinterpret_cast<VkImage *>( swapchainImages.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSwapchainImagesKHR" );
    VULKAN_HPP_ASSERT( swapchainImageCount <= swapchainImages.size() );
    if ( swapchainImageCount < swapchainImages.size() )
    {
      swapchainImages.resize( swapchainImageCount );
    }
    return createResultValueType( result, swapchainImages );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::acquireNextImageKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain,
                                                                             uint64_t                           timeout,
                                                                             VULKAN_HPP_NAMESPACE::Semaphore    semaphore,
                                                                             VULKAN_HPP_NAMESPACE::Fence        fence,
                                                                             uint32_t *                         pImageIndex,
                                                                             Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAcquireNextImageKHR(
      m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), pImageIndex ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<uint32_t> Device::acquireNextImageKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain,
                                                                                            uint64_t                           timeout,
                                                                                            VULKAN_HPP_NAMESPACE::Semaphore    semaphore,
                                                                                            VULKAN_HPP_NAMESPACE::Fence        fence,
                                                                                            Dispatch const &                   d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint32_t                     imageIndex;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAcquireNextImageKHR(
      m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), &imageIndex ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::acquireNextImageKHR",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess,
                   VULKAN_HPP_NAMESPACE::Result::eTimeout,
                   VULKAN_HPP_NAMESPACE::Result::eNotReady,
                   VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

    return ResultValue<uint32_t>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), imageIndex );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Queue::presentKHR( const VULKAN_HPP_NAMESPACE::PresentInfoKHR * pPresentInfo,
                                                                   Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR *>( pPresentInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Queue::presentKHR( const VULKAN_HPP_NAMESPACE::PresentInfoKHR & presentInfo,
                                                                                         Dispatch const &                             d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR *>( &presentInfo ) ) );
    resultCheck(
      result, VULKAN_HPP_NAMESPACE_STRING "::Queue::presentKHR", { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getGroupPresentCapabilitiesKHR(
    VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR * pDeviceGroupPresentCapabilities, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetDeviceGroupPresentCapabilitiesKHR( m_device, reinterpret_cast<VkDeviceGroupPresentCapabilitiesKHR *>( pDeviceGroupPresentCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR>::type
                       Device::getGroupPresentCapabilitiesKHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DeviceGroupPresentCapabilitiesKHR deviceGroupPresentCapabilities;
    VULKAN_HPP_NAMESPACE::Result                            result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetDeviceGroupPresentCapabilitiesKHR( m_device, reinterpret_cast<VkDeviceGroupPresentCapabilitiesKHR *>( &deviceGroupPresentCapabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getGroupPresentCapabilitiesKHR" );

    return createResultValueType( result, deviceGroupPresentCapabilities );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getGroupSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR                       surface,
                                                                                        VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR * pModes,
                                                                                        Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetDeviceGroupSurfacePresentModesKHR(
      m_device, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkDeviceGroupPresentModeFlagsKHR *>( pModes ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR>::type
                       Device::getGroupSurfacePresentModesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DeviceGroupPresentModeFlagsKHR modes;
    VULKAN_HPP_NAMESPACE::Result                         result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDeviceGroupSurfacePresentModesKHR(
      m_device, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkDeviceGroupPresentModeFlagsKHR *>( &modes ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getGroupSurfacePresentModesKHR" );

    return createResultValueType( result, modes );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getPresentRectanglesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface,
                                                                                         uint32_t *                       pRectCount,
                                                                                         VULKAN_HPP_NAMESPACE::Rect2D *   pRects,
                                                                                         Dispatch const &                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDevicePresentRectanglesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pRectCount, reinterpret_cast<VkRect2D *>( pRects ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Rect2DAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::Rect2D, Rect2DAllocator>>::type
                       PhysicalDevice::getPresentRectanglesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Rect2D, Rect2DAllocator> rects;
    uint32_t                                                   rectCount;
    VULKAN_HPP_NAMESPACE::Result                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDevicePresentRectanglesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &rectCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && rectCount )
      {
        rects.resize( rectCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDevicePresentRectanglesKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &rectCount, reinterpret_cast<VkRect2D *>( rects.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getPresentRectanglesKHR" );
    VULKAN_HPP_ASSERT( rectCount <= rects.size() );
    if ( rectCount < rects.size() )
    {
      rects.resize( rectCount );
    }
    return createResultValueType( result, rects );
  }

  template <typename Rect2DAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::Rect2D>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::Rect2D, Rect2DAllocator>>::type
    PhysicalDevice::getPresentRectanglesKHR( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Rect2DAllocator & rect2DAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::Rect2D, Rect2DAllocator> rects( rect2DAllocator );
    uint32_t                                                   rectCount;
    VULKAN_HPP_NAMESPACE::Result                               result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetPhysicalDevicePresentRectanglesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &rectCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && rectCount )
      {
        rects.resize( rectCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDevicePresentRectanglesKHR(
          m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &rectCount, reinterpret_cast<VkRect2D *>( rects.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getPresentRectanglesKHR" );
    VULKAN_HPP_ASSERT( rectCount <= rects.size() );
    if ( rectCount < rects.size() )
    {
      rects.resize( rectCount );
    }
    return createResultValueType( result, rects );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::acquireNextImage2KHR( const VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR * pAcquireInfo,
                                                                              uint32_t *                                            pImageIndex,
                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAcquireNextImage2KHR( m_device, reinterpret_cast<const VkAcquireNextImageInfoKHR *>( pAcquireInfo ), pImageIndex ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ResultValue<uint32_t> Device::acquireNextImage2KHR( const VULKAN_HPP_NAMESPACE::AcquireNextImageInfoKHR & acquireInfo,
                                                                                             Dispatch const &                                      d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint32_t                     imageIndex;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkAcquireNextImage2KHR( m_device, reinterpret_cast<const VkAcquireNextImageInfoKHR *>( &acquireInfo ), &imageIndex ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::acquireNextImage2KHR",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess,
                   VULKAN_HPP_NAMESPACE::Result::eTimeout,
                   VULKAN_HPP_NAMESPACE::Result::eNotReady,
                   VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

    return ResultValue<uint32_t>( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), imageIndex );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_display ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayPropertiesKHR( uint32_t *                                   pPropertyCount,
                                                                                         VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR * pProperties,
                                                                                         Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPropertiesKHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayPropertiesKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR, DisplayPropertiesKHRAllocator>>::type
                       PhysicalDevice::getDisplayPropertiesKHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR, DisplayPropertiesKHRAllocator> properties;
    uint32_t                                                                               propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                           result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayPropertiesKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR, DisplayPropertiesKHRAllocator>>::type
                       PhysicalDevice::getDisplayPropertiesKHR( DisplayPropertiesKHRAllocator & displayPropertiesKHRAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPropertiesKHR, DisplayPropertiesKHRAllocator> properties( displayPropertiesKHRAllocator );
    uint32_t                                                                               propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                           result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayPlanePropertiesKHR( uint32_t *                                        pPropertyCount,
                                                                                              VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR * pProperties,
                                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayPlanePropertiesKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR, DisplayPlanePropertiesKHRAllocator>>::type
    PhysicalDevice::getDisplayPlanePropertiesKHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR, DisplayPlanePropertiesKHRAllocator> properties;
    uint32_t                                                                                         propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
          m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlanePropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayPlanePropertiesKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR, DisplayPlanePropertiesKHRAllocator>>::type
    PhysicalDevice::getDisplayPlanePropertiesKHR( DisplayPlanePropertiesKHRAllocator & displayPlanePropertiesKHRAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPlanePropertiesKHR, DisplayPlanePropertiesKHRAllocator> properties( displayPlanePropertiesKHRAllocator );
    uint32_t                                                                                         propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
          m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlanePropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR( uint32_t                           planeIndex,
                                                                                                     uint32_t *                         pDisplayCount,
                                                                                                     VULKAN_HPP_NAMESPACE::DisplayKHR * pDisplays,
                                                                                                     Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, pDisplayCount, reinterpret_cast<VkDisplayKHR *>( pDisplays ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayKHR, DisplayKHRAllocator>>::type
                       PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayKHR, DisplayKHRAllocator> displays;
    uint32_t                                                           displayCount;
    VULKAN_HPP_NAMESPACE::Result                                       result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && displayCount )
      {
        displays.resize( displayCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR *>( displays.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
    VULKAN_HPP_ASSERT( displayCount <= displays.size() );
    if ( displayCount < displays.size() )
    {
      displays.resize( displayCount );
    }
    return createResultValueType( result, displays );
  }

  template <typename DisplayKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayKHR, DisplayKHRAllocator>>::type
    PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex, DisplayKHRAllocator & displayKHRAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayKHR, DisplayKHRAllocator> displays( displayKHRAllocator );
    uint32_t                                                           displayCount;
    VULKAN_HPP_NAMESPACE::Result                                       result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && displayCount )
      {
        displays.resize( displayCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR *>( displays.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
    VULKAN_HPP_ASSERT( displayCount <= displays.size() );
    if ( displayCount < displays.size() )
    {
      displays.resize( displayCount );
    }
    return createResultValueType( result, displays );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayModePropertiesKHR( VULKAN_HPP_NAMESPACE::DisplayKHR                 display,
                                                                                             uint32_t *                                       pPropertyCount,
                                                                                             VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR * pProperties,
                                                                                             Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetDisplayModePropertiesKHR(
      m_physicalDevice, static_cast<VkDisplayKHR>( display ), pPropertyCount, reinterpret_cast<VkDisplayModePropertiesKHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayModePropertiesKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR, DisplayModePropertiesKHRAllocator>>::type
    PhysicalDevice::getDisplayModePropertiesKHR( VULKAN_HPP_NAMESPACE::DisplayKHR display, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR, DisplayModePropertiesKHRAllocator> properties;
    uint32_t                                                                                       propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                   result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayModePropertiesKHR(
          m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModePropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayModePropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayModePropertiesKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR, DisplayModePropertiesKHRAllocator>>::type
    PhysicalDevice::getDisplayModePropertiesKHR( VULKAN_HPP_NAMESPACE::DisplayKHR    display,
                                                 DisplayModePropertiesKHRAllocator & displayModePropertiesKHRAllocator,
                                                 Dispatch const &                    d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayModePropertiesKHR, DisplayModePropertiesKHRAllocator> properties( displayModePropertiesKHRAllocator );
    uint32_t                                                                                       propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                   result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayModePropertiesKHR(
          m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModePropertiesKHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayModePropertiesKHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::createDisplayModeKHR( VULKAN_HPP_NAMESPACE::DisplayKHR                       display,
                                                                                      const VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR * pCreateInfo,
                                                                                      const VULKAN_HPP_NAMESPACE::AllocationCallbacks *      pAllocator,
                                                                                      VULKAN_HPP_NAMESPACE::DisplayModeKHR *                 pMode,
                                                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDisplayModeKHR( m_physicalDevice,
                                                          static_cast<VkDisplayKHR>( display ),
                                                          reinterpret_cast<const VkDisplayModeCreateInfoKHR *>( pCreateInfo ),
                                                          reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                          reinterpret_cast<VkDisplayModeKHR *>( pMode ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DisplayModeKHR>::type
                       PhysicalDevice::createDisplayModeKHR( VULKAN_HPP_NAMESPACE::DisplayKHR                          display,
                                          const VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR &    createInfo,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                          Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DisplayModeKHR mode;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDisplayModeKHR( m_physicalDevice,
                                static_cast<VkDisplayKHR>( display ),
                                reinterpret_cast<const VkDisplayModeCreateInfoKHR *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkDisplayModeKHR *>( &mode ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::createDisplayModeKHR" );

    return createResultValueType( result, mode );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::DisplayModeKHR, Dispatch>>::type
                       PhysicalDevice::createDisplayModeKHRUnique( VULKAN_HPP_NAMESPACE::DisplayKHR                          display,
                                                const VULKAN_HPP_NAMESPACE::DisplayModeCreateInfoKHR &    createInfo,
                                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DisplayModeKHR mode;
    VULKAN_HPP_NAMESPACE::Result         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkCreateDisplayModeKHR( m_physicalDevice,
                                static_cast<VkDisplayKHR>( display ),
                                reinterpret_cast<const VkDisplayModeCreateInfoKHR *>( &createInfo ),
                                reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
                                reinterpret_cast<VkDisplayModeKHR *>( &mode ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::createDisplayModeKHRUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::DisplayModeKHR, Dispatch>( mode, ObjectDestroy<PhysicalDevice, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getDisplayPlaneCapabilitiesKHR( VULKAN_HPP_NAMESPACE::DisplayModeKHR                mode,
                                                    uint32_t                                            planeIndex,
                                                    VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR * pCapabilities,
                                                    Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetDisplayPlaneCapabilitiesKHR(
      m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR *>( pCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR>::type
    PhysicalDevice::getDisplayPlaneCapabilitiesKHR( VULKAN_HPP_NAMESPACE::DisplayModeKHR mode, uint32_t planeIndex, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilitiesKHR capabilities;
    VULKAN_HPP_NAMESPACE::Result                      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayPlaneCapabilitiesKHR(
      m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR *>( &capabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneCapabilitiesKHR" );

    return createResultValueType( result, capabilities );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Instance::createDisplayPlaneSurfaceKHR( const VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR * pCreateInfo,
                                                                                        const VULKAN_HPP_NAMESPACE::AllocationCallbacks *         pAllocator,
                                                                                        VULKAN_HPP_NAMESPACE::SurfaceKHR *                        pSurface,
                                                                                        Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDisplayPlaneSurfaceKHR( m_instance,
                                                                  reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR *>( pCreateInfo ),
                                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                                  reinterpret_cast<VkSurfaceKHR *>( pSurface ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SurfaceKHR>::type
                       Instance::createDisplayPlaneSurfaceKHR( const VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR & createInfo,
                                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                            Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDisplayPlaneSurfaceKHR(
      m_instance,
      reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createDisplayPlaneSurfaceKHR" );

    return createResultValueType( result, surface );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR, Dispatch>>::type
                       Instance::createDisplayPlaneSurfaceKHRUnique( const VULKAN_HPP_NAMESPACE::DisplaySurfaceCreateInfoKHR & createInfo,
                                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                  Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDisplayPlaneSurfaceKHR(
      m_instance,
      reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createDisplayPlaneSurfaceKHRUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR, Dispatch>( surface, ObjectDestroy<Instance, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_display_swapchain ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::createSharedSwapchainsKHR( uint32_t                                             swapchainCount,
                                                                                   const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR * pCreateInfos,
                                                                                   const VULKAN_HPP_NAMESPACE::AllocationCallbacks *    pAllocator,
                                                                                   VULKAN_HPP_NAMESPACE::SwapchainKHR *                 pSwapchains,
                                                                                   Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSharedSwapchainsKHR( m_device,
                                                               swapchainCount,
                                                               reinterpret_cast<const VkSwapchainCreateInfoKHR *>( pCreateInfos ),
                                                               reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                               reinterpret_cast<VkSwapchainKHR *>( pSwapchains ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename SwapchainKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainKHRAllocator>>::type
    Device::createSharedSwapchainsKHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                       Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                    allocator,
                                       Dispatch const &                                                                             d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainKHRAllocator> swapchains( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                                           result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      createInfos.size(),
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( swapchains.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainsKHR" );

    return createResultValueType( result, swapchains );
  }

  template <typename SwapchainKHRAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, VULKAN_HPP_NAMESPACE::SwapchainKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainKHRAllocator>>::type
    Device::createSharedSwapchainsKHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                       Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                    allocator,
                                       SwapchainKHRAllocator &                                                                      swapchainKHRAllocator,
                                       Dispatch const &                                                                             d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainKHRAllocator> swapchains( createInfos.size(), swapchainKHRAllocator );
    VULKAN_HPP_NAMESPACE::Result                                           result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      createInfos.size(),
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( swapchains.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainsKHR" );

    return createResultValueType( result, swapchains );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SwapchainKHR>::type
                       Device::createSharedSwapchainKHR( const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR &      createInfo,
                                      Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                      Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      1,
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainKHR" );

    return createResultValueType( result, swapchain );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch, typename SwapchainKHRAllocator>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>, SwapchainKHRAllocator>>::type
    Device::createSharedSwapchainsKHRUnique( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                    allocator,
                                             Dispatch const &                                                                             d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR> swapchains( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      createInfos.size(),
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( swapchains.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainsKHRUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>, SwapchainKHRAllocator> uniqueSwapchains;
    uniqueSwapchains.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & swapchain : swapchains )
    {
      uniqueSwapchains.push_back( UniqueHandle<SwapchainKHR, Dispatch>( swapchain, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueSwapchains ) );
  }

  template <typename Dispatch,
            typename SwapchainKHRAllocator,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>, SwapchainKHRAllocator>>::type
    Device::createSharedSwapchainsKHRUnique( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR> const & createInfos,
                                             Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>                                    allocator,
                                             SwapchainKHRAllocator &                                                                      swapchainKHRAllocator,
                                             Dispatch const &                                                                             d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SwapchainKHR> swapchains( createInfos.size() );
    VULKAN_HPP_NAMESPACE::Result                    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      createInfos.size(),
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( createInfos.data() ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( swapchains.data() ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainsKHRUnique" );
    std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>, SwapchainKHRAllocator> uniqueSwapchains( swapchainKHRAllocator );
    uniqueSwapchains.reserve( createInfos.size() );
    ObjectDestroy<Device, Dispatch> deleter( *this, allocator, d );
    for ( auto const & swapchain : swapchains )
    {
      uniqueSwapchains.push_back( UniqueHandle<SwapchainKHR, Dispatch>( swapchain, deleter ) );
    }
    return createResultValueType( result, std::move( uniqueSwapchains ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>>::type
                       Device::createSharedSwapchainKHRUnique( const VULKAN_HPP_NAMESPACE::SwapchainCreateInfoKHR &      createInfo,
                                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                            Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain;
    VULKAN_HPP_NAMESPACE::Result       result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSharedSwapchainsKHR(
      m_device,
      1,
      reinterpret_cast<const VkSwapchainCreateInfoKHR *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSwapchainKHR *>( &swapchain ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSharedSwapchainKHRUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR, Dispatch>( swapchain, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_external_memory_fd ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getMemoryFdKHR( const VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR * pGetFdInfo,
                                                                        int *                                            pFd,
                                                                        Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetMemoryFdKHR( m_device, reinterpret_cast<const VkMemoryGetFdInfoKHR *>( pGetFdInfo ), pFd ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<int>::type Device::getMemoryFdKHR( const VULKAN_HPP_NAMESPACE::MemoryGetFdInfoKHR & getFdInfo,
                                                                                                     Dispatch const &                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    int                          fd;
    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetMemoryFdKHR( m_device, reinterpret_cast<const VkMemoryGetFdInfoKHR *>( &getFdInfo ), &fd ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryFdKHR" );

    return createResultValueType( result, fd );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getMemoryFdPropertiesKHR( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType,
                                                                                  int                                                    fd,
                                                                                  VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR *          pMemoryFdProperties,
                                                                                  Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetMemoryFdPropertiesKHR(
      m_device, static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ), fd, reinterpret_cast<VkMemoryFdPropertiesKHR *>( pMemoryFdProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR>::type
                       Device::getMemoryFdPropertiesKHR( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType, int fd, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryFdPropertiesKHR memoryFdProperties;
    VULKAN_HPP_NAMESPACE::Result                result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetMemoryFdPropertiesKHR(
      m_device, static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ), fd, reinterpret_cast<VkMemoryFdPropertiesKHR *>( &memoryFdProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryFdPropertiesKHR" );

    return createResultValueType( result, memoryFdProperties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_external_semaphore_fd ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::importSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR * pImportSemaphoreFdInfo,
                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkImportSemaphoreFdKHR( m_device, reinterpret_cast<const VkImportSemaphoreFdInfoKHR *>( pImportSemaphoreFdInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::importSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::ImportSemaphoreFdInfoKHR & importSemaphoreFdInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkImportSemaphoreFdKHR( m_device, reinterpret_cast<const VkImportSemaphoreFdInfoKHR *>( &importSemaphoreFdInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importSemaphoreFdKHR" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR * pGetFdInfo,
                                                                           int *                                               pFd,
                                                                           Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetSemaphoreFdKHR( m_device, reinterpret_cast<const VkSemaphoreGetFdInfoKHR *>( pGetFdInfo ), pFd ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<int>::type
                       Device::getSemaphoreFdKHR( const VULKAN_HPP_NAMESPACE::SemaphoreGetFdInfoKHR & getFdInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    int                          fd;
    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetSemaphoreFdKHR( m_device, reinterpret_cast<const VkSemaphoreGetFdInfoKHR *>( &getFdInfo ), &fd ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSemaphoreFdKHR" );

    return createResultValueType( result, fd );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_direct_mode_display ===

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE Result PhysicalDevice::releaseDisplayEXT( VULKAN_HPP_NAMESPACE::DisplayKHR display, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkReleaseDisplayEXT( m_physicalDevice, static_cast<VkDisplayKHR>( display ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::releaseDisplayEXT( VULKAN_HPP_NAMESPACE::DisplayKHR display, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkReleaseDisplayEXT( m_physicalDevice, static_cast<VkDisplayKHR>( display ) );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  //=== VK_EXT_display_surface_counter ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getSurfaceCapabilities2EXT( VULKAN_HPP_NAMESPACE::SurfaceKHR                surface,
                                                VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT * pSurfaceCapabilities,
                                                Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceCapabilities2EXT(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilities2EXT *>( pSurfaceCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT>::type
                       PhysicalDevice::getSurfaceCapabilities2EXT( VULKAN_HPP_NAMESPACE::SurfaceKHR surface, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceCapabilities2EXT surfaceCapabilities;
    VULKAN_HPP_NAMESPACE::Result                  result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceCapabilities2EXT(
      m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilities2EXT *>( &surfaceCapabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2EXT" );

    return createResultValueType( result, surfaceCapabilities );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_display_control ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::displayPowerControlEXT( VULKAN_HPP_NAMESPACE::DisplayKHR                  display,
                                                                                const VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT * pDisplayPowerInfo,
                                                                                Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkDisplayPowerControlEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayPowerInfoEXT *>( pDisplayPowerInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::displayPowerControlEXT( VULKAN_HPP_NAMESPACE::DisplayKHR                  display,
                                                                                         const VULKAN_HPP_NAMESPACE::DisplayPowerInfoEXT & displayPowerInfo,
                                                                                         Dispatch const &                                  d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkDisplayPowerControlEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayPowerInfoEXT *>( &displayPowerInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::displayPowerControlEXT" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::registerEventEXT( const VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT *  pDeviceEventInfo,
                                                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                          VULKAN_HPP_NAMESPACE::Fence *                     pFence,
                                                                          Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkRegisterDeviceEventEXT( m_device,
                                                            reinterpret_cast<const VkDeviceEventInfoEXT *>( pDeviceEventInfo ),
                                                            reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                            reinterpret_cast<VkFence *>( pFence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Fence>::type
                       Device::registerEventEXT( const VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT &          deviceEventInfo,
                              Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                              Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkRegisterDeviceEventEXT(
      m_device,
      reinterpret_cast<const VkDeviceEventInfoEXT *>( &deviceEventInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::registerEventEXT" );

    return createResultValueType( result, fence );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>>::type
                       Device::registerEventEXTUnique( const VULKAN_HPP_NAMESPACE::DeviceEventInfoEXT &          deviceEventInfo,
                                    Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                    Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkRegisterDeviceEventEXT(
      m_device,
      reinterpret_cast<const VkDeviceEventInfoEXT *>( &deviceEventInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::registerEventEXTUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>( fence, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::registerDisplayEventEXT( VULKAN_HPP_NAMESPACE::DisplayKHR                  display,
                                                                                 const VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT * pDisplayEventInfo,
                                                                                 const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                                 VULKAN_HPP_NAMESPACE::Fence *                     pFence,
                                                                                 Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkRegisterDisplayEventEXT( m_device,
                                                             static_cast<VkDisplayKHR>( display ),
                                                             reinterpret_cast<const VkDisplayEventInfoEXT *>( pDisplayEventInfo ),
                                                             reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                             reinterpret_cast<VkFence *>( pFence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::Fence>::type
                       Device::registerDisplayEventEXT( VULKAN_HPP_NAMESPACE::DisplayKHR                          display,
                                     const VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT &         displayEventInfo,
                                     Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                     Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkRegisterDisplayEventEXT(
      m_device,
      static_cast<VkDisplayKHR>( display ),
      reinterpret_cast<const VkDisplayEventInfoEXT *>( &displayEventInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::registerDisplayEventEXT" );

    return createResultValueType( result, fence );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>>::type
                       Device::registerDisplayEventEXTUnique( VULKAN_HPP_NAMESPACE::DisplayKHR                          display,
                                           const VULKAN_HPP_NAMESPACE::DisplayEventInfoEXT &         displayEventInfo,
                                           Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                           Dispatch const &                                          d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Fence  fence;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkRegisterDisplayEventEXT(
      m_device,
      static_cast<VkDisplayKHR>( display ),
      reinterpret_cast<const VkDisplayEventInfoEXT *>( &displayEventInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkFence *>( &fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::registerDisplayEventEXTUnique" );

    return createResultValueType( result,
                                  UniqueHandle<VULKAN_HPP_NAMESPACE::Fence, Dispatch>( fence, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSwapchainCounterEXT( VULKAN_HPP_NAMESPACE::SwapchainKHR              swapchain,
                                                                                VULKAN_HPP_NAMESPACE::SurfaceCounterFlagBitsEXT counter,
                                                                                uint64_t *                                      pCounterValue,
                                                                                Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetSwapchainCounterEXT( m_device, static_cast<VkSwapchainKHR>( swapchain ), static_cast<VkSurfaceCounterFlagBitsEXT>( counter ), pCounterValue ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<uint64_t>::type Device::getSwapchainCounterEXT(
    VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain, VULKAN_HPP_NAMESPACE::SurfaceCounterFlagBitsEXT counter, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint64_t                     counterValue;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetSwapchainCounterEXT( m_device, static_cast<VkSwapchainKHR>( swapchain ), static_cast<VkSurfaceCounterFlagBitsEXT>( counter ), &counterValue ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSwapchainCounterEXT" );

    return createResultValueType( result, counterValue );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_discard_rectangles ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleEXT( uint32_t                             firstDiscardRectangle,
                                                                uint32_t                             discardRectangleCount,
                                                                const VULKAN_HPP_NAMESPACE::Rect2D * pDiscardRectangles,
                                                                Dispatch const &                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDiscardRectangleEXT( m_commandBuffer, firstDiscardRectangle, discardRectangleCount, reinterpret_cast<const VkRect2D *>( pDiscardRectangles ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleEXT( uint32_t firstDiscardRectangle,
                                                                VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & discardRectangles,
                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetDiscardRectangleEXT(
      m_commandBuffer, firstDiscardRectangle, discardRectangles.size(), reinterpret_cast<const VkRect2D *>( discardRectangles.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 discardRectangleEnable,
                                                                      Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDiscardRectangleEnableEXT( m_commandBuffer, static_cast<VkBool32>( discardRectangleEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDiscardRectangleModeEXT( VULKAN_HPP_NAMESPACE::DiscardRectangleModeEXT discardRectangleMode,
                                                                    Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDiscardRectangleModeEXT( m_commandBuffer, static_cast<VkDiscardRectangleModeEXT>( discardRectangleMode ) );
  }

  //=== VK_EXT_hdr_metadata ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::setHdrMetadataEXT( uint32_t                                     swapchainCount,
                                                    const VULKAN_HPP_NAMESPACE::SwapchainKHR *   pSwapchains,
                                                    const VULKAN_HPP_NAMESPACE::HdrMetadataEXT * pMetadata,
                                                    Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkSetHdrMetadataEXT(
      m_device, swapchainCount, reinterpret_cast<const VkSwapchainKHR *>( pSwapchains ), reinterpret_cast<const VkHdrMetadataEXT *>( pMetadata ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::setHdrMetadataEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SwapchainKHR> const &   swapchains,
                                                    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::HdrMetadataEXT> const & metadata,
                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( swapchains.size() == metadata.size() );
#  else
    if ( swapchains.size() != metadata.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::Device::setHdrMetadataEXT: swapchains.size() != metadata.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkSetHdrMetadataEXT( m_device,
                           swapchains.size(),
                           reinterpret_cast<const VkSwapchainKHR *>( swapchains.data() ),
                           reinterpret_cast<const VkHdrMetadataEXT *>( metadata.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_shared_presentable_image ===

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSwapchainStatusKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain,
                                                                               Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetSwapchainStatusKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ) ) );
  }
#else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result Device::getSwapchainStatusKHR( VULKAN_HPP_NAMESPACE::SwapchainKHR swapchain,
                                                                                                     Dispatch const &                   d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetSwapchainStatusKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ) ) );
    resultCheck( result,
                 VULKAN_HPP_NAMESPACE_STRING "::Device::getSwapchainStatusKHR",
                 { VULKAN_HPP_NAMESPACE::Result::eSuccess, VULKAN_HPP_NAMESPACE::Result::eSuboptimalKHR } );

    return static_cast<VULKAN_HPP_NAMESPACE::Result>( result );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  //=== VK_KHR_external_fence_fd ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::importFenceFdKHR( const VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR * pImportFenceFdInfo,
                                                                          Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkImportFenceFdKHR( m_device, reinterpret_cast<const VkImportFenceFdInfoKHR *>( pImportFenceFdInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::importFenceFdKHR( const VULKAN_HPP_NAMESPACE::ImportFenceFdInfoKHR & importFenceFdInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkImportFenceFdKHR( m_device, reinterpret_cast<const VkImportFenceFdInfoKHR *>( &importFenceFdInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceFdKHR" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getFenceFdKHR( const VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR * pGetFdInfo,
                                                                       int *                                           pFd,
                                                                       Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetFenceFdKHR( m_device, reinterpret_cast<const VkFenceGetFdInfoKHR *>( pGetFdInfo ), pFd ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<int>::type Device::getFenceFdKHR( const VULKAN_HPP_NAMESPACE::FenceGetFdInfoKHR & getFdInfo,
                                                                                                    Dispatch const &                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    int                          fd;
    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetFenceFdKHR( m_device, reinterpret_cast<const VkFenceGetFdInfoKHR *>( &getFdInfo ), &fd ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceFdKHR" );

    return createResultValueType( result, fd );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_performance_query ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR( uint32_t                                                 queueFamilyIndex,
                                                                     uint32_t *                                               pCounterCount,
                                                                     VULKAN_HPP_NAMESPACE::PerformanceCounterKHR *            pCounters,
                                                                     VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR * pCounterDescriptions,
                                                                     Dispatch const &                                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR( m_physicalDevice,
                                                                         queueFamilyIndex,
                                                                         pCounterCount,
                                                                         reinterpret_cast<VkPerformanceCounterKHR *>( pCounters ),
                                                                         reinterpret_cast<VkPerformanceCounterDescriptionKHR *>( pCounterDescriptions ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PerformanceCounterKHRAllocator, typename PerformanceCounterDescriptionKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator>,
                                       std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator>>>::type
    PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR( uint32_t queueFamilyIndex, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator>,
              std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator>>
                                                                                                                     data_;
    std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator> &                       counters            = data_.first;
    std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator> & counterDescriptions = data_.second;
    uint32_t                                                                                                         counterCount;
    VULKAN_HPP_NAMESPACE::Result                                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR( m_physicalDevice, queueFamilyIndex, &counterCount, nullptr, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && counterCount )
      {
        counters.resize( counterCount );
        counterDescriptions.resize( counterCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
          m_physicalDevice,
          queueFamilyIndex,
          &counterCount,
          reinterpret_cast<VkPerformanceCounterKHR *>( counters.data() ),
          reinterpret_cast<VkPerformanceCounterDescriptionKHR *>( counterDescriptions.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR" );
    VULKAN_HPP_ASSERT( counterCount <= counters.size() );
    if ( counterCount < counters.size() )
    {
      counters.resize( counterCount );
      counterDescriptions.resize( counterCount );
    }
    return createResultValueType( result, data_ );
  }

  template <typename PerformanceCounterKHRAllocator,
            typename PerformanceCounterDescriptionKHRAllocator,
            typename Dispatch,
            typename B1,
            typename B2,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PerformanceCounterKHR>::value &&
                                      std::is_same<typename B2::value_type, VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR>::value,
                                    int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator>,
                                       std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator>>>::type
    PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR( uint32_t                                    queueFamilyIndex,
                                                                     PerformanceCounterKHRAllocator &            performanceCounterKHRAllocator,
                                                                     PerformanceCounterDescriptionKHRAllocator & performanceCounterDescriptionKHRAllocator,
                                                                     Dispatch const &                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator>,
              std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator>>
      data_(
        std::piecewise_construct, std::forward_as_tuple( performanceCounterKHRAllocator ), std::forward_as_tuple( performanceCounterDescriptionKHRAllocator ) );
    std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterKHR, PerformanceCounterKHRAllocator> &                       counters            = data_.first;
    std::vector<VULKAN_HPP_NAMESPACE::PerformanceCounterDescriptionKHR, PerformanceCounterDescriptionKHRAllocator> & counterDescriptions = data_.second;
    uint32_t                                                                                                         counterCount;
    VULKAN_HPP_NAMESPACE::Result                                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR( m_physicalDevice, queueFamilyIndex, &counterCount, nullptr, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && counterCount )
      {
        counters.resize( counterCount );
        counterDescriptions.resize( counterCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
          m_physicalDevice,
          queueFamilyIndex,
          &counterCount,
          reinterpret_cast<VkPerformanceCounterKHR *>( counters.data() ),
          reinterpret_cast<VkPerformanceCounterDescriptionKHR *>( counterDescriptions.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::enumerateQueueFamilyPerformanceQueryCountersKHR" );
    VULKAN_HPP_ASSERT( counterCount <= counters.size() );
    if ( counterCount < counters.size() )
    {
      counters.resize( counterCount );
      counterDescriptions.resize( counterCount );
    }
    return createResultValueType( result, data_ );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    PhysicalDevice::getQueueFamilyPerformanceQueryPassesKHR( const VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR * pPerformanceQueryCreateInfo,
                                                             uint32_t *                                                      pNumPasses,
                                                             Dispatch const &                                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
      m_physicalDevice, reinterpret_cast<const VkQueryPoolPerformanceCreateInfoKHR *>( pPerformanceQueryCreateInfo ), pNumPasses );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE uint32_t PhysicalDevice::getQueueFamilyPerformanceQueryPassesKHR(
    const VULKAN_HPP_NAMESPACE::QueryPoolPerformanceCreateInfoKHR & performanceQueryCreateInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    uint32_t numPasses;
    d.vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
      m_physicalDevice, reinterpret_cast<const VkQueryPoolPerformanceCreateInfoKHR *>( &performanceQueryCreateInfo ), &numPasses );

    return numPasses;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::acquireProfilingLockKHR( const VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR * pInfo,
                                                                                 Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkAcquireProfilingLockKHR( m_device, reinterpret_cast<const VkAcquireProfilingLockInfoKHR *>( pInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::acquireProfilingLockKHR( const VULKAN_HPP_NAMESPACE::AcquireProfilingLockInfoKHR & info, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkAcquireProfilingLockKHR( m_device, reinterpret_cast<const VkAcquireProfilingLockInfoKHR *>( &info ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::acquireProfilingLockKHR" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Device::releaseProfilingLockKHR( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkReleaseProfilingLockKHR( m_device );
  }

  //=== VK_KHR_get_surface_capabilities2 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
                                                VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR *             pSurfaceCapabilities,
                                                Dispatch const &                                            d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceCapabilities2KHR( m_physicalDevice,
                                                                              reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( pSurfaceInfo ),
                                                                              reinterpret_cast<VkSurfaceCapabilities2KHR *>( pSurfaceCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>::type
    PhysicalDevice::getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR surfaceCapabilities;
    VULKAN_HPP_NAMESPACE::Result                  result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceSurfaceCapabilities2KHR( m_physicalDevice,
                                                    reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                    reinterpret_cast<VkSurfaceCapabilities2KHR *>( &surfaceCapabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2KHR" );

    return createResultValueType( result, surfaceCapabilities );
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>>::type
    PhysicalDevice::getSurfaceCapabilities2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    StructureChain<X, Y, Z...>                      structureChain;
    VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR & surfaceCapabilities = structureChain.template get<VULKAN_HPP_NAMESPACE::SurfaceCapabilities2KHR>();
    VULKAN_HPP_NAMESPACE::Result                    result              = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceSurfaceCapabilities2KHR( m_physicalDevice,
                                                    reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                    reinterpret_cast<VkSurfaceCapabilities2KHR *>( &surfaceCapabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceCapabilities2KHR" );

    return createResultValueType( result, structureChain );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
                                                                                       uint32_t *                                pSurfaceFormatCount,
                                                                                       VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR * pSurfaceFormats,
                                                                                       Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSurfaceFormats2KHR( m_physicalDevice,
                                                                         reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( pSurfaceInfo ),
                                                                         pSurfaceFormatCount,
                                                                         reinterpret_cast<VkSurfaceFormat2KHR *>( pSurfaceFormats ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename SurfaceFormat2KHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR, SurfaceFormat2KHRAllocator>>::type
    PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR, SurfaceFormat2KHRAllocator> surfaceFormats;
    uint32_t                                                                         surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormats2KHR(
        m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        surfaceFormats.resize( surfaceFormatCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceSurfaceFormats2KHR( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                   &surfaceFormatCount,
                                                   reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      surfaceFormats.resize( surfaceFormatCount );
    }
    return createResultValueType( result, surfaceFormats );
  }

  template <typename SurfaceFormat2KHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR, SurfaceFormat2KHRAllocator>>::type
                       PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo,
                                           SurfaceFormat2KHRAllocator &                                surfaceFormat2KHRAllocator,
                                           Dispatch const &                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR, SurfaceFormat2KHRAllocator> surfaceFormats( surfaceFormat2KHRAllocator );
    uint32_t                                                                         surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormats2KHR(
        m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        surfaceFormats.resize( surfaceFormatCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceSurfaceFormats2KHR( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                   &surfaceFormatCount,
                                                   reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      surfaceFormats.resize( surfaceFormatCount );
    }
    return createResultValueType( result, surfaceFormats );
  }

  template <typename StructureChain, typename StructureChainAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<StructureChain, StructureChainAllocator>>::type
    PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<StructureChain, StructureChainAllocator> structureChains;
    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR> surfaceFormats;
    uint32_t                                             surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormats2KHR(
        m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        structureChains.resize( surfaceFormatCount );
        surfaceFormats.resize( surfaceFormatCount );
        for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
        {
          surfaceFormats[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>().pNext;
        }
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceSurfaceFormats2KHR( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                   &surfaceFormatCount,
                                                   reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      structureChains.resize( surfaceFormatCount );
    }
    for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
    {
      structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>() = surfaceFormats[i];
    }
    return createResultValueType( result, structureChains );
  }

  template <typename StructureChain,
            typename StructureChainAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, StructureChain>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<StructureChain, StructureChainAllocator>>::type
                       PhysicalDevice::getSurfaceFormats2KHR( const VULKAN_HPP_NAMESPACE::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo,
                                           StructureChainAllocator &                                   structureChainAllocator,
                                           Dispatch const &                                            d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<StructureChain, StructureChainAllocator> structureChains( structureChainAllocator );
    std::vector<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR> surfaceFormats;
    uint32_t                                             surfaceFormatCount;
    VULKAN_HPP_NAMESPACE::Result                         result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSurfaceFormats2KHR(
        m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ), &surfaceFormatCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && surfaceFormatCount )
      {
        structureChains.resize( surfaceFormatCount );
        surfaceFormats.resize( surfaceFormatCount );
        for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
        {
          surfaceFormats[i].pNext = structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>().pNext;
        }
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceSurfaceFormats2KHR( m_physicalDevice,
                                                   reinterpret_cast<const VkPhysicalDeviceSurfaceInfo2KHR *>( &surfaceInfo ),
                                                   &surfaceFormatCount,
                                                   reinterpret_cast<VkSurfaceFormat2KHR *>( surfaceFormats.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSurfaceFormats2KHR" );
    VULKAN_HPP_ASSERT( surfaceFormatCount <= surfaceFormats.size() );
    if ( surfaceFormatCount < surfaceFormats.size() )
    {
      structureChains.resize( surfaceFormatCount );
    }
    for ( uint32_t i = 0; i < surfaceFormatCount; i++ )
    {
      structureChains[i].template get<VULKAN_HPP_NAMESPACE::SurfaceFormat2KHR>() = surfaceFormats[i];
    }
    return createResultValueType( result, structureChains );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_get_display_properties2 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayProperties2KHR( uint32_t *                                    pPropertyCount,
                                                                                          VULKAN_HPP_NAMESPACE::DisplayProperties2KHR * pProperties,
                                                                                          Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceDisplayProperties2KHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayProperties2KHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayProperties2KHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR, DisplayProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayProperties2KHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR, DisplayProperties2KHRAllocator> properties;
    uint32_t                                                                                 propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayProperties2KHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceDisplayProperties2KHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayProperties2KHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayProperties2KHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR, DisplayProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayProperties2KHR( DisplayProperties2KHRAllocator & displayProperties2KHRAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayProperties2KHR, DisplayProperties2KHRAllocator> properties( displayProperties2KHRAllocator );
    uint32_t                                                                                 propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayProperties2KHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceDisplayProperties2KHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayPlaneProperties2KHR( uint32_t * pPropertyCount,
                                                                                               VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR * pProperties,
                                                                                               Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceDisplayPlaneProperties2KHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlaneProperties2KHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayPlaneProperties2KHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR, DisplayPlaneProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayPlaneProperties2KHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR, DisplayPlaneProperties2KHRAllocator> properties;
    uint32_t                                                                                           propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                       result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlaneProperties2KHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
          m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlaneProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayPlaneProperties2KHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR, DisplayPlaneProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayPlaneProperties2KHR( DisplayPlaneProperties2KHRAllocator & displayPlaneProperties2KHRAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayPlaneProperties2KHR, DisplayPlaneProperties2KHRAllocator> properties( displayPlaneProperties2KHRAllocator );
    uint32_t                                                                                           propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                       result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlaneProperties2KHR( m_physicalDevice, &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
          m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlaneProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getDisplayModeProperties2KHR( VULKAN_HPP_NAMESPACE::DisplayKHR                  display,
                                                                                              uint32_t *                                        pPropertyCount,
                                                                                              VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR * pProperties,
                                                                                              Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetDisplayModeProperties2KHR(
      m_physicalDevice, static_cast<VkDisplayKHR>( display ), pPropertyCount, reinterpret_cast<VkDisplayModeProperties2KHR *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename DisplayModeProperties2KHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR, DisplayModeProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayModeProperties2KHR( VULKAN_HPP_NAMESPACE::DisplayKHR display, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR, DisplayModeProperties2KHRAllocator> properties;
    uint32_t                                                                                         propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetDisplayModeProperties2KHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayModeProperties2KHR(
          m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModeProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayModeProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }

  template <typename DisplayModeProperties2KHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR, DisplayModeProperties2KHRAllocator>>::type
    PhysicalDevice::getDisplayModeProperties2KHR( VULKAN_HPP_NAMESPACE::DisplayKHR     display,
                                                  DisplayModeProperties2KHRAllocator & displayModeProperties2KHRAllocator,
                                                  Dispatch const &                     d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::DisplayModeProperties2KHR, DisplayModeProperties2KHRAllocator> properties( displayModeProperties2KHRAllocator );
    uint32_t                                                                                         propertyCount;
    VULKAN_HPP_NAMESPACE::Result                                                                     result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
        d.vkGetDisplayModeProperties2KHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayModeProperties2KHR(
          m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModeProperties2KHR *>( properties.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayModeProperties2KHR" );
    VULKAN_HPP_ASSERT( propertyCount <= properties.size() );
    if ( propertyCount < properties.size() )
    {
      properties.resize( propertyCount );
    }
    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getDisplayPlaneCapabilities2KHR( const VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR *   pDisplayPlaneInfo,
                                                     VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR * pCapabilities,
                                                     Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetDisplayPlaneCapabilities2KHR( m_physicalDevice,
                                                                     reinterpret_cast<const VkDisplayPlaneInfo2KHR *>( pDisplayPlaneInfo ),
                                                                     reinterpret_cast<VkDisplayPlaneCapabilities2KHR *>( pCapabilities ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR>::type
    PhysicalDevice::getDisplayPlaneCapabilities2KHR( const VULKAN_HPP_NAMESPACE::DisplayPlaneInfo2KHR & displayPlaneInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DisplayPlaneCapabilities2KHR capabilities;
    VULKAN_HPP_NAMESPACE::Result                       result =
      static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetDisplayPlaneCapabilities2KHR( m_physicalDevice,
                                                                                      reinterpret_cast<const VkDisplayPlaneInfo2KHR *>( &displayPlaneInfo ),
                                                                                      reinterpret_cast<VkDisplayPlaneCapabilities2KHR *>( &capabilities ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getDisplayPlaneCapabilities2KHR" );

    return createResultValueType( result, capabilities );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_debug_utils ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::setDebugUtilsObjectNameEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT * pNameInfo,
                                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkSetDebugUtilsObjectNameEXT( m_device, reinterpret_cast<const VkDebugUtilsObjectNameInfoEXT *>( pNameInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::setDebugUtilsObjectNameEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT & nameInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkSetDebugUtilsObjectNameEXT( m_device, reinterpret_cast<const VkDebugUtilsObjectNameInfoEXT *>( &nameInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setDebugUtilsObjectNameEXT" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::setDebugUtilsObjectTagEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT * pTagInfo,
                                                                                   Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkSetDebugUtilsObjectTagEXT( m_device, reinterpret_cast<const VkDebugUtilsObjectTagInfoEXT *>( pTagInfo ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::setDebugUtilsObjectTagEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT & tagInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkSetDebugUtilsObjectTagEXT( m_device, reinterpret_cast<const VkDebugUtilsObjectTagInfoEXT *>( &tagInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::setDebugUtilsObjectTagEXT" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT * pLabelInfo,
                                                         Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkQueueBeginDebugUtilsLabelEXT( m_queue, reinterpret_cast<const VkDebugUtilsLabelEXT *>( pLabelInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo,
                                                         Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkQueueBeginDebugUtilsLabelEXT( m_queue, reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::endDebugUtilsLabelEXT( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkQueueEndDebugUtilsLabelEXT( m_queue );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT * pLabelInfo,
                                                          Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkQueueInsertDebugUtilsLabelEXT( m_queue, reinterpret_cast<const VkDebugUtilsLabelEXT *>( pLabelInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo,
                                                          Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkQueueInsertDebugUtilsLabelEXT( m_queue, reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT * pLabelInfo,
                                                                 Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBeginDebugUtilsLabelEXT( m_commandBuffer, reinterpret_cast<const VkDebugUtilsLabelEXT *>( pLabelInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::beginDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo,
                                                                 Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBeginDebugUtilsLabelEXT( m_commandBuffer, reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::endDebugUtilsLabelEXT( Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdEndDebugUtilsLabelEXT( m_commandBuffer );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT * pLabelInfo,
                                                                  Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdInsertDebugUtilsLabelEXT( m_commandBuffer, reinterpret_cast<const VkDebugUtilsLabelEXT *>( pLabelInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::insertDebugUtilsLabelEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsLabelEXT & labelInfo,
                                                                  Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdInsertDebugUtilsLabelEXT( m_commandBuffer, reinterpret_cast<const VkDebugUtilsLabelEXT *>( &labelInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    Instance::createDebugUtilsMessengerEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                            const VULKAN_HPP_NAMESPACE::AllocationCallbacks *              pAllocator,
                                            VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT *                 pMessenger,
                                            Dispatch const &                                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateDebugUtilsMessengerEXT( m_instance,
                                                                  reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>( pCreateInfo ),
                                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                                  reinterpret_cast<VkDebugUtilsMessengerEXT *>( pMessenger ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT>::type
                       Instance::createDebugUtilsMessengerEXT( const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT & createInfo,
                                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                            Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT messenger;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDebugUtilsMessengerEXT(
      m_instance,
      reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkDebugUtilsMessengerEXT *>( &messenger ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createDebugUtilsMessengerEXT" );

    return createResultValueType( result, messenger );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT, Dispatch>>::type
                       Instance::createDebugUtilsMessengerEXTUnique( const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateInfoEXT & createInfo,
                                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                                  Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT messenger;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateDebugUtilsMessengerEXT(
      m_instance,
      reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkDebugUtilsMessengerEXT *>( &messenger ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createDebugUtilsMessengerEXTUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT, Dispatch>( messenger, ObjectDestroy<Instance, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroyDebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT      messenger,
                                                                  const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                                                  Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyDebugUtilsMessengerEXT(
      m_instance, static_cast<VkDebugUtilsMessengerEXT>( messenger ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroyDebugUtilsMessengerEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT              messenger,
                                                                  Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                                                  Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyDebugUtilsMessengerEXT(
      m_instance,
      static_cast<VkDebugUtilsMessengerEXT>( messenger ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT      messenger,
                                            const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
                                            Dispatch const &                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkDestroyDebugUtilsMessengerEXT(
      m_instance, static_cast<VkDebugUtilsMessengerEXT>( messenger ), reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::destroy( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerEXT              messenger,
                                            Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator,
                                            Dispatch const &                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkDestroyDebugUtilsMessengerEXT(
      m_instance,
      static_cast<VkDebugUtilsMessengerEXT>( messenger ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::submitDebugUtilsMessageEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                               VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                               const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
                                                               Dispatch const &                                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkSubmitDebugUtilsMessageEXT( m_instance,
                                    static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ),
                                    static_cast<VkDebugUtilsMessageTypeFlagsEXT>( messageTypes ),
                                    reinterpret_cast<const VkDebugUtilsMessengerCallbackDataEXT *>( pCallbackData ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void Instance::submitDebugUtilsMessageEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                               VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                               const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT & callbackData,
                                                               Dispatch const &                                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkSubmitDebugUtilsMessageEXT( m_instance,
                                    static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ),
                                    static_cast<VkDebugUtilsMessageTypeFlagsEXT>( messageTypes ),
                                    reinterpret_cast<const VkDebugUtilsMessengerCallbackDataEXT *>( &callbackData ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_sample_locations ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setSampleLocationsEXT( const VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT * pSampleLocationsInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetSampleLocationsEXT( m_commandBuffer, reinterpret_cast<const VkSampleLocationsInfoEXT *>( pSampleLocationsInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setSampleLocationsEXT( const VULKAN_HPP_NAMESPACE::SampleLocationsInfoEXT & sampleLocationsInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetSampleLocationsEXT( m_commandBuffer, reinterpret_cast<const VkSampleLocationsInfoEXT *>( &sampleLocationsInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void PhysicalDevice::getMultisamplePropertiesEXT( VULKAN_HPP_NAMESPACE::SampleCountFlagBits        samples,
                                                                      VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT * pMultisampleProperties,
                                                                      Dispatch const &                                 d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetPhysicalDeviceMultisamplePropertiesEXT(
      m_physicalDevice, static_cast<VkSampleCountFlagBits>( samples ), reinterpret_cast<VkMultisamplePropertiesEXT *>( pMultisampleProperties ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT
    PhysicalDevice::getMultisamplePropertiesEXT( VULKAN_HPP_NAMESPACE::SampleCountFlagBits samples, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MultisamplePropertiesEXT multisampleProperties;
    d.vkGetPhysicalDeviceMultisamplePropertiesEXT(
      m_physicalDevice, static_cast<VkSampleCountFlagBits>( samples ), reinterpret_cast<VkMultisamplePropertiesEXT *>( &multisampleProperties ) );

    return multisampleProperties;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_image_drm_format_modifier ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getImageDrmFormatModifierPropertiesEXT(
    VULKAN_HPP_NAMESPACE::Image image, VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT * pProperties, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetImageDrmFormatModifierPropertiesEXT(
      m_device, static_cast<VkImage>( image ), reinterpret_cast<VkImageDrmFormatModifierPropertiesEXT *>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT>::type
                       Device::getImageDrmFormatModifierPropertiesEXT( VULKAN_HPP_NAMESPACE::Image image, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ImageDrmFormatModifierPropertiesEXT properties;
    VULKAN_HPP_NAMESPACE::Result                              result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetImageDrmFormatModifierPropertiesEXT(
      m_device, static_cast<VkImage>( image ), reinterpret_cast<VkImageDrmFormatModifierPropertiesEXT *>( &properties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getImageDrmFormatModifierPropertiesEXT" );

    return createResultValueType( result, properties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_external_memory_host ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    Device::getMemoryHostPointerPropertiesEXT( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType,
                                               const void *                                           pHostPointer,
                                               VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT * pMemoryHostPointerProperties,
                                               Dispatch const &                                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetMemoryHostPointerPropertiesEXT( m_device,
                                                                       static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                                       pHostPointer,
                                                                       reinterpret_cast<VkMemoryHostPointerPropertiesEXT *>( pMemoryHostPointerProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT>::type
                       Device::getMemoryHostPointerPropertiesEXT( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType,
                                               const void *                                           pHostPointer,
                                               Dispatch const &                                       d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemoryHostPointerPropertiesEXT memoryHostPointerProperties;
    VULKAN_HPP_NAMESPACE::Result                         result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetMemoryHostPointerPropertiesEXT( m_device,
                                             static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                             pHostPointer,
                                             reinterpret_cast<VkMemoryHostPointerPropertiesEXT *>( &memoryHostPointerProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemoryHostPointerPropertiesEXT" );

    return createResultValueType( result, memoryHostPointerProperties );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_calibrated_timestamps ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getCalibrateableTimeDomainsEXT( uint32_t *                            pTimeDomainCount,
                                                                                                VULKAN_HPP_NAMESPACE::TimeDomainEXT * pTimeDomains,
                                                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( m_physicalDevice, pTimeDomainCount, reinterpret_cast<VkTimeDomainEXT *>( pTimeDomains ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename TimeDomainEXTAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::TimeDomainEXT, TimeDomainEXTAllocator>>::type
                       PhysicalDevice::getCalibrateableTimeDomainsEXT( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::TimeDomainEXT, TimeDomainEXTAllocator> timeDomains;
    uint32_t                                                                 timeDomainCount;
    VULKAN_HPP_NAMESPACE::Result                                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( m_physicalDevice, &timeDomainCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && timeDomainCount )
      {
        timeDomains.resize( timeDomainCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( m_physicalDevice, &timeDomainCount, reinterpret_cast<VkTimeDomainEXT *>( timeDomains.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getCalibrateableTimeDomainsEXT" );
    VULKAN_HPP_ASSERT( timeDomainCount <= timeDomains.size() );
    if ( timeDomainCount < timeDomains.size() )
    {
      timeDomains.resize( timeDomainCount );
    }
    return createResultValueType( result, timeDomains );
  }

  template <typename TimeDomainEXTAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::TimeDomainEXT>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::TimeDomainEXT, TimeDomainEXTAllocator>>::type
                       PhysicalDevice::getCalibrateableTimeDomainsEXT( TimeDomainEXTAllocator & timeDomainEXTAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::TimeDomainEXT, TimeDomainEXTAllocator> timeDomains( timeDomainEXTAllocator );
    uint32_t                                                                 timeDomainCount;
    VULKAN_HPP_NAMESPACE::Result                                             result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( m_physicalDevice, &timeDomainCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && timeDomainCount )
      {
        timeDomains.resize( timeDomainCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
          d.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( m_physicalDevice, &timeDomainCount, reinterpret_cast<VkTimeDomainEXT *>( timeDomains.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getCalibrateableTimeDomainsEXT" );
    VULKAN_HPP_ASSERT( timeDomainCount <= timeDomains.size() );
    if ( timeDomainCount < timeDomains.size() )
    {
      timeDomains.resize( timeDomainCount );
    }
    return createResultValueType( result, timeDomains );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getCalibratedTimestampsEXT( uint32_t                                                 timestampCount,
                                                                                    const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoEXT * pTimestampInfos,
                                                                                    uint64_t *                                               pTimestamps,
                                                                                    uint64_t *                                               pMaxDeviation,
                                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetCalibratedTimestampsEXT(
      m_device, timestampCount, reinterpret_cast<const VkCalibratedTimestampInfoEXT *>( pTimestampInfos ), pTimestamps, pMaxDeviation ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Uint64_tAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::pair<std::vector<uint64_t, Uint64_tAllocator>, uint64_t>>::type
    Device::getCalibratedTimestampsEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoEXT> const & timestampInfos,
                                        Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<std::vector<uint64_t, Uint64_tAllocator>, uint64_t> data_(
      std::piecewise_construct, std::forward_as_tuple( timestampInfos.size() ), std::forward_as_tuple( 0 ) );
    std::vector<uint64_t, Uint64_tAllocator> & timestamps   = data_.first;
    uint64_t &                                 maxDeviation = data_.second;
    VULKAN_HPP_NAMESPACE::Result               result       = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetCalibratedTimestampsEXT(
      m_device, timestampInfos.size(), reinterpret_cast<const VkCalibratedTimestampInfoEXT *>( timestampInfos.data() ), timestamps.data(), &maxDeviation ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getCalibratedTimestampsEXT" );

    return createResultValueType( result, data_ );
  }

  template <typename Uint64_tAllocator,
            typename Dispatch,
            typename B0,
            typename std::enable_if<std::is_same<typename B0::value_type, uint64_t>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::pair<std::vector<uint64_t, Uint64_tAllocator>, uint64_t>>::type
    Device::getCalibratedTimestampsEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoEXT> const & timestampInfos,
                                        Uint64_tAllocator &                                                                              uint64_tAllocator,
                                        Dispatch const &                                                                                 d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<std::vector<uint64_t, Uint64_tAllocator>, uint64_t> data_(
      std::piecewise_construct, std::forward_as_tuple( timestampInfos.size(), uint64_tAllocator ), std::forward_as_tuple( 0 ) );
    std::vector<uint64_t, Uint64_tAllocator> & timestamps   = data_.first;
    uint64_t &                                 maxDeviation = data_.second;
    VULKAN_HPP_NAMESPACE::Result               result       = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetCalibratedTimestampsEXT(
      m_device, timestampInfos.size(), reinterpret_cast<const VkCalibratedTimestampInfoEXT *>( timestampInfos.data() ), timestamps.data(), &maxDeviation ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getCalibratedTimestampsEXT" );

    return createResultValueType( result, data_ );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::pair<uint64_t, uint64_t>>::type
                       Device::getCalibratedTimestampEXT( const VULKAN_HPP_NAMESPACE::CalibratedTimestampInfoEXT & timestampInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::pair<uint64_t, uint64_t> data_;
    uint64_t &                    timestamp    = data_.first;
    uint64_t &                    maxDeviation = data_.second;
    VULKAN_HPP_NAMESPACE::Result  result       = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetCalibratedTimestampsEXT( m_device, 1, reinterpret_cast<const VkCalibratedTimestampInfoEXT *>( &timestampInfo ), &timestamp, &maxDeviation ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getCalibratedTimestampEXT" );

    return createResultValueType( result, data_ );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_fragment_shading_rate ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getFragmentShadingRatesKHR( uint32_t *                                                   pFragmentShadingRateCount,
                                                VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR * pFragmentShadingRates,
                                                Dispatch const &                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceFragmentShadingRatesKHR(
      m_physicalDevice, pFragmentShadingRateCount, reinterpret_cast<VkPhysicalDeviceFragmentShadingRateKHR *>( pFragmentShadingRates ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename PhysicalDeviceFragmentShadingRateKHRAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR, PhysicalDeviceFragmentShadingRateKHRAllocator>>::type
    PhysicalDevice::getFragmentShadingRatesKHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR, PhysicalDeviceFragmentShadingRateKHRAllocator> fragmentShadingRates;
    uint32_t                                                                                                               fragmentShadingRateCount;
    VULKAN_HPP_NAMESPACE::Result                                                                                           result;
    do
    {
      result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceFragmentShadingRatesKHR( m_physicalDevice, &fragmentShadingRateCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && fragmentShadingRateCount )
      {
        fragmentShadingRates.resize( fragmentShadingRateCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceFragmentShadingRatesKHR(
          m_physicalDevice, &fragmentShadingRateCount, reinterpret_cast<VkPhysicalDeviceFragmentShadingRateKHR *>( fragmentShadingRates.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getFragmentShadingRatesKHR" );
    VULKAN_HPP_ASSERT( fragmentShadingRateCount <= fragmentShadingRates.size() );
    if ( fragmentShadingRateCount < fragmentShadingRates.size() )
    {
      fragmentShadingRates.resize( fragmentShadingRateCount );
    }
    return createResultValueType( result, fragmentShadingRates );
  }

  template <typename PhysicalDeviceFragmentShadingRateKHRAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE
    typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR, PhysicalDeviceFragmentShadingRateKHRAllocator>>::type
    PhysicalDevice::getFragmentShadingRatesKHR( PhysicalDeviceFragmentShadingRateKHRAllocator & physicalDeviceFragmentShadingRateKHRAllocator,
                                                Dispatch const &                                d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::PhysicalDeviceFragmentShadingRateKHR, PhysicalDeviceFragmentShadingRateKHRAllocator> fragmentShadingRates(
      physicalDeviceFragmentShadingRateKHRAllocator );
    uint32_t                     fragmentShadingRateCount;
    VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceFragmentShadingRatesKHR( m_physicalDevice, &fragmentShadingRateCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && fragmentShadingRateCount )
      {
        fragmentShadingRates.resize( fragmentShadingRateCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceFragmentShadingRatesKHR(
          m_physicalDevice, &fragmentShadingRateCount, reinterpret_cast<VkPhysicalDeviceFragmentShadingRateKHR *>( fragmentShadingRates.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getFragmentShadingRatesKHR" );
    VULKAN_HPP_ASSERT( fragmentShadingRateCount <= fragmentShadingRates.size() );
    if ( fragmentShadingRateCount < fragmentShadingRates.size() )
    {
      fragmentShadingRates.resize( fragmentShadingRateCount );
    }
    return createResultValueType( result, fragmentShadingRates );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setFragmentShadingRateKHR( const VULKAN_HPP_NAMESPACE::Extent2D *                       pFragmentSize,
                                                                   const VULKAN_HPP_NAMESPACE::FragmentShadingRateCombinerOpKHR combinerOps[2],
                                                                   Dispatch const &                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetFragmentShadingRateKHR(
      m_commandBuffer, reinterpret_cast<const VkExtent2D *>( pFragmentSize ), reinterpret_cast<const VkFragmentShadingRateCombinerOpKHR *>( combinerOps ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setFragmentShadingRateKHR( const VULKAN_HPP_NAMESPACE::Extent2D &                       fragmentSize,
                                                                   const VULKAN_HPP_NAMESPACE::FragmentShadingRateCombinerOpKHR combinerOps[2],
                                                                   Dispatch const &                                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetFragmentShadingRateKHR(
      m_commandBuffer, reinterpret_cast<const VkExtent2D *>( &fragmentSize ), reinterpret_cast<const VkFragmentShadingRateCombinerOpKHR *>( combinerOps ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_headless_surface ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Instance::createHeadlessSurfaceEXT( const VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT * pCreateInfo,
                                                                                    const VULKAN_HPP_NAMESPACE::AllocationCallbacks *          pAllocator,
                                                                                    VULKAN_HPP_NAMESPACE::SurfaceKHR *                         pSurface,
                                                                                    Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateHeadlessSurfaceEXT( m_instance,
                                                              reinterpret_cast<const VkHeadlessSurfaceCreateInfoEXT *>( pCreateInfo ),
                                                              reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                              reinterpret_cast<VkSurfaceKHR *>( pSurface ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SurfaceKHR>::type
                       Instance::createHeadlessSurfaceEXT( const VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT & createInfo,
                                        Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>  allocator,
                                        Dispatch const &                                           d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateHeadlessSurfaceEXT(
      m_instance,
      reinterpret_cast<const VkHeadlessSurfaceCreateInfoEXT *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createHeadlessSurfaceEXT" );

    return createResultValueType( result, surface );
  }

#  ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR, Dispatch>>::type
                       Instance::createHeadlessSurfaceEXTUnique( const VULKAN_HPP_NAMESPACE::HeadlessSurfaceCreateInfoEXT & createInfo,
                                              Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>  allocator,
                                              Dispatch const &                                           d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SurfaceKHR surface;
    VULKAN_HPP_NAMESPACE::Result     result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateHeadlessSurfaceEXT(
      m_instance,
      reinterpret_cast<const VkHeadlessSurfaceCreateInfoEXT *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSurfaceKHR *>( &surface ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Instance::createHeadlessSurfaceEXTUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR, Dispatch>( surface, ObjectDestroy<Instance, Dispatch>( *this, allocator, d ) ) );
  }
#  endif /* VULKAN_HPP_NO_SMART_HANDLE */
#endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_line_rasterization ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setLineStippleEXT( uint32_t lineStippleFactor, uint16_t lineStipplePattern, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetLineStippleEXT( m_commandBuffer, lineStippleFactor, lineStipplePattern );
  }

  //=== VK_EXT_extended_dynamic_state ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setCullModeEXT( VULKAN_HPP_NAMESPACE::CullModeFlags cullMode, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetCullModeEXT( m_commandBuffer, static_cast<VkCullModeFlags>( cullMode ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setFrontFaceEXT( VULKAN_HPP_NAMESPACE::FrontFace frontFace, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetFrontFaceEXT( m_commandBuffer, static_cast<VkFrontFace>( frontFace ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveTopologyEXT( VULKAN_HPP_NAMESPACE::PrimitiveTopology primitiveTopology,
                                                                 Dispatch const &                        d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetPrimitiveTopologyEXT( m_commandBuffer, static_cast<VkPrimitiveTopology>( primitiveTopology ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewportWithCountEXT( uint32_t                               viewportCount,
                                                                 const VULKAN_HPP_NAMESPACE::Viewport * pViewports,
                                                                 Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetViewportWithCountEXT( m_commandBuffer, viewportCount, reinterpret_cast<const VkViewport *>( pViewports ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setViewportWithCountEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Viewport> const & viewports,
                                                                 Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetViewportWithCountEXT( m_commandBuffer, viewports.size(), reinterpret_cast<const VkViewport *>( viewports.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void
    CommandBuffer::setScissorWithCountEXT( uint32_t scissorCount, const VULKAN_HPP_NAMESPACE::Rect2D * pScissors, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetScissorWithCountEXT( m_commandBuffer, scissorCount, reinterpret_cast<const VkRect2D *>( pScissors ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setScissorWithCountEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Rect2D> const & scissors,
                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetScissorWithCountEXT( m_commandBuffer, scissors.size(), reinterpret_cast<const VkRect2D *>( scissors.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers2EXT( uint32_t                                 firstBinding,
                                                               uint32_t                                 bindingCount,
                                                               const VULKAN_HPP_NAMESPACE::Buffer *     pBuffers,
                                                               const VULKAN_HPP_NAMESPACE::DeviceSize * pOffsets,
                                                               const VULKAN_HPP_NAMESPACE::DeviceSize * pSizes,
                                                               const VULKAN_HPP_NAMESPACE::DeviceSize * pStrides,
                                                               Dispatch const &                         d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBindVertexBuffers2EXT( m_commandBuffer,
                                  firstBinding,
                                  bindingCount,
                                  reinterpret_cast<const VkBuffer *>( pBuffers ),
                                  reinterpret_cast<const VkDeviceSize *>( pOffsets ),
                                  reinterpret_cast<const VkDeviceSize *>( pSizes ),
                                  reinterpret_cast<const VkDeviceSize *>( pStrides ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::bindVertexBuffers2EXT( uint32_t                                                                         firstBinding,
                                                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Buffer> const &     buffers,
                                                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & offsets,
                                                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & sizes,
                                                               VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DeviceSize> const & strides,
                                                               Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( buffers.size() == offsets.size() );
    VULKAN_HPP_ASSERT( sizes.empty() || buffers.size() == sizes.size() );
    VULKAN_HPP_ASSERT( strides.empty() || buffers.size() == strides.size() );
#  else
    if ( buffers.size() != offsets.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != offsets.size()" );
    }
    if ( !sizes.empty() && buffers.size() != sizes.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != sizes.size()" );
    }
    if ( !strides.empty() && buffers.size() != strides.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::bindVertexBuffers2EXT: buffers.size() != strides.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkCmdBindVertexBuffers2EXT( m_commandBuffer,
                                  firstBinding,
                                  buffers.size(),
                                  reinterpret_cast<const VkBuffer *>( buffers.data() ),
                                  reinterpret_cast<const VkDeviceSize *>( offsets.data() ),
                                  reinterpret_cast<const VkDeviceSize *>( sizes.data() ),
                                  reinterpret_cast<const VkDeviceSize *>( strides.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthTestEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthTestEnableEXT( m_commandBuffer, static_cast<VkBool32>( depthTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthWriteEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthWriteEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthWriteEnableEXT( m_commandBuffer, static_cast<VkBool32>( depthWriteEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthCompareOpEXT( VULKAN_HPP_NAMESPACE::CompareOp depthCompareOp, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthCompareOpEXT( m_commandBuffer, static_cast<VkCompareOp>( depthCompareOp ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthBoundsTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBoundsTestEnable,
                                                                     Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBoundsTestEnableEXT( m_commandBuffer, static_cast<VkBool32>( depthBoundsTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setStencilTestEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 stencilTestEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilTestEnableEXT( m_commandBuffer, static_cast<VkBool32>( stencilTestEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setStencilOpEXT( VULKAN_HPP_NAMESPACE::StencilFaceFlags faceMask,
                                                         VULKAN_HPP_NAMESPACE::StencilOp        failOp,
                                                         VULKAN_HPP_NAMESPACE::StencilOp        passOp,
                                                         VULKAN_HPP_NAMESPACE::StencilOp        depthFailOp,
                                                         VULKAN_HPP_NAMESPACE::CompareOp        compareOp,
                                                         Dispatch const &                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetStencilOpEXT( m_commandBuffer,
                            static_cast<VkStencilFaceFlags>( faceMask ),
                            static_cast<VkStencilOp>( failOp ),
                            static_cast<VkStencilOp>( passOp ),
                            static_cast<VkStencilOp>( depthFailOp ),
                            static_cast<VkCompareOp>( compareOp ) );
  }

  //=== VK_KHR_object_refresh ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::refreshObjectsKHR( const VULKAN_HPP_NAMESPACE::RefreshObjectListKHR * pRefreshObjects,
                                                           Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdRefreshObjectsKHR( m_commandBuffer, reinterpret_cast<const VkRefreshObjectListKHR *>( pRefreshObjects ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::refreshObjectsKHR( const VULKAN_HPP_NAMESPACE::RefreshObjectListKHR & refreshObjects,
                                                           Dispatch const &                                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdRefreshObjectsKHR( m_commandBuffer, reinterpret_cast<const VkRefreshObjectListKHR *>( &refreshObjects ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getRefreshableObjectTypesKHR( uint32_t *                         pRefreshableObjectTypeCount,
                                                                                              VULKAN_HPP_NAMESPACE::ObjectType * pRefreshableObjectTypes,
                                                                                              Dispatch const &                   d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceRefreshableObjectTypesKHR(
      m_physicalDevice, pRefreshableObjectTypeCount, reinterpret_cast<VkObjectType *>( pRefreshableObjectTypes ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename ObjectTypeAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ObjectType, ObjectTypeAllocator>>::type
                       PhysicalDevice::getRefreshableObjectTypesKHR( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ObjectType, ObjectTypeAllocator> refreshableObjectTypes;
    uint32_t                                                           refreshableObjectTypeCount;
    VULKAN_HPP_NAMESPACE::Result                                       result;
    do
    {
      result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceRefreshableObjectTypesKHR( m_physicalDevice, &refreshableObjectTypeCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && refreshableObjectTypeCount )
      {
        refreshableObjectTypes.resize( refreshableObjectTypeCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceRefreshableObjectTypesKHR(
          m_physicalDevice, &refreshableObjectTypeCount, reinterpret_cast<VkObjectType *>( refreshableObjectTypes.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );

    VULKAN_HPP_ASSERT( refreshableObjectTypeCount <= refreshableObjectTypes.size() );
    if ( refreshableObjectTypeCount < refreshableObjectTypes.size() )
    {
      refreshableObjectTypes.resize( refreshableObjectTypeCount );
    }
    return refreshableObjectTypes;
  }

  template <typename ObjectTypeAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::ObjectType>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<std::vector<VULKAN_HPP_NAMESPACE::ObjectType, ObjectTypeAllocator>>::type
                       PhysicalDevice::getRefreshableObjectTypesKHR( ObjectTypeAllocator & objectTypeAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::ObjectType, ObjectTypeAllocator> refreshableObjectTypes( objectTypeAllocator );
    uint32_t                                                           refreshableObjectTypeCount;
    VULKAN_HPP_NAMESPACE::Result                                       result;
    do
    {
      result =
        static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceRefreshableObjectTypesKHR( m_physicalDevice, &refreshableObjectTypeCount, nullptr ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && refreshableObjectTypeCount )
      {
        refreshableObjectTypes.resize( refreshableObjectTypeCount );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceRefreshableObjectTypesKHR(
          m_physicalDevice, &refreshableObjectTypeCount, reinterpret_cast<VkObjectType *>( refreshableObjectTypes.data() ) ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );

    VULKAN_HPP_ASSERT( refreshableObjectTypeCount <= refreshableObjectTypes.size() );
    if ( refreshableObjectTypeCount < refreshableObjectTypes.size() )
    {
      refreshableObjectTypes.resize( refreshableObjectTypeCount );
    }
    return refreshableObjectTypes;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_synchronization2 ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setEvent2KHR( VULKAN_HPP_NAMESPACE::Event                  event,
                                                      const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfo,
                                                      Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetEvent2KHR( m_commandBuffer, static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( pDependencyInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setEvent2KHR( VULKAN_HPP_NAMESPACE::Event                  event,
                                                      const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo,
                                                      Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetEvent2KHR( m_commandBuffer, static_cast<VkEvent>( event ), reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resetEvent2KHR( VULKAN_HPP_NAMESPACE::Event               event,
                                                        VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stageMask,
                                                        Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResetEvent2KHR( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags2>( stageMask ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::waitEvents2KHR( uint32_t                                     eventCount,
                                                        const VULKAN_HPP_NAMESPACE::Event *          pEvents,
                                                        const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfos,
                                                        Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWaitEvents2KHR(
      m_commandBuffer, eventCount, reinterpret_cast<const VkEvent *>( pEvents ), reinterpret_cast<const VkDependencyInfo *>( pDependencyInfos ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::waitEvents2KHR( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Event> const &          events,
                                                        VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::DependencyInfo> const & dependencyInfos,
                                                        Dispatch const & d ) const VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( events.size() == dependencyInfos.size() );
#  else
    if ( events.size() != dependencyInfos.size() )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::CommandBuffer::waitEvents2KHR: events.size() != dependencyInfos.size()" );
    }
#  endif /*VULKAN_HPP_NO_EXCEPTIONS*/

    d.vkCmdWaitEvents2KHR( m_commandBuffer,
                           events.size(),
                           reinterpret_cast<const VkEvent *>( events.data() ),
                           reinterpret_cast<const VkDependencyInfo *>( dependencyInfos.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2KHR( const VULKAN_HPP_NAMESPACE::DependencyInfo * pDependencyInfo,
                                                             Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdPipelineBarrier2KHR( m_commandBuffer, reinterpret_cast<const VkDependencyInfo *>( pDependencyInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::pipelineBarrier2KHR( const VULKAN_HPP_NAMESPACE::DependencyInfo & dependencyInfo,
                                                             Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdPipelineBarrier2KHR( m_commandBuffer, reinterpret_cast<const VkDependencyInfo *>( &dependencyInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::writeTimestamp2KHR( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                            VULKAN_HPP_NAMESPACE::QueryPool           queryPool,
                                                            uint32_t                                  query,
                                                            Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWriteTimestamp2KHR( m_commandBuffer, static_cast<VkPipelineStageFlags2>( stage ), static_cast<VkQueryPool>( queryPool ), query );
  }

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Queue::submit2KHR( uint32_t                                  submitCount,
                                                                   const VULKAN_HPP_NAMESPACE::SubmitInfo2 * pSubmits,
                                                                   VULKAN_HPP_NAMESPACE::Fence               fence,
                                                                   Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkQueueSubmit2KHR( m_queue, submitCount, reinterpret_cast<const VkSubmitInfo2 *>( pSubmits ), static_cast<VkFence>( fence ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type Queue::submit2KHR(
    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::SubmitInfo2> const & submits, VULKAN_HPP_NAMESPACE::Fence fence, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkQueueSubmit2KHR( m_queue, submits.size(), reinterpret_cast<const VkSubmitInfo2 *>( submits.data() ), static_cast<VkFence>( fence ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Queue::submit2KHR" );

    return createResultValueType( result );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::writeBufferMarker2AMD( VULKAN_HPP_NAMESPACE::PipelineStageFlags2 stage,
                                                               VULKAN_HPP_NAMESPACE::Buffer              dstBuffer,
                                                               VULKAN_HPP_NAMESPACE::DeviceSize          dstOffset,
                                                               uint32_t                                  marker,
                                                               Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdWriteBufferMarker2AMD(
      m_commandBuffer, static_cast<VkPipelineStageFlags2>( stage ), static_cast<VkBuffer>( dstBuffer ), static_cast<VkDeviceSize>( dstOffset ), marker );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void Queue::getCheckpointData2NV( uint32_t *                                pCheckpointDataCount,
                                                      VULKAN_HPP_NAMESPACE::CheckpointData2NV * pCheckpointData,
                                                      Dispatch const &                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkGetQueueCheckpointData2NV( m_queue, pCheckpointDataCount, reinterpret_cast<VkCheckpointData2NV *>( pCheckpointData ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename CheckpointData2NVAllocator, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV, CheckpointData2NVAllocator>
                                         Queue::getCheckpointData2NV( Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV, CheckpointData2NVAllocator> checkpointData;
    uint32_t                                                                         checkpointDataCount;
    d.vkGetQueueCheckpointData2NV( m_queue, &checkpointDataCount, nullptr );
    checkpointData.resize( checkpointDataCount );
    d.vkGetQueueCheckpointData2NV( m_queue, &checkpointDataCount, reinterpret_cast<VkCheckpointData2NV *>( checkpointData.data() ) );

    VULKAN_HPP_ASSERT( checkpointDataCount <= checkpointData.size() );
    if ( checkpointDataCount < checkpointData.size() )
    {
      checkpointData.resize( checkpointDataCount );
    }
    return checkpointData;
  }

  template <typename CheckpointData2NVAllocator,
            typename Dispatch,
            typename B1,
            typename std::enable_if<std::is_same<typename B1::value_type, VULKAN_HPP_NAMESPACE::CheckpointData2NV>::value, int>::type>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV, CheckpointData2NVAllocator>
                                         Queue::getCheckpointData2NV( CheckpointData2NVAllocator & checkpointData2NVAllocator, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    std::vector<VULKAN_HPP_NAMESPACE::CheckpointData2NV, CheckpointData2NVAllocator> checkpointData( checkpointData2NVAllocator );
    uint32_t                                                                         checkpointDataCount;
    d.vkGetQueueCheckpointData2NV( m_queue, &checkpointDataCount, nullptr );
    checkpointData.resize( checkpointDataCount );
    d.vkGetQueueCheckpointData2NV( m_queue, &checkpointDataCount, reinterpret_cast<VkCheckpointData2NV *>( checkpointData.data() ) );

    VULKAN_HPP_ASSERT( checkpointDataCount <= checkpointData.size() );
    if ( checkpointDataCount < checkpointData.size() )
    {
      checkpointData.resize( checkpointDataCount );
    }
    return checkpointData;
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_KHR_copy_commands2 ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 * pCopyBufferInfo,
                                                        Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBuffer2KHR( m_commandBuffer, reinterpret_cast<const VkCopyBufferInfo2 *>( pCopyBufferInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferInfo2 & copyBufferInfo,
                                                        Dispatch const &                              d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBuffer2KHR( m_commandBuffer, reinterpret_cast<const VkCopyBufferInfo2 *>( &copyBufferInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage2KHR( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 * pCopyImageInfo,
                                                       Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImage2KHR( m_commandBuffer, reinterpret_cast<const VkCopyImageInfo2 *>( pCopyImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImage2KHR( const VULKAN_HPP_NAMESPACE::CopyImageInfo2 & copyImageInfo,
                                                       Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImage2KHR( m_commandBuffer, reinterpret_cast<const VkCopyImageInfo2 *>( &copyImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 * pCopyBufferToImageInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyBufferToImage2KHR( m_commandBuffer, reinterpret_cast<const VkCopyBufferToImageInfo2 *>( pCopyBufferToImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyBufferToImage2KHR( const VULKAN_HPP_NAMESPACE::CopyBufferToImageInfo2 & copyBufferToImageInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyBufferToImage2KHR( m_commandBuffer, reinterpret_cast<const VkCopyBufferToImageInfo2 *>( &copyBufferToImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 * pCopyImageToBufferInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdCopyImageToBuffer2KHR( m_commandBuffer, reinterpret_cast<const VkCopyImageToBufferInfo2 *>( pCopyImageToBufferInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::copyImageToBuffer2KHR( const VULKAN_HPP_NAMESPACE::CopyImageToBufferInfo2 & copyImageToBufferInfo,
                                                               Dispatch const &                                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdCopyImageToBuffer2KHR( m_commandBuffer, reinterpret_cast<const VkCopyImageToBufferInfo2 *>( &copyImageToBufferInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage2KHR( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 * pBlitImageInfo,
                                                       Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdBlitImage2KHR( m_commandBuffer, reinterpret_cast<const VkBlitImageInfo2 *>( pBlitImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::blitImage2KHR( const VULKAN_HPP_NAMESPACE::BlitImageInfo2 & blitImageInfo,
                                                       Dispatch const &                             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdBlitImage2KHR( m_commandBuffer, reinterpret_cast<const VkBlitImageInfo2 *>( &blitImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage2KHR( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 * pResolveImageInfo,
                                                          Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdResolveImage2KHR( m_commandBuffer, reinterpret_cast<const VkResolveImageInfo2 *>( pResolveImageInfo ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::resolveImage2KHR( const VULKAN_HPP_NAMESPACE::ResolveImageInfo2 & resolveImageInfo,
                                                          Dispatch const &                                d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdResolveImage2KHR( m_commandBuffer, reinterpret_cast<const VkResolveImageInfo2 *>( &resolveImageInfo ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  //=== VK_EXT_vertex_input_dynamic_state ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setVertexInputEXT( uint32_t                                                          vertexBindingDescriptionCount,
                                                           const VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT *   pVertexBindingDescriptions,
                                                           uint32_t                                                          vertexAttributeDescriptionCount,
                                                           const VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT * pVertexAttributeDescriptions,
                                                           Dispatch const &                                                  d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetVertexInputEXT( m_commandBuffer,
                              vertexBindingDescriptionCount,
                              reinterpret_cast<const VkVertexInputBindingDescription2EXT *>( pVertexBindingDescriptions ),
                              vertexAttributeDescriptionCount,
                              reinterpret_cast<const VkVertexInputAttributeDescription2EXT *>( pVertexAttributeDescriptions ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setVertexInputEXT(
    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputBindingDescription2EXT> const &   vertexBindingDescriptions,
    VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::VertexInputAttributeDescription2EXT> const & vertexAttributeDescriptions,
    Dispatch const &                                                                                          d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetVertexInputEXT( m_commandBuffer,
                              vertexBindingDescriptions.size(),
                              reinterpret_cast<const VkVertexInputBindingDescription2EXT *>( vertexBindingDescriptions.data() ),
                              vertexAttributeDescriptions.size(),
                              reinterpret_cast<const VkVertexInputAttributeDescription2EXT *>( vertexAttributeDescriptions.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV * pGetSciSyncHandleInfo,
                                                                                void *                                              pHandle,
                                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetFenceSciSyncFenceNV( m_device, reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( pGetSciSyncHandleInfo ), pHandle ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename HandleType, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<HandleType>::type
                       Device::getFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    HandleType                   handle;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetFenceSciSyncFenceNV( m_device, reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( &getSciSyncHandleInfo ), &handle ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceSciSyncFenceNV" );

    return createResultValueType( result, handle );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV * pGetSciSyncHandleInfo,
                                                                              void *                                              pHandle,
                                                                              Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetFenceSciSyncObjNV( m_device, reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( pGetSciSyncHandleInfo ), pHandle ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename HandleType, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<HandleType>::type
                       Device::getFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::FenceGetSciSyncInfoNV & getSciSyncHandleInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    HandleType                   handle;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetFenceSciSyncObjNV( m_device, reinterpret_cast<const VkFenceGetSciSyncInfoNV *>( &getSciSyncHandleInfo ), &handle ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getFenceSciSyncObjNV" );

    return createResultValueType( result, handle );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::importFenceSciSyncFenceNV(
    const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV * pImportFenceSciSyncInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkImportFenceSciSyncFenceNV( m_device, reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( pImportFenceSciSyncInfo ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::importFenceSciSyncFenceNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkImportFenceSciSyncFenceNV( m_device, reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( &importFenceSciSyncInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceSciSyncFenceNV" );

    return createResultValueType( result );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::importFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV * pImportFenceSciSyncInfo,
                                                                                 Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkImportFenceSciSyncObjNV( m_device, reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( pImportFenceSciSyncInfo ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::importFenceSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportFenceSciSyncInfoNV & importFenceSciSyncInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkImportFenceSciSyncObjNV( m_device, reinterpret_cast<const VkImportFenceSciSyncInfoNV *>( &importFenceSciSyncInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importFenceSciSyncObjNV" );

    return createResultValueType( result );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSciSyncAttributesNV(
    const VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV * pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSciSyncAttributesNV(
      m_physicalDevice, reinterpret_cast<const VkSciSyncAttributesInfoNV *>( pSciSyncAttributesInfo ), pAttributes ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE typename ResultValueType<void>::type PhysicalDevice::getSciSyncAttributesNV(
    const VULKAN_HPP_NAMESPACE::SciSyncAttributesInfoNV & sciSyncAttributesInfo, NvSciSyncAttrList pAttributes, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSciSyncAttributesNV(
      m_physicalDevice, reinterpret_cast<const VkSciSyncAttributesInfoNV *>( &sciSyncAttributesInfo ), pAttributes ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSciSyncAttributesNV" );

    return createResultValueType( result );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV * pGetSciSyncInfo,
                                                                                  void *                                                  pHandle,
                                                                                  Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetSemaphoreSciSyncObjNV( m_device, reinterpret_cast<const VkSemaphoreGetSciSyncInfoNV *>( pGetSciSyncInfo ), pHandle ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename HandleType, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<HandleType>::type
                       Device::getSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::SemaphoreGetSciSyncInfoNV & getSciSyncInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    HandleType                   handle;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetSemaphoreSciSyncObjNV( m_device, reinterpret_cast<const VkSemaphoreGetSciSyncInfoNV *>( &getSciSyncInfo ), &handle ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getSemaphoreSciSyncObjNV" );

    return createResultValueType( result, handle );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::importSemaphoreSciSyncObjNV(
    const VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV * pImportSemaphoreSciSyncInfo, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkImportSemaphoreSciSyncObjNV( m_device, reinterpret_cast<const VkImportSemaphoreSciSyncInfoNV *>( pImportSemaphoreSciSyncInfo ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::importSemaphoreSciSyncObjNV( const VULKAN_HPP_NAMESPACE::ImportSemaphoreSciSyncInfoNV & importSemaphoreSciSyncInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkImportSemaphoreSciSyncObjNV( m_device, reinterpret_cast<const VkImportSemaphoreSciSyncInfoNV *>( &importSemaphoreSciSyncInfo ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::importSemaphoreSciSyncObjNV" );

    return createResultValueType( result );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */
#endif   /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_memory_sci_buf ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getMemorySciBufNV( const VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV * pGetSciBufInfo,
                                                                           NvSciBufObj *                                       pHandle,
                                                                           Dispatch const &                                    d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetMemorySciBufNV( m_device, reinterpret_cast<const VkMemoryGetSciBufInfoNV *>( pGetSciBufInfo ), pHandle ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<NvSciBufObj>::type
                       Device::getMemorySciBufNV( const VULKAN_HPP_NAMESPACE::MemoryGetSciBufInfoNV & getSciBufInfo, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    NvSciBufObj                  handle;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetMemorySciBufNV( m_device, reinterpret_cast<const VkMemoryGetSciBufInfoNV *>( &getSciBufInfo ), &handle ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getMemorySciBufNV" );

    return createResultValueType( result, handle );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    PhysicalDevice::getExternalMemorySciBufPropertiesNV( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType,
                                                         NvSciBufObj                                            handle,
                                                         VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV *       pMemorySciBufProperties,
                                                         Dispatch const &                                       d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>(
      d.vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( m_physicalDevice,
                                                             static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                             handle,
                                                             reinterpret_cast<VkMemorySciBufPropertiesNV *>( pMemorySciBufProperties ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV>::type
                       PhysicalDevice::getExternalMemorySciBufPropertiesNV( VULKAN_HPP_NAMESPACE::ExternalMemoryHandleTypeFlagBits handleType,
                                                         NvSciBufObj                                            handle,
                                                         Dispatch const &                                       d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::MemorySciBufPropertiesNV memorySciBufProperties;
    VULKAN_HPP_NAMESPACE::Result                   result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( m_physicalDevice,
                                                             static_cast<VkExternalMemoryHandleTypeFlagBits>( handleType ),
                                                             handle,
                                                             reinterpret_cast<VkMemorySciBufPropertiesNV *>( &memorySciBufProperties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getExternalMemorySciBufPropertiesNV" );

    return createResultValueType( result, memorySciBufProperties );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#  ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result PhysicalDevice::getSciBufAttributesNV( NvSciBufAttrList pAttributes,
                                                                                       Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetPhysicalDeviceSciBufAttributesNV( m_physicalDevice, pAttributes ) );
  }
#  else
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    PhysicalDevice::getSciBufAttributesNV( NvSciBufAttrList pAttributes, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkGetPhysicalDeviceSciBufAttributesNV( m_physicalDevice, pAttributes ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::PhysicalDevice::getSciBufAttributesNV" );

    return createResultValueType( result );
  }
#  endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif   /*VK_USE_PLATFORM_SCI*/

  //=== VK_EXT_extended_dynamic_state2 ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setPatchControlPointsEXT( uint32_t patchControlPoints, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetPatchControlPointsEXT( m_commandBuffer, patchControlPoints );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setRasterizerDiscardEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 rasterizerDiscardEnable,
                                                                       Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetRasterizerDiscardEnableEXT( m_commandBuffer, static_cast<VkBool32>( rasterizerDiscardEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setDepthBiasEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 depthBiasEnable, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetDepthBiasEnableEXT( m_commandBuffer, static_cast<VkBool32>( depthBiasEnable ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setLogicOpEXT( VULKAN_HPP_NAMESPACE::LogicOp logicOp, Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetLogicOpEXT( m_commandBuffer, static_cast<VkLogicOp>( logicOp ) );
  }

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setPrimitiveRestartEnableEXT( VULKAN_HPP_NAMESPACE::Bool32 primitiveRestartEnable,
                                                                      Dispatch const &             d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetPrimitiveRestartEnableEXT( m_commandBuffer, static_cast<VkBool32>( primitiveRestartEnable ) );
  }

  //=== VK_EXT_color_write_enable ===

  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setColorWriteEnableEXT( uint32_t                             attachmentCount,
                                                                const VULKAN_HPP_NAMESPACE::Bool32 * pColorWriteEnables,
                                                                Dispatch const &                     d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    d.vkCmdSetColorWriteEnableEXT( m_commandBuffer, attachmentCount, reinterpret_cast<const VkBool32 *>( pColorWriteEnables ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_INLINE void CommandBuffer::setColorWriteEnableEXT( VULKAN_HPP_NAMESPACE::ArrayProxy<const VULKAN_HPP_NAMESPACE::Bool32> const & colorWriteEnables,
                                                                Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    d.vkCmdSetColorWriteEnableEXT( m_commandBuffer, colorWriteEnables.size(), reinterpret_cast<const VkBool32 *>( colorWriteEnables.data() ) );
  }
#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result
    Device::createSemaphoreSciSyncPoolNV( const VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV * pCreateInfo,
                                          const VULKAN_HPP_NAMESPACE::AllocationCallbacks *              pAllocator,
                                          VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV *                 pSemaphorePool,
                                          Dispatch const &                                               d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkCreateSemaphoreSciSyncPoolNV( m_device,
                                                                  reinterpret_cast<const VkSemaphoreSciSyncPoolCreateInfoNV *>( pCreateInfo ),
                                                                  reinterpret_cast<const VkAllocationCallbacks *>( pAllocator ),
                                                                  reinterpret_cast<VkSemaphoreSciSyncPoolNV *>( pSemaphorePool ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV>::type
                       Device::createSemaphoreSciSyncPoolNV( const VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV & createInfo,
                                          Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                          Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV semaphorePool;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSemaphoreSciSyncPoolNV(
      m_device,
      reinterpret_cast<const VkSemaphoreSciSyncPoolCreateInfoNV *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSemaphoreSciSyncPoolNV *>( &semaphorePool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSemaphoreSciSyncPoolNV" );

    return createResultValueType( result, semaphorePool );
  }

#    ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV, Dispatch>>::type
                       Device::createSemaphoreSciSyncPoolNVUnique( const VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolCreateInfoNV & createInfo,
                                                Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks>      allocator,
                                                Dispatch const &                                               d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV semaphorePool;
    VULKAN_HPP_NAMESPACE::Result                 result = static_cast<VULKAN_HPP_NAMESPACE::Result>( d.vkCreateSemaphoreSciSyncPoolNV(
      m_device,
      reinterpret_cast<const VkSemaphoreSciSyncPoolCreateInfoNV *>( &createInfo ),
      reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ),
      reinterpret_cast<VkSemaphoreSciSyncPoolNV *>( &semaphorePool ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::createSemaphoreSciSyncPoolNVUnique" );

    return createResultValueType(
      result, UniqueHandle<VULKAN_HPP_NAMESPACE::SemaphoreSciSyncPoolNV, Dispatch>( semaphorePool, ObjectDestroy<Device, Dispatch>( *this, allocator, d ) ) );
  }
#    endif /* VULKAN_HPP_NO_SMART_HANDLE */
#  endif   /* VULKAN_HPP_DISABLE_ENHANCED_MODE */
#endif     /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===

  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE Result Device::getScreenBufferPropertiesQNX( const struct _screen_buffer *                     buffer,
                                                                                      VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX * pProperties,
                                                                                      Dispatch const & d ) const VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    return static_cast<Result>( d.vkGetScreenBufferPropertiesQNX( m_device, buffer, reinterpret_cast<VkScreenBufferPropertiesQNX *>( pProperties ) ) );
  }

#  ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>::type
                       Device::getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX properties;
    VULKAN_HPP_NAMESPACE::Result                    result = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetScreenBufferPropertiesQNX( m_device, &buffer, reinterpret_cast<VkScreenBufferPropertiesQNX *>( &properties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getScreenBufferPropertiesQNX" );

    return createResultValueType( result, properties );
  }

  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE typename ResultValueType<VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>>::type
                       Device::getScreenBufferPropertiesQNX( const struct _screen_buffer & buffer, Dispatch const & d ) const
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );

    StructureChain<X, Y, Z...>                        structureChain;
    VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX & properties = structureChain.template get<VULKAN_HPP_NAMESPACE::ScreenBufferPropertiesQNX>();
    VULKAN_HPP_NAMESPACE::Result                      result     = static_cast<VULKAN_HPP_NAMESPACE::Result>(
      d.vkGetScreenBufferPropertiesQNX( m_device, &buffer, reinterpret_cast<VkScreenBufferPropertiesQNX *>( &properties ) ) );
    resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::Device::getScreenBufferPropertiesQNX" );

    return createResultValueType( result, structureChain );
  }
#  endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */
#endif   /*VK_USE_PLATFORM_SCREEN_QNX*/

}  // namespace VULKAN_HPP_NAMESPACE
#endif
