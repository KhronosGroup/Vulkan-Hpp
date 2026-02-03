${licenseHeader}

#ifndef VULKAN_FORMAT_TRAITS_HPP
#  define VULKAN_FORMAT_TRAITS_HPP

#if defined( VULKAN_HPP_CXX_MODULE )
#  define VULKAN_HPP_EXPORT export
#else
#  include <vulkan/${api}.hpp>
#endif

namespace VULKAN_HPP_NAMESPACE
{
${formatTraits}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
