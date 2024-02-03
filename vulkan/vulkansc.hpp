// Copyright 2015-2024 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_HPP
#define VULKAN_HPP

#include <algorithm>
#include <array>     // ArrayWrapperND
#include <string.h>  // strnlen
#include <string>    // std::string
#include <vulkan/vulkan_hpp_macros.hpp>
#include <vulkan/vulkan_sc_core.h>

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

static_assert( VK_HEADER_VERSION == 14, "Wrong VK_HEADER_VERSION!" );

// <tuple> includes <sys/sysmacros.h> through some other header
// this results in major(x) being resolved to gnu_dev_major(x)
// which is an expression in a constructor initializer list.
#if defined( major )
#  undef major
#endif
#if defined( minor )
#  undef minor
#endif

// Windows defines MemoryBarrier which is deprecated and collides
// with the VULKAN_HPP_NAMESPACE::MemoryBarrier struct.
#if defined( MemoryBarrier )
#  undef MemoryBarrier
#endif

// XLib.h defines True/False, which collides with our vk::True/vk::False
// ->  undef them and provide some namepace-secure constexpr
#if defined( True )
#  undef True
constexpr int True = 1;
#endif
#if defined( False )
#  undef False
constexpr int False = 0;
#endif

namespace VULKAN_HPP_NAMESPACE
{
  template <typename T, size_t N>
  class ArrayWrapper1D : public std::array<T, N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper1D() VULKAN_HPP_NOEXCEPT : std::array<T, N>() {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper1D( std::array<T, N> const & data ) VULKAN_HPP_NOEXCEPT : std::array<T, N>( data ) {}

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    VULKAN_HPP_CONSTEXPR_14 ArrayWrapper1D( std::string const & data ) VULKAN_HPP_NOEXCEPT
    {
      copy( data.data(), data.length() );
    }

#if 17 <= VULKAN_HPP_CPP_VERSION
    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    VULKAN_HPP_CONSTEXPR_14 ArrayWrapper1D( std::string_view data ) VULKAN_HPP_NOEXCEPT
    {
      copy( data.data(), data.length() );
    }
#endif

#if ( VK_USE_64_BIT_PTR_DEFINES == 0 )
    // on 32 bit compiles, needs overloads on index type int to resolve ambiguities
    VULKAN_HPP_CONSTEXPR T const & operator[]( int index ) const VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[]( index );
    }

    T & operator[]( int index ) VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[]( index );
    }
#endif

    operator T const *() const VULKAN_HPP_NOEXCEPT
    {
      return this->data();
    }

    operator T *() VULKAN_HPP_NOEXCEPT
    {
      return this->data();
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    operator std::string() const
    {
      return std::string( this->data(), strnlen( this->data(), N ) );
    }

#if 17 <= VULKAN_HPP_CPP_VERSION
    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    operator std::string_view() const
    {
      return std::string_view( this->data(), strnlen( this->data(), N ) );
    }
#endif

#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    std::strong_ordering operator<=>( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) <=> *static_cast<std::array<char, N> const *>( &rhs );
    }
#else
    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator<( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) < *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator<=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) <= *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator>( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) > *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator>=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) >= *static_cast<std::array<char, N> const *>( &rhs );
    }
#endif

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator==( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) == *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator!=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) != *static_cast<std::array<char, N> const *>( &rhs );
    }

  private:
    VULKAN_HPP_CONSTEXPR_14 void copy( char const * data, size_t len ) VULKAN_HPP_NOEXCEPT
    {
      size_t n = std::min( N, len );
      for ( size_t i = 0; i < n; ++i )
      {
        ( *this )[i] = data[i];
      }
      for ( size_t i = n; i < N; ++i )
      {
        ( *this )[i] = 0;
      }
    }
  };

  // specialization of relational operators between std::string and arrays of chars
  template <size_t N>
  bool operator<( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs < rhs.data();
  }

  template <size_t N>
  bool operator<=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs <= rhs.data();
  }

  template <size_t N>
  bool operator>( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs > rhs.data();
  }

  template <size_t N>
  bool operator>=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs >= rhs.data();
  }

  template <size_t N>
  bool operator==( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs == rhs.data();
  }

  template <size_t N>
  bool operator!=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs != rhs.data();
  }

  template <typename T, size_t N, size_t M>
  class ArrayWrapper2D : public std::array<ArrayWrapper1D<T, M>, N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper2D() VULKAN_HPP_NOEXCEPT : std::array<ArrayWrapper1D<T, M>, N>() {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper2D( std::array<std::array<T, M>, N> const & data ) VULKAN_HPP_NOEXCEPT
      : std::array<ArrayWrapper1D<T, M>, N>( *reinterpret_cast<std::array<ArrayWrapper1D<T, M>, N> const *>( &data ) )
    {
    }
  };

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  template <typename T>
  class ArrayProxy
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxy() VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {
    }

    VULKAN_HPP_CONSTEXPR ArrayProxy( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {
    }

    ArrayProxy( T const & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {
    }

    ArrayProxy( uint32_t count, T const * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {
    }

    template <std::size_t C>
    ArrayProxy( T const ( &ptr )[C] ) VULKAN_HPP_NOEXCEPT
      : m_count( C )
      , m_ptr( ptr )
    {
    }

#  if __GNUC__ >= 9
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Winit-list-lifetime"
#  endif

    ArrayProxy( std::initializer_list<T> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::initializer_list<typename std::remove_const<T>::type> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

#  if __GNUC__ >= 9
#    pragma GCC diagnostic pop
#  endif

    // Any type with a .data() return type implicitly convertible to T*, and a .size() return type implicitly
    // convertible to size_t. The const version can capture temporaries, with lifetime ending at end of statement.
    template <typename V,
              typename std::enable_if<std::is_convertible<decltype( std::declval<V>().data() ), T *>::value &&
                                      std::is_convertible<decltype( std::declval<V>().size() ), std::size_t>::value>::type * = nullptr>
    ArrayProxy( V const & v ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( v.size() ) )
      , m_ptr( v.data() )
    {
    }

    const T * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    const T * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    const T & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *( m_ptr + m_count - 1 );
    }

    bool empty() const VULKAN_HPP_NOEXCEPT
    {
      return ( m_count == 0 );
    }

    uint32_t size() const VULKAN_HPP_NOEXCEPT
    {
      return m_count;
    }

    T const * data() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

  private:
    uint32_t  m_count;
    T const * m_ptr;
  };

  template <typename T>
  class ArrayProxyNoTemporaries
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxyNoTemporaries() VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {
    }

    VULKAN_HPP_CONSTEXPR ArrayProxyNoTemporaries( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {
    }

    ArrayProxyNoTemporaries( T & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {
    }

    template <typename V>
    ArrayProxyNoTemporaries( V && value ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {
    }

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type && value ) = delete;

    ArrayProxyNoTemporaries( uint32_t count, T * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {
    }

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( uint32_t count, typename std::remove_const<T>::type * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {
    }

    template <std::size_t C>
    ArrayProxyNoTemporaries( T ( &ptr )[C] ) VULKAN_HPP_NOEXCEPT
      : m_count( C )
      , m_ptr( ptr )
    {
    }

    template <std::size_t C>
    ArrayProxyNoTemporaries( T ( &&ptr )[C] ) = delete;

    template <std::size_t C, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type ( &ptr )[C] ) VULKAN_HPP_NOEXCEPT
      : m_count( C )
      , m_ptr( ptr )
    {
    }

    template <std::size_t C, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type ( &&ptr )[C] ) = delete;

    ArrayProxyNoTemporaries( std::initializer_list<T> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

    ArrayProxyNoTemporaries( std::initializer_list<T> const && list ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const && list ) = delete;

    ArrayProxyNoTemporaries( std::initializer_list<T> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

    ArrayProxyNoTemporaries( std::initializer_list<T> && list ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {
    }

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> && list ) = delete;

    // Any type with a .data() return type implicitly convertible to T*, and a .size() return type implicitly convertible to size_t.
    template <typename V,
              typename std::enable_if<std::is_convertible<decltype( std::declval<V>().data() ), T *>::value &&
                                      std::is_convertible<decltype( std::declval<V>().size() ), std::size_t>::value>::type * = nullptr>
    ArrayProxyNoTemporaries( V & v ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( v.size() ) )
      , m_ptr( v.data() )
    {
    }

    const T * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    const T * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    const T & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *( m_ptr + m_count - 1 );
    }

    bool empty() const VULKAN_HPP_NOEXCEPT
    {
      return ( m_count == 0 );
    }

    uint32_t size() const VULKAN_HPP_NOEXCEPT
    {
      return m_count;
    }

    T * data() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

  private:
    uint32_t m_count;
    T *      m_ptr;
  };

  template <typename T>
  class StridedArrayProxy : protected ArrayProxy<T>
  {
  public:
    using ArrayProxy<T>::ArrayProxy;

    StridedArrayProxy( uint32_t count, T const * ptr, uint32_t stride ) VULKAN_HPP_NOEXCEPT
      : ArrayProxy<T>( count, ptr )
      , m_stride( stride )
    {
      VULKAN_HPP_ASSERT( sizeof( T ) <= stride );
    }

    using ArrayProxy<T>::begin;

    const T * end() const VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<T const *>( static_cast<uint8_t const *>( begin() ) + size() * m_stride );
    }

    using ArrayProxy<T>::front;

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( begin() && size() );
      return *reinterpret_cast<T const *>( static_cast<uint8_t const *>( begin() ) + ( size() - 1 ) * m_stride );
    }

    using ArrayProxy<T>::empty;
    using ArrayProxy<T>::size;
    using ArrayProxy<T>::data;

    uint32_t stride() const
    {
      return m_stride;
    }

  private:
    uint32_t m_stride = sizeof( T );
  };

  template <typename RefType>
  class Optional
  {
  public:
    Optional( RefType & reference ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = &reference;
    }

    Optional( RefType * ptr ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = ptr;
    }

    Optional( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = nullptr;
    }

    operator RefType *() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    RefType const * operator->() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return !!m_ptr;
    }

  private:
    RefType * m_ptr;
  };

  template <typename X, typename Y>
  struct StructExtends
  {
    enum
    {
      value = false
    };
  };

  template <typename Type, class...>
  struct IsPartOfStructureChain
  {
    static const bool valid = false;
  };

  template <typename Type, typename Head, typename... Tail>
  struct IsPartOfStructureChain<Type, Head, Tail...>
  {
    static const bool valid = std::is_same<Type, Head>::value || IsPartOfStructureChain<Type, Tail...>::valid;
  };

  template <size_t Index, typename T, typename... ChainElements>
  struct StructureChainContains
  {
    static const bool value = std::is_same<T, typename std::tuple_element<Index, std::tuple<ChainElements...>>::type>::value ||
                              StructureChainContains<Index - 1, T, ChainElements...>::value;
  };

  template <typename T, typename... ChainElements>
  struct StructureChainContains<0, T, ChainElements...>
  {
    static const bool value = std::is_same<T, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value;
  };

  template <size_t Index, typename... ChainElements>
  struct StructureChainValidation
  {
    using TestType          = typename std::tuple_element<Index, std::tuple<ChainElements...>>::type;
    static const bool valid = StructExtends<TestType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value &&
                              ( TestType::allowDuplicate || !StructureChainContains<Index - 1, TestType, ChainElements...>::value ) &&
                              StructureChainValidation<Index - 1, ChainElements...>::valid;
  };

  template <typename... ChainElements>
  struct StructureChainValidation<0, ChainElements...>
  {
    static const bool valid = true;
  };

  template <typename... ChainElements>
  class StructureChain : public std::tuple<ChainElements...>
  {
  public:
    StructureChain() VULKAN_HPP_NOEXCEPT
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain( StructureChain const & rhs ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( rhs )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link( &std::get<0>( *this ),
            &std::get<0>( rhs ),
            reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( *this ) ),
            reinterpret_cast<VkBaseInStructure const *>( &std::get<0>( rhs ) ) );
    }

    StructureChain( StructureChain && rhs ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( std::forward<std::tuple<ChainElements...>>( rhs ) )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link( &std::get<0>( *this ),
            &std::get<0>( rhs ),
            reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( *this ) ),
            reinterpret_cast<VkBaseInStructure const *>( &std::get<0>( rhs ) ) );
    }

    StructureChain( ChainElements const &... elems ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( elems... )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain & operator=( StructureChain const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      std::tuple<ChainElements...>::operator=( rhs );
      link( &std::get<0>( *this ),
            &std::get<0>( rhs ),
            reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( *this ) ),
            reinterpret_cast<VkBaseInStructure const *>( &std::get<0>( rhs ) ) );
      return *this;
    }

    StructureChain & operator=( StructureChain && rhs ) = delete;

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    T & get() VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( static_cast<std::tuple<ChainElements...> &>( *this ) );
    }

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    T const & get() const VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( static_cast<std::tuple<ChainElements...> const &>( *this ) );
    }

    template <typename T0, typename T1, typename... Ts>
    std::tuple<T0 &, T1 &, Ts &...> get() VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    template <typename T0, typename T1, typename... Ts>
    std::tuple<T0 const &, T1 const &, Ts const &...> get() const VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    // assign a complete structure to the StructureChain without modifying the chaining
    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    StructureChain & assign( const T & rhs ) VULKAN_HPP_NOEXCEPT
    {
      T &    lhs   = get<T, Which>();
      void * pNext = lhs.pNext;
      lhs          = rhs;
      lhs.pNext    = pNext;
      return *this;
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value && ( Which == 0 ), bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      return true;
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || ( Which != 0 ), bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't unlink Structure that's not part of this StructureChain!" );
      return isLinked( reinterpret_cast<VkBaseInStructure const *>( &get<ClassType, Which>() ) );
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || ( Which != 0 ), void>::type
      relink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't relink Structure that's not part of this StructureChain!" );
      auto pNext = reinterpret_cast<VkBaseInStructure *>( &get<ClassType, Which>() );
      VULKAN_HPP_ASSERT( !isLinked( pNext ) );
      auto & headElement = std::get<0>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      pNext->pNext       = reinterpret_cast<VkBaseInStructure const *>( headElement.pNext );
      headElement.pNext  = pNext;
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || ( Which != 0 ), void>::type
      unlink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't unlink Structure that's not part of this StructureChain!" );
      unlink( reinterpret_cast<VkBaseOutStructure const *>( &get<ClassType, Which>() ) );
    }

  private:
    template <int Index, typename T, int Which, typename, class First, class... Types>
    struct ChainElementIndex : ChainElementIndex<Index + 1, T, Which, void, Types...>
    {
    };

    template <int Index, typename T, int Which, class First, class... Types>
    struct ChainElementIndex<Index, T, Which, typename std::enable_if<!std::is_same<T, First>::value, void>::type, First, Types...>
      : ChainElementIndex<Index + 1, T, Which, void, Types...>
    {
    };

    template <int Index, typename T, int Which, class First, class... Types>
    struct ChainElementIndex<Index, T, Which, typename std::enable_if<std::is_same<T, First>::value, void>::type, First, Types...>
      : ChainElementIndex<Index + 1, T, Which - 1, void, Types...>
    {
    };

    template <int Index, typename T, class First, class... Types>
    struct ChainElementIndex<Index, T, 0, typename std::enable_if<std::is_same<T, First>::value, void>::type, First, Types...>
      : std::integral_constant<int, Index>
    {
    };

    bool isLinked( VkBaseInStructure const * pNext ) const VULKAN_HPP_NOEXCEPT
    {
      VkBaseInStructure const * elementPtr =
        reinterpret_cast<VkBaseInStructure const *>( &std::get<0>( static_cast<std::tuple<ChainElements...> const &>( *this ) ) );
      while ( elementPtr )
      {
        if ( elementPtr->pNext == pNext )
        {
          return true;
        }
        elementPtr = elementPtr->pNext;
      }
      return false;
    }

    template <size_t Index>
    typename std::enable_if<Index != 0, void>::type link() VULKAN_HPP_NOEXCEPT
    {
      auto & x = std::get<Index - 1>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      x.pNext  = &std::get<Index>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      link<Index - 1>();
    }

    template <size_t Index>
    typename std::enable_if<Index == 0, void>::type link() VULKAN_HPP_NOEXCEPT
    {
    }

    void link( void * dstBase, void const * srcBase, VkBaseOutStructure * dst, VkBaseInStructure const * src )
    {
      while ( src->pNext )
      {
        std::ptrdiff_t offset = reinterpret_cast<char const *>( src->pNext ) - reinterpret_cast<char const *>( srcBase );
        dst->pNext            = reinterpret_cast<VkBaseOutStructure *>( reinterpret_cast<char *>( dstBase ) + offset );
        dst                   = dst->pNext;
        src                   = src->pNext;
      }
      dst->pNext = nullptr;
    }

    void unlink( VkBaseOutStructure const * pNext ) VULKAN_HPP_NOEXCEPT
    {
      VkBaseOutStructure * elementPtr = reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( static_cast<std::tuple<ChainElements...> &>( *this ) ) );
      while ( elementPtr && ( elementPtr->pNext != pNext ) )
      {
        elementPtr = elementPtr->pNext;
      }
      if ( elementPtr )
      {
        elementPtr->pNext = pNext->pNext;
      }
      else
      {
        VULKAN_HPP_ASSERT( false );  // fires, if the ClassType member has already been unlinked !
      }
    }
  };

  // interupt the VULKAN_HPP_NAMESPACE for a moment to add specializations of std::tuple_size and std::tuple_element for the StructureChain!
}

namespace std
{
  template <typename... Elements>
  struct tuple_size<VULKAN_HPP_NAMESPACE::StructureChain<Elements...>>
  {
    static constexpr size_t value = std::tuple_size<std::tuple<Elements...>>::value;
  };

  template <std::size_t Index, typename... Elements>
  struct tuple_element<Index, VULKAN_HPP_NAMESPACE::StructureChain<Elements...>>
  {
    using type = typename std::tuple_element<Index, std::tuple<Elements...>>::type;
  };
}  // namespace std

namespace VULKAN_HPP_NAMESPACE
{

#  if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  template <typename Type, typename Dispatch>
  class UniqueHandleTraits;

  template <typename Type, typename Dispatch>
  class UniqueHandle : public UniqueHandleTraits<Type, Dispatch>::deleter
  {
  private:
    using Deleter = typename UniqueHandleTraits<Type, Dispatch>::deleter;

  public:
    using element_type = Type;

    UniqueHandle() : Deleter(), m_value() {}

    explicit UniqueHandle( Type const & value, Deleter const & deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
      : Deleter( deleter )
      , m_value( value )
    {
    }

    UniqueHandle( UniqueHandle const & ) = delete;

    UniqueHandle( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
      : Deleter( std::move( static_cast<Deleter &>( other ) ) )
      , m_value( other.release() )
    {
    }

    ~UniqueHandle() VULKAN_HPP_NOEXCEPT
    {
      if ( m_value )
      {
        this->destroy( m_value );
      }
    }

    UniqueHandle & operator=( UniqueHandle const & ) = delete;

    UniqueHandle & operator=( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
    {
      reset( other.release() );
      *static_cast<Deleter *>( this ) = std::move( static_cast<Deleter &>( other ) );
      return *this;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return m_value.operator bool();
    }

#    if defined( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
    operator Type() const VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }
#    endif

    Type const * operator->() const VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type * operator->() VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type const & operator*() const VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    Type & operator*() VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    const Type & get() const VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    Type & get() VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    void reset( Type const & value = Type() ) VULKAN_HPP_NOEXCEPT
    {
      if ( m_value != value )
      {
        if ( m_value )
        {
          this->destroy( m_value );
        }
        m_value = value;
      }
    }

    Type release() VULKAN_HPP_NOEXCEPT
    {
      Type value = m_value;
      m_value    = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type, Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
    {
      std::swap( m_value, rhs.m_value );
      std::swap( static_cast<Deleter &>( *this ), static_cast<Deleter &>( rhs ) );
    }

  private:
    Type m_value;
  };

  template <typename UniqueType>
  VULKAN_HPP_INLINE std::vector<typename UniqueType::element_type> uniqueToRaw( std::vector<UniqueType> const & handles )
  {
    std::vector<typename UniqueType::element_type> newBuffer( handles.size() );
    std::transform( handles.begin(), handles.end(), newBuffer.begin(), []( UniqueType const & handle ) { return handle.get(); } );
    return newBuffer;
  }

  template <typename Type, typename Dispatch>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type, Dispatch> & lhs, UniqueHandle<Type, Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
  {
    lhs.swap( rhs );
  }
#  endif
#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

  class DispatchLoaderBase
  {
  public:
    DispatchLoaderBase() = default;
    DispatchLoaderBase( std::nullptr_t )
#if !defined( NDEBUG )
      : m_valid( false )
#endif
    {
    }

#if !defined( NDEBUG )
    size_t getVkHeaderVersion() const
    {
      VULKAN_HPP_ASSERT( m_valid );
      return vkHeaderVersion;
    }

  private:
    size_t vkHeaderVersion = VK_HEADER_VERSION;
    bool   m_valid         = true;
#endif
  };

#if !defined( VK_NO_PROTOTYPES )
  class DispatchLoaderStatic : public DispatchLoaderBase
  {
  public:
    //=== VK_VERSION_1_0 ===

    VkResult
      vkCreateInstance( const VkInstanceCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkInstance * pInstance ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateInstance( pCreateInfo, pAllocator, pInstance );
    }

    void vkDestroyInstance( VkInstance instance, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyInstance( instance, pAllocator );
    }

    VkResult vkEnumeratePhysicalDevices( VkInstance instance, uint32_t * pPhysicalDeviceCount, VkPhysicalDevice * pPhysicalDevices ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices );
    }

    void vkGetPhysicalDeviceFeatures( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures * pFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceFeatures( physicalDevice, pFeatures );
    }

    void
      vkGetPhysicalDeviceFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties * pFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceFormatProperties( physicalDevice, format, pFormatProperties );
    }

    VkResult vkGetPhysicalDeviceImageFormatProperties( VkPhysicalDevice          physicalDevice,
                                                       VkFormat                  format,
                                                       VkImageType               type,
                                                       VkImageTiling             tiling,
                                                       VkImageUsageFlags         usage,
                                                       VkImageCreateFlags        flags,
                                                       VkImageFormatProperties * pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties );
    }

    void vkGetPhysicalDeviceProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceProperties( physicalDevice, pProperties );
    }

    void vkGetPhysicalDeviceQueueFamilyProperties( VkPhysicalDevice          physicalDevice,
                                                   uint32_t *                pQueueFamilyPropertyCount,
                                                   VkQueueFamilyProperties * pQueueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
    }

    void vkGetPhysicalDeviceMemoryProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties * pMemoryProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceMemoryProperties( physicalDevice, pMemoryProperties );
    }

    PFN_vkVoidFunction vkGetInstanceProcAddr( VkInstance instance, const char * pName ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetInstanceProcAddr( instance, pName );
    }

    PFN_vkVoidFunction vkGetDeviceProcAddr( VkDevice device, const char * pName ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceProcAddr( device, pName );
    }

    VkResult vkCreateDevice( VkPhysicalDevice              physicalDevice,
                             const VkDeviceCreateInfo *    pCreateInfo,
                             const VkAllocationCallbacks * pAllocator,
                             VkDevice *                    pDevice ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDevice( physicalDevice, pCreateInfo, pAllocator, pDevice );
    }

    void vkDestroyDevice( VkDevice device, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyDevice( device, pAllocator );
    }

    VkResult vkEnumerateInstanceExtensionProperties( const char *            pLayerName,
                                                     uint32_t *              pPropertyCount,
                                                     VkExtensionProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, pProperties );
    }

    VkResult vkEnumerateDeviceExtensionProperties( VkPhysicalDevice        physicalDevice,
                                                   const char *            pLayerName,
                                                   uint32_t *              pPropertyCount,
                                                   VkExtensionProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, pProperties );
    }

    VkResult vkEnumerateInstanceLayerProperties( uint32_t * pPropertyCount, VkLayerProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumerateInstanceLayerProperties( pPropertyCount, pProperties );
    }

    VkResult
      vkEnumerateDeviceLayerProperties( VkPhysicalDevice physicalDevice, uint32_t * pPropertyCount, VkLayerProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, pProperties );
    }

    void vkGetDeviceQueue( VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue * pQueue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
    }

    VkResult vkQueueSubmit( VkQueue queue, uint32_t submitCount, const VkSubmitInfo * pSubmits, VkFence fence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueSubmit( queue, submitCount, pSubmits, fence );
    }

    VkResult vkQueueWaitIdle( VkQueue queue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueWaitIdle( queue );
    }

    VkResult vkDeviceWaitIdle( VkDevice device ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDeviceWaitIdle( device );
    }

    VkResult vkAllocateMemory( VkDevice                      device,
                               const VkMemoryAllocateInfo *  pAllocateInfo,
                               const VkAllocationCallbacks * pAllocator,
                               VkDeviceMemory *              pMemory ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAllocateMemory( device, pAllocateInfo, pAllocator, pMemory );
    }

    VkResult vkMapMemory( VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void ** ppData ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkMapMemory( device, memory, offset, size, flags, ppData );
    }

    void vkUnmapMemory( VkDevice device, VkDeviceMemory memory ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkUnmapMemory( device, memory );
    }

    VkResult vkFlushMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange * pMemoryRanges ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkFlushMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
    }

    VkResult vkInvalidateMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange * pMemoryRanges ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkInvalidateMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
    }

    void vkGetDeviceMemoryCommitment( VkDevice device, VkDeviceMemory memory, VkDeviceSize * pCommittedMemoryInBytes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
    }

    VkResult vkBindBufferMemory( VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkBindBufferMemory( device, buffer, memory, memoryOffset );
    }

    VkResult vkBindImageMemory( VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkBindImageMemory( device, image, memory, memoryOffset );
    }

    void vkGetBufferMemoryRequirements( VkDevice device, VkBuffer buffer, VkMemoryRequirements * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetBufferMemoryRequirements( device, buffer, pMemoryRequirements );
    }

    void vkGetImageMemoryRequirements( VkDevice device, VkImage image, VkMemoryRequirements * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetImageMemoryRequirements( device, image, pMemoryRequirements );
    }

    VkResult vkCreateFence( VkDevice                      device,
                            const VkFenceCreateInfo *     pCreateInfo,
                            const VkAllocationCallbacks * pAllocator,
                            VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateFence( device, pCreateInfo, pAllocator, pFence );
    }

    void vkDestroyFence( VkDevice device, VkFence fence, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyFence( device, fence, pAllocator );
    }

    VkResult vkResetFences( VkDevice device, uint32_t fenceCount, const VkFence * pFences ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetFences( device, fenceCount, pFences );
    }

    VkResult vkGetFenceStatus( VkDevice device, VkFence fence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetFenceStatus( device, fence );
    }

    VkResult vkWaitForFences( VkDevice device, uint32_t fenceCount, const VkFence * pFences, VkBool32 waitAll, uint64_t timeout ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkWaitForFences( device, fenceCount, pFences, waitAll, timeout );
    }

    VkResult vkCreateSemaphore( VkDevice                      device,
                                const VkSemaphoreCreateInfo * pCreateInfo,
                                const VkAllocationCallbacks * pAllocator,
                                VkSemaphore *                 pSemaphore ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSemaphore( device, pCreateInfo, pAllocator, pSemaphore );
    }

    void vkDestroySemaphore( VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroySemaphore( device, semaphore, pAllocator );
    }

    VkResult vkCreateEvent( VkDevice                      device,
                            const VkEventCreateInfo *     pCreateInfo,
                            const VkAllocationCallbacks * pAllocator,
                            VkEvent *                     pEvent ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateEvent( device, pCreateInfo, pAllocator, pEvent );
    }

    void vkDestroyEvent( VkDevice device, VkEvent event, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyEvent( device, event, pAllocator );
    }

    VkResult vkGetEventStatus( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetEventStatus( device, event );
    }

    VkResult vkSetEvent( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSetEvent( device, event );
    }

    VkResult vkResetEvent( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetEvent( device, event );
    }

    VkResult vkCreateQueryPool( VkDevice                      device,
                                const VkQueryPoolCreateInfo * pCreateInfo,
                                const VkAllocationCallbacks * pAllocator,
                                VkQueryPool *                 pQueryPool ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateQueryPool( device, pCreateInfo, pAllocator, pQueryPool );
    }

    VkResult vkGetQueryPoolResults( VkDevice           device,
                                    VkQueryPool        queryPool,
                                    uint32_t           firstQuery,
                                    uint32_t           queryCount,
                                    size_t             dataSize,
                                    void *             pData,
                                    VkDeviceSize       stride,
                                    VkQueryResultFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetQueryPoolResults( device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags );
    }

    VkResult vkCreateBuffer( VkDevice                      device,
                             const VkBufferCreateInfo *    pCreateInfo,
                             const VkAllocationCallbacks * pAllocator,
                             VkBuffer *                    pBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateBuffer( device, pCreateInfo, pAllocator, pBuffer );
    }

    void vkDestroyBuffer( VkDevice device, VkBuffer buffer, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyBuffer( device, buffer, pAllocator );
    }

    VkResult vkCreateBufferView( VkDevice                       device,
                                 const VkBufferViewCreateInfo * pCreateInfo,
                                 const VkAllocationCallbacks *  pAllocator,
                                 VkBufferView *                 pView ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateBufferView( device, pCreateInfo, pAllocator, pView );
    }

    void vkDestroyBufferView( VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyBufferView( device, bufferView, pAllocator );
    }

    VkResult vkCreateImage( VkDevice                      device,
                            const VkImageCreateInfo *     pCreateInfo,
                            const VkAllocationCallbacks * pAllocator,
                            VkImage *                     pImage ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateImage( device, pCreateInfo, pAllocator, pImage );
    }

    void vkDestroyImage( VkDevice device, VkImage image, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyImage( device, image, pAllocator );
    }

    void vkGetImageSubresourceLayout( VkDevice                   device,
                                      VkImage                    image,
                                      const VkImageSubresource * pSubresource,
                                      VkSubresourceLayout *      pLayout ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetImageSubresourceLayout( device, image, pSubresource, pLayout );
    }

    VkResult vkCreateImageView( VkDevice                      device,
                                const VkImageViewCreateInfo * pCreateInfo,
                                const VkAllocationCallbacks * pAllocator,
                                VkImageView *                 pView ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateImageView( device, pCreateInfo, pAllocator, pView );
    }

    void vkDestroyImageView( VkDevice device, VkImageView imageView, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyImageView( device, imageView, pAllocator );
    }

    VkResult vkCreatePipelineCache( VkDevice                          device,
                                    const VkPipelineCacheCreateInfo * pCreateInfo,
                                    const VkAllocationCallbacks *     pAllocator,
                                    VkPipelineCache *                 pPipelineCache ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreatePipelineCache( device, pCreateInfo, pAllocator, pPipelineCache );
    }

    void vkDestroyPipelineCache( VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyPipelineCache( device, pipelineCache, pAllocator );
    }

    VkResult vkCreateGraphicsPipelines( VkDevice                             device,
                                        VkPipelineCache                      pipelineCache,
                                        uint32_t                             createInfoCount,
                                        const VkGraphicsPipelineCreateInfo * pCreateInfos,
                                        const VkAllocationCallbacks *        pAllocator,
                                        VkPipeline *                         pPipelines ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateGraphicsPipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
    }

    VkResult vkCreateComputePipelines( VkDevice                            device,
                                       VkPipelineCache                     pipelineCache,
                                       uint32_t                            createInfoCount,
                                       const VkComputePipelineCreateInfo * pCreateInfos,
                                       const VkAllocationCallbacks *       pAllocator,
                                       VkPipeline *                        pPipelines ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateComputePipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
    }

    void vkDestroyPipeline( VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyPipeline( device, pipeline, pAllocator );
    }

    VkResult vkCreatePipelineLayout( VkDevice                           device,
                                     const VkPipelineLayoutCreateInfo * pCreateInfo,
                                     const VkAllocationCallbacks *      pAllocator,
                                     VkPipelineLayout *                 pPipelineLayout ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreatePipelineLayout( device, pCreateInfo, pAllocator, pPipelineLayout );
    }

    void vkDestroyPipelineLayout( VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyPipelineLayout( device, pipelineLayout, pAllocator );
    }

    VkResult vkCreateSampler( VkDevice                      device,
                              const VkSamplerCreateInfo *   pCreateInfo,
                              const VkAllocationCallbacks * pAllocator,
                              VkSampler *                   pSampler ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSampler( device, pCreateInfo, pAllocator, pSampler );
    }

    void vkDestroySampler( VkDevice device, VkSampler sampler, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroySampler( device, sampler, pAllocator );
    }

    VkResult vkCreateDescriptorSetLayout( VkDevice                                device,
                                          const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
                                          const VkAllocationCallbacks *           pAllocator,
                                          VkDescriptorSetLayout *                 pSetLayout ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDescriptorSetLayout( device, pCreateInfo, pAllocator, pSetLayout );
    }

    void vkDestroyDescriptorSetLayout( VkDevice                      device,
                                       VkDescriptorSetLayout         descriptorSetLayout,
                                       const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyDescriptorSetLayout( device, descriptorSetLayout, pAllocator );
    }

    VkResult vkCreateDescriptorPool( VkDevice                           device,
                                     const VkDescriptorPoolCreateInfo * pCreateInfo,
                                     const VkAllocationCallbacks *      pAllocator,
                                     VkDescriptorPool *                 pDescriptorPool ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDescriptorPool( device, pCreateInfo, pAllocator, pDescriptorPool );
    }

    VkResult vkResetDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetDescriptorPool( device, descriptorPool, flags );
    }

    VkResult vkAllocateDescriptorSets( VkDevice                            device,
                                       const VkDescriptorSetAllocateInfo * pAllocateInfo,
                                       VkDescriptorSet *                   pDescriptorSets ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAllocateDescriptorSets( device, pAllocateInfo, pDescriptorSets );
    }

    VkResult vkFreeDescriptorSets( VkDevice                device,
                                   VkDescriptorPool        descriptorPool,
                                   uint32_t                descriptorSetCount,
                                   const VkDescriptorSet * pDescriptorSets ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkFreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets );
    }

    void vkUpdateDescriptorSets( VkDevice                     device,
                                 uint32_t                     descriptorWriteCount,
                                 const VkWriteDescriptorSet * pDescriptorWrites,
                                 uint32_t                     descriptorCopyCount,
                                 const VkCopyDescriptorSet *  pDescriptorCopies ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkUpdateDescriptorSets( device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies );
    }

    VkResult vkCreateFramebuffer( VkDevice                        device,
                                  const VkFramebufferCreateInfo * pCreateInfo,
                                  const VkAllocationCallbacks *   pAllocator,
                                  VkFramebuffer *                 pFramebuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateFramebuffer( device, pCreateInfo, pAllocator, pFramebuffer );
    }

    void vkDestroyFramebuffer( VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyFramebuffer( device, framebuffer, pAllocator );
    }

    VkResult vkCreateRenderPass( VkDevice                       device,
                                 const VkRenderPassCreateInfo * pCreateInfo,
                                 const VkAllocationCallbacks *  pAllocator,
                                 VkRenderPass *                 pRenderPass ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateRenderPass( device, pCreateInfo, pAllocator, pRenderPass );
    }

    void vkDestroyRenderPass( VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyRenderPass( device, renderPass, pAllocator );
    }

    void vkGetRenderAreaGranularity( VkDevice device, VkRenderPass renderPass, VkExtent2D * pGranularity ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetRenderAreaGranularity( device, renderPass, pGranularity );
    }

    VkResult vkCreateCommandPool( VkDevice                        device,
                                  const VkCommandPoolCreateInfo * pCreateInfo,
                                  const VkAllocationCallbacks *   pAllocator,
                                  VkCommandPool *                 pCommandPool ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateCommandPool( device, pCreateInfo, pAllocator, pCommandPool );
    }

    VkResult vkResetCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetCommandPool( device, commandPool, flags );
    }

    VkResult vkAllocateCommandBuffers( VkDevice                            device,
                                       const VkCommandBufferAllocateInfo * pAllocateInfo,
                                       VkCommandBuffer *                   pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAllocateCommandBuffers( device, pAllocateInfo, pCommandBuffers );
    }

    void vkFreeCommandBuffers( VkDevice                device,
                               VkCommandPool           commandPool,
                               uint32_t                commandBufferCount,
                               const VkCommandBuffer * pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkFreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
    }

    VkResult vkBeginCommandBuffer( VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo * pBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkBeginCommandBuffer( commandBuffer, pBeginInfo );
    }

    VkResult vkEndCommandBuffer( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEndCommandBuffer( commandBuffer );
    }

    VkResult vkResetCommandBuffer( VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetCommandBuffer( commandBuffer, flags );
    }

    void vkCmdBindPipeline( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindPipeline( commandBuffer, pipelineBindPoint, pipeline );
    }

    void
      vkCmdSetViewport( VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport * pViewports ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetViewport( commandBuffer, firstViewport, viewportCount, pViewports );
    }

    void vkCmdSetScissor( VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D * pScissors ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetScissor( commandBuffer, firstScissor, scissorCount, pScissors );
    }

    void vkCmdSetLineWidth( VkCommandBuffer commandBuffer, float lineWidth ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetLineWidth( commandBuffer, lineWidth );
    }

    void vkCmdSetDepthBias( VkCommandBuffer commandBuffer,
                            float           depthBiasConstantFactor,
                            float           depthBiasClamp,
                            float           depthBiasSlopeFactor ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
    }

    void vkCmdSetBlendConstants( VkCommandBuffer commandBuffer, const float blendConstants[4] ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetBlendConstants( commandBuffer, blendConstants );
    }

    void vkCmdSetDepthBounds( VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
    }

    void vkCmdSetStencilCompareMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilCompareMask( commandBuffer, faceMask, compareMask );
    }

    void vkCmdSetStencilWriteMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilWriteMask( commandBuffer, faceMask, writeMask );
    }

    void vkCmdSetStencilReference( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilReference( commandBuffer, faceMask, reference );
    }

    void vkCmdBindDescriptorSets( VkCommandBuffer         commandBuffer,
                                  VkPipelineBindPoint     pipelineBindPoint,
                                  VkPipelineLayout        layout,
                                  uint32_t                firstSet,
                                  uint32_t                descriptorSetCount,
                                  const VkDescriptorSet * pDescriptorSets,
                                  uint32_t                dynamicOffsetCount,
                                  const uint32_t *        pDynamicOffsets ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindDescriptorSets(
        commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
    }

    void vkCmdBindIndexBuffer( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindIndexBuffer( commandBuffer, buffer, offset, indexType );
    }

    void vkCmdBindVertexBuffers( VkCommandBuffer      commandBuffer,
                                 uint32_t             firstBinding,
                                 uint32_t             bindingCount,
                                 const VkBuffer *     pBuffers,
                                 const VkDeviceSize * pOffsets ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
    }

    void vkCmdDraw( VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
    }

    void vkCmdDrawIndexed( VkCommandBuffer commandBuffer,
                           uint32_t        indexCount,
                           uint32_t        instanceCount,
                           uint32_t        firstIndex,
                           int32_t         vertexOffset,
                           uint32_t        firstInstance ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
    }

    void vkCmdDrawIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
    }

    void vkCmdDrawIndexedIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
    }

    void vkCmdDispatch( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDispatch( commandBuffer, groupCountX, groupCountY, groupCountZ );
    }

    void vkCmdDispatchIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDispatchIndirect( commandBuffer, buffer, offset );
    }

    void vkCmdCopyBuffer( VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy * pRegions ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions );
    }

    void vkCmdCopyImage( VkCommandBuffer     commandBuffer,
                         VkImage             srcImage,
                         VkImageLayout       srcImageLayout,
                         VkImage             dstImage,
                         VkImageLayout       dstImageLayout,
                         uint32_t            regionCount,
                         const VkImageCopy * pRegions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
    }

    void vkCmdBlitImage( VkCommandBuffer     commandBuffer,
                         VkImage             srcImage,
                         VkImageLayout       srcImageLayout,
                         VkImage             dstImage,
                         VkImageLayout       dstImageLayout,
                         uint32_t            regionCount,
                         const VkImageBlit * pRegions,
                         VkFilter            filter ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter );
    }

    void vkCmdCopyBufferToImage( VkCommandBuffer           commandBuffer,
                                 VkBuffer                  srcBuffer,
                                 VkImage                   dstImage,
                                 VkImageLayout             dstImageLayout,
                                 uint32_t                  regionCount,
                                 const VkBufferImageCopy * pRegions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions );
    }

    void vkCmdCopyImageToBuffer( VkCommandBuffer           commandBuffer,
                                 VkImage                   srcImage,
                                 VkImageLayout             srcImageLayout,
                                 VkBuffer                  dstBuffer,
                                 uint32_t                  regionCount,
                                 const VkBufferImageCopy * pRegions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions );
    }

    void vkCmdUpdateBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void * pData ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
    }

    void
      vkCmdFillBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
    }

    void vkCmdClearColorImage( VkCommandBuffer                 commandBuffer,
                               VkImage                         image,
                               VkImageLayout                   imageLayout,
                               const VkClearColorValue *       pColor,
                               uint32_t                        rangeCount,
                               const VkImageSubresourceRange * pRanges ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdClearColorImage( commandBuffer, image, imageLayout, pColor, rangeCount, pRanges );
    }

    void vkCmdClearDepthStencilImage( VkCommandBuffer                  commandBuffer,
                                      VkImage                          image,
                                      VkImageLayout                    imageLayout,
                                      const VkClearDepthStencilValue * pDepthStencil,
                                      uint32_t                         rangeCount,
                                      const VkImageSubresourceRange *  pRanges ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdClearDepthStencilImage( commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges );
    }

    void vkCmdClearAttachments( VkCommandBuffer           commandBuffer,
                                uint32_t                  attachmentCount,
                                const VkClearAttachment * pAttachments,
                                uint32_t                  rectCount,
                                const VkClearRect *       pRects ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdClearAttachments( commandBuffer, attachmentCount, pAttachments, rectCount, pRects );
    }

    void vkCmdResolveImage( VkCommandBuffer        commandBuffer,
                            VkImage                srcImage,
                            VkImageLayout          srcImageLayout,
                            VkImage                dstImage,
                            VkImageLayout          dstImageLayout,
                            uint32_t               regionCount,
                            const VkImageResolve * pRegions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
    }

    void vkCmdSetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetEvent( commandBuffer, event, stageMask );
    }

    void vkCmdResetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResetEvent( commandBuffer, event, stageMask );
    }

    void vkCmdWaitEvents( VkCommandBuffer               commandBuffer,
                          uint32_t                      eventCount,
                          const VkEvent *               pEvents,
                          VkPipelineStageFlags          srcStageMask,
                          VkPipelineStageFlags          dstStageMask,
                          uint32_t                      memoryBarrierCount,
                          const VkMemoryBarrier *       pMemoryBarriers,
                          uint32_t                      bufferMemoryBarrierCount,
                          const VkBufferMemoryBarrier * pBufferMemoryBarriers,
                          uint32_t                      imageMemoryBarrierCount,
                          const VkImageMemoryBarrier *  pImageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWaitEvents( commandBuffer,
                                eventCount,
                                pEvents,
                                srcStageMask,
                                dstStageMask,
                                memoryBarrierCount,
                                pMemoryBarriers,
                                bufferMemoryBarrierCount,
                                pBufferMemoryBarriers,
                                imageMemoryBarrierCount,
                                pImageMemoryBarriers );
    }

    void vkCmdPipelineBarrier( VkCommandBuffer               commandBuffer,
                               VkPipelineStageFlags          srcStageMask,
                               VkPipelineStageFlags          dstStageMask,
                               VkDependencyFlags             dependencyFlags,
                               uint32_t                      memoryBarrierCount,
                               const VkMemoryBarrier *       pMemoryBarriers,
                               uint32_t                      bufferMemoryBarrierCount,
                               const VkBufferMemoryBarrier * pBufferMemoryBarriers,
                               uint32_t                      imageMemoryBarrierCount,
                               const VkImageMemoryBarrier *  pImageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdPipelineBarrier( commandBuffer,
                                     srcStageMask,
                                     dstStageMask,
                                     dependencyFlags,
                                     memoryBarrierCount,
                                     pMemoryBarriers,
                                     bufferMemoryBarrierCount,
                                     pBufferMemoryBarriers,
                                     imageMemoryBarrierCount,
                                     pImageMemoryBarriers );
    }

    void vkCmdBeginQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBeginQuery( commandBuffer, queryPool, query, flags );
    }

    void vkCmdEndQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdEndQuery( commandBuffer, queryPool, query );
    }

    void vkCmdResetQueryPool( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
    }

    void vkCmdWriteTimestamp( VkCommandBuffer         commandBuffer,
                              VkPipelineStageFlagBits pipelineStage,
                              VkQueryPool             queryPool,
                              uint32_t                query ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWriteTimestamp( commandBuffer, pipelineStage, queryPool, query );
    }

    void vkCmdCopyQueryPoolResults( VkCommandBuffer    commandBuffer,
                                    VkQueryPool        queryPool,
                                    uint32_t           firstQuery,
                                    uint32_t           queryCount,
                                    VkBuffer           dstBuffer,
                                    VkDeviceSize       dstOffset,
                                    VkDeviceSize       stride,
                                    VkQueryResultFlags flags ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyQueryPoolResults( commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags );
    }

    void vkCmdPushConstants( VkCommandBuffer    commandBuffer,
                             VkPipelineLayout   layout,
                             VkShaderStageFlags stageFlags,
                             uint32_t           offset,
                             uint32_t           size,
                             const void *       pValues ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdPushConstants( commandBuffer, layout, stageFlags, offset, size, pValues );
    }

    void vkCmdBeginRenderPass( VkCommandBuffer               commandBuffer,
                               const VkRenderPassBeginInfo * pRenderPassBegin,
                               VkSubpassContents             contents ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBeginRenderPass( commandBuffer, pRenderPassBegin, contents );
    }

    void vkCmdNextSubpass( VkCommandBuffer commandBuffer, VkSubpassContents contents ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdNextSubpass( commandBuffer, contents );
    }

    void vkCmdEndRenderPass( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdEndRenderPass( commandBuffer );
    }

    void vkCmdExecuteCommands( VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer * pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
    }

    //=== VK_VERSION_1_1 ===

    VkResult vkEnumerateInstanceVersion( uint32_t * pApiVersion ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumerateInstanceVersion( pApiVersion );
    }

    VkResult vkBindBufferMemory2( VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo * pBindInfos ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkBindBufferMemory2( device, bindInfoCount, pBindInfos );
    }

    VkResult vkBindImageMemory2( VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo * pBindInfos ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkBindImageMemory2( device, bindInfoCount, pBindInfos );
    }

    void vkGetDeviceGroupPeerMemoryFeatures( VkDevice                   device,
                                             uint32_t                   heapIndex,
                                             uint32_t                   localDeviceIndex,
                                             uint32_t                   remoteDeviceIndex,
                                             VkPeerMemoryFeatureFlags * pPeerMemoryFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceGroupPeerMemoryFeatures( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
    }

    void vkCmdSetDeviceMask( VkCommandBuffer commandBuffer, uint32_t deviceMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDeviceMask( commandBuffer, deviceMask );
    }

    void vkCmdDispatchBase( VkCommandBuffer commandBuffer,
                            uint32_t        baseGroupX,
                            uint32_t        baseGroupY,
                            uint32_t        baseGroupZ,
                            uint32_t        groupCountX,
                            uint32_t        groupCountY,
                            uint32_t        groupCountZ ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDispatchBase( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
    }

    VkResult vkEnumeratePhysicalDeviceGroups( VkInstance                        instance,
                                              uint32_t *                        pPhysicalDeviceGroupCount,
                                              VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumeratePhysicalDeviceGroups( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
    }

    void vkGetImageMemoryRequirements2( VkDevice                               device,
                                        const VkImageMemoryRequirementsInfo2 * pInfo,
                                        VkMemoryRequirements2 *                pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetImageMemoryRequirements2( device, pInfo, pMemoryRequirements );
    }

    void vkGetBufferMemoryRequirements2( VkDevice                                device,
                                         const VkBufferMemoryRequirementsInfo2 * pInfo,
                                         VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetBufferMemoryRequirements2( device, pInfo, pMemoryRequirements );
    }

    void vkGetPhysicalDeviceFeatures2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2 * pFeatures ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceFeatures2( physicalDevice, pFeatures );
    }

    void vkGetPhysicalDeviceProperties2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2 * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceProperties2( physicalDevice, pProperties );
    }

    void vkGetPhysicalDeviceFormatProperties2( VkPhysicalDevice      physicalDevice,
                                               VkFormat              format,
                                               VkFormatProperties2 * pFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceFormatProperties2( physicalDevice, format, pFormatProperties );
    }

    VkResult vkGetPhysicalDeviceImageFormatProperties2( VkPhysicalDevice                         physicalDevice,
                                                        const VkPhysicalDeviceImageFormatInfo2 * pImageFormatInfo,
                                                        VkImageFormatProperties2 *               pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceImageFormatProperties2( physicalDevice, pImageFormatInfo, pImageFormatProperties );
    }

    void vkGetPhysicalDeviceQueueFamilyProperties2( VkPhysicalDevice           physicalDevice,
                                                    uint32_t *                 pQueueFamilyPropertyCount,
                                                    VkQueueFamilyProperties2 * pQueueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceQueueFamilyProperties2( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
    }

    void vkGetPhysicalDeviceMemoryProperties2( VkPhysicalDevice                    physicalDevice,
                                               VkPhysicalDeviceMemoryProperties2 * pMemoryProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceMemoryProperties2( physicalDevice, pMemoryProperties );
    }

    void vkGetDeviceQueue2( VkDevice device, const VkDeviceQueueInfo2 * pQueueInfo, VkQueue * pQueue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceQueue2( device, pQueueInfo, pQueue );
    }

    VkResult vkCreateSamplerYcbcrConversion( VkDevice                                   device,
                                             const VkSamplerYcbcrConversionCreateInfo * pCreateInfo,
                                             const VkAllocationCallbacks *              pAllocator,
                                             VkSamplerYcbcrConversion *                 pYcbcrConversion ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSamplerYcbcrConversion( device, pCreateInfo, pAllocator, pYcbcrConversion );
    }

    void vkDestroySamplerYcbcrConversion( VkDevice                      device,
                                          VkSamplerYcbcrConversion      ycbcrConversion,
                                          const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroySamplerYcbcrConversion( device, ycbcrConversion, pAllocator );
    }

    void vkGetPhysicalDeviceExternalBufferProperties( VkPhysicalDevice                           physicalDevice,
                                                      const VkPhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
                                                      VkExternalBufferProperties *               pExternalBufferProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceExternalBufferProperties( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
    }

    void vkGetPhysicalDeviceExternalFenceProperties( VkPhysicalDevice                          physicalDevice,
                                                     const VkPhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
                                                     VkExternalFenceProperties *               pExternalFenceProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceExternalFenceProperties( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
    }

    void vkGetPhysicalDeviceExternalSemaphoreProperties( VkPhysicalDevice                              physicalDevice,
                                                         const VkPhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
                                                         VkExternalSemaphoreProperties *               pExternalSemaphoreProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceExternalSemaphoreProperties( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
    }

    void vkGetDescriptorSetLayoutSupport( VkDevice                                device,
                                          const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
                                          VkDescriptorSetLayoutSupport *          pSupport ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDescriptorSetLayoutSupport( device, pCreateInfo, pSupport );
    }

    //=== VK_VERSION_1_2 ===

    void vkCmdDrawIndirectCount( VkCommandBuffer commandBuffer,
                                 VkBuffer        buffer,
                                 VkDeviceSize    offset,
                                 VkBuffer        countBuffer,
                                 VkDeviceSize    countBufferOffset,
                                 uint32_t        maxDrawCount,
                                 uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDrawIndirectCount( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
    }

    void vkCmdDrawIndexedIndirectCount( VkCommandBuffer commandBuffer,
                                        VkBuffer        buffer,
                                        VkDeviceSize    offset,
                                        VkBuffer        countBuffer,
                                        VkDeviceSize    countBufferOffset,
                                        uint32_t        maxDrawCount,
                                        uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdDrawIndexedIndirectCount( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
    }

    VkResult vkCreateRenderPass2( VkDevice                        device,
                                  const VkRenderPassCreateInfo2 * pCreateInfo,
                                  const VkAllocationCallbacks *   pAllocator,
                                  VkRenderPass *                  pRenderPass ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateRenderPass2( device, pCreateInfo, pAllocator, pRenderPass );
    }

    void vkCmdBeginRenderPass2( VkCommandBuffer               commandBuffer,
                                const VkRenderPassBeginInfo * pRenderPassBegin,
                                const VkSubpassBeginInfo *    pSubpassBeginInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBeginRenderPass2( commandBuffer, pRenderPassBegin, pSubpassBeginInfo );
    }

    void vkCmdNextSubpass2( VkCommandBuffer            commandBuffer,
                            const VkSubpassBeginInfo * pSubpassBeginInfo,
                            const VkSubpassEndInfo *   pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdNextSubpass2( commandBuffer, pSubpassBeginInfo, pSubpassEndInfo );
    }

    void vkCmdEndRenderPass2( VkCommandBuffer commandBuffer, const VkSubpassEndInfo * pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdEndRenderPass2( commandBuffer, pSubpassEndInfo );
    }

    void vkResetQueryPool( VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkResetQueryPool( device, queryPool, firstQuery, queryCount );
    }

    VkResult vkGetSemaphoreCounterValue( VkDevice device, VkSemaphore semaphore, uint64_t * pValue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSemaphoreCounterValue( device, semaphore, pValue );
    }

    VkResult vkWaitSemaphores( VkDevice device, const VkSemaphoreWaitInfo * pWaitInfo, uint64_t timeout ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkWaitSemaphores( device, pWaitInfo, timeout );
    }

    VkResult vkSignalSemaphore( VkDevice device, const VkSemaphoreSignalInfo * pSignalInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSignalSemaphore( device, pSignalInfo );
    }

    VkDeviceAddress vkGetBufferDeviceAddress( VkDevice device, const VkBufferDeviceAddressInfo * pInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetBufferDeviceAddress( device, pInfo );
    }

    uint64_t vkGetBufferOpaqueCaptureAddress( VkDevice device, const VkBufferDeviceAddressInfo * pInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetBufferOpaqueCaptureAddress( device, pInfo );
    }

    uint64_t vkGetDeviceMemoryOpaqueCaptureAddress( VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo * pInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceMemoryOpaqueCaptureAddress( device, pInfo );
    }

    //=== VK_VERSION_1_3 ===

    VkResult vkGetPhysicalDeviceToolProperties( VkPhysicalDevice                 physicalDevice,
                                                uint32_t *                       pToolCount,
                                                VkPhysicalDeviceToolProperties * pToolProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceToolProperties( physicalDevice, pToolCount, pToolProperties );
    }

    VkResult vkCreatePrivateDataSlot( VkDevice                            device,
                                      const VkPrivateDataSlotCreateInfo * pCreateInfo,
                                      const VkAllocationCallbacks *       pAllocator,
                                      VkPrivateDataSlot *                 pPrivateDataSlot ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreatePrivateDataSlot( device, pCreateInfo, pAllocator, pPrivateDataSlot );
    }

    void vkDestroyPrivateDataSlot( VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyPrivateDataSlot( device, privateDataSlot, pAllocator );
    }

    VkResult vkSetPrivateData( VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkSetPrivateData( device, objectType, objectHandle, privateDataSlot, data );
    }

    void vkGetPrivateData( VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t * pData ) const
      VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPrivateData( device, objectType, objectHandle, privateDataSlot, pData );
    }

    void vkCmdSetEvent2( VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetEvent2( commandBuffer, event, pDependencyInfo );
    }

    void vkCmdResetEvent2( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResetEvent2( commandBuffer, event, stageMask );
    }

    void vkCmdWaitEvents2( VkCommandBuffer          commandBuffer,
                           uint32_t                 eventCount,
                           const VkEvent *          pEvents,
                           const VkDependencyInfo * pDependencyInfos ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWaitEvents2( commandBuffer, eventCount, pEvents, pDependencyInfos );
    }

    void vkCmdPipelineBarrier2( VkCommandBuffer commandBuffer, const VkDependencyInfo * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdPipelineBarrier2( commandBuffer, pDependencyInfo );
    }

    void vkCmdWriteTimestamp2( VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWriteTimestamp2( commandBuffer, stage, queryPool, query );
    }

    VkResult vkQueueSubmit2( VkQueue queue, uint32_t submitCount, const VkSubmitInfo2 * pSubmits, VkFence fence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueSubmit2( queue, submitCount, pSubmits, fence );
    }

    void vkCmdCopyBuffer2( VkCommandBuffer commandBuffer, const VkCopyBufferInfo2 * pCopyBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBuffer2( commandBuffer, pCopyBufferInfo );
    }

    void vkCmdCopyImage2( VkCommandBuffer commandBuffer, const VkCopyImageInfo2 * pCopyImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImage2( commandBuffer, pCopyImageInfo );
    }

    void vkCmdCopyBufferToImage2( VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2 * pCopyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBufferToImage2( commandBuffer, pCopyBufferToImageInfo );
    }

    void vkCmdCopyImageToBuffer2( VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2 * pCopyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImageToBuffer2( commandBuffer, pCopyImageToBufferInfo );
    }

    void vkCmdBlitImage2( VkCommandBuffer commandBuffer, const VkBlitImageInfo2 * pBlitImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBlitImage2( commandBuffer, pBlitImageInfo );
    }

    void vkCmdResolveImage2( VkCommandBuffer commandBuffer, const VkResolveImageInfo2 * pResolveImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResolveImage2( commandBuffer, pResolveImageInfo );
    }

    void vkCmdBeginRendering( VkCommandBuffer commandBuffer, const VkRenderingInfo * pRenderingInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBeginRendering( commandBuffer, pRenderingInfo );
    }

    void vkCmdEndRendering( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdEndRendering( commandBuffer );
    }

    void vkCmdSetCullMode( VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetCullMode( commandBuffer, cullMode );
    }

    void vkCmdSetFrontFace( VkCommandBuffer commandBuffer, VkFrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetFrontFace( commandBuffer, frontFace );
    }

    void vkCmdSetPrimitiveTopology( VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetPrimitiveTopology( commandBuffer, primitiveTopology );
    }

    void vkCmdSetViewportWithCount( VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport * pViewports ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetViewportWithCount( commandBuffer, viewportCount, pViewports );
    }

    void vkCmdSetScissorWithCount( VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D * pScissors ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetScissorWithCount( commandBuffer, scissorCount, pScissors );
    }

    void vkCmdBindVertexBuffers2( VkCommandBuffer      commandBuffer,
                                  uint32_t             firstBinding,
                                  uint32_t             bindingCount,
                                  const VkBuffer *     pBuffers,
                                  const VkDeviceSize * pOffsets,
                                  const VkDeviceSize * pSizes,
                                  const VkDeviceSize * pStrides ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindVertexBuffers2( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides );
    }

    void vkCmdSetDepthTestEnable( VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthTestEnable( commandBuffer, depthTestEnable );
    }

    void vkCmdSetDepthWriteEnable( VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthWriteEnable( commandBuffer, depthWriteEnable );
    }

    void vkCmdSetDepthCompareOp( VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthCompareOp( commandBuffer, depthCompareOp );
    }

    void vkCmdSetDepthBoundsTestEnable( VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBoundsTestEnable( commandBuffer, depthBoundsTestEnable );
    }

    void vkCmdSetStencilTestEnable( VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilTestEnable( commandBuffer, stencilTestEnable );
    }

    void vkCmdSetStencilOp( VkCommandBuffer    commandBuffer,
                            VkStencilFaceFlags faceMask,
                            VkStencilOp        failOp,
                            VkStencilOp        passOp,
                            VkStencilOp        depthFailOp,
                            VkCompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilOp( commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp );
    }

    void vkCmdSetRasterizerDiscardEnable( VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetRasterizerDiscardEnable( commandBuffer, rasterizerDiscardEnable );
    }

    void vkCmdSetDepthBiasEnable( VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBiasEnable( commandBuffer, depthBiasEnable );
    }

    void vkCmdSetPrimitiveRestartEnable( VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetPrimitiveRestartEnable( commandBuffer, primitiveRestartEnable );
    }

    void vkGetDeviceBufferMemoryRequirements( VkDevice                                 device,
                                              const VkDeviceBufferMemoryRequirements * pInfo,
                                              VkMemoryRequirements2 *                  pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceBufferMemoryRequirements( device, pInfo, pMemoryRequirements );
    }

    void vkGetDeviceImageMemoryRequirements( VkDevice                                device,
                                             const VkDeviceImageMemoryRequirements * pInfo,
                                             VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceImageMemoryRequirements( device, pInfo, pMemoryRequirements );
    }

    void vkGetDeviceImageSparseMemoryRequirements( VkDevice                                device,
                                                   const VkDeviceImageMemoryRequirements * pInfo,
                                                   uint32_t *                              pSparseMemoryRequirementCount,
                                                   VkSparseImageMemoryRequirements2 *      pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceImageSparseMemoryRequirements( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
    }

    //=== VKSC_VERSION_1_0 ===

    void vkGetCommandPoolMemoryConsumption( VkDevice                         device,
                                            VkCommandPool                    commandPool,
                                            VkCommandBuffer                  commandBuffer,
                                            VkCommandPoolMemoryConsumption * pConsumption ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetCommandPoolMemoryConsumption( device, commandPool, commandBuffer, pConsumption );
    }

    VkResult vkGetFaultData( VkDevice             device,
                             VkFaultQueryBehavior faultQueryBehavior,
                             VkBool32 *           pUnrecordedFaults,
                             uint32_t *           pFaultCount,
                             VkFaultData *        pFaults ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetFaultData( device, faultQueryBehavior, pUnrecordedFaults, pFaultCount, pFaults );
    }

    //=== VK_KHR_surface ===

    void vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroySurfaceKHR( instance, surface, pAllocator );
    }

    VkResult vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice,
                                                   uint32_t         queueFamilyIndex,
                                                   VkSurfaceKHR     surface,
                                                   VkBool32 *       pSupported ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
    }

    VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice           physicalDevice,
                                                        VkSurfaceKHR               surface,
                                                        VkSurfaceCapabilitiesKHR * pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
    }

    VkResult vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice     physicalDevice,
                                                   VkSurfaceKHR         surface,
                                                   uint32_t *           pSurfaceFormatCount,
                                                   VkSurfaceFormatKHR * pSurfaceFormats ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
    }

    VkResult vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice   physicalDevice,
                                                        VkSurfaceKHR       surface,
                                                        uint32_t *         pPresentModeCount,
                                                        VkPresentModeKHR * pPresentModes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
    }

    //=== VK_KHR_swapchain ===

    VkResult vkCreateSwapchainKHR( VkDevice                         device,
                                   const VkSwapchainCreateInfoKHR * pCreateInfo,
                                   const VkAllocationCallbacks *    pAllocator,
                                   VkSwapchainKHR *                 pSwapchain ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
    }

    VkResult vkGetSwapchainImagesKHR( VkDevice       device,
                                      VkSwapchainKHR swapchain,
                                      uint32_t *     pSwapchainImageCount,
                                      VkImage *      pSwapchainImages ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages );
    }

    VkResult vkAcquireNextImageKHR(
      VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t * pImageIndex ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex );
    }

    VkResult vkQueuePresentKHR( VkQueue queue, const VkPresentInfoKHR * pPresentInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueuePresentKHR( queue, pPresentInfo );
    }

    VkResult vkGetDeviceGroupPresentCapabilitiesKHR( VkDevice                              device,
                                                     VkDeviceGroupPresentCapabilitiesKHR * pDeviceGroupPresentCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceGroupPresentCapabilitiesKHR( device, pDeviceGroupPresentCapabilities );
    }

    VkResult
      vkGetDeviceGroupSurfacePresentModesKHR( VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR * pModes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDeviceGroupSurfacePresentModesKHR( device, surface, pModes );
    }

    VkResult vkGetPhysicalDevicePresentRectanglesKHR( VkPhysicalDevice physicalDevice,
                                                      VkSurfaceKHR     surface,
                                                      uint32_t *       pRectCount,
                                                      VkRect2D *       pRects ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDevicePresentRectanglesKHR( physicalDevice, surface, pRectCount, pRects );
    }

    VkResult vkAcquireNextImage2KHR( VkDevice device, const VkAcquireNextImageInfoKHR * pAcquireInfo, uint32_t * pImageIndex ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAcquireNextImage2KHR( device, pAcquireInfo, pImageIndex );
    }

    //=== VK_KHR_display ===

    VkResult vkGetPhysicalDeviceDisplayPropertiesKHR( VkPhysicalDevice         physicalDevice,
                                                      uint32_t *               pPropertyCount,
                                                      VkDisplayPropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, pProperties );
    }

    VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR( VkPhysicalDevice              physicalDevice,
                                                           uint32_t *                    pPropertyCount,
                                                           VkDisplayPlanePropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, pProperties );
    }

    VkResult vkGetDisplayPlaneSupportedDisplaysKHR( VkPhysicalDevice physicalDevice,
                                                    uint32_t         planeIndex,
                                                    uint32_t *       pDisplayCount,
                                                    VkDisplayKHR *   pDisplays ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays );
    }

    VkResult vkGetDisplayModePropertiesKHR( VkPhysicalDevice             physicalDevice,
                                            VkDisplayKHR                 display,
                                            uint32_t *                   pPropertyCount,
                                            VkDisplayModePropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, pProperties );
    }

    VkResult vkCreateDisplayModeKHR( VkPhysicalDevice                   physicalDevice,
                                     VkDisplayKHR                       display,
                                     const VkDisplayModeCreateInfoKHR * pCreateInfo,
                                     const VkAllocationCallbacks *      pAllocator,
                                     VkDisplayModeKHR *                 pMode ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDisplayModeKHR( physicalDevice, display, pCreateInfo, pAllocator, pMode );
    }

    VkResult vkGetDisplayPlaneCapabilitiesKHR( VkPhysicalDevice                physicalDevice,
                                               VkDisplayModeKHR                mode,
                                               uint32_t                        planeIndex,
                                               VkDisplayPlaneCapabilitiesKHR * pCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, pCapabilities );
    }

    VkResult vkCreateDisplayPlaneSurfaceKHR( VkInstance                            instance,
                                             const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
                                             const VkAllocationCallbacks *         pAllocator,
                                             VkSurfaceKHR *                        pSurface ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
    }

    //=== VK_KHR_display_swapchain ===

    VkResult vkCreateSharedSwapchainsKHR( VkDevice                         device,
                                          uint32_t                         swapchainCount,
                                          const VkSwapchainCreateInfoKHR * pCreateInfos,
                                          const VkAllocationCallbacks *    pAllocator,
                                          VkSwapchainKHR *                 pSwapchains ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSharedSwapchainsKHR( device, swapchainCount, pCreateInfos, pAllocator, pSwapchains );
    }

    //=== VK_KHR_external_memory_fd ===

    VkResult vkGetMemoryFdKHR( VkDevice device, const VkMemoryGetFdInfoKHR * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetMemoryFdKHR( device, pGetFdInfo, pFd );
    }

    VkResult vkGetMemoryFdPropertiesKHR( VkDevice                           device,
                                         VkExternalMemoryHandleTypeFlagBits handleType,
                                         int                                fd,
                                         VkMemoryFdPropertiesKHR *          pMemoryFdProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetMemoryFdPropertiesKHR( device, handleType, fd, pMemoryFdProperties );
    }

    //=== VK_KHR_external_semaphore_fd ===

    VkResult vkImportSemaphoreFdKHR( VkDevice device, const VkImportSemaphoreFdInfoKHR * pImportSemaphoreFdInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkImportSemaphoreFdKHR( device, pImportSemaphoreFdInfo );
    }

    VkResult vkGetSemaphoreFdKHR( VkDevice device, const VkSemaphoreGetFdInfoKHR * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSemaphoreFdKHR( device, pGetFdInfo, pFd );
    }

    //=== VK_EXT_direct_mode_display ===

    VkResult vkReleaseDisplayEXT( VkPhysicalDevice physicalDevice, VkDisplayKHR display ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkReleaseDisplayEXT( physicalDevice, display );
    }

    //=== VK_EXT_display_surface_counter ===

    VkResult vkGetPhysicalDeviceSurfaceCapabilities2EXT( VkPhysicalDevice            physicalDevice,
                                                         VkSurfaceKHR                surface,
                                                         VkSurfaceCapabilities2EXT * pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceCapabilities2EXT( physicalDevice, surface, pSurfaceCapabilities );
    }

    //=== VK_EXT_display_control ===

    VkResult vkDisplayPowerControlEXT( VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT * pDisplayPowerInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDisplayPowerControlEXT( device, display, pDisplayPowerInfo );
    }

    VkResult vkRegisterDeviceEventEXT( VkDevice                      device,
                                       const VkDeviceEventInfoEXT *  pDeviceEventInfo,
                                       const VkAllocationCallbacks * pAllocator,
                                       VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkRegisterDeviceEventEXT( device, pDeviceEventInfo, pAllocator, pFence );
    }

    VkResult vkRegisterDisplayEventEXT( VkDevice                      device,
                                        VkDisplayKHR                  display,
                                        const VkDisplayEventInfoEXT * pDisplayEventInfo,
                                        const VkAllocationCallbacks * pAllocator,
                                        VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkRegisterDisplayEventEXT( device, display, pDisplayEventInfo, pAllocator, pFence );
    }

    VkResult vkGetSwapchainCounterEXT( VkDevice                    device,
                                       VkSwapchainKHR              swapchain,
                                       VkSurfaceCounterFlagBitsEXT counter,
                                       uint64_t *                  pCounterValue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSwapchainCounterEXT( device, swapchain, counter, pCounterValue );
    }

    //=== VK_EXT_discard_rectangles ===

    void vkCmdSetDiscardRectangleEXT( VkCommandBuffer  commandBuffer,
                                      uint32_t         firstDiscardRectangle,
                                      uint32_t         discardRectangleCount,
                                      const VkRect2D * pDiscardRectangles ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDiscardRectangleEXT( commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles );
    }

    void vkCmdSetDiscardRectangleEnableEXT( VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDiscardRectangleEnableEXT( commandBuffer, discardRectangleEnable );
    }

    void vkCmdSetDiscardRectangleModeEXT( VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDiscardRectangleModeEXT( commandBuffer, discardRectangleMode );
    }

    //=== VK_EXT_hdr_metadata ===

    void vkSetHdrMetadataEXT( VkDevice                 device,
                              uint32_t                 swapchainCount,
                              const VkSwapchainKHR *   pSwapchains,
                              const VkHdrMetadataEXT * pMetadata ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSetHdrMetadataEXT( device, swapchainCount, pSwapchains, pMetadata );
    }

    //=== VK_KHR_shared_presentable_image ===

    VkResult vkGetSwapchainStatusKHR( VkDevice device, VkSwapchainKHR swapchain ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSwapchainStatusKHR( device, swapchain );
    }

    //=== VK_KHR_external_fence_fd ===

    VkResult vkImportFenceFdKHR( VkDevice device, const VkImportFenceFdInfoKHR * pImportFenceFdInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkImportFenceFdKHR( device, pImportFenceFdInfo );
    }

    VkResult vkGetFenceFdKHR( VkDevice device, const VkFenceGetFdInfoKHR * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetFenceFdKHR( device, pGetFdInfo, pFd );
    }

    //=== VK_KHR_performance_query ===

    VkResult
      vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR( VkPhysicalDevice                     physicalDevice,
                                                                       uint32_t                             queueFamilyIndex,
                                                                       uint32_t *                           pCounterCount,
                                                                       VkPerformanceCounterKHR *            pCounters,
                                                                       VkPerformanceCounterDescriptionKHR * pCounterDescriptions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
        physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions );
    }

    void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR( VkPhysicalDevice                            physicalDevice,
                                                                  const VkQueryPoolPerformanceCreateInfoKHR * pPerformanceQueryCreateInfo,
                                                                  uint32_t *                                  pNumPasses ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR( physicalDevice, pPerformanceQueryCreateInfo, pNumPasses );
    }

    VkResult vkAcquireProfilingLockKHR( VkDevice device, const VkAcquireProfilingLockInfoKHR * pInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkAcquireProfilingLockKHR( device, pInfo );
    }

    void vkReleaseProfilingLockKHR( VkDevice device ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkReleaseProfilingLockKHR( device );
    }

    //=== VK_KHR_get_surface_capabilities2 ===

    VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR( VkPhysicalDevice                        physicalDevice,
                                                         const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
                                                         VkSurfaceCapabilities2KHR *             pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceCapabilities2KHR( physicalDevice, pSurfaceInfo, pSurfaceCapabilities );
    }

    VkResult vkGetPhysicalDeviceSurfaceFormats2KHR( VkPhysicalDevice                        physicalDevice,
                                                    const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
                                                    uint32_t *                              pSurfaceFormatCount,
                                                    VkSurfaceFormat2KHR *                   pSurfaceFormats ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSurfaceFormats2KHR( physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats );
    }

    //=== VK_KHR_get_display_properties2 ===

    VkResult vkGetPhysicalDeviceDisplayProperties2KHR( VkPhysicalDevice          physicalDevice,
                                                       uint32_t *                pPropertyCount,
                                                       VkDisplayProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceDisplayProperties2KHR( physicalDevice, pPropertyCount, pProperties );
    }

    VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR( VkPhysicalDevice               physicalDevice,
                                                            uint32_t *                     pPropertyCount,
                                                            VkDisplayPlaneProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceDisplayPlaneProperties2KHR( physicalDevice, pPropertyCount, pProperties );
    }

    VkResult vkGetDisplayModeProperties2KHR( VkPhysicalDevice              physicalDevice,
                                             VkDisplayKHR                  display,
                                             uint32_t *                    pPropertyCount,
                                             VkDisplayModeProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDisplayModeProperties2KHR( physicalDevice, display, pPropertyCount, pProperties );
    }

    VkResult vkGetDisplayPlaneCapabilities2KHR( VkPhysicalDevice                 physicalDevice,
                                                const VkDisplayPlaneInfo2KHR *   pDisplayPlaneInfo,
                                                VkDisplayPlaneCapabilities2KHR * pCapabilities ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetDisplayPlaneCapabilities2KHR( physicalDevice, pDisplayPlaneInfo, pCapabilities );
    }

    //=== VK_EXT_debug_utils ===

    VkResult vkSetDebugUtilsObjectNameEXT( VkDevice device, const VkDebugUtilsObjectNameInfoEXT * pNameInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSetDebugUtilsObjectNameEXT( device, pNameInfo );
    }

    VkResult vkSetDebugUtilsObjectTagEXT( VkDevice device, const VkDebugUtilsObjectTagInfoEXT * pTagInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSetDebugUtilsObjectTagEXT( device, pTagInfo );
    }

    void vkQueueBeginDebugUtilsLabelEXT( VkQueue queue, const VkDebugUtilsLabelEXT * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueBeginDebugUtilsLabelEXT( queue, pLabelInfo );
    }

    void vkQueueEndDebugUtilsLabelEXT( VkQueue queue ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueEndDebugUtilsLabelEXT( queue );
    }

    void vkQueueInsertDebugUtilsLabelEXT( VkQueue queue, const VkDebugUtilsLabelEXT * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueInsertDebugUtilsLabelEXT( queue, pLabelInfo );
    }

    void vkCmdBeginDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBeginDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
    }

    void vkCmdEndDebugUtilsLabelEXT( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdEndDebugUtilsLabelEXT( commandBuffer );
    }

    void vkCmdInsertDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdInsertDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
    }

    VkResult vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                             const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                             const VkAllocationCallbacks *              pAllocator,
                                             VkDebugUtilsMessengerEXT *                 pMessenger ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
    }

    void vkDestroyDebugUtilsMessengerEXT( VkInstance                    instance,
                                          VkDebugUtilsMessengerEXT      messenger,
                                          const VkAllocationCallbacks * pAllocator ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
    }

    void vkSubmitDebugUtilsMessageEXT( VkInstance                                   instance,
                                       VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                       VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                       const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkSubmitDebugUtilsMessageEXT( instance, messageSeverity, messageTypes, pCallbackData );
    }

    //=== VK_EXT_sample_locations ===

    void vkCmdSetSampleLocationsEXT( VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT * pSampleLocationsInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetSampleLocationsEXT( commandBuffer, pSampleLocationsInfo );
    }

    void vkGetPhysicalDeviceMultisamplePropertiesEXT( VkPhysicalDevice             physicalDevice,
                                                      VkSampleCountFlagBits        samples,
                                                      VkMultisamplePropertiesEXT * pMultisampleProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceMultisamplePropertiesEXT( physicalDevice, samples, pMultisampleProperties );
    }

    //=== VK_EXT_image_drm_format_modifier ===

    VkResult
      vkGetImageDrmFormatModifierPropertiesEXT( VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetImageDrmFormatModifierPropertiesEXT( device, image, pProperties );
    }

    //=== VK_EXT_external_memory_host ===

    VkResult vkGetMemoryHostPointerPropertiesEXT( VkDevice                           device,
                                                  VkExternalMemoryHandleTypeFlagBits handleType,
                                                  const void *                       pHostPointer,
                                                  VkMemoryHostPointerPropertiesEXT * pMemoryHostPointerProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetMemoryHostPointerPropertiesEXT( device, handleType, pHostPointer, pMemoryHostPointerProperties );
    }

    //=== VK_EXT_calibrated_timestamps ===

    VkResult vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( VkPhysicalDevice  physicalDevice,
                                                             uint32_t *        pTimeDomainCount,
                                                             VkTimeDomainKHR * pTimeDomains ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( physicalDevice, pTimeDomainCount, pTimeDomains );
    }

    VkResult vkGetCalibratedTimestampsEXT( VkDevice                             device,
                                           uint32_t                             timestampCount,
                                           const VkCalibratedTimestampInfoKHR * pTimestampInfos,
                                           uint64_t *                           pTimestamps,
                                           uint64_t *                           pMaxDeviation ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetCalibratedTimestampsEXT( device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation );
    }

    //=== VK_KHR_fragment_shading_rate ===

    VkResult vkGetPhysicalDeviceFragmentShadingRatesKHR( VkPhysicalDevice                         physicalDevice,
                                                         uint32_t *                               pFragmentShadingRateCount,
                                                         VkPhysicalDeviceFragmentShadingRateKHR * pFragmentShadingRates ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceFragmentShadingRatesKHR( physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates );
    }

    void vkCmdSetFragmentShadingRateKHR( VkCommandBuffer                          commandBuffer,
                                         const VkExtent2D *                       pFragmentSize,
                                         const VkFragmentShadingRateCombinerOpKHR combinerOps[2] ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetFragmentShadingRateKHR( commandBuffer, pFragmentSize, combinerOps );
    }

    //=== VK_EXT_headless_surface ===

    VkResult vkCreateHeadlessSurfaceEXT( VkInstance                             instance,
                                         const VkHeadlessSurfaceCreateInfoEXT * pCreateInfo,
                                         const VkAllocationCallbacks *          pAllocator,
                                         VkSurfaceKHR *                         pSurface ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateHeadlessSurfaceEXT( instance, pCreateInfo, pAllocator, pSurface );
    }

    //=== VK_EXT_line_rasterization ===

    void vkCmdSetLineStippleEXT( VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetLineStippleEXT( commandBuffer, lineStippleFactor, lineStipplePattern );
    }

    //=== VK_EXT_extended_dynamic_state ===

    void vkCmdSetCullModeEXT( VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetCullModeEXT( commandBuffer, cullMode );
    }

    void vkCmdSetFrontFaceEXT( VkCommandBuffer commandBuffer, VkFrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetFrontFaceEXT( commandBuffer, frontFace );
    }

    void vkCmdSetPrimitiveTopologyEXT( VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetPrimitiveTopologyEXT( commandBuffer, primitiveTopology );
    }

    void vkCmdSetViewportWithCountEXT( VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport * pViewports ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetViewportWithCountEXT( commandBuffer, viewportCount, pViewports );
    }

    void vkCmdSetScissorWithCountEXT( VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D * pScissors ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetScissorWithCountEXT( commandBuffer, scissorCount, pScissors );
    }

    void vkCmdBindVertexBuffers2EXT( VkCommandBuffer      commandBuffer,
                                     uint32_t             firstBinding,
                                     uint32_t             bindingCount,
                                     const VkBuffer *     pBuffers,
                                     const VkDeviceSize * pOffsets,
                                     const VkDeviceSize * pSizes,
                                     const VkDeviceSize * pStrides ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBindVertexBuffers2EXT( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides );
    }

    void vkCmdSetDepthTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthTestEnableEXT( commandBuffer, depthTestEnable );
    }

    void vkCmdSetDepthWriteEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthWriteEnableEXT( commandBuffer, depthWriteEnable );
    }

    void vkCmdSetDepthCompareOpEXT( VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthCompareOpEXT( commandBuffer, depthCompareOp );
    }

    void vkCmdSetDepthBoundsTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBoundsTestEnableEXT( commandBuffer, depthBoundsTestEnable );
    }

    void vkCmdSetStencilTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilTestEnableEXT( commandBuffer, stencilTestEnable );
    }

    void vkCmdSetStencilOpEXT( VkCommandBuffer    commandBuffer,
                               VkStencilFaceFlags faceMask,
                               VkStencilOp        failOp,
                               VkStencilOp        passOp,
                               VkStencilOp        depthFailOp,
                               VkCompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetStencilOpEXT( commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp );
    }

    //=== VK_KHR_object_refresh ===

    void vkCmdRefreshObjectsKHR( VkCommandBuffer commandBuffer, const VkRefreshObjectListKHR * pRefreshObjects ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdRefreshObjectsKHR( commandBuffer, pRefreshObjects );
    }

    VkResult vkGetPhysicalDeviceRefreshableObjectTypesKHR( VkPhysicalDevice physicalDevice,
                                                           uint32_t *       pRefreshableObjectTypeCount,
                                                           VkObjectType *   pRefreshableObjectTypes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceRefreshableObjectTypesKHR( physicalDevice, pRefreshableObjectTypeCount, pRefreshableObjectTypes );
    }

    //=== VK_KHR_synchronization2 ===

    void vkCmdSetEvent2KHR( VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetEvent2KHR( commandBuffer, event, pDependencyInfo );
    }

    void vkCmdResetEvent2KHR( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResetEvent2KHR( commandBuffer, event, stageMask );
    }

    void vkCmdWaitEvents2KHR( VkCommandBuffer          commandBuffer,
                              uint32_t                 eventCount,
                              const VkEvent *          pEvents,
                              const VkDependencyInfo * pDependencyInfos ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWaitEvents2KHR( commandBuffer, eventCount, pEvents, pDependencyInfos );
    }

    void vkCmdPipelineBarrier2KHR( VkCommandBuffer commandBuffer, const VkDependencyInfo * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdPipelineBarrier2KHR( commandBuffer, pDependencyInfo );
    }

    void vkCmdWriteTimestamp2KHR( VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWriteTimestamp2KHR( commandBuffer, stage, queryPool, query );
    }

    VkResult vkQueueSubmit2KHR( VkQueue queue, uint32_t submitCount, const VkSubmitInfo2 * pSubmits, VkFence fence ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkQueueSubmit2KHR( queue, submitCount, pSubmits, fence );
    }

    void vkCmdWriteBufferMarker2AMD(
      VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdWriteBufferMarker2AMD( commandBuffer, stage, dstBuffer, dstOffset, marker );
    }

    void vkGetQueueCheckpointData2NV( VkQueue queue, uint32_t * pCheckpointDataCount, VkCheckpointData2NV * pCheckpointData ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetQueueCheckpointData2NV( queue, pCheckpointDataCount, pCheckpointData );
    }

    //=== VK_KHR_copy_commands2 ===

    void vkCmdCopyBuffer2KHR( VkCommandBuffer commandBuffer, const VkCopyBufferInfo2 * pCopyBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBuffer2KHR( commandBuffer, pCopyBufferInfo );
    }

    void vkCmdCopyImage2KHR( VkCommandBuffer commandBuffer, const VkCopyImageInfo2 * pCopyImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImage2KHR( commandBuffer, pCopyImageInfo );
    }

    void vkCmdCopyBufferToImage2KHR( VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2 * pCopyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyBufferToImage2KHR( commandBuffer, pCopyBufferToImageInfo );
    }

    void vkCmdCopyImageToBuffer2KHR( VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2 * pCopyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdCopyImageToBuffer2KHR( commandBuffer, pCopyImageToBufferInfo );
    }

    void vkCmdBlitImage2KHR( VkCommandBuffer commandBuffer, const VkBlitImageInfo2 * pBlitImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdBlitImage2KHR( commandBuffer, pBlitImageInfo );
    }

    void vkCmdResolveImage2KHR( VkCommandBuffer commandBuffer, const VkResolveImageInfo2 * pResolveImageInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdResolveImage2KHR( commandBuffer, pResolveImageInfo );
    }

    //=== VK_EXT_vertex_input_dynamic_state ===

    void vkCmdSetVertexInputEXT( VkCommandBuffer                               commandBuffer,
                                 uint32_t                                      vertexBindingDescriptionCount,
                                 const VkVertexInputBindingDescription2EXT *   pVertexBindingDescriptions,
                                 uint32_t                                      vertexAttributeDescriptionCount,
                                 const VkVertexInputAttributeDescription2EXT * pVertexAttributeDescriptions ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetVertexInputEXT(
        commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions );
    }

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync ===

    VkResult vkGetFenceSciSyncFenceNV( VkDevice device, const VkFenceGetSciSyncInfoNV * pGetSciSyncHandleInfo, void * pHandle ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetFenceSciSyncFenceNV( device, pGetSciSyncHandleInfo, pHandle );
    }

    VkResult vkGetFenceSciSyncObjNV( VkDevice device, const VkFenceGetSciSyncInfoNV * pGetSciSyncHandleInfo, void * pHandle ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetFenceSciSyncObjNV( device, pGetSciSyncHandleInfo, pHandle );
    }

    VkResult vkImportFenceSciSyncFenceNV( VkDevice device, const VkImportFenceSciSyncInfoNV * pImportFenceSciSyncInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkImportFenceSciSyncFenceNV( device, pImportFenceSciSyncInfo );
    }

    VkResult vkImportFenceSciSyncObjNV( VkDevice device, const VkImportFenceSciSyncInfoNV * pImportFenceSciSyncInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkImportFenceSciSyncObjNV( device, pImportFenceSciSyncInfo );
    }

    VkResult vkGetPhysicalDeviceSciSyncAttributesNV( VkPhysicalDevice                  physicalDevice,
                                                     const VkSciSyncAttributesInfoNV * pSciSyncAttributesInfo,
                                                     NvSciSyncAttrList                 pAttributes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSciSyncAttributesNV( physicalDevice, pSciSyncAttributesInfo, pAttributes );
    }

    VkResult vkGetSemaphoreSciSyncObjNV( VkDevice device, const VkSemaphoreGetSciSyncInfoNV * pGetSciSyncInfo, void * pHandle ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetSemaphoreSciSyncObjNV( device, pGetSciSyncInfo, pHandle );
    }

    VkResult vkImportSemaphoreSciSyncObjNV( VkDevice device, const VkImportSemaphoreSciSyncInfoNV * pImportSemaphoreSciSyncInfo ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkImportSemaphoreSciSyncObjNV( device, pImportSemaphoreSciSyncInfo );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_memory_sci_buf ===

    VkResult vkGetMemorySciBufNV( VkDevice device, const VkMemoryGetSciBufInfoNV * pGetSciBufInfo, NvSciBufObj * pHandle ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetMemorySciBufNV( device, pGetSciBufInfo, pHandle );
    }

    VkResult vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( VkPhysicalDevice                   physicalDevice,
                                                                  VkExternalMemoryHandleTypeFlagBits handleType,
                                                                  NvSciBufObj                        handle,
                                                                  VkMemorySciBufPropertiesNV *       pMemorySciBufProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( physicalDevice, handleType, handle, pMemorySciBufProperties );
    }

    VkResult vkGetPhysicalDeviceSciBufAttributesNV( VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetPhysicalDeviceSciBufAttributesNV( physicalDevice, pAttributes );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

    //=== VK_EXT_extended_dynamic_state2 ===

    void vkCmdSetPatchControlPointsEXT( VkCommandBuffer commandBuffer, uint32_t patchControlPoints ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetPatchControlPointsEXT( commandBuffer, patchControlPoints );
    }

    void vkCmdSetRasterizerDiscardEnableEXT( VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetRasterizerDiscardEnableEXT( commandBuffer, rasterizerDiscardEnable );
    }

    void vkCmdSetDepthBiasEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetDepthBiasEnableEXT( commandBuffer, depthBiasEnable );
    }

    void vkCmdSetLogicOpEXT( VkCommandBuffer commandBuffer, VkLogicOp logicOp ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetLogicOpEXT( commandBuffer, logicOp );
    }

    void vkCmdSetPrimitiveRestartEnableEXT( VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetPrimitiveRestartEnableEXT( commandBuffer, primitiveRestartEnable );
    }

    //=== VK_EXT_color_write_enable ===

    void vkCmdSetColorWriteEnableEXT( VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32 * pColorWriteEnables ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCmdSetColorWriteEnableEXT( commandBuffer, attachmentCount, pColorWriteEnables );
    }

#  if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync2 ===

    VkResult vkCreateSemaphoreSciSyncPoolNV( VkDevice                                   device,
                                             const VkSemaphoreSciSyncPoolCreateInfoNV * pCreateInfo,
                                             const VkAllocationCallbacks *              pAllocator,
                                             VkSemaphoreSciSyncPoolNV *                 pSemaphorePool ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkCreateSemaphoreSciSyncPoolNV( device, pCreateInfo, pAllocator, pSemaphorePool );
    }
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
    //=== VK_QNX_external_memory_screen_buffer ===

    VkResult vkGetScreenBufferPropertiesQNX( VkDevice                      device,
                                             const struct _screen_buffer * buffer,
                                             VkScreenBufferPropertiesQNX * pProperties ) const VULKAN_HPP_NOEXCEPT
    {
      return ::vkGetScreenBufferPropertiesQNX( device, buffer, pProperties );
    }
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/
  };

  inline ::VULKAN_HPP_NAMESPACE::DispatchLoaderStatic & getDispatchLoaderStatic()
  {
    static ::VULKAN_HPP_NAMESPACE::DispatchLoaderStatic dls;
    return dls;
  }
