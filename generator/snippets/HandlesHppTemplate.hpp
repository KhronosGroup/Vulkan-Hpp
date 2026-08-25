// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

${copyrightMessage}

#ifndef VULKAN_HANDLES_HPP
#  define VULKAN_HANDLES_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private, include "vulkan/vulkan.hpp"

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
  ${structForwardDeclarations}
  ${handleForwardDeclarations}
  ${funcPointerReturns}
  ${uniqueHandles}
  ${handles}

  // operators to compare VULKAN_HPP_NAMESPACE::-handles
#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator==( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) == static_cast<typename T::NativeType>( rhs );
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator!=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) != static_cast<typename T::NativeType>( rhs );
  }

#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#  else
  template <VulkanHandleType T>
#  endif
  auto operator<=>( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) <=> static_cast<typename T::NativeType>( rhs );
  }
#else
#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#  else
  template <VulkanHandleType T>
#  endif
  bool operator<( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) < static_cast<typename T::NativeType>( rhs );
  }

#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#  else
  template <VulkanHandleType T>
#  endif
  bool operator<=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) <= static_cast<typename T::NativeType>( rhs );
  }

#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#  else
  template <VulkanHandleType T>
#  endif
  bool operator>( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) > static_cast<typename T::NativeType>( rhs );
  }

#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#  else
  template <VulkanHandleType T>
#  endif
  bool operator>=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) >= static_cast<typename T::NativeType>( rhs );
  }
#endif

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator==( T const & v, std::nullptr_t )
  {
    return !v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator==( std::nullptr_t, T const & v )
  {
    return !v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator!=( T const & v, std::nullptr_t )
  {
    return !!v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
#else
  template <VulkanHandleType T>
#endif
  bool operator!=( std::nullptr_t, T const & v )
  {
    return !!v;
  }
}   // namespace VULKAN_HPP_NAMESPACE
#endif
