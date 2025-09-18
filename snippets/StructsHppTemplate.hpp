${licenseHeader}

#ifndef VULKAN_STRUCTS_HPP
#  define VULKAN_STRUCTS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

#if !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <cstring>  // strcmp
#endif

namespace VULKAN_HPP_NAMESPACE
{
${structs}
}   // namespace VULKAN_HPP_NAMESPACE
#endif