#endif

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  struct AllocationCallbacks;

  template <typename OwnerType, typename Dispatch>
  class ObjectDestroy
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( OwnerType                                               owner,
                   Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                   Dispatch const & dispatch                               VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {
    }

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
    {
      return m_allocationCallbacks;
    }

    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
    {
      return *m_dispatch;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.destroy( t, m_allocationCallbacks, *m_dispatch );
    }

  private:
    OwnerType                           m_owner               = {};
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };

  class NoParent;

  template <typename Dispatch>
  class ObjectDestroy<NoParent, Dispatch>
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( Optional<const AllocationCallbacks> allocationCallbacks,
                   Dispatch const & dispatch           VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {
    }

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
    {
      return m_allocationCallbacks;
    }

    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
    {
      return *m_dispatch;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_dispatch );
      t.destroy( m_allocationCallbacks, *m_dispatch );
    }

  private:
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };

  template <typename OwnerType, typename Dispatch>
  class ObjectFree
  {
  public:
    ObjectFree() = default;

    ObjectFree( OwnerType                                               owner,
                Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                Dispatch const & dispatch                               VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {
    }

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
    {
      return m_allocationCallbacks;
    }

    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
    {
      return *m_dispatch;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      ( m_owner.free )( t, m_allocationCallbacks, *m_dispatch );
    }

  private:
    OwnerType                           m_owner               = {};
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };

  template <typename OwnerType, typename Dispatch>
  class ObjectRelease
  {
  public:
    ObjectRelease() = default;

    ObjectRelease( OwnerType owner, Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_dispatch( &dispatch )
    {
    }

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
    {
      return *m_dispatch;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.release( t, *m_dispatch );
    }

  private:
    OwnerType        m_owner    = {};
    Dispatch const * m_dispatch = nullptr;
  };

  template <typename OwnerType, typename PoolType, typename Dispatch>
  class PoolFree
  {
  public:
    PoolFree() = default;

    PoolFree( OwnerType owner, PoolType pool, Dispatch const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_pool( pool )
      , m_dispatch( &dispatch )
    {
    }

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

    PoolType getPool() const VULKAN_HPP_NOEXCEPT
    {
      return m_pool;
    }

    Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
    {
      return *m_dispatch;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      ( m_owner.free )( m_pool, t, *m_dispatch );
    }

  private:
    OwnerType        m_owner    = OwnerType();
    PoolType         m_pool     = PoolType();
    Dispatch const * m_dispatch = nullptr;
  };

#endif  // !VULKAN_HPP_NO_SMART_HANDLE

  //==================
  //=== BASE TYPEs ===
  //==================

  using Bool32          = uint32_t;
  using DeviceAddress   = uint64_t;
  using DeviceSize      = uint64_t;
  using RemoteAddressNV = void *;
  using SampleMask      = uint32_t;

}  // namespace VULKAN_HPP_NAMESPACE

#include <vulkan/vulkansc_enums.hpp>
#if !defined( VULKAN_HPP_NO_TO_STRING )
#  include <vulkan/vulkansc_to_string.hpp>
#endif

#ifndef VULKAN_HPP_NO_EXCEPTIONS
namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {
  };
}  // namespace std
#endif

namespace VULKAN_HPP_NAMESPACE
{
#ifndef VULKAN_HPP_NO_EXCEPTIONS
  class ErrorCategoryImpl : public std::error_category
  {
  public:
    virtual const char * name() const VULKAN_HPP_NOEXCEPT override
    {
      return VULKAN_HPP_NAMESPACE_STRING "::Result";
    }

    virtual std::string message( int ev ) const override
    {
#  if defined( VULKAN_HPP_NO_TO_STRING )
      return std::to_string( ev );
#  else
      return VULKAN_HPP_NAMESPACE::to_string( static_cast<VULKAN_HPP_NAMESPACE::Result>( ev ) );
#  endif
    }
  };

  class Error
  {
  public:
    Error() VULKAN_HPP_NOEXCEPT                = default;
    Error( const Error & ) VULKAN_HPP_NOEXCEPT = default;
    virtual ~Error() VULKAN_HPP_NOEXCEPT       = default;

    virtual const char * what() const VULKAN_HPP_NOEXCEPT = 0;
  };

  class LogicError
    : public Error
    , public std::logic_error
  {
  public:
    explicit LogicError( const std::string & what ) : Error(), std::logic_error( what ) {}

    explicit LogicError( char const * what ) : Error(), std::logic_error( what ) {}

    virtual const char * what() const VULKAN_HPP_NOEXCEPT
    {
      return std::logic_error::what();
    }
  };

  class SystemError
    : public Error
    , public std::system_error
  {
  public:
    SystemError( std::error_code ec ) : Error(), std::system_error( ec ) {}

    SystemError( std::error_code ec, std::string const & what ) : Error(), std::system_error( ec, what ) {}

    SystemError( std::error_code ec, char const * what ) : Error(), std::system_error( ec, what ) {}

    SystemError( int ev, std::error_category const & ecat ) : Error(), std::system_error( ev, ecat ) {}

    SystemError( int ev, std::error_category const & ecat, std::string const & what ) : Error(), std::system_error( ev, ecat, what ) {}

    SystemError( int ev, std::error_category const & ecat, char const * what ) : Error(), std::system_error( ev, ecat, what ) {}

    virtual const char * what() const VULKAN_HPP_NOEXCEPT
    {
      return std::system_error::what();
    }
  };

  VULKAN_HPP_INLINE const std::error_category & errorCategory() VULKAN_HPP_NOEXCEPT
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code( Result e ) VULKAN_HPP_NOEXCEPT
  {
    return std::error_code( static_cast<int>( e ), errorCategory() );
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition( Result e ) VULKAN_HPP_NOEXCEPT
  {
    return std::error_condition( static_cast<int>( e ), errorCategory() );
  }

  class OutOfHostMemoryError : public SystemError
  {
  public:
    OutOfHostMemoryError( std::string const & message ) : SystemError( make_error_code( Result::eErrorOutOfHostMemory ), message ) {}

    OutOfHostMemoryError( char const * message ) : SystemError( make_error_code( Result::eErrorOutOfHostMemory ), message ) {}
  };

  class OutOfDeviceMemoryError : public SystemError
  {
  public:
    OutOfDeviceMemoryError( std::string const & message ) : SystemError( make_error_code( Result::eErrorOutOfDeviceMemory ), message ) {}

    OutOfDeviceMemoryError( char const * message ) : SystemError( make_error_code( Result::eErrorOutOfDeviceMemory ), message ) {}
  };

  class InitializationFailedError : public SystemError
  {
  public:
    InitializationFailedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInitializationFailed ), message ) {}

    InitializationFailedError( char const * message ) : SystemError( make_error_code( Result::eErrorInitializationFailed ), message ) {}
  };

  class DeviceLostError : public SystemError
  {
  public:
    DeviceLostError( std::string const & message ) : SystemError( make_error_code( Result::eErrorDeviceLost ), message ) {}

    DeviceLostError( char const * message ) : SystemError( make_error_code( Result::eErrorDeviceLost ), message ) {}
  };

  class MemoryMapFailedError : public SystemError
  {
  public:
    MemoryMapFailedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorMemoryMapFailed ), message ) {}

    MemoryMapFailedError( char const * message ) : SystemError( make_error_code( Result::eErrorMemoryMapFailed ), message ) {}
  };

  class LayerNotPresentError : public SystemError
  {
  public:
    LayerNotPresentError( std::string const & message ) : SystemError( make_error_code( Result::eErrorLayerNotPresent ), message ) {}

    LayerNotPresentError( char const * message ) : SystemError( make_error_code( Result::eErrorLayerNotPresent ), message ) {}
  };

  class ExtensionNotPresentError : public SystemError
  {
  public:
    ExtensionNotPresentError( std::string const & message ) : SystemError( make_error_code( Result::eErrorExtensionNotPresent ), message ) {}

    ExtensionNotPresentError( char const * message ) : SystemError( make_error_code( Result::eErrorExtensionNotPresent ), message ) {}
  };

  class FeatureNotPresentError : public SystemError
  {
  public:
    FeatureNotPresentError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFeatureNotPresent ), message ) {}

    FeatureNotPresentError( char const * message ) : SystemError( make_error_code( Result::eErrorFeatureNotPresent ), message ) {}
  };

  class IncompatibleDriverError : public SystemError
  {
  public:
    IncompatibleDriverError( std::string const & message ) : SystemError( make_error_code( Result::eErrorIncompatibleDriver ), message ) {}

    IncompatibleDriverError( char const * message ) : SystemError( make_error_code( Result::eErrorIncompatibleDriver ), message ) {}
  };

  class TooManyObjectsError : public SystemError
  {
  public:
    TooManyObjectsError( std::string const & message ) : SystemError( make_error_code( Result::eErrorTooManyObjects ), message ) {}

    TooManyObjectsError( char const * message ) : SystemError( make_error_code( Result::eErrorTooManyObjects ), message ) {}
  };

  class FormatNotSupportedError : public SystemError
  {
  public:
    FormatNotSupportedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFormatNotSupported ), message ) {}

    FormatNotSupportedError( char const * message ) : SystemError( make_error_code( Result::eErrorFormatNotSupported ), message ) {}
  };

  class FragmentedPoolError : public SystemError
  {
  public:
    FragmentedPoolError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFragmentedPool ), message ) {}

    FragmentedPoolError( char const * message ) : SystemError( make_error_code( Result::eErrorFragmentedPool ), message ) {}
  };

  class UnknownError : public SystemError
  {
  public:
    UnknownError( std::string const & message ) : SystemError( make_error_code( Result::eErrorUnknown ), message ) {}

    UnknownError( char const * message ) : SystemError( make_error_code( Result::eErrorUnknown ), message ) {}
  };

  class OutOfPoolMemoryError : public SystemError
  {
  public:
    OutOfPoolMemoryError( std::string const & message ) : SystemError( make_error_code( Result::eErrorOutOfPoolMemory ), message ) {}

    OutOfPoolMemoryError( char const * message ) : SystemError( make_error_code( Result::eErrorOutOfPoolMemory ), message ) {}
  };

  class InvalidExternalHandleError : public SystemError
  {
  public:
    InvalidExternalHandleError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidExternalHandle ), message ) {}

    InvalidExternalHandleError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidExternalHandle ), message ) {}
  };

  class FragmentationError : public SystemError
  {
  public:
    FragmentationError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFragmentation ), message ) {}

    FragmentationError( char const * message ) : SystemError( make_error_code( Result::eErrorFragmentation ), message ) {}
  };

  class InvalidOpaqueCaptureAddressError : public SystemError
  {
  public:
    InvalidOpaqueCaptureAddressError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidOpaqueCaptureAddress ), message ) {}

    InvalidOpaqueCaptureAddressError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidOpaqueCaptureAddress ), message ) {}
  };

  class ValidationFailedError : public SystemError
  {
  public:
    ValidationFailedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorValidationFailed ), message ) {}

    ValidationFailedError( char const * message ) : SystemError( make_error_code( Result::eErrorValidationFailed ), message ) {}
  };

  class InvalidPipelineCacheDataError : public SystemError
  {
  public:
    InvalidPipelineCacheDataError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidPipelineCacheData ), message ) {}

    InvalidPipelineCacheDataError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidPipelineCacheData ), message ) {}
  };

  class NoPipelineMatchError : public SystemError
  {
  public:
    NoPipelineMatchError( std::string const & message ) : SystemError( make_error_code( Result::eErrorNoPipelineMatch ), message ) {}

    NoPipelineMatchError( char const * message ) : SystemError( make_error_code( Result::eErrorNoPipelineMatch ), message ) {}
  };

  class SurfaceLostKHRError : public SystemError
  {
  public:
    SurfaceLostKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorSurfaceLostKHR ), message ) {}

    SurfaceLostKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorSurfaceLostKHR ), message ) {}
  };

  class NativeWindowInUseKHRError : public SystemError
  {
  public:
    NativeWindowInUseKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorNativeWindowInUseKHR ), message ) {}

    NativeWindowInUseKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorNativeWindowInUseKHR ), message ) {}
  };

  class OutOfDateKHRError : public SystemError
  {
  public:
    OutOfDateKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorOutOfDateKHR ), message ) {}

    OutOfDateKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorOutOfDateKHR ), message ) {}
  };

  class IncompatibleDisplayKHRError : public SystemError
  {
  public:
    IncompatibleDisplayKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorIncompatibleDisplayKHR ), message ) {}

    IncompatibleDisplayKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorIncompatibleDisplayKHR ), message ) {}
  };

  class InvalidDrmFormatModifierPlaneLayoutEXTError : public SystemError
  {
  public:
    InvalidDrmFormatModifierPlaneLayoutEXTError( std::string const & message )
      : SystemError( make_error_code( Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT ), message )
    {
    }

    InvalidDrmFormatModifierPlaneLayoutEXTError( char const * message )
      : SystemError( make_error_code( Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT ), message )
    {
    }
  };

  namespace detail
  {
    [[noreturn]] VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
    {
      switch ( result )
      {
        case Result::eErrorOutOfHostMemory: throw OutOfHostMemoryError( message );
        case Result::eErrorOutOfDeviceMemory: throw OutOfDeviceMemoryError( message );
        case Result::eErrorInitializationFailed: throw InitializationFailedError( message );
        case Result::eErrorDeviceLost: throw DeviceLostError( message );
        case Result::eErrorMemoryMapFailed: throw MemoryMapFailedError( message );
        case Result::eErrorLayerNotPresent: throw LayerNotPresentError( message );
        case Result::eErrorExtensionNotPresent: throw ExtensionNotPresentError( message );
        case Result::eErrorFeatureNotPresent: throw FeatureNotPresentError( message );
        case Result::eErrorIncompatibleDriver: throw IncompatibleDriverError( message );
        case Result::eErrorTooManyObjects: throw TooManyObjectsError( message );
        case Result::eErrorFormatNotSupported: throw FormatNotSupportedError( message );
        case Result::eErrorFragmentedPool: throw FragmentedPoolError( message );
        case Result::eErrorUnknown: throw UnknownError( message );
        case Result::eErrorOutOfPoolMemory: throw OutOfPoolMemoryError( message );
        case Result::eErrorInvalidExternalHandle: throw InvalidExternalHandleError( message );
        case Result::eErrorFragmentation: throw FragmentationError( message );
        case Result::eErrorInvalidOpaqueCaptureAddress: throw InvalidOpaqueCaptureAddressError( message );
        case Result::eErrorValidationFailed: throw ValidationFailedError( message );
        case Result::eErrorInvalidPipelineCacheData: throw InvalidPipelineCacheDataError( message );
        case Result::eErrorNoPipelineMatch: throw NoPipelineMatchError( message );
        case Result::eErrorSurfaceLostKHR: throw SurfaceLostKHRError( message );
        case Result::eErrorNativeWindowInUseKHR: throw NativeWindowInUseKHRError( message );
        case Result::eErrorOutOfDateKHR: throw OutOfDateKHRError( message );
        case Result::eErrorIncompatibleDisplayKHR: throw IncompatibleDisplayKHRError( message );
        case Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT: throw InvalidDrmFormatModifierPlaneLayoutEXTError( message );
        default: throw SystemError( make_error_code( result ), message );
      }
    }
  }  // namespace detail
