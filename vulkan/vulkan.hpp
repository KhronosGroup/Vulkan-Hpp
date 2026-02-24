// Copyright 2015-2026 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_HPP
#define VULKAN_HPP

// DEBUG: temporarily disable all smart handles
#ifndef VULKAN_HPP_NO_SMART_HANDLE
#  define VULKAN_HPP_NO_SMART_HANDLE
#endif

#if !defined( VULKAN_HPP_CXX_MODULE )
#  include <vulkan/vulkan.h>
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

VULKAN_HPP_STATIC_ASSERT( VK_HEADER_VERSION == 344, "Wrong VK_HEADER_VERSION!" );

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
#  if !defined( VULKAN_HPP_DEFAULT_DISPATCHER_HANDLED )
  // dispatcher will be placed once in user code
  extern VULKAN_HPP_STORAGE_API DispatchLoader defaultDispatchLoader;
#  endif
#else
  // dispatcher will be placed into every including TU
  static vk::DispatchLoader defaultDispatchLoader;
#endif
}  // namespace VULKAN_HPP_NAMESPACE

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

// XLib.h defines True/False, which collides with our VULKAN_HPP_NAMESPACE::True/VULKAN_HPP_NAMESPACE::False
// ->  undef them and provide some namepace-secure constexpr
#if defined( True )
#  undef True
constexpr int True = 1;
#endif
#if defined( False )
#  undef False
constexpr int False = 0;
#endif

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
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

  private:
    VULKAN_HPP_CONSTEXPR_14 void copy( char const * data, size_t len ) VULKAN_HPP_NOEXCEPT
    {
      size_t n = (std::min)( N - 1, len );
      for ( size_t i = 0; i < n; ++i )
      {
        ( *this )[i] = data[i];
      }
      ( *this )[n] = 0;
    }
  };

// relational operators between ArrayWrapper1D of chars with potentially different sizes
#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  template <size_t N, size_t M>
  std::strong_ordering operator<=>( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    int result = strcmp( lhs.data(), rhs.data() );
    return ( result < 0 ) ? std::strong_ordering::less : ( ( result > 0 ) ? std::strong_ordering::greater : std::strong_ordering::equal );
  }
#else
  template <size_t N, size_t M>
  bool operator<( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) < 0;
  }

  template <size_t N, size_t M>
  bool operator<=( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) <= 0;
  }

  template <size_t N, size_t M>
  bool operator>( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) > 0;
  }

  template <size_t N, size_t M>
  bool operator>=( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) >= 0;
  }
#endif

  template <size_t N, size_t M>
  bool operator==( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) == 0;
  }

  template <size_t N, size_t M>
  bool operator!=( ArrayWrapper1D<char, N> const & lhs, ArrayWrapper1D<char, M> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return strcmp( lhs.data(), rhs.data() ) != 0;
  }

// specialization of relational operators between std::string and arrays of chars
#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  template <size_t N>
  std::strong_ordering operator<=>( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs <=> rhs.data();
  }
#else
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
#endif

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

#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  template <size_t N>
  std::strong_ordering operator<=>( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() <=> rhs;
  }
#else
  template <size_t N>
  bool operator<( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() < rhs;
  }

  template <size_t N>
  bool operator<=( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() <= rhs;
  }

  template <size_t N>
  bool operator>( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() > rhs;
  }

  template <size_t N>
  bool operator>=( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() >= rhs;
  }
#endif

  template <size_t N>
  bool operator==( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() == rhs;
  }

  template <size_t N>
  bool operator!=( ArrayWrapper1D<char, N> const & lhs, std::string const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs.data() != rhs;
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

    T const * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    T const * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    T const & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    T const & back() const VULKAN_HPP_NOEXCEPT
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

    template <typename B = T, typename std::enable_if<std::is_convertible<B, T>::value && std::is_lvalue_reference<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( B && value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {
    }

    ArrayProxyNoTemporaries( uint32_t count, T * ptr ) VULKAN_HPP_NOEXCEPT
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

    // Any l-value reference with a .data() return type implicitly convertible to T*, and a .size() return type implicitly convertible to size_t.
    template <typename V,
              typename std::enable_if<!std::is_convertible<decltype( std::declval<V>().begin() ), T *>::value &&
                                        std::is_convertible<decltype( std::declval<V>().data() ), T *>::value &&
                                        std::is_convertible<decltype( std::declval<V>().size() ), std::size_t>::value &&
                                        std::is_lvalue_reference<V>::value,
                                      int>::type = 0>
    ArrayProxyNoTemporaries( V && v ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( v.size() ) )
      , m_ptr( v.data() )
    {
    }

    // Any l-value reference with a .begin() return type implicitly convertible to T*, and a .size() return type implicitly convertible to size_t.
    template <typename V,
              typename std::enable_if<std::is_convertible<decltype( std::declval<V>().begin() ), T *>::value &&
                                        std::is_convertible<decltype( std::declval<V>().size() ), std::size_t>::value &&
                                        std::is_lvalue_reference<V>::value,
                                      int>::type = 0>
    ArrayProxyNoTemporaries( V && v ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( v.size() ) )
      , m_ptr( v.begin() )
    {
    }

    T const * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    T const * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    T const & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    T const & back() const VULKAN_HPP_NOEXCEPT
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

    T const * end() const VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<T const *>( static_cast<uint8_t const *>( begin() ) + size() * m_stride );
    }

    using ArrayProxy<T>::front;

    T const & back() const VULKAN_HPP_NOEXCEPT
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

    RefType const * get() const VULKAN_HPP_NOEXCEPT
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
    static bool const valid = false;
  };

  template <typename Type, typename Head, typename... Tail>
  struct IsPartOfStructureChain<Type, Head, Tail...>
  {
    static bool const valid = std::is_same<Type, Head>::value || IsPartOfStructureChain<Type, Tail...>::valid;
  };

  template <size_t Index, typename T, typename... ChainElements>
  struct StructureChainContains
  {
    static bool const value = std::is_same<T, typename std::tuple_element<Index, std::tuple<ChainElements...>>::type>::value ||
                              StructureChainContains<Index - 1, T, ChainElements...>::value;
  };

  template <typename T, typename... ChainElements>
  struct StructureChainContains<0, T, ChainElements...>
  {
    static bool const value = std::is_same<T, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value;
  };

  template <size_t Index, typename... ChainElements>
  struct StructureChainValidation
  {
    using TestType          = typename std::tuple_element<Index, std::tuple<ChainElements...>>::type;
    static bool const valid = StructExtends<TestType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value &&
                              ( TestType::allowDuplicate || !StructureChainContains<Index - 1, TestType, ChainElements...>::value ) &&
                              StructureChainValidation<Index - 1, ChainElements...>::valid;
  };

  template <typename... ChainElements>
  struct StructureChainValidation<0, ChainElements...>
  {
    static bool const valid = true;
  };

  template <typename... ChainElements>
  class StructureChain : public std::tuple<ChainElements...>
  {
    // Note: StructureChain has no move constructor or move assignment operator, as it is not supposed to contain movable containers.
    //       In order to get a copy-operation on a move-operations, those functions are neither deleted nor defaulted.
  public:
    StructureChain() VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_STATIC_ASSERT( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain( StructureChain const & rhs ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( rhs )
    {
      VULKAN_HPP_STATIC_ASSERT( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
      link( &std::get<0>( *this ),
            &std::get<0>( rhs ),
            reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( *this ) ),
            reinterpret_cast<VkBaseInStructure const *>( &std::get<0>( rhs ) ) );
    }

    StructureChain( ChainElements const &... elems ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( elems... )
    {
      VULKAN_HPP_STATIC_ASSERT( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid, "The structure chain is not valid!" );
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

#  if defined( VULKAN_HPP_USE_REFLECT ) && ( 14 <= VULKAN_HPP_CPP_VERSION )
  private:
    // some helper structs to strip away the first two elements from a tuple
    template <std::size_t I, std::size_t N, std::size_t... integers>
    struct makeIndexSequenceHelper
    {
      using type = typename makeIndexSequenceHelper<I + 1, N, integers..., I>::type;
    };

    template <std::size_t N, std::size_t... integers>
    struct makeIndexSequenceHelper<N, N, integers...>
    {
      using type = std::index_sequence<integers...>;
    };

    template <std::size_t I, std::size_t N>
    using makeIndexSequence = typename makeIndexSequenceHelper<I, N>::type;

    template <typename Tuple, std::size_t... Is>
    auto subTuple( Tuple & t, std::index_sequence<Is...> )
    {
      return std::make_tuple( std::get<Is>( t )... );
    }

  public:
    // compare a complete structure in the StructureChain, ignoring the chaining
    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    VULKAN_HPP_NODISCARD bool elementEquals( T rhs ) VULKAN_HPP_NOEXCEPT
    {
      auto lhsTuple = get<T, Which>().reflect();
      auto rhsTuple = rhs.reflect();
      // skip the first two members: sType and pNext
      auto indexSequence = makeIndexSequence<2, std::tuple_size<decltype( lhsTuple )>{}>{};
      return subTuple( lhsTuple, indexSequence ) == subTuple( rhsTuple, indexSequence );
    }
#  endif

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
      VULKAN_HPP_NODISCARD T & get() & VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( static_cast<std::tuple<ChainElements...> &>( *this ) );
    }

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    VULKAN_HPP_NODISCARD T const & get() const & VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( static_cast<std::tuple<ChainElements...> const &>( *this ) );
    }

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
      VULKAN_HPP_NODISCARD T && get() && VULKAN_HPP_NOEXCEPT
    {
      return std::move( std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( static_cast<std::tuple<ChainElements...> &>( *this ) ) );
    }

    template <typename T0, typename T1, typename... Ts>
      VULKAN_HPP_NODISCARD std::tuple<T0 &, T1 &, Ts &...> get() & VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    template <typename T0, typename T1, typename... Ts>
    VULKAN_HPP_NODISCARD std::tuple<T0 const &, T1 const &, Ts const &...> get() const & VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    template <typename T0, typename T1, typename... Ts>
      VULKAN_HPP_NODISCARD std::tuple<T0 &&, T1 &&, Ts &&...> get() && VULKAN_HPP_NOEXCEPT
    {
      return std::forward_as_tuple( std::move( get<T0>() ), std::move( get<T1>() ), std::move( get<Ts>() )... );
    }

    template <typename T0, typename T1, typename... Ts>
    VULKAN_HPP_NODISCARD std::tuple<T0 const &&, T1 const &&, Ts const &&...> get() const && VULKAN_HPP_NOEXCEPT
    {
      return std::forward_as_tuple( std::move( get<T0>() ), std::move( get<T1>() ), std::move( get<Ts>() )... );
    }

    // assign a complete structure to the StructureChain without modifying the chaining
    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    StructureChain & assign( T const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      T &  lhs   = get<T, Which>();
      auto pNext = lhs.pNext;
      lhs        = rhs;
      lhs.pNext  = pNext;
      return *this;
    }

    template <typename ClassType, size_t Which = 0>
    VULKAN_HPP_NODISCARD
      typename std::enable_if<std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value && ( Which == 0 ), bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      return true;
    }

    template <typename ClassType, size_t Which = 0>
    VULKAN_HPP_NODISCARD
      typename std::enable_if<!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || ( Which != 0 ), bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_STATIC_ASSERT( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't unlink Structure that's not part of this StructureChain!" );
      return isLinked( reinterpret_cast<VkBaseInStructure const *>( &get<ClassType, Which>() ) );
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || ( Which != 0 ), void>::type
      relink() VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_STATIC_ASSERT( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't relink Structure that's not part of this StructureChain!" );
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
      VULKAN_HPP_STATIC_ASSERT( IsPartOfStructureChain<ClassType, ChainElements...>::valid, "Can't unlink Structure that's not part of this StructureChain!" );
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

    VULKAN_HPP_NODISCARD bool isLinked( VkBaseInStructure const * pNext ) const VULKAN_HPP_NOEXCEPT
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

VULKAN_HPP_EXPORT namespace std
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

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
#  if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  template <typename Type>
  class UniqueHandleTraits;

  template <typename Type>
  class UniqueHandle : public UniqueHandleTraits<Type>::deleter
  {
  private:
    using Deleter = typename UniqueHandleTraits<Type>::deleter;

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

    Type const & get() const VULKAN_HPP_NOEXCEPT
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

    void swap( UniqueHandle<Type> & rhs ) VULKAN_HPP_NOEXCEPT
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

  template <typename Type>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type> & lhs, UniqueHandle<Type> & rhs ) VULKAN_HPP_NOEXCEPT
  {
    lhs.swap( rhs );
  }
#  endif
#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

  namespace detail
  {
    //==========================
    //=== Is Dispatched List ===
    //==========================

    // C++11-compatible void_t
    template <typename...>
    struct voider
    {
      typedef void type;
    };

    template <typename... Ts>
    using void_t = typename voider<Ts...>::type;

    // helper macro to declare a SFINAE-friendly has_<fn> trait
#define DECLARE_IS_DISPATCHED( name )                                 \
  template <typename D, typename = void>                              \
  struct has_##name : std::false_type                                 \
  {                                                                   \
  };                                                                  \
  template <typename D>                                               \
  struct has_##name<D, void_t<decltype( &D::name )>> : std::true_type \
  {                                                                   \
  };

    DECLARE_IS_DISPATCHED( vkAcquireDrmDisplayEXT )
    DECLARE_IS_DISPATCHED( vkAcquireFullScreenExclusiveModeEXT )
    DECLARE_IS_DISPATCHED( vkAcquireImageANDROID )
    DECLARE_IS_DISPATCHED( vkAcquireImageOHOS )
    DECLARE_IS_DISPATCHED( vkAcquireNextImage2KHR )
    DECLARE_IS_DISPATCHED( vkAcquireNextImageKHR )
    DECLARE_IS_DISPATCHED( vkAcquirePerformanceConfigurationINTEL )
    DECLARE_IS_DISPATCHED( vkAcquireProfilingLockKHR )
    DECLARE_IS_DISPATCHED( vkAcquireWinrtDisplayNV )
    DECLARE_IS_DISPATCHED( vkAcquireXlibDisplayEXT )
    DECLARE_IS_DISPATCHED( vkAllocateCommandBuffers )
    DECLARE_IS_DISPATCHED( vkAllocateDescriptorSets )
    DECLARE_IS_DISPATCHED( vkAllocateMemory )
    DECLARE_IS_DISPATCHED( vkAntiLagUpdateAMD )
    DECLARE_IS_DISPATCHED( vkBeginCommandBuffer )
    DECLARE_IS_DISPATCHED( vkBindAccelerationStructureMemoryNV )
    DECLARE_IS_DISPATCHED( vkBindBufferMemory )
    DECLARE_IS_DISPATCHED( vkBindBufferMemory2 )
    DECLARE_IS_DISPATCHED( vkBindBufferMemory2KHR )
    DECLARE_IS_DISPATCHED( vkBindDataGraphPipelineSessionMemoryARM )
    DECLARE_IS_DISPATCHED( vkBindImageMemory )
    DECLARE_IS_DISPATCHED( vkBindImageMemory2 )
    DECLARE_IS_DISPATCHED( vkBindImageMemory2KHR )
    DECLARE_IS_DISPATCHED( vkBindOpticalFlowSessionImageNV )
    DECLARE_IS_DISPATCHED( vkBindTensorMemoryARM )
    DECLARE_IS_DISPATCHED( vkBindVideoSessionMemoryKHR )
    DECLARE_IS_DISPATCHED( vkBuildAccelerationStructuresKHR )
    DECLARE_IS_DISPATCHED( vkBuildMicromapsEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginConditionalRenderingEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginCustomResolveEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginPerTileExecutionQCOM )
    DECLARE_IS_DISPATCHED( vkCmdBeginQuery )
    DECLARE_IS_DISPATCHED( vkCmdBeginQueryIndexedEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginRenderPass )
    DECLARE_IS_DISPATCHED( vkCmdBeginRenderPass2 )
    DECLARE_IS_DISPATCHED( vkCmdBeginRenderPass2KHR )
    DECLARE_IS_DISPATCHED( vkCmdBeginRendering )
    DECLARE_IS_DISPATCHED( vkCmdBeginRenderingKHR )
    DECLARE_IS_DISPATCHED( vkCmdBeginTransformFeedbackEXT )
    DECLARE_IS_DISPATCHED( vkCmdBeginVideoCodingKHR )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorBufferEmbeddedSamplers2EXT )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorBufferEmbeddedSamplersEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorBuffersEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorSets )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorSets2 )
    DECLARE_IS_DISPATCHED( vkCmdBindDescriptorSets2KHR )
    DECLARE_IS_DISPATCHED( vkCmdBindIndexBuffer )
    DECLARE_IS_DISPATCHED( vkCmdBindIndexBuffer2 )
    DECLARE_IS_DISPATCHED( vkCmdBindIndexBuffer2KHR )
    DECLARE_IS_DISPATCHED( vkCmdBindInvocationMaskHUAWEI )
    DECLARE_IS_DISPATCHED( vkCmdBindPipeline )
    DECLARE_IS_DISPATCHED( vkCmdBindPipelineShaderGroupNV )
    DECLARE_IS_DISPATCHED( vkCmdBindResourceHeapEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindSamplerHeapEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindShadersEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindShadingRateImageNV )
    DECLARE_IS_DISPATCHED( vkCmdBindTileMemoryQCOM )
    DECLARE_IS_DISPATCHED( vkCmdBindTransformFeedbackBuffersEXT )
    DECLARE_IS_DISPATCHED( vkCmdBindVertexBuffers )
    DECLARE_IS_DISPATCHED( vkCmdBindVertexBuffers2 )
    DECLARE_IS_DISPATCHED( vkCmdBindVertexBuffers2EXT )
    DECLARE_IS_DISPATCHED( vkCmdBlitImage )
    DECLARE_IS_DISPATCHED( vkCmdBlitImage2 )
    DECLARE_IS_DISPATCHED( vkCmdBlitImage2KHR )
    DECLARE_IS_DISPATCHED( vkCmdBuildAccelerationStructureNV )
    DECLARE_IS_DISPATCHED( vkCmdBuildAccelerationStructuresIndirectKHR )
    DECLARE_IS_DISPATCHED( vkCmdBuildAccelerationStructuresKHR )
    DECLARE_IS_DISPATCHED( vkCmdBuildClusterAccelerationStructureIndirectNV )
    DECLARE_IS_DISPATCHED( vkCmdBuildMicromapsEXT )
    DECLARE_IS_DISPATCHED( vkCmdBuildPartitionedAccelerationStructuresNV )
    DECLARE_IS_DISPATCHED( vkCmdClearAttachments )
    DECLARE_IS_DISPATCHED( vkCmdClearColorImage )
    DECLARE_IS_DISPATCHED( vkCmdClearDepthStencilImage )
    DECLARE_IS_DISPATCHED( vkCmdControlVideoCodingKHR )
    DECLARE_IS_DISPATCHED( vkCmdConvertCooperativeVectorMatrixNV )
    DECLARE_IS_DISPATCHED( vkCmdCopyAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyAccelerationStructureNV )
    DECLARE_IS_DISPATCHED( vkCmdCopyAccelerationStructureToMemoryKHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyBuffer )
    DECLARE_IS_DISPATCHED( vkCmdCopyBuffer2 )
    DECLARE_IS_DISPATCHED( vkCmdCopyBuffer2KHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyBufferToImage )
    DECLARE_IS_DISPATCHED( vkCmdCopyBufferToImage2 )
    DECLARE_IS_DISPATCHED( vkCmdCopyBufferToImage2KHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyImage )
    DECLARE_IS_DISPATCHED( vkCmdCopyImage2 )
    DECLARE_IS_DISPATCHED( vkCmdCopyImage2KHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyImageToBuffer )
    DECLARE_IS_DISPATCHED( vkCmdCopyImageToBuffer2 )
    DECLARE_IS_DISPATCHED( vkCmdCopyImageToBuffer2KHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryIndirectKHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryIndirectNV )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryToAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryToImageIndirectKHR )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryToImageIndirectNV )
    DECLARE_IS_DISPATCHED( vkCmdCopyMemoryToMicromapEXT )
    DECLARE_IS_DISPATCHED( vkCmdCopyMicromapEXT )
    DECLARE_IS_DISPATCHED( vkCmdCopyMicromapToMemoryEXT )
    DECLARE_IS_DISPATCHED( vkCmdCopyQueryPoolResults )
    DECLARE_IS_DISPATCHED( vkCmdCopyTensorARM )
    DECLARE_IS_DISPATCHED( vkCmdCuLaunchKernelNVX )
    DECLARE_IS_DISPATCHED( vkCmdCudaLaunchKernelNV )
    DECLARE_IS_DISPATCHED( vkCmdDebugMarkerBeginEXT )
    DECLARE_IS_DISPATCHED( vkCmdDebugMarkerEndEXT )
    DECLARE_IS_DISPATCHED( vkCmdDebugMarkerInsertEXT )
    DECLARE_IS_DISPATCHED( vkCmdDecodeVideoKHR )
    DECLARE_IS_DISPATCHED( vkCmdDecompressMemoryEXT )
    DECLARE_IS_DISPATCHED( vkCmdDecompressMemoryIndirectCountEXT )
    DECLARE_IS_DISPATCHED( vkCmdDecompressMemoryIndirectCountNV )
    DECLARE_IS_DISPATCHED( vkCmdDecompressMemoryNV )
    DECLARE_IS_DISPATCHED( vkCmdDispatch )
    DECLARE_IS_DISPATCHED( vkCmdDispatchBase )
    DECLARE_IS_DISPATCHED( vkCmdDispatchBaseKHR )
    DECLARE_IS_DISPATCHED( vkCmdDispatchDataGraphARM )
    DECLARE_IS_DISPATCHED( vkCmdDispatchGraphAMDX )
    DECLARE_IS_DISPATCHED( vkCmdDispatchGraphIndirectAMDX )
    DECLARE_IS_DISPATCHED( vkCmdDispatchGraphIndirectCountAMDX )
    DECLARE_IS_DISPATCHED( vkCmdDispatchIndirect )
    DECLARE_IS_DISPATCHED( vkCmdDispatchTileQCOM )
    DECLARE_IS_DISPATCHED( vkCmdDraw )
    DECLARE_IS_DISPATCHED( vkCmdDrawClusterHUAWEI )
    DECLARE_IS_DISPATCHED( vkCmdDrawClusterIndirectHUAWEI )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndexed )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndexedIndirect )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndexedIndirectCount )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndexedIndirectCountAMD )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndexedIndirectCountKHR )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndirect )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndirectByteCountEXT )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndirectCount )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndirectCountAMD )
    DECLARE_IS_DISPATCHED( vkCmdDrawIndirectCountKHR )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksEXT )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksIndirectCountEXT )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksIndirectCountNV )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksIndirectEXT )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksIndirectNV )
    DECLARE_IS_DISPATCHED( vkCmdDrawMeshTasksNV )
    DECLARE_IS_DISPATCHED( vkCmdDrawMultiEXT )
    DECLARE_IS_DISPATCHED( vkCmdDrawMultiIndexedEXT )
    DECLARE_IS_DISPATCHED( vkCmdEncodeVideoKHR )
    DECLARE_IS_DISPATCHED( vkCmdEndConditionalRenderingEXT )
    DECLARE_IS_DISPATCHED( vkCmdEndDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkCmdEndPerTileExecutionQCOM )
    DECLARE_IS_DISPATCHED( vkCmdEndQuery )
    DECLARE_IS_DISPATCHED( vkCmdEndQueryIndexedEXT )
    DECLARE_IS_DISPATCHED( vkCmdEndRenderPass )
    DECLARE_IS_DISPATCHED( vkCmdEndRenderPass2 )
    DECLARE_IS_DISPATCHED( vkCmdEndRenderPass2KHR )
    DECLARE_IS_DISPATCHED( vkCmdEndRendering )
    DECLARE_IS_DISPATCHED( vkCmdEndRenderingKHR )
    DECLARE_IS_DISPATCHED( vkCmdEndRendering2KHR )
    DECLARE_IS_DISPATCHED( vkCmdEndRendering2EXT )
    DECLARE_IS_DISPATCHED( vkCmdEndTransformFeedbackEXT )
    DECLARE_IS_DISPATCHED( vkCmdEndVideoCodingKHR )
    DECLARE_IS_DISPATCHED( vkCmdExecuteCommands )
    DECLARE_IS_DISPATCHED( vkCmdExecuteGeneratedCommandsEXT )
    DECLARE_IS_DISPATCHED( vkCmdExecuteGeneratedCommandsNV )
    DECLARE_IS_DISPATCHED( vkCmdFillBuffer )
    DECLARE_IS_DISPATCHED( vkCmdInitializeGraphScratchMemoryAMDX )
    DECLARE_IS_DISPATCHED( vkCmdInsertDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkCmdNextSubpass )
    DECLARE_IS_DISPATCHED( vkCmdNextSubpass2 )
    DECLARE_IS_DISPATCHED( vkCmdNextSubpass2KHR )
    DECLARE_IS_DISPATCHED( vkCmdOpticalFlowExecuteNV )
    DECLARE_IS_DISPATCHED( vkCmdPipelineBarrier )
    DECLARE_IS_DISPATCHED( vkCmdPipelineBarrier2 )
    DECLARE_IS_DISPATCHED( vkCmdPipelineBarrier2KHR )
    DECLARE_IS_DISPATCHED( vkCmdPreprocessGeneratedCommandsEXT )
    DECLARE_IS_DISPATCHED( vkCmdPreprocessGeneratedCommandsNV )
    DECLARE_IS_DISPATCHED( vkCmdPushConstants )
    DECLARE_IS_DISPATCHED( vkCmdPushConstants2 )
    DECLARE_IS_DISPATCHED( vkCmdPushConstants2KHR )
    DECLARE_IS_DISPATCHED( vkCmdPushDataEXT )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSet )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSetKHR )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSet2 )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSet2KHR )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSetWithTemplate )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSetWithTemplateKHR )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSetWithTemplate2 )
    DECLARE_IS_DISPATCHED( vkCmdPushDescriptorSetWithTemplate2KHR )
    DECLARE_IS_DISPATCHED( vkCmdRefreshObjectsKHR )
    DECLARE_IS_DISPATCHED( vkCmdResetEvent )
    DECLARE_IS_DISPATCHED( vkCmdResetEvent2 )
    DECLARE_IS_DISPATCHED( vkCmdResetEvent2KHR )
    DECLARE_IS_DISPATCHED( vkCmdResetQueryPool )
    DECLARE_IS_DISPATCHED( vkCmdResolveImage )
    DECLARE_IS_DISPATCHED( vkCmdResolveImage2 )
    DECLARE_IS_DISPATCHED( vkCmdResolveImage2KHR )
    DECLARE_IS_DISPATCHED( vkCmdSetAlphaToCoverageEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetAlphaToOneEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetAttachmentFeedbackLoopEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetBlendConstants )
    DECLARE_IS_DISPATCHED( vkCmdSetCheckpointNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoarseSampleOrderNV )
    DECLARE_IS_DISPATCHED( vkCmdSetColorBlendAdvancedEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetColorBlendEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetColorBlendEquationEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetColorWriteEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetColorWriteMaskEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetComputeOccupancyPriorityNV )
    DECLARE_IS_DISPATCHED( vkCmdSetConservativeRasterizationModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageModulationModeNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageModulationTableEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageModulationTableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageReductionModeNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageToColorEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCoverageToColorLocationNV )
    DECLARE_IS_DISPATCHED( vkCmdSetCullMode )
    DECLARE_IS_DISPATCHED( vkCmdSetCullModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBias )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBias2EXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBiasEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBiasEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBounds )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBoundsTestEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthBoundsTestEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthClampEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthClampRangeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthClipEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthClipNegativeOneToOneEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthCompareOp )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthCompareOpEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthTestEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthTestEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthWriteEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetDepthWriteEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDescriptorBufferOffsets2EXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDescriptorBufferOffsetsEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDeviceMask )
    DECLARE_IS_DISPATCHED( vkCmdSetDeviceMaskKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetDiscardRectangleEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDiscardRectangleEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetDiscardRectangleModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetEvent )
    DECLARE_IS_DISPATCHED( vkCmdSetEvent2 )
    DECLARE_IS_DISPATCHED( vkCmdSetEvent2KHR )
    DECLARE_IS_DISPATCHED( vkCmdSetExclusiveScissorEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetExclusiveScissorNV )
    DECLARE_IS_DISPATCHED( vkCmdSetExtraPrimitiveOverestimationSizeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetFragmentShadingRateEnumNV )
    DECLARE_IS_DISPATCHED( vkCmdSetFragmentShadingRateKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetFrontFace )
    DECLARE_IS_DISPATCHED( vkCmdSetFrontFaceEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetLineRasterizationModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetLineStipple )
    DECLARE_IS_DISPATCHED( vkCmdSetLineStippleEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetLineStippleKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetLineStippleEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetLineWidth )
    DECLARE_IS_DISPATCHED( vkCmdSetLogicOpEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetLogicOpEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetPatchControlPointsEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetPerformanceMarkerINTEL )
    DECLARE_IS_DISPATCHED( vkCmdSetPerformanceOverrideINTEL )
    DECLARE_IS_DISPATCHED( vkCmdSetPerformanceStreamMarkerINTEL )
    DECLARE_IS_DISPATCHED( vkCmdSetPolygonModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetPrimitiveRestartEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetPrimitiveRestartEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetPrimitiveTopology )
    DECLARE_IS_DISPATCHED( vkCmdSetPrimitiveTopologyEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetProvokingVertexModeEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetRasterizationSamplesEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetRasterizationStreamEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetRasterizerDiscardEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetRasterizerDiscardEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetRayTracingPipelineStackSizeKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetRenderingAttachmentLocations )
    DECLARE_IS_DISPATCHED( vkCmdSetRenderingAttachmentLocationsKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetRenderingInputAttachmentIndices )
    DECLARE_IS_DISPATCHED( vkCmdSetRenderingInputAttachmentIndicesKHR )
    DECLARE_IS_DISPATCHED( vkCmdSetRepresentativeFragmentTestEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetSampleLocationsEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetSampleLocationsEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetSampleMaskEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetScissor )
    DECLARE_IS_DISPATCHED( vkCmdSetScissorWithCount )
    DECLARE_IS_DISPATCHED( vkCmdSetScissorWithCountEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetShadingRateImageEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilCompareMask )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilOp )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilOpEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilReference )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilTestEnable )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilTestEnableEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetStencilWriteMask )
    DECLARE_IS_DISPATCHED( vkCmdSetTessellationDomainOriginEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetVertexInputEXT )
    DECLARE_IS_DISPATCHED( vkCmdSetViewport )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportShadingRatePaletteNV )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportSwizzleNV )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportWScalingEnableNV )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportWScalingNV )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportWithCount )
    DECLARE_IS_DISPATCHED( vkCmdSetViewportWithCountEXT )
    DECLARE_IS_DISPATCHED( vkCmdSubpassShadingHUAWEI )
    DECLARE_IS_DISPATCHED( vkCmdTraceRaysIndirect2KHR )
    DECLARE_IS_DISPATCHED( vkCmdTraceRaysIndirectKHR )
    DECLARE_IS_DISPATCHED( vkCmdTraceRaysKHR )
    DECLARE_IS_DISPATCHED( vkCmdTraceRaysNV )
    DECLARE_IS_DISPATCHED( vkCmdUpdateBuffer )
    DECLARE_IS_DISPATCHED( vkCmdUpdatePipelineIndirectBufferNV )
    DECLARE_IS_DISPATCHED( vkCmdWaitEvents )
    DECLARE_IS_DISPATCHED( vkCmdWaitEvents2 )
    DECLARE_IS_DISPATCHED( vkCmdWaitEvents2KHR )
    DECLARE_IS_DISPATCHED( vkCmdWriteAccelerationStructuresPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkCmdWriteAccelerationStructuresPropertiesNV )
    DECLARE_IS_DISPATCHED( vkCmdWriteBufferMarker2AMD )
    DECLARE_IS_DISPATCHED( vkCmdWriteBufferMarkerAMD )
    DECLARE_IS_DISPATCHED( vkCmdWriteMicromapsPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkCmdWriteTimestamp )
    DECLARE_IS_DISPATCHED( vkCmdWriteTimestamp2 )
    DECLARE_IS_DISPATCHED( vkCmdWriteTimestamp2KHR )
    DECLARE_IS_DISPATCHED( vkCompileDeferredNV )
    DECLARE_IS_DISPATCHED( vkConvertCooperativeVectorMatrixNV )
    DECLARE_IS_DISPATCHED( vkCopyAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkCopyAccelerationStructureToMemoryKHR )
    DECLARE_IS_DISPATCHED( vkCopyImageToImage )
    DECLARE_IS_DISPATCHED( vkCopyImageToImageEXT )
    DECLARE_IS_DISPATCHED( vkCopyImageToMemory )
    DECLARE_IS_DISPATCHED( vkCopyImageToMemoryEXT )
    DECLARE_IS_DISPATCHED( vkCopyMemoryToAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkCopyMemoryToImage )
    DECLARE_IS_DISPATCHED( vkCopyMemoryToImageEXT )
    DECLARE_IS_DISPATCHED( vkCopyMemoryToMicromapEXT )
    DECLARE_IS_DISPATCHED( vkCopyMicromapEXT )
    DECLARE_IS_DISPATCHED( vkCopyMicromapToMemoryEXT )
    DECLARE_IS_DISPATCHED( vkCreateAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkCreateAccelerationStructureNV )
    DECLARE_IS_DISPATCHED( vkCreateAndroidSurfaceKHR )
    DECLARE_IS_DISPATCHED( vkCreateBuffer )
    DECLARE_IS_DISPATCHED( vkCreateBufferCollectionFUCHSIA )
    DECLARE_IS_DISPATCHED( vkCreateBufferView )
    DECLARE_IS_DISPATCHED( vkCreateCommandPool )
    DECLARE_IS_DISPATCHED( vkCreateComputePipelines )
    DECLARE_IS_DISPATCHED( vkCreateCuFunctionNVX )
    DECLARE_IS_DISPATCHED( vkCreateCuModuleNVX )
    DECLARE_IS_DISPATCHED( vkCreateCudaFunctionNV )
    DECLARE_IS_DISPATCHED( vkCreateCudaModuleNV )
    DECLARE_IS_DISPATCHED( vkCreateDataGraphPipelineSessionARM )
    DECLARE_IS_DISPATCHED( vkCreateDataGraphPipelinesARM )
    DECLARE_IS_DISPATCHED( vkCreateDebugReportCallbackEXT )
    DECLARE_IS_DISPATCHED( vkCreateDebugUtilsMessengerEXT )
    DECLARE_IS_DISPATCHED( vkCreateDeferredOperationKHR )
    DECLARE_IS_DISPATCHED( vkCreateDescriptorPool )
    DECLARE_IS_DISPATCHED( vkCreateDescriptorSetLayout )
    DECLARE_IS_DISPATCHED( vkCreateDescriptorUpdateTemplate )
    DECLARE_IS_DISPATCHED( vkCreateDescriptorUpdateTemplateKHR )
    DECLARE_IS_DISPATCHED( vkCreateDevice )
    DECLARE_IS_DISPATCHED( vkCreateDirectFBSurfaceEXT )
    DECLARE_IS_DISPATCHED( vkCreateDisplayModeKHR )
    DECLARE_IS_DISPATCHED( vkCreateDisplayPlaneSurfaceKHR )
    DECLARE_IS_DISPATCHED( vkCreateEvent )
    DECLARE_IS_DISPATCHED( vkCreateExecutionGraphPipelinesAMDX )
    DECLARE_IS_DISPATCHED( vkCreateExternalComputeQueueNV )
    DECLARE_IS_DISPATCHED( vkCreateFence )
    DECLARE_IS_DISPATCHED( vkCreateFramebuffer )
    DECLARE_IS_DISPATCHED( vkCreateGraphicsPipelines )
    DECLARE_IS_DISPATCHED( vkCreateHeadlessSurfaceEXT )
    DECLARE_IS_DISPATCHED( vkCreateIOSSurfaceMVK )
    DECLARE_IS_DISPATCHED( vkCreateImage )
    DECLARE_IS_DISPATCHED( vkCreateImagePipeSurfaceFUCHSIA )
    DECLARE_IS_DISPATCHED( vkCreateImageView )
    DECLARE_IS_DISPATCHED( vkCreateIndirectCommandsLayoutEXT )
    DECLARE_IS_DISPATCHED( vkCreateIndirectCommandsLayoutNV )
    DECLARE_IS_DISPATCHED( vkCreateIndirectExecutionSetEXT )
    DECLARE_IS_DISPATCHED( vkCreateInstance )
    DECLARE_IS_DISPATCHED( vkCreateMacOSSurfaceMVK )
    DECLARE_IS_DISPATCHED( vkCreateMetalSurfaceEXT )
    DECLARE_IS_DISPATCHED( vkCreateMicromapEXT )
    DECLARE_IS_DISPATCHED( vkCreateOpticalFlowSessionNV )
    DECLARE_IS_DISPATCHED( vkCreatePipelineBinariesKHR )
    DECLARE_IS_DISPATCHED( vkCreatePipelineCache )
    DECLARE_IS_DISPATCHED( vkCreatePipelineLayout )
    DECLARE_IS_DISPATCHED( vkCreatePrivateDataSlot )
    DECLARE_IS_DISPATCHED( vkCreatePrivateDataSlotEXT )
    DECLARE_IS_DISPATCHED( vkCreateQueryPool )
    DECLARE_IS_DISPATCHED( vkCreateRayTracingPipelinesKHR )
    DECLARE_IS_DISPATCHED( vkCreateRayTracingPipelinesNV )
    DECLARE_IS_DISPATCHED( vkCreateRenderPass )
    DECLARE_IS_DISPATCHED( vkCreateRenderPass2 )
    DECLARE_IS_DISPATCHED( vkCreateRenderPass2KHR )
    DECLARE_IS_DISPATCHED( vkCreateSampler )
    DECLARE_IS_DISPATCHED( vkCreateSamplerYcbcrConversion )
    DECLARE_IS_DISPATCHED( vkCreateSamplerYcbcrConversionKHR )
    DECLARE_IS_DISPATCHED( vkCreateScreenSurfaceQNX )
    DECLARE_IS_DISPATCHED( vkCreateSemaphore )
    DECLARE_IS_DISPATCHED( vkCreateSemaphoreSciSyncPoolNV )
    DECLARE_IS_DISPATCHED( vkCreateShaderModule )
    DECLARE_IS_DISPATCHED( vkCreateShadersEXT )
    DECLARE_IS_DISPATCHED( vkCreateSharedSwapchainsKHR )
    DECLARE_IS_DISPATCHED( vkCreateStreamDescriptorSurfaceGGP )
    DECLARE_IS_DISPATCHED( vkCreateSurfaceOHOS )
    DECLARE_IS_DISPATCHED( vkCreateSwapchainKHR )
    DECLARE_IS_DISPATCHED( vkCreateTensorARM )
    DECLARE_IS_DISPATCHED( vkCreateTensorViewARM )
    DECLARE_IS_DISPATCHED( vkCreateUbmSurfaceSEC )
    DECLARE_IS_DISPATCHED( vkCreateValidationCacheEXT )
    DECLARE_IS_DISPATCHED( vkCreateViSurfaceNN )
    DECLARE_IS_DISPATCHED( vkCreateVideoSessionKHR )
    DECLARE_IS_DISPATCHED( vkCreateVideoSessionParametersKHR )
    DECLARE_IS_DISPATCHED( vkCreateWaylandSurfaceKHR )
    DECLARE_IS_DISPATCHED( vkCreateWin32SurfaceKHR )
    DECLARE_IS_DISPATCHED( vkCreateXcbSurfaceKHR )
    DECLARE_IS_DISPATCHED( vkCreateXlibSurfaceKHR )
    DECLARE_IS_DISPATCHED( vkDebugMarkerSetObjectNameEXT )
    DECLARE_IS_DISPATCHED( vkDebugMarkerSetObjectTagEXT )
    DECLARE_IS_DISPATCHED( vkDebugReportMessageEXT )
    DECLARE_IS_DISPATCHED( vkDeferredOperationJoinKHR )
    DECLARE_IS_DISPATCHED( vkDestroyAccelerationStructureKHR )
    DECLARE_IS_DISPATCHED( vkDestroyAccelerationStructureNV )
    DECLARE_IS_DISPATCHED( vkDestroyBuffer )
    DECLARE_IS_DISPATCHED( vkDestroyBufferCollectionFUCHSIA )
    DECLARE_IS_DISPATCHED( vkDestroyBufferView )
    DECLARE_IS_DISPATCHED( vkDestroyCommandPool )
    DECLARE_IS_DISPATCHED( vkDestroyCuFunctionNVX )
    DECLARE_IS_DISPATCHED( vkDestroyCuModuleNVX )
    DECLARE_IS_DISPATCHED( vkDestroyCudaFunctionNV )
    DECLARE_IS_DISPATCHED( vkDestroyCudaModuleNV )
    DECLARE_IS_DISPATCHED( vkDestroyDataGraphPipelineSessionARM )
    DECLARE_IS_DISPATCHED( vkDestroyDebugReportCallbackEXT )
    DECLARE_IS_DISPATCHED( vkDestroyDebugUtilsMessengerEXT )
    DECLARE_IS_DISPATCHED( vkDestroyDeferredOperationKHR )
    DECLARE_IS_DISPATCHED( vkDestroyDescriptorPool )
    DECLARE_IS_DISPATCHED( vkDestroyDescriptorSetLayout )
    DECLARE_IS_DISPATCHED( vkDestroyDescriptorUpdateTemplate )
    DECLARE_IS_DISPATCHED( vkDestroyDescriptorUpdateTemplateKHR )
    DECLARE_IS_DISPATCHED( vkDestroyDevice )
    DECLARE_IS_DISPATCHED( vkDestroyEvent )
    DECLARE_IS_DISPATCHED( vkDestroyExternalComputeQueueNV )
    DECLARE_IS_DISPATCHED( vkDestroyFence )
    DECLARE_IS_DISPATCHED( vkDestroyFramebuffer )
    DECLARE_IS_DISPATCHED( vkDestroyImage )
    DECLARE_IS_DISPATCHED( vkDestroyImageView )
    DECLARE_IS_DISPATCHED( vkDestroyIndirectCommandsLayoutEXT )
    DECLARE_IS_DISPATCHED( vkDestroyIndirectCommandsLayoutNV )
    DECLARE_IS_DISPATCHED( vkDestroyIndirectExecutionSetEXT )
    DECLARE_IS_DISPATCHED( vkDestroyInstance )
    DECLARE_IS_DISPATCHED( vkDestroyMicromapEXT )
    DECLARE_IS_DISPATCHED( vkDestroyOpticalFlowSessionNV )
    DECLARE_IS_DISPATCHED( vkDestroyPipeline )
    DECLARE_IS_DISPATCHED( vkDestroyPipelineBinaryKHR )
    DECLARE_IS_DISPATCHED( vkDestroyPipelineCache )
    DECLARE_IS_DISPATCHED( vkDestroyPipelineLayout )
    DECLARE_IS_DISPATCHED( vkDestroyPrivateDataSlot )
    DECLARE_IS_DISPATCHED( vkDestroyPrivateDataSlotEXT )
    DECLARE_IS_DISPATCHED( vkDestroyQueryPool )
    DECLARE_IS_DISPATCHED( vkDestroyRenderPass )
    DECLARE_IS_DISPATCHED( vkDestroySampler )
    DECLARE_IS_DISPATCHED( vkDestroySamplerYcbcrConversion )
    DECLARE_IS_DISPATCHED( vkDestroySamplerYcbcrConversionKHR )
    DECLARE_IS_DISPATCHED( vkDestroySemaphore )
    DECLARE_IS_DISPATCHED( vkDestroySemaphoreSciSyncPoolNV )
    DECLARE_IS_DISPATCHED( vkDestroyShaderEXT )
    DECLARE_IS_DISPATCHED( vkDestroyShaderModule )
    DECLARE_IS_DISPATCHED( vkDestroySurfaceKHR )
    DECLARE_IS_DISPATCHED( vkDestroySwapchainKHR )
    DECLARE_IS_DISPATCHED( vkDestroyTensorARM )
    DECLARE_IS_DISPATCHED( vkDestroyTensorViewARM )
    DECLARE_IS_DISPATCHED( vkDestroyValidationCacheEXT )
    DECLARE_IS_DISPATCHED( vkDestroyVideoSessionKHR )
    DECLARE_IS_DISPATCHED( vkDestroyVideoSessionParametersKHR )
    DECLARE_IS_DISPATCHED( vkDeviceWaitIdle )
    DECLARE_IS_DISPATCHED( vkDisplayPowerControlEXT )
    DECLARE_IS_DISPATCHED( vkEndCommandBuffer )
    DECLARE_IS_DISPATCHED( vkEnumerateDeviceExtensionProperties )
    DECLARE_IS_DISPATCHED( vkEnumerateDeviceLayerProperties )
    DECLARE_IS_DISPATCHED( vkEnumerateInstanceExtensionProperties )
    DECLARE_IS_DISPATCHED( vkEnumerateInstanceLayerProperties )
    DECLARE_IS_DISPATCHED( vkEnumerateInstanceVersion )
    DECLARE_IS_DISPATCHED( vkEnumeratePhysicalDeviceGroups )
    DECLARE_IS_DISPATCHED( vkEnumeratePhysicalDeviceGroupsKHR )
    DECLARE_IS_DISPATCHED( vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM )
    DECLARE_IS_DISPATCHED( vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR )
    DECLARE_IS_DISPATCHED( vkEnumeratePhysicalDevices )
    DECLARE_IS_DISPATCHED( vkExportMetalObjectsEXT )
    DECLARE_IS_DISPATCHED( vkFlushMappedMemoryRanges )
    DECLARE_IS_DISPATCHED( vkFreeCommandBuffers )
    DECLARE_IS_DISPATCHED( vkFreeDescriptorSets )
    DECLARE_IS_DISPATCHED( vkFreeMemory )
    DECLARE_IS_DISPATCHED( vkGetAccelerationStructureBuildSizesKHR )
    DECLARE_IS_DISPATCHED( vkGetAccelerationStructureDeviceAddressKHR )
    DECLARE_IS_DISPATCHED( vkGetAccelerationStructureHandleNV )
    DECLARE_IS_DISPATCHED( vkGetAccelerationStructureMemoryRequirementsNV )
    DECLARE_IS_DISPATCHED( vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT )
    DECLARE_IS_DISPATCHED( vkGetAndroidHardwareBufferPropertiesANDROID )
    DECLARE_IS_DISPATCHED( vkGetBufferCollectionPropertiesFUCHSIA )
    DECLARE_IS_DISPATCHED( vkGetBufferDeviceAddress )
    DECLARE_IS_DISPATCHED( vkGetBufferDeviceAddressEXT )
    DECLARE_IS_DISPATCHED( vkGetBufferDeviceAddressKHR )
    DECLARE_IS_DISPATCHED( vkGetBufferMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetBufferMemoryRequirements2 )
    DECLARE_IS_DISPATCHED( vkGetBufferMemoryRequirements2KHR )
    DECLARE_IS_DISPATCHED( vkGetBufferOpaqueCaptureAddress )
    DECLARE_IS_DISPATCHED( vkGetBufferOpaqueCaptureAddressKHR )
    DECLARE_IS_DISPATCHED( vkGetBufferOpaqueCaptureDescriptorDataEXT )
    DECLARE_IS_DISPATCHED( vkGetCalibratedTimestampsKHR )
    DECLARE_IS_DISPATCHED( vkGetCalibratedTimestampsEXT )
    DECLARE_IS_DISPATCHED( vkGetClusterAccelerationStructureBuildSizesNV )
    DECLARE_IS_DISPATCHED( vkGetCommandPoolMemoryConsumption )
    DECLARE_IS_DISPATCHED( vkGetCudaModuleCacheNV )
    DECLARE_IS_DISPATCHED( vkGetDataGraphPipelineAvailablePropertiesARM )
    DECLARE_IS_DISPATCHED( vkGetDataGraphPipelinePropertiesARM )
    DECLARE_IS_DISPATCHED( vkGetDataGraphPipelineSessionBindPointRequirementsARM )
    DECLARE_IS_DISPATCHED( vkGetDataGraphPipelineSessionMemoryRequirementsARM )
    DECLARE_IS_DISPATCHED( vkGetDeferredOperationMaxConcurrencyKHR )
    DECLARE_IS_DISPATCHED( vkGetDeferredOperationResultKHR )
    DECLARE_IS_DISPATCHED( vkGetDescriptorEXT )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetHostMappingVALVE )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetLayoutBindingOffsetEXT )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetLayoutHostMappingInfoVALVE )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetLayoutSizeEXT )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetLayoutSupport )
    DECLARE_IS_DISPATCHED( vkGetDescriptorSetLayoutSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceAccelerationStructureCompatibilityKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceBufferMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetDeviceBufferMemoryRequirementsKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceCombinedImageSamplerIndexNVX )
    DECLARE_IS_DISPATCHED( vkGetDeviceFaultInfoEXT )
    DECLARE_IS_DISPATCHED( vkGetDeviceGroupPeerMemoryFeatures )
    DECLARE_IS_DISPATCHED( vkGetDeviceGroupPeerMemoryFeaturesKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceGroupPresentCapabilitiesKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceGroupSurfacePresentModes2EXT )
    DECLARE_IS_DISPATCHED( vkGetDeviceGroupSurfacePresentModesKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageMemoryRequirementsKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageSparseMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageSparseMemoryRequirementsKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageSubresourceLayout )
    DECLARE_IS_DISPATCHED( vkGetDeviceImageSubresourceLayoutKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceMemoryCommitment )
    DECLARE_IS_DISPATCHED( vkGetDeviceMemoryOpaqueCaptureAddress )
    DECLARE_IS_DISPATCHED( vkGetDeviceMemoryOpaqueCaptureAddressKHR )
    DECLARE_IS_DISPATCHED( vkGetDeviceMicromapCompatibilityEXT )
    DECLARE_IS_DISPATCHED( vkGetDeviceProcAddr )
    DECLARE_IS_DISPATCHED( vkGetDeviceQueue )
    DECLARE_IS_DISPATCHED( vkGetDeviceQueue2 )
    DECLARE_IS_DISPATCHED( vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI )
    DECLARE_IS_DISPATCHED( vkGetDeviceTensorMemoryRequirementsARM )
    DECLARE_IS_DISPATCHED( vkGetDisplayModeProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetDisplayModePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetDisplayPlaneCapabilities2KHR )
    DECLARE_IS_DISPATCHED( vkGetDisplayPlaneCapabilitiesKHR )
    DECLARE_IS_DISPATCHED( vkGetDisplayPlaneSupportedDisplaysKHR )
    DECLARE_IS_DISPATCHED( vkGetDrmDisplayEXT )
    DECLARE_IS_DISPATCHED( vkGetDynamicRenderingTilePropertiesQCOM )
    DECLARE_IS_DISPATCHED( vkGetEncodedVideoSessionParametersKHR )
    DECLARE_IS_DISPATCHED( vkGetEventStatus )
    DECLARE_IS_DISPATCHED( vkGetExecutionGraphPipelineNodeIndexAMDX )
    DECLARE_IS_DISPATCHED( vkGetExecutionGraphPipelineScratchSizeAMDX )
    DECLARE_IS_DISPATCHED( vkGetExternalComputeQueueDataNV )
    DECLARE_IS_DISPATCHED( vkGetFaultData )
    DECLARE_IS_DISPATCHED( vkGetFenceFdKHR )
    DECLARE_IS_DISPATCHED( vkGetFenceSciSyncFenceNV )
    DECLARE_IS_DISPATCHED( vkGetFenceSciSyncObjNV )
    DECLARE_IS_DISPATCHED( vkGetFenceStatus )
    DECLARE_IS_DISPATCHED( vkGetFenceWin32HandleKHR )
    DECLARE_IS_DISPATCHED( vkGetFramebufferTilePropertiesQCOM )
    DECLARE_IS_DISPATCHED( vkGetGeneratedCommandsMemoryRequirementsEXT )
    DECLARE_IS_DISPATCHED( vkGetGeneratedCommandsMemoryRequirementsNV )
    DECLARE_IS_DISPATCHED( vkGetImageDrmFormatModifierPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetImageMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetImageMemoryRequirements2 )
    DECLARE_IS_DISPATCHED( vkGetImageMemoryRequirements2KHR )
    DECLARE_IS_DISPATCHED( vkGetImageOpaqueCaptureDataEXT )
    DECLARE_IS_DISPATCHED( vkGetImageOpaqueCaptureDescriptorDataEXT )
    DECLARE_IS_DISPATCHED( vkGetImageSparseMemoryRequirements )
    DECLARE_IS_DISPATCHED( vkGetImageSparseMemoryRequirements2 )
    DECLARE_IS_DISPATCHED( vkGetImageSparseMemoryRequirements2KHR )
    DECLARE_IS_DISPATCHED( vkGetImageSubresourceLayout )
    DECLARE_IS_DISPATCHED( vkGetImageSubresourceLayout2 )
    DECLARE_IS_DISPATCHED( vkGetImageSubresourceLayout2EXT )
    DECLARE_IS_DISPATCHED( vkGetImageSubresourceLayout2KHR )
    DECLARE_IS_DISPATCHED( vkGetImageViewAddressNVX )
    DECLARE_IS_DISPATCHED( vkGetImageViewHandle64NVX )
    DECLARE_IS_DISPATCHED( vkGetImageViewHandleNVX )
    DECLARE_IS_DISPATCHED( vkGetImageViewOpaqueCaptureDescriptorDataEXT )
    DECLARE_IS_DISPATCHED( vkGetInstanceProcAddr )
    DECLARE_IS_DISPATCHED( vkGetLatencyTimingsNV )
    DECLARE_IS_DISPATCHED( vkGetMemoryAndroidHardwareBufferANDROID )
    DECLARE_IS_DISPATCHED( vkGetMemoryFdKHR )
    DECLARE_IS_DISPATCHED( vkGetMemoryFdPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetMemoryHostPointerPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetMemoryMetalHandleEXT )
    DECLARE_IS_DISPATCHED( vkGetMemoryMetalHandlePropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetMemoryNativeBufferOHOS )
    DECLARE_IS_DISPATCHED( vkGetMemoryRemoteAddressNV )
    DECLARE_IS_DISPATCHED( vkGetMemorySciBufNV )
    DECLARE_IS_DISPATCHED( vkGetMemoryWin32HandleKHR )
    DECLARE_IS_DISPATCHED( vkGetMemoryWin32HandleNV )
    DECLARE_IS_DISPATCHED( vkGetMemoryWin32HandlePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetMemoryZirconHandleFUCHSIA )
    DECLARE_IS_DISPATCHED( vkGetMemoryZirconHandlePropertiesFUCHSIA )
    DECLARE_IS_DISPATCHED( vkGetMicromapBuildSizesEXT )
    DECLARE_IS_DISPATCHED( vkGetNativeBufferPropertiesOHOS )
    DECLARE_IS_DISPATCHED( vkGetPartitionedAccelerationStructuresBuildSizesNV )
    DECLARE_IS_DISPATCHED( vkGetPastPresentationTimingEXT )
    DECLARE_IS_DISPATCHED( vkGetPastPresentationTimingGOOGLE )
    DECLARE_IS_DISPATCHED( vkGetPerformanceParameterINTEL )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCalibrateableTimeDomainsKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCalibrateableTimeDomainsEXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCooperativeMatrixPropertiesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceCooperativeVectorPropertiesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDescriptorSizeEXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDirectFBPresentationSupportEXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDisplayPlaneProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDisplayPlanePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDisplayProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceDisplayPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalBufferProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalBufferPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalFenceProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalFencePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalImageFormatPropertiesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalSemaphoreProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalSemaphorePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceExternalTensorPropertiesARM )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFeatures )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFeatures2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFeatures2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFormatProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFormatProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFormatProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceFragmentShadingRatesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceImageFormatProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceImageFormatProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceImageFormatProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceMemoryProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceMemoryProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceMemoryProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceMultisamplePropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceOpticalFlowImageFormatsNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDevicePresentRectanglesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceQueueFamilyProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceRefreshableObjectTypesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSciBufAttributesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSciSyncAttributesNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceScreenPresentationSupportQNX )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSparseImageFormatProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSparseImageFormatProperties2 )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSparseImageFormatProperties2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceCapabilities2EXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceCapabilities2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceCapabilitiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceFormats2KHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceFormatsKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfacePresentModes2EXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfacePresentModesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceSurfaceSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceToolProperties )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceToolPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceUbmPresentationSupportSEC )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceVideoCapabilitiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceVideoFormatPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceWaylandPresentationSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceWin32PresentationSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceXcbPresentationSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetPhysicalDeviceXlibPresentationSupportKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelineBinaryDataKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelineCacheData )
    DECLARE_IS_DISPATCHED( vkGetPipelineExecutableInternalRepresentationsKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelineExecutablePropertiesKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelineExecutableStatisticsKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelineIndirectDeviceAddressNV )
    DECLARE_IS_DISPATCHED( vkGetPipelineIndirectMemoryRequirementsNV )
    DECLARE_IS_DISPATCHED( vkGetPipelineKeyKHR )
    DECLARE_IS_DISPATCHED( vkGetPipelinePropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetPrivateData )
    DECLARE_IS_DISPATCHED( vkGetPrivateDataEXT )
    DECLARE_IS_DISPATCHED( vkGetQueryPoolResults )
    DECLARE_IS_DISPATCHED( vkGetQueueCheckpointData2NV )
    DECLARE_IS_DISPATCHED( vkGetQueueCheckpointDataNV )
    DECLARE_IS_DISPATCHED( vkGetRandROutputDisplayEXT )
    DECLARE_IS_DISPATCHED( vkGetRayTracingCaptureReplayShaderGroupHandlesKHR )
    DECLARE_IS_DISPATCHED( vkGetRayTracingShaderGroupHandlesKHR )
    DECLARE_IS_DISPATCHED( vkGetRayTracingShaderGroupHandlesNV )
    DECLARE_IS_DISPATCHED( vkGetRayTracingShaderGroupStackSizeKHR )
    DECLARE_IS_DISPATCHED( vkGetRefreshCycleDurationGOOGLE )
    DECLARE_IS_DISPATCHED( vkGetRenderAreaGranularity )
    DECLARE_IS_DISPATCHED( vkGetRenderingAreaGranularity )
    DECLARE_IS_DISPATCHED( vkGetRenderingAreaGranularityKHR )
    DECLARE_IS_DISPATCHED( vkGetSamplerOpaqueCaptureDescriptorDataEXT )
    DECLARE_IS_DISPATCHED( vkGetScreenBufferPropertiesQNX )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreCounterValue )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreCounterValueKHR )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreFdKHR )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreSciSyncObjNV )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreWin32HandleKHR )
    DECLARE_IS_DISPATCHED( vkGetSemaphoreZirconHandleFUCHSIA )
    DECLARE_IS_DISPATCHED( vkGetShaderBinaryDataEXT )
    DECLARE_IS_DISPATCHED( vkGetShaderInfoAMD )
    DECLARE_IS_DISPATCHED( vkGetShaderModuleCreateInfoIdentifierEXT )
    DECLARE_IS_DISPATCHED( vkGetShaderModuleIdentifierEXT )
    DECLARE_IS_DISPATCHED( vkGetSwapchainCounterEXT )
    DECLARE_IS_DISPATCHED( vkGetSwapchainGrallocUsage2ANDROID )
    DECLARE_IS_DISPATCHED( vkGetSwapchainGrallocUsageANDROID )
    DECLARE_IS_DISPATCHED( vkGetSwapchainGrallocUsageOHOS )
    DECLARE_IS_DISPATCHED( vkGetSwapchainImagesKHR )
    DECLARE_IS_DISPATCHED( vkGetSwapchainStatusKHR )
    DECLARE_IS_DISPATCHED( vkGetSwapchainTimeDomainPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetSwapchainTimingPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkGetTensorMemoryRequirementsARM )
    DECLARE_IS_DISPATCHED( vkGetTensorOpaqueCaptureDataARM )
    DECLARE_IS_DISPATCHED( vkGetTensorOpaqueCaptureDescriptorDataARM )
    DECLARE_IS_DISPATCHED( vkGetTensorViewOpaqueCaptureDescriptorDataARM )
    DECLARE_IS_DISPATCHED( vkGetValidationCacheDataEXT )
    DECLARE_IS_DISPATCHED( vkGetVideoSessionMemoryRequirementsKHR )
    DECLARE_IS_DISPATCHED( vkGetWinrtDisplayNV )
    DECLARE_IS_DISPATCHED( vkImportFenceFdKHR )
    DECLARE_IS_DISPATCHED( vkImportFenceSciSyncFenceNV )
    DECLARE_IS_DISPATCHED( vkImportFenceSciSyncObjNV )
    DECLARE_IS_DISPATCHED( vkImportFenceWin32HandleKHR )
    DECLARE_IS_DISPATCHED( vkImportSemaphoreFdKHR )
    DECLARE_IS_DISPATCHED( vkImportSemaphoreSciSyncObjNV )
    DECLARE_IS_DISPATCHED( vkImportSemaphoreWin32HandleKHR )
    DECLARE_IS_DISPATCHED( vkImportSemaphoreZirconHandleFUCHSIA )
    DECLARE_IS_DISPATCHED( vkInitializePerformanceApiINTEL )
    DECLARE_IS_DISPATCHED( vkInvalidateMappedMemoryRanges )
    DECLARE_IS_DISPATCHED( vkLatencySleepNV )
    DECLARE_IS_DISPATCHED( vkMapMemory )
    DECLARE_IS_DISPATCHED( vkMapMemory2 )
    DECLARE_IS_DISPATCHED( vkMapMemory2KHR )
    DECLARE_IS_DISPATCHED( vkMergePipelineCaches )
    DECLARE_IS_DISPATCHED( vkMergeValidationCachesEXT )
    DECLARE_IS_DISPATCHED( vkQueueBeginDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkQueueBindSparse )
    DECLARE_IS_DISPATCHED( vkQueueEndDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkQueueInsertDebugUtilsLabelEXT )
    DECLARE_IS_DISPATCHED( vkQueueNotifyOutOfBandNV )
    DECLARE_IS_DISPATCHED( vkQueuePresentKHR )
    DECLARE_IS_DISPATCHED( vkQueueSetPerformanceConfigurationINTEL )
    DECLARE_IS_DISPATCHED( vkQueueSignalReleaseImageANDROID )
    DECLARE_IS_DISPATCHED( vkQueueSignalReleaseImageOHOS )
    DECLARE_IS_DISPATCHED( vkQueueSubmit )
    DECLARE_IS_DISPATCHED( vkQueueSubmit2 )
    DECLARE_IS_DISPATCHED( vkQueueSubmit2KHR )
    DECLARE_IS_DISPATCHED( vkQueueWaitIdle )
    DECLARE_IS_DISPATCHED( vkRegisterCustomBorderColorEXT )
    DECLARE_IS_DISPATCHED( vkRegisterDeviceEventEXT )
    DECLARE_IS_DISPATCHED( vkRegisterDisplayEventEXT )
    DECLARE_IS_DISPATCHED( vkReleaseCapturedPipelineDataKHR )
    DECLARE_IS_DISPATCHED( vkReleaseDisplayEXT )
    DECLARE_IS_DISPATCHED( vkReleaseFullScreenExclusiveModeEXT )
    DECLARE_IS_DISPATCHED( vkReleasePerformanceConfigurationINTEL )
    DECLARE_IS_DISPATCHED( vkReleaseProfilingLockKHR )
    DECLARE_IS_DISPATCHED( vkReleaseSwapchainImagesKHR )
    DECLARE_IS_DISPATCHED( vkReleaseSwapchainImagesEXT )
    DECLARE_IS_DISPATCHED( vkResetCommandBuffer )
    DECLARE_IS_DISPATCHED( vkResetCommandPool )
    DECLARE_IS_DISPATCHED( vkResetDescriptorPool )
    DECLARE_IS_DISPATCHED( vkResetEvent )
    DECLARE_IS_DISPATCHED( vkResetFences )
    DECLARE_IS_DISPATCHED( vkResetQueryPool )
    DECLARE_IS_DISPATCHED( vkResetQueryPoolEXT )
    DECLARE_IS_DISPATCHED( vkSetBufferCollectionBufferConstraintsFUCHSIA )
    DECLARE_IS_DISPATCHED( vkSetBufferCollectionImageConstraintsFUCHSIA )
    DECLARE_IS_DISPATCHED( vkSetDebugUtilsObjectNameEXT )
    DECLARE_IS_DISPATCHED( vkSetDebugUtilsObjectTagEXT )
    DECLARE_IS_DISPATCHED( vkSetDeviceMemoryPriorityEXT )
    DECLARE_IS_DISPATCHED( vkSetEvent )
    DECLARE_IS_DISPATCHED( vkSetHdrMetadataEXT )
    DECLARE_IS_DISPATCHED( vkSetLatencyMarkerNV )
    DECLARE_IS_DISPATCHED( vkSetLatencySleepModeNV )
    DECLARE_IS_DISPATCHED( vkSetLocalDimmingAMD )
    DECLARE_IS_DISPATCHED( vkSetPrivateData )
    DECLARE_IS_DISPATCHED( vkSetPrivateDataEXT )
    DECLARE_IS_DISPATCHED( vkSetSwapchainPresentTimingQueueSizeEXT )
    DECLARE_IS_DISPATCHED( vkSignalSemaphore )
    DECLARE_IS_DISPATCHED( vkSignalSemaphoreKHR )
    DECLARE_IS_DISPATCHED( vkSubmitDebugUtilsMessageEXT )
    DECLARE_IS_DISPATCHED( vkTransitionImageLayout )
    DECLARE_IS_DISPATCHED( vkTransitionImageLayoutEXT )
    DECLARE_IS_DISPATCHED( vkTrimCommandPool )
    DECLARE_IS_DISPATCHED( vkTrimCommandPoolKHR )
    DECLARE_IS_DISPATCHED( vkUninitializePerformanceApiINTEL )
    DECLARE_IS_DISPATCHED( vkUnmapMemory )
    DECLARE_IS_DISPATCHED( vkUnmapMemory2 )
    DECLARE_IS_DISPATCHED( vkUnmapMemory2KHR )
    DECLARE_IS_DISPATCHED( vkUnregisterCustomBorderColorEXT )
    DECLARE_IS_DISPATCHED( vkUpdateDescriptorSetWithTemplate )
    DECLARE_IS_DISPATCHED( vkUpdateDescriptorSetWithTemplateKHR )
    DECLARE_IS_DISPATCHED( vkUpdateDescriptorSets )
    DECLARE_IS_DISPATCHED( vkUpdateIndirectExecutionSetPipelineEXT )
    DECLARE_IS_DISPATCHED( vkUpdateIndirectExecutionSetShaderEXT )
    DECLARE_IS_DISPATCHED( vkUpdateVideoSessionParametersKHR )
    DECLARE_IS_DISPATCHED( vkWaitForFences )
    DECLARE_IS_DISPATCHED( vkWaitForPresent2KHR )
    DECLARE_IS_DISPATCHED( vkWaitForPresentKHR )
    DECLARE_IS_DISPATCHED( vkWaitSemaphores )
    DECLARE_IS_DISPATCHED( vkWaitSemaphoresKHR )
    DECLARE_IS_DISPATCHED( vkWriteAccelerationStructuresPropertiesKHR )
    DECLARE_IS_DISPATCHED( vkWriteMicromapsPropertiesEXT )
    DECLARE_IS_DISPATCHED( vkWriteResourceDescriptorsEXT )
    DECLARE_IS_DISPATCHED( vkWriteSamplerDescriptorsEXT )

#undef DECLARE_IS_DISPATCHED

#define IS_DISPATCHED( name ) ::VULKAN_HPP_NAMESPACE::detail::has_##name<Dispatch>::value

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

#if !defined( VK_NO_PROTOTYPES ) || ( defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ) && ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 0 ) )
    class DispatchLoaderStatic : public DispatchLoaderBase
    {
    public:
      // These commands are listed as `VULKAN_HPP_INLINE` to account for P1779R3: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1779r3.html
      // That is, member functions defined in a class definition in a module interface unit are no longer implicitly inline.

      //=== VK_VERSION_1_0 ===

      VULKAN_HPP_INLINE VkResult vkCreateInstance( VkInstanceCreateInfo const *  pCreateInfo,
                                                   VkAllocationCallbacks const * pAllocator,
                                                   VkInstance *                  pInstance ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateInstance( pCreateInfo, pAllocator, pInstance );
      }

      VULKAN_HPP_INLINE void vkDestroyInstance( VkInstance instance, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyInstance( instance, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkEnumeratePhysicalDevices( VkInstance         instance,
                                                             uint32_t *         pPhysicalDeviceCount,
                                                             VkPhysicalDevice * pPhysicalDevices ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFeatures( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures * pFeatures ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFeatures( physicalDevice, pFeatures );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFormatProperties( VkPhysicalDevice     physicalDevice,
                                                                  VkFormat             format,
                                                                  VkFormatProperties * pFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFormatProperties( physicalDevice, format, pFormatProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceImageFormatProperties( VkPhysicalDevice          physicalDevice,
                                                                           VkFormat                  format,
                                                                           VkImageType               type,
                                                                           VkImageTiling             tiling,
                                                                           VkImageUsageFlags         usage,
                                                                           VkImageCreateFlags        flags,
                                                                           VkImageFormatProperties * pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceProperties( VkPhysicalDevice             physicalDevice,
                                                            VkPhysicalDeviceProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceProperties( physicalDevice, pProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceQueueFamilyProperties( VkPhysicalDevice          physicalDevice,
                                                                       uint32_t *                pQueueFamilyPropertyCount,
                                                                       VkQueueFamilyProperties * pQueueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceMemoryProperties( VkPhysicalDevice                   physicalDevice,
                                                                  VkPhysicalDeviceMemoryProperties * pMemoryProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceMemoryProperties( physicalDevice, pMemoryProperties );
      }

      VULKAN_HPP_INLINE PFN_vkVoidFunction vkGetInstanceProcAddr( VkInstance instance, char const * pName ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetInstanceProcAddr( instance, pName );
      }

      VULKAN_HPP_INLINE PFN_vkVoidFunction vkGetDeviceProcAddr( VkDevice device, char const * pName ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceProcAddr( device, pName );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDevice( VkPhysicalDevice              physicalDevice,
                                                 VkDeviceCreateInfo const *    pCreateInfo,
                                                 VkAllocationCallbacks const * pAllocator,
                                                 VkDevice *                    pDevice ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDevice( physicalDevice, pCreateInfo, pAllocator, pDevice );
      }

      VULKAN_HPP_INLINE void vkDestroyDevice( VkDevice device, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDevice( device, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkEnumerateInstanceExtensionProperties( char const *            pLayerName,
                                                                         uint32_t *              pPropertyCount,
                                                                         VkExtensionProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkEnumerateDeviceExtensionProperties( VkPhysicalDevice        physicalDevice,
                                                                       char const *            pLayerName,
                                                                       uint32_t *              pPropertyCount,
                                                                       VkExtensionProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkEnumerateInstanceLayerProperties( uint32_t * pPropertyCount, VkLayerProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumerateInstanceLayerProperties( pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkEnumerateDeviceLayerProperties( VkPhysicalDevice    physicalDevice,
                                                                   uint32_t *          pPropertyCount,
                                                                   VkLayerProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE void vkGetDeviceQueue( VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue * pQueue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
      }

      VULKAN_HPP_INLINE VkResult vkQueueSubmit( VkQueue queue, uint32_t submitCount, VkSubmitInfo const * pSubmits, VkFence fence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueSubmit( queue, submitCount, pSubmits, fence );
      }

      VULKAN_HPP_INLINE VkResult vkQueueWaitIdle( VkQueue queue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueWaitIdle( queue );
      }

      VULKAN_HPP_INLINE VkResult vkDeviceWaitIdle( VkDevice device ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDeviceWaitIdle( device );
      }

      VULKAN_HPP_INLINE VkResult vkAllocateMemory( VkDevice                      device,
                                                   VkMemoryAllocateInfo const *  pAllocateInfo,
                                                   VkAllocationCallbacks const * pAllocator,
                                                   VkDeviceMemory *              pMemory ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAllocateMemory( device, pAllocateInfo, pAllocator, pMemory );
      }

      VULKAN_HPP_INLINE void vkFreeMemory( VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkFreeMemory( device, memory, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkMapMemory(
        VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void ** ppData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkMapMemory( device, memory, offset, size, flags, ppData );
      }

      VULKAN_HPP_INLINE void vkUnmapMemory( VkDevice device, VkDeviceMemory memory ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUnmapMemory( device, memory );
      }

      VULKAN_HPP_INLINE VkResult vkFlushMappedMemoryRanges( VkDevice                    device,
                                                            uint32_t                    memoryRangeCount,
                                                            VkMappedMemoryRange const * pMemoryRanges ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkFlushMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
      }

      VULKAN_HPP_INLINE VkResult vkInvalidateMappedMemoryRanges( VkDevice                    device,
                                                                 uint32_t                    memoryRangeCount,
                                                                 VkMappedMemoryRange const * pMemoryRanges ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkInvalidateMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
      }

      VULKAN_HPP_INLINE void
        vkGetDeviceMemoryCommitment( VkDevice device, VkDeviceMemory memory, VkDeviceSize * pCommittedMemoryInBytes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
      }

      VULKAN_HPP_INLINE VkResult vkBindBufferMemory( VkDevice       device,
                                                     VkBuffer       buffer,
                                                     VkDeviceMemory memory,
                                                     VkDeviceSize   memoryOffset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindBufferMemory( device, buffer, memory, memoryOffset );
      }

      VULKAN_HPP_INLINE VkResult vkBindImageMemory( VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindImageMemory( device, image, memory, memoryOffset );
      }

      VULKAN_HPP_INLINE void
        vkGetBufferMemoryRequirements( VkDevice device, VkBuffer buffer, VkMemoryRequirements * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferMemoryRequirements( device, buffer, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void
        vkGetImageMemoryRequirements( VkDevice device, VkImage image, VkMemoryRequirements * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageMemoryRequirements( device, image, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetImageSparseMemoryRequirements( VkDevice                          device,
                                                                 VkImage                           image,
                                                                 uint32_t *                        pSparseMemoryRequirementCount,
                                                                 VkSparseImageMemoryRequirements * pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSparseMemoryRequirements( device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceSparseImageFormatProperties( VkPhysicalDevice                physicalDevice,
                                                                             VkFormat                        format,
                                                                             VkImageType                     type,
                                                                             VkSampleCountFlagBits           samples,
                                                                             VkImageUsageFlags               usage,
                                                                             VkImageTiling                   tiling,
                                                                             uint32_t *                      pPropertyCount,
                                                                             VkSparseImageFormatProperties * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkQueueBindSparse( VkQueue                  queue,
                                                    uint32_t                 bindInfoCount,
                                                    VkBindSparseInfo const * pBindInfo,
                                                    VkFence                  fence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueBindSparse( queue, bindInfoCount, pBindInfo, fence );
      }

      VULKAN_HPP_INLINE VkResult vkCreateFence( VkDevice                      device,
                                                VkFenceCreateInfo const *     pCreateInfo,
                                                VkAllocationCallbacks const * pAllocator,
                                                VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateFence( device, pCreateInfo, pAllocator, pFence );
      }

      VULKAN_HPP_INLINE void vkDestroyFence( VkDevice device, VkFence fence, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyFence( device, fence, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkResetFences( VkDevice device, uint32_t fenceCount, VkFence const * pFences ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetFences( device, fenceCount, pFences );
      }

      VULKAN_HPP_INLINE VkResult vkGetFenceStatus( VkDevice device, VkFence fence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetFenceStatus( device, fence );
      }

      VULKAN_HPP_INLINE VkResult
        vkWaitForFences( VkDevice device, uint32_t fenceCount, VkFence const * pFences, VkBool32 waitAll, uint64_t timeout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWaitForFences( device, fenceCount, pFences, waitAll, timeout );
      }

      VULKAN_HPP_INLINE VkResult vkCreateSemaphore( VkDevice                      device,
                                                    VkSemaphoreCreateInfo const * pCreateInfo,
                                                    VkAllocationCallbacks const * pAllocator,
                                                    VkSemaphore *                 pSemaphore ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSemaphore( device, pCreateInfo, pAllocator, pSemaphore );
      }

      VULKAN_HPP_INLINE void vkDestroySemaphore( VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySemaphore( device, semaphore, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateQueryPool( VkDevice                      device,
                                                    VkQueryPoolCreateInfo const * pCreateInfo,
                                                    VkAllocationCallbacks const * pAllocator,
                                                    VkQueryPool *                 pQueryPool ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateQueryPool( device, pCreateInfo, pAllocator, pQueryPool );
      }

      VULKAN_HPP_INLINE void vkDestroyQueryPool( VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyQueryPool( device, queryPool, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetQueryPoolResults( VkDevice           device,
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

      VULKAN_HPP_INLINE VkResult vkCreateBuffer( VkDevice                      device,
                                                 VkBufferCreateInfo const *    pCreateInfo,
                                                 VkAllocationCallbacks const * pAllocator,
                                                 VkBuffer *                    pBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateBuffer( device, pCreateInfo, pAllocator, pBuffer );
      }

      VULKAN_HPP_INLINE void vkDestroyBuffer( VkDevice device, VkBuffer buffer, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyBuffer( device, buffer, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateImage( VkDevice                      device,
                                                VkImageCreateInfo const *     pCreateInfo,
                                                VkAllocationCallbacks const * pAllocator,
                                                VkImage *                     pImage ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateImage( device, pCreateInfo, pAllocator, pImage );
      }

      VULKAN_HPP_INLINE void vkDestroyImage( VkDevice device, VkImage image, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyImage( device, image, pAllocator );
      }

      VULKAN_HPP_INLINE void vkGetImageSubresourceLayout( VkDevice                   device,
                                                          VkImage                    image,
                                                          VkImageSubresource const * pSubresource,
                                                          VkSubresourceLayout *      pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSubresourceLayout( device, image, pSubresource, pLayout );
      }

      VULKAN_HPP_INLINE VkResult vkCreateImageView( VkDevice                      device,
                                                    VkImageViewCreateInfo const * pCreateInfo,
                                                    VkAllocationCallbacks const * pAllocator,
                                                    VkImageView *                 pView ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateImageView( device, pCreateInfo, pAllocator, pView );
      }

      VULKAN_HPP_INLINE void vkDestroyImageView( VkDevice device, VkImageView imageView, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyImageView( device, imageView, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateCommandPool( VkDevice                        device,
                                                      VkCommandPoolCreateInfo const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkCommandPool *                 pCommandPool ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateCommandPool( device, pCreateInfo, pAllocator, pCommandPool );
      }

      VULKAN_HPP_INLINE void
        vkDestroyCommandPool( VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyCommandPool( device, commandPool, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkResetCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetCommandPool( device, commandPool, flags );
      }

      VULKAN_HPP_INLINE VkResult vkAllocateCommandBuffers( VkDevice                            device,
                                                           VkCommandBufferAllocateInfo const * pAllocateInfo,
                                                           VkCommandBuffer *                   pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAllocateCommandBuffers( device, pAllocateInfo, pCommandBuffers );
      }

      VULKAN_HPP_INLINE void vkFreeCommandBuffers( VkDevice                device,
                                                   VkCommandPool           commandPool,
                                                   uint32_t                commandBufferCount,
                                                   VkCommandBuffer const * pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkFreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
      }

      VULKAN_HPP_INLINE VkResult vkBeginCommandBuffer( VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo const * pBeginInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBeginCommandBuffer( commandBuffer, pBeginInfo );
      }

      VULKAN_HPP_INLINE VkResult vkEndCommandBuffer( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEndCommandBuffer( commandBuffer );
      }

      VULKAN_HPP_INLINE VkResult vkResetCommandBuffer( VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetCommandBuffer( commandBuffer, flags );
      }

      VULKAN_HPP_INLINE void vkCmdCopyBuffer(
        VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy const * pRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImage( VkCommandBuffer     commandBuffer,
                                             VkImage             srcImage,
                                             VkImageLayout       srcImageLayout,
                                             VkImage             dstImage,
                                             VkImageLayout       dstImageLayout,
                                             uint32_t            regionCount,
                                             VkImageCopy const * pRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
      }

      VULKAN_HPP_INLINE void vkCmdCopyBufferToImage( VkCommandBuffer           commandBuffer,
                                                     VkBuffer                  srcBuffer,
                                                     VkImage                   dstImage,
                                                     VkImageLayout             dstImageLayout,
                                                     uint32_t                  regionCount,
                                                     VkBufferImageCopy const * pRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImageToBuffer( VkCommandBuffer           commandBuffer,
                                                     VkImage                   srcImage,
                                                     VkImageLayout             srcImageLayout,
                                                     VkBuffer                  dstBuffer,
                                                     uint32_t                  regionCount,
                                                     VkBufferImageCopy const * pRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions );
      }

      VULKAN_HPP_INLINE void vkCmdUpdateBuffer(
        VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void const * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
      }

      VULKAN_HPP_INLINE void
        vkCmdFillBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
      }

      VULKAN_HPP_INLINE void vkCmdPipelineBarrier( VkCommandBuffer               commandBuffer,
                                                   VkPipelineStageFlags          srcStageMask,
                                                   VkPipelineStageFlags          dstStageMask,
                                                   VkDependencyFlags             dependencyFlags,
                                                   uint32_t                      memoryBarrierCount,
                                                   VkMemoryBarrier const *       pMemoryBarriers,
                                                   uint32_t                      bufferMemoryBarrierCount,
                                                   VkBufferMemoryBarrier const * pBufferMemoryBarriers,
                                                   uint32_t                      imageMemoryBarrierCount,
                                                   VkImageMemoryBarrier const *  pImageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
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

      VULKAN_HPP_INLINE void
        vkCmdBeginQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginQuery( commandBuffer, queryPool, query, flags );
      }

      VULKAN_HPP_INLINE void vkCmdEndQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndQuery( commandBuffer, queryPool, query );
      }

      VULKAN_HPP_INLINE void
        vkCmdResetQueryPool( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
      }

      VULKAN_HPP_INLINE void vkCmdWriteTimestamp( VkCommandBuffer         commandBuffer,
                                                  VkPipelineStageFlagBits pipelineStage,
                                                  VkQueryPool             queryPool,
                                                  uint32_t                query ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteTimestamp( commandBuffer, pipelineStage, queryPool, query );
      }

      VULKAN_HPP_INLINE void vkCmdCopyQueryPoolResults( VkCommandBuffer    commandBuffer,
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

      VULKAN_HPP_INLINE void
        vkCmdExecuteCommands( VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer const * pCommandBuffers ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
      }

      VULKAN_HPP_INLINE VkResult vkCreateEvent( VkDevice                      device,
                                                VkEventCreateInfo const *     pCreateInfo,
                                                VkAllocationCallbacks const * pAllocator,
                                                VkEvent *                     pEvent ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateEvent( device, pCreateInfo, pAllocator, pEvent );
      }

      VULKAN_HPP_INLINE void vkDestroyEvent( VkDevice device, VkEvent event, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyEvent( device, event, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetEventStatus( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetEventStatus( device, event );
      }

      VULKAN_HPP_INLINE VkResult vkSetEvent( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetEvent( device, event );
      }

      VULKAN_HPP_INLINE VkResult vkResetEvent( VkDevice device, VkEvent event ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetEvent( device, event );
      }

      VULKAN_HPP_INLINE VkResult vkCreateBufferView( VkDevice                       device,
                                                     VkBufferViewCreateInfo const * pCreateInfo,
                                                     VkAllocationCallbacks const *  pAllocator,
                                                     VkBufferView *                 pView ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateBufferView( device, pCreateInfo, pAllocator, pView );
      }

      VULKAN_HPP_INLINE void vkDestroyBufferView( VkDevice device, VkBufferView bufferView, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyBufferView( device, bufferView, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateShaderModule( VkDevice                         device,
                                                       VkShaderModuleCreateInfo const * pCreateInfo,
                                                       VkAllocationCallbacks const *    pAllocator,
                                                       VkShaderModule *                 pShaderModule ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateShaderModule( device, pCreateInfo, pAllocator, pShaderModule );
      }

      VULKAN_HPP_INLINE void
        vkDestroyShaderModule( VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyShaderModule( device, shaderModule, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreatePipelineCache( VkDevice                          device,
                                                        VkPipelineCacheCreateInfo const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkPipelineCache *                 pPipelineCache ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreatePipelineCache( device, pCreateInfo, pAllocator, pPipelineCache );
      }

      VULKAN_HPP_INLINE void
        vkDestroyPipelineCache( VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPipelineCache( device, pipelineCache, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetPipelineCacheData( VkDevice        device,
                                                         VkPipelineCache pipelineCache,
                                                         size_t *        pDataSize,
                                                         void *          pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineCacheData( device, pipelineCache, pDataSize, pData );
      }

      VULKAN_HPP_INLINE VkResult vkMergePipelineCaches( VkDevice                device,
                                                        VkPipelineCache         dstCache,
                                                        uint32_t                srcCacheCount,
                                                        VkPipelineCache const * pSrcCaches ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkMergePipelineCaches( device, dstCache, srcCacheCount, pSrcCaches );
      }

      VULKAN_HPP_INLINE VkResult vkCreateComputePipelines( VkDevice                            device,
                                                           VkPipelineCache                     pipelineCache,
                                                           uint32_t                            createInfoCount,
                                                           VkComputePipelineCreateInfo const * pCreateInfos,
                                                           VkAllocationCallbacks const *       pAllocator,
                                                           VkPipeline *                        pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateComputePipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE void vkDestroyPipeline( VkDevice device, VkPipeline pipeline, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPipeline( device, pipeline, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreatePipelineLayout( VkDevice                           device,
                                                         VkPipelineLayoutCreateInfo const * pCreateInfo,
                                                         VkAllocationCallbacks const *      pAllocator,
                                                         VkPipelineLayout *                 pPipelineLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreatePipelineLayout( device, pCreateInfo, pAllocator, pPipelineLayout );
      }

      VULKAN_HPP_INLINE void
        vkDestroyPipelineLayout( VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPipelineLayout( device, pipelineLayout, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateSampler( VkDevice                      device,
                                                  VkSamplerCreateInfo const *   pCreateInfo,
                                                  VkAllocationCallbacks const * pAllocator,
                                                  VkSampler *                   pSampler ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSampler( device, pCreateInfo, pAllocator, pSampler );
      }

      VULKAN_HPP_INLINE void vkDestroySampler( VkDevice device, VkSampler sampler, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySampler( device, sampler, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDescriptorSetLayout( VkDevice                                device,
                                                              VkDescriptorSetLayoutCreateInfo const * pCreateInfo,
                                                              VkAllocationCallbacks const *           pAllocator,
                                                              VkDescriptorSetLayout *                 pSetLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDescriptorSetLayout( device, pCreateInfo, pAllocator, pSetLayout );
      }

      VULKAN_HPP_INLINE void vkDestroyDescriptorSetLayout( VkDevice                      device,
                                                           VkDescriptorSetLayout         descriptorSetLayout,
                                                           VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDescriptorSetLayout( device, descriptorSetLayout, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDescriptorPool( VkDevice                           device,
                                                         VkDescriptorPoolCreateInfo const * pCreateInfo,
                                                         VkAllocationCallbacks const *      pAllocator,
                                                         VkDescriptorPool *                 pDescriptorPool ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDescriptorPool( device, pCreateInfo, pAllocator, pDescriptorPool );
      }

      VULKAN_HPP_INLINE void
        vkDestroyDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDescriptorPool( device, descriptorPool, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkResetDescriptorPool( VkDevice                   device,
                                                        VkDescriptorPool           descriptorPool,
                                                        VkDescriptorPoolResetFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetDescriptorPool( device, descriptorPool, flags );
      }

      VULKAN_HPP_INLINE VkResult vkAllocateDescriptorSets( VkDevice                            device,
                                                           VkDescriptorSetAllocateInfo const * pAllocateInfo,
                                                           VkDescriptorSet *                   pDescriptorSets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAllocateDescriptorSets( device, pAllocateInfo, pDescriptorSets );
      }

      VULKAN_HPP_INLINE VkResult vkFreeDescriptorSets( VkDevice                device,
                                                       VkDescriptorPool        descriptorPool,
                                                       uint32_t                descriptorSetCount,
                                                       VkDescriptorSet const * pDescriptorSets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkFreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets );
      }

      VULKAN_HPP_INLINE void vkUpdateDescriptorSets( VkDevice                     device,
                                                     uint32_t                     descriptorWriteCount,
                                                     VkWriteDescriptorSet const * pDescriptorWrites,
                                                     uint32_t                     descriptorCopyCount,
                                                     VkCopyDescriptorSet const *  pDescriptorCopies ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateDescriptorSets( device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies );
      }

      VULKAN_HPP_INLINE void
        vkCmdBindPipeline( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindPipeline( commandBuffer, pipelineBindPoint, pipeline );
      }

      VULKAN_HPP_INLINE void vkCmdBindDescriptorSets( VkCommandBuffer         commandBuffer,
                                                      VkPipelineBindPoint     pipelineBindPoint,
                                                      VkPipelineLayout        layout,
                                                      uint32_t                firstSet,
                                                      uint32_t                descriptorSetCount,
                                                      VkDescriptorSet const * pDescriptorSets,
                                                      uint32_t                dynamicOffsetCount,
                                                      uint32_t const *        pDynamicOffsets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorSets(
          commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
      }

      VULKAN_HPP_INLINE void vkCmdClearColorImage( VkCommandBuffer                 commandBuffer,
                                                   VkImage                         image,
                                                   VkImageLayout                   imageLayout,
                                                   VkClearColorValue const *       pColor,
                                                   uint32_t                        rangeCount,
                                                   VkImageSubresourceRange const * pRanges ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdClearColorImage( commandBuffer, image, imageLayout, pColor, rangeCount, pRanges );
      }

      VULKAN_HPP_INLINE void
        vkCmdDispatch( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatch( commandBuffer, groupCountX, groupCountY, groupCountZ );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchIndirect( commandBuffer, buffer, offset );
      }

      VULKAN_HPP_INLINE void vkCmdSetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetEvent( commandBuffer, event, stageMask );
      }

      VULKAN_HPP_INLINE void vkCmdResetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResetEvent( commandBuffer, event, stageMask );
      }

      VULKAN_HPP_INLINE void vkCmdWaitEvents( VkCommandBuffer               commandBuffer,
                                              uint32_t                      eventCount,
                                              VkEvent const *               pEvents,
                                              VkPipelineStageFlags          srcStageMask,
                                              VkPipelineStageFlags          dstStageMask,
                                              uint32_t                      memoryBarrierCount,
                                              VkMemoryBarrier const *       pMemoryBarriers,
                                              uint32_t                      bufferMemoryBarrierCount,
                                              VkBufferMemoryBarrier const * pBufferMemoryBarriers,
                                              uint32_t                      imageMemoryBarrierCount,
                                              VkImageMemoryBarrier const *  pImageMemoryBarriers ) const VULKAN_HPP_NOEXCEPT
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

      VULKAN_HPP_INLINE void vkCmdPushConstants( VkCommandBuffer    commandBuffer,
                                                 VkPipelineLayout   layout,
                                                 VkShaderStageFlags stageFlags,
                                                 uint32_t           offset,
                                                 uint32_t           size,
                                                 void const *       pValues ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushConstants( commandBuffer, layout, stageFlags, offset, size, pValues );
      }

      VULKAN_HPP_INLINE VkResult vkCreateGraphicsPipelines( VkDevice                             device,
                                                            VkPipelineCache                      pipelineCache,
                                                            uint32_t                             createInfoCount,
                                                            VkGraphicsPipelineCreateInfo const * pCreateInfos,
                                                            VkAllocationCallbacks const *        pAllocator,
                                                            VkPipeline *                         pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateGraphicsPipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE VkResult vkCreateFramebuffer( VkDevice                        device,
                                                      VkFramebufferCreateInfo const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkFramebuffer *                 pFramebuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateFramebuffer( device, pCreateInfo, pAllocator, pFramebuffer );
      }

      VULKAN_HPP_INLINE void
        vkDestroyFramebuffer( VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyFramebuffer( device, framebuffer, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateRenderPass( VkDevice                       device,
                                                     VkRenderPassCreateInfo const * pCreateInfo,
                                                     VkAllocationCallbacks const *  pAllocator,
                                                     VkRenderPass *                 pRenderPass ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateRenderPass( device, pCreateInfo, pAllocator, pRenderPass );
      }

      VULKAN_HPP_INLINE void vkDestroyRenderPass( VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyRenderPass( device, renderPass, pAllocator );
      }

      VULKAN_HPP_INLINE void vkGetRenderAreaGranularity( VkDevice device, VkRenderPass renderPass, VkExtent2D * pGranularity ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRenderAreaGranularity( device, renderPass, pGranularity );
      }

      VULKAN_HPP_INLINE void vkCmdSetViewport( VkCommandBuffer    commandBuffer,
                                               uint32_t           firstViewport,
                                               uint32_t           viewportCount,
                                               VkViewport const * pViewports ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewport( commandBuffer, firstViewport, viewportCount, pViewports );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetScissor( VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D const * pScissors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetScissor( commandBuffer, firstScissor, scissorCount, pScissors );
      }

      VULKAN_HPP_INLINE void vkCmdSetLineWidth( VkCommandBuffer commandBuffer, float lineWidth ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineWidth( commandBuffer, lineWidth );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBias( VkCommandBuffer commandBuffer,
                                                float           depthBiasConstantFactor,
                                                float           depthBiasClamp,
                                                float           depthBiasSlopeFactor ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
      }

      VULKAN_HPP_INLINE void vkCmdSetBlendConstants( VkCommandBuffer commandBuffer, float const blendConstants[4] ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetBlendConstants( commandBuffer, blendConstants );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBounds( VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetStencilCompareMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilCompareMask( commandBuffer, faceMask, compareMask );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetStencilWriteMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilWriteMask( commandBuffer, faceMask, writeMask );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetStencilReference( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilReference( commandBuffer, faceMask, reference );
      }

      VULKAN_HPP_INLINE void
        vkCmdBindIndexBuffer( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindIndexBuffer( commandBuffer, buffer, offset, indexType );
      }

      VULKAN_HPP_INLINE void vkCmdBindVertexBuffers( VkCommandBuffer      commandBuffer,
                                                     uint32_t             firstBinding,
                                                     uint32_t             bindingCount,
                                                     VkBuffer const *     pBuffers,
                                                     VkDeviceSize const * pOffsets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
      }

      VULKAN_HPP_INLINE void vkCmdDraw(
        VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndexed( VkCommandBuffer commandBuffer,
                                               uint32_t        indexCount,
                                               uint32_t        instanceCount,
                                               uint32_t        firstIndex,
                                               int32_t         vertexOffset,
                                               uint32_t        firstInstance ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
      }

      VULKAN_HPP_INLINE void
        vkCmdDrawIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndexedIndirect(
        VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdBlitImage( VkCommandBuffer     commandBuffer,
                                             VkImage             srcImage,
                                             VkImageLayout       srcImageLayout,
                                             VkImage             dstImage,
                                             VkImageLayout       dstImageLayout,
                                             uint32_t            regionCount,
                                             VkImageBlit const * pRegions,
                                             VkFilter            filter ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter );
      }

      VULKAN_HPP_INLINE void vkCmdClearDepthStencilImage( VkCommandBuffer                  commandBuffer,
                                                          VkImage                          image,
                                                          VkImageLayout                    imageLayout,
                                                          VkClearDepthStencilValue const * pDepthStencil,
                                                          uint32_t                         rangeCount,
                                                          VkImageSubresourceRange const *  pRanges ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdClearDepthStencilImage( commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges );
      }

      VULKAN_HPP_INLINE void vkCmdClearAttachments( VkCommandBuffer           commandBuffer,
                                                    uint32_t                  attachmentCount,
                                                    VkClearAttachment const * pAttachments,
                                                    uint32_t                  rectCount,
                                                    VkClearRect const *       pRects ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdClearAttachments( commandBuffer, attachmentCount, pAttachments, rectCount, pRects );
      }

      VULKAN_HPP_INLINE void vkCmdResolveImage( VkCommandBuffer        commandBuffer,
                                                VkImage                srcImage,
                                                VkImageLayout          srcImageLayout,
                                                VkImage                dstImage,
                                                VkImageLayout          dstImageLayout,
                                                uint32_t               regionCount,
                                                VkImageResolve const * pRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
      }

      VULKAN_HPP_INLINE void vkCmdBeginRenderPass( VkCommandBuffer               commandBuffer,
                                                   VkRenderPassBeginInfo const * pRenderPassBegin,
                                                   VkSubpassContents             contents ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginRenderPass( commandBuffer, pRenderPassBegin, contents );
      }

      VULKAN_HPP_INLINE void vkCmdNextSubpass( VkCommandBuffer commandBuffer, VkSubpassContents contents ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdNextSubpass( commandBuffer, contents );
      }

      VULKAN_HPP_INLINE void vkCmdEndRenderPass( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRenderPass( commandBuffer );
      }

      //=== VK_VERSION_1_1 ===

      VULKAN_HPP_INLINE VkResult vkEnumerateInstanceVersion( uint32_t * pApiVersion ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumerateInstanceVersion( pApiVersion );
      }

      VULKAN_HPP_INLINE VkResult vkBindBufferMemory2( VkDevice                       device,
                                                      uint32_t                       bindInfoCount,
                                                      VkBindBufferMemoryInfo const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindBufferMemory2( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE VkResult vkBindImageMemory2( VkDevice                      device,
                                                     uint32_t                      bindInfoCount,
                                                     VkBindImageMemoryInfo const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindImageMemory2( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE void vkGetDeviceGroupPeerMemoryFeatures( VkDevice                   device,
                                                                 uint32_t                   heapIndex,
                                                                 uint32_t                   localDeviceIndex,
                                                                 uint32_t                   remoteDeviceIndex,
                                                                 VkPeerMemoryFeatureFlags * pPeerMemoryFeatures ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceGroupPeerMemoryFeatures( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
      }

      VULKAN_HPP_INLINE void vkCmdSetDeviceMask( VkCommandBuffer commandBuffer, uint32_t deviceMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDeviceMask( commandBuffer, deviceMask );
      }

      VULKAN_HPP_INLINE VkResult vkEnumeratePhysicalDeviceGroups( VkInstance                        instance,
                                                                  uint32_t *                        pPhysicalDeviceGroupCount,
                                                                  VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumeratePhysicalDeviceGroups( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
      }

      VULKAN_HPP_INLINE void vkGetImageMemoryRequirements2( VkDevice                               device,
                                                            VkImageMemoryRequirementsInfo2 const * pInfo,
                                                            VkMemoryRequirements2 *                pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageMemoryRequirements2( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetBufferMemoryRequirements2( VkDevice                                device,
                                                             VkBufferMemoryRequirementsInfo2 const * pInfo,
                                                             VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferMemoryRequirements2( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetImageSparseMemoryRequirements2( VkDevice                                     device,
                                                                  VkImageSparseMemoryRequirementsInfo2 const * pInfo,
                                                                  uint32_t *                                   pSparseMemoryRequirementCount,
                                                                  VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSparseMemoryRequirements2( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFeatures2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2 * pFeatures ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFeatures2( physicalDevice, pFeatures );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceProperties2( VkPhysicalDevice              physicalDevice,
                                                             VkPhysicalDeviceProperties2 * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceProperties2( physicalDevice, pProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFormatProperties2( VkPhysicalDevice      physicalDevice,
                                                                   VkFormat              format,
                                                                   VkFormatProperties2 * pFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFormatProperties2( physicalDevice, format, pFormatProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceImageFormatProperties2( VkPhysicalDevice                         physicalDevice,
                                                                            VkPhysicalDeviceImageFormatInfo2 const * pImageFormatInfo,
                                                                            VkImageFormatProperties2 * pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceImageFormatProperties2( physicalDevice, pImageFormatInfo, pImageFormatProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceQueueFamilyProperties2( VkPhysicalDevice           physicalDevice,
                                                                        uint32_t *                 pQueueFamilyPropertyCount,
                                                                        VkQueueFamilyProperties2 * pQueueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyProperties2( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceMemoryProperties2( VkPhysicalDevice                    physicalDevice,
                                                                   VkPhysicalDeviceMemoryProperties2 * pMemoryProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceMemoryProperties2( physicalDevice, pMemoryProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceSparseImageFormatProperties2( VkPhysicalDevice                               physicalDevice,
                                                                              VkPhysicalDeviceSparseImageFormatInfo2 const * pFormatInfo,
                                                                              uint32_t *                                     pPropertyCount,
                                                                              VkSparseImageFormatProperties2 * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSparseImageFormatProperties2( physicalDevice, pFormatInfo, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE void vkTrimCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkTrimCommandPool( device, commandPool, flags );
      }

      VULKAN_HPP_INLINE void vkGetDeviceQueue2( VkDevice device, VkDeviceQueueInfo2 const * pQueueInfo, VkQueue * pQueue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceQueue2( device, pQueueInfo, pQueue );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceExternalBufferProperties( VkPhysicalDevice                           physicalDevice,
                                                                          VkPhysicalDeviceExternalBufferInfo const * pExternalBufferInfo,
                                                                          VkExternalBufferProperties * pExternalBufferProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalBufferProperties( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceExternalFenceProperties( VkPhysicalDevice                          physicalDevice,
                                                                         VkPhysicalDeviceExternalFenceInfo const * pExternalFenceInfo,
                                                                         VkExternalFenceProperties * pExternalFenceProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalFenceProperties( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
      }

      VULKAN_HPP_INLINE void
        vkGetPhysicalDeviceExternalSemaphoreProperties( VkPhysicalDevice                              physicalDevice,
                                                        VkPhysicalDeviceExternalSemaphoreInfo const * pExternalSemaphoreInfo,
                                                        VkExternalSemaphoreProperties *               pExternalSemaphoreProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalSemaphoreProperties( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchBase( VkCommandBuffer commandBuffer,
                                                uint32_t        baseGroupX,
                                                uint32_t        baseGroupY,
                                                uint32_t        baseGroupZ,
                                                uint32_t        groupCountX,
                                                uint32_t        groupCountY,
                                                uint32_t        groupCountZ ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchBase( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDescriptorUpdateTemplate( VkDevice                                     device,
                                                                   VkDescriptorUpdateTemplateCreateInfo const * pCreateInfo,
                                                                   VkAllocationCallbacks const *                pAllocator,
                                                                   VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDescriptorUpdateTemplate( device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate );
      }

      VULKAN_HPP_INLINE void vkDestroyDescriptorUpdateTemplate( VkDevice                      device,
                                                                VkDescriptorUpdateTemplate    descriptorUpdateTemplate,
                                                                VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDescriptorUpdateTemplate( device, descriptorUpdateTemplate, pAllocator );
      }

      VULKAN_HPP_INLINE void vkUpdateDescriptorSetWithTemplate( VkDevice                   device,
                                                                VkDescriptorSet            descriptorSet,
                                                                VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                                                void const *               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateDescriptorSetWithTemplate( device, descriptorSet, descriptorUpdateTemplate, pData );
      }

      VULKAN_HPP_INLINE void vkGetDescriptorSetLayoutSupport( VkDevice                                device,
                                                              VkDescriptorSetLayoutCreateInfo const * pCreateInfo,
                                                              VkDescriptorSetLayoutSupport *          pSupport ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetLayoutSupport( device, pCreateInfo, pSupport );
      }

      VULKAN_HPP_INLINE VkResult vkCreateSamplerYcbcrConversion( VkDevice                                   device,
                                                                 VkSamplerYcbcrConversionCreateInfo const * pCreateInfo,
                                                                 VkAllocationCallbacks const *              pAllocator,
                                                                 VkSamplerYcbcrConversion *                 pYcbcrConversion ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSamplerYcbcrConversion( device, pCreateInfo, pAllocator, pYcbcrConversion );
      }

      VULKAN_HPP_INLINE void vkDestroySamplerYcbcrConversion( VkDevice                      device,
                                                              VkSamplerYcbcrConversion      ycbcrConversion,
                                                              VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySamplerYcbcrConversion( device, ycbcrConversion, pAllocator );
      }

      //=== VK_VERSION_1_2 ===

      VULKAN_HPP_INLINE void vkResetQueryPool( VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetQueryPool( device, queryPool, firstQuery, queryCount );
      }

      VULKAN_HPP_INLINE VkResult vkGetSemaphoreCounterValue( VkDevice device, VkSemaphore semaphore, uint64_t * pValue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSemaphoreCounterValue( device, semaphore, pValue );
      }

      VULKAN_HPP_INLINE VkResult vkWaitSemaphores( VkDevice device, VkSemaphoreWaitInfo const * pWaitInfo, uint64_t timeout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWaitSemaphores( device, pWaitInfo, timeout );
      }

      VULKAN_HPP_INLINE VkResult vkSignalSemaphore( VkDevice device, VkSemaphoreSignalInfo const * pSignalInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSignalSemaphore( device, pSignalInfo );
      }

      VULKAN_HPP_INLINE VkDeviceAddress vkGetBufferDeviceAddress( VkDevice device, VkBufferDeviceAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferDeviceAddress( device, pInfo );
      }

      VULKAN_HPP_INLINE uint64_t vkGetBufferOpaqueCaptureAddress( VkDevice device, VkBufferDeviceAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferOpaqueCaptureAddress( device, pInfo );
      }

      VULKAN_HPP_INLINE uint64_t vkGetDeviceMemoryOpaqueCaptureAddress( VkDevice                                       device,
                                                                        VkDeviceMemoryOpaqueCaptureAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceMemoryOpaqueCaptureAddress( device, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndirectCount( VkCommandBuffer commandBuffer,
                                                     VkBuffer        buffer,
                                                     VkDeviceSize    offset,
                                                     VkBuffer        countBuffer,
                                                     VkDeviceSize    countBufferOffset,
                                                     uint32_t        maxDrawCount,
                                                     uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndirectCount( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndexedIndirectCount( VkCommandBuffer commandBuffer,
                                                            VkBuffer        buffer,
                                                            VkDeviceSize    offset,
                                                            VkBuffer        countBuffer,
                                                            VkDeviceSize    countBufferOffset,
                                                            uint32_t        maxDrawCount,
                                                            uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndexedIndirectCount( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      VULKAN_HPP_INLINE VkResult vkCreateRenderPass2( VkDevice                        device,
                                                      VkRenderPassCreateInfo2 const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkRenderPass *                  pRenderPass ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateRenderPass2( device, pCreateInfo, pAllocator, pRenderPass );
      }

      VULKAN_HPP_INLINE void vkCmdBeginRenderPass2( VkCommandBuffer               commandBuffer,
                                                    VkRenderPassBeginInfo const * pRenderPassBegin,
                                                    VkSubpassBeginInfo const *    pSubpassBeginInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginRenderPass2( commandBuffer, pRenderPassBegin, pSubpassBeginInfo );
      }

      VULKAN_HPP_INLINE void vkCmdNextSubpass2( VkCommandBuffer            commandBuffer,
                                                VkSubpassBeginInfo const * pSubpassBeginInfo,
                                                VkSubpassEndInfo const *   pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdNextSubpass2( commandBuffer, pSubpassBeginInfo, pSubpassEndInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndRenderPass2( VkCommandBuffer commandBuffer, VkSubpassEndInfo const * pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRenderPass2( commandBuffer, pSubpassEndInfo );
      }

      //=== VK_VERSION_1_3 ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceToolProperties( VkPhysicalDevice                 physicalDevice,
                                                                    uint32_t *                       pToolCount,
                                                                    VkPhysicalDeviceToolProperties * pToolProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceToolProperties( physicalDevice, pToolCount, pToolProperties );
      }

      VULKAN_HPP_INLINE VkResult vkCreatePrivateDataSlot( VkDevice                            device,
                                                          VkPrivateDataSlotCreateInfo const * pCreateInfo,
                                                          VkAllocationCallbacks const *       pAllocator,
                                                          VkPrivateDataSlot *                 pPrivateDataSlot ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreatePrivateDataSlot( device, pCreateInfo, pAllocator, pPrivateDataSlot );
      }

      VULKAN_HPP_INLINE void
        vkDestroyPrivateDataSlot( VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPrivateDataSlot( device, privateDataSlot, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkSetPrivateData(
        VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetPrivateData( device, objectType, objectHandle, privateDataSlot, data );
      }

      VULKAN_HPP_INLINE void vkGetPrivateData(
        VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPrivateData( device, objectType, objectHandle, privateDataSlot, pData );
      }

      VULKAN_HPP_INLINE void vkCmdPipelineBarrier2( VkCommandBuffer commandBuffer, VkDependencyInfo const * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPipelineBarrier2( commandBuffer, pDependencyInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdWriteTimestamp2( VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteTimestamp2( commandBuffer, stage, queryPool, query );
      }

      VULKAN_HPP_INLINE VkResult vkQueueSubmit2( VkQueue queue, uint32_t submitCount, VkSubmitInfo2 const * pSubmits, VkFence fence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueSubmit2( queue, submitCount, pSubmits, fence );
      }

      VULKAN_HPP_INLINE void vkCmdCopyBuffer2( VkCommandBuffer commandBuffer, VkCopyBufferInfo2 const * pCopyBufferInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBuffer2( commandBuffer, pCopyBufferInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImage2( VkCommandBuffer commandBuffer, VkCopyImageInfo2 const * pCopyImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImage2( commandBuffer, pCopyImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyBufferToImage2( VkCommandBuffer                  commandBuffer,
                                                      VkCopyBufferToImageInfo2 const * pCopyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBufferToImage2( commandBuffer, pCopyBufferToImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImageToBuffer2( VkCommandBuffer                  commandBuffer,
                                                      VkCopyImageToBufferInfo2 const * pCopyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImageToBuffer2( commandBuffer, pCopyImageToBufferInfo );
      }

      VULKAN_HPP_INLINE void vkGetDeviceBufferMemoryRequirements( VkDevice                                 device,
                                                                  VkDeviceBufferMemoryRequirements const * pInfo,
                                                                  VkMemoryRequirements2 *                  pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceBufferMemoryRequirements( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetDeviceImageMemoryRequirements( VkDevice                                device,
                                                                 VkDeviceImageMemoryRequirements const * pInfo,
                                                                 VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageMemoryRequirements( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetDeviceImageSparseMemoryRequirements( VkDevice                                device,
                                                                       VkDeviceImageMemoryRequirements const * pInfo,
                                                                       uint32_t *                              pSparseMemoryRequirementCount,
                                                                       VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageSparseMemoryRequirements( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkCmdSetEvent2( VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo const * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetEvent2( commandBuffer, event, pDependencyInfo );
      }

      VULKAN_HPP_INLINE void vkCmdResetEvent2( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResetEvent2( commandBuffer, event, stageMask );
      }

      VULKAN_HPP_INLINE void vkCmdWaitEvents2( VkCommandBuffer          commandBuffer,
                                               uint32_t                 eventCount,
                                               VkEvent const *          pEvents,
                                               VkDependencyInfo const * pDependencyInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWaitEvents2( commandBuffer, eventCount, pEvents, pDependencyInfos );
      }

      VULKAN_HPP_INLINE void vkCmdBlitImage2( VkCommandBuffer commandBuffer, VkBlitImageInfo2 const * pBlitImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBlitImage2( commandBuffer, pBlitImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdResolveImage2( VkCommandBuffer commandBuffer, VkResolveImageInfo2 const * pResolveImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResolveImage2( commandBuffer, pResolveImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBeginRendering( VkCommandBuffer commandBuffer, VkRenderingInfo const * pRenderingInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginRendering( commandBuffer, pRenderingInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndRendering( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRendering( commandBuffer );
      }

      VULKAN_HPP_INLINE void vkCmdSetCullMode( VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCullMode( commandBuffer, cullMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetFrontFace( VkCommandBuffer commandBuffer, VkFrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetFrontFace( commandBuffer, frontFace );
      }

      VULKAN_HPP_INLINE void vkCmdSetPrimitiveTopology( VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPrimitiveTopology( commandBuffer, primitiveTopology );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetViewportWithCount( VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport const * pViewports ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportWithCount( commandBuffer, viewportCount, pViewports );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetScissorWithCount( VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D const * pScissors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetScissorWithCount( commandBuffer, scissorCount, pScissors );
      }

      VULKAN_HPP_INLINE void vkCmdBindVertexBuffers2( VkCommandBuffer      commandBuffer,
                                                      uint32_t             firstBinding,
                                                      uint32_t             bindingCount,
                                                      VkBuffer const *     pBuffers,
                                                      VkDeviceSize const * pOffsets,
                                                      VkDeviceSize const * pSizes,
                                                      VkDeviceSize const * pStrides ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindVertexBuffers2( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthTestEnable( VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthTestEnable( commandBuffer, depthTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthWriteEnable( VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthWriteEnable( commandBuffer, depthWriteEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthCompareOp( VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthCompareOp( commandBuffer, depthCompareOp );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBoundsTestEnable( VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBoundsTestEnable( commandBuffer, depthBoundsTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetStencilTestEnable( VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilTestEnable( commandBuffer, stencilTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetStencilOp( VkCommandBuffer    commandBuffer,
                                                VkStencilFaceFlags faceMask,
                                                VkStencilOp        failOp,
                                                VkStencilOp        passOp,
                                                VkStencilOp        depthFailOp,
                                                VkCompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilOp( commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp );
      }

      VULKAN_HPP_INLINE void vkCmdSetRasterizerDiscardEnable( VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRasterizerDiscardEnable( commandBuffer, rasterizerDiscardEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBiasEnable( VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBiasEnable( commandBuffer, depthBiasEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetPrimitiveRestartEnable( VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPrimitiveRestartEnable( commandBuffer, primitiveRestartEnable );
      }

      //=== VK_VERSION_1_4 ===

      VULKAN_HPP_INLINE VkResult vkMapMemory2( VkDevice device, VkMemoryMapInfo const * pMemoryMapInfo, void ** ppData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkMapMemory2( device, pMemoryMapInfo, ppData );
      }

      VULKAN_HPP_INLINE VkResult vkUnmapMemory2( VkDevice device, VkMemoryUnmapInfo const * pMemoryUnmapInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUnmapMemory2( device, pMemoryUnmapInfo );
      }

      VULKAN_HPP_INLINE void vkGetDeviceImageSubresourceLayout( VkDevice                             device,
                                                                VkDeviceImageSubresourceInfo const * pInfo,
                                                                VkSubresourceLayout2 *               pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageSubresourceLayout( device, pInfo, pLayout );
      }

      VULKAN_HPP_INLINE void vkGetImageSubresourceLayout2( VkDevice                    device,
                                                           VkImage                     image,
                                                           VkImageSubresource2 const * pSubresource,
                                                           VkSubresourceLayout2 *      pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSubresourceLayout2( device, image, pSubresource, pLayout );
      }

      VULKAN_HPP_INLINE VkResult vkCopyMemoryToImage( VkDevice device, VkCopyMemoryToImageInfo const * pCopyMemoryToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMemoryToImage( device, pCopyMemoryToImageInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyImageToMemory( VkDevice device, VkCopyImageToMemoryInfo const * pCopyImageToMemoryInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyImageToMemory( device, pCopyImageToMemoryInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyImageToImage( VkDevice device, VkCopyImageToImageInfo const * pCopyImageToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyImageToImage( device, pCopyImageToImageInfo );
      }

      VULKAN_HPP_INLINE VkResult vkTransitionImageLayout( VkDevice                                device,
                                                          uint32_t                                transitionCount,
                                                          VkHostImageLayoutTransitionInfo const * pTransitions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkTransitionImageLayout( device, transitionCount, pTransitions );
      }

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSet( VkCommandBuffer              commandBuffer,
                                                     VkPipelineBindPoint          pipelineBindPoint,
                                                     VkPipelineLayout             layout,
                                                     uint32_t                     set,
                                                     uint32_t                     descriptorWriteCount,
                                                     VkWriteDescriptorSet const * pDescriptorWrites ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSet( commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites );
      }

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSetWithTemplate( VkCommandBuffer            commandBuffer,
                                                                 VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                                                 VkPipelineLayout           layout,
                                                                 uint32_t                   set,
                                                                 void const *               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSetWithTemplate( commandBuffer, descriptorUpdateTemplate, layout, set, pData );
      }

      VULKAN_HPP_INLINE void vkCmdBindDescriptorSets2( VkCommandBuffer                  commandBuffer,
                                                       VkBindDescriptorSetsInfo const * pBindDescriptorSetsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorSets2( commandBuffer, pBindDescriptorSetsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdPushConstants2( VkCommandBuffer commandBuffer, VkPushConstantsInfo const * pPushConstantsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushConstants2( commandBuffer, pPushConstantsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSet2( VkCommandBuffer                 commandBuffer,
                                                      VkPushDescriptorSetInfo const * pPushDescriptorSetInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSet2( commandBuffer, pPushDescriptorSetInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdPushDescriptorSetWithTemplate2( VkCommandBuffer                             commandBuffer,
                                             VkPushDescriptorSetWithTemplateInfo const * pPushDescriptorSetWithTemplateInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSetWithTemplate2( commandBuffer, pPushDescriptorSetWithTemplateInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetLineStipple( VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineStipple( commandBuffer, lineStippleFactor, lineStipplePattern );
      }

      VULKAN_HPP_INLINE void vkCmdBindIndexBuffer2(
        VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindIndexBuffer2( commandBuffer, buffer, offset, size, indexType );
      }

      VULKAN_HPP_INLINE void
        vkGetRenderingAreaGranularity( VkDevice device, VkRenderingAreaInfo const * pRenderingAreaInfo, VkExtent2D * pGranularity ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRenderingAreaGranularity( device, pRenderingAreaInfo, pGranularity );
      }

      VULKAN_HPP_INLINE void vkCmdSetRenderingAttachmentLocations( VkCommandBuffer                           commandBuffer,
                                                                   VkRenderingAttachmentLocationInfo const * pLocationInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRenderingAttachmentLocations( commandBuffer, pLocationInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetRenderingInputAttachmentIndices( VkCommandBuffer                             commandBuffer,
                                                 VkRenderingInputAttachmentIndexInfo const * pInputAttachmentIndexInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRenderingInputAttachmentIndices( commandBuffer, pInputAttachmentIndexInfo );
      }
#  if !defined( VK_ONLY_EXPORTED_PROTOTYPES )

      //=== VK_KHR_surface ===

      VULKAN_HPP_INLINE void
        vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySurfaceKHR( instance, surface, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice,
                                                                       uint32_t         queueFamilyIndex,
                                                                       VkSurfaceKHR     surface,
                                                                       VkBool32 *       pSupported ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice           physicalDevice,
                                                                            VkSurfaceKHR               surface,
                                                                            VkSurfaceCapabilitiesKHR * pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice     physicalDevice,
                                                                       VkSurfaceKHR         surface,
                                                                       uint32_t *           pSurfaceFormatCount,
                                                                       VkSurfaceFormatKHR * pSurfaceFormats ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice   physicalDevice,
                                                                            VkSurfaceKHR       surface,
                                                                            uint32_t *         pPresentModeCount,
                                                                            VkPresentModeKHR * pPresentModes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
      }

      //=== VK_KHR_swapchain ===

      VULKAN_HPP_INLINE VkResult vkCreateSwapchainKHR( VkDevice                         device,
                                                       VkSwapchainCreateInfoKHR const * pCreateInfo,
                                                       VkAllocationCallbacks const *    pAllocator,
                                                       VkSwapchainKHR *                 pSwapchain ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
      }

      VULKAN_HPP_INLINE void
        vkDestroySwapchainKHR( VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySwapchainKHR( device, swapchain, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetSwapchainImagesKHR( VkDevice       device,
                                                          VkSwapchainKHR swapchain,
                                                          uint32_t *     pSwapchainImageCount,
                                                          VkImage *      pSwapchainImages ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages );
      }

      VULKAN_HPP_INLINE VkResult vkAcquireNextImageKHR(
        VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t * pImageIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex );
      }

      VULKAN_HPP_INLINE VkResult vkQueuePresentKHR( VkQueue queue, VkPresentInfoKHR const * pPresentInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueuePresentKHR( queue, pPresentInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetDeviceGroupPresentCapabilitiesKHR(
        VkDevice device, VkDeviceGroupPresentCapabilitiesKHR * pDeviceGroupPresentCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceGroupPresentCapabilitiesKHR( device, pDeviceGroupPresentCapabilities );
      }

      VULKAN_HPP_INLINE VkResult vkGetDeviceGroupSurfacePresentModesKHR( VkDevice                           device,
                                                                         VkSurfaceKHR                       surface,
                                                                         VkDeviceGroupPresentModeFlagsKHR * pModes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceGroupSurfacePresentModesKHR( device, surface, pModes );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDevicePresentRectanglesKHR( VkPhysicalDevice physicalDevice,
                                                                          VkSurfaceKHR     surface,
                                                                          uint32_t *       pRectCount,
                                                                          VkRect2D *       pRects ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDevicePresentRectanglesKHR( physicalDevice, surface, pRectCount, pRects );
      }

      VULKAN_HPP_INLINE VkResult vkAcquireNextImage2KHR( VkDevice                          device,
                                                         VkAcquireNextImageInfoKHR const * pAcquireInfo,
                                                         uint32_t *                        pImageIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireNextImage2KHR( device, pAcquireInfo, pImageIndex );
      }

      //=== VK_KHR_display ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceDisplayPropertiesKHR( VkPhysicalDevice         physicalDevice,
                                                                          uint32_t *               pPropertyCount,
                                                                          VkDisplayPropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR( VkPhysicalDevice              physicalDevice,
                                                                               uint32_t *                    pPropertyCount,
                                                                               VkDisplayPlanePropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetDisplayPlaneSupportedDisplaysKHR( VkPhysicalDevice physicalDevice,
                                                                        uint32_t         planeIndex,
                                                                        uint32_t *       pDisplayCount,
                                                                        VkDisplayKHR *   pDisplays ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays );
      }

      VULKAN_HPP_INLINE VkResult vkGetDisplayModePropertiesKHR( VkPhysicalDevice             physicalDevice,
                                                                VkDisplayKHR                 display,
                                                                uint32_t *                   pPropertyCount,
                                                                VkDisplayModePropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDisplayModeKHR( VkPhysicalDevice                   physicalDevice,
                                                         VkDisplayKHR                       display,
                                                         VkDisplayModeCreateInfoKHR const * pCreateInfo,
                                                         VkAllocationCallbacks const *      pAllocator,
                                                         VkDisplayModeKHR *                 pMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDisplayModeKHR( physicalDevice, display, pCreateInfo, pAllocator, pMode );
      }

      VULKAN_HPP_INLINE VkResult vkGetDisplayPlaneCapabilitiesKHR( VkPhysicalDevice                physicalDevice,
                                                                   VkDisplayModeKHR                mode,
                                                                   uint32_t                        planeIndex,
                                                                   VkDisplayPlaneCapabilitiesKHR * pCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, pCapabilities );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDisplayPlaneSurfaceKHR( VkInstance                            instance,
                                                                 VkDisplaySurfaceCreateInfoKHR const * pCreateInfo,
                                                                 VkAllocationCallbacks const *         pAllocator,
                                                                 VkSurfaceKHR *                        pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }

      //=== VK_KHR_display_swapchain ===

      VULKAN_HPP_INLINE VkResult vkCreateSharedSwapchainsKHR( VkDevice                         device,
                                                              uint32_t                         swapchainCount,
                                                              VkSwapchainCreateInfoKHR const * pCreateInfos,
                                                              VkAllocationCallbacks const *    pAllocator,
                                                              VkSwapchainKHR *                 pSwapchains ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSharedSwapchainsKHR( device, swapchainCount, pCreateInfos, pAllocator, pSwapchains );
      }

#    if defined( VK_USE_PLATFORM_XLIB_KHR )
      //=== VK_KHR_xlib_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateXlibSurfaceKHR( VkInstance                         instance,
                                                         VkXlibSurfaceCreateInfoKHR const * pCreateInfo,
                                                         VkAllocationCallbacks const *      pAllocator,
                                                         VkSurfaceKHR *                     pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateXlibSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceXlibPresentationSupportKHR( VkPhysicalDevice physicalDevice,
                                                                                uint32_t         queueFamilyIndex,
                                                                                Display *        dpy,
                                                                                VisualID         visualID ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
      }
#    endif /*VK_USE_PLATFORM_XLIB_KHR*/

#    if defined( VK_USE_PLATFORM_XCB_KHR )
      //=== VK_KHR_xcb_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateXcbSurfaceKHR( VkInstance                        instance,
                                                        VkXcbSurfaceCreateInfoKHR const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkSurfaceKHR *                    pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateXcbSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceXcbPresentationSupportKHR( VkPhysicalDevice   physicalDevice,
                                                                               uint32_t           queueFamilyIndex,
                                                                               xcb_connection_t * connection,
                                                                               xcb_visualid_t     visual_id ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
      }
#    endif /*VK_USE_PLATFORM_XCB_KHR*/

#    if defined( VK_USE_PLATFORM_WAYLAND_KHR )
      //=== VK_KHR_wayland_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateWaylandSurfaceKHR( VkInstance                            instance,
                                                            VkWaylandSurfaceCreateInfoKHR const * pCreateInfo,
                                                            VkAllocationCallbacks const *         pAllocator,
                                                            VkSurfaceKHR *                        pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateWaylandSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceWaylandPresentationSupportKHR( VkPhysicalDevice    physicalDevice,
                                                                                   uint32_t            queueFamilyIndex,
                                                                                   struct wl_display * display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
      }
#    endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#    if defined( VK_USE_PLATFORM_ANDROID_KHR )
      //=== VK_KHR_android_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateAndroidSurfaceKHR( VkInstance                            instance,
                                                            VkAndroidSurfaceCreateInfoKHR const * pCreateInfo,
                                                            VkAllocationCallbacks const *         pAllocator,
                                                            VkSurfaceKHR *                        pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateAndroidSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_win32_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateWin32SurfaceKHR( VkInstance                          instance,
                                                          VkWin32SurfaceCreateInfoKHR const * pCreateInfo,
                                                          VkAllocationCallbacks const *       pAllocator,
                                                          VkSurfaceKHR *                      pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateWin32SurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceWin32PresentationSupportKHR( VkPhysicalDevice physicalDevice,
                                                                                 uint32_t         queueFamilyIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, queueFamilyIndex );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_EXT_debug_report ===

      VULKAN_HPP_INLINE VkResult vkCreateDebugReportCallbackEXT( VkInstance                                 instance,
                                                                 VkDebugReportCallbackCreateInfoEXT const * pCreateInfo,
                                                                 VkAllocationCallbacks const *              pAllocator,
                                                                 VkDebugReportCallbackEXT *                 pCallback ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDebugReportCallbackEXT( instance, pCreateInfo, pAllocator, pCallback );
      }

      VULKAN_HPP_INLINE void vkDestroyDebugReportCallbackEXT( VkInstance                    instance,
                                                              VkDebugReportCallbackEXT      callback,
                                                              VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDebugReportCallbackEXT( instance, callback, pAllocator );
      }

      VULKAN_HPP_INLINE void vkDebugReportMessageEXT( VkInstance                 instance,
                                                      VkDebugReportFlagsEXT      flags,
                                                      VkDebugReportObjectTypeEXT objectType,
                                                      uint64_t                   object,
                                                      size_t                     location,
                                                      int32_t                    messageCode,
                                                      char const *               pLayerPrefix,
                                                      char const *               pMessage ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDebugReportMessageEXT( instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage );
      }

      //=== VK_EXT_debug_marker ===

      VULKAN_HPP_INLINE VkResult vkDebugMarkerSetObjectTagEXT( VkDevice device, VkDebugMarkerObjectTagInfoEXT const * pTagInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDebugMarkerSetObjectTagEXT( device, pTagInfo );
      }

      VULKAN_HPP_INLINE VkResult vkDebugMarkerSetObjectNameEXT( VkDevice device, VkDebugMarkerObjectNameInfoEXT const * pNameInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDebugMarkerSetObjectNameEXT( device, pNameInfo );
      }

      VULKAN_HPP_INLINE void vkCmdDebugMarkerBeginEXT( VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT const * pMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDebugMarkerBeginEXT( commandBuffer, pMarkerInfo );
      }

      VULKAN_HPP_INLINE void vkCmdDebugMarkerEndEXT( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDebugMarkerEndEXT( commandBuffer );
      }

      VULKAN_HPP_INLINE void vkCmdDebugMarkerInsertEXT( VkCommandBuffer                    commandBuffer,
                                                        VkDebugMarkerMarkerInfoEXT const * pMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDebugMarkerInsertEXT( commandBuffer, pMarkerInfo );
      }

      //=== VK_KHR_video_queue ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceVideoCapabilitiesKHR( VkPhysicalDevice              physicalDevice,
                                                                          VkVideoProfileInfoKHR const * pVideoProfile,
                                                                          VkVideoCapabilitiesKHR *      pCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceVideoCapabilitiesKHR( physicalDevice, pVideoProfile, pCapabilities );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceVideoFormatPropertiesKHR( VkPhysicalDevice                           physicalDevice,
                                                                              VkPhysicalDeviceVideoFormatInfoKHR const * pVideoFormatInfo,
                                                                              uint32_t *                                 pVideoFormatPropertyCount,
                                                                              VkVideoFormatPropertiesKHR * pVideoFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceVideoFormatPropertiesKHR( physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties );
      }

      VULKAN_HPP_INLINE VkResult vkCreateVideoSessionKHR( VkDevice                            device,
                                                          VkVideoSessionCreateInfoKHR const * pCreateInfo,
                                                          VkAllocationCallbacks const *       pAllocator,
                                                          VkVideoSessionKHR *                 pVideoSession ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateVideoSessionKHR( device, pCreateInfo, pAllocator, pVideoSession );
      }

      VULKAN_HPP_INLINE void
        vkDestroyVideoSessionKHR( VkDevice device, VkVideoSessionKHR videoSession, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyVideoSessionKHR( device, videoSession, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetVideoSessionMemoryRequirementsKHR( VkDevice                              device,
                                                                         VkVideoSessionKHR                     videoSession,
                                                                         uint32_t *                            pMemoryRequirementsCount,
                                                                         VkVideoSessionMemoryRequirementsKHR * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetVideoSessionMemoryRequirementsKHR( device, videoSession, pMemoryRequirementsCount, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE VkResult vkBindVideoSessionMemoryKHR( VkDevice                                device,
                                                              VkVideoSessionKHR                       videoSession,
                                                              uint32_t                                bindSessionMemoryInfoCount,
                                                              VkBindVideoSessionMemoryInfoKHR const * pBindSessionMemoryInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindVideoSessionMemoryKHR( device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos );
      }

      VULKAN_HPP_INLINE VkResult vkCreateVideoSessionParametersKHR( VkDevice                                      device,
                                                                    VkVideoSessionParametersCreateInfoKHR const * pCreateInfo,
                                                                    VkAllocationCallbacks const *                 pAllocator,
                                                                    VkVideoSessionParametersKHR * pVideoSessionParameters ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateVideoSessionParametersKHR( device, pCreateInfo, pAllocator, pVideoSessionParameters );
      }

      VULKAN_HPP_INLINE VkResult vkUpdateVideoSessionParametersKHR( VkDevice                                      device,
                                                                    VkVideoSessionParametersKHR                   videoSessionParameters,
                                                                    VkVideoSessionParametersUpdateInfoKHR const * pUpdateInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateVideoSessionParametersKHR( device, videoSessionParameters, pUpdateInfo );
      }

      VULKAN_HPP_INLINE void vkDestroyVideoSessionParametersKHR( VkDevice                      device,
                                                                 VkVideoSessionParametersKHR   videoSessionParameters,
                                                                 VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyVideoSessionParametersKHR( device, videoSessionParameters, pAllocator );
      }

      VULKAN_HPP_INLINE void vkCmdBeginVideoCodingKHR( VkCommandBuffer commandBuffer, VkVideoBeginCodingInfoKHR const * pBeginInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginVideoCodingKHR( commandBuffer, pBeginInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndVideoCodingKHR( VkCommandBuffer commandBuffer, VkVideoEndCodingInfoKHR const * pEndCodingInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndVideoCodingKHR( commandBuffer, pEndCodingInfo );
      }

      VULKAN_HPP_INLINE void vkCmdControlVideoCodingKHR( VkCommandBuffer                     commandBuffer,
                                                         VkVideoCodingControlInfoKHR const * pCodingControlInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdControlVideoCodingKHR( commandBuffer, pCodingControlInfo );
      }

      //=== VK_KHR_video_decode_queue ===

      VULKAN_HPP_INLINE void vkCmdDecodeVideoKHR( VkCommandBuffer commandBuffer, VkVideoDecodeInfoKHR const * pDecodeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDecodeVideoKHR( commandBuffer, pDecodeInfo );
      }

      //=== VK_EXT_transform_feedback ===

      VULKAN_HPP_INLINE void vkCmdBindTransformFeedbackBuffersEXT( VkCommandBuffer      commandBuffer,
                                                                   uint32_t             firstBinding,
                                                                   uint32_t             bindingCount,
                                                                   VkBuffer const *     pBuffers,
                                                                   VkDeviceSize const * pOffsets,
                                                                   VkDeviceSize const * pSizes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindTransformFeedbackBuffersEXT( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes );
      }

      VULKAN_HPP_INLINE void vkCmdBeginTransformFeedbackEXT( VkCommandBuffer      commandBuffer,
                                                             uint32_t             firstCounterBuffer,
                                                             uint32_t             counterBufferCount,
                                                             VkBuffer const *     pCounterBuffers,
                                                             VkDeviceSize const * pCounterBufferOffsets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginTransformFeedbackEXT( commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets );
      }

      VULKAN_HPP_INLINE void vkCmdEndTransformFeedbackEXT( VkCommandBuffer      commandBuffer,
                                                           uint32_t             firstCounterBuffer,
                                                           uint32_t             counterBufferCount,
                                                           VkBuffer const *     pCounterBuffers,
                                                           VkDeviceSize const * pCounterBufferOffsets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndTransformFeedbackEXT( commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets );
      }

      VULKAN_HPP_INLINE void vkCmdBeginQueryIndexedEXT(
        VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginQueryIndexedEXT( commandBuffer, queryPool, query, flags, index );
      }

      VULKAN_HPP_INLINE void
        vkCmdEndQueryIndexedEXT( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndQueryIndexedEXT( commandBuffer, queryPool, query, index );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndirectByteCountEXT( VkCommandBuffer commandBuffer,
                                                            uint32_t        instanceCount,
                                                            uint32_t        firstInstance,
                                                            VkBuffer        counterBuffer,
                                                            VkDeviceSize    counterBufferOffset,
                                                            uint32_t        counterOffset,
                                                            uint32_t        vertexStride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndirectByteCountEXT( commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride );
      }

      //=== VK_NVX_binary_import ===

      VULKAN_HPP_INLINE VkResult vkCreateCuModuleNVX( VkDevice                        device,
                                                      VkCuModuleCreateInfoNVX const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkCuModuleNVX *                 pModule ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateCuModuleNVX( device, pCreateInfo, pAllocator, pModule );
      }

      VULKAN_HPP_INLINE VkResult vkCreateCuFunctionNVX( VkDevice                          device,
                                                        VkCuFunctionCreateInfoNVX const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkCuFunctionNVX *                 pFunction ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateCuFunctionNVX( device, pCreateInfo, pAllocator, pFunction );
      }

      VULKAN_HPP_INLINE void vkDestroyCuModuleNVX( VkDevice device, VkCuModuleNVX module, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyCuModuleNVX( device, module, pAllocator );
      }

      VULKAN_HPP_INLINE void
        vkDestroyCuFunctionNVX( VkDevice device, VkCuFunctionNVX function, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyCuFunctionNVX( device, function, pAllocator );
      }

      VULKAN_HPP_INLINE void vkCmdCuLaunchKernelNVX( VkCommandBuffer commandBuffer, VkCuLaunchInfoNVX const * pLaunchInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCuLaunchKernelNVX( commandBuffer, pLaunchInfo );
      }

      //=== VK_NVX_image_view_handle ===

      VULKAN_HPP_INLINE uint32_t vkGetImageViewHandleNVX( VkDevice device, VkImageViewHandleInfoNVX const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageViewHandleNVX( device, pInfo );
      }

      VULKAN_HPP_INLINE uint64_t vkGetImageViewHandle64NVX( VkDevice device, VkImageViewHandleInfoNVX const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageViewHandle64NVX( device, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetImageViewAddressNVX( VkDevice                          device,
                                                           VkImageView                       imageView,
                                                           VkImageViewAddressPropertiesNVX * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageViewAddressNVX( device, imageView, pProperties );
      }

      VULKAN_HPP_INLINE uint64_t vkGetDeviceCombinedImageSamplerIndexNVX( VkDevice device,
                                                                          uint64_t imageViewIndex,
                                                                          uint64_t samplerIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceCombinedImageSamplerIndexNVX( device, imageViewIndex, samplerIndex );
      }

      //=== VK_AMD_draw_indirect_count ===

      VULKAN_HPP_INLINE void vkCmdDrawIndirectCountAMD( VkCommandBuffer commandBuffer,
                                                        VkBuffer        buffer,
                                                        VkDeviceSize    offset,
                                                        VkBuffer        countBuffer,
                                                        VkDeviceSize    countBufferOffset,
                                                        uint32_t        maxDrawCount,
                                                        uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndirectCountAMD( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndexedIndirectCountAMD( VkCommandBuffer commandBuffer,
                                                               VkBuffer        buffer,
                                                               VkDeviceSize    offset,
                                                               VkBuffer        countBuffer,
                                                               VkDeviceSize    countBufferOffset,
                                                               uint32_t        maxDrawCount,
                                                               uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndexedIndirectCountAMD( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      //=== VK_AMD_shader_info ===

      VULKAN_HPP_INLINE VkResult vkGetShaderInfoAMD( VkDevice              device,
                                                     VkPipeline            pipeline,
                                                     VkShaderStageFlagBits shaderStage,
                                                     VkShaderInfoTypeAMD   infoType,
                                                     size_t *              pInfoSize,
                                                     void *                pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetShaderInfoAMD( device, pipeline, shaderStage, infoType, pInfoSize, pInfo );
      }

      //=== VK_KHR_dynamic_rendering ===

      VULKAN_HPP_INLINE void vkCmdBeginRenderingKHR( VkCommandBuffer commandBuffer, VkRenderingInfo const * pRenderingInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginRenderingKHR( commandBuffer, pRenderingInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndRenderingKHR( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRenderingKHR( commandBuffer );
      }

#    if defined( VK_USE_PLATFORM_GGP )
      //=== VK_GGP_stream_descriptor_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateStreamDescriptorSurfaceGGP( VkInstance                                     instance,
                                                                     VkStreamDescriptorSurfaceCreateInfoGGP const * pCreateInfo,
                                                                     VkAllocationCallbacks const *                  pAllocator,
                                                                     VkSurfaceKHR *                                 pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateStreamDescriptorSurfaceGGP( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_GGP*/

      //=== VK_NV_external_memory_capabilities ===

      VULKAN_HPP_INLINE VkResult
        vkGetPhysicalDeviceExternalImageFormatPropertiesNV( VkPhysicalDevice                    physicalDevice,
                                                            VkFormat                            format,
                                                            VkImageType                         type,
                                                            VkImageTiling                       tiling,
                                                            VkImageUsageFlags                   usage,
                                                            VkImageCreateFlags                  flags,
                                                            VkExternalMemoryHandleTypeFlagsNV   externalHandleType,
                                                            VkExternalImageFormatPropertiesNV * pExternalImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalImageFormatPropertiesNV(
          physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_NV_external_memory_win32 ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryWin32HandleNV( VkDevice                          device,
                                                           VkDeviceMemory                    memory,
                                                           VkExternalMemoryHandleTypeFlagsNV handleType,
                                                           HANDLE *                          pHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryWin32HandleNV( device, memory, handleType, pHandle );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_get_physical_device_properties2 ===

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFeatures2KHR( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2 * pFeatures ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFeatures2KHR( physicalDevice, pFeatures );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceProperties2KHR( VkPhysicalDevice              physicalDevice,
                                                                VkPhysicalDeviceProperties2 * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceProperties2KHR( physicalDevice, pProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceFormatProperties2KHR( VkPhysicalDevice      physicalDevice,
                                                                      VkFormat              format,
                                                                      VkFormatProperties2 * pFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFormatProperties2KHR( physicalDevice, format, pFormatProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceImageFormatProperties2KHR( VkPhysicalDevice                         physicalDevice,
                                                                               VkPhysicalDeviceImageFormatInfo2 const * pImageFormatInfo,
                                                                               VkImageFormatProperties2 * pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceImageFormatProperties2KHR( physicalDevice, pImageFormatInfo, pImageFormatProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceQueueFamilyProperties2KHR( VkPhysicalDevice           physicalDevice,
                                                                           uint32_t *                 pQueueFamilyPropertyCount,
                                                                           VkQueueFamilyProperties2 * pQueueFamilyProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyProperties2KHR( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceMemoryProperties2KHR( VkPhysicalDevice                    physicalDevice,
                                                                      VkPhysicalDeviceMemoryProperties2 * pMemoryProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceMemoryProperties2KHR( physicalDevice, pMemoryProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceSparseImageFormatProperties2KHR( VkPhysicalDevice                               physicalDevice,
                                                                                 VkPhysicalDeviceSparseImageFormatInfo2 const * pFormatInfo,
                                                                                 uint32_t *                                     pPropertyCount,
                                                                                 VkSparseImageFormatProperties2 * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSparseImageFormatProperties2KHR( physicalDevice, pFormatInfo, pPropertyCount, pProperties );
      }

      //=== VK_KHR_device_group ===

      VULKAN_HPP_INLINE void vkGetDeviceGroupPeerMemoryFeaturesKHR( VkDevice                   device,
                                                                    uint32_t                   heapIndex,
                                                                    uint32_t                   localDeviceIndex,
                                                                    uint32_t                   remoteDeviceIndex,
                                                                    VkPeerMemoryFeatureFlags * pPeerMemoryFeatures ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceGroupPeerMemoryFeaturesKHR( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
      }

      VULKAN_HPP_INLINE void vkCmdSetDeviceMaskKHR( VkCommandBuffer commandBuffer, uint32_t deviceMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDeviceMaskKHR( commandBuffer, deviceMask );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchBaseKHR( VkCommandBuffer commandBuffer,
                                                   uint32_t        baseGroupX,
                                                   uint32_t        baseGroupY,
                                                   uint32_t        baseGroupZ,
                                                   uint32_t        groupCountX,
                                                   uint32_t        groupCountY,
                                                   uint32_t        groupCountZ ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchBaseKHR( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
      }

#    if defined( VK_USE_PLATFORM_VI_NN )
      //=== VK_NN_vi_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateViSurfaceNN( VkInstance                      instance,
                                                      VkViSurfaceCreateInfoNN const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkSurfaceKHR *                  pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateViSurfaceNN( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_VI_NN*/

      //=== VK_KHR_maintenance1 ===

      VULKAN_HPP_INLINE void vkTrimCommandPoolKHR( VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkTrimCommandPoolKHR( device, commandPool, flags );
      }

      //=== VK_KHR_device_group_creation ===

      VULKAN_HPP_INLINE VkResult vkEnumeratePhysicalDeviceGroupsKHR(
        VkInstance instance, uint32_t * pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumeratePhysicalDeviceGroupsKHR( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
      }

      //=== VK_KHR_external_memory_capabilities ===

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceExternalBufferPropertiesKHR( VkPhysicalDevice                           physicalDevice,
                                                                             VkPhysicalDeviceExternalBufferInfo const * pExternalBufferInfo,
                                                                             VkExternalBufferProperties * pExternalBufferProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalBufferPropertiesKHR( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_memory_win32 ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryWin32HandleKHR( VkDevice                              device,
                                                            VkMemoryGetWin32HandleInfoKHR const * pGetWin32HandleInfo,
                                                            HANDLE *                              pHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryWin32HandleKHR( device, pGetWin32HandleInfo, pHandle );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetMemoryWin32HandlePropertiesKHR( VkDevice                           device,
                                             VkExternalMemoryHandleTypeFlagBits handleType,
                                             HANDLE                             handle,
                                             VkMemoryWin32HandlePropertiesKHR * pMemoryWin32HandleProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryWin32HandlePropertiesKHR( device, handleType, handle, pMemoryWin32HandleProperties );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_memory_fd ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryFdKHR( VkDevice device, VkMemoryGetFdInfoKHR const * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryFdKHR( device, pGetFdInfo, pFd );
      }

      VULKAN_HPP_INLINE VkResult vkGetMemoryFdPropertiesKHR( VkDevice                           device,
                                                             VkExternalMemoryHandleTypeFlagBits handleType,
                                                             int                                fd,
                                                             VkMemoryFdPropertiesKHR *          pMemoryFdProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryFdPropertiesKHR( device, handleType, fd, pMemoryFdProperties );
      }

      //=== VK_KHR_external_semaphore_capabilities ===

      VULKAN_HPP_INLINE void
        vkGetPhysicalDeviceExternalSemaphorePropertiesKHR( VkPhysicalDevice                              physicalDevice,
                                                           VkPhysicalDeviceExternalSemaphoreInfo const * pExternalSemaphoreInfo,
                                                           VkExternalSemaphoreProperties * pExternalSemaphoreProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalSemaphorePropertiesKHR( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_semaphore_win32 ===

      VULKAN_HPP_INLINE VkResult vkImportSemaphoreWin32HandleKHR(
        VkDevice device, VkImportSemaphoreWin32HandleInfoKHR const * pImportSemaphoreWin32HandleInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkImportSemaphoreWin32HandleKHR( device, pImportSemaphoreWin32HandleInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetSemaphoreWin32HandleKHR( VkDevice                                 device,
                                                               VkSemaphoreGetWin32HandleInfoKHR const * pGetWin32HandleInfo,
                                                               HANDLE *                                 pHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSemaphoreWin32HandleKHR( device, pGetWin32HandleInfo, pHandle );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_semaphore_fd ===

      VULKAN_HPP_INLINE VkResult vkImportSemaphoreFdKHR( VkDevice device, VkImportSemaphoreFdInfoKHR const * pImportSemaphoreFdInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkImportSemaphoreFdKHR( device, pImportSemaphoreFdInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetSemaphoreFdKHR( VkDevice device, VkSemaphoreGetFdInfoKHR const * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSemaphoreFdKHR( device, pGetFdInfo, pFd );
      }

      //=== VK_KHR_push_descriptor ===

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSetKHR( VkCommandBuffer              commandBuffer,
                                                        VkPipelineBindPoint          pipelineBindPoint,
                                                        VkPipelineLayout             layout,
                                                        uint32_t                     set,
                                                        uint32_t                     descriptorWriteCount,
                                                        VkWriteDescriptorSet const * pDescriptorWrites ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSetKHR( commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites );
      }

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSetWithTemplateKHR( VkCommandBuffer            commandBuffer,
                                                                    VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                                                    VkPipelineLayout           layout,
                                                                    uint32_t                   set,
                                                                    void const *               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSetWithTemplateKHR( commandBuffer, descriptorUpdateTemplate, layout, set, pData );
      }

      //=== VK_EXT_conditional_rendering ===

      VULKAN_HPP_INLINE void
        vkCmdBeginConditionalRenderingEXT( VkCommandBuffer                            commandBuffer,
                                           VkConditionalRenderingBeginInfoEXT const * pConditionalRenderingBegin ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginConditionalRenderingEXT( commandBuffer, pConditionalRenderingBegin );
      }

      VULKAN_HPP_INLINE void vkCmdEndConditionalRenderingEXT( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndConditionalRenderingEXT( commandBuffer );
      }

      //=== VK_KHR_descriptor_update_template ===

      VULKAN_HPP_INLINE VkResult vkCreateDescriptorUpdateTemplateKHR( VkDevice                                     device,
                                                                      VkDescriptorUpdateTemplateCreateInfo const * pCreateInfo,
                                                                      VkAllocationCallbacks const *                pAllocator,
                                                                      VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDescriptorUpdateTemplateKHR( device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate );
      }

      VULKAN_HPP_INLINE void vkDestroyDescriptorUpdateTemplateKHR( VkDevice                      device,
                                                                   VkDescriptorUpdateTemplate    descriptorUpdateTemplate,
                                                                   VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDescriptorUpdateTemplateKHR( device, descriptorUpdateTemplate, pAllocator );
      }

      VULKAN_HPP_INLINE void vkUpdateDescriptorSetWithTemplateKHR( VkDevice                   device,
                                                                   VkDescriptorSet            descriptorSet,
                                                                   VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                                                   void const *               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateDescriptorSetWithTemplateKHR( device, descriptorSet, descriptorUpdateTemplate, pData );
      }

      //=== VK_NV_clip_space_w_scaling ===

      VULKAN_HPP_INLINE void vkCmdSetViewportWScalingNV( VkCommandBuffer              commandBuffer,
                                                         uint32_t                     firstViewport,
                                                         uint32_t                     viewportCount,
                                                         VkViewportWScalingNV const * pViewportWScalings ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportWScalingNV( commandBuffer, firstViewport, viewportCount, pViewportWScalings );
      }

      //=== VK_EXT_direct_mode_display ===

      VULKAN_HPP_INLINE VkResult vkReleaseDisplayEXT( VkPhysicalDevice physicalDevice, VkDisplayKHR display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseDisplayEXT( physicalDevice, display );
      }

#    if defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
      //=== VK_EXT_acquire_xlib_display ===

      VULKAN_HPP_INLINE VkResult vkAcquireXlibDisplayEXT( VkPhysicalDevice physicalDevice, Display * dpy, VkDisplayKHR display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireXlibDisplayEXT( physicalDevice, dpy, display );
      }

      VULKAN_HPP_INLINE VkResult vkGetRandROutputDisplayEXT( VkPhysicalDevice physicalDevice,
                                                             Display *        dpy,
                                                             RROutput         rrOutput,
                                                             VkDisplayKHR *   pDisplay ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRandROutputDisplayEXT( physicalDevice, dpy, rrOutput, pDisplay );
      }
#    endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

      //=== VK_EXT_display_surface_counter ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceCapabilities2EXT( VkPhysicalDevice            physicalDevice,
                                                                             VkSurfaceKHR                surface,
                                                                             VkSurfaceCapabilities2EXT * pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceCapabilities2EXT( physicalDevice, surface, pSurfaceCapabilities );
      }

      //=== VK_EXT_display_control ===

      VULKAN_HPP_INLINE VkResult vkDisplayPowerControlEXT( VkDevice                      device,
                                                           VkDisplayKHR                  display,
                                                           VkDisplayPowerInfoEXT const * pDisplayPowerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDisplayPowerControlEXT( device, display, pDisplayPowerInfo );
      }

      VULKAN_HPP_INLINE VkResult vkRegisterDeviceEventEXT( VkDevice                      device,
                                                           VkDeviceEventInfoEXT const *  pDeviceEventInfo,
                                                           VkAllocationCallbacks const * pAllocator,
                                                           VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkRegisterDeviceEventEXT( device, pDeviceEventInfo, pAllocator, pFence );
      }

      VULKAN_HPP_INLINE VkResult vkRegisterDisplayEventEXT( VkDevice                      device,
                                                            VkDisplayKHR                  display,
                                                            VkDisplayEventInfoEXT const * pDisplayEventInfo,
                                                            VkAllocationCallbacks const * pAllocator,
                                                            VkFence *                     pFence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkRegisterDisplayEventEXT( device, display, pDisplayEventInfo, pAllocator, pFence );
      }

      VULKAN_HPP_INLINE VkResult vkGetSwapchainCounterEXT( VkDevice                    device,
                                                           VkSwapchainKHR              swapchain,
                                                           VkSurfaceCounterFlagBitsEXT counter,
                                                           uint64_t *                  pCounterValue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSwapchainCounterEXT( device, swapchain, counter, pCounterValue );
      }

      //=== VK_GOOGLE_display_timing ===

      VULKAN_HPP_INLINE VkResult vkGetRefreshCycleDurationGOOGLE( VkDevice                       device,
                                                                  VkSwapchainKHR                 swapchain,
                                                                  VkRefreshCycleDurationGOOGLE * pDisplayTimingProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRefreshCycleDurationGOOGLE( device, swapchain, pDisplayTimingProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPastPresentationTimingGOOGLE( VkDevice                         device,
                                                                    VkSwapchainKHR                   swapchain,
                                                                    uint32_t *                       pPresentationTimingCount,
                                                                    VkPastPresentationTimingGOOGLE * pPresentationTimings ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPastPresentationTimingGOOGLE( device, swapchain, pPresentationTimingCount, pPresentationTimings );
      }

      //=== VK_EXT_discard_rectangles ===

      VULKAN_HPP_INLINE void vkCmdSetDiscardRectangleEXT( VkCommandBuffer  commandBuffer,
                                                          uint32_t         firstDiscardRectangle,
                                                          uint32_t         discardRectangleCount,
                                                          VkRect2D const * pDiscardRectangles ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDiscardRectangleEXT( commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles );
      }

      VULKAN_HPP_INLINE void vkCmdSetDiscardRectangleEnableEXT( VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDiscardRectangleEnableEXT( commandBuffer, discardRectangleEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDiscardRectangleModeEXT( VkCommandBuffer           commandBuffer,
                                                              VkDiscardRectangleModeEXT discardRectangleMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDiscardRectangleModeEXT( commandBuffer, discardRectangleMode );
      }

      //=== VK_EXT_hdr_metadata ===

      VULKAN_HPP_INLINE void vkSetHdrMetadataEXT( VkDevice                 device,
                                                  uint32_t                 swapchainCount,
                                                  VkSwapchainKHR const *   pSwapchains,
                                                  VkHdrMetadataEXT const * pMetadata ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetHdrMetadataEXT( device, swapchainCount, pSwapchains, pMetadata );
      }

      //=== VK_KHR_create_renderpass2 ===

      VULKAN_HPP_INLINE VkResult vkCreateRenderPass2KHR( VkDevice                        device,
                                                         VkRenderPassCreateInfo2 const * pCreateInfo,
                                                         VkAllocationCallbacks const *   pAllocator,
                                                         VkRenderPass *                  pRenderPass ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateRenderPass2KHR( device, pCreateInfo, pAllocator, pRenderPass );
      }

      VULKAN_HPP_INLINE void vkCmdBeginRenderPass2KHR( VkCommandBuffer               commandBuffer,
                                                       VkRenderPassBeginInfo const * pRenderPassBegin,
                                                       VkSubpassBeginInfo const *    pSubpassBeginInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginRenderPass2KHR( commandBuffer, pRenderPassBegin, pSubpassBeginInfo );
      }

      VULKAN_HPP_INLINE void vkCmdNextSubpass2KHR( VkCommandBuffer            commandBuffer,
                                                   VkSubpassBeginInfo const * pSubpassBeginInfo,
                                                   VkSubpassEndInfo const *   pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdNextSubpass2KHR( commandBuffer, pSubpassBeginInfo, pSubpassEndInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndRenderPass2KHR( VkCommandBuffer commandBuffer, VkSubpassEndInfo const * pSubpassEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRenderPass2KHR( commandBuffer, pSubpassEndInfo );
      }

      //=== VK_KHR_shared_presentable_image ===

      VULKAN_HPP_INLINE VkResult vkGetSwapchainStatusKHR( VkDevice device, VkSwapchainKHR swapchain ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSwapchainStatusKHR( device, swapchain );
      }

      //=== VK_KHR_external_fence_capabilities ===

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceExternalFencePropertiesKHR( VkPhysicalDevice                          physicalDevice,
                                                                            VkPhysicalDeviceExternalFenceInfo const * pExternalFenceInfo,
                                                                            VkExternalFenceProperties * pExternalFenceProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalFencePropertiesKHR( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_fence_win32 ===

      VULKAN_HPP_INLINE VkResult vkImportFenceWin32HandleKHR( VkDevice                                device,
                                                              VkImportFenceWin32HandleInfoKHR const * pImportFenceWin32HandleInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkImportFenceWin32HandleKHR( device, pImportFenceWin32HandleInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetFenceWin32HandleKHR( VkDevice                             device,
                                                           VkFenceGetWin32HandleInfoKHR const * pGetWin32HandleInfo,
                                                           HANDLE *                             pHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetFenceWin32HandleKHR( device, pGetWin32HandleInfo, pHandle );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_fence_fd ===

      VULKAN_HPP_INLINE VkResult vkImportFenceFdKHR( VkDevice device, VkImportFenceFdInfoKHR const * pImportFenceFdInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkImportFenceFdKHR( device, pImportFenceFdInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetFenceFdKHR( VkDevice device, VkFenceGetFdInfoKHR const * pGetFdInfo, int * pFd ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetFenceFdKHR( device, pGetFdInfo, pFd );
      }

      //=== VK_KHR_performance_query ===

      VULKAN_HPP_INLINE VkResult
        vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR( VkPhysicalDevice                     physicalDevice,
                                                                         uint32_t                             queueFamilyIndex,
                                                                         uint32_t *                           pCounterCount,
                                                                         VkPerformanceCounterKHR *            pCounters,
                                                                         VkPerformanceCounterDescriptionKHR * pCounterDescriptions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
          physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR( VkPhysicalDevice                            physicalDevice,
                                                                                      VkQueryPoolPerformanceCreateInfoKHR const * pPerformanceQueryCreateInfo,
                                                                                      uint32_t * pNumPasses ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR( physicalDevice, pPerformanceQueryCreateInfo, pNumPasses );
      }

      VULKAN_HPP_INLINE VkResult vkAcquireProfilingLockKHR( VkDevice device, VkAcquireProfilingLockInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireProfilingLockKHR( device, pInfo );
      }

      VULKAN_HPP_INLINE void vkReleaseProfilingLockKHR( VkDevice device ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseProfilingLockKHR( device );
      }

      //=== VK_KHR_get_surface_capabilities2 ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR( VkPhysicalDevice                        physicalDevice,
                                                                             VkPhysicalDeviceSurfaceInfo2KHR const * pSurfaceInfo,
                                                                             VkSurfaceCapabilities2KHR * pSurfaceCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceCapabilities2KHR( physicalDevice, pSurfaceInfo, pSurfaceCapabilities );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfaceFormats2KHR( VkPhysicalDevice                        physicalDevice,
                                                                        VkPhysicalDeviceSurfaceInfo2KHR const * pSurfaceInfo,
                                                                        uint32_t *                              pSurfaceFormatCount,
                                                                        VkSurfaceFormat2KHR *                   pSurfaceFormats ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfaceFormats2KHR( physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats );
      }

      //=== VK_KHR_get_display_properties2 ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceDisplayProperties2KHR( VkPhysicalDevice          physicalDevice,
                                                                           uint32_t *                pPropertyCount,
                                                                           VkDisplayProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDisplayProperties2KHR( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR( VkPhysicalDevice               physicalDevice,
                                                                                uint32_t *                     pPropertyCount,
                                                                                VkDisplayPlaneProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDisplayPlaneProperties2KHR( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetDisplayModeProperties2KHR( VkPhysicalDevice              physicalDevice,
                                                                 VkDisplayKHR                  display,
                                                                 uint32_t *                    pPropertyCount,
                                                                 VkDisplayModeProperties2KHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDisplayModeProperties2KHR( physicalDevice, display, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetDisplayPlaneCapabilities2KHR( VkPhysicalDevice                 physicalDevice,
                                                                    VkDisplayPlaneInfo2KHR const *   pDisplayPlaneInfo,
                                                                    VkDisplayPlaneCapabilities2KHR * pCapabilities ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDisplayPlaneCapabilities2KHR( physicalDevice, pDisplayPlaneInfo, pCapabilities );
      }

#    if defined( VK_USE_PLATFORM_IOS_MVK )
      //=== VK_MVK_ios_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateIOSSurfaceMVK( VkInstance                        instance,
                                                        VkIOSSurfaceCreateInfoMVK const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkSurfaceKHR *                    pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateIOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_IOS_MVK*/

#    if defined( VK_USE_PLATFORM_MACOS_MVK )
      //=== VK_MVK_macos_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateMacOSSurfaceMVK( VkInstance                          instance,
                                                          VkMacOSSurfaceCreateInfoMVK const * pCreateInfo,
                                                          VkAllocationCallbacks const *       pAllocator,
                                                          VkSurfaceKHR *                      pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateMacOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_MACOS_MVK*/

      //=== VK_EXT_debug_utils ===

      VULKAN_HPP_INLINE VkResult vkSetDebugUtilsObjectNameEXT( VkDevice device, VkDebugUtilsObjectNameInfoEXT const * pNameInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetDebugUtilsObjectNameEXT( device, pNameInfo );
      }

      VULKAN_HPP_INLINE VkResult vkSetDebugUtilsObjectTagEXT( VkDevice device, VkDebugUtilsObjectTagInfoEXT const * pTagInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetDebugUtilsObjectTagEXT( device, pTagInfo );
      }

      VULKAN_HPP_INLINE void vkQueueBeginDebugUtilsLabelEXT( VkQueue queue, VkDebugUtilsLabelEXT const * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueBeginDebugUtilsLabelEXT( queue, pLabelInfo );
      }

      VULKAN_HPP_INLINE void vkQueueEndDebugUtilsLabelEXT( VkQueue queue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueEndDebugUtilsLabelEXT( queue );
      }

      VULKAN_HPP_INLINE void vkQueueInsertDebugUtilsLabelEXT( VkQueue queue, VkDebugUtilsLabelEXT const * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueInsertDebugUtilsLabelEXT( queue, pLabelInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBeginDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT const * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndDebugUtilsLabelEXT( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndDebugUtilsLabelEXT( commandBuffer );
      }

      VULKAN_HPP_INLINE void vkCmdInsertDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT const * pLabelInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdInsertDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                                                 VkDebugUtilsMessengerCreateInfoEXT const * pCreateInfo,
                                                                 VkAllocationCallbacks const *              pAllocator,
                                                                 VkDebugUtilsMessengerEXT *                 pMessenger ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
      }

      VULKAN_HPP_INLINE void vkDestroyDebugUtilsMessengerEXT( VkInstance                    instance,
                                                              VkDebugUtilsMessengerEXT      messenger,
                                                              VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
      }

      VULKAN_HPP_INLINE void vkSubmitDebugUtilsMessageEXT( VkInstance                                   instance,
                                                           VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                           VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSubmitDebugUtilsMessageEXT( instance, messageSeverity, messageTypes, pCallbackData );
      }

#    if defined( VK_USE_PLATFORM_ANDROID_KHR )
      //=== VK_ANDROID_external_memory_android_hardware_buffer ===

      VULKAN_HPP_INLINE VkResult vkGetAndroidHardwareBufferPropertiesANDROID( VkDevice                                   device,
                                                                              const struct AHardwareBuffer *             buffer,
                                                                              VkAndroidHardwareBufferPropertiesANDROID * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAndroidHardwareBufferPropertiesANDROID( device, buffer, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetMemoryAndroidHardwareBufferANDROID( VkDevice                                            device,
                                                                          VkMemoryGetAndroidHardwareBufferInfoANDROID const * pInfo,
                                                                          struct AHardwareBuffer ** pBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryAndroidHardwareBufferANDROID( device, pInfo, pBuffer );
      }
#    endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#    if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_AMDX_shader_enqueue ===

      VULKAN_HPP_INLINE VkResult vkCreateExecutionGraphPipelinesAMDX( VkDevice                                       device,
                                                                      VkPipelineCache                                pipelineCache,
                                                                      uint32_t                                       createInfoCount,
                                                                      VkExecutionGraphPipelineCreateInfoAMDX const * pCreateInfos,
                                                                      VkAllocationCallbacks const *                  pAllocator,
                                                                      VkPipeline *                                   pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateExecutionGraphPipelinesAMDX( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE VkResult vkGetExecutionGraphPipelineScratchSizeAMDX( VkDevice                                  device,
                                                                             VkPipeline                                executionGraph,
                                                                             VkExecutionGraphPipelineScratchSizeAMDX * pSizeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetExecutionGraphPipelineScratchSizeAMDX( device, executionGraph, pSizeInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetExecutionGraphPipelineNodeIndexAMDX( VkDevice                                        device,
                                                                           VkPipeline                                      executionGraph,
                                                                           VkPipelineShaderStageNodeCreateInfoAMDX const * pNodeInfo,
                                                                           uint32_t * pNodeIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetExecutionGraphPipelineNodeIndexAMDX( device, executionGraph, pNodeInfo, pNodeIndex );
      }

      VULKAN_HPP_INLINE void vkCmdInitializeGraphScratchMemoryAMDX( VkCommandBuffer commandBuffer,
                                                                    VkPipeline      executionGraph,
                                                                    VkDeviceAddress scratch,
                                                                    VkDeviceSize    scratchSize ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdInitializeGraphScratchMemoryAMDX( commandBuffer, executionGraph, scratch, scratchSize );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchGraphAMDX( VkCommandBuffer                      commandBuffer,
                                                     VkDeviceAddress                      scratch,
                                                     VkDeviceSize                         scratchSize,
                                                     VkDispatchGraphCountInfoAMDX const * pCountInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchGraphAMDX( commandBuffer, scratch, scratchSize, pCountInfo );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchGraphIndirectAMDX( VkCommandBuffer                      commandBuffer,
                                                             VkDeviceAddress                      scratch,
                                                             VkDeviceSize                         scratchSize,
                                                             VkDispatchGraphCountInfoAMDX const * pCountInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchGraphIndirectAMDX( commandBuffer, scratch, scratchSize, pCountInfo );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchGraphIndirectCountAMDX( VkCommandBuffer commandBuffer,
                                                                  VkDeviceAddress scratch,
                                                                  VkDeviceSize    scratchSize,
                                                                  VkDeviceAddress countInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchGraphIndirectCountAMDX( commandBuffer, scratch, scratchSize, countInfo );
      }
#    endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_EXT_descriptor_heap ===

      VULKAN_HPP_INLINE VkResult vkWriteSamplerDescriptorsEXT( VkDevice                      device,
                                                               uint32_t                      samplerCount,
                                                               VkSamplerCreateInfo const *   pSamplers,
                                                               VkHostAddressRangeEXT const * pDescriptors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWriteSamplerDescriptorsEXT( device, samplerCount, pSamplers, pDescriptors );
      }

      VULKAN_HPP_INLINE VkResult vkWriteResourceDescriptorsEXT( VkDevice                            device,
                                                                uint32_t                            resourceCount,
                                                                VkResourceDescriptorInfoEXT const * pResources,
                                                                VkHostAddressRangeEXT const *       pDescriptors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWriteResourceDescriptorsEXT( device, resourceCount, pResources, pDescriptors );
      }

      VULKAN_HPP_INLINE void vkCmdBindSamplerHeapEXT( VkCommandBuffer commandBuffer, VkBindHeapInfoEXT const * pBindInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindSamplerHeapEXT( commandBuffer, pBindInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBindResourceHeapEXT( VkCommandBuffer commandBuffer, VkBindHeapInfoEXT const * pBindInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindResourceHeapEXT( commandBuffer, pBindInfo );
      }

      VULKAN_HPP_INLINE void vkCmdPushDataEXT( VkCommandBuffer commandBuffer, VkPushDataInfoEXT const * pPushDataInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDataEXT( commandBuffer, pPushDataInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetImageOpaqueCaptureDataEXT( VkDevice                device,
                                                                 uint32_t                imageCount,
                                                                 VkImage const *         pImages,
                                                                 VkHostAddressRangeEXT * pDatas ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageOpaqueCaptureDataEXT( device, imageCount, pImages, pDatas );
      }

      VULKAN_HPP_INLINE VkDeviceSize vkGetPhysicalDeviceDescriptorSizeEXT( VkPhysicalDevice physicalDevice,
                                                                           VkDescriptorType descriptorType ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDescriptorSizeEXT( physicalDevice, descriptorType );
      }

      VULKAN_HPP_INLINE VkResult vkRegisterCustomBorderColorEXT( VkDevice                                        device,
                                                                 VkSamplerCustomBorderColorCreateInfoEXT const * pBorderColor,
                                                                 VkBool32                                        requestIndex,
                                                                 uint32_t *                                      pIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkRegisterCustomBorderColorEXT( device, pBorderColor, requestIndex, pIndex );
      }

      VULKAN_HPP_INLINE void vkUnregisterCustomBorderColorEXT( VkDevice device, uint32_t index ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUnregisterCustomBorderColorEXT( device, index );
      }

      VULKAN_HPP_INLINE VkResult vkGetTensorOpaqueCaptureDataARM( VkDevice                device,
                                                                  uint32_t                tensorCount,
                                                                  VkTensorARM const *     pTensors,
                                                                  VkHostAddressRangeEXT * pDatas ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetTensorOpaqueCaptureDataARM( device, tensorCount, pTensors, pDatas );
      }

      //=== VK_EXT_sample_locations ===

      VULKAN_HPP_INLINE void vkCmdSetSampleLocationsEXT( VkCommandBuffer                  commandBuffer,
                                                         VkSampleLocationsInfoEXT const * pSampleLocationsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetSampleLocationsEXT( commandBuffer, pSampleLocationsInfo );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceMultisamplePropertiesEXT( VkPhysicalDevice             physicalDevice,
                                                                          VkSampleCountFlagBits        samples,
                                                                          VkMultisamplePropertiesEXT * pMultisampleProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceMultisamplePropertiesEXT( physicalDevice, samples, pMultisampleProperties );
      }

      //=== VK_KHR_get_memory_requirements2 ===

      VULKAN_HPP_INLINE void vkGetImageMemoryRequirements2KHR( VkDevice                               device,
                                                               VkImageMemoryRequirementsInfo2 const * pInfo,
                                                               VkMemoryRequirements2 *                pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageMemoryRequirements2KHR( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetBufferMemoryRequirements2KHR( VkDevice                                device,
                                                                VkBufferMemoryRequirementsInfo2 const * pInfo,
                                                                VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferMemoryRequirements2KHR( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetImageSparseMemoryRequirements2KHR( VkDevice                                     device,
                                                                     VkImageSparseMemoryRequirementsInfo2 const * pInfo,
                                                                     uint32_t *                                   pSparseMemoryRequirementCount,
                                                                     VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSparseMemoryRequirements2KHR( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
      }

      //=== VK_KHR_acceleration_structure ===

      VULKAN_HPP_INLINE VkResult vkCreateAccelerationStructureKHR( VkDevice                                     device,
                                                                   VkAccelerationStructureCreateInfoKHR const * pCreateInfo,
                                                                   VkAllocationCallbacks const *                pAllocator,
                                                                   VkAccelerationStructureKHR * pAccelerationStructure ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateAccelerationStructureKHR( device, pCreateInfo, pAllocator, pAccelerationStructure );
      }

      VULKAN_HPP_INLINE void vkDestroyAccelerationStructureKHR( VkDevice                      device,
                                                                VkAccelerationStructureKHR    accelerationStructure,
                                                                VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyAccelerationStructureKHR( device, accelerationStructure, pAllocator );
      }

      VULKAN_HPP_INLINE void
        vkCmdBuildAccelerationStructuresKHR( VkCommandBuffer                                          commandBuffer,
                                             uint32_t                                                 infoCount,
                                             VkAccelerationStructureBuildGeometryInfoKHR const *      pInfos,
                                             VkAccelerationStructureBuildRangeInfoKHR const * const * ppBuildRangeInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildAccelerationStructuresKHR( commandBuffer, infoCount, pInfos, ppBuildRangeInfos );
      }

      VULKAN_HPP_INLINE void vkCmdBuildAccelerationStructuresIndirectKHR( VkCommandBuffer                                     commandBuffer,
                                                                          uint32_t                                            infoCount,
                                                                          VkAccelerationStructureBuildGeometryInfoKHR const * pInfos,
                                                                          VkDeviceAddress const *                             pIndirectDeviceAddresses,
                                                                          uint32_t const *                                    pIndirectStrides,
                                                                          uint32_t const * const * ppMaxPrimitiveCounts ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildAccelerationStructuresIndirectKHR(
          commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts );
      }

      VULKAN_HPP_INLINE VkResult
        vkBuildAccelerationStructuresKHR( VkDevice                                                 device,
                                          VkDeferredOperationKHR                                   deferredOperation,
                                          uint32_t                                                 infoCount,
                                          VkAccelerationStructureBuildGeometryInfoKHR const *      pInfos,
                                          VkAccelerationStructureBuildRangeInfoKHR const * const * ppBuildRangeInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBuildAccelerationStructuresKHR( device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos );
      }

      VULKAN_HPP_INLINE VkResult vkCopyAccelerationStructureKHR( VkDevice                                   device,
                                                                 VkDeferredOperationKHR                     deferredOperation,
                                                                 VkCopyAccelerationStructureInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyAccelerationStructureKHR( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyAccelerationStructureToMemoryKHR( VkDevice                                           device,
                                                                         VkDeferredOperationKHR                             deferredOperation,
                                                                         VkCopyAccelerationStructureToMemoryInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyAccelerationStructureToMemoryKHR( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyMemoryToAccelerationStructureKHR( VkDevice                                           device,
                                                                         VkDeferredOperationKHR                             deferredOperation,
                                                                         VkCopyMemoryToAccelerationStructureInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMemoryToAccelerationStructureKHR( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkWriteAccelerationStructuresPropertiesKHR( VkDevice                           device,
                                                                             uint32_t                           accelerationStructureCount,
                                                                             VkAccelerationStructureKHR const * pAccelerationStructures,
                                                                             VkQueryType                        queryType,
                                                                             size_t                             dataSize,
                                                                             void *                             pData,
                                                                             size_t                             stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWriteAccelerationStructuresPropertiesKHR( device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride );
      }

      VULKAN_HPP_INLINE void vkCmdCopyAccelerationStructureKHR( VkCommandBuffer                            commandBuffer,
                                                                VkCopyAccelerationStructureInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyAccelerationStructureKHR( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyAccelerationStructureToMemoryKHR( VkCommandBuffer                                    commandBuffer,
                                                                        VkCopyAccelerationStructureToMemoryInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyAccelerationStructureToMemoryKHR( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyMemoryToAccelerationStructureKHR( VkCommandBuffer                                    commandBuffer,
                                                                        VkCopyMemoryToAccelerationStructureInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryToAccelerationStructureKHR( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE VkDeviceAddress
        vkGetAccelerationStructureDeviceAddressKHR( VkDevice device, VkAccelerationStructureDeviceAddressInfoKHR const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAccelerationStructureDeviceAddressKHR( device, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdWriteAccelerationStructuresPropertiesKHR( VkCommandBuffer                    commandBuffer,
                                                                            uint32_t                           accelerationStructureCount,
                                                                            VkAccelerationStructureKHR const * pAccelerationStructures,
                                                                            VkQueryType                        queryType,
                                                                            VkQueryPool                        queryPool,
                                                                            uint32_t                           firstQuery ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteAccelerationStructuresPropertiesKHR(
          commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery );
      }

      VULKAN_HPP_INLINE void
        vkGetDeviceAccelerationStructureCompatibilityKHR( VkDevice                                      device,
                                                          VkAccelerationStructureVersionInfoKHR const * pVersionInfo,
                                                          VkAccelerationStructureCompatibilityKHR *     pCompatibility ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceAccelerationStructureCompatibilityKHR( device, pVersionInfo, pCompatibility );
      }

      VULKAN_HPP_INLINE void vkGetAccelerationStructureBuildSizesKHR( VkDevice                                            device,
                                                                      VkAccelerationStructureBuildTypeKHR                 buildType,
                                                                      VkAccelerationStructureBuildGeometryInfoKHR const * pBuildInfo,
                                                                      uint32_t const *                                    pMaxPrimitiveCounts,
                                                                      VkAccelerationStructureBuildSizesInfoKHR *          pSizeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAccelerationStructureBuildSizesKHR( device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo );
      }

      //=== VK_KHR_ray_tracing_pipeline ===

      VULKAN_HPP_INLINE void vkCmdTraceRaysKHR( VkCommandBuffer                         commandBuffer,
                                                VkStridedDeviceAddressRegionKHR const * pRaygenShaderBindingTable,
                                                VkStridedDeviceAddressRegionKHR const * pMissShaderBindingTable,
                                                VkStridedDeviceAddressRegionKHR const * pHitShaderBindingTable,
                                                VkStridedDeviceAddressRegionKHR const * pCallableShaderBindingTable,
                                                uint32_t                                width,
                                                uint32_t                                height,
                                                uint32_t                                depth ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdTraceRaysKHR(
          commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth );
      }

      VULKAN_HPP_INLINE VkResult vkCreateRayTracingPipelinesKHR( VkDevice                                  device,
                                                                 VkDeferredOperationKHR                    deferredOperation,
                                                                 VkPipelineCache                           pipelineCache,
                                                                 uint32_t                                  createInfoCount,
                                                                 VkRayTracingPipelineCreateInfoKHR const * pCreateInfos,
                                                                 VkAllocationCallbacks const *             pAllocator,
                                                                 VkPipeline *                              pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateRayTracingPipelinesKHR( device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE VkResult vkGetRayTracingShaderGroupHandlesKHR(
        VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRayTracingShaderGroupHandlesKHR( device, pipeline, firstGroup, groupCount, dataSize, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(
        VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRayTracingCaptureReplayShaderGroupHandlesKHR( device, pipeline, firstGroup, groupCount, dataSize, pData );
      }

      VULKAN_HPP_INLINE void vkCmdTraceRaysIndirectKHR( VkCommandBuffer                         commandBuffer,
                                                        VkStridedDeviceAddressRegionKHR const * pRaygenShaderBindingTable,
                                                        VkStridedDeviceAddressRegionKHR const * pMissShaderBindingTable,
                                                        VkStridedDeviceAddressRegionKHR const * pHitShaderBindingTable,
                                                        VkStridedDeviceAddressRegionKHR const * pCallableShaderBindingTable,
                                                        VkDeviceAddress                         indirectDeviceAddress ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdTraceRaysIndirectKHR(
          commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress );
      }

      VULKAN_HPP_INLINE VkDeviceSize vkGetRayTracingShaderGroupStackSizeKHR( VkDevice               device,
                                                                             VkPipeline             pipeline,
                                                                             uint32_t               group,
                                                                             VkShaderGroupShaderKHR groupShader ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRayTracingShaderGroupStackSizeKHR( device, pipeline, group, groupShader );
      }

      VULKAN_HPP_INLINE void vkCmdSetRayTracingPipelineStackSizeKHR( VkCommandBuffer commandBuffer, uint32_t pipelineStackSize ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRayTracingPipelineStackSizeKHR( commandBuffer, pipelineStackSize );
      }

      //=== VK_KHR_sampler_ycbcr_conversion ===

      VULKAN_HPP_INLINE VkResult vkCreateSamplerYcbcrConversionKHR( VkDevice                                   device,
                                                                    VkSamplerYcbcrConversionCreateInfo const * pCreateInfo,
                                                                    VkAllocationCallbacks const *              pAllocator,
                                                                    VkSamplerYcbcrConversion *                 pYcbcrConversion ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSamplerYcbcrConversionKHR( device, pCreateInfo, pAllocator, pYcbcrConversion );
      }

      VULKAN_HPP_INLINE void vkDestroySamplerYcbcrConversionKHR( VkDevice                      device,
                                                                 VkSamplerYcbcrConversion      ycbcrConversion,
                                                                 VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroySamplerYcbcrConversionKHR( device, ycbcrConversion, pAllocator );
      }

      //=== VK_KHR_bind_memory2 ===

      VULKAN_HPP_INLINE VkResult vkBindBufferMemory2KHR( VkDevice                       device,
                                                         uint32_t                       bindInfoCount,
                                                         VkBindBufferMemoryInfo const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindBufferMemory2KHR( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE VkResult vkBindImageMemory2KHR( VkDevice                      device,
                                                        uint32_t                      bindInfoCount,
                                                        VkBindImageMemoryInfo const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindImageMemory2KHR( device, bindInfoCount, pBindInfos );
      }

      //=== VK_EXT_image_drm_format_modifier ===

      VULKAN_HPP_INLINE VkResult vkGetImageDrmFormatModifierPropertiesEXT( VkDevice                                device,
                                                                           VkImage                                 image,
                                                                           VkImageDrmFormatModifierPropertiesEXT * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageDrmFormatModifierPropertiesEXT( device, image, pProperties );
      }

      //=== VK_EXT_validation_cache ===

      VULKAN_HPP_INLINE VkResult vkCreateValidationCacheEXT( VkDevice                               device,
                                                             VkValidationCacheCreateInfoEXT const * pCreateInfo,
                                                             VkAllocationCallbacks const *          pAllocator,
                                                             VkValidationCacheEXT *                 pValidationCache ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateValidationCacheEXT( device, pCreateInfo, pAllocator, pValidationCache );
      }

      VULKAN_HPP_INLINE void
        vkDestroyValidationCacheEXT( VkDevice device, VkValidationCacheEXT validationCache, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyValidationCacheEXT( device, validationCache, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkMergeValidationCachesEXT( VkDevice                     device,
                                                             VkValidationCacheEXT         dstCache,
                                                             uint32_t                     srcCacheCount,
                                                             VkValidationCacheEXT const * pSrcCaches ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkMergeValidationCachesEXT( device, dstCache, srcCacheCount, pSrcCaches );
      }

      VULKAN_HPP_INLINE VkResult vkGetValidationCacheDataEXT( VkDevice             device,
                                                              VkValidationCacheEXT validationCache,
                                                              size_t *             pDataSize,
                                                              void *               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetValidationCacheDataEXT( device, validationCache, pDataSize, pData );
      }

      //=== VK_NV_shading_rate_image ===

      VULKAN_HPP_INLINE void
        vkCmdBindShadingRateImageNV( VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindShadingRateImageNV( commandBuffer, imageView, imageLayout );
      }

      VULKAN_HPP_INLINE void vkCmdSetViewportShadingRatePaletteNV( VkCommandBuffer                commandBuffer,
                                                                   uint32_t                       firstViewport,
                                                                   uint32_t                       viewportCount,
                                                                   VkShadingRatePaletteNV const * pShadingRatePalettes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportShadingRatePaletteNV( commandBuffer, firstViewport, viewportCount, pShadingRatePalettes );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoarseSampleOrderNV( VkCommandBuffer                     commandBuffer,
                                                          VkCoarseSampleOrderTypeNV           sampleOrderType,
                                                          uint32_t                            customSampleOrderCount,
                                                          VkCoarseSampleOrderCustomNV const * pCustomSampleOrders ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoarseSampleOrderNV( commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders );
      }

      //=== VK_NV_ray_tracing ===

      VULKAN_HPP_INLINE VkResult vkCreateAccelerationStructureNV( VkDevice                                    device,
                                                                  VkAccelerationStructureCreateInfoNV const * pCreateInfo,
                                                                  VkAllocationCallbacks const *               pAllocator,
                                                                  VkAccelerationStructureNV *                 pAccelerationStructure ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateAccelerationStructureNV( device, pCreateInfo, pAllocator, pAccelerationStructure );
      }

      VULKAN_HPP_INLINE void vkDestroyAccelerationStructureNV( VkDevice                      device,
                                                               VkAccelerationStructureNV     accelerationStructure,
                                                               VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyAccelerationStructureNV( device, accelerationStructure, pAllocator );
      }

      VULKAN_HPP_INLINE void vkGetAccelerationStructureMemoryRequirementsNV( VkDevice                                                device,
                                                                             VkAccelerationStructureMemoryRequirementsInfoNV const * pInfo,
                                                                             VkMemoryRequirements2KHR * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAccelerationStructureMemoryRequirementsNV( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE VkResult vkBindAccelerationStructureMemoryNV( VkDevice                                        device,
                                                                      uint32_t                                        bindInfoCount,
                                                                      VkBindAccelerationStructureMemoryInfoNV const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindAccelerationStructureMemoryNV( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE void vkCmdBuildAccelerationStructureNV( VkCommandBuffer                       commandBuffer,
                                                                VkAccelerationStructureInfoNV const * pInfo,
                                                                VkBuffer                              instanceData,
                                                                VkDeviceSize                          instanceOffset,
                                                                VkBool32                              update,
                                                                VkAccelerationStructureNV             dst,
                                                                VkAccelerationStructureNV             src,
                                                                VkBuffer                              scratch,
                                                                VkDeviceSize                          scratchOffset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildAccelerationStructureNV( commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset );
      }

      VULKAN_HPP_INLINE void vkCmdCopyAccelerationStructureNV( VkCommandBuffer                    commandBuffer,
                                                               VkAccelerationStructureNV          dst,
                                                               VkAccelerationStructureNV          src,
                                                               VkCopyAccelerationStructureModeKHR mode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyAccelerationStructureNV( commandBuffer, dst, src, mode );
      }

      VULKAN_HPP_INLINE void vkCmdTraceRaysNV( VkCommandBuffer commandBuffer,
                                               VkBuffer        raygenShaderBindingTableBuffer,
                                               VkDeviceSize    raygenShaderBindingOffset,
                                               VkBuffer        missShaderBindingTableBuffer,
                                               VkDeviceSize    missShaderBindingOffset,
                                               VkDeviceSize    missShaderBindingStride,
                                               VkBuffer        hitShaderBindingTableBuffer,
                                               VkDeviceSize    hitShaderBindingOffset,
                                               VkDeviceSize    hitShaderBindingStride,
                                               VkBuffer        callableShaderBindingTableBuffer,
                                               VkDeviceSize    callableShaderBindingOffset,
                                               VkDeviceSize    callableShaderBindingStride,
                                               uint32_t        width,
                                               uint32_t        height,
                                               uint32_t        depth ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdTraceRaysNV( commandBuffer,
                                   raygenShaderBindingTableBuffer,
                                   raygenShaderBindingOffset,
                                   missShaderBindingTableBuffer,
                                   missShaderBindingOffset,
                                   missShaderBindingStride,
                                   hitShaderBindingTableBuffer,
                                   hitShaderBindingOffset,
                                   hitShaderBindingStride,
                                   callableShaderBindingTableBuffer,
                                   callableShaderBindingOffset,
                                   callableShaderBindingStride,
                                   width,
                                   height,
                                   depth );
      }

      VULKAN_HPP_INLINE VkResult vkCreateRayTracingPipelinesNV( VkDevice                                 device,
                                                                VkPipelineCache                          pipelineCache,
                                                                uint32_t                                 createInfoCount,
                                                                VkRayTracingPipelineCreateInfoNV const * pCreateInfos,
                                                                VkAllocationCallbacks const *            pAllocator,
                                                                VkPipeline *                             pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateRayTracingPipelinesNV( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE VkResult vkGetRayTracingShaderGroupHandlesNV(
        VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRayTracingShaderGroupHandlesNV( device, pipeline, firstGroup, groupCount, dataSize, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetAccelerationStructureHandleNV( VkDevice                  device,
                                                                     VkAccelerationStructureNV accelerationStructure,
                                                                     size_t                    dataSize,
                                                                     void *                    pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAccelerationStructureHandleNV( device, accelerationStructure, dataSize, pData );
      }

      VULKAN_HPP_INLINE void vkCmdWriteAccelerationStructuresPropertiesNV( VkCommandBuffer                   commandBuffer,
                                                                           uint32_t                          accelerationStructureCount,
                                                                           VkAccelerationStructureNV const * pAccelerationStructures,
                                                                           VkQueryType                       queryType,
                                                                           VkQueryPool                       queryPool,
                                                                           uint32_t                          firstQuery ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteAccelerationStructuresPropertiesNV(
          commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery );
      }

      VULKAN_HPP_INLINE VkResult vkCompileDeferredNV( VkDevice device, VkPipeline pipeline, uint32_t shader ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCompileDeferredNV( device, pipeline, shader );
      }

      //=== VK_KHR_maintenance3 ===

      VULKAN_HPP_INLINE void vkGetDescriptorSetLayoutSupportKHR( VkDevice                                device,
                                                                 VkDescriptorSetLayoutCreateInfo const * pCreateInfo,
                                                                 VkDescriptorSetLayoutSupport *          pSupport ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetLayoutSupportKHR( device, pCreateInfo, pSupport );
      }

      //=== VK_KHR_draw_indirect_count ===

      VULKAN_HPP_INLINE void vkCmdDrawIndirectCountKHR( VkCommandBuffer commandBuffer,
                                                        VkBuffer        buffer,
                                                        VkDeviceSize    offset,
                                                        VkBuffer        countBuffer,
                                                        VkDeviceSize    countBufferOffset,
                                                        uint32_t        maxDrawCount,
                                                        uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndirectCountKHR( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawIndexedIndirectCountKHR( VkCommandBuffer commandBuffer,
                                                               VkBuffer        buffer,
                                                               VkDeviceSize    offset,
                                                               VkBuffer        countBuffer,
                                                               VkDeviceSize    countBufferOffset,
                                                               uint32_t        maxDrawCount,
                                                               uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawIndexedIndirectCountKHR( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      //=== VK_EXT_external_memory_host ===

      VULKAN_HPP_INLINE VkResult
        vkGetMemoryHostPointerPropertiesEXT( VkDevice                           device,
                                             VkExternalMemoryHandleTypeFlagBits handleType,
                                             void const *                       pHostPointer,
                                             VkMemoryHostPointerPropertiesEXT * pMemoryHostPointerProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryHostPointerPropertiesEXT( device, handleType, pHostPointer, pMemoryHostPointerProperties );
      }

      //=== VK_AMD_buffer_marker ===

      VULKAN_HPP_INLINE void vkCmdWriteBufferMarkerAMD( VkCommandBuffer         commandBuffer,
                                                        VkPipelineStageFlagBits pipelineStage,
                                                        VkBuffer                dstBuffer,
                                                        VkDeviceSize            dstOffset,
                                                        uint32_t                marker ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteBufferMarkerAMD( commandBuffer, pipelineStage, dstBuffer, dstOffset, marker );
      }

      VULKAN_HPP_INLINE void vkCmdWriteBufferMarker2AMD(
        VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteBufferMarker2AMD( commandBuffer, stage, dstBuffer, dstOffset, marker );
      }

      //=== VK_EXT_calibrated_timestamps ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( VkPhysicalDevice  physicalDevice,
                                                                                 uint32_t *        pTimeDomainCount,
                                                                                 VkTimeDomainKHR * pTimeDomains ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( physicalDevice, pTimeDomainCount, pTimeDomains );
      }

      VULKAN_HPP_INLINE VkResult vkGetCalibratedTimestampsEXT( VkDevice                             device,
                                                               uint32_t                             timestampCount,
                                                               VkCalibratedTimestampInfoKHR const * pTimestampInfos,
                                                               uint64_t *                           pTimestamps,
                                                               uint64_t *                           pMaxDeviation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetCalibratedTimestampsEXT( device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation );
      }

      //=== VK_NV_mesh_shader ===

      VULKAN_HPP_INLINE void vkCmdDrawMeshTasksNV( VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksNV( commandBuffer, taskCount, firstTask );
      }

      VULKAN_HPP_INLINE void vkCmdDrawMeshTasksIndirectNV(
        VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksIndirectNV( commandBuffer, buffer, offset, drawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawMeshTasksIndirectCountNV( VkCommandBuffer commandBuffer,
                                                                VkBuffer        buffer,
                                                                VkDeviceSize    offset,
                                                                VkBuffer        countBuffer,
                                                                VkDeviceSize    countBufferOffset,
                                                                uint32_t        maxDrawCount,
                                                                uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksIndirectCountNV( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      //=== VK_NV_scissor_exclusive ===

      VULKAN_HPP_INLINE void vkCmdSetExclusiveScissorEnableNV( VkCommandBuffer  commandBuffer,
                                                               uint32_t         firstExclusiveScissor,
                                                               uint32_t         exclusiveScissorCount,
                                                               VkBool32 const * pExclusiveScissorEnables ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetExclusiveScissorEnableNV( commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables );
      }

      VULKAN_HPP_INLINE void vkCmdSetExclusiveScissorNV( VkCommandBuffer  commandBuffer,
                                                         uint32_t         firstExclusiveScissor,
                                                         uint32_t         exclusiveScissorCount,
                                                         VkRect2D const * pExclusiveScissors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetExclusiveScissorNV( commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors );
      }

      //=== VK_NV_device_diagnostic_checkpoints ===

      VULKAN_HPP_INLINE void vkCmdSetCheckpointNV( VkCommandBuffer commandBuffer, void const * pCheckpointMarker ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCheckpointNV( commandBuffer, pCheckpointMarker );
      }

      VULKAN_HPP_INLINE void
        vkGetQueueCheckpointDataNV( VkQueue queue, uint32_t * pCheckpointDataCount, VkCheckpointDataNV * pCheckpointData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetQueueCheckpointDataNV( queue, pCheckpointDataCount, pCheckpointData );
      }

      VULKAN_HPP_INLINE void
        vkGetQueueCheckpointData2NV( VkQueue queue, uint32_t * pCheckpointDataCount, VkCheckpointData2NV * pCheckpointData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetQueueCheckpointData2NV( queue, pCheckpointDataCount, pCheckpointData );
      }

      //=== VK_KHR_timeline_semaphore ===

      VULKAN_HPP_INLINE VkResult vkGetSemaphoreCounterValueKHR( VkDevice device, VkSemaphore semaphore, uint64_t * pValue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSemaphoreCounterValueKHR( device, semaphore, pValue );
      }

      VULKAN_HPP_INLINE VkResult vkWaitSemaphoresKHR( VkDevice device, VkSemaphoreWaitInfo const * pWaitInfo, uint64_t timeout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWaitSemaphoresKHR( device, pWaitInfo, timeout );
      }

      VULKAN_HPP_INLINE VkResult vkSignalSemaphoreKHR( VkDevice device, VkSemaphoreSignalInfo const * pSignalInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSignalSemaphoreKHR( device, pSignalInfo );
      }

      //=== VK_EXT_present_timing ===

      VULKAN_HPP_INLINE VkResult vkSetSwapchainPresentTimingQueueSizeEXT( VkDevice device, VkSwapchainKHR swapchain, uint32_t size ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetSwapchainPresentTimingQueueSizeEXT( device, swapchain, size );
      }

      VULKAN_HPP_INLINE VkResult vkGetSwapchainTimingPropertiesEXT( VkDevice                         device,
                                                                    VkSwapchainKHR                   swapchain,
                                                                    VkSwapchainTimingPropertiesEXT * pSwapchainTimingProperties,
                                                                    uint64_t * pSwapchainTimingPropertiesCounter ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSwapchainTimingPropertiesEXT( device, swapchain, pSwapchainTimingProperties, pSwapchainTimingPropertiesCounter );
      }

      VULKAN_HPP_INLINE VkResult vkGetSwapchainTimeDomainPropertiesEXT( VkDevice                             device,
                                                                        VkSwapchainKHR                       swapchain,
                                                                        VkSwapchainTimeDomainPropertiesEXT * pSwapchainTimeDomainProperties,
                                                                        uint64_t *                           pTimeDomainsCounter ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSwapchainTimeDomainPropertiesEXT( device, swapchain, pSwapchainTimeDomainProperties, pTimeDomainsCounter );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetPastPresentationTimingEXT( VkDevice                                device,
                                        VkPastPresentationTimingInfoEXT const * pPastPresentationTimingInfo,
                                        VkPastPresentationTimingPropertiesEXT * pPastPresentationTimingProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPastPresentationTimingEXT( device, pPastPresentationTimingInfo, pPastPresentationTimingProperties );
      }

      //=== VK_INTEL_performance_query ===

      VULKAN_HPP_INLINE VkResult vkInitializePerformanceApiINTEL( VkDevice                                    device,
                                                                  VkInitializePerformanceApiInfoINTEL const * pInitializeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkInitializePerformanceApiINTEL( device, pInitializeInfo );
      }

      VULKAN_HPP_INLINE void vkUninitializePerformanceApiINTEL( VkDevice device ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUninitializePerformanceApiINTEL( device );
      }

      VULKAN_HPP_INLINE VkResult vkCmdSetPerformanceMarkerINTEL( VkCommandBuffer                      commandBuffer,
                                                                 VkPerformanceMarkerInfoINTEL const * pMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPerformanceMarkerINTEL( commandBuffer, pMarkerInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCmdSetPerformanceStreamMarkerINTEL( VkCommandBuffer                            commandBuffer,
                                                                       VkPerformanceStreamMarkerInfoINTEL const * pMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPerformanceStreamMarkerINTEL( commandBuffer, pMarkerInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCmdSetPerformanceOverrideINTEL( VkCommandBuffer                        commandBuffer,
                                                                   VkPerformanceOverrideInfoINTEL const * pOverrideInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPerformanceOverrideINTEL( commandBuffer, pOverrideInfo );
      }

      VULKAN_HPP_INLINE VkResult vkAcquirePerformanceConfigurationINTEL( VkDevice                                           device,
                                                                         VkPerformanceConfigurationAcquireInfoINTEL const * pAcquireInfo,
                                                                         VkPerformanceConfigurationINTEL * pConfiguration ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquirePerformanceConfigurationINTEL( device, pAcquireInfo, pConfiguration );
      }

      VULKAN_HPP_INLINE VkResult vkReleasePerformanceConfigurationINTEL( VkDevice                        device,
                                                                         VkPerformanceConfigurationINTEL configuration ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleasePerformanceConfigurationINTEL( device, configuration );
      }

      VULKAN_HPP_INLINE VkResult vkQueueSetPerformanceConfigurationINTEL( VkQueue                         queue,
                                                                          VkPerformanceConfigurationINTEL configuration ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueSetPerformanceConfigurationINTEL( queue, configuration );
      }

      VULKAN_HPP_INLINE VkResult vkGetPerformanceParameterINTEL( VkDevice                        device,
                                                                 VkPerformanceParameterTypeINTEL parameter,
                                                                 VkPerformanceValueINTEL *       pValue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPerformanceParameterINTEL( device, parameter, pValue );
      }

      //=== VK_AMD_display_native_hdr ===

      VULKAN_HPP_INLINE void vkSetLocalDimmingAMD( VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetLocalDimmingAMD( device, swapChain, localDimmingEnable );
      }

#    if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_imagepipe_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateImagePipeSurfaceFUCHSIA( VkInstance                                  instance,
                                                                  VkImagePipeSurfaceCreateInfoFUCHSIA const * pCreateInfo,
                                                                  VkAllocationCallbacks const *               pAllocator,
                                                                  VkSurfaceKHR *                              pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateImagePipeSurfaceFUCHSIA( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_FUCHSIA*/

#    if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_metal_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateMetalSurfaceEXT( VkInstance                          instance,
                                                          VkMetalSurfaceCreateInfoEXT const * pCreateInfo,
                                                          VkAllocationCallbacks const *       pAllocator,
                                                          VkSurfaceKHR *                      pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateMetalSurfaceEXT( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_KHR_fragment_shading_rate ===

      VULKAN_HPP_INLINE VkResult
        vkGetPhysicalDeviceFragmentShadingRatesKHR( VkPhysicalDevice                         physicalDevice,
                                                    uint32_t *                               pFragmentShadingRateCount,
                                                    VkPhysicalDeviceFragmentShadingRateKHR * pFragmentShadingRates ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceFragmentShadingRatesKHR( physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates );
      }

      VULKAN_HPP_INLINE void vkCmdSetFragmentShadingRateKHR( VkCommandBuffer                          commandBuffer,
                                                             VkExtent2D const *                       pFragmentSize,
                                                             VkFragmentShadingRateCombinerOpKHR const combinerOps[2] ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetFragmentShadingRateKHR( commandBuffer, pFragmentSize, combinerOps );
      }

      //=== VK_KHR_dynamic_rendering_local_read ===

      VULKAN_HPP_INLINE void vkCmdSetRenderingAttachmentLocationsKHR( VkCommandBuffer                           commandBuffer,
                                                                      VkRenderingAttachmentLocationInfo const * pLocationInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRenderingAttachmentLocationsKHR( commandBuffer, pLocationInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetRenderingInputAttachmentIndicesKHR( VkCommandBuffer                             commandBuffer,
                                                    VkRenderingInputAttachmentIndexInfo const * pInputAttachmentIndexInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRenderingInputAttachmentIndicesKHR( commandBuffer, pInputAttachmentIndexInfo );
      }

      //=== VK_EXT_buffer_device_address ===

      VULKAN_HPP_INLINE VkDeviceAddress vkGetBufferDeviceAddressEXT( VkDevice device, VkBufferDeviceAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferDeviceAddressEXT( device, pInfo );
      }

      //=== VK_EXT_tooling_info ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceToolPropertiesEXT( VkPhysicalDevice                 physicalDevice,
                                                                       uint32_t *                       pToolCount,
                                                                       VkPhysicalDeviceToolProperties * pToolProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceToolPropertiesEXT( physicalDevice, pToolCount, pToolProperties );
      }

      //=== VK_KHR_present_wait ===

      VULKAN_HPP_INLINE VkResult vkWaitForPresentKHR( VkDevice       device,
                                                      VkSwapchainKHR swapchain,
                                                      uint64_t       presentId,
                                                      uint64_t       timeout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWaitForPresentKHR( device, swapchain, presentId, timeout );
      }

      //=== VK_NV_cooperative_matrix ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCooperativeMatrixPropertiesNV( VkPhysicalDevice                  physicalDevice,
                                                                                   uint32_t *                        pPropertyCount,
                                                                                   VkCooperativeMatrixPropertiesNV * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCooperativeMatrixPropertiesNV( physicalDevice, pPropertyCount, pProperties );
      }

      //=== VK_NV_coverage_reduction_mode ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(
        VkPhysicalDevice physicalDevice, uint32_t * pCombinationCount, VkFramebufferMixedSamplesCombinationNV * pCombinations ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV( physicalDevice, pCombinationCount, pCombinations );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_EXT_full_screen_exclusive ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceSurfacePresentModes2EXT( VkPhysicalDevice                        physicalDevice,
                                                                             VkPhysicalDeviceSurfaceInfo2KHR const * pSurfaceInfo,
                                                                             uint32_t *                              pPresentModeCount,
                                                                             VkPresentModeKHR *                      pPresentModes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceSurfacePresentModes2EXT( physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes );
      }

      VULKAN_HPP_INLINE VkResult vkAcquireFullScreenExclusiveModeEXT( VkDevice device, VkSwapchainKHR swapchain ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireFullScreenExclusiveModeEXT( device, swapchain );
      }

      VULKAN_HPP_INLINE VkResult vkReleaseFullScreenExclusiveModeEXT( VkDevice device, VkSwapchainKHR swapchain ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseFullScreenExclusiveModeEXT( device, swapchain );
      }

      VULKAN_HPP_INLINE VkResult vkGetDeviceGroupSurfacePresentModes2EXT( VkDevice                                device,
                                                                          VkPhysicalDeviceSurfaceInfo2KHR const * pSurfaceInfo,
                                                                          VkDeviceGroupPresentModeFlagsKHR *      pModes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceGroupSurfacePresentModes2EXT( device, pSurfaceInfo, pModes );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_EXT_headless_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateHeadlessSurfaceEXT( VkInstance                             instance,
                                                             VkHeadlessSurfaceCreateInfoEXT const * pCreateInfo,
                                                             VkAllocationCallbacks const *          pAllocator,
                                                             VkSurfaceKHR *                         pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateHeadlessSurfaceEXT( instance, pCreateInfo, pAllocator, pSurface );
      }

      //=== VK_KHR_buffer_device_address ===

      VULKAN_HPP_INLINE VkDeviceAddress vkGetBufferDeviceAddressKHR( VkDevice device, VkBufferDeviceAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferDeviceAddressKHR( device, pInfo );
      }

      VULKAN_HPP_INLINE uint64_t vkGetBufferOpaqueCaptureAddressKHR( VkDevice device, VkBufferDeviceAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferOpaqueCaptureAddressKHR( device, pInfo );
      }

      VULKAN_HPP_INLINE uint64_t vkGetDeviceMemoryOpaqueCaptureAddressKHR( VkDevice                                       device,
                                                                           VkDeviceMemoryOpaqueCaptureAddressInfo const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceMemoryOpaqueCaptureAddressKHR( device, pInfo );
      }

      //=== VK_EXT_line_rasterization ===

      VULKAN_HPP_INLINE void
        vkCmdSetLineStippleEXT( VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineStippleEXT( commandBuffer, lineStippleFactor, lineStipplePattern );
      }

      //=== VK_EXT_host_query_reset ===

      VULKAN_HPP_INLINE void vkResetQueryPoolEXT( VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkResetQueryPoolEXT( device, queryPool, firstQuery, queryCount );
      }

      //=== VK_EXT_extended_dynamic_state ===

      VULKAN_HPP_INLINE void vkCmdSetCullModeEXT( VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCullModeEXT( commandBuffer, cullMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetFrontFaceEXT( VkCommandBuffer commandBuffer, VkFrontFace frontFace ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetFrontFaceEXT( commandBuffer, frontFace );
      }

      VULKAN_HPP_INLINE void vkCmdSetPrimitiveTopologyEXT( VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPrimitiveTopologyEXT( commandBuffer, primitiveTopology );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetViewportWithCountEXT( VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport const * pViewports ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportWithCountEXT( commandBuffer, viewportCount, pViewports );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetScissorWithCountEXT( VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D const * pScissors ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetScissorWithCountEXT( commandBuffer, scissorCount, pScissors );
      }

      VULKAN_HPP_INLINE void vkCmdBindVertexBuffers2EXT( VkCommandBuffer      commandBuffer,
                                                         uint32_t             firstBinding,
                                                         uint32_t             bindingCount,
                                                         VkBuffer const *     pBuffers,
                                                         VkDeviceSize const * pOffsets,
                                                         VkDeviceSize const * pSizes,
                                                         VkDeviceSize const * pStrides ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindVertexBuffers2EXT( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthTestEnableEXT( commandBuffer, depthTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthWriteEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthWriteEnableEXT( commandBuffer, depthWriteEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthCompareOpEXT( VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthCompareOpEXT( commandBuffer, depthCompareOp );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBoundsTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBoundsTestEnableEXT( commandBuffer, depthBoundsTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetStencilTestEnableEXT( VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilTestEnableEXT( commandBuffer, stencilTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetStencilOpEXT( VkCommandBuffer    commandBuffer,
                                                   VkStencilFaceFlags faceMask,
                                                   VkStencilOp        failOp,
                                                   VkStencilOp        passOp,
                                                   VkStencilOp        depthFailOp,
                                                   VkCompareOp        compareOp ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetStencilOpEXT( commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp );
      }

      //=== VK_KHR_deferred_host_operations ===

      VULKAN_HPP_INLINE VkResult vkCreateDeferredOperationKHR( VkDevice                      device,
                                                               VkAllocationCallbacks const * pAllocator,
                                                               VkDeferredOperationKHR *      pDeferredOperation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDeferredOperationKHR( device, pAllocator, pDeferredOperation );
      }

      VULKAN_HPP_INLINE void
        vkDestroyDeferredOperationKHR( VkDevice device, VkDeferredOperationKHR operation, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDeferredOperationKHR( device, operation, pAllocator );
      }

      VULKAN_HPP_INLINE uint32_t vkGetDeferredOperationMaxConcurrencyKHR( VkDevice device, VkDeferredOperationKHR operation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeferredOperationMaxConcurrencyKHR( device, operation );
      }

      VULKAN_HPP_INLINE VkResult vkGetDeferredOperationResultKHR( VkDevice device, VkDeferredOperationKHR operation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeferredOperationResultKHR( device, operation );
      }

      VULKAN_HPP_INLINE VkResult vkDeferredOperationJoinKHR( VkDevice device, VkDeferredOperationKHR operation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDeferredOperationJoinKHR( device, operation );
      }

      //=== VK_KHR_pipeline_executable_properties ===

      VULKAN_HPP_INLINE VkResult vkGetPipelineExecutablePropertiesKHR( VkDevice                            device,
                                                                       VkPipelineInfoKHR const *           pPipelineInfo,
                                                                       uint32_t *                          pExecutableCount,
                                                                       VkPipelineExecutablePropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineExecutablePropertiesKHR( device, pPipelineInfo, pExecutableCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetPipelineExecutableStatisticsKHR( VkDevice                            device,
                                                                       VkPipelineExecutableInfoKHR const * pExecutableInfo,
                                                                       uint32_t *                          pStatisticCount,
                                                                       VkPipelineExecutableStatisticKHR *  pStatistics ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineExecutableStatisticsKHR( device, pExecutableInfo, pStatisticCount, pStatistics );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetPipelineExecutableInternalRepresentationsKHR( VkDevice                                        device,
                                                           VkPipelineExecutableInfoKHR const *             pExecutableInfo,
                                                           uint32_t *                                      pInternalRepresentationCount,
                                                           VkPipelineExecutableInternalRepresentationKHR * pInternalRepresentations ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineExecutableInternalRepresentationsKHR( device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations );
      }

      //=== VK_EXT_host_image_copy ===

      VULKAN_HPP_INLINE VkResult vkCopyMemoryToImageEXT( VkDevice device, VkCopyMemoryToImageInfo const * pCopyMemoryToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMemoryToImageEXT( device, pCopyMemoryToImageInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyImageToMemoryEXT( VkDevice device, VkCopyImageToMemoryInfo const * pCopyImageToMemoryInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyImageToMemoryEXT( device, pCopyImageToMemoryInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyImageToImageEXT( VkDevice device, VkCopyImageToImageInfo const * pCopyImageToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyImageToImageEXT( device, pCopyImageToImageInfo );
      }

      VULKAN_HPP_INLINE VkResult vkTransitionImageLayoutEXT( VkDevice                                device,
                                                             uint32_t                                transitionCount,
                                                             VkHostImageLayoutTransitionInfo const * pTransitions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkTransitionImageLayoutEXT( device, transitionCount, pTransitions );
      }

      VULKAN_HPP_INLINE void vkGetImageSubresourceLayout2EXT( VkDevice                    device,
                                                              VkImage                     image,
                                                              VkImageSubresource2 const * pSubresource,
                                                              VkSubresourceLayout2 *      pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSubresourceLayout2EXT( device, image, pSubresource, pLayout );
      }

      //=== VK_KHR_map_memory2 ===

      VULKAN_HPP_INLINE VkResult vkMapMemory2KHR( VkDevice device, VkMemoryMapInfo const * pMemoryMapInfo, void ** ppData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkMapMemory2KHR( device, pMemoryMapInfo, ppData );
      }

      VULKAN_HPP_INLINE VkResult vkUnmapMemory2KHR( VkDevice device, VkMemoryUnmapInfo const * pMemoryUnmapInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUnmapMemory2KHR( device, pMemoryUnmapInfo );
      }

      //=== VK_EXT_swapchain_maintenance1 ===

      VULKAN_HPP_INLINE VkResult vkReleaseSwapchainImagesEXT( VkDevice device, VkReleaseSwapchainImagesInfoKHR const * pReleaseInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseSwapchainImagesEXT( device, pReleaseInfo );
      }

      //=== VK_NV_device_generated_commands ===

      VULKAN_HPP_INLINE void vkGetGeneratedCommandsMemoryRequirementsNV( VkDevice                                            device,
                                                                         VkGeneratedCommandsMemoryRequirementsInfoNV const * pInfo,
                                                                         VkMemoryRequirements2 * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetGeneratedCommandsMemoryRequirementsNV( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkCmdPreprocessGeneratedCommandsNV( VkCommandBuffer                   commandBuffer,
                                                                 VkGeneratedCommandsInfoNV const * pGeneratedCommandsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPreprocessGeneratedCommandsNV( commandBuffer, pGeneratedCommandsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdExecuteGeneratedCommandsNV( VkCommandBuffer                   commandBuffer,
                                                              VkBool32                          isPreprocessed,
                                                              VkGeneratedCommandsInfoNV const * pGeneratedCommandsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdExecuteGeneratedCommandsNV( commandBuffer, isPreprocessed, pGeneratedCommandsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBindPipelineShaderGroupNV( VkCommandBuffer     commandBuffer,
                                                             VkPipelineBindPoint pipelineBindPoint,
                                                             VkPipeline          pipeline,
                                                             uint32_t            groupIndex ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindPipelineShaderGroupNV( commandBuffer, pipelineBindPoint, pipeline, groupIndex );
      }

      VULKAN_HPP_INLINE VkResult vkCreateIndirectCommandsLayoutNV( VkDevice                                     device,
                                                                   VkIndirectCommandsLayoutCreateInfoNV const * pCreateInfo,
                                                                   VkAllocationCallbacks const *                pAllocator,
                                                                   VkIndirectCommandsLayoutNV * pIndirectCommandsLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateIndirectCommandsLayoutNV( device, pCreateInfo, pAllocator, pIndirectCommandsLayout );
      }

      VULKAN_HPP_INLINE void vkDestroyIndirectCommandsLayoutNV( VkDevice                      device,
                                                                VkIndirectCommandsLayoutNV    indirectCommandsLayout,
                                                                VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyIndirectCommandsLayoutNV( device, indirectCommandsLayout, pAllocator );
      }

      //=== VK_EXT_depth_bias_control ===

      VULKAN_HPP_INLINE void vkCmdSetDepthBias2EXT( VkCommandBuffer commandBuffer, VkDepthBiasInfoEXT const * pDepthBiasInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBias2EXT( commandBuffer, pDepthBiasInfo );
      }

      //=== VK_EXT_acquire_drm_display ===

      VULKAN_HPP_INLINE VkResult vkAcquireDrmDisplayEXT( VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireDrmDisplayEXT( physicalDevice, drmFd, display );
      }

      VULKAN_HPP_INLINE VkResult vkGetDrmDisplayEXT( VkPhysicalDevice physicalDevice,
                                                     int32_t          drmFd,
                                                     uint32_t         connectorId,
                                                     VkDisplayKHR *   display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDrmDisplayEXT( physicalDevice, drmFd, connectorId, display );
      }

      //=== VK_EXT_private_data ===

      VULKAN_HPP_INLINE VkResult vkCreatePrivateDataSlotEXT( VkDevice                            device,
                                                             VkPrivateDataSlotCreateInfo const * pCreateInfo,
                                                             VkAllocationCallbacks const *       pAllocator,
                                                             VkPrivateDataSlot *                 pPrivateDataSlot ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreatePrivateDataSlotEXT( device, pCreateInfo, pAllocator, pPrivateDataSlot );
      }

      VULKAN_HPP_INLINE void
        vkDestroyPrivateDataSlotEXT( VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPrivateDataSlotEXT( device, privateDataSlot, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkSetPrivateDataEXT(
        VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetPrivateDataEXT( device, objectType, objectHandle, privateDataSlot, data );
      }

      VULKAN_HPP_INLINE void vkGetPrivateDataEXT(
        VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPrivateDataEXT( device, objectType, objectHandle, privateDataSlot, pData );
      }

      //=== VK_KHR_video_encode_queue ===

      VULKAN_HPP_INLINE VkResult
        vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR( VkPhysicalDevice                                       physicalDevice,
                                                                 VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR const * pQualityLevelInfo,
                                                                 VkVideoEncodeQualityLevelPropertiesKHR * pQualityLevelProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR( physicalDevice, pQualityLevelInfo, pQualityLevelProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetEncodedVideoSessionParametersKHR( VkDevice                                         device,
                                                                        VkVideoEncodeSessionParametersGetInfoKHR const * pVideoSessionParametersInfo,
                                                                        VkVideoEncodeSessionParametersFeedbackInfoKHR *  pFeedbackInfo,
                                                                        size_t *                                         pDataSize,
                                                                        void *                                           pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetEncodedVideoSessionParametersKHR( device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData );
      }

      VULKAN_HPP_INLINE void vkCmdEncodeVideoKHR( VkCommandBuffer commandBuffer, VkVideoEncodeInfoKHR const * pEncodeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEncodeVideoKHR( commandBuffer, pEncodeInfo );
      }

#    if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_NV_cuda_kernel_launch ===

      VULKAN_HPP_INLINE VkResult vkCreateCudaModuleNV( VkDevice                         device,
                                                       VkCudaModuleCreateInfoNV const * pCreateInfo,
                                                       VkAllocationCallbacks const *    pAllocator,
                                                       VkCudaModuleNV *                 pModule ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateCudaModuleNV( device, pCreateInfo, pAllocator, pModule );
      }

      VULKAN_HPP_INLINE VkResult vkGetCudaModuleCacheNV( VkDevice       device,
                                                         VkCudaModuleNV module,
                                                         size_t *       pCacheSize,
                                                         void *         pCacheData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetCudaModuleCacheNV( device, module, pCacheSize, pCacheData );
      }

      VULKAN_HPP_INLINE VkResult vkCreateCudaFunctionNV( VkDevice                           device,
                                                         VkCudaFunctionCreateInfoNV const * pCreateInfo,
                                                         VkAllocationCallbacks const *      pAllocator,
                                                         VkCudaFunctionNV *                 pFunction ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateCudaFunctionNV( device, pCreateInfo, pAllocator, pFunction );
      }

      VULKAN_HPP_INLINE void vkDestroyCudaModuleNV( VkDevice device, VkCudaModuleNV module, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyCudaModuleNV( device, module, pAllocator );
      }

      VULKAN_HPP_INLINE void
        vkDestroyCudaFunctionNV( VkDevice device, VkCudaFunctionNV function, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyCudaFunctionNV( device, function, pAllocator );
      }

      VULKAN_HPP_INLINE void vkCmdCudaLaunchKernelNV( VkCommandBuffer commandBuffer, VkCudaLaunchInfoNV const * pLaunchInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCudaLaunchKernelNV( commandBuffer, pLaunchInfo );
      }
#    endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_QCOM_tile_shading ===

      VULKAN_HPP_INLINE void vkCmdDispatchTileQCOM( VkCommandBuffer commandBuffer, VkDispatchTileInfoQCOM const * pDispatchTileInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchTileQCOM( commandBuffer, pDispatchTileInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBeginPerTileExecutionQCOM( VkCommandBuffer                commandBuffer,
                                                             VkPerTileBeginInfoQCOM const * pPerTileBeginInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginPerTileExecutionQCOM( commandBuffer, pPerTileBeginInfo );
      }

      VULKAN_HPP_INLINE void vkCmdEndPerTileExecutionQCOM( VkCommandBuffer              commandBuffer,
                                                           VkPerTileEndInfoQCOM const * pPerTileEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndPerTileExecutionQCOM( commandBuffer, pPerTileEndInfo );
      }

#    if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_metal_objects ===

      VULKAN_HPP_INLINE void vkExportMetalObjectsEXT( VkDevice device, VkExportMetalObjectsInfoEXT * pMetalObjectsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkExportMetalObjectsEXT( device, pMetalObjectsInfo );
      }
#    endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_KHR_synchronization2 ===

      VULKAN_HPP_INLINE void
        vkCmdSetEvent2KHR( VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo const * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetEvent2KHR( commandBuffer, event, pDependencyInfo );
      }

      VULKAN_HPP_INLINE void vkCmdResetEvent2KHR( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResetEvent2KHR( commandBuffer, event, stageMask );
      }

      VULKAN_HPP_INLINE void vkCmdWaitEvents2KHR( VkCommandBuffer          commandBuffer,
                                                  uint32_t                 eventCount,
                                                  VkEvent const *          pEvents,
                                                  VkDependencyInfo const * pDependencyInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWaitEvents2KHR( commandBuffer, eventCount, pEvents, pDependencyInfos );
      }

      VULKAN_HPP_INLINE void vkCmdPipelineBarrier2KHR( VkCommandBuffer commandBuffer, VkDependencyInfo const * pDependencyInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPipelineBarrier2KHR( commandBuffer, pDependencyInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdWriteTimestamp2KHR( VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteTimestamp2KHR( commandBuffer, stage, queryPool, query );
      }

      VULKAN_HPP_INLINE VkResult vkQueueSubmit2KHR( VkQueue               queue,
                                                    uint32_t              submitCount,
                                                    VkSubmitInfo2 const * pSubmits,
                                                    VkFence               fence ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueSubmit2KHR( queue, submitCount, pSubmits, fence );
      }

      //=== VK_EXT_descriptor_buffer ===

      VULKAN_HPP_INLINE void
        vkGetDescriptorSetLayoutSizeEXT( VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize * pLayoutSizeInBytes ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetLayoutSizeEXT( device, layout, pLayoutSizeInBytes );
      }

      VULKAN_HPP_INLINE void vkGetDescriptorSetLayoutBindingOffsetEXT( VkDevice              device,
                                                                       VkDescriptorSetLayout layout,
                                                                       uint32_t              binding,
                                                                       VkDeviceSize *        pOffset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetLayoutBindingOffsetEXT( device, layout, binding, pOffset );
      }

      VULKAN_HPP_INLINE void
        vkGetDescriptorEXT( VkDevice device, VkDescriptorGetInfoEXT const * pDescriptorInfo, size_t dataSize, void * pDescriptor ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorEXT( device, pDescriptorInfo, dataSize, pDescriptor );
      }

      VULKAN_HPP_INLINE void vkCmdBindDescriptorBuffersEXT( VkCommandBuffer                          commandBuffer,
                                                            uint32_t                                 bufferCount,
                                                            VkDescriptorBufferBindingInfoEXT const * pBindingInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorBuffersEXT( commandBuffer, bufferCount, pBindingInfos );
      }

      VULKAN_HPP_INLINE void vkCmdSetDescriptorBufferOffsetsEXT( VkCommandBuffer      commandBuffer,
                                                                 VkPipelineBindPoint  pipelineBindPoint,
                                                                 VkPipelineLayout     layout,
                                                                 uint32_t             firstSet,
                                                                 uint32_t             setCount,
                                                                 uint32_t const *     pBufferIndices,
                                                                 VkDeviceSize const * pOffsets ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDescriptorBufferOffsetsEXT( commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets );
      }

      VULKAN_HPP_INLINE void vkCmdBindDescriptorBufferEmbeddedSamplersEXT( VkCommandBuffer     commandBuffer,
                                                                           VkPipelineBindPoint pipelineBindPoint,
                                                                           VkPipelineLayout    layout,
                                                                           uint32_t            set ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorBufferEmbeddedSamplersEXT( commandBuffer, pipelineBindPoint, layout, set );
      }

      VULKAN_HPP_INLINE VkResult vkGetBufferOpaqueCaptureDescriptorDataEXT( VkDevice                                     device,
                                                                            VkBufferCaptureDescriptorDataInfoEXT const * pInfo,
                                                                            void *                                       pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferOpaqueCaptureDescriptorDataEXT( device, pInfo, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetImageOpaqueCaptureDescriptorDataEXT( VkDevice                                    device,
                                                                           VkImageCaptureDescriptorDataInfoEXT const * pInfo,
                                                                           void *                                      pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageOpaqueCaptureDescriptorDataEXT( device, pInfo, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetImageViewOpaqueCaptureDescriptorDataEXT( VkDevice                                        device,
                                                                               VkImageViewCaptureDescriptorDataInfoEXT const * pInfo,
                                                                               void *                                          pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageViewOpaqueCaptureDescriptorDataEXT( device, pInfo, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetSamplerOpaqueCaptureDescriptorDataEXT( VkDevice                                      device,
                                                                             VkSamplerCaptureDescriptorDataInfoEXT const * pInfo,
                                                                             void *                                        pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSamplerOpaqueCaptureDescriptorDataEXT( device, pInfo, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT( VkDevice                                                    device,
                                                                                           VkAccelerationStructureCaptureDescriptorDataInfoEXT const * pInfo,
                                                                                           void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT( device, pInfo, pData );
      }

      //=== VK_NV_fragment_shading_rate_enums ===

      VULKAN_HPP_INLINE void vkCmdSetFragmentShadingRateEnumNV( VkCommandBuffer                          commandBuffer,
                                                                VkFragmentShadingRateNV                  shadingRate,
                                                                VkFragmentShadingRateCombinerOpKHR const combinerOps[2] ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetFragmentShadingRateEnumNV( commandBuffer, shadingRate, combinerOps );
      }

      //=== VK_EXT_mesh_shader ===

      VULKAN_HPP_INLINE void
        vkCmdDrawMeshTasksEXT( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksEXT( commandBuffer, groupCountX, groupCountY, groupCountZ );
      }

      VULKAN_HPP_INLINE void vkCmdDrawMeshTasksIndirectEXT(
        VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksIndirectEXT( commandBuffer, buffer, offset, drawCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawMeshTasksIndirectCountEXT( VkCommandBuffer commandBuffer,
                                                                 VkBuffer        buffer,
                                                                 VkDeviceSize    offset,
                                                                 VkBuffer        countBuffer,
                                                                 VkDeviceSize    countBufferOffset,
                                                                 uint32_t        maxDrawCount,
                                                                 uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMeshTasksIndirectCountEXT( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
      }

      //=== VK_KHR_copy_commands2 ===

      VULKAN_HPP_INLINE void vkCmdCopyBuffer2KHR( VkCommandBuffer commandBuffer, VkCopyBufferInfo2 const * pCopyBufferInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBuffer2KHR( commandBuffer, pCopyBufferInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImage2KHR( VkCommandBuffer commandBuffer, VkCopyImageInfo2 const * pCopyImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImage2KHR( commandBuffer, pCopyImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyBufferToImage2KHR( VkCommandBuffer                  commandBuffer,
                                                         VkCopyBufferToImageInfo2 const * pCopyBufferToImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyBufferToImage2KHR( commandBuffer, pCopyBufferToImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyImageToBuffer2KHR( VkCommandBuffer                  commandBuffer,
                                                         VkCopyImageToBufferInfo2 const * pCopyImageToBufferInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyImageToBuffer2KHR( commandBuffer, pCopyImageToBufferInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBlitImage2KHR( VkCommandBuffer commandBuffer, VkBlitImageInfo2 const * pBlitImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBlitImage2KHR( commandBuffer, pBlitImageInfo );
      }

      VULKAN_HPP_INLINE void vkCmdResolveImage2KHR( VkCommandBuffer commandBuffer, VkResolveImageInfo2 const * pResolveImageInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdResolveImage2KHR( commandBuffer, pResolveImageInfo );
      }

      //=== VK_EXT_device_fault ===

      VULKAN_HPP_INLINE VkResult vkGetDeviceFaultInfoEXT( VkDevice                 device,
                                                          VkDeviceFaultCountsEXT * pFaultCounts,
                                                          VkDeviceFaultInfoEXT *   pFaultInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceFaultInfoEXT( device, pFaultCounts, pFaultInfo );
      }

#    if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_NV_acquire_winrt_display ===

      VULKAN_HPP_INLINE VkResult vkAcquireWinrtDisplayNV( VkPhysicalDevice physicalDevice, VkDisplayKHR display ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAcquireWinrtDisplayNV( physicalDevice, display );
      }

      VULKAN_HPP_INLINE VkResult vkGetWinrtDisplayNV( VkPhysicalDevice physicalDevice,
                                                      uint32_t         deviceRelativeId,
                                                      VkDisplayKHR *   pDisplay ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetWinrtDisplayNV( physicalDevice, deviceRelativeId, pDisplay );
      }
#    endif /*VK_USE_PLATFORM_WIN32_KHR*/

#    if defined( VK_USE_PLATFORM_DIRECTFB_EXT )
      //=== VK_EXT_directfb_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateDirectFBSurfaceEXT( VkInstance                             instance,
                                                             VkDirectFBSurfaceCreateInfoEXT const * pCreateInfo,
                                                             VkAllocationCallbacks const *          pAllocator,
                                                             VkSurfaceKHR *                         pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDirectFBSurfaceEXT( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceDirectFBPresentationSupportEXT( VkPhysicalDevice physicalDevice,
                                                                                    uint32_t         queueFamilyIndex,
                                                                                    IDirectFB *      dfb ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceDirectFBPresentationSupportEXT( physicalDevice, queueFamilyIndex, dfb );
      }
#    endif /*VK_USE_PLATFORM_DIRECTFB_EXT*/

      //=== VK_EXT_vertex_input_dynamic_state ===

      VULKAN_HPP_INLINE void vkCmdSetVertexInputEXT( VkCommandBuffer                               commandBuffer,
                                                     uint32_t                                      vertexBindingDescriptionCount,
                                                     VkVertexInputBindingDescription2EXT const *   pVertexBindingDescriptions,
                                                     uint32_t                                      vertexAttributeDescriptionCount,
                                                     VkVertexInputAttributeDescription2EXT const * pVertexAttributeDescriptions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetVertexInputEXT(
          commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions );
      }

#    if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_memory ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryZirconHandleFUCHSIA( VkDevice                                   device,
                                                                 VkMemoryGetZirconHandleInfoFUCHSIA const * pGetZirconHandleInfo,
                                                                 zx_handle_t *                              pZirconHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryZirconHandleFUCHSIA( device, pGetZirconHandleInfo, pZirconHandle );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetMemoryZirconHandlePropertiesFUCHSIA( VkDevice                                device,
                                                  VkExternalMemoryHandleTypeFlagBits      handleType,
                                                  zx_handle_t                             zirconHandle,
                                                  VkMemoryZirconHandlePropertiesFUCHSIA * pMemoryZirconHandleProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryZirconHandlePropertiesFUCHSIA( device, handleType, zirconHandle, pMemoryZirconHandleProperties );
      }
#    endif /*VK_USE_PLATFORM_FUCHSIA*/

#    if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_semaphore ===

      VULKAN_HPP_INLINE VkResult vkImportSemaphoreZirconHandleFUCHSIA(
        VkDevice device, VkImportSemaphoreZirconHandleInfoFUCHSIA const * pImportSemaphoreZirconHandleInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkImportSemaphoreZirconHandleFUCHSIA( device, pImportSemaphoreZirconHandleInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetSemaphoreZirconHandleFUCHSIA( VkDevice                                      device,
                                                                    VkSemaphoreGetZirconHandleInfoFUCHSIA const * pGetZirconHandleInfo,
                                                                    zx_handle_t *                                 pZirconHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetSemaphoreZirconHandleFUCHSIA( device, pGetZirconHandleInfo, pZirconHandle );
      }
#    endif /*VK_USE_PLATFORM_FUCHSIA*/

#    if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_buffer_collection ===

      VULKAN_HPP_INLINE VkResult vkCreateBufferCollectionFUCHSIA( VkDevice                                    device,
                                                                  VkBufferCollectionCreateInfoFUCHSIA const * pCreateInfo,
                                                                  VkAllocationCallbacks const *               pAllocator,
                                                                  VkBufferCollectionFUCHSIA *                 pCollection ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateBufferCollectionFUCHSIA( device, pCreateInfo, pAllocator, pCollection );
      }

      VULKAN_HPP_INLINE VkResult vkSetBufferCollectionImageConstraintsFUCHSIA(
        VkDevice device, VkBufferCollectionFUCHSIA collection, VkImageConstraintsInfoFUCHSIA const * pImageConstraintsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetBufferCollectionImageConstraintsFUCHSIA( device, collection, pImageConstraintsInfo );
      }

      VULKAN_HPP_INLINE VkResult vkSetBufferCollectionBufferConstraintsFUCHSIA(
        VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferConstraintsInfoFUCHSIA const * pBufferConstraintsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetBufferCollectionBufferConstraintsFUCHSIA( device, collection, pBufferConstraintsInfo );
      }

      VULKAN_HPP_INLINE void vkDestroyBufferCollectionFUCHSIA( VkDevice                      device,
                                                               VkBufferCollectionFUCHSIA     collection,
                                                               VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyBufferCollectionFUCHSIA( device, collection, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetBufferCollectionPropertiesFUCHSIA( VkDevice                              device,
                                                                         VkBufferCollectionFUCHSIA             collection,
                                                                         VkBufferCollectionPropertiesFUCHSIA * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetBufferCollectionPropertiesFUCHSIA( device, collection, pProperties );
      }
#    endif /*VK_USE_PLATFORM_FUCHSIA*/

      //=== VK_HUAWEI_subpass_shading ===

      VULKAN_HPP_INLINE VkResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI( VkDevice     device,
                                                                                  VkRenderPass renderpass,
                                                                                  VkExtent2D * pMaxWorkgroupSize ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI( device, renderpass, pMaxWorkgroupSize );
      }

      VULKAN_HPP_INLINE void vkCmdSubpassShadingHUAWEI( VkCommandBuffer commandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSubpassShadingHUAWEI( commandBuffer );
      }

      //=== VK_HUAWEI_invocation_mask ===

      VULKAN_HPP_INLINE void
        vkCmdBindInvocationMaskHUAWEI( VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindInvocationMaskHUAWEI( commandBuffer, imageView, imageLayout );
      }

      //=== VK_NV_external_memory_rdma ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryRemoteAddressNV( VkDevice                               device,
                                                             VkMemoryGetRemoteAddressInfoNV const * pMemoryGetRemoteAddressInfo,
                                                             VkRemoteAddressNV *                    pAddress ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryRemoteAddressNV( device, pMemoryGetRemoteAddressInfo, pAddress );
      }

      //=== VK_EXT_pipeline_properties ===

      VULKAN_HPP_INLINE VkResult vkGetPipelinePropertiesEXT( VkDevice                  device,
                                                             VkPipelineInfoEXT const * pPipelineInfo,
                                                             VkBaseOutStructure *      pPipelineProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelinePropertiesEXT( device, pPipelineInfo, pPipelineProperties );
      }

      //=== VK_EXT_extended_dynamic_state2 ===

      VULKAN_HPP_INLINE void vkCmdSetPatchControlPointsEXT( VkCommandBuffer commandBuffer, uint32_t patchControlPoints ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPatchControlPointsEXT( commandBuffer, patchControlPoints );
      }

      VULKAN_HPP_INLINE void vkCmdSetRasterizerDiscardEnableEXT( VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRasterizerDiscardEnableEXT( commandBuffer, rasterizerDiscardEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthBiasEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthBiasEnableEXT( commandBuffer, depthBiasEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetLogicOpEXT( VkCommandBuffer commandBuffer, VkLogicOp logicOp ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLogicOpEXT( commandBuffer, logicOp );
      }

      VULKAN_HPP_INLINE void vkCmdSetPrimitiveRestartEnableEXT( VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPrimitiveRestartEnableEXT( commandBuffer, primitiveRestartEnable );
      }

#    if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_screen_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateScreenSurfaceQNX( VkInstance                           instance,
                                                           VkScreenSurfaceCreateInfoQNX const * pCreateInfo,
                                                           VkAllocationCallbacks const *        pAllocator,
                                                           VkSurfaceKHR *                       pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateScreenSurfaceQNX( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceScreenPresentationSupportQNX( VkPhysicalDevice        physicalDevice,
                                                                                  uint32_t                queueFamilyIndex,
                                                                                  struct _screen_window * window ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceScreenPresentationSupportQNX( physicalDevice, queueFamilyIndex, window );
      }
#    endif /*VK_USE_PLATFORM_SCREEN_QNX*/

      //=== VK_EXT_color_write_enable ===

      VULKAN_HPP_INLINE void
        vkCmdSetColorWriteEnableEXT( VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkBool32 const * pColorWriteEnables ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetColorWriteEnableEXT( commandBuffer, attachmentCount, pColorWriteEnables );
      }

      //=== VK_KHR_ray_tracing_maintenance1 ===

      VULKAN_HPP_INLINE void vkCmdTraceRaysIndirect2KHR( VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdTraceRaysIndirect2KHR( commandBuffer, indirectDeviceAddress );
      }

      //=== VK_EXT_multi_draw ===

      VULKAN_HPP_INLINE void vkCmdDrawMultiEXT( VkCommandBuffer            commandBuffer,
                                                uint32_t                   drawCount,
                                                VkMultiDrawInfoEXT const * pVertexInfo,
                                                uint32_t                   instanceCount,
                                                uint32_t                   firstInstance,
                                                uint32_t                   stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMultiEXT( commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride );
      }

      VULKAN_HPP_INLINE void vkCmdDrawMultiIndexedEXT( VkCommandBuffer                   commandBuffer,
                                                       uint32_t                          drawCount,
                                                       VkMultiDrawIndexedInfoEXT const * pIndexInfo,
                                                       uint32_t                          instanceCount,
                                                       uint32_t                          firstInstance,
                                                       uint32_t                          stride,
                                                       int32_t const *                   pVertexOffset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawMultiIndexedEXT( commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset );
      }

      //=== VK_EXT_opacity_micromap ===

      VULKAN_HPP_INLINE VkResult vkCreateMicromapEXT( VkDevice                        device,
                                                      VkMicromapCreateInfoEXT const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkMicromapEXT *                 pMicromap ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateMicromapEXT( device, pCreateInfo, pAllocator, pMicromap );
      }

      VULKAN_HPP_INLINE void vkDestroyMicromapEXT( VkDevice device, VkMicromapEXT micromap, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyMicromapEXT( device, micromap, pAllocator );
      }

      VULKAN_HPP_INLINE void
        vkCmdBuildMicromapsEXT( VkCommandBuffer commandBuffer, uint32_t infoCount, VkMicromapBuildInfoEXT const * pInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildMicromapsEXT( commandBuffer, infoCount, pInfos );
      }

      VULKAN_HPP_INLINE VkResult vkBuildMicromapsEXT( VkDevice                       device,
                                                      VkDeferredOperationKHR         deferredOperation,
                                                      uint32_t                       infoCount,
                                                      VkMicromapBuildInfoEXT const * pInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBuildMicromapsEXT( device, deferredOperation, infoCount, pInfos );
      }

      VULKAN_HPP_INLINE VkResult vkCopyMicromapEXT( VkDevice                      device,
                                                    VkDeferredOperationKHR        deferredOperation,
                                                    VkCopyMicromapInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMicromapEXT( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyMicromapToMemoryEXT( VkDevice                              device,
                                                            VkDeferredOperationKHR                deferredOperation,
                                                            VkCopyMicromapToMemoryInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMicromapToMemoryEXT( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCopyMemoryToMicromapEXT( VkDevice                              device,
                                                            VkDeferredOperationKHR                deferredOperation,
                                                            VkCopyMemoryToMicromapInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCopyMemoryToMicromapEXT( device, deferredOperation, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkWriteMicromapsPropertiesEXT( VkDevice              device,
                                                                uint32_t              micromapCount,
                                                                VkMicromapEXT const * pMicromaps,
                                                                VkQueryType           queryType,
                                                                size_t                dataSize,
                                                                void *                pData,
                                                                size_t                stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWriteMicromapsPropertiesEXT( device, micromapCount, pMicromaps, queryType, dataSize, pData, stride );
      }

      VULKAN_HPP_INLINE void vkCmdCopyMicromapEXT( VkCommandBuffer commandBuffer, VkCopyMicromapInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMicromapEXT( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyMicromapToMemoryEXT( VkCommandBuffer                       commandBuffer,
                                                           VkCopyMicromapToMemoryInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMicromapToMemoryEXT( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdCopyMemoryToMicromapEXT( VkCommandBuffer                       commandBuffer,
                                                           VkCopyMemoryToMicromapInfoEXT const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryToMicromapEXT( commandBuffer, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdWriteMicromapsPropertiesEXT( VkCommandBuffer       commandBuffer,
                                                               uint32_t              micromapCount,
                                                               VkMicromapEXT const * pMicromaps,
                                                               VkQueryType           queryType,
                                                               VkQueryPool           queryPool,
                                                               uint32_t              firstQuery ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdWriteMicromapsPropertiesEXT( commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery );
      }

      VULKAN_HPP_INLINE void vkGetDeviceMicromapCompatibilityEXT( VkDevice                                  device,
                                                                  VkMicromapVersionInfoEXT const *          pVersionInfo,
                                                                  VkAccelerationStructureCompatibilityKHR * pCompatibility ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceMicromapCompatibilityEXT( device, pVersionInfo, pCompatibility );
      }

      VULKAN_HPP_INLINE void vkGetMicromapBuildSizesEXT( VkDevice                            device,
                                                         VkAccelerationStructureBuildTypeKHR buildType,
                                                         VkMicromapBuildInfoEXT const *      pBuildInfo,
                                                         VkMicromapBuildSizesInfoEXT *       pSizeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMicromapBuildSizesEXT( device, buildType, pBuildInfo, pSizeInfo );
      }

      //=== VK_HUAWEI_cluster_culling_shader ===

      VULKAN_HPP_INLINE void
        vkCmdDrawClusterHUAWEI( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawClusterHUAWEI( commandBuffer, groupCountX, groupCountY, groupCountZ );
      }

      VULKAN_HPP_INLINE void vkCmdDrawClusterIndirectHUAWEI( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDrawClusterIndirectHUAWEI( commandBuffer, buffer, offset );
      }

      //=== VK_EXT_pageable_device_local_memory ===

      VULKAN_HPP_INLINE void vkSetDeviceMemoryPriorityEXT( VkDevice device, VkDeviceMemory memory, float priority ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetDeviceMemoryPriorityEXT( device, memory, priority );
      }

      //=== VK_KHR_maintenance4 ===

      VULKAN_HPP_INLINE void vkGetDeviceBufferMemoryRequirementsKHR( VkDevice                                 device,
                                                                     VkDeviceBufferMemoryRequirements const * pInfo,
                                                                     VkMemoryRequirements2 *                  pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceBufferMemoryRequirementsKHR( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkGetDeviceImageMemoryRequirementsKHR( VkDevice                                device,
                                                                    VkDeviceImageMemoryRequirements const * pInfo,
                                                                    VkMemoryRequirements2 *                 pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageMemoryRequirementsKHR( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void
        vkGetDeviceImageSparseMemoryRequirementsKHR( VkDevice                                device,
                                                     VkDeviceImageMemoryRequirements const * pInfo,
                                                     uint32_t *                              pSparseMemoryRequirementCount,
                                                     VkSparseImageMemoryRequirements2 *      pSparseMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageSparseMemoryRequirementsKHR( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
      }

      //=== VK_VALVE_descriptor_set_host_mapping ===

      VULKAN_HPP_INLINE void vkGetDescriptorSetLayoutHostMappingInfoVALVE( VkDevice                                     device,
                                                                           VkDescriptorSetBindingReferenceVALVE const * pBindingReference,
                                                                           VkDescriptorSetLayoutHostMappingInfoVALVE *  pHostMapping ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetLayoutHostMappingInfoVALVE( device, pBindingReference, pHostMapping );
      }

      VULKAN_HPP_INLINE void vkGetDescriptorSetHostMappingVALVE( VkDevice device, VkDescriptorSet descriptorSet, void ** ppData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDescriptorSetHostMappingVALVE( device, descriptorSet, ppData );
      }

      //=== VK_NV_copy_memory_indirect ===

      VULKAN_HPP_INLINE void vkCmdCopyMemoryIndirectNV( VkCommandBuffer commandBuffer,
                                                        VkDeviceAddress copyBufferAddress,
                                                        uint32_t        copyCount,
                                                        uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryIndirectNV( commandBuffer, copyBufferAddress, copyCount, stride );
      }

      VULKAN_HPP_INLINE void vkCmdCopyMemoryToImageIndirectNV( VkCommandBuffer                  commandBuffer,
                                                               VkDeviceAddress                  copyBufferAddress,
                                                               uint32_t                         copyCount,
                                                               uint32_t                         stride,
                                                               VkImage                          dstImage,
                                                               VkImageLayout                    dstImageLayout,
                                                               VkImageSubresourceLayers const * pImageSubresources ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryToImageIndirectNV( commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources );
      }

      //=== VK_NV_memory_decompression ===

      VULKAN_HPP_INLINE void vkCmdDecompressMemoryNV( VkCommandBuffer                    commandBuffer,
                                                      uint32_t                           decompressRegionCount,
                                                      VkDecompressMemoryRegionNV const * pDecompressMemoryRegions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDecompressMemoryNV( commandBuffer, decompressRegionCount, pDecompressMemoryRegions );
      }

      VULKAN_HPP_INLINE void vkCmdDecompressMemoryIndirectCountNV( VkCommandBuffer commandBuffer,
                                                                   VkDeviceAddress indirectCommandsAddress,
                                                                   VkDeviceAddress indirectCommandsCountAddress,
                                                                   uint32_t        stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDecompressMemoryIndirectCountNV( commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride );
      }

      //=== VK_NV_device_generated_commands_compute ===

      VULKAN_HPP_INLINE void vkGetPipelineIndirectMemoryRequirementsNV( VkDevice                            device,
                                                                        VkComputePipelineCreateInfo const * pCreateInfo,
                                                                        VkMemoryRequirements2 *             pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineIndirectMemoryRequirementsNV( device, pCreateInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkCmdUpdatePipelineIndirectBufferNV( VkCommandBuffer     commandBuffer,
                                                                  VkPipelineBindPoint pipelineBindPoint,
                                                                  VkPipeline          pipeline ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdUpdatePipelineIndirectBufferNV( commandBuffer, pipelineBindPoint, pipeline );
      }

      VULKAN_HPP_INLINE VkDeviceAddress vkGetPipelineIndirectDeviceAddressNV( VkDevice                                      device,
                                                                              VkPipelineIndirectDeviceAddressInfoNV const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineIndirectDeviceAddressNV( device, pInfo );
      }

#    if defined( VK_USE_PLATFORM_OHOS )
      //=== VK_OHOS_external_memory ===

      VULKAN_HPP_INLINE VkResult vkGetNativeBufferPropertiesOHOS( VkDevice                       device,
                                                                  const struct OH_NativeBuffer * buffer,
                                                                  VkNativeBufferPropertiesOHOS * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetNativeBufferPropertiesOHOS( device, buffer, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetMemoryNativeBufferOHOS( VkDevice                                device,
                                                              VkMemoryGetNativeBufferInfoOHOS const * pInfo,
                                                              struct OH_NativeBuffer **               pBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryNativeBufferOHOS( device, pInfo, pBuffer );
      }
#    endif /*VK_USE_PLATFORM_OHOS*/

      //=== VK_EXT_extended_dynamic_state3 ===

      VULKAN_HPP_INLINE void vkCmdSetDepthClampEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthClampEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthClampEnableEXT( commandBuffer, depthClampEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetPolygonModeEXT( VkCommandBuffer commandBuffer, VkPolygonMode polygonMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetPolygonModeEXT( commandBuffer, polygonMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetRasterizationSamplesEXT( VkCommandBuffer       commandBuffer,
                                                              VkSampleCountFlagBits rasterizationSamples ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRasterizationSamplesEXT( commandBuffer, rasterizationSamples );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetSampleMaskEXT( VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, VkSampleMask const * pSampleMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetSampleMaskEXT( commandBuffer, samples, pSampleMask );
      }

      VULKAN_HPP_INLINE void vkCmdSetAlphaToCoverageEnableEXT( VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetAlphaToCoverageEnableEXT( commandBuffer, alphaToCoverageEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetAlphaToOneEnableEXT( VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetAlphaToOneEnableEXT( commandBuffer, alphaToOneEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetLogicOpEnableEXT( VkCommandBuffer commandBuffer, VkBool32 logicOpEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLogicOpEnableEXT( commandBuffer, logicOpEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetColorBlendEnableEXT( VkCommandBuffer  commandBuffer,
                                                          uint32_t         firstAttachment,
                                                          uint32_t         attachmentCount,
                                                          VkBool32 const * pColorBlendEnables ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetColorBlendEnableEXT( commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables );
      }

      VULKAN_HPP_INLINE void vkCmdSetColorBlendEquationEXT( VkCommandBuffer                 commandBuffer,
                                                            uint32_t                        firstAttachment,
                                                            uint32_t                        attachmentCount,
                                                            VkColorBlendEquationEXT const * pColorBlendEquations ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetColorBlendEquationEXT( commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations );
      }

      VULKAN_HPP_INLINE void vkCmdSetColorWriteMaskEXT( VkCommandBuffer               commandBuffer,
                                                        uint32_t                      firstAttachment,
                                                        uint32_t                      attachmentCount,
                                                        VkColorComponentFlags const * pColorWriteMasks ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetColorWriteMaskEXT( commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks );
      }

      VULKAN_HPP_INLINE void vkCmdSetTessellationDomainOriginEXT( VkCommandBuffer            commandBuffer,
                                                                  VkTessellationDomainOrigin domainOrigin ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetTessellationDomainOriginEXT( commandBuffer, domainOrigin );
      }

      VULKAN_HPP_INLINE void vkCmdSetRasterizationStreamEXT( VkCommandBuffer commandBuffer, uint32_t rasterizationStream ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRasterizationStreamEXT( commandBuffer, rasterizationStream );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetConservativeRasterizationModeEXT( VkCommandBuffer                    commandBuffer,
                                                  VkConservativeRasterizationModeEXT conservativeRasterizationMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetConservativeRasterizationModeEXT( commandBuffer, conservativeRasterizationMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetExtraPrimitiveOverestimationSizeEXT( VkCommandBuffer commandBuffer,
                                                                          float           extraPrimitiveOverestimationSize ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetExtraPrimitiveOverestimationSizeEXT( commandBuffer, extraPrimitiveOverestimationSize );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthClipEnableEXT( VkCommandBuffer commandBuffer, VkBool32 depthClipEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthClipEnableEXT( commandBuffer, depthClipEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetSampleLocationsEnableEXT( VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetSampleLocationsEnableEXT( commandBuffer, sampleLocationsEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetColorBlendAdvancedEXT( VkCommandBuffer                 commandBuffer,
                                                            uint32_t                        firstAttachment,
                                                            uint32_t                        attachmentCount,
                                                            VkColorBlendAdvancedEXT const * pColorBlendAdvanced ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetColorBlendAdvancedEXT( commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced );
      }

      VULKAN_HPP_INLINE void vkCmdSetProvokingVertexModeEXT( VkCommandBuffer          commandBuffer,
                                                             VkProvokingVertexModeEXT provokingVertexMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetProvokingVertexModeEXT( commandBuffer, provokingVertexMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetLineRasterizationModeEXT( VkCommandBuffer            commandBuffer,
                                                               VkLineRasterizationModeEXT lineRasterizationMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineRasterizationModeEXT( commandBuffer, lineRasterizationMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetLineStippleEnableEXT( VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineStippleEnableEXT( commandBuffer, stippledLineEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthClipNegativeOneToOneEXT( VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthClipNegativeOneToOneEXT( commandBuffer, negativeOneToOne );
      }

      VULKAN_HPP_INLINE void vkCmdSetViewportWScalingEnableNV( VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportWScalingEnableNV( commandBuffer, viewportWScalingEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetViewportSwizzleNV( VkCommandBuffer             commandBuffer,
                                                        uint32_t                    firstViewport,
                                                        uint32_t                    viewportCount,
                                                        VkViewportSwizzleNV const * pViewportSwizzles ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetViewportSwizzleNV( commandBuffer, firstViewport, viewportCount, pViewportSwizzles );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageToColorEnableNV( VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageToColorEnableNV( commandBuffer, coverageToColorEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageToColorLocationNV( VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageToColorLocationNV( commandBuffer, coverageToColorLocation );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageModulationModeNV( VkCommandBuffer            commandBuffer,
                                                               VkCoverageModulationModeNV coverageModulationMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageModulationModeNV( commandBuffer, coverageModulationMode );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageModulationTableEnableNV( VkCommandBuffer commandBuffer,
                                                                      VkBool32        coverageModulationTableEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageModulationTableEnableNV( commandBuffer, coverageModulationTableEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageModulationTableNV( VkCommandBuffer commandBuffer,
                                                                uint32_t        coverageModulationTableCount,
                                                                float const *   pCoverageModulationTable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageModulationTableNV( commandBuffer, coverageModulationTableCount, pCoverageModulationTable );
      }

      VULKAN_HPP_INLINE void vkCmdSetShadingRateImageEnableNV( VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetShadingRateImageEnableNV( commandBuffer, shadingRateImageEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetRepresentativeFragmentTestEnableNV( VkCommandBuffer commandBuffer,
                                                                         VkBool32        representativeFragmentTestEnable ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetRepresentativeFragmentTestEnableNV( commandBuffer, representativeFragmentTestEnable );
      }

      VULKAN_HPP_INLINE void vkCmdSetCoverageReductionModeNV( VkCommandBuffer           commandBuffer,
                                                              VkCoverageReductionModeNV coverageReductionMode ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetCoverageReductionModeNV( commandBuffer, coverageReductionMode );
      }

      //=== VK_ARM_tensors ===

      VULKAN_HPP_INLINE VkResult vkCreateTensorARM( VkDevice                      device,
                                                    VkTensorCreateInfoARM const * pCreateInfo,
                                                    VkAllocationCallbacks const * pAllocator,
                                                    VkTensorARM *                 pTensor ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateTensorARM( device, pCreateInfo, pAllocator, pTensor );
      }

      VULKAN_HPP_INLINE void vkDestroyTensorARM( VkDevice device, VkTensorARM tensor, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyTensorARM( device, tensor, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateTensorViewARM( VkDevice                          device,
                                                        VkTensorViewCreateInfoARM const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkTensorViewARM *                 pView ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateTensorViewARM( device, pCreateInfo, pAllocator, pView );
      }

      VULKAN_HPP_INLINE void
        vkDestroyTensorViewARM( VkDevice device, VkTensorViewARM tensorView, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyTensorViewARM( device, tensorView, pAllocator );
      }

      VULKAN_HPP_INLINE void vkGetTensorMemoryRequirementsARM( VkDevice                                  device,
                                                               VkTensorMemoryRequirementsInfoARM const * pInfo,
                                                               VkMemoryRequirements2 *                   pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetTensorMemoryRequirementsARM( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE VkResult vkBindTensorMemoryARM( VkDevice                          device,
                                                        uint32_t                          bindInfoCount,
                                                        VkBindTensorMemoryInfoARM const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindTensorMemoryARM( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE void vkGetDeviceTensorMemoryRequirementsARM( VkDevice                                    device,
                                                                     VkDeviceTensorMemoryRequirementsARM const * pInfo,
                                                                     VkMemoryRequirements2 *                     pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceTensorMemoryRequirementsARM( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkCmdCopyTensorARM( VkCommandBuffer commandBuffer, VkCopyTensorInfoARM const * pCopyTensorInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyTensorARM( commandBuffer, pCopyTensorInfo );
      }

      VULKAN_HPP_INLINE void
        vkGetPhysicalDeviceExternalTensorPropertiesARM( VkPhysicalDevice                              physicalDevice,
                                                        VkPhysicalDeviceExternalTensorInfoARM const * pExternalTensorInfo,
                                                        VkExternalTensorPropertiesARM *               pExternalTensorProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceExternalTensorPropertiesARM( physicalDevice, pExternalTensorInfo, pExternalTensorProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetTensorOpaqueCaptureDescriptorDataARM( VkDevice                                     device,
                                                                            VkTensorCaptureDescriptorDataInfoARM const * pInfo,
                                                                            void *                                       pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetTensorOpaqueCaptureDescriptorDataARM( device, pInfo, pData );
      }

      VULKAN_HPP_INLINE VkResult vkGetTensorViewOpaqueCaptureDescriptorDataARM( VkDevice                                         device,
                                                                                VkTensorViewCaptureDescriptorDataInfoARM const * pInfo,
                                                                                void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetTensorViewOpaqueCaptureDescriptorDataARM( device, pInfo, pData );
      }

      //=== VK_EXT_shader_module_identifier ===

      VULKAN_HPP_INLINE void
        vkGetShaderModuleIdentifierEXT( VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT * pIdentifier ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetShaderModuleIdentifierEXT( device, shaderModule, pIdentifier );
      }

      VULKAN_HPP_INLINE void vkGetShaderModuleCreateInfoIdentifierEXT( VkDevice                         device,
                                                                       VkShaderModuleCreateInfo const * pCreateInfo,
                                                                       VkShaderModuleIdentifierEXT *    pIdentifier ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetShaderModuleCreateInfoIdentifierEXT( device, pCreateInfo, pIdentifier );
      }

      //=== VK_NV_optical_flow ===

      VULKAN_HPP_INLINE VkResult
        vkGetPhysicalDeviceOpticalFlowImageFormatsNV( VkPhysicalDevice                       physicalDevice,
                                                      VkOpticalFlowImageFormatInfoNV const * pOpticalFlowImageFormatInfo,
                                                      uint32_t *                             pFormatCount,
                                                      VkOpticalFlowImageFormatPropertiesNV * pImageFormatProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceOpticalFlowImageFormatsNV( physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties );
      }

      VULKAN_HPP_INLINE VkResult vkCreateOpticalFlowSessionNV( VkDevice                                 device,
                                                               VkOpticalFlowSessionCreateInfoNV const * pCreateInfo,
                                                               VkAllocationCallbacks const *            pAllocator,
                                                               VkOpticalFlowSessionNV *                 pSession ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateOpticalFlowSessionNV( device, pCreateInfo, pAllocator, pSession );
      }

      VULKAN_HPP_INLINE void
        vkDestroyOpticalFlowSessionNV( VkDevice device, VkOpticalFlowSessionNV session, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyOpticalFlowSessionNV( device, session, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkBindOpticalFlowSessionImageNV( VkDevice                           device,
                                                                  VkOpticalFlowSessionNV             session,
                                                                  VkOpticalFlowSessionBindingPointNV bindingPoint,
                                                                  VkImageView                        view,
                                                                  VkImageLayout                      layout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindOpticalFlowSessionImageNV( device, session, bindingPoint, view, layout );
      }

      VULKAN_HPP_INLINE void vkCmdOpticalFlowExecuteNV( VkCommandBuffer                    commandBuffer,
                                                        VkOpticalFlowSessionNV             session,
                                                        VkOpticalFlowExecuteInfoNV const * pExecuteInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdOpticalFlowExecuteNV( commandBuffer, session, pExecuteInfo );
      }

      //=== VK_KHR_maintenance5 ===

      VULKAN_HPP_INLINE void vkCmdBindIndexBuffer2KHR(
        VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindIndexBuffer2KHR( commandBuffer, buffer, offset, size, indexType );
      }

      VULKAN_HPP_INLINE void
        vkGetRenderingAreaGranularityKHR( VkDevice device, VkRenderingAreaInfo const * pRenderingAreaInfo, VkExtent2D * pGranularity ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetRenderingAreaGranularityKHR( device, pRenderingAreaInfo, pGranularity );
      }

      VULKAN_HPP_INLINE void vkGetDeviceImageSubresourceLayoutKHR( VkDevice                             device,
                                                                   VkDeviceImageSubresourceInfo const * pInfo,
                                                                   VkSubresourceLayout2 *               pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDeviceImageSubresourceLayoutKHR( device, pInfo, pLayout );
      }

      VULKAN_HPP_INLINE void vkGetImageSubresourceLayout2KHR( VkDevice                    device,
                                                              VkImage                     image,
                                                              VkImageSubresource2 const * pSubresource,
                                                              VkSubresourceLayout2 *      pLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetImageSubresourceLayout2KHR( device, image, pSubresource, pLayout );
      }

      //=== VK_AMD_anti_lag ===

      VULKAN_HPP_INLINE void vkAntiLagUpdateAMD( VkDevice device, VkAntiLagDataAMD const * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkAntiLagUpdateAMD( device, pData );
      }

      //=== VK_KHR_present_wait2 ===

      VULKAN_HPP_INLINE VkResult vkWaitForPresent2KHR( VkDevice                      device,
                                                       VkSwapchainKHR                swapchain,
                                                       VkPresentWait2InfoKHR const * pPresentWait2Info ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkWaitForPresent2KHR( device, swapchain, pPresentWait2Info );
      }

      //=== VK_EXT_shader_object ===

      VULKAN_HPP_INLINE VkResult vkCreateShadersEXT( VkDevice                      device,
                                                     uint32_t                      createInfoCount,
                                                     VkShaderCreateInfoEXT const * pCreateInfos,
                                                     VkAllocationCallbacks const * pAllocator,
                                                     VkShaderEXT *                 pShaders ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateShadersEXT( device, createInfoCount, pCreateInfos, pAllocator, pShaders );
      }

      VULKAN_HPP_INLINE void vkDestroyShaderEXT( VkDevice device, VkShaderEXT shader, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyShaderEXT( device, shader, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetShaderBinaryDataEXT( VkDevice device, VkShaderEXT shader, size_t * pDataSize, void * pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetShaderBinaryDataEXT( device, shader, pDataSize, pData );
      }

      VULKAN_HPP_INLINE void vkCmdBindShadersEXT( VkCommandBuffer               commandBuffer,
                                                  uint32_t                      stageCount,
                                                  VkShaderStageFlagBits const * pStages,
                                                  VkShaderEXT const *           pShaders ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindShadersEXT( commandBuffer, stageCount, pStages, pShaders );
      }

      VULKAN_HPP_INLINE void vkCmdSetDepthClampRangeEXT( VkCommandBuffer              commandBuffer,
                                                         VkDepthClampModeEXT          depthClampMode,
                                                         VkDepthClampRangeEXT const * pDepthClampRange ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDepthClampRangeEXT( commandBuffer, depthClampMode, pDepthClampRange );
      }

      //=== VK_KHR_pipeline_binary ===

      VULKAN_HPP_INLINE VkResult vkCreatePipelineBinariesKHR( VkDevice                              device,
                                                              VkPipelineBinaryCreateInfoKHR const * pCreateInfo,
                                                              VkAllocationCallbacks const *         pAllocator,
                                                              VkPipelineBinaryHandlesInfoKHR *      pBinaries ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreatePipelineBinariesKHR( device, pCreateInfo, pAllocator, pBinaries );
      }

      VULKAN_HPP_INLINE void
        vkDestroyPipelineBinaryKHR( VkDevice device, VkPipelineBinaryKHR pipelineBinary, VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyPipelineBinaryKHR( device, pipelineBinary, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkGetPipelineKeyKHR( VkDevice                        device,
                                                      VkPipelineCreateInfoKHR const * pPipelineCreateInfo,
                                                      VkPipelineBinaryKeyKHR *        pPipelineKey ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineKeyKHR( device, pPipelineCreateInfo, pPipelineKey );
      }

      VULKAN_HPP_INLINE VkResult vkGetPipelineBinaryDataKHR( VkDevice                            device,
                                                             VkPipelineBinaryDataInfoKHR const * pInfo,
                                                             VkPipelineBinaryKeyKHR *            pPipelineBinaryKey,
                                                             size_t *                            pPipelineBinaryDataSize,
                                                             void *                              pPipelineBinaryData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPipelineBinaryDataKHR( device, pInfo, pPipelineBinaryKey, pPipelineBinaryDataSize, pPipelineBinaryData );
      }

      VULKAN_HPP_INLINE VkResult vkReleaseCapturedPipelineDataKHR( VkDevice                                     device,
                                                                   VkReleaseCapturedPipelineDataInfoKHR const * pInfo,
                                                                   VkAllocationCallbacks const *                pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseCapturedPipelineDataKHR( device, pInfo, pAllocator );
      }

      //=== VK_QCOM_tile_properties ===

      VULKAN_HPP_INLINE VkResult vkGetFramebufferTilePropertiesQCOM( VkDevice               device,
                                                                     VkFramebuffer          framebuffer,
                                                                     uint32_t *             pPropertiesCount,
                                                                     VkTilePropertiesQCOM * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetFramebufferTilePropertiesQCOM( device, framebuffer, pPropertiesCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetDynamicRenderingTilePropertiesQCOM( VkDevice                device,
                                                                          VkRenderingInfo const * pRenderingInfo,
                                                                          VkTilePropertiesQCOM *  pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDynamicRenderingTilePropertiesQCOM( device, pRenderingInfo, pProperties );
      }

      //=== VK_KHR_swapchain_maintenance1 ===

      VULKAN_HPP_INLINE VkResult vkReleaseSwapchainImagesKHR( VkDevice device, VkReleaseSwapchainImagesInfoKHR const * pReleaseInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkReleaseSwapchainImagesKHR( device, pReleaseInfo );
      }

      //=== VK_NV_cooperative_vector ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCooperativeVectorPropertiesNV( VkPhysicalDevice                  physicalDevice,
                                                                                   uint32_t *                        pPropertyCount,
                                                                                   VkCooperativeVectorPropertiesNV * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCooperativeVectorPropertiesNV( physicalDevice, pPropertyCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkConvertCooperativeVectorMatrixNV( VkDevice                                       device,
                                                                     VkConvertCooperativeVectorMatrixInfoNV const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkConvertCooperativeVectorMatrixNV( device, pInfo );
      }

      VULKAN_HPP_INLINE void vkCmdConvertCooperativeVectorMatrixNV( VkCommandBuffer                                commandBuffer,
                                                                    uint32_t                                       infoCount,
                                                                    VkConvertCooperativeVectorMatrixInfoNV const * pInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdConvertCooperativeVectorMatrixNV( commandBuffer, infoCount, pInfos );
      }

      //=== VK_NV_low_latency2 ===

      VULKAN_HPP_INLINE VkResult vkSetLatencySleepModeNV( VkDevice                         device,
                                                          VkSwapchainKHR                   swapchain,
                                                          VkLatencySleepModeInfoNV const * pSleepModeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetLatencySleepModeNV( device, swapchain, pSleepModeInfo );
      }

      VULKAN_HPP_INLINE VkResult vkLatencySleepNV( VkDevice                     device,
                                                   VkSwapchainKHR               swapchain,
                                                   VkLatencySleepInfoNV const * pSleepInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkLatencySleepNV( device, swapchain, pSleepInfo );
      }

      VULKAN_HPP_INLINE void
        vkSetLatencyMarkerNV( VkDevice device, VkSwapchainKHR swapchain, VkSetLatencyMarkerInfoNV const * pLatencyMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkSetLatencyMarkerNV( device, swapchain, pLatencyMarkerInfo );
      }

      VULKAN_HPP_INLINE void
        vkGetLatencyTimingsNV( VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV * pLatencyMarkerInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetLatencyTimingsNV( device, swapchain, pLatencyMarkerInfo );
      }

      VULKAN_HPP_INLINE void vkQueueNotifyOutOfBandNV( VkQueue queue, VkOutOfBandQueueTypeInfoNV const * pQueueTypeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkQueueNotifyOutOfBandNV( queue, pQueueTypeInfo );
      }

      //=== VK_KHR_cooperative_matrix ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR( VkPhysicalDevice                   physicalDevice,
                                                                                    uint32_t *                         pPropertyCount,
                                                                                    VkCooperativeMatrixPropertiesKHR * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR( physicalDevice, pPropertyCount, pProperties );
      }

      //=== VK_ARM_data_graph ===

      VULKAN_HPP_INLINE VkResult vkCreateDataGraphPipelinesARM( VkDevice                                 device,
                                                                VkDeferredOperationKHR                   deferredOperation,
                                                                VkPipelineCache                          pipelineCache,
                                                                uint32_t                                 createInfoCount,
                                                                VkDataGraphPipelineCreateInfoARM const * pCreateInfos,
                                                                VkAllocationCallbacks const *            pAllocator,
                                                                VkPipeline *                             pPipelines ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDataGraphPipelinesARM( device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
      }

      VULKAN_HPP_INLINE VkResult vkCreateDataGraphPipelineSessionARM( VkDevice                                        device,
                                                                      VkDataGraphPipelineSessionCreateInfoARM const * pCreateInfo,
                                                                      VkAllocationCallbacks const *                   pAllocator,
                                                                      VkDataGraphPipelineSessionARM *                 pSession ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateDataGraphPipelineSessionARM( device, pCreateInfo, pAllocator, pSession );
      }

      VULKAN_HPP_INLINE VkResult vkGetDataGraphPipelineSessionBindPointRequirementsARM(
        VkDevice                                                       device,
        VkDataGraphPipelineSessionBindPointRequirementsInfoARM const * pInfo,
        uint32_t *                                                     pBindPointRequirementCount,
        VkDataGraphPipelineSessionBindPointRequirementARM *            pBindPointRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDataGraphPipelineSessionBindPointRequirementsARM( device, pInfo, pBindPointRequirementCount, pBindPointRequirements );
      }

      VULKAN_HPP_INLINE void vkGetDataGraphPipelineSessionMemoryRequirementsARM( VkDevice                                                    device,
                                                                                 VkDataGraphPipelineSessionMemoryRequirementsInfoARM const * pInfo,
                                                                                 VkMemoryRequirements2 * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDataGraphPipelineSessionMemoryRequirementsARM( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE VkResult vkBindDataGraphPipelineSessionMemoryARM(
        VkDevice device, uint32_t bindInfoCount, VkBindDataGraphPipelineSessionMemoryInfoARM const * pBindInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkBindDataGraphPipelineSessionMemoryARM( device, bindInfoCount, pBindInfos );
      }

      VULKAN_HPP_INLINE void vkDestroyDataGraphPipelineSessionARM( VkDevice                      device,
                                                                   VkDataGraphPipelineSessionARM session,
                                                                   VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyDataGraphPipelineSessionARM( device, session, pAllocator );
      }

      VULKAN_HPP_INLINE void vkCmdDispatchDataGraphARM( VkCommandBuffer                            commandBuffer,
                                                        VkDataGraphPipelineSessionARM              session,
                                                        VkDataGraphPipelineDispatchInfoARM const * pInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDispatchDataGraphARM( commandBuffer, session, pInfo );
      }

      VULKAN_HPP_INLINE VkResult vkGetDataGraphPipelineAvailablePropertiesARM( VkDevice                           device,
                                                                               VkDataGraphPipelineInfoARM const * pPipelineInfo,
                                                                               uint32_t *                         pPropertiesCount,
                                                                               VkDataGraphPipelinePropertyARM *   pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDataGraphPipelineAvailablePropertiesARM( device, pPipelineInfo, pPropertiesCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult vkGetDataGraphPipelinePropertiesARM( VkDevice                                    device,
                                                                      VkDataGraphPipelineInfoARM const *          pPipelineInfo,
                                                                      uint32_t                                    propertiesCount,
                                                                      VkDataGraphPipelinePropertyQueryResultARM * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetDataGraphPipelinePropertiesARM( device, pPipelineInfo, propertiesCount, pProperties );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM( VkPhysicalDevice                      physicalDevice,
                                                              uint32_t                              queueFamilyIndex,
                                                              uint32_t *                            pQueueFamilyDataGraphPropertyCount,
                                                              VkQueueFamilyDataGraphPropertiesARM * pQueueFamilyDataGraphProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM(
          physicalDevice, queueFamilyIndex, pQueueFamilyDataGraphPropertyCount, pQueueFamilyDataGraphProperties );
      }

      VULKAN_HPP_INLINE void vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM(
        VkPhysicalDevice                                                    physicalDevice,
        VkPhysicalDeviceQueueFamilyDataGraphProcessingEngineInfoARM const * pQueueFamilyDataGraphProcessingEngineInfo,
        VkQueueFamilyDataGraphProcessingEnginePropertiesARM *               pQueueFamilyDataGraphProcessingEngineProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM(
          physicalDevice, pQueueFamilyDataGraphProcessingEngineInfo, pQueueFamilyDataGraphProcessingEngineProperties );
      }

      //=== VK_EXT_attachment_feedback_loop_dynamic_state ===

      VULKAN_HPP_INLINE void vkCmdSetAttachmentFeedbackLoopEnableEXT( VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetAttachmentFeedbackLoopEnableEXT( commandBuffer, aspectMask );
      }

#    if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===

      VULKAN_HPP_INLINE VkResult vkGetScreenBufferPropertiesQNX( VkDevice                      device,
                                                                 const struct _screen_buffer * buffer,
                                                                 VkScreenBufferPropertiesQNX * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetScreenBufferPropertiesQNX( device, buffer, pProperties );
      }
#    endif /*VK_USE_PLATFORM_SCREEN_QNX*/

      //=== VK_KHR_line_rasterization ===

      VULKAN_HPP_INLINE void
        vkCmdSetLineStippleKHR( VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetLineStippleKHR( commandBuffer, lineStippleFactor, lineStipplePattern );
      }

      //=== VK_KHR_calibrated_timestamps ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCalibrateableTimeDomainsKHR( VkPhysicalDevice  physicalDevice,
                                                                                 uint32_t *        pTimeDomainCount,
                                                                                 VkTimeDomainKHR * pTimeDomains ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCalibrateableTimeDomainsKHR( physicalDevice, pTimeDomainCount, pTimeDomains );
      }

      VULKAN_HPP_INLINE VkResult vkGetCalibratedTimestampsKHR( VkDevice                             device,
                                                               uint32_t                             timestampCount,
                                                               VkCalibratedTimestampInfoKHR const * pTimestampInfos,
                                                               uint64_t *                           pTimestamps,
                                                               uint64_t *                           pMaxDeviation ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetCalibratedTimestampsKHR( device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation );
      }

      //=== VK_KHR_maintenance6 ===

      VULKAN_HPP_INLINE void vkCmdBindDescriptorSets2KHR( VkCommandBuffer                  commandBuffer,
                                                          VkBindDescriptorSetsInfo const * pBindDescriptorSetsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorSets2KHR( commandBuffer, pBindDescriptorSetsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdPushConstants2KHR( VkCommandBuffer commandBuffer, VkPushConstantsInfo const * pPushConstantsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushConstants2KHR( commandBuffer, pPushConstantsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdPushDescriptorSet2KHR( VkCommandBuffer                 commandBuffer,
                                                         VkPushDescriptorSetInfo const * pPushDescriptorSetInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSet2KHR( commandBuffer, pPushDescriptorSetInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdPushDescriptorSetWithTemplate2KHR( VkCommandBuffer                             commandBuffer,
                                                VkPushDescriptorSetWithTemplateInfo const * pPushDescriptorSetWithTemplateInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPushDescriptorSetWithTemplate2KHR( commandBuffer, pPushDescriptorSetWithTemplateInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdSetDescriptorBufferOffsets2EXT( VkCommandBuffer                             commandBuffer,
                                             VkSetDescriptorBufferOffsetsInfoEXT const * pSetDescriptorBufferOffsetsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetDescriptorBufferOffsets2EXT( commandBuffer, pSetDescriptorBufferOffsetsInfo );
      }

      VULKAN_HPP_INLINE void vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(
        VkCommandBuffer                                       commandBuffer,
        VkBindDescriptorBufferEmbeddedSamplersInfoEXT const * pBindDescriptorBufferEmbeddedSamplersInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindDescriptorBufferEmbeddedSamplers2EXT( commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo );
      }

      //=== VK_QCOM_tile_memory_heap ===

      VULKAN_HPP_INLINE void vkCmdBindTileMemoryQCOM( VkCommandBuffer                  commandBuffer,
                                                      VkTileMemoryBindInfoQCOM const * pTileMemoryBindInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBindTileMemoryQCOM( commandBuffer, pTileMemoryBindInfo );
      }

      //=== VK_KHR_copy_memory_indirect ===

      VULKAN_HPP_INLINE void vkCmdCopyMemoryIndirectKHR( VkCommandBuffer                     commandBuffer,
                                                         VkCopyMemoryIndirectInfoKHR const * pCopyMemoryIndirectInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryIndirectKHR( commandBuffer, pCopyMemoryIndirectInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdCopyMemoryToImageIndirectKHR( VkCommandBuffer                            commandBuffer,
                                           VkCopyMemoryToImageIndirectInfoKHR const * pCopyMemoryToImageIndirectInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdCopyMemoryToImageIndirectKHR( commandBuffer, pCopyMemoryToImageIndirectInfo );
      }

      //=== VK_EXT_memory_decompression ===

      VULKAN_HPP_INLINE void vkCmdDecompressMemoryEXT( VkCommandBuffer                   commandBuffer,
                                                       VkDecompressMemoryInfoEXT const * pDecompressMemoryInfoEXT ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDecompressMemoryEXT( commandBuffer, pDecompressMemoryInfoEXT );
      }

      VULKAN_HPP_INLINE void vkCmdDecompressMemoryIndirectCountEXT( VkCommandBuffer                     commandBuffer,
                                                                    VkMemoryDecompressionMethodFlagsEXT decompressionMethod,
                                                                    VkDeviceAddress                     indirectCommandsAddress,
                                                                    VkDeviceAddress                     indirectCommandsCountAddress,
                                                                    uint32_t                            maxDecompressionCount,
                                                                    uint32_t                            stride ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdDecompressMemoryIndirectCountEXT(
          commandBuffer, decompressionMethod, indirectCommandsAddress, indirectCommandsCountAddress, maxDecompressionCount, stride );
      }

      //=== VK_NV_external_compute_queue ===

      VULKAN_HPP_INLINE VkResult vkCreateExternalComputeQueueNV( VkDevice                                   device,
                                                                 VkExternalComputeQueueCreateInfoNV const * pCreateInfo,
                                                                 VkAllocationCallbacks const *              pAllocator,
                                                                 VkExternalComputeQueueNV *                 pExternalQueue ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateExternalComputeQueueNV( device, pCreateInfo, pAllocator, pExternalQueue );
      }

      VULKAN_HPP_INLINE void vkDestroyExternalComputeQueueNV( VkDevice                      device,
                                                              VkExternalComputeQueueNV      externalQueue,
                                                              VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyExternalComputeQueueNV( device, externalQueue, pAllocator );
      }

      VULKAN_HPP_INLINE void vkGetExternalComputeQueueDataNV( VkExternalComputeQueueNV             externalQueue,
                                                              VkExternalComputeQueueDataParamsNV * params,
                                                              void *                               pData ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetExternalComputeQueueDataNV( externalQueue, params, pData );
      }

      //=== VK_NV_cluster_acceleration_structure ===

      VULKAN_HPP_INLINE void vkGetClusterAccelerationStructureBuildSizesNV( VkDevice                                          device,
                                                                            VkClusterAccelerationStructureInputInfoNV const * pInfo,
                                                                            VkAccelerationStructureBuildSizesInfoKHR * pSizeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetClusterAccelerationStructureBuildSizesNV( device, pInfo, pSizeInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdBuildClusterAccelerationStructureIndirectNV( VkCommandBuffer                                      commandBuffer,
                                                          VkClusterAccelerationStructureCommandsInfoNV const * pCommandInfos ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildClusterAccelerationStructureIndirectNV( commandBuffer, pCommandInfos );
      }

      //=== VK_NV_partitioned_acceleration_structure ===

      VULKAN_HPP_INLINE void
        vkGetPartitionedAccelerationStructuresBuildSizesNV( VkDevice                                                   device,
                                                            VkPartitionedAccelerationStructureInstancesInputNV const * pInfo,
                                                            VkAccelerationStructureBuildSizesInfoKHR *                 pSizeInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPartitionedAccelerationStructuresBuildSizesNV( device, pInfo, pSizeInfo );
      }

      VULKAN_HPP_INLINE void
        vkCmdBuildPartitionedAccelerationStructuresNV( VkCommandBuffer                                       commandBuffer,
                                                       VkBuildPartitionedAccelerationStructureInfoNV const * pBuildInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBuildPartitionedAccelerationStructuresNV( commandBuffer, pBuildInfo );
      }

      //=== VK_EXT_device_generated_commands ===

      VULKAN_HPP_INLINE void vkGetGeneratedCommandsMemoryRequirementsEXT( VkDevice                                             device,
                                                                          VkGeneratedCommandsMemoryRequirementsInfoEXT const * pInfo,
                                                                          VkMemoryRequirements2 * pMemoryRequirements ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetGeneratedCommandsMemoryRequirementsEXT( device, pInfo, pMemoryRequirements );
      }

      VULKAN_HPP_INLINE void vkCmdPreprocessGeneratedCommandsEXT( VkCommandBuffer                    commandBuffer,
                                                                  VkGeneratedCommandsInfoEXT const * pGeneratedCommandsInfo,
                                                                  VkCommandBuffer                    stateCommandBuffer ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdPreprocessGeneratedCommandsEXT( commandBuffer, pGeneratedCommandsInfo, stateCommandBuffer );
      }

      VULKAN_HPP_INLINE void vkCmdExecuteGeneratedCommandsEXT( VkCommandBuffer                    commandBuffer,
                                                               VkBool32                           isPreprocessed,
                                                               VkGeneratedCommandsInfoEXT const * pGeneratedCommandsInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdExecuteGeneratedCommandsEXT( commandBuffer, isPreprocessed, pGeneratedCommandsInfo );
      }

      VULKAN_HPP_INLINE VkResult vkCreateIndirectCommandsLayoutEXT( VkDevice                                      device,
                                                                    VkIndirectCommandsLayoutCreateInfoEXT const * pCreateInfo,
                                                                    VkAllocationCallbacks const *                 pAllocator,
                                                                    VkIndirectCommandsLayoutEXT * pIndirectCommandsLayout ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateIndirectCommandsLayoutEXT( device, pCreateInfo, pAllocator, pIndirectCommandsLayout );
      }

      VULKAN_HPP_INLINE void vkDestroyIndirectCommandsLayoutEXT( VkDevice                      device,
                                                                 VkIndirectCommandsLayoutEXT   indirectCommandsLayout,
                                                                 VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyIndirectCommandsLayoutEXT( device, indirectCommandsLayout, pAllocator );
      }

      VULKAN_HPP_INLINE VkResult vkCreateIndirectExecutionSetEXT( VkDevice                                    device,
                                                                  VkIndirectExecutionSetCreateInfoEXT const * pCreateInfo,
                                                                  VkAllocationCallbacks const *               pAllocator,
                                                                  VkIndirectExecutionSetEXT *                 pIndirectExecutionSet ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateIndirectExecutionSetEXT( device, pCreateInfo, pAllocator, pIndirectExecutionSet );
      }

      VULKAN_HPP_INLINE void vkDestroyIndirectExecutionSetEXT( VkDevice                      device,
                                                               VkIndirectExecutionSetEXT     indirectExecutionSet,
                                                               VkAllocationCallbacks const * pAllocator ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkDestroyIndirectExecutionSetEXT( device, indirectExecutionSet, pAllocator );
      }

      VULKAN_HPP_INLINE void
        vkUpdateIndirectExecutionSetPipelineEXT( VkDevice                                       device,
                                                 VkIndirectExecutionSetEXT                      indirectExecutionSet,
                                                 uint32_t                                       executionSetWriteCount,
                                                 VkWriteIndirectExecutionSetPipelineEXT const * pExecutionSetWrites ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateIndirectExecutionSetPipelineEXT( device, indirectExecutionSet, executionSetWriteCount, pExecutionSetWrites );
      }

      VULKAN_HPP_INLINE void vkUpdateIndirectExecutionSetShaderEXT( VkDevice                                     device,
                                                                    VkIndirectExecutionSetEXT                    indirectExecutionSet,
                                                                    uint32_t                                     executionSetWriteCount,
                                                                    VkWriteIndirectExecutionSetShaderEXT const * pExecutionSetWrites ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkUpdateIndirectExecutionSetShaderEXT( device, indirectExecutionSet, executionSetWriteCount, pExecutionSetWrites );
      }

#    if defined( VK_USE_PLATFORM_OHOS )
      //=== VK_OHOS_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateSurfaceOHOS( VkInstance                      instance,
                                                      VkSurfaceCreateInfoOHOS const * pCreateInfo,
                                                      VkAllocationCallbacks const *   pAllocator,
                                                      VkSurfaceKHR *                  pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateSurfaceOHOS( instance, pCreateInfo, pAllocator, pSurface );
      }
#    endif /*VK_USE_PLATFORM_OHOS*/

      //=== VK_NV_cooperative_matrix2 ===

      VULKAN_HPP_INLINE VkResult vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(
        VkPhysicalDevice physicalDevice, uint32_t * pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV * pProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV( physicalDevice, pPropertyCount, pProperties );
      }

#    if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_external_memory_metal ===

      VULKAN_HPP_INLINE VkResult vkGetMemoryMetalHandleEXT( VkDevice                              device,
                                                            VkMemoryGetMetalHandleInfoEXT const * pGetMetalHandleInfo,
                                                            void **                               pHandle ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryMetalHandleEXT( device, pGetMetalHandleInfo, pHandle );
      }

      VULKAN_HPP_INLINE VkResult
        vkGetMemoryMetalHandlePropertiesEXT( VkDevice                           device,
                                             VkExternalMemoryHandleTypeFlagBits handleType,
                                             void const *                       pHandle,
                                             VkMemoryMetalHandlePropertiesEXT * pMemoryMetalHandleProperties ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetMemoryMetalHandlePropertiesEXT( device, handleType, pHandle, pMemoryMetalHandleProperties );
      }
#    endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_ARM_performance_counters_by_region ===

      VULKAN_HPP_INLINE VkResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM(
        VkPhysicalDevice                     physicalDevice,
        uint32_t                             queueFamilyIndex,
        uint32_t *                           pCounterCount,
        VkPerformanceCounterARM *            pCounters,
        VkPerformanceCounterDescriptionARM * pCounterDescriptions ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM(
          physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions );
      }

      //=== VK_EXT_fragment_density_map_offset ===

      VULKAN_HPP_INLINE void vkCmdEndRendering2EXT( VkCommandBuffer commandBuffer, VkRenderingEndInfoKHR const * pRenderingEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRendering2EXT( commandBuffer, pRenderingEndInfo );
      }

      //=== VK_EXT_custom_resolve ===

      VULKAN_HPP_INLINE void vkCmdBeginCustomResolveEXT( VkCommandBuffer                     commandBuffer,
                                                         VkBeginCustomResolveInfoEXT const * pBeginCustomResolveInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdBeginCustomResolveEXT( commandBuffer, pBeginCustomResolveInfo );
      }

      //=== VK_KHR_maintenance10 ===

      VULKAN_HPP_INLINE void vkCmdEndRendering2KHR( VkCommandBuffer commandBuffer, VkRenderingEndInfoKHR const * pRenderingEndInfo ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdEndRendering2KHR( commandBuffer, pRenderingEndInfo );
      }

      //=== VK_NV_compute_occupancy_priority ===

      VULKAN_HPP_INLINE void vkCmdSetComputeOccupancyPriorityNV( VkCommandBuffer                                commandBuffer,
                                                                 VkComputeOccupancyPriorityParametersNV const * pParameters ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCmdSetComputeOccupancyPriorityNV( commandBuffer, pParameters );
      }

#    if defined( VK_USE_PLATFORM_UBM_SEC )
      //=== VK_SEC_ubm_surface ===

      VULKAN_HPP_INLINE VkResult vkCreateUbmSurfaceSEC( VkInstance                        instance,
                                                        VkUbmSurfaceCreateInfoSEC const * pCreateInfo,
                                                        VkAllocationCallbacks const *     pAllocator,
                                                        VkSurfaceKHR *                    pSurface ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkCreateUbmSurfaceSEC( instance, pCreateInfo, pAllocator, pSurface );
      }

      VULKAN_HPP_INLINE VkBool32 vkGetPhysicalDeviceUbmPresentationSupportSEC( VkPhysicalDevice    physicalDevice,
                                                                               uint32_t            queueFamilyIndex,
                                                                               struct ubm_device * device ) const VULKAN_HPP_NOEXCEPT
      {
        return ::vkGetPhysicalDeviceUbmPresentationSupportSEC( physicalDevice, queueFamilyIndex, device );
      }
#    endif /*VK_USE_PLATFORM_UBM_SEC*/
#  endif   /*VK_ONLY_EXPORTED_PROTOTYPES*/
    };

    inline DispatchLoaderStatic & getDispatchLoaderStatic()
    {
      static DispatchLoaderStatic dls;
      return dls;
    }
#endif
  }  // namespace detail
#if ( 14 <= VULKAN_HPP_CPP_VERSION )
  using std::exchange;
#else
  template <class T, class U = T>
  VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_INLINE T exchange( T & obj, U && newValue )
  {
    T oldValue = std::move( obj );
    obj        = std::forward<U>( newValue );
    return oldValue;
  }
#endif

  struct AllocationCallbacks;

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

  namespace detail
  {
    template <typename OwnerType>
    class ObjectDestroy
    {
    public:
      ObjectDestroy() = default;

      ObjectDestroy( OwnerType                                               owner,
                     Optional<AllocationCallbacks const> allocationCallbacks VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr ),
                     DispatchLoader const & dispatch                         VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
        : m_owner( owner )
        , m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {
      }

      OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
      {
        return m_owner;
      }

      Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
      {
        return m_allocationCallbacks;
      }

      DispatchLoader const & getDispatch() const VULKAN_HPP_NOEXCEPT
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
      Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
      DispatchLoader const *              m_dispatch            = nullptr;
    };

    class NoParent;

    template <>
    class ObjectDestroy<NoParent>
    {
    public:
      ObjectDestroy() = default;

      ObjectDestroy( Optional<AllocationCallbacks const> allocationCallbacks,
                     DispatchLoader const & dispatch     VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
        : m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {
      }

      Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
      {
        return m_allocationCallbacks;
      }

      DispatchLoader const & getDispatch() const VULKAN_HPP_NOEXCEPT
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
      Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
      DispatchLoader const *              m_dispatch            = nullptr;
    };

    template <typename OwnerType, typename Dispatch>
    class DummyDestroy
    {
    public:
      DummyDestroy() = default;

      DummyDestroy( OwnerType                                               owner,
                    Optional<AllocationCallbacks const> allocationCallbacks VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr ),
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

      Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
      {
        return m_allocationCallbacks;
      }

      Dispatch const & getDispatch() const VULKAN_HPP_NOEXCEPT
      {
        return *m_dispatch;
      }

    protected:
      template <typename T>
      void destroy( T /*t*/ ) VULKAN_HPP_NOEXCEPT
      {
      }

    private:
      OwnerType                           m_owner               = {};
      Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
      Dispatch const *                    m_dispatch            = nullptr;
    };

    template <typename OwnerType, typename Dispatch>
    class ObjectFree
    {
    public:
      ObjectFree() = default;

      ObjectFree( OwnerType                                               owner,
                  Optional<AllocationCallbacks const> allocationCallbacks VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr ),
                  DispatchLoader const & dispatch                         VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
        : m_owner( owner )
        , m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {
      }

      OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
      {
        return m_owner;
      }

      Optional<AllocationCallbacks const> getAllocator() const VULKAN_HPP_NOEXCEPT
      {
        return m_allocationCallbacks;
      }

      DispatchLoader const & getDispatch() const VULKAN_HPP_NOEXCEPT
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
      Optional<AllocationCallbacks const> m_allocationCallbacks = nullptr;
      DispatchLoader const *              m_dispatch            = nullptr;
    };

    template <typename OwnerType>
    class ObjectRelease
    {
    public:
      ObjectRelease() = default;

      ObjectRelease( OwnerType owner, DispatchLoader const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
        : m_owner( owner )
        , m_dispatch( &dispatch )
      {
      }

      OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
      {
        return m_owner;
      }

      DispatchLoader const & getDispatch() const VULKAN_HPP_NOEXCEPT
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
      OwnerType              m_owner    = {};
      DispatchLoader const * m_dispatch = nullptr;
    };

    template <typename OwnerType, typename PoolType>
    class PoolFree
    {
    public:
      PoolFree() = default;

      PoolFree( OwnerType owner, PoolType pool, DispatchLoader const & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) VULKAN_HPP_NOEXCEPT
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

      DispatchLoader const & getDispatch() const VULKAN_HPP_NOEXCEPT
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
      OwnerType              m_owner    = OwnerType();
      PoolType               m_pool     = PoolType();
      DispatchLoader const * m_dispatch = nullptr;
    };
  }  // namespace detail
#endif  // !VULKAN_HPP_NO_SMART_HANDLE

  //==================
  //=== BASE TYPEs ===
  //==================

  using Bool32          = uint32_t;
  using DeviceAddress   = uint64_t;
  using DeviceSize      = uint64_t;
  using RemoteAddressNV = void *;
  using SampleMask      = uint32_t;

  template <typename Type, Type value = Type{}>
  struct CppType
  {
  };
}  // namespace VULKAN_HPP_NAMESPACE

#include <vulkan/vulkan_enums.hpp>
#if !defined( VULKAN_HPP_NO_TO_STRING )
#  include <vulkan/vulkan_to_string.hpp>
#endif

#ifndef VULKAN_HPP_NO_EXCEPTIONS
VULKAN_HPP_EXPORT namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {
  };
}  // namespace std
#endif

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
#ifndef VULKAN_HPP_NO_EXCEPTIONS
  class ErrorCategoryImpl : public std::error_category
  {
  public:
    virtual char const * name() const VULKAN_HPP_NOEXCEPT override
    {
      return VULKAN_HPP_NAMESPACE_STRING "::Result";
    }

    virtual std::string message( int ev ) const override
    {
#  if defined( VULKAN_HPP_NO_TO_STRING )
      return std::to_string( ev );
#  else
      return to_string( static_cast<Result>( ev ) );
#  endif
    }
  };

  class Error
  {
  public:
    Error() VULKAN_HPP_NOEXCEPT                = default;
    Error( Error const & ) VULKAN_HPP_NOEXCEPT = default;
    virtual ~Error() VULKAN_HPP_NOEXCEPT       = default;

    virtual char const * what() const VULKAN_HPP_NOEXCEPT = 0;
  };

  class LogicError
    : public Error
    , public std::logic_error
  {
  public:
    explicit LogicError( std::string const & what ) : Error(), std::logic_error( what ) {}

    explicit LogicError( char const * what ) : Error(), std::logic_error( what ) {}

    virtual char const * what() const VULKAN_HPP_NOEXCEPT
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

    virtual char const * what() const VULKAN_HPP_NOEXCEPT
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

  class ValidationFailedError : public SystemError
  {
  public:
    ValidationFailedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorValidationFailed ), message ) {}

    ValidationFailedError( char const * message ) : SystemError( make_error_code( Result::eErrorValidationFailed ), message ) {}
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

  class InvalidOpaqueCaptureAddressError : public SystemError
  {
  public:
    InvalidOpaqueCaptureAddressError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidOpaqueCaptureAddress ), message ) {}

    InvalidOpaqueCaptureAddressError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidOpaqueCaptureAddress ), message ) {}
  };

  class FragmentationError : public SystemError
  {
  public:
    FragmentationError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFragmentation ), message ) {}

    FragmentationError( char const * message ) : SystemError( make_error_code( Result::eErrorFragmentation ), message ) {}
  };

  class NotPermittedError : public SystemError
  {
  public:
    NotPermittedError( std::string const & message ) : SystemError( make_error_code( Result::eErrorNotPermitted ), message ) {}

    NotPermittedError( char const * message ) : SystemError( make_error_code( Result::eErrorNotPermitted ), message ) {}
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

  class InvalidShaderNVError : public SystemError
  {
  public:
    InvalidShaderNVError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidShaderNV ), message ) {}

    InvalidShaderNVError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidShaderNV ), message ) {}
  };

  class ImageUsageNotSupportedKHRError : public SystemError
  {
  public:
    ImageUsageNotSupportedKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorImageUsageNotSupportedKHR ), message ) {}

    ImageUsageNotSupportedKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorImageUsageNotSupportedKHR ), message ) {}
  };

  class VideoPictureLayoutNotSupportedKHRError : public SystemError
  {
  public:
    VideoPictureLayoutNotSupportedKHRError( std::string const & message )
      : SystemError( make_error_code( Result::eErrorVideoPictureLayoutNotSupportedKHR ), message )
    {
    }

    VideoPictureLayoutNotSupportedKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorVideoPictureLayoutNotSupportedKHR ), message )
    {
    }
  };

  class VideoProfileOperationNotSupportedKHRError : public SystemError
  {
  public:
    VideoProfileOperationNotSupportedKHRError( std::string const & message )
      : SystemError( make_error_code( Result::eErrorVideoProfileOperationNotSupportedKHR ), message )
    {
    }

    VideoProfileOperationNotSupportedKHRError( char const * message )
      : SystemError( make_error_code( Result::eErrorVideoProfileOperationNotSupportedKHR ), message )
    {
    }
  };

  class VideoProfileFormatNotSupportedKHRError : public SystemError
  {
  public:
    VideoProfileFormatNotSupportedKHRError( std::string const & message )
      : SystemError( make_error_code( Result::eErrorVideoProfileFormatNotSupportedKHR ), message )
    {
    }

    VideoProfileFormatNotSupportedKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorVideoProfileFormatNotSupportedKHR ), message )
    {
    }
  };

  class VideoProfileCodecNotSupportedKHRError : public SystemError
  {
  public:
    VideoProfileCodecNotSupportedKHRError( std::string const & message )
      : SystemError( make_error_code( Result::eErrorVideoProfileCodecNotSupportedKHR ), message )
    {
    }

    VideoProfileCodecNotSupportedKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorVideoProfileCodecNotSupportedKHR ), message ) {}
  };

  class VideoStdVersionNotSupportedKHRError : public SystemError
  {
  public:
    VideoStdVersionNotSupportedKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorVideoStdVersionNotSupportedKHR ), message )
    {
    }

    VideoStdVersionNotSupportedKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorVideoStdVersionNotSupportedKHR ), message ) {}
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

  class PresentTimingQueueFullEXTError : public SystemError
  {
  public:
    PresentTimingQueueFullEXTError( std::string const & message ) : SystemError( make_error_code( Result::eErrorPresentTimingQueueFullEXT ), message ) {}

    PresentTimingQueueFullEXTError( char const * message ) : SystemError( make_error_code( Result::eErrorPresentTimingQueueFullEXT ), message ) {}
  };

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  class FullScreenExclusiveModeLostEXTError : public SystemError
  {
  public:
    FullScreenExclusiveModeLostEXTError( std::string const & message ) : SystemError( make_error_code( Result::eErrorFullScreenExclusiveModeLostEXT ), message )
    {
    }

    FullScreenExclusiveModeLostEXTError( char const * message ) : SystemError( make_error_code( Result::eErrorFullScreenExclusiveModeLostEXT ), message ) {}
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

  class InvalidVideoStdParametersKHRError : public SystemError
  {
  public:
    InvalidVideoStdParametersKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorInvalidVideoStdParametersKHR ), message ) {}

    InvalidVideoStdParametersKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorInvalidVideoStdParametersKHR ), message ) {}
  };

  class CompressionExhaustedEXTError : public SystemError
  {
  public:
    CompressionExhaustedEXTError( std::string const & message ) : SystemError( make_error_code( Result::eErrorCompressionExhaustedEXT ), message ) {}

    CompressionExhaustedEXTError( char const * message ) : SystemError( make_error_code( Result::eErrorCompressionExhaustedEXT ), message ) {}
  };

  class NotEnoughSpaceKHRError : public SystemError
  {
  public:
    NotEnoughSpaceKHRError( std::string const & message ) : SystemError( make_error_code( Result::eErrorNotEnoughSpaceKHR ), message ) {}

    NotEnoughSpaceKHRError( char const * message ) : SystemError( make_error_code( Result::eErrorNotEnoughSpaceKHR ), message ) {}
  };

  namespace detail
  {
    [[noreturn]] VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
    {
      switch ( result )
      {
        case Result::eErrorOutOfHostMemory                       : throw OutOfHostMemoryError( message );
        case Result::eErrorOutOfDeviceMemory                     : throw OutOfDeviceMemoryError( message );
        case Result::eErrorInitializationFailed                  : throw InitializationFailedError( message );
        case Result::eErrorDeviceLost                            : throw DeviceLostError( message );
        case Result::eErrorMemoryMapFailed                       : throw MemoryMapFailedError( message );
        case Result::eErrorLayerNotPresent                       : throw LayerNotPresentError( message );
        case Result::eErrorExtensionNotPresent                   : throw ExtensionNotPresentError( message );
        case Result::eErrorFeatureNotPresent                     : throw FeatureNotPresentError( message );
        case Result::eErrorIncompatibleDriver                    : throw IncompatibleDriverError( message );
        case Result::eErrorTooManyObjects                        : throw TooManyObjectsError( message );
        case Result::eErrorFormatNotSupported                    : throw FormatNotSupportedError( message );
        case Result::eErrorFragmentedPool                        : throw FragmentedPoolError( message );
        case Result::eErrorUnknown                               : throw UnknownError( message );
        case Result::eErrorValidationFailed                      : throw ValidationFailedError( message );
        case Result::eErrorOutOfPoolMemory                       : throw OutOfPoolMemoryError( message );
        case Result::eErrorInvalidExternalHandle                 : throw InvalidExternalHandleError( message );
        case Result::eErrorInvalidOpaqueCaptureAddress           : throw InvalidOpaqueCaptureAddressError( message );
        case Result::eErrorFragmentation                         : throw FragmentationError( message );
        case Result::eErrorNotPermitted                          : throw NotPermittedError( message );
        case Result::eErrorSurfaceLostKHR                        : throw SurfaceLostKHRError( message );
        case Result::eErrorNativeWindowInUseKHR                  : throw NativeWindowInUseKHRError( message );
        case Result::eErrorOutOfDateKHR                          : throw OutOfDateKHRError( message );
        case Result::eErrorIncompatibleDisplayKHR                : throw IncompatibleDisplayKHRError( message );
        case Result::eErrorInvalidShaderNV                       : throw InvalidShaderNVError( message );
        case Result::eErrorImageUsageNotSupportedKHR             : throw ImageUsageNotSupportedKHRError( message );
        case Result::eErrorVideoPictureLayoutNotSupportedKHR     : throw VideoPictureLayoutNotSupportedKHRError( message );
        case Result::eErrorVideoProfileOperationNotSupportedKHR  : throw VideoProfileOperationNotSupportedKHRError( message );
        case Result::eErrorVideoProfileFormatNotSupportedKHR     : throw VideoProfileFormatNotSupportedKHRError( message );
        case Result::eErrorVideoProfileCodecNotSupportedKHR      : throw VideoProfileCodecNotSupportedKHRError( message );
        case Result::eErrorVideoStdVersionNotSupportedKHR        : throw VideoStdVersionNotSupportedKHRError( message );
        case Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT: throw InvalidDrmFormatModifierPlaneLayoutEXTError( message );
        case Result::eErrorPresentTimingQueueFullEXT             : throw PresentTimingQueueFullEXTError( message );
#  if defined( VK_USE_PLATFORM_WIN32_KHR )
        case Result::eErrorFullScreenExclusiveModeLostEXT: throw FullScreenExclusiveModeLostEXTError( message );
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/
        case Result::eErrorInvalidVideoStdParametersKHR: throw InvalidVideoStdParametersKHRError( message );
        case Result::eErrorCompressionExhaustedEXT     : throw CompressionExhaustedEXTError( message );
        case Result::eErrorNotEnoughSpaceKHR           : throw NotEnoughSpaceKHRError( message );
        default                                        : throw SystemError( make_error_code( result ), message );
      }
    }
  }  // namespace detail
#endif

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

    std::tuple<Result, T> asTuple() &&
    {
      return std::make_tuple( result, std::move( value ) );
    }

    // std::expected-look alike
    bool has_value() const VULKAN_HPP_NOEXCEPT
    {
      return result == vk::Result::eSuccess;
    }

    T const * operator->() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( has_value() );
      return &value;
    }

    T * operator->() VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( has_value() );
      return &value;
    }

    T const & operator*() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( has_value() );
      return value;
    }

    T & operator*() VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( has_value() );
      return value;
    }
  };

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
#  ifdef VULKAN_HPP_EXPECTED
    using type = VULKAN_HPP_EXPECTED<T, Result>;
#  else
    using type = ResultValue<T>;
#  endif
#else
    using type = T;
#endif
  };

  template <>
  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
#  ifdef VULKAN_HPP_EXPECTED
    using type = VULKAN_HPP_EXPECTED<void, Result>;
#  else
    using type = Result;
#  endif
#else
    using type = void;
#endif
  };

  namespace detail
  {
    template <typename T>
    VULKAN_HPP_CONSTEXPR bool ignore( T const & ) VULKAN_HPP_NOEXCEPT
    {
      return true;
    }

    VULKAN_HPP_INLINE typename ResultValueType<void>::type createResultValueType( Result result )
    {
#if defined( VULKAN_HPP_NO_EXCEPTIONS )
#  ifdef VULKAN_HPP_UNEXPECTED
      if ( result == Result::eSuccess )
      {
        return {};
      }
      return VULKAN_HPP_UNEXPECTED( result );
#  else
      return result;
#  endif
#else
      ignore( result );
#endif
    }

    template <typename T>
    VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T & data )
    {
#if defined( VULKAN_HPP_NO_EXCEPTIONS )
#  ifdef VULKAN_HPP_EXPECTED
      if ( result == Result::eSuccess )
      {
        return data;
      }
      return VULKAN_HPP_UNEXPECTED( data );
#  else
      return ResultValue<T>( result, data );
#  endif
#else
      ignore( result );
      return data;
#endif
    }

    template <typename T>
    VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T && data )
    {
#if defined( VULKAN_HPP_NO_EXCEPTIONS )
#  ifdef VULKAN_HPP_EXPECTED
      if ( result == Result::eSuccess )
      {
        return std::move( data );
      }
      return VULKAN_HPP_UNEXPECTED( result );
#  else
      return ResultValue<T>( result, std::move( data ) );
#  endif
#else
      ignore( result );
      return std::move( data );
#endif
    }
  }  // namespace detail

  namespace detail
  {
    VULKAN_HPP_INLINE void resultCheck( Result result, char const * message )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      ignore( message );
      VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
#else
      if ( result != Result::eSuccess )
      {
        throwResultException( result, message );
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
        throwResultException( result, message );
      }
#endif
    }

    template <typename HandleType, typename AllocatorType>
    VULKAN_HPP_INLINE void resultCheck( Result                                         result,
                                        char const *                                   message,
                                        std::initializer_list<Result>                  successCodes,
                                        VkDevice                                       device,
                                        std::vector<HandleType, AllocatorType> const & pipelines,
                                        AllocationCallbacks const *                    pAllocator,
                                        DispatchLoader const &                         d )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      ignore( message );
      ignore( successCodes );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      ignore( device );
      ignore( pipelines );
      ignore( pAllocator );
      ignore( d );
      VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
      if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
      {
        for ( HandleType pipeline : pipelines )
        {
          d.vkDestroyPipeline( device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<VkAllocationCallbacks const *>( pAllocator ) );
        }
        throwResultException( result, message );
      }
#endif
    }
  }  // namespace detail

  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================

  //=== VK_VERSION_1_0 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t False                     = VK_FALSE;
  VULKAN_HPP_CONSTEXPR_INLINE float    LodClampNone              = VK_LOD_CLAMP_NONE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyIgnored        = VK_QUEUE_FAMILY_IGNORED;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t RemainingArrayLayers      = VK_REMAINING_ARRAY_LAYERS;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t RemainingMipLevels        = VK_REMAINING_MIP_LEVELS;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t True                      = VK_TRUE;
  VULKAN_HPP_CONSTEXPR_INLINE uint64_t WholeSize                 = VK_WHOLE_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxMemoryTypes            = VK_MAX_MEMORY_TYPES;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxPhysicalDeviceNameSize = VK_MAX_PHYSICAL_DEVICE_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t UuidSize                  = VK_UUID_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxExtensionNameSize      = VK_MAX_EXTENSION_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDescriptionSize        = VK_MAX_DESCRIPTION_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxMemoryHeaps            = VK_MAX_MEMORY_HEAPS;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t AttachmentUnused          = VK_ATTACHMENT_UNUSED;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t SubpassExternal           = VK_SUBPASS_EXTERNAL;

  //=== VK_VERSION_1_1 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDeviceGroupSize  = VK_MAX_DEVICE_GROUP_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t LuidSize            = VK_LUID_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyExternal = VK_QUEUE_FAMILY_EXTERNAL;

  //=== VK_VERSION_1_2 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverNameSize = VK_MAX_DRIVER_NAME_SIZE;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverInfoSize = VK_MAX_DRIVER_INFO_SIZE;

  //=== VK_VERSION_1_4 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxGlobalPrioritySize = VK_MAX_GLOBAL_PRIORITY_SIZE;

  //=== VK_KHR_device_group_creation ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDeviceGroupSizeKHR = VK_MAX_DEVICE_GROUP_SIZE_KHR;

  //=== VK_KHR_external_memory_capabilities ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t LuidSizeKHR = VK_LUID_SIZE_KHR;

  //=== VK_KHR_external_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyExternalKHR = VK_QUEUE_FAMILY_EXTERNAL_KHR;

  //=== VK_EXT_queue_family_foreign ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t QueueFamilyForeignEXT = VK_QUEUE_FAMILY_FOREIGN_EXT;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_shader_enqueue ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t ShaderIndexUnusedAMDX = VK_SHADER_INDEX_UNUSED_AMDX;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_KHR_ray_tracing_pipeline ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t ShaderUnusedKHR = VK_SHADER_UNUSED_KHR;

  //=== VK_NV_ray_tracing ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t ShaderUnusedNV = VK_SHADER_UNUSED_NV;

  //=== VK_KHR_global_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxGlobalPrioritySizeKHR = VK_MAX_GLOBAL_PRIORITY_SIZE_KHR;

  //=== VK_KHR_driver_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverNameSizeKHR = VK_MAX_DRIVER_NAME_SIZE_KHR;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxDriverInfoSizeKHR = VK_MAX_DRIVER_INFO_SIZE_KHR;

  //=== VK_EXT_global_priority_query ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxGlobalPrioritySizeEXT = VK_MAX_GLOBAL_PRIORITY_SIZE_EXT;

  //=== VK_EXT_image_sliced_view_of_3d ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t Remaining3DSlicesEXT = VK_REMAINING_3D_SLICES_EXT;

  //=== VK_EXT_shader_module_identifier ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxShaderModuleIdentifierSizeEXT = VK_MAX_SHADER_MODULE_IDENTIFIER_SIZE_EXT;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_dense_geometry_format ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t CompressedTriangleFormatDgf1ByteAlignmentAMDX = VK_COMPRESSED_TRIANGLE_FORMAT_DGF1_BYTE_ALIGNMENT_AMDX;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t CompressedTriangleFormatDgf1ByteStrideAMDX    = VK_COMPRESSED_TRIANGLE_FORMAT_DGF1_BYTE_STRIDE_AMDX;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_KHR_pipeline_binary ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxPipelineBinaryKeySizeKHR = VK_MAX_PIPELINE_BINARY_KEY_SIZE_KHR;

  //=== VK_ARM_data_graph ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxPhysicalDeviceDataGraphOperationSetNameSizeARM = VK_MAX_PHYSICAL_DEVICE_DATA_GRAPH_OPERATION_SET_NAME_SIZE_ARM;

  //=== VK_KHR_video_decode_av1 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxVideoAv1ReferencesPerFrameKHR = VK_MAX_VIDEO_AV1_REFERENCES_PER_FRAME_KHR;

  //=== VK_KHR_video_decode_vp9 ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t MaxVideoVp9ReferencesPerFrameKHR = VK_MAX_VIDEO_VP9_REFERENCES_PER_FRAME_KHR;

  //=== VK_NV_partitioned_acceleration_structure ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t PartitionedAccelerationStructurePartitionIndexGlobalNV = VK_PARTITIONED_ACCELERATION_STRUCTURE_PARTITION_INDEX_GLOBAL_NV;

  //=== VK_QCOM_data_graph_model ===
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t DataGraphModelToolchainVersionLengthQCOM = VK_DATA_GRAPH_MODEL_TOOLCHAIN_VERSION_LENGTH_QCOM;

  //=== VK_NV_compute_occupancy_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE float ComputeOccupancyPriorityLowNV    = VK_COMPUTE_OCCUPANCY_PRIORITY_LOW_NV;
  VULKAN_HPP_CONSTEXPR_INLINE float ComputeOccupancyPriorityNormalNV = VK_COMPUTE_OCCUPANCY_PRIORITY_NORMAL_NV;
  VULKAN_HPP_CONSTEXPR_INLINE float ComputeOccupancyPriorityHighNV   = VK_COMPUTE_OCCUPANCY_PRIORITY_HIGH_NV;

  //========================
  //=== CONSTEXPR VALUEs ===
  //========================
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t HeaderVersion      = VK_HEADER_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE uint32_t Use64BitPtrDefines = VK_USE_64_BIT_PTR_DEFINES;

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
    return ( (uint32_t)( version ) & 0xFFFU );
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
  VULKAN_HPP_CONSTEXPR uint32_t makeVersion( T const major, T const minor, T const patch )
  {
    return ( ( ( (uint32_t)( major ) ) << 22U ) | ( ( (uint32_t)( minor ) ) << 12U ) | ( (uint32_t)( patch ) ) );
  }
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t versionMajor( T const version )
  {
    return ( (uint32_t)( version ) >> 22U );
  }
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t versionMinor( T const version )
  {
    return ( ( (uint32_t)( version ) >> 12U ) & 0x3FFU );
  }
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  VULKAN_HPP_CONSTEXPR uint32_t versionPatch( T const version )
  {
    return ( (uint32_t)( version ) & 0xFFFU );
  }

  //=========================
  //=== CONSTEXPR CALLERs ===
  //=========================
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion            = makeApiVersion( 0, 1, 0, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion10          = makeApiVersion( 0, 1, 0, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion11          = makeApiVersion( 0, 1, 1, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion12          = makeApiVersion( 0, 1, 2, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion13          = makeApiVersion( 0, 1, 3, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto ApiVersion14          = makeApiVersion( 0, 1, 4, 0 );
  VULKAN_HPP_CONSTEXPR_INLINE auto HeaderVersionComplete = makeApiVersion( 0, 1, 4, VK_HEADER_VERSION );

  //=================================
  //=== CONSTEXPR EXTENSION NAMEs ===
  //=================================

  //=== VK_KHR_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceSpecVersion   = VK_KHR_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceExtensionName = VK_KHR_SURFACE_EXTENSION_NAME;

  //=== VK_KHR_swapchain ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainSpecVersion   = VK_KHR_SWAPCHAIN_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainExtensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

  //=== VK_KHR_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySpecVersion   = VK_KHR_DISPLAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplayExtensionName = VK_KHR_DISPLAY_EXTENSION_NAME;

  //=== VK_KHR_display_swapchain ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySwapchainSpecVersion   = VK_KHR_DISPLAY_SWAPCHAIN_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDisplaySwapchainExtensionName = VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_XLIB_KHR )
  //=== VK_KHR_xlib_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRXlibSurfaceSpecVersion   = VK_KHR_XLIB_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRXlibSurfaceExtensionName = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#if defined( VK_USE_PLATFORM_XCB_KHR )
  //=== VK_KHR_xcb_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRXcbSurfaceSpecVersion   = VK_KHR_XCB_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRXcbSurfaceExtensionName = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#if defined( VK_USE_PLATFORM_WAYLAND_KHR )
  //=== VK_KHR_wayland_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWaylandSurfaceSpecVersion   = VK_KHR_WAYLAND_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWaylandSurfaceExtensionName = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
  //=== VK_KHR_android_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRAndroidSurfaceSpecVersion   = VK_KHR_ANDROID_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRAndroidSurfaceExtensionName = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_win32_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWin32SurfaceSpecVersion   = VK_KHR_WIN32_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWin32SurfaceExtensionName = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_EXT_debug_report ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_debug_report extension has been deprecated by VK_EXT_debug_utils." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugReportSpecVersion = VK_EXT_DEBUG_REPORT_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_debug_report extension has been deprecated by VK_EXT_debug_utils." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugReportExtensionName = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;

  //=== VK_NV_glsl_shader ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_glsl_shader extension has been deprecated." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVGlslShaderSpecVersion = VK_NV_GLSL_SHADER_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_glsl_shader extension has been deprecated." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVGlslShaderExtensionName = VK_NV_GLSL_SHADER_EXTENSION_NAME;

  //=== VK_EXT_depth_range_unrestricted ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthRangeUnrestrictedSpecVersion   = VK_EXT_DEPTH_RANGE_UNRESTRICTED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthRangeUnrestrictedExtensionName = VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME;

  //=== VK_KHR_sampler_mirror_clamp_to_edge ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSamplerMirrorClampToEdgeSpecVersion   = VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSamplerMirrorClampToEdgeExtensionName = VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME;

  //=== VK_IMG_filter_cubic ===
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGFilterCubicSpecVersion   = VK_IMG_FILTER_CUBIC_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGFilterCubicExtensionName = VK_IMG_FILTER_CUBIC_EXTENSION_NAME;

  //=== VK_AMD_rasterization_order ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDRasterizationOrderSpecVersion   = VK_AMD_RASTERIZATION_ORDER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDRasterizationOrderExtensionName = VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME;

  //=== VK_AMD_shader_trinary_minmax ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderTrinaryMinmaxSpecVersion   = VK_AMD_SHADER_TRINARY_MINMAX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderTrinaryMinmaxExtensionName = VK_AMD_SHADER_TRINARY_MINMAX_EXTENSION_NAME;

  //=== VK_AMD_shader_explicit_vertex_parameter ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderExplicitVertexParameterSpecVersion   = VK_AMD_SHADER_EXPLICIT_VERTEX_PARAMETER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderExplicitVertexParameterExtensionName = VK_AMD_SHADER_EXPLICIT_VERTEX_PARAMETER_EXTENSION_NAME;

  //=== VK_EXT_debug_marker ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugMarkerSpecVersion   = VK_EXT_DEBUG_MARKER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugMarkerExtensionName = VK_EXT_DEBUG_MARKER_EXTENSION_NAME;

  //=== VK_KHR_video_queue ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoQueueSpecVersion   = VK_KHR_VIDEO_QUEUE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoQueueExtensionName = VK_KHR_VIDEO_QUEUE_EXTENSION_NAME;

  //=== VK_KHR_video_decode_queue ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeQueueSpecVersion   = VK_KHR_VIDEO_DECODE_QUEUE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeQueueExtensionName = VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME;

  //=== VK_AMD_gcn_shader ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGcnShaderSpecVersion   = VK_AMD_GCN_SHADER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGcnShaderExtensionName = VK_AMD_GCN_SHADER_EXTENSION_NAME;

  //=== VK_NV_dedicated_allocation ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_dedicated_allocation extension has been deprecated by VK_KHR_dedicated_allocation." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDedicatedAllocationSpecVersion = VK_NV_DEDICATED_ALLOCATION_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_dedicated_allocation extension has been deprecated by VK_KHR_dedicated_allocation." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDedicatedAllocationExtensionName = VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME;

  //=== VK_EXT_transform_feedback ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTransformFeedbackSpecVersion   = VK_EXT_TRANSFORM_FEEDBACK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTransformFeedbackExtensionName = VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME;

  //=== VK_NVX_binary_import ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXBinaryImportSpecVersion   = VK_NVX_BINARY_IMPORT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXBinaryImportExtensionName = VK_NVX_BINARY_IMPORT_EXTENSION_NAME;

  //=== VK_NVX_image_view_handle ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXImageViewHandleSpecVersion   = VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXImageViewHandleExtensionName = VK_NVX_IMAGE_VIEW_HANDLE_EXTENSION_NAME;

  //=== VK_AMD_draw_indirect_count ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDrawIndirectCountSpecVersion   = VK_AMD_DRAW_INDIRECT_COUNT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDrawIndirectCountExtensionName = VK_AMD_DRAW_INDIRECT_COUNT_EXTENSION_NAME;

  //=== VK_AMD_negative_viewport_height ===
  VULKAN_HPP_DEPRECATED( "The VK_AMD_negative_viewport_height extension has been obsoleted by VK_KHR_maintenance1." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDNegativeViewportHeightSpecVersion = VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_AMD_negative_viewport_height extension has been obsoleted by VK_KHR_maintenance1." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDNegativeViewportHeightExtensionName = VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME;

  //=== VK_AMD_gpu_shader_half_float ===
  VULKAN_HPP_DEPRECATED( "The VK_AMD_gpu_shader_half_float extension has been deprecated by VK_KHR_shader_float16_int8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGpuShaderHalfFloatSpecVersion = VK_AMD_GPU_SHADER_HALF_FLOAT_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_AMD_gpu_shader_half_float extension has been deprecated by VK_KHR_shader_float16_int8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGpuShaderHalfFloatExtensionName = VK_AMD_GPU_SHADER_HALF_FLOAT_EXTENSION_NAME;

  //=== VK_AMD_shader_ballot ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderBallotSpecVersion   = VK_AMD_SHADER_BALLOT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderBallotExtensionName = VK_AMD_SHADER_BALLOT_EXTENSION_NAME;

  //=== VK_KHR_video_encode_h264 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeH264SpecVersion   = VK_KHR_VIDEO_ENCODE_H264_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeH264ExtensionName = VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME;

  //=== VK_KHR_video_encode_h265 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeH265SpecVersion   = VK_KHR_VIDEO_ENCODE_H265_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeH265ExtensionName = VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME;

  //=== VK_KHR_video_decode_h264 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeH264SpecVersion   = VK_KHR_VIDEO_DECODE_H264_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeH264ExtensionName = VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME;

  //=== VK_AMD_texture_gather_bias_lod ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDTextureGatherBiasLodSpecVersion   = VK_AMD_TEXTURE_GATHER_BIAS_LOD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDTextureGatherBiasLodExtensionName = VK_AMD_TEXTURE_GATHER_BIAS_LOD_EXTENSION_NAME;

  //=== VK_AMD_shader_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderInfoSpecVersion   = VK_AMD_SHADER_INFO_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderInfoExtensionName = VK_AMD_SHADER_INFO_EXTENSION_NAME;

  //=== VK_KHR_dynamic_rendering ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDynamicRenderingSpecVersion   = VK_KHR_DYNAMIC_RENDERING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDynamicRenderingExtensionName = VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME;

  //=== VK_AMD_shader_image_load_store_lod ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderImageLoadStoreLodSpecVersion   = VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderImageLoadStoreLodExtensionName = VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_GGP )
  //=== VK_GGP_stream_descriptor_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GGPStreamDescriptorSurfaceSpecVersion   = VK_GGP_STREAM_DESCRIPTOR_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GGPStreamDescriptorSurfaceExtensionName = VK_GGP_STREAM_DESCRIPTOR_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_GGP*/

  //=== VK_NV_corner_sampled_image ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCornerSampledImageSpecVersion   = VK_NV_CORNER_SAMPLED_IMAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCornerSampledImageExtensionName = VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME;

  //=== VK_KHR_multiview ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMultiviewSpecVersion   = VK_KHR_MULTIVIEW_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMultiviewExtensionName = VK_KHR_MULTIVIEW_EXTENSION_NAME;

  //=== VK_IMG_format_pvrtc ===
  VULKAN_HPP_DEPRECATED( "The VK_IMG_format_pvrtc extension has been deprecated." )
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGFormatPvrtcSpecVersion = VK_IMG_FORMAT_PVRTC_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_IMG_format_pvrtc extension has been deprecated." )
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGFormatPvrtcExtensionName = VK_IMG_FORMAT_PVRTC_EXTENSION_NAME;

  //=== VK_NV_external_memory_capabilities ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory_capabilities extension has been deprecated by VK_KHR_external_memory_capabilities." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryCapabilitiesSpecVersion = VK_NV_EXTERNAL_MEMORY_CAPABILITIES_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory_capabilities extension has been deprecated by VK_KHR_external_memory_capabilities." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryCapabilitiesExtensionName = VK_NV_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME;

  //=== VK_NV_external_memory ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory extension has been deprecated by VK_KHR_external_memory." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemorySpecVersion = VK_NV_EXTERNAL_MEMORY_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory extension has been deprecated by VK_KHR_external_memory." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryExtensionName = VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_NV_external_memory_win32 ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory_win32 extension has been deprecated by VK_KHR_external_memory_win32." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryWin32SpecVersion = VK_NV_EXTERNAL_MEMORY_WIN32_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_external_memory_win32 extension has been deprecated by VK_KHR_external_memory_win32." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryWin32ExtensionName = VK_NV_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_NV_win32_keyed_mutex ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVWin32KeyedMutexSpecVersion   = VK_NV_WIN32_KEYED_MUTEX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVWin32KeyedMutexExtensionName = VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_get_physical_device_properties2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetPhysicalDeviceProperties2SpecVersion   = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetPhysicalDeviceProperties2ExtensionName = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;

  //=== VK_KHR_device_group ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeviceGroupSpecVersion   = VK_KHR_DEVICE_GROUP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeviceGroupExtensionName = VK_KHR_DEVICE_GROUP_EXTENSION_NAME;

  //=== VK_EXT_validation_flags ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_flags extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFlagsSpecVersion = VK_EXT_VALIDATION_FLAGS_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_flags extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFlagsExtensionName = VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_VI_NN )
  //=== VK_NN_vi_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NNViSurfaceSpecVersion   = VK_NN_VI_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NNViSurfaceExtensionName = VK_NN_VI_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_VI_NN*/

  //=== VK_KHR_shader_draw_parameters ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderDrawParametersSpecVersion   = VK_KHR_SHADER_DRAW_PARAMETERS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderDrawParametersExtensionName = VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME;

  //=== VK_EXT_shader_subgroup_ballot ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_subgroup_ballot extension has been deprecated by VK_VERSION_1_2." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupBallotSpecVersion = VK_EXT_SHADER_SUBGROUP_BALLOT_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_subgroup_ballot extension has been deprecated by VK_VERSION_1_2." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupBallotExtensionName = VK_EXT_SHADER_SUBGROUP_BALLOT_EXTENSION_NAME;

  //=== VK_EXT_shader_subgroup_vote ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_subgroup_vote extension has been deprecated by VK_VERSION_1_1." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupVoteSpecVersion = VK_EXT_SHADER_SUBGROUP_VOTE_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_shader_subgroup_vote extension has been deprecated by VK_VERSION_1_1." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupVoteExtensionName = VK_EXT_SHADER_SUBGROUP_VOTE_EXTENSION_NAME;

  //=== VK_EXT_texture_compression_astc_hdr ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstcHdrSpecVersion   = VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstcHdrExtensionName = VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME;

  //=== VK_EXT_astc_decode_mode ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAstcDecodeModeSpecVersion   = VK_EXT_ASTC_DECODE_MODE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAstcDecodeModeExtensionName = VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME;

  //=== VK_EXT_pipeline_robustness ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineRobustnessSpecVersion   = VK_EXT_PIPELINE_ROBUSTNESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineRobustnessExtensionName = VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME;

  //=== VK_KHR_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance1SpecVersion   = VK_KHR_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance1ExtensionName = VK_KHR_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_KHR_device_group_creation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeviceGroupCreationSpecVersion   = VK_KHR_DEVICE_GROUP_CREATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeviceGroupCreationExtensionName = VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME;

  //=== VK_KHR_external_memory_capabilities ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryCapabilitiesSpecVersion   = VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryCapabilitiesExtensionName = VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME;

  //=== VK_KHR_external_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemorySpecVersion   = VK_KHR_EXTERNAL_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryExtensionName = VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_memory_win32 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryWin32SpecVersion   = VK_KHR_EXTERNAL_MEMORY_WIN32_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryWin32ExtensionName = VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_external_memory_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryFdSpecVersion   = VK_KHR_EXTERNAL_MEMORY_FD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalMemoryFdExtensionName = VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_win32_keyed_mutex ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWin32KeyedMutexSpecVersion   = VK_KHR_WIN32_KEYED_MUTEX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWin32KeyedMutexExtensionName = VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_external_semaphore_capabilities ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreCapabilitiesSpecVersion   = VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreCapabilitiesExtensionName = VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME;

  //=== VK_KHR_external_semaphore ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreSpecVersion   = VK_KHR_EXTERNAL_SEMAPHORE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreExtensionName = VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_semaphore_win32 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreWin32SpecVersion   = VK_KHR_EXTERNAL_SEMAPHORE_WIN32_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreWin32ExtensionName = VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_external_semaphore_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreFdSpecVersion   = VK_KHR_EXTERNAL_SEMAPHORE_FD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalSemaphoreFdExtensionName = VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME;

  //=== VK_KHR_push_descriptor ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPushDescriptorSpecVersion   = VK_KHR_PUSH_DESCRIPTOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPushDescriptorExtensionName = VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME;

  //=== VK_EXT_conditional_rendering ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConditionalRenderingSpecVersion   = VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConditionalRenderingExtensionName = VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME;

  //=== VK_KHR_shader_float16_int8 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloat16Int8SpecVersion   = VK_KHR_SHADER_FLOAT16_INT8_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloat16Int8ExtensionName = VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME;

  //=== VK_KHR_16bit_storage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHR16BitStorageSpecVersion   = VK_KHR_16BIT_STORAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHR16BitStorageExtensionName = VK_KHR_16BIT_STORAGE_EXTENSION_NAME;

  //=== VK_KHR_incremental_present ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIncrementalPresentSpecVersion   = VK_KHR_INCREMENTAL_PRESENT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIncrementalPresentExtensionName = VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME;

  //=== VK_KHR_descriptor_update_template ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDescriptorUpdateTemplateSpecVersion   = VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDescriptorUpdateTemplateExtensionName = VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME;

  //=== VK_NV_clip_space_w_scaling ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVClipSpaceWScalingSpecVersion   = VK_NV_CLIP_SPACE_W_SCALING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVClipSpaceWScalingExtensionName = VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME;

  //=== VK_EXT_direct_mode_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectModeDisplaySpecVersion   = VK_EXT_DIRECT_MODE_DISPLAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectModeDisplayExtensionName = VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
  //=== VK_EXT_acquire_xlib_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAcquireXlibDisplaySpecVersion   = VK_EXT_ACQUIRE_XLIB_DISPLAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAcquireXlibDisplayExtensionName = VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

  //=== VK_EXT_display_surface_counter ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplaySurfaceCounterSpecVersion   = VK_EXT_DISPLAY_SURFACE_COUNTER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplaySurfaceCounterExtensionName = VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME;

  //=== VK_EXT_display_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplayControlSpecVersion   = VK_EXT_DISPLAY_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDisplayControlExtensionName = VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME;

  //=== VK_GOOGLE_display_timing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEDisplayTimingSpecVersion   = VK_GOOGLE_DISPLAY_TIMING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEDisplayTimingExtensionName = VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME;

  //=== VK_NV_sample_mask_override_coverage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVSampleMaskOverrideCoverageSpecVersion   = VK_NV_SAMPLE_MASK_OVERRIDE_COVERAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVSampleMaskOverrideCoverageExtensionName = VK_NV_SAMPLE_MASK_OVERRIDE_COVERAGE_EXTENSION_NAME;

  //=== VK_NV_geometry_shader_passthrough ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVGeometryShaderPassthroughSpecVersion   = VK_NV_GEOMETRY_SHADER_PASSTHROUGH_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVGeometryShaderPassthroughExtensionName = VK_NV_GEOMETRY_SHADER_PASSTHROUGH_EXTENSION_NAME;

  //=== VK_NV_viewport_array2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVViewportArray2SpecVersion   = VK_NV_VIEWPORT_ARRAY_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVViewportArray2ExtensionName = VK_NV_VIEWPORT_ARRAY_2_EXTENSION_NAME;

  //=== VK_NVX_multiview_per_view_attributes ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXMultiviewPerViewAttributesSpecVersion   = VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVXMultiviewPerViewAttributesExtensionName = VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME;

  //=== VK_NV_viewport_swizzle ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVViewportSwizzleSpecVersion   = VK_NV_VIEWPORT_SWIZZLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVViewportSwizzleExtensionName = VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME;

  //=== VK_EXT_discard_rectangles ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDiscardRectanglesSpecVersion   = VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDiscardRectanglesExtensionName = VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME;

  //=== VK_EXT_conservative_rasterization ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConservativeRasterizationSpecVersion   = VK_EXT_CONSERVATIVE_RASTERIZATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTConservativeRasterizationExtensionName = VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME;

  //=== VK_EXT_depth_clip_enable ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipEnableSpecVersion   = VK_EXT_DEPTH_CLIP_ENABLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipEnableExtensionName = VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME;

  //=== VK_EXT_swapchain_colorspace ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainColorSpaceSpecVersion   = VK_EXT_SWAPCHAIN_COLOR_SPACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainColorSpaceExtensionName = VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME;

  //=== VK_EXT_hdr_metadata ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHdrMetadataSpecVersion   = VK_EXT_HDR_METADATA_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHdrMetadataExtensionName = VK_EXT_HDR_METADATA_EXTENSION_NAME;

  //=== VK_KHR_imageless_framebuffer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRImagelessFramebufferSpecVersion   = VK_KHR_IMAGELESS_FRAMEBUFFER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRImagelessFramebufferExtensionName = VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME;

  //=== VK_KHR_create_renderpass2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCreateRenderpass2SpecVersion   = VK_KHR_CREATE_RENDERPASS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCreateRenderpass2ExtensionName = VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME;

  //=== VK_IMG_relaxed_line_rasterization ===
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGRelaxedLineRasterizationSpecVersion   = VK_IMG_RELAXED_LINE_RASTERIZATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto IMGRelaxedLineRasterizationExtensionName = VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME;

  //=== VK_KHR_shared_presentable_image ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSharedPresentableImageSpecVersion   = VK_KHR_SHARED_PRESENTABLE_IMAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSharedPresentableImageExtensionName = VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME;

  //=== VK_KHR_external_fence_capabilities ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceCapabilitiesSpecVersion   = VK_KHR_EXTERNAL_FENCE_CAPABILITIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceCapabilitiesExtensionName = VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME;

  //=== VK_KHR_external_fence ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceSpecVersion   = VK_KHR_EXTERNAL_FENCE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceExtensionName = VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_fence_win32 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceWin32SpecVersion   = VK_KHR_EXTERNAL_FENCE_WIN32_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceWin32ExtensionName = VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_external_fence_fd ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceFdSpecVersion   = VK_KHR_EXTERNAL_FENCE_FD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRExternalFenceFdExtensionName = VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME;

  //=== VK_KHR_performance_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPerformanceQuerySpecVersion   = VK_KHR_PERFORMANCE_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPerformanceQueryExtensionName = VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME;

  //=== VK_KHR_maintenance2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance2SpecVersion   = VK_KHR_MAINTENANCE_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance2ExtensionName = VK_KHR_MAINTENANCE_2_EXTENSION_NAME;

  //=== VK_KHR_get_surface_capabilities2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetSurfaceCapabilities2SpecVersion   = VK_KHR_GET_SURFACE_CAPABILITIES_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetSurfaceCapabilities2ExtensionName = VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME;

  //=== VK_KHR_variable_pointers ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVariablePointersSpecVersion   = VK_KHR_VARIABLE_POINTERS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVariablePointersExtensionName = VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME;

  //=== VK_KHR_get_display_properties2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetDisplayProperties2SpecVersion   = VK_KHR_GET_DISPLAY_PROPERTIES_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetDisplayProperties2ExtensionName = VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_IOS_MVK )
  //=== VK_MVK_ios_surface ===
  VULKAN_HPP_DEPRECATED( "The VK_MVK_ios_surface extension has been deprecated by VK_EXT_metal_surface." )
  VULKAN_HPP_CONSTEXPR_INLINE auto MVKIosSurfaceSpecVersion = VK_MVK_IOS_SURFACE_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_MVK_ios_surface extension has been deprecated by VK_EXT_metal_surface." )
  VULKAN_HPP_CONSTEXPR_INLINE auto MVKIosSurfaceExtensionName = VK_MVK_IOS_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#if defined( VK_USE_PLATFORM_MACOS_MVK )
  //=== VK_MVK_macos_surface ===
  VULKAN_HPP_DEPRECATED( "The VK_MVK_macos_surface extension has been deprecated by VK_EXT_metal_surface." )
  VULKAN_HPP_CONSTEXPR_INLINE auto MVKMacosSurfaceSpecVersion = VK_MVK_MACOS_SURFACE_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_MVK_macos_surface extension has been deprecated by VK_EXT_metal_surface." )
  VULKAN_HPP_CONSTEXPR_INLINE auto MVKMacosSurfaceExtensionName = VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

  //=== VK_EXT_external_memory_dma_buf ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryDmaBufSpecVersion   = VK_EXT_EXTERNAL_MEMORY_DMA_BUF_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryDmaBufExtensionName = VK_EXT_EXTERNAL_MEMORY_DMA_BUF_EXTENSION_NAME;

  //=== VK_EXT_queue_family_foreign ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTQueueFamilyForeignSpecVersion   = VK_EXT_QUEUE_FAMILY_FOREIGN_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTQueueFamilyForeignExtensionName = VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME;

  //=== VK_KHR_dedicated_allocation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDedicatedAllocationSpecVersion   = VK_KHR_DEDICATED_ALLOCATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDedicatedAllocationExtensionName = VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME;

  //=== VK_EXT_debug_utils ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugUtilsSpecVersion   = VK_EXT_DEBUG_UTILS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDebugUtilsExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
  //=== VK_ANDROID_external_memory_android_hardware_buffer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ANDROIDExternalMemoryAndroidHardwareBufferSpecVersion   = VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ANDROIDExternalMemoryAndroidHardwareBufferExtensionName = VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

  //=== VK_EXT_sampler_filter_minmax ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSamplerFilterMinmaxSpecVersion   = VK_EXT_SAMPLER_FILTER_MINMAX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSamplerFilterMinmaxExtensionName = VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME;

  //=== VK_KHR_storage_buffer_storage_class ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRStorageBufferStorageClassSpecVersion   = VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRStorageBufferStorageClassExtensionName = VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME;

  //=== VK_AMD_gpu_shader_int16 ===
  VULKAN_HPP_DEPRECATED( "The VK_AMD_gpu_shader_int16 extension has been deprecated by VK_KHR_shader_float16_int8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGpuShaderInt16SpecVersion = VK_AMD_GPU_SHADER_INT16_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_AMD_gpu_shader_int16 extension has been deprecated by VK_KHR_shader_float16_int8." )
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDGpuShaderInt16ExtensionName = VK_AMD_GPU_SHADER_INT16_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_shader_enqueue ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDXShaderEnqueueSpecVersion   = VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDXShaderEnqueueExtensionName = VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_EXT_descriptor_heap ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorHeapSpecVersion   = VK_EXT_DESCRIPTOR_HEAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorHeapExtensionName = VK_EXT_DESCRIPTOR_HEAP_EXTENSION_NAME;

  //=== VK_AMD_mixed_attachment_samples ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDMixedAttachmentSamplesSpecVersion   = VK_AMD_MIXED_ATTACHMENT_SAMPLES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDMixedAttachmentSamplesExtensionName = VK_AMD_MIXED_ATTACHMENT_SAMPLES_EXTENSION_NAME;

  //=== VK_AMD_shader_fragment_mask ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderFragmentMaskSpecVersion   = VK_AMD_SHADER_FRAGMENT_MASK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderFragmentMaskExtensionName = VK_AMD_SHADER_FRAGMENT_MASK_EXTENSION_NAME;

  //=== VK_EXT_inline_uniform_block ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTInlineUniformBlockSpecVersion   = VK_EXT_INLINE_UNIFORM_BLOCK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTInlineUniformBlockExtensionName = VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME;

  //=== VK_EXT_shader_stencil_export ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderStencilExportSpecVersion   = VK_EXT_SHADER_STENCIL_EXPORT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderStencilExportExtensionName = VK_EXT_SHADER_STENCIL_EXPORT_EXTENSION_NAME;

  //=== VK_KHR_shader_bfloat16 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderBfloat16SpecVersion   = VK_KHR_SHADER_BFLOAT16_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderBfloat16ExtensionName = VK_KHR_SHADER_BFLOAT16_EXTENSION_NAME;

  //=== VK_EXT_sample_locations ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSampleLocationsSpecVersion   = VK_EXT_SAMPLE_LOCATIONS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSampleLocationsExtensionName = VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME;

  //=== VK_KHR_relaxed_block_layout ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRelaxedBlockLayoutSpecVersion   = VK_KHR_RELAXED_BLOCK_LAYOUT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRelaxedBlockLayoutExtensionName = VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME;

  //=== VK_KHR_get_memory_requirements2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetMemoryRequirements2SpecVersion   = VK_KHR_GET_MEMORY_REQUIREMENTS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGetMemoryRequirements2ExtensionName = VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME;

  //=== VK_KHR_image_format_list ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRImageFormatListSpecVersion   = VK_KHR_IMAGE_FORMAT_LIST_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRImageFormatListExtensionName = VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME;

  //=== VK_EXT_blend_operation_advanced ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBlendOperationAdvancedSpecVersion   = VK_EXT_BLEND_OPERATION_ADVANCED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBlendOperationAdvancedExtensionName = VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME;

  //=== VK_NV_fragment_coverage_to_color ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentCoverageToColorSpecVersion   = VK_NV_FRAGMENT_COVERAGE_TO_COLOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentCoverageToColorExtensionName = VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME;

  //=== VK_KHR_acceleration_structure ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRAccelerationStructureSpecVersion   = VK_KHR_ACCELERATION_STRUCTURE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRAccelerationStructureExtensionName = VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME;

  //=== VK_KHR_ray_tracing_pipeline ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingPipelineSpecVersion   = VK_KHR_RAY_TRACING_PIPELINE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingPipelineExtensionName = VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME;

  //=== VK_KHR_ray_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayQuerySpecVersion   = VK_KHR_RAY_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayQueryExtensionName = VK_KHR_RAY_QUERY_EXTENSION_NAME;

  //=== VK_NV_framebuffer_mixed_samples ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFramebufferMixedSamplesSpecVersion   = VK_NV_FRAMEBUFFER_MIXED_SAMPLES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFramebufferMixedSamplesExtensionName = VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME;

  //=== VK_NV_fill_rectangle ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFillRectangleSpecVersion   = VK_NV_FILL_RECTANGLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFillRectangleExtensionName = VK_NV_FILL_RECTANGLE_EXTENSION_NAME;

  //=== VK_NV_shader_sm_builtins ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderSmBuiltinsSpecVersion   = VK_NV_SHADER_SM_BUILTINS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderSmBuiltinsExtensionName = VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME;

  //=== VK_EXT_post_depth_coverage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPostDepthCoverageSpecVersion   = VK_EXT_POST_DEPTH_COVERAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPostDepthCoverageExtensionName = VK_EXT_POST_DEPTH_COVERAGE_EXTENSION_NAME;

  //=== VK_KHR_sampler_ycbcr_conversion ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSamplerYcbcrConversionSpecVersion   = VK_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSamplerYcbcrConversionExtensionName = VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME;

  //=== VK_KHR_bind_memory2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRBindMemory2SpecVersion   = VK_KHR_BIND_MEMORY_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRBindMemory2ExtensionName = VK_KHR_BIND_MEMORY_2_EXTENSION_NAME;

  //=== VK_EXT_image_drm_format_modifier ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageDrmFormatModifierSpecVersion   = VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageDrmFormatModifierExtensionName = VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME;

  //=== VK_EXT_validation_cache ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationCacheSpecVersion   = VK_EXT_VALIDATION_CACHE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationCacheExtensionName = VK_EXT_VALIDATION_CACHE_EXTENSION_NAME;

  //=== VK_EXT_descriptor_indexing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorIndexingSpecVersion   = VK_EXT_DESCRIPTOR_INDEXING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorIndexingExtensionName = VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME;

  //=== VK_EXT_shader_viewport_index_layer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderViewportIndexLayerSpecVersion   = VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderViewportIndexLayerExtensionName = VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_KHR_portability_subset ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPortabilitySubsetSpecVersion   = VK_KHR_PORTABILITY_SUBSET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPortabilitySubsetExtensionName = VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_NV_shading_rate_image ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShadingRateImageSpecVersion   = VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShadingRateImageExtensionName = VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME;

  //=== VK_NV_ray_tracing ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_ray_tracing extension has been deprecated by VK_KHR_ray_tracing_pipeline." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingSpecVersion = VK_NV_RAY_TRACING_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_ray_tracing extension has been deprecated by VK_KHR_ray_tracing_pipeline." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingExtensionName = VK_NV_RAY_TRACING_EXTENSION_NAME;

  //=== VK_NV_representative_fragment_test ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRepresentativeFragmentTestSpecVersion   = VK_NV_REPRESENTATIVE_FRAGMENT_TEST_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRepresentativeFragmentTestExtensionName = VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME;

  //=== VK_KHR_maintenance3 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance3SpecVersion   = VK_KHR_MAINTENANCE_3_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance3ExtensionName = VK_KHR_MAINTENANCE_3_EXTENSION_NAME;

  //=== VK_KHR_draw_indirect_count ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDrawIndirectCountSpecVersion   = VK_KHR_DRAW_INDIRECT_COUNT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDrawIndirectCountExtensionName = VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME;

  //=== VK_EXT_filter_cubic ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFilterCubicSpecVersion   = VK_EXT_FILTER_CUBIC_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFilterCubicExtensionName = VK_EXT_FILTER_CUBIC_EXTENSION_NAME;

  //=== VK_QCOM_render_pass_shader_resolve ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassShaderResolveSpecVersion   = VK_QCOM_RENDER_PASS_SHADER_RESOLVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassShaderResolveExtensionName = VK_QCOM_RENDER_PASS_SHADER_RESOLVE_EXTENSION_NAME;

  //=== VK_QCOM_cooperative_matrix_conversion ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMCooperativeMatrixConversionSpecVersion   = VK_QCOM_COOPERATIVE_MATRIX_CONVERSION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMCooperativeMatrixConversionExtensionName = VK_QCOM_COOPERATIVE_MATRIX_CONVERSION_EXTENSION_NAME;

  //=== VK_EXT_global_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPrioritySpecVersion   = VK_EXT_GLOBAL_PRIORITY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPriorityExtensionName = VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME;

  //=== VK_KHR_shader_subgroup_extended_types ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupExtendedTypesSpecVersion   = VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupExtendedTypesExtensionName = VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME;

  //=== VK_KHR_8bit_storage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHR8BitStorageSpecVersion   = VK_KHR_8BIT_STORAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHR8BitStorageExtensionName = VK_KHR_8BIT_STORAGE_EXTENSION_NAME;

  //=== VK_EXT_external_memory_host ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryHostSpecVersion   = VK_EXT_EXTERNAL_MEMORY_HOST_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryHostExtensionName = VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME;

  //=== VK_AMD_buffer_marker ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDBufferMarkerSpecVersion   = VK_AMD_BUFFER_MARKER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDBufferMarkerExtensionName = VK_AMD_BUFFER_MARKER_EXTENSION_NAME;

  //=== VK_KHR_shader_atomic_int64 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderAtomicInt64SpecVersion   = VK_KHR_SHADER_ATOMIC_INT64_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderAtomicInt64ExtensionName = VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME;

  //=== VK_KHR_shader_clock ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderClockSpecVersion   = VK_KHR_SHADER_CLOCK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderClockExtensionName = VK_KHR_SHADER_CLOCK_EXTENSION_NAME;

  //=== VK_AMD_pipeline_compiler_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDPipelineCompilerControlSpecVersion   = VK_AMD_PIPELINE_COMPILER_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDPipelineCompilerControlExtensionName = VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME;

  //=== VK_EXT_calibrated_timestamps ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCalibratedTimestampsSpecVersion   = VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCalibratedTimestampsExtensionName = VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME;

  //=== VK_AMD_shader_core_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderCorePropertiesSpecVersion   = VK_AMD_SHADER_CORE_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderCorePropertiesExtensionName = VK_AMD_SHADER_CORE_PROPERTIES_EXTENSION_NAME;

  //=== VK_KHR_video_decode_h265 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeH265SpecVersion   = VK_KHR_VIDEO_DECODE_H265_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeH265ExtensionName = VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME;

  //=== VK_KHR_global_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGlobalPrioritySpecVersion   = VK_KHR_GLOBAL_PRIORITY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRGlobalPriorityExtensionName = VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME;

  //=== VK_AMD_memory_overallocation_behavior ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDMemoryOverallocationBehaviorSpecVersion   = VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDMemoryOverallocationBehaviorExtensionName = VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME;

  //=== VK_EXT_vertex_attribute_divisor ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeDivisorSpecVersion   = VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeDivisorExtensionName = VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_GGP )
  //=== VK_GGP_frame_token ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GGPFrameTokenSpecVersion   = VK_GGP_FRAME_TOKEN_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GGPFrameTokenExtensionName = VK_GGP_FRAME_TOKEN_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_GGP*/

  //=== VK_EXT_pipeline_creation_feedback ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineCreationFeedbackSpecVersion   = VK_EXT_PIPELINE_CREATION_FEEDBACK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineCreationFeedbackExtensionName = VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME;

  //=== VK_KHR_driver_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDriverPropertiesSpecVersion   = VK_KHR_DRIVER_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDriverPropertiesExtensionName = VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME;

  //=== VK_KHR_shader_float_controls ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloatControlsSpecVersion   = VK_KHR_SHADER_FLOAT_CONTROLS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloatControlsExtensionName = VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME;

  //=== VK_NV_shader_subgroup_partitioned ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderSubgroupPartitionedSpecVersion   = VK_NV_SHADER_SUBGROUP_PARTITIONED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderSubgroupPartitionedExtensionName = VK_NV_SHADER_SUBGROUP_PARTITIONED_EXTENSION_NAME;

  //=== VK_KHR_depth_stencil_resolve ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDepthStencilResolveSpecVersion   = VK_KHR_DEPTH_STENCIL_RESOLVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDepthStencilResolveExtensionName = VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME;

  //=== VK_KHR_swapchain_mutable_format ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMutableFormatSpecVersion   = VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMutableFormatExtensionName = VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME;

  //=== VK_NV_compute_shader_derivatives ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVComputeShaderDerivativesSpecVersion   = VK_NV_COMPUTE_SHADER_DERIVATIVES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVComputeShaderDerivativesExtensionName = VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME;

  //=== VK_NV_mesh_shader ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVMeshShaderSpecVersion   = VK_NV_MESH_SHADER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVMeshShaderExtensionName = VK_NV_MESH_SHADER_EXTENSION_NAME;

  //=== VK_NV_fragment_shader_barycentric ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentShaderBarycentricSpecVersion   = VK_NV_FRAGMENT_SHADER_BARYCENTRIC_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentShaderBarycentricExtensionName = VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME;

  //=== VK_NV_shader_image_footprint ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderImageFootprintSpecVersion   = VK_NV_SHADER_IMAGE_FOOTPRINT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderImageFootprintExtensionName = VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME;

  //=== VK_NV_scissor_exclusive ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVScissorExclusiveSpecVersion   = VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVScissorExclusiveExtensionName = VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME;

  //=== VK_NV_device_diagnostic_checkpoints ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceDiagnosticCheckpointsSpecVersion   = VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceDiagnosticCheckpointsExtensionName = VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME;

  //=== VK_KHR_timeline_semaphore ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRTimelineSemaphoreSpecVersion   = VK_KHR_TIMELINE_SEMAPHORE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRTimelineSemaphoreExtensionName = VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME;

  //=== VK_EXT_present_timing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPresentTimingSpecVersion   = VK_EXT_PRESENT_TIMING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPresentTimingExtensionName = VK_EXT_PRESENT_TIMING_EXTENSION_NAME;

  //=== VK_INTEL_shader_integer_functions2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto INTELShaderIntegerFunctions2SpecVersion   = VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto INTELShaderIntegerFunctions2ExtensionName = VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME;

  //=== VK_INTEL_performance_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto INTELPerformanceQuerySpecVersion   = VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto INTELPerformanceQueryExtensionName = VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME;

  //=== VK_KHR_vulkan_memory_model ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVulkanMemoryModelSpecVersion   = VK_KHR_VULKAN_MEMORY_MODEL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVulkanMemoryModelExtensionName = VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME;

  //=== VK_EXT_pci_bus_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPciBusInfoSpecVersion   = VK_EXT_PCI_BUS_INFO_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPciBusInfoExtensionName = VK_EXT_PCI_BUS_INFO_EXTENSION_NAME;

  //=== VK_AMD_display_native_hdr ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDisplayNativeHdrSpecVersion   = VK_AMD_DISPLAY_NATIVE_HDR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDisplayNativeHdrExtensionName = VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_imagepipe_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAImagepipeSurfaceSpecVersion   = VK_FUCHSIA_IMAGEPIPE_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAImagepipeSurfaceExtensionName = VK_FUCHSIA_IMAGEPIPE_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

  //=== VK_KHR_shader_terminate_invocation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderTerminateInvocationSpecVersion   = VK_KHR_SHADER_TERMINATE_INVOCATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderTerminateInvocationExtensionName = VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_METAL_EXT )
  //=== VK_EXT_metal_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMetalSurfaceSpecVersion   = VK_EXT_METAL_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMetalSurfaceExtensionName = VK_EXT_METAL_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

  //=== VK_EXT_fragment_density_map ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMapSpecVersion   = VK_EXT_FRAGMENT_DENSITY_MAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMapExtensionName = VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME;

  //=== VK_EXT_scalar_block_layout ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTScalarBlockLayoutSpecVersion   = VK_EXT_SCALAR_BLOCK_LAYOUT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTScalarBlockLayoutExtensionName = VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME;

  //=== VK_GOOGLE_hlsl_functionality1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEHlslFunctionality1SpecVersion   = VK_GOOGLE_HLSL_FUNCTIONALITY_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEHlslFunctionality1ExtensionName = VK_GOOGLE_HLSL_FUNCTIONALITY_1_EXTENSION_NAME;

  //=== VK_GOOGLE_decorate_string ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEDecorateStringSpecVersion   = VK_GOOGLE_DECORATE_STRING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEDecorateStringExtensionName = VK_GOOGLE_DECORATE_STRING_EXTENSION_NAME;

  //=== VK_EXT_subgroup_size_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubgroupSizeControlSpecVersion   = VK_EXT_SUBGROUP_SIZE_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubgroupSizeControlExtensionName = VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME;

  //=== VK_KHR_fragment_shading_rate ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShadingRateSpecVersion   = VK_KHR_FRAGMENT_SHADING_RATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShadingRateExtensionName = VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME;

  //=== VK_AMD_shader_core_properties2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderCoreProperties2SpecVersion   = VK_AMD_SHADER_CORE_PROPERTIES_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderCoreProperties2ExtensionName = VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME;

  //=== VK_AMD_device_coherent_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDeviceCoherentMemorySpecVersion   = VK_AMD_DEVICE_COHERENT_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDDeviceCoherentMemoryExtensionName = VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME;

  //=== VK_KHR_dynamic_rendering_local_read ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDynamicRenderingLocalReadSpecVersion   = VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDynamicRenderingLocalReadExtensionName = VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME;

  //=== VK_EXT_shader_image_atomic_int64 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderImageAtomicInt64SpecVersion   = VK_EXT_SHADER_IMAGE_ATOMIC_INT64_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderImageAtomicInt64ExtensionName = VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME;

  //=== VK_KHR_shader_quad_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderQuadControlSpecVersion   = VK_KHR_SHADER_QUAD_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderQuadControlExtensionName = VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME;

  //=== VK_KHR_spirv_1_4 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSpirv14SpecVersion   = VK_KHR_SPIRV_1_4_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSpirv14ExtensionName = VK_KHR_SPIRV_1_4_EXTENSION_NAME;

  //=== VK_EXT_memory_budget ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryBudgetSpecVersion   = VK_EXT_MEMORY_BUDGET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryBudgetExtensionName = VK_EXT_MEMORY_BUDGET_EXTENSION_NAME;

  //=== VK_EXT_memory_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryPrioritySpecVersion   = VK_EXT_MEMORY_PRIORITY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryPriorityExtensionName = VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME;

  //=== VK_KHR_surface_protected_capabilities ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceProtectedCapabilitiesSpecVersion   = VK_KHR_SURFACE_PROTECTED_CAPABILITIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceProtectedCapabilitiesExtensionName = VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME;

  //=== VK_NV_dedicated_allocation_image_aliasing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDedicatedAllocationImageAliasingSpecVersion   = VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDedicatedAllocationImageAliasingExtensionName = VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME;

  //=== VK_KHR_separate_depth_stencil_layouts ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSeparateDepthStencilLayoutsSpecVersion   = VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSeparateDepthStencilLayoutsExtensionName = VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME;

  //=== VK_EXT_buffer_device_address ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_buffer_device_address extension has been deprecated by VK_KHR_buffer_device_address." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBufferDeviceAddressSpecVersion = VK_EXT_BUFFER_DEVICE_ADDRESS_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_buffer_device_address extension has been deprecated by VK_KHR_buffer_device_address." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBufferDeviceAddressExtensionName = VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME;

  //=== VK_EXT_tooling_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTToolingInfoSpecVersion   = VK_EXT_TOOLING_INFO_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTToolingInfoExtensionName = VK_EXT_TOOLING_INFO_EXTENSION_NAME;

  //=== VK_EXT_separate_stencil_usage ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSeparateStencilUsageSpecVersion   = VK_EXT_SEPARATE_STENCIL_USAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSeparateStencilUsageExtensionName = VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME;

  //=== VK_EXT_validation_features ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_features extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFeaturesSpecVersion = VK_EXT_VALIDATION_FEATURES_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_validation_features extension has been deprecated by VK_EXT_layer_settings." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTValidationFeaturesExtensionName = VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME;

  //=== VK_KHR_present_wait ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentWaitSpecVersion   = VK_KHR_PRESENT_WAIT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentWaitExtensionName = VK_KHR_PRESENT_WAIT_EXTENSION_NAME;

  //=== VK_NV_cooperative_matrix ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeMatrixSpecVersion   = VK_NV_COOPERATIVE_MATRIX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeMatrixExtensionName = VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME;

  //=== VK_NV_coverage_reduction_mode ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCoverageReductionModeSpecVersion   = VK_NV_COVERAGE_REDUCTION_MODE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCoverageReductionModeExtensionName = VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME;

  //=== VK_EXT_fragment_shader_interlock ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentShaderInterlockSpecVersion   = VK_EXT_FRAGMENT_SHADER_INTERLOCK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentShaderInterlockExtensionName = VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME;

  //=== VK_EXT_ycbcr_image_arrays ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcrImageArraysSpecVersion   = VK_EXT_YCBCR_IMAGE_ARRAYS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcrImageArraysExtensionName = VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME;

  //=== VK_KHR_uniform_buffer_standard_layout ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRUniformBufferStandardLayoutSpecVersion   = VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRUniformBufferStandardLayoutExtensionName = VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME;

  //=== VK_EXT_provoking_vertex ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTProvokingVertexSpecVersion   = VK_EXT_PROVOKING_VERTEX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTProvokingVertexExtensionName = VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_EXT_full_screen_exclusive ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFullScreenExclusiveSpecVersion   = VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFullScreenExclusiveExtensionName = VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_EXT_headless_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHeadlessSurfaceSpecVersion   = VK_EXT_HEADLESS_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHeadlessSurfaceExtensionName = VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME;

  //=== VK_KHR_buffer_device_address ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRBufferDeviceAddressSpecVersion   = VK_KHR_BUFFER_DEVICE_ADDRESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRBufferDeviceAddressExtensionName = VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME;

  //=== VK_EXT_line_rasterization ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLineRasterizationSpecVersion   = VK_EXT_LINE_RASTERIZATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLineRasterizationExtensionName = VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME;

  //=== VK_EXT_shader_atomic_float ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloatSpecVersion   = VK_EXT_SHADER_ATOMIC_FLOAT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloatExtensionName = VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME;

  //=== VK_EXT_host_query_reset ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHostQueryResetSpecVersion   = VK_EXT_HOST_QUERY_RESET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHostQueryResetExtensionName = VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME;

  //=== VK_EXT_index_type_uint8 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTIndexTypeUint8SpecVersion   = VK_EXT_INDEX_TYPE_UINT8_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTIndexTypeUint8ExtensionName = VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME;

  //=== VK_EXT_extended_dynamic_state ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicStateSpecVersion   = VK_EXT_EXTENDED_DYNAMIC_STATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicStateExtensionName = VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME;

  //=== VK_KHR_deferred_host_operations ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeferredHostOperationsSpecVersion   = VK_KHR_DEFERRED_HOST_OPERATIONS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDeferredHostOperationsExtensionName = VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME;

  //=== VK_KHR_pipeline_executable_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineExecutablePropertiesSpecVersion   = VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineExecutablePropertiesExtensionName = VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME;

  //=== VK_EXT_host_image_copy ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHostImageCopySpecVersion   = VK_EXT_HOST_IMAGE_COPY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTHostImageCopyExtensionName = VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME;

  //=== VK_KHR_map_memory2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMapMemory2SpecVersion   = VK_KHR_MAP_MEMORY_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMapMemory2ExtensionName = VK_KHR_MAP_MEMORY_2_EXTENSION_NAME;

  //=== VK_EXT_map_memory_placed ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMapMemoryPlacedSpecVersion   = VK_EXT_MAP_MEMORY_PLACED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMapMemoryPlacedExtensionName = VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME;

  //=== VK_EXT_shader_atomic_float2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloat2SpecVersion   = VK_EXT_SHADER_ATOMIC_FLOAT_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderAtomicFloat2ExtensionName = VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME;

  //=== VK_EXT_surface_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSurfaceMaintenance1SpecVersion   = VK_EXT_SURFACE_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSurfaceMaintenance1ExtensionName = VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_EXT_swapchain_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainMaintenance1SpecVersion   = VK_EXT_SWAPCHAIN_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSwapchainMaintenance1ExtensionName = VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_EXT_shader_demote_to_helper_invocation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderDemoteToHelperInvocationSpecVersion   = VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderDemoteToHelperInvocationExtensionName = VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME;

  //=== VK_NV_device_generated_commands ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceGeneratedCommandsSpecVersion   = VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceGeneratedCommandsExtensionName = VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME;

  //=== VK_NV_inherited_viewport_scissor ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVInheritedViewportScissorSpecVersion   = VK_NV_INHERITED_VIEWPORT_SCISSOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVInheritedViewportScissorExtensionName = VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME;

  //=== VK_KHR_shader_integer_dot_product ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderIntegerDotProductSpecVersion   = VK_KHR_SHADER_INTEGER_DOT_PRODUCT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderIntegerDotProductExtensionName = VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME;

  //=== VK_EXT_texel_buffer_alignment ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTexelBufferAlignmentSpecVersion   = VK_EXT_TEXEL_BUFFER_ALIGNMENT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTexelBufferAlignmentExtensionName = VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME;

  //=== VK_QCOM_render_pass_transform ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassTransformSpecVersion   = VK_QCOM_RENDER_PASS_TRANSFORM_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassTransformExtensionName = VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME;

  //=== VK_EXT_depth_bias_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthBiasControlSpecVersion   = VK_EXT_DEPTH_BIAS_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthBiasControlExtensionName = VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME;

  //=== VK_EXT_device_memory_report ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceMemoryReportSpecVersion   = VK_EXT_DEVICE_MEMORY_REPORT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceMemoryReportExtensionName = VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME;

  //=== VK_EXT_acquire_drm_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAcquireDrmDisplaySpecVersion   = VK_EXT_ACQUIRE_DRM_DISPLAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAcquireDrmDisplayExtensionName = VK_EXT_ACQUIRE_DRM_DISPLAY_EXTENSION_NAME;

  //=== VK_EXT_robustness2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRobustness2SpecVersion   = VK_EXT_ROBUSTNESS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRobustness2ExtensionName = VK_EXT_ROBUSTNESS_2_EXTENSION_NAME;

  //=== VK_EXT_custom_border_color ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomBorderColorSpecVersion   = VK_EXT_CUSTOM_BORDER_COLOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomBorderColorExtensionName = VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME;

  //=== VK_EXT_texture_compression_astc_3d ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstc3DSpecVersion   = VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTTextureCompressionAstc3DExtensionName = VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_EXTENSION_NAME;

  //=== VK_GOOGLE_user_type ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEUserTypeSpecVersion   = VK_GOOGLE_USER_TYPE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLEUserTypeExtensionName = VK_GOOGLE_USER_TYPE_EXTENSION_NAME;

  //=== VK_KHR_pipeline_library ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineLibrarySpecVersion   = VK_KHR_PIPELINE_LIBRARY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineLibraryExtensionName = VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME;

  //=== VK_NV_present_barrier ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPresentBarrierSpecVersion   = VK_NV_PRESENT_BARRIER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPresentBarrierExtensionName = VK_NV_PRESENT_BARRIER_EXTENSION_NAME;

  //=== VK_KHR_shader_non_semantic_info ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderNonSemanticInfoSpecVersion   = VK_KHR_SHADER_NON_SEMANTIC_INFO_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderNonSemanticInfoExtensionName = VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME;

  //=== VK_KHR_present_id ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentIdSpecVersion   = VK_KHR_PRESENT_ID_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentIdExtensionName = VK_KHR_PRESENT_ID_EXTENSION_NAME;

  //=== VK_EXT_private_data ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrivateDataSpecVersion   = VK_EXT_PRIVATE_DATA_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrivateDataExtensionName = VK_EXT_PRIVATE_DATA_EXTENSION_NAME;

  //=== VK_EXT_pipeline_creation_cache_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineCreationCacheControlSpecVersion   = VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineCreationCacheControlExtensionName = VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME;

  //=== VK_KHR_video_encode_queue ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeQueueSpecVersion   = VK_KHR_VIDEO_ENCODE_QUEUE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeQueueExtensionName = VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME;

  //=== VK_NV_device_diagnostics_config ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceDiagnosticsConfigSpecVersion   = VK_NV_DEVICE_DIAGNOSTICS_CONFIG_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceDiagnosticsConfigExtensionName = VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME;

  //=== VK_QCOM_render_pass_store_ops ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassStoreOpsSpecVersion   = VK_QCOM_RENDER_PASS_STORE_OPS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRenderPassStoreOpsExtensionName = VK_QCOM_RENDER_PASS_STORE_OPS_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_cuda_kernel_launch ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCudaKernelLaunchSpecVersion   = VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCudaKernelLaunchExtensionName = VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_QCOM_tile_shading ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTileShadingSpecVersion   = VK_QCOM_TILE_SHADING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTileShadingExtensionName = VK_QCOM_TILE_SHADING_EXTENSION_NAME;

  //=== VK_NV_low_latency ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLowLatencySpecVersion   = VK_NV_LOW_LATENCY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLowLatencyExtensionName = VK_NV_LOW_LATENCY_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_METAL_EXT )
  //=== VK_EXT_metal_objects ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMetalObjectsSpecVersion   = VK_EXT_METAL_OBJECTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMetalObjectsExtensionName = VK_EXT_METAL_OBJECTS_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

  //=== VK_KHR_synchronization2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSynchronization2SpecVersion   = VK_KHR_SYNCHRONIZATION_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSynchronization2ExtensionName = VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME;

  //=== VK_EXT_descriptor_buffer ===
  VULKAN_HPP_DEPRECATED( "The VK_EXT_descriptor_buffer extension has been deprecated by VK_EXT_descriptor_heap." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorBufferSpecVersion = VK_EXT_DESCRIPTOR_BUFFER_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_EXT_descriptor_buffer extension has been deprecated by VK_EXT_descriptor_heap." )
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDescriptorBufferExtensionName = VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME;

  //=== VK_EXT_graphics_pipeline_library ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGraphicsPipelineLibrarySpecVersion   = VK_EXT_GRAPHICS_PIPELINE_LIBRARY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGraphicsPipelineLibraryExtensionName = VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME;

  //=== VK_AMD_shader_early_and_late_fragment_tests ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderEarlyAndLateFragmentTestsSpecVersion   = VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDShaderEarlyAndLateFragmentTestsExtensionName = VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME;

  //=== VK_KHR_fragment_shader_barycentric ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShaderBarycentricSpecVersion   = VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFragmentShaderBarycentricExtensionName = VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME;

  //=== VK_KHR_shader_subgroup_uniform_control_flow ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupUniformControlFlowSpecVersion   = VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupUniformControlFlowExtensionName = VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME;

  //=== VK_KHR_zero_initialize_workgroup_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRZeroInitializeWorkgroupMemorySpecVersion   = VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRZeroInitializeWorkgroupMemoryExtensionName = VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME;

  //=== VK_NV_fragment_shading_rate_enums ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentShadingRateEnumsSpecVersion   = VK_NV_FRAGMENT_SHADING_RATE_ENUMS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVFragmentShadingRateEnumsExtensionName = VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME;

  //=== VK_NV_ray_tracing_motion_blur ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingMotionBlurSpecVersion   = VK_NV_RAY_TRACING_MOTION_BLUR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingMotionBlurExtensionName = VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME;

  //=== VK_EXT_mesh_shader ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMeshShaderSpecVersion   = VK_EXT_MESH_SHADER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMeshShaderExtensionName = VK_EXT_MESH_SHADER_EXTENSION_NAME;

  //=== VK_EXT_ycbcr_2plane_444_formats ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcr2Plane444FormatsSpecVersion   = VK_EXT_YCBCR_2PLANE_444_FORMATS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTYcbcr2Plane444FormatsExtensionName = VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME;

  //=== VK_EXT_fragment_density_map2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMap2SpecVersion   = VK_EXT_FRAGMENT_DENSITY_MAP_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMap2ExtensionName = VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME;

  //=== VK_QCOM_rotated_copy_commands ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRotatedCopyCommandsSpecVersion   = VK_QCOM_ROTATED_COPY_COMMANDS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMRotatedCopyCommandsExtensionName = VK_QCOM_ROTATED_COPY_COMMANDS_EXTENSION_NAME;

  //=== VK_EXT_image_robustness ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageRobustnessSpecVersion   = VK_EXT_IMAGE_ROBUSTNESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageRobustnessExtensionName = VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME;

  //=== VK_KHR_workgroup_memory_explicit_layout ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWorkgroupMemoryExplicitLayoutSpecVersion   = VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRWorkgroupMemoryExplicitLayoutExtensionName = VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME;

  //=== VK_KHR_copy_commands2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyCommands2SpecVersion   = VK_KHR_COPY_COMMANDS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyCommands2ExtensionName = VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME;

  //=== VK_EXT_image_compression_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageCompressionControlSpecVersion   = VK_EXT_IMAGE_COMPRESSION_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageCompressionControlExtensionName = VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME;

  //=== VK_EXT_attachment_feedback_loop_layout ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAttachmentFeedbackLoopLayoutSpecVersion   = VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAttachmentFeedbackLoopLayoutExtensionName = VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME;

  //=== VK_EXT_4444_formats ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXT4444FormatsSpecVersion   = VK_EXT_4444_FORMATS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXT4444FormatsExtensionName = VK_EXT_4444_FORMATS_EXTENSION_NAME;

  //=== VK_EXT_device_fault ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceFaultSpecVersion   = VK_EXT_DEVICE_FAULT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceFaultExtensionName = VK_EXT_DEVICE_FAULT_EXTENSION_NAME;

  //=== VK_ARM_rasterization_order_attachment_access ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMRasterizationOrderAttachmentAccessSpecVersion   = VK_ARM_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMRasterizationOrderAttachmentAccessExtensionName = VK_ARM_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME;

  //=== VK_EXT_rgba10x6_formats ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRgba10X6FormatsSpecVersion   = VK_EXT_RGBA10X6_FORMATS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRgba10X6FormatsExtensionName = VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_NV_acquire_winrt_display ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVAcquireWinrtDisplaySpecVersion   = VK_NV_ACQUIRE_WINRT_DISPLAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVAcquireWinrtDisplayExtensionName = VK_NV_ACQUIRE_WINRT_DISPLAY_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#if defined( VK_USE_PLATFORM_DIRECTFB_EXT )
  //=== VK_EXT_directfb_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectfbSurfaceSpecVersion   = VK_EXT_DIRECTFB_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDirectfbSurfaceExtensionName = VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_DIRECTFB_EXT*/

  //=== VK_VALVE_mutable_descriptor_type ===
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEMutableDescriptorTypeSpecVersion   = VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEMutableDescriptorTypeExtensionName = VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME;

  //=== VK_EXT_vertex_input_dynamic_state ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexInputDynamicStateSpecVersion   = VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexInputDynamicStateExtensionName = VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME;

  //=== VK_EXT_physical_device_drm ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPhysicalDeviceDrmSpecVersion   = VK_EXT_PHYSICAL_DEVICE_DRM_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPhysicalDeviceDrmExtensionName = VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME;

  //=== VK_EXT_device_address_binding_report ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceAddressBindingReportSpecVersion   = VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceAddressBindingReportExtensionName = VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME;

  //=== VK_EXT_depth_clip_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipControlSpecVersion   = VK_EXT_DEPTH_CLIP_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClipControlExtensionName = VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME;

  //=== VK_EXT_primitive_topology_list_restart ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrimitiveTopologyListRestartSpecVersion   = VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrimitiveTopologyListRestartExtensionName = VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME;

  //=== VK_KHR_format_feature_flags2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFormatFeatureFlags2SpecVersion   = VK_KHR_FORMAT_FEATURE_FLAGS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRFormatFeatureFlags2ExtensionName = VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME;

  //=== VK_EXT_present_mode_fifo_latest_ready ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPresentModeFifoLatestReadySpecVersion   = VK_EXT_PRESENT_MODE_FIFO_LATEST_READY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPresentModeFifoLatestReadyExtensionName = VK_EXT_PRESENT_MODE_FIFO_LATEST_READY_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_external_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAExternalMemorySpecVersion   = VK_FUCHSIA_EXTERNAL_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAExternalMemoryExtensionName = VK_FUCHSIA_EXTERNAL_MEMORY_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_external_semaphore ===
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAExternalSemaphoreSpecVersion   = VK_FUCHSIA_EXTERNAL_SEMAPHORE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIAExternalSemaphoreExtensionName = VK_FUCHSIA_EXTERNAL_SEMAPHORE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_buffer_collection ===
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIABufferCollectionSpecVersion   = VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto FUCHSIABufferCollectionExtensionName = VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

  //=== VK_HUAWEI_subpass_shading ===
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEISubpassShadingSpecVersion   = VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEISubpassShadingExtensionName = VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME;

  //=== VK_HUAWEI_invocation_mask ===
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIInvocationMaskSpecVersion   = VK_HUAWEI_INVOCATION_MASK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIInvocationMaskExtensionName = VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME;

  //=== VK_NV_external_memory_rdma ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryRdmaSpecVersion   = VK_NV_EXTERNAL_MEMORY_RDMA_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalMemoryRdmaExtensionName = VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME;

  //=== VK_EXT_pipeline_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelinePropertiesSpecVersion   = VK_EXT_PIPELINE_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelinePropertiesExtensionName = VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME;

  //=== VK_EXT_frame_boundary ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFrameBoundarySpecVersion   = VK_EXT_FRAME_BOUNDARY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFrameBoundaryExtensionName = VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME;

  //=== VK_EXT_multisampled_render_to_single_sampled ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMultisampledRenderToSingleSampledSpecVersion   = VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMultisampledRenderToSingleSampledExtensionName = VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME;

  //=== VK_EXT_extended_dynamic_state2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState2SpecVersion   = VK_EXT_EXTENDED_DYNAMIC_STATE_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState2ExtensionName = VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_screen_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXScreenSurfaceSpecVersion   = VK_QNX_SCREEN_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXScreenSurfaceExtensionName = VK_QNX_SCREEN_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  //=== VK_EXT_color_write_enable ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTColorWriteEnableSpecVersion   = VK_EXT_COLOR_WRITE_ENABLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTColorWriteEnableExtensionName = VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME;

  //=== VK_EXT_primitives_generated_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrimitivesGeneratedQuerySpecVersion   = VK_EXT_PRIMITIVES_GENERATED_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPrimitivesGeneratedQueryExtensionName = VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME;

  //=== VK_KHR_ray_tracing_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingMaintenance1SpecVersion   = VK_KHR_RAY_TRACING_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingMaintenance1ExtensionName = VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_KHR_shader_untyped_pointers ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderUntypedPointersSpecVersion   = VK_KHR_SHADER_UNTYPED_POINTERS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderUntypedPointersExtensionName = VK_KHR_SHADER_UNTYPED_POINTERS_EXTENSION_NAME;

  //=== VK_EXT_global_priority_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPriorityQuerySpecVersion   = VK_EXT_GLOBAL_PRIORITY_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTGlobalPriorityQueryExtensionName = VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME;

  //=== VK_VALVE_video_encode_rgb_conversion ===
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEVideoEncodeRgbConversionSpecVersion   = VK_VALVE_VIDEO_ENCODE_RGB_CONVERSION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEVideoEncodeRgbConversionExtensionName = VK_VALVE_VIDEO_ENCODE_RGB_CONVERSION_EXTENSION_NAME;

  //=== VK_EXT_image_view_min_lod ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageViewMinLodSpecVersion   = VK_EXT_IMAGE_VIEW_MIN_LOD_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageViewMinLodExtensionName = VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME;

  //=== VK_EXT_multi_draw ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMultiDrawSpecVersion   = VK_EXT_MULTI_DRAW_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMultiDrawExtensionName = VK_EXT_MULTI_DRAW_EXTENSION_NAME;

  //=== VK_EXT_image_2d_view_of_3d ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImage2DViewOf3DSpecVersion   = VK_EXT_IMAGE_2D_VIEW_OF_3D_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImage2DViewOf3DExtensionName = VK_EXT_IMAGE_2D_VIEW_OF_3D_EXTENSION_NAME;

  //=== VK_KHR_portability_enumeration ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPortabilityEnumerationSpecVersion   = VK_KHR_PORTABILITY_ENUMERATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPortabilityEnumerationExtensionName = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;

  //=== VK_EXT_shader_tile_image ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderTileImageSpecVersion   = VK_EXT_SHADER_TILE_IMAGE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderTileImageExtensionName = VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME;

  //=== VK_EXT_opacity_micromap ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTOpacityMicromapSpecVersion   = VK_EXT_OPACITY_MICROMAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTOpacityMicromapExtensionName = VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_displacement_micromap ===
  VULKAN_HPP_DEPRECATED( "The VK_NV_displacement_micromap extension has been deprecated by VK_NV_cluster_acceleration_structure." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDisplacementMicromapSpecVersion = VK_NV_DISPLACEMENT_MICROMAP_SPEC_VERSION;
  VULKAN_HPP_DEPRECATED( "The VK_NV_displacement_micromap extension has been deprecated by VK_NV_cluster_acceleration_structure." )
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDisplacementMicromapExtensionName = VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_EXT_load_store_op_none ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLoadStoreOpNoneSpecVersion   = VK_EXT_LOAD_STORE_OP_NONE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLoadStoreOpNoneExtensionName = VK_EXT_LOAD_STORE_OP_NONE_EXTENSION_NAME;

  //=== VK_HUAWEI_cluster_culling_shader ===
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIClusterCullingShaderSpecVersion   = VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIClusterCullingShaderExtensionName = VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME;

  //=== VK_EXT_border_color_swizzle ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBorderColorSwizzleSpecVersion   = VK_EXT_BORDER_COLOR_SWIZZLE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTBorderColorSwizzleExtensionName = VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME;

  //=== VK_EXT_pageable_device_local_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPageableDeviceLocalMemorySpecVersion   = VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPageableDeviceLocalMemoryExtensionName = VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME;

  //=== VK_KHR_maintenance4 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance4SpecVersion   = VK_KHR_MAINTENANCE_4_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance4ExtensionName = VK_KHR_MAINTENANCE_4_EXTENSION_NAME;

  //=== VK_ARM_shader_core_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMShaderCorePropertiesSpecVersion   = VK_ARM_SHADER_CORE_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMShaderCorePropertiesExtensionName = VK_ARM_SHADER_CORE_PROPERTIES_EXTENSION_NAME;

  //=== VK_KHR_shader_subgroup_rotate ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupRotateSpecVersion   = VK_KHR_SHADER_SUBGROUP_ROTATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderSubgroupRotateExtensionName = VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME;

  //=== VK_ARM_scheduling_controls ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMSchedulingControlsSpecVersion   = VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMSchedulingControlsExtensionName = VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME;

  //=== VK_EXT_image_sliced_view_of_3d ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageSlicedViewOf3DSpecVersion   = VK_EXT_IMAGE_SLICED_VIEW_OF_3D_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageSlicedViewOf3DExtensionName = VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME;

  //=== VK_VALVE_descriptor_set_host_mapping ===
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEDescriptorSetHostMappingSpecVersion   = VK_VALVE_DESCRIPTOR_SET_HOST_MAPPING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEDescriptorSetHostMappingExtensionName = VK_VALVE_DESCRIPTOR_SET_HOST_MAPPING_EXTENSION_NAME;

  //=== VK_EXT_depth_clamp_zero_one ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClampZeroOneSpecVersion   = VK_EXT_DEPTH_CLAMP_ZERO_ONE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClampZeroOneExtensionName = VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME;

  //=== VK_EXT_non_seamless_cube_map ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTNonSeamlessCubeMapSpecVersion   = VK_EXT_NON_SEAMLESS_CUBE_MAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTNonSeamlessCubeMapExtensionName = VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME;

  //=== VK_ARM_render_pass_striped ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMRenderPassStripedSpecVersion   = VK_ARM_RENDER_PASS_STRIPED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMRenderPassStripedExtensionName = VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME;

  //=== VK_QCOM_fragment_density_map_offset ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFragmentDensityMapOffsetSpecVersion   = VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFragmentDensityMapOffsetExtensionName = VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME;

  //=== VK_NV_copy_memory_indirect ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCopyMemoryIndirectSpecVersion   = VK_NV_COPY_MEMORY_INDIRECT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCopyMemoryIndirectExtensionName = VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME;

  //=== VK_NV_memory_decompression ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVMemoryDecompressionSpecVersion   = VK_NV_MEMORY_DECOMPRESSION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVMemoryDecompressionExtensionName = VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME;

  //=== VK_NV_device_generated_commands_compute ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceGeneratedCommandsComputeSpecVersion   = VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDeviceGeneratedCommandsComputeExtensionName = VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME;

  //=== VK_NV_ray_tracing_linear_swept_spheres ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingLinearSweptSpheresSpecVersion   = VK_NV_RAY_TRACING_LINEAR_SWEPT_SPHERES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingLinearSweptSpheresExtensionName = VK_NV_RAY_TRACING_LINEAR_SWEPT_SPHERES_EXTENSION_NAME;

  //=== VK_NV_linear_color_attachment ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLinearColorAttachmentSpecVersion   = VK_NV_LINEAR_COLOR_ATTACHMENT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLinearColorAttachmentExtensionName = VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME;

  //=== VK_GOOGLE_surfaceless_query ===
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLESurfacelessQuerySpecVersion   = VK_GOOGLE_SURFACELESS_QUERY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto GOOGLESurfacelessQueryExtensionName = VK_GOOGLE_SURFACELESS_QUERY_EXTENSION_NAME;

  //=== VK_KHR_shader_maximal_reconvergence ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderMaximalReconvergenceSpecVersion   = VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderMaximalReconvergenceExtensionName = VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME;

  //=== VK_EXT_image_compression_control_swapchain ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageCompressionControlSwapchainSpecVersion   = VK_EXT_IMAGE_COMPRESSION_CONTROL_SWAPCHAIN_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTImageCompressionControlSwapchainExtensionName = VK_EXT_IMAGE_COMPRESSION_CONTROL_SWAPCHAIN_EXTENSION_NAME;

  //=== VK_QCOM_image_processing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMImageProcessingSpecVersion   = VK_QCOM_IMAGE_PROCESSING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMImageProcessingExtensionName = VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME;

  //=== VK_EXT_nested_command_buffer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTNestedCommandBufferSpecVersion   = VK_EXT_NESTED_COMMAND_BUFFER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTNestedCommandBufferExtensionName = VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_OHOS )
  //=== VK_OHOS_external_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto OHOSExternalMemorySpecVersion   = VK_OHOS_EXTERNAL_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto OHOSExternalMemoryExtensionName = VK_OHOS_EXTERNAL_MEMORY_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_OHOS*/

  //=== VK_EXT_external_memory_acquire_unmodified ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryAcquireUnmodifiedSpecVersion   = VK_EXT_EXTERNAL_MEMORY_ACQUIRE_UNMODIFIED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryAcquireUnmodifiedExtensionName = VK_EXT_EXTERNAL_MEMORY_ACQUIRE_UNMODIFIED_EXTENSION_NAME;

  //=== VK_EXT_extended_dynamic_state3 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState3SpecVersion   = VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExtendedDynamicState3ExtensionName = VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME;

  //=== VK_EXT_subpass_merge_feedback ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubpassMergeFeedbackSpecVersion   = VK_EXT_SUBPASS_MERGE_FEEDBACK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTSubpassMergeFeedbackExtensionName = VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME;

  //=== VK_LUNARG_direct_driver_loading ===
  VULKAN_HPP_CONSTEXPR_INLINE auto LUNARGDirectDriverLoadingSpecVersion   = VK_LUNARG_DIRECT_DRIVER_LOADING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto LUNARGDirectDriverLoadingExtensionName = VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME;

  //=== VK_ARM_tensors ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMTensorsSpecVersion   = VK_ARM_TENSORS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMTensorsExtensionName = VK_ARM_TENSORS_EXTENSION_NAME;

  //=== VK_EXT_shader_module_identifier ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderModuleIdentifierSpecVersion   = VK_EXT_SHADER_MODULE_IDENTIFIER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderModuleIdentifierExtensionName = VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME;

  //=== VK_EXT_rasterization_order_attachment_access ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRasterizationOrderAttachmentAccessSpecVersion   = VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRasterizationOrderAttachmentAccessExtensionName = VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME;

  //=== VK_NV_optical_flow ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVOpticalFlowSpecVersion   = VK_NV_OPTICAL_FLOW_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVOpticalFlowExtensionName = VK_NV_OPTICAL_FLOW_EXTENSION_NAME;

  //=== VK_EXT_legacy_dithering ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLegacyDitheringSpecVersion   = VK_EXT_LEGACY_DITHERING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLegacyDitheringExtensionName = VK_EXT_LEGACY_DITHERING_EXTENSION_NAME;

  //=== VK_EXT_pipeline_protected_access ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineProtectedAccessSpecVersion   = VK_EXT_PIPELINE_PROTECTED_ACCESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineProtectedAccessExtensionName = VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
  //=== VK_ANDROID_external_format_resolve ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ANDROIDExternalFormatResolveSpecVersion   = VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ANDROIDExternalFormatResolveExtensionName = VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

  //=== VK_KHR_maintenance5 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance5SpecVersion   = VK_KHR_MAINTENANCE_5_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance5ExtensionName = VK_KHR_MAINTENANCE_5_EXTENSION_NAME;

  //=== VK_AMD_anti_lag ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDAntiLagSpecVersion   = VK_AMD_ANTI_LAG_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDAntiLagExtensionName = VK_AMD_ANTI_LAG_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_dense_geometry_format ===
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDXDenseGeometryFormatSpecVersion   = VK_AMDX_DENSE_GEOMETRY_FORMAT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto AMDXDenseGeometryFormatExtensionName = VK_AMDX_DENSE_GEOMETRY_FORMAT_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_KHR_present_id2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentId2SpecVersion   = VK_KHR_PRESENT_ID_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentId2ExtensionName = VK_KHR_PRESENT_ID_2_EXTENSION_NAME;

  //=== VK_KHR_present_wait2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentWait2SpecVersion   = VK_KHR_PRESENT_WAIT_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentWait2ExtensionName = VK_KHR_PRESENT_WAIT_2_EXTENSION_NAME;

  //=== VK_KHR_ray_tracing_position_fetch ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingPositionFetchSpecVersion   = VK_KHR_RAY_TRACING_POSITION_FETCH_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRayTracingPositionFetchExtensionName = VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME;

  //=== VK_EXT_shader_object ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderObjectSpecVersion   = VK_EXT_SHADER_OBJECT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderObjectExtensionName = VK_EXT_SHADER_OBJECT_EXTENSION_NAME;

  //=== VK_KHR_pipeline_binary ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineBinarySpecVersion   = VK_KHR_PIPELINE_BINARY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPipelineBinaryExtensionName = VK_KHR_PIPELINE_BINARY_EXTENSION_NAME;

  //=== VK_QCOM_tile_properties ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTilePropertiesSpecVersion   = VK_QCOM_TILE_PROPERTIES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTilePropertiesExtensionName = VK_QCOM_TILE_PROPERTIES_EXTENSION_NAME;

  //=== VK_SEC_amigo_profiling ===
  VULKAN_HPP_CONSTEXPR_INLINE auto SECAmigoProfilingSpecVersion   = VK_SEC_AMIGO_PROFILING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto SECAmigoProfilingExtensionName = VK_SEC_AMIGO_PROFILING_EXTENSION_NAME;

  //=== VK_KHR_surface_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceMaintenance1SpecVersion   = VK_KHR_SURFACE_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSurfaceMaintenance1ExtensionName = VK_KHR_SURFACE_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_KHR_swapchain_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMaintenance1SpecVersion   = VK_KHR_SWAPCHAIN_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRSwapchainMaintenance1ExtensionName = VK_KHR_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_QCOM_multiview_per_view_viewports ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMMultiviewPerViewViewportsSpecVersion   = VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMMultiviewPerViewViewportsExtensionName = VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME;

  //=== VK_NV_ray_tracing_invocation_reorder ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingInvocationReorderSpecVersion   = VK_NV_RAY_TRACING_INVOCATION_REORDER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingInvocationReorderExtensionName = VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME;

  //=== VK_NV_cooperative_vector ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeVectorSpecVersion   = VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeVectorExtensionName = VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME;

  //=== VK_NV_extended_sparse_address_space ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExtendedSparseAddressSpaceSpecVersion   = VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExtendedSparseAddressSpaceExtensionName = VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME;

  //=== VK_EXT_mutable_descriptor_type ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMutableDescriptorTypeSpecVersion   = VK_EXT_MUTABLE_DESCRIPTOR_TYPE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMutableDescriptorTypeExtensionName = VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME;

  //=== VK_EXT_legacy_vertex_attributes ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLegacyVertexAttributesSpecVersion   = VK_EXT_LEGACY_VERTEX_ATTRIBUTES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLegacyVertexAttributesExtensionName = VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME;

  //=== VK_EXT_layer_settings ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLayerSettingsSpecVersion   = VK_EXT_LAYER_SETTINGS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTLayerSettingsExtensionName = VK_EXT_LAYER_SETTINGS_EXTENSION_NAME;

  //=== VK_ARM_shader_core_builtins ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMShaderCoreBuiltinsSpecVersion   = VK_ARM_SHADER_CORE_BUILTINS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMShaderCoreBuiltinsExtensionName = VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME;

  //=== VK_EXT_pipeline_library_group_handles ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineLibraryGroupHandlesSpecVersion   = VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTPipelineLibraryGroupHandlesExtensionName = VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME;

  //=== VK_EXT_dynamic_rendering_unused_attachments ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDynamicRenderingUnusedAttachmentsSpecVersion   = VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDynamicRenderingUnusedAttachmentsExtensionName = VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME;

  //=== VK_KHR_internally_synchronized_queues ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRInternallySynchronizedQueuesSpecVersion   = VK_KHR_INTERNALLY_SYNCHRONIZED_QUEUES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRInternallySynchronizedQueuesExtensionName = VK_KHR_INTERNALLY_SYNCHRONIZED_QUEUES_EXTENSION_NAME;

  //=== VK_NV_low_latency2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLowLatency2SpecVersion   = VK_NV_LOW_LATENCY_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVLowLatency2ExtensionName = VK_NV_LOW_LATENCY_2_EXTENSION_NAME;

  //=== VK_KHR_cooperative_matrix ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCooperativeMatrixSpecVersion   = VK_KHR_COOPERATIVE_MATRIX_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCooperativeMatrixExtensionName = VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME;

  //=== VK_ARM_data_graph ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMDataGraphSpecVersion   = VK_ARM_DATA_GRAPH_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMDataGraphExtensionName = VK_ARM_DATA_GRAPH_EXTENSION_NAME;

  //=== VK_QCOM_multiview_per_view_render_areas ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMMultiviewPerViewRenderAreasSpecVersion   = VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMMultiviewPerViewRenderAreasExtensionName = VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME;

  //=== VK_KHR_compute_shader_derivatives ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRComputeShaderDerivativesSpecVersion   = VK_KHR_COMPUTE_SHADER_DERIVATIVES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRComputeShaderDerivativesExtensionName = VK_KHR_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME;

  //=== VK_KHR_video_decode_av1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeAv1SpecVersion   = VK_KHR_VIDEO_DECODE_AV1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeAv1ExtensionName = VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME;

  //=== VK_KHR_video_encode_av1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeAv1SpecVersion   = VK_KHR_VIDEO_ENCODE_AV1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeAv1ExtensionName = VK_KHR_VIDEO_ENCODE_AV1_EXTENSION_NAME;

  //=== VK_KHR_video_decode_vp9 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeVp9SpecVersion   = VK_KHR_VIDEO_DECODE_VP9_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoDecodeVp9ExtensionName = VK_KHR_VIDEO_DECODE_VP9_EXTENSION_NAME;

  //=== VK_KHR_video_maintenance1 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoMaintenance1SpecVersion   = VK_KHR_VIDEO_MAINTENANCE_1_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoMaintenance1ExtensionName = VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME;

  //=== VK_NV_per_stage_descriptor_set ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPerStageDescriptorSetSpecVersion   = VK_NV_PER_STAGE_DESCRIPTOR_SET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPerStageDescriptorSetExtensionName = VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME;

  //=== VK_QCOM_image_processing2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMImageProcessing2SpecVersion   = VK_QCOM_IMAGE_PROCESSING_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMImageProcessing2ExtensionName = VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME;

  //=== VK_QCOM_filter_cubic_weights ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFilterCubicWeightsSpecVersion   = VK_QCOM_FILTER_CUBIC_WEIGHTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFilterCubicWeightsExtensionName = VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME;

  //=== VK_QCOM_ycbcr_degamma ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMYcbcrDegammaSpecVersion   = VK_QCOM_YCBCR_DEGAMMA_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMYcbcrDegammaExtensionName = VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME;

  //=== VK_QCOM_filter_cubic_clamp ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFilterCubicClampSpecVersion   = VK_QCOM_FILTER_CUBIC_CLAMP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMFilterCubicClampExtensionName = VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME;

  //=== VK_EXT_attachment_feedback_loop_dynamic_state ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAttachmentFeedbackLoopDynamicStateSpecVersion   = VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTAttachmentFeedbackLoopDynamicStateExtensionName = VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME;

  //=== VK_KHR_vertex_attribute_divisor ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVertexAttributeDivisorSpecVersion   = VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVertexAttributeDivisorExtensionName = VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME;

  //=== VK_KHR_load_store_op_none ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRLoadStoreOpNoneSpecVersion   = VK_KHR_LOAD_STORE_OP_NONE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRLoadStoreOpNoneExtensionName = VK_KHR_LOAD_STORE_OP_NONE_EXTENSION_NAME;

  //=== VK_KHR_unified_image_layouts ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRUnifiedImageLayoutsSpecVersion   = VK_KHR_UNIFIED_IMAGE_LAYOUTS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRUnifiedImageLayoutsExtensionName = VK_KHR_UNIFIED_IMAGE_LAYOUTS_EXTENSION_NAME;

  //=== VK_KHR_shader_float_controls2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloatControls2SpecVersion   = VK_KHR_SHADER_FLOAT_CONTROLS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFloatControls2ExtensionName = VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
  //=== VK_QNX_external_memory_screen_buffer ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXExternalMemoryScreenBufferSpecVersion   = VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QNXExternalMemoryScreenBufferExtensionName = VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

  //=== VK_MSFT_layered_driver ===
  VULKAN_HPP_CONSTEXPR_INLINE auto MSFTLayeredDriverSpecVersion   = VK_MSFT_LAYERED_DRIVER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto MSFTLayeredDriverExtensionName = VK_MSFT_LAYERED_DRIVER_EXTENSION_NAME;

  //=== VK_KHR_index_type_uint8 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIndexTypeUint8SpecVersion   = VK_KHR_INDEX_TYPE_UINT8_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRIndexTypeUint8ExtensionName = VK_KHR_INDEX_TYPE_UINT8_EXTENSION_NAME;

  //=== VK_KHR_line_rasterization ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRLineRasterizationSpecVersion   = VK_KHR_LINE_RASTERIZATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRLineRasterizationExtensionName = VK_KHR_LINE_RASTERIZATION_EXTENSION_NAME;

  //=== VK_KHR_calibrated_timestamps ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCalibratedTimestampsSpecVersion   = VK_KHR_CALIBRATED_TIMESTAMPS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCalibratedTimestampsExtensionName = VK_KHR_CALIBRATED_TIMESTAMPS_EXTENSION_NAME;

  //=== VK_KHR_shader_expect_assume ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderExpectAssumeSpecVersion   = VK_KHR_SHADER_EXPECT_ASSUME_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderExpectAssumeExtensionName = VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME;

  //=== VK_KHR_maintenance6 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance6SpecVersion   = VK_KHR_MAINTENANCE_6_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance6ExtensionName = VK_KHR_MAINTENANCE_6_EXTENSION_NAME;

  //=== VK_NV_descriptor_pool_overallocation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDescriptorPoolOverallocationSpecVersion   = VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDescriptorPoolOverallocationExtensionName = VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME;

  //=== VK_QCOM_tile_memory_heap ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTileMemoryHeapSpecVersion   = VK_QCOM_TILE_MEMORY_HEAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMTileMemoryHeapExtensionName = VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME;

  //=== VK_KHR_copy_memory_indirect ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyMemoryIndirectSpecVersion   = VK_KHR_COPY_MEMORY_INDIRECT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRCopyMemoryIndirectExtensionName = VK_KHR_COPY_MEMORY_INDIRECT_EXTENSION_NAME;

  //=== VK_EXT_memory_decompression ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryDecompressionSpecVersion   = VK_EXT_MEMORY_DECOMPRESSION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTMemoryDecompressionExtensionName = VK_EXT_MEMORY_DECOMPRESSION_EXTENSION_NAME;

  //=== VK_NV_display_stereo ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDisplayStereoSpecVersion   = VK_NV_DISPLAY_STEREO_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVDisplayStereoExtensionName = VK_NV_DISPLAY_STEREO_EXTENSION_NAME;

  //=== VK_KHR_video_encode_intra_refresh ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeIntraRefreshSpecVersion   = VK_KHR_VIDEO_ENCODE_INTRA_REFRESH_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeIntraRefreshExtensionName = VK_KHR_VIDEO_ENCODE_INTRA_REFRESH_EXTENSION_NAME;

  //=== VK_KHR_video_encode_quantization_map ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeQuantizationMapSpecVersion   = VK_KHR_VIDEO_ENCODE_QUANTIZATION_MAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoEncodeQuantizationMapExtensionName = VK_KHR_VIDEO_ENCODE_QUANTIZATION_MAP_EXTENSION_NAME;

  //=== VK_NV_raw_access_chains ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRawAccessChainsSpecVersion   = VK_NV_RAW_ACCESS_CHAINS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRawAccessChainsExtensionName = VK_NV_RAW_ACCESS_CHAINS_EXTENSION_NAME;

  //=== VK_NV_external_compute_queue ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalComputeQueueSpecVersion   = VK_NV_EXTERNAL_COMPUTE_QUEUE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVExternalComputeQueueExtensionName = VK_NV_EXTERNAL_COMPUTE_QUEUE_EXTENSION_NAME;

  //=== VK_KHR_shader_relaxed_extended_instruction ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderRelaxedExtendedInstructionSpecVersion   = VK_KHR_SHADER_RELAXED_EXTENDED_INSTRUCTION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderRelaxedExtendedInstructionExtensionName = VK_KHR_SHADER_RELAXED_EXTENDED_INSTRUCTION_EXTENSION_NAME;

  //=== VK_NV_command_buffer_inheritance ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCommandBufferInheritanceSpecVersion   = VK_NV_COMMAND_BUFFER_INHERITANCE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCommandBufferInheritanceExtensionName = VK_NV_COMMAND_BUFFER_INHERITANCE_EXTENSION_NAME;

  //=== VK_KHR_maintenance7 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance7SpecVersion   = VK_KHR_MAINTENANCE_7_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance7ExtensionName = VK_KHR_MAINTENANCE_7_EXTENSION_NAME;

  //=== VK_NV_shader_atomic_float16_vector ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderAtomicFloat16VectorSpecVersion   = VK_NV_SHADER_ATOMIC_FLOAT16_VECTOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVShaderAtomicFloat16VectorExtensionName = VK_NV_SHADER_ATOMIC_FLOAT16_VECTOR_EXTENSION_NAME;

  //=== VK_EXT_shader_replicated_composites ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderReplicatedCompositesSpecVersion   = VK_EXT_SHADER_REPLICATED_COMPOSITES_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderReplicatedCompositesExtensionName = VK_EXT_SHADER_REPLICATED_COMPOSITES_EXTENSION_NAME;

  //=== VK_EXT_shader_float8 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderFloat8SpecVersion   = VK_EXT_SHADER_FLOAT8_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderFloat8ExtensionName = VK_EXT_SHADER_FLOAT8_EXTENSION_NAME;

  //=== VK_NV_ray_tracing_validation ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingValidationSpecVersion   = VK_NV_RAY_TRACING_VALIDATION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVRayTracingValidationExtensionName = VK_NV_RAY_TRACING_VALIDATION_EXTENSION_NAME;

  //=== VK_NV_cluster_acceleration_structure ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVClusterAccelerationStructureSpecVersion   = VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVClusterAccelerationStructureExtensionName = VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME;

  //=== VK_NV_partitioned_acceleration_structure ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPartitionedAccelerationStructureSpecVersion   = VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPartitionedAccelerationStructureExtensionName = VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME;

  //=== VK_EXT_device_generated_commands ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceGeneratedCommandsSpecVersion   = VK_EXT_DEVICE_GENERATED_COMMANDS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDeviceGeneratedCommandsExtensionName = VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME;

  //=== VK_KHR_maintenance8 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance8SpecVersion   = VK_KHR_MAINTENANCE_8_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance8ExtensionName = VK_KHR_MAINTENANCE_8_EXTENSION_NAME;

  //=== VK_MESA_image_alignment_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto MESAImageAlignmentControlSpecVersion   = VK_MESA_IMAGE_ALIGNMENT_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto MESAImageAlignmentControlExtensionName = VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME;

  //=== VK_KHR_shader_fma ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFmaSpecVersion   = VK_KHR_SHADER_FMA_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRShaderFmaExtensionName = VK_KHR_SHADER_FMA_EXTENSION_NAME;

  //=== VK_NV_push_constant_bank ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPushConstantBankSpecVersion   = VK_NV_PUSH_CONSTANT_BANK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPushConstantBankExtensionName = VK_NV_PUSH_CONSTANT_BANK_EXTENSION_NAME;

  //=== VK_EXT_ray_tracing_invocation_reorder ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRayTracingInvocationReorderSpecVersion   = VK_EXT_RAY_TRACING_INVOCATION_REORDER_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTRayTracingInvocationReorderExtensionName = VK_EXT_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME;

  //=== VK_EXT_depth_clamp_control ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClampControlSpecVersion   = VK_EXT_DEPTH_CLAMP_CONTROL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTDepthClampControlExtensionName = VK_EXT_DEPTH_CLAMP_CONTROL_EXTENSION_NAME;

  //=== VK_KHR_maintenance9 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance9SpecVersion   = VK_KHR_MAINTENANCE_9_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance9ExtensionName = VK_KHR_MAINTENANCE_9_EXTENSION_NAME;

  //=== VK_KHR_video_maintenance2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoMaintenance2SpecVersion   = VK_KHR_VIDEO_MAINTENANCE_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRVideoMaintenance2ExtensionName = VK_KHR_VIDEO_MAINTENANCE_2_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_OHOS )
  //=== VK_OHOS_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto OHOSSurfaceSpecVersion   = VK_OHOS_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto OHOSSurfaceExtensionName = VK_OHOS_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_OHOS*/

  //=== VK_HUAWEI_hdr_vivid ===
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIHdrVividSpecVersion   = VK_HUAWEI_HDR_VIVID_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto HUAWEIHdrVividExtensionName = VK_HUAWEI_HDR_VIVID_EXTENSION_NAME;

  //=== VK_NV_cooperative_matrix2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeMatrix2SpecVersion   = VK_NV_COOPERATIVE_MATRIX_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVCooperativeMatrix2ExtensionName = VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME;

  //=== VK_ARM_pipeline_opacity_micromap ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMPipelineOpacityMicromapSpecVersion   = VK_ARM_PIPELINE_OPACITY_MICROMAP_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMPipelineOpacityMicromapExtensionName = VK_ARM_PIPELINE_OPACITY_MICROMAP_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_METAL_EXT )
  //=== VK_EXT_external_memory_metal ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryMetalSpecVersion   = VK_EXT_EXTERNAL_MEMORY_METAL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTExternalMemoryMetalExtensionName = VK_EXT_EXTERNAL_MEMORY_METAL_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

  //=== VK_KHR_depth_clamp_zero_one ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDepthClampZeroOneSpecVersion   = VK_KHR_DEPTH_CLAMP_ZERO_ONE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRDepthClampZeroOneExtensionName = VK_KHR_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME;

  //=== VK_ARM_performance_counters_by_region ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMPerformanceCountersByRegionSpecVersion   = VK_ARM_PERFORMANCE_COUNTERS_BY_REGION_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMPerformanceCountersByRegionExtensionName = VK_ARM_PERFORMANCE_COUNTERS_BY_REGION_EXTENSION_NAME;

  //=== VK_EXT_vertex_attribute_robustness ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeRobustnessSpecVersion   = VK_EXT_VERTEX_ATTRIBUTE_ROBUSTNESS_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTVertexAttributeRobustnessExtensionName = VK_EXT_VERTEX_ATTRIBUTE_ROBUSTNESS_EXTENSION_NAME;

  //=== VK_ARM_format_pack ===
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMFormatPackSpecVersion   = VK_ARM_FORMAT_PACK_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto ARMFormatPackExtensionName = VK_ARM_FORMAT_PACK_EXTENSION_NAME;

  //=== VK_VALVE_fragment_density_map_layered ===
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEFragmentDensityMapLayeredSpecVersion   = VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEFragmentDensityMapLayeredExtensionName = VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME;

  //=== VK_KHR_robustness2 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRobustness2SpecVersion   = VK_KHR_ROBUSTNESS_2_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRRobustness2ExtensionName = VK_KHR_ROBUSTNESS_2_EXTENSION_NAME;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_present_metering ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPresentMeteringSpecVersion   = VK_NV_PRESENT_METERING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVPresentMeteringExtensionName = VK_NV_PRESENT_METERING_EXTENSION_NAME;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_EXT_fragment_density_map_offset ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMapOffsetSpecVersion   = VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTFragmentDensityMapOffsetExtensionName = VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME;

  //=== VK_EXT_zero_initialize_device_memory ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTZeroInitializeDeviceMemorySpecVersion   = VK_EXT_ZERO_INITIALIZE_DEVICE_MEMORY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTZeroInitializeDeviceMemoryExtensionName = VK_EXT_ZERO_INITIALIZE_DEVICE_MEMORY_EXTENSION_NAME;

  //=== VK_KHR_present_mode_fifo_latest_ready ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentModeFifoLatestReadySpecVersion   = VK_KHR_PRESENT_MODE_FIFO_LATEST_READY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRPresentModeFifoLatestReadyExtensionName = VK_KHR_PRESENT_MODE_FIFO_LATEST_READY_EXTENSION_NAME;

  //=== VK_EXT_shader_64bit_indexing ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShader64BitIndexingSpecVersion   = VK_EXT_SHADER_64BIT_INDEXING_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShader64BitIndexingExtensionName = VK_EXT_SHADER_64BIT_INDEXING_EXTENSION_NAME;

  //=== VK_EXT_custom_resolve ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomResolveSpecVersion   = VK_EXT_CUSTOM_RESOLVE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTCustomResolveExtensionName = VK_EXT_CUSTOM_RESOLVE_EXTENSION_NAME;

  //=== VK_QCOM_data_graph_model ===
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMDataGraphModelSpecVersion   = VK_QCOM_DATA_GRAPH_MODEL_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto QCOMDataGraphModelExtensionName = VK_QCOM_DATA_GRAPH_MODEL_EXTENSION_NAME;

  //=== VK_KHR_maintenance10 ===
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance10SpecVersion   = VK_KHR_MAINTENANCE_10_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto KHRMaintenance10ExtensionName = VK_KHR_MAINTENANCE_10_EXTENSION_NAME;

  //=== VK_EXT_shader_long_vector ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderLongVectorSpecVersion   = VK_EXT_SHADER_LONG_VECTOR_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderLongVectorExtensionName = VK_EXT_SHADER_LONG_VECTOR_EXTENSION_NAME;

  //=== VK_SEC_pipeline_cache_incremental_mode ===
  VULKAN_HPP_CONSTEXPR_INLINE auto SECPipelineCacheIncrementalModeSpecVersion   = VK_SEC_PIPELINE_CACHE_INCREMENTAL_MODE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto SECPipelineCacheIncrementalModeExtensionName = VK_SEC_PIPELINE_CACHE_INCREMENTAL_MODE_EXTENSION_NAME;

  //=== VK_EXT_shader_uniform_buffer_unsized_array ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderUniformBufferUnsizedArraySpecVersion   = VK_EXT_SHADER_UNIFORM_BUFFER_UNSIZED_ARRAY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderUniformBufferUnsizedArrayExtensionName = VK_EXT_SHADER_UNIFORM_BUFFER_UNSIZED_ARRAY_EXTENSION_NAME;

  //=== VK_NV_compute_occupancy_priority ===
  VULKAN_HPP_CONSTEXPR_INLINE auto NVComputeOccupancyPrioritySpecVersion   = VK_NV_COMPUTE_OCCUPANCY_PRIORITY_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto NVComputeOccupancyPriorityExtensionName = VK_NV_COMPUTE_OCCUPANCY_PRIORITY_EXTENSION_NAME;

  //=== VK_EXT_shader_subgroup_partitioned ===
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupPartitionedSpecVersion   = VK_EXT_SHADER_SUBGROUP_PARTITIONED_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto EXTShaderSubgroupPartitionedExtensionName = VK_EXT_SHADER_SUBGROUP_PARTITIONED_EXTENSION_NAME;

#if defined( VK_USE_PLATFORM_UBM_SEC )
  //=== VK_SEC_ubm_surface ===
  VULKAN_HPP_CONSTEXPR_INLINE auto SECUbmSurfaceSpecVersion   = VK_SEC_UBM_SURFACE_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto SECUbmSurfaceExtensionName = VK_SEC_UBM_SURFACE_EXTENSION_NAME;
#endif /*VK_USE_PLATFORM_UBM_SEC*/

  //=== VK_VALVE_shader_mixed_float_dot_product ===
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEShaderMixedFloatDotProductSpecVersion   = VK_VALVE_SHADER_MIXED_FLOAT_DOT_PRODUCT_SPEC_VERSION;
  VULKAN_HPP_CONSTEXPR_INLINE auto VALVEShaderMixedFloatDotProductExtensionName = VK_VALVE_SHADER_MIXED_FLOAT_DOT_PRODUCT_EXTENSION_NAME;

}  // namespace VULKAN_HPP_NAMESPACE

// clang-format off
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_funcs.hpp>
// clang-format on

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
  // define forward-declared DispatchLoader init() functions
  void DispatchLoader::init( Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
  {
    init( static_cast<VkInstance>( instanceCpp ) );
  }

  void DispatchLoader::init( Device deviceCpp ) VULKAN_HPP_NOEXCEPT
  {
    init( static_cast<VkDevice>( deviceCpp ) );
  }

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )

  //=======================
  //=== STRUCTS EXTENDS ===
  //=======================

  //=== VK_VERSION_1_0 ===
  template <>
  struct StructExtends<ShaderModuleCreateInfo, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ShaderModuleCreateInfo, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, BindDescriptorSetsInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushConstantsInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushDescriptorSetInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, PushDescriptorSetWithTemplateInfo>
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

  template <>
  struct StructExtends<PipelineLayoutCreateInfo, IndirectCommandsLayoutCreateInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VERSION_1_1 ===
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
  struct StructExtends<DeviceGroupBindSparseInfo, BindSparseInfo>
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
  struct StructExtends<ImageViewUsageCreateInfo, ImageViewCreateInfo>
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
  struct StructExtends<PhysicalDeviceMaintenance3Properties, PhysicalDeviceProperties2>
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
  struct StructExtends<PhysicalDeviceDriverProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

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
  struct StructExtends<RenderPassAttachmentBeginInfo, RenderPassBeginInfo>
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
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, ExecutionGraphPipelineCreateInfoAMDX>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/
  template <>
  struct StructExtends<PipelineCreationFeedbackCreateInfo, DataGraphPipelineCreateInfoARM>
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

  //=== VK_VERSION_1_4 ===
  template <>
  struct StructExtends<PhysicalDeviceVulkan14Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan14Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVulkan14Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceQueueGlobalPriorityCreateInfo, DeviceQueueCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceGlobalPriorityQueryFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceGlobalPriorityQueryFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueueFamilyGlobalPriorityProperties, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceIndexTypeUint8Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceIndexTypeUint8Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance5Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance5Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance5Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferUsageFlags2CreateInfo, BufferViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferUsageFlags2CreateInfo, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferUsageFlags2CreateInfo, PhysicalDeviceExternalBufferInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferUsageFlags2CreateInfo, DescriptorBufferBindingInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance6Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance6Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance6Properties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindMemoryStatus, BindBufferMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BindMemoryStatus, BindImageMemoryInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHostImageCopyFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHostImageCopyFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHostImageCopyProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SubresourceHostMemcpySize, SubresourceLayout2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<HostImageCopyDevicePerformanceQuery, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupRotateFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupRotateFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFloatControls2Features, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFloatControls2Features, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderExpectAssumeFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderExpectAssumeFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreateFlags2CreateInfo, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreateFlags2CreateInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreateFlags2CreateInfo, RayTracingPipelineCreateInfoNV>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCreateFlags2CreateInfo, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePushDescriptorProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineProtectedAccessFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineProtectedAccessFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineRobustnessFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineRobustnessFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineRobustnessProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRobustnessCreateInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRobustnessCreateInfo, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRobustnessCreateInfo, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRobustnessCreateInfo, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLineRasterizationFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLineRasterizationFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLineRasterizationProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRasterizationLineStateCreateInfo, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeDivisorProperties, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineVertexInputDivisorStateCreateInfo, PipelineVertexInputStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeDivisorFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeDivisorFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingLocalReadFeatures, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingLocalReadFeatures, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingAttachmentLocationInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingAttachmentLocationInfo, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingInputAttachmentIndexInfo, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingInputAttachmentIndexInfo, CommandBufferInheritanceInfo>
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

  //=== VK_EXT_debug_report ===
  template <>
  struct StructExtends<DebugReportCallbackCreateInfoEXT, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_rasterization_order ===
  template <>
  struct StructExtends<PipelineRasterizationStateRasterizationOrderAMD, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_queue ===
  template <>
  struct StructExtends<QueueFamilyQueryResultStatusPropertiesKHR, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueueFamilyVideoPropertiesKHR, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoProfileListInfoKHR, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoProfileListInfoKHR, PhysicalDeviceVideoFormatInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoProfileListInfoKHR, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoProfileListInfoKHR, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_decode_queue ===
  template <>
  struct StructExtends<VideoDecodeCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeUsageInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeUsageInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_dedicated_allocation ===
  template <>
  struct StructExtends<DedicatedAllocationImageCreateInfoNV, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DedicatedAllocationBufferCreateInfoNV, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DedicatedAllocationMemoryAllocateInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_transform_feedback ===
  template <>
  struct StructExtends<PhysicalDeviceTransformFeedbackFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTransformFeedbackFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTransformFeedbackPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRasterizationStateStreamCreateInfoEXT, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NVX_binary_import ===
  template <>
  struct StructExtends<CuModuleTexturingModeCreateInfoNVX, CuModuleCreateInfoNVX>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_h264 ===
  template <>
  struct StructExtends<VideoEncodeH264CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264QualityLevelPropertiesKHR, VideoEncodeQualityLevelPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264SessionCreateInfoKHR, VideoSessionCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264SessionParametersAddInfoKHR, VideoSessionParametersUpdateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264SessionParametersGetInfoKHR, VideoEncodeSessionParametersGetInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264SessionParametersFeedbackInfoKHR, VideoEncodeSessionParametersFeedbackInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264PictureInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264RateControlInfoKHR, VideoCodingControlInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264RateControlInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264RateControlLayerInfoKHR, VideoEncodeRateControlLayerInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264GopRemainingFrameInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_h265 ===
  template <>
  struct StructExtends<VideoEncodeH265CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265SessionCreateInfoKHR, VideoSessionCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265QualityLevelPropertiesKHR, VideoEncodeQualityLevelPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265SessionParametersAddInfoKHR, VideoSessionParametersUpdateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265SessionParametersGetInfoKHR, VideoEncodeSessionParametersGetInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265SessionParametersFeedbackInfoKHR, VideoEncodeSessionParametersFeedbackInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265PictureInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265RateControlInfoKHR, VideoCodingControlInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265RateControlInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265RateControlLayerInfoKHR, VideoEncodeRateControlLayerInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265GopRemainingFrameInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_decode_h264 ===
  template <>
  struct StructExtends<VideoDecodeH264ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264SessionParametersAddInfoKHR, VideoSessionParametersUpdateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264PictureInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_texture_gather_bias_lod ===
  template <>
  struct StructExtends<TextureLODGatherFormatPropertiesAMD, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_corner_sampled_image ===
  template <>
  struct StructExtends<PhysicalDeviceCornerSampledImageFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCornerSampledImageFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_external_memory ===
  template <>
  struct StructExtends<ExternalMemoryImageCreateInfoNV, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMemoryAllocateInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_NV_external_memory_win32 ===
  template <>
  struct StructExtends<ImportMemoryWin32HandleInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMemoryWin32HandleInfoNV, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_NV_win32_keyed_mutex ===
  template <>
  struct StructExtends<Win32KeyedMutexAcquireReleaseInfoNV, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<Win32KeyedMutexAcquireReleaseInfoNV, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_EXT_validation_flags ===
  template <>
  struct StructExtends<ValidationFlagsEXT, InstanceCreateInfo>
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

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_memory_win32 ===
  template <>
  struct StructExtends<ImportMemoryWin32HandleInfoKHR, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMemoryWin32HandleInfoKHR, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_KHR_external_memory_fd ===
  template <>
  struct StructExtends<ImportMemoryFdInfoKHR, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_win32_keyed_mutex ===
  template <>
  struct StructExtends<Win32KeyedMutexAcquireReleaseInfoKHR, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<Win32KeyedMutexAcquireReleaseInfoKHR, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_semaphore_win32 ===
  template <>
  struct StructExtends<ExportSemaphoreWin32HandleInfoKHR, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<D3D12FenceSubmitInfoKHR, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

  //=== VK_EXT_conditional_rendering ===
  template <>
  struct StructExtends<PhysicalDeviceConditionalRenderingFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceConditionalRenderingFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandBufferInheritanceConditionalRenderingInfoEXT, CommandBufferInheritanceInfo>
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

  //=== VK_NV_clip_space_w_scaling ===
  template <>
  struct StructExtends<PipelineViewportWScalingStateCreateInfoNV, PipelineViewportStateCreateInfo>
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

  //=== VK_GOOGLE_display_timing ===
  template <>
  struct StructExtends<PresentTimesInfoGOOGLE, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NVX_multiview_per_view_attributes ===
  template <>
  struct StructExtends<PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultiviewPerViewAttributesInfoNVX, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultiviewPerViewAttributesInfoNVX, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultiviewPerViewAttributesInfoNVX, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_viewport_swizzle ===
  template <>
  struct StructExtends<PipelineViewportSwizzleStateCreateInfoNV, PipelineViewportStateCreateInfo>
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

  //=== VK_IMG_relaxed_line_rasterization ===
  template <>
  struct StructExtends<PhysicalDeviceRelaxedLineRasterizationFeaturesIMG, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRelaxedLineRasterizationFeaturesIMG, DeviceCreateInfo>
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

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_KHR_external_fence_win32 ===
  template <>
  struct StructExtends<ExportFenceWin32HandleInfoKHR, FenceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

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

  template <>
  struct StructExtends<DebugUtilsObjectNameInfoEXT, ResourceDescriptorInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DebugUtilsObjectNameInfoEXT, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_ANDROID_KHR )
  //=== VK_ANDROID_external_memory_android_hardware_buffer ===
  template <>
  struct StructExtends<AndroidHardwareBufferUsageANDROID, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AndroidHardwareBufferFormatPropertiesANDROID, AndroidHardwareBufferPropertiesANDROID>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportAndroidHardwareBufferInfoANDROID, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatANDROID, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatANDROID, SamplerYcbcrConversionCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatANDROID, AttachmentDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatANDROID, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatANDROID, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AndroidHardwareBufferFormatProperties2ANDROID, AndroidHardwareBufferPropertiesANDROID>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_shader_enqueue ===
  template <>
  struct StructExtends<PhysicalDeviceShaderEnqueueFeaturesAMDX, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderEnqueueFeaturesAMDX, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderEnqueuePropertiesAMDX, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineShaderStageNodeCreateInfoAMDX, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_EXT_descriptor_heap ===
  template <>
  struct StructExtends<ShaderDescriptorSetAndBindingMappingInfoEXT, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ShaderDescriptorSetAndBindingMappingInfoEXT, ShaderCreateInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDataCreateInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDataCreateInfoEXT, TensorCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorHeapFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorHeapFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorHeapPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandBufferInheritanceDescriptorHeapInfoEXT, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerCustomBorderColorIndexCreateInfoEXT, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<IndirectCommandsLayoutPushDataTokenNV, IndirectCommandsLayoutTokenNV>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SubsampledImageFormatPropertiesEXT, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorHeapTensorPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_mixed_attachment_samples ===
  template <>
  struct StructExtends<AttachmentSampleCountInfoAMD, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AttachmentSampleCountInfoAMD, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_bfloat16 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderBfloat16FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderBfloat16FeaturesKHR, DeviceCreateInfo>
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

  //=== VK_NV_fragment_coverage_to_color ===
  template <>
  struct StructExtends<PipelineCoverageToColorStateCreateInfoNV, PipelineMultisampleStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_acceleration_structure ===
  template <>
  struct StructExtends<WriteDescriptorSetAccelerationStructureKHR, WriteDescriptorSet>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAccelerationStructureFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAccelerationStructureFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAccelerationStructurePropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_ray_tracing_pipeline ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingPipelineFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingPipelineFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingPipelinePropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_ray_query ===
  template <>
  struct StructExtends<PhysicalDeviceRayQueryFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayQueryFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_framebuffer_mixed_samples ===
  template <>
  struct StructExtends<PipelineCoverageModulationStateCreateInfoNV, PipelineMultisampleStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_shader_sm_builtins ===
  template <>
  struct StructExtends<PhysicalDeviceShaderSMBuiltinsPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSMBuiltinsFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSMBuiltinsFeaturesNV, DeviceCreateInfo>
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

  //=== VK_EXT_validation_cache ===
  template <>
  struct StructExtends<ShaderModuleValidationCacheCreateInfoEXT, ShaderModuleCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ShaderModuleValidationCacheCreateInfoEXT, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_KHR_portability_subset ===
  template <>
  struct StructExtends<PhysicalDevicePortabilitySubsetFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePortabilitySubsetFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePortabilitySubsetPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_NV_shading_rate_image ===
  template <>
  struct StructExtends<PipelineViewportShadingRateImageStateCreateInfoNV, PipelineViewportStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShadingRateImageFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShadingRateImageFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShadingRateImagePropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineViewportCoarseSampleOrderStateCreateInfoNV, PipelineViewportStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_ray_tracing ===
  template <>
  struct StructExtends<WriteDescriptorSetAccelerationStructureNV, WriteDescriptorSet>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_representative_fragment_test ===
  template <>
  struct StructExtends<PhysicalDeviceRepresentativeFragmentTestFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRepresentativeFragmentTestFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRepresentativeFragmentTestStateCreateInfoNV, GraphicsPipelineCreateInfo>
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

  //=== VK_QCOM_cooperative_matrix_conversion ===
  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixConversionFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixConversionFeaturesQCOM, DeviceCreateInfo>
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

  //=== VK_AMD_pipeline_compiler_control ===
  template <>
  struct StructExtends<PipelineCompilerControlCreateInfoAMD, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCompilerControlCreateInfoAMD, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  template <>
  struct StructExtends<PipelineCompilerControlCreateInfoAMD, ExecutionGraphPipelineCreateInfoAMDX>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_AMD_shader_core_properties ===
  template <>
  struct StructExtends<PhysicalDeviceShaderCorePropertiesAMD, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_decode_h265 ===
  template <>
  struct StructExtends<VideoDecodeH265ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265SessionParametersAddInfoKHR, VideoSessionParametersUpdateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265PictureInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_memory_overallocation_behavior ===
  template <>
  struct StructExtends<DeviceMemoryOverallocationCreateInfoAMD, DeviceCreateInfo>
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

#  if defined( VK_USE_PLATFORM_GGP )
  //=== VK_GGP_frame_token ===
  template <>
  struct StructExtends<PresentFrameTokenGGP, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_GGP*/

  //=== VK_NV_mesh_shader ===
  template <>
  struct StructExtends<PhysicalDeviceMeshShaderFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMeshShaderFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMeshShaderPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_shader_image_footprint ===
  template <>
  struct StructExtends<PhysicalDeviceShaderImageFootprintFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderImageFootprintFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_scissor_exclusive ===
  template <>
  struct StructExtends<PipelineViewportExclusiveScissorStateCreateInfoNV, PipelineViewportStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExclusiveScissorFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExclusiveScissorFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_device_diagnostic_checkpoints ===
  template <>
  struct StructExtends<QueueFamilyCheckpointPropertiesNV, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueueFamilyCheckpointProperties2NV, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_present_timing ===
  template <>
  struct StructExtends<PhysicalDevicePresentTimingFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentTimingFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PresentTimingSurfaceCapabilitiesEXT, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainCalibratedTimestampInfoEXT, CalibratedTimestampInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PresentTimingsInfoEXT, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_INTEL_shader_integer_functions2 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_INTEL_performance_query ===
  template <>
  struct StructExtends<QueryPoolPerformanceQueryCreateInfoINTEL, QueryPoolCreateInfo>
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

  //=== VK_AMD_display_native_hdr ===
  template <>
  struct StructExtends<DisplayNativeHdrSurfaceCapabilitiesAMD, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainDisplayNativeHdrCreateInfoAMD, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_fragment_density_map ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassFragmentDensityMapCreateInfoEXT, RenderPassCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassFragmentDensityMapCreateInfoEXT, RenderPassCreateInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingFragmentDensityMapAttachmentInfoEXT, RenderingInfo>
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

  template <>
  struct StructExtends<RenderingFragmentShadingRateAttachmentInfoKHR, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_shader_core_properties2 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderCoreProperties2AMD, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_device_coherent_memory ===
  template <>
  struct StructExtends<PhysicalDeviceCoherentMemoryFeaturesAMD, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCoherentMemoryFeaturesAMD, DeviceCreateInfo>
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

  //=== VK_KHR_shader_quad_control ===
  template <>
  struct StructExtends<PhysicalDeviceShaderQuadControlFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderQuadControlFeaturesKHR, DeviceCreateInfo>
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

  //=== VK_EXT_memory_priority ===
  template <>
  struct StructExtends<PhysicalDeviceMemoryPriorityFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMemoryPriorityFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryPriorityAllocateInfoEXT, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_surface_protected_capabilities ===
  template <>
  struct StructExtends<SurfaceProtectedCapabilitiesKHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_dedicated_allocation_image_aliasing ===
  template <>
  struct StructExtends<PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_buffer_device_address ===
  template <>
  struct StructExtends<PhysicalDeviceBufferDeviceAddressFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBufferDeviceAddressFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferDeviceAddressCreateInfoEXT, BufferCreateInfo>
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

  template <>
  struct StructExtends<ValidationFeaturesEXT, ShaderModuleCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ValidationFeaturesEXT, ShaderCreateInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_present_wait ===
  template <>
  struct StructExtends<PhysicalDevicePresentWaitFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentWaitFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_cooperative_matrix ===
  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_coverage_reduction_mode ===
  template <>
  struct StructExtends<PhysicalDeviceCoverageReductionModeFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCoverageReductionModeFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineCoverageReductionStateCreateInfoNV, PipelineMultisampleStateCreateInfo>
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

  //=== VK_EXT_provoking_vertex ===
  template <>
  struct StructExtends<PhysicalDeviceProvokingVertexFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceProvokingVertexFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceProvokingVertexPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineRasterizationProvokingVertexStateCreateInfoEXT, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_WIN32_KHR )
  //=== VK_EXT_full_screen_exclusive ===
  template <>
  struct StructExtends<SurfaceFullScreenExclusiveInfoEXT, PhysicalDeviceSurfaceInfo2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfaceFullScreenExclusiveInfoEXT, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfaceCapabilitiesFullScreenExclusiveEXT, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfaceFullScreenExclusiveWin32InfoEXT, PhysicalDeviceSurfaceInfo2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfaceFullScreenExclusiveWin32InfoEXT, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_WIN32_KHR*/

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

  //=== VK_KHR_pipeline_executable_properties ===
  template <>
  struct StructExtends<PhysicalDevicePipelineExecutablePropertiesFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineExecutablePropertiesFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_map_memory_placed ===
  template <>
  struct StructExtends<PhysicalDeviceMapMemoryPlacedFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMapMemoryPlacedFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMapMemoryPlacedPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryMapPlacedInfoEXT, MemoryMapInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_atomic_float2 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloat2FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloat2FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_device_generated_commands ===
  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GraphicsPipelineShaderGroupsCreateInfoNV, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_inherited_viewport_scissor ===
  template <>
  struct StructExtends<PhysicalDeviceInheritedViewportScissorFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInheritedViewportScissorFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandBufferInheritanceViewportScissorInfoNV, CommandBufferInheritanceInfo>
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

  //=== VK_QCOM_render_pass_transform ===
  template <>
  struct StructExtends<RenderPassTransformBeginInfoQCOM, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CommandBufferInheritanceRenderPassTransformInfoQCOM, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_depth_bias_control ===
  template <>
  struct StructExtends<PhysicalDeviceDepthBiasControlFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthBiasControlFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DepthBiasRepresentationInfoEXT, DepthBiasInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DepthBiasRepresentationInfoEXT, PipelineRasterizationStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_device_memory_report ===
  template <>
  struct StructExtends<PhysicalDeviceDeviceMemoryReportFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceMemoryReportFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceDeviceMemoryReportCreateInfoEXT, DeviceCreateInfo>
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

  //=== VK_EXT_texture_compression_astc_3d ===
  template <>
  struct StructExtends<PhysicalDeviceTextureCompressionASTC3DFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTextureCompressionASTC3DFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_pipeline_library ===
  template <>
  struct StructExtends<PipelineLibraryCreateInfoKHR, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_present_barrier ===
  template <>
  struct StructExtends<PhysicalDevicePresentBarrierFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentBarrierFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfaceCapabilitiesPresentBarrierNV, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainPresentBarrierCreateInfoNV, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_present_id ===
  template <>
  struct StructExtends<PresentIdKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentIdFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentIdFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_queue ===
  template <>
  struct StructExtends<VideoEncodeCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueryPoolVideoEncodeFeedbackCreateInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeUsageInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeUsageInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeRateControlInfoKHR, VideoCodingControlInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeRateControlInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeQualityLevelInfoKHR, VideoCodingControlInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeQualityLevelInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_device_diagnostics_config ===
  template <>
  struct StructExtends<PhysicalDeviceDiagnosticsConfigFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDiagnosticsConfigFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceDiagnosticsConfigCreateInfoNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_cuda_kernel_launch ===
  template <>
  struct StructExtends<PhysicalDeviceCudaKernelLaunchFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCudaKernelLaunchFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCudaKernelLaunchPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_QCOM_tile_shading ===
  template <>
  struct StructExtends<PhysicalDeviceTileShadingFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTileShadingFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTileShadingPropertiesQCOM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassTileShadingCreateInfoQCOM, RenderPassCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassTileShadingCreateInfoQCOM, RenderPassCreateInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassTileShadingCreateInfoQCOM, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassTileShadingCreateInfoQCOM, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_low_latency ===
  template <>
  struct StructExtends<QueryLowLatencySupportNV, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_METAL_EXT )
  //=== VK_EXT_metal_objects ===
  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, BufferViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalObjectCreateInfoEXT, EventCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalDeviceInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalCommandQueueInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalBufferInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMetalBufferInfoEXT, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalTextureInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMetalTextureInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalIOSurfaceInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMetalIOSurfaceInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExportMetalSharedEventInfoEXT, ExportMetalObjectsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMetalSharedEventInfoEXT, SemaphoreCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportMetalSharedEventInfoEXT, EventCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_METAL_EXT*/

  //=== VK_EXT_descriptor_buffer ===
  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorBufferBindingPushDescriptorBufferHandleEXT, DescriptorBufferBindingInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, AccelerationStructureCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, AccelerationStructureCreateInfoNV>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, TensorCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpaqueCaptureDescriptorDataCreateInfoEXT, TensorViewCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_graphics_pipeline_library ===
  template <>
  struct StructExtends<PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GraphicsPipelineLibraryCreateInfoEXT, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_AMD_shader_early_and_late_fragment_tests ===
  template <>
  struct StructExtends<PhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_fragment_shader_barycentric ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentShaderBarycentricFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShaderBarycentricFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShaderBarycentricPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_subgroup_uniform_control_flow ===
  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_fragment_shading_rate_enums ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRateEnumsFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRateEnumsFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentShadingRateEnumsPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineFragmentShadingRateEnumStateCreateInfoNV, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_ray_tracing_motion_blur ===
  template <>
  struct StructExtends<AccelerationStructureGeometryMotionTrianglesDataNV, AccelerationStructureGeometryTrianglesDataKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureMotionInfoNV, AccelerationStructureCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingMotionBlurFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingMotionBlurFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_mesh_shader ===
  template <>
  struct StructExtends<PhysicalDeviceMeshShaderFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMeshShaderFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMeshShaderPropertiesEXT, PhysicalDeviceProperties2>
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

  //=== VK_EXT_fragment_density_map2 ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMap2FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMap2FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMap2PropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_rotated_copy_commands ===
  template <>
  struct StructExtends<CopyCommandTransformInfoQCOM, BufferImageCopy2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CopyCommandTransformInfoQCOM, ImageBlit2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_workgroup_memory_explicit_layout ===
  template <>
  struct StructExtends<PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_compression_control ===
  template <>
  struct StructExtends<PhysicalDeviceImageCompressionControlFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageCompressionControlFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionControlEXT, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionControlEXT, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionControlEXT, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionPropertiesEXT, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionPropertiesEXT, SurfaceFormat2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageCompressionPropertiesEXT, SubresourceLayout2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_attachment_feedback_loop_layout ===
  template <>
  struct StructExtends<PhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT, DeviceCreateInfo>
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

  //=== VK_EXT_device_fault ===
  template <>
  struct StructExtends<PhysicalDeviceFaultFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFaultFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_rgba10x6_formats ===
  template <>
  struct StructExtends<PhysicalDeviceRGBA10X6FormatsFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRGBA10X6FormatsFeaturesEXT, DeviceCreateInfo>
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

  //=== VK_EXT_physical_device_drm ===
  template <>
  struct StructExtends<PhysicalDeviceDrmPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_device_address_binding_report ===
  template <>
  struct StructExtends<PhysicalDeviceAddressBindingReportFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAddressBindingReportFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceAddressBindingCallbackDataEXT, DebugUtilsMessengerCallbackDataEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_depth_clip_control ===
  template <>
  struct StructExtends<PhysicalDeviceDepthClipControlFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthClipControlFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineViewportDepthClipControlCreateInfoEXT, PipelineViewportStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_primitive_topology_list_restart ===
  template <>
  struct StructExtends<PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_external_memory ===
  template <>
  struct StructExtends<ImportMemoryZirconHandleInfoFUCHSIA, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_FUCHSIA*/

#  if defined( VK_USE_PLATFORM_FUCHSIA )
  //=== VK_FUCHSIA_buffer_collection ===
  template <>
  struct StructExtends<ImportMemoryBufferCollectionFUCHSIA, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferCollectionImageCreateInfoFUCHSIA, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BufferCollectionBufferCreateInfoFUCHSIA, BufferCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_FUCHSIA*/

  //=== VK_HUAWEI_subpass_shading ===
  template <>
  struct StructExtends<SubpassShadingPipelineCreateInfoHUAWEI, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubpassShadingFeaturesHUAWEI, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubpassShadingFeaturesHUAWEI, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubpassShadingPropertiesHUAWEI, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_HUAWEI_invocation_mask ===
  template <>
  struct StructExtends<PhysicalDeviceInvocationMaskFeaturesHUAWEI, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInvocationMaskFeaturesHUAWEI, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_external_memory_rdma ===
  template <>
  struct StructExtends<PhysicalDeviceExternalMemoryRDMAFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalMemoryRDMAFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_pipeline_properties ===
  template <>
  struct StructExtends<PhysicalDevicePipelinePropertiesFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelinePropertiesFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_frame_boundary ===
  template <>
  struct StructExtends<PhysicalDeviceFrameBoundaryFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFrameBoundaryFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryEXT, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryEXT, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryEXT, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryEXT, BindSparseInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_multisampled_render_to_single_sampled ===
  template <>
  struct StructExtends<PhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SubpassResolvePerformanceQueryEXT, FormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultisampledRenderToSingleSampledInfoEXT, SubpassDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultisampledRenderToSingleSampledInfoEXT, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

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

  //=== VK_EXT_primitives_generated_query ===
  template <>
  struct StructExtends<PhysicalDevicePrimitivesGeneratedQueryFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePrimitivesGeneratedQueryFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_ray_tracing_maintenance1 ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingMaintenance1FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingMaintenance1FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_untyped_pointers ===
  template <>
  struct StructExtends<PhysicalDeviceShaderUntypedPointersFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderUntypedPointersFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VALVE_video_encode_rgb_conversion ===
  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeRgbConversionFeaturesVALVE, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeRgbConversionFeaturesVALVE, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeRgbConversionCapabilitiesVALVE, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeProfileRgbConversionInfoVALVE, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeSessionRgbConversionCreateInfoVALVE, VideoSessionCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_view_min_lod ===
  template <>
  struct StructExtends<PhysicalDeviceImageViewMinLodFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageViewMinLodFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageViewMinLodCreateInfoEXT, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_multi_draw ===
  template <>
  struct StructExtends<PhysicalDeviceMultiDrawFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiDrawFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiDrawPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_2d_view_of_3d ===
  template <>
  struct StructExtends<PhysicalDeviceImage2DViewOf3DFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImage2DViewOf3DFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_tile_image ===
  template <>
  struct StructExtends<PhysicalDeviceShaderTileImageFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderTileImageFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderTileImagePropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_opacity_micromap ===
  template <>
  struct StructExtends<PhysicalDeviceOpacityMicromapFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceOpacityMicromapFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceOpacityMicromapPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureTrianglesOpacityMicromapEXT, AccelerationStructureGeometryTrianglesDataKHR>
  {
    enum
    {
      value = true
    };
  };
#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  template <>
  struct StructExtends<AccelerationStructureTrianglesOpacityMicromapEXT, AccelerationStructureDenseGeometryFormatTrianglesDataAMDX>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_displacement_micromap ===
  template <>
  struct StructExtends<PhysicalDeviceDisplacementMicromapFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDisplacementMicromapFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDisplacementMicromapPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureTrianglesDisplacementMicromapNV, AccelerationStructureGeometryTrianglesDataKHR>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_HUAWEI_cluster_culling_shader ===
  template <>
  struct StructExtends<PhysicalDeviceClusterCullingShaderFeaturesHUAWEI, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceClusterCullingShaderFeaturesHUAWEI, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceClusterCullingShaderPropertiesHUAWEI, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI, PhysicalDeviceClusterCullingShaderFeaturesHUAWEI>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_border_color_swizzle ===
  template <>
  struct StructExtends<PhysicalDeviceBorderColorSwizzleFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceBorderColorSwizzleFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerBorderColorComponentMappingCreateInfoEXT, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_pageable_device_local_memory ===
  template <>
  struct StructExtends<PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_shader_core_properties ===
  template <>
  struct StructExtends<PhysicalDeviceShaderCorePropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_scheduling_controls ===
  template <>
  struct StructExtends<DeviceQueueShaderCoreControlCreateInfoARM, DeviceQueueCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DeviceQueueShaderCoreControlCreateInfoARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSchedulingControlsFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSchedulingControlsFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSchedulingControlsPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_sliced_view_of_3d ===
  template <>
  struct StructExtends<PhysicalDeviceImageSlicedViewOf3DFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageSlicedViewOf3DFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageViewSlicedCreateInfoEXT, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VALVE_descriptor_set_host_mapping ===
  template <>
  struct StructExtends<PhysicalDeviceDescriptorSetHostMappingFeaturesVALVE, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorSetHostMappingFeaturesVALVE, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_non_seamless_cube_map ===
  template <>
  struct StructExtends<PhysicalDeviceNonSeamlessCubeMapFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceNonSeamlessCubeMapFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_render_pass_striped ===
  template <>
  struct StructExtends<PhysicalDeviceRenderPassStripedFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRenderPassStripedFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRenderPassStripedPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassStripeBeginInfoARM, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassStripeBeginInfoARM, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassStripeSubmitInfoARM, CommandBufferSubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_copy_memory_indirect ===
  template <>
  struct StructExtends<PhysicalDeviceCopyMemoryIndirectFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCopyMemoryIndirectFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_device_generated_commands_compute ===
  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ComputePipelineIndirectBufferInfoNV, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_ray_tracing_linear_swept_spheres ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureGeometryLinearSweptSpheresDataNV, AccelerationStructureGeometryKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureGeometrySpheresDataNV, AccelerationStructureGeometryKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_linear_color_attachment ===
  template <>
  struct StructExtends<PhysicalDeviceLinearColorAttachmentFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLinearColorAttachmentFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_maximal_reconvergence ===
  template <>
  struct StructExtends<PhysicalDeviceShaderMaximalReconvergenceFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderMaximalReconvergenceFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_image_compression_control_swapchain ===
  template <>
  struct StructExtends<PhysicalDeviceImageCompressionControlSwapchainFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageCompressionControlSwapchainFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_image_processing ===
  template <>
  struct StructExtends<ImageViewSampleWeightCreateInfoQCOM, ImageViewCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageProcessingFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageProcessingFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageProcessingPropertiesQCOM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_nested_command_buffer ===
  template <>
  struct StructExtends<PhysicalDeviceNestedCommandBufferFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceNestedCommandBufferFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceNestedCommandBufferPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_OHOS )
  //=== VK_OHOS_external_memory ===
  template <>
  struct StructExtends<NativeBufferUsageOHOS, ImageFormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<NativeBufferFormatPropertiesOHOS, NativeBufferPropertiesOHOS>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImportNativeBufferInfoOHOS, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatOHOS, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatOHOS, SamplerYcbcrConversionCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatOHOS, AttachmentDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatOHOS, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalFormatOHOS, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_OHOS*/

  //=== VK_EXT_external_memory_acquire_unmodified ===
  template <>
  struct StructExtends<ExternalMemoryAcquireUnmodifiedEXT, BufferMemoryBarrier>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryAcquireUnmodifiedEXT, BufferMemoryBarrier2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryAcquireUnmodifiedEXT, ImageMemoryBarrier>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryAcquireUnmodifiedEXT, ImageMemoryBarrier2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_extended_dynamic_state3 ===
  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicState3FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicState3FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedDynamicState3PropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_subpass_merge_feedback ===
  template <>
  struct StructExtends<PhysicalDeviceSubpassMergeFeedbackFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSubpassMergeFeedbackFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassCreationControlEXT, RenderPassCreateInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassCreationControlEXT, SubpassDescription2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassCreationFeedbackCreateInfoEXT, RenderPassCreateInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassSubpassFeedbackCreateInfoEXT, SubpassDescription2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_LUNARG_direct_driver_loading ===
  template <>
  struct StructExtends<DirectDriverLoadingListLUNARG, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_tensors ===
  template <>
  struct StructExtends<TensorDescriptionARM, DataGraphPipelineResourceInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TensorDescriptionARM, DataGraphPipelineConstantARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<WriteDescriptorSetTensorARM, WriteDescriptorSet>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TensorFormatPropertiesARM, FormatProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTensorPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TensorMemoryBarrierARM, DependencyInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TensorDependencyInfoARM, DependencyInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTensorFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTensorFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryDedicatedAllocateInfoTensorARM, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ExternalMemoryTensorCreateInfoARM, TensorCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferTensorFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferTensorFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorBufferTensorPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DescriptorGetTensorInfoARM, DescriptorGetInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryTensorsARM, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryTensorsARM, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryTensorsARM, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<FrameBoundaryTensorsARM, BindSparseInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_module_identifier ===
  template <>
  struct StructExtends<PhysicalDeviceShaderModuleIdentifierFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderModuleIdentifierFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderModuleIdentifierPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineShaderStageModuleIdentifierCreateInfoEXT, PipelineShaderStageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_rasterization_order_attachment_access ===
  template <>
  struct StructExtends<PhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_optical_flow ===
  template <>
  struct StructExtends<PhysicalDeviceOpticalFlowFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceOpticalFlowFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceOpticalFlowPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpticalFlowImageFormatInfoNV, PhysicalDeviceImageFormatInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpticalFlowImageFormatInfoNV, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<OpticalFlowSessionCreatePrivateDataInfoNV, OpticalFlowSessionCreateInfoNV>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_legacy_dithering ===
  template <>
  struct StructExtends<PhysicalDeviceLegacyDitheringFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLegacyDitheringFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_ANDROID_KHR )
  //=== VK_ANDROID_external_format_resolve ===
  template <>
  struct StructExtends<PhysicalDeviceExternalFormatResolveFeaturesANDROID, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalFormatResolveFeaturesANDROID, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalFormatResolvePropertiesANDROID, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AndroidHardwareBufferFormatResolvePropertiesANDROID, AndroidHardwareBufferPropertiesANDROID>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_ANDROID_KHR*/

  //=== VK_AMD_anti_lag ===
  template <>
  struct StructExtends<PhysicalDeviceAntiLagFeaturesAMD, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAntiLagFeaturesAMD, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_AMDX_dense_geometry_format ===
  template <>
  struct StructExtends<PhysicalDeviceDenseGeometryFormatFeaturesAMDX, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDenseGeometryFormatFeaturesAMDX, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AccelerationStructureDenseGeometryFormatTrianglesDataAMDX, AccelerationStructureGeometryKHR>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_KHR_present_id2 ===
  template <>
  struct StructExtends<SurfaceCapabilitiesPresentId2KHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PresentId2KHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentId2FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentId2FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_present_wait2 ===
  template <>
  struct StructExtends<SurfaceCapabilitiesPresentWait2KHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentWait2FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentWait2FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_ray_tracing_position_fetch ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingPositionFetchFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingPositionFetchFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_object ===
  template <>
  struct StructExtends<PhysicalDeviceShaderObjectFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderObjectFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderObjectPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_pipeline_binary ===
  template <>
  struct StructExtends<PhysicalDevicePipelineBinaryFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineBinaryFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineBinaryPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DevicePipelineBinaryInternalCacheControlKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineBinaryInfoKHR, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineBinaryInfoKHR, ComputePipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineBinaryInfoKHR, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_tile_properties ===
  template <>
  struct StructExtends<PhysicalDeviceTilePropertiesFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTilePropertiesFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_SEC_amigo_profiling ===
  template <>
  struct StructExtends<PhysicalDeviceAmigoProfilingFeaturesSEC, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAmigoProfilingFeaturesSEC, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AmigoProfilingSubmitInfoSEC, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_surface_maintenance1 ===
  template <>
  struct StructExtends<SurfacePresentModeKHR, PhysicalDeviceSurfaceInfo2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfacePresentScalingCapabilitiesKHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SurfacePresentModeCompatibilityKHR, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_swapchain_maintenance1 ===
  template <>
  struct StructExtends<PhysicalDeviceSwapchainMaintenance1FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceSwapchainMaintenance1FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainPresentFenceInfoKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainPresentModesCreateInfoKHR, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainPresentModeInfoKHR, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainPresentScalingCreateInfoKHR, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_multiview_per_view_viewports ===
  template <>
  struct StructExtends<PhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_ray_tracing_invocation_reorder ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_cooperative_vector ===
  template <>
  struct StructExtends<PhysicalDeviceCooperativeVectorPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeVectorFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeVectorFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_extended_sparse_address_space ===
  template <>
  struct StructExtends<PhysicalDeviceExtendedSparseAddressSpaceFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedSparseAddressSpaceFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExtendedSparseAddressSpacePropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_mutable_descriptor_type ===
  template <>
  struct StructExtends<PhysicalDeviceMutableDescriptorTypeFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMutableDescriptorTypeFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MutableDescriptorTypeCreateInfoEXT, DescriptorSetLayoutCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MutableDescriptorTypeCreateInfoEXT, DescriptorPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_legacy_vertex_attributes ===
  template <>
  struct StructExtends<PhysicalDeviceLegacyVertexAttributesFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLegacyVertexAttributesFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLegacyVertexAttributesPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_layer_settings ===
  template <>
  struct StructExtends<LayerSettingsCreateInfoEXT, InstanceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_shader_core_builtins ===
  template <>
  struct StructExtends<PhysicalDeviceShaderCoreBuiltinsFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderCoreBuiltinsFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderCoreBuiltinsPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_pipeline_library_group_handles ===
  template <>
  struct StructExtends<PhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_dynamic_rendering_unused_attachments ===
  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_internally_synchronized_queues ===
  template <>
  struct StructExtends<PhysicalDeviceInternallySynchronizedQueuesFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceInternallySynchronizedQueuesFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_low_latency2 ===
  template <>
  struct StructExtends<LatencySubmissionPresentIdNV, SubmitInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<LatencySubmissionPresentIdNV, SubmitInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SwapchainLatencyCreateInfoNV, SwapchainCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<LatencySurfaceCapabilitiesNV, SurfaceCapabilities2KHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_cooperative_matrix ===
  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrixPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_data_graph ===
  template <>
  struct StructExtends<PhysicalDeviceDataGraphFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDataGraphFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphPipelineCompilerControlCreateInfoARM, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphPipelineShaderModuleCreateInfoARM, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphPipelineIdentifierCreateInfoARM, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphProcessingEngineCreateInfoARM, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphProcessingEngineCreateInfoARM, DescriptorPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphProcessingEngineCreateInfoARM, CommandPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DataGraphPipelineConstantTensorSemiStructuredSparsityInfoARM, DataGraphPipelineConstantARM>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_multiview_per_view_render_areas ===
  template <>
  struct StructExtends<PhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultiviewPerViewRenderAreasRenderPassBeginInfoQCOM, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MultiviewPerViewRenderAreasRenderPassBeginInfoQCOM, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_compute_shader_derivatives ===
  template <>
  struct StructExtends<PhysicalDeviceComputeShaderDerivativesFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceComputeShaderDerivativesFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceComputeShaderDerivativesPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_decode_av1 ===
  template <>
  struct StructExtends<VideoDecodeAV1ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1PictureInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_av1 ===
  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeAV1FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeAV1FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1QualityLevelPropertiesKHR, VideoEncodeQualityLevelPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1SessionCreateInfoKHR, VideoSessionCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1SessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1PictureInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1DpbSlotInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1GopRemainingFrameInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1RateControlInfoKHR, VideoCodingControlInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1RateControlInfoKHR, VideoBeginCodingInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1RateControlLayerInfoKHR, VideoEncodeRateControlLayerInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_decode_vp9 ===
  template <>
  struct StructExtends<PhysicalDeviceVideoDecodeVP9FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoDecodeVP9FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeVP9ProfileInfoKHR, VideoProfileInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeVP9ProfileInfoKHR, QueryPoolCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeVP9CapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeVP9PictureInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_maintenance1 ===
  template <>
  struct StructExtends<PhysicalDeviceVideoMaintenance1FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoMaintenance1FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoInlineQueryInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoInlineQueryInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_per_stage_descriptor_set ===
  template <>
  struct StructExtends<PhysicalDevicePerStageDescriptorSetFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePerStageDescriptorSetFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_image_processing2 ===
  template <>
  struct StructExtends<PhysicalDeviceImageProcessing2FeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageProcessing2FeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageProcessing2PropertiesQCOM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerBlockMatchWindowCreateInfoQCOM, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_filter_cubic_weights ===
  template <>
  struct StructExtends<PhysicalDeviceCubicWeightsFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCubicWeightsFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerCubicWeightsCreateInfoQCOM, SamplerCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<BlitImageCubicWeightsInfoQCOM, BlitImageInfo2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_ycbcr_degamma ===
  template <>
  struct StructExtends<PhysicalDeviceYcbcrDegammaFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceYcbcrDegammaFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<SamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM, SamplerYcbcrConversionCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_filter_cubic_clamp ===
  template <>
  struct StructExtends<PhysicalDeviceCubicClampFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCubicClampFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_attachment_feedback_loop_dynamic_state ===
  template <>
  struct StructExtends<PhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_unified_image_layouts ===
  template <>
  struct StructExtends<PhysicalDeviceUnifiedImageLayoutsFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceUnifiedImageLayoutsFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<AttachmentFeedbackLoopInfoEXT, RenderingAttachmentInfo>
  {
    enum
    {
      value = true
    };
  };

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

  //=== VK_MSFT_layered_driver ===
  template <>
  struct StructExtends<PhysicalDeviceLayeredDriverPropertiesMSFT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_descriptor_pool_overallocation ===
  template <>
  struct StructExtends<PhysicalDeviceDescriptorPoolOverallocationFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDescriptorPoolOverallocationFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_tile_memory_heap ===
  template <>
  struct StructExtends<PhysicalDeviceTileMemoryHeapFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTileMemoryHeapFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceTileMemoryHeapPropertiesQCOM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TileMemoryRequirementsQCOM, MemoryRequirements2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TileMemoryBindInfoQCOM, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TileMemorySizeInfoQCOM, RenderPassCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TileMemorySizeInfoQCOM, RenderPassCreateInfo2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<TileMemorySizeInfoQCOM, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_copy_memory_indirect ===
  template <>
  struct StructExtends<PhysicalDeviceCopyMemoryIndirectFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCopyMemoryIndirectFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCopyMemoryIndirectPropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_memory_decompression ===
  template <>
  struct StructExtends<PhysicalDeviceMemoryDecompressionFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMemoryDecompressionFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMemoryDecompressionPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_display_stereo ===
  template <>
  struct StructExtends<DisplaySurfaceStereoCreateInfoNV, DisplaySurfaceCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<DisplayModeStereoPropertiesNV, DisplayModeProperties2KHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_intra_refresh ===
  template <>
  struct StructExtends<VideoEncodeIntraRefreshCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeSessionIntraRefreshCreateInfoKHR, VideoSessionCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeIntraRefreshInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoReferenceIntraRefreshInfoKHR, VideoReferenceSlotInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeIntraRefreshFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeIntraRefreshFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_encode_quantization_map ===
  template <>
  struct StructExtends<VideoEncodeQuantizationMapCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoFormatQuantizationMapPropertiesKHR, VideoFormatPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeQuantizationMapInfoKHR, VideoEncodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeQuantizationMapSessionParametersCreateInfoKHR, VideoSessionParametersCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH264QuantizationMapCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeH265QuantizationMapCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoFormatH265QuantizationMapPropertiesKHR, VideoFormatPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoEncodeAV1QuantizationMapCapabilitiesKHR, VideoCapabilitiesKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoFormatAV1QuantizationMapPropertiesKHR, VideoFormatPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_raw_access_chains ===
  template <>
  struct StructExtends<PhysicalDeviceRawAccessChainsFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRawAccessChainsFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_external_compute_queue ===
  template <>
  struct StructExtends<ExternalComputeQueueDeviceCreateInfoNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceExternalComputeQueuePropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_relaxed_extended_instruction ===
  template <>
  struct StructExtends<PhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_command_buffer_inheritance ===
  template <>
  struct StructExtends<PhysicalDeviceCommandBufferInheritanceFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCommandBufferInheritanceFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_maintenance7 ===
  template <>
  struct StructExtends<PhysicalDeviceMaintenance7FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance7FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance7PropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLayeredApiPropertiesListKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceLayeredApiVulkanPropertiesKHR, PhysicalDeviceLayeredApiPropertiesKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_shader_atomic_float16_vector ===
  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloat16VectorFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderAtomicFloat16VectorFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_replicated_composites ===
  template <>
  struct StructExtends<PhysicalDeviceShaderReplicatedCompositesFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderReplicatedCompositesFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_float8 ===
  template <>
  struct StructExtends<PhysicalDeviceShaderFloat8FeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFloat8FeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_ray_tracing_validation ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingValidationFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingValidationFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_cluster_acceleration_structure ===
  template <>
  struct StructExtends<PhysicalDeviceClusterAccelerationStructureFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceClusterAccelerationStructureFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceClusterAccelerationStructurePropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RayTracingPipelineClusterAccelerationStructureCreateInfoNV, RayTracingPipelineCreateInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_partitioned_acceleration_structure ===
  template <>
  struct StructExtends<PhysicalDevicePartitionedAccelerationStructureFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePartitionedAccelerationStructureFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePartitionedAccelerationStructurePropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PartitionedAccelerationStructureFlagsNV, PartitionedAccelerationStructureInstancesInputNV>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<WriteDescriptorSetPartitionedAccelerationStructureNV, WriteDescriptorSet>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_device_generated_commands ===
  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDeviceGeneratedCommandsPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GeneratedCommandsPipelineInfoEXT, GeneratedCommandsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GeneratedCommandsPipelineInfoEXT, GeneratedCommandsMemoryRequirementsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GeneratedCommandsShaderInfoEXT, GeneratedCommandsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<GeneratedCommandsShaderInfoEXT, GeneratedCommandsMemoryRequirementsInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_maintenance8 ===
  template <>
  struct StructExtends<MemoryBarrierAccessFlags3KHR, SubpassDependency2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryBarrierAccessFlags3KHR, BufferMemoryBarrier2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<MemoryBarrierAccessFlags3KHR, ImageMemoryBarrier2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance8FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance8FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_MESA_image_alignment_control ===
  template <>
  struct StructExtends<PhysicalDeviceImageAlignmentControlFeaturesMESA, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageAlignmentControlFeaturesMESA, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceImageAlignmentControlPropertiesMESA, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ImageAlignmentControlCreateInfoMESA, ImageCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_shader_fma ===
  template <>
  struct StructExtends<PhysicalDeviceShaderFmaFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderFmaFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_push_constant_bank ===
  template <>
  struct StructExtends<PushConstantBankInfoNV, DescriptorSetAndBindingMappingEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PushConstantBankInfoNV, PushDataInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PushConstantBankInfoNV, PushConstantsInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PushConstantBankInfoNV, IndirectCommandsLayoutTokenEXT>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePushConstantBankFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePushConstantBankFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePushConstantBankPropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_ray_tracing_invocation_reorder ===
  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRayTracingInvocationReorderFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_depth_clamp_control ===
  template <>
  struct StructExtends<PhysicalDeviceDepthClampControlFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthClampControlFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineViewportDepthClampControlCreateInfoEXT, PipelineViewportStateCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_maintenance9 ===
  template <>
  struct StructExtends<PhysicalDeviceMaintenance9FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance9FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance9PropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<QueueFamilyOwnershipTransferPropertiesKHR, QueueFamilyProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_video_maintenance2 ===
  template <>
  struct StructExtends<PhysicalDeviceVideoMaintenance2FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVideoMaintenance2FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH264InlineSessionParametersInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeH265InlineSessionParametersInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<VideoDecodeAV1InlineSessionParametersInfoKHR, VideoDecodeInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_HUAWEI_hdr_vivid ===
  template <>
  struct StructExtends<PhysicalDeviceHdrVividFeaturesHUAWEI, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceHdrVividFeaturesHUAWEI, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<HdrVividDynamicMetadataHUAWEI, HdrMetadataEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_cooperative_matrix2 ===
  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrix2FeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrix2FeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCooperativeMatrix2PropertiesNV, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_pipeline_opacity_micromap ===
  template <>
  struct StructExtends<PhysicalDevicePipelineOpacityMicromapFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineOpacityMicromapFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_USE_PLATFORM_METAL_EXT )
  //=== VK_EXT_external_memory_metal ===
  template <>
  struct StructExtends<ImportMemoryMetalHandleInfoEXT, MemoryAllocateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_USE_PLATFORM_METAL_EXT*/

  //=== VK_KHR_depth_clamp_zero_one ===
  template <>
  struct StructExtends<PhysicalDeviceDepthClampZeroOneFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDepthClampZeroOneFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_performance_counters_by_region ===
  template <>
  struct StructExtends<PhysicalDevicePerformanceCountersByRegionFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePerformanceCountersByRegionFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePerformanceCountersByRegionPropertiesARM, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassPerformanceCountersByRegionBeginInfoARM, RenderPassBeginInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassPerformanceCountersByRegionBeginInfoARM, RenderingInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_vertex_attribute_robustness ===
  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeRobustnessFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceVertexAttributeRobustnessFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_ARM_format_pack ===
  template <>
  struct StructExtends<PhysicalDeviceFormatPackFeaturesARM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFormatPackFeaturesARM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VALVE_fragment_density_map_layered ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapLayeredFeaturesVALVE, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapLayeredFeaturesVALVE, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapLayeredPropertiesVALVE, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PipelineFragmentDensityMapLayeredCreateInfoVALVE, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_robustness2 ===
  template <>
  struct StructExtends<PhysicalDeviceRobustness2FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRobustness2FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceRobustness2PropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

#  if defined( VK_ENABLE_BETA_EXTENSIONS )
  //=== VK_NV_present_metering ===
  template <>
  struct StructExtends<SetPresentConfigNV, PresentInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentMeteringFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentMeteringFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };
#  endif /*VK_ENABLE_BETA_EXTENSIONS*/

  //=== VK_EXT_fragment_density_map_offset ===
  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapOffsetFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapOffsetFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceFragmentDensityMapOffsetPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassFragmentDensityMapOffsetEndInfoEXT, SubpassEndInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderPassFragmentDensityMapOffsetEndInfoEXT, RenderingEndInfoKHR>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_zero_initialize_device_memory ===
  template <>
  struct StructExtends<PhysicalDeviceZeroInitializeDeviceMemoryFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceZeroInitializeDeviceMemoryFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_present_mode_fifo_latest_ready ===
  template <>
  struct StructExtends<PhysicalDevicePresentModeFifoLatestReadyFeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePresentModeFifoLatestReadyFeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_64bit_indexing ===
  template <>
  struct StructExtends<PhysicalDeviceShader64BitIndexingFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShader64BitIndexingFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_custom_resolve ===
  template <>
  struct StructExtends<PhysicalDeviceCustomResolveFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceCustomResolveFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CustomResolveCreateInfoEXT, GraphicsPipelineCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CustomResolveCreateInfoEXT, CommandBufferInheritanceInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<CustomResolveCreateInfoEXT, ShaderCreateInfoEXT>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_QCOM_data_graph_model ===
  template <>
  struct StructExtends<DataGraphPipelineBuiltinModelCreateInfoQCOM, DataGraphPipelineCreateInfoARM>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDataGraphModelFeaturesQCOM, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceDataGraphModelFeaturesQCOM, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_KHR_maintenance10 ===
  template <>
  struct StructExtends<PhysicalDeviceMaintenance10FeaturesKHR, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance10FeaturesKHR, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceMaintenance10PropertiesKHR, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<RenderingAttachmentFlagsInfoKHR, RenderingAttachmentInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<ResolveImageModeInfoKHR, ResolveImageInfo2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_long_vector ===
  template <>
  struct StructExtends<PhysicalDeviceShaderLongVectorFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderLongVectorFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderLongVectorPropertiesEXT, PhysicalDeviceProperties2>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_SEC_pipeline_cache_incremental_mode ===
  template <>
  struct StructExtends<PhysicalDevicePipelineCacheIncrementalModeFeaturesSEC, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDevicePipelineCacheIncrementalModeFeaturesSEC, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_uniform_buffer_unsized_array ===
  template <>
  struct StructExtends<PhysicalDeviceShaderUniformBufferUnsizedArrayFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderUniformBufferUnsizedArrayFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_NV_compute_occupancy_priority ===
  template <>
  struct StructExtends<PhysicalDeviceComputeOccupancyPriorityFeaturesNV, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceComputeOccupancyPriorityFeaturesNV, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_EXT_shader_subgroup_partitioned ===
  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupPartitionedFeaturesEXT, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderSubgroupPartitionedFeaturesEXT, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

  //=== VK_VALVE_shader_mixed_float_dot_product ===
  template <>
  struct StructExtends<PhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE, PhysicalDeviceFeatures2>
  {
    enum
    {
      value = true
    };
  };

  template <>
  struct StructExtends<PhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE, DeviceCreateInfo>
  {
    enum
    {
      value = true
    };
  };

#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE
}  // namespace VULKAN_HPP_NAMESPACE
#endif
