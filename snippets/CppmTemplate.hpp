${licenseHeader}

module;

#define VULKAN_HPP_CXX_MODULE 1

#include <vulkan/vulkan_hpp_macros.hpp>

#if !defined( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#  define VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING \
  "The Vulkan-Hpp C++ named module is experimental. " \
  "It is subject to change without prior notice.\n" \
  "To silence this warning, define the VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING macro.\n\n" \
  "For feedback, go to: https://github.com/KhronosGroup/Vulkan-Hpp/issues"

  VULKAN_HPP_COMPILE_WARNING( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#endif

#include <vulkan/${api}.hpp>
#include <vulkan/${api}_extension_inspection.hpp>
#include <vulkan/${api}_format_traits.hpp>
#include <vulkan/${api}_hash.hpp>
#include <vulkan/${api}_raii.hpp>
#include <vulkan/${api}_shared.hpp>

export module ${api};
export import :video;
export import std;

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

export
{
// This VkFlags type is used as part of a bitfield in some structures.
// As it can't be mimicked by vk-data types, we need to export just that.
using ::VkGeometryInstanceFlagsKHR;

${pfnCommands}
}
