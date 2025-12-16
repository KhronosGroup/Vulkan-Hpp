
  namespace detail
  {
    VULKAN_HPP_INLINE void resultCheck( Result result, char const * message )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      ignore( result );		// just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
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
      ignore( result );  		// just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      ignore( message );
      ignore( successCodes );	// just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
      if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
      {
        throwResultException( result, message );
      }
#endif
    }

    template <typename HandleType, typename AllocatorType, typename Dispatch>
    VULKAN_HPP_INLINE void resultCheck( Result                                         result,
                                        char const *                                   message,
                                        std::initializer_list<Result>                  successCodes,
                                        VkDevice                                       device,
                                        std::vector<HandleType, AllocatorType> const & pipelines,
                                        AllocationCallbacks const *                    pAllocator,
                                        Dispatch const &                               d )
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      ignore( result );  		// just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
      ignore( message );
      ignore( successCodes );	// just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
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
          d.vkDestroyPipeline( device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<VkAllocationCallbacks const*>( pAllocator ) );
        }
        throwResultException( result, message );
      }
#endif
    }
  }  // namespace detail
