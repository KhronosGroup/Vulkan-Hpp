${licenseHeader}
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
  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) == static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) != static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator<( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) < static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator<=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) <= static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator>( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) > static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator>=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) >= static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( T const & v, std::nullptr_t )
  {
    return !v;
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( std::nullptr_t, T const & v )
  {
    return !v;
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( T const & v, std::nullptr_t )
  {
    return !!v;
  }

  template <typename T, typename std::enable_if<isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( std::nullptr_t, T const & v )
  {
    return !!v;
  }
}   // namespace VULKAN_HPP_NAMESPACE
#endif
