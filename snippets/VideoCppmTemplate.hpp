${copyrightMessage}

module;

#define VULKAN_HPP_CXX_MODULE 1

#include <vulkan/vulkan_hpp_macros.hpp>

#if !defined( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#  define VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING \
  "\n\tThe Vulkan-Hpp C++ named module is experimental. It is subject to change without prior notice.\n" \
  "\tTo silence this warning, define the VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING macro.\n" \
  "\tFor feedback, go to: https://github.com/KhronosGroup/Vulkan-Hpp/issues"

  VULKAN_HPP_COMPILE_WARNING( VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING )
#endif

#include <vulkan/vulkan_video.hpp>

export module vulkan:video;

export namespace VULKAN_HPP_NAMESPACE::VULKAN_HPP_VIDEO_NAMESPACE
{
${usings}
}   // namespace VULKAN_HPP_NAMESPACE::VULKAN_HPP_VIDEO_NAMESPACE
