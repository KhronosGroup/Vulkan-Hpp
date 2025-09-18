${licenseHeader}

#ifndef VULKAN_ENUMS_HPP
#  define VULKAN_ENUMS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

#if !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <type_traits>  // for std::underlying_type
#endif

namespace VULKAN_HPP_NAMESPACE
{
${Flags}

${enums}
${objectTypeToDebugReportObjectType}
}   // namespace VULKAN_HPP_NAMESPACE
#endif