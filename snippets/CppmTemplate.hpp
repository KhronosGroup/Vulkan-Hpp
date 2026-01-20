${licenseHeader}

module;

#define VULKAN_HPP_CXX_MODULE 1

#include <cassert>
#include <cstring>
#include <vulkan/${vulkan_h}>
#include <vulkan/vulkan_hpp_macros.hpp>

#if !defined( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#  define VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING \
  "\n\tThe Vulkan-Hpp C++ named module is experimental. It is subject to change without prior notice.\n" \
  "\tTo silence this warning, define the VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING macro.\n" \
  "\tFor feedback, go to: https://github.com/KhronosGroup/Vulkan-Hpp/issues"

  VULKAN_HPP_COMPILE_WARNING( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#endif

VULKAN_HPP_STATIC_ASSERT( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

export module ${api};

export import std;

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 5244)
#elif defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#elif defined(__GNUC__)
#endif

#include <vulkan/${api}.hpp>
#include <vulkan/${api}_extension_inspection.hpp>
#include <vulkan/${api}_format_traits.hpp>
#include <vulkan/${api}_hash.hpp>
#include <vulkan/${api}_raii.hpp>
#include <vulkan/${api}_shared.hpp>

#if defined(_MSC_VER)
#  pragma warning(pop)
#elif defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#endif

export
{
// This VkFlags type is used as part of a bitfield in some structures.
// As it can't be mimicked by vk-data types, we need to export just that.
using ::VkGeometryInstanceFlagsKHR;

${pfnCommands}
}
