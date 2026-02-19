#pragma once
#include <cstdlib>
#include <iostream>
#define release_assert( x )                                                                            \
  do                                                                                                   \
  {                                                                                                    \
    if ( !( x ) )                                                                                      \
    {                                                                                                  \
      std::cout << __FILE__ << ':' << __LINE__ << ": Assertion \'" << #x << "\' failed." << std::endl; \
      std::abort();                                                                                    \
    }                                                                                                  \
  } while ( false )

#if VULKAN_HPP_CPP_VERSION >= 17
#  define VULKAN_HPP_MAYBE_UNUSED [[maybe_unused]]
#  define VULKAN_HPP_UNUSED( var )
#else
#  define VULKAN_HPP_MAYBE_UNUSED
#  define VULKAN_HPP_UNUSED( var ) ( (void)( var ) )
#endif

