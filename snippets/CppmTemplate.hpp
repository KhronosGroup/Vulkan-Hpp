${licenseHeader}

// Note: This module is still in an experimental state.
// Any feedback is welcome on https://github.com/KhronosGroup/Vulkan-Hpp/issues.

module;

#include <vulkan/vulkan_hpp_macros.hpp>

#if defined( __cpp_lib_modules ) && !defined( VULKAN_HPP_ENABLE_STD_MODULE )
#define VULKAN_HPP_ENABLE_STD_MODULE
#endif

#include <vulkan/${api}.hpp>
#include <vulkan/${api}_extension_inspection.hpp>
#include <vulkan/${api}_format_traits.hpp>
#include <vulkan/${api}_hash.hpp>
#include <vulkan/${api}_raii.hpp>
#include <vulkan/${api}_shared.hpp>
#ifndef VULKAN_HPP_NO_TO_STRING
#include <vulkan/${api}_to_string.hpp>
#endif

export module ${api}_hpp;

export namespace VULKAN_HPP_NAMESPACE
{
  ${usings}

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    ${raiiUsings}
  } // namespace VULKAN_HPP_RAII_NAMESPACE
#endif
} // namespace VULKAN_HPP_NAMESPACE

export namespace std
{
  ${hashSpecializations}

  //=================================================================
  //=== Required exports for VULKAN_HPP_NAMESPACE::StructureChain ===
  //=================================================================

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  using std::tuple_size;
  using std::tuple_element;
#endif
}

// This VkFlags type is used as part of a bitfield in some structure.
// As it can't be mimicked by vk-data types, we need to export just that!!
export using ::VkGeometryInstanceFlagsKHR;

${pfnCommands}