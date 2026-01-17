${copyrightMessage}

module;

#define VULKAN_HPP_CXX_MODULE 1

${includes}

// clang-format off
#include <vulkan/vulkan_hpp_macros.hpp>
// clang-format on
#include <vulkan/vulkan.hpp>

export module vulkan:video;

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 5244)
#elif defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#elif defined(__GNUC__)
#endif

#include <vulkan/vulkan_video.hpp>

#if defined(_MSC_VER)
#  pragma warning(pop)
#elif defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#endif
