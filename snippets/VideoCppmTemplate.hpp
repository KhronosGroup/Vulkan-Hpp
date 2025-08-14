${copyrightMessage}

// Note: This module is still in an experimental state.
// Any feedback is welcome on https://github.com/KhronosGroup/Vulkan-Hpp/issues.

module;

#include <vulkan/vulkan_hpp_macros.hpp>

#if defined( __cpp_lib_modules ) && !defined( VULKAN_HPP_ENABLE_STD_MODULE )
#define VULKAN_HPP_ENABLE_STD_MODULE
#endif

#include <vulkan/vulkan_video.hpp>

export module vulkan_video_hpp;

export namespace VULKAN_HPP_NAMESPACE
{
namespace VULKAN_HPP_VIDEO_NAMESPACE
{
${usings}
}   // namespace VULKAN_HPP_VIDEO_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE