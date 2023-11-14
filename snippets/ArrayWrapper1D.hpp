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
