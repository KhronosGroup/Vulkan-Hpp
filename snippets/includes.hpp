#include <vulkan/vulkan_hpp_macros.hpp>

#if defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE )
#include <string.h>
import VULKAN_HPP_STD_MODULE;
#else
# include <algorithm>
# include <array>     // ArrayWrapperND
# include <string.h>  // strnlen
# include <string>    // std::string
# include <utility>   // std::exchange
#endif
#include <vulkan/${vulkan_h}>

#if 17 <= VULKAN_HPP_CPP_VERSION && !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <string_view>
#endif

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE ) && !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <tuple>   // std::tie
#  include <vector>  // std::vector
#endif

#if !defined( VULKAN_HPP_NO_EXCEPTIONS ) && !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <system_error>  // std::is_error_code_enum
#endif

#if ( VULKAN_HPP_ASSERT == assert )
#  include <cassert>
#endif

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL == 1
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
#    include <dlfcn.h>
#  elif defined( _WIN32 ) && !defined( VULKAN_HPP_NO_WIN32_PROTOTYPES )
using HINSTANCE = struct HINSTANCE__ *;
#    if defined( _WIN64 )
using FARPROC = int64_t(__stdcall *)();
#    else
using FARPROC = int(__stdcall *)();
#    endif
extern "C" __declspec( dllimport ) HINSTANCE __stdcall LoadLibraryA( char const * lpLibFileName );
extern "C" __declspec( dllimport ) int __stdcall FreeLibrary( HINSTANCE hLibModule );
extern "C" __declspec( dllimport ) FARPROC __stdcall GetProcAddress( HINSTANCE hModule, const char * lpProcName );
#  endif
#endif

#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR ) && !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <compare>
#endif

#if defined( VULKAN_HPP_SUPPORT_SPAN ) && !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <span>
#endif
