
  namespace detail
  {
    VULKAN_HPP_INLINE void resultCheck( Result result, char const * message )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_NAMESPACE::detail::ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      VULKAN_HPP_NAMESPACE::detail::ignore( message );
      VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
#else
      if ( result != Result::eSuccess )
      {
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, message );
      }
#endif
    }

    VULKAN_HPP_INLINE void resultCheck( Result result, char const * message, std::initializer_list<Result> successCodes )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      VULKAN_HPP_NAMESPACE::detail::ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      VULKAN_HPP_NAMESPACE::detail::ignore( message );
      VULKAN_HPP_NAMESPACE::detail::ignore( successCodes );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
      if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
      {
        VULKAN_HPP_NAMESPACE::detail::throwResultException( result, message );
      }
#endif
    }
  }  // namespace detail
