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
