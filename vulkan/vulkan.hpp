// Copyright (c) 2015-2017 The Khronos Group Inc.
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and/or associated documentation files (the
// "Materials"), to deal in the Materials without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Materials, and to
// permit persons to whom the Materials are furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Materials.
// 
// THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

// This header is generated from the Khronos Vulkan XML API Registry.


#ifndef VULKAN_HPP
#define VULKAN_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <vulkan/vulkan.h>
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
# include <memory>
# include <vector>
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

static_assert( VK_HEADER_VERSION ==  39 , "Wrong VK_HEADER_VERSION!" );

// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default.
// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
# if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )
#  define VULKAN_HPP_TYPESAFE_CONVERSION
# endif
#endif

#if !defined(VULKAN_HPP_HAS_UNRESTRICTED_UNIONS)
# if defined(__clang__)
#  if __has_feature(cxx_unrestricted_unions)
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  if 40600 <= GCC_VERSION
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(_MSC_VER)
#  if 1900 <= _MSC_VER
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# endif
#endif


#if !defined(VULKAN_HPP_INLINE)
# if defined(__clang___)
#  if __has_attribute(always_inline)
#   define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
#  else
#    define VULKAN_HPP_INLINE inline
#  endif
# elif defined(__GNUC__)
#  define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
# elif defined(_MSC_VER)
#  define VULKAN_HPP_INLINE __forceinline
# else
#  define VULKAN_HPP_INLINE inline
# endif
#endif

namespace vk
{
  template <typename FlagBitsType> struct FlagTraits
  {
    enum { allFlags = 0 };
  };

  template <typename BitType, typename MaskType = VkFlags>
  class Flags
  {
  public:
    Flags()
      : m_mask(0)
    {
    }

    Flags(BitType bit)
      : m_mask(static_cast<MaskType>(bit))
    {
    }

    Flags(Flags<BitType> const& rhs)
      : m_mask(rhs.m_mask)
    {
    }

    Flags<BitType> & operator=(Flags<BitType> const& rhs)
    {
      m_mask = rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator|=(Flags<BitType> const& rhs)
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator&=(Flags<BitType> const& rhs)
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator^=(Flags<BitType> const& rhs)
    {
      m_mask ^= rhs.m_mask;
      return *this;
    }

    Flags<BitType> operator|(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result |= rhs;
      return result;
    }

    Flags<BitType> operator&(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result &= rhs;
      return result;
    }

    Flags<BitType> operator^(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result ^= rhs;
      return result;
    }

    bool operator!() const
    {
      return !m_mask;
    }

    Flags<BitType> operator~() const
    {
      Flags<BitType> result(*this);
      result.m_mask ^= FlagTraits<BitType>::allFlags;
      return result;
    }

    bool operator==(Flags<BitType> const& rhs) const
    {
      return m_mask == rhs.m_mask;
    }

    bool operator!=(Flags<BitType> const& rhs) const
    {
      return m_mask != rhs.m_mask;
    }

    explicit operator bool() const
    {
      return !!m_mask;
    }

    explicit operator MaskType() const
    {
        return m_mask;
    }

  private:
    MaskType  m_mask;
  };
  
  template <typename BitType>
  Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
  {
    return flags | bit;
  }
  
  template <typename BitType>
  Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
  {
    return flags & bit;
  }
  
  template <typename BitType>
  Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
  {
    return flags ^ bit;
  }


  template <typename RefType>
  class Optional
  {
  public:
    Optional(RefType & reference) { m_ptr = &reference; }
    Optional(RefType * ptr) { m_ptr = ptr; }
    Optional(std::nullptr_t) { m_ptr = nullptr; }

    operator RefType*() const { return m_ptr; }
    RefType const* operator->() const { return m_ptr; }
    explicit operator bool() const { return !!m_ptr; }

  private:
    RefType *m_ptr;
  };

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename T>
  class ArrayProxy
  {
  public:
    ArrayProxy(std::nullptr_t)
      : m_count(0)
      , m_ptr(nullptr)
    {}

    ArrayProxy(T & ptr)
      : m_count(1)
      , m_ptr(&ptr)
    {}

    ArrayProxy(uint32_t count, T * ptr)
      : m_count(count)
      , m_ptr(ptr)
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    ArrayProxy(std::initializer_list<T> const& data)
      : m_count(static_cast<uint32_t>(data.end() - data.begin()))
      , m_ptr(data.begin())
    {}

    const T * begin() const
    {
      return m_ptr;
    }

    const T * end() const
    {
      return m_ptr + m_count;
    }

    const T & front() const
    {
      assert(m_count && m_ptr);
      return *m_ptr;
    }

    const T & back() const
    {
      assert(m_count && m_ptr);
      return *(m_ptr + m_count - 1);
    }

    bool empty() const
    {
      return (m_count == 0);
    }

    uint32_t size() const
    {
      return m_count;
    }

    T * data() const
    {
      return m_ptr;
    }

  private:
    uint32_t  m_count;
    T *       m_ptr;
  };
#endif


#if defined(VULKAN_HPP_NO_EXCEPTIONS) && !defined(VULKAN_HPP_NO_SMART_HANDLE)
#  define VULKAN_HPP_NO_SMART_HANDLE
#endif

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  class Device;
  class Instance;
  struct AllocationCallbacks;

  template <typename Type, typename Deleter>
  class UniqueHandle
  {
  public:
    explicit UniqueHandle( Type const& value = Type(), Deleter const& deleter = Deleter() )
      : m_value( value )
      , m_deleter( deleter )
    {}

    UniqueHandle( UniqueHandle const& ) = delete;

    UniqueHandle( UniqueHandle && other )
      : m_value( other.release() )
      , m_deleter( std::move( other.m_deleter ) )
    {}

    ~UniqueHandle()
    {
      destroy();
    }

    UniqueHandle & operator=( UniqueHandle const& ) = delete;

    UniqueHandle & operator=( UniqueHandle && other )
    {
      reset( other.release() );
      m_deleter = std::move( other.m_deleter );
      return *this;
    }

    explicit operator bool() const
    {
      return m_value.operator bool();
    }

    Type const* operator->() const
    {
      return &m_value;
    }

    Type const& operator*() const
    {
      return m_value;
    }

    Type get() const
    {
      return m_value;
    }

    Deleter & getDeleter()
    {
      return m_deleter;
    }

    Deleter const& getDeleter() const
    {
      return m_deleter;
    }

    void reset( Type const& value = Type() )
    {
      if ( m_value != value )
      {
        destroy();
        m_value = value;
      }
    }

    Type release()
    {
      Type value = m_value;
      m_value = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type, Deleter> & rhs )
    {
      std::swap(m_value, rhs.m_value);
      std::swap(m_deleter, rhs.m_deleter);
    }

  private:
    void destroy()
    {
      if ( m_value )
      {
        m_deleter( m_value );
      }
    }

  private:
    Type    m_value;
    Deleter m_deleter;
  };

  template <typename Type, typename Deleter>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type,Deleter> & lhs, UniqueHandle<Type,Deleter> & rhs )
  {
    lhs.swap( rhs );
  }
#endif

  enum class Result
  {
    eSuccess = VK_SUCCESS,
    eNotReady = VK_NOT_READY,
    eTimeout = VK_TIMEOUT,
    eEventSet = VK_EVENT_SET,
    eEventReset = VK_EVENT_RESET,
    eIncomplete = VK_INCOMPLETE,
    eErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
    eErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    eErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,
    eErrorDeviceLost = VK_ERROR_DEVICE_LOST,
    eErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,
    eErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
    eErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
    eErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
    eErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
    eErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
    eErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,
    eErrorFragmentedPool = VK_ERROR_FRAGMENTED_POOL,
    eErrorSurfaceLostKHR = VK_ERROR_SURFACE_LOST_KHR,
    eErrorNativeWindowInUseKHR = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    eSuboptimalKHR = VK_SUBOPTIMAL_KHR,
    eErrorOutOfDateKHR = VK_ERROR_OUT_OF_DATE_KHR,
    eErrorIncompatibleDisplayKHR = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    eErrorValidationFailedEXT = VK_ERROR_VALIDATION_FAILED_EXT,
    eErrorInvalidShaderNV = VK_ERROR_INVALID_SHADER_NV,
    eErrorOutOfPoolMemoryKHR = VK_ERROR_OUT_OF_POOL_MEMORY_KHR
  };

  VULKAN_HPP_INLINE std::string to_string(Result value)
  {
    switch (value)
    {
    case Result::eSuccess: return "Success";
    case Result::eNotReady: return "NotReady";
    case Result::eTimeout: return "Timeout";
    case Result::eEventSet: return "EventSet";
    case Result::eEventReset: return "EventReset";
    case Result::eIncomplete: return "Incomplete";
    case Result::eErrorOutOfHostMemory: return "ErrorOutOfHostMemory";
    case Result::eErrorOutOfDeviceMemory: return "ErrorOutOfDeviceMemory";
    case Result::eErrorInitializationFailed: return "ErrorInitializationFailed";
    case Result::eErrorDeviceLost: return "ErrorDeviceLost";
    case Result::eErrorMemoryMapFailed: return "ErrorMemoryMapFailed";
    case Result::eErrorLayerNotPresent: return "ErrorLayerNotPresent";
    case Result::eErrorExtensionNotPresent: return "ErrorExtensionNotPresent";
    case Result::eErrorFeatureNotPresent: return "ErrorFeatureNotPresent";
    case Result::eErrorIncompatibleDriver: return "ErrorIncompatibleDriver";
    case Result::eErrorTooManyObjects: return "ErrorTooManyObjects";
    case Result::eErrorFormatNotSupported: return "ErrorFormatNotSupported";
    case Result::eErrorFragmentedPool: return "ErrorFragmentedPool";
    case Result::eErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
    case Result::eErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
    case Result::eSuboptimalKHR: return "SuboptimalKHR";
    case Result::eErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
    case Result::eErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
    case Result::eErrorValidationFailedEXT: return "ErrorValidationFailedEXT";
    case Result::eErrorInvalidShaderNV: return "ErrorInvalidShaderNV";
    case Result::eErrorOutOfPoolMemoryKHR: return "ErrorOutOfPoolMemoryKHR";
    default: return "invalid";
    }
  }

#if defined(_MSC_VER) && (_MSC_VER == 1800)
# define noexcept _NOEXCEPT
#endif

  class ErrorCategoryImpl : public std::error_category
  {
    public:
    virtual const char* name() const noexcept override { return "vk::Result"; }
    virtual std::string message(int ev) const override { return to_string(static_cast<Result>(ev)); }
  };

#if defined(_MSC_VER) && (_MSC_VER == 1800)
# undef noexcept
#endif

  VULKAN_HPP_INLINE const std::error_category& errorCategory()
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code(Result e)
  {
    return std::error_code(static_cast<int>(e), errorCategory());
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e)
  {
    return std::error_condition(static_cast<int>(e), errorCategory());
  }

} // namespace vk

namespace std
{
  template <>
  struct is_error_code_enum<vk::Result> : public true_type
  {};
}

namespace vk
{
  template <typename T>
  struct ResultValue
  {
    ResultValue( Result r, T & v )
      : result( r )
      , value( v )
    {}

    Result  result;
    T       value;

    operator std::tuple<Result&, T&>() { return std::tuple<Result&, T&>(result, value); }
  };

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef ResultValue<T>  type;
#else
    typedef T              type;
#endif
  };

  template <>  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef Result type;
#else
    typedef void   type;
#endif
  };

  VULKAN_HPP_INLINE ResultValueType<void>::type createResultValue( Result result, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( result == Result::eSuccess );
    return result;
#else
    if ( result != Result::eSuccess )
    {
      throw std::system_error( result, message );
    }
#endif
  }

  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValue( Result result, T & data, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( result == Result::eSuccess );
    return ResultValue<T>( result, data );
#else
    if ( result != Result::eSuccess )
    {
      throw std::system_error( result, message );
    }
    return data;
#endif
  }

  VULKAN_HPP_INLINE Result createResultValue( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throw std::system_error( result, message );
    }
#endif
    return result;
  }

  template <typename T>
  VULKAN_HPP_INLINE ResultValue<T> createResultValue( Result result, T & data, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throw std::system_error( result, message );
    }
#endif
    return ResultValue<T>( result, data );
  }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  class DeviceDeleter
  {
  public:
    DeviceDeleter( Optional<const AllocationCallbacks> allocator = nullptr )
      : m_allocator( allocator )
    {}

    void operator()( Device device );

  private:
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueDevice = UniqueHandle<Device, DeviceDeleter>;

  class InstanceDeleter
  {
  public:
    InstanceDeleter( Optional<const AllocationCallbacks> allocator = nullptr )
      : m_allocator( allocator )
    {}

    void operator()( Instance instance );

  private:
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueInstance = UniqueHandle<Instance, InstanceDeleter>;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  using SampleMask = uint32_t;

  using Bool32 = uint32_t;

  using DeviceSize = uint64_t;

  enum class FramebufferCreateFlagBits
  {
  };

  using FramebufferCreateFlags = Flags<FramebufferCreateFlagBits, VkFramebufferCreateFlags>;

  VULKAN_HPP_INLINE FramebufferCreateFlags operator|( FramebufferCreateFlagBits bit0, FramebufferCreateFlagBits bit1 )
  {
    return FramebufferCreateFlags( bit0 ) | bit1;
  }

  enum class QueryPoolCreateFlagBits
  {
  };

  using QueryPoolCreateFlags = Flags<QueryPoolCreateFlagBits, VkQueryPoolCreateFlags>;

  VULKAN_HPP_INLINE QueryPoolCreateFlags operator|( QueryPoolCreateFlagBits bit0, QueryPoolCreateFlagBits bit1 )
  {
    return QueryPoolCreateFlags( bit0 ) | bit1;
  }

  enum class RenderPassCreateFlagBits
  {
  };

  using RenderPassCreateFlags = Flags<RenderPassCreateFlagBits, VkRenderPassCreateFlags>;

  VULKAN_HPP_INLINE RenderPassCreateFlags operator|( RenderPassCreateFlagBits bit0, RenderPassCreateFlagBits bit1 )
  {
    return RenderPassCreateFlags( bit0 ) | bit1;
  }

  enum class SamplerCreateFlagBits
  {
  };

  using SamplerCreateFlags = Flags<SamplerCreateFlagBits, VkSamplerCreateFlags>;

  VULKAN_HPP_INLINE SamplerCreateFlags operator|( SamplerCreateFlagBits bit0, SamplerCreateFlagBits bit1 )
  {
    return SamplerCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineLayoutCreateFlagBits
  {
  };

  using PipelineLayoutCreateFlags = Flags<PipelineLayoutCreateFlagBits, VkPipelineLayoutCreateFlags>;

  VULKAN_HPP_INLINE PipelineLayoutCreateFlags operator|( PipelineLayoutCreateFlagBits bit0, PipelineLayoutCreateFlagBits bit1 )
  {
    return PipelineLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineCacheCreateFlagBits
  {
  };

  using PipelineCacheCreateFlags = Flags<PipelineCacheCreateFlagBits, VkPipelineCacheCreateFlags>;

  VULKAN_HPP_INLINE PipelineCacheCreateFlags operator|( PipelineCacheCreateFlagBits bit0, PipelineCacheCreateFlagBits bit1 )
  {
    return PipelineCacheCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDepthStencilStateCreateFlagBits
  {
  };

  using PipelineDepthStencilStateCreateFlags = Flags<PipelineDepthStencilStateCreateFlagBits, VkPipelineDepthStencilStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineDepthStencilStateCreateFlags operator|( PipelineDepthStencilStateCreateFlagBits bit0, PipelineDepthStencilStateCreateFlagBits bit1 )
  {
    return PipelineDepthStencilStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDynamicStateCreateFlagBits
  {
  };

  using PipelineDynamicStateCreateFlags = Flags<PipelineDynamicStateCreateFlagBits, VkPipelineDynamicStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineDynamicStateCreateFlags operator|( PipelineDynamicStateCreateFlagBits bit0, PipelineDynamicStateCreateFlagBits bit1 )
  {
    return PipelineDynamicStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineColorBlendStateCreateFlagBits
  {
  };

  using PipelineColorBlendStateCreateFlags = Flags<PipelineColorBlendStateCreateFlagBits, VkPipelineColorBlendStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineColorBlendStateCreateFlags operator|( PipelineColorBlendStateCreateFlagBits bit0, PipelineColorBlendStateCreateFlagBits bit1 )
  {
    return PipelineColorBlendStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineMultisampleStateCreateFlagBits
  {
  };

  using PipelineMultisampleStateCreateFlags = Flags<PipelineMultisampleStateCreateFlagBits, VkPipelineMultisampleStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineMultisampleStateCreateFlags operator|( PipelineMultisampleStateCreateFlagBits bit0, PipelineMultisampleStateCreateFlagBits bit1 )
  {
    return PipelineMultisampleStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineRasterizationStateCreateFlagBits
  {
  };

  using PipelineRasterizationStateCreateFlags = Flags<PipelineRasterizationStateCreateFlagBits, VkPipelineRasterizationStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineRasterizationStateCreateFlags operator|( PipelineRasterizationStateCreateFlagBits bit0, PipelineRasterizationStateCreateFlagBits bit1 )
  {
    return PipelineRasterizationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineViewportStateCreateFlagBits
  {
  };

  using PipelineViewportStateCreateFlags = Flags<PipelineViewportStateCreateFlagBits, VkPipelineViewportStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineViewportStateCreateFlags operator|( PipelineViewportStateCreateFlagBits bit0, PipelineViewportStateCreateFlagBits bit1 )
  {
    return PipelineViewportStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineTessellationStateCreateFlagBits
  {
  };

  using PipelineTessellationStateCreateFlags = Flags<PipelineTessellationStateCreateFlagBits, VkPipelineTessellationStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineTessellationStateCreateFlags operator|( PipelineTessellationStateCreateFlagBits bit0, PipelineTessellationStateCreateFlagBits bit1 )
  {
    return PipelineTessellationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineInputAssemblyStateCreateFlagBits
  {
  };

  using PipelineInputAssemblyStateCreateFlags = Flags<PipelineInputAssemblyStateCreateFlagBits, VkPipelineInputAssemblyStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineInputAssemblyStateCreateFlags operator|( PipelineInputAssemblyStateCreateFlagBits bit0, PipelineInputAssemblyStateCreateFlagBits bit1 )
  {
    return PipelineInputAssemblyStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineVertexInputStateCreateFlagBits
  {
  };

  using PipelineVertexInputStateCreateFlags = Flags<PipelineVertexInputStateCreateFlagBits, VkPipelineVertexInputStateCreateFlags>;

  VULKAN_HPP_INLINE PipelineVertexInputStateCreateFlags operator|( PipelineVertexInputStateCreateFlagBits bit0, PipelineVertexInputStateCreateFlagBits bit1 )
  {
    return PipelineVertexInputStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineShaderStageCreateFlagBits
  {
  };

  using PipelineShaderStageCreateFlags = Flags<PipelineShaderStageCreateFlagBits, VkPipelineShaderStageCreateFlags>;

  VULKAN_HPP_INLINE PipelineShaderStageCreateFlags operator|( PipelineShaderStageCreateFlagBits bit0, PipelineShaderStageCreateFlagBits bit1 )
  {
    return PipelineShaderStageCreateFlags( bit0 ) | bit1;
  }

  enum class DescriptorSetLayoutCreateFlagBits
  {
  };

  using DescriptorSetLayoutCreateFlags = Flags<DescriptorSetLayoutCreateFlagBits, VkDescriptorSetLayoutCreateFlags>;

  VULKAN_HPP_INLINE DescriptorSetLayoutCreateFlags operator|( DescriptorSetLayoutCreateFlagBits bit0, DescriptorSetLayoutCreateFlagBits bit1 )
  {
    return DescriptorSetLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class BufferViewCreateFlagBits
  {
  };

  using BufferViewCreateFlags = Flags<BufferViewCreateFlagBits, VkBufferViewCreateFlags>;

  VULKAN_HPP_INLINE BufferViewCreateFlags operator|( BufferViewCreateFlagBits bit0, BufferViewCreateFlagBits bit1 )
  {
    return BufferViewCreateFlags( bit0 ) | bit1;
  }

  enum class InstanceCreateFlagBits
  {
  };

  using InstanceCreateFlags = Flags<InstanceCreateFlagBits, VkInstanceCreateFlags>;

  VULKAN_HPP_INLINE InstanceCreateFlags operator|( InstanceCreateFlagBits bit0, InstanceCreateFlagBits bit1 )
  {
    return InstanceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceCreateFlagBits
  {
  };

  using DeviceCreateFlags = Flags<DeviceCreateFlagBits, VkDeviceCreateFlags>;

  VULKAN_HPP_INLINE DeviceCreateFlags operator|( DeviceCreateFlagBits bit0, DeviceCreateFlagBits bit1 )
  {
    return DeviceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceQueueCreateFlagBits
  {
  };

  using DeviceQueueCreateFlags = Flags<DeviceQueueCreateFlagBits, VkDeviceQueueCreateFlags>;

  VULKAN_HPP_INLINE DeviceQueueCreateFlags operator|( DeviceQueueCreateFlagBits bit0, DeviceQueueCreateFlagBits bit1 )
  {
    return DeviceQueueCreateFlags( bit0 ) | bit1;
  }

  enum class ImageViewCreateFlagBits
  {
  };

  using ImageViewCreateFlags = Flags<ImageViewCreateFlagBits, VkImageViewCreateFlags>;

  VULKAN_HPP_INLINE ImageViewCreateFlags operator|( ImageViewCreateFlagBits bit0, ImageViewCreateFlagBits bit1 )
  {
    return ImageViewCreateFlags( bit0 ) | bit1;
  }

  enum class SemaphoreCreateFlagBits
  {
  };

  using SemaphoreCreateFlags = Flags<SemaphoreCreateFlagBits, VkSemaphoreCreateFlags>;

  VULKAN_HPP_INLINE SemaphoreCreateFlags operator|( SemaphoreCreateFlagBits bit0, SemaphoreCreateFlagBits bit1 )
  {
    return SemaphoreCreateFlags( bit0 ) | bit1;
  }

  enum class ShaderModuleCreateFlagBits
  {
  };

  using ShaderModuleCreateFlags = Flags<ShaderModuleCreateFlagBits, VkShaderModuleCreateFlags>;

  VULKAN_HPP_INLINE ShaderModuleCreateFlags operator|( ShaderModuleCreateFlagBits bit0, ShaderModuleCreateFlagBits bit1 )
  {
    return ShaderModuleCreateFlags( bit0 ) | bit1;
  }

  enum class EventCreateFlagBits
  {
  };

  using EventCreateFlags = Flags<EventCreateFlagBits, VkEventCreateFlags>;

  VULKAN_HPP_INLINE EventCreateFlags operator|( EventCreateFlagBits bit0, EventCreateFlagBits bit1 )
  {
    return EventCreateFlags( bit0 ) | bit1;
  }

  enum class MemoryMapFlagBits
  {
  };

  using MemoryMapFlags = Flags<MemoryMapFlagBits, VkMemoryMapFlags>;

  VULKAN_HPP_INLINE MemoryMapFlags operator|( MemoryMapFlagBits bit0, MemoryMapFlagBits bit1 )
  {
    return MemoryMapFlags( bit0 ) | bit1;
  }

  enum class SubpassDescriptionFlagBits
  {
  };

  using SubpassDescriptionFlags = Flags<SubpassDescriptionFlagBits, VkSubpassDescriptionFlags>;

  VULKAN_HPP_INLINE SubpassDescriptionFlags operator|( SubpassDescriptionFlagBits bit0, SubpassDescriptionFlagBits bit1 )
  {
    return SubpassDescriptionFlags( bit0 ) | bit1;
  }

  enum class DescriptorPoolResetFlagBits
  {
  };

  using DescriptorPoolResetFlags = Flags<DescriptorPoolResetFlagBits, VkDescriptorPoolResetFlags>;

  VULKAN_HPP_INLINE DescriptorPoolResetFlags operator|( DescriptorPoolResetFlagBits bit0, DescriptorPoolResetFlagBits bit1 )
  {
    return DescriptorPoolResetFlags( bit0 ) | bit1;
  }

  enum class SwapchainCreateFlagBitsKHR
  {
  };

  using SwapchainCreateFlagsKHR = Flags<SwapchainCreateFlagBitsKHR, VkSwapchainCreateFlagsKHR>;

  VULKAN_HPP_INLINE SwapchainCreateFlagsKHR operator|( SwapchainCreateFlagBitsKHR bit0, SwapchainCreateFlagBitsKHR bit1 )
  {
    return SwapchainCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplayModeCreateFlagBitsKHR
  {
  };

  using DisplayModeCreateFlagsKHR = Flags<DisplayModeCreateFlagBitsKHR, VkDisplayModeCreateFlagsKHR>;

  VULKAN_HPP_INLINE DisplayModeCreateFlagsKHR operator|( DisplayModeCreateFlagBitsKHR bit0, DisplayModeCreateFlagBitsKHR bit1 )
  {
    return DisplayModeCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplaySurfaceCreateFlagBitsKHR
  {
  };

  using DisplaySurfaceCreateFlagsKHR = Flags<DisplaySurfaceCreateFlagBitsKHR, VkDisplaySurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE DisplaySurfaceCreateFlagsKHR operator|( DisplaySurfaceCreateFlagBitsKHR bit0, DisplaySurfaceCreateFlagBitsKHR bit1 )
  {
    return DisplaySurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  enum class AndroidSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  using AndroidSurfaceCreateFlagsKHR = Flags<AndroidSurfaceCreateFlagBitsKHR, VkAndroidSurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE AndroidSurfaceCreateFlagsKHR operator|( AndroidSurfaceCreateFlagBitsKHR bit0, AndroidSurfaceCreateFlagBitsKHR bit1 )
  {
    return AndroidSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  enum class MirSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  using MirSurfaceCreateFlagsKHR = Flags<MirSurfaceCreateFlagBitsKHR, VkMirSurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE MirSurfaceCreateFlagsKHR operator|( MirSurfaceCreateFlagBitsKHR bit0, MirSurfaceCreateFlagBitsKHR bit1 )
  {
    return MirSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_VI_NN
  enum class ViSurfaceCreateFlagBitsNN
  {
  };
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_VI_NN
  using ViSurfaceCreateFlagsNN = Flags<ViSurfaceCreateFlagBitsNN, VkViSurfaceCreateFlagsNN>;

  VULKAN_HPP_INLINE ViSurfaceCreateFlagsNN operator|( ViSurfaceCreateFlagBitsNN bit0, ViSurfaceCreateFlagBitsNN bit1 )
  {
    return ViSurfaceCreateFlagsNN( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  enum class WaylandSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  using WaylandSurfaceCreateFlagsKHR = Flags<WaylandSurfaceCreateFlagBitsKHR, VkWaylandSurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE WaylandSurfaceCreateFlagsKHR operator|( WaylandSurfaceCreateFlagBitsKHR bit0, WaylandSurfaceCreateFlagBitsKHR bit1 )
  {
    return WaylandSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  enum class Win32SurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  using Win32SurfaceCreateFlagsKHR = Flags<Win32SurfaceCreateFlagBitsKHR, VkWin32SurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE Win32SurfaceCreateFlagsKHR operator|( Win32SurfaceCreateFlagBitsKHR bit0, Win32SurfaceCreateFlagBitsKHR bit1 )
  {
    return Win32SurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  enum class XlibSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  using XlibSurfaceCreateFlagsKHR = Flags<XlibSurfaceCreateFlagBitsKHR, VkXlibSurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE XlibSurfaceCreateFlagsKHR operator|( XlibSurfaceCreateFlagBitsKHR bit0, XlibSurfaceCreateFlagBitsKHR bit1 )
  {
    return XlibSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  enum class XcbSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  using XcbSurfaceCreateFlagsKHR = Flags<XcbSurfaceCreateFlagBitsKHR, VkXcbSurfaceCreateFlagsKHR>;

  VULKAN_HPP_INLINE XcbSurfaceCreateFlagsKHR operator|( XcbSurfaceCreateFlagBitsKHR bit0, XcbSurfaceCreateFlagBitsKHR bit1 )
  {
    return XcbSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  enum class CommandPoolTrimFlagBitsKHR
  {
  };

  using CommandPoolTrimFlagsKHR = Flags<CommandPoolTrimFlagBitsKHR, VkCommandPoolTrimFlagsKHR>;

  VULKAN_HPP_INLINE CommandPoolTrimFlagsKHR operator|( CommandPoolTrimFlagBitsKHR bit0, CommandPoolTrimFlagBitsKHR bit1 )
  {
    return CommandPoolTrimFlagsKHR( bit0 ) | bit1;
  }

  class DeviceMemory
  {
  public:
    DeviceMemory()
      : m_deviceMemory(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DeviceMemory(VkDeviceMemory deviceMemory)
       : m_deviceMemory(deviceMemory)
    {}

    DeviceMemory& operator=(VkDeviceMemory deviceMemory)
    {
      m_deviceMemory = deviceMemory;
      return *this;
    }
#endif

    bool operator==(DeviceMemory const &rhs) const
    {
      return m_deviceMemory == rhs.m_deviceMemory;
    }

    bool operator!=(DeviceMemory const &rhs) const
    {
      return m_deviceMemory != rhs.m_deviceMemory;
    }

    bool operator<(DeviceMemory const &rhs) const
    {
      return m_deviceMemory < rhs.m_deviceMemory;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDeviceMemory() const
    {
      return m_deviceMemory;
    }

    explicit operator bool() const
    {
      return m_deviceMemory != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_deviceMemory == VK_NULL_HANDLE;
    }

  private:
    VkDeviceMemory m_deviceMemory;
  };
  static_assert( sizeof( DeviceMemory ) == sizeof( VkDeviceMemory ), "handle and wrapper have different size!" );

  class CommandPool
  {
  public:
    CommandPool()
      : m_commandPool(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    CommandPool(VkCommandPool commandPool)
       : m_commandPool(commandPool)
    {}

    CommandPool& operator=(VkCommandPool commandPool)
    {
      m_commandPool = commandPool;
      return *this;
    }
#endif

    bool operator==(CommandPool const &rhs) const
    {
      return m_commandPool == rhs.m_commandPool;
    }

    bool operator!=(CommandPool const &rhs) const
    {
      return m_commandPool != rhs.m_commandPool;
    }

    bool operator<(CommandPool const &rhs) const
    {
      return m_commandPool < rhs.m_commandPool;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkCommandPool() const
    {
      return m_commandPool;
    }

    explicit operator bool() const
    {
      return m_commandPool != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_commandPool == VK_NULL_HANDLE;
    }

  private:
    VkCommandPool m_commandPool;
  };
  static_assert( sizeof( CommandPool ) == sizeof( VkCommandPool ), "handle and wrapper have different size!" );

  class Buffer
  {
  public:
    Buffer()
      : m_buffer(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Buffer(VkBuffer buffer)
       : m_buffer(buffer)
    {}

    Buffer& operator=(VkBuffer buffer)
    {
      m_buffer = buffer;
      return *this;
    }
#endif

    bool operator==(Buffer const &rhs) const
    {
      return m_buffer == rhs.m_buffer;
    }

    bool operator!=(Buffer const &rhs) const
    {
      return m_buffer != rhs.m_buffer;
    }

    bool operator<(Buffer const &rhs) const
    {
      return m_buffer < rhs.m_buffer;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkBuffer() const
    {
      return m_buffer;
    }

    explicit operator bool() const
    {
      return m_buffer != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_buffer == VK_NULL_HANDLE;
    }

  private:
    VkBuffer m_buffer;
  };
  static_assert( sizeof( Buffer ) == sizeof( VkBuffer ), "handle and wrapper have different size!" );

  class BufferView
  {
  public:
    BufferView()
      : m_bufferView(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    BufferView(VkBufferView bufferView)
       : m_bufferView(bufferView)
    {}

    BufferView& operator=(VkBufferView bufferView)
    {
      m_bufferView = bufferView;
      return *this;
    }
#endif

    bool operator==(BufferView const &rhs) const
    {
      return m_bufferView == rhs.m_bufferView;
    }

    bool operator!=(BufferView const &rhs) const
    {
      return m_bufferView != rhs.m_bufferView;
    }

    bool operator<(BufferView const &rhs) const
    {
      return m_bufferView < rhs.m_bufferView;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkBufferView() const
    {
      return m_bufferView;
    }

    explicit operator bool() const
    {
      return m_bufferView != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_bufferView == VK_NULL_HANDLE;
    }

  private:
    VkBufferView m_bufferView;
  };
  static_assert( sizeof( BufferView ) == sizeof( VkBufferView ), "handle and wrapper have different size!" );

  class Image
  {
  public:
    Image()
      : m_image(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Image(VkImage image)
       : m_image(image)
    {}

    Image& operator=(VkImage image)
    {
      m_image = image;
      return *this;
    }
#endif

    bool operator==(Image const &rhs) const
    {
      return m_image == rhs.m_image;
    }

    bool operator!=(Image const &rhs) const
    {
      return m_image != rhs.m_image;
    }

    bool operator<(Image const &rhs) const
    {
      return m_image < rhs.m_image;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkImage() const
    {
      return m_image;
    }

    explicit operator bool() const
    {
      return m_image != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_image == VK_NULL_HANDLE;
    }

  private:
    VkImage m_image;
  };
  static_assert( sizeof( Image ) == sizeof( VkImage ), "handle and wrapper have different size!" );

  class ImageView
  {
  public:
    ImageView()
      : m_imageView(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ImageView(VkImageView imageView)
       : m_imageView(imageView)
    {}

    ImageView& operator=(VkImageView imageView)
    {
      m_imageView = imageView;
      return *this;
    }
#endif

    bool operator==(ImageView const &rhs) const
    {
      return m_imageView == rhs.m_imageView;
    }

    bool operator!=(ImageView const &rhs) const
    {
      return m_imageView != rhs.m_imageView;
    }

    bool operator<(ImageView const &rhs) const
    {
      return m_imageView < rhs.m_imageView;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkImageView() const
    {
      return m_imageView;
    }

    explicit operator bool() const
    {
      return m_imageView != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_imageView == VK_NULL_HANDLE;
    }

  private:
    VkImageView m_imageView;
  };
  static_assert( sizeof( ImageView ) == sizeof( VkImageView ), "handle and wrapper have different size!" );

  class ShaderModule
  {
  public:
    ShaderModule()
      : m_shaderModule(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ShaderModule(VkShaderModule shaderModule)
       : m_shaderModule(shaderModule)
    {}

    ShaderModule& operator=(VkShaderModule shaderModule)
    {
      m_shaderModule = shaderModule;
      return *this;
    }
#endif

    bool operator==(ShaderModule const &rhs) const
    {
      return m_shaderModule == rhs.m_shaderModule;
    }

    bool operator!=(ShaderModule const &rhs) const
    {
      return m_shaderModule != rhs.m_shaderModule;
    }

    bool operator<(ShaderModule const &rhs) const
    {
      return m_shaderModule < rhs.m_shaderModule;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkShaderModule() const
    {
      return m_shaderModule;
    }

    explicit operator bool() const
    {
      return m_shaderModule != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_shaderModule == VK_NULL_HANDLE;
    }

  private:
    VkShaderModule m_shaderModule;
  };
  static_assert( sizeof( ShaderModule ) == sizeof( VkShaderModule ), "handle and wrapper have different size!" );

  class Pipeline
  {
  public:
    Pipeline()
      : m_pipeline(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Pipeline(VkPipeline pipeline)
       : m_pipeline(pipeline)
    {}

    Pipeline& operator=(VkPipeline pipeline)
    {
      m_pipeline = pipeline;
      return *this;
    }
#endif

    bool operator==(Pipeline const &rhs) const
    {
      return m_pipeline == rhs.m_pipeline;
    }

    bool operator!=(Pipeline const &rhs) const
    {
      return m_pipeline != rhs.m_pipeline;
    }

    bool operator<(Pipeline const &rhs) const
    {
      return m_pipeline < rhs.m_pipeline;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkPipeline() const
    {
      return m_pipeline;
    }

    explicit operator bool() const
    {
      return m_pipeline != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_pipeline == VK_NULL_HANDLE;
    }

  private:
    VkPipeline m_pipeline;
  };
  static_assert( sizeof( Pipeline ) == sizeof( VkPipeline ), "handle and wrapper have different size!" );

  class PipelineLayout
  {
  public:
    PipelineLayout()
      : m_pipelineLayout(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    PipelineLayout(VkPipelineLayout pipelineLayout)
       : m_pipelineLayout(pipelineLayout)
    {}

    PipelineLayout& operator=(VkPipelineLayout pipelineLayout)
    {
      m_pipelineLayout = pipelineLayout;
      return *this;
    }
#endif

    bool operator==(PipelineLayout const &rhs) const
    {
      return m_pipelineLayout == rhs.m_pipelineLayout;
    }

    bool operator!=(PipelineLayout const &rhs) const
    {
      return m_pipelineLayout != rhs.m_pipelineLayout;
    }

    bool operator<(PipelineLayout const &rhs) const
    {
      return m_pipelineLayout < rhs.m_pipelineLayout;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkPipelineLayout() const
    {
      return m_pipelineLayout;
    }

    explicit operator bool() const
    {
      return m_pipelineLayout != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_pipelineLayout == VK_NULL_HANDLE;
    }

  private:
    VkPipelineLayout m_pipelineLayout;
  };
  static_assert( sizeof( PipelineLayout ) == sizeof( VkPipelineLayout ), "handle and wrapper have different size!" );

  class Sampler
  {
  public:
    Sampler()
      : m_sampler(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Sampler(VkSampler sampler)
       : m_sampler(sampler)
    {}

    Sampler& operator=(VkSampler sampler)
    {
      m_sampler = sampler;
      return *this;
    }
#endif

    bool operator==(Sampler const &rhs) const
    {
      return m_sampler == rhs.m_sampler;
    }

    bool operator!=(Sampler const &rhs) const
    {
      return m_sampler != rhs.m_sampler;
    }

    bool operator<(Sampler const &rhs) const
    {
      return m_sampler < rhs.m_sampler;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkSampler() const
    {
      return m_sampler;
    }

    explicit operator bool() const
    {
      return m_sampler != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_sampler == VK_NULL_HANDLE;
    }

  private:
    VkSampler m_sampler;
  };
  static_assert( sizeof( Sampler ) == sizeof( VkSampler ), "handle and wrapper have different size!" );

  class DescriptorSet
  {
  public:
    DescriptorSet()
      : m_descriptorSet(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DescriptorSet(VkDescriptorSet descriptorSet)
       : m_descriptorSet(descriptorSet)
    {}

    DescriptorSet& operator=(VkDescriptorSet descriptorSet)
    {
      m_descriptorSet = descriptorSet;
      return *this;
    }
#endif

    bool operator==(DescriptorSet const &rhs) const
    {
      return m_descriptorSet == rhs.m_descriptorSet;
    }

    bool operator!=(DescriptorSet const &rhs) const
    {
      return m_descriptorSet != rhs.m_descriptorSet;
    }

    bool operator<(DescriptorSet const &rhs) const
    {
      return m_descriptorSet < rhs.m_descriptorSet;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDescriptorSet() const
    {
      return m_descriptorSet;
    }

    explicit operator bool() const
    {
      return m_descriptorSet != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_descriptorSet == VK_NULL_HANDLE;
    }

  private:
    VkDescriptorSet m_descriptorSet;
  };
  static_assert( sizeof( DescriptorSet ) == sizeof( VkDescriptorSet ), "handle and wrapper have different size!" );

  class DescriptorSetLayout
  {
  public:
    DescriptorSetLayout()
      : m_descriptorSetLayout(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
       : m_descriptorSetLayout(descriptorSetLayout)
    {}

    DescriptorSetLayout& operator=(VkDescriptorSetLayout descriptorSetLayout)
    {
      m_descriptorSetLayout = descriptorSetLayout;
      return *this;
    }
#endif

    bool operator==(DescriptorSetLayout const &rhs) const
    {
      return m_descriptorSetLayout == rhs.m_descriptorSetLayout;
    }

    bool operator!=(DescriptorSetLayout const &rhs) const
    {
      return m_descriptorSetLayout != rhs.m_descriptorSetLayout;
    }

    bool operator<(DescriptorSetLayout const &rhs) const
    {
      return m_descriptorSetLayout < rhs.m_descriptorSetLayout;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDescriptorSetLayout() const
    {
      return m_descriptorSetLayout;
    }

    explicit operator bool() const
    {
      return m_descriptorSetLayout != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_descriptorSetLayout == VK_NULL_HANDLE;
    }

  private:
    VkDescriptorSetLayout m_descriptorSetLayout;
  };
  static_assert( sizeof( DescriptorSetLayout ) == sizeof( VkDescriptorSetLayout ), "handle and wrapper have different size!" );

  class DescriptorPool
  {
  public:
    DescriptorPool()
      : m_descriptorPool(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DescriptorPool(VkDescriptorPool descriptorPool)
       : m_descriptorPool(descriptorPool)
    {}

    DescriptorPool& operator=(VkDescriptorPool descriptorPool)
    {
      m_descriptorPool = descriptorPool;
      return *this;
    }
#endif

    bool operator==(DescriptorPool const &rhs) const
    {
      return m_descriptorPool == rhs.m_descriptorPool;
    }

    bool operator!=(DescriptorPool const &rhs) const
    {
      return m_descriptorPool != rhs.m_descriptorPool;
    }

    bool operator<(DescriptorPool const &rhs) const
    {
      return m_descriptorPool < rhs.m_descriptorPool;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDescriptorPool() const
    {
      return m_descriptorPool;
    }

    explicit operator bool() const
    {
      return m_descriptorPool != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_descriptorPool == VK_NULL_HANDLE;
    }

  private:
    VkDescriptorPool m_descriptorPool;
  };
  static_assert( sizeof( DescriptorPool ) == sizeof( VkDescriptorPool ), "handle and wrapper have different size!" );

  class Fence
  {
  public:
    Fence()
      : m_fence(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Fence(VkFence fence)
       : m_fence(fence)
    {}

    Fence& operator=(VkFence fence)
    {
      m_fence = fence;
      return *this;
    }
#endif

    bool operator==(Fence const &rhs) const
    {
      return m_fence == rhs.m_fence;
    }

    bool operator!=(Fence const &rhs) const
    {
      return m_fence != rhs.m_fence;
    }

    bool operator<(Fence const &rhs) const
    {
      return m_fence < rhs.m_fence;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkFence() const
    {
      return m_fence;
    }

    explicit operator bool() const
    {
      return m_fence != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_fence == VK_NULL_HANDLE;
    }

  private:
    VkFence m_fence;
  };
  static_assert( sizeof( Fence ) == sizeof( VkFence ), "handle and wrapper have different size!" );

  class Semaphore
  {
  public:
    Semaphore()
      : m_semaphore(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Semaphore(VkSemaphore semaphore)
       : m_semaphore(semaphore)
    {}

    Semaphore& operator=(VkSemaphore semaphore)
    {
      m_semaphore = semaphore;
      return *this;
    }
#endif

    bool operator==(Semaphore const &rhs) const
    {
      return m_semaphore == rhs.m_semaphore;
    }

    bool operator!=(Semaphore const &rhs) const
    {
      return m_semaphore != rhs.m_semaphore;
    }

    bool operator<(Semaphore const &rhs) const
    {
      return m_semaphore < rhs.m_semaphore;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkSemaphore() const
    {
      return m_semaphore;
    }

    explicit operator bool() const
    {
      return m_semaphore != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_semaphore == VK_NULL_HANDLE;
    }

  private:
    VkSemaphore m_semaphore;
  };
  static_assert( sizeof( Semaphore ) == sizeof( VkSemaphore ), "handle and wrapper have different size!" );

  class Event
  {
  public:
    Event()
      : m_event(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Event(VkEvent event)
       : m_event(event)
    {}

    Event& operator=(VkEvent event)
    {
      m_event = event;
      return *this;
    }
#endif

    bool operator==(Event const &rhs) const
    {
      return m_event == rhs.m_event;
    }

    bool operator!=(Event const &rhs) const
    {
      return m_event != rhs.m_event;
    }

    bool operator<(Event const &rhs) const
    {
      return m_event < rhs.m_event;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkEvent() const
    {
      return m_event;
    }

    explicit operator bool() const
    {
      return m_event != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_event == VK_NULL_HANDLE;
    }

  private:
    VkEvent m_event;
  };
  static_assert( sizeof( Event ) == sizeof( VkEvent ), "handle and wrapper have different size!" );

  class QueryPool
  {
  public:
    QueryPool()
      : m_queryPool(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    QueryPool(VkQueryPool queryPool)
       : m_queryPool(queryPool)
    {}

    QueryPool& operator=(VkQueryPool queryPool)
    {
      m_queryPool = queryPool;
      return *this;
    }
#endif

    bool operator==(QueryPool const &rhs) const
    {
      return m_queryPool == rhs.m_queryPool;
    }

    bool operator!=(QueryPool const &rhs) const
    {
      return m_queryPool != rhs.m_queryPool;
    }

    bool operator<(QueryPool const &rhs) const
    {
      return m_queryPool < rhs.m_queryPool;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkQueryPool() const
    {
      return m_queryPool;
    }

    explicit operator bool() const
    {
      return m_queryPool != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_queryPool == VK_NULL_HANDLE;
    }

  private:
    VkQueryPool m_queryPool;
  };
  static_assert( sizeof( QueryPool ) == sizeof( VkQueryPool ), "handle and wrapper have different size!" );

  class Framebuffer
  {
  public:
    Framebuffer()
      : m_framebuffer(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Framebuffer(VkFramebuffer framebuffer)
       : m_framebuffer(framebuffer)
    {}

    Framebuffer& operator=(VkFramebuffer framebuffer)
    {
      m_framebuffer = framebuffer;
      return *this;
    }
#endif

    bool operator==(Framebuffer const &rhs) const
    {
      return m_framebuffer == rhs.m_framebuffer;
    }

    bool operator!=(Framebuffer const &rhs) const
    {
      return m_framebuffer != rhs.m_framebuffer;
    }

    bool operator<(Framebuffer const &rhs) const
    {
      return m_framebuffer < rhs.m_framebuffer;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkFramebuffer() const
    {
      return m_framebuffer;
    }

    explicit operator bool() const
    {
      return m_framebuffer != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_framebuffer == VK_NULL_HANDLE;
    }

  private:
    VkFramebuffer m_framebuffer;
  };
  static_assert( sizeof( Framebuffer ) == sizeof( VkFramebuffer ), "handle and wrapper have different size!" );

  class RenderPass
  {
  public:
    RenderPass()
      : m_renderPass(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    RenderPass(VkRenderPass renderPass)
       : m_renderPass(renderPass)
    {}

    RenderPass& operator=(VkRenderPass renderPass)
    {
      m_renderPass = renderPass;
      return *this;
    }
#endif

    bool operator==(RenderPass const &rhs) const
    {
      return m_renderPass == rhs.m_renderPass;
    }

    bool operator!=(RenderPass const &rhs) const
    {
      return m_renderPass != rhs.m_renderPass;
    }

    bool operator<(RenderPass const &rhs) const
    {
      return m_renderPass < rhs.m_renderPass;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkRenderPass() const
    {
      return m_renderPass;
    }

    explicit operator bool() const
    {
      return m_renderPass != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_renderPass == VK_NULL_HANDLE;
    }

  private:
    VkRenderPass m_renderPass;
  };
  static_assert( sizeof( RenderPass ) == sizeof( VkRenderPass ), "handle and wrapper have different size!" );

  class PipelineCache
  {
  public:
    PipelineCache()
      : m_pipelineCache(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    PipelineCache(VkPipelineCache pipelineCache)
       : m_pipelineCache(pipelineCache)
    {}

    PipelineCache& operator=(VkPipelineCache pipelineCache)
    {
      m_pipelineCache = pipelineCache;
      return *this;
    }
#endif

    bool operator==(PipelineCache const &rhs) const
    {
      return m_pipelineCache == rhs.m_pipelineCache;
    }

    bool operator!=(PipelineCache const &rhs) const
    {
      return m_pipelineCache != rhs.m_pipelineCache;
    }

    bool operator<(PipelineCache const &rhs) const
    {
      return m_pipelineCache < rhs.m_pipelineCache;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkPipelineCache() const
    {
      return m_pipelineCache;
    }

    explicit operator bool() const
    {
      return m_pipelineCache != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_pipelineCache == VK_NULL_HANDLE;
    }

  private:
    VkPipelineCache m_pipelineCache;
  };
  static_assert( sizeof( PipelineCache ) == sizeof( VkPipelineCache ), "handle and wrapper have different size!" );

  class ObjectTableNVX
  {
  public:
    ObjectTableNVX()
      : m_objectTableNVX(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ObjectTableNVX(VkObjectTableNVX objectTableNVX)
       : m_objectTableNVX(objectTableNVX)
    {}

    ObjectTableNVX& operator=(VkObjectTableNVX objectTableNVX)
    {
      m_objectTableNVX = objectTableNVX;
      return *this;
    }
#endif

    bool operator==(ObjectTableNVX const &rhs) const
    {
      return m_objectTableNVX == rhs.m_objectTableNVX;
    }

    bool operator!=(ObjectTableNVX const &rhs) const
    {
      return m_objectTableNVX != rhs.m_objectTableNVX;
    }

    bool operator<(ObjectTableNVX const &rhs) const
    {
      return m_objectTableNVX < rhs.m_objectTableNVX;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkObjectTableNVX() const
    {
      return m_objectTableNVX;
    }

    explicit operator bool() const
    {
      return m_objectTableNVX != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_objectTableNVX == VK_NULL_HANDLE;
    }

  private:
    VkObjectTableNVX m_objectTableNVX;
  };
  static_assert( sizeof( ObjectTableNVX ) == sizeof( VkObjectTableNVX ), "handle and wrapper have different size!" );

  class IndirectCommandsLayoutNVX
  {
  public:
    IndirectCommandsLayoutNVX()
      : m_indirectCommandsLayoutNVX(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    IndirectCommandsLayoutNVX(VkIndirectCommandsLayoutNVX indirectCommandsLayoutNVX)
       : m_indirectCommandsLayoutNVX(indirectCommandsLayoutNVX)
    {}

    IndirectCommandsLayoutNVX& operator=(VkIndirectCommandsLayoutNVX indirectCommandsLayoutNVX)
    {
      m_indirectCommandsLayoutNVX = indirectCommandsLayoutNVX;
      return *this;
    }
#endif

    bool operator==(IndirectCommandsLayoutNVX const &rhs) const
    {
      return m_indirectCommandsLayoutNVX == rhs.m_indirectCommandsLayoutNVX;
    }

    bool operator!=(IndirectCommandsLayoutNVX const &rhs) const
    {
      return m_indirectCommandsLayoutNVX != rhs.m_indirectCommandsLayoutNVX;
    }

    bool operator<(IndirectCommandsLayoutNVX const &rhs) const
    {
      return m_indirectCommandsLayoutNVX < rhs.m_indirectCommandsLayoutNVX;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkIndirectCommandsLayoutNVX() const
    {
      return m_indirectCommandsLayoutNVX;
    }

    explicit operator bool() const
    {
      return m_indirectCommandsLayoutNVX != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_indirectCommandsLayoutNVX == VK_NULL_HANDLE;
    }

  private:
    VkIndirectCommandsLayoutNVX m_indirectCommandsLayoutNVX;
  };
  static_assert( sizeof( IndirectCommandsLayoutNVX ) == sizeof( VkIndirectCommandsLayoutNVX ), "handle and wrapper have different size!" );

  class DisplayKHR
  {
  public:
    DisplayKHR()
      : m_displayKHR(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DisplayKHR(VkDisplayKHR displayKHR)
       : m_displayKHR(displayKHR)
    {}

    DisplayKHR& operator=(VkDisplayKHR displayKHR)
    {
      m_displayKHR = displayKHR;
      return *this;
    }
#endif

    bool operator==(DisplayKHR const &rhs) const
    {
      return m_displayKHR == rhs.m_displayKHR;
    }

    bool operator!=(DisplayKHR const &rhs) const
    {
      return m_displayKHR != rhs.m_displayKHR;
    }

    bool operator<(DisplayKHR const &rhs) const
    {
      return m_displayKHR < rhs.m_displayKHR;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDisplayKHR() const
    {
      return m_displayKHR;
    }

    explicit operator bool() const
    {
      return m_displayKHR != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_displayKHR == VK_NULL_HANDLE;
    }

  private:
    VkDisplayKHR m_displayKHR;
  };
  static_assert( sizeof( DisplayKHR ) == sizeof( VkDisplayKHR ), "handle and wrapper have different size!" );

  class DisplayModeKHR
  {
  public:
    DisplayModeKHR()
      : m_displayModeKHR(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DisplayModeKHR(VkDisplayModeKHR displayModeKHR)
       : m_displayModeKHR(displayModeKHR)
    {}

    DisplayModeKHR& operator=(VkDisplayModeKHR displayModeKHR)
    {
      m_displayModeKHR = displayModeKHR;
      return *this;
    }
#endif

    bool operator==(DisplayModeKHR const &rhs) const
    {
      return m_displayModeKHR == rhs.m_displayModeKHR;
    }

    bool operator!=(DisplayModeKHR const &rhs) const
    {
      return m_displayModeKHR != rhs.m_displayModeKHR;
    }

    bool operator<(DisplayModeKHR const &rhs) const
    {
      return m_displayModeKHR < rhs.m_displayModeKHR;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDisplayModeKHR() const
    {
      return m_displayModeKHR;
    }

    explicit operator bool() const
    {
      return m_displayModeKHR != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_displayModeKHR == VK_NULL_HANDLE;
    }

  private:
    VkDisplayModeKHR m_displayModeKHR;
  };
  static_assert( sizeof( DisplayModeKHR ) == sizeof( VkDisplayModeKHR ), "handle and wrapper have different size!" );

  class SurfaceKHR
  {
  public:
    SurfaceKHR()
      : m_surfaceKHR(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    SurfaceKHR(VkSurfaceKHR surfaceKHR)
       : m_surfaceKHR(surfaceKHR)
    {}

    SurfaceKHR& operator=(VkSurfaceKHR surfaceKHR)
    {
      m_surfaceKHR = surfaceKHR;
      return *this;
    }
#endif

    bool operator==(SurfaceKHR const &rhs) const
    {
      return m_surfaceKHR == rhs.m_surfaceKHR;
    }

    bool operator!=(SurfaceKHR const &rhs) const
    {
      return m_surfaceKHR != rhs.m_surfaceKHR;
    }

    bool operator<(SurfaceKHR const &rhs) const
    {
      return m_surfaceKHR < rhs.m_surfaceKHR;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkSurfaceKHR() const
    {
      return m_surfaceKHR;
    }

    explicit operator bool() const
    {
      return m_surfaceKHR != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_surfaceKHR == VK_NULL_HANDLE;
    }

  private:
    VkSurfaceKHR m_surfaceKHR;
  };
  static_assert( sizeof( SurfaceKHR ) == sizeof( VkSurfaceKHR ), "handle and wrapper have different size!" );

  class SwapchainKHR
  {
  public:
    SwapchainKHR()
      : m_swapchainKHR(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    SwapchainKHR(VkSwapchainKHR swapchainKHR)
       : m_swapchainKHR(swapchainKHR)
    {}

    SwapchainKHR& operator=(VkSwapchainKHR swapchainKHR)
    {
      m_swapchainKHR = swapchainKHR;
      return *this;
    }
#endif

    bool operator==(SwapchainKHR const &rhs) const
    {
      return m_swapchainKHR == rhs.m_swapchainKHR;
    }

    bool operator!=(SwapchainKHR const &rhs) const
    {
      return m_swapchainKHR != rhs.m_swapchainKHR;
    }

    bool operator<(SwapchainKHR const &rhs) const
    {
      return m_swapchainKHR < rhs.m_swapchainKHR;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkSwapchainKHR() const
    {
      return m_swapchainKHR;
    }

    explicit operator bool() const
    {
      return m_swapchainKHR != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_swapchainKHR == VK_NULL_HANDLE;
    }

  private:
    VkSwapchainKHR m_swapchainKHR;
  };
  static_assert( sizeof( SwapchainKHR ) == sizeof( VkSwapchainKHR ), "handle and wrapper have different size!" );

  class DebugReportCallbackEXT
  {
  public:
    DebugReportCallbackEXT()
      : m_debugReportCallbackEXT(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    DebugReportCallbackEXT(VkDebugReportCallbackEXT debugReportCallbackEXT)
       : m_debugReportCallbackEXT(debugReportCallbackEXT)
    {}

    DebugReportCallbackEXT& operator=(VkDebugReportCallbackEXT debugReportCallbackEXT)
    {
      m_debugReportCallbackEXT = debugReportCallbackEXT;
      return *this;
    }
#endif

    bool operator==(DebugReportCallbackEXT const &rhs) const
    {
      return m_debugReportCallbackEXT == rhs.m_debugReportCallbackEXT;
    }

    bool operator!=(DebugReportCallbackEXT const &rhs) const
    {
      return m_debugReportCallbackEXT != rhs.m_debugReportCallbackEXT;
    }

    bool operator<(DebugReportCallbackEXT const &rhs) const
    {
      return m_debugReportCallbackEXT < rhs.m_debugReportCallbackEXT;
    }

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDebugReportCallbackEXT() const
    {
      return m_debugReportCallbackEXT;
    }

    explicit operator bool() const
    {
      return m_debugReportCallbackEXT != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_debugReportCallbackEXT == VK_NULL_HANDLE;
    }

  private:
    VkDebugReportCallbackEXT m_debugReportCallbackEXT;
  };
  static_assert( sizeof( DebugReportCallbackEXT ) == sizeof( VkDebugReportCallbackEXT ), "handle and wrapper have different size!" );

  struct Offset2D
  {
    Offset2D( int32_t x_ = 0, int32_t y_ = 0 )
      : x( x_ )
      , y( y_ )
    {
    }

    Offset2D( VkOffset2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Offset2D) );
    }

    Offset2D& operator=( VkOffset2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Offset2D) );
      return *this;
    }

    Offset2D& setX( int32_t x_ )
    {
      x = x_;
      return *this;
    }

    Offset2D& setY( int32_t y_ )
    {
      y = y_;
      return *this;
    }

    operator const VkOffset2D&() const
    {
      return *reinterpret_cast<const VkOffset2D*>(this);
    }

    bool operator==( Offset2D const& rhs ) const
    {
      return ( x == rhs.x )
          && ( y == rhs.y );
    }

    bool operator!=( Offset2D const& rhs ) const
    {
      return !operator==( rhs );
    }

    int32_t x;
    int32_t y;
  };
  static_assert( sizeof( Offset2D ) == sizeof( VkOffset2D ), "struct and wrapper have different size!" );

  struct Offset3D
  {
    Offset3D( int32_t x_ = 0, int32_t y_ = 0, int32_t z_ = 0 )
      : x( x_ )
      , y( y_ )
      , z( z_ )
    {
    }

    Offset3D( VkOffset3D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Offset3D) );
    }

    Offset3D& operator=( VkOffset3D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Offset3D) );
      return *this;
    }

    Offset3D& setX( int32_t x_ )
    {
      x = x_;
      return *this;
    }

    Offset3D& setY( int32_t y_ )
    {
      y = y_;
      return *this;
    }

    Offset3D& setZ( int32_t z_ )
    {
      z = z_;
      return *this;
    }

    operator const VkOffset3D&() const
    {
      return *reinterpret_cast<const VkOffset3D*>(this);
    }

    bool operator==( Offset3D const& rhs ) const
    {
      return ( x == rhs.x )
          && ( y == rhs.y )
          && ( z == rhs.z );
    }

    bool operator!=( Offset3D const& rhs ) const
    {
      return !operator==( rhs );
    }

    int32_t x;
    int32_t y;
    int32_t z;
  };
  static_assert( sizeof( Offset3D ) == sizeof( VkOffset3D ), "struct and wrapper have different size!" );

  struct Extent2D
  {
    Extent2D( uint32_t width_ = 0, uint32_t height_ = 0 )
      : width( width_ )
      , height( height_ )
    {
    }

    Extent2D( VkExtent2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Extent2D) );
    }

    Extent2D& operator=( VkExtent2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Extent2D) );
      return *this;
    }

    Extent2D& setWidth( uint32_t width_ )
    {
      width = width_;
      return *this;
    }

    Extent2D& setHeight( uint32_t height_ )
    {
      height = height_;
      return *this;
    }

    operator const VkExtent2D&() const
    {
      return *reinterpret_cast<const VkExtent2D*>(this);
    }

    bool operator==( Extent2D const& rhs ) const
    {
      return ( width == rhs.width )
          && ( height == rhs.height );
    }

    bool operator!=( Extent2D const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t width;
    uint32_t height;
  };
  static_assert( sizeof( Extent2D ) == sizeof( VkExtent2D ), "struct and wrapper have different size!" );

  struct Extent3D
  {
    Extent3D( uint32_t width_ = 0, uint32_t height_ = 0, uint32_t depth_ = 0 )
      : width( width_ )
      , height( height_ )
      , depth( depth_ )
    {
    }

    Extent3D( VkExtent3D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Extent3D) );
    }

    Extent3D& operator=( VkExtent3D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Extent3D) );
      return *this;
    }

    Extent3D& setWidth( uint32_t width_ )
    {
      width = width_;
      return *this;
    }

    Extent3D& setHeight( uint32_t height_ )
    {
      height = height_;
      return *this;
    }

    Extent3D& setDepth( uint32_t depth_ )
    {
      depth = depth_;
      return *this;
    }

    operator const VkExtent3D&() const
    {
      return *reinterpret_cast<const VkExtent3D*>(this);
    }

    bool operator==( Extent3D const& rhs ) const
    {
      return ( width == rhs.width )
          && ( height == rhs.height )
          && ( depth == rhs.depth );
    }

    bool operator!=( Extent3D const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t width;
    uint32_t height;
    uint32_t depth;
  };
  static_assert( sizeof( Extent3D ) == sizeof( VkExtent3D ), "struct and wrapper have different size!" );

  struct Viewport
  {
    Viewport( float x_ = 0, float y_ = 0, float width_ = 0, float height_ = 0, float minDepth_ = 0, float maxDepth_ = 0 )
      : x( x_ )
      , y( y_ )
      , width( width_ )
      , height( height_ )
      , minDepth( minDepth_ )
      , maxDepth( maxDepth_ )
    {
    }

    Viewport( VkViewport const & rhs )
    {
      memcpy( this, &rhs, sizeof(Viewport) );
    }

    Viewport& operator=( VkViewport const & rhs )
    {
      memcpy( this, &rhs, sizeof(Viewport) );
      return *this;
    }

    Viewport& setX( float x_ )
    {
      x = x_;
      return *this;
    }

    Viewport& setY( float y_ )
    {
      y = y_;
      return *this;
    }

    Viewport& setWidth( float width_ )
    {
      width = width_;
      return *this;
    }

    Viewport& setHeight( float height_ )
    {
      height = height_;
      return *this;
    }

    Viewport& setMinDepth( float minDepth_ )
    {
      minDepth = minDepth_;
      return *this;
    }

    Viewport& setMaxDepth( float maxDepth_ )
    {
      maxDepth = maxDepth_;
      return *this;
    }

    operator const VkViewport&() const
    {
      return *reinterpret_cast<const VkViewport*>(this);
    }

    bool operator==( Viewport const& rhs ) const
    {
      return ( x == rhs.x )
          && ( y == rhs.y )
          && ( width == rhs.width )
          && ( height == rhs.height )
          && ( minDepth == rhs.minDepth )
          && ( maxDepth == rhs.maxDepth );
    }

    bool operator!=( Viewport const& rhs ) const
    {
      return !operator==( rhs );
    }

    float x;
    float y;
    float width;
    float height;
    float minDepth;
    float maxDepth;
  };
  static_assert( sizeof( Viewport ) == sizeof( VkViewport ), "struct and wrapper have different size!" );

  struct Rect2D
  {
    Rect2D( Offset2D offset_ = Offset2D(), Extent2D extent_ = Extent2D() )
      : offset( offset_ )
      , extent( extent_ )
    {
    }

    Rect2D( VkRect2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Rect2D) );
    }

    Rect2D& operator=( VkRect2D const & rhs )
    {
      memcpy( this, &rhs, sizeof(Rect2D) );
      return *this;
    }

    Rect2D& setOffset( Offset2D offset_ )
    {
      offset = offset_;
      return *this;
    }

    Rect2D& setExtent( Extent2D extent_ )
    {
      extent = extent_;
      return *this;
    }

    operator const VkRect2D&() const
    {
      return *reinterpret_cast<const VkRect2D*>(this);
    }

    bool operator==( Rect2D const& rhs ) const
    {
      return ( offset == rhs.offset )
          && ( extent == rhs.extent );
    }

    bool operator!=( Rect2D const& rhs ) const
    {
      return !operator==( rhs );
    }

    Offset2D offset;
    Extent2D extent;
  };
  static_assert( sizeof( Rect2D ) == sizeof( VkRect2D ), "struct and wrapper have different size!" );

  struct ClearRect
  {
    ClearRect( Rect2D rect_ = Rect2D(), uint32_t baseArrayLayer_ = 0, uint32_t layerCount_ = 0 )
      : rect( rect_ )
      , baseArrayLayer( baseArrayLayer_ )
      , layerCount( layerCount_ )
    {
    }

    ClearRect( VkClearRect const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearRect) );
    }

    ClearRect& operator=( VkClearRect const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearRect) );
      return *this;
    }

    ClearRect& setRect( Rect2D rect_ )
    {
      rect = rect_;
      return *this;
    }

    ClearRect& setBaseArrayLayer( uint32_t baseArrayLayer_ )
    {
      baseArrayLayer = baseArrayLayer_;
      return *this;
    }

    ClearRect& setLayerCount( uint32_t layerCount_ )
    {
      layerCount = layerCount_;
      return *this;
    }

    operator const VkClearRect&() const
    {
      return *reinterpret_cast<const VkClearRect*>(this);
    }

    bool operator==( ClearRect const& rhs ) const
    {
      return ( rect == rhs.rect )
          && ( baseArrayLayer == rhs.baseArrayLayer )
          && ( layerCount == rhs.layerCount );
    }

    bool operator!=( ClearRect const& rhs ) const
    {
      return !operator==( rhs );
    }

    Rect2D rect;
    uint32_t baseArrayLayer;
    uint32_t layerCount;
  };
  static_assert( sizeof( ClearRect ) == sizeof( VkClearRect ), "struct and wrapper have different size!" );

  struct ExtensionProperties
  {
    operator const VkExtensionProperties&() const
    {
      return *reinterpret_cast<const VkExtensionProperties*>(this);
    }

    bool operator==( ExtensionProperties const& rhs ) const
    {
      return ( memcmp( extensionName, rhs.extensionName, VK_MAX_EXTENSION_NAME_SIZE * sizeof( char ) ) == 0 )
          && ( specVersion == rhs.specVersion );
    }

    bool operator!=( ExtensionProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    char extensionName[VK_MAX_EXTENSION_NAME_SIZE];
    uint32_t specVersion;
  };
  static_assert( sizeof( ExtensionProperties ) == sizeof( VkExtensionProperties ), "struct and wrapper have different size!" );

  struct LayerProperties
  {
    operator const VkLayerProperties&() const
    {
      return *reinterpret_cast<const VkLayerProperties*>(this);
    }

    bool operator==( LayerProperties const& rhs ) const
    {
      return ( memcmp( layerName, rhs.layerName, VK_MAX_EXTENSION_NAME_SIZE * sizeof( char ) ) == 0 )
          && ( specVersion == rhs.specVersion )
          && ( implementationVersion == rhs.implementationVersion )
          && ( memcmp( description, rhs.description, VK_MAX_DESCRIPTION_SIZE * sizeof( char ) ) == 0 );
    }

    bool operator!=( LayerProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    char layerName[VK_MAX_EXTENSION_NAME_SIZE];
    uint32_t specVersion;
    uint32_t implementationVersion;
    char description[VK_MAX_DESCRIPTION_SIZE];
  };
  static_assert( sizeof( LayerProperties ) == sizeof( VkLayerProperties ), "struct and wrapper have different size!" );

  struct AllocationCallbacks
  {
    AllocationCallbacks( void* pUserData_ = nullptr, PFN_vkAllocationFunction pfnAllocation_ = nullptr, PFN_vkReallocationFunction pfnReallocation_ = nullptr, PFN_vkFreeFunction pfnFree_ = nullptr, PFN_vkInternalAllocationNotification pfnInternalAllocation_ = nullptr, PFN_vkInternalFreeNotification pfnInternalFree_ = nullptr )
      : pUserData( pUserData_ )
      , pfnAllocation( pfnAllocation_ )
      , pfnReallocation( pfnReallocation_ )
      , pfnFree( pfnFree_ )
      , pfnInternalAllocation( pfnInternalAllocation_ )
      , pfnInternalFree( pfnInternalFree_ )
    {
    }

    AllocationCallbacks( VkAllocationCallbacks const & rhs )
    {
      memcpy( this, &rhs, sizeof(AllocationCallbacks) );
    }

    AllocationCallbacks& operator=( VkAllocationCallbacks const & rhs )
    {
      memcpy( this, &rhs, sizeof(AllocationCallbacks) );
      return *this;
    }

    AllocationCallbacks& setPUserData( void* pUserData_ )
    {
      pUserData = pUserData_;
      return *this;
    }

    AllocationCallbacks& setPfnAllocation( PFN_vkAllocationFunction pfnAllocation_ )
    {
      pfnAllocation = pfnAllocation_;
      return *this;
    }

    AllocationCallbacks& setPfnReallocation( PFN_vkReallocationFunction pfnReallocation_ )
    {
      pfnReallocation = pfnReallocation_;
      return *this;
    }

    AllocationCallbacks& setPfnFree( PFN_vkFreeFunction pfnFree_ )
    {
      pfnFree = pfnFree_;
      return *this;
    }

    AllocationCallbacks& setPfnInternalAllocation( PFN_vkInternalAllocationNotification pfnInternalAllocation_ )
    {
      pfnInternalAllocation = pfnInternalAllocation_;
      return *this;
    }

    AllocationCallbacks& setPfnInternalFree( PFN_vkInternalFreeNotification pfnInternalFree_ )
    {
      pfnInternalFree = pfnInternalFree_;
      return *this;
    }

    operator const VkAllocationCallbacks&() const
    {
      return *reinterpret_cast<const VkAllocationCallbacks*>(this);
    }

    bool operator==( AllocationCallbacks const& rhs ) const
    {
      return ( pUserData == rhs.pUserData )
          && ( pfnAllocation == rhs.pfnAllocation )
          && ( pfnReallocation == rhs.pfnReallocation )
          && ( pfnFree == rhs.pfnFree )
          && ( pfnInternalAllocation == rhs.pfnInternalAllocation )
          && ( pfnInternalFree == rhs.pfnInternalFree );
    }

    bool operator!=( AllocationCallbacks const& rhs ) const
    {
      return !operator==( rhs );
    }

    void* pUserData;
    PFN_vkAllocationFunction pfnAllocation;
    PFN_vkReallocationFunction pfnReallocation;
    PFN_vkFreeFunction pfnFree;
    PFN_vkInternalAllocationNotification pfnInternalAllocation;
    PFN_vkInternalFreeNotification pfnInternalFree;
  };
  static_assert( sizeof( AllocationCallbacks ) == sizeof( VkAllocationCallbacks ), "struct and wrapper have different size!" );

  struct MemoryRequirements
  {
    operator const VkMemoryRequirements&() const
    {
      return *reinterpret_cast<const VkMemoryRequirements*>(this);
    }

    bool operator==( MemoryRequirements const& rhs ) const
    {
      return ( size == rhs.size )
          && ( alignment == rhs.alignment )
          && ( memoryTypeBits == rhs.memoryTypeBits );
    }

    bool operator!=( MemoryRequirements const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize size;
    DeviceSize alignment;
    uint32_t memoryTypeBits;
  };
  static_assert( sizeof( MemoryRequirements ) == sizeof( VkMemoryRequirements ), "struct and wrapper have different size!" );

  struct DescriptorBufferInfo
  {
    DescriptorBufferInfo( Buffer buffer_ = Buffer(), DeviceSize offset_ = 0, DeviceSize range_ = 0 )
      : buffer( buffer_ )
      , offset( offset_ )
      , range( range_ )
    {
    }

    DescriptorBufferInfo( VkDescriptorBufferInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorBufferInfo) );
    }

    DescriptorBufferInfo& operator=( VkDescriptorBufferInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorBufferInfo) );
      return *this;
    }

    DescriptorBufferInfo& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    DescriptorBufferInfo& setOffset( DeviceSize offset_ )
    {
      offset = offset_;
      return *this;
    }

    DescriptorBufferInfo& setRange( DeviceSize range_ )
    {
      range = range_;
      return *this;
    }

    operator const VkDescriptorBufferInfo&() const
    {
      return *reinterpret_cast<const VkDescriptorBufferInfo*>(this);
    }

    bool operator==( DescriptorBufferInfo const& rhs ) const
    {
      return ( buffer == rhs.buffer )
          && ( offset == rhs.offset )
          && ( range == rhs.range );
    }

    bool operator!=( DescriptorBufferInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    Buffer buffer;
    DeviceSize offset;
    DeviceSize range;
  };
  static_assert( sizeof( DescriptorBufferInfo ) == sizeof( VkDescriptorBufferInfo ), "struct and wrapper have different size!" );

  struct SubresourceLayout
  {
    operator const VkSubresourceLayout&() const
    {
      return *reinterpret_cast<const VkSubresourceLayout*>(this);
    }

    bool operator==( SubresourceLayout const& rhs ) const
    {
      return ( offset == rhs.offset )
          && ( size == rhs.size )
          && ( rowPitch == rhs.rowPitch )
          && ( arrayPitch == rhs.arrayPitch )
          && ( depthPitch == rhs.depthPitch );
    }

    bool operator!=( SubresourceLayout const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize offset;
    DeviceSize size;
    DeviceSize rowPitch;
    DeviceSize arrayPitch;
    DeviceSize depthPitch;
  };
  static_assert( sizeof( SubresourceLayout ) == sizeof( VkSubresourceLayout ), "struct and wrapper have different size!" );

  struct BufferCopy
  {
    BufferCopy( DeviceSize srcOffset_ = 0, DeviceSize dstOffset_ = 0, DeviceSize size_ = 0 )
      : srcOffset( srcOffset_ )
      , dstOffset( dstOffset_ )
      , size( size_ )
    {
    }

    BufferCopy( VkBufferCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferCopy) );
    }

    BufferCopy& operator=( VkBufferCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferCopy) );
      return *this;
    }

    BufferCopy& setSrcOffset( DeviceSize srcOffset_ )
    {
      srcOffset = srcOffset_;
      return *this;
    }

    BufferCopy& setDstOffset( DeviceSize dstOffset_ )
    {
      dstOffset = dstOffset_;
      return *this;
    }

    BufferCopy& setSize( DeviceSize size_ )
    {
      size = size_;
      return *this;
    }

    operator const VkBufferCopy&() const
    {
      return *reinterpret_cast<const VkBufferCopy*>(this);
    }

    bool operator==( BufferCopy const& rhs ) const
    {
      return ( srcOffset == rhs.srcOffset )
          && ( dstOffset == rhs.dstOffset )
          && ( size == rhs.size );
    }

    bool operator!=( BufferCopy const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize srcOffset;
    DeviceSize dstOffset;
    DeviceSize size;
  };
  static_assert( sizeof( BufferCopy ) == sizeof( VkBufferCopy ), "struct and wrapper have different size!" );

  struct SpecializationMapEntry
  {
    SpecializationMapEntry( uint32_t constantID_ = 0, uint32_t offset_ = 0, size_t size_ = 0 )
      : constantID( constantID_ )
      , offset( offset_ )
      , size( size_ )
    {
    }

    SpecializationMapEntry( VkSpecializationMapEntry const & rhs )
    {
      memcpy( this, &rhs, sizeof(SpecializationMapEntry) );
    }

    SpecializationMapEntry& operator=( VkSpecializationMapEntry const & rhs )
    {
      memcpy( this, &rhs, sizeof(SpecializationMapEntry) );
      return *this;
    }

    SpecializationMapEntry& setConstantID( uint32_t constantID_ )
    {
      constantID = constantID_;
      return *this;
    }

    SpecializationMapEntry& setOffset( uint32_t offset_ )
    {
      offset = offset_;
      return *this;
    }

    SpecializationMapEntry& setSize( size_t size_ )
    {
      size = size_;
      return *this;
    }

    operator const VkSpecializationMapEntry&() const
    {
      return *reinterpret_cast<const VkSpecializationMapEntry*>(this);
    }

    bool operator==( SpecializationMapEntry const& rhs ) const
    {
      return ( constantID == rhs.constantID )
          && ( offset == rhs.offset )
          && ( size == rhs.size );
    }

    bool operator!=( SpecializationMapEntry const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t constantID;
    uint32_t offset;
    size_t size;
  };
  static_assert( sizeof( SpecializationMapEntry ) == sizeof( VkSpecializationMapEntry ), "struct and wrapper have different size!" );

  struct SpecializationInfo
  {
    SpecializationInfo( uint32_t mapEntryCount_ = 0, const SpecializationMapEntry* pMapEntries_ = nullptr, size_t dataSize_ = 0, const void* pData_ = nullptr )
      : mapEntryCount( mapEntryCount_ )
      , pMapEntries( pMapEntries_ )
      , dataSize( dataSize_ )
      , pData( pData_ )
    {
    }

    SpecializationInfo( VkSpecializationInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SpecializationInfo) );
    }

    SpecializationInfo& operator=( VkSpecializationInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SpecializationInfo) );
      return *this;
    }

    SpecializationInfo& setMapEntryCount( uint32_t mapEntryCount_ )
    {
      mapEntryCount = mapEntryCount_;
      return *this;
    }

    SpecializationInfo& setPMapEntries( const SpecializationMapEntry* pMapEntries_ )
    {
      pMapEntries = pMapEntries_;
      return *this;
    }

    SpecializationInfo& setDataSize( size_t dataSize_ )
    {
      dataSize = dataSize_;
      return *this;
    }

    SpecializationInfo& setPData( const void* pData_ )
    {
      pData = pData_;
      return *this;
    }

    operator const VkSpecializationInfo&() const
    {
      return *reinterpret_cast<const VkSpecializationInfo*>(this);
    }

    bool operator==( SpecializationInfo const& rhs ) const
    {
      return ( mapEntryCount == rhs.mapEntryCount )
          && ( pMapEntries == rhs.pMapEntries )
          && ( dataSize == rhs.dataSize )
          && ( pData == rhs.pData );
    }

    bool operator!=( SpecializationInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t mapEntryCount;
    const SpecializationMapEntry* pMapEntries;
    size_t dataSize;
    const void* pData;
  };
  static_assert( sizeof( SpecializationInfo ) == sizeof( VkSpecializationInfo ), "struct and wrapper have different size!" );

  union ClearColorValue
  {
    ClearColorValue( const std::array<float,4>& float32_ = { {0} } )
    {
      memcpy( &float32, float32_.data(), 4 * sizeof( float ) );
    }

    ClearColorValue( const std::array<int32_t,4>& int32_ )
    {
      memcpy( &int32, int32_.data(), 4 * sizeof( int32_t ) );
    }

    ClearColorValue( const std::array<uint32_t,4>& uint32_ )
    {
      memcpy( &uint32, uint32_.data(), 4 * sizeof( uint32_t ) );
    }

    ClearColorValue& setFloat32( std::array<float,4> float32_ )
    {
      memcpy( &float32, float32_.data(), 4 * sizeof( float ) );
      return *this;
    }

    ClearColorValue& setInt32( std::array<int32_t,4> int32_ )
    {
      memcpy( &int32, int32_.data(), 4 * sizeof( int32_t ) );
      return *this;
    }

    ClearColorValue& setUint32( std::array<uint32_t,4> uint32_ )
    {
      memcpy( &uint32, uint32_.data(), 4 * sizeof( uint32_t ) );
      return *this;
    }

    operator VkClearColorValue const& () const
    {
      return *reinterpret_cast<const VkClearColorValue*>(this);
    }

    float float32[4];
    int32_t int32[4];
    uint32_t uint32[4];
  };

  struct ClearDepthStencilValue
  {
    ClearDepthStencilValue( float depth_ = 0, uint32_t stencil_ = 0 )
      : depth( depth_ )
      , stencil( stencil_ )
    {
    }

    ClearDepthStencilValue( VkClearDepthStencilValue const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearDepthStencilValue) );
    }

    ClearDepthStencilValue& operator=( VkClearDepthStencilValue const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearDepthStencilValue) );
      return *this;
    }

    ClearDepthStencilValue& setDepth( float depth_ )
    {
      depth = depth_;
      return *this;
    }

    ClearDepthStencilValue& setStencil( uint32_t stencil_ )
    {
      stencil = stencil_;
      return *this;
    }

    operator const VkClearDepthStencilValue&() const
    {
      return *reinterpret_cast<const VkClearDepthStencilValue*>(this);
    }

    bool operator==( ClearDepthStencilValue const& rhs ) const
    {
      return ( depth == rhs.depth )
          && ( stencil == rhs.stencil );
    }

    bool operator!=( ClearDepthStencilValue const& rhs ) const
    {
      return !operator==( rhs );
    }

    float depth;
    uint32_t stencil;
  };
  static_assert( sizeof( ClearDepthStencilValue ) == sizeof( VkClearDepthStencilValue ), "struct and wrapper have different size!" );

  union ClearValue
  {
    ClearValue( ClearColorValue color_ = ClearColorValue() )
    {
      color = color_;
    }

    ClearValue( ClearDepthStencilValue depthStencil_ )
    {
      depthStencil = depthStencil_;
    }

    ClearValue& setColor( ClearColorValue color_ )
    {
      color = color_;
      return *this;
    }

    ClearValue& setDepthStencil( ClearDepthStencilValue depthStencil_ )
    {
      depthStencil = depthStencil_;
      return *this;
    }

    operator VkClearValue const& () const
    {
      return *reinterpret_cast<const VkClearValue*>(this);
    }

#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
    ClearColorValue color;
    ClearDepthStencilValue depthStencil;
#else
    VkClearColorValue color;
    VkClearDepthStencilValue depthStencil;
#endif  // VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
  };

  struct PhysicalDeviceFeatures
  {
    PhysicalDeviceFeatures( Bool32 robustBufferAccess_ = 0, Bool32 fullDrawIndexUint32_ = 0, Bool32 imageCubeArray_ = 0, Bool32 independentBlend_ = 0, Bool32 geometryShader_ = 0, Bool32 tessellationShader_ = 0, Bool32 sampleRateShading_ = 0, Bool32 dualSrcBlend_ = 0, Bool32 logicOp_ = 0, Bool32 multiDrawIndirect_ = 0, Bool32 drawIndirectFirstInstance_ = 0, Bool32 depthClamp_ = 0, Bool32 depthBiasClamp_ = 0, Bool32 fillModeNonSolid_ = 0, Bool32 depthBounds_ = 0, Bool32 wideLines_ = 0, Bool32 largePoints_ = 0, Bool32 alphaToOne_ = 0, Bool32 multiViewport_ = 0, Bool32 samplerAnisotropy_ = 0, Bool32 textureCompressionETC2_ = 0, Bool32 textureCompressionASTC_LDR_ = 0, Bool32 textureCompressionBC_ = 0, Bool32 occlusionQueryPrecise_ = 0, Bool32 pipelineStatisticsQuery_ = 0, Bool32 vertexPipelineStoresAndAtomics_ = 0, Bool32 fragmentStoresAndAtomics_ = 0, Bool32 shaderTessellationAndGeometryPointSize_ = 0, Bool32 shaderImageGatherExtended_ = 0, Bool32 shaderStorageImageExtendedFormats_ = 0, Bool32 shaderStorageImageMultisample_ = 0, Bool32 shaderStorageImageReadWithoutFormat_ = 0, Bool32 shaderStorageImageWriteWithoutFormat_ = 0, Bool32 shaderUniformBufferArrayDynamicIndexing_ = 0, Bool32 shaderSampledImageArrayDynamicIndexing_ = 0, Bool32 shaderStorageBufferArrayDynamicIndexing_ = 0, Bool32 shaderStorageImageArrayDynamicIndexing_ = 0, Bool32 shaderClipDistance_ = 0, Bool32 shaderCullDistance_ = 0, Bool32 shaderFloat64_ = 0, Bool32 shaderInt64_ = 0, Bool32 shaderInt16_ = 0, Bool32 shaderResourceResidency_ = 0, Bool32 shaderResourceMinLod_ = 0, Bool32 sparseBinding_ = 0, Bool32 sparseResidencyBuffer_ = 0, Bool32 sparseResidencyImage2D_ = 0, Bool32 sparseResidencyImage3D_ = 0, Bool32 sparseResidency2Samples_ = 0, Bool32 sparseResidency4Samples_ = 0, Bool32 sparseResidency8Samples_ = 0, Bool32 sparseResidency16Samples_ = 0, Bool32 sparseResidencyAliased_ = 0, Bool32 variableMultisampleRate_ = 0, Bool32 inheritedQueries_ = 0 )
      : robustBufferAccess( robustBufferAccess_ )
      , fullDrawIndexUint32( fullDrawIndexUint32_ )
      , imageCubeArray( imageCubeArray_ )
      , independentBlend( independentBlend_ )
      , geometryShader( geometryShader_ )
      , tessellationShader( tessellationShader_ )
      , sampleRateShading( sampleRateShading_ )
      , dualSrcBlend( dualSrcBlend_ )
      , logicOp( logicOp_ )
      , multiDrawIndirect( multiDrawIndirect_ )
      , drawIndirectFirstInstance( drawIndirectFirstInstance_ )
      , depthClamp( depthClamp_ )
      , depthBiasClamp( depthBiasClamp_ )
      , fillModeNonSolid( fillModeNonSolid_ )
      , depthBounds( depthBounds_ )
      , wideLines( wideLines_ )
      , largePoints( largePoints_ )
      , alphaToOne( alphaToOne_ )
      , multiViewport( multiViewport_ )
      , samplerAnisotropy( samplerAnisotropy_ )
      , textureCompressionETC2( textureCompressionETC2_ )
      , textureCompressionASTC_LDR( textureCompressionASTC_LDR_ )
      , textureCompressionBC( textureCompressionBC_ )
      , occlusionQueryPrecise( occlusionQueryPrecise_ )
      , pipelineStatisticsQuery( pipelineStatisticsQuery_ )
      , vertexPipelineStoresAndAtomics( vertexPipelineStoresAndAtomics_ )
      , fragmentStoresAndAtomics( fragmentStoresAndAtomics_ )
      , shaderTessellationAndGeometryPointSize( shaderTessellationAndGeometryPointSize_ )
      , shaderImageGatherExtended( shaderImageGatherExtended_ )
      , shaderStorageImageExtendedFormats( shaderStorageImageExtendedFormats_ )
      , shaderStorageImageMultisample( shaderStorageImageMultisample_ )
      , shaderStorageImageReadWithoutFormat( shaderStorageImageReadWithoutFormat_ )
      , shaderStorageImageWriteWithoutFormat( shaderStorageImageWriteWithoutFormat_ )
      , shaderUniformBufferArrayDynamicIndexing( shaderUniformBufferArrayDynamicIndexing_ )
      , shaderSampledImageArrayDynamicIndexing( shaderSampledImageArrayDynamicIndexing_ )
      , shaderStorageBufferArrayDynamicIndexing( shaderStorageBufferArrayDynamicIndexing_ )
      , shaderStorageImageArrayDynamicIndexing( shaderStorageImageArrayDynamicIndexing_ )
      , shaderClipDistance( shaderClipDistance_ )
      , shaderCullDistance( shaderCullDistance_ )
      , shaderFloat64( shaderFloat64_ )
      , shaderInt64( shaderInt64_ )
      , shaderInt16( shaderInt16_ )
      , shaderResourceResidency( shaderResourceResidency_ )
      , shaderResourceMinLod( shaderResourceMinLod_ )
      , sparseBinding( sparseBinding_ )
      , sparseResidencyBuffer( sparseResidencyBuffer_ )
      , sparseResidencyImage2D( sparseResidencyImage2D_ )
      , sparseResidencyImage3D( sparseResidencyImage3D_ )
      , sparseResidency2Samples( sparseResidency2Samples_ )
      , sparseResidency4Samples( sparseResidency4Samples_ )
      , sparseResidency8Samples( sparseResidency8Samples_ )
      , sparseResidency16Samples( sparseResidency16Samples_ )
      , sparseResidencyAliased( sparseResidencyAliased_ )
      , variableMultisampleRate( variableMultisampleRate_ )
      , inheritedQueries( inheritedQueries_ )
    {
    }

    PhysicalDeviceFeatures( VkPhysicalDeviceFeatures const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceFeatures) );
    }

    PhysicalDeviceFeatures& operator=( VkPhysicalDeviceFeatures const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceFeatures) );
      return *this;
    }

    PhysicalDeviceFeatures& setRobustBufferAccess( Bool32 robustBufferAccess_ )
    {
      robustBufferAccess = robustBufferAccess_;
      return *this;
    }

    PhysicalDeviceFeatures& setFullDrawIndexUint32( Bool32 fullDrawIndexUint32_ )
    {
      fullDrawIndexUint32 = fullDrawIndexUint32_;
      return *this;
    }

    PhysicalDeviceFeatures& setImageCubeArray( Bool32 imageCubeArray_ )
    {
      imageCubeArray = imageCubeArray_;
      return *this;
    }

    PhysicalDeviceFeatures& setIndependentBlend( Bool32 independentBlend_ )
    {
      independentBlend = independentBlend_;
      return *this;
    }

    PhysicalDeviceFeatures& setGeometryShader( Bool32 geometryShader_ )
    {
      geometryShader = geometryShader_;
      return *this;
    }

    PhysicalDeviceFeatures& setTessellationShader( Bool32 tessellationShader_ )
    {
      tessellationShader = tessellationShader_;
      return *this;
    }

    PhysicalDeviceFeatures& setSampleRateShading( Bool32 sampleRateShading_ )
    {
      sampleRateShading = sampleRateShading_;
      return *this;
    }

    PhysicalDeviceFeatures& setDualSrcBlend( Bool32 dualSrcBlend_ )
    {
      dualSrcBlend = dualSrcBlend_;
      return *this;
    }

    PhysicalDeviceFeatures& setLogicOp( Bool32 logicOp_ )
    {
      logicOp = logicOp_;
      return *this;
    }

    PhysicalDeviceFeatures& setMultiDrawIndirect( Bool32 multiDrawIndirect_ )
    {
      multiDrawIndirect = multiDrawIndirect_;
      return *this;
    }

    PhysicalDeviceFeatures& setDrawIndirectFirstInstance( Bool32 drawIndirectFirstInstance_ )
    {
      drawIndirectFirstInstance = drawIndirectFirstInstance_;
      return *this;
    }

    PhysicalDeviceFeatures& setDepthClamp( Bool32 depthClamp_ )
    {
      depthClamp = depthClamp_;
      return *this;
    }

    PhysicalDeviceFeatures& setDepthBiasClamp( Bool32 depthBiasClamp_ )
    {
      depthBiasClamp = depthBiasClamp_;
      return *this;
    }

    PhysicalDeviceFeatures& setFillModeNonSolid( Bool32 fillModeNonSolid_ )
    {
      fillModeNonSolid = fillModeNonSolid_;
      return *this;
    }

    PhysicalDeviceFeatures& setDepthBounds( Bool32 depthBounds_ )
    {
      depthBounds = depthBounds_;
      return *this;
    }

    PhysicalDeviceFeatures& setWideLines( Bool32 wideLines_ )
    {
      wideLines = wideLines_;
      return *this;
    }

    PhysicalDeviceFeatures& setLargePoints( Bool32 largePoints_ )
    {
      largePoints = largePoints_;
      return *this;
    }

    PhysicalDeviceFeatures& setAlphaToOne( Bool32 alphaToOne_ )
    {
      alphaToOne = alphaToOne_;
      return *this;
    }

    PhysicalDeviceFeatures& setMultiViewport( Bool32 multiViewport_ )
    {
      multiViewport = multiViewport_;
      return *this;
    }

    PhysicalDeviceFeatures& setSamplerAnisotropy( Bool32 samplerAnisotropy_ )
    {
      samplerAnisotropy = samplerAnisotropy_;
      return *this;
    }

    PhysicalDeviceFeatures& setTextureCompressionETC2( Bool32 textureCompressionETC2_ )
    {
      textureCompressionETC2 = textureCompressionETC2_;
      return *this;
    }

    PhysicalDeviceFeatures& setTextureCompressionASTC_LDR( Bool32 textureCompressionASTC_LDR_ )
    {
      textureCompressionASTC_LDR = textureCompressionASTC_LDR_;
      return *this;
    }

    PhysicalDeviceFeatures& setTextureCompressionBC( Bool32 textureCompressionBC_ )
    {
      textureCompressionBC = textureCompressionBC_;
      return *this;
    }

    PhysicalDeviceFeatures& setOcclusionQueryPrecise( Bool32 occlusionQueryPrecise_ )
    {
      occlusionQueryPrecise = occlusionQueryPrecise_;
      return *this;
    }

    PhysicalDeviceFeatures& setPipelineStatisticsQuery( Bool32 pipelineStatisticsQuery_ )
    {
      pipelineStatisticsQuery = pipelineStatisticsQuery_;
      return *this;
    }

    PhysicalDeviceFeatures& setVertexPipelineStoresAndAtomics( Bool32 vertexPipelineStoresAndAtomics_ )
    {
      vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics_;
      return *this;
    }

    PhysicalDeviceFeatures& setFragmentStoresAndAtomics( Bool32 fragmentStoresAndAtomics_ )
    {
      fragmentStoresAndAtomics = fragmentStoresAndAtomics_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderTessellationAndGeometryPointSize( Bool32 shaderTessellationAndGeometryPointSize_ )
    {
      shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderImageGatherExtended( Bool32 shaderImageGatherExtended_ )
    {
      shaderImageGatherExtended = shaderImageGatherExtended_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageImageExtendedFormats( Bool32 shaderStorageImageExtendedFormats_ )
    {
      shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageImageMultisample( Bool32 shaderStorageImageMultisample_ )
    {
      shaderStorageImageMultisample = shaderStorageImageMultisample_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageImageReadWithoutFormat( Bool32 shaderStorageImageReadWithoutFormat_ )
    {
      shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageImageWriteWithoutFormat( Bool32 shaderStorageImageWriteWithoutFormat_ )
    {
      shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderUniformBufferArrayDynamicIndexing( Bool32 shaderUniformBufferArrayDynamicIndexing_ )
    {
      shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderSampledImageArrayDynamicIndexing( Bool32 shaderSampledImageArrayDynamicIndexing_ )
    {
      shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageBufferArrayDynamicIndexing( Bool32 shaderStorageBufferArrayDynamicIndexing_ )
    {
      shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderStorageImageArrayDynamicIndexing( Bool32 shaderStorageImageArrayDynamicIndexing_ )
    {
      shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderClipDistance( Bool32 shaderClipDistance_ )
    {
      shaderClipDistance = shaderClipDistance_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderCullDistance( Bool32 shaderCullDistance_ )
    {
      shaderCullDistance = shaderCullDistance_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderFloat64( Bool32 shaderFloat64_ )
    {
      shaderFloat64 = shaderFloat64_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderInt64( Bool32 shaderInt64_ )
    {
      shaderInt64 = shaderInt64_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderInt16( Bool32 shaderInt16_ )
    {
      shaderInt16 = shaderInt16_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderResourceResidency( Bool32 shaderResourceResidency_ )
    {
      shaderResourceResidency = shaderResourceResidency_;
      return *this;
    }

    PhysicalDeviceFeatures& setShaderResourceMinLod( Bool32 shaderResourceMinLod_ )
    {
      shaderResourceMinLod = shaderResourceMinLod_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseBinding( Bool32 sparseBinding_ )
    {
      sparseBinding = sparseBinding_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidencyBuffer( Bool32 sparseResidencyBuffer_ )
    {
      sparseResidencyBuffer = sparseResidencyBuffer_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidencyImage2D( Bool32 sparseResidencyImage2D_ )
    {
      sparseResidencyImage2D = sparseResidencyImage2D_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidencyImage3D( Bool32 sparseResidencyImage3D_ )
    {
      sparseResidencyImage3D = sparseResidencyImage3D_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidency2Samples( Bool32 sparseResidency2Samples_ )
    {
      sparseResidency2Samples = sparseResidency2Samples_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidency4Samples( Bool32 sparseResidency4Samples_ )
    {
      sparseResidency4Samples = sparseResidency4Samples_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidency8Samples( Bool32 sparseResidency8Samples_ )
    {
      sparseResidency8Samples = sparseResidency8Samples_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidency16Samples( Bool32 sparseResidency16Samples_ )
    {
      sparseResidency16Samples = sparseResidency16Samples_;
      return *this;
    }

    PhysicalDeviceFeatures& setSparseResidencyAliased( Bool32 sparseResidencyAliased_ )
    {
      sparseResidencyAliased = sparseResidencyAliased_;
      return *this;
    }

    PhysicalDeviceFeatures& setVariableMultisampleRate( Bool32 variableMultisampleRate_ )
    {
      variableMultisampleRate = variableMultisampleRate_;
      return *this;
    }

    PhysicalDeviceFeatures& setInheritedQueries( Bool32 inheritedQueries_ )
    {
      inheritedQueries = inheritedQueries_;
      return *this;
    }

    operator const VkPhysicalDeviceFeatures&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceFeatures*>(this);
    }

    bool operator==( PhysicalDeviceFeatures const& rhs ) const
    {
      return ( robustBufferAccess == rhs.robustBufferAccess )
          && ( fullDrawIndexUint32 == rhs.fullDrawIndexUint32 )
          && ( imageCubeArray == rhs.imageCubeArray )
          && ( independentBlend == rhs.independentBlend )
          && ( geometryShader == rhs.geometryShader )
          && ( tessellationShader == rhs.tessellationShader )
          && ( sampleRateShading == rhs.sampleRateShading )
          && ( dualSrcBlend == rhs.dualSrcBlend )
          && ( logicOp == rhs.logicOp )
          && ( multiDrawIndirect == rhs.multiDrawIndirect )
          && ( drawIndirectFirstInstance == rhs.drawIndirectFirstInstance )
          && ( depthClamp == rhs.depthClamp )
          && ( depthBiasClamp == rhs.depthBiasClamp )
          && ( fillModeNonSolid == rhs.fillModeNonSolid )
          && ( depthBounds == rhs.depthBounds )
          && ( wideLines == rhs.wideLines )
          && ( largePoints == rhs.largePoints )
          && ( alphaToOne == rhs.alphaToOne )
          && ( multiViewport == rhs.multiViewport )
          && ( samplerAnisotropy == rhs.samplerAnisotropy )
          && ( textureCompressionETC2 == rhs.textureCompressionETC2 )
          && ( textureCompressionASTC_LDR == rhs.textureCompressionASTC_LDR )
          && ( textureCompressionBC == rhs.textureCompressionBC )
          && ( occlusionQueryPrecise == rhs.occlusionQueryPrecise )
          && ( pipelineStatisticsQuery == rhs.pipelineStatisticsQuery )
          && ( vertexPipelineStoresAndAtomics == rhs.vertexPipelineStoresAndAtomics )
          && ( fragmentStoresAndAtomics == rhs.fragmentStoresAndAtomics )
          && ( shaderTessellationAndGeometryPointSize == rhs.shaderTessellationAndGeometryPointSize )
          && ( shaderImageGatherExtended == rhs.shaderImageGatherExtended )
          && ( shaderStorageImageExtendedFormats == rhs.shaderStorageImageExtendedFormats )
          && ( shaderStorageImageMultisample == rhs.shaderStorageImageMultisample )
          && ( shaderStorageImageReadWithoutFormat == rhs.shaderStorageImageReadWithoutFormat )
          && ( shaderStorageImageWriteWithoutFormat == rhs.shaderStorageImageWriteWithoutFormat )
          && ( shaderUniformBufferArrayDynamicIndexing == rhs.shaderUniformBufferArrayDynamicIndexing )
          && ( shaderSampledImageArrayDynamicIndexing == rhs.shaderSampledImageArrayDynamicIndexing )
          && ( shaderStorageBufferArrayDynamicIndexing == rhs.shaderStorageBufferArrayDynamicIndexing )
          && ( shaderStorageImageArrayDynamicIndexing == rhs.shaderStorageImageArrayDynamicIndexing )
          && ( shaderClipDistance == rhs.shaderClipDistance )
          && ( shaderCullDistance == rhs.shaderCullDistance )
          && ( shaderFloat64 == rhs.shaderFloat64 )
          && ( shaderInt64 == rhs.shaderInt64 )
          && ( shaderInt16 == rhs.shaderInt16 )
          && ( shaderResourceResidency == rhs.shaderResourceResidency )
          && ( shaderResourceMinLod == rhs.shaderResourceMinLod )
          && ( sparseBinding == rhs.sparseBinding )
          && ( sparseResidencyBuffer == rhs.sparseResidencyBuffer )
          && ( sparseResidencyImage2D == rhs.sparseResidencyImage2D )
          && ( sparseResidencyImage3D == rhs.sparseResidencyImage3D )
          && ( sparseResidency2Samples == rhs.sparseResidency2Samples )
          && ( sparseResidency4Samples == rhs.sparseResidency4Samples )
          && ( sparseResidency8Samples == rhs.sparseResidency8Samples )
          && ( sparseResidency16Samples == rhs.sparseResidency16Samples )
          && ( sparseResidencyAliased == rhs.sparseResidencyAliased )
          && ( variableMultisampleRate == rhs.variableMultisampleRate )
          && ( inheritedQueries == rhs.inheritedQueries );
    }

    bool operator!=( PhysicalDeviceFeatures const& rhs ) const
    {
      return !operator==( rhs );
    }

    Bool32 robustBufferAccess;
    Bool32 fullDrawIndexUint32;
    Bool32 imageCubeArray;
    Bool32 independentBlend;
    Bool32 geometryShader;
    Bool32 tessellationShader;
    Bool32 sampleRateShading;
    Bool32 dualSrcBlend;
    Bool32 logicOp;
    Bool32 multiDrawIndirect;
    Bool32 drawIndirectFirstInstance;
    Bool32 depthClamp;
    Bool32 depthBiasClamp;
    Bool32 fillModeNonSolid;
    Bool32 depthBounds;
    Bool32 wideLines;
    Bool32 largePoints;
    Bool32 alphaToOne;
    Bool32 multiViewport;
    Bool32 samplerAnisotropy;
    Bool32 textureCompressionETC2;
    Bool32 textureCompressionASTC_LDR;
    Bool32 textureCompressionBC;
    Bool32 occlusionQueryPrecise;
    Bool32 pipelineStatisticsQuery;
    Bool32 vertexPipelineStoresAndAtomics;
    Bool32 fragmentStoresAndAtomics;
    Bool32 shaderTessellationAndGeometryPointSize;
    Bool32 shaderImageGatherExtended;
    Bool32 shaderStorageImageExtendedFormats;
    Bool32 shaderStorageImageMultisample;
    Bool32 shaderStorageImageReadWithoutFormat;
    Bool32 shaderStorageImageWriteWithoutFormat;
    Bool32 shaderUniformBufferArrayDynamicIndexing;
    Bool32 shaderSampledImageArrayDynamicIndexing;
    Bool32 shaderStorageBufferArrayDynamicIndexing;
    Bool32 shaderStorageImageArrayDynamicIndexing;
    Bool32 shaderClipDistance;
    Bool32 shaderCullDistance;
    Bool32 shaderFloat64;
    Bool32 shaderInt64;
    Bool32 shaderInt16;
    Bool32 shaderResourceResidency;
    Bool32 shaderResourceMinLod;
    Bool32 sparseBinding;
    Bool32 sparseResidencyBuffer;
    Bool32 sparseResidencyImage2D;
    Bool32 sparseResidencyImage3D;
    Bool32 sparseResidency2Samples;
    Bool32 sparseResidency4Samples;
    Bool32 sparseResidency8Samples;
    Bool32 sparseResidency16Samples;
    Bool32 sparseResidencyAliased;
    Bool32 variableMultisampleRate;
    Bool32 inheritedQueries;
  };
  static_assert( sizeof( PhysicalDeviceFeatures ) == sizeof( VkPhysicalDeviceFeatures ), "struct and wrapper have different size!" );

  struct PhysicalDeviceSparseProperties
  {
    operator const VkPhysicalDeviceSparseProperties&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceSparseProperties*>(this);
    }

    bool operator==( PhysicalDeviceSparseProperties const& rhs ) const
    {
      return ( residencyStandard2DBlockShape == rhs.residencyStandard2DBlockShape )
          && ( residencyStandard2DMultisampleBlockShape == rhs.residencyStandard2DMultisampleBlockShape )
          && ( residencyStandard3DBlockShape == rhs.residencyStandard3DBlockShape )
          && ( residencyAlignedMipSize == rhs.residencyAlignedMipSize )
          && ( residencyNonResidentStrict == rhs.residencyNonResidentStrict );
    }

    bool operator!=( PhysicalDeviceSparseProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    Bool32 residencyStandard2DBlockShape;
    Bool32 residencyStandard2DMultisampleBlockShape;
    Bool32 residencyStandard3DBlockShape;
    Bool32 residencyAlignedMipSize;
    Bool32 residencyNonResidentStrict;
  };
  static_assert( sizeof( PhysicalDeviceSparseProperties ) == sizeof( VkPhysicalDeviceSparseProperties ), "struct and wrapper have different size!" );

  struct DrawIndirectCommand
  {
    DrawIndirectCommand( uint32_t vertexCount_ = 0, uint32_t instanceCount_ = 0, uint32_t firstVertex_ = 0, uint32_t firstInstance_ = 0 )
      : vertexCount( vertexCount_ )
      , instanceCount( instanceCount_ )
      , firstVertex( firstVertex_ )
      , firstInstance( firstInstance_ )
    {
    }

    DrawIndirectCommand( VkDrawIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DrawIndirectCommand) );
    }

    DrawIndirectCommand& operator=( VkDrawIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DrawIndirectCommand) );
      return *this;
    }

    DrawIndirectCommand& setVertexCount( uint32_t vertexCount_ )
    {
      vertexCount = vertexCount_;
      return *this;
    }

    DrawIndirectCommand& setInstanceCount( uint32_t instanceCount_ )
    {
      instanceCount = instanceCount_;
      return *this;
    }

    DrawIndirectCommand& setFirstVertex( uint32_t firstVertex_ )
    {
      firstVertex = firstVertex_;
      return *this;
    }

    DrawIndirectCommand& setFirstInstance( uint32_t firstInstance_ )
    {
      firstInstance = firstInstance_;
      return *this;
    }

    operator const VkDrawIndirectCommand&() const
    {
      return *reinterpret_cast<const VkDrawIndirectCommand*>(this);
    }

    bool operator==( DrawIndirectCommand const& rhs ) const
    {
      return ( vertexCount == rhs.vertexCount )
          && ( instanceCount == rhs.instanceCount )
          && ( firstVertex == rhs.firstVertex )
          && ( firstInstance == rhs.firstInstance );
    }

    bool operator!=( DrawIndirectCommand const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    uint32_t firstInstance;
  };
  static_assert( sizeof( DrawIndirectCommand ) == sizeof( VkDrawIndirectCommand ), "struct and wrapper have different size!" );

  struct DrawIndexedIndirectCommand
  {
    DrawIndexedIndirectCommand( uint32_t indexCount_ = 0, uint32_t instanceCount_ = 0, uint32_t firstIndex_ = 0, int32_t vertexOffset_ = 0, uint32_t firstInstance_ = 0 )
      : indexCount( indexCount_ )
      , instanceCount( instanceCount_ )
      , firstIndex( firstIndex_ )
      , vertexOffset( vertexOffset_ )
      , firstInstance( firstInstance_ )
    {
    }

    DrawIndexedIndirectCommand( VkDrawIndexedIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DrawIndexedIndirectCommand) );
    }

    DrawIndexedIndirectCommand& operator=( VkDrawIndexedIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DrawIndexedIndirectCommand) );
      return *this;
    }

    DrawIndexedIndirectCommand& setIndexCount( uint32_t indexCount_ )
    {
      indexCount = indexCount_;
      return *this;
    }

    DrawIndexedIndirectCommand& setInstanceCount( uint32_t instanceCount_ )
    {
      instanceCount = instanceCount_;
      return *this;
    }

    DrawIndexedIndirectCommand& setFirstIndex( uint32_t firstIndex_ )
    {
      firstIndex = firstIndex_;
      return *this;
    }

    DrawIndexedIndirectCommand& setVertexOffset( int32_t vertexOffset_ )
    {
      vertexOffset = vertexOffset_;
      return *this;
    }

    DrawIndexedIndirectCommand& setFirstInstance( uint32_t firstInstance_ )
    {
      firstInstance = firstInstance_;
      return *this;
    }

    operator const VkDrawIndexedIndirectCommand&() const
    {
      return *reinterpret_cast<const VkDrawIndexedIndirectCommand*>(this);
    }

    bool operator==( DrawIndexedIndirectCommand const& rhs ) const
    {
      return ( indexCount == rhs.indexCount )
          && ( instanceCount == rhs.instanceCount )
          && ( firstIndex == rhs.firstIndex )
          && ( vertexOffset == rhs.vertexOffset )
          && ( firstInstance == rhs.firstInstance );
    }

    bool operator!=( DrawIndexedIndirectCommand const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t indexCount;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t vertexOffset;
    uint32_t firstInstance;
  };
  static_assert( sizeof( DrawIndexedIndirectCommand ) == sizeof( VkDrawIndexedIndirectCommand ), "struct and wrapper have different size!" );

  struct DispatchIndirectCommand
  {
    DispatchIndirectCommand( uint32_t x_ = 0, uint32_t y_ = 0, uint32_t z_ = 0 )
      : x( x_ )
      , y( y_ )
      , z( z_ )
    {
    }

    DispatchIndirectCommand( VkDispatchIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DispatchIndirectCommand) );
    }

    DispatchIndirectCommand& operator=( VkDispatchIndirectCommand const & rhs )
    {
      memcpy( this, &rhs, sizeof(DispatchIndirectCommand) );
      return *this;
    }

    DispatchIndirectCommand& setX( uint32_t x_ )
    {
      x = x_;
      return *this;
    }

    DispatchIndirectCommand& setY( uint32_t y_ )
    {
      y = y_;
      return *this;
    }

    DispatchIndirectCommand& setZ( uint32_t z_ )
    {
      z = z_;
      return *this;
    }

    operator const VkDispatchIndirectCommand&() const
    {
      return *reinterpret_cast<const VkDispatchIndirectCommand*>(this);
    }

    bool operator==( DispatchIndirectCommand const& rhs ) const
    {
      return ( x == rhs.x )
          && ( y == rhs.y )
          && ( z == rhs.z );
    }

    bool operator!=( DispatchIndirectCommand const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t x;
    uint32_t y;
    uint32_t z;
  };
  static_assert( sizeof( DispatchIndirectCommand ) == sizeof( VkDispatchIndirectCommand ), "struct and wrapper have different size!" );

  struct DisplayPlanePropertiesKHR
  {
    operator const VkDisplayPlanePropertiesKHR&() const
    {
      return *reinterpret_cast<const VkDisplayPlanePropertiesKHR*>(this);
    }

    bool operator==( DisplayPlanePropertiesKHR const& rhs ) const
    {
      return ( currentDisplay == rhs.currentDisplay )
          && ( currentStackIndex == rhs.currentStackIndex );
    }

    bool operator!=( DisplayPlanePropertiesKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    DisplayKHR currentDisplay;
    uint32_t currentStackIndex;
  };
  static_assert( sizeof( DisplayPlanePropertiesKHR ) == sizeof( VkDisplayPlanePropertiesKHR ), "struct and wrapper have different size!" );

  struct DisplayModeParametersKHR
  {
    DisplayModeParametersKHR( Extent2D visibleRegion_ = Extent2D(), uint32_t refreshRate_ = 0 )
      : visibleRegion( visibleRegion_ )
      , refreshRate( refreshRate_ )
    {
    }

    DisplayModeParametersKHR( VkDisplayModeParametersKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayModeParametersKHR) );
    }

    DisplayModeParametersKHR& operator=( VkDisplayModeParametersKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayModeParametersKHR) );
      return *this;
    }

    DisplayModeParametersKHR& setVisibleRegion( Extent2D visibleRegion_ )
    {
      visibleRegion = visibleRegion_;
      return *this;
    }

    DisplayModeParametersKHR& setRefreshRate( uint32_t refreshRate_ )
    {
      refreshRate = refreshRate_;
      return *this;
    }

    operator const VkDisplayModeParametersKHR&() const
    {
      return *reinterpret_cast<const VkDisplayModeParametersKHR*>(this);
    }

    bool operator==( DisplayModeParametersKHR const& rhs ) const
    {
      return ( visibleRegion == rhs.visibleRegion )
          && ( refreshRate == rhs.refreshRate );
    }

    bool operator!=( DisplayModeParametersKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    Extent2D visibleRegion;
    uint32_t refreshRate;
  };
  static_assert( sizeof( DisplayModeParametersKHR ) == sizeof( VkDisplayModeParametersKHR ), "struct and wrapper have different size!" );

  struct DisplayModePropertiesKHR
  {
    operator const VkDisplayModePropertiesKHR&() const
    {
      return *reinterpret_cast<const VkDisplayModePropertiesKHR*>(this);
    }

    bool operator==( DisplayModePropertiesKHR const& rhs ) const
    {
      return ( displayMode == rhs.displayMode )
          && ( parameters == rhs.parameters );
    }

    bool operator!=( DisplayModePropertiesKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    DisplayModeKHR displayMode;
    DisplayModeParametersKHR parameters;
  };
  static_assert( sizeof( DisplayModePropertiesKHR ) == sizeof( VkDisplayModePropertiesKHR ), "struct and wrapper have different size!" );

  enum class ImageLayout
  {
    eUndefined = VK_IMAGE_LAYOUT_UNDEFINED,
    eGeneral = VK_IMAGE_LAYOUT_GENERAL,
    eColorAttachmentOptimal = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    eDepthStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    eDepthStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    eShaderReadOnlyOptimal = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    eTransferSrcOptimal = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    eTransferDstOptimal = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    ePreinitialized = VK_IMAGE_LAYOUT_PREINITIALIZED,
    ePresentSrcKHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };

  struct DescriptorImageInfo
  {
    DescriptorImageInfo( Sampler sampler_ = Sampler(), ImageView imageView_ = ImageView(), ImageLayout imageLayout_ = ImageLayout::eUndefined )
      : sampler( sampler_ )
      , imageView( imageView_ )
      , imageLayout( imageLayout_ )
    {
    }

    DescriptorImageInfo( VkDescriptorImageInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorImageInfo) );
    }

    DescriptorImageInfo& operator=( VkDescriptorImageInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorImageInfo) );
      return *this;
    }

    DescriptorImageInfo& setSampler( Sampler sampler_ )
    {
      sampler = sampler_;
      return *this;
    }

    DescriptorImageInfo& setImageView( ImageView imageView_ )
    {
      imageView = imageView_;
      return *this;
    }

    DescriptorImageInfo& setImageLayout( ImageLayout imageLayout_ )
    {
      imageLayout = imageLayout_;
      return *this;
    }

    operator const VkDescriptorImageInfo&() const
    {
      return *reinterpret_cast<const VkDescriptorImageInfo*>(this);
    }

    bool operator==( DescriptorImageInfo const& rhs ) const
    {
      return ( sampler == rhs.sampler )
          && ( imageView == rhs.imageView )
          && ( imageLayout == rhs.imageLayout );
    }

    bool operator!=( DescriptorImageInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    Sampler sampler;
    ImageView imageView;
    ImageLayout imageLayout;
  };
  static_assert( sizeof( DescriptorImageInfo ) == sizeof( VkDescriptorImageInfo ), "struct and wrapper have different size!" );

  struct AttachmentReference
  {
    AttachmentReference( uint32_t attachment_ = 0, ImageLayout layout_ = ImageLayout::eUndefined )
      : attachment( attachment_ )
      , layout( layout_ )
    {
    }

    AttachmentReference( VkAttachmentReference const & rhs )
    {
      memcpy( this, &rhs, sizeof(AttachmentReference) );
    }

    AttachmentReference& operator=( VkAttachmentReference const & rhs )
    {
      memcpy( this, &rhs, sizeof(AttachmentReference) );
      return *this;
    }

    AttachmentReference& setAttachment( uint32_t attachment_ )
    {
      attachment = attachment_;
      return *this;
    }

    AttachmentReference& setLayout( ImageLayout layout_ )
    {
      layout = layout_;
      return *this;
    }

    operator const VkAttachmentReference&() const
    {
      return *reinterpret_cast<const VkAttachmentReference*>(this);
    }

    bool operator==( AttachmentReference const& rhs ) const
    {
      return ( attachment == rhs.attachment )
          && ( layout == rhs.layout );
    }

    bool operator!=( AttachmentReference const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t attachment;
    ImageLayout layout;
  };
  static_assert( sizeof( AttachmentReference ) == sizeof( VkAttachmentReference ), "struct and wrapper have different size!" );

  enum class AttachmentLoadOp
  {
    eLoad = VK_ATTACHMENT_LOAD_OP_LOAD,
    eClear = VK_ATTACHMENT_LOAD_OP_CLEAR,
    eDontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE
  };

  enum class AttachmentStoreOp
  {
    eStore = VK_ATTACHMENT_STORE_OP_STORE,
    eDontCare = VK_ATTACHMENT_STORE_OP_DONT_CARE
  };

  enum class ImageType
  {
    e1D = VK_IMAGE_TYPE_1D,
    e2D = VK_IMAGE_TYPE_2D,
    e3D = VK_IMAGE_TYPE_3D
  };

  enum class ImageTiling
  {
    eOptimal = VK_IMAGE_TILING_OPTIMAL,
    eLinear = VK_IMAGE_TILING_LINEAR
  };

  enum class ImageViewType
  {
    e1D = VK_IMAGE_VIEW_TYPE_1D,
    e2D = VK_IMAGE_VIEW_TYPE_2D,
    e3D = VK_IMAGE_VIEW_TYPE_3D,
    eCube = VK_IMAGE_VIEW_TYPE_CUBE,
    e1DArray = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    e2DArray = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    eCubeArray = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
  };

  enum class CommandBufferLevel
  {
    ePrimary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    eSecondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
  };

  enum class ComponentSwizzle
  {
    eIdentity = VK_COMPONENT_SWIZZLE_IDENTITY,
    eZero = VK_COMPONENT_SWIZZLE_ZERO,
    eOne = VK_COMPONENT_SWIZZLE_ONE,
    eR = VK_COMPONENT_SWIZZLE_R,
    eG = VK_COMPONENT_SWIZZLE_G,
    eB = VK_COMPONENT_SWIZZLE_B,
    eA = VK_COMPONENT_SWIZZLE_A
  };

  struct ComponentMapping
  {
    ComponentMapping( ComponentSwizzle r_ = ComponentSwizzle::eIdentity, ComponentSwizzle g_ = ComponentSwizzle::eIdentity, ComponentSwizzle b_ = ComponentSwizzle::eIdentity, ComponentSwizzle a_ = ComponentSwizzle::eIdentity )
      : r( r_ )
      , g( g_ )
      , b( b_ )
      , a( a_ )
    {
    }

    ComponentMapping( VkComponentMapping const & rhs )
    {
      memcpy( this, &rhs, sizeof(ComponentMapping) );
    }

    ComponentMapping& operator=( VkComponentMapping const & rhs )
    {
      memcpy( this, &rhs, sizeof(ComponentMapping) );
      return *this;
    }

    ComponentMapping& setR( ComponentSwizzle r_ )
    {
      r = r_;
      return *this;
    }

    ComponentMapping& setG( ComponentSwizzle g_ )
    {
      g = g_;
      return *this;
    }

    ComponentMapping& setB( ComponentSwizzle b_ )
    {
      b = b_;
      return *this;
    }

    ComponentMapping& setA( ComponentSwizzle a_ )
    {
      a = a_;
      return *this;
    }

    operator const VkComponentMapping&() const
    {
      return *reinterpret_cast<const VkComponentMapping*>(this);
    }

    bool operator==( ComponentMapping const& rhs ) const
    {
      return ( r == rhs.r )
          && ( g == rhs.g )
          && ( b == rhs.b )
          && ( a == rhs.a );
    }

    bool operator!=( ComponentMapping const& rhs ) const
    {
      return !operator==( rhs );
    }

    ComponentSwizzle r;
    ComponentSwizzle g;
    ComponentSwizzle b;
    ComponentSwizzle a;
  };
  static_assert( sizeof( ComponentMapping ) == sizeof( VkComponentMapping ), "struct and wrapper have different size!" );

  enum class DescriptorType
  {
    eSampler = VK_DESCRIPTOR_TYPE_SAMPLER,
    eCombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    eSampledImage = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    eStorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    eUniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    eStorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    eUniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    eStorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    eUniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    eStorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    eInputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
  };

  struct DescriptorPoolSize
  {
    DescriptorPoolSize( DescriptorType type_ = DescriptorType::eSampler, uint32_t descriptorCount_ = 0 )
      : type( type_ )
      , descriptorCount( descriptorCount_ )
    {
    }

    DescriptorPoolSize( VkDescriptorPoolSize const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorPoolSize) );
    }

    DescriptorPoolSize& operator=( VkDescriptorPoolSize const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorPoolSize) );
      return *this;
    }

    DescriptorPoolSize& setType( DescriptorType type_ )
    {
      type = type_;
      return *this;
    }

    DescriptorPoolSize& setDescriptorCount( uint32_t descriptorCount_ )
    {
      descriptorCount = descriptorCount_;
      return *this;
    }

    operator const VkDescriptorPoolSize&() const
    {
      return *reinterpret_cast<const VkDescriptorPoolSize*>(this);
    }

    bool operator==( DescriptorPoolSize const& rhs ) const
    {
      return ( type == rhs.type )
          && ( descriptorCount == rhs.descriptorCount );
    }

    bool operator!=( DescriptorPoolSize const& rhs ) const
    {
      return !operator==( rhs );
    }

    DescriptorType type;
    uint32_t descriptorCount;
  };
  static_assert( sizeof( DescriptorPoolSize ) == sizeof( VkDescriptorPoolSize ), "struct and wrapper have different size!" );

  enum class QueryType
  {
    eOcclusion = VK_QUERY_TYPE_OCCLUSION,
    ePipelineStatistics = VK_QUERY_TYPE_PIPELINE_STATISTICS,
    eTimestamp = VK_QUERY_TYPE_TIMESTAMP
  };

  enum class BorderColor
  {
    eFloatTransparentBlack = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
    eIntTransparentBlack = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
    eFloatOpaqueBlack = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
    eIntOpaqueBlack = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    eFloatOpaqueWhite = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    eIntOpaqueWhite = VK_BORDER_COLOR_INT_OPAQUE_WHITE
  };

  enum class PipelineBindPoint
  {
    eGraphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
    eCompute = VK_PIPELINE_BIND_POINT_COMPUTE
  };

  struct SubpassDescription
  {
    SubpassDescription( SubpassDescriptionFlags flags_ = SubpassDescriptionFlags(), PipelineBindPoint pipelineBindPoint_ = PipelineBindPoint::eGraphics, uint32_t inputAttachmentCount_ = 0, const AttachmentReference* pInputAttachments_ = nullptr, uint32_t colorAttachmentCount_ = 0, const AttachmentReference* pColorAttachments_ = nullptr, const AttachmentReference* pResolveAttachments_ = nullptr, const AttachmentReference* pDepthStencilAttachment_ = nullptr, uint32_t preserveAttachmentCount_ = 0, const uint32_t* pPreserveAttachments_ = nullptr )
      : flags( flags_ )
      , pipelineBindPoint( pipelineBindPoint_ )
      , inputAttachmentCount( inputAttachmentCount_ )
      , pInputAttachments( pInputAttachments_ )
      , colorAttachmentCount( colorAttachmentCount_ )
      , pColorAttachments( pColorAttachments_ )
      , pResolveAttachments( pResolveAttachments_ )
      , pDepthStencilAttachment( pDepthStencilAttachment_ )
      , preserveAttachmentCount( preserveAttachmentCount_ )
      , pPreserveAttachments( pPreserveAttachments_ )
    {
    }

    SubpassDescription( VkSubpassDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubpassDescription) );
    }

    SubpassDescription& operator=( VkSubpassDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubpassDescription) );
      return *this;
    }

    SubpassDescription& setFlags( SubpassDescriptionFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    SubpassDescription& setPipelineBindPoint( PipelineBindPoint pipelineBindPoint_ )
    {
      pipelineBindPoint = pipelineBindPoint_;
      return *this;
    }

    SubpassDescription& setInputAttachmentCount( uint32_t inputAttachmentCount_ )
    {
      inputAttachmentCount = inputAttachmentCount_;
      return *this;
    }

    SubpassDescription& setPInputAttachments( const AttachmentReference* pInputAttachments_ )
    {
      pInputAttachments = pInputAttachments_;
      return *this;
    }

    SubpassDescription& setColorAttachmentCount( uint32_t colorAttachmentCount_ )
    {
      colorAttachmentCount = colorAttachmentCount_;
      return *this;
    }

    SubpassDescription& setPColorAttachments( const AttachmentReference* pColorAttachments_ )
    {
      pColorAttachments = pColorAttachments_;
      return *this;
    }

    SubpassDescription& setPResolveAttachments( const AttachmentReference* pResolveAttachments_ )
    {
      pResolveAttachments = pResolveAttachments_;
      return *this;
    }

    SubpassDescription& setPDepthStencilAttachment( const AttachmentReference* pDepthStencilAttachment_ )
    {
      pDepthStencilAttachment = pDepthStencilAttachment_;
      return *this;
    }

    SubpassDescription& setPreserveAttachmentCount( uint32_t preserveAttachmentCount_ )
    {
      preserveAttachmentCount = preserveAttachmentCount_;
      return *this;
    }

    SubpassDescription& setPPreserveAttachments( const uint32_t* pPreserveAttachments_ )
    {
      pPreserveAttachments = pPreserveAttachments_;
      return *this;
    }

    operator const VkSubpassDescription&() const
    {
      return *reinterpret_cast<const VkSubpassDescription*>(this);
    }

    bool operator==( SubpassDescription const& rhs ) const
    {
      return ( flags == rhs.flags )
          && ( pipelineBindPoint == rhs.pipelineBindPoint )
          && ( inputAttachmentCount == rhs.inputAttachmentCount )
          && ( pInputAttachments == rhs.pInputAttachments )
          && ( colorAttachmentCount == rhs.colorAttachmentCount )
          && ( pColorAttachments == rhs.pColorAttachments )
          && ( pResolveAttachments == rhs.pResolveAttachments )
          && ( pDepthStencilAttachment == rhs.pDepthStencilAttachment )
          && ( preserveAttachmentCount == rhs.preserveAttachmentCount )
          && ( pPreserveAttachments == rhs.pPreserveAttachments );
    }

    bool operator!=( SubpassDescription const& rhs ) const
    {
      return !operator==( rhs );
    }

    SubpassDescriptionFlags flags;
    PipelineBindPoint pipelineBindPoint;
    uint32_t inputAttachmentCount;
    const AttachmentReference* pInputAttachments;
    uint32_t colorAttachmentCount;
    const AttachmentReference* pColorAttachments;
    const AttachmentReference* pResolveAttachments;
    const AttachmentReference* pDepthStencilAttachment;
    uint32_t preserveAttachmentCount;
    const uint32_t* pPreserveAttachments;
  };
  static_assert( sizeof( SubpassDescription ) == sizeof( VkSubpassDescription ), "struct and wrapper have different size!" );

  enum class PipelineCacheHeaderVersion
  {
    eOne = VK_PIPELINE_CACHE_HEADER_VERSION_ONE
  };

  enum class PrimitiveTopology
  {
    ePointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    eLineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    eLineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    eTriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    eTriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    eTriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    eLineListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
    eLineStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
    eTriangleListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
    eTriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
    ePatchList = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
  };

  enum class SharingMode
  {
    eExclusive = VK_SHARING_MODE_EXCLUSIVE,
    eConcurrent = VK_SHARING_MODE_CONCURRENT
  };

  enum class IndexType
  {
    eUint16 = VK_INDEX_TYPE_UINT16,
    eUint32 = VK_INDEX_TYPE_UINT32
  };

  enum class Filter
  {
    eNearest = VK_FILTER_NEAREST,
    eLinear = VK_FILTER_LINEAR,
    eCubicIMG = VK_FILTER_CUBIC_IMG
  };

  enum class SamplerMipmapMode
  {
    eNearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    eLinear = VK_SAMPLER_MIPMAP_MODE_LINEAR
  };

  enum class SamplerAddressMode
  {
    eRepeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    eMirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    eClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    eClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    eMirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
  };

  enum class CompareOp
  {
    eNever = VK_COMPARE_OP_NEVER,
    eLess = VK_COMPARE_OP_LESS,
    eEqual = VK_COMPARE_OP_EQUAL,
    eLessOrEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
    eGreater = VK_COMPARE_OP_GREATER,
    eNotEqual = VK_COMPARE_OP_NOT_EQUAL,
    eGreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
    eAlways = VK_COMPARE_OP_ALWAYS
  };

  enum class PolygonMode
  {
    eFill = VK_POLYGON_MODE_FILL,
    eLine = VK_POLYGON_MODE_LINE,
    ePoint = VK_POLYGON_MODE_POINT
  };

  enum class CullModeFlagBits
  {
    eNone = VK_CULL_MODE_NONE,
    eFront = VK_CULL_MODE_FRONT_BIT,
    eBack = VK_CULL_MODE_BACK_BIT,
    eFrontAndBack = VK_CULL_MODE_FRONT_AND_BACK
  };

  using CullModeFlags = Flags<CullModeFlagBits, VkCullModeFlags>;

  VULKAN_HPP_INLINE CullModeFlags operator|( CullModeFlagBits bit0, CullModeFlagBits bit1 )
  {
    return CullModeFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CullModeFlags operator~( CullModeFlagBits bits )
  {
    return ~( CullModeFlags( bits ) );
  }

  template <> struct FlagTraits<CullModeFlagBits>
  {
    enum
    {
      allFlags = VkFlags(CullModeFlagBits::eNone) | VkFlags(CullModeFlagBits::eFront) | VkFlags(CullModeFlagBits::eBack) | VkFlags(CullModeFlagBits::eFrontAndBack)
    };
  };

  enum class FrontFace
  {
    eCounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    eClockwise = VK_FRONT_FACE_CLOCKWISE
  };

  enum class BlendFactor
  {
    eZero = VK_BLEND_FACTOR_ZERO,
    eOne = VK_BLEND_FACTOR_ONE,
    eSrcColor = VK_BLEND_FACTOR_SRC_COLOR,
    eOneMinusSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    eDstColor = VK_BLEND_FACTOR_DST_COLOR,
    eOneMinusDstColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    eSrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA,
    eOneMinusSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    eDstAlpha = VK_BLEND_FACTOR_DST_ALPHA,
    eOneMinusDstAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    eConstantColor = VK_BLEND_FACTOR_CONSTANT_COLOR,
    eOneMinusConstantColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    eConstantAlpha = VK_BLEND_FACTOR_CONSTANT_ALPHA,
    eOneMinusConstantAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    eSrcAlphaSaturate = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    eSrc1Color = VK_BLEND_FACTOR_SRC1_COLOR,
    eOneMinusSrc1Color = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    eSrc1Alpha = VK_BLEND_FACTOR_SRC1_ALPHA,
    eOneMinusSrc1Alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
  };

  enum class BlendOp
  {
    eAdd = VK_BLEND_OP_ADD,
    eSubtract = VK_BLEND_OP_SUBTRACT,
    eReverseSubtract = VK_BLEND_OP_REVERSE_SUBTRACT,
    eMin = VK_BLEND_OP_MIN,
    eMax = VK_BLEND_OP_MAX
  };

  enum class StencilOp
  {
    eKeep = VK_STENCIL_OP_KEEP,
    eZero = VK_STENCIL_OP_ZERO,
    eReplace = VK_STENCIL_OP_REPLACE,
    eIncrementAndClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
    eDecrementAndClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
    eInvert = VK_STENCIL_OP_INVERT,
    eIncrementAndWrap = VK_STENCIL_OP_INCREMENT_AND_WRAP,
    eDecrementAndWrap = VK_STENCIL_OP_DECREMENT_AND_WRAP
  };

  struct StencilOpState
  {
    StencilOpState( StencilOp failOp_ = StencilOp::eKeep, StencilOp passOp_ = StencilOp::eKeep, StencilOp depthFailOp_ = StencilOp::eKeep, CompareOp compareOp_ = CompareOp::eNever, uint32_t compareMask_ = 0, uint32_t writeMask_ = 0, uint32_t reference_ = 0 )
      : failOp( failOp_ )
      , passOp( passOp_ )
      , depthFailOp( depthFailOp_ )
      , compareOp( compareOp_ )
      , compareMask( compareMask_ )
      , writeMask( writeMask_ )
      , reference( reference_ )
    {
    }

    StencilOpState( VkStencilOpState const & rhs )
    {
      memcpy( this, &rhs, sizeof(StencilOpState) );
    }

    StencilOpState& operator=( VkStencilOpState const & rhs )
    {
      memcpy( this, &rhs, sizeof(StencilOpState) );
      return *this;
    }

    StencilOpState& setFailOp( StencilOp failOp_ )
    {
      failOp = failOp_;
      return *this;
    }

    StencilOpState& setPassOp( StencilOp passOp_ )
    {
      passOp = passOp_;
      return *this;
    }

    StencilOpState& setDepthFailOp( StencilOp depthFailOp_ )
    {
      depthFailOp = depthFailOp_;
      return *this;
    }

    StencilOpState& setCompareOp( CompareOp compareOp_ )
    {
      compareOp = compareOp_;
      return *this;
    }

    StencilOpState& setCompareMask( uint32_t compareMask_ )
    {
      compareMask = compareMask_;
      return *this;
    }

    StencilOpState& setWriteMask( uint32_t writeMask_ )
    {
      writeMask = writeMask_;
      return *this;
    }

    StencilOpState& setReference( uint32_t reference_ )
    {
      reference = reference_;
      return *this;
    }

    operator const VkStencilOpState&() const
    {
      return *reinterpret_cast<const VkStencilOpState*>(this);
    }

    bool operator==( StencilOpState const& rhs ) const
    {
      return ( failOp == rhs.failOp )
          && ( passOp == rhs.passOp )
          && ( depthFailOp == rhs.depthFailOp )
          && ( compareOp == rhs.compareOp )
          && ( compareMask == rhs.compareMask )
          && ( writeMask == rhs.writeMask )
          && ( reference == rhs.reference );
    }

    bool operator!=( StencilOpState const& rhs ) const
    {
      return !operator==( rhs );
    }

    StencilOp failOp;
    StencilOp passOp;
    StencilOp depthFailOp;
    CompareOp compareOp;
    uint32_t compareMask;
    uint32_t writeMask;
    uint32_t reference;
  };
  static_assert( sizeof( StencilOpState ) == sizeof( VkStencilOpState ), "struct and wrapper have different size!" );

  enum class LogicOp
  {
    eClear = VK_LOGIC_OP_CLEAR,
    eAnd = VK_LOGIC_OP_AND,
    eAndReverse = VK_LOGIC_OP_AND_REVERSE,
    eCopy = VK_LOGIC_OP_COPY,
    eAndInverted = VK_LOGIC_OP_AND_INVERTED,
    eNoOp = VK_LOGIC_OP_NO_OP,
    eXor = VK_LOGIC_OP_XOR,
    eOr = VK_LOGIC_OP_OR,
    eNor = VK_LOGIC_OP_NOR,
    eEquivalent = VK_LOGIC_OP_EQUIVALENT,
    eInvert = VK_LOGIC_OP_INVERT,
    eOrReverse = VK_LOGIC_OP_OR_REVERSE,
    eCopyInverted = VK_LOGIC_OP_COPY_INVERTED,
    eOrInverted = VK_LOGIC_OP_OR_INVERTED,
    eNand = VK_LOGIC_OP_NAND,
    eSet = VK_LOGIC_OP_SET
  };

  enum class InternalAllocationType
  {
    eExecutable = VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE
  };

  enum class SystemAllocationScope
  {
    eCommand = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND,
    eObject = VK_SYSTEM_ALLOCATION_SCOPE_OBJECT,
    eCache = VK_SYSTEM_ALLOCATION_SCOPE_CACHE,
    eDevice = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE,
    eInstance = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE
  };

  enum class PhysicalDeviceType
  {
    eOther = VK_PHYSICAL_DEVICE_TYPE_OTHER,
    eIntegratedGpu = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    eDiscreteGpu = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    eVirtualGpu = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    eCpu = VK_PHYSICAL_DEVICE_TYPE_CPU
  };

  enum class VertexInputRate
  {
    eVertex = VK_VERTEX_INPUT_RATE_VERTEX,
    eInstance = VK_VERTEX_INPUT_RATE_INSTANCE
  };

  struct VertexInputBindingDescription
  {
    VertexInputBindingDescription( uint32_t binding_ = 0, uint32_t stride_ = 0, VertexInputRate inputRate_ = VertexInputRate::eVertex )
      : binding( binding_ )
      , stride( stride_ )
      , inputRate( inputRate_ )
    {
    }

    VertexInputBindingDescription( VkVertexInputBindingDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(VertexInputBindingDescription) );
    }

    VertexInputBindingDescription& operator=( VkVertexInputBindingDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(VertexInputBindingDescription) );
      return *this;
    }

    VertexInputBindingDescription& setBinding( uint32_t binding_ )
    {
      binding = binding_;
      return *this;
    }

    VertexInputBindingDescription& setStride( uint32_t stride_ )
    {
      stride = stride_;
      return *this;
    }

    VertexInputBindingDescription& setInputRate( VertexInputRate inputRate_ )
    {
      inputRate = inputRate_;
      return *this;
    }

    operator const VkVertexInputBindingDescription&() const
    {
      return *reinterpret_cast<const VkVertexInputBindingDescription*>(this);
    }

    bool operator==( VertexInputBindingDescription const& rhs ) const
    {
      return ( binding == rhs.binding )
          && ( stride == rhs.stride )
          && ( inputRate == rhs.inputRate );
    }

    bool operator!=( VertexInputBindingDescription const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t binding;
    uint32_t stride;
    VertexInputRate inputRate;
  };
  static_assert( sizeof( VertexInputBindingDescription ) == sizeof( VkVertexInputBindingDescription ), "struct and wrapper have different size!" );

  enum class Format
  {
    eUndefined = VK_FORMAT_UNDEFINED,
    eR4G4UnormPack8 = VK_FORMAT_R4G4_UNORM_PACK8,
    eR4G4B4A4UnormPack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
    eB4G4R4A4UnormPack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
    eR5G6B5UnormPack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
    eB5G6R5UnormPack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
    eR5G5B5A1UnormPack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
    eB5G5R5A1UnormPack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
    eA1R5G5B5UnormPack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
    eR8Unorm = VK_FORMAT_R8_UNORM,
    eR8Snorm = VK_FORMAT_R8_SNORM,
    eR8Uscaled = VK_FORMAT_R8_USCALED,
    eR8Sscaled = VK_FORMAT_R8_SSCALED,
    eR8Uint = VK_FORMAT_R8_UINT,
    eR8Sint = VK_FORMAT_R8_SINT,
    eR8Srgb = VK_FORMAT_R8_SRGB,
    eR8G8Unorm = VK_FORMAT_R8G8_UNORM,
    eR8G8Snorm = VK_FORMAT_R8G8_SNORM,
    eR8G8Uscaled = VK_FORMAT_R8G8_USCALED,
    eR8G8Sscaled = VK_FORMAT_R8G8_SSCALED,
    eR8G8Uint = VK_FORMAT_R8G8_UINT,
    eR8G8Sint = VK_FORMAT_R8G8_SINT,
    eR8G8Srgb = VK_FORMAT_R8G8_SRGB,
    eR8G8B8Unorm = VK_FORMAT_R8G8B8_UNORM,
    eR8G8B8Snorm = VK_FORMAT_R8G8B8_SNORM,
    eR8G8B8Uscaled = VK_FORMAT_R8G8B8_USCALED,
    eR8G8B8Sscaled = VK_FORMAT_R8G8B8_SSCALED,
    eR8G8B8Uint = VK_FORMAT_R8G8B8_UINT,
    eR8G8B8Sint = VK_FORMAT_R8G8B8_SINT,
    eR8G8B8Srgb = VK_FORMAT_R8G8B8_SRGB,
    eB8G8R8Unorm = VK_FORMAT_B8G8R8_UNORM,
    eB8G8R8Snorm = VK_FORMAT_B8G8R8_SNORM,
    eB8G8R8Uscaled = VK_FORMAT_B8G8R8_USCALED,
    eB8G8R8Sscaled = VK_FORMAT_B8G8R8_SSCALED,
    eB8G8R8Uint = VK_FORMAT_B8G8R8_UINT,
    eB8G8R8Sint = VK_FORMAT_B8G8R8_SINT,
    eB8G8R8Srgb = VK_FORMAT_B8G8R8_SRGB,
    eR8G8B8A8Unorm = VK_FORMAT_R8G8B8A8_UNORM,
    eR8G8B8A8Snorm = VK_FORMAT_R8G8B8A8_SNORM,
    eR8G8B8A8Uscaled = VK_FORMAT_R8G8B8A8_USCALED,
    eR8G8B8A8Sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
    eR8G8B8A8Uint = VK_FORMAT_R8G8B8A8_UINT,
    eR8G8B8A8Sint = VK_FORMAT_R8G8B8A8_SINT,
    eR8G8B8A8Srgb = VK_FORMAT_R8G8B8A8_SRGB,
    eB8G8R8A8Unorm = VK_FORMAT_B8G8R8A8_UNORM,
    eB8G8R8A8Snorm = VK_FORMAT_B8G8R8A8_SNORM,
    eB8G8R8A8Uscaled = VK_FORMAT_B8G8R8A8_USCALED,
    eB8G8R8A8Sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
    eB8G8R8A8Uint = VK_FORMAT_B8G8R8A8_UINT,
    eB8G8R8A8Sint = VK_FORMAT_B8G8R8A8_SINT,
    eB8G8R8A8Srgb = VK_FORMAT_B8G8R8A8_SRGB,
    eA8B8G8R8UnormPack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
    eA8B8G8R8SnormPack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
    eA8B8G8R8UscaledPack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
    eA8B8G8R8SscaledPack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
    eA8B8G8R8UintPack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
    eA8B8G8R8SintPack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
    eA8B8G8R8SrgbPack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
    eA2R10G10B10UnormPack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
    eA2R10G10B10SnormPack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
    eA2R10G10B10UscaledPack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
    eA2R10G10B10SscaledPack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
    eA2R10G10B10UintPack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
    eA2R10G10B10SintPack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
    eA2B10G10R10UnormPack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
    eA2B10G10R10SnormPack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
    eA2B10G10R10UscaledPack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
    eA2B10G10R10SscaledPack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
    eA2B10G10R10UintPack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
    eA2B10G10R10SintPack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
    eR16Unorm = VK_FORMAT_R16_UNORM,
    eR16Snorm = VK_FORMAT_R16_SNORM,
    eR16Uscaled = VK_FORMAT_R16_USCALED,
    eR16Sscaled = VK_FORMAT_R16_SSCALED,
    eR16Uint = VK_FORMAT_R16_UINT,
    eR16Sint = VK_FORMAT_R16_SINT,
    eR16Sfloat = VK_FORMAT_R16_SFLOAT,
    eR16G16Unorm = VK_FORMAT_R16G16_UNORM,
    eR16G16Snorm = VK_FORMAT_R16G16_SNORM,
    eR16G16Uscaled = VK_FORMAT_R16G16_USCALED,
    eR16G16Sscaled = VK_FORMAT_R16G16_SSCALED,
    eR16G16Uint = VK_FORMAT_R16G16_UINT,
    eR16G16Sint = VK_FORMAT_R16G16_SINT,
    eR16G16Sfloat = VK_FORMAT_R16G16_SFLOAT,
    eR16G16B16Unorm = VK_FORMAT_R16G16B16_UNORM,
    eR16G16B16Snorm = VK_FORMAT_R16G16B16_SNORM,
    eR16G16B16Uscaled = VK_FORMAT_R16G16B16_USCALED,
    eR16G16B16Sscaled = VK_FORMAT_R16G16B16_SSCALED,
    eR16G16B16Uint = VK_FORMAT_R16G16B16_UINT,
    eR16G16B16Sint = VK_FORMAT_R16G16B16_SINT,
    eR16G16B16Sfloat = VK_FORMAT_R16G16B16_SFLOAT,
    eR16G16B16A16Unorm = VK_FORMAT_R16G16B16A16_UNORM,
    eR16G16B16A16Snorm = VK_FORMAT_R16G16B16A16_SNORM,
    eR16G16B16A16Uscaled = VK_FORMAT_R16G16B16A16_USCALED,
    eR16G16B16A16Sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
    eR16G16B16A16Uint = VK_FORMAT_R16G16B16A16_UINT,
    eR16G16B16A16Sint = VK_FORMAT_R16G16B16A16_SINT,
    eR16G16B16A16Sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
    eR32Uint = VK_FORMAT_R32_UINT,
    eR32Sint = VK_FORMAT_R32_SINT,
    eR32Sfloat = VK_FORMAT_R32_SFLOAT,
    eR32G32Uint = VK_FORMAT_R32G32_UINT,
    eR32G32Sint = VK_FORMAT_R32G32_SINT,
    eR32G32Sfloat = VK_FORMAT_R32G32_SFLOAT,
    eR32G32B32Uint = VK_FORMAT_R32G32B32_UINT,
    eR32G32B32Sint = VK_FORMAT_R32G32B32_SINT,
    eR32G32B32Sfloat = VK_FORMAT_R32G32B32_SFLOAT,
    eR32G32B32A32Uint = VK_FORMAT_R32G32B32A32_UINT,
    eR32G32B32A32Sint = VK_FORMAT_R32G32B32A32_SINT,
    eR32G32B32A32Sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
    eR64Uint = VK_FORMAT_R64_UINT,
    eR64Sint = VK_FORMAT_R64_SINT,
    eR64Sfloat = VK_FORMAT_R64_SFLOAT,
    eR64G64Uint = VK_FORMAT_R64G64_UINT,
    eR64G64Sint = VK_FORMAT_R64G64_SINT,
    eR64G64Sfloat = VK_FORMAT_R64G64_SFLOAT,
    eR64G64B64Uint = VK_FORMAT_R64G64B64_UINT,
    eR64G64B64Sint = VK_FORMAT_R64G64B64_SINT,
    eR64G64B64Sfloat = VK_FORMAT_R64G64B64_SFLOAT,
    eR64G64B64A64Uint = VK_FORMAT_R64G64B64A64_UINT,
    eR64G64B64A64Sint = VK_FORMAT_R64G64B64A64_SINT,
    eR64G64B64A64Sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
    eB10G11R11UfloatPack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
    eE5B9G9R9UfloatPack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
    eD16Unorm = VK_FORMAT_D16_UNORM,
    eX8D24UnormPack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
    eD32Sfloat = VK_FORMAT_D32_SFLOAT,
    eS8Uint = VK_FORMAT_S8_UINT,
    eD16UnormS8Uint = VK_FORMAT_D16_UNORM_S8_UINT,
    eD24UnormS8Uint = VK_FORMAT_D24_UNORM_S8_UINT,
    eD32SfloatS8Uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
    eBc1RgbUnormBlock = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
    eBc1RgbSrgbBlock = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
    eBc1RgbaUnormBlock = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
    eBc1RgbaSrgbBlock = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
    eBc2UnormBlock = VK_FORMAT_BC2_UNORM_BLOCK,
    eBc2SrgbBlock = VK_FORMAT_BC2_SRGB_BLOCK,
    eBc3UnormBlock = VK_FORMAT_BC3_UNORM_BLOCK,
    eBc3SrgbBlock = VK_FORMAT_BC3_SRGB_BLOCK,
    eBc4UnormBlock = VK_FORMAT_BC4_UNORM_BLOCK,
    eBc4SnormBlock = VK_FORMAT_BC4_SNORM_BLOCK,
    eBc5UnormBlock = VK_FORMAT_BC5_UNORM_BLOCK,
    eBc5SnormBlock = VK_FORMAT_BC5_SNORM_BLOCK,
    eBc6HUfloatBlock = VK_FORMAT_BC6H_UFLOAT_BLOCK,
    eBc6HSfloatBlock = VK_FORMAT_BC6H_SFLOAT_BLOCK,
    eBc7UnormBlock = VK_FORMAT_BC7_UNORM_BLOCK,
    eBc7SrgbBlock = VK_FORMAT_BC7_SRGB_BLOCK,
    eEtc2R8G8B8UnormBlock = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
    eEtc2R8G8B8SrgbBlock = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
    eEtc2R8G8B8A1UnormBlock = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
    eEtc2R8G8B8A1SrgbBlock = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
    eEtc2R8G8B8A8UnormBlock = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
    eEtc2R8G8B8A8SrgbBlock = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
    eEacR11UnormBlock = VK_FORMAT_EAC_R11_UNORM_BLOCK,
    eEacR11SnormBlock = VK_FORMAT_EAC_R11_SNORM_BLOCK,
    eEacR11G11UnormBlock = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
    eEacR11G11SnormBlock = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
    eAstc4x4UnormBlock = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
    eAstc4x4SrgbBlock = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
    eAstc5x4UnormBlock = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
    eAstc5x4SrgbBlock = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
    eAstc5x5UnormBlock = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
    eAstc5x5SrgbBlock = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
    eAstc6x5UnormBlock = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
    eAstc6x5SrgbBlock = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
    eAstc6x6UnormBlock = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
    eAstc6x6SrgbBlock = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
    eAstc8x5UnormBlock = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
    eAstc8x5SrgbBlock = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
    eAstc8x6UnormBlock = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
    eAstc8x6SrgbBlock = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
    eAstc8x8UnormBlock = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
    eAstc8x8SrgbBlock = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
    eAstc10x5UnormBlock = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
    eAstc10x5SrgbBlock = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
    eAstc10x6UnormBlock = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
    eAstc10x6SrgbBlock = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
    eAstc10x8UnormBlock = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
    eAstc10x8SrgbBlock = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
    eAstc10x10UnormBlock = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
    eAstc10x10SrgbBlock = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
    eAstc12x10UnormBlock = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
    eAstc12x10SrgbBlock = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
    eAstc12x12UnormBlock = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
    eAstc12x12SrgbBlock = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
    ePvrtc12BppUnormBlockIMG = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
    ePvrtc14BppUnormBlockIMG = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
    ePvrtc22BppUnormBlockIMG = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
    ePvrtc24BppUnormBlockIMG = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
    ePvrtc12BppSrgbBlockIMG = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
    ePvrtc14BppSrgbBlockIMG = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
    ePvrtc22BppSrgbBlockIMG = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
    ePvrtc24BppSrgbBlockIMG = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG
  };

  struct VertexInputAttributeDescription
  {
    VertexInputAttributeDescription( uint32_t location_ = 0, uint32_t binding_ = 0, Format format_ = Format::eUndefined, uint32_t offset_ = 0 )
      : location( location_ )
      , binding( binding_ )
      , format( format_ )
      , offset( offset_ )
    {
    }

    VertexInputAttributeDescription( VkVertexInputAttributeDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(VertexInputAttributeDescription) );
    }

    VertexInputAttributeDescription& operator=( VkVertexInputAttributeDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(VertexInputAttributeDescription) );
      return *this;
    }

    VertexInputAttributeDescription& setLocation( uint32_t location_ )
    {
      location = location_;
      return *this;
    }

    VertexInputAttributeDescription& setBinding( uint32_t binding_ )
    {
      binding = binding_;
      return *this;
    }

    VertexInputAttributeDescription& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    VertexInputAttributeDescription& setOffset( uint32_t offset_ )
    {
      offset = offset_;
      return *this;
    }

    operator const VkVertexInputAttributeDescription&() const
    {
      return *reinterpret_cast<const VkVertexInputAttributeDescription*>(this);
    }

    bool operator==( VertexInputAttributeDescription const& rhs ) const
    {
      return ( location == rhs.location )
          && ( binding == rhs.binding )
          && ( format == rhs.format )
          && ( offset == rhs.offset );
    }

    bool operator!=( VertexInputAttributeDescription const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t location;
    uint32_t binding;
    Format format;
    uint32_t offset;
  };
  static_assert( sizeof( VertexInputAttributeDescription ) == sizeof( VkVertexInputAttributeDescription ), "struct and wrapper have different size!" );

  enum class StructureType
  {
    eApplicationInfo = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    eInstanceCreateInfo = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    eDeviceQueueCreateInfo = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    eDeviceCreateInfo = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    eSubmitInfo = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    eMemoryAllocateInfo = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    eMappedMemoryRange = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    eBindSparseInfo = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
    eFenceCreateInfo = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    eSemaphoreCreateInfo = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    eEventCreateInfo = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
    eQueryPoolCreateInfo = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
    eBufferCreateInfo = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    eBufferViewCreateInfo = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
    eImageCreateInfo = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    eImageViewCreateInfo = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    eShaderModuleCreateInfo = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    ePipelineCacheCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
    ePipelineShaderStageCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    ePipelineVertexInputStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    ePipelineInputAssemblyStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    ePipelineTessellationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    ePipelineViewportStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    ePipelineRasterizationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    ePipelineMultisampleStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    ePipelineDepthStencilStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    ePipelineColorBlendStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    ePipelineDynamicStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    eGraphicsPipelineCreateInfo = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    eComputePipelineCreateInfo = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    ePipelineLayoutCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    eSamplerCreateInfo = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    eDescriptorSetLayoutCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    eDescriptorPoolCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    eDescriptorSetAllocateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    eWriteDescriptorSet = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    eCopyDescriptorSet = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,
    eFramebufferCreateInfo = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    eRenderPassCreateInfo = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    eCommandPoolCreateInfo = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    eCommandBufferAllocateInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    eCommandBufferInheritanceInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
    eCommandBufferBeginInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    eRenderPassBeginInfo = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    eBufferMemoryBarrier = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    eImageMemoryBarrier = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    eMemoryBarrier = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
    eLoaderInstanceCreateInfo = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO,
    eLoaderDeviceCreateInfo = VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO,
    eSwapchainCreateInfoKHR = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    ePresentInfoKHR = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    eDisplayModeCreateInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
    eDisplaySurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
    eDisplayPresentInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR,
    eXlibSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
    eXcbSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
    eWaylandSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
    eMirSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR,
    eAndroidSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
    eWin32SurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    eDebugReportCallbackCreateInfoEXT = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
    ePipelineRasterizationStateRasterizationOrderAMD = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD,
    eDebugMarkerObjectNameInfoEXT = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,
    eDebugMarkerObjectTagInfoEXT = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT,
    eDebugMarkerMarkerInfoEXT = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,
    eDedicatedAllocationImageCreateInfoNV = VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV,
    eDedicatedAllocationBufferCreateInfoNV = VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV,
    eDedicatedAllocationMemoryAllocateInfoNV = VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV,
    eExternalMemoryImageCreateInfoNV = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_NV,
    eExportMemoryAllocateInfoNV = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_NV,
    eImportMemoryWin32HandleInfoNV = VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV,
    eExportMemoryWin32HandleInfoNV = VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV,
    eWin32KeyedMutexAcquireReleaseInfoNV = VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV,
    ePhysicalDeviceFeatures2KHR = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
    ePhysicalDeviceProperties2KHR = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR,
    eFormatProperties2KHR = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR,
    eImageFormatProperties2KHR = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2_KHR,
    ePhysicalDeviceImageFormatInfo2KHR = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2_KHR,
    eQueueFamilyProperties2KHR = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2_KHR,
    ePhysicalDeviceMemoryProperties2KHR = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2_KHR,
    eSparseImageFormatProperties2KHR = VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2_KHR,
    ePhysicalDeviceSparseImageFormatInfo2KHR = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SPARSE_IMAGE_FORMAT_INFO_2_KHR,
    eValidationFlagsEXT = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT,
    eViSurfaceCreateInfoNN = VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN,
    eObjectTableCreateInfoNVX = VK_STRUCTURE_TYPE_OBJECT_TABLE_CREATE_INFO_NVX,
    eIndirectCommandsLayoutCreateInfoNVX = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NVX,
    eCmdProcessCommandsInfoNVX = VK_STRUCTURE_TYPE_CMD_PROCESS_COMMANDS_INFO_NVX,
    eCmdReserveSpaceForCommandsInfoNVX = VK_STRUCTURE_TYPE_CMD_RESERVE_SPACE_FOR_COMMANDS_INFO_NVX,
    eDeviceGeneratedCommandsLimitsNVX = VK_STRUCTURE_TYPE_DEVICE_GENERATED_COMMANDS_LIMITS_NVX,
    eDeviceGeneratedCommandsFeaturesNVX = VK_STRUCTURE_TYPE_DEVICE_GENERATED_COMMANDS_FEATURES_NVX,
    eSurfaceCapabilities2EXT = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES2_EXT,
    eDisplayPowerInfoEXT = VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT,
    eDeviceEventInfoEXT = VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT,
    eDisplayEventInfoEXT = VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT,
    eSwapchainCounterCreateInfoEXT = VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT
  };

  struct ApplicationInfo
  {
    ApplicationInfo( const char* pApplicationName_ = nullptr, uint32_t applicationVersion_ = 0, const char* pEngineName_ = nullptr, uint32_t engineVersion_ = 0, uint32_t apiVersion_ = 0 )
      : sType( StructureType::eApplicationInfo )
      , pNext( nullptr )
      , pApplicationName( pApplicationName_ )
      , applicationVersion( applicationVersion_ )
      , pEngineName( pEngineName_ )
      , engineVersion( engineVersion_ )
      , apiVersion( apiVersion_ )
    {
    }

    ApplicationInfo( VkApplicationInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ApplicationInfo) );
    }

    ApplicationInfo& operator=( VkApplicationInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ApplicationInfo) );
      return *this;
    }

    ApplicationInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ApplicationInfo& setPApplicationName( const char* pApplicationName_ )
    {
      pApplicationName = pApplicationName_;
      return *this;
    }

    ApplicationInfo& setApplicationVersion( uint32_t applicationVersion_ )
    {
      applicationVersion = applicationVersion_;
      return *this;
    }

    ApplicationInfo& setPEngineName( const char* pEngineName_ )
    {
      pEngineName = pEngineName_;
      return *this;
    }

    ApplicationInfo& setEngineVersion( uint32_t engineVersion_ )
    {
      engineVersion = engineVersion_;
      return *this;
    }

    ApplicationInfo& setApiVersion( uint32_t apiVersion_ )
    {
      apiVersion = apiVersion_;
      return *this;
    }

    operator const VkApplicationInfo&() const
    {
      return *reinterpret_cast<const VkApplicationInfo*>(this);
    }

    bool operator==( ApplicationInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( pApplicationName == rhs.pApplicationName )
          && ( applicationVersion == rhs.applicationVersion )
          && ( pEngineName == rhs.pEngineName )
          && ( engineVersion == rhs.engineVersion )
          && ( apiVersion == rhs.apiVersion );
    }

    bool operator!=( ApplicationInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    const char* pApplicationName;
    uint32_t applicationVersion;
    const char* pEngineName;
    uint32_t engineVersion;
    uint32_t apiVersion;
  };
  static_assert( sizeof( ApplicationInfo ) == sizeof( VkApplicationInfo ), "struct and wrapper have different size!" );

  struct DeviceQueueCreateInfo
  {
    DeviceQueueCreateInfo( DeviceQueueCreateFlags flags_ = DeviceQueueCreateFlags(), uint32_t queueFamilyIndex_ = 0, uint32_t queueCount_ = 0, const float* pQueuePriorities_ = nullptr )
      : sType( StructureType::eDeviceQueueCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , queueFamilyIndex( queueFamilyIndex_ )
      , queueCount( queueCount_ )
      , pQueuePriorities( pQueuePriorities_ )
    {
    }

    DeviceQueueCreateInfo( VkDeviceQueueCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceQueueCreateInfo) );
    }

    DeviceQueueCreateInfo& operator=( VkDeviceQueueCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceQueueCreateInfo) );
      return *this;
    }

    DeviceQueueCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DeviceQueueCreateInfo& setFlags( DeviceQueueCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    DeviceQueueCreateInfo& setQueueFamilyIndex( uint32_t queueFamilyIndex_ )
    {
      queueFamilyIndex = queueFamilyIndex_;
      return *this;
    }

    DeviceQueueCreateInfo& setQueueCount( uint32_t queueCount_ )
    {
      queueCount = queueCount_;
      return *this;
    }

    DeviceQueueCreateInfo& setPQueuePriorities( const float* pQueuePriorities_ )
    {
      pQueuePriorities = pQueuePriorities_;
      return *this;
    }

    operator const VkDeviceQueueCreateInfo&() const
    {
      return *reinterpret_cast<const VkDeviceQueueCreateInfo*>(this);
    }

    bool operator==( DeviceQueueCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( queueFamilyIndex == rhs.queueFamilyIndex )
          && ( queueCount == rhs.queueCount )
          && ( pQueuePriorities == rhs.pQueuePriorities );
    }

    bool operator!=( DeviceQueueCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DeviceQueueCreateFlags flags;
    uint32_t queueFamilyIndex;
    uint32_t queueCount;
    const float* pQueuePriorities;
  };
  static_assert( sizeof( DeviceQueueCreateInfo ) == sizeof( VkDeviceQueueCreateInfo ), "struct and wrapper have different size!" );

  struct DeviceCreateInfo
  {
    DeviceCreateInfo( DeviceCreateFlags flags_ = DeviceCreateFlags(), uint32_t queueCreateInfoCount_ = 0, const DeviceQueueCreateInfo* pQueueCreateInfos_ = nullptr, uint32_t enabledLayerCount_ = 0, const char* const* ppEnabledLayerNames_ = nullptr, uint32_t enabledExtensionCount_ = 0, const char* const* ppEnabledExtensionNames_ = nullptr, const PhysicalDeviceFeatures* pEnabledFeatures_ = nullptr )
      : sType( StructureType::eDeviceCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , queueCreateInfoCount( queueCreateInfoCount_ )
      , pQueueCreateInfos( pQueueCreateInfos_ )
      , enabledLayerCount( enabledLayerCount_ )
      , ppEnabledLayerNames( ppEnabledLayerNames_ )
      , enabledExtensionCount( enabledExtensionCount_ )
      , ppEnabledExtensionNames( ppEnabledExtensionNames_ )
      , pEnabledFeatures( pEnabledFeatures_ )
    {
    }

    DeviceCreateInfo( VkDeviceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceCreateInfo) );
    }

    DeviceCreateInfo& operator=( VkDeviceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceCreateInfo) );
      return *this;
    }

    DeviceCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DeviceCreateInfo& setFlags( DeviceCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    DeviceCreateInfo& setQueueCreateInfoCount( uint32_t queueCreateInfoCount_ )
    {
      queueCreateInfoCount = queueCreateInfoCount_;
      return *this;
    }

    DeviceCreateInfo& setPQueueCreateInfos( const DeviceQueueCreateInfo* pQueueCreateInfos_ )
    {
      pQueueCreateInfos = pQueueCreateInfos_;
      return *this;
    }

    DeviceCreateInfo& setEnabledLayerCount( uint32_t enabledLayerCount_ )
    {
      enabledLayerCount = enabledLayerCount_;
      return *this;
    }

    DeviceCreateInfo& setPpEnabledLayerNames( const char* const* ppEnabledLayerNames_ )
    {
      ppEnabledLayerNames = ppEnabledLayerNames_;
      return *this;
    }

    DeviceCreateInfo& setEnabledExtensionCount( uint32_t enabledExtensionCount_ )
    {
      enabledExtensionCount = enabledExtensionCount_;
      return *this;
    }

    DeviceCreateInfo& setPpEnabledExtensionNames( const char* const* ppEnabledExtensionNames_ )
    {
      ppEnabledExtensionNames = ppEnabledExtensionNames_;
      return *this;
    }

    DeviceCreateInfo& setPEnabledFeatures( const PhysicalDeviceFeatures* pEnabledFeatures_ )
    {
      pEnabledFeatures = pEnabledFeatures_;
      return *this;
    }

    operator const VkDeviceCreateInfo&() const
    {
      return *reinterpret_cast<const VkDeviceCreateInfo*>(this);
    }

    bool operator==( DeviceCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( queueCreateInfoCount == rhs.queueCreateInfoCount )
          && ( pQueueCreateInfos == rhs.pQueueCreateInfos )
          && ( enabledLayerCount == rhs.enabledLayerCount )
          && ( ppEnabledLayerNames == rhs.ppEnabledLayerNames )
          && ( enabledExtensionCount == rhs.enabledExtensionCount )
          && ( ppEnabledExtensionNames == rhs.ppEnabledExtensionNames )
          && ( pEnabledFeatures == rhs.pEnabledFeatures );
    }

    bool operator!=( DeviceCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DeviceCreateFlags flags;
    uint32_t queueCreateInfoCount;
    const DeviceQueueCreateInfo* pQueueCreateInfos;
    uint32_t enabledLayerCount;
    const char* const* ppEnabledLayerNames;
    uint32_t enabledExtensionCount;
    const char* const* ppEnabledExtensionNames;
    const PhysicalDeviceFeatures* pEnabledFeatures;
  };
  static_assert( sizeof( DeviceCreateInfo ) == sizeof( VkDeviceCreateInfo ), "struct and wrapper have different size!" );

  struct InstanceCreateInfo
  {
    InstanceCreateInfo( InstanceCreateFlags flags_ = InstanceCreateFlags(), const ApplicationInfo* pApplicationInfo_ = nullptr, uint32_t enabledLayerCount_ = 0, const char* const* ppEnabledLayerNames_ = nullptr, uint32_t enabledExtensionCount_ = 0, const char* const* ppEnabledExtensionNames_ = nullptr )
      : sType( StructureType::eInstanceCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , pApplicationInfo( pApplicationInfo_ )
      , enabledLayerCount( enabledLayerCount_ )
      , ppEnabledLayerNames( ppEnabledLayerNames_ )
      , enabledExtensionCount( enabledExtensionCount_ )
      , ppEnabledExtensionNames( ppEnabledExtensionNames_ )
    {
    }

    InstanceCreateInfo( VkInstanceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(InstanceCreateInfo) );
    }

    InstanceCreateInfo& operator=( VkInstanceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(InstanceCreateInfo) );
      return *this;
    }

    InstanceCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    InstanceCreateInfo& setFlags( InstanceCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    InstanceCreateInfo& setPApplicationInfo( const ApplicationInfo* pApplicationInfo_ )
    {
      pApplicationInfo = pApplicationInfo_;
      return *this;
    }

    InstanceCreateInfo& setEnabledLayerCount( uint32_t enabledLayerCount_ )
    {
      enabledLayerCount = enabledLayerCount_;
      return *this;
    }

    InstanceCreateInfo& setPpEnabledLayerNames( const char* const* ppEnabledLayerNames_ )
    {
      ppEnabledLayerNames = ppEnabledLayerNames_;
      return *this;
    }

    InstanceCreateInfo& setEnabledExtensionCount( uint32_t enabledExtensionCount_ )
    {
      enabledExtensionCount = enabledExtensionCount_;
      return *this;
    }

    InstanceCreateInfo& setPpEnabledExtensionNames( const char* const* ppEnabledExtensionNames_ )
    {
      ppEnabledExtensionNames = ppEnabledExtensionNames_;
      return *this;
    }

    operator const VkInstanceCreateInfo&() const
    {
      return *reinterpret_cast<const VkInstanceCreateInfo*>(this);
    }

    bool operator==( InstanceCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( pApplicationInfo == rhs.pApplicationInfo )
          && ( enabledLayerCount == rhs.enabledLayerCount )
          && ( ppEnabledLayerNames == rhs.ppEnabledLayerNames )
          && ( enabledExtensionCount == rhs.enabledExtensionCount )
          && ( ppEnabledExtensionNames == rhs.ppEnabledExtensionNames );
    }

    bool operator!=( InstanceCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    InstanceCreateFlags flags;
    const ApplicationInfo* pApplicationInfo;
    uint32_t enabledLayerCount;
    const char* const* ppEnabledLayerNames;
    uint32_t enabledExtensionCount;
    const char* const* ppEnabledExtensionNames;
  };
  static_assert( sizeof( InstanceCreateInfo ) == sizeof( VkInstanceCreateInfo ), "struct and wrapper have different size!" );

  struct MemoryAllocateInfo
  {
    MemoryAllocateInfo( DeviceSize allocationSize_ = 0, uint32_t memoryTypeIndex_ = 0 )
      : sType( StructureType::eMemoryAllocateInfo )
      , pNext( nullptr )
      , allocationSize( allocationSize_ )
      , memoryTypeIndex( memoryTypeIndex_ )
    {
    }

    MemoryAllocateInfo( VkMemoryAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(MemoryAllocateInfo) );
    }

    MemoryAllocateInfo& operator=( VkMemoryAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(MemoryAllocateInfo) );
      return *this;
    }

    MemoryAllocateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    MemoryAllocateInfo& setAllocationSize( DeviceSize allocationSize_ )
    {
      allocationSize = allocationSize_;
      return *this;
    }

    MemoryAllocateInfo& setMemoryTypeIndex( uint32_t memoryTypeIndex_ )
    {
      memoryTypeIndex = memoryTypeIndex_;
      return *this;
    }

    operator const VkMemoryAllocateInfo&() const
    {
      return *reinterpret_cast<const VkMemoryAllocateInfo*>(this);
    }

    bool operator==( MemoryAllocateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( allocationSize == rhs.allocationSize )
          && ( memoryTypeIndex == rhs.memoryTypeIndex );
    }

    bool operator!=( MemoryAllocateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DeviceSize allocationSize;
    uint32_t memoryTypeIndex;
  };
  static_assert( sizeof( MemoryAllocateInfo ) == sizeof( VkMemoryAllocateInfo ), "struct and wrapper have different size!" );

  struct MappedMemoryRange
  {
    MappedMemoryRange( DeviceMemory memory_ = DeviceMemory(), DeviceSize offset_ = 0, DeviceSize size_ = 0 )
      : sType( StructureType::eMappedMemoryRange )
      , pNext( nullptr )
      , memory( memory_ )
      , offset( offset_ )
      , size( size_ )
    {
    }

    MappedMemoryRange( VkMappedMemoryRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(MappedMemoryRange) );
    }

    MappedMemoryRange& operator=( VkMappedMemoryRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(MappedMemoryRange) );
      return *this;
    }

    MappedMemoryRange& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    MappedMemoryRange& setMemory( DeviceMemory memory_ )
    {
      memory = memory_;
      return *this;
    }

    MappedMemoryRange& setOffset( DeviceSize offset_ )
    {
      offset = offset_;
      return *this;
    }

    MappedMemoryRange& setSize( DeviceSize size_ )
    {
      size = size_;
      return *this;
    }

    operator const VkMappedMemoryRange&() const
    {
      return *reinterpret_cast<const VkMappedMemoryRange*>(this);
    }

    bool operator==( MappedMemoryRange const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( memory == rhs.memory )
          && ( offset == rhs.offset )
          && ( size == rhs.size );
    }

    bool operator!=( MappedMemoryRange const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DeviceMemory memory;
    DeviceSize offset;
    DeviceSize size;
  };
  static_assert( sizeof( MappedMemoryRange ) == sizeof( VkMappedMemoryRange ), "struct and wrapper have different size!" );

  struct WriteDescriptorSet
  {
    WriteDescriptorSet( DescriptorSet dstSet_ = DescriptorSet(), uint32_t dstBinding_ = 0, uint32_t dstArrayElement_ = 0, uint32_t descriptorCount_ = 0, DescriptorType descriptorType_ = DescriptorType::eSampler, const DescriptorImageInfo* pImageInfo_ = nullptr, const DescriptorBufferInfo* pBufferInfo_ = nullptr, const BufferView* pTexelBufferView_ = nullptr )
      : sType( StructureType::eWriteDescriptorSet )
      , pNext( nullptr )
      , dstSet( dstSet_ )
      , dstBinding( dstBinding_ )
      , dstArrayElement( dstArrayElement_ )
      , descriptorCount( descriptorCount_ )
      , descriptorType( descriptorType_ )
      , pImageInfo( pImageInfo_ )
      , pBufferInfo( pBufferInfo_ )
      , pTexelBufferView( pTexelBufferView_ )
    {
    }

    WriteDescriptorSet( VkWriteDescriptorSet const & rhs )
    {
      memcpy( this, &rhs, sizeof(WriteDescriptorSet) );
    }

    WriteDescriptorSet& operator=( VkWriteDescriptorSet const & rhs )
    {
      memcpy( this, &rhs, sizeof(WriteDescriptorSet) );
      return *this;
    }

    WriteDescriptorSet& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    WriteDescriptorSet& setDstSet( DescriptorSet dstSet_ )
    {
      dstSet = dstSet_;
      return *this;
    }

    WriteDescriptorSet& setDstBinding( uint32_t dstBinding_ )
    {
      dstBinding = dstBinding_;
      return *this;
    }

    WriteDescriptorSet& setDstArrayElement( uint32_t dstArrayElement_ )
    {
      dstArrayElement = dstArrayElement_;
      return *this;
    }

    WriteDescriptorSet& setDescriptorCount( uint32_t descriptorCount_ )
    {
      descriptorCount = descriptorCount_;
      return *this;
    }

    WriteDescriptorSet& setDescriptorType( DescriptorType descriptorType_ )
    {
      descriptorType = descriptorType_;
      return *this;
    }

    WriteDescriptorSet& setPImageInfo( const DescriptorImageInfo* pImageInfo_ )
    {
      pImageInfo = pImageInfo_;
      return *this;
    }

    WriteDescriptorSet& setPBufferInfo( const DescriptorBufferInfo* pBufferInfo_ )
    {
      pBufferInfo = pBufferInfo_;
      return *this;
    }

    WriteDescriptorSet& setPTexelBufferView( const BufferView* pTexelBufferView_ )
    {
      pTexelBufferView = pTexelBufferView_;
      return *this;
    }

    operator const VkWriteDescriptorSet&() const
    {
      return *reinterpret_cast<const VkWriteDescriptorSet*>(this);
    }

    bool operator==( WriteDescriptorSet const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( dstSet == rhs.dstSet )
          && ( dstBinding == rhs.dstBinding )
          && ( dstArrayElement == rhs.dstArrayElement )
          && ( descriptorCount == rhs.descriptorCount )
          && ( descriptorType == rhs.descriptorType )
          && ( pImageInfo == rhs.pImageInfo )
          && ( pBufferInfo == rhs.pBufferInfo )
          && ( pTexelBufferView == rhs.pTexelBufferView );
    }

    bool operator!=( WriteDescriptorSet const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DescriptorSet dstSet;
    uint32_t dstBinding;
    uint32_t dstArrayElement;
    uint32_t descriptorCount;
    DescriptorType descriptorType;
    const DescriptorImageInfo* pImageInfo;
    const DescriptorBufferInfo* pBufferInfo;
    const BufferView* pTexelBufferView;
  };
  static_assert( sizeof( WriteDescriptorSet ) == sizeof( VkWriteDescriptorSet ), "struct and wrapper have different size!" );

  struct CopyDescriptorSet
  {
    CopyDescriptorSet( DescriptorSet srcSet_ = DescriptorSet(), uint32_t srcBinding_ = 0, uint32_t srcArrayElement_ = 0, DescriptorSet dstSet_ = DescriptorSet(), uint32_t dstBinding_ = 0, uint32_t dstArrayElement_ = 0, uint32_t descriptorCount_ = 0 )
      : sType( StructureType::eCopyDescriptorSet )
      , pNext( nullptr )
      , srcSet( srcSet_ )
      , srcBinding( srcBinding_ )
      , srcArrayElement( srcArrayElement_ )
      , dstSet( dstSet_ )
      , dstBinding( dstBinding_ )
      , dstArrayElement( dstArrayElement_ )
      , descriptorCount( descriptorCount_ )
    {
    }

    CopyDescriptorSet( VkCopyDescriptorSet const & rhs )
    {
      memcpy( this, &rhs, sizeof(CopyDescriptorSet) );
    }

    CopyDescriptorSet& operator=( VkCopyDescriptorSet const & rhs )
    {
      memcpy( this, &rhs, sizeof(CopyDescriptorSet) );
      return *this;
    }

    CopyDescriptorSet& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CopyDescriptorSet& setSrcSet( DescriptorSet srcSet_ )
    {
      srcSet = srcSet_;
      return *this;
    }

    CopyDescriptorSet& setSrcBinding( uint32_t srcBinding_ )
    {
      srcBinding = srcBinding_;
      return *this;
    }

    CopyDescriptorSet& setSrcArrayElement( uint32_t srcArrayElement_ )
    {
      srcArrayElement = srcArrayElement_;
      return *this;
    }

    CopyDescriptorSet& setDstSet( DescriptorSet dstSet_ )
    {
      dstSet = dstSet_;
      return *this;
    }

    CopyDescriptorSet& setDstBinding( uint32_t dstBinding_ )
    {
      dstBinding = dstBinding_;
      return *this;
    }

    CopyDescriptorSet& setDstArrayElement( uint32_t dstArrayElement_ )
    {
      dstArrayElement = dstArrayElement_;
      return *this;
    }

    CopyDescriptorSet& setDescriptorCount( uint32_t descriptorCount_ )
    {
      descriptorCount = descriptorCount_;
      return *this;
    }

    operator const VkCopyDescriptorSet&() const
    {
      return *reinterpret_cast<const VkCopyDescriptorSet*>(this);
    }

    bool operator==( CopyDescriptorSet const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( srcSet == rhs.srcSet )
          && ( srcBinding == rhs.srcBinding )
          && ( srcArrayElement == rhs.srcArrayElement )
          && ( dstSet == rhs.dstSet )
          && ( dstBinding == rhs.dstBinding )
          && ( dstArrayElement == rhs.dstArrayElement )
          && ( descriptorCount == rhs.descriptorCount );
    }

    bool operator!=( CopyDescriptorSet const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DescriptorSet srcSet;
    uint32_t srcBinding;
    uint32_t srcArrayElement;
    DescriptorSet dstSet;
    uint32_t dstBinding;
    uint32_t dstArrayElement;
    uint32_t descriptorCount;
  };
  static_assert( sizeof( CopyDescriptorSet ) == sizeof( VkCopyDescriptorSet ), "struct and wrapper have different size!" );

  struct BufferViewCreateInfo
  {
    BufferViewCreateInfo( BufferViewCreateFlags flags_ = BufferViewCreateFlags(), Buffer buffer_ = Buffer(), Format format_ = Format::eUndefined, DeviceSize offset_ = 0, DeviceSize range_ = 0 )
      : sType( StructureType::eBufferViewCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , buffer( buffer_ )
      , format( format_ )
      , offset( offset_ )
      , range( range_ )
    {
    }

    BufferViewCreateInfo( VkBufferViewCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferViewCreateInfo) );
    }

    BufferViewCreateInfo& operator=( VkBufferViewCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferViewCreateInfo) );
      return *this;
    }

    BufferViewCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    BufferViewCreateInfo& setFlags( BufferViewCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    BufferViewCreateInfo& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    BufferViewCreateInfo& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    BufferViewCreateInfo& setOffset( DeviceSize offset_ )
    {
      offset = offset_;
      return *this;
    }

    BufferViewCreateInfo& setRange( DeviceSize range_ )
    {
      range = range_;
      return *this;
    }

    operator const VkBufferViewCreateInfo&() const
    {
      return *reinterpret_cast<const VkBufferViewCreateInfo*>(this);
    }

    bool operator==( BufferViewCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( buffer == rhs.buffer )
          && ( format == rhs.format )
          && ( offset == rhs.offset )
          && ( range == rhs.range );
    }

    bool operator!=( BufferViewCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    BufferViewCreateFlags flags;
    Buffer buffer;
    Format format;
    DeviceSize offset;
    DeviceSize range;
  };
  static_assert( sizeof( BufferViewCreateInfo ) == sizeof( VkBufferViewCreateInfo ), "struct and wrapper have different size!" );

  struct ShaderModuleCreateInfo
  {
    ShaderModuleCreateInfo( ShaderModuleCreateFlags flags_ = ShaderModuleCreateFlags(), size_t codeSize_ = 0, const uint32_t* pCode_ = nullptr )
      : sType( StructureType::eShaderModuleCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , codeSize( codeSize_ )
      , pCode( pCode_ )
    {
    }

    ShaderModuleCreateInfo( VkShaderModuleCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ShaderModuleCreateInfo) );
    }

    ShaderModuleCreateInfo& operator=( VkShaderModuleCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ShaderModuleCreateInfo) );
      return *this;
    }

    ShaderModuleCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ShaderModuleCreateInfo& setFlags( ShaderModuleCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    ShaderModuleCreateInfo& setCodeSize( size_t codeSize_ )
    {
      codeSize = codeSize_;
      return *this;
    }

    ShaderModuleCreateInfo& setPCode( const uint32_t* pCode_ )
    {
      pCode = pCode_;
      return *this;
    }

    operator const VkShaderModuleCreateInfo&() const
    {
      return *reinterpret_cast<const VkShaderModuleCreateInfo*>(this);
    }

    bool operator==( ShaderModuleCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( codeSize == rhs.codeSize )
          && ( pCode == rhs.pCode );
    }

    bool operator!=( ShaderModuleCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ShaderModuleCreateFlags flags;
    size_t codeSize;
    const uint32_t* pCode;
  };
  static_assert( sizeof( ShaderModuleCreateInfo ) == sizeof( VkShaderModuleCreateInfo ), "struct and wrapper have different size!" );

  struct DescriptorSetAllocateInfo
  {
    DescriptorSetAllocateInfo( DescriptorPool descriptorPool_ = DescriptorPool(), uint32_t descriptorSetCount_ = 0, const DescriptorSetLayout* pSetLayouts_ = nullptr )
      : sType( StructureType::eDescriptorSetAllocateInfo )
      , pNext( nullptr )
      , descriptorPool( descriptorPool_ )
      , descriptorSetCount( descriptorSetCount_ )
      , pSetLayouts( pSetLayouts_ )
    {
    }

    DescriptorSetAllocateInfo( VkDescriptorSetAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetAllocateInfo) );
    }

    DescriptorSetAllocateInfo& operator=( VkDescriptorSetAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetAllocateInfo) );
      return *this;
    }

    DescriptorSetAllocateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DescriptorSetAllocateInfo& setDescriptorPool( DescriptorPool descriptorPool_ )
    {
      descriptorPool = descriptorPool_;
      return *this;
    }

    DescriptorSetAllocateInfo& setDescriptorSetCount( uint32_t descriptorSetCount_ )
    {
      descriptorSetCount = descriptorSetCount_;
      return *this;
    }

    DescriptorSetAllocateInfo& setPSetLayouts( const DescriptorSetLayout* pSetLayouts_ )
    {
      pSetLayouts = pSetLayouts_;
      return *this;
    }

    operator const VkDescriptorSetAllocateInfo&() const
    {
      return *reinterpret_cast<const VkDescriptorSetAllocateInfo*>(this);
    }

    bool operator==( DescriptorSetAllocateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( descriptorPool == rhs.descriptorPool )
          && ( descriptorSetCount == rhs.descriptorSetCount )
          && ( pSetLayouts == rhs.pSetLayouts );
    }

    bool operator!=( DescriptorSetAllocateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DescriptorPool descriptorPool;
    uint32_t descriptorSetCount;
    const DescriptorSetLayout* pSetLayouts;
  };
  static_assert( sizeof( DescriptorSetAllocateInfo ) == sizeof( VkDescriptorSetAllocateInfo ), "struct and wrapper have different size!" );

  struct PipelineVertexInputStateCreateInfo
  {
    PipelineVertexInputStateCreateInfo( PipelineVertexInputStateCreateFlags flags_ = PipelineVertexInputStateCreateFlags(), uint32_t vertexBindingDescriptionCount_ = 0, const VertexInputBindingDescription* pVertexBindingDescriptions_ = nullptr, uint32_t vertexAttributeDescriptionCount_ = 0, const VertexInputAttributeDescription* pVertexAttributeDescriptions_ = nullptr )
      : sType( StructureType::ePipelineVertexInputStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , vertexBindingDescriptionCount( vertexBindingDescriptionCount_ )
      , pVertexBindingDescriptions( pVertexBindingDescriptions_ )
      , vertexAttributeDescriptionCount( vertexAttributeDescriptionCount_ )
      , pVertexAttributeDescriptions( pVertexAttributeDescriptions_ )
    {
    }

    PipelineVertexInputStateCreateInfo( VkPipelineVertexInputStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineVertexInputStateCreateInfo) );
    }

    PipelineVertexInputStateCreateInfo& operator=( VkPipelineVertexInputStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineVertexInputStateCreateInfo) );
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setFlags( PipelineVertexInputStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setVertexBindingDescriptionCount( uint32_t vertexBindingDescriptionCount_ )
    {
      vertexBindingDescriptionCount = vertexBindingDescriptionCount_;
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setPVertexBindingDescriptions( const VertexInputBindingDescription* pVertexBindingDescriptions_ )
    {
      pVertexBindingDescriptions = pVertexBindingDescriptions_;
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setVertexAttributeDescriptionCount( uint32_t vertexAttributeDescriptionCount_ )
    {
      vertexAttributeDescriptionCount = vertexAttributeDescriptionCount_;
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setPVertexAttributeDescriptions( const VertexInputAttributeDescription* pVertexAttributeDescriptions_ )
    {
      pVertexAttributeDescriptions = pVertexAttributeDescriptions_;
      return *this;
    }

    operator const VkPipelineVertexInputStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>(this);
    }

    bool operator==( PipelineVertexInputStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( vertexBindingDescriptionCount == rhs.vertexBindingDescriptionCount )
          && ( pVertexBindingDescriptions == rhs.pVertexBindingDescriptions )
          && ( vertexAttributeDescriptionCount == rhs.vertexAttributeDescriptionCount )
          && ( pVertexAttributeDescriptions == rhs.pVertexAttributeDescriptions );
    }

    bool operator!=( PipelineVertexInputStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineVertexInputStateCreateFlags flags;
    uint32_t vertexBindingDescriptionCount;
    const VertexInputBindingDescription* pVertexBindingDescriptions;
    uint32_t vertexAttributeDescriptionCount;
    const VertexInputAttributeDescription* pVertexAttributeDescriptions;
  };
  static_assert( sizeof( PipelineVertexInputStateCreateInfo ) == sizeof( VkPipelineVertexInputStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineInputAssemblyStateCreateInfo
  {
    PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateFlags flags_ = PipelineInputAssemblyStateCreateFlags(), PrimitiveTopology topology_ = PrimitiveTopology::ePointList, Bool32 primitiveRestartEnable_ = 0 )
      : sType( StructureType::ePipelineInputAssemblyStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , topology( topology_ )
      , primitiveRestartEnable( primitiveRestartEnable_ )
    {
    }

    PipelineInputAssemblyStateCreateInfo( VkPipelineInputAssemblyStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineInputAssemblyStateCreateInfo) );
    }

    PipelineInputAssemblyStateCreateInfo& operator=( VkPipelineInputAssemblyStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineInputAssemblyStateCreateInfo) );
      return *this;
    }

    PipelineInputAssemblyStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineInputAssemblyStateCreateInfo& setFlags( PipelineInputAssemblyStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineInputAssemblyStateCreateInfo& setTopology( PrimitiveTopology topology_ )
    {
      topology = topology_;
      return *this;
    }

    PipelineInputAssemblyStateCreateInfo& setPrimitiveRestartEnable( Bool32 primitiveRestartEnable_ )
    {
      primitiveRestartEnable = primitiveRestartEnable_;
      return *this;
    }

    operator const VkPipelineInputAssemblyStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo*>(this);
    }

    bool operator==( PipelineInputAssemblyStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( topology == rhs.topology )
          && ( primitiveRestartEnable == rhs.primitiveRestartEnable );
    }

    bool operator!=( PipelineInputAssemblyStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineInputAssemblyStateCreateFlags flags;
    PrimitiveTopology topology;
    Bool32 primitiveRestartEnable;
  };
  static_assert( sizeof( PipelineInputAssemblyStateCreateInfo ) == sizeof( VkPipelineInputAssemblyStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineTessellationStateCreateInfo
  {
    PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateFlags flags_ = PipelineTessellationStateCreateFlags(), uint32_t patchControlPoints_ = 0 )
      : sType( StructureType::ePipelineTessellationStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , patchControlPoints( patchControlPoints_ )
    {
    }

    PipelineTessellationStateCreateInfo( VkPipelineTessellationStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineTessellationStateCreateInfo) );
    }

    PipelineTessellationStateCreateInfo& operator=( VkPipelineTessellationStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineTessellationStateCreateInfo) );
      return *this;
    }

    PipelineTessellationStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineTessellationStateCreateInfo& setFlags( PipelineTessellationStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineTessellationStateCreateInfo& setPatchControlPoints( uint32_t patchControlPoints_ )
    {
      patchControlPoints = patchControlPoints_;
      return *this;
    }

    operator const VkPipelineTessellationStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>(this);
    }

    bool operator==( PipelineTessellationStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( patchControlPoints == rhs.patchControlPoints );
    }

    bool operator!=( PipelineTessellationStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineTessellationStateCreateFlags flags;
    uint32_t patchControlPoints;
  };
  static_assert( sizeof( PipelineTessellationStateCreateInfo ) == sizeof( VkPipelineTessellationStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineViewportStateCreateInfo
  {
    PipelineViewportStateCreateInfo( PipelineViewportStateCreateFlags flags_ = PipelineViewportStateCreateFlags(), uint32_t viewportCount_ = 0, const Viewport* pViewports_ = nullptr, uint32_t scissorCount_ = 0, const Rect2D* pScissors_ = nullptr )
      : sType( StructureType::ePipelineViewportStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , viewportCount( viewportCount_ )
      , pViewports( pViewports_ )
      , scissorCount( scissorCount_ )
      , pScissors( pScissors_ )
    {
    }

    PipelineViewportStateCreateInfo( VkPipelineViewportStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineViewportStateCreateInfo) );
    }

    PipelineViewportStateCreateInfo& operator=( VkPipelineViewportStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineViewportStateCreateInfo) );
      return *this;
    }

    PipelineViewportStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineViewportStateCreateInfo& setFlags( PipelineViewportStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineViewportStateCreateInfo& setViewportCount( uint32_t viewportCount_ )
    {
      viewportCount = viewportCount_;
      return *this;
    }

    PipelineViewportStateCreateInfo& setPViewports( const Viewport* pViewports_ )
    {
      pViewports = pViewports_;
      return *this;
    }

    PipelineViewportStateCreateInfo& setScissorCount( uint32_t scissorCount_ )
    {
      scissorCount = scissorCount_;
      return *this;
    }

    PipelineViewportStateCreateInfo& setPScissors( const Rect2D* pScissors_ )
    {
      pScissors = pScissors_;
      return *this;
    }

    operator const VkPipelineViewportStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineViewportStateCreateInfo*>(this);
    }

    bool operator==( PipelineViewportStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( viewportCount == rhs.viewportCount )
          && ( pViewports == rhs.pViewports )
          && ( scissorCount == rhs.scissorCount )
          && ( pScissors == rhs.pScissors );
    }

    bool operator!=( PipelineViewportStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineViewportStateCreateFlags flags;
    uint32_t viewportCount;
    const Viewport* pViewports;
    uint32_t scissorCount;
    const Rect2D* pScissors;
  };
  static_assert( sizeof( PipelineViewportStateCreateInfo ) == sizeof( VkPipelineViewportStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineRasterizationStateCreateInfo
  {
    PipelineRasterizationStateCreateInfo( PipelineRasterizationStateCreateFlags flags_ = PipelineRasterizationStateCreateFlags(), Bool32 depthClampEnable_ = 0, Bool32 rasterizerDiscardEnable_ = 0, PolygonMode polygonMode_ = PolygonMode::eFill, CullModeFlags cullMode_ = CullModeFlags(), FrontFace frontFace_ = FrontFace::eCounterClockwise, Bool32 depthBiasEnable_ = 0, float depthBiasConstantFactor_ = 0, float depthBiasClamp_ = 0, float depthBiasSlopeFactor_ = 0, float lineWidth_ = 0 )
      : sType( StructureType::ePipelineRasterizationStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , depthClampEnable( depthClampEnable_ )
      , rasterizerDiscardEnable( rasterizerDiscardEnable_ )
      , polygonMode( polygonMode_ )
      , cullMode( cullMode_ )
      , frontFace( frontFace_ )
      , depthBiasEnable( depthBiasEnable_ )
      , depthBiasConstantFactor( depthBiasConstantFactor_ )
      , depthBiasClamp( depthBiasClamp_ )
      , depthBiasSlopeFactor( depthBiasSlopeFactor_ )
      , lineWidth( lineWidth_ )
    {
    }

    PipelineRasterizationStateCreateInfo( VkPipelineRasterizationStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineRasterizationStateCreateInfo) );
    }

    PipelineRasterizationStateCreateInfo& operator=( VkPipelineRasterizationStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineRasterizationStateCreateInfo) );
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setFlags( PipelineRasterizationStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setDepthClampEnable( Bool32 depthClampEnable_ )
    {
      depthClampEnable = depthClampEnable_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setRasterizerDiscardEnable( Bool32 rasterizerDiscardEnable_ )
    {
      rasterizerDiscardEnable = rasterizerDiscardEnable_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setPolygonMode( PolygonMode polygonMode_ )
    {
      polygonMode = polygonMode_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setCullMode( CullModeFlags cullMode_ )
    {
      cullMode = cullMode_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setFrontFace( FrontFace frontFace_ )
    {
      frontFace = frontFace_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setDepthBiasEnable( Bool32 depthBiasEnable_ )
    {
      depthBiasEnable = depthBiasEnable_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setDepthBiasConstantFactor( float depthBiasConstantFactor_ )
    {
      depthBiasConstantFactor = depthBiasConstantFactor_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setDepthBiasClamp( float depthBiasClamp_ )
    {
      depthBiasClamp = depthBiasClamp_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setDepthBiasSlopeFactor( float depthBiasSlopeFactor_ )
    {
      depthBiasSlopeFactor = depthBiasSlopeFactor_;
      return *this;
    }

    PipelineRasterizationStateCreateInfo& setLineWidth( float lineWidth_ )
    {
      lineWidth = lineWidth_;
      return *this;
    }

    operator const VkPipelineRasterizationStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>(this);
    }

    bool operator==( PipelineRasterizationStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( depthClampEnable == rhs.depthClampEnable )
          && ( rasterizerDiscardEnable == rhs.rasterizerDiscardEnable )
          && ( polygonMode == rhs.polygonMode )
          && ( cullMode == rhs.cullMode )
          && ( frontFace == rhs.frontFace )
          && ( depthBiasEnable == rhs.depthBiasEnable )
          && ( depthBiasConstantFactor == rhs.depthBiasConstantFactor )
          && ( depthBiasClamp == rhs.depthBiasClamp )
          && ( depthBiasSlopeFactor == rhs.depthBiasSlopeFactor )
          && ( lineWidth == rhs.lineWidth );
    }

    bool operator!=( PipelineRasterizationStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineRasterizationStateCreateFlags flags;
    Bool32 depthClampEnable;
    Bool32 rasterizerDiscardEnable;
    PolygonMode polygonMode;
    CullModeFlags cullMode;
    FrontFace frontFace;
    Bool32 depthBiasEnable;
    float depthBiasConstantFactor;
    float depthBiasClamp;
    float depthBiasSlopeFactor;
    float lineWidth;
  };
  static_assert( sizeof( PipelineRasterizationStateCreateInfo ) == sizeof( VkPipelineRasterizationStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineDepthStencilStateCreateInfo
  {
    PipelineDepthStencilStateCreateInfo( PipelineDepthStencilStateCreateFlags flags_ = PipelineDepthStencilStateCreateFlags(), Bool32 depthTestEnable_ = 0, Bool32 depthWriteEnable_ = 0, CompareOp depthCompareOp_ = CompareOp::eNever, Bool32 depthBoundsTestEnable_ = 0, Bool32 stencilTestEnable_ = 0, StencilOpState front_ = StencilOpState(), StencilOpState back_ = StencilOpState(), float minDepthBounds_ = 0, float maxDepthBounds_ = 0 )
      : sType( StructureType::ePipelineDepthStencilStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , depthTestEnable( depthTestEnable_ )
      , depthWriteEnable( depthWriteEnable_ )
      , depthCompareOp( depthCompareOp_ )
      , depthBoundsTestEnable( depthBoundsTestEnable_ )
      , stencilTestEnable( stencilTestEnable_ )
      , front( front_ )
      , back( back_ )
      , minDepthBounds( minDepthBounds_ )
      , maxDepthBounds( maxDepthBounds_ )
    {
    }

    PipelineDepthStencilStateCreateInfo( VkPipelineDepthStencilStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineDepthStencilStateCreateInfo) );
    }

    PipelineDepthStencilStateCreateInfo& operator=( VkPipelineDepthStencilStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineDepthStencilStateCreateInfo) );
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setFlags( PipelineDepthStencilStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setDepthTestEnable( Bool32 depthTestEnable_ )
    {
      depthTestEnable = depthTestEnable_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setDepthWriteEnable( Bool32 depthWriteEnable_ )
    {
      depthWriteEnable = depthWriteEnable_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setDepthCompareOp( CompareOp depthCompareOp_ )
    {
      depthCompareOp = depthCompareOp_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setDepthBoundsTestEnable( Bool32 depthBoundsTestEnable_ )
    {
      depthBoundsTestEnable = depthBoundsTestEnable_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setStencilTestEnable( Bool32 stencilTestEnable_ )
    {
      stencilTestEnable = stencilTestEnable_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setFront( StencilOpState front_ )
    {
      front = front_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setBack( StencilOpState back_ )
    {
      back = back_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setMinDepthBounds( float minDepthBounds_ )
    {
      minDepthBounds = minDepthBounds_;
      return *this;
    }

    PipelineDepthStencilStateCreateInfo& setMaxDepthBounds( float maxDepthBounds_ )
    {
      maxDepthBounds = maxDepthBounds_;
      return *this;
    }

    operator const VkPipelineDepthStencilStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>(this);
    }

    bool operator==( PipelineDepthStencilStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( depthTestEnable == rhs.depthTestEnable )
          && ( depthWriteEnable == rhs.depthWriteEnable )
          && ( depthCompareOp == rhs.depthCompareOp )
          && ( depthBoundsTestEnable == rhs.depthBoundsTestEnable )
          && ( stencilTestEnable == rhs.stencilTestEnable )
          && ( front == rhs.front )
          && ( back == rhs.back )
          && ( minDepthBounds == rhs.minDepthBounds )
          && ( maxDepthBounds == rhs.maxDepthBounds );
    }

    bool operator!=( PipelineDepthStencilStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineDepthStencilStateCreateFlags flags;
    Bool32 depthTestEnable;
    Bool32 depthWriteEnable;
    CompareOp depthCompareOp;
    Bool32 depthBoundsTestEnable;
    Bool32 stencilTestEnable;
    StencilOpState front;
    StencilOpState back;
    float minDepthBounds;
    float maxDepthBounds;
  };
  static_assert( sizeof( PipelineDepthStencilStateCreateInfo ) == sizeof( VkPipelineDepthStencilStateCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineCacheCreateInfo
  {
    PipelineCacheCreateInfo( PipelineCacheCreateFlags flags_ = PipelineCacheCreateFlags(), size_t initialDataSize_ = 0, const void* pInitialData_ = nullptr )
      : sType( StructureType::ePipelineCacheCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , initialDataSize( initialDataSize_ )
      , pInitialData( pInitialData_ )
    {
    }

    PipelineCacheCreateInfo( VkPipelineCacheCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineCacheCreateInfo) );
    }

    PipelineCacheCreateInfo& operator=( VkPipelineCacheCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineCacheCreateInfo) );
      return *this;
    }

    PipelineCacheCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineCacheCreateInfo& setFlags( PipelineCacheCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineCacheCreateInfo& setInitialDataSize( size_t initialDataSize_ )
    {
      initialDataSize = initialDataSize_;
      return *this;
    }

    PipelineCacheCreateInfo& setPInitialData( const void* pInitialData_ )
    {
      pInitialData = pInitialData_;
      return *this;
    }

    operator const VkPipelineCacheCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineCacheCreateInfo*>(this);
    }

    bool operator==( PipelineCacheCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( initialDataSize == rhs.initialDataSize )
          && ( pInitialData == rhs.pInitialData );
    }

    bool operator!=( PipelineCacheCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineCacheCreateFlags flags;
    size_t initialDataSize;
    const void* pInitialData;
  };
  static_assert( sizeof( PipelineCacheCreateInfo ) == sizeof( VkPipelineCacheCreateInfo ), "struct and wrapper have different size!" );

  struct SamplerCreateInfo
  {
    SamplerCreateInfo( SamplerCreateFlags flags_ = SamplerCreateFlags(), Filter magFilter_ = Filter::eNearest, Filter minFilter_ = Filter::eNearest, SamplerMipmapMode mipmapMode_ = SamplerMipmapMode::eNearest, SamplerAddressMode addressModeU_ = SamplerAddressMode::eRepeat, SamplerAddressMode addressModeV_ = SamplerAddressMode::eRepeat, SamplerAddressMode addressModeW_ = SamplerAddressMode::eRepeat, float mipLodBias_ = 0, Bool32 anisotropyEnable_ = 0, float maxAnisotropy_ = 0, Bool32 compareEnable_ = 0, CompareOp compareOp_ = CompareOp::eNever, float minLod_ = 0, float maxLod_ = 0, BorderColor borderColor_ = BorderColor::eFloatTransparentBlack, Bool32 unnormalizedCoordinates_ = 0 )
      : sType( StructureType::eSamplerCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , magFilter( magFilter_ )
      , minFilter( minFilter_ )
      , mipmapMode( mipmapMode_ )
      , addressModeU( addressModeU_ )
      , addressModeV( addressModeV_ )
      , addressModeW( addressModeW_ )
      , mipLodBias( mipLodBias_ )
      , anisotropyEnable( anisotropyEnable_ )
      , maxAnisotropy( maxAnisotropy_ )
      , compareEnable( compareEnable_ )
      , compareOp( compareOp_ )
      , minLod( minLod_ )
      , maxLod( maxLod_ )
      , borderColor( borderColor_ )
      , unnormalizedCoordinates( unnormalizedCoordinates_ )
    {
    }

    SamplerCreateInfo( VkSamplerCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SamplerCreateInfo) );
    }

    SamplerCreateInfo& operator=( VkSamplerCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SamplerCreateInfo) );
      return *this;
    }

    SamplerCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    SamplerCreateInfo& setFlags( SamplerCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    SamplerCreateInfo& setMagFilter( Filter magFilter_ )
    {
      magFilter = magFilter_;
      return *this;
    }

    SamplerCreateInfo& setMinFilter( Filter minFilter_ )
    {
      minFilter = minFilter_;
      return *this;
    }

    SamplerCreateInfo& setMipmapMode( SamplerMipmapMode mipmapMode_ )
    {
      mipmapMode = mipmapMode_;
      return *this;
    }

    SamplerCreateInfo& setAddressModeU( SamplerAddressMode addressModeU_ )
    {
      addressModeU = addressModeU_;
      return *this;
    }

    SamplerCreateInfo& setAddressModeV( SamplerAddressMode addressModeV_ )
    {
      addressModeV = addressModeV_;
      return *this;
    }

    SamplerCreateInfo& setAddressModeW( SamplerAddressMode addressModeW_ )
    {
      addressModeW = addressModeW_;
      return *this;
    }

    SamplerCreateInfo& setMipLodBias( float mipLodBias_ )
    {
      mipLodBias = mipLodBias_;
      return *this;
    }

    SamplerCreateInfo& setAnisotropyEnable( Bool32 anisotropyEnable_ )
    {
      anisotropyEnable = anisotropyEnable_;
      return *this;
    }

    SamplerCreateInfo& setMaxAnisotropy( float maxAnisotropy_ )
    {
      maxAnisotropy = maxAnisotropy_;
      return *this;
    }

    SamplerCreateInfo& setCompareEnable( Bool32 compareEnable_ )
    {
      compareEnable = compareEnable_;
      return *this;
    }

    SamplerCreateInfo& setCompareOp( CompareOp compareOp_ )
    {
      compareOp = compareOp_;
      return *this;
    }

    SamplerCreateInfo& setMinLod( float minLod_ )
    {
      minLod = minLod_;
      return *this;
    }

    SamplerCreateInfo& setMaxLod( float maxLod_ )
    {
      maxLod = maxLod_;
      return *this;
    }

    SamplerCreateInfo& setBorderColor( BorderColor borderColor_ )
    {
      borderColor = borderColor_;
      return *this;
    }

    SamplerCreateInfo& setUnnormalizedCoordinates( Bool32 unnormalizedCoordinates_ )
    {
      unnormalizedCoordinates = unnormalizedCoordinates_;
      return *this;
    }

    operator const VkSamplerCreateInfo&() const
    {
      return *reinterpret_cast<const VkSamplerCreateInfo*>(this);
    }

    bool operator==( SamplerCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( magFilter == rhs.magFilter )
          && ( minFilter == rhs.minFilter )
          && ( mipmapMode == rhs.mipmapMode )
          && ( addressModeU == rhs.addressModeU )
          && ( addressModeV == rhs.addressModeV )
          && ( addressModeW == rhs.addressModeW )
          && ( mipLodBias == rhs.mipLodBias )
          && ( anisotropyEnable == rhs.anisotropyEnable )
          && ( maxAnisotropy == rhs.maxAnisotropy )
          && ( compareEnable == rhs.compareEnable )
          && ( compareOp == rhs.compareOp )
          && ( minLod == rhs.minLod )
          && ( maxLod == rhs.maxLod )
          && ( borderColor == rhs.borderColor )
          && ( unnormalizedCoordinates == rhs.unnormalizedCoordinates );
    }

    bool operator!=( SamplerCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    SamplerCreateFlags flags;
    Filter magFilter;
    Filter minFilter;
    SamplerMipmapMode mipmapMode;
    SamplerAddressMode addressModeU;
    SamplerAddressMode addressModeV;
    SamplerAddressMode addressModeW;
    float mipLodBias;
    Bool32 anisotropyEnable;
    float maxAnisotropy;
    Bool32 compareEnable;
    CompareOp compareOp;
    float minLod;
    float maxLod;
    BorderColor borderColor;
    Bool32 unnormalizedCoordinates;
  };
  static_assert( sizeof( SamplerCreateInfo ) == sizeof( VkSamplerCreateInfo ), "struct and wrapper have different size!" );

  struct CommandBufferAllocateInfo
  {
    CommandBufferAllocateInfo( CommandPool commandPool_ = CommandPool(), CommandBufferLevel level_ = CommandBufferLevel::ePrimary, uint32_t commandBufferCount_ = 0 )
      : sType( StructureType::eCommandBufferAllocateInfo )
      , pNext( nullptr )
      , commandPool( commandPool_ )
      , level( level_ )
      , commandBufferCount( commandBufferCount_ )
    {
    }

    CommandBufferAllocateInfo( VkCommandBufferAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferAllocateInfo) );
    }

    CommandBufferAllocateInfo& operator=( VkCommandBufferAllocateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferAllocateInfo) );
      return *this;
    }

    CommandBufferAllocateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CommandBufferAllocateInfo& setCommandPool( CommandPool commandPool_ )
    {
      commandPool = commandPool_;
      return *this;
    }

    CommandBufferAllocateInfo& setLevel( CommandBufferLevel level_ )
    {
      level = level_;
      return *this;
    }

    CommandBufferAllocateInfo& setCommandBufferCount( uint32_t commandBufferCount_ )
    {
      commandBufferCount = commandBufferCount_;
      return *this;
    }

    operator const VkCommandBufferAllocateInfo&() const
    {
      return *reinterpret_cast<const VkCommandBufferAllocateInfo*>(this);
    }

    bool operator==( CommandBufferAllocateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( commandPool == rhs.commandPool )
          && ( level == rhs.level )
          && ( commandBufferCount == rhs.commandBufferCount );
    }

    bool operator!=( CommandBufferAllocateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    CommandPool commandPool;
    CommandBufferLevel level;
    uint32_t commandBufferCount;
  };
  static_assert( sizeof( CommandBufferAllocateInfo ) == sizeof( VkCommandBufferAllocateInfo ), "struct and wrapper have different size!" );

  struct RenderPassBeginInfo
  {
    RenderPassBeginInfo( RenderPass renderPass_ = RenderPass(), Framebuffer framebuffer_ = Framebuffer(), Rect2D renderArea_ = Rect2D(), uint32_t clearValueCount_ = 0, const ClearValue* pClearValues_ = nullptr )
      : sType( StructureType::eRenderPassBeginInfo )
      , pNext( nullptr )
      , renderPass( renderPass_ )
      , framebuffer( framebuffer_ )
      , renderArea( renderArea_ )
      , clearValueCount( clearValueCount_ )
      , pClearValues( pClearValues_ )
    {
    }

    RenderPassBeginInfo( VkRenderPassBeginInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(RenderPassBeginInfo) );
    }

    RenderPassBeginInfo& operator=( VkRenderPassBeginInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(RenderPassBeginInfo) );
      return *this;
    }

    RenderPassBeginInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    RenderPassBeginInfo& setRenderPass( RenderPass renderPass_ )
    {
      renderPass = renderPass_;
      return *this;
    }

    RenderPassBeginInfo& setFramebuffer( Framebuffer framebuffer_ )
    {
      framebuffer = framebuffer_;
      return *this;
    }

    RenderPassBeginInfo& setRenderArea( Rect2D renderArea_ )
    {
      renderArea = renderArea_;
      return *this;
    }

    RenderPassBeginInfo& setClearValueCount( uint32_t clearValueCount_ )
    {
      clearValueCount = clearValueCount_;
      return *this;
    }

    RenderPassBeginInfo& setPClearValues( const ClearValue* pClearValues_ )
    {
      pClearValues = pClearValues_;
      return *this;
    }

    operator const VkRenderPassBeginInfo&() const
    {
      return *reinterpret_cast<const VkRenderPassBeginInfo*>(this);
    }

    bool operator==( RenderPassBeginInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( renderPass == rhs.renderPass )
          && ( framebuffer == rhs.framebuffer )
          && ( renderArea == rhs.renderArea )
          && ( clearValueCount == rhs.clearValueCount )
          && ( pClearValues == rhs.pClearValues );
    }

    bool operator!=( RenderPassBeginInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    RenderPass renderPass;
    Framebuffer framebuffer;
    Rect2D renderArea;
    uint32_t clearValueCount;
    const ClearValue* pClearValues;
  };
  static_assert( sizeof( RenderPassBeginInfo ) == sizeof( VkRenderPassBeginInfo ), "struct and wrapper have different size!" );

  struct EventCreateInfo
  {
    EventCreateInfo( EventCreateFlags flags_ = EventCreateFlags() )
      : sType( StructureType::eEventCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
    {
    }

    EventCreateInfo( VkEventCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(EventCreateInfo) );
    }

    EventCreateInfo& operator=( VkEventCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(EventCreateInfo) );
      return *this;
    }

    EventCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    EventCreateInfo& setFlags( EventCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkEventCreateInfo&() const
    {
      return *reinterpret_cast<const VkEventCreateInfo*>(this);
    }

    bool operator==( EventCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags );
    }

    bool operator!=( EventCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    EventCreateFlags flags;
  };
  static_assert( sizeof( EventCreateInfo ) == sizeof( VkEventCreateInfo ), "struct and wrapper have different size!" );

  struct SemaphoreCreateInfo
  {
    SemaphoreCreateInfo( SemaphoreCreateFlags flags_ = SemaphoreCreateFlags() )
      : sType( StructureType::eSemaphoreCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
    {
    }

    SemaphoreCreateInfo( VkSemaphoreCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SemaphoreCreateInfo) );
    }

    SemaphoreCreateInfo& operator=( VkSemaphoreCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SemaphoreCreateInfo) );
      return *this;
    }

    SemaphoreCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    SemaphoreCreateInfo& setFlags( SemaphoreCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkSemaphoreCreateInfo&() const
    {
      return *reinterpret_cast<const VkSemaphoreCreateInfo*>(this);
    }

    bool operator==( SemaphoreCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags );
    }

    bool operator!=( SemaphoreCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    SemaphoreCreateFlags flags;
  };
  static_assert( sizeof( SemaphoreCreateInfo ) == sizeof( VkSemaphoreCreateInfo ), "struct and wrapper have different size!" );

  struct FramebufferCreateInfo
  {
    FramebufferCreateInfo( FramebufferCreateFlags flags_ = FramebufferCreateFlags(), RenderPass renderPass_ = RenderPass(), uint32_t attachmentCount_ = 0, const ImageView* pAttachments_ = nullptr, uint32_t width_ = 0, uint32_t height_ = 0, uint32_t layers_ = 0 )
      : sType( StructureType::eFramebufferCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , renderPass( renderPass_ )
      , attachmentCount( attachmentCount_ )
      , pAttachments( pAttachments_ )
      , width( width_ )
      , height( height_ )
      , layers( layers_ )
    {
    }

    FramebufferCreateInfo( VkFramebufferCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(FramebufferCreateInfo) );
    }

    FramebufferCreateInfo& operator=( VkFramebufferCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(FramebufferCreateInfo) );
      return *this;
    }

    FramebufferCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    FramebufferCreateInfo& setFlags( FramebufferCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    FramebufferCreateInfo& setRenderPass( RenderPass renderPass_ )
    {
      renderPass = renderPass_;
      return *this;
    }

    FramebufferCreateInfo& setAttachmentCount( uint32_t attachmentCount_ )
    {
      attachmentCount = attachmentCount_;
      return *this;
    }

    FramebufferCreateInfo& setPAttachments( const ImageView* pAttachments_ )
    {
      pAttachments = pAttachments_;
      return *this;
    }

    FramebufferCreateInfo& setWidth( uint32_t width_ )
    {
      width = width_;
      return *this;
    }

    FramebufferCreateInfo& setHeight( uint32_t height_ )
    {
      height = height_;
      return *this;
    }

    FramebufferCreateInfo& setLayers( uint32_t layers_ )
    {
      layers = layers_;
      return *this;
    }

    operator const VkFramebufferCreateInfo&() const
    {
      return *reinterpret_cast<const VkFramebufferCreateInfo*>(this);
    }

    bool operator==( FramebufferCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( renderPass == rhs.renderPass )
          && ( attachmentCount == rhs.attachmentCount )
          && ( pAttachments == rhs.pAttachments )
          && ( width == rhs.width )
          && ( height == rhs.height )
          && ( layers == rhs.layers );
    }

    bool operator!=( FramebufferCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    FramebufferCreateFlags flags;
    RenderPass renderPass;
    uint32_t attachmentCount;
    const ImageView* pAttachments;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
  };
  static_assert( sizeof( FramebufferCreateInfo ) == sizeof( VkFramebufferCreateInfo ), "struct and wrapper have different size!" );

  struct DisplayModeCreateInfoKHR
  {
    DisplayModeCreateInfoKHR( DisplayModeCreateFlagsKHR flags_ = DisplayModeCreateFlagsKHR(), DisplayModeParametersKHR parameters_ = DisplayModeParametersKHR() )
      : sType( StructureType::eDisplayModeCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , parameters( parameters_ )
    {
    }

    DisplayModeCreateInfoKHR( VkDisplayModeCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayModeCreateInfoKHR) );
    }

    DisplayModeCreateInfoKHR& operator=( VkDisplayModeCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayModeCreateInfoKHR) );
      return *this;
    }

    DisplayModeCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DisplayModeCreateInfoKHR& setFlags( DisplayModeCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    DisplayModeCreateInfoKHR& setParameters( DisplayModeParametersKHR parameters_ )
    {
      parameters = parameters_;
      return *this;
    }

    operator const VkDisplayModeCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkDisplayModeCreateInfoKHR*>(this);
    }

    bool operator==( DisplayModeCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( parameters == rhs.parameters );
    }

    bool operator!=( DisplayModeCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DisplayModeCreateFlagsKHR flags;
    DisplayModeParametersKHR parameters;
  };
  static_assert( sizeof( DisplayModeCreateInfoKHR ) == sizeof( VkDisplayModeCreateInfoKHR ), "struct and wrapper have different size!" );

  struct DisplayPresentInfoKHR
  {
    DisplayPresentInfoKHR( Rect2D srcRect_ = Rect2D(), Rect2D dstRect_ = Rect2D(), Bool32 persistent_ = 0 )
      : sType( StructureType::eDisplayPresentInfoKHR )
      , pNext( nullptr )
      , srcRect( srcRect_ )
      , dstRect( dstRect_ )
      , persistent( persistent_ )
    {
    }

    DisplayPresentInfoKHR( VkDisplayPresentInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayPresentInfoKHR) );
    }

    DisplayPresentInfoKHR& operator=( VkDisplayPresentInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayPresentInfoKHR) );
      return *this;
    }

    DisplayPresentInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DisplayPresentInfoKHR& setSrcRect( Rect2D srcRect_ )
    {
      srcRect = srcRect_;
      return *this;
    }

    DisplayPresentInfoKHR& setDstRect( Rect2D dstRect_ )
    {
      dstRect = dstRect_;
      return *this;
    }

    DisplayPresentInfoKHR& setPersistent( Bool32 persistent_ )
    {
      persistent = persistent_;
      return *this;
    }

    operator const VkDisplayPresentInfoKHR&() const
    {
      return *reinterpret_cast<const VkDisplayPresentInfoKHR*>(this);
    }

    bool operator==( DisplayPresentInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( srcRect == rhs.srcRect )
          && ( dstRect == rhs.dstRect )
          && ( persistent == rhs.persistent );
    }

    bool operator!=( DisplayPresentInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Rect2D srcRect;
    Rect2D dstRect;
    Bool32 persistent;
  };
  static_assert( sizeof( DisplayPresentInfoKHR ) == sizeof( VkDisplayPresentInfoKHR ), "struct and wrapper have different size!" );

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  struct AndroidSurfaceCreateInfoKHR
  {
    AndroidSurfaceCreateInfoKHR( AndroidSurfaceCreateFlagsKHR flags_ = AndroidSurfaceCreateFlagsKHR(), ANativeWindow* window_ = nullptr )
      : sType( StructureType::eAndroidSurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , window( window_ )
    {
    }

    AndroidSurfaceCreateInfoKHR( VkAndroidSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(AndroidSurfaceCreateInfoKHR) );
    }

    AndroidSurfaceCreateInfoKHR& operator=( VkAndroidSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(AndroidSurfaceCreateInfoKHR) );
      return *this;
    }

    AndroidSurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    AndroidSurfaceCreateInfoKHR& setFlags( AndroidSurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    AndroidSurfaceCreateInfoKHR& setWindow( ANativeWindow* window_ )
    {
      window = window_;
      return *this;
    }

    operator const VkAndroidSurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>(this);
    }

    bool operator==( AndroidSurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( window == rhs.window );
    }

    bool operator!=( AndroidSurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    AndroidSurfaceCreateFlagsKHR flags;
    ANativeWindow* window;
  };
  static_assert( sizeof( AndroidSurfaceCreateInfoKHR ) == sizeof( VkAndroidSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  struct MirSurfaceCreateInfoKHR
  {
    MirSurfaceCreateInfoKHR( MirSurfaceCreateFlagsKHR flags_ = MirSurfaceCreateFlagsKHR(), MirConnection* connection_ = nullptr, MirSurface* mirSurface_ = nullptr )
      : sType( StructureType::eMirSurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , connection( connection_ )
      , mirSurface( mirSurface_ )
    {
    }

    MirSurfaceCreateInfoKHR( VkMirSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(MirSurfaceCreateInfoKHR) );
    }

    MirSurfaceCreateInfoKHR& operator=( VkMirSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(MirSurfaceCreateInfoKHR) );
      return *this;
    }

    MirSurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    MirSurfaceCreateInfoKHR& setFlags( MirSurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    MirSurfaceCreateInfoKHR& setConnection( MirConnection* connection_ )
    {
      connection = connection_;
      return *this;
    }

    MirSurfaceCreateInfoKHR& setMirSurface( MirSurface* mirSurface_ )
    {
      mirSurface = mirSurface_;
      return *this;
    }

    operator const VkMirSurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>(this);
    }

    bool operator==( MirSurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( connection == rhs.connection )
          && ( mirSurface == rhs.mirSurface );
    }

    bool operator!=( MirSurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    MirSurfaceCreateFlagsKHR flags;
    MirConnection* connection;
    MirSurface* mirSurface;
  };
  static_assert( sizeof( MirSurfaceCreateInfoKHR ) == sizeof( VkMirSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_VI_NN
  struct ViSurfaceCreateInfoNN
  {
    ViSurfaceCreateInfoNN( ViSurfaceCreateFlagsNN flags_ = ViSurfaceCreateFlagsNN(), void* window_ = nullptr )
      : sType( StructureType::eViSurfaceCreateInfoNN )
      , pNext( nullptr )
      , flags( flags_ )
      , window( window_ )
    {
    }

    ViSurfaceCreateInfoNN( VkViSurfaceCreateInfoNN const & rhs )
    {
      memcpy( this, &rhs, sizeof(ViSurfaceCreateInfoNN) );
    }

    ViSurfaceCreateInfoNN& operator=( VkViSurfaceCreateInfoNN const & rhs )
    {
      memcpy( this, &rhs, sizeof(ViSurfaceCreateInfoNN) );
      return *this;
    }

    ViSurfaceCreateInfoNN& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ViSurfaceCreateInfoNN& setFlags( ViSurfaceCreateFlagsNN flags_ )
    {
      flags = flags_;
      return *this;
    }

    ViSurfaceCreateInfoNN& setWindow( void* window_ )
    {
      window = window_;
      return *this;
    }

    operator const VkViSurfaceCreateInfoNN&() const
    {
      return *reinterpret_cast<const VkViSurfaceCreateInfoNN*>(this);
    }

    bool operator==( ViSurfaceCreateInfoNN const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( window == rhs.window );
    }

    bool operator!=( ViSurfaceCreateInfoNN const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ViSurfaceCreateFlagsNN flags;
    void* window;
  };
  static_assert( sizeof( ViSurfaceCreateInfoNN ) == sizeof( VkViSurfaceCreateInfoNN ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  struct WaylandSurfaceCreateInfoKHR
  {
    WaylandSurfaceCreateInfoKHR( WaylandSurfaceCreateFlagsKHR flags_ = WaylandSurfaceCreateFlagsKHR(), struct wl_display* display_ = nullptr, struct wl_surface* surface_ = nullptr )
      : sType( StructureType::eWaylandSurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , display( display_ )
      , surface( surface_ )
    {
    }

    WaylandSurfaceCreateInfoKHR( VkWaylandSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(WaylandSurfaceCreateInfoKHR) );
    }

    WaylandSurfaceCreateInfoKHR& operator=( VkWaylandSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(WaylandSurfaceCreateInfoKHR) );
      return *this;
    }

    WaylandSurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    WaylandSurfaceCreateInfoKHR& setFlags( WaylandSurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    WaylandSurfaceCreateInfoKHR& setDisplay( struct wl_display* display_ )
    {
      display = display_;
      return *this;
    }

    WaylandSurfaceCreateInfoKHR& setSurface( struct wl_surface* surface_ )
    {
      surface = surface_;
      return *this;
    }

    operator const VkWaylandSurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>(this);
    }

    bool operator==( WaylandSurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( display == rhs.display )
          && ( surface == rhs.surface );
    }

    bool operator!=( WaylandSurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    WaylandSurfaceCreateFlagsKHR flags;
    struct wl_display* display;
    struct wl_surface* surface;
  };
  static_assert( sizeof( WaylandSurfaceCreateInfoKHR ) == sizeof( VkWaylandSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  struct Win32SurfaceCreateInfoKHR
  {
    Win32SurfaceCreateInfoKHR( Win32SurfaceCreateFlagsKHR flags_ = Win32SurfaceCreateFlagsKHR(), HINSTANCE hinstance_ = 0, HWND hwnd_ = 0 )
      : sType( StructureType::eWin32SurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , hinstance( hinstance_ )
      , hwnd( hwnd_ )
    {
    }

    Win32SurfaceCreateInfoKHR( VkWin32SurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(Win32SurfaceCreateInfoKHR) );
    }

    Win32SurfaceCreateInfoKHR& operator=( VkWin32SurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(Win32SurfaceCreateInfoKHR) );
      return *this;
    }

    Win32SurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    Win32SurfaceCreateInfoKHR& setFlags( Win32SurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    Win32SurfaceCreateInfoKHR& setHinstance( HINSTANCE hinstance_ )
    {
      hinstance = hinstance_;
      return *this;
    }

    Win32SurfaceCreateInfoKHR& setHwnd( HWND hwnd_ )
    {
      hwnd = hwnd_;
      return *this;
    }

    operator const VkWin32SurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>(this);
    }

    bool operator==( Win32SurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( hinstance == rhs.hinstance )
          && ( hwnd == rhs.hwnd );
    }

    bool operator!=( Win32SurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Win32SurfaceCreateFlagsKHR flags;
    HINSTANCE hinstance;
    HWND hwnd;
  };
  static_assert( sizeof( Win32SurfaceCreateInfoKHR ) == sizeof( VkWin32SurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  struct XlibSurfaceCreateInfoKHR
  {
    XlibSurfaceCreateInfoKHR( XlibSurfaceCreateFlagsKHR flags_ = XlibSurfaceCreateFlagsKHR(), Display* dpy_ = nullptr, Window window_ = 0 )
      : sType( StructureType::eXlibSurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , dpy( dpy_ )
      , window( window_ )
    {
    }

    XlibSurfaceCreateInfoKHR( VkXlibSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(XlibSurfaceCreateInfoKHR) );
    }

    XlibSurfaceCreateInfoKHR& operator=( VkXlibSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(XlibSurfaceCreateInfoKHR) );
      return *this;
    }

    XlibSurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    XlibSurfaceCreateInfoKHR& setFlags( XlibSurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    XlibSurfaceCreateInfoKHR& setDpy( Display* dpy_ )
    {
      dpy = dpy_;
      return *this;
    }

    XlibSurfaceCreateInfoKHR& setWindow( Window window_ )
    {
      window = window_;
      return *this;
    }

    operator const VkXlibSurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>(this);
    }

    bool operator==( XlibSurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( dpy == rhs.dpy )
          && ( window == rhs.window );
    }

    bool operator!=( XlibSurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    XlibSurfaceCreateFlagsKHR flags;
    Display* dpy;
    Window window;
  };
  static_assert( sizeof( XlibSurfaceCreateInfoKHR ) == sizeof( VkXlibSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  struct XcbSurfaceCreateInfoKHR
  {
    XcbSurfaceCreateInfoKHR( XcbSurfaceCreateFlagsKHR flags_ = XcbSurfaceCreateFlagsKHR(), xcb_connection_t* connection_ = nullptr, xcb_window_t window_ = 0 )
      : sType( StructureType::eXcbSurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , connection( connection_ )
      , window( window_ )
    {
    }

    XcbSurfaceCreateInfoKHR( VkXcbSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(XcbSurfaceCreateInfoKHR) );
    }

    XcbSurfaceCreateInfoKHR& operator=( VkXcbSurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(XcbSurfaceCreateInfoKHR) );
      return *this;
    }

    XcbSurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    XcbSurfaceCreateInfoKHR& setFlags( XcbSurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    XcbSurfaceCreateInfoKHR& setConnection( xcb_connection_t* connection_ )
    {
      connection = connection_;
      return *this;
    }

    XcbSurfaceCreateInfoKHR& setWindow( xcb_window_t window_ )
    {
      window = window_;
      return *this;
    }

    operator const VkXcbSurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>(this);
    }

    bool operator==( XcbSurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( connection == rhs.connection )
          && ( window == rhs.window );
    }

    bool operator!=( XcbSurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    XcbSurfaceCreateFlagsKHR flags;
    xcb_connection_t* connection;
    xcb_window_t window;
  };
  static_assert( sizeof( XcbSurfaceCreateInfoKHR ) == sizeof( VkXcbSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  struct DebugMarkerMarkerInfoEXT
  {
    DebugMarkerMarkerInfoEXT( const char* pMarkerName_ = nullptr, std::array<float,4> const& color_ = { { 0, 0, 0, 0 } } )
      : sType( StructureType::eDebugMarkerMarkerInfoEXT )
      , pNext( nullptr )
      , pMarkerName( pMarkerName_ )
    {
      memcpy( &color, color_.data(), 4 * sizeof( float ) );
    }

    DebugMarkerMarkerInfoEXT( VkDebugMarkerMarkerInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerMarkerInfoEXT) );
    }

    DebugMarkerMarkerInfoEXT& operator=( VkDebugMarkerMarkerInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerMarkerInfoEXT) );
      return *this;
    }

    DebugMarkerMarkerInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DebugMarkerMarkerInfoEXT& setPMarkerName( const char* pMarkerName_ )
    {
      pMarkerName = pMarkerName_;
      return *this;
    }

    DebugMarkerMarkerInfoEXT& setColor( std::array<float,4> color_ )
    {
      memcpy( &color, color_.data(), 4 * sizeof( float ) );
      return *this;
    }

    operator const VkDebugMarkerMarkerInfoEXT&() const
    {
      return *reinterpret_cast<const VkDebugMarkerMarkerInfoEXT*>(this);
    }

    bool operator==( DebugMarkerMarkerInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( pMarkerName == rhs.pMarkerName )
          && ( memcmp( color, rhs.color, 4 * sizeof( float ) ) == 0 );
    }

    bool operator!=( DebugMarkerMarkerInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    const char* pMarkerName;
    float color[4];
  };
  static_assert( sizeof( DebugMarkerMarkerInfoEXT ) == sizeof( VkDebugMarkerMarkerInfoEXT ), "struct and wrapper have different size!" );

  struct DedicatedAllocationImageCreateInfoNV
  {
    DedicatedAllocationImageCreateInfoNV( Bool32 dedicatedAllocation_ = 0 )
      : sType( StructureType::eDedicatedAllocationImageCreateInfoNV )
      , pNext( nullptr )
      , dedicatedAllocation( dedicatedAllocation_ )
    {
    }

    DedicatedAllocationImageCreateInfoNV( VkDedicatedAllocationImageCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationImageCreateInfoNV) );
    }

    DedicatedAllocationImageCreateInfoNV& operator=( VkDedicatedAllocationImageCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationImageCreateInfoNV) );
      return *this;
    }

    DedicatedAllocationImageCreateInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DedicatedAllocationImageCreateInfoNV& setDedicatedAllocation( Bool32 dedicatedAllocation_ )
    {
      dedicatedAllocation = dedicatedAllocation_;
      return *this;
    }

    operator const VkDedicatedAllocationImageCreateInfoNV&() const
    {
      return *reinterpret_cast<const VkDedicatedAllocationImageCreateInfoNV*>(this);
    }

    bool operator==( DedicatedAllocationImageCreateInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( dedicatedAllocation == rhs.dedicatedAllocation );
    }

    bool operator!=( DedicatedAllocationImageCreateInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Bool32 dedicatedAllocation;
  };
  static_assert( sizeof( DedicatedAllocationImageCreateInfoNV ) == sizeof( VkDedicatedAllocationImageCreateInfoNV ), "struct and wrapper have different size!" );

  struct DedicatedAllocationBufferCreateInfoNV
  {
    DedicatedAllocationBufferCreateInfoNV( Bool32 dedicatedAllocation_ = 0 )
      : sType( StructureType::eDedicatedAllocationBufferCreateInfoNV )
      , pNext( nullptr )
      , dedicatedAllocation( dedicatedAllocation_ )
    {
    }

    DedicatedAllocationBufferCreateInfoNV( VkDedicatedAllocationBufferCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationBufferCreateInfoNV) );
    }

    DedicatedAllocationBufferCreateInfoNV& operator=( VkDedicatedAllocationBufferCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationBufferCreateInfoNV) );
      return *this;
    }

    DedicatedAllocationBufferCreateInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DedicatedAllocationBufferCreateInfoNV& setDedicatedAllocation( Bool32 dedicatedAllocation_ )
    {
      dedicatedAllocation = dedicatedAllocation_;
      return *this;
    }

    operator const VkDedicatedAllocationBufferCreateInfoNV&() const
    {
      return *reinterpret_cast<const VkDedicatedAllocationBufferCreateInfoNV*>(this);
    }

    bool operator==( DedicatedAllocationBufferCreateInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( dedicatedAllocation == rhs.dedicatedAllocation );
    }

    bool operator!=( DedicatedAllocationBufferCreateInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Bool32 dedicatedAllocation;
  };
  static_assert( sizeof( DedicatedAllocationBufferCreateInfoNV ) == sizeof( VkDedicatedAllocationBufferCreateInfoNV ), "struct and wrapper have different size!" );

  struct DedicatedAllocationMemoryAllocateInfoNV
  {
    DedicatedAllocationMemoryAllocateInfoNV( Image image_ = Image(), Buffer buffer_ = Buffer() )
      : sType( StructureType::eDedicatedAllocationMemoryAllocateInfoNV )
      , pNext( nullptr )
      , image( image_ )
      , buffer( buffer_ )
    {
    }

    DedicatedAllocationMemoryAllocateInfoNV( VkDedicatedAllocationMemoryAllocateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationMemoryAllocateInfoNV) );
    }

    DedicatedAllocationMemoryAllocateInfoNV& operator=( VkDedicatedAllocationMemoryAllocateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(DedicatedAllocationMemoryAllocateInfoNV) );
      return *this;
    }

    DedicatedAllocationMemoryAllocateInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DedicatedAllocationMemoryAllocateInfoNV& setImage( Image image_ )
    {
      image = image_;
      return *this;
    }

    DedicatedAllocationMemoryAllocateInfoNV& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    operator const VkDedicatedAllocationMemoryAllocateInfoNV&() const
    {
      return *reinterpret_cast<const VkDedicatedAllocationMemoryAllocateInfoNV*>(this);
    }

    bool operator==( DedicatedAllocationMemoryAllocateInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( image == rhs.image )
          && ( buffer == rhs.buffer );
    }

    bool operator!=( DedicatedAllocationMemoryAllocateInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Image image;
    Buffer buffer;
  };
  static_assert( sizeof( DedicatedAllocationMemoryAllocateInfoNV ) == sizeof( VkDedicatedAllocationMemoryAllocateInfoNV ), "struct and wrapper have different size!" );

#ifdef VK_USE_PLATFORM_WIN32_KHR
  struct ExportMemoryWin32HandleInfoNV
  {
    ExportMemoryWin32HandleInfoNV( const SECURITY_ATTRIBUTES* pAttributes_ = nullptr, DWORD dwAccess_ = 0 )
      : sType( StructureType::eExportMemoryWin32HandleInfoNV )
      , pNext( nullptr )
      , pAttributes( pAttributes_ )
      , dwAccess( dwAccess_ )
    {
    }

    ExportMemoryWin32HandleInfoNV( VkExportMemoryWin32HandleInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExportMemoryWin32HandleInfoNV) );
    }

    ExportMemoryWin32HandleInfoNV& operator=( VkExportMemoryWin32HandleInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExportMemoryWin32HandleInfoNV) );
      return *this;
    }

    ExportMemoryWin32HandleInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ExportMemoryWin32HandleInfoNV& setPAttributes( const SECURITY_ATTRIBUTES* pAttributes_ )
    {
      pAttributes = pAttributes_;
      return *this;
    }

    ExportMemoryWin32HandleInfoNV& setDwAccess( DWORD dwAccess_ )
    {
      dwAccess = dwAccess_;
      return *this;
    }

    operator const VkExportMemoryWin32HandleInfoNV&() const
    {
      return *reinterpret_cast<const VkExportMemoryWin32HandleInfoNV*>(this);
    }

    bool operator==( ExportMemoryWin32HandleInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( pAttributes == rhs.pAttributes )
          && ( dwAccess == rhs.dwAccess );
    }

    bool operator!=( ExportMemoryWin32HandleInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    const SECURITY_ATTRIBUTES* pAttributes;
    DWORD dwAccess;
  };
  static_assert( sizeof( ExportMemoryWin32HandleInfoNV ) == sizeof( VkExportMemoryWin32HandleInfoNV ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  struct Win32KeyedMutexAcquireReleaseInfoNV
  {
    Win32KeyedMutexAcquireReleaseInfoNV( uint32_t acquireCount_ = 0, const DeviceMemory* pAcquireSyncs_ = nullptr, const uint64_t* pAcquireKeys_ = nullptr, const uint32_t* pAcquireTimeoutMilliseconds_ = nullptr, uint32_t releaseCount_ = 0, const DeviceMemory* pReleaseSyncs_ = nullptr, const uint64_t* pReleaseKeys_ = nullptr )
      : sType( StructureType::eWin32KeyedMutexAcquireReleaseInfoNV )
      , pNext( nullptr )
      , acquireCount( acquireCount_ )
      , pAcquireSyncs( pAcquireSyncs_ )
      , pAcquireKeys( pAcquireKeys_ )
      , pAcquireTimeoutMilliseconds( pAcquireTimeoutMilliseconds_ )
      , releaseCount( releaseCount_ )
      , pReleaseSyncs( pReleaseSyncs_ )
      , pReleaseKeys( pReleaseKeys_ )
    {
    }

    Win32KeyedMutexAcquireReleaseInfoNV( VkWin32KeyedMutexAcquireReleaseInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(Win32KeyedMutexAcquireReleaseInfoNV) );
    }

    Win32KeyedMutexAcquireReleaseInfoNV& operator=( VkWin32KeyedMutexAcquireReleaseInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(Win32KeyedMutexAcquireReleaseInfoNV) );
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setAcquireCount( uint32_t acquireCount_ )
    {
      acquireCount = acquireCount_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPAcquireSyncs( const DeviceMemory* pAcquireSyncs_ )
    {
      pAcquireSyncs = pAcquireSyncs_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPAcquireKeys( const uint64_t* pAcquireKeys_ )
    {
      pAcquireKeys = pAcquireKeys_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPAcquireTimeoutMilliseconds( const uint32_t* pAcquireTimeoutMilliseconds_ )
    {
      pAcquireTimeoutMilliseconds = pAcquireTimeoutMilliseconds_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setReleaseCount( uint32_t releaseCount_ )
    {
      releaseCount = releaseCount_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPReleaseSyncs( const DeviceMemory* pReleaseSyncs_ )
    {
      pReleaseSyncs = pReleaseSyncs_;
      return *this;
    }

    Win32KeyedMutexAcquireReleaseInfoNV& setPReleaseKeys( const uint64_t* pReleaseKeys_ )
    {
      pReleaseKeys = pReleaseKeys_;
      return *this;
    }

    operator const VkWin32KeyedMutexAcquireReleaseInfoNV&() const
    {
      return *reinterpret_cast<const VkWin32KeyedMutexAcquireReleaseInfoNV*>(this);
    }

    bool operator==( Win32KeyedMutexAcquireReleaseInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( acquireCount == rhs.acquireCount )
          && ( pAcquireSyncs == rhs.pAcquireSyncs )
          && ( pAcquireKeys == rhs.pAcquireKeys )
          && ( pAcquireTimeoutMilliseconds == rhs.pAcquireTimeoutMilliseconds )
          && ( releaseCount == rhs.releaseCount )
          && ( pReleaseSyncs == rhs.pReleaseSyncs )
          && ( pReleaseKeys == rhs.pReleaseKeys );
    }

    bool operator!=( Win32KeyedMutexAcquireReleaseInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t acquireCount;
    const DeviceMemory* pAcquireSyncs;
    const uint64_t* pAcquireKeys;
    const uint32_t* pAcquireTimeoutMilliseconds;
    uint32_t releaseCount;
    const DeviceMemory* pReleaseSyncs;
    const uint64_t* pReleaseKeys;
  };
  static_assert( sizeof( Win32KeyedMutexAcquireReleaseInfoNV ) == sizeof( VkWin32KeyedMutexAcquireReleaseInfoNV ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  struct DeviceGeneratedCommandsFeaturesNVX
  {
    DeviceGeneratedCommandsFeaturesNVX( Bool32 computeBindingPointSupport_ = 0 )
      : sType( StructureType::eDeviceGeneratedCommandsFeaturesNVX )
      , pNext( nullptr )
      , computeBindingPointSupport( computeBindingPointSupport_ )
    {
    }

    DeviceGeneratedCommandsFeaturesNVX( VkDeviceGeneratedCommandsFeaturesNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceGeneratedCommandsFeaturesNVX) );
    }

    DeviceGeneratedCommandsFeaturesNVX& operator=( VkDeviceGeneratedCommandsFeaturesNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceGeneratedCommandsFeaturesNVX) );
      return *this;
    }

    DeviceGeneratedCommandsFeaturesNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DeviceGeneratedCommandsFeaturesNVX& setComputeBindingPointSupport( Bool32 computeBindingPointSupport_ )
    {
      computeBindingPointSupport = computeBindingPointSupport_;
      return *this;
    }

    operator const VkDeviceGeneratedCommandsFeaturesNVX&() const
    {
      return *reinterpret_cast<const VkDeviceGeneratedCommandsFeaturesNVX*>(this);
    }

    bool operator==( DeviceGeneratedCommandsFeaturesNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( computeBindingPointSupport == rhs.computeBindingPointSupport );
    }

    bool operator!=( DeviceGeneratedCommandsFeaturesNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Bool32 computeBindingPointSupport;
  };
  static_assert( sizeof( DeviceGeneratedCommandsFeaturesNVX ) == sizeof( VkDeviceGeneratedCommandsFeaturesNVX ), "struct and wrapper have different size!" );

  struct DeviceGeneratedCommandsLimitsNVX
  {
    DeviceGeneratedCommandsLimitsNVX( uint32_t maxIndirectCommandsLayoutTokenCount_ = 0, uint32_t maxObjectEntryCounts_ = 0, uint32_t minSequenceCountBufferOffsetAlignment_ = 0, uint32_t minSequenceIndexBufferOffsetAlignment_ = 0, uint32_t minCommandsTokenBufferOffsetAlignment_ = 0 )
      : sType( StructureType::eDeviceGeneratedCommandsLimitsNVX )
      , pNext( nullptr )
      , maxIndirectCommandsLayoutTokenCount( maxIndirectCommandsLayoutTokenCount_ )
      , maxObjectEntryCounts( maxObjectEntryCounts_ )
      , minSequenceCountBufferOffsetAlignment( minSequenceCountBufferOffsetAlignment_ )
      , minSequenceIndexBufferOffsetAlignment( minSequenceIndexBufferOffsetAlignment_ )
      , minCommandsTokenBufferOffsetAlignment( minCommandsTokenBufferOffsetAlignment_ )
    {
    }

    DeviceGeneratedCommandsLimitsNVX( VkDeviceGeneratedCommandsLimitsNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceGeneratedCommandsLimitsNVX) );
    }

    DeviceGeneratedCommandsLimitsNVX& operator=( VkDeviceGeneratedCommandsLimitsNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceGeneratedCommandsLimitsNVX) );
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setMaxIndirectCommandsLayoutTokenCount( uint32_t maxIndirectCommandsLayoutTokenCount_ )
    {
      maxIndirectCommandsLayoutTokenCount = maxIndirectCommandsLayoutTokenCount_;
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setMaxObjectEntryCounts( uint32_t maxObjectEntryCounts_ )
    {
      maxObjectEntryCounts = maxObjectEntryCounts_;
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setMinSequenceCountBufferOffsetAlignment( uint32_t minSequenceCountBufferOffsetAlignment_ )
    {
      minSequenceCountBufferOffsetAlignment = minSequenceCountBufferOffsetAlignment_;
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setMinSequenceIndexBufferOffsetAlignment( uint32_t minSequenceIndexBufferOffsetAlignment_ )
    {
      minSequenceIndexBufferOffsetAlignment = minSequenceIndexBufferOffsetAlignment_;
      return *this;
    }

    DeviceGeneratedCommandsLimitsNVX& setMinCommandsTokenBufferOffsetAlignment( uint32_t minCommandsTokenBufferOffsetAlignment_ )
    {
      minCommandsTokenBufferOffsetAlignment = minCommandsTokenBufferOffsetAlignment_;
      return *this;
    }

    operator const VkDeviceGeneratedCommandsLimitsNVX&() const
    {
      return *reinterpret_cast<const VkDeviceGeneratedCommandsLimitsNVX*>(this);
    }

    bool operator==( DeviceGeneratedCommandsLimitsNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( maxIndirectCommandsLayoutTokenCount == rhs.maxIndirectCommandsLayoutTokenCount )
          && ( maxObjectEntryCounts == rhs.maxObjectEntryCounts )
          && ( minSequenceCountBufferOffsetAlignment == rhs.minSequenceCountBufferOffsetAlignment )
          && ( minSequenceIndexBufferOffsetAlignment == rhs.minSequenceIndexBufferOffsetAlignment )
          && ( minCommandsTokenBufferOffsetAlignment == rhs.minCommandsTokenBufferOffsetAlignment );
    }

    bool operator!=( DeviceGeneratedCommandsLimitsNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t maxIndirectCommandsLayoutTokenCount;
    uint32_t maxObjectEntryCounts;
    uint32_t minSequenceCountBufferOffsetAlignment;
    uint32_t minSequenceIndexBufferOffsetAlignment;
    uint32_t minCommandsTokenBufferOffsetAlignment;
  };
  static_assert( sizeof( DeviceGeneratedCommandsLimitsNVX ) == sizeof( VkDeviceGeneratedCommandsLimitsNVX ), "struct and wrapper have different size!" );

  struct CmdReserveSpaceForCommandsInfoNVX
  {
    CmdReserveSpaceForCommandsInfoNVX( ObjectTableNVX objectTable_ = ObjectTableNVX(), IndirectCommandsLayoutNVX indirectCommandsLayout_ = IndirectCommandsLayoutNVX(), uint32_t maxSequencesCount_ = 0 )
      : sType( StructureType::eCmdReserveSpaceForCommandsInfoNVX )
      , pNext( nullptr )
      , objectTable( objectTable_ )
      , indirectCommandsLayout( indirectCommandsLayout_ )
      , maxSequencesCount( maxSequencesCount_ )
    {
    }

    CmdReserveSpaceForCommandsInfoNVX( VkCmdReserveSpaceForCommandsInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(CmdReserveSpaceForCommandsInfoNVX) );
    }

    CmdReserveSpaceForCommandsInfoNVX& operator=( VkCmdReserveSpaceForCommandsInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(CmdReserveSpaceForCommandsInfoNVX) );
      return *this;
    }

    CmdReserveSpaceForCommandsInfoNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CmdReserveSpaceForCommandsInfoNVX& setObjectTable( ObjectTableNVX objectTable_ )
    {
      objectTable = objectTable_;
      return *this;
    }

    CmdReserveSpaceForCommandsInfoNVX& setIndirectCommandsLayout( IndirectCommandsLayoutNVX indirectCommandsLayout_ )
    {
      indirectCommandsLayout = indirectCommandsLayout_;
      return *this;
    }

    CmdReserveSpaceForCommandsInfoNVX& setMaxSequencesCount( uint32_t maxSequencesCount_ )
    {
      maxSequencesCount = maxSequencesCount_;
      return *this;
    }

    operator const VkCmdReserveSpaceForCommandsInfoNVX&() const
    {
      return *reinterpret_cast<const VkCmdReserveSpaceForCommandsInfoNVX*>(this);
    }

    bool operator==( CmdReserveSpaceForCommandsInfoNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( objectTable == rhs.objectTable )
          && ( indirectCommandsLayout == rhs.indirectCommandsLayout )
          && ( maxSequencesCount == rhs.maxSequencesCount );
    }

    bool operator!=( CmdReserveSpaceForCommandsInfoNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ObjectTableNVX objectTable;
    IndirectCommandsLayoutNVX indirectCommandsLayout;
    uint32_t maxSequencesCount;
  };
  static_assert( sizeof( CmdReserveSpaceForCommandsInfoNVX ) == sizeof( VkCmdReserveSpaceForCommandsInfoNVX ), "struct and wrapper have different size!" );

  struct PhysicalDeviceFeatures2KHR
  {
    PhysicalDeviceFeatures2KHR( PhysicalDeviceFeatures features_ = PhysicalDeviceFeatures() )
      : sType( StructureType::ePhysicalDeviceFeatures2KHR )
      , pNext( nullptr )
      , features( features_ )
    {
    }

    PhysicalDeviceFeatures2KHR( VkPhysicalDeviceFeatures2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceFeatures2KHR) );
    }

    PhysicalDeviceFeatures2KHR& operator=( VkPhysicalDeviceFeatures2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceFeatures2KHR) );
      return *this;
    }

    PhysicalDeviceFeatures2KHR& setPNext( void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PhysicalDeviceFeatures2KHR& setFeatures( PhysicalDeviceFeatures features_ )
    {
      features = features_;
      return *this;
    }

    operator const VkPhysicalDeviceFeatures2KHR&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceFeatures2KHR*>(this);
    }

    bool operator==( PhysicalDeviceFeatures2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( features == rhs.features );
    }

    bool operator!=( PhysicalDeviceFeatures2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    PhysicalDeviceFeatures features;
  };
  static_assert( sizeof( PhysicalDeviceFeatures2KHR ) == sizeof( VkPhysicalDeviceFeatures2KHR ), "struct and wrapper have different size!" );

  enum class SubpassContents
  {
    eInline = VK_SUBPASS_CONTENTS_INLINE,
    eSecondaryCommandBuffers = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
  };

  struct PresentInfoKHR
  {
    PresentInfoKHR( uint32_t waitSemaphoreCount_ = 0, const Semaphore* pWaitSemaphores_ = nullptr, uint32_t swapchainCount_ = 0, const SwapchainKHR* pSwapchains_ = nullptr, const uint32_t* pImageIndices_ = nullptr, Result* pResults_ = nullptr )
      : sType( StructureType::ePresentInfoKHR )
      , pNext( nullptr )
      , waitSemaphoreCount( waitSemaphoreCount_ )
      , pWaitSemaphores( pWaitSemaphores_ )
      , swapchainCount( swapchainCount_ )
      , pSwapchains( pSwapchains_ )
      , pImageIndices( pImageIndices_ )
      , pResults( pResults_ )
    {
    }

    PresentInfoKHR( VkPresentInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PresentInfoKHR) );
    }

    PresentInfoKHR& operator=( VkPresentInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PresentInfoKHR) );
      return *this;
    }

    PresentInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PresentInfoKHR& setWaitSemaphoreCount( uint32_t waitSemaphoreCount_ )
    {
      waitSemaphoreCount = waitSemaphoreCount_;
      return *this;
    }

    PresentInfoKHR& setPWaitSemaphores( const Semaphore* pWaitSemaphores_ )
    {
      pWaitSemaphores = pWaitSemaphores_;
      return *this;
    }

    PresentInfoKHR& setSwapchainCount( uint32_t swapchainCount_ )
    {
      swapchainCount = swapchainCount_;
      return *this;
    }

    PresentInfoKHR& setPSwapchains( const SwapchainKHR* pSwapchains_ )
    {
      pSwapchains = pSwapchains_;
      return *this;
    }

    PresentInfoKHR& setPImageIndices( const uint32_t* pImageIndices_ )
    {
      pImageIndices = pImageIndices_;
      return *this;
    }

    PresentInfoKHR& setPResults( Result* pResults_ )
    {
      pResults = pResults_;
      return *this;
    }

    operator const VkPresentInfoKHR&() const
    {
      return *reinterpret_cast<const VkPresentInfoKHR*>(this);
    }

    bool operator==( PresentInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( waitSemaphoreCount == rhs.waitSemaphoreCount )
          && ( pWaitSemaphores == rhs.pWaitSemaphores )
          && ( swapchainCount == rhs.swapchainCount )
          && ( pSwapchains == rhs.pSwapchains )
          && ( pImageIndices == rhs.pImageIndices )
          && ( pResults == rhs.pResults );
    }

    bool operator!=( PresentInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t waitSemaphoreCount;
    const Semaphore* pWaitSemaphores;
    uint32_t swapchainCount;
    const SwapchainKHR* pSwapchains;
    const uint32_t* pImageIndices;
    Result* pResults;
  };
  static_assert( sizeof( PresentInfoKHR ) == sizeof( VkPresentInfoKHR ), "struct and wrapper have different size!" );

  enum class DynamicState
  {
    eViewport = VK_DYNAMIC_STATE_VIEWPORT,
    eScissor = VK_DYNAMIC_STATE_SCISSOR,
    eLineWidth = VK_DYNAMIC_STATE_LINE_WIDTH,
    eDepthBias = VK_DYNAMIC_STATE_DEPTH_BIAS,
    eBlendConstants = VK_DYNAMIC_STATE_BLEND_CONSTANTS,
    eDepthBounds = VK_DYNAMIC_STATE_DEPTH_BOUNDS,
    eStencilCompareMask = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
    eStencilWriteMask = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
    eStencilReference = VK_DYNAMIC_STATE_STENCIL_REFERENCE
  };

  struct PipelineDynamicStateCreateInfo
  {
    PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateFlags flags_ = PipelineDynamicStateCreateFlags(), uint32_t dynamicStateCount_ = 0, const DynamicState* pDynamicStates_ = nullptr )
      : sType( StructureType::ePipelineDynamicStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , dynamicStateCount( dynamicStateCount_ )
      , pDynamicStates( pDynamicStates_ )
    {
    }

    PipelineDynamicStateCreateInfo( VkPipelineDynamicStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineDynamicStateCreateInfo) );
    }

    PipelineDynamicStateCreateInfo& operator=( VkPipelineDynamicStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineDynamicStateCreateInfo) );
      return *this;
    }

    PipelineDynamicStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineDynamicStateCreateInfo& setFlags( PipelineDynamicStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineDynamicStateCreateInfo& setDynamicStateCount( uint32_t dynamicStateCount_ )
    {
      dynamicStateCount = dynamicStateCount_;
      return *this;
    }

    PipelineDynamicStateCreateInfo& setPDynamicStates( const DynamicState* pDynamicStates_ )
    {
      pDynamicStates = pDynamicStates_;
      return *this;
    }

    operator const VkPipelineDynamicStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>(this);
    }

    bool operator==( PipelineDynamicStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( dynamicStateCount == rhs.dynamicStateCount )
          && ( pDynamicStates == rhs.pDynamicStates );
    }

    bool operator!=( PipelineDynamicStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineDynamicStateCreateFlags flags;
    uint32_t dynamicStateCount;
    const DynamicState* pDynamicStates;
  };
  static_assert( sizeof( PipelineDynamicStateCreateInfo ) == sizeof( VkPipelineDynamicStateCreateInfo ), "struct and wrapper have different size!" );

  enum class QueueFlagBits
  {
    eGraphics = VK_QUEUE_GRAPHICS_BIT,
    eCompute = VK_QUEUE_COMPUTE_BIT,
    eTransfer = VK_QUEUE_TRANSFER_BIT,
    eSparseBinding = VK_QUEUE_SPARSE_BINDING_BIT
  };

  using QueueFlags = Flags<QueueFlagBits, VkQueueFlags>;

  VULKAN_HPP_INLINE QueueFlags operator|( QueueFlagBits bit0, QueueFlagBits bit1 )
  {
    return QueueFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE QueueFlags operator~( QueueFlagBits bits )
  {
    return ~( QueueFlags( bits ) );
  }

  template <> struct FlagTraits<QueueFlagBits>
  {
    enum
    {
      allFlags = VkFlags(QueueFlagBits::eGraphics) | VkFlags(QueueFlagBits::eCompute) | VkFlags(QueueFlagBits::eTransfer) | VkFlags(QueueFlagBits::eSparseBinding)
    };
  };

  struct QueueFamilyProperties
  {
    operator const VkQueueFamilyProperties&() const
    {
      return *reinterpret_cast<const VkQueueFamilyProperties*>(this);
    }

    bool operator==( QueueFamilyProperties const& rhs ) const
    {
      return ( queueFlags == rhs.queueFlags )
          && ( queueCount == rhs.queueCount )
          && ( timestampValidBits == rhs.timestampValidBits )
          && ( minImageTransferGranularity == rhs.minImageTransferGranularity );
    }

    bool operator!=( QueueFamilyProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    QueueFlags queueFlags;
    uint32_t queueCount;
    uint32_t timestampValidBits;
    Extent3D minImageTransferGranularity;
  };
  static_assert( sizeof( QueueFamilyProperties ) == sizeof( VkQueueFamilyProperties ), "struct and wrapper have different size!" );

  struct QueueFamilyProperties2KHR
  {
    operator const VkQueueFamilyProperties2KHR&() const
    {
      return *reinterpret_cast<const VkQueueFamilyProperties2KHR*>(this);
    }

    bool operator==( QueueFamilyProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( queueFamilyProperties == rhs.queueFamilyProperties );
    }

    bool operator!=( QueueFamilyProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    QueueFamilyProperties queueFamilyProperties;
  };
  static_assert( sizeof( QueueFamilyProperties2KHR ) == sizeof( VkQueueFamilyProperties2KHR ), "struct and wrapper have different size!" );

  enum class MemoryPropertyFlagBits
  {
    eDeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    eHostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    eHostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    eHostCached = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
    eLazilyAllocated = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
  };

  using MemoryPropertyFlags = Flags<MemoryPropertyFlagBits, VkMemoryPropertyFlags>;

  VULKAN_HPP_INLINE MemoryPropertyFlags operator|( MemoryPropertyFlagBits bit0, MemoryPropertyFlagBits bit1 )
  {
    return MemoryPropertyFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE MemoryPropertyFlags operator~( MemoryPropertyFlagBits bits )
  {
    return ~( MemoryPropertyFlags( bits ) );
  }

  template <> struct FlagTraits<MemoryPropertyFlagBits>
  {
    enum
    {
      allFlags = VkFlags(MemoryPropertyFlagBits::eDeviceLocal) | VkFlags(MemoryPropertyFlagBits::eHostVisible) | VkFlags(MemoryPropertyFlagBits::eHostCoherent) | VkFlags(MemoryPropertyFlagBits::eHostCached) | VkFlags(MemoryPropertyFlagBits::eLazilyAllocated)
    };
  };

  struct MemoryType
  {
    operator const VkMemoryType&() const
    {
      return *reinterpret_cast<const VkMemoryType*>(this);
    }

    bool operator==( MemoryType const& rhs ) const
    {
      return ( propertyFlags == rhs.propertyFlags )
          && ( heapIndex == rhs.heapIndex );
    }

    bool operator!=( MemoryType const& rhs ) const
    {
      return !operator==( rhs );
    }

    MemoryPropertyFlags propertyFlags;
    uint32_t heapIndex;
  };
  static_assert( sizeof( MemoryType ) == sizeof( VkMemoryType ), "struct and wrapper have different size!" );

  enum class MemoryHeapFlagBits
  {
    eDeviceLocal = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
  };

  using MemoryHeapFlags = Flags<MemoryHeapFlagBits, VkMemoryHeapFlags>;

  VULKAN_HPP_INLINE MemoryHeapFlags operator|( MemoryHeapFlagBits bit0, MemoryHeapFlagBits bit1 )
  {
    return MemoryHeapFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE MemoryHeapFlags operator~( MemoryHeapFlagBits bits )
  {
    return ~( MemoryHeapFlags( bits ) );
  }

  template <> struct FlagTraits<MemoryHeapFlagBits>
  {
    enum
    {
      allFlags = VkFlags(MemoryHeapFlagBits::eDeviceLocal)
    };
  };

  struct MemoryHeap
  {
    operator const VkMemoryHeap&() const
    {
      return *reinterpret_cast<const VkMemoryHeap*>(this);
    }

    bool operator==( MemoryHeap const& rhs ) const
    {
      return ( size == rhs.size )
          && ( flags == rhs.flags );
    }

    bool operator!=( MemoryHeap const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize size;
    MemoryHeapFlags flags;
  };
  static_assert( sizeof( MemoryHeap ) == sizeof( VkMemoryHeap ), "struct and wrapper have different size!" );

  struct PhysicalDeviceMemoryProperties
  {
    operator const VkPhysicalDeviceMemoryProperties&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceMemoryProperties*>(this);
    }

    bool operator==( PhysicalDeviceMemoryProperties const& rhs ) const
    {
      return ( memoryTypeCount == rhs.memoryTypeCount )
          && ( memcmp( memoryTypes, rhs.memoryTypes, VK_MAX_MEMORY_TYPES * sizeof( MemoryType ) ) == 0 )
          && ( memoryHeapCount == rhs.memoryHeapCount )
          && ( memcmp( memoryHeaps, rhs.memoryHeaps, VK_MAX_MEMORY_HEAPS * sizeof( MemoryHeap ) ) == 0 );
    }

    bool operator!=( PhysicalDeviceMemoryProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t memoryTypeCount;
    MemoryType memoryTypes[VK_MAX_MEMORY_TYPES];
    uint32_t memoryHeapCount;
    MemoryHeap memoryHeaps[VK_MAX_MEMORY_HEAPS];
  };
  static_assert( sizeof( PhysicalDeviceMemoryProperties ) == sizeof( VkPhysicalDeviceMemoryProperties ), "struct and wrapper have different size!" );

  struct PhysicalDeviceMemoryProperties2KHR
  {
    operator const VkPhysicalDeviceMemoryProperties2KHR&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceMemoryProperties2KHR*>(this);
    }

    bool operator==( PhysicalDeviceMemoryProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( memoryProperties == rhs.memoryProperties );
    }

    bool operator!=( PhysicalDeviceMemoryProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    PhysicalDeviceMemoryProperties memoryProperties;
  };
  static_assert( sizeof( PhysicalDeviceMemoryProperties2KHR ) == sizeof( VkPhysicalDeviceMemoryProperties2KHR ), "struct and wrapper have different size!" );

  enum class AccessFlagBits
  {
    eIndirectCommandRead = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    eIndexRead = VK_ACCESS_INDEX_READ_BIT,
    eVertexAttributeRead = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    eUniformRead = VK_ACCESS_UNIFORM_READ_BIT,
    eInputAttachmentRead = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    eShaderRead = VK_ACCESS_SHADER_READ_BIT,
    eShaderWrite = VK_ACCESS_SHADER_WRITE_BIT,
    eColorAttachmentRead = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    eColorAttachmentWrite = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    eDepthStencilAttachmentRead = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    eDepthStencilAttachmentWrite = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    eTransferRead = VK_ACCESS_TRANSFER_READ_BIT,
    eTransferWrite = VK_ACCESS_TRANSFER_WRITE_BIT,
    eHostRead = VK_ACCESS_HOST_READ_BIT,
    eHostWrite = VK_ACCESS_HOST_WRITE_BIT,
    eMemoryRead = VK_ACCESS_MEMORY_READ_BIT,
    eMemoryWrite = VK_ACCESS_MEMORY_WRITE_BIT,
    eCommandProcessReadNVX = VK_ACCESS_COMMAND_PROCESS_READ_BIT_NVX,
    eCommandProcessWriteNVX = VK_ACCESS_COMMAND_PROCESS_WRITE_BIT_NVX
  };

  using AccessFlags = Flags<AccessFlagBits, VkAccessFlags>;

  VULKAN_HPP_INLINE AccessFlags operator|( AccessFlagBits bit0, AccessFlagBits bit1 )
  {
    return AccessFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE AccessFlags operator~( AccessFlagBits bits )
  {
    return ~( AccessFlags( bits ) );
  }

  template <> struct FlagTraits<AccessFlagBits>
  {
    enum
    {
      allFlags = VkFlags(AccessFlagBits::eIndirectCommandRead) | VkFlags(AccessFlagBits::eIndexRead) | VkFlags(AccessFlagBits::eVertexAttributeRead) | VkFlags(AccessFlagBits::eUniformRead) | VkFlags(AccessFlagBits::eInputAttachmentRead) | VkFlags(AccessFlagBits::eShaderRead) | VkFlags(AccessFlagBits::eShaderWrite) | VkFlags(AccessFlagBits::eColorAttachmentRead) | VkFlags(AccessFlagBits::eColorAttachmentWrite) | VkFlags(AccessFlagBits::eDepthStencilAttachmentRead) | VkFlags(AccessFlagBits::eDepthStencilAttachmentWrite) | VkFlags(AccessFlagBits::eTransferRead) | VkFlags(AccessFlagBits::eTransferWrite) | VkFlags(AccessFlagBits::eHostRead) | VkFlags(AccessFlagBits::eHostWrite) | VkFlags(AccessFlagBits::eMemoryRead) | VkFlags(AccessFlagBits::eMemoryWrite) | VkFlags(AccessFlagBits::eCommandProcessReadNVX) | VkFlags(AccessFlagBits::eCommandProcessWriteNVX)
    };
  };

  struct MemoryBarrier
  {
    MemoryBarrier( AccessFlags srcAccessMask_ = AccessFlags(), AccessFlags dstAccessMask_ = AccessFlags() )
      : sType( StructureType::eMemoryBarrier )
      , pNext( nullptr )
      , srcAccessMask( srcAccessMask_ )
      , dstAccessMask( dstAccessMask_ )
    {
    }

    MemoryBarrier( VkMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(MemoryBarrier) );
    }

    MemoryBarrier& operator=( VkMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(MemoryBarrier) );
      return *this;
    }

    MemoryBarrier& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    MemoryBarrier& setSrcAccessMask( AccessFlags srcAccessMask_ )
    {
      srcAccessMask = srcAccessMask_;
      return *this;
    }

    MemoryBarrier& setDstAccessMask( AccessFlags dstAccessMask_ )
    {
      dstAccessMask = dstAccessMask_;
      return *this;
    }

    operator const VkMemoryBarrier&() const
    {
      return *reinterpret_cast<const VkMemoryBarrier*>(this);
    }

    bool operator==( MemoryBarrier const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( srcAccessMask == rhs.srcAccessMask )
          && ( dstAccessMask == rhs.dstAccessMask );
    }

    bool operator!=( MemoryBarrier const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    AccessFlags srcAccessMask;
    AccessFlags dstAccessMask;
  };
  static_assert( sizeof( MemoryBarrier ) == sizeof( VkMemoryBarrier ), "struct and wrapper have different size!" );

  struct BufferMemoryBarrier
  {
    BufferMemoryBarrier( AccessFlags srcAccessMask_ = AccessFlags(), AccessFlags dstAccessMask_ = AccessFlags(), uint32_t srcQueueFamilyIndex_ = 0, uint32_t dstQueueFamilyIndex_ = 0, Buffer buffer_ = Buffer(), DeviceSize offset_ = 0, DeviceSize size_ = 0 )
      : sType( StructureType::eBufferMemoryBarrier )
      , pNext( nullptr )
      , srcAccessMask( srcAccessMask_ )
      , dstAccessMask( dstAccessMask_ )
      , srcQueueFamilyIndex( srcQueueFamilyIndex_ )
      , dstQueueFamilyIndex( dstQueueFamilyIndex_ )
      , buffer( buffer_ )
      , offset( offset_ )
      , size( size_ )
    {
    }

    BufferMemoryBarrier( VkBufferMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferMemoryBarrier) );
    }

    BufferMemoryBarrier& operator=( VkBufferMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferMemoryBarrier) );
      return *this;
    }

    BufferMemoryBarrier& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    BufferMemoryBarrier& setSrcAccessMask( AccessFlags srcAccessMask_ )
    {
      srcAccessMask = srcAccessMask_;
      return *this;
    }

    BufferMemoryBarrier& setDstAccessMask( AccessFlags dstAccessMask_ )
    {
      dstAccessMask = dstAccessMask_;
      return *this;
    }

    BufferMemoryBarrier& setSrcQueueFamilyIndex( uint32_t srcQueueFamilyIndex_ )
    {
      srcQueueFamilyIndex = srcQueueFamilyIndex_;
      return *this;
    }

    BufferMemoryBarrier& setDstQueueFamilyIndex( uint32_t dstQueueFamilyIndex_ )
    {
      dstQueueFamilyIndex = dstQueueFamilyIndex_;
      return *this;
    }

    BufferMemoryBarrier& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    BufferMemoryBarrier& setOffset( DeviceSize offset_ )
    {
      offset = offset_;
      return *this;
    }

    BufferMemoryBarrier& setSize( DeviceSize size_ )
    {
      size = size_;
      return *this;
    }

    operator const VkBufferMemoryBarrier&() const
    {
      return *reinterpret_cast<const VkBufferMemoryBarrier*>(this);
    }

    bool operator==( BufferMemoryBarrier const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( srcAccessMask == rhs.srcAccessMask )
          && ( dstAccessMask == rhs.dstAccessMask )
          && ( srcQueueFamilyIndex == rhs.srcQueueFamilyIndex )
          && ( dstQueueFamilyIndex == rhs.dstQueueFamilyIndex )
          && ( buffer == rhs.buffer )
          && ( offset == rhs.offset )
          && ( size == rhs.size );
    }

    bool operator!=( BufferMemoryBarrier const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    AccessFlags srcAccessMask;
    AccessFlags dstAccessMask;
    uint32_t srcQueueFamilyIndex;
    uint32_t dstQueueFamilyIndex;
    Buffer buffer;
    DeviceSize offset;
    DeviceSize size;
  };
  static_assert( sizeof( BufferMemoryBarrier ) == sizeof( VkBufferMemoryBarrier ), "struct and wrapper have different size!" );

  enum class BufferUsageFlagBits
  {
    eTransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    eTransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    eUniformTexelBuffer = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
    eStorageTexelBuffer = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
    eUniformBuffer = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    eStorageBuffer = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    eIndexBuffer = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    eVertexBuffer = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    eIndirectBuffer = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
  };

  using BufferUsageFlags = Flags<BufferUsageFlagBits, VkBufferUsageFlags>;

  VULKAN_HPP_INLINE BufferUsageFlags operator|( BufferUsageFlagBits bit0, BufferUsageFlagBits bit1 )
  {
    return BufferUsageFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE BufferUsageFlags operator~( BufferUsageFlagBits bits )
  {
    return ~( BufferUsageFlags( bits ) );
  }

  template <> struct FlagTraits<BufferUsageFlagBits>
  {
    enum
    {
      allFlags = VkFlags(BufferUsageFlagBits::eTransferSrc) | VkFlags(BufferUsageFlagBits::eTransferDst) | VkFlags(BufferUsageFlagBits::eUniformTexelBuffer) | VkFlags(BufferUsageFlagBits::eStorageTexelBuffer) | VkFlags(BufferUsageFlagBits::eUniformBuffer) | VkFlags(BufferUsageFlagBits::eStorageBuffer) | VkFlags(BufferUsageFlagBits::eIndexBuffer) | VkFlags(BufferUsageFlagBits::eVertexBuffer) | VkFlags(BufferUsageFlagBits::eIndirectBuffer)
    };
  };

  enum class BufferCreateFlagBits
  {
    eSparseBinding = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT
  };

  using BufferCreateFlags = Flags<BufferCreateFlagBits, VkBufferCreateFlags>;

  VULKAN_HPP_INLINE BufferCreateFlags operator|( BufferCreateFlagBits bit0, BufferCreateFlagBits bit1 )
  {
    return BufferCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE BufferCreateFlags operator~( BufferCreateFlagBits bits )
  {
    return ~( BufferCreateFlags( bits ) );
  }

  template <> struct FlagTraits<BufferCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(BufferCreateFlagBits::eSparseBinding) | VkFlags(BufferCreateFlagBits::eSparseResidency) | VkFlags(BufferCreateFlagBits::eSparseAliased)
    };
  };

  struct BufferCreateInfo
  {
    BufferCreateInfo( BufferCreateFlags flags_ = BufferCreateFlags(), DeviceSize size_ = 0, BufferUsageFlags usage_ = BufferUsageFlags(), SharingMode sharingMode_ = SharingMode::eExclusive, uint32_t queueFamilyIndexCount_ = 0, const uint32_t* pQueueFamilyIndices_ = nullptr )
      : sType( StructureType::eBufferCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , size( size_ )
      , usage( usage_ )
      , sharingMode( sharingMode_ )
      , queueFamilyIndexCount( queueFamilyIndexCount_ )
      , pQueueFamilyIndices( pQueueFamilyIndices_ )
    {
    }

    BufferCreateInfo( VkBufferCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferCreateInfo) );
    }

    BufferCreateInfo& operator=( VkBufferCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferCreateInfo) );
      return *this;
    }

    BufferCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    BufferCreateInfo& setFlags( BufferCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    BufferCreateInfo& setSize( DeviceSize size_ )
    {
      size = size_;
      return *this;
    }

    BufferCreateInfo& setUsage( BufferUsageFlags usage_ )
    {
      usage = usage_;
      return *this;
    }

    BufferCreateInfo& setSharingMode( SharingMode sharingMode_ )
    {
      sharingMode = sharingMode_;
      return *this;
    }

    BufferCreateInfo& setQueueFamilyIndexCount( uint32_t queueFamilyIndexCount_ )
    {
      queueFamilyIndexCount = queueFamilyIndexCount_;
      return *this;
    }

    BufferCreateInfo& setPQueueFamilyIndices( const uint32_t* pQueueFamilyIndices_ )
    {
      pQueueFamilyIndices = pQueueFamilyIndices_;
      return *this;
    }

    operator const VkBufferCreateInfo&() const
    {
      return *reinterpret_cast<const VkBufferCreateInfo*>(this);
    }

    bool operator==( BufferCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( size == rhs.size )
          && ( usage == rhs.usage )
          && ( sharingMode == rhs.sharingMode )
          && ( queueFamilyIndexCount == rhs.queueFamilyIndexCount )
          && ( pQueueFamilyIndices == rhs.pQueueFamilyIndices );
    }

    bool operator!=( BufferCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    BufferCreateFlags flags;
    DeviceSize size;
    BufferUsageFlags usage;
    SharingMode sharingMode;
    uint32_t queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
  };
  static_assert( sizeof( BufferCreateInfo ) == sizeof( VkBufferCreateInfo ), "struct and wrapper have different size!" );

  enum class ShaderStageFlagBits
  {
    eVertex = VK_SHADER_STAGE_VERTEX_BIT,
    eTessellationControl = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    eTessellationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    eGeometry = VK_SHADER_STAGE_GEOMETRY_BIT,
    eFragment = VK_SHADER_STAGE_FRAGMENT_BIT,
    eCompute = VK_SHADER_STAGE_COMPUTE_BIT,
    eAllGraphics = VK_SHADER_STAGE_ALL_GRAPHICS,
    eAll = VK_SHADER_STAGE_ALL
  };

  using ShaderStageFlags = Flags<ShaderStageFlagBits, VkShaderStageFlags>;

  VULKAN_HPP_INLINE ShaderStageFlags operator|( ShaderStageFlagBits bit0, ShaderStageFlagBits bit1 )
  {
    return ShaderStageFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ShaderStageFlags operator~( ShaderStageFlagBits bits )
  {
    return ~( ShaderStageFlags( bits ) );
  }

  template <> struct FlagTraits<ShaderStageFlagBits>
  {
    enum
    {
      allFlags = VkFlags(ShaderStageFlagBits::eVertex) | VkFlags(ShaderStageFlagBits::eTessellationControl) | VkFlags(ShaderStageFlagBits::eTessellationEvaluation) | VkFlags(ShaderStageFlagBits::eGeometry) | VkFlags(ShaderStageFlagBits::eFragment) | VkFlags(ShaderStageFlagBits::eCompute) | VkFlags(ShaderStageFlagBits::eAllGraphics) | VkFlags(ShaderStageFlagBits::eAll)
    };
  };

  struct DescriptorSetLayoutBinding
  {
    DescriptorSetLayoutBinding( uint32_t binding_ = 0, DescriptorType descriptorType_ = DescriptorType::eSampler, uint32_t descriptorCount_ = 0, ShaderStageFlags stageFlags_ = ShaderStageFlags(), const Sampler* pImmutableSamplers_ = nullptr )
      : binding( binding_ )
      , descriptorType( descriptorType_ )
      , descriptorCount( descriptorCount_ )
      , stageFlags( stageFlags_ )
      , pImmutableSamplers( pImmutableSamplers_ )
    {
    }

    DescriptorSetLayoutBinding( VkDescriptorSetLayoutBinding const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetLayoutBinding) );
    }

    DescriptorSetLayoutBinding& operator=( VkDescriptorSetLayoutBinding const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetLayoutBinding) );
      return *this;
    }

    DescriptorSetLayoutBinding& setBinding( uint32_t binding_ )
    {
      binding = binding_;
      return *this;
    }

    DescriptorSetLayoutBinding& setDescriptorType( DescriptorType descriptorType_ )
    {
      descriptorType = descriptorType_;
      return *this;
    }

    DescriptorSetLayoutBinding& setDescriptorCount( uint32_t descriptorCount_ )
    {
      descriptorCount = descriptorCount_;
      return *this;
    }

    DescriptorSetLayoutBinding& setStageFlags( ShaderStageFlags stageFlags_ )
    {
      stageFlags = stageFlags_;
      return *this;
    }

    DescriptorSetLayoutBinding& setPImmutableSamplers( const Sampler* pImmutableSamplers_ )
    {
      pImmutableSamplers = pImmutableSamplers_;
      return *this;
    }

    operator const VkDescriptorSetLayoutBinding&() const
    {
      return *reinterpret_cast<const VkDescriptorSetLayoutBinding*>(this);
    }

    bool operator==( DescriptorSetLayoutBinding const& rhs ) const
    {
      return ( binding == rhs.binding )
          && ( descriptorType == rhs.descriptorType )
          && ( descriptorCount == rhs.descriptorCount )
          && ( stageFlags == rhs.stageFlags )
          && ( pImmutableSamplers == rhs.pImmutableSamplers );
    }

    bool operator!=( DescriptorSetLayoutBinding const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t binding;
    DescriptorType descriptorType;
    uint32_t descriptorCount;
    ShaderStageFlags stageFlags;
    const Sampler* pImmutableSamplers;
  };
  static_assert( sizeof( DescriptorSetLayoutBinding ) == sizeof( VkDescriptorSetLayoutBinding ), "struct and wrapper have different size!" );

  struct DescriptorSetLayoutCreateInfo
  {
    DescriptorSetLayoutCreateInfo( DescriptorSetLayoutCreateFlags flags_ = DescriptorSetLayoutCreateFlags(), uint32_t bindingCount_ = 0, const DescriptorSetLayoutBinding* pBindings_ = nullptr )
      : sType( StructureType::eDescriptorSetLayoutCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , bindingCount( bindingCount_ )
      , pBindings( pBindings_ )
    {
    }

    DescriptorSetLayoutCreateInfo( VkDescriptorSetLayoutCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetLayoutCreateInfo) );
    }

    DescriptorSetLayoutCreateInfo& operator=( VkDescriptorSetLayoutCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorSetLayoutCreateInfo) );
      return *this;
    }

    DescriptorSetLayoutCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DescriptorSetLayoutCreateInfo& setFlags( DescriptorSetLayoutCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    DescriptorSetLayoutCreateInfo& setBindingCount( uint32_t bindingCount_ )
    {
      bindingCount = bindingCount_;
      return *this;
    }

    DescriptorSetLayoutCreateInfo& setPBindings( const DescriptorSetLayoutBinding* pBindings_ )
    {
      pBindings = pBindings_;
      return *this;
    }

    operator const VkDescriptorSetLayoutCreateInfo&() const
    {
      return *reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>(this);
    }

    bool operator==( DescriptorSetLayoutCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( bindingCount == rhs.bindingCount )
          && ( pBindings == rhs.pBindings );
    }

    bool operator!=( DescriptorSetLayoutCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DescriptorSetLayoutCreateFlags flags;
    uint32_t bindingCount;
    const DescriptorSetLayoutBinding* pBindings;
  };
  static_assert( sizeof( DescriptorSetLayoutCreateInfo ) == sizeof( VkDescriptorSetLayoutCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineShaderStageCreateInfo
  {
    PipelineShaderStageCreateInfo( PipelineShaderStageCreateFlags flags_ = PipelineShaderStageCreateFlags(), ShaderStageFlagBits stage_ = ShaderStageFlagBits::eVertex, ShaderModule module_ = ShaderModule(), const char* pName_ = nullptr, const SpecializationInfo* pSpecializationInfo_ = nullptr )
      : sType( StructureType::ePipelineShaderStageCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , stage( stage_ )
      , module( module_ )
      , pName( pName_ )
      , pSpecializationInfo( pSpecializationInfo_ )
    {
    }

    PipelineShaderStageCreateInfo( VkPipelineShaderStageCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineShaderStageCreateInfo) );
    }

    PipelineShaderStageCreateInfo& operator=( VkPipelineShaderStageCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineShaderStageCreateInfo) );
      return *this;
    }

    PipelineShaderStageCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineShaderStageCreateInfo& setFlags( PipelineShaderStageCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineShaderStageCreateInfo& setStage( ShaderStageFlagBits stage_ )
    {
      stage = stage_;
      return *this;
    }

    PipelineShaderStageCreateInfo& setModule( ShaderModule module_ )
    {
      module = module_;
      return *this;
    }

    PipelineShaderStageCreateInfo& setPName( const char* pName_ )
    {
      pName = pName_;
      return *this;
    }

    PipelineShaderStageCreateInfo& setPSpecializationInfo( const SpecializationInfo* pSpecializationInfo_ )
    {
      pSpecializationInfo = pSpecializationInfo_;
      return *this;
    }

    operator const VkPipelineShaderStageCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineShaderStageCreateInfo*>(this);
    }

    bool operator==( PipelineShaderStageCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( stage == rhs.stage )
          && ( module == rhs.module )
          && ( pName == rhs.pName )
          && ( pSpecializationInfo == rhs.pSpecializationInfo );
    }

    bool operator!=( PipelineShaderStageCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineShaderStageCreateFlags flags;
    ShaderStageFlagBits stage;
    ShaderModule module;
    const char* pName;
    const SpecializationInfo* pSpecializationInfo;
  };
  static_assert( sizeof( PipelineShaderStageCreateInfo ) == sizeof( VkPipelineShaderStageCreateInfo ), "struct and wrapper have different size!" );

  struct PushConstantRange
  {
    PushConstantRange( ShaderStageFlags stageFlags_ = ShaderStageFlags(), uint32_t offset_ = 0, uint32_t size_ = 0 )
      : stageFlags( stageFlags_ )
      , offset( offset_ )
      , size( size_ )
    {
    }

    PushConstantRange( VkPushConstantRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(PushConstantRange) );
    }

    PushConstantRange& operator=( VkPushConstantRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(PushConstantRange) );
      return *this;
    }

    PushConstantRange& setStageFlags( ShaderStageFlags stageFlags_ )
    {
      stageFlags = stageFlags_;
      return *this;
    }

    PushConstantRange& setOffset( uint32_t offset_ )
    {
      offset = offset_;
      return *this;
    }

    PushConstantRange& setSize( uint32_t size_ )
    {
      size = size_;
      return *this;
    }

    operator const VkPushConstantRange&() const
    {
      return *reinterpret_cast<const VkPushConstantRange*>(this);
    }

    bool operator==( PushConstantRange const& rhs ) const
    {
      return ( stageFlags == rhs.stageFlags )
          && ( offset == rhs.offset )
          && ( size == rhs.size );
    }

    bool operator!=( PushConstantRange const& rhs ) const
    {
      return !operator==( rhs );
    }

    ShaderStageFlags stageFlags;
    uint32_t offset;
    uint32_t size;
  };
  static_assert( sizeof( PushConstantRange ) == sizeof( VkPushConstantRange ), "struct and wrapper have different size!" );

  struct PipelineLayoutCreateInfo
  {
    PipelineLayoutCreateInfo( PipelineLayoutCreateFlags flags_ = PipelineLayoutCreateFlags(), uint32_t setLayoutCount_ = 0, const DescriptorSetLayout* pSetLayouts_ = nullptr, uint32_t pushConstantRangeCount_ = 0, const PushConstantRange* pPushConstantRanges_ = nullptr )
      : sType( StructureType::ePipelineLayoutCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , setLayoutCount( setLayoutCount_ )
      , pSetLayouts( pSetLayouts_ )
      , pushConstantRangeCount( pushConstantRangeCount_ )
      , pPushConstantRanges( pPushConstantRanges_ )
    {
    }

    PipelineLayoutCreateInfo( VkPipelineLayoutCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineLayoutCreateInfo) );
    }

    PipelineLayoutCreateInfo& operator=( VkPipelineLayoutCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineLayoutCreateInfo) );
      return *this;
    }

    PipelineLayoutCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineLayoutCreateInfo& setFlags( PipelineLayoutCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineLayoutCreateInfo& setSetLayoutCount( uint32_t setLayoutCount_ )
    {
      setLayoutCount = setLayoutCount_;
      return *this;
    }

    PipelineLayoutCreateInfo& setPSetLayouts( const DescriptorSetLayout* pSetLayouts_ )
    {
      pSetLayouts = pSetLayouts_;
      return *this;
    }

    PipelineLayoutCreateInfo& setPushConstantRangeCount( uint32_t pushConstantRangeCount_ )
    {
      pushConstantRangeCount = pushConstantRangeCount_;
      return *this;
    }

    PipelineLayoutCreateInfo& setPPushConstantRanges( const PushConstantRange* pPushConstantRanges_ )
    {
      pPushConstantRanges = pPushConstantRanges_;
      return *this;
    }

    operator const VkPipelineLayoutCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineLayoutCreateInfo*>(this);
    }

    bool operator==( PipelineLayoutCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( setLayoutCount == rhs.setLayoutCount )
          && ( pSetLayouts == rhs.pSetLayouts )
          && ( pushConstantRangeCount == rhs.pushConstantRangeCount )
          && ( pPushConstantRanges == rhs.pPushConstantRanges );
    }

    bool operator!=( PipelineLayoutCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineLayoutCreateFlags flags;
    uint32_t setLayoutCount;
    const DescriptorSetLayout* pSetLayouts;
    uint32_t pushConstantRangeCount;
    const PushConstantRange* pPushConstantRanges;
  };
  static_assert( sizeof( PipelineLayoutCreateInfo ) == sizeof( VkPipelineLayoutCreateInfo ), "struct and wrapper have different size!" );

  enum class ImageUsageFlagBits
  {
    eTransferSrc = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    eTransferDst = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    eSampled = VK_IMAGE_USAGE_SAMPLED_BIT,
    eStorage = VK_IMAGE_USAGE_STORAGE_BIT,
    eColorAttachment = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    eDepthStencilAttachment = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    eTransientAttachment = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    eInputAttachment = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
  };

  using ImageUsageFlags = Flags<ImageUsageFlagBits, VkImageUsageFlags>;

  VULKAN_HPP_INLINE ImageUsageFlags operator|( ImageUsageFlagBits bit0, ImageUsageFlagBits bit1 )
  {
    return ImageUsageFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ImageUsageFlags operator~( ImageUsageFlagBits bits )
  {
    return ~( ImageUsageFlags( bits ) );
  }

  template <> struct FlagTraits<ImageUsageFlagBits>
  {
    enum
    {
      allFlags = VkFlags(ImageUsageFlagBits::eTransferSrc) | VkFlags(ImageUsageFlagBits::eTransferDst) | VkFlags(ImageUsageFlagBits::eSampled) | VkFlags(ImageUsageFlagBits::eStorage) | VkFlags(ImageUsageFlagBits::eColorAttachment) | VkFlags(ImageUsageFlagBits::eDepthStencilAttachment) | VkFlags(ImageUsageFlagBits::eTransientAttachment) | VkFlags(ImageUsageFlagBits::eInputAttachment)
    };
  };

  enum class ImageCreateFlagBits
  {
    eSparseBinding = VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency = VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased = VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
    eMutableFormat = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
    eCubeCompatible = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
    e2DArrayCompatibleKHR = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR
  };

  using ImageCreateFlags = Flags<ImageCreateFlagBits, VkImageCreateFlags>;

  VULKAN_HPP_INLINE ImageCreateFlags operator|( ImageCreateFlagBits bit0, ImageCreateFlagBits bit1 )
  {
    return ImageCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ImageCreateFlags operator~( ImageCreateFlagBits bits )
  {
    return ~( ImageCreateFlags( bits ) );
  }

  template <> struct FlagTraits<ImageCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(ImageCreateFlagBits::eSparseBinding) | VkFlags(ImageCreateFlagBits::eSparseResidency) | VkFlags(ImageCreateFlagBits::eSparseAliased) | VkFlags(ImageCreateFlagBits::eMutableFormat) | VkFlags(ImageCreateFlagBits::eCubeCompatible) | VkFlags(ImageCreateFlagBits::e2DArrayCompatibleKHR)
    };
  };

  struct PhysicalDeviceImageFormatInfo2KHR
  {
    PhysicalDeviceImageFormatInfo2KHR( Format format_ = Format::eUndefined, ImageType type_ = ImageType::e1D, ImageTiling tiling_ = ImageTiling::eOptimal, ImageUsageFlags usage_ = ImageUsageFlags(), ImageCreateFlags flags_ = ImageCreateFlags() )
      : sType( StructureType::ePhysicalDeviceImageFormatInfo2KHR )
      , pNext( nullptr )
      , format( format_ )
      , type( type_ )
      , tiling( tiling_ )
      , usage( usage_ )
      , flags( flags_ )
    {
    }

    PhysicalDeviceImageFormatInfo2KHR( VkPhysicalDeviceImageFormatInfo2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceImageFormatInfo2KHR) );
    }

    PhysicalDeviceImageFormatInfo2KHR& operator=( VkPhysicalDeviceImageFormatInfo2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceImageFormatInfo2KHR) );
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setType( ImageType type_ )
    {
      type = type_;
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setTiling( ImageTiling tiling_ )
    {
      tiling = tiling_;
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setUsage( ImageUsageFlags usage_ )
    {
      usage = usage_;
      return *this;
    }

    PhysicalDeviceImageFormatInfo2KHR& setFlags( ImageCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkPhysicalDeviceImageFormatInfo2KHR&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2KHR*>(this);
    }

    bool operator==( PhysicalDeviceImageFormatInfo2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( format == rhs.format )
          && ( type == rhs.type )
          && ( tiling == rhs.tiling )
          && ( usage == rhs.usage )
          && ( flags == rhs.flags );
    }

    bool operator!=( PhysicalDeviceImageFormatInfo2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Format format;
    ImageType type;
    ImageTiling tiling;
    ImageUsageFlags usage;
    ImageCreateFlags flags;
  };
  static_assert( sizeof( PhysicalDeviceImageFormatInfo2KHR ) == sizeof( VkPhysicalDeviceImageFormatInfo2KHR ), "struct and wrapper have different size!" );

  enum class PipelineCreateFlagBits
  {
    eDisableOptimization = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT,
    eAllowDerivatives = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
    eDerivative = VK_PIPELINE_CREATE_DERIVATIVE_BIT
  };

  using PipelineCreateFlags = Flags<PipelineCreateFlagBits, VkPipelineCreateFlags>;

  VULKAN_HPP_INLINE PipelineCreateFlags operator|( PipelineCreateFlagBits bit0, PipelineCreateFlagBits bit1 )
  {
    return PipelineCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE PipelineCreateFlags operator~( PipelineCreateFlagBits bits )
  {
    return ~( PipelineCreateFlags( bits ) );
  }

  template <> struct FlagTraits<PipelineCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(PipelineCreateFlagBits::eDisableOptimization) | VkFlags(PipelineCreateFlagBits::eAllowDerivatives) | VkFlags(PipelineCreateFlagBits::eDerivative)
    };
  };

  struct ComputePipelineCreateInfo
  {
    ComputePipelineCreateInfo( PipelineCreateFlags flags_ = PipelineCreateFlags(), PipelineShaderStageCreateInfo stage_ = PipelineShaderStageCreateInfo(), PipelineLayout layout_ = PipelineLayout(), Pipeline basePipelineHandle_ = Pipeline(), int32_t basePipelineIndex_ = 0 )
      : sType( StructureType::eComputePipelineCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , stage( stage_ )
      , layout( layout_ )
      , basePipelineHandle( basePipelineHandle_ )
      , basePipelineIndex( basePipelineIndex_ )
    {
    }

    ComputePipelineCreateInfo( VkComputePipelineCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ComputePipelineCreateInfo) );
    }

    ComputePipelineCreateInfo& operator=( VkComputePipelineCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ComputePipelineCreateInfo) );
      return *this;
    }

    ComputePipelineCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ComputePipelineCreateInfo& setFlags( PipelineCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    ComputePipelineCreateInfo& setStage( PipelineShaderStageCreateInfo stage_ )
    {
      stage = stage_;
      return *this;
    }

    ComputePipelineCreateInfo& setLayout( PipelineLayout layout_ )
    {
      layout = layout_;
      return *this;
    }

    ComputePipelineCreateInfo& setBasePipelineHandle( Pipeline basePipelineHandle_ )
    {
      basePipelineHandle = basePipelineHandle_;
      return *this;
    }

    ComputePipelineCreateInfo& setBasePipelineIndex( int32_t basePipelineIndex_ )
    {
      basePipelineIndex = basePipelineIndex_;
      return *this;
    }

    operator const VkComputePipelineCreateInfo&() const
    {
      return *reinterpret_cast<const VkComputePipelineCreateInfo*>(this);
    }

    bool operator==( ComputePipelineCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( stage == rhs.stage )
          && ( layout == rhs.layout )
          && ( basePipelineHandle == rhs.basePipelineHandle )
          && ( basePipelineIndex == rhs.basePipelineIndex );
    }

    bool operator!=( ComputePipelineCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineCreateFlags flags;
    PipelineShaderStageCreateInfo stage;
    PipelineLayout layout;
    Pipeline basePipelineHandle;
    int32_t basePipelineIndex;
  };
  static_assert( sizeof( ComputePipelineCreateInfo ) == sizeof( VkComputePipelineCreateInfo ), "struct and wrapper have different size!" );

  enum class ColorComponentFlagBits
  {
    eR = VK_COLOR_COMPONENT_R_BIT,
    eG = VK_COLOR_COMPONENT_G_BIT,
    eB = VK_COLOR_COMPONENT_B_BIT,
    eA = VK_COLOR_COMPONENT_A_BIT
  };

  using ColorComponentFlags = Flags<ColorComponentFlagBits, VkColorComponentFlags>;

  VULKAN_HPP_INLINE ColorComponentFlags operator|( ColorComponentFlagBits bit0, ColorComponentFlagBits bit1 )
  {
    return ColorComponentFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ColorComponentFlags operator~( ColorComponentFlagBits bits )
  {
    return ~( ColorComponentFlags( bits ) );
  }

  template <> struct FlagTraits<ColorComponentFlagBits>
  {
    enum
    {
      allFlags = VkFlags(ColorComponentFlagBits::eR) | VkFlags(ColorComponentFlagBits::eG) | VkFlags(ColorComponentFlagBits::eB) | VkFlags(ColorComponentFlagBits::eA)
    };
  };

  struct PipelineColorBlendAttachmentState
  {
    PipelineColorBlendAttachmentState( Bool32 blendEnable_ = 0, BlendFactor srcColorBlendFactor_ = BlendFactor::eZero, BlendFactor dstColorBlendFactor_ = BlendFactor::eZero, BlendOp colorBlendOp_ = BlendOp::eAdd, BlendFactor srcAlphaBlendFactor_ = BlendFactor::eZero, BlendFactor dstAlphaBlendFactor_ = BlendFactor::eZero, BlendOp alphaBlendOp_ = BlendOp::eAdd, ColorComponentFlags colorWriteMask_ = ColorComponentFlags() )
      : blendEnable( blendEnable_ )
      , srcColorBlendFactor( srcColorBlendFactor_ )
      , dstColorBlendFactor( dstColorBlendFactor_ )
      , colorBlendOp( colorBlendOp_ )
      , srcAlphaBlendFactor( srcAlphaBlendFactor_ )
      , dstAlphaBlendFactor( dstAlphaBlendFactor_ )
      , alphaBlendOp( alphaBlendOp_ )
      , colorWriteMask( colorWriteMask_ )
    {
    }

    PipelineColorBlendAttachmentState( VkPipelineColorBlendAttachmentState const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineColorBlendAttachmentState) );
    }

    PipelineColorBlendAttachmentState& operator=( VkPipelineColorBlendAttachmentState const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineColorBlendAttachmentState) );
      return *this;
    }

    PipelineColorBlendAttachmentState& setBlendEnable( Bool32 blendEnable_ )
    {
      blendEnable = blendEnable_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setSrcColorBlendFactor( BlendFactor srcColorBlendFactor_ )
    {
      srcColorBlendFactor = srcColorBlendFactor_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setDstColorBlendFactor( BlendFactor dstColorBlendFactor_ )
    {
      dstColorBlendFactor = dstColorBlendFactor_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setColorBlendOp( BlendOp colorBlendOp_ )
    {
      colorBlendOp = colorBlendOp_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setSrcAlphaBlendFactor( BlendFactor srcAlphaBlendFactor_ )
    {
      srcAlphaBlendFactor = srcAlphaBlendFactor_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setDstAlphaBlendFactor( BlendFactor dstAlphaBlendFactor_ )
    {
      dstAlphaBlendFactor = dstAlphaBlendFactor_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setAlphaBlendOp( BlendOp alphaBlendOp_ )
    {
      alphaBlendOp = alphaBlendOp_;
      return *this;
    }

    PipelineColorBlendAttachmentState& setColorWriteMask( ColorComponentFlags colorWriteMask_ )
    {
      colorWriteMask = colorWriteMask_;
      return *this;
    }

    operator const VkPipelineColorBlendAttachmentState&() const
    {
      return *reinterpret_cast<const VkPipelineColorBlendAttachmentState*>(this);
    }

    bool operator==( PipelineColorBlendAttachmentState const& rhs ) const
    {
      return ( blendEnable == rhs.blendEnable )
          && ( srcColorBlendFactor == rhs.srcColorBlendFactor )
          && ( dstColorBlendFactor == rhs.dstColorBlendFactor )
          && ( colorBlendOp == rhs.colorBlendOp )
          && ( srcAlphaBlendFactor == rhs.srcAlphaBlendFactor )
          && ( dstAlphaBlendFactor == rhs.dstAlphaBlendFactor )
          && ( alphaBlendOp == rhs.alphaBlendOp )
          && ( colorWriteMask == rhs.colorWriteMask );
    }

    bool operator!=( PipelineColorBlendAttachmentState const& rhs ) const
    {
      return !operator==( rhs );
    }

    Bool32 blendEnable;
    BlendFactor srcColorBlendFactor;
    BlendFactor dstColorBlendFactor;
    BlendOp colorBlendOp;
    BlendFactor srcAlphaBlendFactor;
    BlendFactor dstAlphaBlendFactor;
    BlendOp alphaBlendOp;
    ColorComponentFlags colorWriteMask;
  };
  static_assert( sizeof( PipelineColorBlendAttachmentState ) == sizeof( VkPipelineColorBlendAttachmentState ), "struct and wrapper have different size!" );

  struct PipelineColorBlendStateCreateInfo
  {
    PipelineColorBlendStateCreateInfo( PipelineColorBlendStateCreateFlags flags_ = PipelineColorBlendStateCreateFlags(), Bool32 logicOpEnable_ = 0, LogicOp logicOp_ = LogicOp::eClear, uint32_t attachmentCount_ = 0, const PipelineColorBlendAttachmentState* pAttachments_ = nullptr, std::array<float,4> const& blendConstants_ = { { 0, 0, 0, 0 } } )
      : sType( StructureType::ePipelineColorBlendStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , logicOpEnable( logicOpEnable_ )
      , logicOp( logicOp_ )
      , attachmentCount( attachmentCount_ )
      , pAttachments( pAttachments_ )
    {
      memcpy( &blendConstants, blendConstants_.data(), 4 * sizeof( float ) );
    }

    PipelineColorBlendStateCreateInfo( VkPipelineColorBlendStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineColorBlendStateCreateInfo) );
    }

    PipelineColorBlendStateCreateInfo& operator=( VkPipelineColorBlendStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineColorBlendStateCreateInfo) );
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setFlags( PipelineColorBlendStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setLogicOpEnable( Bool32 logicOpEnable_ )
    {
      logicOpEnable = logicOpEnable_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setLogicOp( LogicOp logicOp_ )
    {
      logicOp = logicOp_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setAttachmentCount( uint32_t attachmentCount_ )
    {
      attachmentCount = attachmentCount_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setPAttachments( const PipelineColorBlendAttachmentState* pAttachments_ )
    {
      pAttachments = pAttachments_;
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setBlendConstants( std::array<float,4> blendConstants_ )
    {
      memcpy( &blendConstants, blendConstants_.data(), 4 * sizeof( float ) );
      return *this;
    }

    operator const VkPipelineColorBlendStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineColorBlendStateCreateInfo*>(this);
    }

    bool operator==( PipelineColorBlendStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( logicOpEnable == rhs.logicOpEnable )
          && ( logicOp == rhs.logicOp )
          && ( attachmentCount == rhs.attachmentCount )
          && ( pAttachments == rhs.pAttachments )
          && ( memcmp( blendConstants, rhs.blendConstants, 4 * sizeof( float ) ) == 0 );
    }

    bool operator!=( PipelineColorBlendStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineColorBlendStateCreateFlags flags;
    Bool32 logicOpEnable;
    LogicOp logicOp;
    uint32_t attachmentCount;
    const PipelineColorBlendAttachmentState* pAttachments;
    float blendConstants[4];
  };
  static_assert( sizeof( PipelineColorBlendStateCreateInfo ) == sizeof( VkPipelineColorBlendStateCreateInfo ), "struct and wrapper have different size!" );

  enum class FenceCreateFlagBits
  {
    eSignaled = VK_FENCE_CREATE_SIGNALED_BIT
  };

  using FenceCreateFlags = Flags<FenceCreateFlagBits, VkFenceCreateFlags>;

  VULKAN_HPP_INLINE FenceCreateFlags operator|( FenceCreateFlagBits bit0, FenceCreateFlagBits bit1 )
  {
    return FenceCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE FenceCreateFlags operator~( FenceCreateFlagBits bits )
  {
    return ~( FenceCreateFlags( bits ) );
  }

  template <> struct FlagTraits<FenceCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(FenceCreateFlagBits::eSignaled)
    };
  };

  struct FenceCreateInfo
  {
    FenceCreateInfo( FenceCreateFlags flags_ = FenceCreateFlags() )
      : sType( StructureType::eFenceCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
    {
    }

    FenceCreateInfo( VkFenceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(FenceCreateInfo) );
    }

    FenceCreateInfo& operator=( VkFenceCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(FenceCreateInfo) );
      return *this;
    }

    FenceCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    FenceCreateInfo& setFlags( FenceCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkFenceCreateInfo&() const
    {
      return *reinterpret_cast<const VkFenceCreateInfo*>(this);
    }

    bool operator==( FenceCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags );
    }

    bool operator!=( FenceCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    FenceCreateFlags flags;
  };
  static_assert( sizeof( FenceCreateInfo ) == sizeof( VkFenceCreateInfo ), "struct and wrapper have different size!" );

  enum class FormatFeatureFlagBits
  {
    eSampledImage = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
    eStorageImage = VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
    eStorageImageAtomic = VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
    eUniformTexelBuffer = VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
    eStorageTexelBuffer = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
    eStorageTexelBufferAtomic = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    eVertexBuffer = VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
    eColorAttachment = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
    eColorAttachmentBlend = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
    eDepthStencilAttachment = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
    eBlitSrc = VK_FORMAT_FEATURE_BLIT_SRC_BIT,
    eBlitDst = VK_FORMAT_FEATURE_BLIT_DST_BIT,
    eSampledImageFilterLinear = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
    eSampledImageFilterCubicIMG = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG,
    eTransferSrcKHR = VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR,
    eTransferDstKHR = VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR
  };

  using FormatFeatureFlags = Flags<FormatFeatureFlagBits, VkFormatFeatureFlags>;

  VULKAN_HPP_INLINE FormatFeatureFlags operator|( FormatFeatureFlagBits bit0, FormatFeatureFlagBits bit1 )
  {
    return FormatFeatureFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE FormatFeatureFlags operator~( FormatFeatureFlagBits bits )
  {
    return ~( FormatFeatureFlags( bits ) );
  }

  template <> struct FlagTraits<FormatFeatureFlagBits>
  {
    enum
    {
      allFlags = VkFlags(FormatFeatureFlagBits::eSampledImage) | VkFlags(FormatFeatureFlagBits::eStorageImage) | VkFlags(FormatFeatureFlagBits::eStorageImageAtomic) | VkFlags(FormatFeatureFlagBits::eUniformTexelBuffer) | VkFlags(FormatFeatureFlagBits::eStorageTexelBuffer) | VkFlags(FormatFeatureFlagBits::eStorageTexelBufferAtomic) | VkFlags(FormatFeatureFlagBits::eVertexBuffer) | VkFlags(FormatFeatureFlagBits::eColorAttachment) | VkFlags(FormatFeatureFlagBits::eColorAttachmentBlend) | VkFlags(FormatFeatureFlagBits::eDepthStencilAttachment) | VkFlags(FormatFeatureFlagBits::eBlitSrc) | VkFlags(FormatFeatureFlagBits::eBlitDst) | VkFlags(FormatFeatureFlagBits::eSampledImageFilterLinear) | VkFlags(FormatFeatureFlagBits::eSampledImageFilterCubicIMG) | VkFlags(FormatFeatureFlagBits::eTransferSrcKHR) | VkFlags(FormatFeatureFlagBits::eTransferDstKHR)
    };
  };

  struct FormatProperties
  {
    operator const VkFormatProperties&() const
    {
      return *reinterpret_cast<const VkFormatProperties*>(this);
    }

    bool operator==( FormatProperties const& rhs ) const
    {
      return ( linearTilingFeatures == rhs.linearTilingFeatures )
          && ( optimalTilingFeatures == rhs.optimalTilingFeatures )
          && ( bufferFeatures == rhs.bufferFeatures );
    }

    bool operator!=( FormatProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    FormatFeatureFlags linearTilingFeatures;
    FormatFeatureFlags optimalTilingFeatures;
    FormatFeatureFlags bufferFeatures;
  };
  static_assert( sizeof( FormatProperties ) == sizeof( VkFormatProperties ), "struct and wrapper have different size!" );

  struct FormatProperties2KHR
  {
    operator const VkFormatProperties2KHR&() const
    {
      return *reinterpret_cast<const VkFormatProperties2KHR*>(this);
    }

    bool operator==( FormatProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( formatProperties == rhs.formatProperties );
    }

    bool operator!=( FormatProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    FormatProperties formatProperties;
  };
  static_assert( sizeof( FormatProperties2KHR ) == sizeof( VkFormatProperties2KHR ), "struct and wrapper have different size!" );

  enum class QueryControlFlagBits
  {
    ePrecise = VK_QUERY_CONTROL_PRECISE_BIT
  };

  using QueryControlFlags = Flags<QueryControlFlagBits, VkQueryControlFlags>;

  VULKAN_HPP_INLINE QueryControlFlags operator|( QueryControlFlagBits bit0, QueryControlFlagBits bit1 )
  {
    return QueryControlFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE QueryControlFlags operator~( QueryControlFlagBits bits )
  {
    return ~( QueryControlFlags( bits ) );
  }

  template <> struct FlagTraits<QueryControlFlagBits>
  {
    enum
    {
      allFlags = VkFlags(QueryControlFlagBits::ePrecise)
    };
  };

  enum class QueryResultFlagBits
  {
    e64 = VK_QUERY_RESULT_64_BIT,
    eWait = VK_QUERY_RESULT_WAIT_BIT,
    eWithAvailability = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT,
    ePartial = VK_QUERY_RESULT_PARTIAL_BIT
  };

  using QueryResultFlags = Flags<QueryResultFlagBits, VkQueryResultFlags>;

  VULKAN_HPP_INLINE QueryResultFlags operator|( QueryResultFlagBits bit0, QueryResultFlagBits bit1 )
  {
    return QueryResultFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE QueryResultFlags operator~( QueryResultFlagBits bits )
  {
    return ~( QueryResultFlags( bits ) );
  }

  template <> struct FlagTraits<QueryResultFlagBits>
  {
    enum
    {
      allFlags = VkFlags(QueryResultFlagBits::e64) | VkFlags(QueryResultFlagBits::eWait) | VkFlags(QueryResultFlagBits::eWithAvailability) | VkFlags(QueryResultFlagBits::ePartial)
    };
  };

  enum class CommandBufferUsageFlagBits
  {
    eOneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    eRenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
    eSimultaneousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
  };

  using CommandBufferUsageFlags = Flags<CommandBufferUsageFlagBits, VkCommandBufferUsageFlags>;

  VULKAN_HPP_INLINE CommandBufferUsageFlags operator|( CommandBufferUsageFlagBits bit0, CommandBufferUsageFlagBits bit1 )
  {
    return CommandBufferUsageFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CommandBufferUsageFlags operator~( CommandBufferUsageFlagBits bits )
  {
    return ~( CommandBufferUsageFlags( bits ) );
  }

  template <> struct FlagTraits<CommandBufferUsageFlagBits>
  {
    enum
    {
      allFlags = VkFlags(CommandBufferUsageFlagBits::eOneTimeSubmit) | VkFlags(CommandBufferUsageFlagBits::eRenderPassContinue) | VkFlags(CommandBufferUsageFlagBits::eSimultaneousUse)
    };
  };

  enum class QueryPipelineStatisticFlagBits
  {
    eInputAssemblyVertices = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
    eInputAssemblyPrimitives = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
    eVertexShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
    eGeometryShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
    eGeometryShaderPrimitives = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
    eClippingInvocations = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
    eClippingPrimitives = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
    eFragmentShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
    eTessellationControlShaderPatches = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
    eTessellationEvaluationShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
    eComputeShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT
  };

  using QueryPipelineStatisticFlags = Flags<QueryPipelineStatisticFlagBits, VkQueryPipelineStatisticFlags>;

  VULKAN_HPP_INLINE QueryPipelineStatisticFlags operator|( QueryPipelineStatisticFlagBits bit0, QueryPipelineStatisticFlagBits bit1 )
  {
    return QueryPipelineStatisticFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE QueryPipelineStatisticFlags operator~( QueryPipelineStatisticFlagBits bits )
  {
    return ~( QueryPipelineStatisticFlags( bits ) );
  }

  template <> struct FlagTraits<QueryPipelineStatisticFlagBits>
  {
    enum
    {
      allFlags = VkFlags(QueryPipelineStatisticFlagBits::eInputAssemblyVertices) | VkFlags(QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives) | VkFlags(QueryPipelineStatisticFlagBits::eVertexShaderInvocations) | VkFlags(QueryPipelineStatisticFlagBits::eGeometryShaderInvocations) | VkFlags(QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives) | VkFlags(QueryPipelineStatisticFlagBits::eClippingInvocations) | VkFlags(QueryPipelineStatisticFlagBits::eClippingPrimitives) | VkFlags(QueryPipelineStatisticFlagBits::eFragmentShaderInvocations) | VkFlags(QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches) | VkFlags(QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations) | VkFlags(QueryPipelineStatisticFlagBits::eComputeShaderInvocations)
    };
  };

  struct CommandBufferInheritanceInfo
  {
    CommandBufferInheritanceInfo( RenderPass renderPass_ = RenderPass(), uint32_t subpass_ = 0, Framebuffer framebuffer_ = Framebuffer(), Bool32 occlusionQueryEnable_ = 0, QueryControlFlags queryFlags_ = QueryControlFlags(), QueryPipelineStatisticFlags pipelineStatistics_ = QueryPipelineStatisticFlags() )
      : sType( StructureType::eCommandBufferInheritanceInfo )
      , pNext( nullptr )
      , renderPass( renderPass_ )
      , subpass( subpass_ )
      , framebuffer( framebuffer_ )
      , occlusionQueryEnable( occlusionQueryEnable_ )
      , queryFlags( queryFlags_ )
      , pipelineStatistics( pipelineStatistics_ )
    {
    }

    CommandBufferInheritanceInfo( VkCommandBufferInheritanceInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferInheritanceInfo) );
    }

    CommandBufferInheritanceInfo& operator=( VkCommandBufferInheritanceInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferInheritanceInfo) );
      return *this;
    }

    CommandBufferInheritanceInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CommandBufferInheritanceInfo& setRenderPass( RenderPass renderPass_ )
    {
      renderPass = renderPass_;
      return *this;
    }

    CommandBufferInheritanceInfo& setSubpass( uint32_t subpass_ )
    {
      subpass = subpass_;
      return *this;
    }

    CommandBufferInheritanceInfo& setFramebuffer( Framebuffer framebuffer_ )
    {
      framebuffer = framebuffer_;
      return *this;
    }

    CommandBufferInheritanceInfo& setOcclusionQueryEnable( Bool32 occlusionQueryEnable_ )
    {
      occlusionQueryEnable = occlusionQueryEnable_;
      return *this;
    }

    CommandBufferInheritanceInfo& setQueryFlags( QueryControlFlags queryFlags_ )
    {
      queryFlags = queryFlags_;
      return *this;
    }

    CommandBufferInheritanceInfo& setPipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics_ )
    {
      pipelineStatistics = pipelineStatistics_;
      return *this;
    }

    operator const VkCommandBufferInheritanceInfo&() const
    {
      return *reinterpret_cast<const VkCommandBufferInheritanceInfo*>(this);
    }

    bool operator==( CommandBufferInheritanceInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( renderPass == rhs.renderPass )
          && ( subpass == rhs.subpass )
          && ( framebuffer == rhs.framebuffer )
          && ( occlusionQueryEnable == rhs.occlusionQueryEnable )
          && ( queryFlags == rhs.queryFlags )
          && ( pipelineStatistics == rhs.pipelineStatistics );
    }

    bool operator!=( CommandBufferInheritanceInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    RenderPass renderPass;
    uint32_t subpass;
    Framebuffer framebuffer;
    Bool32 occlusionQueryEnable;
    QueryControlFlags queryFlags;
    QueryPipelineStatisticFlags pipelineStatistics;
  };
  static_assert( sizeof( CommandBufferInheritanceInfo ) == sizeof( VkCommandBufferInheritanceInfo ), "struct and wrapper have different size!" );

  struct CommandBufferBeginInfo
  {
    CommandBufferBeginInfo( CommandBufferUsageFlags flags_ = CommandBufferUsageFlags(), const CommandBufferInheritanceInfo* pInheritanceInfo_ = nullptr )
      : sType( StructureType::eCommandBufferBeginInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , pInheritanceInfo( pInheritanceInfo_ )
    {
    }

    CommandBufferBeginInfo( VkCommandBufferBeginInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferBeginInfo) );
    }

    CommandBufferBeginInfo& operator=( VkCommandBufferBeginInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandBufferBeginInfo) );
      return *this;
    }

    CommandBufferBeginInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CommandBufferBeginInfo& setFlags( CommandBufferUsageFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    CommandBufferBeginInfo& setPInheritanceInfo( const CommandBufferInheritanceInfo* pInheritanceInfo_ )
    {
      pInheritanceInfo = pInheritanceInfo_;
      return *this;
    }

    operator const VkCommandBufferBeginInfo&() const
    {
      return *reinterpret_cast<const VkCommandBufferBeginInfo*>(this);
    }

    bool operator==( CommandBufferBeginInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( pInheritanceInfo == rhs.pInheritanceInfo );
    }

    bool operator!=( CommandBufferBeginInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    CommandBufferUsageFlags flags;
    const CommandBufferInheritanceInfo* pInheritanceInfo;
  };
  static_assert( sizeof( CommandBufferBeginInfo ) == sizeof( VkCommandBufferBeginInfo ), "struct and wrapper have different size!" );

  struct QueryPoolCreateInfo
  {
    QueryPoolCreateInfo( QueryPoolCreateFlags flags_ = QueryPoolCreateFlags(), QueryType queryType_ = QueryType::eOcclusion, uint32_t queryCount_ = 0, QueryPipelineStatisticFlags pipelineStatistics_ = QueryPipelineStatisticFlags() )
      : sType( StructureType::eQueryPoolCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , queryType( queryType_ )
      , queryCount( queryCount_ )
      , pipelineStatistics( pipelineStatistics_ )
    {
    }

    QueryPoolCreateInfo( VkQueryPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(QueryPoolCreateInfo) );
    }

    QueryPoolCreateInfo& operator=( VkQueryPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(QueryPoolCreateInfo) );
      return *this;
    }

    QueryPoolCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    QueryPoolCreateInfo& setFlags( QueryPoolCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    QueryPoolCreateInfo& setQueryType( QueryType queryType_ )
    {
      queryType = queryType_;
      return *this;
    }

    QueryPoolCreateInfo& setQueryCount( uint32_t queryCount_ )
    {
      queryCount = queryCount_;
      return *this;
    }

    QueryPoolCreateInfo& setPipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics_ )
    {
      pipelineStatistics = pipelineStatistics_;
      return *this;
    }

    operator const VkQueryPoolCreateInfo&() const
    {
      return *reinterpret_cast<const VkQueryPoolCreateInfo*>(this);
    }

    bool operator==( QueryPoolCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( queryType == rhs.queryType )
          && ( queryCount == rhs.queryCount )
          && ( pipelineStatistics == rhs.pipelineStatistics );
    }

    bool operator!=( QueryPoolCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    QueryPoolCreateFlags flags;
    QueryType queryType;
    uint32_t queryCount;
    QueryPipelineStatisticFlags pipelineStatistics;
  };
  static_assert( sizeof( QueryPoolCreateInfo ) == sizeof( VkQueryPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class ImageAspectFlagBits
  {
    eColor = VK_IMAGE_ASPECT_COLOR_BIT,
    eDepth = VK_IMAGE_ASPECT_DEPTH_BIT,
    eStencil = VK_IMAGE_ASPECT_STENCIL_BIT,
    eMetadata = VK_IMAGE_ASPECT_METADATA_BIT
  };

  using ImageAspectFlags = Flags<ImageAspectFlagBits, VkImageAspectFlags>;

  VULKAN_HPP_INLINE ImageAspectFlags operator|( ImageAspectFlagBits bit0, ImageAspectFlagBits bit1 )
  {
    return ImageAspectFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ImageAspectFlags operator~( ImageAspectFlagBits bits )
  {
    return ~( ImageAspectFlags( bits ) );
  }

  template <> struct FlagTraits<ImageAspectFlagBits>
  {
    enum
    {
      allFlags = VkFlags(ImageAspectFlagBits::eColor) | VkFlags(ImageAspectFlagBits::eDepth) | VkFlags(ImageAspectFlagBits::eStencil) | VkFlags(ImageAspectFlagBits::eMetadata)
    };
  };

  struct ImageSubresource
  {
    ImageSubresource( ImageAspectFlags aspectMask_ = ImageAspectFlags(), uint32_t mipLevel_ = 0, uint32_t arrayLayer_ = 0 )
      : aspectMask( aspectMask_ )
      , mipLevel( mipLevel_ )
      , arrayLayer( arrayLayer_ )
    {
    }

    ImageSubresource( VkImageSubresource const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresource) );
    }

    ImageSubresource& operator=( VkImageSubresource const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresource) );
      return *this;
    }

    ImageSubresource& setAspectMask( ImageAspectFlags aspectMask_ )
    {
      aspectMask = aspectMask_;
      return *this;
    }

    ImageSubresource& setMipLevel( uint32_t mipLevel_ )
    {
      mipLevel = mipLevel_;
      return *this;
    }

    ImageSubresource& setArrayLayer( uint32_t arrayLayer_ )
    {
      arrayLayer = arrayLayer_;
      return *this;
    }

    operator const VkImageSubresource&() const
    {
      return *reinterpret_cast<const VkImageSubresource*>(this);
    }

    bool operator==( ImageSubresource const& rhs ) const
    {
      return ( aspectMask == rhs.aspectMask )
          && ( mipLevel == rhs.mipLevel )
          && ( arrayLayer == rhs.arrayLayer );
    }

    bool operator!=( ImageSubresource const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageAspectFlags aspectMask;
    uint32_t mipLevel;
    uint32_t arrayLayer;
  };
  static_assert( sizeof( ImageSubresource ) == sizeof( VkImageSubresource ), "struct and wrapper have different size!" );

  struct ImageSubresourceLayers
  {
    ImageSubresourceLayers( ImageAspectFlags aspectMask_ = ImageAspectFlags(), uint32_t mipLevel_ = 0, uint32_t baseArrayLayer_ = 0, uint32_t layerCount_ = 0 )
      : aspectMask( aspectMask_ )
      , mipLevel( mipLevel_ )
      , baseArrayLayer( baseArrayLayer_ )
      , layerCount( layerCount_ )
    {
    }

    ImageSubresourceLayers( VkImageSubresourceLayers const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresourceLayers) );
    }

    ImageSubresourceLayers& operator=( VkImageSubresourceLayers const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresourceLayers) );
      return *this;
    }

    ImageSubresourceLayers& setAspectMask( ImageAspectFlags aspectMask_ )
    {
      aspectMask = aspectMask_;
      return *this;
    }

    ImageSubresourceLayers& setMipLevel( uint32_t mipLevel_ )
    {
      mipLevel = mipLevel_;
      return *this;
    }

    ImageSubresourceLayers& setBaseArrayLayer( uint32_t baseArrayLayer_ )
    {
      baseArrayLayer = baseArrayLayer_;
      return *this;
    }

    ImageSubresourceLayers& setLayerCount( uint32_t layerCount_ )
    {
      layerCount = layerCount_;
      return *this;
    }

    operator const VkImageSubresourceLayers&() const
    {
      return *reinterpret_cast<const VkImageSubresourceLayers*>(this);
    }

    bool operator==( ImageSubresourceLayers const& rhs ) const
    {
      return ( aspectMask == rhs.aspectMask )
          && ( mipLevel == rhs.mipLevel )
          && ( baseArrayLayer == rhs.baseArrayLayer )
          && ( layerCount == rhs.layerCount );
    }

    bool operator!=( ImageSubresourceLayers const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageAspectFlags aspectMask;
    uint32_t mipLevel;
    uint32_t baseArrayLayer;
    uint32_t layerCount;
  };
  static_assert( sizeof( ImageSubresourceLayers ) == sizeof( VkImageSubresourceLayers ), "struct and wrapper have different size!" );

  struct ImageSubresourceRange
  {
    ImageSubresourceRange( ImageAspectFlags aspectMask_ = ImageAspectFlags(), uint32_t baseMipLevel_ = 0, uint32_t levelCount_ = 0, uint32_t baseArrayLayer_ = 0, uint32_t layerCount_ = 0 )
      : aspectMask( aspectMask_ )
      , baseMipLevel( baseMipLevel_ )
      , levelCount( levelCount_ )
      , baseArrayLayer( baseArrayLayer_ )
      , layerCount( layerCount_ )
    {
    }

    ImageSubresourceRange( VkImageSubresourceRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresourceRange) );
    }

    ImageSubresourceRange& operator=( VkImageSubresourceRange const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageSubresourceRange) );
      return *this;
    }

    ImageSubresourceRange& setAspectMask( ImageAspectFlags aspectMask_ )
    {
      aspectMask = aspectMask_;
      return *this;
    }

    ImageSubresourceRange& setBaseMipLevel( uint32_t baseMipLevel_ )
    {
      baseMipLevel = baseMipLevel_;
      return *this;
    }

    ImageSubresourceRange& setLevelCount( uint32_t levelCount_ )
    {
      levelCount = levelCount_;
      return *this;
    }

    ImageSubresourceRange& setBaseArrayLayer( uint32_t baseArrayLayer_ )
    {
      baseArrayLayer = baseArrayLayer_;
      return *this;
    }

    ImageSubresourceRange& setLayerCount( uint32_t layerCount_ )
    {
      layerCount = layerCount_;
      return *this;
    }

    operator const VkImageSubresourceRange&() const
    {
      return *reinterpret_cast<const VkImageSubresourceRange*>(this);
    }

    bool operator==( ImageSubresourceRange const& rhs ) const
    {
      return ( aspectMask == rhs.aspectMask )
          && ( baseMipLevel == rhs.baseMipLevel )
          && ( levelCount == rhs.levelCount )
          && ( baseArrayLayer == rhs.baseArrayLayer )
          && ( layerCount == rhs.layerCount );
    }

    bool operator!=( ImageSubresourceRange const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageAspectFlags aspectMask;
    uint32_t baseMipLevel;
    uint32_t levelCount;
    uint32_t baseArrayLayer;
    uint32_t layerCount;
  };
  static_assert( sizeof( ImageSubresourceRange ) == sizeof( VkImageSubresourceRange ), "struct and wrapper have different size!" );

  struct ImageMemoryBarrier
  {
    ImageMemoryBarrier( AccessFlags srcAccessMask_ = AccessFlags(), AccessFlags dstAccessMask_ = AccessFlags(), ImageLayout oldLayout_ = ImageLayout::eUndefined, ImageLayout newLayout_ = ImageLayout::eUndefined, uint32_t srcQueueFamilyIndex_ = 0, uint32_t dstQueueFamilyIndex_ = 0, Image image_ = Image(), ImageSubresourceRange subresourceRange_ = ImageSubresourceRange() )
      : sType( StructureType::eImageMemoryBarrier )
      , pNext( nullptr )
      , srcAccessMask( srcAccessMask_ )
      , dstAccessMask( dstAccessMask_ )
      , oldLayout( oldLayout_ )
      , newLayout( newLayout_ )
      , srcQueueFamilyIndex( srcQueueFamilyIndex_ )
      , dstQueueFamilyIndex( dstQueueFamilyIndex_ )
      , image( image_ )
      , subresourceRange( subresourceRange_ )
    {
    }

    ImageMemoryBarrier( VkImageMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageMemoryBarrier) );
    }

    ImageMemoryBarrier& operator=( VkImageMemoryBarrier const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageMemoryBarrier) );
      return *this;
    }

    ImageMemoryBarrier& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ImageMemoryBarrier& setSrcAccessMask( AccessFlags srcAccessMask_ )
    {
      srcAccessMask = srcAccessMask_;
      return *this;
    }

    ImageMemoryBarrier& setDstAccessMask( AccessFlags dstAccessMask_ )
    {
      dstAccessMask = dstAccessMask_;
      return *this;
    }

    ImageMemoryBarrier& setOldLayout( ImageLayout oldLayout_ )
    {
      oldLayout = oldLayout_;
      return *this;
    }

    ImageMemoryBarrier& setNewLayout( ImageLayout newLayout_ )
    {
      newLayout = newLayout_;
      return *this;
    }

    ImageMemoryBarrier& setSrcQueueFamilyIndex( uint32_t srcQueueFamilyIndex_ )
    {
      srcQueueFamilyIndex = srcQueueFamilyIndex_;
      return *this;
    }

    ImageMemoryBarrier& setDstQueueFamilyIndex( uint32_t dstQueueFamilyIndex_ )
    {
      dstQueueFamilyIndex = dstQueueFamilyIndex_;
      return *this;
    }

    ImageMemoryBarrier& setImage( Image image_ )
    {
      image = image_;
      return *this;
    }

    ImageMemoryBarrier& setSubresourceRange( ImageSubresourceRange subresourceRange_ )
    {
      subresourceRange = subresourceRange_;
      return *this;
    }

    operator const VkImageMemoryBarrier&() const
    {
      return *reinterpret_cast<const VkImageMemoryBarrier*>(this);
    }

    bool operator==( ImageMemoryBarrier const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( srcAccessMask == rhs.srcAccessMask )
          && ( dstAccessMask == rhs.dstAccessMask )
          && ( oldLayout == rhs.oldLayout )
          && ( newLayout == rhs.newLayout )
          && ( srcQueueFamilyIndex == rhs.srcQueueFamilyIndex )
          && ( dstQueueFamilyIndex == rhs.dstQueueFamilyIndex )
          && ( image == rhs.image )
          && ( subresourceRange == rhs.subresourceRange );
    }

    bool operator!=( ImageMemoryBarrier const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    AccessFlags srcAccessMask;
    AccessFlags dstAccessMask;
    ImageLayout oldLayout;
    ImageLayout newLayout;
    uint32_t srcQueueFamilyIndex;
    uint32_t dstQueueFamilyIndex;
    Image image;
    ImageSubresourceRange subresourceRange;
  };
  static_assert( sizeof( ImageMemoryBarrier ) == sizeof( VkImageMemoryBarrier ), "struct and wrapper have different size!" );

  struct ImageViewCreateInfo
  {
    ImageViewCreateInfo( ImageViewCreateFlags flags_ = ImageViewCreateFlags(), Image image_ = Image(), ImageViewType viewType_ = ImageViewType::e1D, Format format_ = Format::eUndefined, ComponentMapping components_ = ComponentMapping(), ImageSubresourceRange subresourceRange_ = ImageSubresourceRange() )
      : sType( StructureType::eImageViewCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , image( image_ )
      , viewType( viewType_ )
      , format( format_ )
      , components( components_ )
      , subresourceRange( subresourceRange_ )
    {
    }

    ImageViewCreateInfo( VkImageViewCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageViewCreateInfo) );
    }

    ImageViewCreateInfo& operator=( VkImageViewCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageViewCreateInfo) );
      return *this;
    }

    ImageViewCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ImageViewCreateInfo& setFlags( ImageViewCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    ImageViewCreateInfo& setImage( Image image_ )
    {
      image = image_;
      return *this;
    }

    ImageViewCreateInfo& setViewType( ImageViewType viewType_ )
    {
      viewType = viewType_;
      return *this;
    }

    ImageViewCreateInfo& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    ImageViewCreateInfo& setComponents( ComponentMapping components_ )
    {
      components = components_;
      return *this;
    }

    ImageViewCreateInfo& setSubresourceRange( ImageSubresourceRange subresourceRange_ )
    {
      subresourceRange = subresourceRange_;
      return *this;
    }

    operator const VkImageViewCreateInfo&() const
    {
      return *reinterpret_cast<const VkImageViewCreateInfo*>(this);
    }

    bool operator==( ImageViewCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( image == rhs.image )
          && ( viewType == rhs.viewType )
          && ( format == rhs.format )
          && ( components == rhs.components )
          && ( subresourceRange == rhs.subresourceRange );
    }

    bool operator!=( ImageViewCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ImageViewCreateFlags flags;
    Image image;
    ImageViewType viewType;
    Format format;
    ComponentMapping components;
    ImageSubresourceRange subresourceRange;
  };
  static_assert( sizeof( ImageViewCreateInfo ) == sizeof( VkImageViewCreateInfo ), "struct and wrapper have different size!" );

  struct ImageCopy
  {
    ImageCopy( ImageSubresourceLayers srcSubresource_ = ImageSubresourceLayers(), Offset3D srcOffset_ = Offset3D(), ImageSubresourceLayers dstSubresource_ = ImageSubresourceLayers(), Offset3D dstOffset_ = Offset3D(), Extent3D extent_ = Extent3D() )
      : srcSubresource( srcSubresource_ )
      , srcOffset( srcOffset_ )
      , dstSubresource( dstSubresource_ )
      , dstOffset( dstOffset_ )
      , extent( extent_ )
    {
    }

    ImageCopy( VkImageCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageCopy) );
    }

    ImageCopy& operator=( VkImageCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageCopy) );
      return *this;
    }

    ImageCopy& setSrcSubresource( ImageSubresourceLayers srcSubresource_ )
    {
      srcSubresource = srcSubresource_;
      return *this;
    }

    ImageCopy& setSrcOffset( Offset3D srcOffset_ )
    {
      srcOffset = srcOffset_;
      return *this;
    }

    ImageCopy& setDstSubresource( ImageSubresourceLayers dstSubresource_ )
    {
      dstSubresource = dstSubresource_;
      return *this;
    }

    ImageCopy& setDstOffset( Offset3D dstOffset_ )
    {
      dstOffset = dstOffset_;
      return *this;
    }

    ImageCopy& setExtent( Extent3D extent_ )
    {
      extent = extent_;
      return *this;
    }

    operator const VkImageCopy&() const
    {
      return *reinterpret_cast<const VkImageCopy*>(this);
    }

    bool operator==( ImageCopy const& rhs ) const
    {
      return ( srcSubresource == rhs.srcSubresource )
          && ( srcOffset == rhs.srcOffset )
          && ( dstSubresource == rhs.dstSubresource )
          && ( dstOffset == rhs.dstOffset )
          && ( extent == rhs.extent );
    }

    bool operator!=( ImageCopy const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageSubresourceLayers srcSubresource;
    Offset3D srcOffset;
    ImageSubresourceLayers dstSubresource;
    Offset3D dstOffset;
    Extent3D extent;
  };
  static_assert( sizeof( ImageCopy ) == sizeof( VkImageCopy ), "struct and wrapper have different size!" );

  struct ImageBlit
  {
    ImageBlit( ImageSubresourceLayers srcSubresource_ = ImageSubresourceLayers(), std::array<Offset3D,2> const& srcOffsets_ = { { Offset3D(), Offset3D() } }, ImageSubresourceLayers dstSubresource_ = ImageSubresourceLayers(), std::array<Offset3D,2> const& dstOffsets_ = { { Offset3D(), Offset3D() } } )
      : srcSubresource( srcSubresource_ )
      , dstSubresource( dstSubresource_ )
    {
      memcpy( &srcOffsets, srcOffsets_.data(), 2 * sizeof( Offset3D ) );
      memcpy( &dstOffsets, dstOffsets_.data(), 2 * sizeof( Offset3D ) );
    }

    ImageBlit( VkImageBlit const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageBlit) );
    }

    ImageBlit& operator=( VkImageBlit const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageBlit) );
      return *this;
    }

    ImageBlit& setSrcSubresource( ImageSubresourceLayers srcSubresource_ )
    {
      srcSubresource = srcSubresource_;
      return *this;
    }

    ImageBlit& setSrcOffsets( std::array<Offset3D,2> srcOffsets_ )
    {
      memcpy( &srcOffsets, srcOffsets_.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    ImageBlit& setDstSubresource( ImageSubresourceLayers dstSubresource_ )
    {
      dstSubresource = dstSubresource_;
      return *this;
    }

    ImageBlit& setDstOffsets( std::array<Offset3D,2> dstOffsets_ )
    {
      memcpy( &dstOffsets, dstOffsets_.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    operator const VkImageBlit&() const
    {
      return *reinterpret_cast<const VkImageBlit*>(this);
    }

    bool operator==( ImageBlit const& rhs ) const
    {
      return ( srcSubresource == rhs.srcSubresource )
          && ( memcmp( srcOffsets, rhs.srcOffsets, 2 * sizeof( Offset3D ) ) == 0 )
          && ( dstSubresource == rhs.dstSubresource )
          && ( memcmp( dstOffsets, rhs.dstOffsets, 2 * sizeof( Offset3D ) ) == 0 );
    }

    bool operator!=( ImageBlit const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageSubresourceLayers srcSubresource;
    Offset3D srcOffsets[2];
    ImageSubresourceLayers dstSubresource;
    Offset3D dstOffsets[2];
  };
  static_assert( sizeof( ImageBlit ) == sizeof( VkImageBlit ), "struct and wrapper have different size!" );

  struct BufferImageCopy
  {
    BufferImageCopy( DeviceSize bufferOffset_ = 0, uint32_t bufferRowLength_ = 0, uint32_t bufferImageHeight_ = 0, ImageSubresourceLayers imageSubresource_ = ImageSubresourceLayers(), Offset3D imageOffset_ = Offset3D(), Extent3D imageExtent_ = Extent3D() )
      : bufferOffset( bufferOffset_ )
      , bufferRowLength( bufferRowLength_ )
      , bufferImageHeight( bufferImageHeight_ )
      , imageSubresource( imageSubresource_ )
      , imageOffset( imageOffset_ )
      , imageExtent( imageExtent_ )
    {
    }

    BufferImageCopy( VkBufferImageCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferImageCopy) );
    }

    BufferImageCopy& operator=( VkBufferImageCopy const & rhs )
    {
      memcpy( this, &rhs, sizeof(BufferImageCopy) );
      return *this;
    }

    BufferImageCopy& setBufferOffset( DeviceSize bufferOffset_ )
    {
      bufferOffset = bufferOffset_;
      return *this;
    }

    BufferImageCopy& setBufferRowLength( uint32_t bufferRowLength_ )
    {
      bufferRowLength = bufferRowLength_;
      return *this;
    }

    BufferImageCopy& setBufferImageHeight( uint32_t bufferImageHeight_ )
    {
      bufferImageHeight = bufferImageHeight_;
      return *this;
    }

    BufferImageCopy& setImageSubresource( ImageSubresourceLayers imageSubresource_ )
    {
      imageSubresource = imageSubresource_;
      return *this;
    }

    BufferImageCopy& setImageOffset( Offset3D imageOffset_ )
    {
      imageOffset = imageOffset_;
      return *this;
    }

    BufferImageCopy& setImageExtent( Extent3D imageExtent_ )
    {
      imageExtent = imageExtent_;
      return *this;
    }

    operator const VkBufferImageCopy&() const
    {
      return *reinterpret_cast<const VkBufferImageCopy*>(this);
    }

    bool operator==( BufferImageCopy const& rhs ) const
    {
      return ( bufferOffset == rhs.bufferOffset )
          && ( bufferRowLength == rhs.bufferRowLength )
          && ( bufferImageHeight == rhs.bufferImageHeight )
          && ( imageSubresource == rhs.imageSubresource )
          && ( imageOffset == rhs.imageOffset )
          && ( imageExtent == rhs.imageExtent );
    }

    bool operator!=( BufferImageCopy const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize bufferOffset;
    uint32_t bufferRowLength;
    uint32_t bufferImageHeight;
    ImageSubresourceLayers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
  };
  static_assert( sizeof( BufferImageCopy ) == sizeof( VkBufferImageCopy ), "struct and wrapper have different size!" );

  struct ImageResolve
  {
    ImageResolve( ImageSubresourceLayers srcSubresource_ = ImageSubresourceLayers(), Offset3D srcOffset_ = Offset3D(), ImageSubresourceLayers dstSubresource_ = ImageSubresourceLayers(), Offset3D dstOffset_ = Offset3D(), Extent3D extent_ = Extent3D() )
      : srcSubresource( srcSubresource_ )
      , srcOffset( srcOffset_ )
      , dstSubresource( dstSubresource_ )
      , dstOffset( dstOffset_ )
      , extent( extent_ )
    {
    }

    ImageResolve( VkImageResolve const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageResolve) );
    }

    ImageResolve& operator=( VkImageResolve const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageResolve) );
      return *this;
    }

    ImageResolve& setSrcSubresource( ImageSubresourceLayers srcSubresource_ )
    {
      srcSubresource = srcSubresource_;
      return *this;
    }

    ImageResolve& setSrcOffset( Offset3D srcOffset_ )
    {
      srcOffset = srcOffset_;
      return *this;
    }

    ImageResolve& setDstSubresource( ImageSubresourceLayers dstSubresource_ )
    {
      dstSubresource = dstSubresource_;
      return *this;
    }

    ImageResolve& setDstOffset( Offset3D dstOffset_ )
    {
      dstOffset = dstOffset_;
      return *this;
    }

    ImageResolve& setExtent( Extent3D extent_ )
    {
      extent = extent_;
      return *this;
    }

    operator const VkImageResolve&() const
    {
      return *reinterpret_cast<const VkImageResolve*>(this);
    }

    bool operator==( ImageResolve const& rhs ) const
    {
      return ( srcSubresource == rhs.srcSubresource )
          && ( srcOffset == rhs.srcOffset )
          && ( dstSubresource == rhs.dstSubresource )
          && ( dstOffset == rhs.dstOffset )
          && ( extent == rhs.extent );
    }

    bool operator!=( ImageResolve const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageSubresourceLayers srcSubresource;
    Offset3D srcOffset;
    ImageSubresourceLayers dstSubresource;
    Offset3D dstOffset;
    Extent3D extent;
  };
  static_assert( sizeof( ImageResolve ) == sizeof( VkImageResolve ), "struct and wrapper have different size!" );

  struct ClearAttachment
  {
    ClearAttachment( ImageAspectFlags aspectMask_ = ImageAspectFlags(), uint32_t colorAttachment_ = 0, ClearValue clearValue_ = ClearValue() )
      : aspectMask( aspectMask_ )
      , colorAttachment( colorAttachment_ )
      , clearValue( clearValue_ )
    {
    }

    ClearAttachment( VkClearAttachment const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearAttachment) );
    }

    ClearAttachment& operator=( VkClearAttachment const & rhs )
    {
      memcpy( this, &rhs, sizeof(ClearAttachment) );
      return *this;
    }

    ClearAttachment& setAspectMask( ImageAspectFlags aspectMask_ )
    {
      aspectMask = aspectMask_;
      return *this;
    }

    ClearAttachment& setColorAttachment( uint32_t colorAttachment_ )
    {
      colorAttachment = colorAttachment_;
      return *this;
    }

    ClearAttachment& setClearValue( ClearValue clearValue_ )
    {
      clearValue = clearValue_;
      return *this;
    }

    operator const VkClearAttachment&() const
    {
      return *reinterpret_cast<const VkClearAttachment*>(this);
    }

    ImageAspectFlags aspectMask;
    uint32_t colorAttachment;
    ClearValue clearValue;
  };
  static_assert( sizeof( ClearAttachment ) == sizeof( VkClearAttachment ), "struct and wrapper have different size!" );

  enum class SparseImageFormatFlagBits
  {
    eSingleMiptail = VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT,
    eAlignedMipSize = VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT,
    eNonstandardBlockSize = VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT
  };

  using SparseImageFormatFlags = Flags<SparseImageFormatFlagBits, VkSparseImageFormatFlags>;

  VULKAN_HPP_INLINE SparseImageFormatFlags operator|( SparseImageFormatFlagBits bit0, SparseImageFormatFlagBits bit1 )
  {
    return SparseImageFormatFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE SparseImageFormatFlags operator~( SparseImageFormatFlagBits bits )
  {
    return ~( SparseImageFormatFlags( bits ) );
  }

  template <> struct FlagTraits<SparseImageFormatFlagBits>
  {
    enum
    {
      allFlags = VkFlags(SparseImageFormatFlagBits::eSingleMiptail) | VkFlags(SparseImageFormatFlagBits::eAlignedMipSize) | VkFlags(SparseImageFormatFlagBits::eNonstandardBlockSize)
    };
  };

  struct SparseImageFormatProperties
  {
    operator const VkSparseImageFormatProperties&() const
    {
      return *reinterpret_cast<const VkSparseImageFormatProperties*>(this);
    }

    bool operator==( SparseImageFormatProperties const& rhs ) const
    {
      return ( aspectMask == rhs.aspectMask )
          && ( imageGranularity == rhs.imageGranularity )
          && ( flags == rhs.flags );
    }

    bool operator!=( SparseImageFormatProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageAspectFlags aspectMask;
    Extent3D imageGranularity;
    SparseImageFormatFlags flags;
  };
  static_assert( sizeof( SparseImageFormatProperties ) == sizeof( VkSparseImageFormatProperties ), "struct and wrapper have different size!" );

  struct SparseImageMemoryRequirements
  {
    operator const VkSparseImageMemoryRequirements&() const
    {
      return *reinterpret_cast<const VkSparseImageMemoryRequirements*>(this);
    }

    bool operator==( SparseImageMemoryRequirements const& rhs ) const
    {
      return ( formatProperties == rhs.formatProperties )
          && ( imageMipTailFirstLod == rhs.imageMipTailFirstLod )
          && ( imageMipTailSize == rhs.imageMipTailSize )
          && ( imageMipTailOffset == rhs.imageMipTailOffset )
          && ( imageMipTailStride == rhs.imageMipTailStride );
    }

    bool operator!=( SparseImageMemoryRequirements const& rhs ) const
    {
      return !operator==( rhs );
    }

    SparseImageFormatProperties formatProperties;
    uint32_t imageMipTailFirstLod;
    DeviceSize imageMipTailSize;
    DeviceSize imageMipTailOffset;
    DeviceSize imageMipTailStride;
  };
  static_assert( sizeof( SparseImageMemoryRequirements ) == sizeof( VkSparseImageMemoryRequirements ), "struct and wrapper have different size!" );

  struct SparseImageFormatProperties2KHR
  {
    operator const VkSparseImageFormatProperties2KHR&() const
    {
      return *reinterpret_cast<const VkSparseImageFormatProperties2KHR*>(this);
    }

    bool operator==( SparseImageFormatProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( properties == rhs.properties );
    }

    bool operator!=( SparseImageFormatProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    SparseImageFormatProperties properties;
  };
  static_assert( sizeof( SparseImageFormatProperties2KHR ) == sizeof( VkSparseImageFormatProperties2KHR ), "struct and wrapper have different size!" );

  enum class SparseMemoryBindFlagBits
  {
    eMetadata = VK_SPARSE_MEMORY_BIND_METADATA_BIT
  };

  using SparseMemoryBindFlags = Flags<SparseMemoryBindFlagBits, VkSparseMemoryBindFlags>;

  VULKAN_HPP_INLINE SparseMemoryBindFlags operator|( SparseMemoryBindFlagBits bit0, SparseMemoryBindFlagBits bit1 )
  {
    return SparseMemoryBindFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE SparseMemoryBindFlags operator~( SparseMemoryBindFlagBits bits )
  {
    return ~( SparseMemoryBindFlags( bits ) );
  }

  template <> struct FlagTraits<SparseMemoryBindFlagBits>
  {
    enum
    {
      allFlags = VkFlags(SparseMemoryBindFlagBits::eMetadata)
    };
  };

  struct SparseMemoryBind
  {
    SparseMemoryBind( DeviceSize resourceOffset_ = 0, DeviceSize size_ = 0, DeviceMemory memory_ = DeviceMemory(), DeviceSize memoryOffset_ = 0, SparseMemoryBindFlags flags_ = SparseMemoryBindFlags() )
      : resourceOffset( resourceOffset_ )
      , size( size_ )
      , memory( memory_ )
      , memoryOffset( memoryOffset_ )
      , flags( flags_ )
    {
    }

    SparseMemoryBind( VkSparseMemoryBind const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseMemoryBind) );
    }

    SparseMemoryBind& operator=( VkSparseMemoryBind const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseMemoryBind) );
      return *this;
    }

    SparseMemoryBind& setResourceOffset( DeviceSize resourceOffset_ )
    {
      resourceOffset = resourceOffset_;
      return *this;
    }

    SparseMemoryBind& setSize( DeviceSize size_ )
    {
      size = size_;
      return *this;
    }

    SparseMemoryBind& setMemory( DeviceMemory memory_ )
    {
      memory = memory_;
      return *this;
    }

    SparseMemoryBind& setMemoryOffset( DeviceSize memoryOffset_ )
    {
      memoryOffset = memoryOffset_;
      return *this;
    }

    SparseMemoryBind& setFlags( SparseMemoryBindFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkSparseMemoryBind&() const
    {
      return *reinterpret_cast<const VkSparseMemoryBind*>(this);
    }

    bool operator==( SparseMemoryBind const& rhs ) const
    {
      return ( resourceOffset == rhs.resourceOffset )
          && ( size == rhs.size )
          && ( memory == rhs.memory )
          && ( memoryOffset == rhs.memoryOffset )
          && ( flags == rhs.flags );
    }

    bool operator!=( SparseMemoryBind const& rhs ) const
    {
      return !operator==( rhs );
    }

    DeviceSize resourceOffset;
    DeviceSize size;
    DeviceMemory memory;
    DeviceSize memoryOffset;
    SparseMemoryBindFlags flags;
  };
  static_assert( sizeof( SparseMemoryBind ) == sizeof( VkSparseMemoryBind ), "struct and wrapper have different size!" );

  struct SparseImageMemoryBind
  {
    SparseImageMemoryBind( ImageSubresource subresource_ = ImageSubresource(), Offset3D offset_ = Offset3D(), Extent3D extent_ = Extent3D(), DeviceMemory memory_ = DeviceMemory(), DeviceSize memoryOffset_ = 0, SparseMemoryBindFlags flags_ = SparseMemoryBindFlags() )
      : subresource( subresource_ )
      , offset( offset_ )
      , extent( extent_ )
      , memory( memory_ )
      , memoryOffset( memoryOffset_ )
      , flags( flags_ )
    {
    }

    SparseImageMemoryBind( VkSparseImageMemoryBind const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageMemoryBind) );
    }

    SparseImageMemoryBind& operator=( VkSparseImageMemoryBind const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageMemoryBind) );
      return *this;
    }

    SparseImageMemoryBind& setSubresource( ImageSubresource subresource_ )
    {
      subresource = subresource_;
      return *this;
    }

    SparseImageMemoryBind& setOffset( Offset3D offset_ )
    {
      offset = offset_;
      return *this;
    }

    SparseImageMemoryBind& setExtent( Extent3D extent_ )
    {
      extent = extent_;
      return *this;
    }

    SparseImageMemoryBind& setMemory( DeviceMemory memory_ )
    {
      memory = memory_;
      return *this;
    }

    SparseImageMemoryBind& setMemoryOffset( DeviceSize memoryOffset_ )
    {
      memoryOffset = memoryOffset_;
      return *this;
    }

    SparseImageMemoryBind& setFlags( SparseMemoryBindFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkSparseImageMemoryBind&() const
    {
      return *reinterpret_cast<const VkSparseImageMemoryBind*>(this);
    }

    bool operator==( SparseImageMemoryBind const& rhs ) const
    {
      return ( subresource == rhs.subresource )
          && ( offset == rhs.offset )
          && ( extent == rhs.extent )
          && ( memory == rhs.memory )
          && ( memoryOffset == rhs.memoryOffset )
          && ( flags == rhs.flags );
    }

    bool operator!=( SparseImageMemoryBind const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageSubresource subresource;
    Offset3D offset;
    Extent3D extent;
    DeviceMemory memory;
    DeviceSize memoryOffset;
    SparseMemoryBindFlags flags;
  };
  static_assert( sizeof( SparseImageMemoryBind ) == sizeof( VkSparseImageMemoryBind ), "struct and wrapper have different size!" );

  struct SparseBufferMemoryBindInfo
  {
    SparseBufferMemoryBindInfo( Buffer buffer_ = Buffer(), uint32_t bindCount_ = 0, const SparseMemoryBind* pBinds_ = nullptr )
      : buffer( buffer_ )
      , bindCount( bindCount_ )
      , pBinds( pBinds_ )
    {
    }

    SparseBufferMemoryBindInfo( VkSparseBufferMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseBufferMemoryBindInfo) );
    }

    SparseBufferMemoryBindInfo& operator=( VkSparseBufferMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseBufferMemoryBindInfo) );
      return *this;
    }

    SparseBufferMemoryBindInfo& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    SparseBufferMemoryBindInfo& setBindCount( uint32_t bindCount_ )
    {
      bindCount = bindCount_;
      return *this;
    }

    SparseBufferMemoryBindInfo& setPBinds( const SparseMemoryBind* pBinds_ )
    {
      pBinds = pBinds_;
      return *this;
    }

    operator const VkSparseBufferMemoryBindInfo&() const
    {
      return *reinterpret_cast<const VkSparseBufferMemoryBindInfo*>(this);
    }

    bool operator==( SparseBufferMemoryBindInfo const& rhs ) const
    {
      return ( buffer == rhs.buffer )
          && ( bindCount == rhs.bindCount )
          && ( pBinds == rhs.pBinds );
    }

    bool operator!=( SparseBufferMemoryBindInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    Buffer buffer;
    uint32_t bindCount;
    const SparseMemoryBind* pBinds;
  };
  static_assert( sizeof( SparseBufferMemoryBindInfo ) == sizeof( VkSparseBufferMemoryBindInfo ), "struct and wrapper have different size!" );

  struct SparseImageOpaqueMemoryBindInfo
  {
    SparseImageOpaqueMemoryBindInfo( Image image_ = Image(), uint32_t bindCount_ = 0, const SparseMemoryBind* pBinds_ = nullptr )
      : image( image_ )
      , bindCount( bindCount_ )
      , pBinds( pBinds_ )
    {
    }

    SparseImageOpaqueMemoryBindInfo( VkSparseImageOpaqueMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageOpaqueMemoryBindInfo) );
    }

    SparseImageOpaqueMemoryBindInfo& operator=( VkSparseImageOpaqueMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageOpaqueMemoryBindInfo) );
      return *this;
    }

    SparseImageOpaqueMemoryBindInfo& setImage( Image image_ )
    {
      image = image_;
      return *this;
    }

    SparseImageOpaqueMemoryBindInfo& setBindCount( uint32_t bindCount_ )
    {
      bindCount = bindCount_;
      return *this;
    }

    SparseImageOpaqueMemoryBindInfo& setPBinds( const SparseMemoryBind* pBinds_ )
    {
      pBinds = pBinds_;
      return *this;
    }

    operator const VkSparseImageOpaqueMemoryBindInfo&() const
    {
      return *reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>(this);
    }

    bool operator==( SparseImageOpaqueMemoryBindInfo const& rhs ) const
    {
      return ( image == rhs.image )
          && ( bindCount == rhs.bindCount )
          && ( pBinds == rhs.pBinds );
    }

    bool operator!=( SparseImageOpaqueMemoryBindInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    Image image;
    uint32_t bindCount;
    const SparseMemoryBind* pBinds;
  };
  static_assert( sizeof( SparseImageOpaqueMemoryBindInfo ) == sizeof( VkSparseImageOpaqueMemoryBindInfo ), "struct and wrapper have different size!" );

  struct SparseImageMemoryBindInfo
  {
    SparseImageMemoryBindInfo( Image image_ = Image(), uint32_t bindCount_ = 0, const SparseImageMemoryBind* pBinds_ = nullptr )
      : image( image_ )
      , bindCount( bindCount_ )
      , pBinds( pBinds_ )
    {
    }

    SparseImageMemoryBindInfo( VkSparseImageMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageMemoryBindInfo) );
    }

    SparseImageMemoryBindInfo& operator=( VkSparseImageMemoryBindInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SparseImageMemoryBindInfo) );
      return *this;
    }

    SparseImageMemoryBindInfo& setImage( Image image_ )
    {
      image = image_;
      return *this;
    }

    SparseImageMemoryBindInfo& setBindCount( uint32_t bindCount_ )
    {
      bindCount = bindCount_;
      return *this;
    }

    SparseImageMemoryBindInfo& setPBinds( const SparseImageMemoryBind* pBinds_ )
    {
      pBinds = pBinds_;
      return *this;
    }

    operator const VkSparseImageMemoryBindInfo&() const
    {
      return *reinterpret_cast<const VkSparseImageMemoryBindInfo*>(this);
    }

    bool operator==( SparseImageMemoryBindInfo const& rhs ) const
    {
      return ( image == rhs.image )
          && ( bindCount == rhs.bindCount )
          && ( pBinds == rhs.pBinds );
    }

    bool operator!=( SparseImageMemoryBindInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

    Image image;
    uint32_t bindCount;
    const SparseImageMemoryBind* pBinds;
  };
  static_assert( sizeof( SparseImageMemoryBindInfo ) == sizeof( VkSparseImageMemoryBindInfo ), "struct and wrapper have different size!" );

  struct BindSparseInfo
  {
    BindSparseInfo( uint32_t waitSemaphoreCount_ = 0, const Semaphore* pWaitSemaphores_ = nullptr, uint32_t bufferBindCount_ = 0, const SparseBufferMemoryBindInfo* pBufferBinds_ = nullptr, uint32_t imageOpaqueBindCount_ = 0, const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds_ = nullptr, uint32_t imageBindCount_ = 0, const SparseImageMemoryBindInfo* pImageBinds_ = nullptr, uint32_t signalSemaphoreCount_ = 0, const Semaphore* pSignalSemaphores_ = nullptr )
      : sType( StructureType::eBindSparseInfo )
      , pNext( nullptr )
      , waitSemaphoreCount( waitSemaphoreCount_ )
      , pWaitSemaphores( pWaitSemaphores_ )
      , bufferBindCount( bufferBindCount_ )
      , pBufferBinds( pBufferBinds_ )
      , imageOpaqueBindCount( imageOpaqueBindCount_ )
      , pImageOpaqueBinds( pImageOpaqueBinds_ )
      , imageBindCount( imageBindCount_ )
      , pImageBinds( pImageBinds_ )
      , signalSemaphoreCount( signalSemaphoreCount_ )
      , pSignalSemaphores( pSignalSemaphores_ )
    {
    }

    BindSparseInfo( VkBindSparseInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BindSparseInfo) );
    }

    BindSparseInfo& operator=( VkBindSparseInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(BindSparseInfo) );
      return *this;
    }

    BindSparseInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    BindSparseInfo& setWaitSemaphoreCount( uint32_t waitSemaphoreCount_ )
    {
      waitSemaphoreCount = waitSemaphoreCount_;
      return *this;
    }

    BindSparseInfo& setPWaitSemaphores( const Semaphore* pWaitSemaphores_ )
    {
      pWaitSemaphores = pWaitSemaphores_;
      return *this;
    }

    BindSparseInfo& setBufferBindCount( uint32_t bufferBindCount_ )
    {
      bufferBindCount = bufferBindCount_;
      return *this;
    }

    BindSparseInfo& setPBufferBinds( const SparseBufferMemoryBindInfo* pBufferBinds_ )
    {
      pBufferBinds = pBufferBinds_;
      return *this;
    }

    BindSparseInfo& setImageOpaqueBindCount( uint32_t imageOpaqueBindCount_ )
    {
      imageOpaqueBindCount = imageOpaqueBindCount_;
      return *this;
    }

    BindSparseInfo& setPImageOpaqueBinds( const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds_ )
    {
      pImageOpaqueBinds = pImageOpaqueBinds_;
      return *this;
    }

    BindSparseInfo& setImageBindCount( uint32_t imageBindCount_ )
    {
      imageBindCount = imageBindCount_;
      return *this;
    }

    BindSparseInfo& setPImageBinds( const SparseImageMemoryBindInfo* pImageBinds_ )
    {
      pImageBinds = pImageBinds_;
      return *this;
    }

    BindSparseInfo& setSignalSemaphoreCount( uint32_t signalSemaphoreCount_ )
    {
      signalSemaphoreCount = signalSemaphoreCount_;
      return *this;
    }

    BindSparseInfo& setPSignalSemaphores( const Semaphore* pSignalSemaphores_ )
    {
      pSignalSemaphores = pSignalSemaphores_;
      return *this;
    }

    operator const VkBindSparseInfo&() const
    {
      return *reinterpret_cast<const VkBindSparseInfo*>(this);
    }

    bool operator==( BindSparseInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( waitSemaphoreCount == rhs.waitSemaphoreCount )
          && ( pWaitSemaphores == rhs.pWaitSemaphores )
          && ( bufferBindCount == rhs.bufferBindCount )
          && ( pBufferBinds == rhs.pBufferBinds )
          && ( imageOpaqueBindCount == rhs.imageOpaqueBindCount )
          && ( pImageOpaqueBinds == rhs.pImageOpaqueBinds )
          && ( imageBindCount == rhs.imageBindCount )
          && ( pImageBinds == rhs.pImageBinds )
          && ( signalSemaphoreCount == rhs.signalSemaphoreCount )
          && ( pSignalSemaphores == rhs.pSignalSemaphores );
    }

    bool operator!=( BindSparseInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t waitSemaphoreCount;
    const Semaphore* pWaitSemaphores;
    uint32_t bufferBindCount;
    const SparseBufferMemoryBindInfo* pBufferBinds;
    uint32_t imageOpaqueBindCount;
    const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds;
    uint32_t imageBindCount;
    const SparseImageMemoryBindInfo* pImageBinds;
    uint32_t signalSemaphoreCount;
    const Semaphore* pSignalSemaphores;
  };
  static_assert( sizeof( BindSparseInfo ) == sizeof( VkBindSparseInfo ), "struct and wrapper have different size!" );

  enum class PipelineStageFlagBits
  {
    eTopOfPipe = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    eDrawIndirect = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
    eVertexInput = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    eVertexShader = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
    eTessellationControlShader = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
    eTessellationEvaluationShader = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
    eGeometryShader = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
    eFragmentShader = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    eEarlyFragmentTests = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    eLateFragmentTests = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
    eColorAttachmentOutput = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    eComputeShader = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    eTransfer = VK_PIPELINE_STAGE_TRANSFER_BIT,
    eBottomOfPipe = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
    eHost = VK_PIPELINE_STAGE_HOST_BIT,
    eAllGraphics = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
    eAllCommands = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    eCommandProcessNVX = VK_PIPELINE_STAGE_COMMAND_PROCESS_BIT_NVX
  };

  using PipelineStageFlags = Flags<PipelineStageFlagBits, VkPipelineStageFlags>;

  VULKAN_HPP_INLINE PipelineStageFlags operator|( PipelineStageFlagBits bit0, PipelineStageFlagBits bit1 )
  {
    return PipelineStageFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE PipelineStageFlags operator~( PipelineStageFlagBits bits )
  {
    return ~( PipelineStageFlags( bits ) );
  }

  template <> struct FlagTraits<PipelineStageFlagBits>
  {
    enum
    {
      allFlags = VkFlags(PipelineStageFlagBits::eTopOfPipe) | VkFlags(PipelineStageFlagBits::eDrawIndirect) | VkFlags(PipelineStageFlagBits::eVertexInput) | VkFlags(PipelineStageFlagBits::eVertexShader) | VkFlags(PipelineStageFlagBits::eTessellationControlShader) | VkFlags(PipelineStageFlagBits::eTessellationEvaluationShader) | VkFlags(PipelineStageFlagBits::eGeometryShader) | VkFlags(PipelineStageFlagBits::eFragmentShader) | VkFlags(PipelineStageFlagBits::eEarlyFragmentTests) | VkFlags(PipelineStageFlagBits::eLateFragmentTests) | VkFlags(PipelineStageFlagBits::eColorAttachmentOutput) | VkFlags(PipelineStageFlagBits::eComputeShader) | VkFlags(PipelineStageFlagBits::eTransfer) | VkFlags(PipelineStageFlagBits::eBottomOfPipe) | VkFlags(PipelineStageFlagBits::eHost) | VkFlags(PipelineStageFlagBits::eAllGraphics) | VkFlags(PipelineStageFlagBits::eAllCommands) | VkFlags(PipelineStageFlagBits::eCommandProcessNVX)
    };
  };

  enum class CommandPoolCreateFlagBits
  {
    eTransient = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
    eResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
  };

  using CommandPoolCreateFlags = Flags<CommandPoolCreateFlagBits, VkCommandPoolCreateFlags>;

  VULKAN_HPP_INLINE CommandPoolCreateFlags operator|( CommandPoolCreateFlagBits bit0, CommandPoolCreateFlagBits bit1 )
  {
    return CommandPoolCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CommandPoolCreateFlags operator~( CommandPoolCreateFlagBits bits )
  {
    return ~( CommandPoolCreateFlags( bits ) );
  }

  template <> struct FlagTraits<CommandPoolCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(CommandPoolCreateFlagBits::eTransient) | VkFlags(CommandPoolCreateFlagBits::eResetCommandBuffer)
    };
  };

  struct CommandPoolCreateInfo
  {
    CommandPoolCreateInfo( CommandPoolCreateFlags flags_ = CommandPoolCreateFlags(), uint32_t queueFamilyIndex_ = 0 )
      : sType( StructureType::eCommandPoolCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , queueFamilyIndex( queueFamilyIndex_ )
    {
    }

    CommandPoolCreateInfo( VkCommandPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandPoolCreateInfo) );
    }

    CommandPoolCreateInfo& operator=( VkCommandPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(CommandPoolCreateInfo) );
      return *this;
    }

    CommandPoolCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CommandPoolCreateInfo& setFlags( CommandPoolCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    CommandPoolCreateInfo& setQueueFamilyIndex( uint32_t queueFamilyIndex_ )
    {
      queueFamilyIndex = queueFamilyIndex_;
      return *this;
    }

    operator const VkCommandPoolCreateInfo&() const
    {
      return *reinterpret_cast<const VkCommandPoolCreateInfo*>(this);
    }

    bool operator==( CommandPoolCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( queueFamilyIndex == rhs.queueFamilyIndex );
    }

    bool operator!=( CommandPoolCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    CommandPoolCreateFlags flags;
    uint32_t queueFamilyIndex;
  };
  static_assert( sizeof( CommandPoolCreateInfo ) == sizeof( VkCommandPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class CommandPoolResetFlagBits
  {
    eReleaseResources = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT
  };

  using CommandPoolResetFlags = Flags<CommandPoolResetFlagBits, VkCommandPoolResetFlags>;

  VULKAN_HPP_INLINE CommandPoolResetFlags operator|( CommandPoolResetFlagBits bit0, CommandPoolResetFlagBits bit1 )
  {
    return CommandPoolResetFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CommandPoolResetFlags operator~( CommandPoolResetFlagBits bits )
  {
    return ~( CommandPoolResetFlags( bits ) );
  }

  template <> struct FlagTraits<CommandPoolResetFlagBits>
  {
    enum
    {
      allFlags = VkFlags(CommandPoolResetFlagBits::eReleaseResources)
    };
  };

  enum class CommandBufferResetFlagBits
  {
    eReleaseResources = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
  };

  using CommandBufferResetFlags = Flags<CommandBufferResetFlagBits, VkCommandBufferResetFlags>;

  VULKAN_HPP_INLINE CommandBufferResetFlags operator|( CommandBufferResetFlagBits bit0, CommandBufferResetFlagBits bit1 )
  {
    return CommandBufferResetFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CommandBufferResetFlags operator~( CommandBufferResetFlagBits bits )
  {
    return ~( CommandBufferResetFlags( bits ) );
  }

  template <> struct FlagTraits<CommandBufferResetFlagBits>
  {
    enum
    {
      allFlags = VkFlags(CommandBufferResetFlagBits::eReleaseResources)
    };
  };

  enum class SampleCountFlagBits
  {
    e1 = VK_SAMPLE_COUNT_1_BIT,
    e2 = VK_SAMPLE_COUNT_2_BIT,
    e4 = VK_SAMPLE_COUNT_4_BIT,
    e8 = VK_SAMPLE_COUNT_8_BIT,
    e16 = VK_SAMPLE_COUNT_16_BIT,
    e32 = VK_SAMPLE_COUNT_32_BIT,
    e64 = VK_SAMPLE_COUNT_64_BIT
  };

  using SampleCountFlags = Flags<SampleCountFlagBits, VkSampleCountFlags>;

  VULKAN_HPP_INLINE SampleCountFlags operator|( SampleCountFlagBits bit0, SampleCountFlagBits bit1 )
  {
    return SampleCountFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE SampleCountFlags operator~( SampleCountFlagBits bits )
  {
    return ~( SampleCountFlags( bits ) );
  }

  template <> struct FlagTraits<SampleCountFlagBits>
  {
    enum
    {
      allFlags = VkFlags(SampleCountFlagBits::e1) | VkFlags(SampleCountFlagBits::e2) | VkFlags(SampleCountFlagBits::e4) | VkFlags(SampleCountFlagBits::e8) | VkFlags(SampleCountFlagBits::e16) | VkFlags(SampleCountFlagBits::e32) | VkFlags(SampleCountFlagBits::e64)
    };
  };

  struct ImageFormatProperties
  {
    operator const VkImageFormatProperties&() const
    {
      return *reinterpret_cast<const VkImageFormatProperties*>(this);
    }

    bool operator==( ImageFormatProperties const& rhs ) const
    {
      return ( maxExtent == rhs.maxExtent )
          && ( maxMipLevels == rhs.maxMipLevels )
          && ( maxArrayLayers == rhs.maxArrayLayers )
          && ( sampleCounts == rhs.sampleCounts )
          && ( maxResourceSize == rhs.maxResourceSize );
    }

    bool operator!=( ImageFormatProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    Extent3D maxExtent;
    uint32_t maxMipLevels;
    uint32_t maxArrayLayers;
    SampleCountFlags sampleCounts;
    DeviceSize maxResourceSize;
  };
  static_assert( sizeof( ImageFormatProperties ) == sizeof( VkImageFormatProperties ), "struct and wrapper have different size!" );

  struct ImageCreateInfo
  {
    ImageCreateInfo( ImageCreateFlags flags_ = ImageCreateFlags(), ImageType imageType_ = ImageType::e1D, Format format_ = Format::eUndefined, Extent3D extent_ = Extent3D(), uint32_t mipLevels_ = 0, uint32_t arrayLayers_ = 0, SampleCountFlagBits samples_ = SampleCountFlagBits::e1, ImageTiling tiling_ = ImageTiling::eOptimal, ImageUsageFlags usage_ = ImageUsageFlags(), SharingMode sharingMode_ = SharingMode::eExclusive, uint32_t queueFamilyIndexCount_ = 0, const uint32_t* pQueueFamilyIndices_ = nullptr, ImageLayout initialLayout_ = ImageLayout::eUndefined )
      : sType( StructureType::eImageCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , imageType( imageType_ )
      , format( format_ )
      , extent( extent_ )
      , mipLevels( mipLevels_ )
      , arrayLayers( arrayLayers_ )
      , samples( samples_ )
      , tiling( tiling_ )
      , usage( usage_ )
      , sharingMode( sharingMode_ )
      , queueFamilyIndexCount( queueFamilyIndexCount_ )
      , pQueueFamilyIndices( pQueueFamilyIndices_ )
      , initialLayout( initialLayout_ )
    {
    }

    ImageCreateInfo( VkImageCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageCreateInfo) );
    }

    ImageCreateInfo& operator=( VkImageCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImageCreateInfo) );
      return *this;
    }

    ImageCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ImageCreateInfo& setFlags( ImageCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    ImageCreateInfo& setImageType( ImageType imageType_ )
    {
      imageType = imageType_;
      return *this;
    }

    ImageCreateInfo& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    ImageCreateInfo& setExtent( Extent3D extent_ )
    {
      extent = extent_;
      return *this;
    }

    ImageCreateInfo& setMipLevels( uint32_t mipLevels_ )
    {
      mipLevels = mipLevels_;
      return *this;
    }

    ImageCreateInfo& setArrayLayers( uint32_t arrayLayers_ )
    {
      arrayLayers = arrayLayers_;
      return *this;
    }

    ImageCreateInfo& setSamples( SampleCountFlagBits samples_ )
    {
      samples = samples_;
      return *this;
    }

    ImageCreateInfo& setTiling( ImageTiling tiling_ )
    {
      tiling = tiling_;
      return *this;
    }

    ImageCreateInfo& setUsage( ImageUsageFlags usage_ )
    {
      usage = usage_;
      return *this;
    }

    ImageCreateInfo& setSharingMode( SharingMode sharingMode_ )
    {
      sharingMode = sharingMode_;
      return *this;
    }

    ImageCreateInfo& setQueueFamilyIndexCount( uint32_t queueFamilyIndexCount_ )
    {
      queueFamilyIndexCount = queueFamilyIndexCount_;
      return *this;
    }

    ImageCreateInfo& setPQueueFamilyIndices( const uint32_t* pQueueFamilyIndices_ )
    {
      pQueueFamilyIndices = pQueueFamilyIndices_;
      return *this;
    }

    ImageCreateInfo& setInitialLayout( ImageLayout initialLayout_ )
    {
      initialLayout = initialLayout_;
      return *this;
    }

    operator const VkImageCreateInfo&() const
    {
      return *reinterpret_cast<const VkImageCreateInfo*>(this);
    }

    bool operator==( ImageCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( imageType == rhs.imageType )
          && ( format == rhs.format )
          && ( extent == rhs.extent )
          && ( mipLevels == rhs.mipLevels )
          && ( arrayLayers == rhs.arrayLayers )
          && ( samples == rhs.samples )
          && ( tiling == rhs.tiling )
          && ( usage == rhs.usage )
          && ( sharingMode == rhs.sharingMode )
          && ( queueFamilyIndexCount == rhs.queueFamilyIndexCount )
          && ( pQueueFamilyIndices == rhs.pQueueFamilyIndices )
          && ( initialLayout == rhs.initialLayout );
    }

    bool operator!=( ImageCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ImageCreateFlags flags;
    ImageType imageType;
    Format format;
    Extent3D extent;
    uint32_t mipLevels;
    uint32_t arrayLayers;
    SampleCountFlagBits samples;
    ImageTiling tiling;
    ImageUsageFlags usage;
    SharingMode sharingMode;
    uint32_t queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
    ImageLayout initialLayout;
  };
  static_assert( sizeof( ImageCreateInfo ) == sizeof( VkImageCreateInfo ), "struct and wrapper have different size!" );

  struct PipelineMultisampleStateCreateInfo
  {
    PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateFlags flags_ = PipelineMultisampleStateCreateFlags(), SampleCountFlagBits rasterizationSamples_ = SampleCountFlagBits::e1, Bool32 sampleShadingEnable_ = 0, float minSampleShading_ = 0, const SampleMask* pSampleMask_ = nullptr, Bool32 alphaToCoverageEnable_ = 0, Bool32 alphaToOneEnable_ = 0 )
      : sType( StructureType::ePipelineMultisampleStateCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , rasterizationSamples( rasterizationSamples_ )
      , sampleShadingEnable( sampleShadingEnable_ )
      , minSampleShading( minSampleShading_ )
      , pSampleMask( pSampleMask_ )
      , alphaToCoverageEnable( alphaToCoverageEnable_ )
      , alphaToOneEnable( alphaToOneEnable_ )
    {
    }

    PipelineMultisampleStateCreateInfo( VkPipelineMultisampleStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineMultisampleStateCreateInfo) );
    }

    PipelineMultisampleStateCreateInfo& operator=( VkPipelineMultisampleStateCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineMultisampleStateCreateInfo) );
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setFlags( PipelineMultisampleStateCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setRasterizationSamples( SampleCountFlagBits rasterizationSamples_ )
    {
      rasterizationSamples = rasterizationSamples_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setSampleShadingEnable( Bool32 sampleShadingEnable_ )
    {
      sampleShadingEnable = sampleShadingEnable_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setMinSampleShading( float minSampleShading_ )
    {
      minSampleShading = minSampleShading_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setPSampleMask( const SampleMask* pSampleMask_ )
    {
      pSampleMask = pSampleMask_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setAlphaToCoverageEnable( Bool32 alphaToCoverageEnable_ )
    {
      alphaToCoverageEnable = alphaToCoverageEnable_;
      return *this;
    }

    PipelineMultisampleStateCreateInfo& setAlphaToOneEnable( Bool32 alphaToOneEnable_ )
    {
      alphaToOneEnable = alphaToOneEnable_;
      return *this;
    }

    operator const VkPipelineMultisampleStateCreateInfo&() const
    {
      return *reinterpret_cast<const VkPipelineMultisampleStateCreateInfo*>(this);
    }

    bool operator==( PipelineMultisampleStateCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( rasterizationSamples == rhs.rasterizationSamples )
          && ( sampleShadingEnable == rhs.sampleShadingEnable )
          && ( minSampleShading == rhs.minSampleShading )
          && ( pSampleMask == rhs.pSampleMask )
          && ( alphaToCoverageEnable == rhs.alphaToCoverageEnable )
          && ( alphaToOneEnable == rhs.alphaToOneEnable );
    }

    bool operator!=( PipelineMultisampleStateCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineMultisampleStateCreateFlags flags;
    SampleCountFlagBits rasterizationSamples;
    Bool32 sampleShadingEnable;
    float minSampleShading;
    const SampleMask* pSampleMask;
    Bool32 alphaToCoverageEnable;
    Bool32 alphaToOneEnable;
  };
  static_assert( sizeof( PipelineMultisampleStateCreateInfo ) == sizeof( VkPipelineMultisampleStateCreateInfo ), "struct and wrapper have different size!" );

  struct GraphicsPipelineCreateInfo
  {
    GraphicsPipelineCreateInfo( PipelineCreateFlags flags_ = PipelineCreateFlags(), uint32_t stageCount_ = 0, const PipelineShaderStageCreateInfo* pStages_ = nullptr, const PipelineVertexInputStateCreateInfo* pVertexInputState_ = nullptr, const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState_ = nullptr, const PipelineTessellationStateCreateInfo* pTessellationState_ = nullptr, const PipelineViewportStateCreateInfo* pViewportState_ = nullptr, const PipelineRasterizationStateCreateInfo* pRasterizationState_ = nullptr, const PipelineMultisampleStateCreateInfo* pMultisampleState_ = nullptr, const PipelineDepthStencilStateCreateInfo* pDepthStencilState_ = nullptr, const PipelineColorBlendStateCreateInfo* pColorBlendState_ = nullptr, const PipelineDynamicStateCreateInfo* pDynamicState_ = nullptr, PipelineLayout layout_ = PipelineLayout(), RenderPass renderPass_ = RenderPass(), uint32_t subpass_ = 0, Pipeline basePipelineHandle_ = Pipeline(), int32_t basePipelineIndex_ = 0 )
      : sType( StructureType::eGraphicsPipelineCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , stageCount( stageCount_ )
      , pStages( pStages_ )
      , pVertexInputState( pVertexInputState_ )
      , pInputAssemblyState( pInputAssemblyState_ )
      , pTessellationState( pTessellationState_ )
      , pViewportState( pViewportState_ )
      , pRasterizationState( pRasterizationState_ )
      , pMultisampleState( pMultisampleState_ )
      , pDepthStencilState( pDepthStencilState_ )
      , pColorBlendState( pColorBlendState_ )
      , pDynamicState( pDynamicState_ )
      , layout( layout_ )
      , renderPass( renderPass_ )
      , subpass( subpass_ )
      , basePipelineHandle( basePipelineHandle_ )
      , basePipelineIndex( basePipelineIndex_ )
    {
    }

    GraphicsPipelineCreateInfo( VkGraphicsPipelineCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(GraphicsPipelineCreateInfo) );
    }

    GraphicsPipelineCreateInfo& operator=( VkGraphicsPipelineCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(GraphicsPipelineCreateInfo) );
      return *this;
    }

    GraphicsPipelineCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setFlags( PipelineCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setStageCount( uint32_t stageCount_ )
    {
      stageCount = stageCount_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPStages( const PipelineShaderStageCreateInfo* pStages_ )
    {
      pStages = pStages_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPVertexInputState( const PipelineVertexInputStateCreateInfo* pVertexInputState_ )
    {
      pVertexInputState = pVertexInputState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPInputAssemblyState( const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState_ )
    {
      pInputAssemblyState = pInputAssemblyState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPTessellationState( const PipelineTessellationStateCreateInfo* pTessellationState_ )
    {
      pTessellationState = pTessellationState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPViewportState( const PipelineViewportStateCreateInfo* pViewportState_ )
    {
      pViewportState = pViewportState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPRasterizationState( const PipelineRasterizationStateCreateInfo* pRasterizationState_ )
    {
      pRasterizationState = pRasterizationState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPMultisampleState( const PipelineMultisampleStateCreateInfo* pMultisampleState_ )
    {
      pMultisampleState = pMultisampleState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPDepthStencilState( const PipelineDepthStencilStateCreateInfo* pDepthStencilState_ )
    {
      pDepthStencilState = pDepthStencilState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPColorBlendState( const PipelineColorBlendStateCreateInfo* pColorBlendState_ )
    {
      pColorBlendState = pColorBlendState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setPDynamicState( const PipelineDynamicStateCreateInfo* pDynamicState_ )
    {
      pDynamicState = pDynamicState_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setLayout( PipelineLayout layout_ )
    {
      layout = layout_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setRenderPass( RenderPass renderPass_ )
    {
      renderPass = renderPass_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setSubpass( uint32_t subpass_ )
    {
      subpass = subpass_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineHandle( Pipeline basePipelineHandle_ )
    {
      basePipelineHandle = basePipelineHandle_;
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineIndex( int32_t basePipelineIndex_ )
    {
      basePipelineIndex = basePipelineIndex_;
      return *this;
    }

    operator const VkGraphicsPipelineCreateInfo&() const
    {
      return *reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(this);
    }

    bool operator==( GraphicsPipelineCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( stageCount == rhs.stageCount )
          && ( pStages == rhs.pStages )
          && ( pVertexInputState == rhs.pVertexInputState )
          && ( pInputAssemblyState == rhs.pInputAssemblyState )
          && ( pTessellationState == rhs.pTessellationState )
          && ( pViewportState == rhs.pViewportState )
          && ( pRasterizationState == rhs.pRasterizationState )
          && ( pMultisampleState == rhs.pMultisampleState )
          && ( pDepthStencilState == rhs.pDepthStencilState )
          && ( pColorBlendState == rhs.pColorBlendState )
          && ( pDynamicState == rhs.pDynamicState )
          && ( layout == rhs.layout )
          && ( renderPass == rhs.renderPass )
          && ( subpass == rhs.subpass )
          && ( basePipelineHandle == rhs.basePipelineHandle )
          && ( basePipelineIndex == rhs.basePipelineIndex );
    }

    bool operator!=( GraphicsPipelineCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineCreateFlags flags;
    uint32_t stageCount;
    const PipelineShaderStageCreateInfo* pStages;
    const PipelineVertexInputStateCreateInfo* pVertexInputState;
    const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState;
    const PipelineTessellationStateCreateInfo* pTessellationState;
    const PipelineViewportStateCreateInfo* pViewportState;
    const PipelineRasterizationStateCreateInfo* pRasterizationState;
    const PipelineMultisampleStateCreateInfo* pMultisampleState;
    const PipelineDepthStencilStateCreateInfo* pDepthStencilState;
    const PipelineColorBlendStateCreateInfo* pColorBlendState;
    const PipelineDynamicStateCreateInfo* pDynamicState;
    PipelineLayout layout;
    RenderPass renderPass;
    uint32_t subpass;
    Pipeline basePipelineHandle;
    int32_t basePipelineIndex;
  };
  static_assert( sizeof( GraphicsPipelineCreateInfo ) == sizeof( VkGraphicsPipelineCreateInfo ), "struct and wrapper have different size!" );

  struct PhysicalDeviceLimits
  {
    operator const VkPhysicalDeviceLimits&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceLimits*>(this);
    }

    bool operator==( PhysicalDeviceLimits const& rhs ) const
    {
      return ( maxImageDimension1D == rhs.maxImageDimension1D )
          && ( maxImageDimension2D == rhs.maxImageDimension2D )
          && ( maxImageDimension3D == rhs.maxImageDimension3D )
          && ( maxImageDimensionCube == rhs.maxImageDimensionCube )
          && ( maxImageArrayLayers == rhs.maxImageArrayLayers )
          && ( maxTexelBufferElements == rhs.maxTexelBufferElements )
          && ( maxUniformBufferRange == rhs.maxUniformBufferRange )
          && ( maxStorageBufferRange == rhs.maxStorageBufferRange )
          && ( maxPushConstantsSize == rhs.maxPushConstantsSize )
          && ( maxMemoryAllocationCount == rhs.maxMemoryAllocationCount )
          && ( maxSamplerAllocationCount == rhs.maxSamplerAllocationCount )
          && ( bufferImageGranularity == rhs.bufferImageGranularity )
          && ( sparseAddressSpaceSize == rhs.sparseAddressSpaceSize )
          && ( maxBoundDescriptorSets == rhs.maxBoundDescriptorSets )
          && ( maxPerStageDescriptorSamplers == rhs.maxPerStageDescriptorSamplers )
          && ( maxPerStageDescriptorUniformBuffers == rhs.maxPerStageDescriptorUniformBuffers )
          && ( maxPerStageDescriptorStorageBuffers == rhs.maxPerStageDescriptorStorageBuffers )
          && ( maxPerStageDescriptorSampledImages == rhs.maxPerStageDescriptorSampledImages )
          && ( maxPerStageDescriptorStorageImages == rhs.maxPerStageDescriptorStorageImages )
          && ( maxPerStageDescriptorInputAttachments == rhs.maxPerStageDescriptorInputAttachments )
          && ( maxPerStageResources == rhs.maxPerStageResources )
          && ( maxDescriptorSetSamplers == rhs.maxDescriptorSetSamplers )
          && ( maxDescriptorSetUniformBuffers == rhs.maxDescriptorSetUniformBuffers )
          && ( maxDescriptorSetUniformBuffersDynamic == rhs.maxDescriptorSetUniformBuffersDynamic )
          && ( maxDescriptorSetStorageBuffers == rhs.maxDescriptorSetStorageBuffers )
          && ( maxDescriptorSetStorageBuffersDynamic == rhs.maxDescriptorSetStorageBuffersDynamic )
          && ( maxDescriptorSetSampledImages == rhs.maxDescriptorSetSampledImages )
          && ( maxDescriptorSetStorageImages == rhs.maxDescriptorSetStorageImages )
          && ( maxDescriptorSetInputAttachments == rhs.maxDescriptorSetInputAttachments )
          && ( maxVertexInputAttributes == rhs.maxVertexInputAttributes )
          && ( maxVertexInputBindings == rhs.maxVertexInputBindings )
          && ( maxVertexInputAttributeOffset == rhs.maxVertexInputAttributeOffset )
          && ( maxVertexInputBindingStride == rhs.maxVertexInputBindingStride )
          && ( maxVertexOutputComponents == rhs.maxVertexOutputComponents )
          && ( maxTessellationGenerationLevel == rhs.maxTessellationGenerationLevel )
          && ( maxTessellationPatchSize == rhs.maxTessellationPatchSize )
          && ( maxTessellationControlPerVertexInputComponents == rhs.maxTessellationControlPerVertexInputComponents )
          && ( maxTessellationControlPerVertexOutputComponents == rhs.maxTessellationControlPerVertexOutputComponents )
          && ( maxTessellationControlPerPatchOutputComponents == rhs.maxTessellationControlPerPatchOutputComponents )
          && ( maxTessellationControlTotalOutputComponents == rhs.maxTessellationControlTotalOutputComponents )
          && ( maxTessellationEvaluationInputComponents == rhs.maxTessellationEvaluationInputComponents )
          && ( maxTessellationEvaluationOutputComponents == rhs.maxTessellationEvaluationOutputComponents )
          && ( maxGeometryShaderInvocations == rhs.maxGeometryShaderInvocations )
          && ( maxGeometryInputComponents == rhs.maxGeometryInputComponents )
          && ( maxGeometryOutputComponents == rhs.maxGeometryOutputComponents )
          && ( maxGeometryOutputVertices == rhs.maxGeometryOutputVertices )
          && ( maxGeometryTotalOutputComponents == rhs.maxGeometryTotalOutputComponents )
          && ( maxFragmentInputComponents == rhs.maxFragmentInputComponents )
          && ( maxFragmentOutputAttachments == rhs.maxFragmentOutputAttachments )
          && ( maxFragmentDualSrcAttachments == rhs.maxFragmentDualSrcAttachments )
          && ( maxFragmentCombinedOutputResources == rhs.maxFragmentCombinedOutputResources )
          && ( maxComputeSharedMemorySize == rhs.maxComputeSharedMemorySize )
          && ( memcmp( maxComputeWorkGroupCount, rhs.maxComputeWorkGroupCount, 3 * sizeof( uint32_t ) ) == 0 )
          && ( maxComputeWorkGroupInvocations == rhs.maxComputeWorkGroupInvocations )
          && ( memcmp( maxComputeWorkGroupSize, rhs.maxComputeWorkGroupSize, 3 * sizeof( uint32_t ) ) == 0 )
          && ( subPixelPrecisionBits == rhs.subPixelPrecisionBits )
          && ( subTexelPrecisionBits == rhs.subTexelPrecisionBits )
          && ( mipmapPrecisionBits == rhs.mipmapPrecisionBits )
          && ( maxDrawIndexedIndexValue == rhs.maxDrawIndexedIndexValue )
          && ( maxDrawIndirectCount == rhs.maxDrawIndirectCount )
          && ( maxSamplerLodBias == rhs.maxSamplerLodBias )
          && ( maxSamplerAnisotropy == rhs.maxSamplerAnisotropy )
          && ( maxViewports == rhs.maxViewports )
          && ( memcmp( maxViewportDimensions, rhs.maxViewportDimensions, 2 * sizeof( uint32_t ) ) == 0 )
          && ( memcmp( viewportBoundsRange, rhs.viewportBoundsRange, 2 * sizeof( float ) ) == 0 )
          && ( viewportSubPixelBits == rhs.viewportSubPixelBits )
          && ( minMemoryMapAlignment == rhs.minMemoryMapAlignment )
          && ( minTexelBufferOffsetAlignment == rhs.minTexelBufferOffsetAlignment )
          && ( minUniformBufferOffsetAlignment == rhs.minUniformBufferOffsetAlignment )
          && ( minStorageBufferOffsetAlignment == rhs.minStorageBufferOffsetAlignment )
          && ( minTexelOffset == rhs.minTexelOffset )
          && ( maxTexelOffset == rhs.maxTexelOffset )
          && ( minTexelGatherOffset == rhs.minTexelGatherOffset )
          && ( maxTexelGatherOffset == rhs.maxTexelGatherOffset )
          && ( minInterpolationOffset == rhs.minInterpolationOffset )
          && ( maxInterpolationOffset == rhs.maxInterpolationOffset )
          && ( subPixelInterpolationOffsetBits == rhs.subPixelInterpolationOffsetBits )
          && ( maxFramebufferWidth == rhs.maxFramebufferWidth )
          && ( maxFramebufferHeight == rhs.maxFramebufferHeight )
          && ( maxFramebufferLayers == rhs.maxFramebufferLayers )
          && ( framebufferColorSampleCounts == rhs.framebufferColorSampleCounts )
          && ( framebufferDepthSampleCounts == rhs.framebufferDepthSampleCounts )
          && ( framebufferStencilSampleCounts == rhs.framebufferStencilSampleCounts )
          && ( framebufferNoAttachmentsSampleCounts == rhs.framebufferNoAttachmentsSampleCounts )
          && ( maxColorAttachments == rhs.maxColorAttachments )
          && ( sampledImageColorSampleCounts == rhs.sampledImageColorSampleCounts )
          && ( sampledImageIntegerSampleCounts == rhs.sampledImageIntegerSampleCounts )
          && ( sampledImageDepthSampleCounts == rhs.sampledImageDepthSampleCounts )
          && ( sampledImageStencilSampleCounts == rhs.sampledImageStencilSampleCounts )
          && ( storageImageSampleCounts == rhs.storageImageSampleCounts )
          && ( maxSampleMaskWords == rhs.maxSampleMaskWords )
          && ( timestampComputeAndGraphics == rhs.timestampComputeAndGraphics )
          && ( timestampPeriod == rhs.timestampPeriod )
          && ( maxClipDistances == rhs.maxClipDistances )
          && ( maxCullDistances == rhs.maxCullDistances )
          && ( maxCombinedClipAndCullDistances == rhs.maxCombinedClipAndCullDistances )
          && ( discreteQueuePriorities == rhs.discreteQueuePriorities )
          && ( memcmp( pointSizeRange, rhs.pointSizeRange, 2 * sizeof( float ) ) == 0 )
          && ( memcmp( lineWidthRange, rhs.lineWidthRange, 2 * sizeof( float ) ) == 0 )
          && ( pointSizeGranularity == rhs.pointSizeGranularity )
          && ( lineWidthGranularity == rhs.lineWidthGranularity )
          && ( strictLines == rhs.strictLines )
          && ( standardSampleLocations == rhs.standardSampleLocations )
          && ( optimalBufferCopyOffsetAlignment == rhs.optimalBufferCopyOffsetAlignment )
          && ( optimalBufferCopyRowPitchAlignment == rhs.optimalBufferCopyRowPitchAlignment )
          && ( nonCoherentAtomSize == rhs.nonCoherentAtomSize );
    }

    bool operator!=( PhysicalDeviceLimits const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t maxImageDimension1D;
    uint32_t maxImageDimension2D;
    uint32_t maxImageDimension3D;
    uint32_t maxImageDimensionCube;
    uint32_t maxImageArrayLayers;
    uint32_t maxTexelBufferElements;
    uint32_t maxUniformBufferRange;
    uint32_t maxStorageBufferRange;
    uint32_t maxPushConstantsSize;
    uint32_t maxMemoryAllocationCount;
    uint32_t maxSamplerAllocationCount;
    DeviceSize bufferImageGranularity;
    DeviceSize sparseAddressSpaceSize;
    uint32_t maxBoundDescriptorSets;
    uint32_t maxPerStageDescriptorSamplers;
    uint32_t maxPerStageDescriptorUniformBuffers;
    uint32_t maxPerStageDescriptorStorageBuffers;
    uint32_t maxPerStageDescriptorSampledImages;
    uint32_t maxPerStageDescriptorStorageImages;
    uint32_t maxPerStageDescriptorInputAttachments;
    uint32_t maxPerStageResources;
    uint32_t maxDescriptorSetSamplers;
    uint32_t maxDescriptorSetUniformBuffers;
    uint32_t maxDescriptorSetUniformBuffersDynamic;
    uint32_t maxDescriptorSetStorageBuffers;
    uint32_t maxDescriptorSetStorageBuffersDynamic;
    uint32_t maxDescriptorSetSampledImages;
    uint32_t maxDescriptorSetStorageImages;
    uint32_t maxDescriptorSetInputAttachments;
    uint32_t maxVertexInputAttributes;
    uint32_t maxVertexInputBindings;
    uint32_t maxVertexInputAttributeOffset;
    uint32_t maxVertexInputBindingStride;
    uint32_t maxVertexOutputComponents;
    uint32_t maxTessellationGenerationLevel;
    uint32_t maxTessellationPatchSize;
    uint32_t maxTessellationControlPerVertexInputComponents;
    uint32_t maxTessellationControlPerVertexOutputComponents;
    uint32_t maxTessellationControlPerPatchOutputComponents;
    uint32_t maxTessellationControlTotalOutputComponents;
    uint32_t maxTessellationEvaluationInputComponents;
    uint32_t maxTessellationEvaluationOutputComponents;
    uint32_t maxGeometryShaderInvocations;
    uint32_t maxGeometryInputComponents;
    uint32_t maxGeometryOutputComponents;
    uint32_t maxGeometryOutputVertices;
    uint32_t maxGeometryTotalOutputComponents;
    uint32_t maxFragmentInputComponents;
    uint32_t maxFragmentOutputAttachments;
    uint32_t maxFragmentDualSrcAttachments;
    uint32_t maxFragmentCombinedOutputResources;
    uint32_t maxComputeSharedMemorySize;
    uint32_t maxComputeWorkGroupCount[3];
    uint32_t maxComputeWorkGroupInvocations;
    uint32_t maxComputeWorkGroupSize[3];
    uint32_t subPixelPrecisionBits;
    uint32_t subTexelPrecisionBits;
    uint32_t mipmapPrecisionBits;
    uint32_t maxDrawIndexedIndexValue;
    uint32_t maxDrawIndirectCount;
    float maxSamplerLodBias;
    float maxSamplerAnisotropy;
    uint32_t maxViewports;
    uint32_t maxViewportDimensions[2];
    float viewportBoundsRange[2];
    uint32_t viewportSubPixelBits;
    size_t minMemoryMapAlignment;
    DeviceSize minTexelBufferOffsetAlignment;
    DeviceSize minUniformBufferOffsetAlignment;
    DeviceSize minStorageBufferOffsetAlignment;
    int32_t minTexelOffset;
    uint32_t maxTexelOffset;
    int32_t minTexelGatherOffset;
    uint32_t maxTexelGatherOffset;
    float minInterpolationOffset;
    float maxInterpolationOffset;
    uint32_t subPixelInterpolationOffsetBits;
    uint32_t maxFramebufferWidth;
    uint32_t maxFramebufferHeight;
    uint32_t maxFramebufferLayers;
    SampleCountFlags framebufferColorSampleCounts;
    SampleCountFlags framebufferDepthSampleCounts;
    SampleCountFlags framebufferStencilSampleCounts;
    SampleCountFlags framebufferNoAttachmentsSampleCounts;
    uint32_t maxColorAttachments;
    SampleCountFlags sampledImageColorSampleCounts;
    SampleCountFlags sampledImageIntegerSampleCounts;
    SampleCountFlags sampledImageDepthSampleCounts;
    SampleCountFlags sampledImageStencilSampleCounts;
    SampleCountFlags storageImageSampleCounts;
    uint32_t maxSampleMaskWords;
    Bool32 timestampComputeAndGraphics;
    float timestampPeriod;
    uint32_t maxClipDistances;
    uint32_t maxCullDistances;
    uint32_t maxCombinedClipAndCullDistances;
    uint32_t discreteQueuePriorities;
    float pointSizeRange[2];
    float lineWidthRange[2];
    float pointSizeGranularity;
    float lineWidthGranularity;
    Bool32 strictLines;
    Bool32 standardSampleLocations;
    DeviceSize optimalBufferCopyOffsetAlignment;
    DeviceSize optimalBufferCopyRowPitchAlignment;
    DeviceSize nonCoherentAtomSize;
  };
  static_assert( sizeof( PhysicalDeviceLimits ) == sizeof( VkPhysicalDeviceLimits ), "struct and wrapper have different size!" );

  struct PhysicalDeviceProperties
  {
    operator const VkPhysicalDeviceProperties&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceProperties*>(this);
    }

    bool operator==( PhysicalDeviceProperties const& rhs ) const
    {
      return ( apiVersion == rhs.apiVersion )
          && ( driverVersion == rhs.driverVersion )
          && ( vendorID == rhs.vendorID )
          && ( deviceID == rhs.deviceID )
          && ( deviceType == rhs.deviceType )
          && ( memcmp( deviceName, rhs.deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE * sizeof( char ) ) == 0 )
          && ( memcmp( pipelineCacheUUID, rhs.pipelineCacheUUID, VK_UUID_SIZE * sizeof( uint8_t ) ) == 0 )
          && ( limits == rhs.limits )
          && ( sparseProperties == rhs.sparseProperties );
    }

    bool operator!=( PhysicalDeviceProperties const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t apiVersion;
    uint32_t driverVersion;
    uint32_t vendorID;
    uint32_t deviceID;
    PhysicalDeviceType deviceType;
    char deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
    uint8_t pipelineCacheUUID[VK_UUID_SIZE];
    PhysicalDeviceLimits limits;
    PhysicalDeviceSparseProperties sparseProperties;
  };
  static_assert( sizeof( PhysicalDeviceProperties ) == sizeof( VkPhysicalDeviceProperties ), "struct and wrapper have different size!" );

  struct PhysicalDeviceProperties2KHR
  {
    operator const VkPhysicalDeviceProperties2KHR&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceProperties2KHR*>(this);
    }

    bool operator==( PhysicalDeviceProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( properties == rhs.properties );
    }

    bool operator!=( PhysicalDeviceProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    PhysicalDeviceProperties properties;
  };
  static_assert( sizeof( PhysicalDeviceProperties2KHR ) == sizeof( VkPhysicalDeviceProperties2KHR ), "struct and wrapper have different size!" );

  struct ImageFormatProperties2KHR
  {
    operator const VkImageFormatProperties2KHR&() const
    {
      return *reinterpret_cast<const VkImageFormatProperties2KHR*>(this);
    }

    bool operator==( ImageFormatProperties2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( imageFormatProperties == rhs.imageFormatProperties );
    }

    bool operator!=( ImageFormatProperties2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    ImageFormatProperties imageFormatProperties;
  };
  static_assert( sizeof( ImageFormatProperties2KHR ) == sizeof( VkImageFormatProperties2KHR ), "struct and wrapper have different size!" );

  struct PhysicalDeviceSparseImageFormatInfo2KHR
  {
    PhysicalDeviceSparseImageFormatInfo2KHR( Format format_ = Format::eUndefined, ImageType type_ = ImageType::e1D, SampleCountFlagBits samples_ = SampleCountFlagBits::e1, ImageUsageFlags usage_ = ImageUsageFlags(), ImageTiling tiling_ = ImageTiling::eOptimal )
      : sType( StructureType::ePhysicalDeviceSparseImageFormatInfo2KHR )
      , pNext( nullptr )
      , format( format_ )
      , type( type_ )
      , samples( samples_ )
      , usage( usage_ )
      , tiling( tiling_ )
    {
    }

    PhysicalDeviceSparseImageFormatInfo2KHR( VkPhysicalDeviceSparseImageFormatInfo2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceSparseImageFormatInfo2KHR) );
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& operator=( VkPhysicalDeviceSparseImageFormatInfo2KHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(PhysicalDeviceSparseImageFormatInfo2KHR) );
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setType( ImageType type_ )
    {
      type = type_;
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setSamples( SampleCountFlagBits samples_ )
    {
      samples = samples_;
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setUsage( ImageUsageFlags usage_ )
    {
      usage = usage_;
      return *this;
    }

    PhysicalDeviceSparseImageFormatInfo2KHR& setTiling( ImageTiling tiling_ )
    {
      tiling = tiling_;
      return *this;
    }

    operator const VkPhysicalDeviceSparseImageFormatInfo2KHR&() const
    {
      return *reinterpret_cast<const VkPhysicalDeviceSparseImageFormatInfo2KHR*>(this);
    }

    bool operator==( PhysicalDeviceSparseImageFormatInfo2KHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( format == rhs.format )
          && ( type == rhs.type )
          && ( samples == rhs.samples )
          && ( usage == rhs.usage )
          && ( tiling == rhs.tiling );
    }

    bool operator!=( PhysicalDeviceSparseImageFormatInfo2KHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    Format format;
    ImageType type;
    SampleCountFlagBits samples;
    ImageUsageFlags usage;
    ImageTiling tiling;
  };
  static_assert( sizeof( PhysicalDeviceSparseImageFormatInfo2KHR ) == sizeof( VkPhysicalDeviceSparseImageFormatInfo2KHR ), "struct and wrapper have different size!" );

  enum class AttachmentDescriptionFlagBits
  {
    eMayAlias = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT
  };

  using AttachmentDescriptionFlags = Flags<AttachmentDescriptionFlagBits, VkAttachmentDescriptionFlags>;

  VULKAN_HPP_INLINE AttachmentDescriptionFlags operator|( AttachmentDescriptionFlagBits bit0, AttachmentDescriptionFlagBits bit1 )
  {
    return AttachmentDescriptionFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE AttachmentDescriptionFlags operator~( AttachmentDescriptionFlagBits bits )
  {
    return ~( AttachmentDescriptionFlags( bits ) );
  }

  template <> struct FlagTraits<AttachmentDescriptionFlagBits>
  {
    enum
    {
      allFlags = VkFlags(AttachmentDescriptionFlagBits::eMayAlias)
    };
  };

  struct AttachmentDescription
  {
    AttachmentDescription( AttachmentDescriptionFlags flags_ = AttachmentDescriptionFlags(), Format format_ = Format::eUndefined, SampleCountFlagBits samples_ = SampleCountFlagBits::e1, AttachmentLoadOp loadOp_ = AttachmentLoadOp::eLoad, AttachmentStoreOp storeOp_ = AttachmentStoreOp::eStore, AttachmentLoadOp stencilLoadOp_ = AttachmentLoadOp::eLoad, AttachmentStoreOp stencilStoreOp_ = AttachmentStoreOp::eStore, ImageLayout initialLayout_ = ImageLayout::eUndefined, ImageLayout finalLayout_ = ImageLayout::eUndefined )
      : flags( flags_ )
      , format( format_ )
      , samples( samples_ )
      , loadOp( loadOp_ )
      , storeOp( storeOp_ )
      , stencilLoadOp( stencilLoadOp_ )
      , stencilStoreOp( stencilStoreOp_ )
      , initialLayout( initialLayout_ )
      , finalLayout( finalLayout_ )
    {
    }

    AttachmentDescription( VkAttachmentDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(AttachmentDescription) );
    }

    AttachmentDescription& operator=( VkAttachmentDescription const & rhs )
    {
      memcpy( this, &rhs, sizeof(AttachmentDescription) );
      return *this;
    }

    AttachmentDescription& setFlags( AttachmentDescriptionFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    AttachmentDescription& setFormat( Format format_ )
    {
      format = format_;
      return *this;
    }

    AttachmentDescription& setSamples( SampleCountFlagBits samples_ )
    {
      samples = samples_;
      return *this;
    }

    AttachmentDescription& setLoadOp( AttachmentLoadOp loadOp_ )
    {
      loadOp = loadOp_;
      return *this;
    }

    AttachmentDescription& setStoreOp( AttachmentStoreOp storeOp_ )
    {
      storeOp = storeOp_;
      return *this;
    }

    AttachmentDescription& setStencilLoadOp( AttachmentLoadOp stencilLoadOp_ )
    {
      stencilLoadOp = stencilLoadOp_;
      return *this;
    }

    AttachmentDescription& setStencilStoreOp( AttachmentStoreOp stencilStoreOp_ )
    {
      stencilStoreOp = stencilStoreOp_;
      return *this;
    }

    AttachmentDescription& setInitialLayout( ImageLayout initialLayout_ )
    {
      initialLayout = initialLayout_;
      return *this;
    }

    AttachmentDescription& setFinalLayout( ImageLayout finalLayout_ )
    {
      finalLayout = finalLayout_;
      return *this;
    }

    operator const VkAttachmentDescription&() const
    {
      return *reinterpret_cast<const VkAttachmentDescription*>(this);
    }

    bool operator==( AttachmentDescription const& rhs ) const
    {
      return ( flags == rhs.flags )
          && ( format == rhs.format )
          && ( samples == rhs.samples )
          && ( loadOp == rhs.loadOp )
          && ( storeOp == rhs.storeOp )
          && ( stencilLoadOp == rhs.stencilLoadOp )
          && ( stencilStoreOp == rhs.stencilStoreOp )
          && ( initialLayout == rhs.initialLayout )
          && ( finalLayout == rhs.finalLayout );
    }

    bool operator!=( AttachmentDescription const& rhs ) const
    {
      return !operator==( rhs );
    }

    AttachmentDescriptionFlags flags;
    Format format;
    SampleCountFlagBits samples;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
    AttachmentLoadOp stencilLoadOp;
    AttachmentStoreOp stencilStoreOp;
    ImageLayout initialLayout;
    ImageLayout finalLayout;
  };
  static_assert( sizeof( AttachmentDescription ) == sizeof( VkAttachmentDescription ), "struct and wrapper have different size!" );

  enum class StencilFaceFlagBits
  {
    eFront = VK_STENCIL_FACE_FRONT_BIT,
    eBack = VK_STENCIL_FACE_BACK_BIT,
    eVkStencilFrontAndBack = VK_STENCIL_FRONT_AND_BACK
  };

  using StencilFaceFlags = Flags<StencilFaceFlagBits, VkStencilFaceFlags>;

  VULKAN_HPP_INLINE StencilFaceFlags operator|( StencilFaceFlagBits bit0, StencilFaceFlagBits bit1 )
  {
    return StencilFaceFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE StencilFaceFlags operator~( StencilFaceFlagBits bits )
  {
    return ~( StencilFaceFlags( bits ) );
  }

  template <> struct FlagTraits<StencilFaceFlagBits>
  {
    enum
    {
      allFlags = VkFlags(StencilFaceFlagBits::eFront) | VkFlags(StencilFaceFlagBits::eBack) | VkFlags(StencilFaceFlagBits::eVkStencilFrontAndBack)
    };
  };

  enum class DescriptorPoolCreateFlagBits
  {
    eFreeDescriptorSet = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
  };

  using DescriptorPoolCreateFlags = Flags<DescriptorPoolCreateFlagBits, VkDescriptorPoolCreateFlags>;

  VULKAN_HPP_INLINE DescriptorPoolCreateFlags operator|( DescriptorPoolCreateFlagBits bit0, DescriptorPoolCreateFlagBits bit1 )
  {
    return DescriptorPoolCreateFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE DescriptorPoolCreateFlags operator~( DescriptorPoolCreateFlagBits bits )
  {
    return ~( DescriptorPoolCreateFlags( bits ) );
  }

  template <> struct FlagTraits<DescriptorPoolCreateFlagBits>
  {
    enum
    {
      allFlags = VkFlags(DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
    };
  };

  struct DescriptorPoolCreateInfo
  {
    DescriptorPoolCreateInfo( DescriptorPoolCreateFlags flags_ = DescriptorPoolCreateFlags(), uint32_t maxSets_ = 0, uint32_t poolSizeCount_ = 0, const DescriptorPoolSize* pPoolSizes_ = nullptr )
      : sType( StructureType::eDescriptorPoolCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , maxSets( maxSets_ )
      , poolSizeCount( poolSizeCount_ )
      , pPoolSizes( pPoolSizes_ )
    {
    }

    DescriptorPoolCreateInfo( VkDescriptorPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorPoolCreateInfo) );
    }

    DescriptorPoolCreateInfo& operator=( VkDescriptorPoolCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(DescriptorPoolCreateInfo) );
      return *this;
    }

    DescriptorPoolCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DescriptorPoolCreateInfo& setFlags( DescriptorPoolCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    DescriptorPoolCreateInfo& setMaxSets( uint32_t maxSets_ )
    {
      maxSets = maxSets_;
      return *this;
    }

    DescriptorPoolCreateInfo& setPoolSizeCount( uint32_t poolSizeCount_ )
    {
      poolSizeCount = poolSizeCount_;
      return *this;
    }

    DescriptorPoolCreateInfo& setPPoolSizes( const DescriptorPoolSize* pPoolSizes_ )
    {
      pPoolSizes = pPoolSizes_;
      return *this;
    }

    operator const VkDescriptorPoolCreateInfo&() const
    {
      return *reinterpret_cast<const VkDescriptorPoolCreateInfo*>(this);
    }

    bool operator==( DescriptorPoolCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( maxSets == rhs.maxSets )
          && ( poolSizeCount == rhs.poolSizeCount )
          && ( pPoolSizes == rhs.pPoolSizes );
    }

    bool operator!=( DescriptorPoolCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DescriptorPoolCreateFlags flags;
    uint32_t maxSets;
    uint32_t poolSizeCount;
    const DescriptorPoolSize* pPoolSizes;
  };
  static_assert( sizeof( DescriptorPoolCreateInfo ) == sizeof( VkDescriptorPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class DependencyFlagBits
  {
    eByRegion = VK_DEPENDENCY_BY_REGION_BIT
  };

  using DependencyFlags = Flags<DependencyFlagBits, VkDependencyFlags>;

  VULKAN_HPP_INLINE DependencyFlags operator|( DependencyFlagBits bit0, DependencyFlagBits bit1 )
  {
    return DependencyFlags( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE DependencyFlags operator~( DependencyFlagBits bits )
  {
    return ~( DependencyFlags( bits ) );
  }

  template <> struct FlagTraits<DependencyFlagBits>
  {
    enum
    {
      allFlags = VkFlags(DependencyFlagBits::eByRegion)
    };
  };

  struct SubpassDependency
  {
    SubpassDependency( uint32_t srcSubpass_ = 0, uint32_t dstSubpass_ = 0, PipelineStageFlags srcStageMask_ = PipelineStageFlags(), PipelineStageFlags dstStageMask_ = PipelineStageFlags(), AccessFlags srcAccessMask_ = AccessFlags(), AccessFlags dstAccessMask_ = AccessFlags(), DependencyFlags dependencyFlags_ = DependencyFlags() )
      : srcSubpass( srcSubpass_ )
      , dstSubpass( dstSubpass_ )
      , srcStageMask( srcStageMask_ )
      , dstStageMask( dstStageMask_ )
      , srcAccessMask( srcAccessMask_ )
      , dstAccessMask( dstAccessMask_ )
      , dependencyFlags( dependencyFlags_ )
    {
    }

    SubpassDependency( VkSubpassDependency const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubpassDependency) );
    }

    SubpassDependency& operator=( VkSubpassDependency const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubpassDependency) );
      return *this;
    }

    SubpassDependency& setSrcSubpass( uint32_t srcSubpass_ )
    {
      srcSubpass = srcSubpass_;
      return *this;
    }

    SubpassDependency& setDstSubpass( uint32_t dstSubpass_ )
    {
      dstSubpass = dstSubpass_;
      return *this;
    }

    SubpassDependency& setSrcStageMask( PipelineStageFlags srcStageMask_ )
    {
      srcStageMask = srcStageMask_;
      return *this;
    }

    SubpassDependency& setDstStageMask( PipelineStageFlags dstStageMask_ )
    {
      dstStageMask = dstStageMask_;
      return *this;
    }

    SubpassDependency& setSrcAccessMask( AccessFlags srcAccessMask_ )
    {
      srcAccessMask = srcAccessMask_;
      return *this;
    }

    SubpassDependency& setDstAccessMask( AccessFlags dstAccessMask_ )
    {
      dstAccessMask = dstAccessMask_;
      return *this;
    }

    SubpassDependency& setDependencyFlags( DependencyFlags dependencyFlags_ )
    {
      dependencyFlags = dependencyFlags_;
      return *this;
    }

    operator const VkSubpassDependency&() const
    {
      return *reinterpret_cast<const VkSubpassDependency*>(this);
    }

    bool operator==( SubpassDependency const& rhs ) const
    {
      return ( srcSubpass == rhs.srcSubpass )
          && ( dstSubpass == rhs.dstSubpass )
          && ( srcStageMask == rhs.srcStageMask )
          && ( dstStageMask == rhs.dstStageMask )
          && ( srcAccessMask == rhs.srcAccessMask )
          && ( dstAccessMask == rhs.dstAccessMask )
          && ( dependencyFlags == rhs.dependencyFlags );
    }

    bool operator!=( SubpassDependency const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t srcSubpass;
    uint32_t dstSubpass;
    PipelineStageFlags srcStageMask;
    PipelineStageFlags dstStageMask;
    AccessFlags srcAccessMask;
    AccessFlags dstAccessMask;
    DependencyFlags dependencyFlags;
  };
  static_assert( sizeof( SubpassDependency ) == sizeof( VkSubpassDependency ), "struct and wrapper have different size!" );

  struct RenderPassCreateInfo
  {
    RenderPassCreateInfo( RenderPassCreateFlags flags_ = RenderPassCreateFlags(), uint32_t attachmentCount_ = 0, const AttachmentDescription* pAttachments_ = nullptr, uint32_t subpassCount_ = 0, const SubpassDescription* pSubpasses_ = nullptr, uint32_t dependencyCount_ = 0, const SubpassDependency* pDependencies_ = nullptr )
      : sType( StructureType::eRenderPassCreateInfo )
      , pNext( nullptr )
      , flags( flags_ )
      , attachmentCount( attachmentCount_ )
      , pAttachments( pAttachments_ )
      , subpassCount( subpassCount_ )
      , pSubpasses( pSubpasses_ )
      , dependencyCount( dependencyCount_ )
      , pDependencies( pDependencies_ )
    {
    }

    RenderPassCreateInfo( VkRenderPassCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(RenderPassCreateInfo) );
    }

    RenderPassCreateInfo& operator=( VkRenderPassCreateInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(RenderPassCreateInfo) );
      return *this;
    }

    RenderPassCreateInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    RenderPassCreateInfo& setFlags( RenderPassCreateFlags flags_ )
    {
      flags = flags_;
      return *this;
    }

    RenderPassCreateInfo& setAttachmentCount( uint32_t attachmentCount_ )
    {
      attachmentCount = attachmentCount_;
      return *this;
    }

    RenderPassCreateInfo& setPAttachments( const AttachmentDescription* pAttachments_ )
    {
      pAttachments = pAttachments_;
      return *this;
    }

    RenderPassCreateInfo& setSubpassCount( uint32_t subpassCount_ )
    {
      subpassCount = subpassCount_;
      return *this;
    }

    RenderPassCreateInfo& setPSubpasses( const SubpassDescription* pSubpasses_ )
    {
      pSubpasses = pSubpasses_;
      return *this;
    }

    RenderPassCreateInfo& setDependencyCount( uint32_t dependencyCount_ )
    {
      dependencyCount = dependencyCount_;
      return *this;
    }

    RenderPassCreateInfo& setPDependencies( const SubpassDependency* pDependencies_ )
    {
      pDependencies = pDependencies_;
      return *this;
    }

    operator const VkRenderPassCreateInfo&() const
    {
      return *reinterpret_cast<const VkRenderPassCreateInfo*>(this);
    }

    bool operator==( RenderPassCreateInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( attachmentCount == rhs.attachmentCount )
          && ( pAttachments == rhs.pAttachments )
          && ( subpassCount == rhs.subpassCount )
          && ( pSubpasses == rhs.pSubpasses )
          && ( dependencyCount == rhs.dependencyCount )
          && ( pDependencies == rhs.pDependencies );
    }

    bool operator!=( RenderPassCreateInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    RenderPassCreateFlags flags;
    uint32_t attachmentCount;
    const AttachmentDescription* pAttachments;
    uint32_t subpassCount;
    const SubpassDescription* pSubpasses;
    uint32_t dependencyCount;
    const SubpassDependency* pDependencies;
  };
  static_assert( sizeof( RenderPassCreateInfo ) == sizeof( VkRenderPassCreateInfo ), "struct and wrapper have different size!" );

  enum class PresentModeKHR
  {
    eImmediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
    eMailbox = VK_PRESENT_MODE_MAILBOX_KHR,
    eFifo = VK_PRESENT_MODE_FIFO_KHR,
    eFifoRelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR
  };

  enum class ColorSpaceKHR
  {
    eSrgbNonlinear = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    eDisplayP3LinearEXT = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
    eDisplayP3NonlinearEXT = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,
    eScrgbLinearEXT = VK_COLOR_SPACE_SCRGB_LINEAR_EXT,
    eScrgbNonlinearEXT = VK_COLOR_SPACE_SCRGB_NONLINEAR_EXT,
    eDciP3LinearEXT = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,
    eDciP3NonlinearEXT = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,
    eBt709LinearEXT = VK_COLOR_SPACE_BT709_LINEAR_EXT,
    eBt709NonlinearEXT = VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
    eBt2020LinearEXT = VK_COLOR_SPACE_BT2020_LINEAR_EXT,
    eBt2020NonlinearEXT = VK_COLOR_SPACE_BT2020_NONLINEAR_EXT,
    eAdobergbLinearEXT = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,
    eAdobergbNonlinearEXT = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT
  };

  struct SurfaceFormatKHR
  {
    operator const VkSurfaceFormatKHR&() const
    {
      return *reinterpret_cast<const VkSurfaceFormatKHR*>(this);
    }

    bool operator==( SurfaceFormatKHR const& rhs ) const
    {
      return ( format == rhs.format )
          && ( colorSpace == rhs.colorSpace );
    }

    bool operator!=( SurfaceFormatKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    Format format;
    ColorSpaceKHR colorSpace;
  };
  static_assert( sizeof( SurfaceFormatKHR ) == sizeof( VkSurfaceFormatKHR ), "struct and wrapper have different size!" );

  enum class DisplayPlaneAlphaFlagBitsKHR
  {
    eOpaque = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR,
    eGlobal = VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR,
    ePerPixel = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR,
    ePerPixelPremultiplied = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR
  };

  using DisplayPlaneAlphaFlagsKHR = Flags<DisplayPlaneAlphaFlagBitsKHR, VkDisplayPlaneAlphaFlagsKHR>;

  VULKAN_HPP_INLINE DisplayPlaneAlphaFlagsKHR operator|( DisplayPlaneAlphaFlagBitsKHR bit0, DisplayPlaneAlphaFlagBitsKHR bit1 )
  {
    return DisplayPlaneAlphaFlagsKHR( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE DisplayPlaneAlphaFlagsKHR operator~( DisplayPlaneAlphaFlagBitsKHR bits )
  {
    return ~( DisplayPlaneAlphaFlagsKHR( bits ) );
  }

  template <> struct FlagTraits<DisplayPlaneAlphaFlagBitsKHR>
  {
    enum
    {
      allFlags = VkFlags(DisplayPlaneAlphaFlagBitsKHR::eOpaque) | VkFlags(DisplayPlaneAlphaFlagBitsKHR::eGlobal) | VkFlags(DisplayPlaneAlphaFlagBitsKHR::ePerPixel) | VkFlags(DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied)
    };
  };

  struct DisplayPlaneCapabilitiesKHR
  {
    operator const VkDisplayPlaneCapabilitiesKHR&() const
    {
      return *reinterpret_cast<const VkDisplayPlaneCapabilitiesKHR*>(this);
    }

    bool operator==( DisplayPlaneCapabilitiesKHR const& rhs ) const
    {
      return ( supportedAlpha == rhs.supportedAlpha )
          && ( minSrcPosition == rhs.minSrcPosition )
          && ( maxSrcPosition == rhs.maxSrcPosition )
          && ( minSrcExtent == rhs.minSrcExtent )
          && ( maxSrcExtent == rhs.maxSrcExtent )
          && ( minDstPosition == rhs.minDstPosition )
          && ( maxDstPosition == rhs.maxDstPosition )
          && ( minDstExtent == rhs.minDstExtent )
          && ( maxDstExtent == rhs.maxDstExtent );
    }

    bool operator!=( DisplayPlaneCapabilitiesKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    DisplayPlaneAlphaFlagsKHR supportedAlpha;
    Offset2D minSrcPosition;
    Offset2D maxSrcPosition;
    Extent2D minSrcExtent;
    Extent2D maxSrcExtent;
    Offset2D minDstPosition;
    Offset2D maxDstPosition;
    Extent2D minDstExtent;
    Extent2D maxDstExtent;
  };
  static_assert( sizeof( DisplayPlaneCapabilitiesKHR ) == sizeof( VkDisplayPlaneCapabilitiesKHR ), "struct and wrapper have different size!" );

  enum class CompositeAlphaFlagBitsKHR
  {
    eOpaque = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    ePreMultiplied = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    ePostMultiplied = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    eInherit = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
  };

  using CompositeAlphaFlagsKHR = Flags<CompositeAlphaFlagBitsKHR, VkCompositeAlphaFlagsKHR>;

  VULKAN_HPP_INLINE CompositeAlphaFlagsKHR operator|( CompositeAlphaFlagBitsKHR bit0, CompositeAlphaFlagBitsKHR bit1 )
  {
    return CompositeAlphaFlagsKHR( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE CompositeAlphaFlagsKHR operator~( CompositeAlphaFlagBitsKHR bits )
  {
    return ~( CompositeAlphaFlagsKHR( bits ) );
  }

  template <> struct FlagTraits<CompositeAlphaFlagBitsKHR>
  {
    enum
    {
      allFlags = VkFlags(CompositeAlphaFlagBitsKHR::eOpaque) | VkFlags(CompositeAlphaFlagBitsKHR::ePreMultiplied) | VkFlags(CompositeAlphaFlagBitsKHR::ePostMultiplied) | VkFlags(CompositeAlphaFlagBitsKHR::eInherit)
    };
  };

  enum class SurfaceTransformFlagBitsKHR
  {
    eIdentity = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    eRotate90 = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
    eRotate180 = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
    eRotate270 = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
    eHorizontalMirror = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
    eHorizontalMirrorRotate90 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
    eHorizontalMirrorRotate180 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
    eHorizontalMirrorRotate270 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
    eInherit = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR
  };

  using SurfaceTransformFlagsKHR = Flags<SurfaceTransformFlagBitsKHR, VkSurfaceTransformFlagsKHR>;

  VULKAN_HPP_INLINE SurfaceTransformFlagsKHR operator|( SurfaceTransformFlagBitsKHR bit0, SurfaceTransformFlagBitsKHR bit1 )
  {
    return SurfaceTransformFlagsKHR( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE SurfaceTransformFlagsKHR operator~( SurfaceTransformFlagBitsKHR bits )
  {
    return ~( SurfaceTransformFlagsKHR( bits ) );
  }

  template <> struct FlagTraits<SurfaceTransformFlagBitsKHR>
  {
    enum
    {
      allFlags = VkFlags(SurfaceTransformFlagBitsKHR::eIdentity) | VkFlags(SurfaceTransformFlagBitsKHR::eRotate90) | VkFlags(SurfaceTransformFlagBitsKHR::eRotate180) | VkFlags(SurfaceTransformFlagBitsKHR::eRotate270) | VkFlags(SurfaceTransformFlagBitsKHR::eHorizontalMirror) | VkFlags(SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) | VkFlags(SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) | VkFlags(SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) | VkFlags(SurfaceTransformFlagBitsKHR::eInherit)
    };
  };

  struct DisplayPropertiesKHR
  {
    operator const VkDisplayPropertiesKHR&() const
    {
      return *reinterpret_cast<const VkDisplayPropertiesKHR*>(this);
    }

    bool operator==( DisplayPropertiesKHR const& rhs ) const
    {
      return ( display == rhs.display )
          && ( displayName == rhs.displayName )
          && ( physicalDimensions == rhs.physicalDimensions )
          && ( physicalResolution == rhs.physicalResolution )
          && ( supportedTransforms == rhs.supportedTransforms )
          && ( planeReorderPossible == rhs.planeReorderPossible )
          && ( persistentContent == rhs.persistentContent );
    }

    bool operator!=( DisplayPropertiesKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    DisplayKHR display;
    const char* displayName;
    Extent2D physicalDimensions;
    Extent2D physicalResolution;
    SurfaceTransformFlagsKHR supportedTransforms;
    Bool32 planeReorderPossible;
    Bool32 persistentContent;
  };
  static_assert( sizeof( DisplayPropertiesKHR ) == sizeof( VkDisplayPropertiesKHR ), "struct and wrapper have different size!" );

  struct DisplaySurfaceCreateInfoKHR
  {
    DisplaySurfaceCreateInfoKHR( DisplaySurfaceCreateFlagsKHR flags_ = DisplaySurfaceCreateFlagsKHR(), DisplayModeKHR displayMode_ = DisplayModeKHR(), uint32_t planeIndex_ = 0, uint32_t planeStackIndex_ = 0, SurfaceTransformFlagBitsKHR transform_ = SurfaceTransformFlagBitsKHR::eIdentity, float globalAlpha_ = 0, DisplayPlaneAlphaFlagBitsKHR alphaMode_ = DisplayPlaneAlphaFlagBitsKHR::eOpaque, Extent2D imageExtent_ = Extent2D() )
      : sType( StructureType::eDisplaySurfaceCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , displayMode( displayMode_ )
      , planeIndex( planeIndex_ )
      , planeStackIndex( planeStackIndex_ )
      , transform( transform_ )
      , globalAlpha( globalAlpha_ )
      , alphaMode( alphaMode_ )
      , imageExtent( imageExtent_ )
    {
    }

    DisplaySurfaceCreateInfoKHR( VkDisplaySurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplaySurfaceCreateInfoKHR) );
    }

    DisplaySurfaceCreateInfoKHR& operator=( VkDisplaySurfaceCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplaySurfaceCreateInfoKHR) );
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setFlags( DisplaySurfaceCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setDisplayMode( DisplayModeKHR displayMode_ )
    {
      displayMode = displayMode_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setPlaneIndex( uint32_t planeIndex_ )
    {
      planeIndex = planeIndex_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setPlaneStackIndex( uint32_t planeStackIndex_ )
    {
      planeStackIndex = planeStackIndex_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setTransform( SurfaceTransformFlagBitsKHR transform_ )
    {
      transform = transform_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setGlobalAlpha( float globalAlpha_ )
    {
      globalAlpha = globalAlpha_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setAlphaMode( DisplayPlaneAlphaFlagBitsKHR alphaMode_ )
    {
      alphaMode = alphaMode_;
      return *this;
    }

    DisplaySurfaceCreateInfoKHR& setImageExtent( Extent2D imageExtent_ )
    {
      imageExtent = imageExtent_;
      return *this;
    }

    operator const VkDisplaySurfaceCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>(this);
    }

    bool operator==( DisplaySurfaceCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( displayMode == rhs.displayMode )
          && ( planeIndex == rhs.planeIndex )
          && ( planeStackIndex == rhs.planeStackIndex )
          && ( transform == rhs.transform )
          && ( globalAlpha == rhs.globalAlpha )
          && ( alphaMode == rhs.alphaMode )
          && ( imageExtent == rhs.imageExtent );
    }

    bool operator!=( DisplaySurfaceCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DisplaySurfaceCreateFlagsKHR flags;
    DisplayModeKHR displayMode;
    uint32_t planeIndex;
    uint32_t planeStackIndex;
    SurfaceTransformFlagBitsKHR transform;
    float globalAlpha;
    DisplayPlaneAlphaFlagBitsKHR alphaMode;
    Extent2D imageExtent;
  };
  static_assert( sizeof( DisplaySurfaceCreateInfoKHR ) == sizeof( VkDisplaySurfaceCreateInfoKHR ), "struct and wrapper have different size!" );

  struct SurfaceCapabilitiesKHR
  {
    operator const VkSurfaceCapabilitiesKHR&() const
    {
      return *reinterpret_cast<const VkSurfaceCapabilitiesKHR*>(this);
    }

    bool operator==( SurfaceCapabilitiesKHR const& rhs ) const
    {
      return ( minImageCount == rhs.minImageCount )
          && ( maxImageCount == rhs.maxImageCount )
          && ( currentExtent == rhs.currentExtent )
          && ( minImageExtent == rhs.minImageExtent )
          && ( maxImageExtent == rhs.maxImageExtent )
          && ( maxImageArrayLayers == rhs.maxImageArrayLayers )
          && ( supportedTransforms == rhs.supportedTransforms )
          && ( currentTransform == rhs.currentTransform )
          && ( supportedCompositeAlpha == rhs.supportedCompositeAlpha )
          && ( supportedUsageFlags == rhs.supportedUsageFlags );
    }

    bool operator!=( SurfaceCapabilitiesKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

    uint32_t minImageCount;
    uint32_t maxImageCount;
    Extent2D currentExtent;
    Extent2D minImageExtent;
    Extent2D maxImageExtent;
    uint32_t maxImageArrayLayers;
    SurfaceTransformFlagsKHR supportedTransforms;
    SurfaceTransformFlagBitsKHR currentTransform;
    CompositeAlphaFlagsKHR supportedCompositeAlpha;
    ImageUsageFlags supportedUsageFlags;
  };
  static_assert( sizeof( SurfaceCapabilitiesKHR ) == sizeof( VkSurfaceCapabilitiesKHR ), "struct and wrapper have different size!" );

  struct SwapchainCreateInfoKHR
  {
    SwapchainCreateInfoKHR( SwapchainCreateFlagsKHR flags_ = SwapchainCreateFlagsKHR(), SurfaceKHR surface_ = SurfaceKHR(), uint32_t minImageCount_ = 0, Format imageFormat_ = Format::eUndefined, ColorSpaceKHR imageColorSpace_ = ColorSpaceKHR::eSrgbNonlinear, Extent2D imageExtent_ = Extent2D(), uint32_t imageArrayLayers_ = 0, ImageUsageFlags imageUsage_ = ImageUsageFlags(), SharingMode imageSharingMode_ = SharingMode::eExclusive, uint32_t queueFamilyIndexCount_ = 0, const uint32_t* pQueueFamilyIndices_ = nullptr, SurfaceTransformFlagBitsKHR preTransform_ = SurfaceTransformFlagBitsKHR::eIdentity, CompositeAlphaFlagBitsKHR compositeAlpha_ = CompositeAlphaFlagBitsKHR::eOpaque, PresentModeKHR presentMode_ = PresentModeKHR::eImmediate, Bool32 clipped_ = 0, SwapchainKHR oldSwapchain_ = SwapchainKHR() )
      : sType( StructureType::eSwapchainCreateInfoKHR )
      , pNext( nullptr )
      , flags( flags_ )
      , surface( surface_ )
      , minImageCount( minImageCount_ )
      , imageFormat( imageFormat_ )
      , imageColorSpace( imageColorSpace_ )
      , imageExtent( imageExtent_ )
      , imageArrayLayers( imageArrayLayers_ )
      , imageUsage( imageUsage_ )
      , imageSharingMode( imageSharingMode_ )
      , queueFamilyIndexCount( queueFamilyIndexCount_ )
      , pQueueFamilyIndices( pQueueFamilyIndices_ )
      , preTransform( preTransform_ )
      , compositeAlpha( compositeAlpha_ )
      , presentMode( presentMode_ )
      , clipped( clipped_ )
      , oldSwapchain( oldSwapchain_ )
    {
    }

    SwapchainCreateInfoKHR( VkSwapchainCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(SwapchainCreateInfoKHR) );
    }

    SwapchainCreateInfoKHR& operator=( VkSwapchainCreateInfoKHR const & rhs )
    {
      memcpy( this, &rhs, sizeof(SwapchainCreateInfoKHR) );
      return *this;
    }

    SwapchainCreateInfoKHR& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    SwapchainCreateInfoKHR& setFlags( SwapchainCreateFlagsKHR flags_ )
    {
      flags = flags_;
      return *this;
    }

    SwapchainCreateInfoKHR& setSurface( SurfaceKHR surface_ )
    {
      surface = surface_;
      return *this;
    }

    SwapchainCreateInfoKHR& setMinImageCount( uint32_t minImageCount_ )
    {
      minImageCount = minImageCount_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageFormat( Format imageFormat_ )
    {
      imageFormat = imageFormat_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageColorSpace( ColorSpaceKHR imageColorSpace_ )
    {
      imageColorSpace = imageColorSpace_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageExtent( Extent2D imageExtent_ )
    {
      imageExtent = imageExtent_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageArrayLayers( uint32_t imageArrayLayers_ )
    {
      imageArrayLayers = imageArrayLayers_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageUsage( ImageUsageFlags imageUsage_ )
    {
      imageUsage = imageUsage_;
      return *this;
    }

    SwapchainCreateInfoKHR& setImageSharingMode( SharingMode imageSharingMode_ )
    {
      imageSharingMode = imageSharingMode_;
      return *this;
    }

    SwapchainCreateInfoKHR& setQueueFamilyIndexCount( uint32_t queueFamilyIndexCount_ )
    {
      queueFamilyIndexCount = queueFamilyIndexCount_;
      return *this;
    }

    SwapchainCreateInfoKHR& setPQueueFamilyIndices( const uint32_t* pQueueFamilyIndices_ )
    {
      pQueueFamilyIndices = pQueueFamilyIndices_;
      return *this;
    }

    SwapchainCreateInfoKHR& setPreTransform( SurfaceTransformFlagBitsKHR preTransform_ )
    {
      preTransform = preTransform_;
      return *this;
    }

    SwapchainCreateInfoKHR& setCompositeAlpha( CompositeAlphaFlagBitsKHR compositeAlpha_ )
    {
      compositeAlpha = compositeAlpha_;
      return *this;
    }

    SwapchainCreateInfoKHR& setPresentMode( PresentModeKHR presentMode_ )
    {
      presentMode = presentMode_;
      return *this;
    }

    SwapchainCreateInfoKHR& setClipped( Bool32 clipped_ )
    {
      clipped = clipped_;
      return *this;
    }

    SwapchainCreateInfoKHR& setOldSwapchain( SwapchainKHR oldSwapchain_ )
    {
      oldSwapchain = oldSwapchain_;
      return *this;
    }

    operator const VkSwapchainCreateInfoKHR&() const
    {
      return *reinterpret_cast<const VkSwapchainCreateInfoKHR*>(this);
    }

    bool operator==( SwapchainCreateInfoKHR const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( surface == rhs.surface )
          && ( minImageCount == rhs.minImageCount )
          && ( imageFormat == rhs.imageFormat )
          && ( imageColorSpace == rhs.imageColorSpace )
          && ( imageExtent == rhs.imageExtent )
          && ( imageArrayLayers == rhs.imageArrayLayers )
          && ( imageUsage == rhs.imageUsage )
          && ( imageSharingMode == rhs.imageSharingMode )
          && ( queueFamilyIndexCount == rhs.queueFamilyIndexCount )
          && ( pQueueFamilyIndices == rhs.pQueueFamilyIndices )
          && ( preTransform == rhs.preTransform )
          && ( compositeAlpha == rhs.compositeAlpha )
          && ( presentMode == rhs.presentMode )
          && ( clipped == rhs.clipped )
          && ( oldSwapchain == rhs.oldSwapchain );
    }

    bool operator!=( SwapchainCreateInfoKHR const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    SwapchainCreateFlagsKHR flags;
    SurfaceKHR surface;
    uint32_t minImageCount;
    Format imageFormat;
    ColorSpaceKHR imageColorSpace;
    Extent2D imageExtent;
    uint32_t imageArrayLayers;
    ImageUsageFlags imageUsage;
    SharingMode imageSharingMode;
    uint32_t queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
    SurfaceTransformFlagBitsKHR preTransform;
    CompositeAlphaFlagBitsKHR compositeAlpha;
    PresentModeKHR presentMode;
    Bool32 clipped;
    SwapchainKHR oldSwapchain;
  };
  static_assert( sizeof( SwapchainCreateInfoKHR ) == sizeof( VkSwapchainCreateInfoKHR ), "struct and wrapper have different size!" );

  enum class DebugReportFlagBitsEXT
  {
    eInformation = VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
    eWarning = VK_DEBUG_REPORT_WARNING_BIT_EXT,
    ePerformanceWarning = VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
    eError = VK_DEBUG_REPORT_ERROR_BIT_EXT,
    eDebug = VK_DEBUG_REPORT_DEBUG_BIT_EXT
  };

  using DebugReportFlagsEXT = Flags<DebugReportFlagBitsEXT, VkDebugReportFlagsEXT>;

  VULKAN_HPP_INLINE DebugReportFlagsEXT operator|( DebugReportFlagBitsEXT bit0, DebugReportFlagBitsEXT bit1 )
  {
    return DebugReportFlagsEXT( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE DebugReportFlagsEXT operator~( DebugReportFlagBitsEXT bits )
  {
    return ~( DebugReportFlagsEXT( bits ) );
  }

  template <> struct FlagTraits<DebugReportFlagBitsEXT>
  {
    enum
    {
      allFlags = VkFlags(DebugReportFlagBitsEXT::eInformation) | VkFlags(DebugReportFlagBitsEXT::eWarning) | VkFlags(DebugReportFlagBitsEXT::ePerformanceWarning) | VkFlags(DebugReportFlagBitsEXT::eError) | VkFlags(DebugReportFlagBitsEXT::eDebug)
    };
  };

  struct DebugReportCallbackCreateInfoEXT
  {
    DebugReportCallbackCreateInfoEXT( DebugReportFlagsEXT flags_ = DebugReportFlagsEXT(), PFN_vkDebugReportCallbackEXT pfnCallback_ = nullptr, void* pUserData_ = nullptr )
      : sType( StructureType::eDebugReportCallbackCreateInfoEXT )
      , pNext( nullptr )
      , flags( flags_ )
      , pfnCallback( pfnCallback_ )
      , pUserData( pUserData_ )
    {
    }

    DebugReportCallbackCreateInfoEXT( VkDebugReportCallbackCreateInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugReportCallbackCreateInfoEXT) );
    }

    DebugReportCallbackCreateInfoEXT& operator=( VkDebugReportCallbackCreateInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugReportCallbackCreateInfoEXT) );
      return *this;
    }

    DebugReportCallbackCreateInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DebugReportCallbackCreateInfoEXT& setFlags( DebugReportFlagsEXT flags_ )
    {
      flags = flags_;
      return *this;
    }

    DebugReportCallbackCreateInfoEXT& setPfnCallback( PFN_vkDebugReportCallbackEXT pfnCallback_ )
    {
      pfnCallback = pfnCallback_;
      return *this;
    }

    DebugReportCallbackCreateInfoEXT& setPUserData( void* pUserData_ )
    {
      pUserData = pUserData_;
      return *this;
    }

    operator const VkDebugReportCallbackCreateInfoEXT&() const
    {
      return *reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>(this);
    }

    bool operator==( DebugReportCallbackCreateInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( flags == rhs.flags )
          && ( pfnCallback == rhs.pfnCallback )
          && ( pUserData == rhs.pUserData );
    }

    bool operator!=( DebugReportCallbackCreateInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DebugReportFlagsEXT flags;
    PFN_vkDebugReportCallbackEXT pfnCallback;
    void* pUserData;
  };
  static_assert( sizeof( DebugReportCallbackCreateInfoEXT ) == sizeof( VkDebugReportCallbackCreateInfoEXT ), "struct and wrapper have different size!" );

  enum class DebugReportObjectTypeEXT
  {
    eUnknown = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT,
    eInstance = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT,
    ePhysicalDevice = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT,
    eDevice = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT,
    eQueue = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
    eSemaphore = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT,
    eCommandBuffer = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
    eFence = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT,
    eDeviceMemory = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
    eBuffer = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
    eImage = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
    eEvent = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT,
    eQueryPool = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT,
    eBufferView = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
    eImageView = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
    eShaderModule = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
    ePipelineCache = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
    ePipelineLayout = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
    eRenderPass = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
    ePipeline = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
    eDescriptorSetLayout = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
    eSampler = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
    eDescriptorPool = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
    eDescriptorSet = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
    eFramebuffer = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
    eCommandPool = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
    eSurfaceKhr = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT,
    eSwapchainKhr = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT,
    eDebugReport = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT,
    eDisplayKhr = VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT,
    eDisplayModeKhr = VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT,
    eObjectTableNvx = VK_DEBUG_REPORT_OBJECT_TYPE_OBJECT_TABLE_NVX_EXT,
    eIndirectCommandsLayoutNvx = VK_DEBUG_REPORT_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX_EXT
  };

  struct DebugMarkerObjectNameInfoEXT
  {
    DebugMarkerObjectNameInfoEXT( DebugReportObjectTypeEXT objectType_ = DebugReportObjectTypeEXT::eUnknown, uint64_t object_ = 0, const char* pObjectName_ = nullptr )
      : sType( StructureType::eDebugMarkerObjectNameInfoEXT )
      , pNext( nullptr )
      , objectType( objectType_ )
      , object( object_ )
      , pObjectName( pObjectName_ )
    {
    }

    DebugMarkerObjectNameInfoEXT( VkDebugMarkerObjectNameInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerObjectNameInfoEXT) );
    }

    DebugMarkerObjectNameInfoEXT& operator=( VkDebugMarkerObjectNameInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerObjectNameInfoEXT) );
      return *this;
    }

    DebugMarkerObjectNameInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DebugMarkerObjectNameInfoEXT& setObjectType( DebugReportObjectTypeEXT objectType_ )
    {
      objectType = objectType_;
      return *this;
    }

    DebugMarkerObjectNameInfoEXT& setObject( uint64_t object_ )
    {
      object = object_;
      return *this;
    }

    DebugMarkerObjectNameInfoEXT& setPObjectName( const char* pObjectName_ )
    {
      pObjectName = pObjectName_;
      return *this;
    }

    operator const VkDebugMarkerObjectNameInfoEXT&() const
    {
      return *reinterpret_cast<const VkDebugMarkerObjectNameInfoEXT*>(this);
    }

    bool operator==( DebugMarkerObjectNameInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( objectType == rhs.objectType )
          && ( object == rhs.object )
          && ( pObjectName == rhs.pObjectName );
    }

    bool operator!=( DebugMarkerObjectNameInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DebugReportObjectTypeEXT objectType;
    uint64_t object;
    const char* pObjectName;
  };
  static_assert( sizeof( DebugMarkerObjectNameInfoEXT ) == sizeof( VkDebugMarkerObjectNameInfoEXT ), "struct and wrapper have different size!" );

  struct DebugMarkerObjectTagInfoEXT
  {
    DebugMarkerObjectTagInfoEXT( DebugReportObjectTypeEXT objectType_ = DebugReportObjectTypeEXT::eUnknown, uint64_t object_ = 0, uint64_t tagName_ = 0, size_t tagSize_ = 0, const void* pTag_ = nullptr )
      : sType( StructureType::eDebugMarkerObjectTagInfoEXT )
      , pNext( nullptr )
      , objectType( objectType_ )
      , object( object_ )
      , tagName( tagName_ )
      , tagSize( tagSize_ )
      , pTag( pTag_ )
    {
    }

    DebugMarkerObjectTagInfoEXT( VkDebugMarkerObjectTagInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerObjectTagInfoEXT) );
    }

    DebugMarkerObjectTagInfoEXT& operator=( VkDebugMarkerObjectTagInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DebugMarkerObjectTagInfoEXT) );
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setObjectType( DebugReportObjectTypeEXT objectType_ )
    {
      objectType = objectType_;
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setObject( uint64_t object_ )
    {
      object = object_;
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setTagName( uint64_t tagName_ )
    {
      tagName = tagName_;
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setTagSize( size_t tagSize_ )
    {
      tagSize = tagSize_;
      return *this;
    }

    DebugMarkerObjectTagInfoEXT& setPTag( const void* pTag_ )
    {
      pTag = pTag_;
      return *this;
    }

    operator const VkDebugMarkerObjectTagInfoEXT&() const
    {
      return *reinterpret_cast<const VkDebugMarkerObjectTagInfoEXT*>(this);
    }

    bool operator==( DebugMarkerObjectTagInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( objectType == rhs.objectType )
          && ( object == rhs.object )
          && ( tagName == rhs.tagName )
          && ( tagSize == rhs.tagSize )
          && ( pTag == rhs.pTag );
    }

    bool operator!=( DebugMarkerObjectTagInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DebugReportObjectTypeEXT objectType;
    uint64_t object;
    uint64_t tagName;
    size_t tagSize;
    const void* pTag;
  };
  static_assert( sizeof( DebugMarkerObjectTagInfoEXT ) == sizeof( VkDebugMarkerObjectTagInfoEXT ), "struct and wrapper have different size!" );

  enum class DebugReportErrorEXT
  {
    eNone = VK_DEBUG_REPORT_ERROR_NONE_EXT,
    eCallbackRef = VK_DEBUG_REPORT_ERROR_CALLBACK_REF_EXT
  };

  enum class RasterizationOrderAMD
  {
    eStrict = VK_RASTERIZATION_ORDER_STRICT_AMD,
    eRelaxed = VK_RASTERIZATION_ORDER_RELAXED_AMD
  };

  struct PipelineRasterizationStateRasterizationOrderAMD
  {
    PipelineRasterizationStateRasterizationOrderAMD( RasterizationOrderAMD rasterizationOrder_ = RasterizationOrderAMD::eStrict )
      : sType( StructureType::ePipelineRasterizationStateRasterizationOrderAMD )
      , pNext( nullptr )
      , rasterizationOrder( rasterizationOrder_ )
    {
    }

    PipelineRasterizationStateRasterizationOrderAMD( VkPipelineRasterizationStateRasterizationOrderAMD const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineRasterizationStateRasterizationOrderAMD) );
    }

    PipelineRasterizationStateRasterizationOrderAMD& operator=( VkPipelineRasterizationStateRasterizationOrderAMD const & rhs )
    {
      memcpy( this, &rhs, sizeof(PipelineRasterizationStateRasterizationOrderAMD) );
      return *this;
    }

    PipelineRasterizationStateRasterizationOrderAMD& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    PipelineRasterizationStateRasterizationOrderAMD& setRasterizationOrder( RasterizationOrderAMD rasterizationOrder_ )
    {
      rasterizationOrder = rasterizationOrder_;
      return *this;
    }

    operator const VkPipelineRasterizationStateRasterizationOrderAMD&() const
    {
      return *reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD*>(this);
    }

    bool operator==( PipelineRasterizationStateRasterizationOrderAMD const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( rasterizationOrder == rhs.rasterizationOrder );
    }

    bool operator!=( PipelineRasterizationStateRasterizationOrderAMD const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    RasterizationOrderAMD rasterizationOrder;
  };
  static_assert( sizeof( PipelineRasterizationStateRasterizationOrderAMD ) == sizeof( VkPipelineRasterizationStateRasterizationOrderAMD ), "struct and wrapper have different size!" );

  enum class ExternalMemoryHandleTypeFlagBitsNV
  {
    eOpaqueWin32 = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT_NV,
    eOpaqueWin32Kmt = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT_NV,
    eD3D11Image = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_IMAGE_BIT_NV,
    eD3D11ImageKmt = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_IMAGE_KMT_BIT_NV
  };

  using ExternalMemoryHandleTypeFlagsNV = Flags<ExternalMemoryHandleTypeFlagBitsNV, VkExternalMemoryHandleTypeFlagsNV>;

  VULKAN_HPP_INLINE ExternalMemoryHandleTypeFlagsNV operator|( ExternalMemoryHandleTypeFlagBitsNV bit0, ExternalMemoryHandleTypeFlagBitsNV bit1 )
  {
    return ExternalMemoryHandleTypeFlagsNV( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ExternalMemoryHandleTypeFlagsNV operator~( ExternalMemoryHandleTypeFlagBitsNV bits )
  {
    return ~( ExternalMemoryHandleTypeFlagsNV( bits ) );
  }

  template <> struct FlagTraits<ExternalMemoryHandleTypeFlagBitsNV>
  {
    enum
    {
      allFlags = VkFlags(ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32) | VkFlags(ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32Kmt) | VkFlags(ExternalMemoryHandleTypeFlagBitsNV::eD3D11Image) | VkFlags(ExternalMemoryHandleTypeFlagBitsNV::eD3D11ImageKmt)
    };
  };

  struct ExternalMemoryImageCreateInfoNV
  {
    ExternalMemoryImageCreateInfoNV( ExternalMemoryHandleTypeFlagsNV handleTypes_ = ExternalMemoryHandleTypeFlagsNV() )
      : sType( StructureType::eExternalMemoryImageCreateInfoNV )
      , pNext( nullptr )
      , handleTypes( handleTypes_ )
    {
    }

    ExternalMemoryImageCreateInfoNV( VkExternalMemoryImageCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExternalMemoryImageCreateInfoNV) );
    }

    ExternalMemoryImageCreateInfoNV& operator=( VkExternalMemoryImageCreateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExternalMemoryImageCreateInfoNV) );
      return *this;
    }

    ExternalMemoryImageCreateInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ExternalMemoryImageCreateInfoNV& setHandleTypes( ExternalMemoryHandleTypeFlagsNV handleTypes_ )
    {
      handleTypes = handleTypes_;
      return *this;
    }

    operator const VkExternalMemoryImageCreateInfoNV&() const
    {
      return *reinterpret_cast<const VkExternalMemoryImageCreateInfoNV*>(this);
    }

    bool operator==( ExternalMemoryImageCreateInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( handleTypes == rhs.handleTypes );
    }

    bool operator!=( ExternalMemoryImageCreateInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ExternalMemoryHandleTypeFlagsNV handleTypes;
  };
  static_assert( sizeof( ExternalMemoryImageCreateInfoNV ) == sizeof( VkExternalMemoryImageCreateInfoNV ), "struct and wrapper have different size!" );

  struct ExportMemoryAllocateInfoNV
  {
    ExportMemoryAllocateInfoNV( ExternalMemoryHandleTypeFlagsNV handleTypes_ = ExternalMemoryHandleTypeFlagsNV() )
      : sType( StructureType::eExportMemoryAllocateInfoNV )
      , pNext( nullptr )
      , handleTypes( handleTypes_ )
    {
    }

    ExportMemoryAllocateInfoNV( VkExportMemoryAllocateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExportMemoryAllocateInfoNV) );
    }

    ExportMemoryAllocateInfoNV& operator=( VkExportMemoryAllocateInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ExportMemoryAllocateInfoNV) );
      return *this;
    }

    ExportMemoryAllocateInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ExportMemoryAllocateInfoNV& setHandleTypes( ExternalMemoryHandleTypeFlagsNV handleTypes_ )
    {
      handleTypes = handleTypes_;
      return *this;
    }

    operator const VkExportMemoryAllocateInfoNV&() const
    {
      return *reinterpret_cast<const VkExportMemoryAllocateInfoNV*>(this);
    }

    bool operator==( ExportMemoryAllocateInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( handleTypes == rhs.handleTypes );
    }

    bool operator!=( ExportMemoryAllocateInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ExternalMemoryHandleTypeFlagsNV handleTypes;
  };
  static_assert( sizeof( ExportMemoryAllocateInfoNV ) == sizeof( VkExportMemoryAllocateInfoNV ), "struct and wrapper have different size!" );

#ifdef VK_USE_PLATFORM_WIN32_KHR
  struct ImportMemoryWin32HandleInfoNV
  {
    ImportMemoryWin32HandleInfoNV( ExternalMemoryHandleTypeFlagsNV handleType_ = ExternalMemoryHandleTypeFlagsNV(), HANDLE handle_ = 0 )
      : sType( StructureType::eImportMemoryWin32HandleInfoNV )
      , pNext( nullptr )
      , handleType( handleType_ )
      , handle( handle_ )
    {
    }

    ImportMemoryWin32HandleInfoNV( VkImportMemoryWin32HandleInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImportMemoryWin32HandleInfoNV) );
    }

    ImportMemoryWin32HandleInfoNV& operator=( VkImportMemoryWin32HandleInfoNV const & rhs )
    {
      memcpy( this, &rhs, sizeof(ImportMemoryWin32HandleInfoNV) );
      return *this;
    }

    ImportMemoryWin32HandleInfoNV& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ImportMemoryWin32HandleInfoNV& setHandleType( ExternalMemoryHandleTypeFlagsNV handleType_ )
    {
      handleType = handleType_;
      return *this;
    }

    ImportMemoryWin32HandleInfoNV& setHandle( HANDLE handle_ )
    {
      handle = handle_;
      return *this;
    }

    operator const VkImportMemoryWin32HandleInfoNV&() const
    {
      return *reinterpret_cast<const VkImportMemoryWin32HandleInfoNV*>(this);
    }

    bool operator==( ImportMemoryWin32HandleInfoNV const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( handleType == rhs.handleType )
          && ( handle == rhs.handle );
    }

    bool operator!=( ImportMemoryWin32HandleInfoNV const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ExternalMemoryHandleTypeFlagsNV handleType;
    HANDLE handle;
  };
  static_assert( sizeof( ImportMemoryWin32HandleInfoNV ) == sizeof( VkImportMemoryWin32HandleInfoNV ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

  enum class ExternalMemoryFeatureFlagBitsNV
  {
    eDedicatedOnly = VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_NV,
    eExportable = VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT_NV,
    eImportable = VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT_NV
  };

  using ExternalMemoryFeatureFlagsNV = Flags<ExternalMemoryFeatureFlagBitsNV, VkExternalMemoryFeatureFlagsNV>;

  VULKAN_HPP_INLINE ExternalMemoryFeatureFlagsNV operator|( ExternalMemoryFeatureFlagBitsNV bit0, ExternalMemoryFeatureFlagBitsNV bit1 )
  {
    return ExternalMemoryFeatureFlagsNV( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ExternalMemoryFeatureFlagsNV operator~( ExternalMemoryFeatureFlagBitsNV bits )
  {
    return ~( ExternalMemoryFeatureFlagsNV( bits ) );
  }

  template <> struct FlagTraits<ExternalMemoryFeatureFlagBitsNV>
  {
    enum
    {
      allFlags = VkFlags(ExternalMemoryFeatureFlagBitsNV::eDedicatedOnly) | VkFlags(ExternalMemoryFeatureFlagBitsNV::eExportable) | VkFlags(ExternalMemoryFeatureFlagBitsNV::eImportable)
    };
  };

  struct ExternalImageFormatPropertiesNV
  {
    operator const VkExternalImageFormatPropertiesNV&() const
    {
      return *reinterpret_cast<const VkExternalImageFormatPropertiesNV*>(this);
    }

    bool operator==( ExternalImageFormatPropertiesNV const& rhs ) const
    {
      return ( imageFormatProperties == rhs.imageFormatProperties )
          && ( externalMemoryFeatures == rhs.externalMemoryFeatures )
          && ( exportFromImportedHandleTypes == rhs.exportFromImportedHandleTypes )
          && ( compatibleHandleTypes == rhs.compatibleHandleTypes );
    }

    bool operator!=( ExternalImageFormatPropertiesNV const& rhs ) const
    {
      return !operator==( rhs );
    }

    ImageFormatProperties imageFormatProperties;
    ExternalMemoryFeatureFlagsNV externalMemoryFeatures;
    ExternalMemoryHandleTypeFlagsNV exportFromImportedHandleTypes;
    ExternalMemoryHandleTypeFlagsNV compatibleHandleTypes;
  };
  static_assert( sizeof( ExternalImageFormatPropertiesNV ) == sizeof( VkExternalImageFormatPropertiesNV ), "struct and wrapper have different size!" );

  enum class ValidationCheckEXT
  {
    eAll = VK_VALIDATION_CHECK_ALL_EXT
  };

  struct ValidationFlagsEXT
  {
    ValidationFlagsEXT( uint32_t disabledValidationCheckCount_ = 0, ValidationCheckEXT* pDisabledValidationChecks_ = nullptr )
      : sType( StructureType::eValidationFlagsEXT )
      , pNext( nullptr )
      , disabledValidationCheckCount( disabledValidationCheckCount_ )
      , pDisabledValidationChecks( pDisabledValidationChecks_ )
    {
    }

    ValidationFlagsEXT( VkValidationFlagsEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(ValidationFlagsEXT) );
    }

    ValidationFlagsEXT& operator=( VkValidationFlagsEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(ValidationFlagsEXT) );
      return *this;
    }

    ValidationFlagsEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ValidationFlagsEXT& setDisabledValidationCheckCount( uint32_t disabledValidationCheckCount_ )
    {
      disabledValidationCheckCount = disabledValidationCheckCount_;
      return *this;
    }

    ValidationFlagsEXT& setPDisabledValidationChecks( ValidationCheckEXT* pDisabledValidationChecks_ )
    {
      pDisabledValidationChecks = pDisabledValidationChecks_;
      return *this;
    }

    operator const VkValidationFlagsEXT&() const
    {
      return *reinterpret_cast<const VkValidationFlagsEXT*>(this);
    }

    bool operator==( ValidationFlagsEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( disabledValidationCheckCount == rhs.disabledValidationCheckCount )
          && ( pDisabledValidationChecks == rhs.pDisabledValidationChecks );
    }

    bool operator!=( ValidationFlagsEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t disabledValidationCheckCount;
    ValidationCheckEXT* pDisabledValidationChecks;
  };
  static_assert( sizeof( ValidationFlagsEXT ) == sizeof( VkValidationFlagsEXT ), "struct and wrapper have different size!" );

  enum class IndirectCommandsLayoutUsageFlagBitsNVX
  {
    eUnorderedSequences = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_UNORDERED_SEQUENCES_BIT_NVX,
    eSparseSequences = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_SPARSE_SEQUENCES_BIT_NVX,
    eEmptyExecutions = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_EMPTY_EXECUTIONS_BIT_NVX,
    eIndexedSequences = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_INDEXED_SEQUENCES_BIT_NVX
  };

  using IndirectCommandsLayoutUsageFlagsNVX = Flags<IndirectCommandsLayoutUsageFlagBitsNVX, VkIndirectCommandsLayoutUsageFlagsNVX>;

  VULKAN_HPP_INLINE IndirectCommandsLayoutUsageFlagsNVX operator|( IndirectCommandsLayoutUsageFlagBitsNVX bit0, IndirectCommandsLayoutUsageFlagBitsNVX bit1 )
  {
    return IndirectCommandsLayoutUsageFlagsNVX( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE IndirectCommandsLayoutUsageFlagsNVX operator~( IndirectCommandsLayoutUsageFlagBitsNVX bits )
  {
    return ~( IndirectCommandsLayoutUsageFlagsNVX( bits ) );
  }

  template <> struct FlagTraits<IndirectCommandsLayoutUsageFlagBitsNVX>
  {
    enum
    {
      allFlags = VkFlags(IndirectCommandsLayoutUsageFlagBitsNVX::eUnorderedSequences) | VkFlags(IndirectCommandsLayoutUsageFlagBitsNVX::eSparseSequences) | VkFlags(IndirectCommandsLayoutUsageFlagBitsNVX::eEmptyExecutions) | VkFlags(IndirectCommandsLayoutUsageFlagBitsNVX::eIndexedSequences)
    };
  };

  enum class ObjectEntryUsageFlagBitsNVX
  {
    eGraphics = VK_OBJECT_ENTRY_USAGE_GRAPHICS_BIT_NVX,
    eCompute = VK_OBJECT_ENTRY_USAGE_COMPUTE_BIT_NVX
  };

  using ObjectEntryUsageFlagsNVX = Flags<ObjectEntryUsageFlagBitsNVX, VkObjectEntryUsageFlagsNVX>;

  VULKAN_HPP_INLINE ObjectEntryUsageFlagsNVX operator|( ObjectEntryUsageFlagBitsNVX bit0, ObjectEntryUsageFlagBitsNVX bit1 )
  {
    return ObjectEntryUsageFlagsNVX( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ObjectEntryUsageFlagsNVX operator~( ObjectEntryUsageFlagBitsNVX bits )
  {
    return ~( ObjectEntryUsageFlagsNVX( bits ) );
  }

  template <> struct FlagTraits<ObjectEntryUsageFlagBitsNVX>
  {
    enum
    {
      allFlags = VkFlags(ObjectEntryUsageFlagBitsNVX::eGraphics) | VkFlags(ObjectEntryUsageFlagBitsNVX::eCompute)
    };
  };

  enum class IndirectCommandsTokenTypeNVX
  {
    eVkIndirectCommandsTokenPipeline = VK_INDIRECT_COMMANDS_TOKEN_PIPELINE_NVX,
    eVkIndirectCommandsTokenDescriptorSet = VK_INDIRECT_COMMANDS_TOKEN_DESCRIPTOR_SET_NVX,
    eVkIndirectCommandsTokenIndexBuffer = VK_INDIRECT_COMMANDS_TOKEN_INDEX_BUFFER_NVX,
    eVkIndirectCommandsTokenVertexBuffer = VK_INDIRECT_COMMANDS_TOKEN_VERTEX_BUFFER_NVX,
    eVkIndirectCommandsTokenPushConstant = VK_INDIRECT_COMMANDS_TOKEN_PUSH_CONSTANT_NVX,
    eVkIndirectCommandsTokenDrawIndexed = VK_INDIRECT_COMMANDS_TOKEN_DRAW_INDEXED_NVX,
    eVkIndirectCommandsTokenDraw = VK_INDIRECT_COMMANDS_TOKEN_DRAW_NVX,
    eVkIndirectCommandsTokenDispatch = VK_INDIRECT_COMMANDS_TOKEN_DISPATCH_NVX
  };

  struct IndirectCommandsTokenNVX
  {
    IndirectCommandsTokenNVX( IndirectCommandsTokenTypeNVX tokenType_ = IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenPipeline, Buffer buffer_ = Buffer(), DeviceSize offset_ = 0 )
      : tokenType( tokenType_ )
      , buffer( buffer_ )
      , offset( offset_ )
    {
    }

    IndirectCommandsTokenNVX( VkIndirectCommandsTokenNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsTokenNVX) );
    }

    IndirectCommandsTokenNVX& operator=( VkIndirectCommandsTokenNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsTokenNVX) );
      return *this;
    }

    IndirectCommandsTokenNVX& setTokenType( IndirectCommandsTokenTypeNVX tokenType_ )
    {
      tokenType = tokenType_;
      return *this;
    }

    IndirectCommandsTokenNVX& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    IndirectCommandsTokenNVX& setOffset( DeviceSize offset_ )
    {
      offset = offset_;
      return *this;
    }

    operator const VkIndirectCommandsTokenNVX&() const
    {
      return *reinterpret_cast<const VkIndirectCommandsTokenNVX*>(this);
    }

    bool operator==( IndirectCommandsTokenNVX const& rhs ) const
    {
      return ( tokenType == rhs.tokenType )
          && ( buffer == rhs.buffer )
          && ( offset == rhs.offset );
    }

    bool operator!=( IndirectCommandsTokenNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    IndirectCommandsTokenTypeNVX tokenType;
    Buffer buffer;
    DeviceSize offset;
  };
  static_assert( sizeof( IndirectCommandsTokenNVX ) == sizeof( VkIndirectCommandsTokenNVX ), "struct and wrapper have different size!" );

  struct IndirectCommandsLayoutTokenNVX
  {
    IndirectCommandsLayoutTokenNVX( IndirectCommandsTokenTypeNVX tokenType_ = IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenPipeline, uint32_t bindingUnit_ = 0, uint32_t dynamicCount_ = 0, uint32_t divisor_ = 0 )
      : tokenType( tokenType_ )
      , bindingUnit( bindingUnit_ )
      , dynamicCount( dynamicCount_ )
      , divisor( divisor_ )
    {
    }

    IndirectCommandsLayoutTokenNVX( VkIndirectCommandsLayoutTokenNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsLayoutTokenNVX) );
    }

    IndirectCommandsLayoutTokenNVX& operator=( VkIndirectCommandsLayoutTokenNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsLayoutTokenNVX) );
      return *this;
    }

    IndirectCommandsLayoutTokenNVX& setTokenType( IndirectCommandsTokenTypeNVX tokenType_ )
    {
      tokenType = tokenType_;
      return *this;
    }

    IndirectCommandsLayoutTokenNVX& setBindingUnit( uint32_t bindingUnit_ )
    {
      bindingUnit = bindingUnit_;
      return *this;
    }

    IndirectCommandsLayoutTokenNVX& setDynamicCount( uint32_t dynamicCount_ )
    {
      dynamicCount = dynamicCount_;
      return *this;
    }

    IndirectCommandsLayoutTokenNVX& setDivisor( uint32_t divisor_ )
    {
      divisor = divisor_;
      return *this;
    }

    operator const VkIndirectCommandsLayoutTokenNVX&() const
    {
      return *reinterpret_cast<const VkIndirectCommandsLayoutTokenNVX*>(this);
    }

    bool operator==( IndirectCommandsLayoutTokenNVX const& rhs ) const
    {
      return ( tokenType == rhs.tokenType )
          && ( bindingUnit == rhs.bindingUnit )
          && ( dynamicCount == rhs.dynamicCount )
          && ( divisor == rhs.divisor );
    }

    bool operator!=( IndirectCommandsLayoutTokenNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    IndirectCommandsTokenTypeNVX tokenType;
    uint32_t bindingUnit;
    uint32_t dynamicCount;
    uint32_t divisor;
  };
  static_assert( sizeof( IndirectCommandsLayoutTokenNVX ) == sizeof( VkIndirectCommandsLayoutTokenNVX ), "struct and wrapper have different size!" );

  struct IndirectCommandsLayoutCreateInfoNVX
  {
    IndirectCommandsLayoutCreateInfoNVX( PipelineBindPoint pipelineBindPoint_ = PipelineBindPoint::eGraphics, IndirectCommandsLayoutUsageFlagsNVX flags_ = IndirectCommandsLayoutUsageFlagsNVX(), uint32_t tokenCount_ = 0, const IndirectCommandsLayoutTokenNVX* pTokens_ = nullptr )
      : sType( StructureType::eIndirectCommandsLayoutCreateInfoNVX )
      , pNext( nullptr )
      , pipelineBindPoint( pipelineBindPoint_ )
      , flags( flags_ )
      , tokenCount( tokenCount_ )
      , pTokens( pTokens_ )
    {
    }

    IndirectCommandsLayoutCreateInfoNVX( VkIndirectCommandsLayoutCreateInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsLayoutCreateInfoNVX) );
    }

    IndirectCommandsLayoutCreateInfoNVX& operator=( VkIndirectCommandsLayoutCreateInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(IndirectCommandsLayoutCreateInfoNVX) );
      return *this;
    }

    IndirectCommandsLayoutCreateInfoNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    IndirectCommandsLayoutCreateInfoNVX& setPipelineBindPoint( PipelineBindPoint pipelineBindPoint_ )
    {
      pipelineBindPoint = pipelineBindPoint_;
      return *this;
    }

    IndirectCommandsLayoutCreateInfoNVX& setFlags( IndirectCommandsLayoutUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    IndirectCommandsLayoutCreateInfoNVX& setTokenCount( uint32_t tokenCount_ )
    {
      tokenCount = tokenCount_;
      return *this;
    }

    IndirectCommandsLayoutCreateInfoNVX& setPTokens( const IndirectCommandsLayoutTokenNVX* pTokens_ )
    {
      pTokens = pTokens_;
      return *this;
    }

    operator const VkIndirectCommandsLayoutCreateInfoNVX&() const
    {
      return *reinterpret_cast<const VkIndirectCommandsLayoutCreateInfoNVX*>(this);
    }

    bool operator==( IndirectCommandsLayoutCreateInfoNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( pipelineBindPoint == rhs.pipelineBindPoint )
          && ( flags == rhs.flags )
          && ( tokenCount == rhs.tokenCount )
          && ( pTokens == rhs.pTokens );
    }

    bool operator!=( IndirectCommandsLayoutCreateInfoNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    PipelineBindPoint pipelineBindPoint;
    IndirectCommandsLayoutUsageFlagsNVX flags;
    uint32_t tokenCount;
    const IndirectCommandsLayoutTokenNVX* pTokens;
  };
  static_assert( sizeof( IndirectCommandsLayoutCreateInfoNVX ) == sizeof( VkIndirectCommandsLayoutCreateInfoNVX ), "struct and wrapper have different size!" );

  enum class ObjectEntryTypeNVX
  {
    eVkObjectEntryDescriptorSet = VK_OBJECT_ENTRY_DESCRIPTOR_SET_NVX,
    eVkObjectEntryPipeline = VK_OBJECT_ENTRY_PIPELINE_NVX,
    eVkObjectEntryIndexBuffer = VK_OBJECT_ENTRY_INDEX_BUFFER_NVX,
    eVkObjectEntryVertexBuffer = VK_OBJECT_ENTRY_VERTEX_BUFFER_NVX,
    eVkObjectEntryPushConstant = VK_OBJECT_ENTRY_PUSH_CONSTANT_NVX
  };

  struct ObjectTableCreateInfoNVX
  {
    ObjectTableCreateInfoNVX( uint32_t objectCount_ = 0, const ObjectEntryTypeNVX* pObjectEntryTypes_ = nullptr, const uint32_t* pObjectEntryCounts_ = nullptr, const ObjectEntryUsageFlagsNVX* pObjectEntryUsageFlags_ = nullptr, uint32_t maxUniformBuffersPerDescriptor_ = 0, uint32_t maxStorageBuffersPerDescriptor_ = 0, uint32_t maxStorageImagesPerDescriptor_ = 0, uint32_t maxSampledImagesPerDescriptor_ = 0, uint32_t maxPipelineLayouts_ = 0 )
      : sType( StructureType::eObjectTableCreateInfoNVX )
      , pNext( nullptr )
      , objectCount( objectCount_ )
      , pObjectEntryTypes( pObjectEntryTypes_ )
      , pObjectEntryCounts( pObjectEntryCounts_ )
      , pObjectEntryUsageFlags( pObjectEntryUsageFlags_ )
      , maxUniformBuffersPerDescriptor( maxUniformBuffersPerDescriptor_ )
      , maxStorageBuffersPerDescriptor( maxStorageBuffersPerDescriptor_ )
      , maxStorageImagesPerDescriptor( maxStorageImagesPerDescriptor_ )
      , maxSampledImagesPerDescriptor( maxSampledImagesPerDescriptor_ )
      , maxPipelineLayouts( maxPipelineLayouts_ )
    {
    }

    ObjectTableCreateInfoNVX( VkObjectTableCreateInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableCreateInfoNVX) );
    }

    ObjectTableCreateInfoNVX& operator=( VkObjectTableCreateInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableCreateInfoNVX) );
      return *this;
    }

    ObjectTableCreateInfoNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setObjectCount( uint32_t objectCount_ )
    {
      objectCount = objectCount_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setPObjectEntryTypes( const ObjectEntryTypeNVX* pObjectEntryTypes_ )
    {
      pObjectEntryTypes = pObjectEntryTypes_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setPObjectEntryCounts( const uint32_t* pObjectEntryCounts_ )
    {
      pObjectEntryCounts = pObjectEntryCounts_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setPObjectEntryUsageFlags( const ObjectEntryUsageFlagsNVX* pObjectEntryUsageFlags_ )
    {
      pObjectEntryUsageFlags = pObjectEntryUsageFlags_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setMaxUniformBuffersPerDescriptor( uint32_t maxUniformBuffersPerDescriptor_ )
    {
      maxUniformBuffersPerDescriptor = maxUniformBuffersPerDescriptor_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setMaxStorageBuffersPerDescriptor( uint32_t maxStorageBuffersPerDescriptor_ )
    {
      maxStorageBuffersPerDescriptor = maxStorageBuffersPerDescriptor_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setMaxStorageImagesPerDescriptor( uint32_t maxStorageImagesPerDescriptor_ )
    {
      maxStorageImagesPerDescriptor = maxStorageImagesPerDescriptor_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setMaxSampledImagesPerDescriptor( uint32_t maxSampledImagesPerDescriptor_ )
    {
      maxSampledImagesPerDescriptor = maxSampledImagesPerDescriptor_;
      return *this;
    }

    ObjectTableCreateInfoNVX& setMaxPipelineLayouts( uint32_t maxPipelineLayouts_ )
    {
      maxPipelineLayouts = maxPipelineLayouts_;
      return *this;
    }

    operator const VkObjectTableCreateInfoNVX&() const
    {
      return *reinterpret_cast<const VkObjectTableCreateInfoNVX*>(this);
    }

    bool operator==( ObjectTableCreateInfoNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( objectCount == rhs.objectCount )
          && ( pObjectEntryTypes == rhs.pObjectEntryTypes )
          && ( pObjectEntryCounts == rhs.pObjectEntryCounts )
          && ( pObjectEntryUsageFlags == rhs.pObjectEntryUsageFlags )
          && ( maxUniformBuffersPerDescriptor == rhs.maxUniformBuffersPerDescriptor )
          && ( maxStorageBuffersPerDescriptor == rhs.maxStorageBuffersPerDescriptor )
          && ( maxStorageImagesPerDescriptor == rhs.maxStorageImagesPerDescriptor )
          && ( maxSampledImagesPerDescriptor == rhs.maxSampledImagesPerDescriptor )
          && ( maxPipelineLayouts == rhs.maxPipelineLayouts );
    }

    bool operator!=( ObjectTableCreateInfoNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t objectCount;
    const ObjectEntryTypeNVX* pObjectEntryTypes;
    const uint32_t* pObjectEntryCounts;
    const ObjectEntryUsageFlagsNVX* pObjectEntryUsageFlags;
    uint32_t maxUniformBuffersPerDescriptor;
    uint32_t maxStorageBuffersPerDescriptor;
    uint32_t maxStorageImagesPerDescriptor;
    uint32_t maxSampledImagesPerDescriptor;
    uint32_t maxPipelineLayouts;
  };
  static_assert( sizeof( ObjectTableCreateInfoNVX ) == sizeof( VkObjectTableCreateInfoNVX ), "struct and wrapper have different size!" );

  struct ObjectTableEntryNVX
  {
    ObjectTableEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX() )
      : type( type_ )
      , flags( flags_ )
    {
    }

    ObjectTableEntryNVX( VkObjectTableEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableEntryNVX) );
    }

    ObjectTableEntryNVX& operator=( VkObjectTableEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableEntryNVX) );
      return *this;
    }

    ObjectTableEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTableEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    operator const VkObjectTableEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTableEntryNVX*>(this);
    }

    bool operator==( ObjectTableEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags );
    }

    bool operator!=( ObjectTableEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
  };
  static_assert( sizeof( ObjectTableEntryNVX ) == sizeof( VkObjectTableEntryNVX ), "struct and wrapper have different size!" );

  struct ObjectTablePipelineEntryNVX
  {
    ObjectTablePipelineEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX(), Pipeline pipeline_ = Pipeline() )
      : type( type_ )
      , flags( flags_ )
      , pipeline( pipeline_ )
    {
    }

    ObjectTablePipelineEntryNVX( VkObjectTablePipelineEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTablePipelineEntryNVX) );
    }

    ObjectTablePipelineEntryNVX& operator=( VkObjectTablePipelineEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTablePipelineEntryNVX) );
      return *this;
    }

    ObjectTablePipelineEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTablePipelineEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    ObjectTablePipelineEntryNVX& setPipeline( Pipeline pipeline_ )
    {
      pipeline = pipeline_;
      return *this;
    }

    operator const VkObjectTablePipelineEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTablePipelineEntryNVX*>(this);
    }

    bool operator==( ObjectTablePipelineEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags )
          && ( pipeline == rhs.pipeline );
    }

    bool operator!=( ObjectTablePipelineEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
    Pipeline pipeline;
  };
  static_assert( sizeof( ObjectTablePipelineEntryNVX ) == sizeof( VkObjectTablePipelineEntryNVX ), "struct and wrapper have different size!" );

  struct ObjectTableDescriptorSetEntryNVX
  {
    ObjectTableDescriptorSetEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX(), PipelineLayout pipelineLayout_ = PipelineLayout(), DescriptorSet descriptorSet_ = DescriptorSet() )
      : type( type_ )
      , flags( flags_ )
      , pipelineLayout( pipelineLayout_ )
      , descriptorSet( descriptorSet_ )
    {
    }

    ObjectTableDescriptorSetEntryNVX( VkObjectTableDescriptorSetEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableDescriptorSetEntryNVX) );
    }

    ObjectTableDescriptorSetEntryNVX& operator=( VkObjectTableDescriptorSetEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableDescriptorSetEntryNVX) );
      return *this;
    }

    ObjectTableDescriptorSetEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTableDescriptorSetEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    ObjectTableDescriptorSetEntryNVX& setPipelineLayout( PipelineLayout pipelineLayout_ )
    {
      pipelineLayout = pipelineLayout_;
      return *this;
    }

    ObjectTableDescriptorSetEntryNVX& setDescriptorSet( DescriptorSet descriptorSet_ )
    {
      descriptorSet = descriptorSet_;
      return *this;
    }

    operator const VkObjectTableDescriptorSetEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTableDescriptorSetEntryNVX*>(this);
    }

    bool operator==( ObjectTableDescriptorSetEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags )
          && ( pipelineLayout == rhs.pipelineLayout )
          && ( descriptorSet == rhs.descriptorSet );
    }

    bool operator!=( ObjectTableDescriptorSetEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
    PipelineLayout pipelineLayout;
    DescriptorSet descriptorSet;
  };
  static_assert( sizeof( ObjectTableDescriptorSetEntryNVX ) == sizeof( VkObjectTableDescriptorSetEntryNVX ), "struct and wrapper have different size!" );

  struct ObjectTableVertexBufferEntryNVX
  {
    ObjectTableVertexBufferEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX(), Buffer buffer_ = Buffer() )
      : type( type_ )
      , flags( flags_ )
      , buffer( buffer_ )
    {
    }

    ObjectTableVertexBufferEntryNVX( VkObjectTableVertexBufferEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableVertexBufferEntryNVX) );
    }

    ObjectTableVertexBufferEntryNVX& operator=( VkObjectTableVertexBufferEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableVertexBufferEntryNVX) );
      return *this;
    }

    ObjectTableVertexBufferEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTableVertexBufferEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    ObjectTableVertexBufferEntryNVX& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    operator const VkObjectTableVertexBufferEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTableVertexBufferEntryNVX*>(this);
    }

    bool operator==( ObjectTableVertexBufferEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags )
          && ( buffer == rhs.buffer );
    }

    bool operator!=( ObjectTableVertexBufferEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
    Buffer buffer;
  };
  static_assert( sizeof( ObjectTableVertexBufferEntryNVX ) == sizeof( VkObjectTableVertexBufferEntryNVX ), "struct and wrapper have different size!" );

  struct ObjectTableIndexBufferEntryNVX
  {
    ObjectTableIndexBufferEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX(), Buffer buffer_ = Buffer(), IndexType indexType_ = IndexType::eUint16 )
      : type( type_ )
      , flags( flags_ )
      , buffer( buffer_ )
      , indexType( indexType_ )
    {
    }

    ObjectTableIndexBufferEntryNVX( VkObjectTableIndexBufferEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableIndexBufferEntryNVX) );
    }

    ObjectTableIndexBufferEntryNVX& operator=( VkObjectTableIndexBufferEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTableIndexBufferEntryNVX) );
      return *this;
    }

    ObjectTableIndexBufferEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTableIndexBufferEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    ObjectTableIndexBufferEntryNVX& setBuffer( Buffer buffer_ )
    {
      buffer = buffer_;
      return *this;
    }

    ObjectTableIndexBufferEntryNVX& setIndexType( IndexType indexType_ )
    {
      indexType = indexType_;
      return *this;
    }

    operator const VkObjectTableIndexBufferEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTableIndexBufferEntryNVX*>(this);
    }

    bool operator==( ObjectTableIndexBufferEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags )
          && ( buffer == rhs.buffer )
          && ( indexType == rhs.indexType );
    }

    bool operator!=( ObjectTableIndexBufferEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
    Buffer buffer;
    IndexType indexType;
  };
  static_assert( sizeof( ObjectTableIndexBufferEntryNVX ) == sizeof( VkObjectTableIndexBufferEntryNVX ), "struct and wrapper have different size!" );

  struct ObjectTablePushConstantEntryNVX
  {
    ObjectTablePushConstantEntryNVX( ObjectEntryTypeNVX type_ = ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet, ObjectEntryUsageFlagsNVX flags_ = ObjectEntryUsageFlagsNVX(), PipelineLayout pipelineLayout_ = PipelineLayout(), ShaderStageFlags stageFlags_ = ShaderStageFlags() )
      : type( type_ )
      , flags( flags_ )
      , pipelineLayout( pipelineLayout_ )
      , stageFlags( stageFlags_ )
    {
    }

    ObjectTablePushConstantEntryNVX( VkObjectTablePushConstantEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTablePushConstantEntryNVX) );
    }

    ObjectTablePushConstantEntryNVX& operator=( VkObjectTablePushConstantEntryNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(ObjectTablePushConstantEntryNVX) );
      return *this;
    }

    ObjectTablePushConstantEntryNVX& setType( ObjectEntryTypeNVX type_ )
    {
      type = type_;
      return *this;
    }

    ObjectTablePushConstantEntryNVX& setFlags( ObjectEntryUsageFlagsNVX flags_ )
    {
      flags = flags_;
      return *this;
    }

    ObjectTablePushConstantEntryNVX& setPipelineLayout( PipelineLayout pipelineLayout_ )
    {
      pipelineLayout = pipelineLayout_;
      return *this;
    }

    ObjectTablePushConstantEntryNVX& setStageFlags( ShaderStageFlags stageFlags_ )
    {
      stageFlags = stageFlags_;
      return *this;
    }

    operator const VkObjectTablePushConstantEntryNVX&() const
    {
      return *reinterpret_cast<const VkObjectTablePushConstantEntryNVX*>(this);
    }

    bool operator==( ObjectTablePushConstantEntryNVX const& rhs ) const
    {
      return ( type == rhs.type )
          && ( flags == rhs.flags )
          && ( pipelineLayout == rhs.pipelineLayout )
          && ( stageFlags == rhs.stageFlags );
    }

    bool operator!=( ObjectTablePushConstantEntryNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

    ObjectEntryTypeNVX type;
    ObjectEntryUsageFlagsNVX flags;
    PipelineLayout pipelineLayout;
    ShaderStageFlags stageFlags;
  };
  static_assert( sizeof( ObjectTablePushConstantEntryNVX ) == sizeof( VkObjectTablePushConstantEntryNVX ), "struct and wrapper have different size!" );

  enum class SurfaceCounterFlagBitsEXT
  {
    eVblankExt = VK_SURFACE_COUNTER_VBLANK_EXT
  };

  using SurfaceCounterFlagsEXT = Flags<SurfaceCounterFlagBitsEXT, VkSurfaceCounterFlagsEXT>;

  VULKAN_HPP_INLINE SurfaceCounterFlagsEXT operator|( SurfaceCounterFlagBitsEXT bit0, SurfaceCounterFlagBitsEXT bit1 )
  {
    return SurfaceCounterFlagsEXT( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE SurfaceCounterFlagsEXT operator~( SurfaceCounterFlagBitsEXT bits )
  {
    return ~( SurfaceCounterFlagsEXT( bits ) );
  }

  template <> struct FlagTraits<SurfaceCounterFlagBitsEXT>
  {
    enum
    {
      allFlags = VkFlags(SurfaceCounterFlagBitsEXT::eVblankExt)
    };
  };

  struct SurfaceCapabilities2EXT
  {
    operator const VkSurfaceCapabilities2EXT&() const
    {
      return *reinterpret_cast<const VkSurfaceCapabilities2EXT*>(this);
    }

    bool operator==( SurfaceCapabilities2EXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( minImageCount == rhs.minImageCount )
          && ( maxImageCount == rhs.maxImageCount )
          && ( currentExtent == rhs.currentExtent )
          && ( minImageExtent == rhs.minImageExtent )
          && ( maxImageExtent == rhs.maxImageExtent )
          && ( maxImageArrayLayers == rhs.maxImageArrayLayers )
          && ( supportedTransforms == rhs.supportedTransforms )
          && ( currentTransform == rhs.currentTransform )
          && ( supportedCompositeAlpha == rhs.supportedCompositeAlpha )
          && ( supportedUsageFlags == rhs.supportedUsageFlags )
          && ( supportedSurfaceCounters == rhs.supportedSurfaceCounters );
    }

    bool operator!=( SurfaceCapabilities2EXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    void* pNext;
    uint32_t minImageCount;
    uint32_t maxImageCount;
    Extent2D currentExtent;
    Extent2D minImageExtent;
    Extent2D maxImageExtent;
    uint32_t maxImageArrayLayers;
    SurfaceTransformFlagsKHR supportedTransforms;
    SurfaceTransformFlagBitsKHR currentTransform;
    CompositeAlphaFlagsKHR supportedCompositeAlpha;
    ImageUsageFlags supportedUsageFlags;
    SurfaceCounterFlagsEXT supportedSurfaceCounters;
  };
  static_assert( sizeof( SurfaceCapabilities2EXT ) == sizeof( VkSurfaceCapabilities2EXT ), "struct and wrapper have different size!" );

  struct SwapchainCounterCreateInfoEXT
  {
    SwapchainCounterCreateInfoEXT( SurfaceCounterFlagsEXT surfaceCounters_ = SurfaceCounterFlagsEXT() )
      : sType( StructureType::eSwapchainCounterCreateInfoEXT )
      , pNext( nullptr )
      , surfaceCounters( surfaceCounters_ )
    {
    }

    SwapchainCounterCreateInfoEXT( VkSwapchainCounterCreateInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(SwapchainCounterCreateInfoEXT) );
    }

    SwapchainCounterCreateInfoEXT& operator=( VkSwapchainCounterCreateInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(SwapchainCounterCreateInfoEXT) );
      return *this;
    }

    SwapchainCounterCreateInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    SwapchainCounterCreateInfoEXT& setSurfaceCounters( SurfaceCounterFlagsEXT surfaceCounters_ )
    {
      surfaceCounters = surfaceCounters_;
      return *this;
    }

    operator const VkSwapchainCounterCreateInfoEXT&() const
    {
      return *reinterpret_cast<const VkSwapchainCounterCreateInfoEXT*>(this);
    }

    bool operator==( SwapchainCounterCreateInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( surfaceCounters == rhs.surfaceCounters );
    }

    bool operator!=( SwapchainCounterCreateInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    SurfaceCounterFlagsEXT surfaceCounters;
  };
  static_assert( sizeof( SwapchainCounterCreateInfoEXT ) == sizeof( VkSwapchainCounterCreateInfoEXT ), "struct and wrapper have different size!" );

  enum class DisplayPowerStateEXT
  {
    eOff = VK_DISPLAY_POWER_STATE_OFF_EXT,
    eSuspend = VK_DISPLAY_POWER_STATE_SUSPEND_EXT,
    eOn = VK_DISPLAY_POWER_STATE_ON_EXT
  };

  struct DisplayPowerInfoEXT
  {
    DisplayPowerInfoEXT( DisplayPowerStateEXT powerState_ = DisplayPowerStateEXT::eOff )
      : sType( StructureType::eDisplayPowerInfoEXT )
      , pNext( nullptr )
      , powerState( powerState_ )
    {
    }

    DisplayPowerInfoEXT( VkDisplayPowerInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayPowerInfoEXT) );
    }

    DisplayPowerInfoEXT& operator=( VkDisplayPowerInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayPowerInfoEXT) );
      return *this;
    }

    DisplayPowerInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DisplayPowerInfoEXT& setPowerState( DisplayPowerStateEXT powerState_ )
    {
      powerState = powerState_;
      return *this;
    }

    operator const VkDisplayPowerInfoEXT&() const
    {
      return *reinterpret_cast<const VkDisplayPowerInfoEXT*>(this);
    }

    bool operator==( DisplayPowerInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( powerState == rhs.powerState );
    }

    bool operator!=( DisplayPowerInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DisplayPowerStateEXT powerState;
  };
  static_assert( sizeof( DisplayPowerInfoEXT ) == sizeof( VkDisplayPowerInfoEXT ), "struct and wrapper have different size!" );

  enum class DeviceEventTypeEXT
  {
    eDisplayHotplug = VK_DEVICE_EVENT_TYPE_DISPLAY_HOTPLUG_EXT
  };

  struct DeviceEventInfoEXT
  {
    DeviceEventInfoEXT( DeviceEventTypeEXT deviceEvent_ = DeviceEventTypeEXT::eDisplayHotplug )
      : sType( StructureType::eDeviceEventInfoEXT )
      , pNext( nullptr )
      , deviceEvent( deviceEvent_ )
    {
    }

    DeviceEventInfoEXT( VkDeviceEventInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceEventInfoEXT) );
    }

    DeviceEventInfoEXT& operator=( VkDeviceEventInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DeviceEventInfoEXT) );
      return *this;
    }

    DeviceEventInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DeviceEventInfoEXT& setDeviceEvent( DeviceEventTypeEXT deviceEvent_ )
    {
      deviceEvent = deviceEvent_;
      return *this;
    }

    operator const VkDeviceEventInfoEXT&() const
    {
      return *reinterpret_cast<const VkDeviceEventInfoEXT*>(this);
    }

    bool operator==( DeviceEventInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( deviceEvent == rhs.deviceEvent );
    }

    bool operator!=( DeviceEventInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DeviceEventTypeEXT deviceEvent;
  };
  static_assert( sizeof( DeviceEventInfoEXT ) == sizeof( VkDeviceEventInfoEXT ), "struct and wrapper have different size!" );

  enum class DisplayEventTypeEXT
  {
    eFirstPixelOut = VK_DISPLAY_EVENT_TYPE_FIRST_PIXEL_OUT_EXT
  };

  struct DisplayEventInfoEXT
  {
    DisplayEventInfoEXT( DisplayEventTypeEXT displayEvent_ = DisplayEventTypeEXT::eFirstPixelOut )
      : sType( StructureType::eDisplayEventInfoEXT )
      , pNext( nullptr )
      , displayEvent( displayEvent_ )
    {
    }

    DisplayEventInfoEXT( VkDisplayEventInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayEventInfoEXT) );
    }

    DisplayEventInfoEXT& operator=( VkDisplayEventInfoEXT const & rhs )
    {
      memcpy( this, &rhs, sizeof(DisplayEventInfoEXT) );
      return *this;
    }

    DisplayEventInfoEXT& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    DisplayEventInfoEXT& setDisplayEvent( DisplayEventTypeEXT displayEvent_ )
    {
      displayEvent = displayEvent_;
      return *this;
    }

    operator const VkDisplayEventInfoEXT&() const
    {
      return *reinterpret_cast<const VkDisplayEventInfoEXT*>(this);
    }

    bool operator==( DisplayEventInfoEXT const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( displayEvent == rhs.displayEvent );
    }

    bool operator!=( DisplayEventInfoEXT const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    DisplayEventTypeEXT displayEvent;
  };
  static_assert( sizeof( DisplayEventInfoEXT ) == sizeof( VkDisplayEventInfoEXT ), "struct and wrapper have different size!" );

  VULKAN_HPP_INLINE Result enumerateInstanceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceLayerProperties( pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Allocator = std::allocator<LayerProperties>>
  typename ResultValueType<std::vector<LayerProperties,Allocator>>::type enumerateInstanceLayerProperties()
  {
    std::vector<LayerProperties,Allocator> properties;
    uint32_t propertyCount;
    Result result;
    do
    {
      result = static_cast<Result>( vkEnumerateInstanceLayerProperties( &propertyCount, nullptr ) );
      if ( ( result == Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<Result>( vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
      }
    } while ( result == Result::eIncomplete );
    assert( propertyCount <= properties.size() ); 
    properties.resize( propertyCount ); 
    return createResultValue( result, properties, "vk::enumerateInstanceLayerProperties" );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  VULKAN_HPP_INLINE Result enumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename Allocator = std::allocator<ExtensionProperties>>
  typename ResultValueType<std::vector<ExtensionProperties,Allocator>>::type enumerateInstanceExtensionProperties( Optional<const std::string> layerName = nullptr )
  {
    std::vector<ExtensionProperties,Allocator> properties;
    uint32_t propertyCount;
    Result result;
    do
    {
      result = static_cast<Result>( vkEnumerateInstanceExtensionProperties( layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
      if ( ( result == Result::eSuccess ) && propertyCount )
      {
        properties.resize( propertyCount );
        result = static_cast<Result>( vkEnumerateInstanceExtensionProperties( layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
      }
    } while ( result == Result::eIncomplete );
    assert( propertyCount <= properties.size() ); 
    properties.resize( propertyCount ); 
    return createResultValue( result, properties, "vk::enumerateInstanceExtensionProperties" );
  }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

  // forward declarations
  struct CmdProcessCommandsInfoNVX;

  class CommandBuffer
  {
  public:
    CommandBuffer()
      : m_commandBuffer(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    CommandBuffer(VkCommandBuffer commandBuffer)
       : m_commandBuffer(commandBuffer)
    {}

    CommandBuffer& operator=(VkCommandBuffer commandBuffer)
    {
      m_commandBuffer = commandBuffer;
      return *this;
    }
#endif

    bool operator==(CommandBuffer const &rhs) const
    {
      return m_commandBuffer == rhs.m_commandBuffer;
    }

    bool operator!=(CommandBuffer const &rhs) const
    {
      return m_commandBuffer != rhs.m_commandBuffer;
    }

    bool operator<(CommandBuffer const &rhs) const
    {
      return m_commandBuffer < rhs.m_commandBuffer;
    }

    Result begin( const CommandBufferBeginInfo* pBeginInfo ) const
    {
      return static_cast<Result>( vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( pBeginInfo ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type begin( const CommandBufferBeginInfo & beginInfo ) const
    {
      Result result = static_cast<Result>( vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( &beginInfo ) ) );
      return createResultValue( result, "vk::CommandBuffer::begin" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result end(  ) const
    {
      return static_cast<Result>( vkEndCommandBuffer( m_commandBuffer ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type end() const
    {
      Result result = static_cast<Result>( vkEndCommandBuffer( m_commandBuffer ) );
      return createResultValue( result, "vk::CommandBuffer::end" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result reset( CommandBufferResetFlags flags ) const
    {
      return static_cast<Result>( vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type reset( CommandBufferResetFlags flags ) const
    {
      Result result = static_cast<Result>( vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
      return createResultValue( result, "vk::CommandBuffer::reset" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void bindPipeline( PipelineBindPoint pipelineBindPoint, Pipeline pipeline ) const
    {
      vkCmdBindPipeline( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipeline>( pipeline ) );
    }


    void setViewport( uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports ) const
    {
      vkCmdSetViewport( m_commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>( pViewports ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void setViewport( uint32_t firstViewport, ArrayProxy<const Viewport> viewports ) const
    {
      vkCmdSetViewport( m_commandBuffer, firstViewport, viewports.size() , reinterpret_cast<const VkViewport*>( viewports.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void setScissor( uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors ) const
    {
      vkCmdSetScissor( m_commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>( pScissors ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void setScissor( uint32_t firstScissor, ArrayProxy<const Rect2D> scissors ) const
    {
      vkCmdSetScissor( m_commandBuffer, firstScissor, scissors.size() , reinterpret_cast<const VkRect2D*>( scissors.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void setLineWidth( float lineWidth ) const
    {
      vkCmdSetLineWidth( m_commandBuffer, lineWidth );
    }


    void setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) const
    {
      vkCmdSetDepthBias( m_commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
    }


    void setBlendConstants( const float blendConstants[4] ) const
    {
      vkCmdSetBlendConstants( m_commandBuffer, blendConstants );
    }


    void setDepthBounds( float minDepthBounds, float maxDepthBounds ) const
    {
      vkCmdSetDepthBounds( m_commandBuffer, minDepthBounds, maxDepthBounds );
    }


    void setStencilCompareMask( StencilFaceFlags faceMask, uint32_t compareMask ) const
    {
      vkCmdSetStencilCompareMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
    }


    void setStencilWriteMask( StencilFaceFlags faceMask, uint32_t writeMask ) const
    {
      vkCmdSetStencilWriteMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
    }


    void setStencilReference( StencilFaceFlags faceMask, uint32_t reference ) const
    {
      vkCmdSetStencilReference( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
    }


    void bindDescriptorSets( PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets ) const
    {
      vkCmdBindDescriptorSets( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipelineLayout>( layout ), firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>( pDescriptorSets ), dynamicOffsetCount, pDynamicOffsets );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void bindDescriptorSets( PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, ArrayProxy<const DescriptorSet> descriptorSets, ArrayProxy<const uint32_t> dynamicOffsets ) const
    {
      vkCmdBindDescriptorSets( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipelineLayout>( layout ), firstSet, descriptorSets.size() , reinterpret_cast<const VkDescriptorSet*>( descriptorSets.data() ), dynamicOffsets.size() , dynamicOffsets.data() );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void bindIndexBuffer( Buffer buffer, DeviceSize offset, IndexType indexType ) const
    {
      vkCmdBindIndexBuffer( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, static_cast<VkIndexType>( indexType ) );
    }


    void bindVertexBuffers( uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets ) const
    {
      vkCmdBindVertexBuffers( m_commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>( pBuffers ), pOffsets );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void bindVertexBuffers( uint32_t firstBinding, ArrayProxy<const Buffer> buffers, ArrayProxy<const DeviceSize> offsets ) const
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      assert( buffers.size() == offsets.size() );
#else
      if ( buffers.size() != offsets.size() )
      {
        throw std::logic_error( "vk::CommandBuffer::bindVertexBuffers: buffers.size() != offsets.size()" );
      }
#endif  // VULKAN_HPP_NO_EXCEPTIONS
      vkCmdBindVertexBuffers( m_commandBuffer, firstBinding, buffers.size() , reinterpret_cast<const VkBuffer*>( buffers.data() ), offsets.data() );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const
    {
      vkCmdDraw( m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
    }


    void drawIndexed( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) const
    {
      vkCmdDrawIndexed( m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
    }


    void drawIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }


    void drawIndexedIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndexedIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }


    void dispatch( uint32_t x, uint32_t y, uint32_t z ) const
    {
      vkCmdDispatch( m_commandBuffer, x, y, z );
    }


    void dispatchIndirect( Buffer buffer, DeviceSize offset ) const
    {
      vkCmdDispatchIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset );
    }


    void copyBuffer( Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions ) const
    {
      vkCmdCopyBuffer( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkBuffer>( dstBuffer ), regionCount, reinterpret_cast<const VkBufferCopy*>( pRegions ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void copyBuffer( Buffer srcBuffer, Buffer dstBuffer, ArrayProxy<const BufferCopy> regions ) const
    {
      vkCmdCopyBuffer( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkBuffer>( dstBuffer ), regions.size() , reinterpret_cast<const VkBufferCopy*>( regions.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void copyImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions ) const
    {
      vkCmdCopyImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageCopy*>( pRegions ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void copyImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, ArrayProxy<const ImageCopy> regions ) const
    {
      vkCmdCopyImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regions.size() , reinterpret_cast<const VkImageCopy*>( regions.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void blitImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter ) const
    {
      vkCmdBlitImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageBlit*>( pRegions ), static_cast<VkFilter>( filter ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void blitImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, ArrayProxy<const ImageBlit> regions, Filter filter ) const
    {
      vkCmdBlitImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regions.size() , reinterpret_cast<const VkImageBlit*>( regions.data() ), static_cast<VkFilter>( filter ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void copyBufferToImage( Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions ) const
    {
      vkCmdCopyBufferToImage( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void copyBufferToImage( Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, ArrayProxy<const BufferImageCopy> regions ) const
    {
      vkCmdCopyBufferToImage( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regions.size() , reinterpret_cast<const VkBufferImageCopy*>( regions.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void copyImageToBuffer( Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions ) const
    {
      vkCmdCopyImageToBuffer( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkBuffer>( dstBuffer ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void copyImageToBuffer( Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, ArrayProxy<const BufferImageCopy> regions ) const
    {
      vkCmdCopyImageToBuffer( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkBuffer>( dstBuffer ), regions.size() , reinterpret_cast<const VkBufferImageCopy*>( regions.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void updateBuffer( Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const void* pData ) const
    {
      vkCmdUpdateBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, dataSize, pData );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename T>
    void updateBuffer( Buffer dstBuffer, DeviceSize dstOffset, ArrayProxy<const T> data ) const
    {
      vkCmdUpdateBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, data.size() * sizeof( T ) , reinterpret_cast<const void*>( data.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void fillBuffer( Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data ) const
    {
      vkCmdFillBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, size, data );
    }


    void clearColorImage( Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges ) const
    {
      vkCmdClearColorImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( pColor ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void clearColorImage( Image image, ImageLayout imageLayout, const ClearColorValue & color, ArrayProxy<const ImageSubresourceRange> ranges ) const
    {
      vkCmdClearColorImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( &color ), ranges.size() , reinterpret_cast<const VkImageSubresourceRange*>( ranges.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void clearDepthStencilImage( Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges ) const
    {
      vkCmdClearDepthStencilImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( pDepthStencil ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void clearDepthStencilImage( Image image, ImageLayout imageLayout, const ClearDepthStencilValue & depthStencil, ArrayProxy<const ImageSubresourceRange> ranges ) const
    {
      vkCmdClearDepthStencilImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( &depthStencil ), ranges.size() , reinterpret_cast<const VkImageSubresourceRange*>( ranges.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void clearAttachments( uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects ) const
    {
      vkCmdClearAttachments( m_commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>( pAttachments ), rectCount, reinterpret_cast<const VkClearRect*>( pRects ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void clearAttachments( ArrayProxy<const ClearAttachment> attachments, ArrayProxy<const ClearRect> rects ) const
    {
      vkCmdClearAttachments( m_commandBuffer, attachments.size() , reinterpret_cast<const VkClearAttachment*>( attachments.data() ), rects.size() , reinterpret_cast<const VkClearRect*>( rects.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void resolveImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions ) const
    {
      vkCmdResolveImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageResolve*>( pRegions ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void resolveImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, ArrayProxy<const ImageResolve> regions ) const
    {
      vkCmdResolveImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regions.size() , reinterpret_cast<const VkImageResolve*>( regions.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void setEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdSetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }


    void resetEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdResetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }


    void waitEvents( uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers ) const
    {
      vkCmdWaitEvents( m_commandBuffer, eventCount, reinterpret_cast<const VkEvent*>( pEvents ), static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void waitEvents( ArrayProxy<const Event> events, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, ArrayProxy<const MemoryBarrier> memoryBarriers, ArrayProxy<const BufferMemoryBarrier> bufferMemoryBarriers, ArrayProxy<const ImageMemoryBarrier> imageMemoryBarriers ) const
    {
      vkCmdWaitEvents( m_commandBuffer, events.size() , reinterpret_cast<const VkEvent*>( events.data() ), static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), memoryBarriers.size() , reinterpret_cast<const VkMemoryBarrier*>( memoryBarriers.data() ), bufferMemoryBarriers.size() , reinterpret_cast<const VkBufferMemoryBarrier*>( bufferMemoryBarriers.data() ), imageMemoryBarriers.size() , reinterpret_cast<const VkImageMemoryBarrier*>( imageMemoryBarriers.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void pipelineBarrier( PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers ) const
    {
      vkCmdPipelineBarrier( m_commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void pipelineBarrier( PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, ArrayProxy<const MemoryBarrier> memoryBarriers, ArrayProxy<const BufferMemoryBarrier> bufferMemoryBarriers, ArrayProxy<const ImageMemoryBarrier> imageMemoryBarriers ) const
    {
      vkCmdPipelineBarrier( m_commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), memoryBarriers.size() , reinterpret_cast<const VkMemoryBarrier*>( memoryBarriers.data() ), bufferMemoryBarriers.size() , reinterpret_cast<const VkBufferMemoryBarrier*>( bufferMemoryBarriers.data() ), imageMemoryBarriers.size() , reinterpret_cast<const VkImageMemoryBarrier*>( imageMemoryBarriers.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void beginQuery( QueryPool queryPool, uint32_t query, QueryControlFlags flags ) const
    {
      vkCmdBeginQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query, static_cast<VkQueryControlFlags>( flags ) );
    }


    void endQuery( QueryPool queryPool, uint32_t query ) const
    {
      vkCmdEndQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query );
    }


    void resetQueryPool( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const
    {
      vkCmdResetQueryPool( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
    }


    void writeTimestamp( PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query ) const
    {
      vkCmdWriteTimestamp( m_commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), static_cast<VkQueryPool>( queryPool ), query );
    }


    void copyQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags ) const
    {
      vkCmdCopyQueryPoolResults( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, static_cast<VkBuffer>( dstBuffer ), dstOffset, stride, static_cast<VkQueryResultFlags>( flags ) );
    }


    void pushConstants( PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues ) const
    {
      vkCmdPushConstants( m_commandBuffer, static_cast<VkPipelineLayout>( layout ), static_cast<VkShaderStageFlags>( stageFlags ), offset, size, pValues );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename T>
    void pushConstants( PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, ArrayProxy<const T> values ) const
    {
      vkCmdPushConstants( m_commandBuffer, static_cast<VkPipelineLayout>( layout ), static_cast<VkShaderStageFlags>( stageFlags ), offset, values.size() * sizeof( T ) , reinterpret_cast<const void*>( values.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void beginRenderPass( const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents ) const
    {
      vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( pRenderPassBegin ), static_cast<VkSubpassContents>( contents ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void beginRenderPass( const RenderPassBeginInfo & renderPassBegin, SubpassContents contents ) const
    {
      vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( &renderPassBegin ), static_cast<VkSubpassContents>( contents ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void nextSubpass( SubpassContents contents ) const
    {
      vkCmdNextSubpass( m_commandBuffer, static_cast<VkSubpassContents>( contents ) );
    }


    void endRenderPass(  ) const
    {
      vkCmdEndRenderPass( m_commandBuffer );
    }


    void executeCommands( uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers ) const
    {
      vkCmdExecuteCommands( m_commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void executeCommands( ArrayProxy<const CommandBuffer> commandBuffers ) const
    {
      vkCmdExecuteCommands( m_commandBuffer, commandBuffers.size() , reinterpret_cast<const VkCommandBuffer*>( commandBuffers.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void debugMarkerBeginEXT( DebugMarkerMarkerInfoEXT* pMarkerInfo ) const
    {
      vkCmdDebugMarkerBeginEXT( m_commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>( pMarkerInfo ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    DebugMarkerMarkerInfoEXT debugMarkerBeginEXT() const
    {
      DebugMarkerMarkerInfoEXT markerInfo;
      vkCmdDebugMarkerBeginEXT( m_commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>( &markerInfo ) );
      return markerInfo;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void debugMarkerEndEXT(  ) const
    {
      vkCmdDebugMarkerEndEXT( m_commandBuffer );
    }


    void debugMarkerInsertEXT( DebugMarkerMarkerInfoEXT* pMarkerInfo ) const
    {
      vkCmdDebugMarkerInsertEXT( m_commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>( pMarkerInfo ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    DebugMarkerMarkerInfoEXT debugMarkerInsertEXT() const
    {
      DebugMarkerMarkerInfoEXT markerInfo;
      vkCmdDebugMarkerInsertEXT( m_commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>( &markerInfo ) );
      return markerInfo;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void drawIndirectCountAMD( Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) const
    {
      vkCmdDrawIndirectCountAMD( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, static_cast<VkBuffer>( countBuffer ), countBufferOffset, maxDrawCount, stride );
    }


    void drawIndexedIndirectCountAMD( Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) const
    {
      vkCmdDrawIndexedIndirectCountAMD( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, static_cast<VkBuffer>( countBuffer ), countBufferOffset, maxDrawCount, stride );
    }


    void processCommandsNVX( const CmdProcessCommandsInfoNVX* pProcessCommandsInfo ) const
    {
      vkCmdProcessCommandsNVX( m_commandBuffer, reinterpret_cast<const VkCmdProcessCommandsInfoNVX*>( pProcessCommandsInfo ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void processCommandsNVX( const CmdProcessCommandsInfoNVX & processCommandsInfo ) const
    {
      vkCmdProcessCommandsNVX( m_commandBuffer, reinterpret_cast<const VkCmdProcessCommandsInfoNVX*>( &processCommandsInfo ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void reserveSpaceForCommandsNVX( const CmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo ) const
    {
      vkCmdReserveSpaceForCommandsNVX( m_commandBuffer, reinterpret_cast<const VkCmdReserveSpaceForCommandsInfoNVX*>( pReserveSpaceInfo ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void reserveSpaceForCommandsNVX( const CmdReserveSpaceForCommandsInfoNVX & reserveSpaceInfo ) const
    {
      vkCmdReserveSpaceForCommandsNVX( m_commandBuffer, reinterpret_cast<const VkCmdReserveSpaceForCommandsInfoNVX*>( &reserveSpaceInfo ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkCommandBuffer() const
    {
      return m_commandBuffer;
    }

    explicit operator bool() const
    {
      return m_commandBuffer != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_commandBuffer == VK_NULL_HANDLE;
    }

  private:
    VkCommandBuffer m_commandBuffer;
  };
  static_assert( sizeof( CommandBuffer ) == sizeof( VkCommandBuffer ), "handle and wrapper have different size!" );

  struct SubmitInfo
  {
    SubmitInfo( uint32_t waitSemaphoreCount_ = 0, const Semaphore* pWaitSemaphores_ = nullptr, const PipelineStageFlags* pWaitDstStageMask_ = nullptr, uint32_t commandBufferCount_ = 0, const CommandBuffer* pCommandBuffers_ = nullptr, uint32_t signalSemaphoreCount_ = 0, const Semaphore* pSignalSemaphores_ = nullptr )
      : sType( StructureType::eSubmitInfo )
      , pNext( nullptr )
      , waitSemaphoreCount( waitSemaphoreCount_ )
      , pWaitSemaphores( pWaitSemaphores_ )
      , pWaitDstStageMask( pWaitDstStageMask_ )
      , commandBufferCount( commandBufferCount_ )
      , pCommandBuffers( pCommandBuffers_ )
      , signalSemaphoreCount( signalSemaphoreCount_ )
      , pSignalSemaphores( pSignalSemaphores_ )
    {
    }

    SubmitInfo( VkSubmitInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubmitInfo) );
    }

    SubmitInfo& operator=( VkSubmitInfo const & rhs )
    {
      memcpy( this, &rhs, sizeof(SubmitInfo) );
      return *this;
    }

    SubmitInfo& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    SubmitInfo& setWaitSemaphoreCount( uint32_t waitSemaphoreCount_ )
    {
      waitSemaphoreCount = waitSemaphoreCount_;
      return *this;
    }

    SubmitInfo& setPWaitSemaphores( const Semaphore* pWaitSemaphores_ )
    {
      pWaitSemaphores = pWaitSemaphores_;
      return *this;
    }

    SubmitInfo& setPWaitDstStageMask( const PipelineStageFlags* pWaitDstStageMask_ )
    {
      pWaitDstStageMask = pWaitDstStageMask_;
      return *this;
    }

    SubmitInfo& setCommandBufferCount( uint32_t commandBufferCount_ )
    {
      commandBufferCount = commandBufferCount_;
      return *this;
    }

    SubmitInfo& setPCommandBuffers( const CommandBuffer* pCommandBuffers_ )
    {
      pCommandBuffers = pCommandBuffers_;
      return *this;
    }

    SubmitInfo& setSignalSemaphoreCount( uint32_t signalSemaphoreCount_ )
    {
      signalSemaphoreCount = signalSemaphoreCount_;
      return *this;
    }

    SubmitInfo& setPSignalSemaphores( const Semaphore* pSignalSemaphores_ )
    {
      pSignalSemaphores = pSignalSemaphores_;
      return *this;
    }

    operator const VkSubmitInfo&() const
    {
      return *reinterpret_cast<const VkSubmitInfo*>(this);
    }

    bool operator==( SubmitInfo const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( waitSemaphoreCount == rhs.waitSemaphoreCount )
          && ( pWaitSemaphores == rhs.pWaitSemaphores )
          && ( pWaitDstStageMask == rhs.pWaitDstStageMask )
          && ( commandBufferCount == rhs.commandBufferCount )
          && ( pCommandBuffers == rhs.pCommandBuffers )
          && ( signalSemaphoreCount == rhs.signalSemaphoreCount )
          && ( pSignalSemaphores == rhs.pSignalSemaphores );
    }

    bool operator!=( SubmitInfo const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    uint32_t waitSemaphoreCount;
    const Semaphore* pWaitSemaphores;
    const PipelineStageFlags* pWaitDstStageMask;
    uint32_t commandBufferCount;
    const CommandBuffer* pCommandBuffers;
    uint32_t signalSemaphoreCount;
    const Semaphore* pSignalSemaphores;
  };
  static_assert( sizeof( SubmitInfo ) == sizeof( VkSubmitInfo ), "struct and wrapper have different size!" );

  class Queue
  {
  public:
    Queue()
      : m_queue(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Queue(VkQueue queue)
       : m_queue(queue)
    {}

    Queue& operator=(VkQueue queue)
    {
      m_queue = queue;
      return *this;
    }
#endif

    bool operator==(Queue const &rhs) const
    {
      return m_queue == rhs.m_queue;
    }

    bool operator!=(Queue const &rhs) const
    {
      return m_queue != rhs.m_queue;
    }

    bool operator<(Queue const &rhs) const
    {
      return m_queue < rhs.m_queue;
    }

    Result submit( uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence ) const
    {
      return static_cast<Result>( vkQueueSubmit( m_queue, submitCount, reinterpret_cast<const VkSubmitInfo*>( pSubmits ), static_cast<VkFence>( fence ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type submit( ArrayProxy<const SubmitInfo> submits, Fence fence ) const
    {
      Result result = static_cast<Result>( vkQueueSubmit( m_queue, submits.size() , reinterpret_cast<const VkSubmitInfo*>( submits.data() ), static_cast<VkFence>( fence ) ) );
      return createResultValue( result, "vk::Queue::submit" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result waitIdle(  ) const
    {
      return static_cast<Result>( vkQueueWaitIdle( m_queue ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type waitIdle() const
    {
      Result result = static_cast<Result>( vkQueueWaitIdle( m_queue ) );
      return createResultValue( result, "vk::Queue::waitIdle" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result bindSparse( uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence ) const
    {
      return static_cast<Result>( vkQueueBindSparse( m_queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>( pBindInfo ), static_cast<VkFence>( fence ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type bindSparse( ArrayProxy<const BindSparseInfo> bindInfo, Fence fence ) const
    {
      Result result = static_cast<Result>( vkQueueBindSparse( m_queue, bindInfo.size() , reinterpret_cast<const VkBindSparseInfo*>( bindInfo.data() ), static_cast<VkFence>( fence ) ) );
      return createResultValue( result, "vk::Queue::bindSparse" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result presentKHR( const PresentInfoKHR* pPresentInfo ) const
    {
      return static_cast<Result>( vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR*>( pPresentInfo ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result presentKHR( const PresentInfoKHR & presentInfo ) const
    {
      Result result = static_cast<Result>( vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR*>( &presentInfo ) ) );
      return createResultValue( result, "vk::Queue::presentKHR", { Result::eSuccess, Result::eSuboptimalKHR } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkQueue() const
    {
      return m_queue;
    }

    explicit operator bool() const
    {
      return m_queue != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_queue == VK_NULL_HANDLE;
    }

  private:
    VkQueue m_queue;
  };
  static_assert( sizeof( Queue ) == sizeof( VkQueue ), "handle and wrapper have different size!" );

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  class BufferDeleter
  {
  public:
    BufferDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Buffer buffer );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueBuffer = UniqueHandle<Buffer, BufferDeleter>;

  class BufferViewDeleter
  {
  public:
    BufferViewDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( BufferView bufferView );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueBufferView = UniqueHandle<BufferView, BufferViewDeleter>;

  class CommandBufferDeleter
  {
  public:
    CommandBufferDeleter( Device const* device = nullptr, CommandPool commandPool = CommandPool() )
      : m_device( device )
      , m_commandPool( commandPool )
    {}

    void operator()( CommandBuffer commandBuffer );

  private:
    Device const* m_device;
    CommandPool m_commandPool;
  };
  using UniqueCommandBuffer = UniqueHandle<CommandBuffer, CommandBufferDeleter>;

  class CommandPoolDeleter
  {
  public:
    CommandPoolDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( CommandPool commandPool );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueCommandPool = UniqueHandle<CommandPool, CommandPoolDeleter>;

  class DescriptorPoolDeleter
  {
  public:
    DescriptorPoolDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( DescriptorPool descriptorPool );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueDescriptorPool = UniqueHandle<DescriptorPool, DescriptorPoolDeleter>;

  class DescriptorSetDeleter
  {
  public:
    DescriptorSetDeleter( Device const* device = nullptr, DescriptorPool descriptorPool = DescriptorPool() )
      : m_device( device )
      , m_descriptorPool( descriptorPool )
    {}

    void operator()( DescriptorSet descriptorSet );

  private:
    Device const* m_device;
    DescriptorPool m_descriptorPool;
  };
  using UniqueDescriptorSet = UniqueHandle<DescriptorSet, DescriptorSetDeleter>;

  class DescriptorSetLayoutDeleter
  {
  public:
    DescriptorSetLayoutDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( DescriptorSetLayout descriptorSetLayout );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueDescriptorSetLayout = UniqueHandle<DescriptorSetLayout, DescriptorSetLayoutDeleter>;

  class DeviceMemoryDeleter
  {
  public:
    DeviceMemoryDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( DeviceMemory deviceMemory );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueDeviceMemory = UniqueHandle<DeviceMemory, DeviceMemoryDeleter>;

  class EventDeleter
  {
  public:
    EventDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Event event );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueEvent = UniqueHandle<Event, EventDeleter>;

  class FenceDeleter
  {
  public:
    FenceDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Fence fence );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueFence = UniqueHandle<Fence, FenceDeleter>;

  class FramebufferDeleter
  {
  public:
    FramebufferDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Framebuffer framebuffer );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueFramebuffer = UniqueHandle<Framebuffer, FramebufferDeleter>;

  class ImageDeleter
  {
  public:
    ImageDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Image image );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueImage = UniqueHandle<Image, ImageDeleter>;

  class ImageViewDeleter
  {
  public:
    ImageViewDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( ImageView imageView );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueImageView = UniqueHandle<ImageView, ImageViewDeleter>;

  class IndirectCommandsLayoutNVXDeleter
  {
  public:
    IndirectCommandsLayoutNVXDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( IndirectCommandsLayoutNVX indirectCommandsLayoutNVX );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueIndirectCommandsLayoutNVX = UniqueHandle<IndirectCommandsLayoutNVX, IndirectCommandsLayoutNVXDeleter>;

  class ObjectTableNVXDeleter
  {
  public:
    ObjectTableNVXDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( ObjectTableNVX objectTableNVX );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueObjectTableNVX = UniqueHandle<ObjectTableNVX, ObjectTableNVXDeleter>;

  class PipelineDeleter
  {
  public:
    PipelineDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Pipeline pipeline );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniquePipeline = UniqueHandle<Pipeline, PipelineDeleter>;

  class PipelineCacheDeleter
  {
  public:
    PipelineCacheDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( PipelineCache pipelineCache );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniquePipelineCache = UniqueHandle<PipelineCache, PipelineCacheDeleter>;

  class PipelineLayoutDeleter
  {
  public:
    PipelineLayoutDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( PipelineLayout pipelineLayout );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniquePipelineLayout = UniqueHandle<PipelineLayout, PipelineLayoutDeleter>;

  class QueryPoolDeleter
  {
  public:
    QueryPoolDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( QueryPool queryPool );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueQueryPool = UniqueHandle<QueryPool, QueryPoolDeleter>;

  class RenderPassDeleter
  {
  public:
    RenderPassDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( RenderPass renderPass );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueRenderPass = UniqueHandle<RenderPass, RenderPassDeleter>;

  class SamplerDeleter
  {
  public:
    SamplerDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Sampler sampler );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueSampler = UniqueHandle<Sampler, SamplerDeleter>;

  class SemaphoreDeleter
  {
  public:
    SemaphoreDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( Semaphore semaphore );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueSemaphore = UniqueHandle<Semaphore, SemaphoreDeleter>;

  class ShaderModuleDeleter
  {
  public:
    ShaderModuleDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( ShaderModule shaderModule );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueShaderModule = UniqueHandle<ShaderModule, ShaderModuleDeleter>;

  class SwapchainKHRDeleter
  {
  public:
    SwapchainKHRDeleter( Device const* device = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_device( device )
      , m_allocator( allocator )
    {}

    void operator()( SwapchainKHR swapchainKHR );

  private:
    Device const* m_device;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueSwapchainKHR = UniqueHandle<SwapchainKHR, SwapchainKHRDeleter>;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  class Device
  {
  public:
    Device()
      : m_device(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Device(VkDevice device)
       : m_device(device)
    {}

    Device& operator=(VkDevice device)
    {
      m_device = device;
      return *this;
    }
#endif

    bool operator==(Device const &rhs) const
    {
      return m_device == rhs.m_device;
    }

    bool operator!=(Device const &rhs) const
    {
      return m_device != rhs.m_device;
    }

    bool operator<(Device const &rhs) const
    {
      return m_device < rhs.m_device;
    }

    PFN_vkVoidFunction getProcAddr( const char* pName ) const
    {
      return vkGetDeviceProcAddr( m_device, pName );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PFN_vkVoidFunction getProcAddr( const std::string & name ) const
    {
      return vkGetDeviceProcAddr( m_device, name.c_str() );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroy( const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDevice( m_device, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroy( Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyDevice( m_device, reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue ) const
    {
      vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>( pQueue ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Queue getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const
    {
      Queue queue;
      vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>( &queue ) );
      return queue;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result waitIdle(  ) const
    {
      return static_cast<Result>( vkDeviceWaitIdle( m_device ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type waitIdle() const
    {
      Result result = static_cast<Result>( vkDeviceWaitIdle( m_device ) );
      return createResultValue( result, "vk::Device::waitIdle" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result allocateMemory( const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory ) const
    {
      return static_cast<Result>( vkAllocateMemory( m_device, reinterpret_cast<const VkMemoryAllocateInfo*>( pAllocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDeviceMemory*>( pMemory ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DeviceMemory>::type allocateMemory( const MemoryAllocateInfo & allocateInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DeviceMemory memory;
      Result result = static_cast<Result>( vkAllocateMemory( m_device, reinterpret_cast<const VkMemoryAllocateInfo*>( &allocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDeviceMemory*>( &memory ) ) );
      return createResultValue( result, memory, "vk::Device::allocateMemory" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueDeviceMemory allocateMemoryUnique( const MemoryAllocateInfo & allocateInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DeviceMemoryDeleter deleter( this, allocator );
      return UniqueDeviceMemory( allocateMemory( allocateInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void freeMemory( DeviceMemory memory, const AllocationCallbacks* pAllocator ) const
    {
      vkFreeMemory( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void freeMemory( DeviceMemory memory, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkFreeMemory( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result mapMemory( DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData ) const
    {
      return static_cast<Result>( vkMapMemory( m_device, static_cast<VkDeviceMemory>( memory ), offset, size, static_cast<VkMemoryMapFlags>( flags ), ppData ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void*>::type mapMemory( DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags = MemoryMapFlags() ) const
    {
      void* pData;
      Result result = static_cast<Result>( vkMapMemory( m_device, static_cast<VkDeviceMemory>( memory ), offset, size, static_cast<VkMemoryMapFlags>( flags ), &pData ) );
      return createResultValue( result, pData, "vk::Device::mapMemory" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void unmapMemory( DeviceMemory memory ) const
    {
      vkUnmapMemory( m_device, static_cast<VkDeviceMemory>( memory ) );
    }


    Result flushMappedMemoryRanges( uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges ) const
    {
      return static_cast<Result>( vkFlushMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type flushMappedMemoryRanges( ArrayProxy<const MappedMemoryRange> memoryRanges ) const
    {
      Result result = static_cast<Result>( vkFlushMappedMemoryRanges( m_device, memoryRanges.size() , reinterpret_cast<const VkMappedMemoryRange*>( memoryRanges.data() ) ) );
      return createResultValue( result, "vk::Device::flushMappedMemoryRanges" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result invalidateMappedMemoryRanges( uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges ) const
    {
      return static_cast<Result>( vkInvalidateMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type invalidateMappedMemoryRanges( ArrayProxy<const MappedMemoryRange> memoryRanges ) const
    {
      Result result = static_cast<Result>( vkInvalidateMappedMemoryRanges( m_device, memoryRanges.size() , reinterpret_cast<const VkMappedMemoryRange*>( memoryRanges.data() ) ) );
      return createResultValue( result, "vk::Device::invalidateMappedMemoryRanges" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getMemoryCommitment( DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes ) const
    {
      vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), pCommittedMemoryInBytes );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    DeviceSize getMemoryCommitment( DeviceMemory memory ) const
    {
      DeviceSize committedMemoryInBytes;
      vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), &committedMemoryInBytes );
      return committedMemoryInBytes;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getBufferMemoryRequirements( Buffer buffer, MemoryRequirements* pMemoryRequirements ) const
    {
      vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    MemoryRequirements getBufferMemoryRequirements( Buffer buffer ) const
    {
      MemoryRequirements memoryRequirements;
      vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements*>( &memoryRequirements ) );
      return memoryRequirements;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result bindBufferMemory( Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      return static_cast<Result>( vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type bindBufferMemory( Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      Result result = static_cast<Result>( vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
      return createResultValue( result, "vk::Device::bindBufferMemory" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getImageMemoryRequirements( Image image, MemoryRequirements* pMemoryRequirements ) const
    {
      vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    MemoryRequirements getImageMemoryRequirements( Image image ) const
    {
      MemoryRequirements memoryRequirements;
      vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements*>( &memoryRequirements ) );
      return memoryRequirements;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result bindImageMemory( Image image, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      return static_cast<Result>( vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type bindImageMemory( Image image, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      Result result = static_cast<Result>( vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
      return createResultValue( result, "vk::Device::bindImageMemory" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getImageSparseMemoryRequirements( Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements ) const
    {
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), pSparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( pSparseMemoryRequirements ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<SparseImageMemoryRequirements>>
    std::vector<SparseImageMemoryRequirements,Allocator> getImageSparseMemoryRequirements( Image image ) const
    {
      std::vector<SparseImageMemoryRequirements,Allocator> sparseMemoryRequirements;
      uint32_t sparseMemoryRequirementCount;
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), &sparseMemoryRequirementCount, nullptr );
      sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), &sparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( sparseMemoryRequirements.data() ) );
      return sparseMemoryRequirements;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createFence( const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence ) const
    {
      return static_cast<Result>( vkCreateFence( m_device, reinterpret_cast<const VkFenceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFence*>( pFence ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Fence>::type createFence( const FenceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Fence fence;
      Result result = static_cast<Result>( vkCreateFence( m_device, reinterpret_cast<const VkFenceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkFence*>( &fence ) ) );
      return createResultValue( result, fence, "vk::Device::createFence" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueFence createFenceUnique( const FenceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      FenceDeleter deleter( this, allocator );
      return UniqueFence( createFence( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyFence( Fence fence, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyFence( Fence fence, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result resetFences( uint32_t fenceCount, const Fence* pFences ) const
    {
      return static_cast<Result>( vkResetFences( m_device, fenceCount, reinterpret_cast<const VkFence*>( pFences ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type resetFences( ArrayProxy<const Fence> fences ) const
    {
      Result result = static_cast<Result>( vkResetFences( m_device, fences.size() , reinterpret_cast<const VkFence*>( fences.data() ) ) );
      return createResultValue( result, "vk::Device::resetFences" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result getFenceStatus( Fence fence ) const
    {
      return static_cast<Result>( vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result getFenceStatus( Fence fence ) const
    {
      Result result = static_cast<Result>( vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
      return createResultValue( result, "vk::Device::getFenceStatus", { Result::eSuccess, Result::eNotReady } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result waitForFences( uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout ) const
    {
      return static_cast<Result>( vkWaitForFences( m_device, fenceCount, reinterpret_cast<const VkFence*>( pFences ), waitAll, timeout ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result waitForFences( ArrayProxy<const Fence> fences, Bool32 waitAll, uint64_t timeout ) const
    {
      Result result = static_cast<Result>( vkWaitForFences( m_device, fences.size() , reinterpret_cast<const VkFence*>( fences.data() ), waitAll, timeout ) );
      return createResultValue( result, "vk::Device::waitForFences", { Result::eSuccess, Result::eTimeout } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createSemaphore( const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore ) const
    {
      return static_cast<Result>( vkCreateSemaphore( m_device, reinterpret_cast<const VkSemaphoreCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSemaphore*>( pSemaphore ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Semaphore>::type createSemaphore( const SemaphoreCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Semaphore semaphore;
      Result result = static_cast<Result>( vkCreateSemaphore( m_device, reinterpret_cast<const VkSemaphoreCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSemaphore*>( &semaphore ) ) );
      return createResultValue( result, semaphore, "vk::Device::createSemaphore" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSemaphore createSemaphoreUnique( const SemaphoreCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SemaphoreDeleter deleter( this, allocator );
      return UniqueSemaphore( createSemaphore( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroySemaphore( Semaphore semaphore, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroySemaphore( Semaphore semaphore, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createEvent( const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent ) const
    {
      return static_cast<Result>( vkCreateEvent( m_device, reinterpret_cast<const VkEventCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkEvent*>( pEvent ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Event>::type createEvent( const EventCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Event event;
      Result result = static_cast<Result>( vkCreateEvent( m_device, reinterpret_cast<const VkEventCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkEvent*>( &event ) ) );
      return createResultValue( result, event, "vk::Device::createEvent" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueEvent createEventUnique( const EventCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      EventDeleter deleter( this, allocator );
      return UniqueEvent( createEvent( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyEvent( Event event, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyEvent( Event event, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result getEventStatus( Event event ) const
    {
      return static_cast<Result>( vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result getEventStatus( Event event ) const
    {
      Result result = static_cast<Result>( vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
      return createResultValue( result, "vk::Device::getEventStatus", { Result::eEventSet, Result::eEventReset } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result setEvent( Event event ) const
    {
      return static_cast<Result>( vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type setEvent( Event event ) const
    {
      Result result = static_cast<Result>( vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
      return createResultValue( result, "vk::Device::setEvent" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result resetEvent( Event event ) const
    {
      return static_cast<Result>( vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type resetEvent( Event event ) const
    {
      Result result = static_cast<Result>( vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
      return createResultValue( result, "vk::Device::resetEvent" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createQueryPool( const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool ) const
    {
      return static_cast<Result>( vkCreateQueryPool( m_device, reinterpret_cast<const VkQueryPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkQueryPool*>( pQueryPool ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<QueryPool>::type createQueryPool( const QueryPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      QueryPool queryPool;
      Result result = static_cast<Result>( vkCreateQueryPool( m_device, reinterpret_cast<const VkQueryPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkQueryPool*>( &queryPool ) ) );
      return createResultValue( result, queryPool, "vk::Device::createQueryPool" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueQueryPool createQueryPoolUnique( const QueryPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      QueryPoolDeleter deleter( this, allocator );
      return UniqueQueryPool( createQueryPool( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyQueryPool( QueryPool queryPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyQueryPool( m_device, static_cast<VkQueryPool>( queryPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyQueryPool( QueryPool queryPool, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyQueryPool( m_device, static_cast<VkQueryPool>( queryPool ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags ) const
    {
      return static_cast<Result>( vkGetQueryPoolResults( m_device, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, dataSize, pData, stride, static_cast<VkQueryResultFlags>( flags ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename T>
    Result getQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, ArrayProxy<T> data, DeviceSize stride, QueryResultFlags flags ) const
    {
      Result result = static_cast<Result>( vkGetQueryPoolResults( m_device, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, data.size() * sizeof( T ) , reinterpret_cast<void*>( data.data() ), stride, static_cast<VkQueryResultFlags>( flags ) ) );
      return createResultValue( result, "vk::Device::getQueryPoolResults", { Result::eSuccess, Result::eNotReady } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createBuffer( const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer ) const
    {
      return static_cast<Result>( vkCreateBuffer( m_device, reinterpret_cast<const VkBufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkBuffer*>( pBuffer ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Buffer>::type createBuffer( const BufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Buffer buffer;
      Result result = static_cast<Result>( vkCreateBuffer( m_device, reinterpret_cast<const VkBufferCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkBuffer*>( &buffer ) ) );
      return createResultValue( result, buffer, "vk::Device::createBuffer" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueBuffer createBufferUnique( const BufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      BufferDeleter deleter( this, allocator );
      return UniqueBuffer( createBuffer( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyBuffer( Buffer buffer, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyBuffer( Buffer buffer, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createBufferView( const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView ) const
    {
      return static_cast<Result>( vkCreateBufferView( m_device, reinterpret_cast<const VkBufferViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkBufferView*>( pView ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<BufferView>::type createBufferView( const BufferViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      BufferView view;
      Result result = static_cast<Result>( vkCreateBufferView( m_device, reinterpret_cast<const VkBufferViewCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkBufferView*>( &view ) ) );
      return createResultValue( result, view, "vk::Device::createBufferView" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueBufferView createBufferViewUnique( const BufferViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      BufferViewDeleter deleter( this, allocator );
      return UniqueBufferView( createBufferView( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyBufferView( BufferView bufferView, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyBufferView( BufferView bufferView, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createImage( const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage ) const
    {
      return static_cast<Result>( vkCreateImage( m_device, reinterpret_cast<const VkImageCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkImage*>( pImage ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Image>::type createImage( const ImageCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Image image;
      Result result = static_cast<Result>( vkCreateImage( m_device, reinterpret_cast<const VkImageCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkImage*>( &image ) ) );
      return createResultValue( result, image, "vk::Device::createImage" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueImage createImageUnique( const ImageCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ImageDeleter deleter( this, allocator );
      return UniqueImage( createImage( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyImage( Image image, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyImage( Image image, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getImageSubresourceLayout( Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout ) const
    {
      vkGetImageSubresourceLayout( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkImageSubresource*>( pSubresource ), reinterpret_cast<VkSubresourceLayout*>( pLayout ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    SubresourceLayout getImageSubresourceLayout( Image image, const ImageSubresource & subresource ) const
    {
      SubresourceLayout layout;
      vkGetImageSubresourceLayout( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkImageSubresource*>( &subresource ), reinterpret_cast<VkSubresourceLayout*>( &layout ) );
      return layout;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createImageView( const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView ) const
    {
      return static_cast<Result>( vkCreateImageView( m_device, reinterpret_cast<const VkImageViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkImageView*>( pView ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ImageView>::type createImageView( const ImageViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ImageView view;
      Result result = static_cast<Result>( vkCreateImageView( m_device, reinterpret_cast<const VkImageViewCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkImageView*>( &view ) ) );
      return createResultValue( result, view, "vk::Device::createImageView" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueImageView createImageViewUnique( const ImageViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ImageViewDeleter deleter( this, allocator );
      return UniqueImageView( createImageView( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyImageView( ImageView imageView, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyImageView( ImageView imageView, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createShaderModule( const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule ) const
    {
      return static_cast<Result>( vkCreateShaderModule( m_device, reinterpret_cast<const VkShaderModuleCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkShaderModule*>( pShaderModule ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ShaderModule>::type createShaderModule( const ShaderModuleCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ShaderModule shaderModule;
      Result result = static_cast<Result>( vkCreateShaderModule( m_device, reinterpret_cast<const VkShaderModuleCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkShaderModule*>( &shaderModule ) ) );
      return createResultValue( result, shaderModule, "vk::Device::createShaderModule" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueShaderModule createShaderModuleUnique( const ShaderModuleCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ShaderModuleDeleter deleter( this, allocator );
      return UniqueShaderModule( createShaderModule( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyShaderModule( ShaderModule shaderModule, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyShaderModule( m_device, static_cast<VkShaderModule>( shaderModule ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyShaderModule( ShaderModule shaderModule, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyShaderModule( m_device, static_cast<VkShaderModule>( shaderModule ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createPipelineCache( const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache ) const
    {
      return static_cast<Result>( vkCreatePipelineCache( m_device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipelineCache*>( pPipelineCache ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<PipelineCache>::type createPipelineCache( const PipelineCacheCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineCache pipelineCache;
      Result result = static_cast<Result>( vkCreatePipelineCache( m_device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipelineCache*>( &pipelineCache ) ) );
      return createResultValue( result, pipelineCache, "vk::Device::createPipelineCache" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniquePipelineCache createPipelineCacheUnique( const PipelineCacheCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineCacheDeleter deleter( this, allocator );
      return UniquePipelineCache( createPipelineCache( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyPipelineCache( PipelineCache pipelineCache, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyPipelineCache( PipelineCache pipelineCache, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getPipelineCacheData( PipelineCache pipelineCache, size_t* pDataSize, void* pData ) const
    {
      return static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), pDataSize, pData ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<uint8_t>>
    typename ResultValueType<std::vector<uint8_t,Allocator>>::type getPipelineCacheData( PipelineCache pipelineCache ) const
    {
      std::vector<uint8_t,Allocator> data;
      size_t dataSize;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), &dataSize, nullptr ) );
        if ( ( result == Result::eSuccess ) && dataSize )
        {
          data.resize( dataSize );
          result = static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), &dataSize, reinterpret_cast<void*>( data.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( dataSize <= data.size() ); 
      data.resize( dataSize ); 
      return createResultValue( result, data, "vk::Device::getPipelineCacheData" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result mergePipelineCaches( PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches ) const
    {
      return static_cast<Result>( vkMergePipelineCaches( m_device, static_cast<VkPipelineCache>( dstCache ), srcCacheCount, reinterpret_cast<const VkPipelineCache*>( pSrcCaches ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type mergePipelineCaches( PipelineCache dstCache, ArrayProxy<const PipelineCache> srcCaches ) const
    {
      Result result = static_cast<Result>( vkMergePipelineCaches( m_device, static_cast<VkPipelineCache>( dstCache ), srcCaches.size() , reinterpret_cast<const VkPipelineCache*>( srcCaches.data() ) ) );
      return createResultValue( result, "vk::Device::mergePipelineCaches" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createGraphicsPipelines( PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines ) const
    {
      return static_cast<Result>( vkCreateGraphicsPipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipeline*>( pPipelines ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<Pipeline>>
    typename ResultValueType<std::vector<Pipeline,Allocator>>::type createGraphicsPipelines( PipelineCache pipelineCache, ArrayProxy<const GraphicsPipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      std::vector<Pipeline,Allocator> pipelines( createInfos.size() );
      Result result = static_cast<Result>( vkCreateGraphicsPipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfos.size() , reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipeline*>( pipelines.data() ) ) );
      return createResultValue( result, pipelines, "vk::Device::createGraphicsPipelines" );
    }

    ResultValueType<Pipeline>::type createGraphicsPipeline( PipelineCache pipelineCache, const GraphicsPipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Pipeline pipeline;
      Result result = static_cast<Result>( vkCreateGraphicsPipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), 1 , reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipeline*>( &pipeline ) ) );
      return createResultValue( result, pipeline, "vk::Device::createGraphicsPipeline" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    template <typename Allocator = std::allocator<Pipeline>>
    std::vector<UniquePipeline> createGraphicsPipelinesUnique( PipelineCache pipelineCache, ArrayProxy<const GraphicsPipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineDeleter deleter( this, allocator );
      std::vector<Pipeline,Allocator> pipelines = createGraphicsPipelines( pipelineCache, createInfos, allocator );
      std::vector<UniquePipeline> uniquePipelines;
      uniquePipelines.reserve( pipelines.size() );
      for ( auto pipeline : pipelines )
      {
        uniquePipelines.push_back( UniquePipeline( pipeline, deleter ) );
      }
      return uniquePipelines;
    }

    UniquePipeline createGraphicsPipelineUnique( PipelineCache pipelineCache, const GraphicsPipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineDeleter deleter( this, allocator );
      return UniquePipeline( createGraphicsPipeline( pipelineCache, createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createComputePipelines( PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines ) const
    {
      return static_cast<Result>( vkCreateComputePipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipeline*>( pPipelines ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<Pipeline>>
    typename ResultValueType<std::vector<Pipeline,Allocator>>::type createComputePipelines( PipelineCache pipelineCache, ArrayProxy<const ComputePipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      std::vector<Pipeline,Allocator> pipelines( createInfos.size() );
      Result result = static_cast<Result>( vkCreateComputePipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfos.size() , reinterpret_cast<const VkComputePipelineCreateInfo*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipeline*>( pipelines.data() ) ) );
      return createResultValue( result, pipelines, "vk::Device::createComputePipelines" );
    }

    ResultValueType<Pipeline>::type createComputePipeline( PipelineCache pipelineCache, const ComputePipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Pipeline pipeline;
      Result result = static_cast<Result>( vkCreateComputePipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), 1 , reinterpret_cast<const VkComputePipelineCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipeline*>( &pipeline ) ) );
      return createResultValue( result, pipeline, "vk::Device::createComputePipeline" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    template <typename Allocator = std::allocator<Pipeline>>
    std::vector<UniquePipeline> createComputePipelinesUnique( PipelineCache pipelineCache, ArrayProxy<const ComputePipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineDeleter deleter( this, allocator );
      std::vector<Pipeline,Allocator> pipelines = createComputePipelines( pipelineCache, createInfos, allocator );
      std::vector<UniquePipeline> uniquePipelines;
      uniquePipelines.reserve( pipelines.size() );
      for ( auto pipeline : pipelines )
      {
        uniquePipelines.push_back( UniquePipeline( pipeline, deleter ) );
      }
      return uniquePipelines;
    }

    UniquePipeline createComputePipelineUnique( PipelineCache pipelineCache, const ComputePipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineDeleter deleter( this, allocator );
      return UniquePipeline( createComputePipeline( pipelineCache, createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyPipeline( Pipeline pipeline, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyPipeline( Pipeline pipeline, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createPipelineLayout( const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout ) const
    {
      return static_cast<Result>( vkCreatePipelineLayout( m_device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipelineLayout*>( pPipelineLayout ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<PipelineLayout>::type createPipelineLayout( const PipelineLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineLayout pipelineLayout;
      Result result = static_cast<Result>( vkCreatePipelineLayout( m_device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkPipelineLayout*>( &pipelineLayout ) ) );
      return createResultValue( result, pipelineLayout, "vk::Device::createPipelineLayout" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniquePipelineLayout createPipelineLayoutUnique( const PipelineLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      PipelineLayoutDeleter deleter( this, allocator );
      return UniquePipelineLayout( createPipelineLayout( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyPipelineLayout( PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyPipelineLayout( PipelineLayout pipelineLayout, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createSampler( const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler ) const
    {
      return static_cast<Result>( vkCreateSampler( m_device, reinterpret_cast<const VkSamplerCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSampler*>( pSampler ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Sampler>::type createSampler( const SamplerCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Sampler sampler;
      Result result = static_cast<Result>( vkCreateSampler( m_device, reinterpret_cast<const VkSamplerCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSampler*>( &sampler ) ) );
      return createResultValue( result, sampler, "vk::Device::createSampler" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSampler createSamplerUnique( const SamplerCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SamplerDeleter deleter( this, allocator );
      return UniqueSampler( createSampler( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroySampler( Sampler sampler, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroySampler( Sampler sampler, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDescriptorSetLayout( const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout ) const
    {
      return static_cast<Result>( vkCreateDescriptorSetLayout( m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDescriptorSetLayout*>( pSetLayout ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DescriptorSetLayout>::type createDescriptorSetLayout( const DescriptorSetLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DescriptorSetLayout setLayout;
      Result result = static_cast<Result>( vkCreateDescriptorSetLayout( m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDescriptorSetLayout*>( &setLayout ) ) );
      return createResultValue( result, setLayout, "vk::Device::createDescriptorSetLayout" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueDescriptorSetLayout createDescriptorSetLayoutUnique( const DescriptorSetLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DescriptorSetLayoutDeleter deleter( this, allocator );
      return UniqueDescriptorSetLayout( createDescriptorSetLayout( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyDescriptorSetLayout( DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDescriptorSetLayout( m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyDescriptorSetLayout( DescriptorSetLayout descriptorSetLayout, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyDescriptorSetLayout( m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDescriptorPool( const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool ) const
    {
      return static_cast<Result>( vkCreateDescriptorPool( m_device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDescriptorPool*>( pDescriptorPool ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DescriptorPool>::type createDescriptorPool( const DescriptorPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DescriptorPool descriptorPool;
      Result result = static_cast<Result>( vkCreateDescriptorPool( m_device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDescriptorPool*>( &descriptorPool ) ) );
      return createResultValue( result, descriptorPool, "vk::Device::createDescriptorPool" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueDescriptorPool createDescriptorPoolUnique( const DescriptorPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DescriptorPoolDeleter deleter( this, allocator );
      return UniqueDescriptorPool( createDescriptorPool( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyDescriptorPool( DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyDescriptorPool( DescriptorPool descriptorPool, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result resetDescriptorPool( DescriptorPool descriptorPool, DescriptorPoolResetFlags flags ) const
    {
      return static_cast<Result>( vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type resetDescriptorPool( DescriptorPool descriptorPool, DescriptorPoolResetFlags flags = DescriptorPoolResetFlags() ) const
    {
      Result result = static_cast<Result>( vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
      return createResultValue( result, "vk::Device::resetDescriptorPool" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result allocateDescriptorSets( const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets ) const
    {
      return static_cast<Result>( vkAllocateDescriptorSets( m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( pAllocateInfo ), reinterpret_cast<VkDescriptorSet*>( pDescriptorSets ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<DescriptorSet>>
    typename ResultValueType<std::vector<DescriptorSet,Allocator>>::type allocateDescriptorSets( const DescriptorSetAllocateInfo & allocateInfo ) const
    {
      std::vector<DescriptorSet,Allocator> descriptorSets( allocateInfo.descriptorSetCount );
      Result result = static_cast<Result>( vkAllocateDescriptorSets( m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( &allocateInfo ), reinterpret_cast<VkDescriptorSet*>( descriptorSets.data() ) ) );
      return createResultValue( result, descriptorSets, "vk::Device::allocateDescriptorSets" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    template <typename Allocator = std::allocator<DescriptorSet>>
    std::vector<UniqueDescriptorSet> allocateDescriptorSetsUnique( const DescriptorSetAllocateInfo & allocateInfo ) const
    {
      DescriptorSetDeleter deleter( this, allocateInfo.descriptorPool );
      std::vector<DescriptorSet,Allocator> descriptorSets = allocateDescriptorSets( allocateInfo );
      std::vector<UniqueDescriptorSet> uniqueDescriptorSets;
      uniqueDescriptorSets.reserve( descriptorSets.size() );
      for ( auto descriptorSet : descriptorSets )
      {
        uniqueDescriptorSets.push_back( UniqueDescriptorSet( descriptorSet, deleter ) );
      }
      return uniqueDescriptorSets;
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result freeDescriptorSets( DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets ) const
    {
      return static_cast<Result>( vkFreeDescriptorSets( m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>( pDescriptorSets ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type freeDescriptorSets( DescriptorPool descriptorPool, ArrayProxy<const DescriptorSet> descriptorSets ) const
    {
      Result result = static_cast<Result>( vkFreeDescriptorSets( m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSets.size() , reinterpret_cast<const VkDescriptorSet*>( descriptorSets.data() ) ) );
      return createResultValue( result, "vk::Device::freeDescriptorSets" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void updateDescriptorSets( uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies ) const
    {
      vkUpdateDescriptorSets( m_device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>( pDescriptorWrites ), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>( pDescriptorCopies ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void updateDescriptorSets( ArrayProxy<const WriteDescriptorSet> descriptorWrites, ArrayProxy<const CopyDescriptorSet> descriptorCopies ) const
    {
      vkUpdateDescriptorSets( m_device, descriptorWrites.size() , reinterpret_cast<const VkWriteDescriptorSet*>( descriptorWrites.data() ), descriptorCopies.size() , reinterpret_cast<const VkCopyDescriptorSet*>( descriptorCopies.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createFramebuffer( const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer ) const
    {
      return static_cast<Result>( vkCreateFramebuffer( m_device, reinterpret_cast<const VkFramebufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFramebuffer*>( pFramebuffer ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Framebuffer>::type createFramebuffer( const FramebufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Framebuffer framebuffer;
      Result result = static_cast<Result>( vkCreateFramebuffer( m_device, reinterpret_cast<const VkFramebufferCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkFramebuffer*>( &framebuffer ) ) );
      return createResultValue( result, framebuffer, "vk::Device::createFramebuffer" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueFramebuffer createFramebufferUnique( const FramebufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      FramebufferDeleter deleter( this, allocator );
      return UniqueFramebuffer( createFramebuffer( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyFramebuffer( Framebuffer framebuffer, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyFramebuffer( Framebuffer framebuffer, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createRenderPass( const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass ) const
    {
      return static_cast<Result>( vkCreateRenderPass( m_device, reinterpret_cast<const VkRenderPassCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkRenderPass*>( pRenderPass ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<RenderPass>::type createRenderPass( const RenderPassCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      RenderPass renderPass;
      Result result = static_cast<Result>( vkCreateRenderPass( m_device, reinterpret_cast<const VkRenderPassCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkRenderPass*>( &renderPass ) ) );
      return createResultValue( result, renderPass, "vk::Device::createRenderPass" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueRenderPass createRenderPassUnique( const RenderPassCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      RenderPassDeleter deleter( this, allocator );
      return UniqueRenderPass( createRenderPass( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyRenderPass( RenderPass renderPass, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyRenderPass( RenderPass renderPass, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getRenderAreaGranularity( RenderPass renderPass, Extent2D* pGranularity ) const
    {
      vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D*>( pGranularity ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Extent2D getRenderAreaGranularity( RenderPass renderPass ) const
    {
      Extent2D granularity;
      vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D*>( &granularity ) );
      return granularity;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createCommandPool( const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool ) const
    {
      return static_cast<Result>( vkCreateCommandPool( m_device, reinterpret_cast<const VkCommandPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkCommandPool*>( pCommandPool ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<CommandPool>::type createCommandPool( const CommandPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      CommandPool commandPool;
      Result result = static_cast<Result>( vkCreateCommandPool( m_device, reinterpret_cast<const VkCommandPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkCommandPool*>( &commandPool ) ) );
      return createResultValue( result, commandPool, "vk::Device::createCommandPool" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueCommandPool createCommandPoolUnique( const CommandPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      CommandPoolDeleter deleter( this, allocator );
      return UniqueCommandPool( createCommandPool( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyCommandPool( CommandPool commandPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyCommandPool( CommandPool commandPool, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result resetCommandPool( CommandPool commandPool, CommandPoolResetFlags flags ) const
    {
      return static_cast<Result>( vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type resetCommandPool( CommandPool commandPool, CommandPoolResetFlags flags ) const
    {
      Result result = static_cast<Result>( vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
      return createResultValue( result, "vk::Device::resetCommandPool" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result allocateCommandBuffers( const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers ) const
    {
      return static_cast<Result>( vkAllocateCommandBuffers( m_device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( pAllocateInfo ), reinterpret_cast<VkCommandBuffer*>( pCommandBuffers ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<CommandBuffer>>
    typename ResultValueType<std::vector<CommandBuffer,Allocator>>::type allocateCommandBuffers( const CommandBufferAllocateInfo & allocateInfo ) const
    {
      std::vector<CommandBuffer,Allocator> commandBuffers( allocateInfo.commandBufferCount );
      Result result = static_cast<Result>( vkAllocateCommandBuffers( m_device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( &allocateInfo ), reinterpret_cast<VkCommandBuffer*>( commandBuffers.data() ) ) );
      return createResultValue( result, commandBuffers, "vk::Device::allocateCommandBuffers" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    template <typename Allocator = std::allocator<CommandBuffer>>
    std::vector<UniqueCommandBuffer> allocateCommandBuffersUnique( const CommandBufferAllocateInfo & allocateInfo ) const
    {
      CommandBufferDeleter deleter( this, allocateInfo.commandPool );
      std::vector<CommandBuffer,Allocator> commandBuffers = allocateCommandBuffers( allocateInfo );
      std::vector<UniqueCommandBuffer> uniqueCommandBuffers;
      uniqueCommandBuffers.reserve( commandBuffers.size() );
      for ( auto commandBuffer : commandBuffers )
      {
        uniqueCommandBuffers.push_back( UniqueCommandBuffer( commandBuffer, deleter ) );
      }
      return uniqueCommandBuffers;
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void freeCommandBuffers( CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers ) const
    {
      vkFreeCommandBuffers( m_device, static_cast<VkCommandPool>( commandPool ), commandBufferCount, reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void freeCommandBuffers( CommandPool commandPool, ArrayProxy<const CommandBuffer> commandBuffers ) const
    {
      vkFreeCommandBuffers( m_device, static_cast<VkCommandPool>( commandPool ), commandBuffers.size() , reinterpret_cast<const VkCommandBuffer*>( commandBuffers.data() ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createSharedSwapchainsKHR( uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains ) const
    {
      return static_cast<Result>( vkCreateSharedSwapchainsKHR( m_device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSwapchainKHR*>( pSwapchains ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<SwapchainKHR>>
    typename ResultValueType<std::vector<SwapchainKHR,Allocator>>::type createSharedSwapchainsKHR( ArrayProxy<const SwapchainCreateInfoKHR> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      std::vector<SwapchainKHR,Allocator> swapchains( createInfos.size() );
      Result result = static_cast<Result>( vkCreateSharedSwapchainsKHR( m_device, createInfos.size() , reinterpret_cast<const VkSwapchainCreateInfoKHR*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSwapchainKHR*>( swapchains.data() ) ) );
      return createResultValue( result, swapchains, "vk::Device::createSharedSwapchainsKHR" );
    }

    ResultValueType<SwapchainKHR>::type createSharedSwapchainKHR( const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SwapchainKHR swapchain;
      Result result = static_cast<Result>( vkCreateSharedSwapchainsKHR( m_device, 1 , reinterpret_cast<const VkSwapchainCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSwapchainKHR*>( &swapchain ) ) );
      return createResultValue( result, swapchain, "vk::Device::createSharedSwapchainKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    template <typename Allocator = std::allocator<SwapchainKHR>>
    std::vector<UniqueSwapchainKHR> createSharedSwapchainsKHRUnique( ArrayProxy<const SwapchainCreateInfoKHR> createInfos, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SwapchainKHRDeleter deleter( this, allocator );
      std::vector<SwapchainKHR,Allocator> swapchainKHRs = createSharedSwapchainsKHR( createInfos, allocator );
      std::vector<UniqueSwapchainKHR> uniqueSwapchainKHRs;
      uniqueSwapchainKHRs.reserve( swapchainKHRs.size() );
      for ( auto swapchainKHR : swapchainKHRs )
      {
        uniqueSwapchainKHRs.push_back( UniqueSwapchainKHR( swapchainKHR, deleter ) );
      }
      return uniqueSwapchainKHRs;
    }

    UniqueSwapchainKHR createSharedSwapchainKHRUnique( const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SwapchainKHRDeleter deleter( this, allocator );
      return UniqueSwapchainKHR( createSharedSwapchainKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createSwapchainKHR( const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain ) const
    {
      return static_cast<Result>( vkCreateSwapchainKHR( m_device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSwapchainKHR*>( pSwapchain ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<SwapchainKHR>::type createSwapchainKHR( const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SwapchainKHR swapchain;
      Result result = static_cast<Result>( vkCreateSwapchainKHR( m_device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSwapchainKHR*>( &swapchain ) ) );
      return createResultValue( result, swapchain, "vk::Device::createSwapchainKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSwapchainKHR createSwapchainKHRUnique( const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SwapchainKHRDeleter deleter( this, allocator );
      return UniqueSwapchainKHR( createSwapchainKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroySwapchainKHR( SwapchainKHR swapchain, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySwapchainKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroySwapchainKHR( SwapchainKHR swapchain, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroySwapchainKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSwapchainImagesKHR( SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages ) const
    {
      return static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), pSwapchainImageCount, reinterpret_cast<VkImage*>( pSwapchainImages ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<Image>>
    typename ResultValueType<std::vector<Image,Allocator>>::type getSwapchainImagesKHR( SwapchainKHR swapchain ) const
    {
      std::vector<Image,Allocator> swapchainImages;
      uint32_t swapchainImageCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && swapchainImageCount )
        {
          swapchainImages.resize( swapchainImageCount );
          result = static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, reinterpret_cast<VkImage*>( swapchainImages.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( swapchainImageCount <= swapchainImages.size() ); 
      swapchainImages.resize( swapchainImageCount ); 
      return createResultValue( result, swapchainImages, "vk::Device::getSwapchainImagesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result acquireNextImageKHR( SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex ) const
    {
      return static_cast<Result>( vkAcquireNextImageKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), pImageIndex ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValue<uint32_t> acquireNextImageKHR( SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence ) const
    {
      uint32_t imageIndex;
      Result result = static_cast<Result>( vkAcquireNextImageKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), &imageIndex ) );
      return createResultValue( result, imageIndex, "vk::Device::acquireNextImageKHR", { Result::eSuccess, Result::eTimeout, Result::eNotReady, Result::eSuboptimalKHR } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result debugMarkerSetObjectNameEXT( DebugMarkerObjectNameInfoEXT* pNameInfo ) const
    {
      return static_cast<Result>( vkDebugMarkerSetObjectNameEXT( m_device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT*>( pNameInfo ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DebugMarkerObjectNameInfoEXT>::type debugMarkerSetObjectNameEXT() const
    {
      DebugMarkerObjectNameInfoEXT nameInfo;
      Result result = static_cast<Result>( vkDebugMarkerSetObjectNameEXT( m_device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT*>( &nameInfo ) ) );
      return createResultValue( result, nameInfo, "vk::Device::debugMarkerSetObjectNameEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result debugMarkerSetObjectTagEXT( DebugMarkerObjectTagInfoEXT* pTagInfo ) const
    {
      return static_cast<Result>( vkDebugMarkerSetObjectTagEXT( m_device, reinterpret_cast<VkDebugMarkerObjectTagInfoEXT*>( pTagInfo ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DebugMarkerObjectTagInfoEXT>::type debugMarkerSetObjectTagEXT() const
    {
      DebugMarkerObjectTagInfoEXT tagInfo;
      Result result = static_cast<Result>( vkDebugMarkerSetObjectTagEXT( m_device, reinterpret_cast<VkDebugMarkerObjectTagInfoEXT*>( &tagInfo ) ) );
      return createResultValue( result, tagInfo, "vk::Device::debugMarkerSetObjectTagEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
    Result getMemoryWin32HandleNV( DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle ) const
    {
      return static_cast<Result>( vkGetMemoryWin32HandleNV( m_device, static_cast<VkDeviceMemory>( memory ), static_cast<VkExternalMemoryHandleTypeFlagsNV>( handleType ), pHandle ) );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WIN32_KHR
    ResultValueType<HANDLE>::type getMemoryWin32HandleNV( DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType ) const
    {
      HANDLE handle;
      Result result = static_cast<Result>( vkGetMemoryWin32HandleNV( m_device, static_cast<VkDeviceMemory>( memory ), static_cast<VkExternalMemoryHandleTypeFlagsNV>( handleType ), &handle ) );
      return createResultValue( result, handle, "vk::Device::getMemoryWin32HandleNV" );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createIndirectCommandsLayoutNVX( const IndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, IndirectCommandsLayoutNVX* pIndirectCommandsLayout ) const
    {
      return static_cast<Result>( vkCreateIndirectCommandsLayoutNVX( m_device, reinterpret_cast<const VkIndirectCommandsLayoutCreateInfoNVX*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkIndirectCommandsLayoutNVX*>( pIndirectCommandsLayout ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<IndirectCommandsLayoutNVX>::type createIndirectCommandsLayoutNVX( const IndirectCommandsLayoutCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      IndirectCommandsLayoutNVX indirectCommandsLayout;
      Result result = static_cast<Result>( vkCreateIndirectCommandsLayoutNVX( m_device, reinterpret_cast<const VkIndirectCommandsLayoutCreateInfoNVX*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkIndirectCommandsLayoutNVX*>( &indirectCommandsLayout ) ) );
      return createResultValue( result, indirectCommandsLayout, "vk::Device::createIndirectCommandsLayoutNVX" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueIndirectCommandsLayoutNVX createIndirectCommandsLayoutNVXUnique( const IndirectCommandsLayoutCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      IndirectCommandsLayoutNVXDeleter deleter( this, allocator );
      return UniqueIndirectCommandsLayoutNVX( createIndirectCommandsLayoutNVX( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyIndirectCommandsLayoutNVX( IndirectCommandsLayoutNVX indirectCommandsLayout, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyIndirectCommandsLayoutNVX( m_device, static_cast<VkIndirectCommandsLayoutNVX>( indirectCommandsLayout ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyIndirectCommandsLayoutNVX( IndirectCommandsLayoutNVX indirectCommandsLayout, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyIndirectCommandsLayoutNVX( m_device, static_cast<VkIndirectCommandsLayoutNVX>( indirectCommandsLayout ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createObjectTableNVX( const ObjectTableCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, ObjectTableNVX* pObjectTable ) const
    {
      return static_cast<Result>( vkCreateObjectTableNVX( m_device, reinterpret_cast<const VkObjectTableCreateInfoNVX*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkObjectTableNVX*>( pObjectTable ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ObjectTableNVX>::type createObjectTableNVX( const ObjectTableCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ObjectTableNVX objectTable;
      Result result = static_cast<Result>( vkCreateObjectTableNVX( m_device, reinterpret_cast<const VkObjectTableCreateInfoNVX*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkObjectTableNVX*>( &objectTable ) ) );
      return createResultValue( result, objectTable, "vk::Device::createObjectTableNVX" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueObjectTableNVX createObjectTableNVXUnique( const ObjectTableCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      ObjectTableNVXDeleter deleter( this, allocator );
      return UniqueObjectTableNVX( createObjectTableNVX( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyObjectTableNVX( ObjectTableNVX objectTable, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyObjectTableNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyObjectTableNVX( ObjectTableNVX objectTable, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyObjectTableNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result registerObjectsNVX( ObjectTableNVX objectTable, uint32_t objectCount, const ObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices ) const
    {
      return static_cast<Result>( vkRegisterObjectsNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), objectCount, reinterpret_cast<const VkObjectTableEntryNVX* const*>( ppObjectTableEntries ), pObjectIndices ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type registerObjectsNVX( ObjectTableNVX objectTable, ArrayProxy<const ObjectTableEntryNVX* const> pObjectTableEntries, ArrayProxy<const uint32_t> objectIndices ) const
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      assert( pObjectTableEntries.size() == objectIndices.size() );
#else
      if ( pObjectTableEntries.size() != objectIndices.size() )
      {
        throw std::logic_error( "vk::Device::registerObjectsNVX: pObjectTableEntries.size() != objectIndices.size()" );
      }
#endif  // VULKAN_HPP_NO_EXCEPTIONS
      Result result = static_cast<Result>( vkRegisterObjectsNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), pObjectTableEntries.size() , reinterpret_cast<const VkObjectTableEntryNVX* const*>( pObjectTableEntries.data() ), objectIndices.data() ) );
      return createResultValue( result, "vk::Device::registerObjectsNVX" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result unregisterObjectsNVX( ObjectTableNVX objectTable, uint32_t objectCount, const ObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices ) const
    {
      return static_cast<Result>( vkUnregisterObjectsNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), objectCount, reinterpret_cast<const VkObjectEntryTypeNVX*>( pObjectEntryTypes ), pObjectIndices ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type unregisterObjectsNVX( ObjectTableNVX objectTable, ArrayProxy<const ObjectEntryTypeNVX> objectEntryTypes, ArrayProxy<const uint32_t> objectIndices ) const
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      assert( objectEntryTypes.size() == objectIndices.size() );
#else
      if ( objectEntryTypes.size() != objectIndices.size() )
      {
        throw std::logic_error( "vk::Device::unregisterObjectsNVX: objectEntryTypes.size() != objectIndices.size()" );
      }
#endif  // VULKAN_HPP_NO_EXCEPTIONS
      Result result = static_cast<Result>( vkUnregisterObjectsNVX( m_device, static_cast<VkObjectTableNVX>( objectTable ), objectEntryTypes.size() , reinterpret_cast<const VkObjectEntryTypeNVX*>( objectEntryTypes.data() ), objectIndices.data() ) );
      return createResultValue( result, "vk::Device::unregisterObjectsNVX" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void trimCommandPoolKHR( CommandPool commandPool, CommandPoolTrimFlagsKHR flags ) const
    {
      vkTrimCommandPoolKHR( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolTrimFlagsKHR>( flags ) );
    }


    Result displayPowerControlEXT( DisplayKHR display, const DisplayPowerInfoEXT* pDisplayPowerInfo ) const
    {
      return static_cast<Result>( vkDisplayPowerControlEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayPowerInfoEXT*>( pDisplayPowerInfo ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type displayPowerControlEXT( DisplayKHR display, const DisplayPowerInfoEXT & displayPowerInfo ) const
    {
      Result result = static_cast<Result>( vkDisplayPowerControlEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayPowerInfoEXT*>( &displayPowerInfo ) ) );
      return createResultValue( result, "vk::Device::displayPowerControlEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result registerEventEXT( const DeviceEventInfoEXT* pDeviceEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence ) const
    {
      return static_cast<Result>( vkRegisterDeviceEventEXT( m_device, reinterpret_cast<const VkDeviceEventInfoEXT*>( pDeviceEventInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFence*>( pFence ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Fence>::type registerEventEXT( const DeviceEventInfoEXT & deviceEventInfo, const AllocationCallbacks & allocator ) const
    {
      Fence fence;
      Result result = static_cast<Result>( vkRegisterDeviceEventEXT( m_device, reinterpret_cast<const VkDeviceEventInfoEXT*>( &deviceEventInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkFence*>( &fence ) ) );
      return createResultValue( result, fence, "vk::Device::registerEventEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result registerDisplayEventEXT( DisplayKHR display, const DisplayEventInfoEXT* pDisplayEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence ) const
    {
      return static_cast<Result>( vkRegisterDisplayEventEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayEventInfoEXT*>( pDisplayEventInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFence*>( pFence ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Fence>::type registerDisplayEventEXT( DisplayKHR display, const DisplayEventInfoEXT & displayEventInfo, const AllocationCallbacks & allocator ) const
    {
      Fence fence;
      Result result = static_cast<Result>( vkRegisterDisplayEventEXT( m_device, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayEventInfoEXT*>( &displayEventInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkFence*>( &fence ) ) );
      return createResultValue( result, fence, "vk::Device::registerDisplayEventEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSwapchainCounterEXT( SwapchainKHR swapchain, SurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue ) const
    {
      return static_cast<Result>( vkGetSwapchainCounterEXT( m_device, static_cast<VkSwapchainKHR>( swapchain ), static_cast<VkSurfaceCounterFlagBitsEXT>( counter ), pCounterValue ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValue<uint64_t> getSwapchainCounterEXT( SwapchainKHR swapchain, SurfaceCounterFlagBitsEXT counter ) const
    {
      uint64_t counterValue;
      Result result = static_cast<Result>( vkGetSwapchainCounterEXT( m_device, static_cast<VkSwapchainKHR>( swapchain ), static_cast<VkSurfaceCounterFlagBitsEXT>( counter ), &counterValue ) );
      return createResultValue( result, counterValue, "vk::Device::getSwapchainCounterEXT", { Result::eSuccess, Result::eErrorDeviceLost, Result::eErrorOutOfDateKHR } );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkDevice() const
    {
      return m_device;
    }

    explicit operator bool() const
    {
      return m_device != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_device == VK_NULL_HANDLE;
    }

  private:
    VkDevice m_device;
  };
  static_assert( sizeof( Device ) == sizeof( VkDevice ), "handle and wrapper have different size!" );

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_INLINE void BufferDeleter::operator()( Buffer buffer )
  {
    m_device->destroyBuffer( buffer, m_allocator );
  }

  VULKAN_HPP_INLINE void BufferViewDeleter::operator()( BufferView bufferView )
  {
    m_device->destroyBufferView( bufferView, m_allocator );
  }

  VULKAN_HPP_INLINE void CommandBufferDeleter::operator()( CommandBuffer commandBuffer )
  {
    m_device->freeCommandBuffers( m_commandPool, commandBuffer );
  }

  VULKAN_HPP_INLINE void CommandPoolDeleter::operator()( CommandPool commandPool )
  {
    m_device->destroyCommandPool( commandPool, m_allocator );
  }

  VULKAN_HPP_INLINE void DescriptorPoolDeleter::operator()( DescriptorPool descriptorPool )
  {
    m_device->destroyDescriptorPool( descriptorPool, m_allocator );
  }

  VULKAN_HPP_INLINE void DescriptorSetDeleter::operator()( DescriptorSet descriptorSet )
  {
    m_device->freeDescriptorSets( m_descriptorPool, descriptorSet );
  }

  VULKAN_HPP_INLINE void DescriptorSetLayoutDeleter::operator()( DescriptorSetLayout descriptorSetLayout )
  {
    m_device->destroyDescriptorSetLayout( descriptorSetLayout, m_allocator );
  }

  VULKAN_HPP_INLINE void DeviceMemoryDeleter::operator()( DeviceMemory deviceMemory )
  {
    m_device->freeMemory( deviceMemory, m_allocator );
  }

  VULKAN_HPP_INLINE void EventDeleter::operator()( Event event )
  {
    m_device->destroyEvent( event, m_allocator );
  }

  VULKAN_HPP_INLINE void FenceDeleter::operator()( Fence fence )
  {
    m_device->destroyFence( fence, m_allocator );
  }

  VULKAN_HPP_INLINE void FramebufferDeleter::operator()( Framebuffer framebuffer )
  {
    m_device->destroyFramebuffer( framebuffer, m_allocator );
  }

  VULKAN_HPP_INLINE void ImageDeleter::operator()( Image image )
  {
    m_device->destroyImage( image, m_allocator );
  }

  VULKAN_HPP_INLINE void ImageViewDeleter::operator()( ImageView imageView )
  {
    m_device->destroyImageView( imageView, m_allocator );
  }

  VULKAN_HPP_INLINE void IndirectCommandsLayoutNVXDeleter::operator()( IndirectCommandsLayoutNVX indirectCommandsLayoutNVX )
  {
    m_device->destroyIndirectCommandsLayoutNVX( indirectCommandsLayoutNVX, m_allocator );
  }

  VULKAN_HPP_INLINE void ObjectTableNVXDeleter::operator()( ObjectTableNVX objectTableNVX )
  {
    m_device->destroyObjectTableNVX( objectTableNVX, m_allocator );
  }

  VULKAN_HPP_INLINE void PipelineDeleter::operator()( Pipeline pipeline )
  {
    m_device->destroyPipeline( pipeline, m_allocator );
  }

  VULKAN_HPP_INLINE void PipelineCacheDeleter::operator()( PipelineCache pipelineCache )
  {
    m_device->destroyPipelineCache( pipelineCache, m_allocator );
  }

  VULKAN_HPP_INLINE void PipelineLayoutDeleter::operator()( PipelineLayout pipelineLayout )
  {
    m_device->destroyPipelineLayout( pipelineLayout, m_allocator );
  }

  VULKAN_HPP_INLINE void QueryPoolDeleter::operator()( QueryPool queryPool )
  {
    m_device->destroyQueryPool( queryPool, m_allocator );
  }

  VULKAN_HPP_INLINE void RenderPassDeleter::operator()( RenderPass renderPass )
  {
    m_device->destroyRenderPass( renderPass, m_allocator );
  }

  VULKAN_HPP_INLINE void SamplerDeleter::operator()( Sampler sampler )
  {
    m_device->destroySampler( sampler, m_allocator );
  }

  VULKAN_HPP_INLINE void SemaphoreDeleter::operator()( Semaphore semaphore )
  {
    m_device->destroySemaphore( semaphore, m_allocator );
  }

  VULKAN_HPP_INLINE void ShaderModuleDeleter::operator()( ShaderModule shaderModule )
  {
    m_device->destroyShaderModule( shaderModule, m_allocator );
  }

  VULKAN_HPP_INLINE void SwapchainKHRDeleter::operator()( SwapchainKHR swapchainKHR )
  {
    m_device->destroySwapchainKHR( swapchainKHR, m_allocator );
  }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  class PhysicalDevice
  {
  public:
    PhysicalDevice()
      : m_physicalDevice(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    PhysicalDevice(VkPhysicalDevice physicalDevice)
       : m_physicalDevice(physicalDevice)
    {}

    PhysicalDevice& operator=(VkPhysicalDevice physicalDevice)
    {
      m_physicalDevice = physicalDevice;
      return *this;
    }
#endif

    bool operator==(PhysicalDevice const &rhs) const
    {
      return m_physicalDevice == rhs.m_physicalDevice;
    }

    bool operator!=(PhysicalDevice const &rhs) const
    {
      return m_physicalDevice != rhs.m_physicalDevice;
    }

    bool operator<(PhysicalDevice const &rhs) const
    {
      return m_physicalDevice < rhs.m_physicalDevice;
    }

    void getProperties( PhysicalDeviceProperties* pProperties ) const
    {
      vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( pProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceProperties getProperties() const
    {
      PhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( &properties ) );
      return properties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getQueueFamilyProperties( uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties ) const
    {
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( pQueueFamilyProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<QueueFamilyProperties>>
    std::vector<QueueFamilyProperties,Allocator> getQueueFamilyProperties() const
    {
      std::vector<QueueFamilyProperties,Allocator> queueFamilyProperties;
      uint32_t queueFamilyPropertyCount;
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( queueFamilyProperties.data() ) );
      return queueFamilyProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getMemoryProperties( PhysicalDeviceMemoryProperties* pMemoryProperties ) const
    {
      vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( pMemoryProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceMemoryProperties getMemoryProperties() const
    {
      PhysicalDeviceMemoryProperties memoryProperties;
      vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( &memoryProperties ) );
      return memoryProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getFeatures( PhysicalDeviceFeatures* pFeatures ) const
    {
      vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( pFeatures ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceFeatures getFeatures() const
    {
      PhysicalDeviceFeatures features;
      vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( &features ) );
      return features;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getFormatProperties( Format format, FormatProperties* pFormatProperties ) const
    {
      vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( pFormatProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    FormatProperties getFormatProperties( Format format ) const
    {
      FormatProperties formatProperties;
      vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( &formatProperties ) );
      return formatProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getImageFormatProperties( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( pImageFormatProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ImageFormatProperties>::type getImageFormatProperties( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags ) const
    {
      ImageFormatProperties imageFormatProperties;
      Result result = static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( &imageFormatProperties ) ) );
      return createResultValue( result, imageFormatProperties, "vk::PhysicalDevice::getImageFormatProperties" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDevice( const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice ) const
    {
      return static_cast<Result>( vkCreateDevice( m_physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDevice*>( pDevice ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Device>::type createDevice( const DeviceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      Device device;
      Result result = static_cast<Result>( vkCreateDevice( m_physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDevice*>( &device ) ) );
      return createResultValue( result, device, "vk::PhysicalDevice::createDevice" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueDevice createDeviceUnique( const DeviceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DeviceDeleter deleter( allocator );
      return UniqueDevice( createDevice( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result enumerateDeviceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties ) const
    {
      return static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<LayerProperties>>
    typename ResultValueType<std::vector<LayerProperties,Allocator>>::type enumerateDeviceLayerProperties() const
    {
      std::vector<LayerProperties,Allocator> properties;
      uint32_t propertyCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( propertyCount <= properties.size() ); 
      properties.resize( propertyCount ); 
      return createResultValue( result, properties, "vk::PhysicalDevice::enumerateDeviceLayerProperties" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result enumerateDeviceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties ) const
    {
      return static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<ExtensionProperties>>
    typename ResultValueType<std::vector<ExtensionProperties,Allocator>>::type enumerateDeviceExtensionProperties( Optional<const std::string> layerName = nullptr ) const
    {
      std::vector<ExtensionProperties,Allocator> properties;
      uint32_t propertyCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName ? layerName->c_str() : nullptr, &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( propertyCount <= properties.size() ); 
      properties.resize( propertyCount ); 
      return createResultValue( result, properties, "vk::PhysicalDevice::enumerateDeviceExtensionProperties" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getSparseImageFormatProperties( Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties ) const
    {
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), pPropertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( pProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<SparseImageFormatProperties>>
    std::vector<SparseImageFormatProperties,Allocator> getSparseImageFormatProperties( Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling ) const
    {
      std::vector<SparseImageFormatProperties,Allocator> properties;
      uint32_t propertyCount;
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), &propertyCount, nullptr );
      properties.resize( propertyCount );
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), &propertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( properties.data() ) );
      return properties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getDisplayPropertiesKHR( uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( pProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<DisplayPropertiesKHR>>
    typename ResultValueType<std::vector<DisplayPropertiesKHR,Allocator>>::type getDisplayPropertiesKHR() const
    {
      std::vector<DisplayPropertiesKHR,Allocator> properties;
      uint32_t propertyCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( properties.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( propertyCount <= properties.size() ); 
      properties.resize( propertyCount ); 
      return createResultValue( result, properties, "vk::PhysicalDevice::getDisplayPropertiesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getDisplayPlanePropertiesKHR( uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( pProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<DisplayPlanePropertiesKHR>>
    typename ResultValueType<std::vector<DisplayPlanePropertiesKHR,Allocator>>::type getDisplayPlanePropertiesKHR() const
    {
      std::vector<DisplayPlanePropertiesKHR,Allocator> properties;
      uint32_t propertyCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( properties.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( propertyCount <= properties.size() ); 
      properties.resize( propertyCount ); 
      return createResultValue( result, properties, "vk::PhysicalDevice::getDisplayPlanePropertiesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays ) const
    {
      return static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, pDisplayCount, reinterpret_cast<VkDisplayKHR*>( pDisplays ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<DisplayKHR>>
    typename ResultValueType<std::vector<DisplayKHR,Allocator>>::type getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex ) const
    {
      std::vector<DisplayKHR,Allocator> displays;
      uint32_t displayCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && displayCount )
        {
          displays.resize( displayCount );
          result = static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR*>( displays.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( displayCount <= displays.size() ); 
      displays.resize( displayCount ); 
      return createResultValue( result, displays, "vk::PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getDisplayModePropertiesKHR( DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), pPropertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( pProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<DisplayModePropertiesKHR>>
    typename ResultValueType<std::vector<DisplayModePropertiesKHR,Allocator>>::type getDisplayModePropertiesKHR( DisplayKHR display ) const
    {
      std::vector<DisplayModePropertiesKHR,Allocator> properties;
      uint32_t propertyCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && propertyCount )
        {
          properties.resize( propertyCount );
          result = static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( properties.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( propertyCount <= properties.size() ); 
      properties.resize( propertyCount ); 
      return createResultValue( result, properties, "vk::PhysicalDevice::getDisplayModePropertiesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDisplayModeKHR( DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode ) const
    {
      return static_cast<Result>( vkCreateDisplayModeKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDisplayModeKHR*>( pMode ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DisplayModeKHR>::type createDisplayModeKHR( DisplayKHR display, const DisplayModeCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DisplayModeKHR mode;
      Result result = static_cast<Result>( vkCreateDisplayModeKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDisplayModeKHR*>( &mode ) ) );
      return createResultValue( result, mode, "vk::PhysicalDevice::createDisplayModeKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getDisplayPlaneCapabilitiesKHR( DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities ) const
    {
      return static_cast<Result>( vkGetDisplayPlaneCapabilitiesKHR( m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( pCapabilities ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DisplayPlaneCapabilitiesKHR>::type getDisplayPlaneCapabilitiesKHR( DisplayModeKHR mode, uint32_t planeIndex ) const
    {
      DisplayPlaneCapabilitiesKHR capabilities;
      Result result = static_cast<Result>( vkGetDisplayPlaneCapabilitiesKHR( m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( &capabilities ) ) );
      return createResultValue( result, capabilities, "vk::PhysicalDevice::getDisplayPlaneCapabilitiesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_MIR_KHR
    Bool32 getMirPresentationSupportKHR( uint32_t queueFamilyIndex, MirConnection* connection ) const
    {
      return vkGetPhysicalDeviceMirPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, connection );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_MIR_KHR
    Bool32 getMirPresentationSupportKHR( uint32_t queueFamilyIndex, MirConnection & connection ) const
    {
      return vkGetPhysicalDeviceMirPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &connection );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSurfaceSupportKHR( uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), pSupported ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<Bool32>::type getSurfaceSupportKHR( uint32_t queueFamilyIndex, SurfaceKHR surface ) const
    {
      Bool32 supported;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), &supported ) );
      return createResultValue( result, supported, "vk::PhysicalDevice::getSurfaceSupportKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSurfaceCapabilitiesKHR( SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR*>( pSurfaceCapabilities ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<SurfaceCapabilitiesKHR>::type getSurfaceCapabilitiesKHR( SurfaceKHR surface ) const
    {
      SurfaceCapabilitiesKHR surfaceCapabilities;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR*>( &surfaceCapabilities ) ) );
      return createResultValue( result, surfaceCapabilities, "vk::PhysicalDevice::getSurfaceCapabilitiesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSurfaceFormatsKHR( SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pSurfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( pSurfaceFormats ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<SurfaceFormatKHR>>
    typename ResultValueType<std::vector<SurfaceFormatKHR,Allocator>>::type getSurfaceFormatsKHR( SurfaceKHR surface ) const
    {
      std::vector<SurfaceFormatKHR,Allocator> surfaceFormats;
      uint32_t surfaceFormatCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && surfaceFormatCount )
        {
          surfaceFormats.resize( surfaceFormatCount );
          result = static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( surfaceFormats.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( surfaceFormatCount <= surfaceFormats.size() ); 
      surfaceFormats.resize( surfaceFormatCount ); 
      return createResultValue( result, surfaceFormats, "vk::PhysicalDevice::getSurfaceFormatsKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSurfacePresentModesKHR( SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pPresentModeCount, reinterpret_cast<VkPresentModeKHR*>( pPresentModes ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<PresentModeKHR>>
    typename ResultValueType<std::vector<PresentModeKHR,Allocator>>::type getSurfacePresentModesKHR( SurfaceKHR surface ) const
    {
      std::vector<PresentModeKHR,Allocator> presentModes;
      uint32_t presentModeCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && presentModeCount )
        {
          presentModes.resize( presentModeCount );
          result = static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, reinterpret_cast<VkPresentModeKHR*>( presentModes.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( presentModeCount <= presentModes.size() ); 
      presentModes.resize( presentModeCount ); 
      return createResultValue( result, presentModes, "vk::PhysicalDevice::getSurfacePresentModesKHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Bool32 getWaylandPresentationSupportKHR( uint32_t queueFamilyIndex, struct wl_display* display ) const
    {
      return vkGetPhysicalDeviceWaylandPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, display );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Bool32 getWaylandPresentationSupportKHR( uint32_t queueFamilyIndex, struct wl_display & display ) const
    {
      return vkGetPhysicalDeviceWaylandPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &display );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
    Bool32 getWin32PresentationSupportKHR( uint32_t queueFamilyIndex ) const
    {
      return vkGetPhysicalDeviceWin32PresentationSupportKHR( m_physicalDevice, queueFamilyIndex );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/


#ifdef VK_USE_PLATFORM_XLIB_KHR
    Bool32 getXlibPresentationSupportKHR( uint32_t queueFamilyIndex, Display* dpy, VisualID visualID ) const
    {
      return vkGetPhysicalDeviceXlibPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, dpy, visualID );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_KHR
    Bool32 getXlibPresentationSupportKHR( uint32_t queueFamilyIndex, Display & dpy, VisualID visualID ) const
    {
      return vkGetPhysicalDeviceXlibPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &dpy, visualID );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XCB_KHR
    Bool32 getXcbPresentationSupportKHR( uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id ) const
    {
      return vkGetPhysicalDeviceXcbPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, connection, visual_id );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XCB_KHR
    Bool32 getXcbPresentationSupportKHR( uint32_t queueFamilyIndex, xcb_connection_t & connection, xcb_visualid_t visual_id ) const
    {
      return vkGetPhysicalDeviceXcbPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &connection, visual_id );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getExternalImageFormatPropertiesNV( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ExternalMemoryHandleTypeFlagsNV externalHandleType, ExternalImageFormatPropertiesNV* pExternalImageFormatProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceExternalImageFormatPropertiesNV( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), static_cast<VkExternalMemoryHandleTypeFlagsNV>( externalHandleType ), reinterpret_cast<VkExternalImageFormatPropertiesNV*>( pExternalImageFormatProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ExternalImageFormatPropertiesNV>::type getExternalImageFormatPropertiesNV( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ExternalMemoryHandleTypeFlagsNV externalHandleType ) const
    {
      ExternalImageFormatPropertiesNV externalImageFormatProperties;
      Result result = static_cast<Result>( vkGetPhysicalDeviceExternalImageFormatPropertiesNV( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), static_cast<VkExternalMemoryHandleTypeFlagsNV>( externalHandleType ), reinterpret_cast<VkExternalImageFormatPropertiesNV*>( &externalImageFormatProperties ) ) );
      return createResultValue( result, externalImageFormatProperties, "vk::PhysicalDevice::getExternalImageFormatPropertiesNV" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getGeneratedCommandsPropertiesNVX( DeviceGeneratedCommandsFeaturesNVX* pFeatures, DeviceGeneratedCommandsLimitsNVX* pLimits ) const
    {
      vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX( m_physicalDevice, reinterpret_cast<VkDeviceGeneratedCommandsFeaturesNVX*>( pFeatures ), reinterpret_cast<VkDeviceGeneratedCommandsLimitsNVX*>( pLimits ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void getGeneratedCommandsPropertiesNVX( DeviceGeneratedCommandsFeaturesNVX & features, DeviceGeneratedCommandsLimitsNVX & limits ) const
    {
      vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX( m_physicalDevice, reinterpret_cast<VkDeviceGeneratedCommandsFeaturesNVX*>( &features ), reinterpret_cast<VkDeviceGeneratedCommandsLimitsNVX*>( &limits ) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getFeatures2KHR( PhysicalDeviceFeatures2KHR* pFeatures ) const
    {
      vkGetPhysicalDeviceFeatures2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures2KHR*>( pFeatures ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceFeatures2KHR getFeatures2KHR() const
    {
      PhysicalDeviceFeatures2KHR features;
      vkGetPhysicalDeviceFeatures2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures2KHR*>( &features ) );
      return features;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getProperties2KHR( PhysicalDeviceProperties2KHR* pProperties ) const
    {
      vkGetPhysicalDeviceProperties2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties2KHR*>( pProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceProperties2KHR getProperties2KHR() const
    {
      PhysicalDeviceProperties2KHR properties;
      vkGetPhysicalDeviceProperties2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties2KHR*>( &properties ) );
      return properties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getFormatProperties2KHR( Format format, FormatProperties2KHR* pFormatProperties ) const
    {
      vkGetPhysicalDeviceFormatProperties2KHR( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2KHR*>( pFormatProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    FormatProperties2KHR getFormatProperties2KHR( Format format ) const
    {
      FormatProperties2KHR formatProperties;
      vkGetPhysicalDeviceFormatProperties2KHR( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties2KHR*>( &formatProperties ) );
      return formatProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getImageFormatProperties2KHR( const PhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo, ImageFormatProperties2KHR* pImageFormatProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties2KHR( m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2KHR*>( pImageFormatInfo ), reinterpret_cast<VkImageFormatProperties2KHR*>( pImageFormatProperties ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<ImageFormatProperties2KHR>::type getImageFormatProperties2KHR( const PhysicalDeviceImageFormatInfo2KHR & imageFormatInfo ) const
    {
      ImageFormatProperties2KHR imageFormatProperties;
      Result result = static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties2KHR( m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2KHR*>( &imageFormatInfo ), reinterpret_cast<VkImageFormatProperties2KHR*>( &imageFormatProperties ) ) );
      return createResultValue( result, imageFormatProperties, "vk::PhysicalDevice::getImageFormatProperties2KHR" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getQueueFamilyProperties2KHR( uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties2KHR* pQueueFamilyProperties ) const
    {
      vkGetPhysicalDeviceQueueFamilyProperties2KHR( m_physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2KHR*>( pQueueFamilyProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<QueueFamilyProperties2KHR>>
    std::vector<QueueFamilyProperties2KHR,Allocator> getQueueFamilyProperties2KHR() const
    {
      std::vector<QueueFamilyProperties2KHR,Allocator> queueFamilyProperties;
      uint32_t queueFamilyPropertyCount;
      vkGetPhysicalDeviceQueueFamilyProperties2KHR( m_physicalDevice, &queueFamilyPropertyCount, nullptr );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      vkGetPhysicalDeviceQueueFamilyProperties2KHR( m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties2KHR*>( queueFamilyProperties.data() ) );
      return queueFamilyProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getMemoryProperties2KHR( PhysicalDeviceMemoryProperties2KHR* pMemoryProperties ) const
    {
      vkGetPhysicalDeviceMemoryProperties2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties2KHR*>( pMemoryProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PhysicalDeviceMemoryProperties2KHR getMemoryProperties2KHR() const
    {
      PhysicalDeviceMemoryProperties2KHR memoryProperties;
      vkGetPhysicalDeviceMemoryProperties2KHR( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties2KHR*>( &memoryProperties ) );
      return memoryProperties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void getSparseImageFormatProperties2KHR( const PhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo, uint32_t* pPropertyCount, SparseImageFormatProperties2KHR* pProperties ) const
    {
      vkGetPhysicalDeviceSparseImageFormatProperties2KHR( m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSparseImageFormatInfo2KHR*>( pFormatInfo ), pPropertyCount, reinterpret_cast<VkSparseImageFormatProperties2KHR*>( pProperties ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<SparseImageFormatProperties2KHR>>
    std::vector<SparseImageFormatProperties2KHR,Allocator> getSparseImageFormatProperties2KHR( const PhysicalDeviceSparseImageFormatInfo2KHR & formatInfo ) const
    {
      std::vector<SparseImageFormatProperties2KHR,Allocator> properties;
      uint32_t propertyCount;
      vkGetPhysicalDeviceSparseImageFormatProperties2KHR( m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSparseImageFormatInfo2KHR*>( &formatInfo ), &propertyCount, nullptr );
      properties.resize( propertyCount );
      vkGetPhysicalDeviceSparseImageFormatProperties2KHR( m_physicalDevice, reinterpret_cast<const VkPhysicalDeviceSparseImageFormatInfo2KHR*>( &formatInfo ), &propertyCount, reinterpret_cast<VkSparseImageFormatProperties2KHR*>( properties.data() ) );
      return properties;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    Result releaseDisplayEXT( DisplayKHR display ) const
    {
      return static_cast<Result>( vkReleaseDisplayEXT( m_physicalDevice, static_cast<VkDisplayKHR>( display ) ) );
    }
#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<void>::type releaseDisplayEXT( DisplayKHR display ) const
    {
      Result result = static_cast<Result>( vkReleaseDisplayEXT( m_physicalDevice, static_cast<VkDisplayKHR>( display ) ) );
      return createResultValue( result, "vk::PhysicalDevice::releaseDisplayEXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    Result acquireXlibDisplayEXT( Display* dpy, DisplayKHR display ) const
    {
      return static_cast<Result>( vkAcquireXlibDisplayEXT( m_physicalDevice, dpy, static_cast<VkDisplayKHR>( display ) ) );
    }
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    ResultValueType<Display>::type acquireXlibDisplayEXT( DisplayKHR display ) const
    {
      Display dpy;
      Result result = static_cast<Result>( vkAcquireXlibDisplayEXT( m_physicalDevice, &dpy, static_cast<VkDisplayKHR>( display ) ) );
      return createResultValue( result, dpy, "vk::PhysicalDevice::acquireXlibDisplayEXT" );
    }
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    Result getRandROutputDisplayEXT( Display* dpy, RROutput rrOutput, DisplayKHR* pDisplay ) const
    {
      return static_cast<Result>( vkGetRandROutputDisplayEXT( m_physicalDevice, dpy, rrOutput, reinterpret_cast<VkDisplayKHR*>( pDisplay ) ) );
    }
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    ResultValueType<void>::type getRandROutputDisplayEXT( Display & dpy, RROutput rrOutput, DisplayKHR & display ) const
    {
      Result result = static_cast<Result>( vkGetRandROutputDisplayEXT( m_physicalDevice, &dpy, rrOutput, reinterpret_cast<VkDisplayKHR*>( &display ) ) );
      return createResultValue( result, "vk::PhysicalDevice::getRandROutputDisplayEXT" );
    }
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result getSurfaceCapabilities2EXT( SurfaceKHR surface, SurfaceCapabilities2EXT* pSurfaceCapabilities ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilities2EXT( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilities2EXT*>( pSurfaceCapabilities ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<SurfaceCapabilities2EXT>::type getSurfaceCapabilities2EXT( SurfaceKHR surface ) const
    {
      SurfaceCapabilities2EXT surfaceCapabilities;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilities2EXT( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilities2EXT*>( &surfaceCapabilities ) ) );
      return createResultValue( result, surfaceCapabilities, "vk::PhysicalDevice::getSurfaceCapabilities2EXT" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkPhysicalDevice() const
    {
      return m_physicalDevice;
    }

    explicit operator bool() const
    {
      return m_physicalDevice != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_physicalDevice == VK_NULL_HANDLE;
    }

  private:
    VkPhysicalDevice m_physicalDevice;
  };
  static_assert( sizeof( PhysicalDevice ) == sizeof( VkPhysicalDevice ), "handle and wrapper have different size!" );

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  class DebugReportCallbackEXTDeleter
  {
  public:
    DebugReportCallbackEXTDeleter( Instance const* instance = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_instance( instance )
      , m_allocator( allocator )
    {}

    void operator()( DebugReportCallbackEXT debugReportCallbackEXT );

  private:
    Instance const* m_instance;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueDebugReportCallbackEXT = UniqueHandle<DebugReportCallbackEXT, DebugReportCallbackEXTDeleter>;

  class SurfaceKHRDeleter
  {
  public:
    SurfaceKHRDeleter( Instance const* instance = nullptr, Optional<const AllocationCallbacks> allocator = nullptr )
      : m_instance( instance )
      , m_allocator( allocator )
    {}

    void operator()( SurfaceKHR surfaceKHR );

  private:
    Instance const* m_instance;
    Optional<const AllocationCallbacks> m_allocator;
  };
  using UniqueSurfaceKHR = UniqueHandle<SurfaceKHR, SurfaceKHRDeleter>;
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  class Instance
  {
  public:
    Instance()
      : m_instance(VK_NULL_HANDLE)
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    Instance(VkInstance instance)
       : m_instance(instance)
    {}

    Instance& operator=(VkInstance instance)
    {
      m_instance = instance;
      return *this;
    }
#endif

    bool operator==(Instance const &rhs) const
    {
      return m_instance == rhs.m_instance;
    }

    bool operator!=(Instance const &rhs) const
    {
      return m_instance != rhs.m_instance;
    }

    bool operator<(Instance const &rhs) const
    {
      return m_instance < rhs.m_instance;
    }

    void destroy( const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyInstance( m_instance, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroy( Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyInstance( m_instance, reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result enumeratePhysicalDevices( uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices ) const
    {
      return static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, pPhysicalDeviceCount, reinterpret_cast<VkPhysicalDevice*>( pPhysicalDevices ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    template <typename Allocator = std::allocator<PhysicalDevice>>
    typename ResultValueType<std::vector<PhysicalDevice,Allocator>>::type enumeratePhysicalDevices() const
    {
      std::vector<PhysicalDevice,Allocator> physicalDevices;
      uint32_t physicalDeviceCount;
      Result result;
      do
      {
        result = static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, nullptr ) );
        if ( ( result == Result::eSuccess ) && physicalDeviceCount )
        {
          physicalDevices.resize( physicalDeviceCount );
          result = static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice*>( physicalDevices.data() ) ) );
        }
      } while ( result == Result::eIncomplete );
      assert( physicalDeviceCount <= physicalDevices.size() ); 
      physicalDevices.resize( physicalDeviceCount ); 
      return createResultValue( result, physicalDevices, "vk::Instance::enumeratePhysicalDevices" );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    PFN_vkVoidFunction getProcAddr( const char* pName ) const
    {
      return vkGetInstanceProcAddr( m_instance, pName );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    PFN_vkVoidFunction getProcAddr( const std::string & name ) const
    {
      return vkGetInstanceProcAddr( m_instance, name.c_str() );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    Result createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateAndroidSurfaceKHR( m_instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    ResultValueType<SurfaceKHR>::type createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateAndroidSurfaceKHR( m_instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createAndroidSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createAndroidSurfaceKHRUnique( const AndroidSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createAndroidSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDisplayPlaneSurfaceKHR( const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateDisplayPlaneSurfaceKHR( m_instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<SurfaceKHR>::type createDisplayPlaneSurfaceKHR( const DisplaySurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateDisplayPlaneSurfaceKHR( m_instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createDisplayPlaneSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createDisplayPlaneSurfaceKHRUnique( const DisplaySurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createDisplayPlaneSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_MIR_KHR
    Result createMirSurfaceKHR( const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateMirSurfaceKHR( m_instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_MIR_KHR
    ResultValueType<SurfaceKHR>::type createMirSurfaceKHR( const MirSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateMirSurfaceKHR( m_instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createMirSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createMirSurfaceKHRUnique( const MirSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createMirSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_MIR_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroySurfaceKHR( SurfaceKHR surface, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroySurfaceKHR( SurfaceKHR surface, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_VI_NN
    Result createViSurfaceNN( const ViSurfaceCreateInfoNN* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateViSurfaceNN( m_instance, reinterpret_cast<const VkViSurfaceCreateInfoNN*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_VI_NN
    ResultValueType<SurfaceKHR>::type createViSurfaceNN( const ViSurfaceCreateInfoNN & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateViSurfaceNN( m_instance, reinterpret_cast<const VkViSurfaceCreateInfoNN*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createViSurfaceNN" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createViSurfaceNNUnique( const ViSurfaceCreateInfoNN & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createViSurfaceNN( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_VI_NN*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Result createWaylandSurfaceKHR( const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateWaylandSurfaceKHR( m_instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    ResultValueType<SurfaceKHR>::type createWaylandSurfaceKHR( const WaylandSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateWaylandSurfaceKHR( m_instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createWaylandSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createWaylandSurfaceKHRUnique( const WaylandSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createWaylandSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
    Result createWin32SurfaceKHR( const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateWin32SurfaceKHR( m_instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WIN32_KHR
    ResultValueType<SurfaceKHR>::type createWin32SurfaceKHR( const Win32SurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateWin32SurfaceKHR( m_instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createWin32SurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createWin32SurfaceKHRUnique( const Win32SurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createWin32SurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
    Result createXlibSurfaceKHR( const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateXlibSurfaceKHR( m_instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_KHR
    ResultValueType<SurfaceKHR>::type createXlibSurfaceKHR( const XlibSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateXlibSurfaceKHR( m_instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createXlibSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createXlibSurfaceKHRUnique( const XlibSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createXlibSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XCB_KHR
    Result createXcbSurfaceKHR( const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateXcbSurfaceKHR( m_instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XCB_KHR
    ResultValueType<SurfaceKHR>::type createXcbSurfaceKHR( const XcbSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateXcbSurfaceKHR( m_instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      return createResultValue( result, surface, "vk::Instance::createXcbSurfaceKHR" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueSurfaceKHR createXcbSurfaceKHRUnique( const XcbSurfaceCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      SurfaceKHRDeleter deleter( this, allocator );
      return UniqueSurfaceKHR( createXcbSurfaceKHR( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    Result createDebugReportCallbackEXT( const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback ) const
    {
      return static_cast<Result>( vkCreateDebugReportCallbackEXT( m_instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDebugReportCallbackEXT*>( pCallback ) ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    ResultValueType<DebugReportCallbackEXT>::type createDebugReportCallbackEXT( const DebugReportCallbackCreateInfoEXT & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DebugReportCallbackEXT callback;
      Result result = static_cast<Result>( vkCreateDebugReportCallbackEXT( m_instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkDebugReportCallbackEXT*>( &callback ) ) );
      return createResultValue( result, callback, "vk::Instance::createDebugReportCallbackEXT" );
    }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    UniqueDebugReportCallbackEXT createDebugReportCallbackEXTUnique( const DebugReportCallbackCreateInfoEXT & createInfo, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      DebugReportCallbackEXTDeleter deleter( this, allocator );
      return UniqueDebugReportCallbackEXT( createDebugReportCallbackEXT( createInfo, allocator ), deleter );
    }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void destroyDebugReportCallbackEXT( DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDebugReportCallbackEXT( m_instance, static_cast<VkDebugReportCallbackEXT>( callback ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void destroyDebugReportCallbackEXT( DebugReportCallbackEXT callback, Optional<const AllocationCallbacks> allocator = nullptr ) const
    {
      vkDestroyDebugReportCallbackEXT( m_instance, static_cast<VkDebugReportCallbackEXT>( callback ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)) );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

    void debugReportMessageEXT( DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage ) const
    {
      vkDebugReportMessageEXT( m_instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, pLayerPrefix, pMessage );
    }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
    void debugReportMessageEXT( DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const std::string & layerPrefix, const std::string & message ) const
    {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
      assert( layerPrefix.size() == message.size() );
#else
      if ( layerPrefix.size() != message.size() )
      {
        throw std::logic_error( "vk::Instance::debugReportMessageEXT: layerPrefix.size() != message.size()" );
      }
#endif  // VULKAN_HPP_NO_EXCEPTIONS
      vkDebugReportMessageEXT( m_instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, layerPrefix.c_str(), message.c_str() );
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    explicit
#endif
    operator VkInstance() const
    {
      return m_instance;
    }

    explicit operator bool() const
    {
      return m_instance != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_instance == VK_NULL_HANDLE;
    }

  private:
    VkInstance m_instance;
  };
  static_assert( sizeof( Instance ) == sizeof( VkInstance ), "handle and wrapper have different size!" );

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_INLINE void DebugReportCallbackEXTDeleter::operator()( DebugReportCallbackEXT debugReportCallbackEXT )
  {
    m_instance->destroyDebugReportCallbackEXT( debugReportCallbackEXT, m_allocator );
  }

  VULKAN_HPP_INLINE void SurfaceKHRDeleter::operator()( SurfaceKHR surfaceKHR )
  {
    m_instance->destroySurfaceKHR( surfaceKHR, m_allocator );
  }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  struct CmdProcessCommandsInfoNVX
  {
    CmdProcessCommandsInfoNVX( ObjectTableNVX objectTable_ = ObjectTableNVX(), IndirectCommandsLayoutNVX indirectCommandsLayout_ = IndirectCommandsLayoutNVX(), uint32_t indirectCommandsTokenCount_ = 0, const IndirectCommandsTokenNVX* pIndirectCommandsTokens_ = nullptr, uint32_t maxSequencesCount_ = 0, CommandBuffer targetCommandBuffer_ = CommandBuffer(), Buffer sequencesCountBuffer_ = Buffer(), DeviceSize sequencesCountOffset_ = 0, Buffer sequencesIndexBuffer_ = Buffer(), DeviceSize sequencesIndexOffset_ = 0 )
      : sType( StructureType::eCmdProcessCommandsInfoNVX )
      , pNext( nullptr )
      , objectTable( objectTable_ )
      , indirectCommandsLayout( indirectCommandsLayout_ )
      , indirectCommandsTokenCount( indirectCommandsTokenCount_ )
      , pIndirectCommandsTokens( pIndirectCommandsTokens_ )
      , maxSequencesCount( maxSequencesCount_ )
      , targetCommandBuffer( targetCommandBuffer_ )
      , sequencesCountBuffer( sequencesCountBuffer_ )
      , sequencesCountOffset( sequencesCountOffset_ )
      , sequencesIndexBuffer( sequencesIndexBuffer_ )
      , sequencesIndexOffset( sequencesIndexOffset_ )
    {
    }

    CmdProcessCommandsInfoNVX( VkCmdProcessCommandsInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(CmdProcessCommandsInfoNVX) );
    }

    CmdProcessCommandsInfoNVX& operator=( VkCmdProcessCommandsInfoNVX const & rhs )
    {
      memcpy( this, &rhs, sizeof(CmdProcessCommandsInfoNVX) );
      return *this;
    }

    CmdProcessCommandsInfoNVX& setPNext( const void* pNext_ )
    {
      pNext = pNext_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setObjectTable( ObjectTableNVX objectTable_ )
    {
      objectTable = objectTable_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setIndirectCommandsLayout( IndirectCommandsLayoutNVX indirectCommandsLayout_ )
    {
      indirectCommandsLayout = indirectCommandsLayout_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setIndirectCommandsTokenCount( uint32_t indirectCommandsTokenCount_ )
    {
      indirectCommandsTokenCount = indirectCommandsTokenCount_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setPIndirectCommandsTokens( const IndirectCommandsTokenNVX* pIndirectCommandsTokens_ )
    {
      pIndirectCommandsTokens = pIndirectCommandsTokens_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setMaxSequencesCount( uint32_t maxSequencesCount_ )
    {
      maxSequencesCount = maxSequencesCount_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setTargetCommandBuffer( CommandBuffer targetCommandBuffer_ )
    {
      targetCommandBuffer = targetCommandBuffer_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setSequencesCountBuffer( Buffer sequencesCountBuffer_ )
    {
      sequencesCountBuffer = sequencesCountBuffer_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setSequencesCountOffset( DeviceSize sequencesCountOffset_ )
    {
      sequencesCountOffset = sequencesCountOffset_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setSequencesIndexBuffer( Buffer sequencesIndexBuffer_ )
    {
      sequencesIndexBuffer = sequencesIndexBuffer_;
      return *this;
    }

    CmdProcessCommandsInfoNVX& setSequencesIndexOffset( DeviceSize sequencesIndexOffset_ )
    {
      sequencesIndexOffset = sequencesIndexOffset_;
      return *this;
    }

    operator const VkCmdProcessCommandsInfoNVX&() const
    {
      return *reinterpret_cast<const VkCmdProcessCommandsInfoNVX*>(this);
    }

    bool operator==( CmdProcessCommandsInfoNVX const& rhs ) const
    {
      return ( sType == rhs.sType )
          && ( pNext == rhs.pNext )
          && ( objectTable == rhs.objectTable )
          && ( indirectCommandsLayout == rhs.indirectCommandsLayout )
          && ( indirectCommandsTokenCount == rhs.indirectCommandsTokenCount )
          && ( pIndirectCommandsTokens == rhs.pIndirectCommandsTokens )
          && ( maxSequencesCount == rhs.maxSequencesCount )
          && ( targetCommandBuffer == rhs.targetCommandBuffer )
          && ( sequencesCountBuffer == rhs.sequencesCountBuffer )
          && ( sequencesCountOffset == rhs.sequencesCountOffset )
          && ( sequencesIndexBuffer == rhs.sequencesIndexBuffer )
          && ( sequencesIndexOffset == rhs.sequencesIndexOffset );
    }

    bool operator!=( CmdProcessCommandsInfoNVX const& rhs ) const
    {
      return !operator==( rhs );
    }

  private:
    StructureType sType;

  public:
    const void* pNext;
    ObjectTableNVX objectTable;
    IndirectCommandsLayoutNVX indirectCommandsLayout;
    uint32_t indirectCommandsTokenCount;
    const IndirectCommandsTokenNVX* pIndirectCommandsTokens;
    uint32_t maxSequencesCount;
    CommandBuffer targetCommandBuffer;
    Buffer sequencesCountBuffer;
    DeviceSize sequencesCountOffset;
    Buffer sequencesIndexBuffer;
    DeviceSize sequencesIndexOffset;
  };
  static_assert( sizeof( CmdProcessCommandsInfoNVX ) == sizeof( VkCmdProcessCommandsInfoNVX ), "struct and wrapper have different size!" );

  VULKAN_HPP_INLINE Result createInstance( const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance )
  {
    return static_cast<Result>( vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkInstance*>( pInstance ) ) );
  }

#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  VULKAN_HPP_INLINE ResultValueType<Instance>::type createInstance( const InstanceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr )
  {
    Instance instance;
    Result result = static_cast<Result>( vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( static_cast<const AllocationCallbacks*>( allocator)), reinterpret_cast<VkInstance*>( &instance ) ) );
    return createResultValue( result, instance, "vk::createInstance" );
  }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_INLINE UniqueInstance createInstanceUnique( const InstanceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr )
  {
    InstanceDeleter deleter( allocator );
    return UniqueInstance( createInstance( createInfo, allocator ), deleter );
  }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  VULKAN_HPP_INLINE void DeviceDeleter::operator()( Device device )
  {
    device.destroy( m_allocator );
  }

  VULKAN_HPP_INLINE void InstanceDeleter::operator()( Instance instance )
  {
    instance.destroy( m_allocator );
  }
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/

  VULKAN_HPP_INLINE std::string to_string(FramebufferCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(FramebufferCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(QueryPoolCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(QueryPoolCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(RenderPassCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(RenderPassCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(SamplerCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(SamplerCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineLayoutCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineLayoutCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineCacheCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineCacheCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineDepthStencilStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineDepthStencilStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineDynamicStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineDynamicStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineColorBlendStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineColorBlendStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineMultisampleStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineMultisampleStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineRasterizationStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineRasterizationStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineViewportStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineViewportStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineTessellationStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineTessellationStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineInputAssemblyStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineInputAssemblyStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineVertexInputStateCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineVertexInputStateCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineShaderStageCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineShaderStageCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorSetLayoutCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorSetLayoutCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(BufferViewCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(BufferViewCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(InstanceCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(InstanceCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DeviceCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DeviceCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DeviceQueueCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DeviceQueueCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageViewCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageViewCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(SemaphoreCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(SemaphoreCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(ShaderModuleCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(ShaderModuleCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(EventCreateFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(EventCreateFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryMapFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryMapFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(SubpassDescriptionFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(SubpassDescriptionFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorPoolResetFlagBits)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorPoolResetFlags)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(SwapchainCreateFlagBitsKHR)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(SwapchainCreateFlagsKHR)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayModeCreateFlagBitsKHR)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayModeCreateFlagsKHR)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(DisplaySurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(DisplaySurfaceCreateFlagsKHR)
  {
    return "{}";
  }

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VULKAN_HPP_INLINE std::string to_string(AndroidSurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  VULKAN_HPP_INLINE std::string to_string(AndroidSurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  VULKAN_HPP_INLINE std::string to_string(MirSurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  VULKAN_HPP_INLINE std::string to_string(MirSurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_VI_NN
  VULKAN_HPP_INLINE std::string to_string(ViSurfaceCreateFlagBitsNN)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_VI_NN
  VULKAN_HPP_INLINE std::string to_string(ViSurfaceCreateFlagsNN)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VULKAN_HPP_INLINE std::string to_string(WaylandSurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  VULKAN_HPP_INLINE std::string to_string(WaylandSurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  VULKAN_HPP_INLINE std::string to_string(Win32SurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  VULKAN_HPP_INLINE std::string to_string(Win32SurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  VULKAN_HPP_INLINE std::string to_string(XlibSurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  VULKAN_HPP_INLINE std::string to_string(XlibSurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  VULKAN_HPP_INLINE std::string to_string(XcbSurfaceCreateFlagBitsKHR)
  {
    return "(void)";
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  VULKAN_HPP_INLINE std::string to_string(XcbSurfaceCreateFlagsKHR)
  {
    return "{}";
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  VULKAN_HPP_INLINE std::string to_string(CommandPoolTrimFlagBitsKHR)
  {
    return "(void)";
  }

  VULKAN_HPP_INLINE std::string to_string(CommandPoolTrimFlagsKHR)
  {
    return "{}";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageLayout value)
  {
    switch (value)
    {
    case ImageLayout::eUndefined: return "Undefined";
    case ImageLayout::eGeneral: return "General";
    case ImageLayout::eColorAttachmentOptimal: return "ColorAttachmentOptimal";
    case ImageLayout::eDepthStencilAttachmentOptimal: return "DepthStencilAttachmentOptimal";
    case ImageLayout::eDepthStencilReadOnlyOptimal: return "DepthStencilReadOnlyOptimal";
    case ImageLayout::eShaderReadOnlyOptimal: return "ShaderReadOnlyOptimal";
    case ImageLayout::eTransferSrcOptimal: return "TransferSrcOptimal";
    case ImageLayout::eTransferDstOptimal: return "TransferDstOptimal";
    case ImageLayout::ePreinitialized: return "Preinitialized";
    case ImageLayout::ePresentSrcKHR: return "PresentSrcKHR";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(AttachmentLoadOp value)
  {
    switch (value)
    {
    case AttachmentLoadOp::eLoad: return "Load";
    case AttachmentLoadOp::eClear: return "Clear";
    case AttachmentLoadOp::eDontCare: return "DontCare";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(AttachmentStoreOp value)
  {
    switch (value)
    {
    case AttachmentStoreOp::eStore: return "Store";
    case AttachmentStoreOp::eDontCare: return "DontCare";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageType value)
  {
    switch (value)
    {
    case ImageType::e1D: return "1D";
    case ImageType::e2D: return "2D";
    case ImageType::e3D: return "3D";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageTiling value)
  {
    switch (value)
    {
    case ImageTiling::eOptimal: return "Optimal";
    case ImageTiling::eLinear: return "Linear";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageViewType value)
  {
    switch (value)
    {
    case ImageViewType::e1D: return "1D";
    case ImageViewType::e2D: return "2D";
    case ImageViewType::e3D: return "3D";
    case ImageViewType::eCube: return "Cube";
    case ImageViewType::e1DArray: return "1DArray";
    case ImageViewType::e2DArray: return "2DArray";
    case ImageViewType::eCubeArray: return "CubeArray";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CommandBufferLevel value)
  {
    switch (value)
    {
    case CommandBufferLevel::ePrimary: return "Primary";
    case CommandBufferLevel::eSecondary: return "Secondary";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ComponentSwizzle value)
  {
    switch (value)
    {
    case ComponentSwizzle::eIdentity: return "Identity";
    case ComponentSwizzle::eZero: return "Zero";
    case ComponentSwizzle::eOne: return "One";
    case ComponentSwizzle::eR: return "R";
    case ComponentSwizzle::eG: return "G";
    case ComponentSwizzle::eB: return "B";
    case ComponentSwizzle::eA: return "A";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorType value)
  {
    switch (value)
    {
    case DescriptorType::eSampler: return "Sampler";
    case DescriptorType::eCombinedImageSampler: return "CombinedImageSampler";
    case DescriptorType::eSampledImage: return "SampledImage";
    case DescriptorType::eStorageImage: return "StorageImage";
    case DescriptorType::eUniformTexelBuffer: return "UniformTexelBuffer";
    case DescriptorType::eStorageTexelBuffer: return "StorageTexelBuffer";
    case DescriptorType::eUniformBuffer: return "UniformBuffer";
    case DescriptorType::eStorageBuffer: return "StorageBuffer";
    case DescriptorType::eUniformBufferDynamic: return "UniformBufferDynamic";
    case DescriptorType::eStorageBufferDynamic: return "StorageBufferDynamic";
    case DescriptorType::eInputAttachment: return "InputAttachment";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueryType value)
  {
    switch (value)
    {
    case QueryType::eOcclusion: return "Occlusion";
    case QueryType::ePipelineStatistics: return "PipelineStatistics";
    case QueryType::eTimestamp: return "Timestamp";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(BorderColor value)
  {
    switch (value)
    {
    case BorderColor::eFloatTransparentBlack: return "FloatTransparentBlack";
    case BorderColor::eIntTransparentBlack: return "IntTransparentBlack";
    case BorderColor::eFloatOpaqueBlack: return "FloatOpaqueBlack";
    case BorderColor::eIntOpaqueBlack: return "IntOpaqueBlack";
    case BorderColor::eFloatOpaqueWhite: return "FloatOpaqueWhite";
    case BorderColor::eIntOpaqueWhite: return "IntOpaqueWhite";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineBindPoint value)
  {
    switch (value)
    {
    case PipelineBindPoint::eGraphics: return "Graphics";
    case PipelineBindPoint::eCompute: return "Compute";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineCacheHeaderVersion value)
  {
    switch (value)
    {
    case PipelineCacheHeaderVersion::eOne: return "One";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PrimitiveTopology value)
  {
    switch (value)
    {
    case PrimitiveTopology::ePointList: return "PointList";
    case PrimitiveTopology::eLineList: return "LineList";
    case PrimitiveTopology::eLineStrip: return "LineStrip";
    case PrimitiveTopology::eTriangleList: return "TriangleList";
    case PrimitiveTopology::eTriangleStrip: return "TriangleStrip";
    case PrimitiveTopology::eTriangleFan: return "TriangleFan";
    case PrimitiveTopology::eLineListWithAdjacency: return "LineListWithAdjacency";
    case PrimitiveTopology::eLineStripWithAdjacency: return "LineStripWithAdjacency";
    case PrimitiveTopology::eTriangleListWithAdjacency: return "TriangleListWithAdjacency";
    case PrimitiveTopology::eTriangleStripWithAdjacency: return "TriangleStripWithAdjacency";
    case PrimitiveTopology::ePatchList: return "PatchList";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SharingMode value)
  {
    switch (value)
    {
    case SharingMode::eExclusive: return "Exclusive";
    case SharingMode::eConcurrent: return "Concurrent";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(IndexType value)
  {
    switch (value)
    {
    case IndexType::eUint16: return "Uint16";
    case IndexType::eUint32: return "Uint32";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(Filter value)
  {
    switch (value)
    {
    case Filter::eNearest: return "Nearest";
    case Filter::eLinear: return "Linear";
    case Filter::eCubicIMG: return "CubicIMG";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SamplerMipmapMode value)
  {
    switch (value)
    {
    case SamplerMipmapMode::eNearest: return "Nearest";
    case SamplerMipmapMode::eLinear: return "Linear";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SamplerAddressMode value)
  {
    switch (value)
    {
    case SamplerAddressMode::eRepeat: return "Repeat";
    case SamplerAddressMode::eMirroredRepeat: return "MirroredRepeat";
    case SamplerAddressMode::eClampToEdge: return "ClampToEdge";
    case SamplerAddressMode::eClampToBorder: return "ClampToBorder";
    case SamplerAddressMode::eMirrorClampToEdge: return "MirrorClampToEdge";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CompareOp value)
  {
    switch (value)
    {
    case CompareOp::eNever: return "Never";
    case CompareOp::eLess: return "Less";
    case CompareOp::eEqual: return "Equal";
    case CompareOp::eLessOrEqual: return "LessOrEqual";
    case CompareOp::eGreater: return "Greater";
    case CompareOp::eNotEqual: return "NotEqual";
    case CompareOp::eGreaterOrEqual: return "GreaterOrEqual";
    case CompareOp::eAlways: return "Always";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PolygonMode value)
  {
    switch (value)
    {
    case PolygonMode::eFill: return "Fill";
    case PolygonMode::eLine: return "Line";
    case PolygonMode::ePoint: return "Point";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CullModeFlagBits value)
  {
    switch (value)
    {
    case CullModeFlagBits::eNone: return "None";
    case CullModeFlagBits::eFront: return "Front";
    case CullModeFlagBits::eBack: return "Back";
    case CullModeFlagBits::eFrontAndBack: return "FrontAndBack";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CullModeFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CullModeFlagBits::eNone) result += "None | ";
    if (value & CullModeFlagBits::eFront) result += "Front | ";
    if (value & CullModeFlagBits::eBack) result += "Back | ";
    if (value & CullModeFlagBits::eFrontAndBack) result += "FrontAndBack | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(FrontFace value)
  {
    switch (value)
    {
    case FrontFace::eCounterClockwise: return "CounterClockwise";
    case FrontFace::eClockwise: return "Clockwise";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(BlendFactor value)
  {
    switch (value)
    {
    case BlendFactor::eZero: return "Zero";
    case BlendFactor::eOne: return "One";
    case BlendFactor::eSrcColor: return "SrcColor";
    case BlendFactor::eOneMinusSrcColor: return "OneMinusSrcColor";
    case BlendFactor::eDstColor: return "DstColor";
    case BlendFactor::eOneMinusDstColor: return "OneMinusDstColor";
    case BlendFactor::eSrcAlpha: return "SrcAlpha";
    case BlendFactor::eOneMinusSrcAlpha: return "OneMinusSrcAlpha";
    case BlendFactor::eDstAlpha: return "DstAlpha";
    case BlendFactor::eOneMinusDstAlpha: return "OneMinusDstAlpha";
    case BlendFactor::eConstantColor: return "ConstantColor";
    case BlendFactor::eOneMinusConstantColor: return "OneMinusConstantColor";
    case BlendFactor::eConstantAlpha: return "ConstantAlpha";
    case BlendFactor::eOneMinusConstantAlpha: return "OneMinusConstantAlpha";
    case BlendFactor::eSrcAlphaSaturate: return "SrcAlphaSaturate";
    case BlendFactor::eSrc1Color: return "Src1Color";
    case BlendFactor::eOneMinusSrc1Color: return "OneMinusSrc1Color";
    case BlendFactor::eSrc1Alpha: return "Src1Alpha";
    case BlendFactor::eOneMinusSrc1Alpha: return "OneMinusSrc1Alpha";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(BlendOp value)
  {
    switch (value)
    {
    case BlendOp::eAdd: return "Add";
    case BlendOp::eSubtract: return "Subtract";
    case BlendOp::eReverseSubtract: return "ReverseSubtract";
    case BlendOp::eMin: return "Min";
    case BlendOp::eMax: return "Max";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(StencilOp value)
  {
    switch (value)
    {
    case StencilOp::eKeep: return "Keep";
    case StencilOp::eZero: return "Zero";
    case StencilOp::eReplace: return "Replace";
    case StencilOp::eIncrementAndClamp: return "IncrementAndClamp";
    case StencilOp::eDecrementAndClamp: return "DecrementAndClamp";
    case StencilOp::eInvert: return "Invert";
    case StencilOp::eIncrementAndWrap: return "IncrementAndWrap";
    case StencilOp::eDecrementAndWrap: return "DecrementAndWrap";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(LogicOp value)
  {
    switch (value)
    {
    case LogicOp::eClear: return "Clear";
    case LogicOp::eAnd: return "And";
    case LogicOp::eAndReverse: return "AndReverse";
    case LogicOp::eCopy: return "Copy";
    case LogicOp::eAndInverted: return "AndInverted";
    case LogicOp::eNoOp: return "NoOp";
    case LogicOp::eXor: return "Xor";
    case LogicOp::eOr: return "Or";
    case LogicOp::eNor: return "Nor";
    case LogicOp::eEquivalent: return "Equivalent";
    case LogicOp::eInvert: return "Invert";
    case LogicOp::eOrReverse: return "OrReverse";
    case LogicOp::eCopyInverted: return "CopyInverted";
    case LogicOp::eOrInverted: return "OrInverted";
    case LogicOp::eNand: return "Nand";
    case LogicOp::eSet: return "Set";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(InternalAllocationType value)
  {
    switch (value)
    {
    case InternalAllocationType::eExecutable: return "Executable";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SystemAllocationScope value)
  {
    switch (value)
    {
    case SystemAllocationScope::eCommand: return "Command";
    case SystemAllocationScope::eObject: return "Object";
    case SystemAllocationScope::eCache: return "Cache";
    case SystemAllocationScope::eDevice: return "Device";
    case SystemAllocationScope::eInstance: return "Instance";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PhysicalDeviceType value)
  {
    switch (value)
    {
    case PhysicalDeviceType::eOther: return "Other";
    case PhysicalDeviceType::eIntegratedGpu: return "IntegratedGpu";
    case PhysicalDeviceType::eDiscreteGpu: return "DiscreteGpu";
    case PhysicalDeviceType::eVirtualGpu: return "VirtualGpu";
    case PhysicalDeviceType::eCpu: return "Cpu";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(VertexInputRate value)
  {
    switch (value)
    {
    case VertexInputRate::eVertex: return "Vertex";
    case VertexInputRate::eInstance: return "Instance";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(Format value)
  {
    switch (value)
    {
    case Format::eUndefined: return "Undefined";
    case Format::eR4G4UnormPack8: return "R4G4UnormPack8";
    case Format::eR4G4B4A4UnormPack16: return "R4G4B4A4UnormPack16";
    case Format::eB4G4R4A4UnormPack16: return "B4G4R4A4UnormPack16";
    case Format::eR5G6B5UnormPack16: return "R5G6B5UnormPack16";
    case Format::eB5G6R5UnormPack16: return "B5G6R5UnormPack16";
    case Format::eR5G5B5A1UnormPack16: return "R5G5B5A1UnormPack16";
    case Format::eB5G5R5A1UnormPack16: return "B5G5R5A1UnormPack16";
    case Format::eA1R5G5B5UnormPack16: return "A1R5G5B5UnormPack16";
    case Format::eR8Unorm: return "R8Unorm";
    case Format::eR8Snorm: return "R8Snorm";
    case Format::eR8Uscaled: return "R8Uscaled";
    case Format::eR8Sscaled: return "R8Sscaled";
    case Format::eR8Uint: return "R8Uint";
    case Format::eR8Sint: return "R8Sint";
    case Format::eR8Srgb: return "R8Srgb";
    case Format::eR8G8Unorm: return "R8G8Unorm";
    case Format::eR8G8Snorm: return "R8G8Snorm";
    case Format::eR8G8Uscaled: return "R8G8Uscaled";
    case Format::eR8G8Sscaled: return "R8G8Sscaled";
    case Format::eR8G8Uint: return "R8G8Uint";
    case Format::eR8G8Sint: return "R8G8Sint";
    case Format::eR8G8Srgb: return "R8G8Srgb";
    case Format::eR8G8B8Unorm: return "R8G8B8Unorm";
    case Format::eR8G8B8Snorm: return "R8G8B8Snorm";
    case Format::eR8G8B8Uscaled: return "R8G8B8Uscaled";
    case Format::eR8G8B8Sscaled: return "R8G8B8Sscaled";
    case Format::eR8G8B8Uint: return "R8G8B8Uint";
    case Format::eR8G8B8Sint: return "R8G8B8Sint";
    case Format::eR8G8B8Srgb: return "R8G8B8Srgb";
    case Format::eB8G8R8Unorm: return "B8G8R8Unorm";
    case Format::eB8G8R8Snorm: return "B8G8R8Snorm";
    case Format::eB8G8R8Uscaled: return "B8G8R8Uscaled";
    case Format::eB8G8R8Sscaled: return "B8G8R8Sscaled";
    case Format::eB8G8R8Uint: return "B8G8R8Uint";
    case Format::eB8G8R8Sint: return "B8G8R8Sint";
    case Format::eB8G8R8Srgb: return "B8G8R8Srgb";
    case Format::eR8G8B8A8Unorm: return "R8G8B8A8Unorm";
    case Format::eR8G8B8A8Snorm: return "R8G8B8A8Snorm";
    case Format::eR8G8B8A8Uscaled: return "R8G8B8A8Uscaled";
    case Format::eR8G8B8A8Sscaled: return "R8G8B8A8Sscaled";
    case Format::eR8G8B8A8Uint: return "R8G8B8A8Uint";
    case Format::eR8G8B8A8Sint: return "R8G8B8A8Sint";
    case Format::eR8G8B8A8Srgb: return "R8G8B8A8Srgb";
    case Format::eB8G8R8A8Unorm: return "B8G8R8A8Unorm";
    case Format::eB8G8R8A8Snorm: return "B8G8R8A8Snorm";
    case Format::eB8G8R8A8Uscaled: return "B8G8R8A8Uscaled";
    case Format::eB8G8R8A8Sscaled: return "B8G8R8A8Sscaled";
    case Format::eB8G8R8A8Uint: return "B8G8R8A8Uint";
    case Format::eB8G8R8A8Sint: return "B8G8R8A8Sint";
    case Format::eB8G8R8A8Srgb: return "B8G8R8A8Srgb";
    case Format::eA8B8G8R8UnormPack32: return "A8B8G8R8UnormPack32";
    case Format::eA8B8G8R8SnormPack32: return "A8B8G8R8SnormPack32";
    case Format::eA8B8G8R8UscaledPack32: return "A8B8G8R8UscaledPack32";
    case Format::eA8B8G8R8SscaledPack32: return "A8B8G8R8SscaledPack32";
    case Format::eA8B8G8R8UintPack32: return "A8B8G8R8UintPack32";
    case Format::eA8B8G8R8SintPack32: return "A8B8G8R8SintPack32";
    case Format::eA8B8G8R8SrgbPack32: return "A8B8G8R8SrgbPack32";
    case Format::eA2R10G10B10UnormPack32: return "A2R10G10B10UnormPack32";
    case Format::eA2R10G10B10SnormPack32: return "A2R10G10B10SnormPack32";
    case Format::eA2R10G10B10UscaledPack32: return "A2R10G10B10UscaledPack32";
    case Format::eA2R10G10B10SscaledPack32: return "A2R10G10B10SscaledPack32";
    case Format::eA2R10G10B10UintPack32: return "A2R10G10B10UintPack32";
    case Format::eA2R10G10B10SintPack32: return "A2R10G10B10SintPack32";
    case Format::eA2B10G10R10UnormPack32: return "A2B10G10R10UnormPack32";
    case Format::eA2B10G10R10SnormPack32: return "A2B10G10R10SnormPack32";
    case Format::eA2B10G10R10UscaledPack32: return "A2B10G10R10UscaledPack32";
    case Format::eA2B10G10R10SscaledPack32: return "A2B10G10R10SscaledPack32";
    case Format::eA2B10G10R10UintPack32: return "A2B10G10R10UintPack32";
    case Format::eA2B10G10R10SintPack32: return "A2B10G10R10SintPack32";
    case Format::eR16Unorm: return "R16Unorm";
    case Format::eR16Snorm: return "R16Snorm";
    case Format::eR16Uscaled: return "R16Uscaled";
    case Format::eR16Sscaled: return "R16Sscaled";
    case Format::eR16Uint: return "R16Uint";
    case Format::eR16Sint: return "R16Sint";
    case Format::eR16Sfloat: return "R16Sfloat";
    case Format::eR16G16Unorm: return "R16G16Unorm";
    case Format::eR16G16Snorm: return "R16G16Snorm";
    case Format::eR16G16Uscaled: return "R16G16Uscaled";
    case Format::eR16G16Sscaled: return "R16G16Sscaled";
    case Format::eR16G16Uint: return "R16G16Uint";
    case Format::eR16G16Sint: return "R16G16Sint";
    case Format::eR16G16Sfloat: return "R16G16Sfloat";
    case Format::eR16G16B16Unorm: return "R16G16B16Unorm";
    case Format::eR16G16B16Snorm: return "R16G16B16Snorm";
    case Format::eR16G16B16Uscaled: return "R16G16B16Uscaled";
    case Format::eR16G16B16Sscaled: return "R16G16B16Sscaled";
    case Format::eR16G16B16Uint: return "R16G16B16Uint";
    case Format::eR16G16B16Sint: return "R16G16B16Sint";
    case Format::eR16G16B16Sfloat: return "R16G16B16Sfloat";
    case Format::eR16G16B16A16Unorm: return "R16G16B16A16Unorm";
    case Format::eR16G16B16A16Snorm: return "R16G16B16A16Snorm";
    case Format::eR16G16B16A16Uscaled: return "R16G16B16A16Uscaled";
    case Format::eR16G16B16A16Sscaled: return "R16G16B16A16Sscaled";
    case Format::eR16G16B16A16Uint: return "R16G16B16A16Uint";
    case Format::eR16G16B16A16Sint: return "R16G16B16A16Sint";
    case Format::eR16G16B16A16Sfloat: return "R16G16B16A16Sfloat";
    case Format::eR32Uint: return "R32Uint";
    case Format::eR32Sint: return "R32Sint";
    case Format::eR32Sfloat: return "R32Sfloat";
    case Format::eR32G32Uint: return "R32G32Uint";
    case Format::eR32G32Sint: return "R32G32Sint";
    case Format::eR32G32Sfloat: return "R32G32Sfloat";
    case Format::eR32G32B32Uint: return "R32G32B32Uint";
    case Format::eR32G32B32Sint: return "R32G32B32Sint";
    case Format::eR32G32B32Sfloat: return "R32G32B32Sfloat";
    case Format::eR32G32B32A32Uint: return "R32G32B32A32Uint";
    case Format::eR32G32B32A32Sint: return "R32G32B32A32Sint";
    case Format::eR32G32B32A32Sfloat: return "R32G32B32A32Sfloat";
    case Format::eR64Uint: return "R64Uint";
    case Format::eR64Sint: return "R64Sint";
    case Format::eR64Sfloat: return "R64Sfloat";
    case Format::eR64G64Uint: return "R64G64Uint";
    case Format::eR64G64Sint: return "R64G64Sint";
    case Format::eR64G64Sfloat: return "R64G64Sfloat";
    case Format::eR64G64B64Uint: return "R64G64B64Uint";
    case Format::eR64G64B64Sint: return "R64G64B64Sint";
    case Format::eR64G64B64Sfloat: return "R64G64B64Sfloat";
    case Format::eR64G64B64A64Uint: return "R64G64B64A64Uint";
    case Format::eR64G64B64A64Sint: return "R64G64B64A64Sint";
    case Format::eR64G64B64A64Sfloat: return "R64G64B64A64Sfloat";
    case Format::eB10G11R11UfloatPack32: return "B10G11R11UfloatPack32";
    case Format::eE5B9G9R9UfloatPack32: return "E5B9G9R9UfloatPack32";
    case Format::eD16Unorm: return "D16Unorm";
    case Format::eX8D24UnormPack32: return "X8D24UnormPack32";
    case Format::eD32Sfloat: return "D32Sfloat";
    case Format::eS8Uint: return "S8Uint";
    case Format::eD16UnormS8Uint: return "D16UnormS8Uint";
    case Format::eD24UnormS8Uint: return "D24UnormS8Uint";
    case Format::eD32SfloatS8Uint: return "D32SfloatS8Uint";
    case Format::eBc1RgbUnormBlock: return "Bc1RgbUnormBlock";
    case Format::eBc1RgbSrgbBlock: return "Bc1RgbSrgbBlock";
    case Format::eBc1RgbaUnormBlock: return "Bc1RgbaUnormBlock";
    case Format::eBc1RgbaSrgbBlock: return "Bc1RgbaSrgbBlock";
    case Format::eBc2UnormBlock: return "Bc2UnormBlock";
    case Format::eBc2SrgbBlock: return "Bc2SrgbBlock";
    case Format::eBc3UnormBlock: return "Bc3UnormBlock";
    case Format::eBc3SrgbBlock: return "Bc3SrgbBlock";
    case Format::eBc4UnormBlock: return "Bc4UnormBlock";
    case Format::eBc4SnormBlock: return "Bc4SnormBlock";
    case Format::eBc5UnormBlock: return "Bc5UnormBlock";
    case Format::eBc5SnormBlock: return "Bc5SnormBlock";
    case Format::eBc6HUfloatBlock: return "Bc6HUfloatBlock";
    case Format::eBc6HSfloatBlock: return "Bc6HSfloatBlock";
    case Format::eBc7UnormBlock: return "Bc7UnormBlock";
    case Format::eBc7SrgbBlock: return "Bc7SrgbBlock";
    case Format::eEtc2R8G8B8UnormBlock: return "Etc2R8G8B8UnormBlock";
    case Format::eEtc2R8G8B8SrgbBlock: return "Etc2R8G8B8SrgbBlock";
    case Format::eEtc2R8G8B8A1UnormBlock: return "Etc2R8G8B8A1UnormBlock";
    case Format::eEtc2R8G8B8A1SrgbBlock: return "Etc2R8G8B8A1SrgbBlock";
    case Format::eEtc2R8G8B8A8UnormBlock: return "Etc2R8G8B8A8UnormBlock";
    case Format::eEtc2R8G8B8A8SrgbBlock: return "Etc2R8G8B8A8SrgbBlock";
    case Format::eEacR11UnormBlock: return "EacR11UnormBlock";
    case Format::eEacR11SnormBlock: return "EacR11SnormBlock";
    case Format::eEacR11G11UnormBlock: return "EacR11G11UnormBlock";
    case Format::eEacR11G11SnormBlock: return "EacR11G11SnormBlock";
    case Format::eAstc4x4UnormBlock: return "Astc4x4UnormBlock";
    case Format::eAstc4x4SrgbBlock: return "Astc4x4SrgbBlock";
    case Format::eAstc5x4UnormBlock: return "Astc5x4UnormBlock";
    case Format::eAstc5x4SrgbBlock: return "Astc5x4SrgbBlock";
    case Format::eAstc5x5UnormBlock: return "Astc5x5UnormBlock";
    case Format::eAstc5x5SrgbBlock: return "Astc5x5SrgbBlock";
    case Format::eAstc6x5UnormBlock: return "Astc6x5UnormBlock";
    case Format::eAstc6x5SrgbBlock: return "Astc6x5SrgbBlock";
    case Format::eAstc6x6UnormBlock: return "Astc6x6UnormBlock";
    case Format::eAstc6x6SrgbBlock: return "Astc6x6SrgbBlock";
    case Format::eAstc8x5UnormBlock: return "Astc8x5UnormBlock";
    case Format::eAstc8x5SrgbBlock: return "Astc8x5SrgbBlock";
    case Format::eAstc8x6UnormBlock: return "Astc8x6UnormBlock";
    case Format::eAstc8x6SrgbBlock: return "Astc8x6SrgbBlock";
    case Format::eAstc8x8UnormBlock: return "Astc8x8UnormBlock";
    case Format::eAstc8x8SrgbBlock: return "Astc8x8SrgbBlock";
    case Format::eAstc10x5UnormBlock: return "Astc10x5UnormBlock";
    case Format::eAstc10x5SrgbBlock: return "Astc10x5SrgbBlock";
    case Format::eAstc10x6UnormBlock: return "Astc10x6UnormBlock";
    case Format::eAstc10x6SrgbBlock: return "Astc10x6SrgbBlock";
    case Format::eAstc10x8UnormBlock: return "Astc10x8UnormBlock";
    case Format::eAstc10x8SrgbBlock: return "Astc10x8SrgbBlock";
    case Format::eAstc10x10UnormBlock: return "Astc10x10UnormBlock";
    case Format::eAstc10x10SrgbBlock: return "Astc10x10SrgbBlock";
    case Format::eAstc12x10UnormBlock: return "Astc12x10UnormBlock";
    case Format::eAstc12x10SrgbBlock: return "Astc12x10SrgbBlock";
    case Format::eAstc12x12UnormBlock: return "Astc12x12UnormBlock";
    case Format::eAstc12x12SrgbBlock: return "Astc12x12SrgbBlock";
    case Format::ePvrtc12BppUnormBlockIMG: return "Pvrtc12BppUnormBlockIMG";
    case Format::ePvrtc14BppUnormBlockIMG: return "Pvrtc14BppUnormBlockIMG";
    case Format::ePvrtc22BppUnormBlockIMG: return "Pvrtc22BppUnormBlockIMG";
    case Format::ePvrtc24BppUnormBlockIMG: return "Pvrtc24BppUnormBlockIMG";
    case Format::ePvrtc12BppSrgbBlockIMG: return "Pvrtc12BppSrgbBlockIMG";
    case Format::ePvrtc14BppSrgbBlockIMG: return "Pvrtc14BppSrgbBlockIMG";
    case Format::ePvrtc22BppSrgbBlockIMG: return "Pvrtc22BppSrgbBlockIMG";
    case Format::ePvrtc24BppSrgbBlockIMG: return "Pvrtc24BppSrgbBlockIMG";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(StructureType value)
  {
    switch (value)
    {
    case StructureType::eApplicationInfo: return "ApplicationInfo";
    case StructureType::eInstanceCreateInfo: return "InstanceCreateInfo";
    case StructureType::eDeviceQueueCreateInfo: return "DeviceQueueCreateInfo";
    case StructureType::eDeviceCreateInfo: return "DeviceCreateInfo";
    case StructureType::eSubmitInfo: return "SubmitInfo";
    case StructureType::eMemoryAllocateInfo: return "MemoryAllocateInfo";
    case StructureType::eMappedMemoryRange: return "MappedMemoryRange";
    case StructureType::eBindSparseInfo: return "BindSparseInfo";
    case StructureType::eFenceCreateInfo: return "FenceCreateInfo";
    case StructureType::eSemaphoreCreateInfo: return "SemaphoreCreateInfo";
    case StructureType::eEventCreateInfo: return "EventCreateInfo";
    case StructureType::eQueryPoolCreateInfo: return "QueryPoolCreateInfo";
    case StructureType::eBufferCreateInfo: return "BufferCreateInfo";
    case StructureType::eBufferViewCreateInfo: return "BufferViewCreateInfo";
    case StructureType::eImageCreateInfo: return "ImageCreateInfo";
    case StructureType::eImageViewCreateInfo: return "ImageViewCreateInfo";
    case StructureType::eShaderModuleCreateInfo: return "ShaderModuleCreateInfo";
    case StructureType::ePipelineCacheCreateInfo: return "PipelineCacheCreateInfo";
    case StructureType::ePipelineShaderStageCreateInfo: return "PipelineShaderStageCreateInfo";
    case StructureType::ePipelineVertexInputStateCreateInfo: return "PipelineVertexInputStateCreateInfo";
    case StructureType::ePipelineInputAssemblyStateCreateInfo: return "PipelineInputAssemblyStateCreateInfo";
    case StructureType::ePipelineTessellationStateCreateInfo: return "PipelineTessellationStateCreateInfo";
    case StructureType::ePipelineViewportStateCreateInfo: return "PipelineViewportStateCreateInfo";
    case StructureType::ePipelineRasterizationStateCreateInfo: return "PipelineRasterizationStateCreateInfo";
    case StructureType::ePipelineMultisampleStateCreateInfo: return "PipelineMultisampleStateCreateInfo";
    case StructureType::ePipelineDepthStencilStateCreateInfo: return "PipelineDepthStencilStateCreateInfo";
    case StructureType::ePipelineColorBlendStateCreateInfo: return "PipelineColorBlendStateCreateInfo";
    case StructureType::ePipelineDynamicStateCreateInfo: return "PipelineDynamicStateCreateInfo";
    case StructureType::eGraphicsPipelineCreateInfo: return "GraphicsPipelineCreateInfo";
    case StructureType::eComputePipelineCreateInfo: return "ComputePipelineCreateInfo";
    case StructureType::ePipelineLayoutCreateInfo: return "PipelineLayoutCreateInfo";
    case StructureType::eSamplerCreateInfo: return "SamplerCreateInfo";
    case StructureType::eDescriptorSetLayoutCreateInfo: return "DescriptorSetLayoutCreateInfo";
    case StructureType::eDescriptorPoolCreateInfo: return "DescriptorPoolCreateInfo";
    case StructureType::eDescriptorSetAllocateInfo: return "DescriptorSetAllocateInfo";
    case StructureType::eWriteDescriptorSet: return "WriteDescriptorSet";
    case StructureType::eCopyDescriptorSet: return "CopyDescriptorSet";
    case StructureType::eFramebufferCreateInfo: return "FramebufferCreateInfo";
    case StructureType::eRenderPassCreateInfo: return "RenderPassCreateInfo";
    case StructureType::eCommandPoolCreateInfo: return "CommandPoolCreateInfo";
    case StructureType::eCommandBufferAllocateInfo: return "CommandBufferAllocateInfo";
    case StructureType::eCommandBufferInheritanceInfo: return "CommandBufferInheritanceInfo";
    case StructureType::eCommandBufferBeginInfo: return "CommandBufferBeginInfo";
    case StructureType::eRenderPassBeginInfo: return "RenderPassBeginInfo";
    case StructureType::eBufferMemoryBarrier: return "BufferMemoryBarrier";
    case StructureType::eImageMemoryBarrier: return "ImageMemoryBarrier";
    case StructureType::eMemoryBarrier: return "MemoryBarrier";
    case StructureType::eLoaderInstanceCreateInfo: return "LoaderInstanceCreateInfo";
    case StructureType::eLoaderDeviceCreateInfo: return "LoaderDeviceCreateInfo";
    case StructureType::eSwapchainCreateInfoKHR: return "SwapchainCreateInfoKHR";
    case StructureType::ePresentInfoKHR: return "PresentInfoKHR";
    case StructureType::eDisplayModeCreateInfoKHR: return "DisplayModeCreateInfoKHR";
    case StructureType::eDisplaySurfaceCreateInfoKHR: return "DisplaySurfaceCreateInfoKHR";
    case StructureType::eDisplayPresentInfoKHR: return "DisplayPresentInfoKHR";
    case StructureType::eXlibSurfaceCreateInfoKHR: return "XlibSurfaceCreateInfoKHR";
    case StructureType::eXcbSurfaceCreateInfoKHR: return "XcbSurfaceCreateInfoKHR";
    case StructureType::eWaylandSurfaceCreateInfoKHR: return "WaylandSurfaceCreateInfoKHR";
    case StructureType::eMirSurfaceCreateInfoKHR: return "MirSurfaceCreateInfoKHR";
    case StructureType::eAndroidSurfaceCreateInfoKHR: return "AndroidSurfaceCreateInfoKHR";
    case StructureType::eWin32SurfaceCreateInfoKHR: return "Win32SurfaceCreateInfoKHR";
    case StructureType::eDebugReportCallbackCreateInfoEXT: return "DebugReportCallbackCreateInfoEXT";
    case StructureType::ePipelineRasterizationStateRasterizationOrderAMD: return "PipelineRasterizationStateRasterizationOrderAMD";
    case StructureType::eDebugMarkerObjectNameInfoEXT: return "DebugMarkerObjectNameInfoEXT";
    case StructureType::eDebugMarkerObjectTagInfoEXT: return "DebugMarkerObjectTagInfoEXT";
    case StructureType::eDebugMarkerMarkerInfoEXT: return "DebugMarkerMarkerInfoEXT";
    case StructureType::eDedicatedAllocationImageCreateInfoNV: return "DedicatedAllocationImageCreateInfoNV";
    case StructureType::eDedicatedAllocationBufferCreateInfoNV: return "DedicatedAllocationBufferCreateInfoNV";
    case StructureType::eDedicatedAllocationMemoryAllocateInfoNV: return "DedicatedAllocationMemoryAllocateInfoNV";
    case StructureType::eExternalMemoryImageCreateInfoNV: return "ExternalMemoryImageCreateInfoNV";
    case StructureType::eExportMemoryAllocateInfoNV: return "ExportMemoryAllocateInfoNV";
    case StructureType::eImportMemoryWin32HandleInfoNV: return "ImportMemoryWin32HandleInfoNV";
    case StructureType::eExportMemoryWin32HandleInfoNV: return "ExportMemoryWin32HandleInfoNV";
    case StructureType::eWin32KeyedMutexAcquireReleaseInfoNV: return "Win32KeyedMutexAcquireReleaseInfoNV";
    case StructureType::ePhysicalDeviceFeatures2KHR: return "PhysicalDeviceFeatures2KHR";
    case StructureType::ePhysicalDeviceProperties2KHR: return "PhysicalDeviceProperties2KHR";
    case StructureType::eFormatProperties2KHR: return "FormatProperties2KHR";
    case StructureType::eImageFormatProperties2KHR: return "ImageFormatProperties2KHR";
    case StructureType::ePhysicalDeviceImageFormatInfo2KHR: return "PhysicalDeviceImageFormatInfo2KHR";
    case StructureType::eQueueFamilyProperties2KHR: return "QueueFamilyProperties2KHR";
    case StructureType::ePhysicalDeviceMemoryProperties2KHR: return "PhysicalDeviceMemoryProperties2KHR";
    case StructureType::eSparseImageFormatProperties2KHR: return "SparseImageFormatProperties2KHR";
    case StructureType::ePhysicalDeviceSparseImageFormatInfo2KHR: return "PhysicalDeviceSparseImageFormatInfo2KHR";
    case StructureType::eValidationFlagsEXT: return "ValidationFlagsEXT";
    case StructureType::eViSurfaceCreateInfoNN: return "ViSurfaceCreateInfoNN";
    case StructureType::eObjectTableCreateInfoNVX: return "ObjectTableCreateInfoNVX";
    case StructureType::eIndirectCommandsLayoutCreateInfoNVX: return "IndirectCommandsLayoutCreateInfoNVX";
    case StructureType::eCmdProcessCommandsInfoNVX: return "CmdProcessCommandsInfoNVX";
    case StructureType::eCmdReserveSpaceForCommandsInfoNVX: return "CmdReserveSpaceForCommandsInfoNVX";
    case StructureType::eDeviceGeneratedCommandsLimitsNVX: return "DeviceGeneratedCommandsLimitsNVX";
    case StructureType::eDeviceGeneratedCommandsFeaturesNVX: return "DeviceGeneratedCommandsFeaturesNVX";
    case StructureType::eSurfaceCapabilities2EXT: return "SurfaceCapabilities2EXT";
    case StructureType::eDisplayPowerInfoEXT: return "DisplayPowerInfoEXT";
    case StructureType::eDeviceEventInfoEXT: return "DeviceEventInfoEXT";
    case StructureType::eDisplayEventInfoEXT: return "DisplayEventInfoEXT";
    case StructureType::eSwapchainCounterCreateInfoEXT: return "SwapchainCounterCreateInfoEXT";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SubpassContents value)
  {
    switch (value)
    {
    case SubpassContents::eInline: return "Inline";
    case SubpassContents::eSecondaryCommandBuffers: return "SecondaryCommandBuffers";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DynamicState value)
  {
    switch (value)
    {
    case DynamicState::eViewport: return "Viewport";
    case DynamicState::eScissor: return "Scissor";
    case DynamicState::eLineWidth: return "LineWidth";
    case DynamicState::eDepthBias: return "DepthBias";
    case DynamicState::eBlendConstants: return "BlendConstants";
    case DynamicState::eDepthBounds: return "DepthBounds";
    case DynamicState::eStencilCompareMask: return "StencilCompareMask";
    case DynamicState::eStencilWriteMask: return "StencilWriteMask";
    case DynamicState::eStencilReference: return "StencilReference";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueueFlagBits value)
  {
    switch (value)
    {
    case QueueFlagBits::eGraphics: return "Graphics";
    case QueueFlagBits::eCompute: return "Compute";
    case QueueFlagBits::eTransfer: return "Transfer";
    case QueueFlagBits::eSparseBinding: return "SparseBinding";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueueFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & QueueFlagBits::eGraphics) result += "Graphics | ";
    if (value & QueueFlagBits::eCompute) result += "Compute | ";
    if (value & QueueFlagBits::eTransfer) result += "Transfer | ";
    if (value & QueueFlagBits::eSparseBinding) result += "SparseBinding | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryPropertyFlagBits value)
  {
    switch (value)
    {
    case MemoryPropertyFlagBits::eDeviceLocal: return "DeviceLocal";
    case MemoryPropertyFlagBits::eHostVisible: return "HostVisible";
    case MemoryPropertyFlagBits::eHostCoherent: return "HostCoherent";
    case MemoryPropertyFlagBits::eHostCached: return "HostCached";
    case MemoryPropertyFlagBits::eLazilyAllocated: return "LazilyAllocated";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryPropertyFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & MemoryPropertyFlagBits::eDeviceLocal) result += "DeviceLocal | ";
    if (value & MemoryPropertyFlagBits::eHostVisible) result += "HostVisible | ";
    if (value & MemoryPropertyFlagBits::eHostCoherent) result += "HostCoherent | ";
    if (value & MemoryPropertyFlagBits::eHostCached) result += "HostCached | ";
    if (value & MemoryPropertyFlagBits::eLazilyAllocated) result += "LazilyAllocated | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryHeapFlagBits value)
  {
    switch (value)
    {
    case MemoryHeapFlagBits::eDeviceLocal: return "DeviceLocal";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(MemoryHeapFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & MemoryHeapFlagBits::eDeviceLocal) result += "DeviceLocal | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(AccessFlagBits value)
  {
    switch (value)
    {
    case AccessFlagBits::eIndirectCommandRead: return "IndirectCommandRead";
    case AccessFlagBits::eIndexRead: return "IndexRead";
    case AccessFlagBits::eVertexAttributeRead: return "VertexAttributeRead";
    case AccessFlagBits::eUniformRead: return "UniformRead";
    case AccessFlagBits::eInputAttachmentRead: return "InputAttachmentRead";
    case AccessFlagBits::eShaderRead: return "ShaderRead";
    case AccessFlagBits::eShaderWrite: return "ShaderWrite";
    case AccessFlagBits::eColorAttachmentRead: return "ColorAttachmentRead";
    case AccessFlagBits::eColorAttachmentWrite: return "ColorAttachmentWrite";
    case AccessFlagBits::eDepthStencilAttachmentRead: return "DepthStencilAttachmentRead";
    case AccessFlagBits::eDepthStencilAttachmentWrite: return "DepthStencilAttachmentWrite";
    case AccessFlagBits::eTransferRead: return "TransferRead";
    case AccessFlagBits::eTransferWrite: return "TransferWrite";
    case AccessFlagBits::eHostRead: return "HostRead";
    case AccessFlagBits::eHostWrite: return "HostWrite";
    case AccessFlagBits::eMemoryRead: return "MemoryRead";
    case AccessFlagBits::eMemoryWrite: return "MemoryWrite";
    case AccessFlagBits::eCommandProcessReadNVX: return "CommandProcessReadNVX";
    case AccessFlagBits::eCommandProcessWriteNVX: return "CommandProcessWriteNVX";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(AccessFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & AccessFlagBits::eIndirectCommandRead) result += "IndirectCommandRead | ";
    if (value & AccessFlagBits::eIndexRead) result += "IndexRead | ";
    if (value & AccessFlagBits::eVertexAttributeRead) result += "VertexAttributeRead | ";
    if (value & AccessFlagBits::eUniformRead) result += "UniformRead | ";
    if (value & AccessFlagBits::eInputAttachmentRead) result += "InputAttachmentRead | ";
    if (value & AccessFlagBits::eShaderRead) result += "ShaderRead | ";
    if (value & AccessFlagBits::eShaderWrite) result += "ShaderWrite | ";
    if (value & AccessFlagBits::eColorAttachmentRead) result += "ColorAttachmentRead | ";
    if (value & AccessFlagBits::eColorAttachmentWrite) result += "ColorAttachmentWrite | ";
    if (value & AccessFlagBits::eDepthStencilAttachmentRead) result += "DepthStencilAttachmentRead | ";
    if (value & AccessFlagBits::eDepthStencilAttachmentWrite) result += "DepthStencilAttachmentWrite | ";
    if (value & AccessFlagBits::eTransferRead) result += "TransferRead | ";
    if (value & AccessFlagBits::eTransferWrite) result += "TransferWrite | ";
    if (value & AccessFlagBits::eHostRead) result += "HostRead | ";
    if (value & AccessFlagBits::eHostWrite) result += "HostWrite | ";
    if (value & AccessFlagBits::eMemoryRead) result += "MemoryRead | ";
    if (value & AccessFlagBits::eMemoryWrite) result += "MemoryWrite | ";
    if (value & AccessFlagBits::eCommandProcessReadNVX) result += "CommandProcessReadNVX | ";
    if (value & AccessFlagBits::eCommandProcessWriteNVX) result += "CommandProcessWriteNVX | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(BufferUsageFlagBits value)
  {
    switch (value)
    {
    case BufferUsageFlagBits::eTransferSrc: return "TransferSrc";
    case BufferUsageFlagBits::eTransferDst: return "TransferDst";
    case BufferUsageFlagBits::eUniformTexelBuffer: return "UniformTexelBuffer";
    case BufferUsageFlagBits::eStorageTexelBuffer: return "StorageTexelBuffer";
    case BufferUsageFlagBits::eUniformBuffer: return "UniformBuffer";
    case BufferUsageFlagBits::eStorageBuffer: return "StorageBuffer";
    case BufferUsageFlagBits::eIndexBuffer: return "IndexBuffer";
    case BufferUsageFlagBits::eVertexBuffer: return "VertexBuffer";
    case BufferUsageFlagBits::eIndirectBuffer: return "IndirectBuffer";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(BufferUsageFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & BufferUsageFlagBits::eTransferSrc) result += "TransferSrc | ";
    if (value & BufferUsageFlagBits::eTransferDst) result += "TransferDst | ";
    if (value & BufferUsageFlagBits::eUniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & BufferUsageFlagBits::eStorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & BufferUsageFlagBits::eUniformBuffer) result += "UniformBuffer | ";
    if (value & BufferUsageFlagBits::eStorageBuffer) result += "StorageBuffer | ";
    if (value & BufferUsageFlagBits::eIndexBuffer) result += "IndexBuffer | ";
    if (value & BufferUsageFlagBits::eVertexBuffer) result += "VertexBuffer | ";
    if (value & BufferUsageFlagBits::eIndirectBuffer) result += "IndirectBuffer | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(BufferCreateFlagBits value)
  {
    switch (value)
    {
    case BufferCreateFlagBits::eSparseBinding: return "SparseBinding";
    case BufferCreateFlagBits::eSparseResidency: return "SparseResidency";
    case BufferCreateFlagBits::eSparseAliased: return "SparseAliased";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(BufferCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & BufferCreateFlagBits::eSparseBinding) result += "SparseBinding | ";
    if (value & BufferCreateFlagBits::eSparseResidency) result += "SparseResidency | ";
    if (value & BufferCreateFlagBits::eSparseAliased) result += "SparseAliased | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ShaderStageFlagBits value)
  {
    switch (value)
    {
    case ShaderStageFlagBits::eVertex: return "Vertex";
    case ShaderStageFlagBits::eTessellationControl: return "TessellationControl";
    case ShaderStageFlagBits::eTessellationEvaluation: return "TessellationEvaluation";
    case ShaderStageFlagBits::eGeometry: return "Geometry";
    case ShaderStageFlagBits::eFragment: return "Fragment";
    case ShaderStageFlagBits::eCompute: return "Compute";
    case ShaderStageFlagBits::eAllGraphics: return "AllGraphics";
    case ShaderStageFlagBits::eAll: return "All";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ShaderStageFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ShaderStageFlagBits::eVertex) result += "Vertex | ";
    if (value & ShaderStageFlagBits::eTessellationControl) result += "TessellationControl | ";
    if (value & ShaderStageFlagBits::eTessellationEvaluation) result += "TessellationEvaluation | ";
    if (value & ShaderStageFlagBits::eGeometry) result += "Geometry | ";
    if (value & ShaderStageFlagBits::eFragment) result += "Fragment | ";
    if (value & ShaderStageFlagBits::eCompute) result += "Compute | ";
    if (value & ShaderStageFlagBits::eAllGraphics) result += "AllGraphics | ";
    if (value & ShaderStageFlagBits::eAll) result += "All | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageUsageFlagBits value)
  {
    switch (value)
    {
    case ImageUsageFlagBits::eTransferSrc: return "TransferSrc";
    case ImageUsageFlagBits::eTransferDst: return "TransferDst";
    case ImageUsageFlagBits::eSampled: return "Sampled";
    case ImageUsageFlagBits::eStorage: return "Storage";
    case ImageUsageFlagBits::eColorAttachment: return "ColorAttachment";
    case ImageUsageFlagBits::eDepthStencilAttachment: return "DepthStencilAttachment";
    case ImageUsageFlagBits::eTransientAttachment: return "TransientAttachment";
    case ImageUsageFlagBits::eInputAttachment: return "InputAttachment";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageUsageFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ImageUsageFlagBits::eTransferSrc) result += "TransferSrc | ";
    if (value & ImageUsageFlagBits::eTransferDst) result += "TransferDst | ";
    if (value & ImageUsageFlagBits::eSampled) result += "Sampled | ";
    if (value & ImageUsageFlagBits::eStorage) result += "Storage | ";
    if (value & ImageUsageFlagBits::eColorAttachment) result += "ColorAttachment | ";
    if (value & ImageUsageFlagBits::eDepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & ImageUsageFlagBits::eTransientAttachment) result += "TransientAttachment | ";
    if (value & ImageUsageFlagBits::eInputAttachment) result += "InputAttachment | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageCreateFlagBits value)
  {
    switch (value)
    {
    case ImageCreateFlagBits::eSparseBinding: return "SparseBinding";
    case ImageCreateFlagBits::eSparseResidency: return "SparseResidency";
    case ImageCreateFlagBits::eSparseAliased: return "SparseAliased";
    case ImageCreateFlagBits::eMutableFormat: return "MutableFormat";
    case ImageCreateFlagBits::eCubeCompatible: return "CubeCompatible";
    case ImageCreateFlagBits::e2DArrayCompatibleKHR: return "2DArrayCompatibleKHR";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ImageCreateFlagBits::eSparseBinding) result += "SparseBinding | ";
    if (value & ImageCreateFlagBits::eSparseResidency) result += "SparseResidency | ";
    if (value & ImageCreateFlagBits::eSparseAliased) result += "SparseAliased | ";
    if (value & ImageCreateFlagBits::eMutableFormat) result += "MutableFormat | ";
    if (value & ImageCreateFlagBits::eCubeCompatible) result += "CubeCompatible | ";
    if (value & ImageCreateFlagBits::e2DArrayCompatibleKHR) result += "2DArrayCompatibleKHR | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineCreateFlagBits value)
  {
    switch (value)
    {
    case PipelineCreateFlagBits::eDisableOptimization: return "DisableOptimization";
    case PipelineCreateFlagBits::eAllowDerivatives: return "AllowDerivatives";
    case PipelineCreateFlagBits::eDerivative: return "Derivative";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & PipelineCreateFlagBits::eDisableOptimization) result += "DisableOptimization | ";
    if (value & PipelineCreateFlagBits::eAllowDerivatives) result += "AllowDerivatives | ";
    if (value & PipelineCreateFlagBits::eDerivative) result += "Derivative | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ColorComponentFlagBits value)
  {
    switch (value)
    {
    case ColorComponentFlagBits::eR: return "R";
    case ColorComponentFlagBits::eG: return "G";
    case ColorComponentFlagBits::eB: return "B";
    case ColorComponentFlagBits::eA: return "A";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ColorComponentFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ColorComponentFlagBits::eR) result += "R | ";
    if (value & ColorComponentFlagBits::eG) result += "G | ";
    if (value & ColorComponentFlagBits::eB) result += "B | ";
    if (value & ColorComponentFlagBits::eA) result += "A | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(FenceCreateFlagBits value)
  {
    switch (value)
    {
    case FenceCreateFlagBits::eSignaled: return "Signaled";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(FenceCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & FenceCreateFlagBits::eSignaled) result += "Signaled | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(FormatFeatureFlagBits value)
  {
    switch (value)
    {
    case FormatFeatureFlagBits::eSampledImage: return "SampledImage";
    case FormatFeatureFlagBits::eStorageImage: return "StorageImage";
    case FormatFeatureFlagBits::eStorageImageAtomic: return "StorageImageAtomic";
    case FormatFeatureFlagBits::eUniformTexelBuffer: return "UniformTexelBuffer";
    case FormatFeatureFlagBits::eStorageTexelBuffer: return "StorageTexelBuffer";
    case FormatFeatureFlagBits::eStorageTexelBufferAtomic: return "StorageTexelBufferAtomic";
    case FormatFeatureFlagBits::eVertexBuffer: return "VertexBuffer";
    case FormatFeatureFlagBits::eColorAttachment: return "ColorAttachment";
    case FormatFeatureFlagBits::eColorAttachmentBlend: return "ColorAttachmentBlend";
    case FormatFeatureFlagBits::eDepthStencilAttachment: return "DepthStencilAttachment";
    case FormatFeatureFlagBits::eBlitSrc: return "BlitSrc";
    case FormatFeatureFlagBits::eBlitDst: return "BlitDst";
    case FormatFeatureFlagBits::eSampledImageFilterLinear: return "SampledImageFilterLinear";
    case FormatFeatureFlagBits::eSampledImageFilterCubicIMG: return "SampledImageFilterCubicIMG";
    case FormatFeatureFlagBits::eTransferSrcKHR: return "TransferSrcKHR";
    case FormatFeatureFlagBits::eTransferDstKHR: return "TransferDstKHR";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(FormatFeatureFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & FormatFeatureFlagBits::eSampledImage) result += "SampledImage | ";
    if (value & FormatFeatureFlagBits::eStorageImage) result += "StorageImage | ";
    if (value & FormatFeatureFlagBits::eStorageImageAtomic) result += "StorageImageAtomic | ";
    if (value & FormatFeatureFlagBits::eUniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & FormatFeatureFlagBits::eStorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & FormatFeatureFlagBits::eStorageTexelBufferAtomic) result += "StorageTexelBufferAtomic | ";
    if (value & FormatFeatureFlagBits::eVertexBuffer) result += "VertexBuffer | ";
    if (value & FormatFeatureFlagBits::eColorAttachment) result += "ColorAttachment | ";
    if (value & FormatFeatureFlagBits::eColorAttachmentBlend) result += "ColorAttachmentBlend | ";
    if (value & FormatFeatureFlagBits::eDepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & FormatFeatureFlagBits::eBlitSrc) result += "BlitSrc | ";
    if (value & FormatFeatureFlagBits::eBlitDst) result += "BlitDst | ";
    if (value & FormatFeatureFlagBits::eSampledImageFilterLinear) result += "SampledImageFilterLinear | ";
    if (value & FormatFeatureFlagBits::eSampledImageFilterCubicIMG) result += "SampledImageFilterCubicIMG | ";
    if (value & FormatFeatureFlagBits::eTransferSrcKHR) result += "TransferSrcKHR | ";
    if (value & FormatFeatureFlagBits::eTransferDstKHR) result += "TransferDstKHR | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(QueryControlFlagBits value)
  {
    switch (value)
    {
    case QueryControlFlagBits::ePrecise: return "Precise";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueryControlFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & QueryControlFlagBits::ePrecise) result += "Precise | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(QueryResultFlagBits value)
  {
    switch (value)
    {
    case QueryResultFlagBits::e64: return "64";
    case QueryResultFlagBits::eWait: return "Wait";
    case QueryResultFlagBits::eWithAvailability: return "WithAvailability";
    case QueryResultFlagBits::ePartial: return "Partial";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueryResultFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & QueryResultFlagBits::e64) result += "64 | ";
    if (value & QueryResultFlagBits::eWait) result += "Wait | ";
    if (value & QueryResultFlagBits::eWithAvailability) result += "WithAvailability | ";
    if (value & QueryResultFlagBits::ePartial) result += "Partial | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(CommandBufferUsageFlagBits value)
  {
    switch (value)
    {
    case CommandBufferUsageFlagBits::eOneTimeSubmit: return "OneTimeSubmit";
    case CommandBufferUsageFlagBits::eRenderPassContinue: return "RenderPassContinue";
    case CommandBufferUsageFlagBits::eSimultaneousUse: return "SimultaneousUse";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CommandBufferUsageFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CommandBufferUsageFlagBits::eOneTimeSubmit) result += "OneTimeSubmit | ";
    if (value & CommandBufferUsageFlagBits::eRenderPassContinue) result += "RenderPassContinue | ";
    if (value & CommandBufferUsageFlagBits::eSimultaneousUse) result += "SimultaneousUse | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(QueryPipelineStatisticFlagBits value)
  {
    switch (value)
    {
    case QueryPipelineStatisticFlagBits::eInputAssemblyVertices: return "InputAssemblyVertices";
    case QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives: return "InputAssemblyPrimitives";
    case QueryPipelineStatisticFlagBits::eVertexShaderInvocations: return "VertexShaderInvocations";
    case QueryPipelineStatisticFlagBits::eGeometryShaderInvocations: return "GeometryShaderInvocations";
    case QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives: return "GeometryShaderPrimitives";
    case QueryPipelineStatisticFlagBits::eClippingInvocations: return "ClippingInvocations";
    case QueryPipelineStatisticFlagBits::eClippingPrimitives: return "ClippingPrimitives";
    case QueryPipelineStatisticFlagBits::eFragmentShaderInvocations: return "FragmentShaderInvocations";
    case QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches: return "TessellationControlShaderPatches";
    case QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations: return "TessellationEvaluationShaderInvocations";
    case QueryPipelineStatisticFlagBits::eComputeShaderInvocations: return "ComputeShaderInvocations";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(QueryPipelineStatisticFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & QueryPipelineStatisticFlagBits::eInputAssemblyVertices) result += "InputAssemblyVertices | ";
    if (value & QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives) result += "InputAssemblyPrimitives | ";
    if (value & QueryPipelineStatisticFlagBits::eVertexShaderInvocations) result += "VertexShaderInvocations | ";
    if (value & QueryPipelineStatisticFlagBits::eGeometryShaderInvocations) result += "GeometryShaderInvocations | ";
    if (value & QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives) result += "GeometryShaderPrimitives | ";
    if (value & QueryPipelineStatisticFlagBits::eClippingInvocations) result += "ClippingInvocations | ";
    if (value & QueryPipelineStatisticFlagBits::eClippingPrimitives) result += "ClippingPrimitives | ";
    if (value & QueryPipelineStatisticFlagBits::eFragmentShaderInvocations) result += "FragmentShaderInvocations | ";
    if (value & QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches) result += "TessellationControlShaderPatches | ";
    if (value & QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations) result += "TessellationEvaluationShaderInvocations | ";
    if (value & QueryPipelineStatisticFlagBits::eComputeShaderInvocations) result += "ComputeShaderInvocations | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ImageAspectFlagBits value)
  {
    switch (value)
    {
    case ImageAspectFlagBits::eColor: return "Color";
    case ImageAspectFlagBits::eDepth: return "Depth";
    case ImageAspectFlagBits::eStencil: return "Stencil";
    case ImageAspectFlagBits::eMetadata: return "Metadata";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ImageAspectFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ImageAspectFlagBits::eColor) result += "Color | ";
    if (value & ImageAspectFlagBits::eDepth) result += "Depth | ";
    if (value & ImageAspectFlagBits::eStencil) result += "Stencil | ";
    if (value & ImageAspectFlagBits::eMetadata) result += "Metadata | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(SparseImageFormatFlagBits value)
  {
    switch (value)
    {
    case SparseImageFormatFlagBits::eSingleMiptail: return "SingleMiptail";
    case SparseImageFormatFlagBits::eAlignedMipSize: return "AlignedMipSize";
    case SparseImageFormatFlagBits::eNonstandardBlockSize: return "NonstandardBlockSize";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SparseImageFormatFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & SparseImageFormatFlagBits::eSingleMiptail) result += "SingleMiptail | ";
    if (value & SparseImageFormatFlagBits::eAlignedMipSize) result += "AlignedMipSize | ";
    if (value & SparseImageFormatFlagBits::eNonstandardBlockSize) result += "NonstandardBlockSize | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(SparseMemoryBindFlagBits value)
  {
    switch (value)
    {
    case SparseMemoryBindFlagBits::eMetadata: return "Metadata";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SparseMemoryBindFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & SparseMemoryBindFlagBits::eMetadata) result += "Metadata | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineStageFlagBits value)
  {
    switch (value)
    {
    case PipelineStageFlagBits::eTopOfPipe: return "TopOfPipe";
    case PipelineStageFlagBits::eDrawIndirect: return "DrawIndirect";
    case PipelineStageFlagBits::eVertexInput: return "VertexInput";
    case PipelineStageFlagBits::eVertexShader: return "VertexShader";
    case PipelineStageFlagBits::eTessellationControlShader: return "TessellationControlShader";
    case PipelineStageFlagBits::eTessellationEvaluationShader: return "TessellationEvaluationShader";
    case PipelineStageFlagBits::eGeometryShader: return "GeometryShader";
    case PipelineStageFlagBits::eFragmentShader: return "FragmentShader";
    case PipelineStageFlagBits::eEarlyFragmentTests: return "EarlyFragmentTests";
    case PipelineStageFlagBits::eLateFragmentTests: return "LateFragmentTests";
    case PipelineStageFlagBits::eColorAttachmentOutput: return "ColorAttachmentOutput";
    case PipelineStageFlagBits::eComputeShader: return "ComputeShader";
    case PipelineStageFlagBits::eTransfer: return "Transfer";
    case PipelineStageFlagBits::eBottomOfPipe: return "BottomOfPipe";
    case PipelineStageFlagBits::eHost: return "Host";
    case PipelineStageFlagBits::eAllGraphics: return "AllGraphics";
    case PipelineStageFlagBits::eAllCommands: return "AllCommands";
    case PipelineStageFlagBits::eCommandProcessNVX: return "CommandProcessNVX";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(PipelineStageFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & PipelineStageFlagBits::eTopOfPipe) result += "TopOfPipe | ";
    if (value & PipelineStageFlagBits::eDrawIndirect) result += "DrawIndirect | ";
    if (value & PipelineStageFlagBits::eVertexInput) result += "VertexInput | ";
    if (value & PipelineStageFlagBits::eVertexShader) result += "VertexShader | ";
    if (value & PipelineStageFlagBits::eTessellationControlShader) result += "TessellationControlShader | ";
    if (value & PipelineStageFlagBits::eTessellationEvaluationShader) result += "TessellationEvaluationShader | ";
    if (value & PipelineStageFlagBits::eGeometryShader) result += "GeometryShader | ";
    if (value & PipelineStageFlagBits::eFragmentShader) result += "FragmentShader | ";
    if (value & PipelineStageFlagBits::eEarlyFragmentTests) result += "EarlyFragmentTests | ";
    if (value & PipelineStageFlagBits::eLateFragmentTests) result += "LateFragmentTests | ";
    if (value & PipelineStageFlagBits::eColorAttachmentOutput) result += "ColorAttachmentOutput | ";
    if (value & PipelineStageFlagBits::eComputeShader) result += "ComputeShader | ";
    if (value & PipelineStageFlagBits::eTransfer) result += "Transfer | ";
    if (value & PipelineStageFlagBits::eBottomOfPipe) result += "BottomOfPipe | ";
    if (value & PipelineStageFlagBits::eHost) result += "Host | ";
    if (value & PipelineStageFlagBits::eAllGraphics) result += "AllGraphics | ";
    if (value & PipelineStageFlagBits::eAllCommands) result += "AllCommands | ";
    if (value & PipelineStageFlagBits::eCommandProcessNVX) result += "CommandProcessNVX | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(CommandPoolCreateFlagBits value)
  {
    switch (value)
    {
    case CommandPoolCreateFlagBits::eTransient: return "Transient";
    case CommandPoolCreateFlagBits::eResetCommandBuffer: return "ResetCommandBuffer";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CommandPoolCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CommandPoolCreateFlagBits::eTransient) result += "Transient | ";
    if (value & CommandPoolCreateFlagBits::eResetCommandBuffer) result += "ResetCommandBuffer | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(CommandPoolResetFlagBits value)
  {
    switch (value)
    {
    case CommandPoolResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CommandPoolResetFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CommandPoolResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(CommandBufferResetFlagBits value)
  {
    switch (value)
    {
    case CommandBufferResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CommandBufferResetFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CommandBufferResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(SampleCountFlagBits value)
  {
    switch (value)
    {
    case SampleCountFlagBits::e1: return "1";
    case SampleCountFlagBits::e2: return "2";
    case SampleCountFlagBits::e4: return "4";
    case SampleCountFlagBits::e8: return "8";
    case SampleCountFlagBits::e16: return "16";
    case SampleCountFlagBits::e32: return "32";
    case SampleCountFlagBits::e64: return "64";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SampleCountFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & SampleCountFlagBits::e1) result += "1 | ";
    if (value & SampleCountFlagBits::e2) result += "2 | ";
    if (value & SampleCountFlagBits::e4) result += "4 | ";
    if (value & SampleCountFlagBits::e8) result += "8 | ";
    if (value & SampleCountFlagBits::e16) result += "16 | ";
    if (value & SampleCountFlagBits::e32) result += "32 | ";
    if (value & SampleCountFlagBits::e64) result += "64 | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(AttachmentDescriptionFlagBits value)
  {
    switch (value)
    {
    case AttachmentDescriptionFlagBits::eMayAlias: return "MayAlias";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(AttachmentDescriptionFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & AttachmentDescriptionFlagBits::eMayAlias) result += "MayAlias | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(StencilFaceFlagBits value)
  {
    switch (value)
    {
    case StencilFaceFlagBits::eFront: return "Front";
    case StencilFaceFlagBits::eBack: return "Back";
    case StencilFaceFlagBits::eVkStencilFrontAndBack: return "VkStencilFrontAndBack";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(StencilFaceFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & StencilFaceFlagBits::eFront) result += "Front | ";
    if (value & StencilFaceFlagBits::eBack) result += "Back | ";
    if (value & StencilFaceFlagBits::eVkStencilFrontAndBack) result += "VkStencilFrontAndBack | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorPoolCreateFlagBits value)
  {
    switch (value)
    {
    case DescriptorPoolCreateFlagBits::eFreeDescriptorSet: return "FreeDescriptorSet";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DescriptorPoolCreateFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & DescriptorPoolCreateFlagBits::eFreeDescriptorSet) result += "FreeDescriptorSet | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(DependencyFlagBits value)
  {
    switch (value)
    {
    case DependencyFlagBits::eByRegion: return "ByRegion";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DependencyFlags value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & DependencyFlagBits::eByRegion) result += "ByRegion | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(PresentModeKHR value)
  {
    switch (value)
    {
    case PresentModeKHR::eImmediate: return "Immediate";
    case PresentModeKHR::eMailbox: return "Mailbox";
    case PresentModeKHR::eFifo: return "Fifo";
    case PresentModeKHR::eFifoRelaxed: return "FifoRelaxed";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ColorSpaceKHR value)
  {
    switch (value)
    {
    case ColorSpaceKHR::eSrgbNonlinear: return "SrgbNonlinear";
    case ColorSpaceKHR::eDisplayP3LinearEXT: return "DisplayP3LinearEXT";
    case ColorSpaceKHR::eDisplayP3NonlinearEXT: return "DisplayP3NonlinearEXT";
    case ColorSpaceKHR::eScrgbLinearEXT: return "ScrgbLinearEXT";
    case ColorSpaceKHR::eScrgbNonlinearEXT: return "ScrgbNonlinearEXT";
    case ColorSpaceKHR::eDciP3LinearEXT: return "DciP3LinearEXT";
    case ColorSpaceKHR::eDciP3NonlinearEXT: return "DciP3NonlinearEXT";
    case ColorSpaceKHR::eBt709LinearEXT: return "Bt709LinearEXT";
    case ColorSpaceKHR::eBt709NonlinearEXT: return "Bt709NonlinearEXT";
    case ColorSpaceKHR::eBt2020LinearEXT: return "Bt2020LinearEXT";
    case ColorSpaceKHR::eBt2020NonlinearEXT: return "Bt2020NonlinearEXT";
    case ColorSpaceKHR::eAdobergbLinearEXT: return "AdobergbLinearEXT";
    case ColorSpaceKHR::eAdobergbNonlinearEXT: return "AdobergbNonlinearEXT";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayPlaneAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case DisplayPlaneAlphaFlagBitsKHR::eOpaque: return "Opaque";
    case DisplayPlaneAlphaFlagBitsKHR::eGlobal: return "Global";
    case DisplayPlaneAlphaFlagBitsKHR::ePerPixel: return "PerPixel";
    case DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied: return "PerPixelPremultiplied";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayPlaneAlphaFlagsKHR value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & DisplayPlaneAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & DisplayPlaneAlphaFlagBitsKHR::eGlobal) result += "Global | ";
    if (value & DisplayPlaneAlphaFlagBitsKHR::ePerPixel) result += "PerPixel | ";
    if (value & DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied) result += "PerPixelPremultiplied | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(CompositeAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case CompositeAlphaFlagBitsKHR::eOpaque: return "Opaque";
    case CompositeAlphaFlagBitsKHR::ePreMultiplied: return "PreMultiplied";
    case CompositeAlphaFlagBitsKHR::ePostMultiplied: return "PostMultiplied";
    case CompositeAlphaFlagBitsKHR::eInherit: return "Inherit";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(CompositeAlphaFlagsKHR value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & CompositeAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & CompositeAlphaFlagBitsKHR::ePreMultiplied) result += "PreMultiplied | ";
    if (value & CompositeAlphaFlagBitsKHR::ePostMultiplied) result += "PostMultiplied | ";
    if (value & CompositeAlphaFlagBitsKHR::eInherit) result += "Inherit | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(SurfaceTransformFlagBitsKHR value)
  {
    switch (value)
    {
    case SurfaceTransformFlagBitsKHR::eIdentity: return "Identity";
    case SurfaceTransformFlagBitsKHR::eRotate90: return "Rotate90";
    case SurfaceTransformFlagBitsKHR::eRotate180: return "Rotate180";
    case SurfaceTransformFlagBitsKHR::eRotate270: return "Rotate270";
    case SurfaceTransformFlagBitsKHR::eHorizontalMirror: return "HorizontalMirror";
    case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90: return "HorizontalMirrorRotate90";
    case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180: return "HorizontalMirrorRotate180";
    case SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270: return "HorizontalMirrorRotate270";
    case SurfaceTransformFlagBitsKHR::eInherit: return "Inherit";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SurfaceTransformFlagsKHR value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & SurfaceTransformFlagBitsKHR::eIdentity) result += "Identity | ";
    if (value & SurfaceTransformFlagBitsKHR::eRotate90) result += "Rotate90 | ";
    if (value & SurfaceTransformFlagBitsKHR::eRotate180) result += "Rotate180 | ";
    if (value & SurfaceTransformFlagBitsKHR::eRotate270) result += "Rotate270 | ";
    if (value & SurfaceTransformFlagBitsKHR::eHorizontalMirror) result += "HorizontalMirror | ";
    if (value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) result += "HorizontalMirrorRotate90 | ";
    if (value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) result += "HorizontalMirrorRotate180 | ";
    if (value & SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) result += "HorizontalMirrorRotate270 | ";
    if (value & SurfaceTransformFlagBitsKHR::eInherit) result += "Inherit | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(DebugReportFlagBitsEXT value)
  {
    switch (value)
    {
    case DebugReportFlagBitsEXT::eInformation: return "Information";
    case DebugReportFlagBitsEXT::eWarning: return "Warning";
    case DebugReportFlagBitsEXT::ePerformanceWarning: return "PerformanceWarning";
    case DebugReportFlagBitsEXT::eError: return "Error";
    case DebugReportFlagBitsEXT::eDebug: return "Debug";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DebugReportFlagsEXT value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & DebugReportFlagBitsEXT::eInformation) result += "Information | ";
    if (value & DebugReportFlagBitsEXT::eWarning) result += "Warning | ";
    if (value & DebugReportFlagBitsEXT::ePerformanceWarning) result += "PerformanceWarning | ";
    if (value & DebugReportFlagBitsEXT::eError) result += "Error | ";
    if (value & DebugReportFlagBitsEXT::eDebug) result += "Debug | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(DebugReportObjectTypeEXT value)
  {
    switch (value)
    {
    case DebugReportObjectTypeEXT::eUnknown: return "Unknown";
    case DebugReportObjectTypeEXT::eInstance: return "Instance";
    case DebugReportObjectTypeEXT::ePhysicalDevice: return "PhysicalDevice";
    case DebugReportObjectTypeEXT::eDevice: return "Device";
    case DebugReportObjectTypeEXT::eQueue: return "Queue";
    case DebugReportObjectTypeEXT::eSemaphore: return "Semaphore";
    case DebugReportObjectTypeEXT::eCommandBuffer: return "CommandBuffer";
    case DebugReportObjectTypeEXT::eFence: return "Fence";
    case DebugReportObjectTypeEXT::eDeviceMemory: return "DeviceMemory";
    case DebugReportObjectTypeEXT::eBuffer: return "Buffer";
    case DebugReportObjectTypeEXT::eImage: return "Image";
    case DebugReportObjectTypeEXT::eEvent: return "Event";
    case DebugReportObjectTypeEXT::eQueryPool: return "QueryPool";
    case DebugReportObjectTypeEXT::eBufferView: return "BufferView";
    case DebugReportObjectTypeEXT::eImageView: return "ImageView";
    case DebugReportObjectTypeEXT::eShaderModule: return "ShaderModule";
    case DebugReportObjectTypeEXT::ePipelineCache: return "PipelineCache";
    case DebugReportObjectTypeEXT::ePipelineLayout: return "PipelineLayout";
    case DebugReportObjectTypeEXT::eRenderPass: return "RenderPass";
    case DebugReportObjectTypeEXT::ePipeline: return "Pipeline";
    case DebugReportObjectTypeEXT::eDescriptorSetLayout: return "DescriptorSetLayout";
    case DebugReportObjectTypeEXT::eSampler: return "Sampler";
    case DebugReportObjectTypeEXT::eDescriptorPool: return "DescriptorPool";
    case DebugReportObjectTypeEXT::eDescriptorSet: return "DescriptorSet";
    case DebugReportObjectTypeEXT::eFramebuffer: return "Framebuffer";
    case DebugReportObjectTypeEXT::eCommandPool: return "CommandPool";
    case DebugReportObjectTypeEXT::eSurfaceKhr: return "SurfaceKhr";
    case DebugReportObjectTypeEXT::eSwapchainKhr: return "SwapchainKhr";
    case DebugReportObjectTypeEXT::eDebugReport: return "DebugReport";
    case DebugReportObjectTypeEXT::eDisplayKhr: return "DisplayKhr";
    case DebugReportObjectTypeEXT::eDisplayModeKhr: return "DisplayModeKhr";
    case DebugReportObjectTypeEXT::eObjectTableNvx: return "ObjectTableNvx";
    case DebugReportObjectTypeEXT::eIndirectCommandsLayoutNvx: return "IndirectCommandsLayoutNvx";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DebugReportErrorEXT value)
  {
    switch (value)
    {
    case DebugReportErrorEXT::eNone: return "None";
    case DebugReportErrorEXT::eCallbackRef: return "CallbackRef";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(RasterizationOrderAMD value)
  {
    switch (value)
    {
    case RasterizationOrderAMD::eStrict: return "Strict";
    case RasterizationOrderAMD::eRelaxed: return "Relaxed";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ExternalMemoryHandleTypeFlagBitsNV value)
  {
    switch (value)
    {
    case ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32: return "OpaqueWin32";
    case ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32Kmt: return "OpaqueWin32Kmt";
    case ExternalMemoryHandleTypeFlagBitsNV::eD3D11Image: return "D3D11Image";
    case ExternalMemoryHandleTypeFlagBitsNV::eD3D11ImageKmt: return "D3D11ImageKmt";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ExternalMemoryHandleTypeFlagsNV value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32) result += "OpaqueWin32 | ";
    if (value & ExternalMemoryHandleTypeFlagBitsNV::eOpaqueWin32Kmt) result += "OpaqueWin32Kmt | ";
    if (value & ExternalMemoryHandleTypeFlagBitsNV::eD3D11Image) result += "D3D11Image | ";
    if (value & ExternalMemoryHandleTypeFlagBitsNV::eD3D11ImageKmt) result += "D3D11ImageKmt | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ExternalMemoryFeatureFlagBitsNV value)
  {
    switch (value)
    {
    case ExternalMemoryFeatureFlagBitsNV::eDedicatedOnly: return "DedicatedOnly";
    case ExternalMemoryFeatureFlagBitsNV::eExportable: return "Exportable";
    case ExternalMemoryFeatureFlagBitsNV::eImportable: return "Importable";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ExternalMemoryFeatureFlagsNV value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ExternalMemoryFeatureFlagBitsNV::eDedicatedOnly) result += "DedicatedOnly | ";
    if (value & ExternalMemoryFeatureFlagBitsNV::eExportable) result += "Exportable | ";
    if (value & ExternalMemoryFeatureFlagBitsNV::eImportable) result += "Importable | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ValidationCheckEXT value)
  {
    switch (value)
    {
    case ValidationCheckEXT::eAll: return "All";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(IndirectCommandsLayoutUsageFlagBitsNVX value)
  {
    switch (value)
    {
    case IndirectCommandsLayoutUsageFlagBitsNVX::eUnorderedSequences: return "UnorderedSequences";
    case IndirectCommandsLayoutUsageFlagBitsNVX::eSparseSequences: return "SparseSequences";
    case IndirectCommandsLayoutUsageFlagBitsNVX::eEmptyExecutions: return "EmptyExecutions";
    case IndirectCommandsLayoutUsageFlagBitsNVX::eIndexedSequences: return "IndexedSequences";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(IndirectCommandsLayoutUsageFlagsNVX value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & IndirectCommandsLayoutUsageFlagBitsNVX::eUnorderedSequences) result += "UnorderedSequences | ";
    if (value & IndirectCommandsLayoutUsageFlagBitsNVX::eSparseSequences) result += "SparseSequences | ";
    if (value & IndirectCommandsLayoutUsageFlagBitsNVX::eEmptyExecutions) result += "EmptyExecutions | ";
    if (value & IndirectCommandsLayoutUsageFlagBitsNVX::eIndexedSequences) result += "IndexedSequences | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(ObjectEntryUsageFlagBitsNVX value)
  {
    switch (value)
    {
    case ObjectEntryUsageFlagBitsNVX::eGraphics: return "Graphics";
    case ObjectEntryUsageFlagBitsNVX::eCompute: return "Compute";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ObjectEntryUsageFlagsNVX value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & ObjectEntryUsageFlagBitsNVX::eGraphics) result += "Graphics | ";
    if (value & ObjectEntryUsageFlagBitsNVX::eCompute) result += "Compute | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(IndirectCommandsTokenTypeNVX value)
  {
    switch (value)
    {
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenPipeline: return "VkIndirectCommandsTokenPipeline";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenDescriptorSet: return "VkIndirectCommandsTokenDescriptorSet";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenIndexBuffer: return "VkIndirectCommandsTokenIndexBuffer";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenVertexBuffer: return "VkIndirectCommandsTokenVertexBuffer";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenPushConstant: return "VkIndirectCommandsTokenPushConstant";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenDrawIndexed: return "VkIndirectCommandsTokenDrawIndexed";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenDraw: return "VkIndirectCommandsTokenDraw";
    case IndirectCommandsTokenTypeNVX::eVkIndirectCommandsTokenDispatch: return "VkIndirectCommandsTokenDispatch";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(ObjectEntryTypeNVX value)
  {
    switch (value)
    {
    case ObjectEntryTypeNVX::eVkObjectEntryDescriptorSet: return "VkObjectEntryDescriptorSet";
    case ObjectEntryTypeNVX::eVkObjectEntryPipeline: return "VkObjectEntryPipeline";
    case ObjectEntryTypeNVX::eVkObjectEntryIndexBuffer: return "VkObjectEntryIndexBuffer";
    case ObjectEntryTypeNVX::eVkObjectEntryVertexBuffer: return "VkObjectEntryVertexBuffer";
    case ObjectEntryTypeNVX::eVkObjectEntryPushConstant: return "VkObjectEntryPushConstant";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SurfaceCounterFlagBitsEXT value)
  {
    switch (value)
    {
    case SurfaceCounterFlagBitsEXT::eVblankExt: return "VblankExt";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(SurfaceCounterFlagsEXT value)
  {
    if (!value) return "{}";
    std::string result;
    if (value & SurfaceCounterFlagBitsEXT::eVblankExt) result += "VblankExt | ";
    return "{" + result.substr(0, result.size() - 3) + "}";
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayPowerStateEXT value)
  {
    switch (value)
    {
    case DisplayPowerStateEXT::eOff: return "Off";
    case DisplayPowerStateEXT::eSuspend: return "Suspend";
    case DisplayPowerStateEXT::eOn: return "On";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DeviceEventTypeEXT value)
  {
    switch (value)
    {
    case DeviceEventTypeEXT::eDisplayHotplug: return "DisplayHotplug";
    default: return "invalid";
    }
  }

  VULKAN_HPP_INLINE std::string to_string(DisplayEventTypeEXT value)
  {
    switch (value)
    {
    case DisplayEventTypeEXT::eFirstPixelOut: return "FirstPixelOut";
    default: return "invalid";
    }
  }

} // namespace vk

#endif
