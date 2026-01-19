#pragma once
#include <iostream>
#define release_assert( x ) \
  do \
  { \
    if ( !( x ) ) \
    { \
      std::cout << __FILE__ << ':' << __LINE__ << ": Assertion \'" << #x << "\' failed."<< std::endl; \
      std::abort(); \
    } \
  } while ( false )