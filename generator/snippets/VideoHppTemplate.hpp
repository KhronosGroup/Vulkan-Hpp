${copyrightMessage}

#ifndef VULKAN_VIDEO_HPP
#define VULKAN_VIDEO_HPP

// here, we consider include files to be available when __has_include is not defined
#if !defined( __has_include )
#  define __has_include( x ) true
#  define has_include_was_not_defined
#endif

#if !defined( VULKAN_HPP_CXX_MODULE )
#  include <vulkan/vulkan.hpp>
${includes}
#endif

#if !defined( VULKAN_HPP_VIDEO_NAMESPACE )
#  define VULKAN_HPP_VIDEO_NAMESPACE video
#endif

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
namespace VULKAN_HPP_VIDEO_NAMESPACE
{
${constants}
${enums}
${structs}
}   // namespace VULKAN_HPP_VIDEO_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE

#if defined( has_include_was_not_defined )
#  undef has_include_was_not_defined
#  undef __has_include
#endif

#endif
