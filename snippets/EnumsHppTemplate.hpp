${licenseHeader}

#ifndef VULKAN_ENUMS_HPP
#  define VULKAN_ENUMS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private, include "vulkan/vulkan.hpp"

#include <type_traits>    // for std::underlying_type

namespace VULKAN_HPP_NAMESPACE
{
${Flags}

${enums}
${objectTypeToDebugReportObjectType}
}   // namespace VULKAN_HPP_NAMESPACE
#endif