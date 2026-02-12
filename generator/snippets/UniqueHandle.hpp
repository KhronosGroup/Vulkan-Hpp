#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
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

#  if defined( VULKAN_HPP_SMART_HANDLE_IMPLICIT_CAST )
  operator Type() const VULKAN_HPP_NOEXCEPT
  {
    return m_value;
  }
#  endif

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
#endif