${licenseHeader}
#ifndef VULKAN_HPP
#  define VULKAN_HPP

${includes}

VULKAN_HPP_STATIC_ASSERT( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
# if !defined( VULKAN_HPP_DEFAULT_DISPATCHER_HANDLED )
    // dispatcher will be placed once in user code
    extern VULKAN_HPP_STORAGE_API DispatchLoader defaultDispatchLoader;
# endif
#else
  // dispatcher will be placed into every including TU
  static vk::DispatchLoader defaultDispatchLoader;
#endif
} // namespace VULKAN_HPP_NAMESPACE

${defines}

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
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

  ${Exchange}

  struct AllocationCallbacks;

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

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
VULKAN_HPP_EXPORT namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {};
}  // namespace std
#endif

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
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

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
  // define forward-declared DispatchLoader init() functions
  void DispatchLoader::init( Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
  {
    init( static_cast<VkInstance>( instanceCpp ) );
  }

  void DispatchLoader::init( Device deviceCpp ) VULKAN_HPP_NOEXCEPT
  {
    init( static_cast<VkDevice>( deviceCpp ) );
  }

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  ${structExtendsStructs}
#endif // VULKAN_HPP_DISABLE_ENHANCED_MODE
}   // namespace VULKAN_HPP_NAMESPACE
#endif
