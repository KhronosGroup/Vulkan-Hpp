#include <algorithm>
#include <array>     // ArrayWrapperND
#include <string.h>  // strnlen
#include <string>    // std::string
#include <vulkan/${vulkan_h}>
#include <vulkan/vulkan_hpp_macros.hpp>

#if 17 <= VULKAN_HPP_CPP_VERSION
#  include <string_view>
#endif

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
#  include <tuple>   // std::tie
#  include <vector>  // std::vector
#endif

#if !defined( VULKAN_HPP_NO_EXCEPTIONS )
#  include <system_error>  // std::is_error_code_enum
#endif

#if ( VULKAN_HPP_ASSERT == assert )
#  include <cassert>
#endif

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL == 1
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
#    include <dlfcn.h>
#  elif defined( _WIN32 ) && !defined( VULKAN_HPP_NO_WIN32_PROTOTYPES )
typedef struct HINSTANCE__ * HINSTANCE;
#    if defined( _WIN64 )
typedef int64_t( __stdcall * FARPROC )();
#    else
typedef int( __stdcall * FARPROC )();
#    endif
extern "C" __declspec( dllimport ) HINSTANCE __stdcall LoadLibraryA( char const * lpLibFileName );
extern "C" __declspec( dllimport ) int __stdcall FreeLibrary( HINSTANCE hLibModule );
extern "C" __declspec( dllimport ) FARPROC __stdcall GetProcAddress( HINSTANCE hModule, const char * lpProcName );
#  endif
#endif

#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
#  include <compare>
#endif

#if defined( VULKAN_HPP_SUPPORT_SPAN )
#  include <span>
#endif
