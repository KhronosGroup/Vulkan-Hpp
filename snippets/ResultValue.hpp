  template <typename T>
  struct ResultValue
  {
#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T & v ) VULKAN_HPP_NOEXCEPT(VULKAN_HPP_NOEXCEPT(T(v)))
#else
    ResultValue( Result r, T & v )
#endif
      : result( r )
      , value( v )
    {}

#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T && v ) VULKAN_HPP_NOEXCEPT(VULKAN_HPP_NOEXCEPT(T(std::move(v))))
#else
    ResultValue( Result r, T && v )
#endif
      : result( r )
      , value( std::move( v ) )
    {}

    Result  result;
    T       value;

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
      VULKAN_HPP_ASSERT(has_value ());
      return value;
    }

    T & operator*() VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(has_value ());
      return value;
    }
  };

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
#ifdef VULKAN_HPP_EXPECTED
  using type = VULKAN_HPP_EXPECTED<T, Result>;
#else
	using type = ResultValue<T>;
#endif
#else
	using type = T;
#endif
  };

  template <>
  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
#ifdef VULKAN_HPP_EXPECTED
    using type = VULKAN_HPP_EXPECTED<void, Result>;
#else
    using type = Result;
#endif
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
#ifdef VULKAN_HPP_UNEXPECTED
      if (result == Result::eSuccess)
      {
          return {};
      }
      return VULKAN_HPP_UNEXPECTED(result);
#else
      return result;
#endif
#else
      ignore( result );
#endif
    }

    template <typename T>
    VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T & data )
    {
#if defined( VULKAN_HPP_NO_EXCEPTIONS )
#ifdef VULKAN_HPP_EXPECTED
      if (result == Result::eSuccess)
      {
          return data;
      }
      return VULKAN_HPP_UNEXPECTED(data);
#else
      return ResultValue<T>( result, data );
#endif
#else
      ignore( result );
      return data;
#endif
    }

    template <typename T>
    VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValueType( Result result, T && data )
    {
#if defined( VULKAN_HPP_NO_EXCEPTIONS )
#ifdef VULKAN_HPP_EXPECTED
      if (result == Result::eSuccess)
      {
          return std::move(data);
      }
      return VULKAN_HPP_UNEXPECTED(result);
#else
      return ResultValue<T>( result, std::move( data ) );
#endif
#else
      ignore( result );
      return std::move( data );
#endif
    }
  }  // namespace detail
