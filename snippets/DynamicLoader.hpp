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
#  if defined( _WIN32 )
        m_library = ::LoadLibraryA( vulkanLibraryName.c_str() );
#  elif defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined(__Fuchsia__)
        m_library = dlopen( vulkanLibraryName.c_str(), RTLD_NOW | RTLD_LOCAL );
#  else
#    error unsupported platform
#  endif
      }
      else
      {
#  if defined( _WIN32 )
        m_library = ::LoadLibraryA( "vulkan-1.dll" );
#  elif defined( __APPLE__ )
        m_library = dlopen( "libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
        if ( !m_library )
        {
          m_library = dlopen( "libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL );
        }
        if ( !m_library )
		{
		  m_library = dlopen( "libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL );
		}
		// Add support for using Vulkan and MoltenVK in a Framework. App store rules for iOS
		// strictly enforce no .dylib's. If they aren't found it just falls through
		if ( !m_library )
		{
		  m_library = dlopen( "vulkan.framework/vulkan", RTLD_NOW | RTLD_LOCAL );
		}
		if ( !m_library )
		{
		  m_library = dlopen( "MoltenVK.framework/MoltenVK", RTLD_NOW | RTLD_LOCAL );
		}
		// modern versions of macOS don't search /usr/local/lib automatically contrary to what man dlopen says
		// Vulkan SDK uses this as the system-wide installation location, so we're going to fallback to this if all else fails
		if ( !m_library && ( getenv("DYLD_FALLBACK_LIBRARY_PATH") == NULL ) )
		{
		  m_library = dlopen( "/usr/local/lib/libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
		}
#  elif defined( __unix__ ) || defined( __QNX__ ) || defined(__Fuchsia__)
        m_library = dlopen( "libvulkan.so", RTLD_NOW | RTLD_LOCAL );
        if ( !m_library )
        {
          m_library = dlopen( "libvulkan.so.1", RTLD_NOW | RTLD_LOCAL );
        }
#  else
#    error unsupported platform
#  endif
      }

#ifndef VULKAN_HPP_NO_EXCEPTIONS
      if ( !m_library )
      {
        // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
        throw std::runtime_error( "Failed to load vulkan library!" );
      }
#endif
    }

    DynamicLoader( DynamicLoader const & ) = delete;

    DynamicLoader( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT : m_library(other.m_library)
    {
      other.m_library = nullptr;
    }

    DynamicLoader &operator=( DynamicLoader const & ) = delete;

    DynamicLoader &operator=( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
    {
      std::swap(m_library, other.m_library);
      return *this;
    }

    ~DynamicLoader() VULKAN_HPP_NOEXCEPT
    {
      if ( m_library )
      {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined(__Fuchsia__)
        dlclose( m_library );
#  elif defined( _WIN32 )
        ::FreeLibrary( m_library );
#  else
#    error unsupported platform
#  endif
      }
    }

    template <typename T>
    T getProcAddress( const char* function ) const VULKAN_HPP_NOEXCEPT
    {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined(__Fuchsia__)
      return (T)dlsym( m_library, function );
#  elif defined( _WIN32 )
      return (T)::GetProcAddress( m_library, function );
#  else
#    error unsupported platform
#  endif
    }

    bool success() const VULKAN_HPP_NOEXCEPT { return m_library != nullptr; }

  private:
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined(__Fuchsia__)
    void * m_library;
#  elif defined( _WIN32 )
    ::HINSTANCE m_library;
#  else
#    error unsupported platform
#  endif
  };
#endif
