${licenseHeader}
#ifndef VULKAN_TO_STRING_HPP
#  define VULKAN_TO_STRING_HPP

#include <vulkan/${api}.hpp>

// ignore warnings on using deprecated enum values in this header
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4996 )
#endif

#if defined( VULKAN_HPP_STD_MODULE )
import VULKAN_HPP_STD_MODULE;
#else
#  if defined( __cpp_lib_format )
#    include <format>   // std::format
#  else
#    include <sstream>  // std::stringstream
#  endif
#endif

namespace VULKAN_HPP_NAMESPACE
{
${bitmasksToString}
${enumsToString}
} // namespace VULKAN_HPP_NAMESPACE

#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#elif defined( _MSC_VER )
#  pragma warning( pop )
#endif

#endif
