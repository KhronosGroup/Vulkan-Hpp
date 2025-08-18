${licenseHeader}
#ifndef VULKAN_HPP
#  define VULKAN_HPP

${includes}

static_assert( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

${defines}

namespace VULKAN_HPP_NAMESPACE
{
  ${ArrayWrapper1D}
  ${ArrayWrapper2D}
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  ${ArrayProxy}
  ${ArrayProxyNoTemporaries}
  ${StridedArrayProxy}
  ${Optional}
  ${StructureChain}
  ${UniqueHandle}
#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

  namespace detail
  {
    ${DispatchLoaderBase}
    ${DispatchLoaderStatic}
  }
  ${Exchange}

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  struct AllocationCallbacks;

  namespace detail
  {
    ${ObjectDestroy}
    ${ObjectFree}
    ${ObjectRelease}
    ${PoolFree}
  }
#endif // !VULKAN_HPP_NO_SMART_HANDLE

  ${baseTypes}

  template <typename Type, Type value = Type{}>
  struct CppType
  {};
} // namespace VULKAN_HPP_NAMESPACE

#include <vulkan/${api}_enums.hpp>
#if !defined( VULKAN_HPP_NO_TO_STRING )
#include <vulkan/${api}_to_string.hpp>
#endif

#ifndef VULKAN_HPP_NO_EXCEPTIONS
namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {};
}  // namespace std
#endif

namespace VULKAN_HPP_NAMESPACE
{
#ifndef VULKAN_HPP_NO_EXCEPTIONS
${Exceptions}
${resultExceptions}
${throwResultException}
#endif

${ResultValue}
${resultChecks}
${constexprDefines}
} // namespace VULKAN_HPP_NAMESPACE

// clang-format off
#include <vulkan/${api}_handles.hpp>
#include <vulkan/${api}_structs.hpp>
#include <vulkan/${api}_funcs.hpp>
// clang-format on


namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  ${structExtendsStructs}
#endif // VULKAN_HPP_DISABLE_ENHANCED_MODE

  namespace detail
  {
    ${DynamicLoader}
    ${DispatchLoaderDynamic}
  }
}   // namespace VULKAN_HPP_NAMESPACE
#endif