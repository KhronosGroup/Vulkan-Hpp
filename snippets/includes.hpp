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

#include <vulkan/vulkan_dispatch_loader.hpp>

// done:
// 1. no more func declarations in vulkan_handles
// -> by forward declaring vk::Instance and vk::Device init functions in dispatch loader
// 2. unify all dispatch loaders (currently only unified static/dynamic)
// 2.5 remove all the dispatcher templating (enable if too)
// 3. fix/remove unique handles.. what about shared ones?
// 4. also changed the init interface of the dispatcher a bit, i
// 5. instead of the dispatch loader, the dynamic loader is now in detail namespace
// 6. removed getVkHeaderVersion from dispatch loader

// TODO:
// some sort of static function that returns the default dispatcher? similar to raii
// dispatch loader init functions should get some comments!