#endif

  template <typename T>
  void ignore( T const & ) VULKAN_HPP_NOEXCEPT
  {
  }

  template <typename T>
  struct ResultValue
  {
#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T & v ) VULKAN_HPP_NOEXCEPT( VULKAN_HPP_NOEXCEPT( T( v ) ) )
#else
    ResultValue( Result r, T & v )
#endif
      : result( r ), value( v )
    {
    }

#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T && v ) VULKAN_HPP_NOEXCEPT( VULKAN_HPP_NOEXCEPT( T( std::move( v ) ) ) )
#else
    ResultValue( Result r, T && v )
#endif
      : result( r ), value( std::move( v ) )
    {
    }

    Result result;
    T      value;

    operator std::tuple<Result &, T &>() VULKAN_HPP_NOEXCEPT
    {
      return std::tuple<Result &, T &>( result, value );
    }
  };

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  template <typename Type, typename Dispatch>
  struct ResultValue<UniqueHandle<Type, Dispatch>>
  {
#  ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, UniqueHandle<Type, Dispatch> && v ) VULKAN_HPP_NOEXCEPT
#  else
    ResultValue( Result r, UniqueHandle<Type, Dispatch> && v )
#  endif
      : result( r )
      , value( std::move( v ) )
    {
    }

    VULKAN_HPP_DEPRECATED(
      "asTuple() on an l-value is deprecated, as it implicitly moves the UniqueHandle out of the ResultValue. Use asTuple() on an r-value instead, requiring to explicitly move the UniqueHandle." )

    std::tuple<Result, UniqueHandle<Type, Dispatch>> asTuple() &
    {
      return std::make_tuple( result, std::move( value ) );
    }

    std::tuple<Result, UniqueHandle<Type, Dispatch>> asTuple() &&
    {
      return std::make_tuple( result, std::move( value ) );
    }

    Result                       result;
    UniqueHandle<Type, Dispatch> value;
  };

  template <typename Type, typename Dispatch>
  struct ResultValue<std::vector<UniqueHandle<Type, Dispatch>>>
  {
#  ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, std::vector<UniqueHandle<Type, Dispatch>> && v ) VULKAN_HPP_NOEXCEPT
#  else
    ResultValue( Result r, std::vector<UniqueHandle<Type, Dispatch>> && v )
#  endif
      : result( r )
      , value( std::move( v ) )
    {
    }

    VULKAN_HPP_DEPRECATED(
      "asTuple() on an l-value is deprecated, as it implicitly moves the UniqueHandle out of the ResultValue. Use asTuple() on an r-value instead, requiring to explicitly move the UniqueHandle." )

    std::tuple<Result, std::vector<UniqueHandle<Type, Dispatch>>> asTuple() &
    {
      return std::make_tuple( result, std::move( value ) );
    }

    std::tuple<Result, std::vector<UniqueHandle<Type, Dispatch>>> asTuple() &&
    {
      return std::make_tuple( result, std::move( value ) );
    }

    Result                                    result;
    std::vector<UniqueHandle<Type, Dispatch>> value;
  };
#endif

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef ResultValue<T> type;
#else
    typedef T    type;
#endif
  };

  template <>
  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef Result type;
#else
    typedef void type;
#endif
  };

  VULKAN_HPP_INLINE typename ResultValueType<void>::type createResultValueType( Result result )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    return result;
#else
    ignore( result );
#endif
  }

  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T & data )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    return ResultValue<T>( result, data );
#else
    ignore( result );
    return data;
#endif
  }

  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T && data )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    return ResultValue<T>( result, std::move( data ) );
#else
    ignore( result );
    return std::move( data );
#endif
  }

  VULKAN_HPP_INLINE void resultCheck( Result result, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    ignore( message );
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
#else
    if ( result != Result::eSuccess )
    {
      detail::throwResultException( result, message );
    }
#endif
  }

  VULKAN_HPP_INLINE void resultCheck( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    ignore( message );
    ignore( successCodes );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      detail::throwResultException( result, message );
    }
