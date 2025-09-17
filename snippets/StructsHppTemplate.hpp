${licenseHeader}

#ifndef VULKAN_STRUCTS_HPP
#  define VULKAN_STRUCTS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private, include "vulkan/vulkan.hpp"

#include <cstring>  // strcmp
#include <cstdlib>  // free

namespace VULKAN_HPP_NAMESPACE
{
${structs}
}   // namespace VULKAN_HPP_NAMESPACE
#endif