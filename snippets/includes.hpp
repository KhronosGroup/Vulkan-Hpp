#if !defined( VULKAN_HPP_CXX_MODULE )
#  include <vulkan/${vulkan_h}>
// clang-format off
#  include <vulkan/vulkan_hpp_macros.hpp>
// clang-format on
#  include <algorithm>
#  include <array>  // ArrayWrapperND
#  include <cassert>
#  include <string.h>  // strnlen
#  include <string>    // std::string
#  include <utility>   // std::exchange
#  if 17 <= VULKAN_HPP_CPP_VERSION
#    include <string_view>
#  endif
#  if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
#    include <tuple>   // std::tie
#    include <vector>  // std::vector
#  endif
#  if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
#    include <compare>
#  endif
#  if !defined( VULKAN_HPP_NO_EXCEPTIONS )
#    include <system_error>  // std::is_error_code_enum
#  endif
#  if defined( VULKAN_HPP_SUPPORT_SPAN )
#    include <span>
#  endif
#endif
