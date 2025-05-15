${licenseHeader}
#ifndef VULKAN_HANDLES_HPP
#  define VULKAN_HANDLES_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

namespace VULKAN_HPP_NAMESPACE
{
  ${structForwardDeclarations}
  ${handleForwardDeclarations}
  ${funcPointerReturns}
  ${uniqueHandles}
  ${handles}
}   // namespace VULKAN_HPP_NAMESPACE
#endif