#endif
  }

  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================

  //=== VK_VERSION_1_0 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t AttachmentUnused          = VK_ATTACHMENT_UNUSED;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t False                     = VK_FALSE;
  VULKAN_HPP_CONSTEXPR_INLINE float    LodClampNone              = VK_LOD_CLAMP_NONE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyIgnored        = VK_QUEUE_FAMILY_IGNORED;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t RemainingArrayLayers      = VK_REMAINING_ARRAY_LAYERS;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t RemainingMipLevels        = VK_REMAINING_MIP_LEVELS;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t SubpassExternal           = VK_SUBPASS_EXTERNAL;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t True                      = VK_TRUE;
  VULKAN_HPP_CONSTEXPR_INLINE uint64_t WholeSize                 = VK_WHOLE_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxMemoryTypes            = VK_MAX_MEMORY_TYPES;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxPhysicalDeviceNameSize = VK_MAX_PHYSICAL_DEVICE_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t UuidSize                  = VK_UUID_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxExtensionNameSize      = VK_MAX_EXTENSION_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDescriptionSize        = VK_MAX_DESCRIPTION_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxMemoryHeaps            = VK_MAX_MEMORY_HEAPS;

  //=== VK_VERSION_1_1 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDeviceGroupSize  = VK_MAX_DEVICE_GROUP_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t LuidSize            = VK_LUID_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyExternal = VK_QUEUE_FAMILY_EXTERNAL;

  //=== VK_VERSION_1_2 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverNameSize = VK_MAX_DRIVER_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverInfoSize = VK_MAX_DRIVER_INFO_SIZE;

  //=== VK_EXT_queue_family_foreign ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyForeignEXT = VK_QUEUE_FAMILY_FOREIGN_EXT;

  //========================
  //=== CONSTEXPR VALUEs ===
  //========================
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t HeaderVersion = VK_HEADER_VERSION;

  //=========================
  //=== CONSTEXPR CALLEEs ===
  //=========================
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t apiVersionMajor( T const version )
  {
    return ( ( (uint32_t)( version ) >> 22U ) & 0x7FU );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t apiVersionMinor( T const version )
  {
    return ( ( (uint32_t)( version ) >> 12U ) & 0x3FFU );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t apiVersionPatch( T const version )
  {
    return ( (uint32_t)(version)&0xFFFU );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t apiVersionVariant( T const version )
  {
    return ( (uint32_t)( version ) >> 29U );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t makeApiVersion( T const variant, T const major, T const minor, T const patch )
  {
    return ( ( ( (uint32_t)( variant ) ) << 29U ) | ( ( (uint32_t)( major ) ) << 22U ) | ( ( (uint32_t)( minor ) ) << 12U ) | ( (uint32_t)( patch ) ) );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_DEPRECATED( "This define is deprecated. VK_MAKE_API_VERSION should be used instead." )
  VULKAN_HPP_CONSTEXPR uint32_t makeVersion( T const major, T const minor, T const patch )
  {
    return ( ( ( (uint32_t)( major ) ) << 22U ) | ( ( (uint32_t)( minor ) ) << 12U ) | ( (uint32_t)( patch ) ) );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_DEPRECATED( "This define is deprecated. VK_API_VERSION_MAJOR should be used instead." )
  VULKAN_HPP_CONSTEXPR uint32_t versionMajor( T const version )
  {
    return ( (uint32_t)( version ) >> 22U );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_DEPRECATED( "This define is deprecated. VK_API_VERSION_MINOR should be used instead." )
  VULKAN_HPP_CONSTEXPR uint32_t versionMinor( T const version )
  {
    return ( ( (uint32_t)( version ) >> 12U ) & 0x3FFU );
  }

  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_DEPRECATED( "This define is deprecated. VK_API_VERSION_PATCH should be used instead." )
  VULKAN_HPP_CONSTEXPR uint32_t versionPatch( T const version )
  {
    return ( (uint32_t)(version)&0xFFFU );
  }

  //=========================
  //=== CONSTEXPR CALLERs ===
  //=========================
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion            = makeApiVersion( 0, 1, 0, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion10          = makeApiVersion( 0, 1, 0, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion11          = makeApiVersion( 0, 1, 1, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion12          = makeApiVersion( 0, 1, 2, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion13          = makeApiVersion( 0, 1, 3, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto HeaderVersionComplete = makeApiVersion( VKSC_API_VARIANT, 1, 0, VK_HEADER_VERSION );

  //=================================
  //=== CONSTEXPR EXTENSION NAMEs ===
  //=================================

  //=== VK_KHR_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceExtensionName = VK_KHR_SURFACE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceSpecVersion   = VK_KHR_SURFACE_SPEC_VERSION;

  //=== VK_KHR_swapchain ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainExtensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainSpecVersion   = VK_KHR_SWAPCHAIN_SPEC_VERSION;

  //=== VK_KHR_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplayExtensionName = VK_KHR_DISPLAY_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySpecVersion   = VK_KHR_DISPLAY_SPEC_VERSION;

  //=== VK_KHR_display_swapchain ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySwapchainExtensionName = VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySwapchainSpecVersion   = VK_KHR_DISPLAY_SWAPCHAIN_SPEC_VERSION;

  //=== VK_EXT_depth_range_unrestricted ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthRangeUnrestrictedExtensionName = VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthRangeUnrestrictedSpecVersion   = VK_EXT_DEPTH_RANGE_UNRESTRICTED_SPEC_VERSION;

  //=== VK_NV_private_vendor_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPrivateVendorInfoExtensionName = VK_NV_PRIVATE_VENDOR_INFO_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPrivateVendorInfoSpecVersion   = VK_NV_PRIVATE_VENDOR_INFO_SPEC_VERSION;

  //=== VK_EXT_texture_compression_astc_hdr ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_texture_compression_astc_hdr extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstcHdrExtensionName = VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_texture_compression_astc_hdr extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstcHdrSpecVersion = VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_SPEC_VERSION;

  //=== VK_EXT_astc_decode_mode ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAstcDecodeModeExtensionName = VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAstcDecodeModeSpecVersion   = VK_EXT_ASTC_DECODE_MODE_SPEC_VERSION;

  //=== VK_KHR_external_memory_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryFdExtensionName = VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryFdSpecVersion   = VK_KHR_EXTERNAL_MEMORY_FD_SPEC_VERSION;

  //=== VK_KHR_external_semaphore_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreFdExtensionName = VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreFdSpecVersion   = VK_KHR_EXTERNAL_SEMAPHORE_FD_SPEC_VERSION;

  //=== VK_KHR_incremental_present ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIncrementalPresentExtensionName = VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIncrementalPresentSpecVersion   = VK_KHR_INCREMENTAL_PRESENT_SPEC_VERSION;

  //=== VK_EXT_direct_mode_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectModeDisplayExtensionName = VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectModeDisplaySpecVersion   = VK_EXT_DIRECT_MODE_DISPLAY_SPEC_VERSION;

  //=== VK_EXT_display_surface_counter ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplaySurfaceCounterExtensionName = VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplaySurfaceCounterSpecVersion   = VK_EXT_DISPLAY_SURFACE_COUNTER_SPEC_VERSION;

  //=== VK_EXT_display_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplayControlExtensionName = VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplayControlSpecVersion   = VK_EXT_DISPLAY_CONTROL_SPEC_VERSION;

  //=== VK_EXT_discard_rectangles ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDiscardRectanglesExtensionName = VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDiscardRectanglesSpecVersion   = VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION;

  //=== VK_EXT_conservative_rasterization ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConservativeRasterizationExtensionName = VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConservativeRasterizationSpecVersion   = VK_EXT_CONSERVATIVE_RASTERIZATION_SPEC_VERSION;

  //=== VK_EXT_depth_clip_enable ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipEnableExtensionName = VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipEnableSpecVersion   = VK_EXT_DEPTH_CLIP_ENABLE_SPEC_VERSION;

  //=== VK_EXT_swapchain_colorspace ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainColorSpaceExtensionName = VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainColorSpaceSpecVersion   = VK_EXT_SWAPCHAIN_COLOR_SPACE_SPEC_VERSION;

  //=== VK_EXT_hdr_metadata ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHdrMetadataExtensionName = VK_EXT_HDR_METADATA_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHdrMetadataSpecVersion   = VK_EXT_HDR_METADATA_SPEC_VERSION;

  //=== VK_KHR_shared_presentable_image ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSharedPresentableImageExtensionName = VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSharedPresentableImageSpecVersion   = VK_KHR_SHARED_PRESENTABLE_IMAGE_SPEC_VERSION;

  //=== VK_KHR_external_fence_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceFdExtensionName = VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceFdSpecVersion   = VK_KHR_EXTERNAL_FENCE_FD_SPEC_VERSION;

  //=== VK_KHR_performance_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPerformanceQueryExtensionName = VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPerformanceQuerySpecVersion   = VK_KHR_PERFORMANCE_QUERY_SPEC_VERSION;

  //=== VK_KHR_get_surface_capabilities2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetSurfaceCapabilities2ExtensionName = VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetSurfaceCapabilities2SpecVersion   = VK_KHR_GET_SURFACE_CAPABILITIES_2_SPEC_VERSION;

  //=== VK_KHR_get_display_properties2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetDisplayProperties2ExtensionName = VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetDisplayProperties2SpecVersion   = VK_KHR_GET_DISPLAY_PROPERTIES_2_SPEC_VERSION;

  //=== VK_EXT_external_memory_dma_buf ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryDmaBufExtensionName = VK_EXT_EXTERNAL_MEMORY_DMA_BUF_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryDmaBufSpecVersion   = VK_EXT_EXTERNAL_MEMORY_DMA_BUF_SPEC_VERSION;

  //=== VK_EXT_queue_family_foreign ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTQueueFamilyForeignExtensionName = VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTQueueFamilyForeignSpecVersion   = VK_EXT_QUEUE_FAMILY_FOREIGN_SPEC_VERSION;

  //=== VK_EXT_debug_utils ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugUtilsExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugUtilsSpecVersion   = VK_EXT_DEBUG_UTILS_SPEC_VERSION;

  //=== VK_EXT_shader_stencil_export ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderStencilExportExtensionName = VK_EXT_SHADER_STENCIL_EXPORT_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderStencilExportSpecVersion   = VK_EXT_SHADER_STENCIL_EXPORT_SPEC_VERSION;

  //=== VK_EXT_sample_locations ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSampleLocationsExtensionName = VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSampleLocationsSpecVersion   = VK_EXT_SAMPLE_LOCATIONS_SPEC_VERSION;

  //=== VK_EXT_blend_operation_advanced ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBlendOperationAdvancedExtensionName = VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBlendOperationAdvancedSpecVersion   = VK_EXT_BLEND_OPERATION_ADVANCED_SPEC_VERSION;

  //=== VK_EXT_post_depth_coverage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPostDepthCoverageExtensionName = VK_EXT_POST_DEPTH_COVERAGE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPostDepthCoverageSpecVersion   = VK_EXT_POST_DEPTH_COVERAGE_SPEC_VERSION;

  //=== VK_EXT_image_drm_format_modifier ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageDrmFormatModifierExtensionName = VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageDrmFormatModifierSpecVersion   = VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION;

  //=== VK_EXT_filter_cubic ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFilterCubicExtensionName = VK_EXT_FILTER_CUBIC_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFilterCubicSpecVersion   = VK_EXT_FILTER_CUBIC_SPEC_VERSION;

  //=== VK_EXT_global_priority ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_global_priority extension has been promoted to VK_KHR_global_priority." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPriorityExtensionName = VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_global_priority extension has been promoted to VK_KHR_global_priority." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPrioritySpecVersion = VK_EXT_GLOBAL_PRIORITY_SPEC_VERSION;

  //=== VK_EXT_external_memory_host ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryHostExtensionName = VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryHostSpecVersion   = VK_EXT_EXTERNAL_MEMORY_HOST_SPEC_VERSION;

  //=== VK_KHR_shader_clock ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderClockExtensionName = VK_KHR_SHADER_CLOCK_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderClockSpecVersion   = VK_KHR_SHADER_CLOCK_SPEC_VERSION;

  //=== VK_EXT_calibrated_timestamps ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_calibrated_timestamps extension has been promoted to VK_KHR_calibrated_timestamps." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCalibratedTimestampsExtensionName = VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_calibrated_timestamps extension has been promoted to VK_KHR_calibrated_timestamps." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCalibratedTimestampsSpecVersion = VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION;

  //=== VK_EXT_vertex_attribute_divisor ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_vertex_attribute_divisor extension has been promoted to VK_KHR_vertex_attribute_divisor." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeDivisorExtensionName = VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_vertex_attribute_divisor extension has been promoted to VK_KHR_vertex_attribute_divisor." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeDivisorSpecVersion = VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_SPEC_VERSION;

  //=== VK_KHR_swapchain_mutable_format ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMutableFormatExtensionName = VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMutableFormatSpecVersion   = VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_SPEC_VERSION;

  //=== VK_EXT_pci_bus_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPciBusInfoExtensionName = VK_EXT_PCI_BUS_INFO_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPciBusInfoSpecVersion   = VK_EXT_PCI_BUS_INFO_SPEC_VERSION;

  //=== VK_KHR_shader_terminate_invocation ===
  VULKAN_HPP_DEPRECATED( "The VK_KHR_shader_terminate_invocation extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderTerminateInvocationExtensionName = VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_KHR_shader_terminate_invocation extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderTerminateInvocationSpecVersion = VK_KHR_SHADER_TERMINATE_INVOCATION_SPEC_VERSION;

  //=== VK_EXT_subgroup_size_control ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_subgroup_size_control extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubgroupSizeControlExtensionName = VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_subgroup_size_control extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubgroupSizeControlSpecVersion = VK_EXT_SUBGROUP_SIZE_CONTROL_SPEC_VERSION;

  //=== VK_KHR_fragment_shading_rate ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShadingRateExtensionName = VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShadingRateSpecVersion   = VK_KHR_FRAGMENT_SHADING_RATE_SPEC_VERSION;

  //=== VK_EXT_shader_image_atomic_int64 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderImageAtomicInt64ExtensionName = VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderImageAtomicInt64SpecVersion   = VK_EXT_SHADER_IMAGE_ATOMIC_INT64_SPEC_VERSION;

  //=== VK_EXT_memory_budget ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryBudgetExtensionName = VK_EXT_MEMORY_BUDGET_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryBudgetSpecVersion   = VK_EXT_MEMORY_BUDGET_SPEC_VERSION;

  //=== VK_EXT_validation_features ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_features extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFeaturesExtensionName = VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_features extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFeaturesSpecVersion = VK_EXT_VALIDATION_FEATURES_SPEC_VERSION;

  //=== VK_EXT_fragment_shader_interlock ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentShaderInterlockExtensionName = VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentShaderInterlockSpecVersion   = VK_EXT_FRAGMENT_SHADER_INTERLOCK_SPEC_VERSION;

  //=== VK_EXT_ycbcr_image_arrays ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcrImageArraysExtensionName = VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcrImageArraysSpecVersion   = VK_EXT_YCBCR_IMAGE_ARRAYS_SPEC_VERSION;

  //=== VK_EXT_headless_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHeadlessSurfaceExtensionName = VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHeadlessSurfaceSpecVersion   = VK_EXT_HEADLESS_SURFACE_SPEC_VERSION;

  //=== VK_EXT_line_rasterization ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_line_rasterization extension has been promoted to VK_KHR_line_rasterization." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLineRasterizationExtensionName = VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_line_rasterization extension has been promoted to VK_KHR_line_rasterization." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLineRasterizationSpecVersion = VK_EXT_LINE_RASTERIZATION_SPEC_VERSION;

  //=== VK_EXT_shader_atomic_float ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloatExtensionName = VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloatSpecVersion   = VK_EXT_SHADER_ATOMIC_FLOAT_SPEC_VERSION;

  //=== VK_EXT_index_type_uint8 ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_index_type_uint8 extension has been promoted to VK_KHR_index_type_uint8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTIndexTypeUint8ExtensionName = VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_index_type_uint8 extension has been promoted to VK_KHR_index_type_uint8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTIndexTypeUint8SpecVersion = VK_EXT_INDEX_TYPE_UINT8_SPEC_VERSION;

  //=== VK_EXT_extended_dynamic_state ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_extended_dynamic_state extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicStateExtensionName = VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_extended_dynamic_state extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicStateSpecVersion = VK_EXT_EXTENDED_DYNAMIC_STATE_SPEC_VERSION;

  //=== VK_EXT_shader_demote_to_helper_invocation ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_demote_to_helper_invocation extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderDemoteToHelperInvocationExtensionName = VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_demote_to_helper_invocation extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderDemoteToHelperInvocationSpecVersion = VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_SPEC_VERSION;

  //=== VK_EXT_texel_buffer_alignment ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_texel_buffer_alignment extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTexelBufferAlignmentExtensionName = VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_texel_buffer_alignment extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTexelBufferAlignmentSpecVersion = VK_EXT_TEXEL_BUFFER_ALIGNMENT_SPEC_VERSION;

  //=== VK_EXT_robustness2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRobustness2ExtensionName = VK_EXT_ROBUSTNESS_2_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRobustness2SpecVersion   = VK_EXT_ROBUSTNESS_2_SPEC_VERSION;

  //=== VK_EXT_custom_border_color ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomBorderColorExtensionName = VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomBorderColorSpecVersion   = VK_EXT_CUSTOM_BORDER_COLOR_SPEC_VERSION;

  //=== VK_KHR_object_refresh ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRObjectRefreshExtensionName = VK_KHR_OBJECT_REFRESH_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRObjectRefreshSpecVersion   = VK_KHR_OBJECT_REFRESH_SPEC_VERSION;

  //=== VK_KHR_synchronization2 ===
  VULKAN_HPP_DEPRECATED( "The VK_KHR_synchronization2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSynchronization2ExtensionName = VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_KHR_synchronization2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSynchronization2SpecVersion = VK_KHR_SYNCHRONIZATION_2_SPEC_VERSION;

  //=== VK_EXT_ycbcr_2plane_444_formats ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_ycbcr_2plane_444_formats extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcr2Plane444FormatsExtensionName = VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_ycbcr_2plane_444_formats extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcr2Plane444FormatsSpecVersion = VK_EXT_YCBCR_2PLANE_444_FORMATS_SPEC_VERSION;

  //=== VK_EXT_image_robustness ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_image_robustness extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageRobustnessExtensionName = VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_image_robustness extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageRobustnessSpecVersion = VK_EXT_IMAGE_ROBUSTNESS_SPEC_VERSION;

  //=== VK_KHR_copy_commands2 ===
  VULKAN_HPP_DEPRECATED( "The VK_KHR_copy_commands2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyCommands2ExtensionName = VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_KHR_copy_commands2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyCommands2SpecVersion = VK_KHR_COPY_COMMANDS_2_SPEC_VERSION;

  //=== VK_EXT_4444_formats ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_4444_formats extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXT4444FormatsExtensionName = VK_EXT_4444_FORMATS_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_4444_formats extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXT4444FormatsSpecVersion = VK_EXT_4444_FORMATS_SPEC_VERSION;

  //=== VK_EXT_vertex_input_dynamic_state ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexInputDynamicStateExtensionName = VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexInputDynamicStateSpecVersion   = VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_sci_sync extension has been deprecated by VK_NV_external_sci_sync2." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalSciSyncExtensionName = VK_NV_EXTERNAL_SCI_SYNC_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_sci_sync extension has been deprecated by VK_NV_external_sci_sync2." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalSciSyncSpecVersion = VK_NV_EXTERNAL_SCI_SYNC_SPEC_VERSION;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_memory_sci_buf ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemorySciBufExtensionName = VK_NV_EXTERNAL_MEMORY_SCI_BUF_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemorySciBufSpecVersion   = VK_NV_EXTERNAL_MEMORY_SCI_BUF_SPEC_VERSION;
#endif /*VK_USE_PLATFORM_SCI*/

  //=== VK_EXT_extended_dynamic_state2 ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_extended_dynamic_state2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState2ExtensionName = VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_extended_dynamic_state2 extension has been promoted to core in version 1.3." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState2SpecVersion = VK_EXT_EXTENDED_DYNAMIC_STATE_2_SPEC_VERSION;

  //=== VK_EXT_color_write_enable ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTColorWriteEnableExtensionName = VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTColorWriteEnableSpecVersion   = VK_EXT_COLOR_WRITE_ENABLE_SPEC_VERSION;

  //=== VK_EXT_application_parameters ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTApplicationParametersExtensionName = VK_EXT_APPLICATION_PARAMETERS_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTApplicationParametersSpecVersion   = VK_EXT_APPLICATION_PARAMETERS_SPEC_VERSION;

#if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalSciSync2ExtensionName = VK_NV_EXTERNAL_SCI_SYNC_2_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalSciSync2SpecVersion   = VK_NV_EXTERNAL_SCI_SYNC_2_SPEC_VERSION;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXExternalMemoryScreenBufferExtensionName = VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME;
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXExternalMemoryScreenBufferSpecVersion   = VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_SPEC_VERSION;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

}  // namespace VULKAN_HPP_NAMESPACE

// clang-format off
#include <vulkan/vulkansc_handles.hpp>
#include <vulkan/vulkansc_structs.hpp>
#include <vulkan/vulkansc_funcs.hpp>

// clang-format on

namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )

  //=======================
  //=== STRUCTS EXTENDS ===
  //=======================

  //=== VK_VERSION_1_0 ===
  template <>
  struct StructExtends<PipelineLayoutCreateInfo, BindDescriptorSetsInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushConstantsInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushDescriptorSetInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushDescriptorSetWithTemplateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, SetDescriptorBufferOffsetsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, BindDescriptorBufferEmbeddedSamplersInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VERSION_1_1 ===
  template <>
  struct StructExtends<PhysicalDeviceSubgroupProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevice16BitStorageFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevice16BitStorageFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryDedicatedRequirements, MemoryRequirements2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryDedicatedAllocateInfo, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryAllocateFlagsInfo, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupRenderPassBeginInfo, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupRenderPassBeginInfo, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupCommandBufferBeginInfo, CommandBufferBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupSubmitInfo, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindBufferMemoryDeviceGroupInfo, BindBufferMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindImageMemoryDeviceGroupInfo, BindImageMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupDeviceCreateInfo, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFeatures2, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePointClippingProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassInputAttachmentAspectCreateInfo, RenderPassCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageViewUsageCreateInfo, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineTessellationDomainOriginStateCreateInfo, PipelineTessellationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassMultiviewCreateInfo, RenderPassCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiviewFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiviewFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiviewProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVariablePointersFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVariablePointersFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceProtectedMemoryFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceProtectedMemoryFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceProtectedMemoryProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ProtectedSubmitInfo, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerYcbcrConversionInfo, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerYcbcrConversionInfo, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindImagePlaneMemoryInfo, BindImageMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImagePlaneMemoryRequirementsInfo, ImageMemoryRequirementsInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSamplerYcbcrConversionFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSamplerYcbcrConversionFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerYcbcrConversionImageFormatProperties, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalImageFormatInfo, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalImageFormatProperties, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceIDProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryImageCreateInfo, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryBufferCreateInfo, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMemoryAllocateInfo, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportFenceCreateInfo, FenceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportSemaphoreCreateInfo, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance3Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderDrawParametersFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderDrawParametersFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VERSION_1_2 ===
  template <>
  struct StructExtends<PhysicalDeviceVulkan11Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan11Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan11Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan12Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan12Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan12Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageFormatListCreateInfo, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageFormatListCreateInfo, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageFormatListCreateInfo, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevice8BitStorageFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevice8BitStorageFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDriverProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicInt64Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicInt64Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFloat16Int8Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFloat16Int8Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFloatControlsProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorSetLayoutBindingFlagsCreateInfo, DescriptorSetLayoutCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorIndexingFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorIndexingFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorIndexingProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorSetVariableDescriptorCountAllocateInfo, DescriptorSetAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorSetVariableDescriptorCountLayoutSupport, DescriptorSetLayoutSupport>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SubpassDescriptionDepthStencilResolve, SubpassDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthStencilResolveProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceScalarBlockLayoutFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceScalarBlockLayoutFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageStencilUsageCreateInfo, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageStencilUsageCreateInfo, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerReductionModeCreateInfo, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSamplerFilterMinmaxProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkanMemoryModelFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkanMemoryModelFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImagelessFramebufferFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImagelessFramebufferFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FramebufferAttachmentsCreateInfo, FramebufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassAttachmentBeginInfo, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceUniformBufferStandardLayoutFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceUniformBufferStandardLayoutFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupExtendedTypesFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupExtendedTypesFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSeparateDepthStencilLayoutsFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSeparateDepthStencilLayoutsFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AttachmentReferenceStencilLayout, AttachmentReference2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AttachmentDescriptionStencilLayout, AttachmentDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHostQueryResetFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHostQueryResetFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTimelineSemaphoreFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTimelineSemaphoreFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTimelineSemaphoreProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SemaphoreTypeCreateInfo, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SemaphoreTypeCreateInfo, PhysicalDeviceExternalSemaphoreInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TimelineSemaphoreSubmitInfo, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TimelineSemaphoreSubmitInfo, BindSparseInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBufferDeviceAddressFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBufferDeviceAddressFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferOpaqueCaptureAddressCreateInfo, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryOpaqueCaptureAddressAllocateInfo, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VERSION_1_3 ===
  template <>
  struct StructExtends<PhysicalDeviceVulkan13Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan13Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan13Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, RayTracingPipelineCreateInfoNV>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, ExecutionGraphPipelineCreateInfoAMDX>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderTerminateInvocationFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderTerminateInvocationFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderDemoteToHelperInvocationFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderDemoteToHelperInvocationFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePrivateDataFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePrivateDataFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DevicePrivateDataCreateInfo, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineCreationCacheControlFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineCreationCacheControlFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryBarrier2, SubpassDependency2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSynchronization2Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSynchronization2Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageRobustnessFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageRobustnessFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubgroupSizeControlFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubgroupSizeControlFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubgroupSizeControlProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineShaderStageRequiredSubgroupSizeCreateInfo, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineShaderStageRequiredSubgroupSizeCreateInfo, ShaderCreateInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInlineUniformBlockFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInlineUniformBlockFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInlineUniformBlockProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<WriteDescriptorSetInlineUniformBlock, WriteDescriptorSet>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorPoolInlineUniformBlockCreateInfo, DescriptorPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTextureCompressionASTCHDRFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTextureCompressionASTCHDRFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRenderingCreateInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandBufferInheritanceRenderingInfo, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderIntegerDotProductFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderIntegerDotProductFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderIntegerDotProductProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTexelBufferAlignmentProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FormatProperties3, FormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance4Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance4Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance4Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VKSC_VERSION_1_0 ===
  template <>
  struct StructExtends<PhysicalDeviceVulkanSC10Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkanSC10Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkanSC10Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceObjectReservationCreateInfo, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandPoolMemoryReservationCreateInfo, CommandPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FaultCallbackInfo, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineOfflineCreateInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineOfflineCreateInfo, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineOfflineCreateInfo, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineOfflineCreateInfo, RayTracingPipelineCreateInfoNV>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_swapchain ===
  template <>
  struct StructExtends<ImageSwapchainCreateInfoKHR, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindImageMemorySwapchainInfoKHR, BindImageMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupPresentInfoKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceGroupSwapchainCreateInfoKHR, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_display_swapchain ===
  template <>
  struct StructExtends<DisplayPresentInfoKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_astc_decode_mode ===
  template <>
  struct StructExtends<ImageViewASTCDecodeModeEXT, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceASTCDecodeFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceASTCDecodeFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_external_memory_fd ===
  template <>
  struct StructExtends<ImportMemoryFdInfoKHR, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_incremental_present ===
  template <>
  struct StructExtends<PresentRegionsKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_display_control ===
  template <>
  struct StructExtends<SwapchainCounterCreateInfoEXT, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_discard_rectangles ===
  template <>
  struct StructExtends<PhysicalDeviceDiscardRectanglePropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineDiscardRectangleStateCreateInfoEXT, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_conservative_rasterization ===
  template <>
  struct StructExtends<PhysicalDeviceConservativeRasterizationPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRasterizationConservativeStateCreateInfoEXT, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_depth_clip_enable ===
  template <>
  struct StructExtends<PhysicalDeviceDepthClipEnableFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthClipEnableFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRasterizationDepthClipStateCreateInfoEXT, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shared_presentable_image ===
  template <>
  struct StructExtends<SharedPresentSurfaceCapabilitiesKHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_performance_query ===
  template <>
  struct StructExtends<PhysicalDevicePerformanceQueryFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePerformanceQueryFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePerformanceQueryPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueryPoolPerformanceCreateInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PerformanceQuerySubmitInfoKHR, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PerformanceQuerySubmitInfoKHR, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PerformanceQueryReservationInfoKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_debug_utils ===
  template <>
  struct StructExtends<DebugUtilsMessengerCreateInfoEXT, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DebugUtilsObjectNameInfoEXT, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_sample_locations ===
  template <>
  struct StructExtends<SampleLocationsInfoEXT, ImageMemoryBarrier>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SampleLocationsInfoEXT, ImageMemoryBarrier2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassSampleLocationsBeginInfoEXT, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineSampleLocationsStateCreateInfoEXT, PipelineMultisampleStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSampleLocationsPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_blend_operation_advanced ===
  template <>
  struct StructExtends<PhysicalDeviceBlendOperationAdvancedFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBlendOperationAdvancedFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBlendOperationAdvancedPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineColorBlendAdvancedStateCreateInfoEXT, PipelineColorBlendStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_drm_format_modifier ===
  template <>
  struct StructExtends<DrmFormatModifierPropertiesListEXT, FormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageDrmFormatModifierInfoEXT, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageDrmFormatModifierListCreateInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageDrmFormatModifierExplicitCreateInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DrmFormatModifierPropertiesList2EXT, FormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_filter_cubic ===
  template <>
  struct StructExtends<PhysicalDeviceImageViewImageFormatInfoEXT, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FilterCubicImageViewImageFormatPropertiesEXT, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_external_memory_host ===
  template <>
  struct StructExtends<ImportMemoryHostPointerInfoEXT, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemoryHostPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_clock ===
  template <>
  struct StructExtends<PhysicalDeviceShaderClockFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderClockFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_vertex_attribute_divisor ===
  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeDivisorPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_pci_bus_info ===
  template <>
  struct StructExtends<PhysicalDevicePCIBusInfoPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_fragment_shading_rate ===
  template <>
  struct StructExtends<FragmentShadingRateAttachmentInfoKHR, SubpassDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineFragmentShadingRateStateCreateInfoKHR, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRateFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRateFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRatePropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_image_atomic_int64 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderImageAtomicInt64FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderImageAtomicInt64FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_memory_budget ===
  template <>
  struct StructExtends<PhysicalDeviceMemoryBudgetPropertiesEXT, PhysicalDeviceMemoryProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_validation_features ===
  template <>
  struct StructExtends<ValidationFeaturesEXT, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_fragment_shader_interlock ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentShaderInterlockFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShaderInterlockFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_ycbcr_image_arrays ===
  template <>
  struct StructExtends<PhysicalDeviceYcbcrImageArraysFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceYcbcrImageArraysFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_atomic_float ===
  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloatFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloatFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_extended_dynamic_state ===
  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicStateFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicStateFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_texel_buffer_alignment ===
  template <>
  struct StructExtends<PhysicalDeviceTexelBufferAlignmentFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTexelBufferAlignmentFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_robustness2 ===
  template <>
  struct StructExtends<PhysicalDeviceRobustness2FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRobustness2FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRobustness2PropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_custom_border_color ===
  template <>
  struct StructExtends<SamplerCustomBorderColorCreateInfoEXT, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCustomBorderColorPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCustomBorderColorFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCustomBorderColorFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_synchronization2 ===
  template <>
  struct StructExtends<QueueFamilyCheckpointProperties2NV, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_ycbcr_2plane_444_formats ===
  template <>
  struct StructExtends<PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_4444_formats ===
  template <>
  struct StructExtends<PhysicalDevice4444FormatsFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevice4444FormatsFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_vertex_input_dynamic_state ===
  template <>
  struct StructExtends<PhysicalDeviceVertexInputDynamicStateFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVertexInputDynamicStateFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync ===
  template <>
  struct StructExtends<ExportFenceSciSyncInfoNV, FenceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportSemaphoreSciSyncInfoNV, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalSciSyncFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalSciSyncFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_memory_sci_buf ===
  template <>
  struct StructExtends<ExportMemorySciBufInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMemorySciBufInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemorySciBufFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemorySciBufFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_SCI*/

  //=== VK_EXT_extended_dynamic_state2 ===
  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicState2FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicState2FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_color_write_enable ===
  template <>
  struct StructExtends<PhysicalDeviceColorWriteEnableFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceColorWriteEnableFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineColorWriteCreateInfoEXT, PipelineColorBlendStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_application_parameters ===
  template <>
  struct StructExtends<ApplicationParametersEXT, ApplicationInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ApplicationParametersEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===
  template <>
  struct StructExtends<PhysicalDeviceExternalSciSync2FeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalSciSync2FeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SemaphoreSciSyncCreateInfoNV, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceSemaphoreSciSyncPoolReservationCreateInfoNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_SCI*/

#  if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===
  template <>
  struct StructExtends<ScreenBufferFormatPropertiesQNX, ScreenBufferPropertiesQNX>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportScreenBufferInfoQNX, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatQNX, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatQNX, SamplerYcbcrConversionCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_SCREEN_QNX*/

#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
  class DynamicLoader
  {
  public:
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    DynamicLoader( std::string const & vulkanLibraryName = {} ) VULKAN_HPP_NOEXCEPT
#  else
    DynamicLoader( std::string const & vulkanLibraryName = {} )
#  endif
    {
      if ( !vulkanLibraryName.empty() )
      {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
        m_library = dlopen( vulkanLibraryName.c_str(), RTLD_NOW | RTLD_LOCAL );
#  elif defined( _WIN32 )
        m_library = ::LoadLibraryA( vulkanLibraryName.c_str() );
#  else
#    error unsupported platform
#  endif
      }
      else
      {
#  if defined( __unix__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
        m_library = dlopen( "libvulkan.so", RTLD_NOW | RTLD_LOCAL );
        if ( m_library == nullptr )
        {
          m_library = dlopen( "libvulkan.so.1", RTLD_NOW | RTLD_LOCAL );
        }
#  elif defined( __APPLE__ )
        m_library = dlopen( "libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
        if ( m_library == nullptr )
        {
          m_library = dlopen( "libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL );
        }
#  elif defined( _WIN32 )
        m_library = ::LoadLibraryA( "vulkan-1.dll" );
#  else
#    error unsupported platform
#  endif
      }

#  ifndef VULKAN_HPP_NO_EXCEPTIONS
      if ( m_library == nullptr )
      {
        // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
        throw std::runtime_error( "Failed to load vulkan library!" );
      }
#  endif
    }

    DynamicLoader( DynamicLoader const & ) = delete;

    DynamicLoader( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT : m_library( other.m_library )
    {
      other.m_library = nullptr;
    }

    DynamicLoader & operator=( DynamicLoader const & ) = delete;

    DynamicLoader & operator=( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
    {
      std::swap( m_library, other.m_library );
      return *this;
    }

    ~DynamicLoader() VULKAN_HPP_NOEXCEPT
    {
      if ( m_library )
      {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
        dlclose( m_library );
#  elif defined( _WIN32 )
        ::FreeLibrary( m_library );
#  else
#    error unsupported platform
#  endif
      }
    }

    template <typename T>
    T getProcAddress( const char * function ) const VULKAN_HPP_NOEXCEPT
    {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
      return (T)dlsym( m_library, function );
#  elif defined( _WIN32 )
      return ( T )::GetProcAddress( m_library, function );
#  else
#    error unsupported platform
#  endif
    }

    bool success() const VULKAN_HPP_NOEXCEPT
    {
      return m_library != nullptr;
    }

  private:
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
    void * m_library;
#  elif defined( _WIN32 )
    ::HINSTANCE m_library;
#  else
#    error unsupported platform
#  endif
  };
#endif

  using PFN_dummy = void ( * )();

  class DispatchLoaderDynamic : public DispatchLoaderBase
  {
  public:
    //=== VK_VERSION_1_0 ===
    PFN_vkCreateInstance                         vkCreateInstance                         = 0;
    PFN_vkDestroyInstance                        vkDestroyInstance                        = 0;
    PFN_vkEnumeratePhysicalDevices               vkEnumeratePhysicalDevices               = 0;
    PFN_vkGetPhysicalDeviceFeatures              vkGetPhysicalDeviceFeatures              = 0;
    PFN_vkGetPhysicalDeviceFormatProperties      vkGetPhysicalDeviceFormatProperties      = 0;
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = 0;
    PFN_vkGetPhysicalDeviceProperties            vkGetPhysicalDeviceProperties            = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = 0;
    PFN_vkGetPhysicalDeviceMemoryProperties      vkGetPhysicalDeviceMemoryProperties      = 0;
    PFN_vkGetInstanceProcAddr                    vkGetInstanceProcAddr                    = 0;
    PFN_vkGetDeviceProcAddr                      vkGetDeviceProcAddr                      = 0;
    PFN_vkCreateDevice                           vkCreateDevice                           = 0;
    PFN_vkDestroyDevice                          vkDestroyDevice                          = 0;
    PFN_vkEnumerateInstanceExtensionProperties   vkEnumerateInstanceExtensionProperties   = 0;
    PFN_vkEnumerateDeviceExtensionProperties     vkEnumerateDeviceExtensionProperties     = 0;
    PFN_vkEnumerateInstanceLayerProperties       vkEnumerateInstanceLayerProperties       = 0;
    PFN_vkEnumerateDeviceLayerProperties         vkEnumerateDeviceLayerProperties         = 0;
    PFN_vkGetDeviceQueue                         vkGetDeviceQueue                         = 0;
    PFN_vkQueueSubmit                            vkQueueSubmit                            = 0;
    PFN_vkQueueWaitIdle                          vkQueueWaitIdle                          = 0;
    PFN_vkDeviceWaitIdle                         vkDeviceWaitIdle                         = 0;
    PFN_vkAllocateMemory                         vkAllocateMemory                         = 0;
    PFN_vkMapMemory                              vkMapMemory                              = 0;
    PFN_vkUnmapMemory                            vkUnmapMemory                            = 0;
    PFN_vkFlushMappedMemoryRanges                vkFlushMappedMemoryRanges                = 0;
    PFN_vkInvalidateMappedMemoryRanges           vkInvalidateMappedMemoryRanges           = 0;
    PFN_vkGetDeviceMemoryCommitment              vkGetDeviceMemoryCommitment              = 0;
    PFN_vkBindBufferMemory                       vkBindBufferMemory                       = 0;
    PFN_vkBindImageMemory                        vkBindImageMemory                        = 0;
    PFN_vkGetBufferMemoryRequirements            vkGetBufferMemoryRequirements            = 0;
    PFN_vkGetImageMemoryRequirements             vkGetImageMemoryRequirements             = 0;
    PFN_vkCreateFence                            vkCreateFence                            = 0;
    PFN_vkDestroyFence                           vkDestroyFence                           = 0;
    PFN_vkResetFences                            vkResetFences                            = 0;
    PFN_vkGetFenceStatus                         vkGetFenceStatus                         = 0;
    PFN_vkWaitForFences                          vkWaitForFences                          = 0;
    PFN_vkCreateSemaphore                        vkCreateSemaphore                        = 0;
    PFN_vkDestroySemaphore                       vkDestroySemaphore                       = 0;
    PFN_vkCreateEvent                            vkCreateEvent                            = 0;
    PFN_vkDestroyEvent                           vkDestroyEvent                           = 0;
    PFN_vkGetEventStatus                         vkGetEventStatus                         = 0;
    PFN_vkSetEvent                               vkSetEvent                               = 0;
    PFN_vkResetEvent                             vkResetEvent                             = 0;
    PFN_vkCreateQueryPool                        vkCreateQueryPool                        = 0;
    PFN_vkGetQueryPoolResults                    vkGetQueryPoolResults                    = 0;
    PFN_vkCreateBuffer                           vkCreateBuffer                           = 0;
    PFN_vkDestroyBuffer                          vkDestroyBuffer                          = 0;
    PFN_vkCreateBufferView                       vkCreateBufferView                       = 0;
    PFN_vkDestroyBufferView                      vkDestroyBufferView                      = 0;
    PFN_vkCreateImage                            vkCreateImage                            = 0;
    PFN_vkDestroyImage                           vkDestroyImage                           = 0;
    PFN_vkGetImageSubresourceLayout              vkGetImageSubresourceLayout              = 0;
    PFN_vkCreateImageView                        vkCreateImageView                        = 0;
    PFN_vkDestroyImageView                       vkDestroyImageView                       = 0;
    PFN_vkCreatePipelineCache                    vkCreatePipelineCache                    = 0;
    PFN_vkDestroyPipelineCache                   vkDestroyPipelineCache                   = 0;
    PFN_vkCreateGraphicsPipelines                vkCreateGraphicsPipelines                = 0;
    PFN_vkCreateComputePipelines                 vkCreateComputePipelines                 = 0;
    PFN_vkDestroyPipeline                        vkDestroyPipeline                        = 0;
    PFN_vkCreatePipelineLayout                   vkCreatePipelineLayout                   = 0;
    PFN_vkDestroyPipelineLayout                  vkDestroyPipelineLayout                  = 0;
    PFN_vkCreateSampler                          vkCreateSampler                          = 0;
    PFN_vkDestroySampler                         vkDestroySampler                         = 0;
    PFN_vkCreateDescriptorSetLayout              vkCreateDescriptorSetLayout              = 0;
    PFN_vkDestroyDescriptorSetLayout             vkDestroyDescriptorSetLayout             = 0;
    PFN_vkCreateDescriptorPool                   vkCreateDescriptorPool                   = 0;
    PFN_vkResetDescriptorPool                    vkResetDescriptorPool                    = 0;
    PFN_vkAllocateDescriptorSets                 vkAllocateDescriptorSets                 = 0;
    PFN_vkFreeDescriptorSets                     vkFreeDescriptorSets                     = 0;
    PFN_vkUpdateDescriptorSets                   vkUpdateDescriptorSets                   = 0;
    PFN_vkCreateFramebuffer                      vkCreateFramebuffer                      = 0;
    PFN_vkDestroyFramebuffer                     vkDestroyFramebuffer                     = 0;
    PFN_vkCreateRenderPass                       vkCreateRenderPass                       = 0;
    PFN_vkDestroyRenderPass                      vkDestroyRenderPass                      = 0;
    PFN_vkGetRenderAreaGranularity               vkGetRenderAreaGranularity               = 0;
    PFN_vkCreateCommandPool                      vkCreateCommandPool                      = 0;
    PFN_vkResetCommandPool                       vkResetCommandPool                       = 0;
    PFN_vkAllocateCommandBuffers                 vkAllocateCommandBuffers                 = 0;
    PFN_vkFreeCommandBuffers                     vkFreeCommandBuffers                     = 0;
    PFN_vkBeginCommandBuffer                     vkBeginCommandBuffer                     = 0;
    PFN_vkEndCommandBuffer                       vkEndCommandBuffer                       = 0;
    PFN_vkResetCommandBuffer                     vkResetCommandBuffer                     = 0;
    PFN_vkCmdBindPipeline                        vkCmdBindPipeline                        = 0;
    PFN_vkCmdSetViewport                         vkCmdSetViewport                         = 0;
    PFN_vkCmdSetScissor                          vkCmdSetScissor                          = 0;
    PFN_vkCmdSetLineWidth                        vkCmdSetLineWidth                        = 0;
    PFN_vkCmdSetDepthBias                        vkCmdSetDepthBias                        = 0;
    PFN_vkCmdSetBlendConstants                   vkCmdSetBlendConstants                   = 0;
    PFN_vkCmdSetDepthBounds                      vkCmdSetDepthBounds                      = 0;
    PFN_vkCmdSetStencilCompareMask               vkCmdSetStencilCompareMask               = 0;
    PFN_vkCmdSetStencilWriteMask                 vkCmdSetStencilWriteMask                 = 0;
    PFN_vkCmdSetStencilReference                 vkCmdSetStencilReference                 = 0;
    PFN_vkCmdBindDescriptorSets                  vkCmdBindDescriptorSets                  = 0;
    PFN_vkCmdBindIndexBuffer                     vkCmdBindIndexBuffer                     = 0;
    PFN_vkCmdBindVertexBuffers                   vkCmdBindVertexBuffers                   = 0;
    PFN_vkCmdDraw                                vkCmdDraw                                = 0;
    PFN_vkCmdDrawIndexed                         vkCmdDrawIndexed                         = 0;
    PFN_vkCmdDrawIndirect                        vkCmdDrawIndirect                        = 0;
    PFN_vkCmdDrawIndexedIndirect                 vkCmdDrawIndexedIndirect                 = 0;
    PFN_vkCmdDispatch                            vkCmdDispatch                            = 0;
    PFN_vkCmdDispatchIndirect                    vkCmdDispatchIndirect                    = 0;
    PFN_vkCmdCopyBuffer                          vkCmdCopyBuffer                          = 0;
    PFN_vkCmdCopyImage                           vkCmdCopyImage                           = 0;
    PFN_vkCmdBlitImage                           vkCmdBlitImage                           = 0;
    PFN_vkCmdCopyBufferToImage                   vkCmdCopyBufferToImage                   = 0;
    PFN_vkCmdCopyImageToBuffer                   vkCmdCopyImageToBuffer                   = 0;
    PFN_vkCmdUpdateBuffer                        vkCmdUpdateBuffer                        = 0;
    PFN_vkCmdFillBuffer                          vkCmdFillBuffer                          = 0;
    PFN_vkCmdClearColorImage                     vkCmdClearColorImage                     = 0;
    PFN_vkCmdClearDepthStencilImage              vkCmdClearDepthStencilImage              = 0;
    PFN_vkCmdClearAttachments                    vkCmdClearAttachments                    = 0;
    PFN_vkCmdResolveImage                        vkCmdResolveImage                        = 0;
    PFN_vkCmdSetEvent                            vkCmdSetEvent                            = 0;
    PFN_vkCmdResetEvent                          vkCmdResetEvent                          = 0;
    PFN_vkCmdWaitEvents                          vkCmdWaitEvents                          = 0;
    PFN_vkCmdPipelineBarrier                     vkCmdPipelineBarrier                     = 0;
    PFN_vkCmdBeginQuery                          vkCmdBeginQuery                          = 0;
    PFN_vkCmdEndQuery                            vkCmdEndQuery                            = 0;
    PFN_vkCmdResetQueryPool                      vkCmdResetQueryPool                      = 0;
    PFN_vkCmdWriteTimestamp                      vkCmdWriteTimestamp                      = 0;
    PFN_vkCmdCopyQueryPoolResults                vkCmdCopyQueryPoolResults                = 0;
    PFN_vkCmdPushConstants                       vkCmdPushConstants                       = 0;
    PFN_vkCmdBeginRenderPass                     vkCmdBeginRenderPass                     = 0;
    PFN_vkCmdNextSubpass                         vkCmdNextSubpass                         = 0;
    PFN_vkCmdEndRenderPass                       vkCmdEndRenderPass                       = 0;
    PFN_vkCmdExecuteCommands                     vkCmdExecuteCommands                     = 0;

    //=== VK_VERSION_1_1 ===
    PFN_vkEnumerateInstanceVersion                     vkEnumerateInstanceVersion                     = 0;
    PFN_vkBindBufferMemory2                            vkBindBufferMemory2                            = 0;
    PFN_vkBindImageMemory2                             vkBindImageMemory2                             = 0;
    PFN_vkGetDeviceGroupPeerMemoryFeatures             vkGetDeviceGroupPeerMemoryFeatures             = 0;
    PFN_vkCmdSetDeviceMask                             vkCmdSetDeviceMask                             = 0;
    PFN_vkCmdDispatchBase                              vkCmdDispatchBase                              = 0;
    PFN_vkEnumeratePhysicalDeviceGroups                vkEnumeratePhysicalDeviceGroups                = 0;
    PFN_vkGetImageMemoryRequirements2                  vkGetImageMemoryRequirements2                  = 0;
    PFN_vkGetBufferMemoryRequirements2                 vkGetBufferMemoryRequirements2                 = 0;
    PFN_vkGetPhysicalDeviceFeatures2                   vkGetPhysicalDeviceFeatures2                   = 0;
    PFN_vkGetPhysicalDeviceProperties2                 vkGetPhysicalDeviceProperties2                 = 0;
    PFN_vkGetPhysicalDeviceFormatProperties2           vkGetPhysicalDeviceFormatProperties2           = 0;
    PFN_vkGetPhysicalDeviceImageFormatProperties2      vkGetPhysicalDeviceImageFormatProperties2      = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2      vkGetPhysicalDeviceQueueFamilyProperties2      = 0;
    PFN_vkGetPhysicalDeviceMemoryProperties2           vkGetPhysicalDeviceMemoryProperties2           = 0;
    PFN_vkGetDeviceQueue2                              vkGetDeviceQueue2                              = 0;
    PFN_vkCreateSamplerYcbcrConversion                 vkCreateSamplerYcbcrConversion                 = 0;
    PFN_vkDestroySamplerYcbcrConversion                vkDestroySamplerYcbcrConversion                = 0;
    PFN_vkGetPhysicalDeviceExternalBufferProperties    vkGetPhysicalDeviceExternalBufferProperties    = 0;
    PFN_vkGetPhysicalDeviceExternalFenceProperties     vkGetPhysicalDeviceExternalFenceProperties     = 0;
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties = 0;
    PFN_vkGetDescriptorSetLayoutSupport                vkGetDescriptorSetLayoutSupport                = 0;

    //=== VK_VERSION_1_2 ===
    PFN_vkCmdDrawIndirectCount                vkCmdDrawIndirectCount                = 0;
    PFN_vkCmdDrawIndexedIndirectCount         vkCmdDrawIndexedIndirectCount         = 0;
    PFN_vkCreateRenderPass2                   vkCreateRenderPass2                   = 0;
    PFN_vkCmdBeginRenderPass2                 vkCmdBeginRenderPass2                 = 0;
    PFN_vkCmdNextSubpass2                     vkCmdNextSubpass2                     = 0;
    PFN_vkCmdEndRenderPass2                   vkCmdEndRenderPass2                   = 0;
    PFN_vkResetQueryPool                      vkResetQueryPool                      = 0;
    PFN_vkGetSemaphoreCounterValue            vkGetSemaphoreCounterValue            = 0;
    PFN_vkWaitSemaphores                      vkWaitSemaphores                      = 0;
    PFN_vkSignalSemaphore                     vkSignalSemaphore                     = 0;
    PFN_vkGetBufferDeviceAddress              vkGetBufferDeviceAddress              = 0;
    PFN_vkGetBufferOpaqueCaptureAddress       vkGetBufferOpaqueCaptureAddress       = 0;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress = 0;

    //=== VK_VERSION_1_3 ===
    PFN_vkGetPhysicalDeviceToolProperties        vkGetPhysicalDeviceToolProperties        = 0;
    PFN_vkCreatePrivateDataSlot                  vkCreatePrivateDataSlot                  = 0;
    PFN_vkDestroyPrivateDataSlot                 vkDestroyPrivateDataSlot                 = 0;
    PFN_vkSetPrivateData                         vkSetPrivateData                         = 0;
    PFN_vkGetPrivateData                         vkGetPrivateData                         = 0;
    PFN_vkCmdSetEvent2                           vkCmdSetEvent2                           = 0;
    PFN_vkCmdResetEvent2                         vkCmdResetEvent2                         = 0;
    PFN_vkCmdWaitEvents2                         vkCmdWaitEvents2                         = 0;
    PFN_vkCmdPipelineBarrier2                    vkCmdPipelineBarrier2                    = 0;
    PFN_vkCmdWriteTimestamp2                     vkCmdWriteTimestamp2                     = 0;
    PFN_vkQueueSubmit2                           vkQueueSubmit2                           = 0;
    PFN_vkCmdCopyBuffer2                         vkCmdCopyBuffer2                         = 0;
    PFN_vkCmdCopyImage2                          vkCmdCopyImage2                          = 0;
    PFN_vkCmdCopyBufferToImage2                  vkCmdCopyBufferToImage2                  = 0;
    PFN_vkCmdCopyImageToBuffer2                  vkCmdCopyImageToBuffer2                  = 0;
    PFN_vkCmdBlitImage2                          vkCmdBlitImage2                          = 0;
    PFN_vkCmdResolveImage2                       vkCmdResolveImage2                       = 0;
    PFN_vkCmdBeginRendering                      vkCmdBeginRendering                      = 0;
    PFN_vkCmdEndRendering                        vkCmdEndRendering                        = 0;
    PFN_vkCmdSetCullMode                         vkCmdSetCullMode                         = 0;
    PFN_vkCmdSetFrontFace                        vkCmdSetFrontFace                        = 0;
    PFN_vkCmdSetPrimitiveTopology                vkCmdSetPrimitiveTopology                = 0;
    PFN_vkCmdSetViewportWithCount                vkCmdSetViewportWithCount                = 0;
    PFN_vkCmdSetScissorWithCount                 vkCmdSetScissorWithCount                 = 0;
    PFN_vkCmdBindVertexBuffers2                  vkCmdBindVertexBuffers2                  = 0;
    PFN_vkCmdSetDepthTestEnable                  vkCmdSetDepthTestEnable                  = 0;
    PFN_vkCmdSetDepthWriteEnable                 vkCmdSetDepthWriteEnable                 = 0;
    PFN_vkCmdSetDepthCompareOp                   vkCmdSetDepthCompareOp                   = 0;
    PFN_vkCmdSetDepthBoundsTestEnable            vkCmdSetDepthBoundsTestEnable            = 0;
    PFN_vkCmdSetStencilTestEnable                vkCmdSetStencilTestEnable                = 0;
    PFN_vkCmdSetStencilOp                        vkCmdSetStencilOp                        = 0;
    PFN_vkCmdSetRasterizerDiscardEnable          vkCmdSetRasterizerDiscardEnable          = 0;
    PFN_vkCmdSetDepthBiasEnable                  vkCmdSetDepthBiasEnable                  = 0;
    PFN_vkCmdSetPrimitiveRestartEnable           vkCmdSetPrimitiveRestartEnable           = 0;
    PFN_vkGetDeviceBufferMemoryRequirements      vkGetDeviceBufferMemoryRequirements      = 0;
    PFN_vkGetDeviceImageMemoryRequirements       vkGetDeviceImageMemoryRequirements       = 0;
    PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements = 0;

    //=== VKSC_VERSION_1_0 ===
    PFN_vkGetCommandPoolMemoryConsumption vkGetCommandPoolMemoryConsumption = 0;
    PFN_vkGetFaultData                    vkGetFaultData                    = 0;

    //=== VK_KHR_surface ===
    PFN_vkDestroySurfaceKHR                       vkDestroySurfaceKHR                       = 0;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR      = 0;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = 0;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR      = 0;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = 0;

    //=== VK_KHR_swapchain ===
    PFN_vkCreateSwapchainKHR                    vkCreateSwapchainKHR                    = 0;
    PFN_vkGetSwapchainImagesKHR                 vkGetSwapchainImagesKHR                 = 0;
    PFN_vkAcquireNextImageKHR                   vkAcquireNextImageKHR                   = 0;
    PFN_vkQueuePresentKHR                       vkQueuePresentKHR                       = 0;
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR  vkGetDeviceGroupPresentCapabilitiesKHR  = 0;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR  vkGetDeviceGroupSurfacePresentModesKHR  = 0;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = 0;
    PFN_vkAcquireNextImage2KHR                  vkAcquireNextImage2KHR                  = 0;

    //=== VK_KHR_display ===
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR      vkGetPhysicalDeviceDisplayPropertiesKHR      = 0;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = 0;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR        vkGetDisplayPlaneSupportedDisplaysKHR        = 0;
    PFN_vkGetDisplayModePropertiesKHR                vkGetDisplayModePropertiesKHR                = 0;
    PFN_vkCreateDisplayModeKHR                       vkCreateDisplayModeKHR                       = 0;
    PFN_vkGetDisplayPlaneCapabilitiesKHR             vkGetDisplayPlaneCapabilitiesKHR             = 0;
    PFN_vkCreateDisplayPlaneSurfaceKHR               vkCreateDisplayPlaneSurfaceKHR               = 0;

    //=== VK_KHR_display_swapchain ===
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = 0;

    //=== VK_KHR_external_memory_fd ===
    PFN_vkGetMemoryFdKHR           vkGetMemoryFdKHR           = 0;
    PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR = 0;

    //=== VK_KHR_external_semaphore_fd ===
    PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR = 0;
    PFN_vkGetSemaphoreFdKHR    vkGetSemaphoreFdKHR    = 0;

    //=== VK_EXT_direct_mode_display ===
    PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT = 0;

    //=== VK_EXT_display_surface_counter ===
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT = 0;

    //=== VK_EXT_display_control ===
    PFN_vkDisplayPowerControlEXT  vkDisplayPowerControlEXT  = 0;
    PFN_vkRegisterDeviceEventEXT  vkRegisterDeviceEventEXT  = 0;
    PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT = 0;
    PFN_vkGetSwapchainCounterEXT  vkGetSwapchainCounterEXT  = 0;

    //=== VK_EXT_discard_rectangles ===
    PFN_vkCmdSetDiscardRectangleEXT       vkCmdSetDiscardRectangleEXT       = 0;
    PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT = 0;
    PFN_vkCmdSetDiscardRectangleModeEXT   vkCmdSetDiscardRectangleModeEXT   = 0;

    //=== VK_EXT_hdr_metadata ===
    PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT = 0;

    //=== VK_KHR_shared_presentable_image ===
    PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR = 0;

    //=== VK_KHR_external_fence_fd ===
    PFN_vkImportFenceFdKHR vkImportFenceFdKHR = 0;
    PFN_vkGetFenceFdKHR    vkGetFenceFdKHR    = 0;

    //=== VK_KHR_performance_query ===
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         = 0;
    PFN_vkAcquireProfilingLockKHR                                       vkAcquireProfilingLockKHR                                       = 0;
    PFN_vkReleaseProfilingLockKHR                                       vkReleaseProfilingLockKHR                                       = 0;

    //=== VK_KHR_get_surface_capabilities2 ===
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR = 0;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR      vkGetPhysicalDeviceSurfaceFormats2KHR      = 0;

    //=== VK_KHR_get_display_properties2 ===
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR      vkGetPhysicalDeviceDisplayProperties2KHR      = 0;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR = 0;
    PFN_vkGetDisplayModeProperties2KHR                vkGetDisplayModeProperties2KHR                = 0;
    PFN_vkGetDisplayPlaneCapabilities2KHR             vkGetDisplayPlaneCapabilities2KHR             = 0;

    //=== VK_EXT_debug_utils ===
    PFN_vkSetDebugUtilsObjectNameEXT    vkSetDebugUtilsObjectNameEXT    = 0;
    PFN_vkSetDebugUtilsObjectTagEXT     vkSetDebugUtilsObjectTagEXT     = 0;
    PFN_vkQueueBeginDebugUtilsLabelEXT  vkQueueBeginDebugUtilsLabelEXT  = 0;
    PFN_vkQueueEndDebugUtilsLabelEXT    vkQueueEndDebugUtilsLabelEXT    = 0;
    PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = 0;
    PFN_vkCmdBeginDebugUtilsLabelEXT    vkCmdBeginDebugUtilsLabelEXT    = 0;
    PFN_vkCmdEndDebugUtilsLabelEXT      vkCmdEndDebugUtilsLabelEXT      = 0;
    PFN_vkCmdInsertDebugUtilsLabelEXT   vkCmdInsertDebugUtilsLabelEXT   = 0;
    PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT  = 0;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = 0;
    PFN_vkSubmitDebugUtilsMessageEXT    vkSubmitDebugUtilsMessageEXT    = 0;

    //=== VK_EXT_sample_locations ===
    PFN_vkCmdSetSampleLocationsEXT                  vkCmdSetSampleLocationsEXT                  = 0;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT = 0;

    //=== VK_EXT_image_drm_format_modifier ===
    PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT = 0;

    //=== VK_EXT_external_memory_host ===
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = 0;

    //=== VK_EXT_calibrated_timestamps ===
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = 0;
    PFN_vkGetCalibratedTimestampsEXT                   vkGetCalibratedTimestampsEXT                   = 0;

    //=== VK_KHR_fragment_shading_rate ===
    PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR = 0;
    PFN_vkCmdSetFragmentShadingRateKHR             vkCmdSetFragmentShadingRateKHR             = 0;

    //=== VK_EXT_headless_surface ===
    PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT = 0;

    //=== VK_EXT_line_rasterization ===
    PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT = 0;

    //=== VK_EXT_extended_dynamic_state ===
    PFN_vkCmdSetCullModeEXT              vkCmdSetCullModeEXT              = 0;
    PFN_vkCmdSetFrontFaceEXT             vkCmdSetFrontFaceEXT             = 0;
    PFN_vkCmdSetPrimitiveTopologyEXT     vkCmdSetPrimitiveTopologyEXT     = 0;
    PFN_vkCmdSetViewportWithCountEXT     vkCmdSetViewportWithCountEXT     = 0;
    PFN_vkCmdSetScissorWithCountEXT      vkCmdSetScissorWithCountEXT      = 0;
    PFN_vkCmdBindVertexBuffers2EXT       vkCmdBindVertexBuffers2EXT       = 0;
    PFN_vkCmdSetDepthTestEnableEXT       vkCmdSetDepthTestEnableEXT       = 0;
    PFN_vkCmdSetDepthWriteEnableEXT      vkCmdSetDepthWriteEnableEXT      = 0;
    PFN_vkCmdSetDepthCompareOpEXT        vkCmdSetDepthCompareOpEXT        = 0;
    PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT = 0;
    PFN_vkCmdSetStencilTestEnableEXT     vkCmdSetStencilTestEnableEXT     = 0;
    PFN_vkCmdSetStencilOpEXT             vkCmdSetStencilOpEXT             = 0;

    //=== VK_KHR_object_refresh ===
    PFN_vkCmdRefreshObjectsKHR                       vkCmdRefreshObjectsKHR                       = 0;
    PFN_vkGetPhysicalDeviceRefreshableObjectTypesKHR vkGetPhysicalDeviceRefreshableObjectTypesKHR = 0;

    //=== VK_KHR_synchronization2 ===
    PFN_vkCmdSetEvent2KHR           vkCmdSetEvent2KHR           = 0;
    PFN_vkCmdResetEvent2KHR         vkCmdResetEvent2KHR         = 0;
    PFN_vkCmdWaitEvents2KHR         vkCmdWaitEvents2KHR         = 0;
    PFN_vkCmdPipelineBarrier2KHR    vkCmdPipelineBarrier2KHR    = 0;
    PFN_vkCmdWriteTimestamp2KHR     vkCmdWriteTimestamp2KHR     = 0;
    PFN_vkQueueSubmit2KHR           vkQueueSubmit2KHR           = 0;
    PFN_vkCmdWriteBufferMarker2AMD  vkCmdWriteBufferMarker2AMD  = 0;
    PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV = 0;

    //=== VK_KHR_copy_commands2 ===
    PFN_vkCmdCopyBuffer2KHR        vkCmdCopyBuffer2KHR        = 0;
    PFN_vkCmdCopyImage2KHR         vkCmdCopyImage2KHR         = 0;
    PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR = 0;
    PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR = 0;
    PFN_vkCmdBlitImage2KHR         vkCmdBlitImage2KHR         = 0;
    PFN_vkCmdResolveImage2KHR      vkCmdResolveImage2KHR      = 0;

    //=== VK_EXT_vertex_input_dynamic_state ===
    PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT = 0;

#if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync ===
    PFN_vkGetFenceSciSyncFenceNV               vkGetFenceSciSyncFenceNV               = 0;
    PFN_vkGetFenceSciSyncObjNV                 vkGetFenceSciSyncObjNV                 = 0;
    PFN_vkImportFenceSciSyncFenceNV            vkImportFenceSciSyncFenceNV            = 0;
    PFN_vkImportFenceSciSyncObjNV              vkImportFenceSciSyncObjNV              = 0;
    PFN_vkGetPhysicalDeviceSciSyncAttributesNV vkGetPhysicalDeviceSciSyncAttributesNV = 0;
    PFN_vkGetSemaphoreSciSyncObjNV             vkGetSemaphoreSciSyncObjNV             = 0;
    PFN_vkImportSemaphoreSciSyncObjNV          vkImportSemaphoreSciSyncObjNV          = 0;
#else
    PFN_dummy vkGetFenceSciSyncFenceNV_placeholder                            = 0;
    PFN_dummy vkGetFenceSciSyncObjNV_placeholder                              = 0;
    PFN_dummy vkImportFenceSciSyncFenceNV_placeholder                         = 0;
    PFN_dummy vkImportFenceSciSyncObjNV_placeholder                           = 0;
    PFN_dummy vkGetPhysicalDeviceSciSyncAttributesNV_placeholder              = 0;
    PFN_dummy vkGetSemaphoreSciSyncObjNV_placeholder                          = 0;
    PFN_dummy vkImportSemaphoreSciSyncObjNV_placeholder                       = 0;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_memory_sci_buf ===
    PFN_vkGetMemorySciBufNV                                 vkGetMemorySciBufNV                                 = 0;
    PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV = 0;
    PFN_vkGetPhysicalDeviceSciBufAttributesNV               vkGetPhysicalDeviceSciBufAttributesNV               = 0;
#else
    PFN_dummy vkGetMemorySciBufNV_placeholder                                 = 0;
    PFN_dummy vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV_placeholder = 0;
    PFN_dummy vkGetPhysicalDeviceSciBufAttributesNV_placeholder               = 0;
#endif /*VK_USE_PLATFORM_SCI*/

    //=== VK_EXT_extended_dynamic_state2 ===
    PFN_vkCmdSetPatchControlPointsEXT      vkCmdSetPatchControlPointsEXT      = 0;
    PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT = 0;
    PFN_vkCmdSetDepthBiasEnableEXT         vkCmdSetDepthBiasEnableEXT         = 0;
    PFN_vkCmdSetLogicOpEXT                 vkCmdSetLogicOpEXT                 = 0;
    PFN_vkCmdSetPrimitiveRestartEnableEXT  vkCmdSetPrimitiveRestartEnableEXT  = 0;

    //=== VK_EXT_color_write_enable ===
    PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT = 0;

#if defined( VK_USE_PLATFORM_SCI )
    //=== VK_NV_external_sci_sync2 ===
    PFN_vkCreateSemaphoreSciSyncPoolNV vkCreateSemaphoreSciSyncPoolNV = 0;
#else
    PFN_dummy vkCreateSemaphoreSciSyncPoolNV_placeholder                      = 0;
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    //=== VK_QNX_external_memory_screen_buffer ===
    PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX = 0;
#else
    PFN_dummy vkGetScreenBufferPropertiesQNX_placeholder                      = 0;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  public:
    DispatchLoaderDynamic() VULKAN_HPP_NOEXCEPT                                    = default;
    DispatchLoaderDynamic( DispatchLoaderDynamic const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    DispatchLoaderDynamic( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      init( getInstanceProcAddr );
    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                           PFN_vkGetInstanceProcAddr getInstanceProcAddr,
                           VkDevice                  device            = {},
                           PFN_vkGetDeviceProcAddr   getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
    }

    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
              = VULKAN_HPP_NAMESPACE::DynamicLoader
#endif
              >
    void init()
    {
      static DynamicLoader dl;
      init( dl );
    }

    template <typename DynamicLoader>
    void init( DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      init( getInstanceProcAddr );
    }

    void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getInstanceProcAddr );

      vkGetInstanceProcAddr = getInstanceProcAddr;

      //=== VK_VERSION_1_0 ===
      vkCreateInstance = PFN_vkCreateInstance( vkGetInstanceProcAddr( NULL, "vkCreateInstance" ) );
      vkEnumerateInstanceExtensionProperties =
        PFN_vkEnumerateInstanceExtensionProperties( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceExtensionProperties" ) );
      vkEnumerateInstanceLayerProperties = PFN_vkEnumerateInstanceLayerProperties( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceLayerProperties" ) );

      //=== VK_VERSION_1_1 ===
      vkEnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceVersion" ) );
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance                instance,
               PFN_vkGetInstanceProcAddr getInstanceProcAddr,
               VkDevice                  device              = {},
               PFN_vkGetDeviceProcAddr /*getDeviceProcAddr*/ = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( instance && getInstanceProcAddr );
      vkGetInstanceProcAddr = getInstanceProcAddr;
      init( VULKAN_HPP_NAMESPACE::Instance( instance ) );
      if ( device )
      {
        init( VULKAN_HPP_NAMESPACE::Device( device ) );
      }
    }

    void init( VULKAN_HPP_NAMESPACE::Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
    {
      VkInstance instance = static_cast<VkInstance>( instanceCpp );

      //=== VK_VERSION_1_0 ===
      vkDestroyInstance                   = PFN_vkDestroyInstance( vkGetInstanceProcAddr( instance, "vkDestroyInstance" ) );
      vkEnumeratePhysicalDevices          = PFN_vkEnumeratePhysicalDevices( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDevices" ) );
      vkGetPhysicalDeviceFeatures         = PFN_vkGetPhysicalDeviceFeatures( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures" ) );
      vkGetPhysicalDeviceFormatProperties = PFN_vkGetPhysicalDeviceFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties" ) );
      vkGetPhysicalDeviceImageFormatProperties =
        PFN_vkGetPhysicalDeviceImageFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties" ) );
      vkGetPhysicalDeviceProperties = PFN_vkGetPhysicalDeviceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties" ) );
      vkGetPhysicalDeviceQueueFamilyProperties =
        PFN_vkGetPhysicalDeviceQueueFamilyProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties" ) );
      vkGetPhysicalDeviceMemoryProperties = PFN_vkGetPhysicalDeviceMemoryProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties" ) );
      vkGetDeviceProcAddr                 = PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr" ) );
      vkCreateDevice                      = PFN_vkCreateDevice( vkGetInstanceProcAddr( instance, "vkCreateDevice" ) );
      vkDestroyDevice                     = PFN_vkDestroyDevice( vkGetInstanceProcAddr( instance, "vkDestroyDevice" ) );
      vkEnumerateDeviceExtensionProperties =
        PFN_vkEnumerateDeviceExtensionProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceExtensionProperties" ) );
      vkEnumerateDeviceLayerProperties = PFN_vkEnumerateDeviceLayerProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceLayerProperties" ) );
      vkGetDeviceQueue                 = PFN_vkGetDeviceQueue( vkGetInstanceProcAddr( instance, "vkGetDeviceQueue" ) );
      vkQueueSubmit                    = PFN_vkQueueSubmit( vkGetInstanceProcAddr( instance, "vkQueueSubmit" ) );
      vkQueueWaitIdle                  = PFN_vkQueueWaitIdle( vkGetInstanceProcAddr( instance, "vkQueueWaitIdle" ) );
      vkDeviceWaitIdle                 = PFN_vkDeviceWaitIdle( vkGetInstanceProcAddr( instance, "vkDeviceWaitIdle" ) );
      vkAllocateMemory                 = PFN_vkAllocateMemory( vkGetInstanceProcAddr( instance, "vkAllocateMemory" ) );
      vkMapMemory                      = PFN_vkMapMemory( vkGetInstanceProcAddr( instance, "vkMapMemory" ) );
      vkUnmapMemory                    = PFN_vkUnmapMemory( vkGetInstanceProcAddr( instance, "vkUnmapMemory" ) );
      vkFlushMappedMemoryRanges        = PFN_vkFlushMappedMemoryRanges( vkGetInstanceProcAddr( instance, "vkFlushMappedMemoryRanges" ) );
      vkInvalidateMappedMemoryRanges   = PFN_vkInvalidateMappedMemoryRanges( vkGetInstanceProcAddr( instance, "vkInvalidateMappedMemoryRanges" ) );
      vkGetDeviceMemoryCommitment      = PFN_vkGetDeviceMemoryCommitment( vkGetInstanceProcAddr( instance, "vkGetDeviceMemoryCommitment" ) );
      vkBindBufferMemory               = PFN_vkBindBufferMemory( vkGetInstanceProcAddr( instance, "vkBindBufferMemory" ) );
      vkBindImageMemory                = PFN_vkBindImageMemory( vkGetInstanceProcAddr( instance, "vkBindImageMemory" ) );
      vkGetBufferMemoryRequirements    = PFN_vkGetBufferMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetBufferMemoryRequirements" ) );
      vkGetImageMemoryRequirements     = PFN_vkGetImageMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetImageMemoryRequirements" ) );
      vkCreateFence                    = PFN_vkCreateFence( vkGetInstanceProcAddr( instance, "vkCreateFence" ) );
      vkDestroyFence                   = PFN_vkDestroyFence( vkGetInstanceProcAddr( instance, "vkDestroyFence" ) );
      vkResetFences                    = PFN_vkResetFences( vkGetInstanceProcAddr( instance, "vkResetFences" ) );
      vkGetFenceStatus                 = PFN_vkGetFenceStatus( vkGetInstanceProcAddr( instance, "vkGetFenceStatus" ) );
      vkWaitForFences                  = PFN_vkWaitForFences( vkGetInstanceProcAddr( instance, "vkWaitForFences" ) );
      vkCreateSemaphore                = PFN_vkCreateSemaphore( vkGetInstanceProcAddr( instance, "vkCreateSemaphore" ) );
      vkDestroySemaphore               = PFN_vkDestroySemaphore( vkGetInstanceProcAddr( instance, "vkDestroySemaphore" ) );
      vkCreateEvent                    = PFN_vkCreateEvent( vkGetInstanceProcAddr( instance, "vkCreateEvent" ) );
      vkDestroyEvent                   = PFN_vkDestroyEvent( vkGetInstanceProcAddr( instance, "vkDestroyEvent" ) );
      vkGetEventStatus                 = PFN_vkGetEventStatus( vkGetInstanceProcAddr( instance, "vkGetEventStatus" ) );
      vkSetEvent                       = PFN_vkSetEvent( vkGetInstanceProcAddr( instance, "vkSetEvent" ) );
      vkResetEvent                     = PFN_vkResetEvent( vkGetInstanceProcAddr( instance, "vkResetEvent" ) );
      vkCreateQueryPool                = PFN_vkCreateQueryPool( vkGetInstanceProcAddr( instance, "vkCreateQueryPool" ) );
      vkGetQueryPoolResults            = PFN_vkGetQueryPoolResults( vkGetInstanceProcAddr( instance, "vkGetQueryPoolResults" ) );
      vkCreateBuffer                   = PFN_vkCreateBuffer( vkGetInstanceProcAddr( instance, "vkCreateBuffer" ) );
      vkDestroyBuffer                  = PFN_vkDestroyBuffer( vkGetInstanceProcAddr( instance, "vkDestroyBuffer" ) );
      vkCreateBufferView               = PFN_vkCreateBufferView( vkGetInstanceProcAddr( instance, "vkCreateBufferView" ) );
      vkDestroyBufferView              = PFN_vkDestroyBufferView( vkGetInstanceProcAddr( instance, "vkDestroyBufferView" ) );
      vkCreateImage                    = PFN_vkCreateImage( vkGetInstanceProcAddr( instance, "vkCreateImage" ) );
      vkDestroyImage                   = PFN_vkDestroyImage( vkGetInstanceProcAddr( instance, "vkDestroyImage" ) );
      vkGetImageSubresourceLayout      = PFN_vkGetImageSubresourceLayout( vkGetInstanceProcAddr( instance, "vkGetImageSubresourceLayout" ) );
      vkCreateImageView                = PFN_vkCreateImageView( vkGetInstanceProcAddr( instance, "vkCreateImageView" ) );
      vkDestroyImageView               = PFN_vkDestroyImageView( vkGetInstanceProcAddr( instance, "vkDestroyImageView" ) );
      vkCreatePipelineCache            = PFN_vkCreatePipelineCache( vkGetInstanceProcAddr( instance, "vkCreatePipelineCache" ) );
      vkDestroyPipelineCache           = PFN_vkDestroyPipelineCache( vkGetInstanceProcAddr( instance, "vkDestroyPipelineCache" ) );
      vkCreateGraphicsPipelines        = PFN_vkCreateGraphicsPipelines( vkGetInstanceProcAddr( instance, "vkCreateGraphicsPipelines" ) );
      vkCreateComputePipelines         = PFN_vkCreateComputePipelines( vkGetInstanceProcAddr( instance, "vkCreateComputePipelines" ) );
      vkDestroyPipeline                = PFN_vkDestroyPipeline( vkGetInstanceProcAddr( instance, "vkDestroyPipeline" ) );
      vkCreatePipelineLayout           = PFN_vkCreatePipelineLayout( vkGetInstanceProcAddr( instance, "vkCreatePipelineLayout" ) );
      vkDestroyPipelineLayout          = PFN_vkDestroyPipelineLayout( vkGetInstanceProcAddr( instance, "vkDestroyPipelineLayout" ) );
      vkCreateSampler                  = PFN_vkCreateSampler( vkGetInstanceProcAddr( instance, "vkCreateSampler" ) );
      vkDestroySampler                 = PFN_vkDestroySampler( vkGetInstanceProcAddr( instance, "vkDestroySampler" ) );
      vkCreateDescriptorSetLayout      = PFN_vkCreateDescriptorSetLayout( vkGetInstanceProcAddr( instance, "vkCreateDescriptorSetLayout" ) );
      vkDestroyDescriptorSetLayout     = PFN_vkDestroyDescriptorSetLayout( vkGetInstanceProcAddr( instance, "vkDestroyDescriptorSetLayout" ) );
      vkCreateDescriptorPool           = PFN_vkCreateDescriptorPool( vkGetInstanceProcAddr( instance, "vkCreateDescriptorPool" ) );
      vkResetDescriptorPool            = PFN_vkResetDescriptorPool( vkGetInstanceProcAddr( instance, "vkResetDescriptorPool" ) );
      vkAllocateDescriptorSets         = PFN_vkAllocateDescriptorSets( vkGetInstanceProcAddr( instance, "vkAllocateDescriptorSets" ) );
      vkFreeDescriptorSets             = PFN_vkFreeDescriptorSets( vkGetInstanceProcAddr( instance, "vkFreeDescriptorSets" ) );
      vkUpdateDescriptorSets           = PFN_vkUpdateDescriptorSets( vkGetInstanceProcAddr( instance, "vkUpdateDescriptorSets" ) );
      vkCreateFramebuffer              = PFN_vkCreateFramebuffer( vkGetInstanceProcAddr( instance, "vkCreateFramebuffer" ) );
      vkDestroyFramebuffer             = PFN_vkDestroyFramebuffer( vkGetInstanceProcAddr( instance, "vkDestroyFramebuffer" ) );
      vkCreateRenderPass               = PFN_vkCreateRenderPass( vkGetInstanceProcAddr( instance, "vkCreateRenderPass" ) );
      vkDestroyRenderPass              = PFN_vkDestroyRenderPass( vkGetInstanceProcAddr( instance, "vkDestroyRenderPass" ) );
      vkGetRenderAreaGranularity       = PFN_vkGetRenderAreaGranularity( vkGetInstanceProcAddr( instance, "vkGetRenderAreaGranularity" ) );
      vkCreateCommandPool              = PFN_vkCreateCommandPool( vkGetInstanceProcAddr( instance, "vkCreateCommandPool" ) );
      vkResetCommandPool               = PFN_vkResetCommandPool( vkGetInstanceProcAddr( instance, "vkResetCommandPool" ) );
      vkAllocateCommandBuffers         = PFN_vkAllocateCommandBuffers( vkGetInstanceProcAddr( instance, "vkAllocateCommandBuffers" ) );
      vkFreeCommandBuffers             = PFN_vkFreeCommandBuffers( vkGetInstanceProcAddr( instance, "vkFreeCommandBuffers" ) );
      vkBeginCommandBuffer             = PFN_vkBeginCommandBuffer( vkGetInstanceProcAddr( instance, "vkBeginCommandBuffer" ) );
      vkEndCommandBuffer               = PFN_vkEndCommandBuffer( vkGetInstanceProcAddr( instance, "vkEndCommandBuffer" ) );
      vkResetCommandBuffer             = PFN_vkResetCommandBuffer( vkGetInstanceProcAddr( instance, "vkResetCommandBuffer" ) );
      vkCmdBindPipeline                = PFN_vkCmdBindPipeline( vkGetInstanceProcAddr( instance, "vkCmdBindPipeline" ) );
      vkCmdSetViewport                 = PFN_vkCmdSetViewport( vkGetInstanceProcAddr( instance, "vkCmdSetViewport" ) );
      vkCmdSetScissor                  = PFN_vkCmdSetScissor( vkGetInstanceProcAddr( instance, "vkCmdSetScissor" ) );
      vkCmdSetLineWidth                = PFN_vkCmdSetLineWidth( vkGetInstanceProcAddr( instance, "vkCmdSetLineWidth" ) );
      vkCmdSetDepthBias                = PFN_vkCmdSetDepthBias( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBias" ) );
      vkCmdSetBlendConstants           = PFN_vkCmdSetBlendConstants( vkGetInstanceProcAddr( instance, "vkCmdSetBlendConstants" ) );
      vkCmdSetDepthBounds              = PFN_vkCmdSetDepthBounds( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBounds" ) );
      vkCmdSetStencilCompareMask       = PFN_vkCmdSetStencilCompareMask( vkGetInstanceProcAddr( instance, "vkCmdSetStencilCompareMask" ) );
      vkCmdSetStencilWriteMask         = PFN_vkCmdSetStencilWriteMask( vkGetInstanceProcAddr( instance, "vkCmdSetStencilWriteMask" ) );
      vkCmdSetStencilReference         = PFN_vkCmdSetStencilReference( vkGetInstanceProcAddr( instance, "vkCmdSetStencilReference" ) );
      vkCmdBindDescriptorSets          = PFN_vkCmdBindDescriptorSets( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorSets" ) );
      vkCmdBindIndexBuffer             = PFN_vkCmdBindIndexBuffer( vkGetInstanceProcAddr( instance, "vkCmdBindIndexBuffer" ) );
      vkCmdBindVertexBuffers           = PFN_vkCmdBindVertexBuffers( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers" ) );
      vkCmdDraw                        = PFN_vkCmdDraw( vkGetInstanceProcAddr( instance, "vkCmdDraw" ) );
      vkCmdDrawIndexed                 = PFN_vkCmdDrawIndexed( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexed" ) );
      vkCmdDrawIndirect                = PFN_vkCmdDrawIndirect( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirect" ) );
      vkCmdDrawIndexedIndirect         = PFN_vkCmdDrawIndexedIndirect( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirect" ) );
      vkCmdDispatch                    = PFN_vkCmdDispatch( vkGetInstanceProcAddr( instance, "vkCmdDispatch" ) );
      vkCmdDispatchIndirect            = PFN_vkCmdDispatchIndirect( vkGetInstanceProcAddr( instance, "vkCmdDispatchIndirect" ) );
      vkCmdCopyBuffer                  = PFN_vkCmdCopyBuffer( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer" ) );
      vkCmdCopyImage                   = PFN_vkCmdCopyImage( vkGetInstanceProcAddr( instance, "vkCmdCopyImage" ) );
      vkCmdBlitImage                   = PFN_vkCmdBlitImage( vkGetInstanceProcAddr( instance, "vkCmdBlitImage" ) );
      vkCmdCopyBufferToImage           = PFN_vkCmdCopyBufferToImage( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage" ) );
      vkCmdCopyImageToBuffer           = PFN_vkCmdCopyImageToBuffer( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer" ) );
      vkCmdUpdateBuffer                = PFN_vkCmdUpdateBuffer( vkGetInstanceProcAddr( instance, "vkCmdUpdateBuffer" ) );
      vkCmdFillBuffer                  = PFN_vkCmdFillBuffer( vkGetInstanceProcAddr( instance, "vkCmdFillBuffer" ) );
      vkCmdClearColorImage             = PFN_vkCmdClearColorImage( vkGetInstanceProcAddr( instance, "vkCmdClearColorImage" ) );
      vkCmdClearDepthStencilImage      = PFN_vkCmdClearDepthStencilImage( vkGetInstanceProcAddr( instance, "vkCmdClearDepthStencilImage" ) );
      vkCmdClearAttachments            = PFN_vkCmdClearAttachments( vkGetInstanceProcAddr( instance, "vkCmdClearAttachments" ) );
      vkCmdResolveImage                = PFN_vkCmdResolveImage( vkGetInstanceProcAddr( instance, "vkCmdResolveImage" ) );
      vkCmdSetEvent                    = PFN_vkCmdSetEvent( vkGetInstanceProcAddr( instance, "vkCmdSetEvent" ) );
      vkCmdResetEvent                  = PFN_vkCmdResetEvent( vkGetInstanceProcAddr( instance, "vkCmdResetEvent" ) );
      vkCmdWaitEvents                  = PFN_vkCmdWaitEvents( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents" ) );
      vkCmdPipelineBarrier             = PFN_vkCmdPipelineBarrier( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier" ) );
      vkCmdBeginQuery                  = PFN_vkCmdBeginQuery( vkGetInstanceProcAddr( instance, "vkCmdBeginQuery" ) );
      vkCmdEndQuery                    = PFN_vkCmdEndQuery( vkGetInstanceProcAddr( instance, "vkCmdEndQuery" ) );
      vkCmdResetQueryPool              = PFN_vkCmdResetQueryPool( vkGetInstanceProcAddr( instance, "vkCmdResetQueryPool" ) );
      vkCmdWriteTimestamp              = PFN_vkCmdWriteTimestamp( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp" ) );
      vkCmdCopyQueryPoolResults        = PFN_vkCmdCopyQueryPoolResults( vkGetInstanceProcAddr( instance, "vkCmdCopyQueryPoolResults" ) );
      vkCmdPushConstants               = PFN_vkCmdPushConstants( vkGetInstanceProcAddr( instance, "vkCmdPushConstants" ) );
      vkCmdBeginRenderPass             = PFN_vkCmdBeginRenderPass( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderPass" ) );
      vkCmdNextSubpass                 = PFN_vkCmdNextSubpass( vkGetInstanceProcAddr( instance, "vkCmdNextSubpass" ) );
      vkCmdEndRenderPass               = PFN_vkCmdEndRenderPass( vkGetInstanceProcAddr( instance, "vkCmdEndRenderPass" ) );
      vkCmdExecuteCommands             = PFN_vkCmdExecuteCommands( vkGetInstanceProcAddr( instance, "vkCmdExecuteCommands" ) );

      //=== VK_VERSION_1_1 ===
      vkBindBufferMemory2                = PFN_vkBindBufferMemory2( vkGetInstanceProcAddr( instance, "vkBindBufferMemory2" ) );
      vkBindImageMemory2                 = PFN_vkBindImageMemory2( vkGetInstanceProcAddr( instance, "vkBindImageMemory2" ) );
      vkGetDeviceGroupPeerMemoryFeatures = PFN_vkGetDeviceGroupPeerMemoryFeatures( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupPeerMemoryFeatures" ) );
      vkCmdSetDeviceMask                 = PFN_vkCmdSetDeviceMask( vkGetInstanceProcAddr( instance, "vkCmdSetDeviceMask" ) );
      vkCmdDispatchBase                  = PFN_vkCmdDispatchBase( vkGetInstanceProcAddr( instance, "vkCmdDispatchBase" ) );
      vkEnumeratePhysicalDeviceGroups    = PFN_vkEnumeratePhysicalDeviceGroups( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceGroups" ) );
      vkGetImageMemoryRequirements2      = PFN_vkGetImageMemoryRequirements2( vkGetInstanceProcAddr( instance, "vkGetImageMemoryRequirements2" ) );
      vkGetBufferMemoryRequirements2     = PFN_vkGetBufferMemoryRequirements2( vkGetInstanceProcAddr( instance, "vkGetBufferMemoryRequirements2" ) );
      vkGetPhysicalDeviceFeatures2       = PFN_vkGetPhysicalDeviceFeatures2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures2" ) );
      vkGetPhysicalDeviceProperties2     = PFN_vkGetPhysicalDeviceProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties2" ) );
      vkGetPhysicalDeviceFormatProperties2 =
        PFN_vkGetPhysicalDeviceFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties2" ) );
      vkGetPhysicalDeviceImageFormatProperties2 =
        PFN_vkGetPhysicalDeviceImageFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties2" ) );
      vkGetPhysicalDeviceQueueFamilyProperties2 =
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties2" ) );
      vkGetPhysicalDeviceMemoryProperties2 =
        PFN_vkGetPhysicalDeviceMemoryProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties2" ) );
      vkGetDeviceQueue2               = PFN_vkGetDeviceQueue2( vkGetInstanceProcAddr( instance, "vkGetDeviceQueue2" ) );
      vkCreateSamplerYcbcrConversion  = PFN_vkCreateSamplerYcbcrConversion( vkGetInstanceProcAddr( instance, "vkCreateSamplerYcbcrConversion" ) );
      vkDestroySamplerYcbcrConversion = PFN_vkDestroySamplerYcbcrConversion( vkGetInstanceProcAddr( instance, "vkDestroySamplerYcbcrConversion" ) );
      vkGetPhysicalDeviceExternalBufferProperties =
        PFN_vkGetPhysicalDeviceExternalBufferProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalBufferProperties" ) );
      vkGetPhysicalDeviceExternalFenceProperties =
        PFN_vkGetPhysicalDeviceExternalFenceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalFenceProperties" ) );
      vkGetPhysicalDeviceExternalSemaphoreProperties =
        PFN_vkGetPhysicalDeviceExternalSemaphoreProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalSemaphoreProperties" ) );
      vkGetDescriptorSetLayoutSupport = PFN_vkGetDescriptorSetLayoutSupport( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutSupport" ) );

      //=== VK_VERSION_1_2 ===
      vkCmdDrawIndirectCount          = PFN_vkCmdDrawIndirectCount( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirectCount" ) );
      vkCmdDrawIndexedIndirectCount   = PFN_vkCmdDrawIndexedIndirectCount( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirectCount" ) );
      vkCreateRenderPass2             = PFN_vkCreateRenderPass2( vkGetInstanceProcAddr( instance, "vkCreateRenderPass2" ) );
      vkCmdBeginRenderPass2           = PFN_vkCmdBeginRenderPass2( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderPass2" ) );
      vkCmdNextSubpass2               = PFN_vkCmdNextSubpass2( vkGetInstanceProcAddr( instance, "vkCmdNextSubpass2" ) );
      vkCmdEndRenderPass2             = PFN_vkCmdEndRenderPass2( vkGetInstanceProcAddr( instance, "vkCmdEndRenderPass2" ) );
      vkResetQueryPool                = PFN_vkResetQueryPool( vkGetInstanceProcAddr( instance, "vkResetQueryPool" ) );
      vkGetSemaphoreCounterValue      = PFN_vkGetSemaphoreCounterValue( vkGetInstanceProcAddr( instance, "vkGetSemaphoreCounterValue" ) );
      vkWaitSemaphores                = PFN_vkWaitSemaphores( vkGetInstanceProcAddr( instance, "vkWaitSemaphores" ) );
      vkSignalSemaphore               = PFN_vkSignalSemaphore( vkGetInstanceProcAddr( instance, "vkSignalSemaphore" ) );
      vkGetBufferDeviceAddress        = PFN_vkGetBufferDeviceAddress( vkGetInstanceProcAddr( instance, "vkGetBufferDeviceAddress" ) );
      vkGetBufferOpaqueCaptureAddress = PFN_vkGetBufferOpaqueCaptureAddress( vkGetInstanceProcAddr( instance, "vkGetBufferOpaqueCaptureAddress" ) );
      vkGetDeviceMemoryOpaqueCaptureAddress =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress( vkGetInstanceProcAddr( instance, "vkGetDeviceMemoryOpaqueCaptureAddress" ) );

      //=== VK_VERSION_1_3 ===
      vkGetPhysicalDeviceToolProperties   = PFN_vkGetPhysicalDeviceToolProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceToolProperties" ) );
      vkCreatePrivateDataSlot             = PFN_vkCreatePrivateDataSlot( vkGetInstanceProcAddr( instance, "vkCreatePrivateDataSlot" ) );
      vkDestroyPrivateDataSlot            = PFN_vkDestroyPrivateDataSlot( vkGetInstanceProcAddr( instance, "vkDestroyPrivateDataSlot" ) );
      vkSetPrivateData                    = PFN_vkSetPrivateData( vkGetInstanceProcAddr( instance, "vkSetPrivateData" ) );
      vkGetPrivateData                    = PFN_vkGetPrivateData( vkGetInstanceProcAddr( instance, "vkGetPrivateData" ) );
      vkCmdSetEvent2                      = PFN_vkCmdSetEvent2( vkGetInstanceProcAddr( instance, "vkCmdSetEvent2" ) );
      vkCmdResetEvent2                    = PFN_vkCmdResetEvent2( vkGetInstanceProcAddr( instance, "vkCmdResetEvent2" ) );
      vkCmdWaitEvents2                    = PFN_vkCmdWaitEvents2( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents2" ) );
      vkCmdPipelineBarrier2               = PFN_vkCmdPipelineBarrier2( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier2" ) );
      vkCmdWriteTimestamp2                = PFN_vkCmdWriteTimestamp2( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp2" ) );
      vkQueueSubmit2                      = PFN_vkQueueSubmit2( vkGetInstanceProcAddr( instance, "vkQueueSubmit2" ) );
      vkCmdCopyBuffer2                    = PFN_vkCmdCopyBuffer2( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer2" ) );
      vkCmdCopyImage2                     = PFN_vkCmdCopyImage2( vkGetInstanceProcAddr( instance, "vkCmdCopyImage2" ) );
      vkCmdCopyBufferToImage2             = PFN_vkCmdCopyBufferToImage2( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage2" ) );
      vkCmdCopyImageToBuffer2             = PFN_vkCmdCopyImageToBuffer2( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer2" ) );
      vkCmdBlitImage2                     = PFN_vkCmdBlitImage2( vkGetInstanceProcAddr( instance, "vkCmdBlitImage2" ) );
      vkCmdResolveImage2                  = PFN_vkCmdResolveImage2( vkGetInstanceProcAddr( instance, "vkCmdResolveImage2" ) );
      vkCmdBeginRendering                 = PFN_vkCmdBeginRendering( vkGetInstanceProcAddr( instance, "vkCmdBeginRendering" ) );
      vkCmdEndRendering                   = PFN_vkCmdEndRendering( vkGetInstanceProcAddr( instance, "vkCmdEndRendering" ) );
      vkCmdSetCullMode                    = PFN_vkCmdSetCullMode( vkGetInstanceProcAddr( instance, "vkCmdSetCullMode" ) );
      vkCmdSetFrontFace                   = PFN_vkCmdSetFrontFace( vkGetInstanceProcAddr( instance, "vkCmdSetFrontFace" ) );
      vkCmdSetPrimitiveTopology           = PFN_vkCmdSetPrimitiveTopology( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveTopology" ) );
      vkCmdSetViewportWithCount           = PFN_vkCmdSetViewportWithCount( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWithCount" ) );
      vkCmdSetScissorWithCount            = PFN_vkCmdSetScissorWithCount( vkGetInstanceProcAddr( instance, "vkCmdSetScissorWithCount" ) );
      vkCmdBindVertexBuffers2             = PFN_vkCmdBindVertexBuffers2( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers2" ) );
      vkCmdSetDepthTestEnable             = PFN_vkCmdSetDepthTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthTestEnable" ) );
      vkCmdSetDepthWriteEnable            = PFN_vkCmdSetDepthWriteEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthWriteEnable" ) );
      vkCmdSetDepthCompareOp              = PFN_vkCmdSetDepthCompareOp( vkGetInstanceProcAddr( instance, "vkCmdSetDepthCompareOp" ) );
      vkCmdSetDepthBoundsTestEnable       = PFN_vkCmdSetDepthBoundsTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBoundsTestEnable" ) );
      vkCmdSetStencilTestEnable           = PFN_vkCmdSetStencilTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetStencilTestEnable" ) );
      vkCmdSetStencilOp                   = PFN_vkCmdSetStencilOp( vkGetInstanceProcAddr( instance, "vkCmdSetStencilOp" ) );
      vkCmdSetRasterizerDiscardEnable     = PFN_vkCmdSetRasterizerDiscardEnable( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizerDiscardEnable" ) );
      vkCmdSetDepthBiasEnable             = PFN_vkCmdSetDepthBiasEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBiasEnable" ) );
      vkCmdSetPrimitiveRestartEnable      = PFN_vkCmdSetPrimitiveRestartEnable( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveRestartEnable" ) );
      vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceBufferMemoryRequirements" ) );
      vkGetDeviceImageMemoryRequirements  = PFN_vkGetDeviceImageMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceImageMemoryRequirements" ) );
      vkGetDeviceImageSparseMemoryRequirements =
        PFN_vkGetDeviceImageSparseMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceImageSparseMemoryRequirements" ) );

      //=== VKSC_VERSION_1_0 ===
      vkGetCommandPoolMemoryConsumption = PFN_vkGetCommandPoolMemoryConsumption( vkGetInstanceProcAddr( instance, "vkGetCommandPoolMemoryConsumption" ) );
      vkGetFaultData                    = PFN_vkGetFaultData( vkGetInstanceProcAddr( instance, "vkGetFaultData" ) );

      //=== VK_KHR_surface ===
      vkDestroySurfaceKHR = PFN_vkDestroySurfaceKHR( vkGetInstanceProcAddr( instance, "vkDestroySurfaceKHR" ) );
      vkGetPhysicalDeviceSurfaceSupportKHR =
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceSupportKHR" ) );
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR =
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" ) );
      vkGetPhysicalDeviceSurfaceFormatsKHR =
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormatsKHR" ) );
      vkGetPhysicalDeviceSurfacePresentModesKHR =
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfacePresentModesKHR" ) );

      //=== VK_KHR_swapchain ===
      vkCreateSwapchainKHR    = PFN_vkCreateSwapchainKHR( vkGetInstanceProcAddr( instance, "vkCreateSwapchainKHR" ) );
      vkGetSwapchainImagesKHR = PFN_vkGetSwapchainImagesKHR( vkGetInstanceProcAddr( instance, "vkGetSwapchainImagesKHR" ) );
      vkAcquireNextImageKHR   = PFN_vkAcquireNextImageKHR( vkGetInstanceProcAddr( instance, "vkAcquireNextImageKHR" ) );
      vkQueuePresentKHR       = PFN_vkQueuePresentKHR( vkGetInstanceProcAddr( instance, "vkQueuePresentKHR" ) );
      vkGetDeviceGroupPresentCapabilitiesKHR =
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupPresentCapabilitiesKHR" ) );
      vkGetDeviceGroupSurfacePresentModesKHR =
        PFN_vkGetDeviceGroupSurfacePresentModesKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupSurfacePresentModesKHR" ) );
      vkGetPhysicalDevicePresentRectanglesKHR =
        PFN_vkGetPhysicalDevicePresentRectanglesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDevicePresentRectanglesKHR" ) );
      vkAcquireNextImage2KHR = PFN_vkAcquireNextImage2KHR( vkGetInstanceProcAddr( instance, "vkAcquireNextImage2KHR" ) );

      //=== VK_KHR_display ===
      vkGetPhysicalDeviceDisplayPropertiesKHR =
        PFN_vkGetPhysicalDeviceDisplayPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPropertiesKHR" ) );
      vkGetPhysicalDeviceDisplayPlanePropertiesKHR =
        PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR" ) );
      vkGetDisplayPlaneSupportedDisplaysKHR =
        PFN_vkGetDisplayPlaneSupportedDisplaysKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneSupportedDisplaysKHR" ) );
      vkGetDisplayModePropertiesKHR    = PFN_vkGetDisplayModePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModePropertiesKHR" ) );
      vkCreateDisplayModeKHR           = PFN_vkCreateDisplayModeKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayModeKHR" ) );
      vkGetDisplayPlaneCapabilitiesKHR = PFN_vkGetDisplayPlaneCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilitiesKHR" ) );
      vkCreateDisplayPlaneSurfaceKHR   = PFN_vkCreateDisplayPlaneSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayPlaneSurfaceKHR" ) );

      //=== VK_KHR_display_swapchain ===
      vkCreateSharedSwapchainsKHR = PFN_vkCreateSharedSwapchainsKHR( vkGetInstanceProcAddr( instance, "vkCreateSharedSwapchainsKHR" ) );

      //=== VK_KHR_external_memory_fd ===
      vkGetMemoryFdKHR           = PFN_vkGetMemoryFdKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryFdKHR" ) );
      vkGetMemoryFdPropertiesKHR = PFN_vkGetMemoryFdPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryFdPropertiesKHR" ) );

      //=== VK_KHR_external_semaphore_fd ===
      vkImportSemaphoreFdKHR = PFN_vkImportSemaphoreFdKHR( vkGetInstanceProcAddr( instance, "vkImportSemaphoreFdKHR" ) );
      vkGetSemaphoreFdKHR    = PFN_vkGetSemaphoreFdKHR( vkGetInstanceProcAddr( instance, "vkGetSemaphoreFdKHR" ) );

      //=== VK_EXT_direct_mode_display ===
      vkReleaseDisplayEXT = PFN_vkReleaseDisplayEXT( vkGetInstanceProcAddr( instance, "vkReleaseDisplayEXT" ) );

      //=== VK_EXT_display_surface_counter ===
      vkGetPhysicalDeviceSurfaceCapabilities2EXT =
        PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT" ) );

      //=== VK_EXT_display_control ===
      vkDisplayPowerControlEXT  = PFN_vkDisplayPowerControlEXT( vkGetInstanceProcAddr( instance, "vkDisplayPowerControlEXT" ) );
      vkRegisterDeviceEventEXT  = PFN_vkRegisterDeviceEventEXT( vkGetInstanceProcAddr( instance, "vkRegisterDeviceEventEXT" ) );
      vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT( vkGetInstanceProcAddr( instance, "vkRegisterDisplayEventEXT" ) );
      vkGetSwapchainCounterEXT  = PFN_vkGetSwapchainCounterEXT( vkGetInstanceProcAddr( instance, "vkGetSwapchainCounterEXT" ) );

      //=== VK_EXT_discard_rectangles ===
      vkCmdSetDiscardRectangleEXT       = PFN_vkCmdSetDiscardRectangleEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleEXT" ) );
      vkCmdSetDiscardRectangleEnableEXT = PFN_vkCmdSetDiscardRectangleEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleEnableEXT" ) );
      vkCmdSetDiscardRectangleModeEXT   = PFN_vkCmdSetDiscardRectangleModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleModeEXT" ) );

      //=== VK_EXT_hdr_metadata ===
      vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT( vkGetInstanceProcAddr( instance, "vkSetHdrMetadataEXT" ) );

      //=== VK_KHR_shared_presentable_image ===
      vkGetSwapchainStatusKHR = PFN_vkGetSwapchainStatusKHR( vkGetInstanceProcAddr( instance, "vkGetSwapchainStatusKHR" ) );

      //=== VK_KHR_external_fence_fd ===
      vkImportFenceFdKHR = PFN_vkImportFenceFdKHR( vkGetInstanceProcAddr( instance, "vkImportFenceFdKHR" ) );
      vkGetFenceFdKHR    = PFN_vkGetFenceFdKHR( vkGetInstanceProcAddr( instance, "vkGetFenceFdKHR" ) );

      //=== VK_KHR_performance_query ===
      vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
        vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR" ) );
      vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR" ) );
      vkAcquireProfilingLockKHR = PFN_vkAcquireProfilingLockKHR( vkGetInstanceProcAddr( instance, "vkAcquireProfilingLockKHR" ) );
      vkReleaseProfilingLockKHR = PFN_vkReleaseProfilingLockKHR( vkGetInstanceProcAddr( instance, "vkReleaseProfilingLockKHR" ) );

      //=== VK_KHR_get_surface_capabilities2 ===
      vkGetPhysicalDeviceSurfaceCapabilities2KHR =
        PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR" ) );
      vkGetPhysicalDeviceSurfaceFormats2KHR =
        PFN_vkGetPhysicalDeviceSurfaceFormats2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormats2KHR" ) );

      //=== VK_KHR_get_display_properties2 ===
      vkGetPhysicalDeviceDisplayProperties2KHR =
        PFN_vkGetPhysicalDeviceDisplayProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayProperties2KHR" ) );
      vkGetPhysicalDeviceDisplayPlaneProperties2KHR =
        PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR" ) );
      vkGetDisplayModeProperties2KHR    = PFN_vkGetDisplayModeProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModeProperties2KHR" ) );
      vkGetDisplayPlaneCapabilities2KHR = PFN_vkGetDisplayPlaneCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilities2KHR" ) );

      //=== VK_EXT_debug_utils ===
      vkSetDebugUtilsObjectNameEXT    = PFN_vkSetDebugUtilsObjectNameEXT( vkGetInstanceProcAddr( instance, "vkSetDebugUtilsObjectNameEXT" ) );
      vkSetDebugUtilsObjectTagEXT     = PFN_vkSetDebugUtilsObjectTagEXT( vkGetInstanceProcAddr( instance, "vkSetDebugUtilsObjectTagEXT" ) );
      vkQueueBeginDebugUtilsLabelEXT  = PFN_vkQueueBeginDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueBeginDebugUtilsLabelEXT" ) );
      vkQueueEndDebugUtilsLabelEXT    = PFN_vkQueueEndDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueEndDebugUtilsLabelEXT" ) );
      vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueInsertDebugUtilsLabelEXT" ) );
      vkCmdBeginDebugUtilsLabelEXT    = PFN_vkCmdBeginDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginDebugUtilsLabelEXT" ) );
      vkCmdEndDebugUtilsLabelEXT      = PFN_vkCmdEndDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdEndDebugUtilsLabelEXT" ) );
      vkCmdInsertDebugUtilsLabelEXT   = PFN_vkCmdInsertDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdInsertDebugUtilsLabelEXT" ) );
      vkCreateDebugUtilsMessengerEXT  = PFN_vkCreateDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
      vkDestroyDebugUtilsMessengerEXT = PFN_vkDestroyDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
      vkSubmitDebugUtilsMessageEXT    = PFN_vkSubmitDebugUtilsMessageEXT( vkGetInstanceProcAddr( instance, "vkSubmitDebugUtilsMessageEXT" ) );

      //=== VK_EXT_sample_locations ===
      vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT( vkGetInstanceProcAddr( instance, "vkCmdSetSampleLocationsEXT" ) );
      vkGetPhysicalDeviceMultisamplePropertiesEXT =
        PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT" ) );

      //=== VK_EXT_image_drm_format_modifier ===
      vkGetImageDrmFormatModifierPropertiesEXT =
        PFN_vkGetImageDrmFormatModifierPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetImageDrmFormatModifierPropertiesEXT" ) );

      //=== VK_EXT_external_memory_host ===
      vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetMemoryHostPointerPropertiesEXT" ) );

      //=== VK_EXT_calibrated_timestamps ===
      vkGetPhysicalDeviceCalibrateableTimeDomainsEXT =
        PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT" ) );
      if ( !vkGetPhysicalDeviceCalibrateableTimeDomainsKHR )
        vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
      vkGetCalibratedTimestampsEXT = PFN_vkGetCalibratedTimestampsEXT( vkGetInstanceProcAddr( instance, "vkGetCalibratedTimestampsEXT" ) );
      if ( !vkGetCalibratedTimestampsKHR )
        vkGetCalibratedTimestampsKHR = vkGetCalibratedTimestampsEXT;

      //=== VK_KHR_fragment_shading_rate ===
      vkGetPhysicalDeviceFragmentShadingRatesKHR =
        PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFragmentShadingRatesKHR" ) );
      vkCmdSetFragmentShadingRateKHR = PFN_vkCmdSetFragmentShadingRateKHR( vkGetInstanceProcAddr( instance, "vkCmdSetFragmentShadingRateKHR" ) );

      //=== VK_EXT_headless_surface ===
      vkCreateHeadlessSurfaceEXT = PFN_vkCreateHeadlessSurfaceEXT( vkGetInstanceProcAddr( instance, "vkCreateHeadlessSurfaceEXT" ) );

      //=== VK_EXT_line_rasterization ===
      vkCmdSetLineStippleEXT = PFN_vkCmdSetLineStippleEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLineStippleEXT" ) );
      if ( !vkCmdSetLineStippleKHR )
        vkCmdSetLineStippleKHR = vkCmdSetLineStippleEXT;

      //=== VK_EXT_extended_dynamic_state ===
      vkCmdSetCullModeEXT = PFN_vkCmdSetCullModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetCullModeEXT" ) );
      if ( !vkCmdSetCullMode )
        vkCmdSetCullMode = vkCmdSetCullModeEXT;
      vkCmdSetFrontFaceEXT = PFN_vkCmdSetFrontFaceEXT( vkGetInstanceProcAddr( instance, "vkCmdSetFrontFaceEXT" ) );
      if ( !vkCmdSetFrontFace )
        vkCmdSetFrontFace = vkCmdSetFrontFaceEXT;
      vkCmdSetPrimitiveTopologyEXT = PFN_vkCmdSetPrimitiveTopologyEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveTopologyEXT" ) );
      if ( !vkCmdSetPrimitiveTopology )
        vkCmdSetPrimitiveTopology = vkCmdSetPrimitiveTopologyEXT;
      vkCmdSetViewportWithCountEXT = PFN_vkCmdSetViewportWithCountEXT( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWithCountEXT" ) );
      if ( !vkCmdSetViewportWithCount )
        vkCmdSetViewportWithCount = vkCmdSetViewportWithCountEXT;
      vkCmdSetScissorWithCountEXT = PFN_vkCmdSetScissorWithCountEXT( vkGetInstanceProcAddr( instance, "vkCmdSetScissorWithCountEXT" ) );
      if ( !vkCmdSetScissorWithCount )
        vkCmdSetScissorWithCount = vkCmdSetScissorWithCountEXT;
      vkCmdBindVertexBuffers2EXT = PFN_vkCmdBindVertexBuffers2EXT( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers2EXT" ) );
      if ( !vkCmdBindVertexBuffers2 )
        vkCmdBindVertexBuffers2 = vkCmdBindVertexBuffers2EXT;
      vkCmdSetDepthTestEnableEXT = PFN_vkCmdSetDepthTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthTestEnableEXT" ) );
      if ( !vkCmdSetDepthTestEnable )
        vkCmdSetDepthTestEnable = vkCmdSetDepthTestEnableEXT;
      vkCmdSetDepthWriteEnableEXT = PFN_vkCmdSetDepthWriteEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthWriteEnableEXT" ) );
      if ( !vkCmdSetDepthWriteEnable )
        vkCmdSetDepthWriteEnable = vkCmdSetDepthWriteEnableEXT;
      vkCmdSetDepthCompareOpEXT = PFN_vkCmdSetDepthCompareOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthCompareOpEXT" ) );
      if ( !vkCmdSetDepthCompareOp )
        vkCmdSetDepthCompareOp = vkCmdSetDepthCompareOpEXT;
      vkCmdSetDepthBoundsTestEnableEXT = PFN_vkCmdSetDepthBoundsTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBoundsTestEnableEXT" ) );
      if ( !vkCmdSetDepthBoundsTestEnable )
        vkCmdSetDepthBoundsTestEnable = vkCmdSetDepthBoundsTestEnableEXT;
      vkCmdSetStencilTestEnableEXT = PFN_vkCmdSetStencilTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetStencilTestEnableEXT" ) );
      if ( !vkCmdSetStencilTestEnable )
        vkCmdSetStencilTestEnable = vkCmdSetStencilTestEnableEXT;
      vkCmdSetStencilOpEXT = PFN_vkCmdSetStencilOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetStencilOpEXT" ) );
      if ( !vkCmdSetStencilOp )
        vkCmdSetStencilOp = vkCmdSetStencilOpEXT;

      //=== VK_KHR_object_refresh ===
      vkCmdRefreshObjectsKHR = PFN_vkCmdRefreshObjectsKHR( vkGetInstanceProcAddr( instance, "vkCmdRefreshObjectsKHR" ) );
      vkGetPhysicalDeviceRefreshableObjectTypesKHR =
        PFN_vkGetPhysicalDeviceRefreshableObjectTypesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceRefreshableObjectTypesKHR" ) );

      //=== VK_KHR_synchronization2 ===
      vkCmdSetEvent2KHR = PFN_vkCmdSetEvent2KHR( vkGetInstanceProcAddr( instance, "vkCmdSetEvent2KHR" ) );
      if ( !vkCmdSetEvent2 )
        vkCmdSetEvent2 = vkCmdSetEvent2KHR;
      vkCmdResetEvent2KHR = PFN_vkCmdResetEvent2KHR( vkGetInstanceProcAddr( instance, "vkCmdResetEvent2KHR" ) );
      if ( !vkCmdResetEvent2 )
        vkCmdResetEvent2 = vkCmdResetEvent2KHR;
      vkCmdWaitEvents2KHR = PFN_vkCmdWaitEvents2KHR( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents2KHR" ) );
      if ( !vkCmdWaitEvents2 )
        vkCmdWaitEvents2 = vkCmdWaitEvents2KHR;
      vkCmdPipelineBarrier2KHR = PFN_vkCmdPipelineBarrier2KHR( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier2KHR" ) );
      if ( !vkCmdPipelineBarrier2 )
        vkCmdPipelineBarrier2 = vkCmdPipelineBarrier2KHR;
      vkCmdWriteTimestamp2KHR = PFN_vkCmdWriteTimestamp2KHR( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp2KHR" ) );
      if ( !vkCmdWriteTimestamp2 )
        vkCmdWriteTimestamp2 = vkCmdWriteTimestamp2KHR;
      vkQueueSubmit2KHR = PFN_vkQueueSubmit2KHR( vkGetInstanceProcAddr( instance, "vkQueueSubmit2KHR" ) );
      if ( !vkQueueSubmit2 )
        vkQueueSubmit2 = vkQueueSubmit2KHR;
      vkCmdWriteBufferMarker2AMD  = PFN_vkCmdWriteBufferMarker2AMD( vkGetInstanceProcAddr( instance, "vkCmdWriteBufferMarker2AMD" ) );
      vkGetQueueCheckpointData2NV = PFN_vkGetQueueCheckpointData2NV( vkGetInstanceProcAddr( instance, "vkGetQueueCheckpointData2NV" ) );

      //=== VK_KHR_copy_commands2 ===
      vkCmdCopyBuffer2KHR = PFN_vkCmdCopyBuffer2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer2KHR" ) );
      if ( !vkCmdCopyBuffer2 )
        vkCmdCopyBuffer2 = vkCmdCopyBuffer2KHR;
      vkCmdCopyImage2KHR = PFN_vkCmdCopyImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyImage2KHR" ) );
      if ( !vkCmdCopyImage2 )
        vkCmdCopyImage2 = vkCmdCopyImage2KHR;
      vkCmdCopyBufferToImage2KHR = PFN_vkCmdCopyBufferToImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage2KHR" ) );
      if ( !vkCmdCopyBufferToImage2 )
        vkCmdCopyBufferToImage2 = vkCmdCopyBufferToImage2KHR;
      vkCmdCopyImageToBuffer2KHR = PFN_vkCmdCopyImageToBuffer2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer2KHR" ) );
      if ( !vkCmdCopyImageToBuffer2 )
        vkCmdCopyImageToBuffer2 = vkCmdCopyImageToBuffer2KHR;
      vkCmdBlitImage2KHR = PFN_vkCmdBlitImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdBlitImage2KHR" ) );
      if ( !vkCmdBlitImage2 )
        vkCmdBlitImage2 = vkCmdBlitImage2KHR;
      vkCmdResolveImage2KHR = PFN_vkCmdResolveImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdResolveImage2KHR" ) );
      if ( !vkCmdResolveImage2 )
        vkCmdResolveImage2 = vkCmdResolveImage2KHR;

      //=== VK_EXT_vertex_input_dynamic_state ===
      vkCmdSetVertexInputEXT = PFN_vkCmdSetVertexInputEXT( vkGetInstanceProcAddr( instance, "vkCmdSetVertexInputEXT" ) );

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===
      vkGetFenceSciSyncFenceNV    = PFN_vkGetFenceSciSyncFenceNV( vkGetInstanceProcAddr( instance, "vkGetFenceSciSyncFenceNV" ) );
      vkGetFenceSciSyncObjNV      = PFN_vkGetFenceSciSyncObjNV( vkGetInstanceProcAddr( instance, "vkGetFenceSciSyncObjNV" ) );
      vkImportFenceSciSyncFenceNV = PFN_vkImportFenceSciSyncFenceNV( vkGetInstanceProcAddr( instance, "vkImportFenceSciSyncFenceNV" ) );
      vkImportFenceSciSyncObjNV   = PFN_vkImportFenceSciSyncObjNV( vkGetInstanceProcAddr( instance, "vkImportFenceSciSyncObjNV" ) );
      vkGetPhysicalDeviceSciSyncAttributesNV =
        PFN_vkGetPhysicalDeviceSciSyncAttributesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSciSyncAttributesNV" ) );
      vkGetSemaphoreSciSyncObjNV    = PFN_vkGetSemaphoreSciSyncObjNV( vkGetInstanceProcAddr( instance, "vkGetSemaphoreSciSyncObjNV" ) );
      vkImportSemaphoreSciSyncObjNV = PFN_vkImportSemaphoreSciSyncObjNV( vkGetInstanceProcAddr( instance, "vkImportSemaphoreSciSyncObjNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===
      vkGetMemorySciBufNV = PFN_vkGetMemorySciBufNV( vkGetInstanceProcAddr( instance, "vkGetMemorySciBufNV" ) );
      vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV =
        PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV" ) );
      vkGetPhysicalDeviceSciBufAttributesNV =
        PFN_vkGetPhysicalDeviceSciBufAttributesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSciBufAttributesNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

      //=== VK_EXT_extended_dynamic_state2 ===
      vkCmdSetPatchControlPointsEXT      = PFN_vkCmdSetPatchControlPointsEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPatchControlPointsEXT" ) );
      vkCmdSetRasterizerDiscardEnableEXT = PFN_vkCmdSetRasterizerDiscardEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizerDiscardEnableEXT" ) );
      if ( !vkCmdSetRasterizerDiscardEnable )
        vkCmdSetRasterizerDiscardEnable = vkCmdSetRasterizerDiscardEnableEXT;
      vkCmdSetDepthBiasEnableEXT = PFN_vkCmdSetDepthBiasEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBiasEnableEXT" ) );
      if ( !vkCmdSetDepthBiasEnable )
        vkCmdSetDepthBiasEnable = vkCmdSetDepthBiasEnableEXT;
      vkCmdSetLogicOpEXT                = PFN_vkCmdSetLogicOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLogicOpEXT" ) );
      vkCmdSetPrimitiveRestartEnableEXT = PFN_vkCmdSetPrimitiveRestartEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveRestartEnableEXT" ) );
      if ( !vkCmdSetPrimitiveRestartEnable )
        vkCmdSetPrimitiveRestartEnable = vkCmdSetPrimitiveRestartEnableEXT;

      //=== VK_EXT_color_write_enable ===
      vkCmdSetColorWriteEnableEXT = PFN_vkCmdSetColorWriteEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorWriteEnableEXT" ) );

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync2 ===
      vkCreateSemaphoreSciSyncPoolNV = PFN_vkCreateSemaphoreSciSyncPoolNV( vkGetInstanceProcAddr( instance, "vkCreateSemaphoreSciSyncPoolNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===
      vkGetScreenBufferPropertiesQNX = PFN_vkGetScreenBufferPropertiesQNX( vkGetInstanceProcAddr( instance, "vkGetScreenBufferPropertiesQNX" ) );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
    }

    void init( VULKAN_HPP_NAMESPACE::Device deviceCpp ) VULKAN_HPP_NOEXCEPT
    {
      VkDevice device = static_cast<VkDevice>( deviceCpp );

      //=== VK_VERSION_1_0 ===
      vkGetDeviceProcAddr            = PFN_vkGetDeviceProcAddr( vkGetDeviceProcAddr( device, "vkGetDeviceProcAddr" ) );
      vkDestroyDevice                = PFN_vkDestroyDevice( vkGetDeviceProcAddr( device, "vkDestroyDevice" ) );
      vkGetDeviceQueue               = PFN_vkGetDeviceQueue( vkGetDeviceProcAddr( device, "vkGetDeviceQueue" ) );
      vkQueueSubmit                  = PFN_vkQueueSubmit( vkGetDeviceProcAddr( device, "vkQueueSubmit" ) );
      vkQueueWaitIdle                = PFN_vkQueueWaitIdle( vkGetDeviceProcAddr( device, "vkQueueWaitIdle" ) );
      vkDeviceWaitIdle               = PFN_vkDeviceWaitIdle( vkGetDeviceProcAddr( device, "vkDeviceWaitIdle" ) );
      vkAllocateMemory               = PFN_vkAllocateMemory( vkGetDeviceProcAddr( device, "vkAllocateMemory" ) );
      vkMapMemory                    = PFN_vkMapMemory( vkGetDeviceProcAddr( device, "vkMapMemory" ) );
      vkUnmapMemory                  = PFN_vkUnmapMemory( vkGetDeviceProcAddr( device, "vkUnmapMemory" ) );
      vkFlushMappedMemoryRanges      = PFN_vkFlushMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkFlushMappedMemoryRanges" ) );
      vkInvalidateMappedMemoryRanges = PFN_vkInvalidateMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkInvalidateMappedMemoryRanges" ) );
      vkGetDeviceMemoryCommitment    = PFN_vkGetDeviceMemoryCommitment( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryCommitment" ) );
      vkBindBufferMemory             = PFN_vkBindBufferMemory( vkGetDeviceProcAddr( device, "vkBindBufferMemory" ) );
      vkBindImageMemory              = PFN_vkBindImageMemory( vkGetDeviceProcAddr( device, "vkBindImageMemory" ) );
      vkGetBufferMemoryRequirements  = PFN_vkGetBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements" ) );
      vkGetImageMemoryRequirements   = PFN_vkGetImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements" ) );
      vkCreateFence                  = PFN_vkCreateFence( vkGetDeviceProcAddr( device, "vkCreateFence" ) );
      vkDestroyFence                 = PFN_vkDestroyFence( vkGetDeviceProcAddr( device, "vkDestroyFence" ) );
      vkResetFences                  = PFN_vkResetFences( vkGetDeviceProcAddr( device, "vkResetFences" ) );
      vkGetFenceStatus               = PFN_vkGetFenceStatus( vkGetDeviceProcAddr( device, "vkGetFenceStatus" ) );
      vkWaitForFences                = PFN_vkWaitForFences( vkGetDeviceProcAddr( device, "vkWaitForFences" ) );
      vkCreateSemaphore              = PFN_vkCreateSemaphore( vkGetDeviceProcAddr( device, "vkCreateSemaphore" ) );
      vkDestroySemaphore             = PFN_vkDestroySemaphore( vkGetDeviceProcAddr( device, "vkDestroySemaphore" ) );
      vkCreateEvent                  = PFN_vkCreateEvent( vkGetDeviceProcAddr( device, "vkCreateEvent" ) );
      vkDestroyEvent                 = PFN_vkDestroyEvent( vkGetDeviceProcAddr( device, "vkDestroyEvent" ) );
      vkGetEventStatus               = PFN_vkGetEventStatus( vkGetDeviceProcAddr( device, "vkGetEventStatus" ) );
      vkSetEvent                     = PFN_vkSetEvent( vkGetDeviceProcAddr( device, "vkSetEvent" ) );
      vkResetEvent                   = PFN_vkResetEvent( vkGetDeviceProcAddr( device, "vkResetEvent" ) );
      vkCreateQueryPool              = PFN_vkCreateQueryPool( vkGetDeviceProcAddr( device, "vkCreateQueryPool" ) );
      vkGetQueryPoolResults          = PFN_vkGetQueryPoolResults( vkGetDeviceProcAddr( device, "vkGetQueryPoolResults" ) );
      vkCreateBuffer                 = PFN_vkCreateBuffer( vkGetDeviceProcAddr( device, "vkCreateBuffer" ) );
      vkDestroyBuffer                = PFN_vkDestroyBuffer( vkGetDeviceProcAddr( device, "vkDestroyBuffer" ) );
      vkCreateBufferView             = PFN_vkCreateBufferView( vkGetDeviceProcAddr( device, "vkCreateBufferView" ) );
      vkDestroyBufferView            = PFN_vkDestroyBufferView( vkGetDeviceProcAddr( device, "vkDestroyBufferView" ) );
      vkCreateImage                  = PFN_vkCreateImage( vkGetDeviceProcAddr( device, "vkCreateImage" ) );
      vkDestroyImage                 = PFN_vkDestroyImage( vkGetDeviceProcAddr( device, "vkDestroyImage" ) );
      vkGetImageSubresourceLayout    = PFN_vkGetImageSubresourceLayout( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout" ) );
      vkCreateImageView              = PFN_vkCreateImageView( vkGetDeviceProcAddr( device, "vkCreateImageView" ) );
      vkDestroyImageView             = PFN_vkDestroyImageView( vkGetDeviceProcAddr( device, "vkDestroyImageView" ) );
      vkCreatePipelineCache          = PFN_vkCreatePipelineCache( vkGetDeviceProcAddr( device, "vkCreatePipelineCache" ) );
      vkDestroyPipelineCache         = PFN_vkDestroyPipelineCache( vkGetDeviceProcAddr( device, "vkDestroyPipelineCache" ) );
      vkCreateGraphicsPipelines      = PFN_vkCreateGraphicsPipelines( vkGetDeviceProcAddr( device, "vkCreateGraphicsPipelines" ) );
      vkCreateComputePipelines       = PFN_vkCreateComputePipelines( vkGetDeviceProcAddr( device, "vkCreateComputePipelines" ) );
      vkDestroyPipeline              = PFN_vkDestroyPipeline( vkGetDeviceProcAddr( device, "vkDestroyPipeline" ) );
      vkCreatePipelineLayout         = PFN_vkCreatePipelineLayout( vkGetDeviceProcAddr( device, "vkCreatePipelineLayout" ) );
      vkDestroyPipelineLayout        = PFN_vkDestroyPipelineLayout( vkGetDeviceProcAddr( device, "vkDestroyPipelineLayout" ) );
      vkCreateSampler                = PFN_vkCreateSampler( vkGetDeviceProcAddr( device, "vkCreateSampler" ) );
      vkDestroySampler               = PFN_vkDestroySampler( vkGetDeviceProcAddr( device, "vkDestroySampler" ) );
      vkCreateDescriptorSetLayout    = PFN_vkCreateDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkCreateDescriptorSetLayout" ) );
      vkDestroyDescriptorSetLayout   = PFN_vkDestroyDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkDestroyDescriptorSetLayout" ) );
      vkCreateDescriptorPool         = PFN_vkCreateDescriptorPool( vkGetDeviceProcAddr( device, "vkCreateDescriptorPool" ) );
      vkResetDescriptorPool          = PFN_vkResetDescriptorPool( vkGetDeviceProcAddr( device, "vkResetDescriptorPool" ) );
      vkAllocateDescriptorSets       = PFN_vkAllocateDescriptorSets( vkGetDeviceProcAddr( device, "vkAllocateDescriptorSets" ) );
      vkFreeDescriptorSets           = PFN_vkFreeDescriptorSets( vkGetDeviceProcAddr( device, "vkFreeDescriptorSets" ) );
      vkUpdateDescriptorSets         = PFN_vkUpdateDescriptorSets( vkGetDeviceProcAddr( device, "vkUpdateDescriptorSets" ) );
      vkCreateFramebuffer            = PFN_vkCreateFramebuffer( vkGetDeviceProcAddr( device, "vkCreateFramebuffer" ) );
      vkDestroyFramebuffer           = PFN_vkDestroyFramebuffer( vkGetDeviceProcAddr( device, "vkDestroyFramebuffer" ) );
      vkCreateRenderPass             = PFN_vkCreateRenderPass( vkGetDeviceProcAddr( device, "vkCreateRenderPass" ) );
      vkDestroyRenderPass            = PFN_vkDestroyRenderPass( vkGetDeviceProcAddr( device, "vkDestroyRenderPass" ) );
      vkGetRenderAreaGranularity     = PFN_vkGetRenderAreaGranularity( vkGetDeviceProcAddr( device, "vkGetRenderAreaGranularity" ) );
      vkCreateCommandPool            = PFN_vkCreateCommandPool( vkGetDeviceProcAddr( device, "vkCreateCommandPool" ) );
      vkResetCommandPool             = PFN_vkResetCommandPool( vkGetDeviceProcAddr( device, "vkResetCommandPool" ) );
      vkAllocateCommandBuffers       = PFN_vkAllocateCommandBuffers( vkGetDeviceProcAddr( device, "vkAllocateCommandBuffers" ) );
      vkFreeCommandBuffers           = PFN_vkFreeCommandBuffers( vkGetDeviceProcAddr( device, "vkFreeCommandBuffers" ) );
      vkBeginCommandBuffer           = PFN_vkBeginCommandBuffer( vkGetDeviceProcAddr( device, "vkBeginCommandBuffer" ) );
      vkEndCommandBuffer             = PFN_vkEndCommandBuffer( vkGetDeviceProcAddr( device, "vkEndCommandBuffer" ) );
      vkResetCommandBuffer           = PFN_vkResetCommandBuffer( vkGetDeviceProcAddr( device, "vkResetCommandBuffer" ) );
      vkCmdBindPipeline              = PFN_vkCmdBindPipeline( vkGetDeviceProcAddr( device, "vkCmdBindPipeline" ) );
      vkCmdSetViewport               = PFN_vkCmdSetViewport( vkGetDeviceProcAddr( device, "vkCmdSetViewport" ) );
      vkCmdSetScissor                = PFN_vkCmdSetScissor( vkGetDeviceProcAddr( device, "vkCmdSetScissor" ) );
      vkCmdSetLineWidth              = PFN_vkCmdSetLineWidth( vkGetDeviceProcAddr( device, "vkCmdSetLineWidth" ) );
      vkCmdSetDepthBias              = PFN_vkCmdSetDepthBias( vkGetDeviceProcAddr( device, "vkCmdSetDepthBias" ) );
      vkCmdSetBlendConstants         = PFN_vkCmdSetBlendConstants( vkGetDeviceProcAddr( device, "vkCmdSetBlendConstants" ) );
      vkCmdSetDepthBounds            = PFN_vkCmdSetDepthBounds( vkGetDeviceProcAddr( device, "vkCmdSetDepthBounds" ) );
      vkCmdSetStencilCompareMask     = PFN_vkCmdSetStencilCompareMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilCompareMask" ) );
      vkCmdSetStencilWriteMask       = PFN_vkCmdSetStencilWriteMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilWriteMask" ) );
      vkCmdSetStencilReference       = PFN_vkCmdSetStencilReference( vkGetDeviceProcAddr( device, "vkCmdSetStencilReference" ) );
      vkCmdBindDescriptorSets        = PFN_vkCmdBindDescriptorSets( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorSets" ) );
      vkCmdBindIndexBuffer           = PFN_vkCmdBindIndexBuffer( vkGetDeviceProcAddr( device, "vkCmdBindIndexBuffer" ) );
      vkCmdBindVertexBuffers         = PFN_vkCmdBindVertexBuffers( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers" ) );
      vkCmdDraw                      = PFN_vkCmdDraw( vkGetDeviceProcAddr( device, "vkCmdDraw" ) );
      vkCmdDrawIndexed               = PFN_vkCmdDrawIndexed( vkGetDeviceProcAddr( device, "vkCmdDrawIndexed" ) );
      vkCmdDrawIndirect              = PFN_vkCmdDrawIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndirect" ) );
      vkCmdDrawIndexedIndirect       = PFN_vkCmdDrawIndexedIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirect" ) );
      vkCmdDispatch                  = PFN_vkCmdDispatch( vkGetDeviceProcAddr( device, "vkCmdDispatch" ) );
      vkCmdDispatchIndirect          = PFN_vkCmdDispatchIndirect( vkGetDeviceProcAddr( device, "vkCmdDispatchIndirect" ) );
      vkCmdCopyBuffer                = PFN_vkCmdCopyBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer" ) );
      vkCmdCopyImage                 = PFN_vkCmdCopyImage( vkGetDeviceProcAddr( device, "vkCmdCopyImage" ) );
      vkCmdBlitImage                 = PFN_vkCmdBlitImage( vkGetDeviceProcAddr( device, "vkCmdBlitImage" ) );
      vkCmdCopyBufferToImage         = PFN_vkCmdCopyBufferToImage( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage" ) );
      vkCmdCopyImageToBuffer         = PFN_vkCmdCopyImageToBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer" ) );
      vkCmdUpdateBuffer              = PFN_vkCmdUpdateBuffer( vkGetDeviceProcAddr( device, "vkCmdUpdateBuffer" ) );
      vkCmdFillBuffer                = PFN_vkCmdFillBuffer( vkGetDeviceProcAddr( device, "vkCmdFillBuffer" ) );
      vkCmdClearColorImage           = PFN_vkCmdClearColorImage( vkGetDeviceProcAddr( device, "vkCmdClearColorImage" ) );
      vkCmdClearDepthStencilImage    = PFN_vkCmdClearDepthStencilImage( vkGetDeviceProcAddr( device, "vkCmdClearDepthStencilImage" ) );
      vkCmdClearAttachments          = PFN_vkCmdClearAttachments( vkGetDeviceProcAddr( device, "vkCmdClearAttachments" ) );
      vkCmdResolveImage              = PFN_vkCmdResolveImage( vkGetDeviceProcAddr( device, "vkCmdResolveImage" ) );
      vkCmdSetEvent                  = PFN_vkCmdSetEvent( vkGetDeviceProcAddr( device, "vkCmdSetEvent" ) );
      vkCmdResetEvent                = PFN_vkCmdResetEvent( vkGetDeviceProcAddr( device, "vkCmdResetEvent" ) );
      vkCmdWaitEvents                = PFN_vkCmdWaitEvents( vkGetDeviceProcAddr( device, "vkCmdWaitEvents" ) );
      vkCmdPipelineBarrier           = PFN_vkCmdPipelineBarrier( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier" ) );
      vkCmdBeginQuery                = PFN_vkCmdBeginQuery( vkGetDeviceProcAddr( device, "vkCmdBeginQuery" ) );
      vkCmdEndQuery                  = PFN_vkCmdEndQuery( vkGetDeviceProcAddr( device, "vkCmdEndQuery" ) );
      vkCmdResetQueryPool            = PFN_vkCmdResetQueryPool( vkGetDeviceProcAddr( device, "vkCmdResetQueryPool" ) );
      vkCmdWriteTimestamp            = PFN_vkCmdWriteTimestamp( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp" ) );
      vkCmdCopyQueryPoolResults      = PFN_vkCmdCopyQueryPoolResults( vkGetDeviceProcAddr( device, "vkCmdCopyQueryPoolResults" ) );
      vkCmdPushConstants             = PFN_vkCmdPushConstants( vkGetDeviceProcAddr( device, "vkCmdPushConstants" ) );
      vkCmdBeginRenderPass           = PFN_vkCmdBeginRenderPass( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass" ) );
      vkCmdNextSubpass               = PFN_vkCmdNextSubpass( vkGetDeviceProcAddr( device, "vkCmdNextSubpass" ) );
      vkCmdEndRenderPass             = PFN_vkCmdEndRenderPass( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass" ) );
      vkCmdExecuteCommands           = PFN_vkCmdExecuteCommands( vkGetDeviceProcAddr( device, "vkCmdExecuteCommands" ) );

      //=== VK_VERSION_1_1 ===
      vkBindBufferMemory2                = PFN_vkBindBufferMemory2( vkGetDeviceProcAddr( device, "vkBindBufferMemory2" ) );
      vkBindImageMemory2                 = PFN_vkBindImageMemory2( vkGetDeviceProcAddr( device, "vkBindImageMemory2" ) );
      vkGetDeviceGroupPeerMemoryFeatures = PFN_vkGetDeviceGroupPeerMemoryFeatures( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPeerMemoryFeatures" ) );
      vkCmdSetDeviceMask                 = PFN_vkCmdSetDeviceMask( vkGetDeviceProcAddr( device, "vkCmdSetDeviceMask" ) );
      vkCmdDispatchBase                  = PFN_vkCmdDispatchBase( vkGetDeviceProcAddr( device, "vkCmdDispatchBase" ) );
      vkGetImageMemoryRequirements2      = PFN_vkGetImageMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements2" ) );
      vkGetBufferMemoryRequirements2     = PFN_vkGetBufferMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements2" ) );
      vkGetDeviceQueue2                  = PFN_vkGetDeviceQueue2( vkGetDeviceProcAddr( device, "vkGetDeviceQueue2" ) );
      vkCreateSamplerYcbcrConversion     = PFN_vkCreateSamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkCreateSamplerYcbcrConversion" ) );
      vkDestroySamplerYcbcrConversion    = PFN_vkDestroySamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkDestroySamplerYcbcrConversion" ) );
      vkGetDescriptorSetLayoutSupport    = PFN_vkGetDescriptorSetLayoutSupport( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutSupport" ) );

      //=== VK_VERSION_1_2 ===
      vkCmdDrawIndirectCount          = PFN_vkCmdDrawIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectCount" ) );
      vkCmdDrawIndexedIndirectCount   = PFN_vkCmdDrawIndexedIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirectCount" ) );
      vkCreateRenderPass2             = PFN_vkCreateRenderPass2( vkGetDeviceProcAddr( device, "vkCreateRenderPass2" ) );
      vkCmdBeginRenderPass2           = PFN_vkCmdBeginRenderPass2( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass2" ) );
      vkCmdNextSubpass2               = PFN_vkCmdNextSubpass2( vkGetDeviceProcAddr( device, "vkCmdNextSubpass2" ) );
      vkCmdEndRenderPass2             = PFN_vkCmdEndRenderPass2( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass2" ) );
      vkResetQueryPool                = PFN_vkResetQueryPool( vkGetDeviceProcAddr( device, "vkResetQueryPool" ) );
      vkGetSemaphoreCounterValue      = PFN_vkGetSemaphoreCounterValue( vkGetDeviceProcAddr( device, "vkGetSemaphoreCounterValue" ) );
      vkWaitSemaphores                = PFN_vkWaitSemaphores( vkGetDeviceProcAddr( device, "vkWaitSemaphores" ) );
      vkSignalSemaphore               = PFN_vkSignalSemaphore( vkGetDeviceProcAddr( device, "vkSignalSemaphore" ) );
      vkGetBufferDeviceAddress        = PFN_vkGetBufferDeviceAddress( vkGetDeviceProcAddr( device, "vkGetBufferDeviceAddress" ) );
      vkGetBufferOpaqueCaptureAddress = PFN_vkGetBufferOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetBufferOpaqueCaptureAddress" ) );
      vkGetDeviceMemoryOpaqueCaptureAddress =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryOpaqueCaptureAddress" ) );

      //=== VK_VERSION_1_3 ===
      vkCreatePrivateDataSlot             = PFN_vkCreatePrivateDataSlot( vkGetDeviceProcAddr( device, "vkCreatePrivateDataSlot" ) );
      vkDestroyPrivateDataSlot            = PFN_vkDestroyPrivateDataSlot( vkGetDeviceProcAddr( device, "vkDestroyPrivateDataSlot" ) );
      vkSetPrivateData                    = PFN_vkSetPrivateData( vkGetDeviceProcAddr( device, "vkSetPrivateData" ) );
      vkGetPrivateData                    = PFN_vkGetPrivateData( vkGetDeviceProcAddr( device, "vkGetPrivateData" ) );
      vkCmdSetEvent2                      = PFN_vkCmdSetEvent2( vkGetDeviceProcAddr( device, "vkCmdSetEvent2" ) );
      vkCmdResetEvent2                    = PFN_vkCmdResetEvent2( vkGetDeviceProcAddr( device, "vkCmdResetEvent2" ) );
      vkCmdWaitEvents2                    = PFN_vkCmdWaitEvents2( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2" ) );
      vkCmdPipelineBarrier2               = PFN_vkCmdPipelineBarrier2( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2" ) );
      vkCmdWriteTimestamp2                = PFN_vkCmdWriteTimestamp2( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2" ) );
      vkQueueSubmit2                      = PFN_vkQueueSubmit2( vkGetDeviceProcAddr( device, "vkQueueSubmit2" ) );
      vkCmdCopyBuffer2                    = PFN_vkCmdCopyBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2" ) );
      vkCmdCopyImage2                     = PFN_vkCmdCopyImage2( vkGetDeviceProcAddr( device, "vkCmdCopyImage2" ) );
      vkCmdCopyBufferToImage2             = PFN_vkCmdCopyBufferToImage2( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2" ) );
      vkCmdCopyImageToBuffer2             = PFN_vkCmdCopyImageToBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2" ) );
      vkCmdBlitImage2                     = PFN_vkCmdBlitImage2( vkGetDeviceProcAddr( device, "vkCmdBlitImage2" ) );
      vkCmdResolveImage2                  = PFN_vkCmdResolveImage2( vkGetDeviceProcAddr( device, "vkCmdResolveImage2" ) );
      vkCmdBeginRendering                 = PFN_vkCmdBeginRendering( vkGetDeviceProcAddr( device, "vkCmdBeginRendering" ) );
      vkCmdEndRendering                   = PFN_vkCmdEndRendering( vkGetDeviceProcAddr( device, "vkCmdEndRendering" ) );
      vkCmdSetCullMode                    = PFN_vkCmdSetCullMode( vkGetDeviceProcAddr( device, "vkCmdSetCullMode" ) );
      vkCmdSetFrontFace                   = PFN_vkCmdSetFrontFace( vkGetDeviceProcAddr( device, "vkCmdSetFrontFace" ) );
      vkCmdSetPrimitiveTopology           = PFN_vkCmdSetPrimitiveTopology( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopology" ) );
      vkCmdSetViewportWithCount           = PFN_vkCmdSetViewportWithCount( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCount" ) );
      vkCmdSetScissorWithCount            = PFN_vkCmdSetScissorWithCount( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCount" ) );
      vkCmdBindVertexBuffers2             = PFN_vkCmdBindVertexBuffers2( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2" ) );
      vkCmdSetDepthTestEnable             = PFN_vkCmdSetDepthTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnable" ) );
      vkCmdSetDepthWriteEnable            = PFN_vkCmdSetDepthWriteEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnable" ) );
      vkCmdSetDepthCompareOp              = PFN_vkCmdSetDepthCompareOp( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOp" ) );
      vkCmdSetDepthBoundsTestEnable       = PFN_vkCmdSetDepthBoundsTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnable" ) );
      vkCmdSetStencilTestEnable           = PFN_vkCmdSetStencilTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnable" ) );
      vkCmdSetStencilOp                   = PFN_vkCmdSetStencilOp( vkGetDeviceProcAddr( device, "vkCmdSetStencilOp" ) );
      vkCmdSetRasterizerDiscardEnable     = PFN_vkCmdSetRasterizerDiscardEnable( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnable" ) );
      vkCmdSetDepthBiasEnable             = PFN_vkCmdSetDepthBiasEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnable" ) );
      vkCmdSetPrimitiveRestartEnable      = PFN_vkCmdSetPrimitiveRestartEnable( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnable" ) );
      vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceBufferMemoryRequirements" ) );
      vkGetDeviceImageMemoryRequirements  = PFN_vkGetDeviceImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageMemoryRequirements" ) );
      vkGetDeviceImageSparseMemoryRequirements =
        PFN_vkGetDeviceImageSparseMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageSparseMemoryRequirements" ) );

      //=== VKSC_VERSION_1_0 ===
      vkGetCommandPoolMemoryConsumption = PFN_vkGetCommandPoolMemoryConsumption( vkGetDeviceProcAddr( device, "vkGetCommandPoolMemoryConsumption" ) );
      vkGetFaultData                    = PFN_vkGetFaultData( vkGetDeviceProcAddr( device, "vkGetFaultData" ) );

      //=== VK_KHR_swapchain ===
      vkCreateSwapchainKHR    = PFN_vkCreateSwapchainKHR( vkGetDeviceProcAddr( device, "vkCreateSwapchainKHR" ) );
      vkGetSwapchainImagesKHR = PFN_vkGetSwapchainImagesKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainImagesKHR" ) );
      vkAcquireNextImageKHR   = PFN_vkAcquireNextImageKHR( vkGetDeviceProcAddr( device, "vkAcquireNextImageKHR" ) );
      vkQueuePresentKHR       = PFN_vkQueuePresentKHR( vkGetDeviceProcAddr( device, "vkQueuePresentKHR" ) );
      vkGetDeviceGroupPresentCapabilitiesKHR =
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPresentCapabilitiesKHR" ) );
      vkGetDeviceGroupSurfacePresentModesKHR =
        PFN_vkGetDeviceGroupSurfacePresentModesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupSurfacePresentModesKHR" ) );
      vkAcquireNextImage2KHR = PFN_vkAcquireNextImage2KHR( vkGetDeviceProcAddr( device, "vkAcquireNextImage2KHR" ) );

      //=== VK_KHR_display_swapchain ===
      vkCreateSharedSwapchainsKHR = PFN_vkCreateSharedSwapchainsKHR( vkGetDeviceProcAddr( device, "vkCreateSharedSwapchainsKHR" ) );

      //=== VK_KHR_external_memory_fd ===
      vkGetMemoryFdKHR           = PFN_vkGetMemoryFdKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdKHR" ) );
      vkGetMemoryFdPropertiesKHR = PFN_vkGetMemoryFdPropertiesKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdPropertiesKHR" ) );

      //=== VK_KHR_external_semaphore_fd ===
      vkImportSemaphoreFdKHR = PFN_vkImportSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkImportSemaphoreFdKHR" ) );
      vkGetSemaphoreFdKHR    = PFN_vkGetSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkGetSemaphoreFdKHR" ) );

      //=== VK_EXT_display_control ===
      vkDisplayPowerControlEXT  = PFN_vkDisplayPowerControlEXT( vkGetDeviceProcAddr( device, "vkDisplayPowerControlEXT" ) );
      vkRegisterDeviceEventEXT  = PFN_vkRegisterDeviceEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDeviceEventEXT" ) );
      vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDisplayEventEXT" ) );
      vkGetSwapchainCounterEXT  = PFN_vkGetSwapchainCounterEXT( vkGetDeviceProcAddr( device, "vkGetSwapchainCounterEXT" ) );

      //=== VK_EXT_discard_rectangles ===
      vkCmdSetDiscardRectangleEXT       = PFN_vkCmdSetDiscardRectangleEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEXT" ) );
      vkCmdSetDiscardRectangleEnableEXT = PFN_vkCmdSetDiscardRectangleEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEnableEXT" ) );
      vkCmdSetDiscardRectangleModeEXT   = PFN_vkCmdSetDiscardRectangleModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleModeEXT" ) );

      //=== VK_EXT_hdr_metadata ===
      vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT( vkGetDeviceProcAddr( device, "vkSetHdrMetadataEXT" ) );

      //=== VK_KHR_shared_presentable_image ===
      vkGetSwapchainStatusKHR = PFN_vkGetSwapchainStatusKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainStatusKHR" ) );

      //=== VK_KHR_external_fence_fd ===
      vkImportFenceFdKHR = PFN_vkImportFenceFdKHR( vkGetDeviceProcAddr( device, "vkImportFenceFdKHR" ) );
      vkGetFenceFdKHR    = PFN_vkGetFenceFdKHR( vkGetDeviceProcAddr( device, "vkGetFenceFdKHR" ) );

      //=== VK_KHR_performance_query ===
      vkAcquireProfilingLockKHR = PFN_vkAcquireProfilingLockKHR( vkGetDeviceProcAddr( device, "vkAcquireProfilingLockKHR" ) );
      vkReleaseProfilingLockKHR = PFN_vkReleaseProfilingLockKHR( vkGetDeviceProcAddr( device, "vkReleaseProfilingLockKHR" ) );

      //=== VK_EXT_debug_utils ===
      vkSetDebugUtilsObjectNameEXT    = PFN_vkSetDebugUtilsObjectNameEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectNameEXT" ) );
      vkSetDebugUtilsObjectTagEXT     = PFN_vkSetDebugUtilsObjectTagEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectTagEXT" ) );
      vkQueueBeginDebugUtilsLabelEXT  = PFN_vkQueueBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueBeginDebugUtilsLabelEXT" ) );
      vkQueueEndDebugUtilsLabelEXT    = PFN_vkQueueEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueEndDebugUtilsLabelEXT" ) );
      vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueInsertDebugUtilsLabelEXT" ) );
      vkCmdBeginDebugUtilsLabelEXT    = PFN_vkCmdBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdBeginDebugUtilsLabelEXT" ) );
      vkCmdEndDebugUtilsLabelEXT      = PFN_vkCmdEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdEndDebugUtilsLabelEXT" ) );
      vkCmdInsertDebugUtilsLabelEXT   = PFN_vkCmdInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdInsertDebugUtilsLabelEXT" ) );

      //=== VK_EXT_sample_locations ===
      vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT( vkGetDeviceProcAddr( device, "vkCmdSetSampleLocationsEXT" ) );

      //=== VK_EXT_image_drm_format_modifier ===
      vkGetImageDrmFormatModifierPropertiesEXT =
        PFN_vkGetImageDrmFormatModifierPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetImageDrmFormatModifierPropertiesEXT" ) );

      //=== VK_EXT_external_memory_host ===
      vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetMemoryHostPointerPropertiesEXT" ) );

      //=== VK_EXT_calibrated_timestamps ===
      vkGetCalibratedTimestampsEXT = PFN_vkGetCalibratedTimestampsEXT( vkGetDeviceProcAddr( device, "vkGetCalibratedTimestampsEXT" ) );
      if ( !vkGetCalibratedTimestampsKHR )
        vkGetCalibratedTimestampsKHR = vkGetCalibratedTimestampsEXT;

      //=== VK_KHR_fragment_shading_rate ===
      vkCmdSetFragmentShadingRateKHR = PFN_vkCmdSetFragmentShadingRateKHR( vkGetDeviceProcAddr( device, "vkCmdSetFragmentShadingRateKHR" ) );

      //=== VK_EXT_line_rasterization ===
      vkCmdSetLineStippleEXT = PFN_vkCmdSetLineStippleEXT( vkGetDeviceProcAddr( device, "vkCmdSetLineStippleEXT" ) );
      if ( !vkCmdSetLineStippleKHR )
        vkCmdSetLineStippleKHR = vkCmdSetLineStippleEXT;

      //=== VK_EXT_extended_dynamic_state ===
      vkCmdSetCullModeEXT = PFN_vkCmdSetCullModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetCullModeEXT" ) );
      if ( !vkCmdSetCullMode )
        vkCmdSetCullMode = vkCmdSetCullModeEXT;
      vkCmdSetFrontFaceEXT = PFN_vkCmdSetFrontFaceEXT( vkGetDeviceProcAddr( device, "vkCmdSetFrontFaceEXT" ) );
      if ( !vkCmdSetFrontFace )
        vkCmdSetFrontFace = vkCmdSetFrontFaceEXT;
      vkCmdSetPrimitiveTopologyEXT = PFN_vkCmdSetPrimitiveTopologyEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopologyEXT" ) );
      if ( !vkCmdSetPrimitiveTopology )
        vkCmdSetPrimitiveTopology = vkCmdSetPrimitiveTopologyEXT;
      vkCmdSetViewportWithCountEXT = PFN_vkCmdSetViewportWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCountEXT" ) );
      if ( !vkCmdSetViewportWithCount )
        vkCmdSetViewportWithCount = vkCmdSetViewportWithCountEXT;
      vkCmdSetScissorWithCountEXT = PFN_vkCmdSetScissorWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCountEXT" ) );
      if ( !vkCmdSetScissorWithCount )
        vkCmdSetScissorWithCount = vkCmdSetScissorWithCountEXT;
      vkCmdBindVertexBuffers2EXT = PFN_vkCmdBindVertexBuffers2EXT( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2EXT" ) );
      if ( !vkCmdBindVertexBuffers2 )
        vkCmdBindVertexBuffers2 = vkCmdBindVertexBuffers2EXT;
      vkCmdSetDepthTestEnableEXT = PFN_vkCmdSetDepthTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnableEXT" ) );
      if ( !vkCmdSetDepthTestEnable )
        vkCmdSetDepthTestEnable = vkCmdSetDepthTestEnableEXT;
      vkCmdSetDepthWriteEnableEXT = PFN_vkCmdSetDepthWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnableEXT" ) );
      if ( !vkCmdSetDepthWriteEnable )
        vkCmdSetDepthWriteEnable = vkCmdSetDepthWriteEnableEXT;
      vkCmdSetDepthCompareOpEXT = PFN_vkCmdSetDepthCompareOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOpEXT" ) );
      if ( !vkCmdSetDepthCompareOp )
        vkCmdSetDepthCompareOp = vkCmdSetDepthCompareOpEXT;
      vkCmdSetDepthBoundsTestEnableEXT = PFN_vkCmdSetDepthBoundsTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnableEXT" ) );
      if ( !vkCmdSetDepthBoundsTestEnable )
        vkCmdSetDepthBoundsTestEnable = vkCmdSetDepthBoundsTestEnableEXT;
      vkCmdSetStencilTestEnableEXT = PFN_vkCmdSetStencilTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnableEXT" ) );
      if ( !vkCmdSetStencilTestEnable )
        vkCmdSetStencilTestEnable = vkCmdSetStencilTestEnableEXT;
      vkCmdSetStencilOpEXT = PFN_vkCmdSetStencilOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilOpEXT" ) );
      if ( !vkCmdSetStencilOp )
        vkCmdSetStencilOp = vkCmdSetStencilOpEXT;

      //=== VK_KHR_object_refresh ===
      vkCmdRefreshObjectsKHR = PFN_vkCmdRefreshObjectsKHR( vkGetDeviceProcAddr( device, "vkCmdRefreshObjectsKHR" ) );

      //=== VK_KHR_synchronization2 ===
      vkCmdSetEvent2KHR = PFN_vkCmdSetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdSetEvent2KHR" ) );
      if ( !vkCmdSetEvent2 )
        vkCmdSetEvent2 = vkCmdSetEvent2KHR;
      vkCmdResetEvent2KHR = PFN_vkCmdResetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdResetEvent2KHR" ) );
      if ( !vkCmdResetEvent2 )
        vkCmdResetEvent2 = vkCmdResetEvent2KHR;
      vkCmdWaitEvents2KHR = PFN_vkCmdWaitEvents2KHR( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2KHR" ) );
      if ( !vkCmdWaitEvents2 )
        vkCmdWaitEvents2 = vkCmdWaitEvents2KHR;
      vkCmdPipelineBarrier2KHR = PFN_vkCmdPipelineBarrier2KHR( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2KHR" ) );
      if ( !vkCmdPipelineBarrier2 )
        vkCmdPipelineBarrier2 = vkCmdPipelineBarrier2KHR;
      vkCmdWriteTimestamp2KHR = PFN_vkCmdWriteTimestamp2KHR( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2KHR" ) );
      if ( !vkCmdWriteTimestamp2 )
        vkCmdWriteTimestamp2 = vkCmdWriteTimestamp2KHR;
      vkQueueSubmit2KHR = PFN_vkQueueSubmit2KHR( vkGetDeviceProcAddr( device, "vkQueueSubmit2KHR" ) );
      if ( !vkQueueSubmit2 )
        vkQueueSubmit2 = vkQueueSubmit2KHR;
      vkCmdWriteBufferMarker2AMD  = PFN_vkCmdWriteBufferMarker2AMD( vkGetDeviceProcAddr( device, "vkCmdWriteBufferMarker2AMD" ) );
      vkGetQueueCheckpointData2NV = PFN_vkGetQueueCheckpointData2NV( vkGetDeviceProcAddr( device, "vkGetQueueCheckpointData2NV" ) );

      //=== VK_KHR_copy_commands2 ===
      vkCmdCopyBuffer2KHR = PFN_vkCmdCopyBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2KHR" ) );
      if ( !vkCmdCopyBuffer2 )
        vkCmdCopyBuffer2 = vkCmdCopyBuffer2KHR;
      vkCmdCopyImage2KHR = PFN_vkCmdCopyImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImage2KHR" ) );
      if ( !vkCmdCopyImage2 )
        vkCmdCopyImage2 = vkCmdCopyImage2KHR;
      vkCmdCopyBufferToImage2KHR = PFN_vkCmdCopyBufferToImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2KHR" ) );
      if ( !vkCmdCopyBufferToImage2 )
        vkCmdCopyBufferToImage2 = vkCmdCopyBufferToImage2KHR;
      vkCmdCopyImageToBuffer2KHR = PFN_vkCmdCopyImageToBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2KHR" ) );
      if ( !vkCmdCopyImageToBuffer2 )
        vkCmdCopyImageToBuffer2 = vkCmdCopyImageToBuffer2KHR;
      vkCmdBlitImage2KHR = PFN_vkCmdBlitImage2KHR( vkGetDeviceProcAddr( device, "vkCmdBlitImage2KHR" ) );
      if ( !vkCmdBlitImage2 )
        vkCmdBlitImage2 = vkCmdBlitImage2KHR;
      vkCmdResolveImage2KHR = PFN_vkCmdResolveImage2KHR( vkGetDeviceProcAddr( device, "vkCmdResolveImage2KHR" ) );
      if ( !vkCmdResolveImage2 )
        vkCmdResolveImage2 = vkCmdResolveImage2KHR;

      //=== VK_EXT_vertex_input_dynamic_state ===
      vkCmdSetVertexInputEXT = PFN_vkCmdSetVertexInputEXT( vkGetDeviceProcAddr( device, "vkCmdSetVertexInputEXT" ) );

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync ===
      vkGetFenceSciSyncFenceNV      = PFN_vkGetFenceSciSyncFenceNV( vkGetDeviceProcAddr( device, "vkGetFenceSciSyncFenceNV" ) );
      vkGetFenceSciSyncObjNV        = PFN_vkGetFenceSciSyncObjNV( vkGetDeviceProcAddr( device, "vkGetFenceSciSyncObjNV" ) );
      vkImportFenceSciSyncFenceNV   = PFN_vkImportFenceSciSyncFenceNV( vkGetDeviceProcAddr( device, "vkImportFenceSciSyncFenceNV" ) );
      vkImportFenceSciSyncObjNV     = PFN_vkImportFenceSciSyncObjNV( vkGetDeviceProcAddr( device, "vkImportFenceSciSyncObjNV" ) );
      vkGetSemaphoreSciSyncObjNV    = PFN_vkGetSemaphoreSciSyncObjNV( vkGetDeviceProcAddr( device, "vkGetSemaphoreSciSyncObjNV" ) );
      vkImportSemaphoreSciSyncObjNV = PFN_vkImportSemaphoreSciSyncObjNV( vkGetDeviceProcAddr( device, "vkImportSemaphoreSciSyncObjNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_memory_sci_buf ===
      vkGetMemorySciBufNV = PFN_vkGetMemorySciBufNV( vkGetDeviceProcAddr( device, "vkGetMemorySciBufNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

      //=== VK_EXT_extended_dynamic_state2 ===
      vkCmdSetPatchControlPointsEXT      = PFN_vkCmdSetPatchControlPointsEXT( vkGetDeviceProcAddr( device, "vkCmdSetPatchControlPointsEXT" ) );
      vkCmdSetRasterizerDiscardEnableEXT = PFN_vkCmdSetRasterizerDiscardEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnableEXT" ) );
      if ( !vkCmdSetRasterizerDiscardEnable )
        vkCmdSetRasterizerDiscardEnable = vkCmdSetRasterizerDiscardEnableEXT;
      vkCmdSetDepthBiasEnableEXT = PFN_vkCmdSetDepthBiasEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnableEXT" ) );
      if ( !vkCmdSetDepthBiasEnable )
        vkCmdSetDepthBiasEnable = vkCmdSetDepthBiasEnableEXT;
      vkCmdSetLogicOpEXT                = PFN_vkCmdSetLogicOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetLogicOpEXT" ) );
      vkCmdSetPrimitiveRestartEnableEXT = PFN_vkCmdSetPrimitiveRestartEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnableEXT" ) );
      if ( !vkCmdSetPrimitiveRestartEnable )
        vkCmdSetPrimitiveRestartEnable = vkCmdSetPrimitiveRestartEnableEXT;

      //=== VK_EXT_color_write_enable ===
      vkCmdSetColorWriteEnableEXT = PFN_vkCmdSetColorWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorWriteEnableEXT" ) );

#if defined( VK_USE_PLATFORM_SCI )
      //=== VK_NV_external_sci_sync2 ===
      vkCreateSemaphoreSciSyncPoolNV = PFN_vkCreateSemaphoreSciSyncPoolNV( vkGetDeviceProcAddr( device, "vkCreateSemaphoreSciSyncPoolNV" ) );
#endif /*VK_USE_PLATFORM_SCI*/

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===
      vkGetScreenBufferPropertiesQNX = PFN_vkGetScreenBufferPropertiesQNX( vkGetDeviceProcAddr( device, "vkGetScreenBufferPropertiesQNX" ) );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/
    }

    template <typename DynamicLoader>
    void init( VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device, DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      PFN_vkGetDeviceProcAddr   getDeviceProcAddr   = dl.template getProcAddress<PFN_vkGetDeviceProcAddr>( "vkGetDeviceProcAddr" );
      init( static_cast<VkInstance>( instance ), getInstanceProcAddr, static_cast<VkDevice>( device ), device ? getDeviceProcAddr : nullptr );
    }

    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
              = VULKAN_HPP_NAMESPACE::DynamicLoader
#endif
              >
    void init( VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device ) VULKAN_HPP_NOEXCEPT
    {
      static DynamicLoader dl;
      init( instance, device, dl );
    }
  };
}  // namespace VULKAN_HPP_NAMESPACE
#endif
