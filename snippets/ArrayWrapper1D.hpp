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
